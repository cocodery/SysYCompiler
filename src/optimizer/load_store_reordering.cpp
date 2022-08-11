#include "load_store_reordering.hh"

LoadStoreReordering::LoadStoreReordering(Function *function)
{
    for (auto &&bbPtr : function->all_blocks) {
        if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
            continue;
        //  插入位置，此处插入的指令
        int unique_void_call_id = INT32_MAX;
        void_call_map.clear();
        map<int, vector<Inst *>> store_insts_to_relocate;
        vector<Inst *> other_insts;
        deque<Inst *> rebuilt_bb;
        for (int i = 0, siz = bbPtr->basic_block.size(); i < siz; ++i) {
            auto &&instPtr = bbPtr->basic_block[i];
            Case (LLIR_STORE, store_inst, instPtr) {
                if (store_inst->src.reg) {
                    int src_id = store_inst->src.reg->reg_id;
                    int dst_id = store_inst->dst.reg->global ? -1 : store_inst->dst.reg->reg_id;
                    int last_load_call_id = -1;
                    bool found_src_id = false;
                    bool found_dst_id = false;
                    for (int j = i - 1; last_load_call_id == -1 && j >= 0; --j) {
                        // 找定值之后的load_inst或者call_inst，所以如果先找到了定值就break
                        if (ProcessInst(bbPtr->basic_block[j]) == src_id) {found_src_id = true; break;}
                        if (ProcessInst(bbPtr->basic_block[j]) == dst_id) {found_dst_id = true; break;}
                        Case (LLIR_LOAD, load_inst, bbPtr->basic_block[j]) {
                            assert(load_inst->src.reg);
                            if (!load_inst->src.reg->global && load_inst->src.reg->reg_id == dst_id)
                                last_load_call_id = load_inst->dst.reg->reg_id;
                        }
                        Case (LLIR_CALL, call_inst, bbPtr->basic_block[j]) {
                            for (auto &&arg : call_inst->args) {
                                if (arg.reg && !arg.reg->global && arg.reg->reg_id == dst_id) {
                                    if (call_inst->func_info->return_type != TypeVoid)
                                        last_load_call_id = call_inst->dst.reg->reg_id;
                                    else {
                                        last_load_call_id = unique_void_call_id;
                                        void_call_map.insert(make_pair(bbPtr->basic_block[j], unique_void_call_id--));
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    src_id = found_src_id ? src_id : -1;
                    dst_id = found_dst_id ? dst_id : -1;
                    int insert_after = std::max(src_id, std::max(dst_id, last_load_call_id));
                    //cout << "store(src): " << store_inst->src.reg->reg_id << " after: " << insert_after << endl;
                    if (insert_after != -1) {
                        if (store_insts_to_relocate.find(insert_after) == store_insts_to_relocate.end())
                            store_insts_to_relocate.insert(make_pair(insert_after, vector<Inst *>{instPtr}));
                        else store_insts_to_relocate.at(insert_after).push_back(instPtr);
                    }
                    else other_insts.push_back(instPtr);
                }
                else other_insts.push_back(instPtr);
                continue;
            }
            else other_insts.push_back(instPtr);
        }

        // 重建
        for (auto &&instPtr : other_insts)
        {
            rebuilt_bb.push_back(instPtr);
            int dst_regid = ProcessInst(instPtr);
            //cout << "dstid: "<< dst_regid << endl;
            if (dst_regid == -1) continue;
            auto &&it = store_insts_to_relocate.find(dst_regid);
            if (it != store_insts_to_relocate.end()) {
                for (auto &&elem : it->second)
                    rebuilt_bb.push_back(elem);
                store_insts_to_relocate.erase(it);
            }
        }
        for (auto &&it = store_insts_to_relocate.rbegin(); it != store_insts_to_relocate.rend(); ++it)
            for (auto &&elem : it->second)
                rebuilt_bb.push_front(elem);
        bbPtr->basic_block = vector<Inst *>(rebuilt_bb.begin(), rebuilt_bb.end());
        
    }
}
int LoadStoreReordering::ProcessInst(Inst* instPtr) // returns dst_regid
{
    int dst_regid = -1;
    Case (LLIR_ALLOCA, alloca_inst, instPtr)
        dst_regid = alloca_inst->reg.reg->reg_id;
    Case (LLIR_BIN, bin_inst, instPtr)
        dst_regid = bin_inst->dst.reg->reg_id;
    Case (LLIR_FBIN, fbin_inst, instPtr)
        dst_regid = fbin_inst->dst.reg->reg_id;
    Case (LLIR_LOAD, load_inst, instPtr)
        dst_regid = load_inst->dst.reg->reg_id;
    Case (LLIR_ICMP, icmp_inst, instPtr)
        dst_regid = icmp_inst->dst.reg->reg_id;
    Case (LLIR_FCMP, fcmp_inst, instPtr)
        dst_regid = fcmp_inst->dst.reg->reg_id;
    Case (LLIR_CALL, call_inst, instPtr) {
        if (call_inst->func_info->return_type != TypeVoid)
            dst_regid = call_inst->dst.reg->reg_id;
        else
            dst_regid = void_call_map.find(instPtr) == void_call_map.end() ? -1 : void_call_map.at(instPtr);
    }
    Case (LLIR_ZEXT, zext_inst, instPtr)
        dst_regid = zext_inst->dst.reg->reg_id;
    Case (LLIR_GEP, gep_inst, instPtr)
        dst_regid = gep_inst->dst.reg->reg_id;
    Case (LLIR_XOR, xor_inst, instPtr)
        dst_regid = xor_inst->dst.reg->reg_id;
    Case (LLIR_BC, bc_inst, instPtr)
        dst_regid = bc_inst->dst.reg->reg_id;
    Case (LLIR_SITOFP, itf_inst, instPtr)
        dst_regid = itf_inst->dst.reg->reg_id;
    Case (LLIR_FPTOSI, fti_inst, instPtr)
        dst_regid = fti_inst->dst.reg->reg_id;
    return dst_regid;
}

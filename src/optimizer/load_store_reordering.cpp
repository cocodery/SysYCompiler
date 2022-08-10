#include "load_store_reordering.hh"

LoadStoreReordering::LoadStoreReordering(Function *function)
{
    for (auto &&bbPtr : function->all_blocks) {
        if (bbPtr->bb_idx == 0 || bbPtr->bb_idx == -1)
            continue;
        //        src的regid, 指令本身（每个regid出现一次赋值，所以只会出现一次store）
        unordered_map<int, Inst *> store_insts_to_relocate;
        vector<Inst *> other_insts;
        for (auto &&instPtr : bbPtr->basic_block) {
            Case (LLIR_STORE, store_inst, instPtr) {
                if (store_inst->src.reg) {
                    store_insts_to_relocate.insert(make_pair(store_inst->src.reg->reg_id, instPtr));
                    continue;
                }
            }
            else other_insts.push_back(instPtr);
        }

        // 重建
        bbPtr->basic_block.clear();
        for (auto &&instPtr : other_insts)
        {
            bbPtr->basic_block.push_back(instPtr);
            int dst_regid = ProcessInst(instPtr);
            if (dst_regid == -1) continue;
            auto &&it = store_insts_to_relocate.find(dst_regid);
            if (it != store_insts_to_relocate.end()) {
                bbPtr->basic_block.push_back(it->second);
                store_insts_to_relocate.erase(it);
            }
        }
    }
}
int LoadStoreReordering::ProcessInst(Inst* instPtr) // returns dst_regid
{
    int dst_regid = -1;
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
    Case (LLIR_CALL, call_inst, instPtr)
        if (call_inst->func_info->return_type != TypeVoid)
            dst_regid = call_inst->dst.reg->reg_id;
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

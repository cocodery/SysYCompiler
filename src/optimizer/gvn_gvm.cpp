#include "gvn_gcm.hh"

void GvnGcm::moveCtv2Rhs() {
    auto &&all_block = function->all_blocks;
    for (auto &&block : all_block) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_BIN, bin_inst, inst) {
                if (bin_inst->op == SUB && bin_inst->src2.ctv) {
                    bin_inst->op = ADD;
                    SRC svalue = SRC(new CTValue(TypeInt, -bin_inst->src2.ctv->int_value, -bin_inst->src2.ctv->int_value));
                    bin_inst->src2 = svalue;
                }
                if (bin_inst->src2.ctv != nullptr) continue; // rhs has been CTValue
                if (bin_inst->src1.ctv == nullptr) continue; // lhs has been VirtReg
                if (bin_inst->op == ADD) {
                    std::swap(bin_inst->src1, bin_inst->src2);
                } else if (bin_inst->op == MUL) {
                    std::swap(bin_inst->src1, bin_inst->src2);
                }
            } else Case (LLIR_ICMP, icmp_inst, inst) {
                if (icmp_inst->op == EQU || icmp_inst->op == NEQ) {
                    std::swap(icmp_inst->src1, icmp_inst->src2);
                }
            } 
        }
    }
}

void GvnGcm::runGVN() {
    auto &&all_blocks = function->all_blocks;
    BasicBlock *entrybb = all_blocks[0];
    stack<BasicBlock *>  postOrderStack = stack<BasicBlock *>();
    list<BasicBlock *> reversePostOrder = list<BasicBlock *>();

    for (auto &&block : all_blocks) {
        block->dirty = false;
    }
    postOrderStack.push(entrybb);
    BasicBlock *currbb = nullptr;
    while (!postOrderStack.empty()) {
        currbb = postOrderStack.top();
        postOrderStack.pop();
        reversePostOrder.push_back(currbb);
        for (auto &&pair : currbb->succs) {
            auto &&succ = pair.second;
            if (!succ->dirty) {
                postOrderStack.push(succ);
                succ->dirty = true;
            }
        }
    }

    for (auto &&block : reversePostOrder) {
        runGvnOnBlock(block);
    }
}

void GvnGcm::runGvnOnBlock(BasicBlock *block) {
    int32_t predSize = block->preds.size();
    list<Inst *> bb_list(block->basic_block.begin(), block->basic_block.end());

    for (auto &&iter = bb_list.begin(); iter != bb_list.end(); ) {
        auto &&inst = *iter;
        Case (LLIR_BIN, bin_inst, inst) {
            SRC value = lookupOrAdd(block->bb_idx, bin_inst);
            auto &&bin_dst = bin_inst->dst;
            if (!(value == bin_dst)) {
                function->replaceSRCs(block, bin_dst.reg, value);
                iter = bb_list.erase(iter);
                continue;
            }
        }
        Case (LLIR_FBIN, fbin_inst, inst) {
            SRC value = lookupOrAdd(block->bb_idx, fbin_inst);
            auto &&fbin_dst = fbin_inst->dst;
            if (!(value == fbin_dst)) {
                function->replaceSRCs(block, fbin_dst.reg, value);
                iter = bb_list.erase(iter);
                continue;
            }
        }
        Case (LLIR_GEP, gep_inst, inst) {
            SRC value = lookupOrAdd(block->bb_idx, gep_inst);
            auto &&gep_dst = gep_inst->dst;
            if (!(value == gep_dst)) {
                function->replaceSRCs(block, gep_dst.reg, value);
                iter = bb_list.erase(iter);
                continue;
            }
        }
        Case (LLIR_PHI, phi_inst, inst) {
            SRC value = lookupOrAdd(block->bb_idx, phi_inst);
            auto &&phi_dst = phi_inst->dst;
            if (!(value == phi_dst)) {
                function->replaceSRCs(block, phi_dst.reg, value);
                iter = bb_list.erase(iter);
                continue;
            }
        }
        Case (LLIR_CALL, call_inst, inst) {
            auto &&func_info = call_inst->func_info;
            if (func_info->side_effect == false) { // 1st: no side-effects function
                SRC value = lookupOrAdd(block->bb_idx, call_inst);
                auto &&call_dst = call_inst->dst;
                // 2nd: void-function can be deleted directly, in fact it's a bug, but it's real feature
                // 3rd: redundant int/float function can be cut
                if (func_info->return_type == TypeVoid || !(value == call_dst)) {
                    if (func_info->return_type != TypeVoid) {
                        function->replaceSRCs(block, call_dst.reg, value);
                    }
                    iter = bb_list.erase(iter);
                    continue;
                }
            }
        }
        ++iter;
    }
    
    block->basic_block = vector<Inst *>(bb_list.begin(), bb_list.end());
}

SRC GvnGcm::lookupOrAdd(int32_t idx, LLIR_BIN *bin_inst) {
    auto &&localValueTable = globalValueTable[idxMap[idx]];
    auto &&bin_inst_op = bin_inst->op;
    auto &&bin_inst_srcs = std::make_pair(bin_inst->src1, bin_inst->src2);
    auto &&lookup_result = localValueTable.bin2src.find({bin_inst_op, bin_inst_srcs});
    if (lookup_result != localValueTable.bin2src.end())
        return lookup_result->second;
    localValueTable.bin2src.insert({{bin_inst_op, bin_inst_srcs}, bin_inst->dst});
    return bin_inst->dst;
}

SRC GvnGcm::lookupOrAdd(int32_t idx, LLIR_FBIN *fbin_inst) {
    auto &&localValueTable = globalValueTable[idxMap[idx]];
    auto &&fbin_inst_op = fbin_inst->op;
    auto &&fbin_inst_srcs = std::make_pair(fbin_inst->src1, fbin_inst->src2);
    auto &&lookup_result = localValueTable.bin2src.find({fbin_inst_op, fbin_inst_srcs});
    if (lookup_result != localValueTable.bin2src.end())
        return lookup_result->second;
    localValueTable.bin2src.insert({{fbin_inst_op, fbin_inst_srcs}, fbin_inst->dst});
    return fbin_inst->dst;
}

SRC GvnGcm::lookupOrAdd(int32_t idx, LLIR_GEP *gep_inst) {
    auto &&localValueTable = globalValueTable[idxMap[idx]];
    auto &&gep_inst_op = GEP;
    auto &&gep_inst_srcs = std::make_pair(gep_inst->src, gep_inst->off);
    for (auto &&pair : localValueTable.bin2src) {
        auto &&gep_op = pair.first.first;
        auto &&gep_srcs = pair.first.second;
        auto &&gep_dst = pair.second;
        if (gep_inst_op == gep_op) {
            if (gep_inst_srcs.first == gep_srcs.first && gep_inst_srcs.second == gep_srcs.second) {
                return gep_dst;
            }
        }
    }
    gep_inst->dst.reg->type.is_array = true;
    localValueTable.bin2src.insert({{gep_inst_op, gep_inst_srcs}, gep_inst->dst});
    return gep_inst->dst;
}

SRC GvnGcm::lookupOrAdd(int32_t idx, LLIR_PHI *phi_inst) {
    auto &&localValueTable = globalValueTable[idxMap[idx]];
    auto &&phi_inst_srcs = phi_inst->srcs;
    auto &&lookup_result = localValueTable.phi2src.find(phi_inst_srcs);
    if (lookup_result != localValueTable.phi2src.end())
        return lookup_result->second;
    localValueTable.phi2src.insert({phi_inst_srcs, phi_inst->dst});
    return phi_inst->dst;
}

SRC GvnGcm::lookupOrAdd(int32_t idx, LLIR_CALL *call_inst) {
    auto &&localValueTable = globalValueTable[idxMap[idx]];
    auto &&func_name = call_inst->func_info->func_name;
    auto &&func_args = call_inst->args;
    for (auto &&pair : localValueTable.fun2src) {
        auto &&func_rel = pair.first;
        auto &&call_dst = pair.second;
        if (func_rel.first == func_name) {
            bool find = true;
            for (int32_t idxx = 0; idxx < func_args.size(); ++idxx) {
                if (!(func_args[idxx] == func_rel.second[idxx])) {
                    find = false;
                    break;
                }
            }
            if (find) {
                return call_dst;
            }
        }
    }
    localValueTable.fun2src.insert({funcRel(func_name, func_args), call_inst->dst});
    return call_inst->dst;
}

void GvnGcm::runGCM() {

}

void GvnGcm::runGvnGcm() {
    moveCtv2Rhs();
    
    // TODO: run DCE here

    runGVN();
}
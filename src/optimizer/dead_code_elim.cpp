#include "dead_code_elim.hh"

void Dce::insertToSet(VirtReg *reg) {
    if (reg != nullptr) {
        usefulRegSet.insert({reg->reg_id, reg->global});
    }
}

bool Dce::checkInSet(VirtReg *reg) {
    return (usefulRegSet.find({reg->reg_id, reg->global}) != usefulRegSet.end());
}

void Dce::dfsReverseOrder(list<BasicBlock *> list, BasicBlock *block) {
    if (block->dirty) return;
    block->dirty = true;
    list.push_back(block);
    for (auto &&succ : block->succs) {
        auto &&succbb = succ.second;
        if (succbb->bb_idx < block->bb_idx || !succbb->dirty) {
            if (succbb->bb_idx < block->bb_idx) {
                succbb->dirty = false;
            }
            dfsReverseOrder(list, succbb);
        }
    }
    block->dirty = false;
    if (block->bb_idx == -1) {
        reverseOrder.push_back(list);
        return;
    }
}

void Dce::buildUsefulRegSet(list<BasicBlock *> path) {
    for (auto &&iter = path.rbegin(); iter != path.rend(); ++iter) {
        auto &&block = *iter;
        for (auto &&it = block->basic_block.rbegin(); it != block->basic_block.rend(); ++it) {
            auto &&inst = *it;
            Case (LLIR_RET, ret_inst, inst) {
                insertToSet(ret_inst->ret_value.reg);
            } else Case (LLIR_BR, br_inst, inst) {
                insertToSet(br_inst->cond.reg);
            } else Case (LLIR_BIN, bin_inst, inst) {
                if (checkInSet(bin_inst->dst.reg)) {
                    insertToSet(bin_inst->src1.reg);
                    insertToSet(bin_inst->src2.reg);
                }
            } else Case (LLIR_FBIN, fbin_inst, inst) {
                if (checkInSet(fbin_inst->dst.reg)) {
                    insertToSet(fbin_inst->src1.reg);
                    insertToSet(fbin_inst->src2.reg);
                }
            } else Case (LLIR_LOAD, load_inst, inst) {
                if (checkInSet(load_inst->dst.reg)) {
                    insertToSet(load_inst->src.reg);
                }
            } else Case (LLIR_STORE, store_inst, inst) {
                insertToSet(store_inst->src.reg);
                insertToSet(store_inst->dst.reg);
            } else Case (LLIR_ICMP, icmp_inst, inst) {
                if (checkInSet(icmp_inst->dst.reg)) {
                    insertToSet(icmp_inst->src1.reg);
                    insertToSet(icmp_inst->src2.reg);
                }
            } else Case (LLIR_FCMP, fcmp_inst, inst) {
                if (checkInSet(fcmp_inst->dst.reg)) {
                    insertToSet(fcmp_inst->src1.reg);
                    insertToSet(fcmp_inst->src2.reg);
                }
            } else Case (LLIR_CALL, call_inst, inst) {
                auto &&func_info = call_inst->func_info;
                if (checkInSet(call_inst->dst.reg) || func_info->side_effect) {
                    insertToSet(call_inst->dst.reg);
                    for (auto &&arg : call_inst->args) {
                        insertToSet(arg.reg);
                    }
                }
            } else Case (LLIR_ZEXT, zext_inst, inst) {
                if (checkInSet(zext_inst->dst.reg)) {
                    insertToSet(zext_inst->src.reg);
                }
            } else Case (LLIR_GEP, gep_inst, inst) {
                if (checkInSet(gep_inst->dst.reg)) {
                    insertToSet(gep_inst->src.reg);
                    insertToSet(gep_inst->off.reg);
                }
            } else Case (LLIR_XOR, xor_inst, inst) {
                if (checkInSet(xor_inst->dst.reg)) {
                    insertToSet(xor_inst->src.reg);
                }
            } else Case (LLIR_BC, bc_inst, inst) {
                if (checkInSet(bc_inst->dst.reg)) {
                    insertToSet(bc_inst->src.reg);
                }
            } else Case (LLIR_SITOFP, itf_inst, inst) {
                if (checkInSet(itf_inst->dst.reg)) {
                    insertToSet(itf_inst->src.reg);
                }
            } else Case (LLIR_FPTOSI, fti_inst, inst) {
                if (checkInSet(fti_inst->dst.reg)) {
                    insertToSet(fti_inst->src.reg);
                }
            } else Case (LLIR_PHI, phi_inst, inst) {
                if (checkInSet(phi_inst->dst.reg)) {
                    for (auto &&src : phi_inst->srcs) {
                        insertToSet(src.first.reg);
                    }
                }
            }
        }
    }
}

void Dce::removeUselessInst() {
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        list<Inst *> bb_list(block->basic_block.begin(), block->basic_block.end());
        for (auto &&iter = bb_list.begin(); iter != bb_list.end(); ) {
            auto &&inst = *iter;
            Case (LLIR_BIN, bin_inst, inst) {
                if (!checkInSet(bin_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_FBIN, fbin_inst, inst) {
                if (!checkInSet(fbin_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_ALLOCA, alloca_inst, inst) {
                if (!checkInSet(alloca_inst->reg.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_LOAD, load_inst, inst) {
                if (!checkInSet(load_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_ICMP, icmp_inst, inst) {
                if (!checkInSet(icmp_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_FCMP, fcmp_inst, inst) {
                if (!checkInSet(fcmp_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_CALL, call_inst, inst) {
                auto &&func_info = call_inst->func_info;
                if (!checkInSet(call_inst->dst.reg) && !func_info->side_effect) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_ZEXT, zext_inst, inst) {
                if (!checkInSet(zext_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_GEP, gep_inst, inst) {
                if (!checkInSet(gep_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_XOR, xor_inst, inst) {
                if (!checkInSet(xor_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_BC, bc_inst, inst) {
                if (!checkInSet(bc_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_SITOFP, itf_inst, inst) {
                if (!checkInSet(itf_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_FPTOSI, fti_inst, inst) {
                if (!checkInSet(fti_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else Case (LLIR_PHI, phi_inst, inst) {
                if (!checkInSet(phi_inst->dst.reg)) {
                    iter = bb_list.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
        block->basic_block = vector<Inst *>(bb_list.begin(), bb_list.end());
    }
}

void Dce::runDeadCodeElim() {
    for (auto &&block : function->all_blocks) {
        block->dirty = false;
    }
    dfsReverseOrder(list<BasicBlock *>(), function->all_blocks[0]);

    for (auto &&path : reverseOrder) {
        buildUsefulRegSet(path);
    }
    
    // removeUselessInst();
}

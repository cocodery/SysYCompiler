#include "dead_code_elim.hh"

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

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

class test_judger {
public:
    std::unordered_map<uint32_t, uint32_t> equal_relationship;

    std::unordered_map<uint32_t, Inst *> define_place;

    std::unordered_set<uint32_t> applied;
    
    void push_back(uint32_t lhs, uint32_t rhs, Inst *_define) {
        // this means lhs is rhs
        // and lhs == rhs
        // but rhs is not lhs
        // just for this period of time
        equal_relationship.emplace(lhs, rhs);
        define_place.emplace(lhs, _define);
    }

    bool get_equ_info(uint32_t lhs, uint32_t rhs) {
        auto &&x = equal_relationship.find(lhs);
        return x != equal_relationship.end() && x->second == rhs;
    }

    bool get_apply_info(uint32_t lhs) {
        return applied.find(lhs) != applied.end();
    }

    auto get_define_palce(uint32_t lhs) {
        return define_place.find(lhs)->second;
    }

    void is_redefined(uint32_t lhs) {
        if (equal_relationship.find(lhs) != equal_relationship.end()) {
            equal_relationship.erase(lhs);
            define_place.erase(lhs);
        }
        if (applied.find(lhs) != applied.end())
            applied.erase(lhs);
    }

    void is_applied(uint32_t lhs) {
        if (equal_relationship.find(lhs) != equal_relationship.end())
            applied.insert(lhs);
    }

    bool has_defined(uint32_t lhs) {
        return equal_relationship.find(lhs) != equal_relationship.end();
    }
};

void Dce::buildUsefulRegSet(Function *f) {
    auto &&path = f->all_blocks;

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
                if (call_inst->dst.reg && checkInSet(call_inst->dst.reg) || func_info->side_effect) {
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

// #define PASS

//         test_judger tj;

//         for (auto &&it = block->basic_block.begin(); it != block->basic_block.end(); ++it) {
//             auto &&inst = *it;
//             Case (LLIR_RET, ret_inst, inst) {
//                 if (ret_inst->ret_value.reg) {
//                     tj.is_applied(ret_inst->ret_value.reg->reg_id);
//                 }
//             } else Case (LLIR_BR, br_inst, inst) {
//                 if (br_inst->cond.reg) {
//                     tj.is_applied(br_inst->cond.reg->reg_id);
//                 }
//             } else Case (LLIR_BIN, bin_inst, inst) {
//                 if (bin_inst->src1.reg) {
//                     tj.is_applied(bin_inst->src1.reg->reg_id);
//                 }
//                 if (bin_inst->src2.reg) {
//                     tj.is_applied(bin_inst->src2.reg->reg_id);
//                 }
//                 tj.is_redefined(bin_inst->dst.reg->reg_id);
//             } else Case (LLIR_FBIN, fbin_inst, inst) {
//                 if (fbin_inst->src1.reg) {
//                     tj.is_applied(fbin_inst->src1.reg->reg_id);
//                 }
//                 if (fbin_inst->src2.reg) {
//                     tj.is_applied(fbin_inst->src2.reg->reg_id);
//                 }
//                 tj.is_redefined(fbin_inst->dst.reg->reg_id);
//             } else Case (LLIR_LOAD, load_inst, inst) {
//                 if (load_inst->src.reg)
//                     tj.push_back(load_inst->dst.reg->reg_id, load_inst->src.reg->reg_id, inst);
//             } else Case (LLIR_STORE, store_inst, inst) {
//                 if (store_inst->src.reg && tj.get_equ_info(store_inst->src.reg->reg_id, store_inst->dst.reg->reg_id)) {
//                     PASS
//                 } else {
//                     if (store_inst->src.reg) {
//                         tj.is_applied(store_inst->src.reg->reg_id);
//                     }
//                     tj.is_redefined(store_inst->dst.reg->reg_id);
//                 }
//             } else Case (LLIR_ICMP, icmp_inst, inst) {
//                 if (icmp_inst->src1.reg) {
//                     tj.is_applied(icmp_inst->src1.reg->reg_id);
//                 }
//                 if (icmp_inst->src2.reg) {
//                     tj.is_applied(icmp_inst->src2.reg->reg_id);
//                 }
//                 tj.is_redefined(icmp_inst->dst.reg->reg_id);
//             } else Case (LLIR_FCMP, fcmp_inst, inst) {
//                 if (fcmp_inst->src1.reg) {
//                     tj.is_applied(fcmp_inst->src1.reg->reg_id);
//                 }
//                 if (fcmp_inst->src2.reg) {
//                     tj.is_applied(fcmp_inst->src2.reg->reg_id);
//                 }
//                 tj.is_redefined(fcmp_inst->dst.reg->reg_id);
//             } else Case (LLIR_CALL, call_inst, inst) {
//                 if (call_inst->dst.reg) tj.is_redefined(call_inst->dst.reg->reg_id);
//                 for (auto &&arg : call_inst->args) {
//                     if (arg.reg) {
//                         tj.is_applied(arg.reg->reg_id);
//                     }
//                 }
//             } else Case (LLIR_ZEXT, zext_inst, inst) {
//                 if (zext_inst->src.reg) {
//                     tj.is_applied(zext_inst->src.reg->reg_id);
//                 }
//                 tj.is_redefined(zext_inst->dst.reg->reg_id);
//             } else Case (LLIR_GEP, gep_inst, inst) {
//                 if (gep_inst->off.reg) {
//                     tj.is_applied(gep_inst->off.reg->reg_id);
//                 }
//                 if (gep_inst->src.reg) {
//                     tj.is_applied(gep_inst->src.reg->reg_id);
//                 }
//                 tj.is_redefined(gep_inst->dst.reg->reg_id);
//             } else Case (LLIR_XOR, xor_inst, inst) {
//                 if (xor_inst->src.reg) {
//                     tj.is_applied(xor_inst->src.reg->reg_id);
//                 }
//                 tj.is_redefined(xor_inst->dst.reg->reg_id);
//             } else Case (LLIR_BC, bc_inst, inst) {
//                 if (bc_inst->src.reg) {
//                     tj.is_applied(bc_inst->src.reg->reg_id);
//                 }
//                 tj.is_redefined(bc_inst->dst.reg->reg_id);
//             } else Case (LLIR_SITOFP, itf_inst, inst) {
//                 if (itf_inst->src.reg) {
//                     tj.is_applied(itf_inst->src.reg->reg_id);
//                 }
//                 tj.is_redefined(itf_inst->dst.reg->reg_id);
//             } else Case (LLIR_FPTOSI, fti_inst, inst) {
//                 if (fti_inst->src.reg) {
//                     tj.is_applied(fti_inst->src.reg->reg_id);
//                 }
//                 tj.is_redefined(fti_inst->dst.reg->reg_id);
//             } else Case (LLIR_PHI, phi_inst, inst) {
//                 for (auto &&src : phi_inst->srcs) {
//                     if (src.first.reg) {
//                         tj.is_applied(src.first.reg->reg_id);
//                     }
//                 }
//                 tj.is_redefined(phi_inst->dst.reg->reg_id);
//             }
//         }

//         for (auto &&it = block->basic_block.begin(); it != block->basic_block.end(); ++it) {
//             auto &&inst = *it;
//             Case (LLIR_STORE, store_inst, inst) {
//                 if (store_inst->src.reg && tj.get_equ_info(store_inst->src.reg->reg_id, store_inst->dst.reg->reg_id)) {
//                     if (!tj.get_apply_info(store_inst->src.reg->reg_id)) {
//                         uselessInstSet.insert(tj.get_define_palce(store_inst->src.reg->reg_id));
//                     }
//                     uselessInstSet.insert(inst);
//                 }
//             }
//         }
    }
}

void Dce::removeUselessInst() {
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        list<Inst *> bb_list(block->basic_block.begin(), block->basic_block.end());
        for (auto &&iter = bb_list.begin(); iter != bb_list.end(); ) {
            auto &&inst = *iter;
            if (uselessInstSet.find(inst) != uselessInstSet.end()) {
                iter = bb_list.erase(iter);
                continue;
            }
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
                if (call_inst->dst.reg && !checkInSet(call_inst->dst.reg) && !func_info->side_effect) {
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
    buildUsefulRegSet(function);
    removeUselessInst();
}

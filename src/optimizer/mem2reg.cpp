#include "mem2reg.hh"

set<pair<int32_t, bool>> Mem2Reg::initDelVarSet() {
    auto && all_blocks = function->all_blocks;
    set<pair<int32_t, bool>> del_variable;
    // init set `del_variable`
    // if a variable is defined, but never be used, delete insts which use it
    // and if it is only be re-assigned, also need to be deleted
    for (auto &&block : all_blocks) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_ALLOCA, alloca_inst, inst) {
                VirtReg *alloca_dst = alloca_inst->reg.ToVirtReg();
                assert(alloca_dst != nullptr);
                del_variable.insert({alloca_dst->reg_id, alloca_dst->global});
            }
            Case (LLIR_LOAD, load_inst, inst) {
                VirtReg *load_src = load_inst->src.ToVirtReg();
                assert(load_src != nullptr);
                del_variable.erase({load_src->reg_id, load_src->global});
            }
            Case (LLIR_GEP, gep_inst, inst) {
                VirtReg *gep_src = gep_inst->src.ToVirtReg();
                assert(gep_src != nullptr);
                del_variable.erase({gep_src->reg_id, gep_src->global});
            }
        }
    }
    // dbg("Variable in del_variable");
    // for (auto &&reg : del_variable) {
    //     dbg(reg.first, reg.second);
    // }
    return del_variable;
}

void Mem2Reg::removeUsedVar(set<pair<int32_t, bool>> &del_variable) {
    auto && all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        for (auto &&iter = block->basic_block.begin(); iter != block->basic_block.end(); ++iter) {
            auto &&inst = *iter;
            Case (LLIR_ALLOCA, alloca_inst, inst) {
                VirtReg *alloca_dst = alloca_inst->reg.ToVirtReg();
                assert(alloca_dst != nullptr);
                if (del_variable.find({alloca_dst->reg_id, alloca_dst->global}) != del_variable.end()) {
                    iter = block->basic_block.erase(iter) - 1;
                }
            } else Case (LLIR_STORE, store_inst, inst) {
                VirtReg *store_dst = store_inst->dst.ToVirtReg();
                assert(store_dst != nullptr);
                if (del_variable.find({store_dst->reg_id, store_dst->global}) != del_variable.end()) {
                    iter = block->basic_block.erase(iter) - 1;
                }
            } else {
                continue;
            }
        }
    }
}

LLIR_ALLOCA *Mem2Reg::getAllocaInst(VirtReg *reg) {
    for (auto &&alloca_inst : allocaInsts) {
        auto &&alloca_dst = alloca_inst->reg.ToVirtReg();
        assert(alloca_dst != nullptr);
        if (reg->reg_id == alloca_dst->reg_id && reg->global == alloca_dst->global) {
            return alloca_inst;
        }
    }
    return nullptr;
}

bool Mem2Reg::inDefBlocks(int32_t index ,BasicBlock *block) {
    for (auto &&defbb : defBlocks[index]) {
        if (block->bb_idx == defbb->bb_idx) {
            return true;
        }
    }
    return false;
}

void Mem2Reg::runMem2Reg() {
    dbg(function->func_info.func_name);
    // delete unused variable
    auto &&del_variable = initDelVarSet();
    removeUsedVar(del_variable);
    // run mem2reg
    auto &&all_blocks = function->all_blocks;
    // init `defBlocks` `allocaLoopup` `allocaInsts`
    for (auto &&block : all_blocks) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_ALLOCA, alloca_inst, inst) {
                allocaInsts.push_back(alloca_inst);
                allocaLoopup[alloca_inst] = allocaInsts.size() - 1;
                defBlocks.push_back(set<BasicBlock *>());
            } else {
                continue;
            }
        }
    }
    for (auto &&block : all_blocks) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_STORE, store_inst, inst) {
                auto &&store_dst = store_inst->dst.ToVirtReg();
                assert(store_dst != nullptr);
                auto &&alloca_inst = getAllocaInst(store_dst);
                if (alloca_inst == nullptr) { // avoid store dst is glb-variable
                    continue;
                }
                if (allocaLoopup.find(alloca_inst) != allocaLoopup.end()) {
                    defBlocks[allocaLoopup[alloca_inst]].insert(block);
                }
            }
        }
    }
    dbg("AllocaLookUp");
    for (auto &&pair : allocaLoopup) {
        dbg(pair.second, pair.first->ToString());
    }
    // insert phi inst
    auto &&var_idx = function->var_idx;
    queue<BasicBlock *> W;
    map<LLIR_PHI *, int32_t> phi2AllocaMap;
    for (auto &&alloca_inst : allocaInsts) {
        int32_t index = allocaLoopup[alloca_inst];
        DeclType type = alloca_inst->reg.getType();
        for (auto &&block : defBlocks[index]) {
            W.push(block);
        }
        while (!W.empty()) {
            BasicBlock *block = W.front();
            W.pop();
            for (auto &&df : block->DomFrontier) {
                if (!df->dirty) { // avoid repeat phi-inst be generated
                    df->dirty = true;
                    LLIR_PHI *phi_inst = new LLIR_PHI(SRC(new VirtReg(var_idx++, type)));
                    df->basic_block.insert(df->basic_block.begin(), phi_inst); // insert phi to bb-front
                    phi2AllocaMap[phi_inst] = index; // phi-srcs from defBlocks[index]
                    if (!inDefBlocks(index, df)) {
                        W.push(df);
                    }
                }
            }
        }
    }
    // rename
    vector<SRC> values;
    for (auto &&alloca_inst : allocaInsts) {
        values.push_back(SRC(new CTValue(alloca_inst->reg.getType(), 0, 0)));
    }
    for (auto &&block : all_blocks) {
        block->dirty = false;
    }
    stack<RenameData *> renameDataStack;
    renameDataStack.push(new RenameData(*all_blocks.begin(), nullptr, values));
    while(!renameDataStack.empty()) {
        RenameData *data = renameDataStack.top();
        renameDataStack.pop();
        vector<SRC> cur_values = values;
        auto &&data_bb = data->block->basic_block;

        for (auto &&inst : data_bb) {
            Case (LLIR_PHI, phi_inst, inst) {
                if (phi2AllocaMap.find(phi_inst) != phi2AllocaMap.end()) {
                    int32_t predIndex = data->pred->bb_idx;
                    phi_inst->insertValue(data->values[phi2AllocaMap[phi_inst]], predIndex);
                }
            }
        }

        if (data->block->dirty) {
            continue;
        }
        data->block->dirty = true;
        for (auto &&inst_iter = data_bb.begin(); inst_iter != data_bb.end(); ++inst_iter) {
            auto inst = *inst_iter;
            bool removeInst = false;
            Case (LLIR_ALLOCA, alloca_inst, inst) {
                if (allocaLoopup.find(alloca_inst) != allocaLoopup.end()) {
                    inst_iter = data_bb.erase(inst_iter) - 1;
                    removeInst = true;
                }
            } else Case (LLIR_LOAD, load_inst, inst) {
                auto &&load_src = load_inst->src.ToVirtReg();
                assert(load_src != nullptr);
                auto &&alloca_inst = getAllocaInst(load_src);
                int allocaIndex = allocaLoopup[alloca_inst];
                load_inst->dst = cur_values[allocaIndex];
                inst_iter = data_bb.erase(inst_iter) - 1;
                removeInst = true;
            } else Case (LLIR_STORE, store_inst, inst) {
                auto &&store_dst = store_inst->dst.ToVirtReg();
                assert(store_dst != nullptr);
                auto &&alloca_inst = getAllocaInst(store_dst);
                int allocaIndex = allocaLoopup[alloca_inst];
                cur_values[allocaIndex] = store_inst->src;
                inst_iter = data_bb.erase(inst_iter) - 1;
                removeInst = true;
            } else Case (LLIR_PHI, phi_inst, inst) {
                int allocaIndex = phi2AllocaMap[phi_inst];
                cur_values[allocaIndex] = phi_inst->dst;
            } 
        }

        for (auto &&pair : data->block->succs) {
            renameDataStack.push(new RenameData(pair.second, data->block, cur_values));
        }
    }
}

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
                defBlocks.push_back(vector<BasicBlock *>());
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
                    defBlocks[allocaLoopup[alloca_inst]].push_back(block);
                }
            }
        }
    }
    // insert phi inst
    queue<BasicBlock *> W;
    map<LLIR_PHI *, int32_t> phi2AllocaMap;
    for (auto &&alloca_inst : allocaInsts) {
        int32_t index = allocaLoopup[alloca_inst];
        for (auto &&block : all_blocks) {
            block->dirty = false;
        }
        for (auto &&block : defBlocks[index]) {
            W.push(block);
        }
        while (!W.empty()) {
            BasicBlock *block = W.front();
            W.pop();
            for (auto &&df : block->DomFrontier) {
                if (!df->dirty) { // avoid repeat phi-inst be generated
                    df->dirty = true;
                    LLIR_PHI *phi_inst = new LLIR_PHI();
                    phi2AllocaMap[phi_inst] = index; // phi-srcs from defBlocks[index]
                    if (!inDefBlocks(index, df)) {
                        W.push(df);
                    }
                }
            }
        }
    }
}

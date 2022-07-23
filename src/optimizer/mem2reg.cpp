#include "mem2reg.hh"

void Mem2Reg::removeUsedVar() {
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
                dbg(alloca_dst);
                dbg(alloca_dst->reg_id, alloca_dst->global);
                cout << "push reg : " << alloca_dst->ToString() << endl;
            }
            Case (LLIR_LOAD, load_inst, inst) {
                VirtReg *load_src = load_inst->src.ToVirtReg();
                assert(load_src != nullptr);
                del_variable.erase({load_src->reg_id, load_src->global});
                dbg(load_src);
                dbg(load_src->reg_id, load_src->global);
                cout << "pop reg : " << load_src->ToString() << endl;
            }
            Case (LLIR_GEP, gep_inst, inst) {
                VirtReg *gep_src = gep_inst->src.ToVirtReg();
                assert(gep_src != nullptr);
                del_variable.erase({gep_src->reg_id, gep_src->global});
                dbg(gep_src);
                dbg(gep_src->reg_id, gep_src->global);
                cout << "pop reg : " << gep_src->ToString() << endl;
            }
        }
    }
    dbg("Variable in del_variable");
    for (auto &&reg : del_variable) {
        dbg(reg.first, reg.second);
    }

    for (auto &&block : all_blocks) {
        for (auto &&iter = block->basic_block.begin(); iter != block->basic_block.end(); ++iter) {
            auto &&inst = *iter;
            Case (LLIR_ALLOCA, alloca_inst, inst) {
                VirtReg *alloca_dst = alloca_inst->reg.ToVirtReg();
                assert(alloca_dst != nullptr);
                if (del_variable.find({alloca_dst->reg_id, alloca_dst->global}) != del_variable.end()) {
                    dbg(alloca_inst->ToString());
                    iter = block->basic_block.erase(iter) - 1;
                    cout << "pop inst : " << alloca_inst->ToString() << endl;
                }
            }
            else Case (LLIR_STORE, store_inst, inst) {
                VirtReg *store_dst = store_inst->dst.ToVirtReg();
                assert(store_dst != nullptr);
                if (del_variable.find({store_dst->reg_id, store_dst->global}) != del_variable.end()) {
                    dbg(store_inst->ToString());
                    iter = block->basic_block.erase(iter) - 1;
                    cout << "pop inst : " << store_inst->ToString() << endl;
                }
            } else {
                continue;
            }
        }
    }
    cout << endl;
}

void Mem2Reg::initMem2Reg() {
    removeUsedVar();
}
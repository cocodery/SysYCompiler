#include "reg2mem.hh"

void Reg2Mem::runReg2Mem() {
    set<LLIR_ALLOCA *> insertedAllocaSet;
    auto &&firstbb = function->all_blocks[1];

    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_PHI, phi_inst, inst) {
                int32_t mapIdx = phi2AllocaMap[phi_inst];
                auto &&alloca_inst = allocaInsts[mapIdx];
                int32_t before_insert = insertedAllocaSet.size();
                insertedAllocaSet.insert(alloca_inst);
                if (before_insert < insertedAllocaSet.size()) {
                    // restore alloca-inst;
                    firstbb->basic_block.insert(firstbb->basic_block.begin(), alloca_inst);
                }
                // get alloca-dst for later use
                auto &&alloca_dst = alloca_inst->reg;

                // restore store-inst in phi-inst
                for (auto &&src : phi_inst->srcs) {
                    auto &&svalue = src.first;
                    auto &&insertbb = function->getSpecificIdxBb(src.second);
                    LLIR_STORE *store_inst = new LLIR_STORE(alloca_dst, svalue);
                    if (auto &&br_inst = dynamic_cast<LLIR_BR *>(insertbb->lastInst()); br_inst != nullptr) {
                        insertbb->basic_block.insert(insertbb->basic_block.end() - 1, store_inst);
                    } else if (auto &&br_inst = dynamic_cast<LLIR_RET *>(insertbb->lastInst()); br_inst != nullptr) {
                        insertbb->basic_block.insert(insertbb->basic_block.end() - 1, store_inst);
                    } else {
                        insertbb->basic_block.push_back(store_inst);
                    }
                }

                auto &&phi_locate_bb = function->getSpecificIdxBb(phi_inst->locatebb_idx);
                auto &&phi_dst = phi_inst->dst;
                LLIR_LOAD *load_inst = new LLIR_LOAD(phi_dst, alloca_dst);
                phi_locate_bb->replaceInst(phi_inst, load_inst);
            }
        }
    }
}

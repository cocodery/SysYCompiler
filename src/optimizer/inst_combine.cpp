#include "inst_combine.hh"

void InstCombine::moveCtv2Rhs() {
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
            }
        }
    }
}

void InstCombine::runInstCombine() {
    moveCtv2Rhs();
    auto &&all_block = function->all_blocks;
    for (auto &&block : all_block) {
        list<Inst *> worklist(block->basic_block.begin(), block->basic_block.end());
        block->basic_block.clear();
        while(!worklist.empty()) {
            auto &&inst1 = worklist.front();
            if (auto &&bin_inst1 = dynamic_cast<LLIR_BIN *>(inst1)) {
                auto &&inst1src2ctv = bin_inst1->src2.ctv;
                if (inst1src2ctv && (bin_inst1->op == ADD || bin_inst1->op == SUB)) {
                    worklist.pop_front();
                    auto &&inst2 = worklist.front();
                    if (auto &&bin_inst2 = dynamic_cast<LLIR_BIN *>(inst2)) {
                        auto &&inst2src2ctv = bin_inst2->src2.ctv;
                        if (inst2src2ctv && (bin_inst1->dst == bin_inst2->src1) && (bin_inst2->op == ADD || bin_inst2->op == SUB)) {
                            bin_inst2->src1 = bin_inst1->src1;
                            int32_t svalue = (bin_inst2->op == ADD) ? 
                                inst1src2ctv->int_value + inst2src2ctv->int_value : inst1src2ctv->int_value - inst2src2ctv->int_value;
                            bin_inst2->src2 = SRC(new CTValue(TypeInt, svalue, svalue));
                        } else {
                            block->basic_block.push_back(inst1);
                        }
                    } else {
                        block->basic_block.push_back(inst1);
                    }

                } else if (bin_inst1->op == MUL || bin_inst1->op == DIV) {
                    if (bin_inst1->src1.ctv && bin_inst1->src1.ctv->int_value == -1 && bin_inst1->op == MUL) {
                        bin_inst1->op = SUB;
                        bin_inst1->src1.ctv->int_value = 0;
                        bin_inst1->src1.ctv->float_value = 0;
                    } else if (bin_inst1->src2.ctv && bin_inst1->src2.ctv->int_value == -1) {
                        bin_inst1->op = SUB;
                        std::swap(bin_inst1->src1, bin_inst1->src2);
                        bin_inst1->src1.ctv->int_value = 0;
                        bin_inst1->src1.ctv->float_value = 0;
                    }
                } else {
                    worklist.pop_front();
                    block->basic_block.push_back(inst1);
                }
            } else {
                worklist.pop_front();
                block->basic_block.push_back(inst1);
            }
        }
    }
}

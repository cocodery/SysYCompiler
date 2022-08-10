#include "constant_propagation.hh"

void ConstantProg::runConstantProg() {
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        processInBlock(block);
    }
}

void ConstantProg::processInBlock(BasicBlock *block) {
    for (auto &&iter = block->basic_block.begin(); iter != block->basic_block.end(); ++iter) {
        auto &&inst = *iter;
        Case (LLIR_BIN, bin_inst, inst) {
            SRC src1 = bin_inst->src1;
            SRC src2 = bin_inst->src2;
            if (src1.ToCTValue() && src2.ToCTValue()) {
                int32_t svalue1 = src1.ctv->int_value;
                int32_t svalue2 = src2.ctv->int_value;
                SRC dst_value;
                if (bin_inst->op == ADD) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 + svalue2, svalue1 + svalue2));
                } else if (bin_inst->op == SUB) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 - svalue2, svalue1 - svalue2));
                } else if (bin_inst->op == MUL) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 * svalue2, svalue1 * svalue2));
                } else if (bin_inst->op == DIV) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 / svalue2, svalue1 / svalue2));
                } else {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 % svalue2, svalue1 % svalue2));
                }
                function->replaceSRCs(block, bin_inst->dst.reg, dst_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } 
        Case (LLIR_FBIN, fbin_inst, inst) {
            SRC src1 = fbin_inst->src1;
            SRC src2 = fbin_inst->src2;
            SRC dst_value;
            if (src1.ToCTValue() && src2.ToCTValue()) {
                double fvalue1 = src1.ctv->float_value;
                double fvalue2 = src2.ctv->float_value;
                SRC dst_value;
                if (fbin_inst->op == ADD) {
                    dst_value = SRC(new CTValue(TypeFloat, fvalue1 + fvalue2, fvalue1 + fvalue2));
                } else if (fbin_inst->op == SUB) {
                    dst_value = SRC(new CTValue(TypeFloat, fvalue1 - fvalue2, fvalue1 - fvalue2));
                } else if (fbin_inst->op == MUL) {
                    dst_value = SRC(new CTValue(TypeFloat, fvalue1 * fvalue2, fvalue1 * fvalue2));
                } else if (fbin_inst->op == DIV) {
                    dst_value = SRC(new CTValue(TypeFloat, fvalue1 / fvalue2, fvalue1 / fvalue2));
                }
                function->replaceSRCs(block, fbin_inst->dst.reg, dst_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        }
        Case (LLIR_ICMP, icmp_inst, inst) {
            SRC src1 = icmp_inst->src1;
            SRC src2 = icmp_inst->src2;
            if (src1.ToCTValue() && src2.ToCTValue()) {
                int32_t svalue1 = src1.ctv->int_value;
                int32_t svalue2 = src2.ctv->int_value;
                SRC dst_value;
                if (icmp_inst->op == LTH) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 < svalue2, svalue1 < svalue2));
                } else if (icmp_inst->op == LEQ) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 <= svalue2, svalue1 <= svalue2));
                } else if (icmp_inst->op == EQU) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 == svalue2, svalue1 == svalue2));
                } else if (icmp_inst->op == NEQ) {
                    dst_value = SRC(new CTValue(TypeInt, svalue1 != svalue2, svalue1 != svalue2));
                } 
                function->replaceSRCs(block, icmp_inst->dst.reg, dst_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } 
        Case (LLIR_FCMP, fcmp_inst, inst) {
            SRC src1 = fcmp_inst->src1;
            SRC src2 = fcmp_inst->src2;
            if (src1.ToCTValue() && src2.ToCTValue()) {
                double fvalue1 = src1.ctv->float_value;
                double fvalue2 = src2.ctv->float_value;
                SRC dst_value;
                if (fcmp_inst->op == LTH) {
                    dst_value = SRC(new CTValue(TypeInt, fvalue1 < fvalue2, fvalue1 < fvalue2));
                } else if (fcmp_inst->op == LEQ) {
                    dst_value = SRC(new CTValue(TypeInt, fvalue1 <= fvalue2, fvalue1 <= fvalue2));
                } else if (fcmp_inst->op == EQU) {
                    dst_value = SRC(new CTValue(TypeInt, fvalue1 == fvalue2, fvalue1 == fvalue2));
                } else if (fcmp_inst->op == NEQ) {
                    dst_value = SRC(new CTValue(TypeInt, fvalue1 != fvalue2, fvalue1 != fvalue2));
                } 
                function->replaceSRCs(block, fcmp_inst->dst.reg, dst_value);
                iter = block->basic_block.erase(iter) - 1;
            }
        } 
        Case (LLIR_PHI, phi_inst, inst) {

        }
    }
}
#include "constant_propagation.hh"

void ConstantProg::runConstantProp() {
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        processInBlock(block);
    }
}

void ConstantProg::processInBlock(BasicBlock *block) {
    list<Inst *> bb_list(block->basic_block.begin(), block->basic_block.end());
    for (auto &&iter = bb_list.begin(); iter != bb_list.end();) {
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
                iter = bb_list.erase(iter);
                continue;
            } else if (src1.ToCTValue()) {
                int32_t svalue1 = src1.ctv->int_value;
                SRC dst_value;
                bool opt = false;
                if (bin_inst->op == ADD && svalue1 == 0) {
                    dst_value = bin_inst->src2;
                    opt = true;
                } else if (bin_inst->op == MUL) {
                    if (svalue1 == 0) {
                        dst_value = SRC(new CTValue(TypeInt, 0, 0));
                        opt = true;
                    } else if (svalue1 == 1) {
                        dst_value = bin_inst->src2;
                        opt = true;
                    }
                } 
                if (opt) {
                    function->replaceSRCs(block, bin_inst->dst.reg, dst_value);
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else if (src2.ToCTValue()) {
                int32_t svalue2 = src2.ctv->int_value;
                SRC dst_value;
                bool opt = false;
                if (bin_inst->op == ADD && svalue2 == 0) {
                    dst_value = bin_inst->src1;
                    opt = true;
                } else if (bin_inst->op == SUB && svalue2 == 0) {
                    dst_value = bin_inst->src1;
                    opt = true;
                } else if (bin_inst->op == MUL) {
                    if (svalue2 == 0) {
                        dst_value = SRC(new CTValue(TypeInt, 0, 0));
                        opt = true;
                    } else if (svalue2 == 1) {
                        dst_value = bin_inst->src1;
                        opt = true;
                    }
                } else if (bin_inst->op == DIV && svalue2 == 1) {
                    dst_value = bin_inst->src1;
                    opt = true;
                }
                if (opt) {
                    function->replaceSRCs(block, bin_inst->dst.reg, dst_value);
                    iter = bb_list.erase(iter);
                    continue;
                }
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
                iter = bb_list.erase(iter);
                continue;
            } else if (src1.ToCTValue()) {
                double fvalue1 = src1.ctv->float_value;
                SRC dst_value;
                bool opt = false;
                if (fbin_inst->op == ADD && fvalue1 == 0) {
                    dst_value = fbin_inst->src2;
                    opt = true;
                } else if (fbin_inst->op == MUL) {
                    if (fvalue1 == 0) {
                        dst_value = SRC(new CTValue(TypeFloat, 0, 0));
                        opt = true;
                    } else if (fvalue1 == 1) {
                        dst_value = fbin_inst->src2;
                        opt = true;
                    }
                } 
                if (opt) {
                    function->replaceSRCs(block, fbin_inst->dst.reg, dst_value);
                    iter = bb_list.erase(iter);
                    continue;
                }
            } else if (src2.ToCTValue()) {
                double fvalue2 = src2.ctv->float_value;
                SRC dst_value;
                bool opt = false;
                if (fbin_inst->op == ADD && fvalue2 == 0) {
                    dst_value = fbin_inst->src1;
                    opt = true;
                } else if (fbin_inst->op == SUB && fvalue2 == 0) {
                    dst_value = fbin_inst->src1;
                    opt = true;
                } else if (fbin_inst->op == MUL) {
                    if (fvalue2 == 0) {
                        dst_value = SRC(new CTValue(TypeFloat, 0, 0));
                        opt = true;
                    } else if (fvalue2 == 1) {
                        dst_value = fbin_inst->src1;
                        opt = true;
                    }
                } else if (fbin_inst->op == DIV && fvalue2 == 1) {
                    dst_value = fbin_inst->src1;
                    opt = true;
                }
                if (opt) {
                    function->replaceSRCs(block, fbin_inst->dst.reg, dst_value);
                    iter = bb_list.erase(iter);
                    continue;
                }
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
                    dst_value = SRC(new CTValue(TypeBool, svalue1 < svalue2, svalue1 < svalue2));
                } else if (icmp_inst->op == LEQ) {
                    dst_value = SRC(new CTValue(TypeBool, svalue1 <= svalue2, svalue1 <= svalue2));
                } else if (icmp_inst->op == EQU) {
                    dst_value = SRC(new CTValue(TypeBool, svalue1 == svalue2, svalue1 == svalue2));
                } else if (icmp_inst->op == NEQ) {
                    dst_value = SRC(new CTValue(TypeBool, svalue1 != svalue2, svalue1 != svalue2));
                } 
                function->replaceSRCs(block, icmp_inst->dst.reg, dst_value);
                iter = bb_list.erase(iter);
                continue;
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
                    dst_value = SRC(new CTValue(TypeBool, fvalue1 < fvalue2, fvalue1 < fvalue2));
                } else if (fcmp_inst->op == LEQ) {
                    dst_value = SRC(new CTValue(TypeBool, fvalue1 <= fvalue2, fvalue1 <= fvalue2));
                } else if (fcmp_inst->op == EQU) {
                    dst_value = SRC(new CTValue(TypeBool, fvalue1 == fvalue2, fvalue1 == fvalue2));
                } else if (fcmp_inst->op == NEQ) {
                    dst_value = SRC(new CTValue(TypeBool, fvalue1 != fvalue2, fvalue1 != fvalue2));
                } 
                function->replaceSRCs(block, fcmp_inst->dst.reg, dst_value);
                iter = bb_list.erase(iter);
                continue;
            }
        } 
        Case (LLIR_PHI, phi_inst, inst) {
            auto &&srcs = phi_inst->srcs;
            bool is_propagation = true;
            for (auto &&src : srcs) {
                if (src.first.ToCTValue() == nullptr) {
                    is_propagation = false;
                    break;
                }
            }
            if (is_propagation) {
                bool propagation = true;
                DeclType decl_type = phi_inst->dst.getType();
                int32_t ivalue = srcs[0].first.ctv->int_value;
                double fvalue = srcs[0].first.ctv->float_value;
                if (decl_type == TypeInt) {
                    for (auto &&src : srcs) {
                        if (src.first.ctv->int_value != ivalue) {
                            propagation = false;
                            break;
                        }
                    }
                } else if (decl_type == TypeFloat) {
                    for (auto &&src : srcs) {
                        if (src.first.ctv->float_value != fvalue) {
                            propagation = false;
                            break;
                        }
                    }
                } else {
                    propagation = false;
                }
                if (propagation) {
                    SRC dst_value = SRC(new CTValue(decl_type, ivalue, fvalue));
                    function->replaceSRCs(block, phi_inst->dst.reg, dst_value);
                    iter = bb_list.erase(iter);
                    continue;
                }
            }
        }
        Case (LLIR_XOR, xor_inst, inst) {
            SRC src = xor_inst->src;
            if (src.ToCTValue()) {
                int32_t svalue1 = src.ctv->int_value;
                SRC dst_value= SRC(new CTValue(TypeBool, svalue1 ^ 1, svalue1 ^ 1));
                function->replaceSRCs(block, xor_inst->dst.reg, dst_value);
                iter = bb_list.erase(iter);
                continue;
            }
        }
        Case (LLIR_ZEXT, zext_inst, inst) {
            SRC src = zext_inst->src;
            if (src.ToCTValue()) {
                int32_t svalue1 = src.ctv->int_value;
                SRC dst_value= SRC(new CTValue(TypeInt, svalue1, svalue1));
                function->replaceSRCs(block, zext_inst->dst.reg, dst_value);
                iter = bb_list.erase(iter);
                continue;
            }
        }
        ++iter;
    }
    block->basic_block.assign(bb_list.begin(), bb_list.end());
}

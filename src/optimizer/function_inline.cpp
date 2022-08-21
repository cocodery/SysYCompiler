#include "function_inline.hh"

bool FuncInline::isRecursive(FunctionInfo *to_check, FunctionInfo *now, set<FunctionInfo *> &vis) {
    // auto &&now_function = info_to_function.at(now);
    for (auto &&called_func : now->called_funcs) {
        if (called_func == to_check) { // 找到递归调用
            return true;
        }
        if (vis.find(called_func) != vis.end()) { // 如果已经找过了这个函数，跳过
            continue;
        }
        vis.insert(called_func);
        if (isRecursive(to_check, called_func, vis)) { // 没找过这个函数，那就看能不能这里找到循环调用
            return true; // 找到了，返回真
        }
        // 没找到，继续
    }
    return false;
}

bool FuncInline::sideEffect(Function *function) {
    if (function->func_info.func_name == "main") {
        return true;
    }
    for (auto &&func_info : function->func_info.called_funcs) {
        if (func_info->side_effect == true) {
            return true;
        }
    }
    int32_t args_num = function->func_info.func_args.size();
    set<VirtReg *> globalOrArgPtr;
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_GEP, gep_inst, inst) {
                auto &&src_reg = gep_inst->src.reg;
                auto &&dst_reg = gep_inst->dst.reg;
                if (src_reg->global || src_reg->reg_id < args_num) {
                    globalOrArgPtr.insert(src_reg);
                }
                if (globalOrArgPtr.find(src_reg) != globalOrArgPtr.end()) {
                    globalOrArgPtr.insert(dst_reg);
                }
            } else Case (LLIR_STORE, store_inst, inst) {
                auto &&dst_reg = store_inst->dst.reg;
                if (dst_reg->global || globalOrArgPtr.find(dst_reg) != globalOrArgPtr.end()) {
                    return true;
                }
            }
        }
    }
    return false;
}

int FuncInline::get_depth(Scope *now_scope, int now_depth) {
    int max_depth = now_depth;
    for (auto iter = now_scope->elements->begin(); iter != now_scope->elements->end(); ++iter) {
        if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
            max_depth = std::max(max_depth, get_depth(scope_node, now_depth + 1));
        }
    }
    return max_depth;
}

bool FuncInline::inLinable(string func_name, FuncMap &func_map) {
    if (func_map.find(func_name) == func_map.end()) {
        return false;
    }
    auto &&func = func_map[func_name];
    // dbg(func->func_info.func_name);
    if (func->multiReturn() || func->func_info.is_recursive || get_depth(func->main_scope, 1) > 3) {
        return false;
    }
    // dbg("return true");
    return true;
}

void FuncInline::initInlineMap(vector<SRC> caller_args, vector<pair<string, VarType>> callee_args) {
    inlineMap.clear();
    bbIdxMap.clear();
    // dbg("init inline-map");
    for (int32_t idx = 0; idx < caller_args.size(); ++idx) {
        inlineMap.insert({{idx, false}, caller_args[idx]});
    }
}

SRC FuncInline::findInMap(SRC src) {
    if ((!src.ctv && !src.reg) || src.ctv || src.reg->global) { 
        return src;
    } else if (inlineMap.find({src.reg->reg_id, src.reg->global}) != inlineMap.end()) {
        return inlineMap[{src.reg->reg_id, src.reg->global}];
    } else {
        SRC new_src = SRC(new VirtReg(function->var_idx++, src.reg->type, src.reg->global));
        inlineMap.insert({{src.reg->reg_id, src.reg->global}, new_src});
        return new_src;
    }

}

list<Inst *> FuncInline::insertBlock(BasicBlock *block, SRC &dst) {
    // dbg("enter insertBlock");
    // dbg(function->var_idx);
    list<Inst *> insertbb;
    for (auto &&inst : block->basic_block) {
        Case (LLIR_RET, ret_inst, inst) {
            // dbg(ret_inst->ToString());
            if (ret_inst->has_retvalue) {
                auto &&ret_value = findInMap(ret_inst->ret_value);
                dst = ret_value;
            }
        } 
        Case (LLIR_BR, br_inst, inst) {
            auto &&cond = SRC();
            if (br_inst->has_cond) {
                cond = findInMap(br_inst->cond);
            }
            LLIR_BR *inst_br = new LLIR_BR(br_inst->has_cond, cond, bbIdxMap[br_inst->tar_true], bbIdxMap[br_inst->tar_false]);
            // dbg(inst_br->ToString());
            insertbb.push_back(inst_br);
        }
        Case (LLIR_BIN, bin_inst, inst) {
            auto &&src1 = findInMap(bin_inst->src1);
            auto &&src2 = findInMap(bin_inst->src2);
            auto &&dst  = findInMap(bin_inst->dst);
            LLIR_BIN *inst_bin = new LLIR_BIN(bin_inst->op, dst, src1 ,src2);
            // dbg(inst_bin->ToString());
            insertbb.push_back(inst_bin);
        } 
        Case (LLIR_FBIN, fbin_inst, inst) {
            auto &&src1 = findInMap(fbin_inst->src1);
            auto &&src2 = findInMap(fbin_inst->src2);
            auto &&dst  = findInMap(fbin_inst->dst);
            LLIR_FBIN *inst_fbin = new LLIR_FBIN(fbin_inst->op, dst, src1 ,src2);
            // dbg(inst_fbin->ToString());
            insertbb.push_back(inst_fbin);
        } 
        Case (LLIR_ALLOCA, alloca_inst, inst) {
            auto &&dst = findInMap(alloca_inst->reg);
            auto &&var = alloca_inst->var;
            LLIR_ALLOCA *inst_alloca = new LLIR_ALLOCA(dst, alloca_inst->var);
            // dbg(inst_alloca->ToString());
            insertbb.push_back(inst_alloca);
        }
        Case (LLIR_LOAD, load_inst, inst) {
            auto &&src = findInMap(load_inst->src);
            auto &&dst = findInMap(load_inst->dst);
            LLIR_LOAD *inst_load= new LLIR_LOAD(dst, src);
            // dbg(inst_load->ToString());
            insertbb.push_back(inst_load); 
        } 
        Case (LLIR_STORE, store_inst, inst) {
            auto &&src = findInMap(store_inst->src);
            auto &&dst = findInMap(store_inst->dst);
            LLIR_STORE *inst_store = new LLIR_STORE(dst, src);
            // dbg(inst_store->ToString());
            insertbb.push_back(inst_store);
        } 
        Case (LLIR_ICMP, icmp_inst, inst) {
            auto &&src1 = findInMap(icmp_inst->src1);
            auto &&src2 = findInMap(icmp_inst->src2);
            auto &&dst  = findInMap(icmp_inst->dst);
            LLIR_ICMP *inst_icmp = new LLIR_ICMP(icmp_inst->op, dst, src1 ,src2);
            // dbg(inst_icmp->ToString());
            insertbb.push_back(inst_icmp);
        } 
        Case (LLIR_FCMP, fcmp_inst, inst) {
            auto &&src1 = findInMap(fcmp_inst->src1);
            auto &&src2 = findInMap(fcmp_inst->src2);
            auto &&dst  = findInMap(fcmp_inst->dst);
            LLIR_FCMP *inst_fcmp = new LLIR_FCMP(fcmp_inst->op, dst, src1 ,src2);
            // dbg(inst_fcmp->ToString());
            insertbb.push_back(inst_fcmp);
        } 
        Case (LLIR_CALL, call_inst, inst) {
            // dbg(call_inst->ToString());
            auto &&dst = findInMap(call_inst->dst);
            vector<SRC> args;
            for (auto &&arg : call_inst->args) {
                auto &&new_arg = findInMap(arg);
                // dbg(new_arg.ToString());
                args.push_back(new_arg);
            }
            LLIR_CALL *inst_call= new LLIR_CALL(dst, args, call_inst->func_info);
            // dbg(inst_call->ToString());
            insertbb.push_back(inst_call);
        } 
        Case (LLIR_GEP, gep_inst, inst) {
            auto &&src1 = findInMap(gep_inst->src);
            auto &&src2 = findInMap(gep_inst->off);
            auto &&dst  = findInMap(gep_inst->dst);
            LLIR_GEP *inst_gep = new LLIR_GEP(dst, src1 ,src2, dst.reg->type);
            // dbg(inst_gep->ToString());
            insertbb.push_back(inst_gep);
        } 
        Case (LLIR_XOR, xor_inst, inst) {
            auto &&src = findInMap(xor_inst->src);
            auto &&dst = findInMap(xor_inst->dst);
            LLIR_XOR *inst_xor = new LLIR_XOR(dst, src);
            // dbg(inst_xor->ToString());
            insertbb.push_back(inst_xor); 
        } 
        Case (LLIR_ZEXT, zext_inst, inst) {
            auto &&src = findInMap(zext_inst->src);
            auto &&dst = findInMap(zext_inst->dst);
            LLIR_ZEXT *inst_zext = new LLIR_ZEXT(dst, src);
            // dbg(inst_zext->ToString());
            insertbb.push_back(inst_zext);         
        } 
        Case (LLIR_SITOFP, itf_inst, inst) {
            auto &&src = findInMap(itf_inst->src);
            auto &&dst = findInMap(itf_inst->dst);
            LLIR_SITOFP *inst_itf = new LLIR_SITOFP(dst, src);
            // dbg(inst_itf->ToString());
            insertbb.push_back(inst_itf); 
        } 
        Case (LLIR_FPTOSI, fti_inst, inst) {
            auto &&src = findInMap(fti_inst->src);
            auto &&dst = findInMap(fti_inst->dst);
            LLIR_FPTOSI *inst_fti = new LLIR_FPTOSI(dst, src);
            // dbg(inst_fti->ToString());
            insertbb.push_back(inst_fti); 
        } 
    }
    // dbg("exit  insertBlock");
    return insertbb;
}

void FuncInline::simpleInline(BasicBlock *block, Function *callee_func) {
    // dbg("excute simple-inline");
    auto &&callee_func_info = callee_func->func_info;
    
    list<Inst *> inst_list(block->basic_block.begin(), block->basic_block.end());

    for (auto &&iter = inst_list.begin(); iter != inst_list.end(); ++iter) {
        auto &&inst = *iter;
        Case (LLIR_CALL, call_inst, inst) {
            if (call_inst->func_info->func_name != callee_func->func_info.func_name) {
                continue;
            }
            // init inline-map here to avoid callee be called more than one time
            initInlineMap(call_inst->args, callee_func->func_info.func_args);
            // dbg(call_inst->ToString());
            // erase this call-inst
            iter = inst_list.erase(iter); 
            auto &&new_call_dst = SRC();
            list<Inst *> insertbb = insertBlock(callee_func->all_blocks[1], new_call_dst);
            if (call_inst->func_info->return_type != TypeVoid) {
                function->replaceSRCs(block, call_inst->dst.reg, new_call_dst);
            }
            
            for (auto &&inst : insertbb) {
                inst_list.insert(iter, inst);
            }
        } 
    }
    block->basic_block = vector<Inst *>(inst_list.begin(), inst_list.end());
    // block->debugBlock();
    // dbg("exit simple-inline");
} 

list<BasicBlock *> FuncInline::genInlineBlocks(Function *callee_func, SRC &dst) {
    list<BasicBlock *> blocks;
    auto &&callee_allbbs = callee_func->all_blocks;
    for (auto &&block : callee_allbbs) {
        bbIdxMap.insert({block->bb_idx, function->bb_idx++});
    }
    for (auto &&block : callee_allbbs) {
        if (block->bb_idx == 0 || block->bb_idx == -1) {
            continue;
        }
        // dbg(block->bb_idx);
        block->debugBlock();
        auto &&insertbb = new BasicBlock(bbIdxMap[block->bb_idx], true);
        auto &&instlist = insertBlock(block, dst);
        insertbb->basic_block = vector<Inst *>(instlist.begin(), instlist.end());
        blocks.push_back(insertbb);
    }
    return blocks;
}

void FuncInline::ctrlflowInline(BasicBlock *block, vector<BasicBlock *> &all_block, Function *callee_func) {
    auto &&callee_func_info = callee_func->func_info;
    vector<BasicBlock *> bb_list;
    for (auto &&bb_iter = all_block.begin(); bb_iter != all_block.end(); ++bb_iter) {
        auto &&bb = *bb_iter;
        if (bb->bb_idx != block->bb_idx) {
            bb_list.push_back(bb);
        } else { // find the block
            BasicBlock *split_bb = new BasicBlock(block->bb_idx, true);
            for (auto &&inst_iter = block->basic_block.begin(); inst_iter != block->basic_block.end(); ++inst_iter) {
                auto &&inst = *inst_iter;
                Case (LLIR_CALL, call_inst, inst) { // meet call-inst
                    if (call_inst->func_info->func_name != callee_func->func_info.func_name) {
                        split_bb->basic_block.push_back(call_inst); // other-call-inst
                    } else { // target-call-inst
                        initInlineMap(call_inst->args, callee_func->func_info.func_args);
                        SRC new_call_dst = SRC();
                        auto &&insertbbs = genInlineBlocks(callee_func, new_call_dst);
                        if (split_bb->basic_block.size()) { // if split-bb before call is not empty
                            bb_list.push_back(split_bb);
                            auto &&firstInsertbb = *insertbbs.begin();
                            LLIR_BR *inst_br = new LLIR_BR(false, SRC(), firstInsertbb->bb_idx, 0);
                            split_bb->basic_block.push_back(inst_br);
                            // create a new split-bb
                            split_bb = new BasicBlock(function->bb_idx++, true);
                        }
                        for (auto &&insertbb : insertbbs) {
                            bb_list.push_back(insertbb); // insert inline-blocks to caller-blocks
                        }
                        if (call_inst->func_info->return_type != TypeVoid) { // replace if has ret-value
                            function->replaceSRCs(block, call_inst->dst.reg, new_call_dst);
                        }
                        auto &&lastInsertbb = *insertbbs.rbegin();
                        LLIR_BR *inst_br = new LLIR_BR(false, SRC(), split_bb->bb_idx, 0);
                        lastInsertbb->basic_block.push_back(inst_br);
                    }
                } else { // other-inst
                    split_bb->basic_block.push_back(inst);
                }
            }
            if (split_bb->basic_block.size()) {
                bb_list.push_back(split_bb);
            }
        }
    }
    all_block.assign(bb_list.begin(), bb_list.end());
}

void FuncInline::excuteFuncInline(BasicBlock *block, vector<BasicBlock *> &all_block, Function *func) {
    if (func->all_blocks.size() == 3) {
        dbg("enter simple-inline");
        simpleInline(block, func);
        dbg("exit  simple-inline");
    } else {
        dbg("enter ctrlflow-inline");
        ctrlflowInline(block, all_block, func);
        dbg("exit  ctrlflow-inline");
    }
}

void FuncInline::rebuildScope(BasicBlock *block, Function *callee_func) {
    auto &&scope = function->main_scope;
    auto &&all_blocks = function->all_blocks;
    for (auto &&iter = scope->elements->begin(); iter != scope->elements->end(); ++iter) {
        if (auto &&block_node = dynamic_cast<BasicBlock *>(*iter); block_node != nullptr) {
            if (block->bb_idx == block_node->bb_idx) {
                int32_t callee_size = callee_func->all_blocks.size() - 2;
                vector<BasicBlock *> insertbbs(all_blocks.begin() + block->bb_idx, all_blocks.begin() + block->bb_idx + callee_size);
                scope->elements->insert(iter, createNewScope(callee_func, insertbbs));
                scope->elements->insert(iter, *(all_blocks.begin() + block->bb_idx + callee_size + 1));
            }
        }
    }
}

Scope *FuncInline::createNewScope(Function *callee_func, vector<BasicBlock *> insertbbs) {
    // int32_t idx = 0;
    // auto &&scope = callee_func->main_scope;
    // Scope *new_scope = new Scope(0);
    // new_scope->elements = new vector<Info *>;
    // stack<Scope *> scope_stack;
    // scope_stack.push(new_scope);
    // auto &&cur_scope = scope_stack.top();
    // for (auto &&iter = scope->elements->begin(); iter != scope->elements->end(); ++iter) {
    //     if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
    //         Scope *n_scope = new Scope(0);
    //         new_scope->elements = new vector<Info *>;
    //         scope_stack.push(n_scope);
    //         cur_scope = scope_stack.top();
    //     } else {
    //         cur_scope->elements->push_back(insertbbs[idx++]);
    //     }
    // }
    
}

void FuncInline::runFuncInline(FuncMap &func_map) {
    if (function->func_info.called_funcs.size() == 0) return;
    auto &&all_blocks = function->all_blocks;
    for (auto i_block = 0; i_block < all_blocks.size(); ++i_block) {
        auto &&block = all_blocks[i_block];
        for (auto iterator_inst = block->basic_block.begin(); iterator_inst != block->basic_block.end(); ++iterator_inst) {
            Case (LLIR_CALL, call_inst, *iterator_inst) {
                string callee_name = call_inst->func_info->func_name;
                if (inLinable(callee_name, func_map)) {
                    dbg(call_inst->ToString());
                    auto &&inlined_func = func_map[callee_name];
                    // init caller-args --> callee-args
                    dbg("inlined function is " + inlined_func->func_info.func_name);
                    excuteFuncInline(block, all_blocks, inlined_func);
                    function->buildAllBlocksCFG();
                    rebuildScope(block, inlined_func);
                    break;
                }
            }
        }
    }
}

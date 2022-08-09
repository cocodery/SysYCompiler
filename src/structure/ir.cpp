#include "ir.hh"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
#pragma clang diagnostic ignored "-Wunused-result"

int32_t tab_num = 1;

Inst *BasicBlock::lastInst() {
    return basic_block.size() > 0 ? basic_block[basic_block.size() - 1] : nullptr;
}

void BasicBlock::debugBlock() {
    cout << get_tabs(tab_num - 1) << "Block" << bb_idx << ":" << endl;
    for (auto inst: basic_block) {
        // LLVM IR
        Case (LLIR_RET, ret_inst, inst) {
            cout << get_tabs() << ret_inst->ToString() << endl;
        } else Case (LLIR_BR, br_inst, inst) {
            cout <<get_tabs() << br_inst->ToString() << endl;
        } else Case (LLIR_BIN, bin_inst, inst) {
            cout << get_tabs() << bin_inst->ToString() << endl;
        } else Case (LLIR_FBIN, fbin_inst, inst) {
            cout << get_tabs() << fbin_inst->ToString() << endl;
        } else Case (LLIR_ALLOCA, alloc_inst, inst) {
            cout << get_tabs() << alloc_inst->ToString() << endl;
        } else Case (LLIR_LOAD, load_inst, inst) {
            cout << get_tabs() << load_inst->ToString() << endl;
        } else Case (LLIR_STORE, store_inst, inst) {
            cout << get_tabs() << store_inst->ToString() << endl;
        } else Case (LLIR_ICMP, icmp_inst, inst) {
            cout << get_tabs() << icmp_inst->ToString() << endl;
        } else Case (LLIR_FCMP, fcmp_inst, inst) {
            cout << get_tabs() << fcmp_inst->ToString() << endl;
        } else Case (LLIR_CALL, call_inst, inst) {
            cout << get_tabs() << call_inst->ToString() << endl;
        } else Case (LLIR_ZEXT, zext_inst, inst) {
            cout << get_tabs() << zext_inst->ToString() << endl;
        } else Case (LLIR_GEP, gep_inst, inst) {
            cout << get_tabs() << gep_inst->ToString() << endl;
        } else Case (LLIR_XOR, xor_inst, inst) {
            cout << get_tabs() << xor_inst->ToString() << endl;
        } else Case (LLIR_BC, bc_inst, inst) {
            cout << get_tabs() << bc_inst->ToString() << endl; 
        } else Case (LLIR_SITOFP, itf_inst, inst) {
            cout << get_tabs() << itf_inst->ToString() << endl;
        } else Case (LLIR_FPTOSI, fti_inst, inst) {
            cout << get_tabs() << fti_inst->ToString() << endl;
        } else Case (LLIR_PHI, phi_inst, inst) {
            cout << get_tabs() << phi_inst->ToString() << endl; 
        }
    }
}


void BasicBlock::printBlock() {
    if (valuable) {
        llir << get_tabs(tab_num - 1) << "Block" << bb_idx << ":" << endl;
        llir << get_tabs() << "; preds = ";
        for (auto &&parant : preds) {
            llir << parant.first << " ";
        }
        llir << endl;
        llir << get_tabs() << "; succs = ";
        for (auto &&child : succs) {
            llir << child.first << " ";
        }
        llir << endl;
    } else {
        return;
    }
    int32_t instIdx = 0;
    for (auto inst: basic_block) {
        // LLVM IR
        Case (LLIR_RET, ret_inst, inst) {
            llir << get_tabs() << ret_inst->ToString();
        } else Case (LLIR_BR, br_inst, inst) {
            llir << get_tabs() << br_inst->ToString();
        } else Case (LLIR_BIN, bin_inst, inst) {
            llir << get_tabs() << bin_inst->ToString();
        } else Case (LLIR_FBIN, fbin_inst, inst) {
            llir << get_tabs() << fbin_inst->ToString();
        } else Case (LLIR_ALLOCA, alloc_inst, inst) {
            llir << get_tabs() << alloc_inst->ToString();
        } else Case (LLIR_LOAD, load_inst, inst) {
            llir << get_tabs() << load_inst->ToString();
        } else Case (LLIR_STORE, store_inst, inst) {
            llir << get_tabs() << store_inst->ToString();
        } else Case (LLIR_ICMP, icmp_inst, inst) {
            llir << get_tabs() << icmp_inst->ToString();
        } else Case (LLIR_FCMP, fcmp_inst, inst) {
            llir << get_tabs() << fcmp_inst->ToString();
        } else Case (LLIR_CALL, call_inst, inst) {
            llir << get_tabs() << call_inst->ToString();
        } else Case (LLIR_ZEXT, zext_inst, inst) {
            llir << get_tabs() << zext_inst->ToString();
        } else Case (LLIR_GEP, gep_inst, inst) {
            llir << get_tabs() << gep_inst->ToString();
        } else Case (LLIR_XOR, xor_inst, inst) {
            llir << get_tabs() << xor_inst->ToString();
        } else Case (LLIR_BC, bc_inst, inst) {
            llir << get_tabs() << bc_inst->ToString();
        } else Case (LLIR_SITOFP, itf_inst, inst) {
            llir << get_tabs() << itf_inst->ToString();
        } else Case (LLIR_FPTOSI, fti_inst, inst) {
            llir << get_tabs() << fti_inst->ToString();
        } else Case (LLIR_PHI, phi_inst, inst) {
            llir << get_tabs() << phi_inst->ToString();
        }
        llir << "; " << std::to_string(firstInstIndex + (instIdx++)) << endl;
    }
}

void BasicBlock::initDom(vector<BasicBlock *> all_blocks) {
    for (auto &&block : all_blocks) {
        dom.insert(block);
    }
}

set<BasicBlock *> BasicBlock::predsDomInter() {
    set<BasicBlock *> ret = (*preds.begin()).second->dom;
    // for (auto &&block : ret) {
    //     // cout << block->bb_idx << ' ';
    // }
    // // cout << endl;
    for (auto &&pred : preds) {
        // // cout << pred.first << ' ';
        auto pred_dom = pred.second->dom;
        set<BasicBlock *> tmp;
        set_intersection(ret.begin(), ret.end(), pred_dom.begin(), pred_dom.end(), inserter(tmp, tmp.begin()));
        ret = tmp;
        // for (auto &&block : ret) {
        //     // cout << block->bb_idx << ' ';
        // }
        // // cout << endl;
    }
    ret.insert(this);
    return ret;
}

void BasicBlock::replaceSRC(VirtReg *old_reg, SRC new_var) {
    for (auto &&inst : basic_block) {
        Case (LLIR_RET, ret_inst, inst) {
            ret_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_BIN, bin_inst, inst) {
            bin_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_FBIN, fbin_inst, inst) {
            fbin_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_ICMP, icmp_inst, inst) {
            icmp_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_FCMP, fcmp_inst, inst) {
            fcmp_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_CALL, call_inst, inst) {
            call_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_ZEXT, zext_inst, inst) {
            zext_inst->replaceSRC(old_reg, new_var);            
        } else Case (LLIR_GEP, gep_inst, inst) {
            gep_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_XOR, xor_inst, inst) {
            xor_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_SITOFP, itf_inst, inst) {
            itf_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_FPTOSI, fti_inst, inst) {
            fti_inst->replaceSRC(old_reg, new_var);
        } else Case (LLIR_PHI, phi_inst, inst) {
            phi_inst->replaceSRC(old_reg, new_var); 
        } else Case (LLIR_STORE, store_inst, inst) {
            store_inst->replaceSRC(old_reg, new_var);
            store_inst->replaceDst(old_reg, new_var);
        } else Case (LLIR_LOAD, load_inst, inst) {
            load_inst->replaceSRC(old_reg, new_var);
        }
    }
}

SRC Scope::resolve(string var_name, FunctionInfo *cur_func_args) {
    auto cur_scope = this;
    VariableTable *cur_table = nullptr;
    while (cur_scope != nullptr) {
        cur_table = cur_scope->local_table;
        // search cur scope's variable table first
        if (cur_table->findInCurTable(var_name)) {
            // cout << "find in `Scope`" << cur_scope->sp_idx << endl;
            auto var = cur_table->getInCurTable(var_name);
            if (var->type.is_array == false && var->type.is_const) {
                return SRC(new CTValue(var->type.decl_type, var->int_scalar, var->float_scalar));
            }
            return SRC(new VirtReg(var->var_idx, var->type, (cur_scope->parent == nullptr)));
        }
        if (cur_func_args != nullptr && cur_scope->parent->parent == nullptr) { // if not in table, search in function args
            // cout << "not find in `Scope`" << cur_scope->sp_idx << " var_table, goto function arguments" << endl;
            auto pair = cur_func_args->findInFuncArgs(var_name);
            if (pair.second.decl_type != TypeVoid) {
                return SRC(new VirtReg(pair.first, pair.second, (cur_scope->parent == nullptr)));
            }
        }
        // cout << "not find in `Scope`" << cur_scope->sp_idx << " var_table, goto parent table" << endl;
        cur_scope = cur_scope->parent;
    }
    assert(cur_table != nullptr);
}

// 仅能在优化之前使用
// 目前仅用于前端代码
BasicBlock *Scope::get_last_bb() {
    int element_size = elements->size();
    auto iter = elements->begin();
    for (int i = 0; i < element_size - 1; ++i) {
        ++iter;
    }
    return dynamic_cast<BasicBlock *>(*iter);
}

void Scope::buildScopeCFG(vector<BasicBlock *> all_blocks) {
    for (auto iter = elements->begin(); iter != elements->end(); ++iter) {
        if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
            scope_node->buildScopeCFG(all_blocks);
        } else {
            BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*iter);
            auto &&last_inst = bb_node->lastInst();
            if (last_inst == nullptr) {
                iter = elements->erase(iter);
                iter = iter - 1;
            } else if (auto &&br_inst = dynamic_cast<LLIR_BR *>(last_inst); br_inst != nullptr) {
                bb_node->valuable = (bb_node->preds.size() != 0);
                // BB-preds都是顺序填入的
                // 没有preds的BB属于不可达BB
                if (bb_node->valuable) {
                    BasicBlock *child_bb1 = all_blocks[br_inst->tar_true  - 1];
                    BasicBlock *child_bb2 = all_blocks[br_inst->tar_false - 1];
                    bb_node->succs.insert({child_bb1->bb_idx, child_bb1});
                    child_bb1->preds.insert({bb_node->bb_idx, bb_node});
                    if (br_inst->has_cond) {
                        bb_node->succs.insert({child_bb2->bb_idx, child_bb2});
                        child_bb2->preds.insert({bb_node->bb_idx, bb_node});
                    }
                }
            } else if (auto &&ret_inst = dynamic_cast<LLIR_RET *>(last_inst); ret_inst != nullptr) {
                // 基本块的最后一个指令是`return instruction`
                // 当前作用域在此基本块之后的`elements`全部无效
                bb_node->valuable = (bb_node->preds.size() != 0);
                elements->erase(iter + 1, elements->end());
            } else {
                // dbg("UnExcepted Last Instruction");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void Scope::printElements() {
    for (auto iter = elements->begin(); iter != elements->end(); ++iter) {
        if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
            scope_node->printScope();
        } else {
            BasicBlock *bb_node = dynamic_cast<BasicBlock *>(*iter);
            bb_node->printBlock();
        }
    }
}

void Scope::printScope() {
    llir << get_tabs() << "; `Scope`" << sp_idx << endl;
    tab_num += 1;
    llir << get_tabs() << "; `BasicBlocks` of `Scope`" << sp_idx << endl;
    printElements();
    tab_num -= 1;
}

void Function::printCallInfo() {
    llir << get_tabs() << "; " <<  func_info.func_name << " call function : ";
    if (func_info.called_funcs.size()) {
        for (auto &&func_info : func_info.called_funcs) {
            llir << func_info->func_name << " ";
        }
    } else {
        llir << "`no function be called`";
    }
    llir << endl;
}

void Function::buildCFG() {
    // insert entry and exit basicblock
    BasicBlock *entrybb = new BasicBlock(0 , true);
    BasicBlock *exitbb  = new BasicBlock(-1, true);
    // first bb's preds is entrybb
    entrybb->succs.insert({(*all_blocks.begin())->bb_idx, *all_blocks.begin()});
    (*all_blocks.begin())->preds.insert({entrybb->bb_idx, entrybb});
    // build control-flow-graph
    main_scope->buildScopeCFG(all_blocks);
    // delete unreachable block
    for (auto iter = all_blocks.begin(); iter != all_blocks.end(); ++iter) {
        if (!(*iter)->valuable) {
            iter = all_blocks.erase(iter) - 1;
        } else {
            auto &&last_inst = (*iter)->lastInst();
            if (auto && ret_inst = dynamic_cast<LLIR_RET *>(last_inst); ret_inst != nullptr) {
                (*iter)->succs.insert({exitbb->bb_idx, exitbb});
                exitbb->preds.insert({(*iter)->bb_idx, (*iter)});
            }
        }
    }
    all_blocks.insert(all_blocks.begin(), entrybb); // first element of all_block must be `entrybb`
    all_blocks.push_back(exitbb);
}

void Function::buildDom() {
    int size = all_blocks.size() - 1;
    all_blocks[0]->dom.insert(all_blocks[0]);
    for (int32_t idx = 1; idx <= size; ++idx) {
        all_blocks[idx]->initDom(all_blocks);
    }
    // for (auto &&block : all_blocks) {
    //     // cout << block->bb_idx << " " << block->dom.size() << endl;
    // }
    bool change = true;
    while (change) {
        change = false;
        for (int32_t idx = 1; idx <= size; ++idx) {
            set<BasicBlock *> tmp = all_blocks[idx]->predsDomInter();
            if (tmp != all_blocks[idx]->dom) {
                all_blocks[idx]->dom = tmp;
                change = true;
            }
        }
    }
    // for (auto&& block : all_blocks) {
    //     // cout << block->bb_idx << " doms  = ";
    //     for (auto &&dom : block->dom) {
    //         // cout << dom->bb_idx << " ";
    //     }
    //     // cout << endl;
    // }
}

void Function::buildIDom() {
    BasicBlock *entrybb = *all_blocks.begin();
    for (auto &&block : all_blocks) {
        if (block->dom.size() > 1) {
            // // cout << bb_idx << "'s Dom : ";
            // for (auto &&block : dom) {
            //     // cout << block->bb_idx << ' ';
            // }
            // // cout << endl;
            block->idom = entrybb; // init idom with entrybb
            // assign idom with the nearest bb
            for (auto &&iter = block->dom.begin(); iter != block->dom.end(); ++iter) {
                int32_t iter_bb_idx = (*iter)->bb_idx; 
                if (iter_bb_idx > block->idom->bb_idx && iter_bb_idx != block->bb_idx) {
                    block->idom = *iter;
                }
            }
            // insert `this` to idom-bb as a domer
            block->idom->domers.insert(block);
            // // cout << "idom " << idom->bb_idx << endl;
        }
    }
    // for (auto &&block : all_blocks) {
    //     // cout << "BB" << block->bb_idx << " domers ";
    //     for (auto &&bb : block->domers) {
    //         // cout << bb->bb_idx << ' ';
    //     }
    //     // cout << endl;
    // }
}

void Function::initBBDF() {
    // init DF(n) with { `empty` }
    for (auto &&block : all_blocks) {
        block->DomFrontier.clear();
    }
    for (auto &&block : all_blocks) {
        if (block->preds.size() > 1) {
            for (auto &&pred : block->preds) {
                BasicBlock *runner = pred.second;
                while (runner->bb_idx != block->idom->bb_idx) {
                    runner->DomFrontier.insert(block);
                    runner = runner->idom;
                }
            }
        }
    }
    // for (auto &&block : all_blocks) {
    //     // dbg(block->bb_idx);
    //     for (auto &&bb : block->DomFrontier) {
    //         // cout << bb->bb_idx << ' ';
    //     }
    //     // cout << endl;
    // }
}

void Function::replaceSRCs(BasicBlock *block, VirtReg *old_reg, SRC new_var) {
    // for (auto &&block : all_blocks) {
    //     block->dirty = false;
    // }
    // queue<BasicBlock *> queueBlocks;
    // queueBlocks.push(block);
    // while(!queueBlocks.empty()) {
    //     BasicBlock *front = queueBlocks.front();
    //     queueBlocks.pop();
    //     if (front->dirty) {
    //         continue;
    //     }
    //     front->dirty = true;
    //     front->replaceSRC(old_reg, new_var);
    //     for (auto &&succ : block->succs) {
    //         queueBlocks.push(succ.second);
    //     }
    // }
    
    for (auto &&block : all_blocks) {
        block->replaceSRC(old_reg, new_var);
    }
}

void LibFunction::printFunction() {
    llir << libfunc_info.printFunctionInfo(true) << endl;
}

CompUnit::CompUnit() {
// Global  symtable Init Part
    global_scope = new Scope(0);
    global_scope->local_table = new VariableTable;
    global_scope->elements = new vector<Info *>;
// Global  Function Init Part
    functions.empty();
// Library Funtions Init Part
    string func_name[14] = { "getint"   , "getch"    , "getfloat", "getarray",
                             "getfarray", "putint"   , "putch"   , "putfloat", 
                             "putarray" , "putfarray", 
                             "_sysy_starttime", "_sysy_stoptime",
                             "imemset", "fmemset" };
    DeclType ret_type[14] = {  TypeInt, TypeInt, TypeFloat, TypeInt, 
                               TypeInt, TypeVoid, TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid };
    for (int32_t i = 0; i < 14; ++i) {
        lib_functions[i] = new LibFunction;
        lib_functions[i]->libfunc_info.is_used = false;
        lib_functions[i]->libfunc_info.func_name = func_name[i];
        lib_functions[i]->libfunc_info.return_type = ret_type[i];
    }
    // getint
    lib_functions[0]->libfunc_info.func_args.resize(0);
    // getch
    lib_functions[1]->libfunc_info.func_args.resize(0);
    // getfloat
    lib_functions[2]->libfunc_info.func_args.resize(0);
    // getarray
    lib_functions[3]->libfunc_info.func_args.push_back(make_pair("", VarType(false, true,  true, TypeInt)));
    lib_functions[3]->libfunc_info.func_args[0].second.array_dims.push_back(-1);
    // getfarray
    lib_functions[4]->libfunc_info.func_args.push_back(make_pair("", VarType(false, true,  true, TypeFloat)));
    lib_functions[4]->libfunc_info.func_args[0].second.array_dims.push_back(-1);
    // putint
    lib_functions[5]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    // putch
    lib_functions[6]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    // putfloat
    lib_functions[7]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeFloat)));
    // putarray
    lib_functions[8]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    lib_functions[8]->libfunc_info.func_args.push_back(make_pair("", VarType(false, true,  true, TypeInt)));
    lib_functions[8]->libfunc_info.func_args[1].second.array_dims.push_back(-1);
    // putfarray
    lib_functions[9]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    lib_functions[9]->libfunc_info.func_args.push_back(make_pair("", VarType(false, true,  true, TypeFloat)));
    lib_functions[9]->libfunc_info.func_args[1].second.array_dims.push_back(-1);
    // _sysy_starttime
    lib_functions[10]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    // sysy_stoptime
    lib_functions[11]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    // imemset
    lib_functions[12]->libfunc_info.func_args.push_back(make_pair("", VarType(false, true, true, TypeInt)));
    lib_functions[12]->libfunc_info.func_args[0].second.array_dims.push_back(-1);
    lib_functions[12]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    lib_functions[12]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    // fmemset
    lib_functions[13]->libfunc_info.func_args.push_back(make_pair("", VarType(false, true, true, TypeFloat)));
    lib_functions[13]->libfunc_info.func_args[0].second.array_dims.push_back(-1);
    lib_functions[13]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
    lib_functions[13]->libfunc_info.func_args.push_back(make_pair("", VarType(false, false, true, TypeInt)));
}

FunctionInfo *CompUnit::getFunctionInfo(string func_name) {
    for (auto function: functions) {
        if (func_name == function->func_info.func_name) {
            return &function->func_info;
        }
    }
    for (auto lib_function: lib_functions) {
        if (func_name == lib_function->libfunc_info.func_name) {
            return &lib_function->libfunc_info;
        }
    }
    return nullptr;
}

void CompUnit::GenerateLLIR(string _llir) {
    llir.open(_llir);
    DebugGlobalTable();
    DebugUserFuncs();
    DebugLibFuncs();
    llir.close();
}

void CompUnit::DebugLibFuncs() {
    llir << "; Init Lib Functions" << endl;
    for (auto &&libfunction : lib_functions) {
        if (libfunction->libfunc_info.is_used) {
            llir << "    ";
            libfunction->printFunction();
        }
    }
}

void CompUnit::DebugUserFuncs() {
    llir << "; User Functions" << endl;
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            llir << get_tabs();
            llir << function->func_info.printFunctionInfo() << endl;
            function->printCallInfo();
            function->main_scope->printScope();
            llir << get_tabs() << "}" << endl;
        }
    }
}

void CompUnit::DebugGlobalTable() {
    llir << "; Global Variable" << endl;
    VariableTable *global_table = global_scope->local_table;
    for (auto pair: global_table->var_table) {
        if (pair.second->type.is_const && !pair.second->type.is_array && pair.second->type.decl_type != TypeFloat) continue;
        Variable *var = pair.second;
        llir << "    " << "@_" << var->var_idx << " = ";
        // var->var_idx = glb_var_idx;
        if (var->type.is_const) { 
            llir << "constant " << var->type.printVarTypeForAlc() << " ";
            if (var->type.is_array) {
                llir << "[";
                if (var->type.decl_type == TypeInt) {
                    llir << "i32 " << var->int_list[0];
                    for (int i = 1; i < var->int_list.size(); ++i) {
                        llir << ", i32 " << var->int_list[i];
                    }
                } else {
                    std::stringstream ss;
                    uint64_t uint64_value = reinterpret_cast<uint64_t&>(var->float_list[0]);
                    ss << "float " << "0x" << std::hex << uint64_value;
                    llir << ss.str();
                    for (int i = 1; i < var->int_list.size(); ++i) {
                        ss.clear();
                        uint64_t uint64_value = reinterpret_cast<uint64_t&>(var->float_list[i]);
                        ss << "float " << "0x" << std::hex << uint64_value;
                        llir << ss.str();
                    }
                }
                llir << "]";
            } else {
                if (var->type.decl_type == TypeInt) {
                    llir << var->int_scalar;
                } else {
                    std::stringstream ss;
                    uint64_t uint64_value = reinterpret_cast<uint64_t&>(var->float_scalar);
                    ss << "0x" << std::hex << uint64_value;
                    llir << ss.str();
                }
            }
        }
        else { 
            llir << "global " << var->type.printVarTypeForAlc() << " ";
            if (var->type.is_array) {
                llir << "zeroinitializer";
            } else {
                if (var->type.decl_type == TypeInt) {
                    llir << "0";
                } else {
                    llir << "0.000000e+00";
                }
            }
        }
        llir << ", align 4" << endl;
    }
    // cout << "Global Init Block" << endl;
    global_scope->elements->resize(1);
}

#pragma clang diagnostic pop

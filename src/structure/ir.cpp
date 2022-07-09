#include "ir.hh"

int32_t tab_num = 1;

void BasicBlock::printBlock() {
    llir << get_tabs() << "; `Block`" << bb_idx << endl;
    for (auto inst: basic_block) {
        Case (ReturnInst, ret_inst, inst) {
            ret_inst->printRetInst();
        }
        Case (LoadNumber, ldc_inst, inst) {
            ldc_inst->printLdcInst();
        }
        Case (UnaryOpInst, uop_inst, inst) {
            uop_inst->printuOpInst();
        }
        Case (BinaryOpInst, bop_inst, inst) {
            bop_inst->printbOpInst();
        }
        Case (StoreMem, stm_inst, inst) {
            stm_inst->printStmInst();
        }
        Case (LoadAddress, lad_inst, inst) {
            lad_inst->printLdaInst();
        }
        Case (LoadValue, ldv_inst, inst) {
            ldv_inst->printLdvInst();
        }
        Case (LoadOffset, ldo_inst, inst) {
            ldo_inst->printLdoInst();
        }
        Case (JzeroInst, jzo_inst, inst) {
            jzo_inst->printJzoInst();
        }
        Case (JnzroInst, jnz_inst, inst) {
            jnz_inst->printJnzInst();
        }
        Case (JumpInst, jmp_inst, inst) {
            jmp_inst->printJmpInst();
        }
        Case (LoadParam, ldp_inst, inst) {
            ldp_inst->printLdpInst();
        }
        Case (CallFuntion, cal_inst, inst) {
            cal_inst->printCalInst();
        }
        // LLVM IR
        Case (LLIR_RET, ret_inst, inst) {
            llir << get_tabs() << ret_inst->ToString() << endl;
        }
        Case (LLIR_BIN, bin_inst, inst) {
            llir << get_tabs() << bin_inst->ToString() << endl;
        }
        Case (LLIR_ALLOCA, alloc_inst, inst) {
            llir << get_tabs() << alloc_inst->ToString() << endl;
        }
        Case (LLIR_LOAD, load_inst, inst) {
            llir << get_tabs() << load_inst->ToString() << endl;
        }
        Case (LLIR_STORE, store_inst, inst) {
            llir << get_tabs() << store_inst->ToString() << endl;
        }
        Case (LLIR_ICMP, icmp_inst, inst) {
            llir << get_tabs() << icmp_inst->ToString() << endl;
        }
    }
}

VirtReg *Scope::resolve(string var_name, FunctionInfo *cur_func_args) {
    auto cur_scope = this;
    VariableTable *cur_table = nullptr;
    int32_t idx = 0;
    DeclType type = TypeVoid;
    while (cur_scope != nullptr) {
        cur_table = cur_scope->local_table;
        // search cur scope's variable table first
        if (cur_table->findInCurTable(var_name)) {
            cout << "find in `Scope`" << cur_scope->sp_idx << endl;
            auto var = cur_table->getInCurTable(var_name);
            idx = var->var_idx;
            type = var->type.decl_type;
            break;
        }
        if (cur_scope->parent->parent == nullptr) { // if not in table, search in function args
            cout << "not find in `Scope`" << cur_scope->sp_idx << " var_table, goto function arguments" << endl;
            auto pair = cur_func_args->findInFuncArgs(var_name);
            idx = pair.first;
            type = pair.second;
            if (type != TypeVoid) break;
        }
        cout << "not find in `Scope`" << cur_scope->sp_idx << " var_table, goto parent table" << endl;
        cur_scope = cur_scope->parent;
    }
    assert(cur_table != nullptr);
    return new VirtReg(idx, type, (cur_scope->parent == nullptr));
}

BasicBlock *Scope::get_last_bb() {
    int element_size = elements->size();
    auto iter = elements->begin();
    for (int i = 0; i < element_size - 1; ++i) {
        ++iter;
    }
    return dynamic_cast<BasicBlock *>(*iter);
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
    llir << get_tabs() << "{ ; `Scope`" << sp_idx << endl;
    tab_num += 1;
    llir << get_tabs() << "; `VariableTable` of `Scope`" << sp_idx << endl;
    local_table->printVaribaleTable();
    llir << get_tabs() << "; `BasicBlocks` of `Scope`" << sp_idx << endl;
    printElements();
    tab_num -= 1;
    llir << get_tabs() << "}" << endl;
}

void LibFunction::printFunction() {
    llir << libfunc_info.printFunctionInfo(true) << endl;
}

CompUnit::CompUnit(string _llir) {
// open llvm ir file
    llir.open(_llir);
// Global  symtable Init Part
    global_scope = new Scope;
    global_scope->local_table = new VariableTable;
    global_scope->elements = new vector<Info *>;
// Global  Function Init Part
    functions.empty();
// Library Funtions Init Part
    string func_name[12] = { "getint"   , "getch"    , "getfloat", "getarray",
                             "getfarray", "putint"   , "putch"   , "putfloat", 
                             "putarray" , "putfarray", 
                             "_sysy_starttime", "_sysy_stoptime" };
    DeclType ret_type[12] = {  TypeInt, TypeInt, TypeFloat, TypeInt, 
                               TypeInt, TypeVoid, TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid };
    for (int32_t i = 0; i < 12; ++i) {
        lib_functions[i].is_used = false;
        lib_functions[i].libfunc_info.func_name = func_name[i];
        lib_functions[i].libfunc_info.return_type = ret_type[i];
    }
    // getint
    lib_functions[0].libfunc_info.func_args.resize(0);
    // getch
    lib_functions[1].libfunc_info.func_args.resize(0);
    // getfloat
    lib_functions[2].libfunc_info.func_args.resize(0);
    // getarray
    lib_functions[3].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, true,  true, TypeInt)));
    lib_functions[3].libfunc_info.func_args[0].second.array_dims.push_back(-1);
    // getfarray
    lib_functions[4].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, true,  true, TypeFloat)));
    lib_functions[4].libfunc_info.func_args[0].second.array_dims.push_back(-1);
    // putint
    lib_functions[5].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeInt)));
    // putch
    lib_functions[6].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeInt)));
    // putfloat
    lib_functions[7].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeFloat)));
    // putarray
    lib_functions[8].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeInt)));
    lib_functions[8].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, true,  true, TypeInt)));
    lib_functions[8].libfunc_info.func_args[1].second.array_dims.push_back(-1);
    // putfarray
    lib_functions[9].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeInt)));
    lib_functions[9].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, true,  true, TypeFloat)));
    lib_functions[9].libfunc_info.func_args[1].second.array_dims.push_back(-1);
    // _sysy_starttime
    lib_functions[10].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeInt)));
    // sysy_stoptime
    lib_functions[11].libfunc_info.func_args.push_back(std::make_pair("", VarType(false, false, true, TypeInt)));
}

void CompUnit::moveGlobalInitToMain() {
    auto glb_init_bb = dynamic_cast<BasicBlock *>(*global_scope->elements->begin());
    Function *main_function = nullptr;
    for (auto function: functions) {
        if (function->func_info.func_name == "main") {
            main_function = function;
        }
    }
    if (main_function != nullptr) {
        auto element = main_function->main_scope->elements;
        element->insert( element->begin(), glb_init_bb);
    }
}

bool CompUnit::inLibFunctions(string func_name) {
    for (auto lib_function: lib_functions) {
        if (func_name == lib_function.libfunc_info.func_name) {
            return true;
        }
    }
    return false;
}

FunctionInfo *CompUnit::getFunctionInfo(string func_name) {
    for (auto function: functions) {
        if (func_name == function->func_info.func_name) {
            return &function->func_info;
        }
    }
    for (auto lib_function: lib_functions) {
        if (func_name == lib_function.libfunc_info.func_name) {
            return &lib_function.libfunc_info;
        }
    }
    return nullptr;
}

void CompUnit::DebugLibFuncs() {
    llir << "; Init Lib Functions" << endl;
    for (int i = 0; i < 12; ++i) {
        llir << "    ";
        lib_functions[i].printFunction();
    }
}

void CompUnit::DebugUserFuncs() {
    llir << "; User Functions" << endl;
    int size = functions.size();
    for (int i = 0; i < size; ++i) {
        llir << "    ";
        llir << functions[i]->func_info.printFunctionInfo() << endl;
        functions[i]->main_scope->printScope();
    }
}

void CompUnit::DebugGlobalTable() {
    llir << "; Global Variable" << endl;
    VariableTable *global_table = global_scope->local_table;
    int32_t glb_var_idx = 1;
    for (auto pair: global_table->var_table) {
        llir << "    " << "@_" << glb_var_idx++ << " = ";
        Variable *var = pair.second;
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
                    llir << "float " << var->int_list[0];
                    for (int i = 1; i < var->int_list.size(); ++i) {
                        llir << ", float " << var->int_list[i];
                    }
                }
                llir << "]";
            } else {
                if (var->type.decl_type == TypeInt) {
                    llir << var->int_scalar;
                } else {
                    llir << var->float_scalar;
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
    cout << "Global Init Block" << endl;
    global_scope->elements->resize(1);
    // global_scope->printElements();
}

#include "ir.hh"

void BasicBlock::printBlock() {
    cout << "\t// `BasicBlock` of current Scope" << endl;
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
        Case (AssignInst, ass_inst, inst) {
            ass_inst->printAssInst();
        }
        Case (LoadAddress, lad_inst, inst) {
            lad_inst->printLadInst();
        }
    }
}

Variable *Scope::resolve(string var_name) {
    if (local_table != nullptr){
        if (local_table->findInCurTable(var_name)) {
            cout << "find in cur scope's var_table" << endl;
            return local_table->getInCurTable(var_name);
        } else {
            cout << "not find in cur scope's var_table, goto parent table" << endl;
            return parent->resolve(var_name);
        }
    } else {
        return nullptr;
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
    cout << "\t{" << endl;
    cout << "\t// `VariableTable` of current Scope" << endl;
    local_table->printVaribaleTable();
    printElements();
    cout << "\t}" << endl;
}

void Function::printFunction() {
    func_info.printFunctionInfo();
    main_scope->printScope();
}

void LibFunction::printFunction() {
    libfunc_info.printFunctionInfo();
}

CompUnit::CompUnit() {
// Global  symtable Init Part
    global_scope = new Scope;
    global_scope->local_table = new VariableTable;
    global_scope->elements = new vector<Info *>;
// Global  Function Init Part
    functions.empty();
// Library Funtions Init Part
    string func_name[11] = { "getint"   , "getch"    , "getfloat", "getarray",
                             "getfarray", "putint"   , "putch"   , "putfloat", 
                             "putarray" , "putfarray", "putf" };
    DeclType ret_type[11] = {  TypeInt, TypeInt, TypeFloat, TypeInt, 
                               TypeInt, TypeVoid, TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid, TypeVoid };
    for (int i = 0; i < 11; ++i) {
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
    lib_functions[3].libfunc_info.func_args.push_back(VarType(false, true,  true, TypeInt));
    // getfarray
    lib_functions[4].libfunc_info.func_args.push_back(VarType(false, true,  true, TypeFloat));
    // putint
    lib_functions[5].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    // putch
    lib_functions[6].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    // putfloat
    lib_functions[7].libfunc_info.func_args.push_back(VarType(false, false, true, TypeFloat));
    // putarray
    lib_functions[8].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    lib_functions[8].libfunc_info.func_args.push_back(VarType(false, true,  true, TypeInt));
    // putfarray
    lib_functions[9].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    lib_functions[9].libfunc_info.func_args.push_back(VarType(false, true,  true, TypeFloat));
}

void CompUnit::DebugLibFuncs() {
    cout << "Init Lib Functions" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << '\t';
        lib_functions[i].printFunction();
    }
}

void CompUnit::DebugUserFuncs() {
    cout << "User Functions" << endl;
    int size = functions.size();
    for (int i = 0; i < size; ++i) {
        cout << '\t';
        functions[i]->printFunction();
    }
}

void CompUnit::DebugGlobalTable() {
    cout << "Global Variable" << endl;
    global_scope->local_table->printVaribaleTable();
}

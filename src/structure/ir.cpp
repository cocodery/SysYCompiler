#include "ir.hh"

CompUnit::CompUnit() {
    string func_name[11] = { "getint", "getch", "getfloat", "getarray",
                             "getfarray", "putint", "putch", "putfloat", 
                             "putarray", "putfarray", "putf" };
    DeclType ret_type[11] = { TypeInt, TypeInt, TypeFloat, TypeInt, 
                               TypeInt, TypeVoid, TypeVoid, TypeVoid,
                               TypeVoid, TypeVoid, TypeVoid };
    for (int i = 0; i < 11; ++i) {
        lib_functions[i].is_used = false;
        lib_functions[i].libfunc_info.func_name = func_name[i];
        lib_functions[i].libfunc_info.return_type = ret_type[i];
    }
    // getint
    lib_functions[0].libfunc_info.func_args = vector<VarType>(0);
    // getch
    lib_functions[1].libfunc_info.func_args = vector<VarType>(0);
    // getfloat
    lib_functions[2].libfunc_info.func_args = vector<VarType>(0);
    // getarray
    lib_functions[3].libfunc_info.func_args.push_back(VarType(false, true, true, TypeInt));
    // getfarray
    lib_functions[4].libfunc_info.func_args.push_back(VarType(false, true, true, TypeFloat));
    // putint
    lib_functions[5].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    // putch
    lib_functions[6].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    // putfloat
    lib_functions[7].libfunc_info.func_args.push_back(VarType(false, false, true, TypeFloat));
    // putarray
    lib_functions[8].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    lib_functions[8].libfunc_info.func_args.push_back(VarType(false, true, true, TypeInt));
    // putfarray
    lib_functions[9].libfunc_info.func_args.push_back(VarType(false, false, true, TypeInt));
    lib_functions[9].libfunc_info.func_args.push_back(VarType(false, true, true, TypeFloat));
}

void CompUnit::DebugLibFuncs() {
    cout << "Init Lib Functions" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << '\t';
        lib_functions[i].libfunc_info.printFunction();
    }
}
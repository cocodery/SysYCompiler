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
}
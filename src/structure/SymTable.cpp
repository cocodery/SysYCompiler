#include "SymTable.hh"

VarType::VarType() : is_const(false), is_array(false), is_args(false), decl_type(TypeVoid) { }

VarType::VarType(bool _const, bool _arr, bool _args, bool _dims, DeclType _type)
    : is_const(_const), is_array(_arr), array_dims(_dims), decl_type(_type) { }

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

Variable::Variable(int vi, VarType vt) : var_idx(vi), type(vt) { }

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

FunctionInfo::FunctionInfo(string _name, DeclType _type, vector<VarType> _args)
    : func_name(_name), return_type(_type), func_args(_args) { }
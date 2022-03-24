#include "SymTable.hh"

VarType::VarType() : is_const(false), is_array(false), decl_type(TypeVoid) { }

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

Variable::Variable(int vi, VarType vt, InitValue iv) : 
    var_idx(vi), type(vt), value(iv) { }

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

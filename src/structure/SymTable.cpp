#include "SymTable.hh"

VarType::VarType() : is_const(false), is_array(false), decl_type(TypeVoid) { }

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

Variable::Variable(int vi, VarType vt) : var_idx(vi), type(vt) { }

FunctionInfo::FunctionInfo() : return_type(TypeVoid) { }

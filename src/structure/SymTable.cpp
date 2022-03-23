#include "SymTable.hh"

VarType::VarType() : is_const(false), is_array(false) { 
    array_dims.reserve(0);
}

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

Variable::Variable(int vi, VarType vt) : var_idx(vi), type(vt) { }

VariableTable::VariableTable() {
    var_table.reserve(0);
}

FunctionInfo::FunctionInfo() : return_type(false) {
    func_args_type.reserve(0);
}

FunctionTable::FunctionTable() {
    func_table.reserve(0);
}

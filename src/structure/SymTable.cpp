#include "SymTable.h"

VarType::VarType() : is_const(false), is_array(false), is_func_args(false) { 
    array_dims.reserve(0);
}

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

InitValue::InitValue() : is_const(false), is_array(false), scalar_init_value(0) {
    list_init_value.reserve(0);
}

Variable::Variable(int vi, VarType vt, InitValue iv) : var_idx(vi), type(vt), init(iv) { }

VariableTable::VariableTable() {
    var_table.reserve(0);
}

FunctionInfo::FunctionInfo() : return_type(false) {
    func_args_type.reserve(0);
}

FunctionTable::FunctionTable() {
    func_table.reserve(0);
}

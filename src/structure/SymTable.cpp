#include "SymTable.h"

VarType::VarType() : is_const(false), is_array(false), is_func_args(false) { 
    array_dims = vector<int32_t>(0);
}

int32_t VarType::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;
}

InitValue::InitValue() : is_const(false), is_array(false), scalar_init_value(0) {
    list_init_value = vector<int32_t>(0);
}

Variable::Variable(VarType vt, InitValue iv) : type(vt), init(iv) { }

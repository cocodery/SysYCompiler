#include "SymTable.h"

Type::Type() : is_const(false), is_func_args(false) { }

int32_t Type::elements_number() {
    int32_t number = 1;
    for (auto i : array_dims) {
        number *= i;
    }
    return number;    
}

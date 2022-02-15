#pragma once

#include <vector>
#include <string>

#include "../common.h"

using std::vector;

class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_func_args;
    vector<int32_t> array_dims;

    VarType();
    int32_t elements_number();
};

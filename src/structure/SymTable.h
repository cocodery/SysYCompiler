#pragma once

#include <vector>
#include <string>

#include "../common.h"

using std::vector;

class Type {
public:
    bool is_variable;
    bool is_const;
    bool is_func_args;
    vector<int32_t> array_dims;

    Type();
    int32_t elements_number();
};

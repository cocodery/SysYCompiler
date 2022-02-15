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

class InitValue {
public:
    bool is_const;
    bool is_array;
    int32_t scalar_init_value;
    vector<int32_t> list_init_value;

    InitValue();
};

class Variable {
public:
    VarType type;
    InitValue init;

    Variable(VarType vt, InitValue iv);
};

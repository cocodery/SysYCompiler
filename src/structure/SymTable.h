#pragma once

#include <vector>
#include <string>
#include <map>

#include "../common.h"

using std::vector;
using std::string;
using std::map;

class VarTypee;
class InitValue;
class Variable;
class FunctionInfo;
class FunctionTable;

using VarMap = map<string, Variable>;

using FuncMap = map<string, FunctionInfo>;

class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_func_args; // is_array && is_func_args will omit first dim
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
    int var_idx;
    VarType type;
    InitValue init;

    Variable() : var_idx(-1) { };
    Variable(int vi, VarType vt, InitValue iv);
};

class VariableTable {
public:
    vector<VarMap> var_table;

    VariableTable();
};

class FunctionInfo {
public:
    bool return_type; // 0 -> void, 1 -> int
    vector<VarType> func_args_type;

    FunctionInfo();
};

class FunctionTable {
public:
    vector<FuncMap> func_table;

    FunctionTable();
};

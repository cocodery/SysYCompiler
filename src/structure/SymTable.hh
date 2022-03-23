#pragma once

#include <vector>
#include <string>
#include <map>

#include "../common.hh"

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
public:
    VarType();
    int32_t elements_number();
};

class InitValue {
public:
    bool is_const;
    bool is_array;
    SysYType scalar_init_value;
    vector<SysYType> list_init_value;
public:
    InitValue();
};

class Variable {
public:
    int var_idx;
    VarType type;
    InitValue init;
public:
    Variable() : var_idx(-1) { };
    Variable(int vi, VarType vt, InitValue iv);
};

class VariableTable {
public:
    vector<VarMap> var_table;
public:
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
public:
    FunctionTable();
};

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
    vector<int32_t> array_dims;
    DeclType decl_type;
    SysYInit init_value;
public:
    VarType();
    int32_t elements_number();
};

class Variable {
public:
    int var_idx;
    VarType type;
public:
    Variable() : var_idx(-1) { };
    Variable(int vi, VarType vt);
};

class VariableTable {
public:
    vector<VarMap> var_table;
public:
    VariableTable();
};

class FunctionInfo {
public:
    SysYType return_type;
    vector<VarType> func_args_type;

    FunctionInfo();
};

class FunctionTable {
public:
    vector<FuncMap> func_table;
public:
    FunctionTable();
};

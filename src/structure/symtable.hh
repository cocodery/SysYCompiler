#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../common.hh"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;

class VarType;
class Variable;
class FunctionInfo;

using VarPair = pair<string, Variable *>;

static int32_t var_index = 0;

class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_args;
    vector<int32_t> array_dims;
    DeclType decl_type;
public:
    VarType();
    VarType(bool _const, bool _arr, bool _args, DeclType _type);
    int32_t elements_number();
    int32_t get_index(vector<int32_t>& arr_idx);
    void printVarTypeForArg();
};

class Variable {
public:
    int32_t var_idx;
    VarType type;
    int32_t int_scalar;
    float float_scalar;
    vector<int32_t> int_list;
    vector<float> float_list;
public:
    void printVariable(string var_name);
    Variable() : var_idx(var_index++) { };
    Variable(VarType vt) : var_idx(var_index++), type(vt) { }
    Variable(int vi, VarType vt) : var_idx(var_index++), type(vt) { }
};

class VariableTable {
public:
    vector<VarPair> var_table;
public:
    VariableTable() { };
    bool findInCurTable(string name);
    Variable *getInCurTable(string name);
    void printVaribaleTable();
};

class FunctionInfo {
public:
    string func_name;
    DeclType return_type;
    vector<VarType> func_args;
public:
    FunctionInfo();
    FunctionInfo(string _name, DeclType _type, vector<VarType> _args);
    bool has_args();
    void printFunctionInfo();
};

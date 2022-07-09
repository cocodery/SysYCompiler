#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "../common.hh"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;
using std::make_pair;

class VarType;
class Variable;
class FunctionInfo;

using VarPair = pair<string, Variable *>;

class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_args;
    vector<int32_t> array_dims;
    DeclType decl_type;
public:
    VarType();
    VarType(DeclType _type);
    VarType(bool _const, bool _arr, bool _args, DeclType _type);
    int32_t elements_number();
    int32_t get_index(vector<int32_t>& arr_idx);
    vector<int32_t> get_dims();
    VarType move_down();
    string printVarTypeForAlc();
    string printVarTypeForArg();
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
    Variable(int vi) : var_idx(vi) { }
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
    vector<pair<string, VarType>> func_args;
public:
    FunctionInfo();
    FunctionInfo(string _name, DeclType _type, vector<pair<string, VarType>> _args);
    bool has_args();
    pair<int32_t, VarType> findInFuncArgs(string var_name);
    string printFunctionInfo(bool islib = false);
};

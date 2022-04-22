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

class VarTypee;
class InitValue;
class Variable;
class FunctionInfo;

using VarPair = pair<string, Variable>;

class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_args;
    bool is_init;
    vector<int32_t> array_dims;
    DeclType decl_type;
    int32_t int_scalar;
    float float_scalar;
    vector<int32_t> int_list;
    vector<float> float_list;
public:
    VarType();
    VarType(bool _const, bool _arr, bool _args, DeclType _type);
    int32_t elements_number();
    string printVarTypeForArg();
};

class Variable {
public:
    int var_idx;
    VarType type;
public:
    void printVariable();
    Variable() : var_idx(-1) { };
    Variable(VarType vt) : var_idx(-1), type(vt) { }
    Variable(int vi, VarType vt) : var_idx(vi), type(vt) { }
};

class VariableTable {
public:
    vector<VarPair> var_table;
public:
    VariableTable() { };
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

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../common.hh"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;

class VarTypee;
class InitValue;
class Variable;
class FunctionInfo;

using VarMap = map<string, Variable>;

class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_args;
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
    string printVarType();
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
    void printFunction();
};

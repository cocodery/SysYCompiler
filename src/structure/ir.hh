#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "inst.hh"
#include "baseclass.hh"

using std::ostream;
using std::vector;
using std::variant;
using std::string;
using std::map;

class BasicBlock: public Info {
public:
    vector<Inst *> basic_block; 
public:
    BasicBlock() { }
    void printBlock();
};

class Scope: public Info {
public:
    VariableTable  *local_table;
    vector<Info *> *elements; // `Info` -> `Scope` or `BasicBlock`
    Scope* parent;
public:
    Scope() { local_table = nullptr; elements = nullptr; parent = nullptr; }
    Variable *resolve(string var_name);
    void printElements();
    void printScope();
};

class Function {
public:
    FunctionInfo func_info;
    Scope *main_scope;
public:
    void printFunction();
};

class LibFunction {
public:
    FunctionInfo libfunc_info;
    bool is_used;
public:
    void printFunction();
};

class CompUnit: public Info {
public:
    Scope *global_scope;
    vector<Function *> functions;
    LibFunction lib_functions[11];
public:
    CompUnit();
    void DebugLibFuncs();
    void DebugUserFuncs();
    void DebugGlobalTable();
};

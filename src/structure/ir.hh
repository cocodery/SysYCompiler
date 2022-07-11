#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "llir.hh"
#include "baseclass.hh"

using std::ostream;
using std::vector;
using std::string;
using std::map;
using std::ofstream;

static ofstream llir;

class BasicBlock: public Info {
public:
    int32_t bb_idx;
    vector<Inst *> basic_block;
    vector<BasicBlock *> parants;
    vector<BasicBlock *> childrens; 
public:
    BasicBlock(int32_t _idx) : bb_idx(_idx) { }
    void printBlock();
};

class Scope: public Info {
public:
    int32_t sp_idx;
    VariableTable  *local_table;
    vector<Info *> *elements; // `Info` -> `Scope` or `BasicBlock`
    Scope* parent;
public:
    Scope(int32_t _idx) : sp_idx(_idx) { local_table = nullptr; elements = nullptr; parent = nullptr; }
    SRC resolve(string var_name, FunctionInfo *cur_func_info);
    BasicBlock *get_last_bb();
    void printElements();
    void printScope();
};

class Function {
public:
    FunctionInfo func_info;
    Scope *main_scope;
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
    LibFunction lib_functions[12];
public:
    CompUnit(string _llir);
    void moveGlobalInitToMain();
    bool inLibFunctions(string func_name);
    FunctionInfo *getFunctionInfo(string func_name);
    void DebugLibFuncs();
    void DebugUserFuncs();
    void DebugGlobalTable();
};

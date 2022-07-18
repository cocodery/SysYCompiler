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
using std::make_pair;

static ofstream llir;

class BasicBlock: public Info {
public:
    int32_t bb_idx;
    vector<Inst *> basic_block;
    vector<BasicBlock *> parants;
    vector<BasicBlock *> childrens; 
public:
    BasicBlock(int32_t _idx) : bb_idx(_idx) { }
    Inst *lastInst();
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
    void buildScopeCFG(vector<BasicBlock *> all_blocks);
    void printElements();
    void printScope();
};

class Function {
public:
    FunctionInfo func_info;
    Scope *main_scope;
    vector<BasicBlock *> all_blocks;
    vector<FunctionInfo *> called_funcs;
public:
    void printCallInfo();
    void buildCFG();
};

class LibFunction {
public:
    FunctionInfo libfunc_info;
public:
    void printFunction();
};

class CompUnit: public Info {
public:
    Scope *global_scope;
    vector<Function *> functions;
    LibFunction *lib_functions[13];
public:
    CompUnit();
    FunctionInfo *getFunctionInfo(string func_name);
    void GenerateLLIR(string _llir);
    void DebugLibFuncs();
    void DebugUserFuncs();
    void DebugGlobalTable();
};

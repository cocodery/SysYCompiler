#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <string>
#include <set>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "llir.hh"
#include "baseclass.hh"

using std::ostream;
using std::vector;
using std::string;
using std::set;
using std::set_union;
using std::set_intersection;
using std::inserter;
using std::map;
using std::ofstream;
using std::make_pair;

static ofstream llir;

class BasicBlock: public Info {
public:
    int32_t bb_idx;
    bool valuable;
    vector<Inst *> basic_block;
    map<int32_t, BasicBlock *> preds;
    map<int32_t, BasicBlock *> succs; 
    set<BasicBlock *> dom; // 支配节点集; 必经节点
    BasicBlock *idom; // 直接支配节点
    set<BasicBlock *> domers; // 支配者节点集 
    set<BasicBlock *> DomFrontier;
    set<int32_t> LiveUse, LiveDef, LiveIn, LiveOut, tmpLiveOut;
public:
    BasicBlock(int32_t _idx, bool _value = false) : bb_idx(_idx), valuable(_value), idom(nullptr) { }
    Inst *lastInst();
    void printBlock();
    void initDom(vector<BasicBlock *> all_blocks);
    void initIDom(BasicBlock *entrybb);
    set<BasicBlock *> predsDomInter();
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
    int32_t var_idx;
    Scope *main_scope;
    vector<BasicBlock *> all_blocks;
    set<FunctionInfo *> called_funcs;
public:
    void printCallInfo();
    void buildCFG();
    void buildDom();
    void buildIDom();
    void initBBDF();
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

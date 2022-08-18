#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

using FuncMap = map<string, Function *>;

class FuncInline {
public:
    Function *function;

    set<FunctionInfo *> vis;
    map<pair<int32_t, bool>, SRC> inlineMap;
public:
    FuncInline(Function *func) : function(func) { }

    bool isRecursive(FunctionInfo *, FunctionInfo *);
    bool sideEffect();

    bool inLinable(string func_name, FuncMap &func_map);
    void initInlineMap(vector<SRC> caller_args, vector<pair<string, VarType>> callee_args);
    SRC findInMap(SRC src);
    list<Inst *> insertBlock(BasicBlock *block, SRC &dst);
    void simpleInline(BasicBlock *block, Function *func);
    void excuteFuncInline(BasicBlock *block, vector<BasicBlock *> &all_block, Function *func);
    void runFuncInline(FuncMap &func_map, VariableTable  *local_table);
};

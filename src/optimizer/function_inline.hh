#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

using FuncMap = map<string, Function *>;

class FuncInline {
public:
    Function *function;

    map<pair<int32_t, bool>, SRC> inlineMap;
    map<int32_t, int32_t> bbIdxMap;
public:
    FuncInline(Function *func) : function(func) { }

    static bool isRecursive(FunctionInfo *, FunctionInfo *, set<FunctionInfo *> &);
    static bool sideEffect(Function *);
    int get_depth(Scope *now_scope, int now_depth);

    bool inLinable(string func_name, FuncMap &func_map);
    void initInlineMap(vector<SRC> caller_args, vector<pair<string, VarType>> callee_args);
    SRC findInMap(SRC src);
    list<Inst *> insertBlock(BasicBlock *block, SRC &dst);
    void simpleInline(BasicBlock *block, Function *callee_func);
    list<BasicBlock *> genInlineBlocks(Function *callee_func, SRC &dst);
    void ctrlflowInline(BasicBlock *block, vector<BasicBlock *> &all_block, Function *callee_func);
    void excuteFuncInline(BasicBlock *block, vector<BasicBlock *> &all_block, Function *func);
    void runFuncInline(FuncMap &func_map);
};

#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class FuncInline {
public:
    Function *function;
    vector<Function *> functions;

    set<FunctionInfo *> vis;
public:
    FuncInline(Function *func) : function(func) { }

    bool isRecursive(FunctionInfo *, FunctionInfo *);
    bool multiReturn();
    bool sideEffect();

    void simpleInline();
    void runFuncInline(vector<Function *> funcs);
};

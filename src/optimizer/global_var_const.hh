#pragma once

#include "common.hh"
#include "ir.hh"

class GlobalVarConst
{
public:
    Scope *global_scope;
    vector<Function *> &functions;
public:
    GlobalVarConst(Scope *glb_scope, vector<Function *> &funcs) : global_scope(glb_scope), functions(funcs) { }
    void runGlobalVarConst();
};

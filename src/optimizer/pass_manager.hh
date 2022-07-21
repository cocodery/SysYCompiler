#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class PassManager {
public:
    Scope *global_scope;
    vector<Function *> functions;
public:
    PassManager(Scope *glb_scope, vector<Function *> funcs) : global_scope(glb_scope), functions(funcs) { }
    void excute_pass() {
        buildCFGs();
        buildDoms();
        buildIDoms();
    }
    void buildCFGs();
    void buildDoms();
    void buildIDoms();
};

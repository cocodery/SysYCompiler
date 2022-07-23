#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include "mem2reg.hh"

class PassManager {
public:
    Scope *global_scope;
    vector<Function *> functions;
public:
    PassManager(Scope *glb_scope, vector<Function *> funcs) : global_scope(glb_scope), functions(funcs) { }
    void excute_pass() {
        for (auto &&function : functions) {
            if (function->func_info.is_used){
                function->buildCFG();
            }
        }

        // remove one-inst block

        compDomInfo();

        mem2reg();
    }
    void compDomInfo();

    void mem2reg();
};

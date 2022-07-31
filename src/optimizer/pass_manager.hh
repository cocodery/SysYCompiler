#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include "mem2reg.hh"
#include "lvn.hh"

class PassManager {
public:
    Scope *global_scope;
    vector<Function *> functions;
public:
    PassManager(Scope *glb_scope, vector<Function *> funcs) : global_scope(glb_scope), functions(funcs) { }
    void excute_pass() {
        compDomInfo();

        lvn();

        mem2reg();

        lvn();
    }
    void compDomInfo();

    void lvn();

    void mem2reg();

    void lvn();
};

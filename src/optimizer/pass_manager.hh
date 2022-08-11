#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include "mem2reg.hh"
#include "constant_propagation.hh"
#include "lvn.hh"
#include "reg2mem.hh"
#include "function_recursion.hh"
#include "load_store_reordering.hh"

class PassManager {
public:
    Scope *global_scope;
    vector<Function *> functions;
public:
    PassManager(Scope *glb_scope, vector<Function *> funcs) : global_scope(glb_scope), functions(funcs) { }
    void excute_pass() {
        for (auto &&function : functions) {
            if (function->func_info.is_used) {
                function->buildDom();
                function->buildIDom();
                function->initBBDF();

                Mem2Reg mem2reg = Mem2Reg(function);
                mem2reg.runMem2Reg();

                ConstantProg constantprog = ConstantProg(function);
                constantprog.runConstantProg();

                LVN lvn = LVN(function);
                //lvn.runLVN();

                Reg2Mem reg2mem = Reg2Mem(function, mem2reg);
                reg2mem.runReg2Mem();

                FunctionRecursion function_recurtion(function);

                LoadStoreReordering load_store_reordering(function);
            }
        }
    }
};

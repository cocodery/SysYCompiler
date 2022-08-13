#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include "function_inline.hh"
#include "mem2reg.hh"
#include "memaccess_opt.hh"
#include "constant_propagation.hh"
#include "lvn.hh"
#include "dead_code_elim.hh"
#include "reg2mem.hh"
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

                FuncInline funcinline = FuncInline(function);

                Mem2Reg mem2reg = Mem2Reg(function);
                mem2reg.runMem2Reg();

                LVN lvn1 = LVN(function);
                lvn1.runLVN();

                ConstantProg constantprog = ConstantProg(function);
                constantprog.runConstantProp();

                LVN lvn2 = LVN(function);
                lvn2.runLVN();

                MemAccessOpt mao = MemAccessOpt(function);
                mao.runMemAccessOpt();

                Dce dce = Dce(function);
                dce.runDeadCodeElim();

                Reg2Mem reg2mem = Reg2Mem(function, mem2reg);
                reg2mem.runReg2Mem();

                LoadStoreReordering load_store_reordering(function);
            }
        }
    }
};

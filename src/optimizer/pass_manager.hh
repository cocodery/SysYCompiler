#pragma once

#include "../common.hh"
#include "../structure/ir.hh"
#include "function_inline.hh"
#include "mem2reg.hh"
#include "memaccess_opt.hh"
#include "constant_propagation.hh"
#include "gvn_gcm.hh"
#include "inst_combine.hh"
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

                for (int32_t idx = 0; idx < 2; ++idx) {
                    Mem2Reg mem2reg = Mem2Reg(function);
                    if (function->func_info.func_name != "long_func") {
                        mem2reg.runMem2Reg();
                    }

                    GvnGcm gvn_gcm1 = GvnGcm(function);
                    gvn_gcm1.runGvnGcm();

                    ConstantProg constantprog1 = ConstantProg(function);
                    constantprog1.runConstantProp();

                    MemAccessOpt mao = MemAccessOpt(function);
                    mao.runMemAccessOpt();

                    InstCombine instcomb = InstCombine(function);
                    instcomb.runInstCombine();

                    ConstantProg constantprog2 = ConstantProg(function);
                    constantprog2.runConstantProp();

                    GvnGcm gvn_gcm2 = GvnGcm(function);
                    gvn_gcm2.runGvnGcm();

                    // // Dce dce = Dce(function);
                    // // dce.runDeadCodeElim();

                    if (function->func_info.func_name != "long_func") {
                        Reg2Mem reg2mem = Reg2Mem(function, mem2reg);
                        reg2mem.runReg2Mem();
                    }

                    LoadStoreReordering load_store_reordering(function);
                }
            }
        }
    }
};

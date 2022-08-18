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
#include "branch_opt.hh"
#include "global_var_const.hh"

using FuncMap = map<string, Function *>;

class PassManager {
public:
    Scope *global_scope;
    vector<Function *> functions;
public:
    PassManager(Scope *glb_scope, vector<Function *> funcs) : global_scope(glb_scope), functions(funcs) { }
    bool do_not_run_m2r_r2m(Function *funcPtr);
    int get_depth(Scope *now_scope, int now_depth);
    void excute_pass() {
        FuncMap funcMap;
        for (auto &&function : functions) {
            FuncInline funcinline = FuncInline(function);
            function->func_info.is_recursive = funcinline.isRecursive(&function->func_info, &function->func_info);
            function->func_info.side_effect  = funcinline.sideEffect();

            funcMap.insert({function->func_info.func_name, function});
        }

        for (int32_t idx = 0; idx < 3; ++idx) {
    /* -------------------- 过程间优化 -------------------- */
            if (idx != 0) { // 如果不是第一次pass，则更新函数的call_count和is_used信息
                // 给所有函数的call计数清零
                for (auto &&function : functions)
                    function->func_info.call_count = 0;
                
                // 给所有函数计算call次数
                for (auto &&function : functions) {
                    if (function->func_info.is_used) {
                        for (auto &&bbPtr : function->all_blocks) {
                            for (auto &&instPtr : bbPtr->basic_block) {
                                Case (LLIR_CALL, call_inst, instPtr) {
                                    ++call_inst->func_info->call_count;
                                }
                            }
                        }
                    }
                }

                // 给所有函数更新is_used信息
                for (auto &&function : functions) {
                    if (function->func_info.is_used && function->func_info.func_name != "main") {
                        if (function->func_info.call_count == 0)
                            function->func_info.is_used = false;
                    }
                }
            }

            GlobalVarConst gvc(global_scope, functions);
            gvc.runGlobalVarConst();

    /* -------------------- 过程内优化 -------------------- */
            for (auto &&function : functions) {
                if (function->func_info.is_used) {
                    function->buildDom();
                    function->buildIDom();
                    function->initBBDF();

                    Mem2Reg mem2reg = Mem2Reg(function);
                    if (!do_not_run_m2r_r2m(function)) {
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

                    // Dce dce = Dce(function);
                    // dce.runDeadCodeElim();

                    if (!do_not_run_m2r_r2m(function)) {
                        Reg2Mem reg2mem = Reg2Mem(function, mem2reg);
                        reg2mem.runReg2Mem();
                    }

                    LoadStoreReordering load_store_reordering(function);
                    
                    BranchOptimization branch_opt = BranchOptimization(function);
                    branch_opt.runBranchOpt(&mem2reg.phi2AllocaMap);

                    FuncInline funcinline = FuncInline(function);

                    funcinline.runFuncInline(funcMap);
                    // funcinline.runFuncInline(functions);
                }
            }
        }
    }
};

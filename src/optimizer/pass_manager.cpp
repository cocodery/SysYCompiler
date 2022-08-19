#include "pass_manager.hh"

int PassManager::get_depth(Scope *now_scope, int now_depth)
{
    int max_depth = now_depth;
    for (auto iter = now_scope->elements->begin(); iter != now_scope->elements->end(); ++iter) {
        if (Scope *scope_node = dynamic_cast<Scope *>(*iter); scope_node != nullptr) {
            max_depth = std::max(max_depth, get_depth(scope_node, now_depth + 1));
        }
    }
    return max_depth;
}

bool PassManager::do_not_run_m2r_r2m(Function *funcPtr)
{
    return (get_depth(funcPtr->main_scope, 1) > 8
         || funcPtr->func_info.func_name == "pseudo_sha1");
}

void PassManager::updateFuncInfo() {
    // 给所有函数的call计数清零，清空所有函数的called_funcs
    for (auto &&function : functions) {
        function->func_info.call_count = 0;
        function->func_info.called_funcs.clear();
    }  
    // 给所有函数计算call次数，并更新它的called_funcs
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            for (auto &&bbPtr : function->all_blocks) {
                for (auto &&instPtr : bbPtr->basic_block) {
                    Case (LLIR_CALL, call_inst, instPtr) {
                        ++call_inst->func_info->call_count;
                        function->func_info.called_funcs.insert(call_inst->func_info);
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
    // 更新所有函数的is_recursive、side_effect信息
    for (auto &&function : functions) {
        if (!function->func_info.is_used) {
            set<FunctionInfo *> vis;
            function->func_info.is_recursive = FuncInline::isRecursive(&function->func_info, &function->func_info, vis);
            function->func_info.side_effect  = FuncInline::sideEffect(function);
        }
    }
}

void PassManager::interOpt() {
    for (auto &&function : functions) {
        if (function->func_info.is_used) {
            function->buildDomInfo();
                    
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
        }
    }
}

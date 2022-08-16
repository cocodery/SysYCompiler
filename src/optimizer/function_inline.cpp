#include "function_inline.hh"

bool FuncInline::isRecursive(FunctionInfo *to_check, FunctionInfo *now) {
    // auto &&now_function = info_to_function.at(now);
    for (auto &&called_func : now->called_funcs) {
        if (called_func == to_check) { // 找到递归调用
            return true;
        }
        if (vis.find(called_func) != vis.end()) { // 如果已经找过了这个函数，跳过
            continue;
        }
        vis.insert(called_func);
        if (isRecursive(to_check, called_func)) { // 没找过这个函数，那就看能不能这里找到循环调用
            return true; // 找到了，返回真
        }
        // 没找到，继续
    }
    return false;
}

bool FuncInline::multiReturn() {
    return (function->getSpecificIdxBb(-1)->preds.size() != 1);
}

bool FuncInline::sideEffect() {
    if (function->func_info.func_name == "main") {
        return true;
    }
    for (auto &&func_info : function->func_info.called_funcs) {
        if (func_info->side_effect == true) {
            return true;
        }
    }
    int32_t args_num = function->func_info.func_args.size();
    set<VirtReg *> globalOrArgPtr;
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        for (auto &&inst : block->basic_block) {
            Case (LLIR_GEP, gep_inst, inst) {
                auto &&src_reg = gep_inst->src.reg;
                auto &&dst_reg = gep_inst->dst.reg;
                if (src_reg->global || src_reg->reg_id < args_num) {
                    globalOrArgPtr.insert(src_reg);
                }
                if (globalOrArgPtr.find(src_reg) != globalOrArgPtr.end()) {
                    globalOrArgPtr.insert(dst_reg);
                }
            } else Case (LLIR_STORE, store_inst, inst) {
                auto &&dst_reg = store_inst->dst.reg;
                if (dst_reg->global || globalOrArgPtr.find(dst_reg) != globalOrArgPtr.end()) {
                    return true;
                }
            }
        }
    }
    return false;
}

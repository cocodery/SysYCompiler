#include "global_var_const.hh"

void GlobalVarConst::runGlobalVarConst()
{
    map<int32_t, SRC> to_replace;
    // 在main函数的bb1中找到可能被常量化的全局变量
    for (auto &&function : functions) {
        if (!function->func_info.is_used || function->func_info.func_name != "main")
            continue;
        // 找到了main函数，对于main函数bb1中的每个对全局变量对赋值进行分析
        set<int32_t> not_to_replace;
        for (auto &&instPtr : function->all_blocks[1]->basic_block) {
            Case (LLIR_STORE, store_inst, instPtr) {
                // 如果是全局变量
                if (store_inst->dst.reg && store_inst->dst.reg->global) {
                    // 如果是第一次找到这个全局变量的store，而且src是ctv，认为它是可能被替换的全局变量
                    if (store_inst->src.ctv && to_replace.find(store_inst->dst.reg->reg_id) == to_replace.end()) {
                        to_replace.insert(make_pair(store_inst->dst.reg->reg_id, store_inst->src));
                    }
                    // 如果不是第一次store，或者src是reg，那么认为这个全局变量不可以被替换
                    else {
                        not_to_replace.insert(store_inst->dst.reg->reg_id);
                    }
                }
            }
        }
        for (auto &&ntr : not_to_replace) {
            to_replace.erase(ntr);
        }
    }
    // 在所有is_used函数中寻找全局变量的store指令，但跳过main的bb1
    for (auto &&function : functions) {
        if (!function->func_info.is_used)
            continue;
        for (auto &&it = function->all_blocks.begin(); it != function->all_blocks.end(); ++it) {
            auto &&bbPtr = *it;
            if (function->func_info.func_name == "main" && bbPtr->bb_idx == 1)
                continue; // 跳过main函数的bb1
            for (auto &&instPtr : bbPtr->basic_block) {
                Case (LLIR_STORE, store_inst, instPtr) {
                    // 在main的bb1之外的地方找到了全局变量的store，认为这个全局变量不可以被替换
                    if (store_inst->dst.reg && store_inst->dst.reg->global) {
                        to_replace.erase(store_inst->dst.reg->reg_id);
                    }
                }
            }
        }
    }
    // 替换
    for (auto &&reg_pair : to_replace) {
        auto &&varIdx = reg_pair.first;
        auto &&constVar = reg_pair.second;
        // 在所有函数中把全局变量替换成ctv
        for (auto &&function : functions) {
            if (!function->func_info.is_used)
                continue;
            for (auto &&bbPtr : function->all_blocks) {
                list<Inst *> bb_list(bbPtr->basic_block.begin(), bbPtr->basic_block.end());
                for (auto &&it = bb_list.begin(); it != bb_list.end();) {
                    Case (LLIR_LOAD, load_inst, *it) {
                        if (load_inst->src.reg && load_inst->src.reg->global && load_inst->src.reg->reg_id == varIdx) {
                            bbPtr->replaceSRC(load_inst->dst.reg, constVar);
                            it = bb_list.erase(it);
                            continue;
                        }
                    }
                    else Case (LLIR_STORE, store_inst, *it) {
                        if (store_inst->dst.reg && store_inst->dst.reg->global && store_inst->dst.reg->reg_id == varIdx) {
                            it = bb_list.erase(it);
                            continue;
                        }
                    }
                    ++it;
                }
                bbPtr->basic_block.assign(bb_list.begin(), bb_list.end());
            }
        }
        // 在符号表中标记为const
        global_scope->local_table->var_table[varIdx - 1].second->type.is_const = true;
    }
}

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

bool FuncInline::inLinable(string func_name, FuncMap &func_map) {
    if (func_map.find(func_name) == func_map.end()) {
        return false;
    }
    auto &&func = func_map[func_name];
    if (func->multiReturn() || func->func_info.is_recursive) {
        return false;
    }
    return true;
}

void FuncInline::initInlineMap(vector<SRC> caller_args, vector<pair<string, VarType>> callee_args) {
    inlineMap.clear();
    for (int32_t idx = 0; idx < caller_args.size(); ++idx) {
        inlineMap.insert({{idx, false}, caller_args[idx]});
    }
}

SRC FuncInline::findInMap(SRC src) {
    if (src.ctv || src.reg->global) { 
        return src;
    } else if (inlineMap.find({src.reg->reg_id, src.reg->global}) != inlineMap.end()) {
        return inlineMap[{src.reg->reg_id, src.reg->global}];
    } else {
        SRC new_src = SRC(new VirtReg(function->var_idx++, src.reg->type, src.reg->global));
        inlineMap.insert({{src.reg->reg_id, src.reg->global}, new_src});
        return new_src;
    }

}

list<Inst *> FuncInline::insertBlock(BasicBlock *block, SRC &dst) {
    list<Inst *> insertbb;
    for (auto &&inst : block->basic_block) {
        Case (LLIR_RET, ret_inst, inst) {
            if (ret_inst->has_retvalue) {
                auto &&ret_value = findInMap(ret_inst->ret_value);
                dst = ret_value;
            }
        } 
        Case (LLIR_BIN, bin_inst, inst) {
            auto &&src1 = findInMap(bin_inst->src1);
            auto &&src2 = findInMap(bin_inst->src2);
            auto &&dst  = findInMap(bin_inst->dst);
            LLIR_BIN *inst_bin = new LLIR_BIN(bin_inst->op, dst, src1 ,src2);
            insertbb.push_back(inst_bin);
        } 
        Case (LLIR_FBIN, fbin_inst, inst) {
            auto &&src1 = findInMap(fbin_inst->src1);
            auto &&src2 = findInMap(fbin_inst->src2);
            auto &&dst  = findInMap(fbin_inst->dst);
            LLIR_FBIN *inst_fbin = new LLIR_FBIN(fbin_inst->op, dst, src1 ,src2);
            insertbb.push_back(inst_fbin);
        } 
        Case (LLIR_ICMP, icmp_inst, inst) {
            auto &&src1 = findInMap(icmp_inst->src1);
            auto &&src2 = findInMap(icmp_inst->src2);
            auto &&dst  = findInMap(icmp_inst->dst);
            LLIR_ICMP *inst_icmp = new LLIR_ICMP(icmp_inst->op, dst, src1 ,src2);
            insertbb.push_back(inst_icmp);
        } 
        Case (LLIR_FCMP, fcmp_inst, inst) {
            auto &&src1 = findInMap(fcmp_inst->src1);
            auto &&src2 = findInMap(fcmp_inst->src2);
            auto &&dst  = findInMap(fcmp_inst->dst);
            LLIR_FCMP *inst_fcmp = new LLIR_FCMP(fcmp_inst->op, dst, src1 ,src2);
            insertbb.push_back(inst_fcmp);
        } 
         Case (LLIR_CALL, call_inst, inst) {
            auto &&dst = findInMap(call_inst->dst);
            vector<SRC> args;
            for (auto &&arg : call_inst->args) {
                args.push_back(findInMap(arg));
            }
            LLIR_CALL *inst_call= new LLIR_CALL(dst, args, call_inst->func_info);
            insertbb.push_back(inst_call);
        } 
        Case (LLIR_ZEXT, zext_inst, inst) {
            auto &&src = findInMap(zext_inst->src);
            auto &&dst = findInMap(zext_inst->dst);
            LLIR_ZEXT *inst_zext = new LLIR_ZEXT(dst, src);
            insertbb.push_back(inst_zext);         
        } 
        Case (LLIR_GEP, gep_inst, inst) {
            auto &&src1 = findInMap(gep_inst->src);
            auto &&src2 = findInMap(gep_inst->off);
            auto &&dst  = findInMap(gep_inst->dst);
            LLIR_GEP *inst_gep = new LLIR_GEP(dst, src1 ,src2, dst.reg->type);
            insertbb.push_back(inst_gep);
        } 
        Case (LLIR_XOR, xor_inst, inst) {
            auto &&src = findInMap(xor_inst->src);
            auto &&dst = findInMap(xor_inst->dst);
            LLIR_XOR *inst_xor = new LLIR_XOR(dst, src);
            insertbb.push_back(inst_xor); 
        } 
        Case (LLIR_SITOFP, itf_inst, inst) {
            auto &&src = findInMap(itf_inst->src);
            auto &&dst = findInMap(itf_inst->dst);
            LLIR_SITOFP *inst_itf = new LLIR_SITOFP(dst, src);
            insertbb.push_back(inst_itf); 
        } 
        Case (LLIR_FPTOSI, fti_inst, inst) {
            auto &&src = findInMap(fti_inst->src);
            auto &&dst = findInMap(fti_inst->dst);
            LLIR_FPTOSI *inst_fti = new LLIR_FPTOSI(dst, src);
            insertbb.push_back(inst_fti); 
        } 
        Case (LLIR_STORE, store_inst, inst) {
            auto &&src = findInMap(store_inst->src);
            auto &&dst = findInMap(store_inst->dst);
            LLIR_STORE *inst_store = new LLIR_STORE(dst, src);
            insertbb.push_back(inst_store);
        } 
        Case (LLIR_LOAD, load_inst, inst) {
            auto &&src = findInMap(load_inst->src);
            auto &&dst = findInMap(load_inst->dst);
            LLIR_LOAD *inst_load= new LLIR_LOAD(dst, src);
            insertbb.push_back(inst_load); 
        } 
    }
    return insertbb;
}

void FuncInline::simpleInline(BasicBlock *block, Function *callee_func) {
    auto &&callee_func_info = callee_func->func_info;
    
    list<Inst *> bb_list(block->basic_block.begin(), block->basic_block.end());

    for (auto &&iter = bb_list.begin(); iter != bb_list.end(); ++iter) {
        auto &&inst = *iter;
        Case (LLIR_CALL, call_inst, inst) {
            if (call_inst->func_info->func_name != callee_func->func_info.func_name) {
                continue;
            }
            // erase this call-inst
            iter = bb_list.erase(iter); 
            auto &&new_call_dst = SRC();
            list<Inst *> insertbb = insertBlock(callee_func->all_blocks[1], new_call_dst);
            function->replaceSRCs(block, call_inst->dst.reg, new_call_dst);
            
            for (auto &&inst : insertbb) {
                bb_list.insert(iter, inst);
            }
        } 
    }
    block->basic_block = vector<Inst *>(bb_list.begin(), bb_list.end());
} 

void FuncInline::excuteFuncInline(BasicBlock *block, vector<BasicBlock *> &all_block, Function *func) {
    if (func->all_blocks.size() == 3) {
        simpleInline(block, func);
    } 
}

void FuncInline::runFuncInline(FuncMap &func_map, VariableTable *glb_table) {
    if (function->func_info.called_funcs.size() == 0) return;
    auto &&all_blocks = function->all_blocks;
    for (auto &&block : all_blocks) {
        bool changed = true;
        while (changed) {
            for (auto &&inst : block->basic_block) {
                Case (LLIR_CALL, call_inst, inst) {
                    string callee_name = call_inst->func_info->func_name;
                    if (inLinable(callee_name, func_map)) {
                        auto &&inlined_func = func_map[callee_name];
                        // init caller-args --> callee-args
                        initInlineMap(call_inst->args, inlined_func->func_info.func_args);
                        excuteFuncInline(block, all_blocks, inlined_func);
                        changed = true;
                        break;
                    }
                }
            }
            changed = false;
        }
    }
}

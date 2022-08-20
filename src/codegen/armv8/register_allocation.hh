#ifndef REGISTER_ALLOCATION_H
#define REGISTER_ALLOCATION_H

// int: r0 to r11 are generally usable

// r12: 存储str指令的立即数
// 全局变量都是地址

// float: s0 tp s31 are generally usable

#include "common.hh"
#include "ir.hh"

bool InstNeedsSrcPreserved(Inst* instPtr)
{
    Case (LLIR_FBIN, fbin_inst, instPtr) {
        return true;
    }
    Case (LLIR_BIN, bin_inst, instPtr) {
        return true;
    }
    Case (LLIR_LOAD, load_inst, instPtr) {
        return true;
    }
    Case (LLIR_STORE, store_inst, instPtr) {
        return true;
    }
    Case (LLIR_GEP, gep_inst, instPtr) {
        if (gep_inst->off.ctv && gep_inst->off.ctv->int_value == 0) return false;
        return gep_inst->src.reg->is_from_gep || gep_inst->src.reg->type.is_args && gep_inst->src.reg->type.is_array;
    }
    return false;
}

void AllocateRegistersForFunction(Function &func)
{
    // TODO : SPILL
    printf("In Function \"%s\":\n", func.func_info.func_name.c_str());

    // 初始化all_insts
    for (auto &&bbPtr : func.all_blocks)
        for (auto &&instPtr : bbPtr->basic_block)
            func.all_insts.push_back(instPtr);

    CLAIM_AVAIL_REGS
    list<int32_t> activeIntervals;
    
    // 给参数0到31分配寄存器
    printf(" - pre allocation\n");
    for (int i = 0; i <= 31; ++i)
    {
        // 不是参数或者这个参数没有被使用，跳过分配
        if (i >= func.func_info.func_args.size())
            break;
        if (func.LiveInterval.find(i) == func.LiveInterval.end())
            continue;
        REGs allocated_register = (i > 3) ? REGs(i + 16) : REGs(i);
        func.AllocationResult.insert(make_pair(i, allocated_register));
        availRegs.erase(allocated_register);
        activeIntervals.push_back(i);
        printf(FOUR_SPACES "var: %d, reg %d\n", i, allocated_register);
    }
    printf(" ----\n");

    vector<pair<int, Function::RANGE>> sorted_liveinterval(func.LiveInterval.begin(), func.LiveInterval.end());
    sort(sorted_liveinterval.begin(), sorted_liveinterval.end(), 
        [](const pair<int, Function::RANGE> &a, const pair<int, Function::RANGE> &b) -> bool { 
            return a.second.first < b.second.first;
        });

    for (auto &&interval : sorted_liveinterval)
    {
        auto &&varIndex = interval.first;
        auto &&varRange = interval.second;

        // 该变量已分配，跳过（r0-r3, s4-s31传参寄存器）
        if (func.AllocationResult.find(varIndex) != func.AllocationResult.end())
            continue;

        // 删除旧变量的分配
        for (auto &&activeIntervalIndexIt = activeIntervals.begin();
            activeIntervalIndexIt != activeIntervals.end();)
        {
            auto &&rangeEnd = func.LiveInterval.at(*activeIntervalIndexIt).second;
            bool src_needed = InstNeedsSrcPreserved(func.all_insts[rangeEnd]);
            if (src_needed && rangeEnd < varRange.first || !src_needed && rangeEnd <= varRange.first)
            {
                availRegs.insert(func.AllocationResult.at(*activeIntervalIndexIt));
                activeIntervalIndexIt = activeIntervals.erase(activeIntervalIndexIt);
            }
            else
                ++activeIntervalIndexIt;
        }
        
        // 给新变量分配寄存器或溢出
        for (auto &&activeIntervalIndexIt = activeIntervals.begin();
            ; ++activeIntervalIndexIt)
        {
            // 找到插入位置
            if (activeIntervalIndexIt == activeIntervals.end()
             || func.LiveInterval.at(*activeIntervalIndexIt).second > varRange.second)
            {
                activeIntervals.insert(activeIntervalIndexIt, varIndex);
                // 无可用寄存器，把活跃区间右侧最靠后的变量溢出，腾出寄存器
                if (availRegs.empty())
                {
                    auto &&spilled_var_idx = activeIntervals.back();
                    printf(FOUR_SPACES RED "var %d spilled!\n" RESET, spilled_var_idx);
                    if (varIndex != spilled_var_idx)
                    {
                        availRegs.insert(func.AllocationResult.at(spilled_var_idx));
                        func.AllocationResult.at(spilled_var_idx) = SPILL;
                    }
                    else
                        func.AllocationResult.insert(make_pair(spilled_var_idx, SPILL));
                    activeIntervals.pop_back();
                }
                // 分配可用寄存器
                auto &&alloc_res = *availRegs.begin();
                func.AllocationResult.insert(make_pair(varIndex, alloc_res));
                availRegs.erase(alloc_res);
                printf(FOUR_SPACES "var: %d, reg %d\n", varIndex, alloc_res);
                break;
            }
        }
    }

    // 给每个指令更新“在此处空闲的寄存器”信息
    for (auto &&interval : func.LiveInterval)
    {
        auto &&varIndex = interval.first;
        auto &&varRange = interval.second;
        auto &&rangeBegin = varRange.first;
        auto &&rangeEnd = varRange.second;
        auto &&allocRes = func.AllocationResult.find(varIndex);

        // 该变量未分配寄存器，即“溢出”
        if (allocRes == func.AllocationResult.end())
            continue;
        else // 该变量被分配了寄存器，给它活跃区间内所有指令标记上“寄存器不可用”
        {
            auto &&allocatedRegister = allocRes->second;
            for (auto &&i = rangeBegin; i <= rangeEnd; ++i)
                func.all_insts[i]->availRegs.erase(allocatedRegister);
        }
    }
}

void AllocateRegistersForAllFunctions(const CompUnit &ir)
{
    printf("\n-----Allocate Registers Start----\n");
    for (auto &&funcPtr : ir.functions)
    {
        // skip unused functions
        if (!funcPtr->func_info.is_used)
            continue;

        AllocateRegistersForFunction(*funcPtr);
    }
    printf("\n-----Allocate Registers End----\n");
}

#endif

#ifndef REGISTER_ALLOCATION_H
#define REGISTER_ALLOCATION_H

// int: r0 to r11 are generally usable

// r12: 存储str指令的立即数
// 全局变量都是地址

// float: s0 tp s31 are generally usable

#include "common.hh"
#include "ir.hh"

void AllocateRegistersForFunction(Function &func)
{
    // TODO : SPILL
    printf("In Function \"%s\":\n", func.func_info.func_name.c_str());
    
    CLAIM_AVAIL_REGS
    //set<REGs> availRegs{r0,  r1};
    std::list<int32_t> activeIntervals;
    
    // 给参数0到3分配寄存器
    printf(" - pre allocation\n");
    for (int i = 0; i <= 3; ++i)
    {
        // 不是参数或者这个参数没有被使用，跳过分配
        if (i >= func.func_info.func_args.size() || 
        func.LiveInterval.find(i) == func.LiveInterval.end())
            continue;
        func.AllocationResult.insert(make_pair(i, (REGs)i));
        availRegs.erase((REGs)i);
        activeIntervals.push_back(i);
        printf(FOUR_SPACES "var: %d, reg %d\n", i, i);
    }
    printf(" ----\n");

    for (auto &&interval : func.LiveInterval)
    {
        auto &&varIndex = interval.first;
        auto &&varRange = interval.second;

        // 该变量已分配，跳过（r0-r3传参寄存器）
        if (func.AllocationResult.find(varIndex) != func.AllocationResult.end())
            continue;

        // 删除旧变量的分配
        for (auto &&activeIntervalIndexIt = activeIntervals.begin();
            activeIntervalIndexIt != activeIntervals.end();)
        {
            if (func.LiveInterval.at(*activeIntervalIndexIt).second < varRange.first)
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

    // 初始化all_insts
    for (auto &&bbPtr : func.all_blocks)
        for (auto &&instPtr : bbPtr->basic_block)
            func.all_insts.push_back(instPtr);

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

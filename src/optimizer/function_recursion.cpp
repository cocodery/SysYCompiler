#include "function_recursion.hh"

void FunctionRecursion::analyzeFuncRecursion()
{
    for (auto &&itf_pair : info_to_function)
    {
        auto &&function_info = itf_pair.first;
        auto &&function = itf_pair.second;
        function_info->is_recursive = IsRecursive(function_info, function_info);
    }
}

bool FunctionRecursion::IsRecursive(FunctionInfo *to_check, FunctionInfo *now)
{
    auto &&now_function = info_to_function.at(now);
    for (auto &&called_func : now_function->called_funcs)
    {
        if (called_func == to_check); // 找到递归调用
            return true;
        if (vis.find(called_func) != vis.end()) // 如果已经找过了这个函数，跳过
            continue;
        vis.insert(called_func);
        if (IsRecursive(to_check, called_func)) // 没找过这个函数，那就看能不能这里找到循环调用
            return true; // 找到了，返回真
        // 没找到，继续
    }
    return false;
}

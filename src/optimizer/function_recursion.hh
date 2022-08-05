#pragma once

#include "common.hh"
#include "ir.hh"

class FunctionRecursion
{
public:
    map<FunctionInfo *, Function *> info_to_function;
    set<FunctionInfo *> vis;
public:
    FunctionRecursion(const vector<Function *> &functions)
    {
        for (auto &&function : functions)
            if (function->func_info.is_used)
                info_to_function.insert(make_pair(&function->func_info, function));
    }
    void analyzeFuncRecursion();
    bool IsRecursive(FunctionInfo *, FunctionInfo *);
};

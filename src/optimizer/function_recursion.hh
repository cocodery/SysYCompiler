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
        for (auto &&function : functions) {
            if (function->func_info.is_used) {
                function->func_info.is_recursive = IsRecursive(&function->func_info, &function->func_info);
            }
        }
    }
    bool IsRecursive(FunctionInfo *, FunctionInfo *);
};

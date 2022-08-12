#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class FuncInline {
public:
    Function *function;

    set<FunctionInfo *> vis;
public:
    FuncInline(Function *func) : function(func) { 
        function->func_info.is_recursive = isRecursive(&function->func_info, &function->func_info);
        function->func_info.side_effect = sideEffect();
    }

    bool isRecursive(FunctionInfo *, FunctionInfo *);
    bool multiReturn();
    bool sideEffect();
};

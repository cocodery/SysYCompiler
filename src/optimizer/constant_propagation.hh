#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class ConstantProg {
public:
    Function *function;
public:
    ConstantProg(Function *func) : function(func) { }
    void runConstantProp();
    void processInBlock(BasicBlock *blcok);
};

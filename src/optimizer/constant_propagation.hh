#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class ConstantProg {
public:
    Function *function;
public:
    ConstantProg(Function *func) : function(func) { }
    void runConstantProg();
    void processInBlock(BasicBlock *blcok);
};

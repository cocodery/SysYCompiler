#pragma once

#include "common.hh"
#include "ir.hh"

class InstCombine {
public:
    Function *function;
public:
    InstCombine(Function *func) : function(func) { }
    void moveCtv2Rhs();
    void runInstCombine();
};

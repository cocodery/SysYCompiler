#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class Dce {
public:
    Function *function;
public:
    Dce(Function *func) : function(func) { }
    void runDeadCodeElim();
};

#pragma once

#include "../common.hh"
#include "../structure/ir.hh"

class MemAccessOpt {
public:
    Function *function;

    map<int32_t, int32_t> bbIdx2MapIdx;
    vector<map<pair<int32_t, bool>, SRC>> addr2Value; 
public:
    MemAccessOpt(Function *func) : function(func) { }
    void runMemAccessOpt();
};

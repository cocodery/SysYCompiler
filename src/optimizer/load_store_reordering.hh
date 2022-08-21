#pragma once

#include "common.hh"
#include "ir.hh"

class LoadStoreReordering
{
public:
    map<Inst *, int> void_call_map;
public:
    LoadStoreReordering(Function *function);
    int GetDstRegId(Inst* instPtr); // returns dst_regid
    void removeRedundantStore(Function *function);
};

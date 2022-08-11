#pragma once

#include "common.hh"
#include "ir.hh"

class LoadStoreReordering
{
public:

public:
    LoadStoreReordering(Function *function);
    int ProcessInst(Inst* instPtr); // returns dst_regid
};

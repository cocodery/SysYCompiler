#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"
#include "value.hh"

class LLIR_RET: public Inst {
public:
    bool has_retvalue;
    Info *ret_value;
public:
    LLIR_RET(bool _hrv, Info *_vr): has_retvalue(_hrv), ret_value(_vr) { }
    string ToString();
    void printRetInst();
};

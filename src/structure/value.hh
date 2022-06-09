#pragma once

#include <iostream>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"

using std::cout;
using std::endl;

class CTValue {
public:
    DeclType type;
    int32_t int_value;
    float float_value;
public:
    CTValue(): type(TypeVoid), int_value(0), float_value(0) { }
    CTValue(DeclType t = TypeVoid, int32_t iv = 0, float fv = 0);
};

class IRValue {
public:
    VarType type;
    VirtReg reg;
    bool is_left_value;
public:
    IRValue() { }
    IRValue(VarType t, VirtReg vg, bool ass = false);
    bool can_assign();
};

CTValue operator + (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue lhs, CTValue rhs);
CTValue operator * (CTValue lhs, CTValue rhs);
CTValue operator / (CTValue lhs, CTValue rhs);
CTValue operator % (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue rhs);
CTValue operator ! (CTValue rhs);
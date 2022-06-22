#pragma once

#include <iostream>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"

using std::cout;
using std::endl;

extern const VirtReg NoRetReg;

class CTValue {
public:
    DeclType type;
    int32_t int_value;
    float float_value;
public:
    CTValue(DeclType t = TypeVoid, int32_t iv = 0, float fv = 0);
};

class IRValue {
public:
    VarType type;
    VirtReg reg;
    bool is_left_value;
public:
    IRValue() : reg(NoRetReg){ }
    IRValue(VarType t, VirtReg vg, bool ass = false);
    bool can_assign() { return !type.is_const && !type.is_array; };
};

CTValue operator + (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue lhs, CTValue rhs);
CTValue operator * (CTValue lhs, CTValue rhs);
CTValue operator / (CTValue lhs, CTValue rhs);
CTValue operator % (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue rhs);
CTValue operator ! (CTValue rhs);
#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../common.hh"
#include "symtable.hh"
#include "ir.hh"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;

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
    IRValue(VarType t, VirtReg vg, bool ass = false) : type(t), reg(vg), is_left_value(ass) { }
    bool can_assign();
};

CTValue operator + (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue lhs, CTValue rhs);
CTValue operator * (CTValue lhs, CTValue rhs);
CTValue operator / (CTValue lhs, CTValue rhs);
CTValue operator % (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue rhs);
CTValue operator ! (CTValue rhs);
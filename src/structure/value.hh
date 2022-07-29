#pragma once

#include <iostream>

#include "../common.hh"
#include "symtable.hh"
#include "baseclass.hh"

using std::cout;
using std::endl;

class CTValue: public Info {
public:
    DeclType type;
    int32_t int_value;
    double float_value;
public:
    CTValue(DeclType t = TypeVoid, int32_t iv = 0, float fv = 0);
    string ToString();
    bool operator == (CTValue &rhs) const {
        return (type == rhs.type) && (int_value == rhs.int_value) && (float_value == rhs.float_value);
    }
};

CTValue operator + (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue lhs, CTValue rhs);
CTValue operator * (CTValue lhs, CTValue rhs);
CTValue operator / (CTValue lhs, CTValue rhs);
CTValue operator % (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue rhs);
CTValue operator ! (CTValue rhs);

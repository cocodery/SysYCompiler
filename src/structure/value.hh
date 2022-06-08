#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../common.hh"

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

CTValue operator + (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue lhs, CTValue rhs);
CTValue operator * (CTValue lhs, CTValue rhs);
CTValue operator / (CTValue lhs, CTValue rhs);
CTValue operator % (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue rhs);
CTValue operator ! (CTValue rhs);
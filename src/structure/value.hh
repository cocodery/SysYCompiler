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
    bool operator == (const CTValue &ctv) const {
        if (type == TypeInt || type == TypeBool) return int_value == ctv.int_value;
        else if (type == TypeFloat) return float_value == ctv.float_value;
        else return false;
    }
    bool operator < (const CTValue &other) const {
        if (type == other.type)
            return (type == TypeInt) ? (int_value < other.int_value) : (float_value < other.float_value);
        else
            return type < other.type;
    }
};

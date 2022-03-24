#pragma once

#define DBG_MARCO_NO_WARNING
#include "dbg.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

using std::string;
using std::vector;

int parseNum(const char *number_str);

enum DeclType {
    TypeVoid, TypeInt, TypeFloat,
};

enum Operator {
    Add, Sub, Mul, Div, Mod,
    Lt, Le, Ge, Gt, Eq, Ne, And, Orr,
};

DeclType getDeclType(string type_str);

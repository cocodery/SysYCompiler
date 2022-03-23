#pragma once

#define DBG_MARCO_NO_WARNING
#include "dbg.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <variant>
#include <cassert>

using std::string;
using std::vector;
using SysYType = std::variant<int32_t, float>;
using SysYInit = std::variant<SysYType, vector<SysYType>>;

int parseNum(const char *number_str);

enum DeclType {
    TypeVoid, TypeInt, TypeFloat,
};

enum Operator {
    Add, Sub, Mul, Div, Mod,
    Lt, Le, Ge, Gt, Eq, Ne, And, Orr,
};

DeclType getDeclType(string type_str);

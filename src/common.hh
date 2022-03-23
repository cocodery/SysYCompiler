#pragma once

#define DBG_MARCO_NO_WARNING
#include "dbg.h"

#include <cstdlib>
#include <string>
#include <variant>
#include <cassert>

using std::string;
using SysYType = std::variant<int32_t, float>;

int parseNum(const char *number_str);

enum DeclType {
    Void, Int, Float
};

enum Operator {
    Add, Sub, Mul, Div, Mod,
    Lt, Le, Ge, Gt, Eq, Ne, And, Orr,
};

class common {
    virtual ~common() { };
};

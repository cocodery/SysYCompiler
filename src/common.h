#pragma once

#define DBG_MARCO_NO_WARNING
#include "dbg.h"

#include <cstdlib>
#include <string>
#include <cassert>

using std::string;

int parseNum(const char *number_str);

enum Operator {
    Add, Sub, Mul, Div, Mod,
    Lt, Le, Ge, Gt, Eq, Ne, And, Orr,
};

class common {
    virtual ~common() { };
};

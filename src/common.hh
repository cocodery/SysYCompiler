#pragma once

#define DBG_MARCO_NO_WARNING
#include "dbg.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

using std::string;
using std::vector;

extern int32_t tab_num;
string get_tabs();

int parseNum(const char *number_str);

enum DeclType {
    TypeVoid, TypeInt, TypeFloat,
};

DeclType getDeclType(string type_str);

string DeclTypeToStr(DeclType type);

#define Case(Type, dst, src) if (auto dst = dynamic_cast<Type *>(src))

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
string get_tabs(int);

int parseNum(const char *number_str);

enum DeclType {
    TypeVoid, TypeInt, TypeFloat, TypeBool, TypeByte
};

enum BinOp {
    ADD, SUB, MUL, DIV, REM,
};

enum RelOp {
    LTH, LEQ, EQU, NEQ,
};

string BinOpToStr(BinOp op);

BinOp StrToBinOp(string op);

string RelOpToStr_int(RelOp op);

string RelOpToStr_float(RelOp op);

RelOp StrToRelOp(string op);

DeclType getDeclType(string type_str);

string DeclTypeToStr(DeclType type);

#define Case(Type, dst, src) if (auto dst = dynamic_cast<Type *>(src))

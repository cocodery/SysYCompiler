#pragma once

#define DBG_MARCO_NO_WARNING
#include "dbg.h"

#define FOUR_SPACES "    "
#define EIGHT_SPACES "        "
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


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

string BinOpToStr_int(BinOp op);

string BinOpToStr_float(BinOp op);

BinOp StrToBinOp(string op);

string RelOpToStr_int(RelOp op);

string RelOpToStr_float(RelOp op);

RelOp StrToRelOp(string op);

DeclType getDeclType(string type_str);

string DeclTypeToStr(DeclType type);

#define Case(Type, dst, src) if (auto dst = dynamic_cast<Type *>(src))

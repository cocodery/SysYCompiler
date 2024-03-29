#pragma once

// #define DBG_MARCO_NO_WARNING
// #include "dbg.h"

#define FOUR_SPACES "    "
#define EIGHT_SPACES "        "
#define GLOB_VAR_PREFIX "global_var_"
#define GLOB_PTR_PREFIX "global_ptr_"
#define LOCA_VAR_PREFIX "local_var_"
#define LOCA_PTR_PREFIX "local_ptr_"
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

// codegen

// 临时的“帮手”寄存器，如果没有空闲寄存器就借用这个寄存器
#define STORE_REGISTER (r0)
#define STORE_REGISTER_SECOND (r1)
#define CMP_REGISTER (r0)
#define REM_REGISTER (r0)
#define GEP_REGISTER (r0)
#define CONST_ARRAY_INIT_REGISTERS {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15}
#define LOCAL_VARS_SAVE_MEMCPY_REGISTERS {r0, r1, r2, r3}
#define LOCAL_VARS_LOAD_MEMCPY_REGISTERS {r0, lr}

//涉及到函数开头或结尾分配空间的寄存器，不能使用r0-r3，因为这些寄存器被用来传递参数
#define PRELLOC_REGISTER (r12)
#define DELLOC_REGISTER (r12)
#define GLOBAL_ALLOC_LR_TEMP (r11)

enum REGs { r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7,  r8,  r9, r10, r11, r12,  sp,  lr,  pc,
            s0,  s1,  s2,  s3,  s4,  s5,  s6,  s7,  s8,  s9, s10, s11, s12, s13, s14, s15,
            s16, s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31,
            SPILL, NOALLOC};

#define AVAIL_REGS_INIT_LIST {r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7,  r8,  r9,  r10, r11, r12, \
                              s4,  s5,  s6,  s7,  s8,  s9,  s10, s11, s12, s13, s14, s15, s16, \
                              s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31}

#define CLAIM_AVAIL_REGS set<REGs> availRegs AVAIL_REGS_INIT_LIST;

// 浮点二元运算的寄存器
#define FLOAT_BINOP_REGISTER_1 (s0)
#define FLOAT_BINOP_REGISTER_2 (s1)
#define TEMP_S_REGISTER_1 (s2)
// #define FLOAT_BINOP_REGISTER_3 (s2) //做cmp运算时保存现场用

#include <cstdlib>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <cassert>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <sstream>

using std::string;
using std::vector;
using std::stack;
using std::queue;
using std::set;
using std::map;
using std::list;
using std::deque;

extern int32_t tab_num;
string get_tabs();
string get_tabs(int);

int parseNum(const char *number_str);

enum DeclType {
    TypeVoid, TypeInt, TypeFloat, TypeBool, TypeByte
};

enum BinOp {
    ADD, SUB, MUL, DIV, REM, GEP
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

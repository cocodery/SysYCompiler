#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <map>

#include "../common.hh"
#include "symtable.hh"
#include "inst.hh"
#include "llir.hh"
#include "baseclass.hh"

using std::ostream;
using std::vector;
using std::string;
using std::map;

static int32_t bb_index = 0;
static int32_t sp_index = 0;

class SRC {
public:
    CTValue *ctv;
    VirtReg *reg;
public:
    SRC(): ctv(nullptr), reg(nullptr) { }
    SRC(CTValue *_ctv): ctv(_ctv), reg(nullptr) { }
    SRC(VirtReg *_reg): ctv(nullptr), reg(_reg) { }
    string ToString();
    CTValue *ToCTValue() {
        return (ctv == nullptr )? nullptr : ctv;
    };
    VirtReg *ToVirtReg() {
        return (reg == nullptr) ? nullptr : reg;
    }
};

class BasicBlock: public Info {
public:
    int32_t bb_idx;
    vector<Inst *> basic_block;
    vector<BasicBlock *> parants;
    vector<BasicBlock *> childrens; 
public:
    BasicBlock() : bb_idx(bb_index++) { }
    void printBlock();
};

class Scope: public Info {
public:
    int32_t sp_idx;
    VariableTable  *local_table;
    vector<Info *> *elements; // `Info` -> `Scope` or `BasicBlock`
    Scope* parent;
public:
    Scope() : sp_idx(sp_index++) { local_table = nullptr; elements = nullptr; parent = nullptr; }
    Variable *resolve(string var_name);
    BasicBlock *get_last_bb();
    void printElements();
    void printScope();
};

class Function {
public:
    FunctionInfo func_info;
    Scope *main_scope;
public:
    void printFunction();
};

class LibFunction {
public:
    FunctionInfo libfunc_info;
    bool is_used;
public:
    void printFunction();
};

class CompUnit: public Info {
public:
    Scope *global_scope;
    vector<Function *> functions;
    LibFunction lib_functions[11];
public:
    CompUnit();
    void moveGlobalInitToMain();
    bool inLibFunctions(string func_name);
    FunctionInfo *getFunctionInfo(string func_name);
    void DebugLibFuncs();
    void DebugUserFuncs();
    void DebugGlobalTable();
};

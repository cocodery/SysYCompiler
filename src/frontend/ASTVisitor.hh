#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <variant>
#include "../common.hh"
#include "../structure/symtable.hh"
#include "../structure/ir.hh"
#include "../structure/llir.hh"
#include "../structure/value.hh"
#include "SysYBaseVisitor.h"

using std::vector;
using std::string;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

class ASTVisitor : public SysYBaseVisitor {
// Some Useful Defination
private:
    DeclType cur_type;
    DeclType ret_type;
    int32_t var_idx;
    int32_t bb_idx;
    int32_t sp_idx;
    bool init_args;
    CompUnit &ir;
    vector<pair<SysYParser::InitVarDefContext *, VarPair>> glb_var_init;
    vector<Info *> *cur_scope_elements;
    Scope          *cur_scope;
    VariableTable  *cur_vartable;
    BasicBlock     *cur_basicblock;
    FunctionInfo   *cur_func_info;
    int32_t continue_target;
// Funtion for helping Build IR and Symbol Table
public:
    bool have_main_func;
    ASTVisitor(CompUnit &_ir);
    
    vector<int32_t> get_array_dims(vector<SysYParser::ConstExpContext *>);
    
    void parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t>& ilist, vector<float>& flist);

    void parse_variable_init(SysYParser::ListInitvalContext *node, VarType type, vector<int32_t> arr_dim, SRC addr, int32_t off);

    void generate_varinit_ir(SysYParser::InitVarDefContext *ctx, VarPair var_pair);

    void local_const_list_init(VarPair var_pair);

    void init_func_args(FunctionInfo *func_info);
// Function for Abstract Syntax Tree
public:
    virtual antlrcpp::Any visitChildren(antlr4::tree::ParseTree *ctx) override;
//Node Visitors:

//compUnit : (decl | funcDef)* EOF ;
    virtual antlrcpp::Any visitCompUnit(SysYParser::CompUnitContext *ctx) override;
 
//decl : constDecl | varDecl ;
    virtual antlrcpp::Any visitDecl(SysYParser::DeclContext *ctx) override;

//constDecl : 'const' bType constDef (',' constDef)* ';' ;
    virtual antlrcpp::Any visitConstDecl(SysYParser::ConstDeclContext *ctx) override;

//bType : 'int' ;
    virtual antlrcpp::Any visitBType(SysYParser::BTypeContext *ctx) override;

//constDef : Identifier ('[' constExp ']')* '=' constInitVal;
    virtual antlrcpp::Any visitConstDef(SysYParser::ConstDefContext *ctx) override;

/*
constInitVal
    : constExp # scalarConstInitVal
    | '{' (constInitVal (',' constInitVal)* )? '}' # listConstInitVal
    ;
*/
    virtual antlrcpp::Any visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) override;

    virtual antlrcpp::Any visitListConstInitVal(SysYParser::ListConstInitValContext *ctx)override;

//varDecl : bType varDef (',' varDef)* ';' ;
    virtual antlrcpp::Any visitVarDecl(SysYParser::VarDeclContext *ctx) override;

/*
varDef
    : Identifier ('[' constExp ']')* # uninitVarDef
    | Identifier ('[' constExp ']')* '=' initVal # initVarDef
    ;
*/
    virtual antlrcpp::Any visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) override;

    virtual antlrcpp::Any visitInitVarDef(SysYParser::InitVarDefContext *ctx) override;

/*
initVal
    : exp # scalarInitVal
    | '{' (initVal (',' initVal)* )? '}' # listInitval
    ;
*/
    virtual antlrcpp::Any visitScalarInitVal(SysYParser::ScalarInitValContext *ctx);

    virtual antlrcpp::Any visitListInitval(SysYParser::ListInitvalContext *ctx) override;

//funcDef : funcType Identifier '(' (funcFParams)? ')' block;
    virtual antlrcpp::Any visitFuncDef(SysYParser::FuncDefContext *ctx) override;

//funcType : 'void' | 'int';
    virtual antlrcpp::Any visitFuncType(SysYParser::FuncTypeContext *ctx) override;

//funcFParams : funcFParam (',' funcFParam)*;
    virtual antlrcpp::Any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override;

//funcFParam : bType Identifier ('[' ']' ('[' constExp ']')* )?;
    virtual antlrcpp::Any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override;

//block : '{' (blockItem)* '}';
    virtual antlrcpp::Any visitBlock(SysYParser::BlockContext *ctx) override;

//blockItem : decl | stmt;
    virtual antlrcpp::Any visitBlockItem(SysYParser::BlockItemContext *ctx) override;

/*
stmt
    : lVal '=' exp ';' # assignment
    | (exp)? ';' # expStmt
    | block # blockStmt
    | 'if' '(' cond ')' stmt # ifStmt1
    | 'if' '(' cond ')' stmt 'else' stmt # ifStmt2
    | 'while' '(' cond ')' stmt # whileStmt
    | 'break' ';' # breakStmt
    | 'continue' ';' # continueStmt
    | 'return' (exp)? ';' # returnStmt
    ;
*/
    virtual antlrcpp::Any visitAssignment(SysYParser::AssignmentContext *ctx) override;

    virtual antlrcpp::Any visitExpStmt(SysYParser::ExpStmtContext *ctx) override;

    virtual antlrcpp::Any visitBlockStmt(SysYParser::BlockStmtContext *ctx) override;

    virtual antlrcpp::Any visitIfStmt(SysYParser::IfStmtContext *ctx) override;

    virtual antlrcpp::Any visitWhileStmt(SysYParser::WhileStmtContext *ctx) override;

    virtual antlrcpp::Any visitBreakStmt(SysYParser::BreakStmtContext *ctx) override;

    virtual antlrcpp::Any visitContinueStmt(SysYParser::ContinueStmtContext *ctx) override;

    virtual antlrcpp::Any visitReturnStmt(SysYParser::ReturnStmtContext *ctx) override;

//exp : addExp;
    virtual antlrcpp::Any visitExp(SysYParser::ExpContext *ctx) override;

//cond : lOrExp;
    virtual antlrcpp::Any visitCond(SysYParser::CondContext *ctx) override;

//lVal : Identifier ('[' exp ']')*;
    virtual antlrcpp::Any visitLVal(SysYParser::LValContext *ctx) override;

/*
primaryExp
    : '(' exp ')' # primaryExp1
    | lVal # primaryExp2
    | number # primaryExp3
    ;
*/
    virtual antlrcpp::Any visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) override;

    virtual antlrcpp::Any visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) override;

    virtual antlrcpp::Any visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) override;

/*
number
    : IntLiteral #number1
    | FloatLiteral #number2
    ; 
*/
    virtual antlrcpp::Any visitNumber1(SysYParser::Number1Context *ctx) override;
    
    virtual antlrcpp::Any visitNumber2(SysYParser::Number2Context *ctx) override;

/*
unaryExp
    : primaryExp # unary1
    | Identifier '(' (funcRParams)? ')' # unary2
    | unaryOp unaryExp # unary3
    ;
*/
    virtual antlrcpp::Any visitUnary1(SysYParser::Unary1Context *ctx) override;

    virtual antlrcpp::Any visitUnary2(SysYParser::Unary2Context *ctx) override;
    
    virtual antlrcpp::Any visitUnary3(SysYParser::Unary3Context *ctx) override;

//unaryOp : '+' | '-' | '!';
    virtual antlrcpp::Any visitUnaryOp(SysYParser::UnaryOpContext *ctx) override;

//funcRParams : funcRParam (',' funcRParam)*;
    virtual antlrcpp::Any visitFuncRParams(SysYParser::FuncRParamsContext *ctx) override;

//funcRParam : exp ;
    virtual antlrcpp::Any visitFuncRParam(SysYParser::FuncRParamContext *ctx) override;

/*
mulExp
    : unaryExp # mul1
    | mulExp ('*' | '/' | '%') unaryExp # mul2
    ;
*/
    virtual antlrcpp::Any visitMul2(SysYParser::Mul2Context *ctx) override;

    virtual antlrcpp::Any visitMul1(SysYParser::Mul1Context *ctx) override;

/*
addExp
    : mulExp # add1
    | addExp ('+' | '-') mulExp # add2
    ;
*/
    virtual antlrcpp::Any visitAdd2(SysYParser::Add2Context *ctx) override;

    virtual antlrcpp::Any visitAdd1(SysYParser::Add1Context *ctx) override;

/*
relExp
    : addExp # rel1
    | relExp ('<' | '>' | '<=' | '>=') addExp # rel2
    ;
*/
    virtual antlrcpp::Any visitRel2(SysYParser::Rel2Context *ctx) override;

    virtual antlrcpp::Any visitRel1(SysYParser::Rel1Context *ctx) override;

/*
eqExp
    : relExp # eq1
    | eqExp ('==' | '!=') relExp # eq2
    ;
*/
    virtual antlrcpp::Any visitEq1(SysYParser::Eq1Context *ctx) override;

    virtual antlrcpp::Any visitEq2(SysYParser::Eq2Context *ctx) override;

/*
lAndExp
    : eqExp # lAnd1
    | lAndExp '&&' eqExp # lAnd2
    ;
*/
    virtual antlrcpp::Any visitLAnd2(SysYParser::LAnd2Context *ctx) override;

    virtual antlrcpp::Any visitLAnd1(SysYParser::LAnd1Context *ctx) override;

/*
lOrExp
    : lAndExp # lOr1
    | lOrExp '||' lAndExp # lOr2
    ;
*/
    virtual antlrcpp::Any visitLOr1(SysYParser::LOr1Context *ctx) override;

    virtual antlrcpp::Any visitLOr2(SysYParser::LOr2Context *ctx) override;

//constExp : addExp ;
    virtual antlrcpp::Any visitConstExp(SysYParser::ConstExpContext *ctx) override;
};
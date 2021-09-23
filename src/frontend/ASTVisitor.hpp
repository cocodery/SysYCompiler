#pragma once

#include <string>
#include <iostream>

#include "SysYBaseVisitor.h"

class ASTVisitor : public SysYBaseVisitor {
private:

public:
    ASTVisitor();
    
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

    virtual antlrcpp::Any visitConstDef(SysYParser::ConstDefContext *ctx);

    virtual antlrcpp::Any visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx);

    virtual antlrcpp::Any visitListConstInitVal(SysYParser::ListConstInitValContext *ctx);

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

    virtual antlrcpp::Any visitScalarInitVal(SysYParser::ScalarInitValContext *ctx);

    virtual antlrcpp::Any visitListInitval(SysYParser::ListInitvalContext *ctx) override;

    virtual antlrcpp::Any visitFuncDef(SysYParser::FuncDefContext *ctx) override;

    virtual antlrcpp::Any visitFuncType(SysYParser::FuncTypeContext *ctx) override;

    virtual antlrcpp::Any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override;

    virtual antlrcpp::Any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override;

    virtual antlrcpp::Any visitBlock(SysYParser::BlockContext *ctx) override;

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
    virtual antlrcpp::Any visitAssignment(SysYParser::AssignmentContext *ctx);

    virtual antlrcpp::Any visitExpStmt(SysYParser::ExpStmtContext *ctx);

    virtual antlrcpp::Any visitBlockStmt(SysYParser::BlockStmtContext *ctx);

    virtual antlrcpp::Any visitIfStmt1(SysYParser::IfStmt1Context *ctx);

    virtual antlrcpp::Any visitIfStmt2(SysYParser::IfStmt2Context *ctx);

    virtual antlrcpp::Any visitWhileStmt(SysYParser::WhileStmtContext *ctx);

    virtual antlrcpp::Any visitBreakStmt(SysYParser::BreakStmtContext *ctx);

    virtual antlrcpp::Any visitContinueStmt(SysYParser::ContinueStmtContext *ctx);

    virtual antlrcpp::Any visitReturnStmt(SysYParser::ReturnStmtContext *ctx) override;

    virtual antlrcpp::Any visitExp(SysYParser::ExpContext *ctx);

    virtual antlrcpp::Any visitCond(SysYParser::CondContext *ctx);

    virtual antlrcpp::Any visitLVal(SysYParser::LValContext *ctx);

    virtual antlrcpp::Any visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx);

    virtual antlrcpp::Any visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx);

    virtual antlrcpp::Any visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx);

    virtual antlrcpp::Any visitNumber(SysYParser::NumberContext *ctx);

    virtual antlrcpp::Any visitUnary1(SysYParser::Unary1Context *ctx);

    virtual antlrcpp::Any visitUnary2(SysYParser::Unary2Context *ctx);
    
    virtual antlrcpp::Any visitUnary3(SysYParser::Unary3Context *ctx);

    virtual antlrcpp::Any visitUnaryOp(SysYParser::UnaryOpContext *ctx);

    virtual antlrcpp::Any visitFuncRParams(SysYParser::FuncRParamsContext *ctx);

    virtual antlrcpp::Any visitFuncRParam(SysYParser::FuncFParamContext *ctx);

    virtual antlrcpp::Any visitMul2(SysYParser::Mul2Context *ctx);

    virtual antlrcpp::Any visitMul1(SysYParser::Mul1Context *ctx);

    virtual antlrcpp::Any visitAdd2(SysYParser::Add2Context *ctx);

    virtual antlrcpp::Any visitAdd1(SysYParser::Add1Context *ctx);

    virtual antlrcpp::Any visitRel2(SysYParser::Rel2Context *ctx);

    virtual antlrcpp::Any visitRel1(SysYParser::Rel1Context *ctx);

    virtual antlrcpp::Any visitEq1(SysYParser::Eq1Context *ctx);

    virtual antlrcpp::Any visitEq2(SysYParser::Eq2Context *ctx);

    virtual antlrcpp::Any visitLAnd2(SysYParser::LAnd2Context *ctx);

    virtual antlrcpp::Any visitLAnd1(SysYParser::LAnd1Context *ctx);

    virtual antlrcpp::Any visitLOr1(SysYParser::LOr1Context *ctx);

    virtual antlrcpp::Any visitLOr2(SysYParser::LOr2Context *ctx);

    virtual antlrcpp::Any visitConstExp(SysYParser::ConstExpContext *ctx);
};
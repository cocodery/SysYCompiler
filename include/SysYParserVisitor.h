
// Generated from SysYParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "SysYParserParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SysYParserParser.
 */
class  SysYParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SysYParserParser.
   */
    virtual antlrcpp::Any visitCompUnit(SysYParserParser::CompUnitContext *context) = 0;

    virtual antlrcpp::Any visitDecl(SysYParserParser::DeclContext *context) = 0;

    virtual antlrcpp::Any visitConstDecl(SysYParserParser::ConstDeclContext *context) = 0;

    virtual antlrcpp::Any visitBType(SysYParserParser::BTypeContext *context) = 0;

    virtual antlrcpp::Any visitConstDef(SysYParserParser::ConstDefContext *context) = 0;

    virtual antlrcpp::Any visitScalarConstInitVal(SysYParserParser::ScalarConstInitValContext *context) = 0;

    virtual antlrcpp::Any visitListConstInitVal(SysYParserParser::ListConstInitValContext *context) = 0;

    virtual antlrcpp::Any visitVarDecl(SysYParserParser::VarDeclContext *context) = 0;

    virtual antlrcpp::Any visitUninitVarDef(SysYParserParser::UninitVarDefContext *context) = 0;

    virtual antlrcpp::Any visitInitVarDef(SysYParserParser::InitVarDefContext *context) = 0;

    virtual antlrcpp::Any visitScalarInitVal(SysYParserParser::ScalarInitValContext *context) = 0;

    virtual antlrcpp::Any visitListInitval(SysYParserParser::ListInitvalContext *context) = 0;

    virtual antlrcpp::Any visitFuncDef(SysYParserParser::FuncDefContext *context) = 0;

    virtual antlrcpp::Any visitFuncType(SysYParserParser::FuncTypeContext *context) = 0;

    virtual antlrcpp::Any visitFuncFParams(SysYParserParser::FuncFParamsContext *context) = 0;

    virtual antlrcpp::Any visitFuncFParam(SysYParserParser::FuncFParamContext *context) = 0;

    virtual antlrcpp::Any visitBlock(SysYParserParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitBlockItem(SysYParserParser::BlockItemContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(SysYParserParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitExpStmt(SysYParserParser::ExpStmtContext *context) = 0;

    virtual antlrcpp::Any visitBlockStmt(SysYParserParser::BlockStmtContext *context) = 0;

    virtual antlrcpp::Any visitIfStmt1(SysYParserParser::IfStmt1Context *context) = 0;

    virtual antlrcpp::Any visitIfStmt2(SysYParserParser::IfStmt2Context *context) = 0;

    virtual antlrcpp::Any visitWhileStmt(SysYParserParser::WhileStmtContext *context) = 0;

    virtual antlrcpp::Any visitBreakStmt(SysYParserParser::BreakStmtContext *context) = 0;

    virtual antlrcpp::Any visitContinueStmt(SysYParserParser::ContinueStmtContext *context) = 0;

    virtual antlrcpp::Any visitReturnStmt(SysYParserParser::ReturnStmtContext *context) = 0;

    virtual antlrcpp::Any visitExp(SysYParserParser::ExpContext *context) = 0;

    virtual antlrcpp::Any visitCond(SysYParserParser::CondContext *context) = 0;

    virtual antlrcpp::Any visitLVal(SysYParserParser::LValContext *context) = 0;

    virtual antlrcpp::Any visitPrimaryExp1(SysYParserParser::PrimaryExp1Context *context) = 0;

    virtual antlrcpp::Any visitPrimaryExp2(SysYParserParser::PrimaryExp2Context *context) = 0;

    virtual antlrcpp::Any visitPrimaryExp3(SysYParserParser::PrimaryExp3Context *context) = 0;

    virtual antlrcpp::Any visitNumber(SysYParserParser::NumberContext *context) = 0;

    virtual antlrcpp::Any visitUnary1(SysYParserParser::Unary1Context *context) = 0;

    virtual antlrcpp::Any visitUnary2(SysYParserParser::Unary2Context *context) = 0;

    virtual antlrcpp::Any visitUnary3(SysYParserParser::Unary3Context *context) = 0;

    virtual antlrcpp::Any visitUnaryOp(SysYParserParser::UnaryOpContext *context) = 0;

    virtual antlrcpp::Any visitFuncRParams(SysYParserParser::FuncRParamsContext *context) = 0;

    virtual antlrcpp::Any visitExpAsRParam(SysYParserParser::ExpAsRParamContext *context) = 0;

    virtual antlrcpp::Any visitMul2(SysYParserParser::Mul2Context *context) = 0;

    virtual antlrcpp::Any visitMul1(SysYParserParser::Mul1Context *context) = 0;

    virtual antlrcpp::Any visitAdd2(SysYParserParser::Add2Context *context) = 0;

    virtual antlrcpp::Any visitAdd1(SysYParserParser::Add1Context *context) = 0;

    virtual antlrcpp::Any visitRel2(SysYParserParser::Rel2Context *context) = 0;

    virtual antlrcpp::Any visitRel1(SysYParserParser::Rel1Context *context) = 0;

    virtual antlrcpp::Any visitEq1(SysYParserParser::Eq1Context *context) = 0;

    virtual antlrcpp::Any visitEq2(SysYParserParser::Eq2Context *context) = 0;

    virtual antlrcpp::Any visitLAnd2(SysYParserParser::LAnd2Context *context) = 0;

    virtual antlrcpp::Any visitLAnd1(SysYParserParser::LAnd1Context *context) = 0;

    virtual antlrcpp::Any visitLOr1(SysYParserParser::LOr1Context *context) = 0;

    virtual antlrcpp::Any visitLOr2(SysYParserParser::LOr2Context *context) = 0;

    virtual antlrcpp::Any visitConstExp(SysYParserParser::ConstExpContext *context) = 0;


};


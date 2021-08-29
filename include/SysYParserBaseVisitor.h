
// Generated from SysYParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "SysYParserVisitor.h"


/**
 * This class provides an empty implementation of SysYParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SysYParserBaseVisitor : public SysYParserVisitor {
public:

  virtual antlrcpp::Any visitCompUnit(SysYParserParser::CompUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecl(SysYParserParser::DeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstDecl(SysYParserParser::ConstDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBType(SysYParserParser::BTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstDef(SysYParserParser::ConstDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitScalarConstInitVal(SysYParserParser::ScalarConstInitValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitListConstInitVal(SysYParserParser::ListConstInitValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDecl(SysYParserParser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUninitVarDef(SysYParserParser::UninitVarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInitVarDef(SysYParserParser::InitVarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitScalarInitVal(SysYParserParser::ScalarInitValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitListInitval(SysYParserParser::ListInitvalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncDef(SysYParserParser::FuncDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncType(SysYParserParser::FuncTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncFParams(SysYParserParser::FuncFParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncFParam(SysYParserParser::FuncFParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(SysYParserParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlockItem(SysYParserParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(SysYParserParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpStmt(SysYParserParser::ExpStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlockStmt(SysYParserParser::BlockStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfStmt1(SysYParserParser::IfStmt1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfStmt2(SysYParserParser::IfStmt2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileStmt(SysYParserParser::WhileStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBreakStmt(SysYParserParser::BreakStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContinueStmt(SysYParserParser::ContinueStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnStmt(SysYParserParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExp(SysYParserParser::ExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCond(SysYParserParser::CondContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLVal(SysYParserParser::LValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimaryExp1(SysYParserParser::PrimaryExp1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimaryExp2(SysYParserParser::PrimaryExp2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimaryExp3(SysYParserParser::PrimaryExp3Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumber(SysYParserParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary1(SysYParserParser::Unary1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary2(SysYParserParser::Unary2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary3(SysYParserParser::Unary3Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryOp(SysYParserParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncRParams(SysYParserParser::FuncRParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpAsRParam(SysYParserParser::ExpAsRParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMul2(SysYParserParser::Mul2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMul1(SysYParserParser::Mul1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdd2(SysYParserParser::Add2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdd1(SysYParserParser::Add1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRel2(SysYParserParser::Rel2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRel1(SysYParserParser::Rel1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEq1(SysYParserParser::Eq1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEq2(SysYParserParser::Eq2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLAnd2(SysYParserParser::LAnd2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLAnd1(SysYParserParser::LAnd1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLOr1(SysYParserParser::LOr1Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLOr2(SysYParserParser::LOr2Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstExp(SysYParserParser::ConstExpContext *ctx) override {
    return visitChildren(ctx);
  }


};


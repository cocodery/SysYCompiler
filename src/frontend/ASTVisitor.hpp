#pragma once

#include <string>

#include "SysYBaseVisitor.h"

class ASTVisitor : public SysYBaseVisitor {
private:

public:
    ASTVisitor();
    
    virtual antlrcpp::Any visitChildren(antlr4::tree::ParseTree *ctx) override;

    virtual antlrcpp::Any visitCompUnit(SysYParser::CompUnitContext *ctx) override;

    virtual antlrcpp::Any visitDecl(SysYParser::DeclContext *ctx) override;

    virtual antlrcpp::Any visitBType(SysYParser::BTypeContext *ctx) override;

    virtual antlrcpp::Any visitFuncDef(SysYParser::FuncDefContext *ctx) override;

    virtual antlrcpp::Any visitFuncType(SysYParser::FuncTypeContext *ctx) override;

    virtual antlrcpp::Any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override;

    virtual antlrcpp::Any visitBlock(SysYParser::BlockContext *ctx) override;

    virtual antlrcpp::Any visitBlockItem(SysYParser::BlockItemContext *ctx) override;

    virtual antlrcpp::Any visitReturnStmt(SysYParser::ReturnStmtContext *ctx) override;

};
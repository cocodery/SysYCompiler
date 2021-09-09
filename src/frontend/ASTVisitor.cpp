#include "ASTVisitor.hpp"

using std::string;

ASTVisitor::ASTVisitor() {

}

antlrcpp::Any ASTVisitor::visitChildren(antlr4::tree::ParseTree *ctx) {
    size_t n = ctx->children.size();
    for (size_t i = 0; i < n; ++i) ctx->children[i]->accept(this);
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitCompUnit(SysYParser::CompUnitContext *ctx) {
    visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitDecl(SysYParser::DeclContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitBType(SysYParser::BTypeContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitFuncType(SysYParser::FuncTypeContext *ctx) {
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitFuncDef(SysYParser::FuncDefContext *ctx) {
    bool rettype = (ctx->funcType()->getText() == "int");
    string name = ctx->Identifier()->getText();
    ctx->block()->accept(this);
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitBlock(SysYParser::BlockContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitBlockItem(SysYParser::BlockItemContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
    return nullptr;
}

#include "ASTVisitor.h"

using std::string;
using std::cout;
using std::endl;

ASTVisitor::ASTVisitor() {
    mode = normal;
}

antlrcpp::Any ASTVisitor::visitChildren(antlr4::tree::ParseTree *ctx) {
    size_t n = ctx->children.size();
    for (size_t i = 0; i < n; ++i) {
        ctx->children[i]->accept(this);
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitCompUnit(SysYParser::CompUnitContext *ctx) {
    visitChildren(ctx);
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitDecl(SysYParser::DeclContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
    dbg("In visitConstDecl");
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitBType(SysYParser::BTypeContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitConstDef(SysYParser::ConstDefContext *ctx) {
    string const_var_name =  ctx->Identifier()->getText();
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) {
    dbg("Program should never reach Function visitScalarConstInitVal");
    exit(EXIT_FAILURE);
}

antlrcpp::Any ASTVisitor::visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) {
    dbg("Program should never reach Function visitListConstInitVal");
    exit(EXIT_FAILURE);
}

antlrcpp::Any ASTVisitor::visitVarDecl(SysYParser::VarDeclContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) {
    dbg("Program should never reach Function visitScalarInitVal");
    exit(EXIT_FAILURE);
}

antlrcpp::Any ASTVisitor::visitListInitval(SysYParser::ListInitvalContext *ctx) {
    dbg("Program should never reach Function visitListInitval");
    exit(EXIT_FAILURE);
}


antlrcpp::Any ASTVisitor::visitFuncDef(SysYParser::FuncDefContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitFuncType(SysYParser::FuncTypeContext *ctx) {
    dbg("Program should never reach Function visitFuncType");
    exit(EXIT_FAILURE);
}

antlrcpp::Any ASTVisitor::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitFuncFParam(SysYParser::FuncFParamContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitBlock(SysYParser::BlockContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitBlockItem(SysYParser::BlockItemContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitAssignment(SysYParser::AssignmentContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitExpStmt(SysYParser::ExpStmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitBlockStmt(SysYParser::BlockStmtContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitIfStmt1(SysYParser::IfStmt1Context *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitIfStmt2(SysYParser::IfStmt2Context *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitWhileStmt(SysYParser::WhileStmtContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitBreakStmt(SysYParser::BreakStmtContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitContinueStmt(SysYParser::ContinueStmtContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitExp(SysYParser::ExpContext *ctx) {
    return ctx->addExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitCond(SysYParser::CondContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitLVal(SysYParser::LValContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) {
    return ctx->exp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) {
    return ctx->lVal()->accept(this);
}

antlrcpp::Any ASTVisitor::visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) {
    int32_t parse_number = ctx->number()->accept(this);
    dbg(parse_number);
    if (mode == compile_time) {
        return parse_number;
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitNumber(SysYParser::NumberContext *ctx) {
    const char *number_str = ctx->getText().c_str();
    int result = parseNum(number_str);
    return result;
}

antlrcpp::Any ASTVisitor::visitUnary1(SysYParser::Unary1Context *ctx) {
    return ctx->primaryExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitUnary2(SysYParser::Unary2Context *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitUnary3(SysYParser::Unary3Context *ctx) {
    // TODO:
    char op = ctx->unaryOp()->getText()[0];
    if (mode == compile_time) {
        int32_t rhs = ctx->unaryExp()->accept(this);
        if (op == '-')
            return -rhs;
        else if (op == '!')
            return !rhs;
        else 
            return rhs;
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitUnaryOp(SysYParser::UnaryOpContext *ctx) {
    dbg("Program should never reach Function visitListConstInitVal");
    exit(EXIT_FAILURE);
}

antlrcpp::Any ASTVisitor::visitFuncRParams(SysYParser::FuncRParamsContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitFuncRParam(SysYParser::FuncRParamContext *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitMul1(SysYParser::Mul1Context *ctx) {
    return ctx->unaryExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitMul2(SysYParser::Mul2Context *ctx) {
    // TODO:
    char op = ctx->children[1]->getText()[0];
    if (mode == compile_time) {
        int32_t lhs = ctx->mulExp()->accept(this);
        int32_t rhs = ctx->unaryExp()->accept(this);
        if (op == '*')
            return lhs * rhs;
        else if (op == '/')
            return lhs / rhs;
        else if (op == '%')
            return lhs % rhs;
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitAdd1(SysYParser::Add1Context *ctx) {
    return ctx->mulExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitAdd2(SysYParser::Add2Context *ctx) {
    // TODO:
    char op = ctx->children[1]->getText()[0];
    if (mode == compile_time) {
        int32_t lhs = ctx->addExp()->accept(this);
        int32_t rhs = ctx->mulExp()->accept(this);
        if (op == '+') 
            return lhs + rhs;
        else if (op == '-') 
            return lhs - rhs;
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitRel1(SysYParser::Rel1Context *ctx) {
    return ctx->addExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitRel2(SysYParser::Rel2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitEq1(SysYParser::Eq1Context *ctx) {
    return ctx->relExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitEq2(SysYParser::Eq2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitLAnd1(SysYParser::LAnd1Context *ctx) {
    return ctx->eqExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitLAnd2(SysYParser::LAnd2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitLOr1(SysYParser::LOr1Context *ctx) {
    return ctx->lAndExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitLOr2(SysYParser::LOr2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitConstExp(SysYParser::ConstExpContext *ctx) {
    dbg("In visitConstExp");
    mode = compile_time;
    int32_t result = ctx->addExp()->accept(this);
    mode = normal;
    dbg(result);
    return result;
}

#include "ASTVisitor.h"

ASTVisitor::ASTVisitor() {
    mode = normal;
}

vector<int32_t> ASTVisitor::get_array_dims(vector<SysYParser::ConstExpContext *> dims) {
    vector<int32_t> array_dims;
    for (auto i : dims) {
        int32_t cur_dim = i->accept(this);
        array_dims.push_back(cur_dim);
    }
    return array_dims;
}

void ASTVisitor::parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t>& list_init_value) {
    int32_t total_size = 1;
    for (auto i: array_dims) {
        total_size *= i;
    }
    if (total_size == 0) return;
    int32_t child_size = total_size / array_dims[0];
    vector<int32_t> child_array_dims = array_dims;
    child_array_dims.erase(child_array_dims.begin());
    int32_t cnt = 0;
    for (auto child: node->constInitVal()) {
        auto scalar_node = dynamic_cast<SysYParser::ScalarConstInitValContext *>(child);
        if (scalar_node != nullptr) {
            int32_t scalar_value = scalar_node->constExp()->accept(this);
            list_init_value.push_back(scalar_value);
            ++cnt;
        }
        else {
            auto list_node = dynamic_cast<SysYParser::ListConstInitValContext *>(child);
            parse_const_init(list_node, child_array_dims, list_init_value);
            cnt += total_size / array_dims[0];
        }
    }
    while (cnt < total_size) {
        list_init_value.push_back(0);
        ++cnt;
    }
    return;
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
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitBType(SysYParser::BTypeContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitConstDef(SysYParser::ConstDefContext *ctx) {
    string const_var_name =  ctx->Identifier()->getText();
    VarType var_type;
    var_type.is_const = true;
    var_type.is_array = !(ctx->constExp().size() == 0);
    var_type.is_func_args = false;
    if (var_type.is_array == true) {
        var_type.array_dims = get_array_dims(ctx->constExp());
    }
    dbg(const_var_name ,var_type.is_const, var_type.is_array, var_type.is_func_args);    
    dbg(var_type.array_dims);
    InitValue const_init_value;
    const_init_value.is_const = var_type.is_const;
    const_init_value.is_array = var_type.is_array;
    auto init_var_node = ctx->constInitVal();
    if (const_init_value.is_array == false) {
        auto node = dynamic_cast<SysYParser::ScalarConstInitValContext *>(init_var_node);
        const_init_value.scalar_init_value = node->constExp()->accept(this);
        dbg(const_init_value.scalar_init_value);
    }
    else {
        auto node = dynamic_cast<SysYParser::ListConstInitValContext *>(init_var_node);
        parse_const_init(node, var_type.array_dims, const_init_value.list_init_value);
        dbg(const_init_value.list_init_value);
    }
    Variable var(const_var_name, var_type, const_init_value);
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
    string var_name = ctx->Identifier()->getText();
    VarType var_type;
    var_type.is_const = false;
    var_type.is_array = !(ctx->constExp().size() == 0);
    var_type.is_func_args = false;
    if (var_type.is_array == true) {
        var_type.array_dims = get_array_dims(ctx->constExp());
    }
    InitValue var_init_value;
    var_init_value.is_array = var_type.is_array;
    var_init_value.is_const = var_type.is_const;
    Variable var(var_name, var_type, var_init_value);
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
    // TODO:
    string var_name = ctx->Identifier()->getText();
    VarType var_type;
    var_type.is_const = false;
    var_type.is_array = !(ctx->constExp().size() == 0);
    var_type.is_func_args = false;
    if (var_type.is_array == true) {
        var_type.array_dims = get_array_dims(ctx->constExp());
    }
    InitValue var_init_value;
    var_init_value.is_array = var_type.is_array;
    var_init_value.is_const = var_type.is_const;
    // TODO: without init value;
    Variable var(var_name, var_type, var_init_value);
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
    int32_t result = parseNum(number_str);
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
    mode = compile_time;
    int32_t result = ctx->addExp()->accept(this);
    mode = normal;
    return result;
}

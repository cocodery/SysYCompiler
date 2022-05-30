#include "ASTVisitor.hh"

ASTVisitor::ASTVisitor(CompUnit &_ir) : ir(_ir) {
    have_main_func = false;
    type = TypeVoid;
    mode = normal;
    cur_scope_elements = nullptr;
    cur_scope = nullptr;
    cur_vartable = ir.global_table;
}

// 假定所有的数组的维度的定义都是`ConstExp`
vector<int32_t> ASTVisitor::get_array_dims(vector<SysYParser::ConstExpContext *> dims) {
    vector<int32_t> array_dims;
    for (auto i : dims) {
        int32_t cur_dim = i->accept(this);
        array_dims.push_back(cur_dim);
    }
    return array_dims;
}

// 递归的对数组初始化进行分析
// int type
void ASTVisitor::parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t>& list) {
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
            list.push_back(scalar_value);
            ++cnt;
        } else {
            auto list_node = dynamic_cast<SysYParser::ListConstInitValContext *>(child);
            parse_const_init(list_node, child_array_dims, list);
            cnt += total_size / array_dims[0];
        }
    }
    while (cnt < total_size) {
        list.push_back(0);
        ++cnt;
    }
    return;
}

// float type
void ASTVisitor::parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<float>& list) {
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
            float scalar_value = scalar_node->constExp()->accept(this);
            list.push_back(scalar_value);
            ++cnt;
        } else {
            auto list_node = dynamic_cast<SysYParser::ListConstInitValContext *>(child);
            parse_const_init(list_node, child_array_dims, list);
            cnt += total_size / array_dims[0];
        }
    }
    while (cnt < total_size) {
        list.push_back(0);
        ++cnt;
    }
    dbg(list);
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
    return have_main_func;
}

antlrcpp::Any ASTVisitor::visitDecl(SysYParser::DeclContext *ctx) {
    dbg("enter Decl");
    visitChildren(ctx);
    dbg("exit Decl");
}

// 设置全局变量`type`, 这个变量仅在变量声明时起作用，结束后恢复
antlrcpp::Any ASTVisitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
    dbg("enter ConstDecl");
    DeclType last_type = type;
    type = getDeclType(ctx->children[1]->getText());
    cout << "Current Type is " << DeclTypeToStr(type) << endl;
    visitChildren(ctx);
    type = last_type;
    dbg("exit ConstDecl");
}

antlrcpp::Any ASTVisitor::visitBType(SysYParser::BTypeContext *ctx) {
    return visitChildren(ctx);
}

// 获取变量的声明，包括变量名，类型，初值
// 若有初值则会进行初始化
// 将变量插入当前作用域的符号表
antlrcpp::Any ASTVisitor::visitConstDef(SysYParser::ConstDefContext *ctx) {
    dbg("enter ConstDef");
    string var_name = ctx->children[0]->getText();
    VarType const_var;
    const_var.is_const = true;
    const_var.is_init = true;
    const_var.is_array = !(ctx->constExp().size() == 0);
    const_var.decl_type = type;
    dbg(DeclTypeToStr(const_var.decl_type));
    dbg(const_var.is_array);
    if (const_var.is_array == true) {
        const_var.array_dims = get_array_dims(ctx->constExp());
        dbg(const_var.array_dims);
    }
    // 分析`const`变量的初值
    auto init_node = ctx->constInitVal();
    if (const_var.is_array == false) {
        auto node = dynamic_cast<SysYParser::ScalarConstInitValContext *>(init_node);
        if (type == TypeInt) {
            const_var.int_scalar = node->constExp()->accept(this);
            dbg(const_var.int_scalar);
        } else if (type == TypeFloat) {
            const_var.float_scalar = node->constExp()->accept(this);
            dbg(const_var.float_scalar);
        }
    } else {
        auto node = dynamic_cast<SysYParser::ListConstInitValContext *>(init_node);
        if (type == TypeInt) {
            dbg("parsing int list");
            parse_const_init(node, const_var.array_dims, const_var.int_list);
            dbg(const_var.int_list);
        } else if (type == TypeFloat) {
            dbg("parsing float list");
            parse_const_init(node, const_var.array_dims, const_var.float_list);
            dbg(const_var.float_list);
        }
    }
    // 写入当前作用域的符号表
    cur_vartable->var_table.push_back(std::make_pair(var_name, const_var));
    dbg("exit ConstDef");
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

// 函数声明分析，获取函数声明，返回类型，参数表
// 因为不存在函数内声明函数的情况，因此直接将函数定义插入函数表
antlrcpp::Any ASTVisitor::visitFuncDef(SysYParser::FuncDefContext *ctx) {
    Function *func = new Function;
    string func_name = ctx->Identifier()->getText();
    dbg(func_name);
    FunctionInfo func_info;
    // get function name
    func_info.func_name = func_name; 
    // get function ret_type
    func_info.return_type = getDeclType(ctx->funcType()->getText());
    // parse function arguments
    if (ctx->funcFParams() != nullptr) {
        func_info.func_args = ctx->funcFParams()->accept(this).as<vector<VarType>>();
    }
    // tag we have `main function`
    if (func_name == "main") have_main_func = true;
    func->func_info = func_info;
    // parse function body
    func->main_scope = ctx->block()->accept(this);
    // push to function table
    ir.functions.push_back(func);
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitFuncType(SysYParser::FuncTypeContext *ctx) {
    dbg("Program should never reach Function visitFuncType");
    exit(EXIT_FAILURE);
}

// 遍历函数参数，将其类型记录并保存进数组
antlrcpp::Any ASTVisitor::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
    vector<VarType> func_args;
    for (auto arg: ctx->funcFParam()) {
        func_args.push_back(arg->accept(this));
    }
    return func_args;
}

// 分析单个函数参数，包括类型，是否为数组，数组的维度
antlrcpp::Any ASTVisitor::visitFuncFParam(SysYParser::FuncFParamContext *ctx) {
    VarType func_arg;
    func_arg.is_args = true;
    func_arg.decl_type = getDeclType(ctx->children[0]->getText());
    if (ctx->getText().find("[") != string::npos) {
        DeclType last_type = type;
        type = func_arg.decl_type;
        func_arg.is_array = true;
        func_arg.array_dims = get_array_dims(ctx->constExp());
        func_arg.array_dims.insert(func_arg.array_dims.begin(), -1);
        type = last_type;
    }
    // dbg(func_arg.array_dims);
    return func_arg;
}

// `Block`分析
// 将该作用域下的符号表，指令保存
antlrcpp::Any ASTVisitor::visitBlock(SysYParser::BlockContext *ctx) {
    // save `cur_scope` `cur_vartable` `cur_elements`
    Scope          *last_scope = cur_scope;
    VariableTable  *last_vartable = cur_vartable;
    vector<Info *> *last_scope_elements = cur_scope_elements;
    // process `Block` part
    Scope *block_scope = new Scope;
    block_scope->local_table = new VariableTable;
    block_scope->elements = new vector<Info *>;
    BasicBlock *scope_block = new BasicBlock;
    // cur_* point to current *
    cur_scope = block_scope;
    cur_vartable = block_scope->local_table;
    cur_scope_elements = block_scope->elements;
    cur_basicblock = scope_block;
    visitChildren(ctx);
    // restore `cur_scope` `cur_vartable` `cur_elements`
    cur_scope = last_scope;
    cur_vartable = last_vartable;
    cur_scope_elements = last_scope_elements;
    return block_scope;
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
    // if ctx->exp() == nullptr, means it's a function without return value
    ReturnInst *ret_inst = new ReturnInst(ctx->exp() != nullptr);
    if (ret_inst->has_retvalue == true) {
        
    }
    cur_basicblock->basic_block.push_back(ret_inst);
    cur_scope_elements->push_back(cur_basicblock);
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
    if (mode == compile_time) {
        return ctx->number()->accept(this);
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitNumber1(SysYParser::Number1Context *ctx) {
    dbg("enter int number");
    int int_literal = parseNum(ctx->IntLiteral()->getText().c_str());
    dbg(int_literal);
    dbg("exit int number");
    if (type == TypeInt) {
        return IRValue(type, int_literal, 0);
    } else if (type == TypeFloat) {
        return IRValue(TypeFloat, 0, float(int_literal));
    }
}

antlrcpp::Any ASTVisitor::visitNumber2(SysYParser::Number2Context *ctx) {
    dbg("enter float number");
    float float_literal = 0;
    sscanf(ctx->FloatLiteral()->getText().c_str(), "%f", &float_literal);
    dbg(float_literal);
    dbg("exit float number");
    if (type == TypeFloat) { 
        return IRValue(type, 0, float_literal);
    } else if (type == TypeInt) {
        return IRValue(TypeInt, int(float_literal), 0);
    }
}

antlrcpp::Any ASTVisitor::visitUnary1(SysYParser::Unary1Context *ctx) {
    dbg("enter unary1");
    return ctx->primaryExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitUnary2(SysYParser::Unary2Context *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitUnary3(SysYParser::Unary3Context *ctx) {
    // TODO:
    dbg("enter unary3");
    char op = ctx->unaryOp()->getText()[0];
    if (mode == compile_time) {
        IRValue rhs = ctx->unaryExp()->accept(this);
        if (op == '-') return -rhs;
        else return rhs;
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
    dbg("enter mul1");
    return ctx->unaryExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitMul2(SysYParser::Mul2Context *ctx) {
    // TODO:
    char op = ctx->children[1]->getText()[0];
    if (mode == compile_time) {
        dbg("enter mul2");
        IRValue lhs = ctx->mulExp()->accept(this);
        IRValue rhs = ctx->unaryExp()->accept(this);
        if (op == '*') return lhs * rhs;
        else if (op == '/') return lhs / rhs;
        else if (op == '%') return lhs % rhs;
        dbg("exit mul2");
    }
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitAdd1(SysYParser::Add1Context *ctx) {
    dbg("enter add1");
    return ctx->mulExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitAdd2(SysYParser::Add2Context *ctx) {
    // TODO:
    char op = ctx->children[1]->getText()[0];
    if (mode == compile_time) {
        dbg("enter add2");
        IRValue lhs = ctx->addExp()->accept(this);
        IRValue rhs = ctx->mulExp()->accept(this);
        dbg("exit add2");
        if (op == '+')  return lhs + rhs;
        else if (op == '-')  return lhs - rhs;
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
    dbg("enter constexp");
    mode = compile_time;
    IRValue result = ctx->addExp()->accept(this);
    result.type = type;
    mode = normal;
    dbg("exit constexp");
    if (result.type == TypeInt) return result.int_value;
    else if (result.type == TypeFloat) return result.float_value;
}
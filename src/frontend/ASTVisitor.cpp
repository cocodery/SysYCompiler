#include "ASTVisitor.hh"

extern const VirtReg NoRetReg;

ASTVisitor::ASTVisitor(CompUnit &_ir) : ir(_ir) {
    have_main_func = false;
    type = TypeVoid;
    mode = normal;
    cur_scope = ir.global_scope;
    cur_scope_elements = ir.global_scope->elements;
    cur_vartable = ir.global_scope->local_table;
}

// finished
// 假定所有的数组的维度的定义都是`ConstExp`
vector<int32_t> ASTVisitor::get_array_dims(vector<SysYParser::ConstExpContext *> dims) {
    DeclType last_type = type;
    type = TypeInt;
    vector<int32_t> array_dims;
    for (auto i : dims) {
        int32_t cur_dim = i->accept(this);
        array_dims.push_back(cur_dim);
    }
    type = last_type;
    return array_dims;
}

// finished
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

// finished
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

// finished
antlrcpp::Any ASTVisitor::visitChildren(antlr4::tree::ParseTree *ctx) {
    size_t n = ctx->children.size();
    for (size_t i = 0; i < n; ++i) {
        ctx->children[i]->accept(this);
    }
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitCompUnit(SysYParser::CompUnitContext *ctx) {
    visitChildren(ctx);
    return have_main_func;
}

// finished
antlrcpp::Any ASTVisitor::visitDecl(SysYParser::DeclContext *ctx) {
    cout << "enter Decl" << endl;
    visitChildren(ctx);
    cout << "exit Decl" << endl;
    return nullptr;
}

// finished
// 设置全局变量`type`, 这个变量仅在变量声明时起作用，结束后恢复
antlrcpp::Any ASTVisitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
    cout << "enter ConstDecl" << endl;
    DeclType last_type = type;
    type = getDeclType(ctx->children[1]->getText());
    cout << "Current Type is " << DeclTypeToStr(type) << endl;
    visitChildren(ctx);
    type = last_type;
    cout << "exit ConstDecl" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitBType(SysYParser::BTypeContext *ctx) {
    return visitChildren(ctx);
}

// finished
// 获取变量的声明，包括变量名，类型，初值
// 若有初值则会进行初始化
// 将变量插入当前作用域的符号表
antlrcpp::Any ASTVisitor::visitConstDef(SysYParser::ConstDefContext *ctx) {
    cout << "enter ConstDef" << endl;
    string var_name = ctx->Identifier()->getText();
    if (cur_vartable->findInCurTable(var_name)) {
        dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    Variable *const_variable = new Variable;
    VarType const_var(true, !(ctx->constExp().size() == 0), false, type);
    dbg(DeclTypeToStr(const_var.decl_type), const_var.is_array);
    if (const_var.is_array == true) {
        const_var.array_dims = get_array_dims(ctx->constExp());
        dbg(const_var.array_dims);
    }
    // 分析`const`变量的初值
    auto init_node = ctx->constInitVal();
    if (const_var.is_array == false) {
        auto node = dynamic_cast<SysYParser::ScalarConstInitValContext *>(init_node);
        if (type == TypeInt) {
            const_variable->int_scalar = node->constExp()->accept(this);
            dbg(const_variable->int_scalar);
        } else if (type == TypeFloat) {
            const_variable->float_scalar = node->constExp()->accept(this);
            dbg(const_variable->float_scalar);
        }
    } else {
        auto node = dynamic_cast<SysYParser::ListConstInitValContext *>(init_node);
        if (type == TypeInt) {
            parse_const_init(node, const_var.array_dims, const_variable->int_list);
            dbg(const_variable->int_list);
        } else if (type == TypeFloat) {
            parse_const_init(node, const_var.array_dims, const_variable->float_list);
            dbg(const_variable->float_list);
        }
    }
    const_variable->type = const_var;
    // 写入当前作用域的符号表
    if (cur_vartable->findInCurTable(var_name)) {
        dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    cur_vartable->var_table.push_back(std::make_pair(var_name, const_variable));
    cout << "exit ConstDef" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) {
    dbg("Program should never reach Function visitScalarConstInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) {
    dbg("Program should never reach Function visitListConstInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitVarDecl(SysYParser::VarDeclContext *ctx) {
    cout << "enter VarDecl" << endl;
    DeclType last_type = type;
    type = getDeclType(ctx->children[0]->getText());
    cout << "Current Type is " << DeclTypeToStr(type) << endl;
    visitChildren(ctx);
    type = last_type;
    cout << "exit VarDecl" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) {
    cout << "enter UninitVarDef" << endl;
    string var_name = ctx->Identifier()->getText();
    if (cur_vartable->findInCurTable(var_name)) {
        dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    Variable *variable = new Variable;
    VarType var(false, !(ctx->constExp().size() == 0), false, type);
    dbg(DeclTypeToStr(var.decl_type), var.is_array);
    if (var.is_array) {
        var.array_dims = get_array_dims(ctx->constExp());
        dbg(var.array_dims);
    }
    variable->type = var;
    cur_vartable->var_table.push_back(std::make_pair(var_name, variable));
    cout << "exit UninitVarDef" << endl;
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
    // TODO:
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) {
    dbg("Program should never reach Function visitScalarInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitListInitval(SysYParser::ListInitvalContext *ctx) {
    dbg("Program should never reach Function visitListInitval");
    exit(EXIT_FAILURE);
}

// finished
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

// finished
antlrcpp::Any ASTVisitor::visitFuncType(SysYParser::FuncTypeContext *ctx) {
    dbg("Program should never reach Function visitFuncType");
    exit(EXIT_FAILURE);
}

// finished
// 遍历函数参数，将其类型记录并保存进数组
antlrcpp::Any ASTVisitor::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
    vector<VarType> func_args;
    for (auto arg: ctx->funcFParam()) {
        func_args.push_back(arg->accept(this));
    }
    return func_args;
}

// finished
// 分析单个函数参数，包括类型，是否为数组，数组的维度
antlrcpp::Any ASTVisitor::visitFuncFParam(SysYParser::FuncFParamContext *ctx) {
    VarType func_arg(false, ctx->getText().find("[") != string::npos, true, getDeclType(ctx->children[0]->getText()));
    if (func_arg.is_array) {
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

// finished
// `Block`分析
// 将该作用域下的符号表，指令保存
antlrcpp::Any ASTVisitor::visitBlock(SysYParser::BlockContext *ctx) {
    cout << "enter Block" << endl;
    // save `cur_scope` `cur_vartable` `cur_elements`
    Scope          *last_scope = cur_scope;
    VariableTable  *last_vartable = cur_vartable;
    vector<Info *> *last_scope_elements = cur_scope_elements;
    // process `Block` part
    Scope *block_scope = new Scope;
    block_scope->local_table = new VariableTable;
    block_scope->elements = new vector<Info *>;
    block_scope->parent = last_scope;
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
    cout << "exit Block" << endl;
    return block_scope;
}

// finished
antlrcpp::Any ASTVisitor::visitBlockItem(SysYParser::BlockItemContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any ASTVisitor::visitAssignment(SysYParser::AssignmentContext *ctx) {
    // TODO:
    IRValue lhs = ctx->lVal()->accept(this);
    IRValue rhs = ctx->exp()->accept(this);
    if (!lhs.can_assign()) {
        cout << "reg" << lhs.reg.reg_id << " can't be assigned" << endl;
        exit(EXIT_FAILURE);
    }
    VirtReg dst = lhs.reg;
    VirtReg src = rhs.reg;
    AssignInst *ass_inst = new AssignInst(dst, src);
    cur_basicblock->basic_block.push_back(ass_inst);
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitExpStmt(SysYParser::ExpStmtContext *ctx) {
    return visitChildren(ctx);
}

// finished
antlrcpp::Any ASTVisitor::visitBlockStmt(SysYParser::BlockStmtContext *ctx) {
    cout << "enter BlockStmt" << endl;
    Scope *block_stmt = ctx->block()->accept(this);
    cur_scope->elements->push_back(block_stmt);
    cout << "exit BlockStmt" << endl;
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

// finished
antlrcpp::Any ASTVisitor::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
    // if ctx->exp() == nullptr, means it's a function without return value
    bool has_retvalue = ctx->exp() != nullptr;
    ReturnInst *ret_inst = nullptr;
    if (has_retvalue) {
        VirtReg dst = ctx->exp()->accept(this).as<IRValue>().reg;
        ret_inst = new ReturnInst(has_retvalue, dst);
    } else {
        VirtReg dst = NoRetReg;
        ret_inst = new ReturnInst(has_retvalue, dst);
    }
    cur_basicblock->basic_block.push_back(ret_inst); // 将指令加入基本块
    cur_scope_elements->push_back(cur_basicblock); // return属于跳转指令, 该基本块结束
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitExp(SysYParser::ExpContext *ctx) {
    return ctx->addExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitCond(SysYParser::CondContext *ctx) {
    mode = condition;
    // we will push instructions to block's elements
    // so we dont need do extra process
    ctx->lOrExp()->accept(this);
    mode = normal;
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitLVal(SysYParser::LValContext *ctx) {
    // TODO:
    cout << "enter LVal" << endl;
    Variable *variable = cur_scope->resolve(ctx->Identifier()->getText());
    if (mode == compile_time) { // 编译期可计算的值
        if (variable == nullptr) {
            cout << "Not find in symtable" << endl;
            exit(EXIT_FAILURE);
        }
        dbg(variable);
        // 分析要读取的数组的下标数组
        vector<int32_t> arr_idx;
        DeclType last_type = type;
        type = TypeInt;
        for (auto i: ctx->exp()) {
            CTValue idx = i->accept(this);
            arr_idx.push_back(idx.int_value);
        }
        type = last_type;
        int32_t idx = variable->type.get_index(arr_idx); // 获取下标
        CTValue ret(TypeVoid);
        if (type == TypeInt){
            if (variable->type.decl_type == TypeInt) {
                if (arr_idx.size() == 0) {
                    ret.int_value = variable->int_scalar;
                } else {
                    ret.int_value = variable->int_list[idx];
                }
            } else {
                if (arr_idx.size() == 0) {
                    ret.int_value = variable->float_scalar;
                } else {
                    ret.int_value = variable->float_list[idx];
                }
            }
        } else if (type == TypeFloat){
            if (variable->type.decl_type == TypeInt) {
                if (arr_idx.size() == 0) {
                    ret.float_value = variable->int_scalar;
                } else {
                    ret.float_value = variable->int_list[idx];
                }
            } else {
                if (arr_idx.size() == 0) {
                    ret.float_value = variable->float_scalar;
                } else {
                    ret.float_value = variable->float_list[idx];
                }
            }
        }
        ret.type = type;
        cout << "exit LVal" << endl;
        return ret;
    } else {
        VarType type = variable->type;        
        VirtReg dst = VirtReg();
        LoadAddress *lad_inst = new LoadAddress(dst, variable);
        cur_basicblock->basic_block.push_back(lad_inst);
        IRValue ret;
        if (!type.is_array) { // 如果不是数组, `ret`类型与`variable`一致
            ret = IRValue(type, dst, true);
        } else { // 如果是数组, 则需进一步分析
            
        }
        cout << "exit LVal" << endl;
        return ret;
    }
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) {
    return ctx->exp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) {
    return ctx->lVal()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) {
    CTValue src = ctx->number()->accept(this);
    if (mode == compile_time) {
        return src;
    } else {
        VirtReg dst = VirtReg();
        LoadNumber *ldc_inst = new LoadNumber(dst, src);
        cur_basicblock->basic_block.push_back(ldc_inst);
        VarType type = VarType(true, false, false, src.type);
        IRValue ret = IRValue(type, dst, false);
        return ret;
    }
}

// finished
antlrcpp::Any ASTVisitor::visitNumber1(SysYParser::Number1Context *ctx) {
    cout << "enter int number" << endl;
    int int_literal = parseNum(ctx->IntLiteral()->getText().c_str());
    dbg(int_literal);
    cout << "exit int number" << endl;
    return CTValue(TypeInt, int_literal, 0);
}

// finished
antlrcpp::Any ASTVisitor::visitNumber2(SysYParser::Number2Context *ctx) {
    cout << "enter float number" << endl;
    float float_literal = 0;
    sscanf(ctx->FloatLiteral()->getText().c_str(), "%f", &float_literal);
    dbg(float_literal);
    cout << "exit float number" << endl;
    return CTValue(TypeFloat, 0, float_literal);
}

// finished
antlrcpp::Any ASTVisitor::visitUnary1(SysYParser::Unary1Context *ctx) {
    return ctx->primaryExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitUnary2(SysYParser::Unary2Context *ctx) {
    // TODO:
    return nullptr;
}

antlrcpp::Any ASTVisitor::visitUnary3(SysYParser::Unary3Context *ctx) {
    // TODO:
    cout << "enter unary3" << endl;
    string op = ctx->unaryOp()->getText();
    if (mode == compile_time) {
        CTValue rhs = ctx->unaryExp()->accept(this);
        if (op == "-") return -rhs;
        else if (op == "!") return !rhs;
        else return rhs;
    } else if (mode == normal) {
        IRValue src = ctx->unaryExp()->accept(this);
        IRValue ret;
        if (op != "+") {
            VirtReg dst = VirtReg();
            UnaryOpInst *uop_inst = new UnaryOpInst(UnaryOp(op), dst, src.reg);
            cur_basicblock->basic_block.push_back(uop_inst);
            ret = IRValue(src.type, dst, false);
        } else {
            ret = src;
        }
        return ret;
    } else {
        if (op == "!") {

        } else {
            CompileMode last_mode = mode;
            mode = normal;
            IRValue src = ctx->unaryExp()->accept(this);
            IRValue ret;
            if (op != "+") {
                VirtReg dst = VirtReg();
                UnaryOpInst *uop_inst = new UnaryOpInst(UnaryOp(op), dst, src.reg);
                cur_basicblock->basic_block.push_back(uop_inst);
                ret = IRValue(src.type, dst, false);
            } else {
                ret = src;
            }
            mode = last_mode;
            return ret;
        }
    }
    return nullptr;
}

// finished
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

// finished
antlrcpp::Any ASTVisitor::visitMul1(SysYParser::Mul1Context *ctx) {
    return ctx->unaryExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitMul2(SysYParser::Mul2Context *ctx) {
    string op = ctx->children[1]->getText();
    if (mode == compile_time) {
        cout << "enter mul2" << endl;
        CTValue lhs = ctx->mulExp()->accept(this);
        CTValue rhs = ctx->unaryExp()->accept(this);
        cout << "exit mul2" << endl;
        if (op == "*") return lhs * rhs;
        else if (op == "/") return lhs / rhs;
        else if (op == "%") return lhs % rhs;
    } else {
        mode = normal;
        CompileMode last_mode = mode;
        IRValue src1 = ctx->mulExp()->accept(this);
        IRValue src2 = ctx->unaryExp()->accept(this);
        mode = last_mode;
        VirtReg dst = VirtReg();
        BinaryOpInst *bin_inst = new BinaryOpInst(BinaryOp(op), dst, src1.reg, src2.reg);
        cur_basicblock->basic_block.push_back(bin_inst);
        VarType ret_type = VarType(false, false, false, TypeVoid);
        if (src1.type.decl_type == src2.type.decl_type) {
            ret_type.decl_type = src1.type.decl_type;
        } else {
            ret_type.decl_type = TypeFloat;
            bin_inst->need_cast = true;
        }
        ret_type.is_const = (src1.type.is_const && src2.type.is_const) ? true : false;
        IRValue ret = IRValue(ret_type, dst, false);
        return ret;
    }
}

// finished
antlrcpp::Any ASTVisitor::visitAdd1(SysYParser::Add1Context *ctx) {
    return ctx->mulExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitAdd2(SysYParser::Add2Context *ctx) {
    string op = ctx->children[1]->getText();
    if (mode == compile_time) {
        cout << "enter add2" << endl;
        CTValue lhs = ctx->addExp()->accept(this);
        CTValue rhs = ctx->mulExp()->accept(this);
        cout << "exit add2" << endl;
        if (op == "+")  return lhs + rhs;
        else if (op == "-")  return lhs - rhs;
    } else {
        mode = normal;
        CompileMode last_mode = mode;
        IRValue src1 = ctx->addExp()->accept(this);
        IRValue src2 = ctx->mulExp()->accept(this);
        mode = last_mode;
        VirtReg dst = VirtReg();
        BinaryOpInst *bin_inst = new BinaryOpInst(BinaryOp(op), dst, src1.reg, src2.reg);
        cur_basicblock->basic_block.push_back(bin_inst);
        VarType ret_type = VarType(false, false, false, TypeVoid);
        if (src1.type.decl_type == src2.type.decl_type) {
            ret_type.decl_type = src1.type.decl_type;
        } else {
            ret_type.decl_type = TypeFloat;
            bin_inst->need_cast = true;
        }
        ret_type.is_const = (src1.type.is_const && src2.type.is_const) ? true : false;
        IRValue ret = IRValue(ret_type, dst, false);
        return ret;
    }
}

// finished
antlrcpp::Any ASTVisitor::visitRel1(SysYParser::Rel1Context *ctx) {
    return ctx->addExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitRel2(SysYParser::Rel2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitEq1(SysYParser::Eq1Context *ctx) {
    return ctx->relExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitEq2(SysYParser::Eq2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitLAnd1(SysYParser::LAnd1Context *ctx) {
    return ctx->eqExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitLAnd2(SysYParser::LAnd2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitLOr1(SysYParser::LOr1Context *ctx) {
    return ctx->lAndExp()->accept(this);
}

antlrcpp::Any ASTVisitor::visitLOr2(SysYParser::LOr2Context *ctx) {
    // TODO:
    string op = ctx->children[1]->getText();
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitConstExp(SysYParser::ConstExpContext *ctx) {
    cout << "enter constexp" << endl;
    mode = compile_time;
    CTValue result = ctx->addExp()->accept(this);
    result.type = type;
    mode = normal;
    cout << "exit constexp" << endl;
    if (result.type == TypeInt) {
        return result.int_value;
    } else if (result.type == TypeFloat) {
        return result.float_value;
    }
}

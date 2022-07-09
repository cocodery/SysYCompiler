#include "ASTVisitor.hh"

ASTVisitor::ASTVisitor(CompUnit &_ir) : ir(_ir) {
    have_main_func = false;
    type = TypeVoid;
    var_idx = 1;
    cur_scope = ir.global_scope;
    cur_scope_elements = ir.global_scope->elements;
    cur_vartable = ir.global_scope->local_table;
    glbvar_init_bb = new BasicBlock;
    cur_basicblock = glbvar_init_bb;
    // ir.global_scope->elements->push_back(glbvar_init_bb);
}

// finished
// 假定所有的数组的维度的定义都是`ConstExp`
vector<int32_t> ASTVisitor::get_array_dims(vector<SysYParser::ConstExpContext *> dims) {
    DeclType last_type = type;
    vector<int32_t> array_dims;
    for (auto i : dims) {
        SRC cur_dim = i->accept(this);
        CTValue *ctv = cur_dim.ToCTValue();
        assert(ctv != nullptr);
        array_dims.push_back(ctv->int_value);
    }
    return array_dims;
}

// finished
// 递归的对数组初始化进行分析
void ASTVisitor::parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t>& ilist, vector<float>& flist) {
    int32_t total_size = 1; // 当前初始化维度的`size`
    for (auto i: array_dims) {
        total_size *= i;
    }
    if (total_size == 0) return; // 如果`size`为 0 则不需要初始化
    int32_t cnt = 0;
    for (auto child: node->constInitVal()) { // 对子结点进行初始化
        // 如果是标量, 计算`constExp`初值, 填入初始化列表
        if (auto scalar_node = dynamic_cast<SysYParser::ScalarConstInitValContext *>(child); scalar_node != nullptr) {
            SRC scalar_value = scalar_node->constExp()->accept(this);
            CTValue *ctv = scalar_value.ToCTValue();
            assert(ctv != nullptr);
            ilist.push_back(ctv->int_value);
            flist.push_back(ctv->float_value);
            ++cnt;
        } else { // 如果是向量, 递归的初始化
            vector<int32_t> child_array_dims = array_dims; // 子结点的维度
            child_array_dims.erase(child_array_dims.begin());
            auto list_node = dynamic_cast<SysYParser::ListConstInitValContext *>(child);
            parse_const_init(list_node, child_array_dims, ilist, flist);
            cnt += total_size / array_dims[0];
        }
    }
    // 如果初始化的个数少于当前维度, 则填充 0
    while (cnt < total_size) {
        ilist.push_back(0);
        flist.push_back(0);
        ++cnt;
    }
    return;
}

void ASTVisitor::parse_variable_init(SysYParser::ListInitvalContext *node, const vector<int32_t> &array_dims, VirtReg addr, int32_t off) {
    /*
    int32_t total_size = 1; // 当前维度需要初始化元素的个数
    for (auto i: array_dims) {
        total_size *= i;
    }
    if (total_size == 0) return;
    vector<int32_t> child_array_dims = array_dims; // 子结点的维度
    child_array_dims.erase(child_array_dims.begin());
    int32_t cnt = 0;
    for (auto child: node->initVal()) {
        if (auto scalar_node = dynamic_cast<SysYParser::ScalarInitValContext *>(child); scalar_node != nullptr) {
            CTValue off_v = CTValue(TypeInt, off, 0);
            VirtReg off_r = VirtReg(var_idx++);
            LoadNumber *ldc_off_inst = new LoadNumber(off_r, off_v);
            cur_basicblock->basic_block.push_back(ldc_off_inst);
            VirtReg addr_off = VirtReg(var_idx++);
            LoadOffset *ldo_inst = new LoadOffset(addr_off, addr, off_r, 1);
            cur_basicblock->basic_block.push_back(ldo_inst);
            IRValue value_v = scalar_node->exp()->accept(this);
            VirtReg value_r = value_v.reg;
            StoreMem *stm_inst = new StoreMem(addr_off, value_r);
            cur_basicblock->basic_block.push_back(stm_inst);
            off += 1;
            cnt += 1;
        } else {
            auto list_node = dynamic_cast<SysYParser::ListInitvalContext *>(child);
            parse_variable_init(list_node, child_array_dims, addr, off);
            cnt += total_size / array_dims[0];
            off += total_size / array_dims[0];
        }
    }
    // 加载 0 到寄存器, 防止每次初始化需要读取
    CTValue zero_v = CTValue(type, 0, 0);
    VirtReg zero_r = VirtReg(var_idx++); // 存储 0
    LoadNumber *ldc_inst = new LoadNumber(zero_r, zero_v);
    cur_basicblock->basic_block.push_back(ldc_inst);
    // 将剩余未初始化的赋值为 0
    while (cnt < total_size) {
        // 存储偏移量
        VirtReg off_r = VirtReg(var_idx++); 
        CTValue off_v = CTValue(TypeInt, off, 0);
        LoadNumber *ldc_off_inst = new LoadNumber(off_r, off_v);
        cur_basicblock->basic_block.push_back(ldc_off_inst);
        // 存储加上偏移量后的地址
        VirtReg addr_off = VirtReg(var_idx++); 
        LoadOffset *ldo_inst = new LoadOffset(addr_off, addr, off_r, 1);
        cur_basicblock->basic_block.push_back(ldo_inst);
        // 存储值到地址
        StoreMem *stm_inst = new StoreMem(addr_off, zero_r);
        cur_basicblock->basic_block.push_back(stm_inst);
        off += 1;
        cnt += 1;
    }
    */
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
    dbg(var_name);
    if (cur_vartable->findInCurTable(var_name)) {
        dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    Variable *const_variable = new Variable(var_idx++);
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
        SRC scalar = node->constExp()->accept(this);
        CTValue *ctv = scalar.ToCTValue();
        assert(ctv != nullptr);
        const_variable->int_scalar = ctv->int_value;
        const_variable->float_scalar = ctv->float_value;
        dbg(const_variable->int_scalar, const_variable->float_scalar);
    } else {
        auto node = dynamic_cast<SysYParser::ListConstInitValContext *>(init_node);
        parse_const_init(node, const_var.array_dims, const_variable->int_list, const_variable->float_list);
        dbg(const_variable->int_list, const_variable->float_list);
    }
    const_variable->type = const_var;
    // 写入当前作用域的符号表
    if (cur_vartable->findInCurTable(var_name)) {
        dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    cur_vartable->var_table.push_back(std::make_pair(var_name, const_variable));
    // 生成 LLIR
    VirtReg *reg = new VirtReg(const_variable->var_idx, const_variable->type.decl_type);
    LLIR_ALLOCA *alloc_inst = new LLIR_ALLOCA(SRC(reg), const_variable);
    cur_basicblock->basic_block.push_back(alloc_inst);
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
    Variable *variable = new Variable(var_idx++);
    VarType var(false, !(ctx->constExp().size() == 0), false, type);
    dbg(DeclTypeToStr(var.decl_type), var.is_array);
    if (var.is_array) {
        var.array_dims = get_array_dims(ctx->constExp());
        dbg(var.array_dims);
    }
    variable->type = var;
    cur_vartable->var_table.push_back(std::make_pair(var_name, variable));
    VirtReg *reg = new VirtReg(variable->var_idx, variable->type.decl_type);
    LLIR_ALLOCA *alloc_inst = new LLIR_ALLOCA(SRC(reg), variable);
    cur_basicblock->basic_block.push_back(alloc_inst);
    cout << "exit UninitVarDef" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
    cout << "enter InitVarDef" << endl;
    // push to variable table, similar with `UninitVarDef`
    string var_name = ctx->Identifier()->getText();
    if (cur_vartable->findInCurTable(var_name)) {
        dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    Variable *variable = new Variable(var_idx++);
    VarType var(false, !(ctx->constExp().size() == 0), false, type);
    dbg(DeclTypeToStr(var.decl_type), var.is_array);
    if (var.is_array) {
        var.array_dims = get_array_dims(ctx->constExp());
        dbg(var.array_dims);
    }
    variable->type = var;
    cur_vartable->var_table.push_back(std::make_pair(var_name, variable));
    // parse `InitVarDef`
    // init global variable before excuting main function
    // init local variable we it first exsit
    // we make sure that all variable don't init at Variable->init_value, but get value via access memory
    // 不管是标量还是向量, 初始化时都会用到首地址
    /*
    VirtReg addr = VirtReg(var_idx++);
    LoadAddress *ldv_inst = new LoadAddress(addr, variable);
    cur_basicblock->basic_block.push_back(ldv_inst);
    auto init_node = ctx->initVal();
    if (var.is_array == false) {
        auto scalar_init = dynamic_cast<SysYParser::ScalarInitValContext *>(init_node);
        IRValue src = scalar_init->exp()->accept(this);
        StoreMem* stm_inst = new StoreMem(addr, src.reg);
        cur_basicblock->basic_block.push_back(stm_inst);
    } else {
        auto node = dynamic_cast<SysYParser::ListInitvalContext *>(init_node);
        parse_variable_init(node, var.array_dims, addr, 0);
    }
    */
    VirtReg *reg = new VirtReg(variable->var_idx, variable->type.decl_type);
    LLIR_ALLOCA *alloc_inst = new LLIR_ALLOCA(SRC(reg), variable);
    cur_basicblock->basic_block.push_back(alloc_inst);
    cout << "exit InitVarDef" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) {
    IRValue ret = ctx->exp()->accept(this);
    return ret;
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
    cout << "enter FuncDef" << endl;
    Function *func = new Function;
    string func_name = ctx->Identifier()->getText();
    dbg(func_name);
    FunctionInfo func_info;
    // get function name
    func_info.func_name = func_name; 
    // get function ret_type
    ret_type = getDeclType(ctx->funcType()->getText());
    func_info.return_type = ret_type;
    // parse function arguments
    if (ctx->funcFParams() != nullptr) {
        func_info.func_args = ctx->funcFParams()->accept(this).as<vector<pair<string, VarType>>>();
    }
    // tag we have `main function`
    if (func_name == "main") have_main_func = true;
    func->func_info = func_info;
    // reset variable idx in function
    var_idx = func_info.func_args.size() == 0 ? 1 : func_info.func_args.size();
    dbg(var_idx);
    // parse function body
    func->main_scope = ctx->block()->accept(this);
    // push to function table
    ir.functions.push_back(func);
    cur_basicblock = glbvar_init_bb;
    cout << "exit FuncDef" << endl;
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
    vector<pair<string, VarType>> func_args;
    int32_t idx = 0;
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
    return make_pair(ctx->Identifier()->getText(), func_arg);
}

// finished
// `Block`分析
// 将该作用域下的符号表，指令保存
antlrcpp::Any ASTVisitor::visitBlock(SysYParser::BlockContext *ctx) {
    cout << "enter Block" << endl;
    // 将上个作用域的基本块push进来
    cur_scope_elements->push_back(cur_basicblock);
    // save `cur_scope` `cur_vartable` `cur_elements`
    Scope          *last_scope = cur_scope;
    VariableTable  *last_vartable = cur_vartable;
    vector<Info *> *last_scope_elements = cur_scope_elements;
    // process `Block` part
    Scope *block_scope = new Scope;
    block_scope->local_table = new VariableTable;
    block_scope->elements = new vector<Info *>;
    block_scope->parent = last_scope;
    // cur_* point to current *
    cur_scope = block_scope;
    cur_vartable = block_scope->local_table;
    cur_scope_elements = block_scope->elements;
    cur_basicblock = new BasicBlock;
    visitChildren(ctx);
    // 新的基本块到右括号就结束了, push
    cur_scope_elements->push_back(cur_basicblock);
    // restore `cur_scope` `cur_vartable` `cur_elements`
    cur_scope = last_scope;
    cur_vartable = last_vartable;
    cur_scope_elements = last_scope_elements;
    // 新的基本块
    cur_basicblock = new BasicBlock;
    cout << "exit Block" << endl;
    return block_scope;
}

// finished
antlrcpp::Any ASTVisitor::visitBlockItem(SysYParser::BlockItemContext *ctx) {
    return visitChildren(ctx);
}

// finished
antlrcpp::Any ASTVisitor::visitAssignment(SysYParser::AssignmentContext *ctx) {
    SRC lhs = ctx->lVal()->accept(this);
    SRC rhs = ctx->exp()->accept(this);
    VirtReg *lhs_reg = lhs.ToVirtReg();
    VirtReg *rhs_reg = rhs.ToVirtReg();
    assert(lhs_reg->assign == true);
    LLIR_STORE *store_inst = new LLIR_STORE(lhs, rhs);
    cur_basicblock->basic_block.push_back(store_inst);
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
    return block_stmt;
}

// finished
antlrcpp::Any ASTVisitor::visitIfStmt1(SysYParser::IfStmt1Context *ctx) {
    cout << "enter IfStmt1" << endl;
    // if condition
    IRValue cond = ctx->cond()->accept(this);
    JzeroInst *jmp_inst = new JzeroInst(cond.reg);
    cur_basicblock->basic_block.push_back(jmp_inst);
    // if stmt body
    if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()); node != nullptr) {
        dbg("Block Stmt Context");
        Scope *block_stmt = node->accept(this);
    } else {
        dbg("Other Stmt Context");
        cur_scope_elements->push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope;
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock;

        ctx->stmt()->accept(this);
        cur_scope_elements->push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock;
    }
    jmp_inst->bb_idx = cur_basicblock->bb_idx;
    cout << "exit IfStmt1" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitIfStmt2(SysYParser::IfStmt2Context *ctx) {
    cout << "enter IfStmt1" << endl;
    int32_t else_begin_bb = 0;
    int32_t else_end_bb = 0;
    IRValue cond = ctx->cond()->accept(this);
    // 条件为假跳转到`else`
    JzeroInst *jmp_else_begin = new JzeroInst(cond.reg);
    cur_basicblock->basic_block.push_back(jmp_else_begin);
    // if-stmt body
    // 完成后跳转到`else`的下一个基本块
    JumpInst *jmp_else_end = new JumpInst();
    if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()[0]); node != nullptr) {
        dbg("Block Stmt Context");
        Scope *block_stmt = node->accept(this);
        BasicBlock *last_bb_of_stmt = block_stmt->get_last_bb();
        last_bb_of_stmt->basic_block.push_back(jmp_else_end); // 跳转到 else 的下一块
    } else {
        dbg("Other Stmt Context");
        cur_scope_elements->push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope;
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock;

        ctx->stmt()[0]->accept(this);
        cur_basicblock->basic_block.push_back(jmp_else_end); // 跳转到 else 的下一块
        cur_scope_elements->push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock;
    }
    // else-stmt body
    if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()[1]); node != nullptr) {
        dbg("Block Stmt Context");
        Scope *block_stmt = node->accept(this);
        jmp_else_begin->bb_idx = dynamic_cast<BasicBlock *>(*block_stmt->elements->begin())->bb_idx;
    } else {
        dbg("Other Stmt Context");
        cur_scope_elements->push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope;
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock;
        jmp_else_begin->bb_idx = cur_basicblock->bb_idx;

        ctx->stmt()[1]->accept(this);
        cur_scope_elements->push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock;
    }
    jmp_else_end->bb_idx = cur_basicblock->bb_idx;
    cout << "exit IfStmt1" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitWhileStmt(SysYParser::WhileStmtContext *ctx) {
    cout << "enter While" << endl;
    cur_scope_elements->push_back(cur_basicblock);
    // 将while条件单独作为一个基本块
    cur_basicblock = new BasicBlock;
    // save continue_target
    int32_t last_continue_target = continue_target;
    continue_target = cur_basicblock->bb_idx;
    dbg(continue_target);
    // save break_inst
    vector<JumpInst *> last_break_insts = break_insts;
    break_insts = vector<JumpInst *>();
    // while condition
    IRValue cond = ctx->cond()->accept(this);
    JzeroInst *jzo_inst = new JzeroInst(cond.reg);
    cur_basicblock->basic_block.push_back(jzo_inst);
    // if stmt body
    if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()); node != nullptr) {
        // 在这里遇到了`Block`作为循环体的`While`
        // 但是无法在生成`Block`的中途插入跳转语句
        // 因此我们要进入`Block`中找到其最后一个`basic_block`
        // 并插入`Jump_Inst`
        dbg("Block Stmt Context");
        JumpInst *jmp_inst = new JumpInst(continue_target);
        Scope *block_stmt = node->accept(this);
        BasicBlock *last_bb_of_stmt = block_stmt->get_last_bb();
        last_bb_of_stmt->basic_block.push_back(jmp_inst);
    } else {
        dbg("Other Stmt Context");
        cur_scope_elements->push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope;
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock;

        ctx->stmt()->accept(this);
        JumpInst *jmp_inst = new JumpInst(continue_target);
        cur_basicblock->basic_block.push_back(jmp_inst);
        cur_scope_elements->push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock;
    }
    // modify break target
    for (int i = 0; i < break_insts.size(); ++i) {
        break_insts[i]->bb_idx = cur_basicblock->bb_idx;
    }
    // restore continue and break
    continue_target = last_continue_target;
    break_insts = last_break_insts;
    jzo_inst->bb_idx = cur_basicblock->bb_idx;
    cout << "exit While" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitBreakStmt(SysYParser::BreakStmtContext *ctx) {
    JumpInst *jmp_inst = new JumpInst();
    cur_basicblock->basic_block.push_back(jmp_inst);
    break_insts.push_back(jmp_inst);
    cur_scope_elements->push_back(cur_basicblock);
    cur_basicblock = new BasicBlock;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitContinueStmt(SysYParser::ContinueStmtContext *ctx) {
    cout << "enter ContinueStmt" << endl;
    JumpInst *jmp_inst = new JumpInst(continue_target);
    cur_basicblock->basic_block.push_back(jmp_inst);
    cur_scope_elements->push_back(cur_basicblock);
    cur_basicblock = new BasicBlock;
    cout << "exit ContinueStmt" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
    cout << "enter visitReturnStmt" << endl;
    // if ctx->exp() == nullptr, means it's a function without return value
    bool has_retvalue = (ctx->exp() != nullptr);
    LLIR_RET *ret_inst = nullptr;
    if (has_retvalue) {
        SRC dst = ctx->exp()->accept(this);
        if (CTValue *ctv = dst.ToCTValue(); ctv != nullptr) {
            ctv->type = ret_type;
            dst = SRC(ctv);
        } else {
            VirtReg *reg = dst.ToVirtReg();
            if (reg->type != ret_type) {
                VirtReg *dst_reg = new VirtReg(var_idx++, ret_type);
                if (reg->type == TypeInt) {
                    LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(dst_reg), SRC(dst));
                    cur_basicblock->basic_block.push_back(itf_inst);
                } else if (reg->type == TypeFloat) {
                    LLIR_FPTOSI *fti_inst = new LLIR_FPTOSI(SRC(dst_reg), SRC(dst));
                    cur_basicblock->basic_block.push_back(fti_inst);
                    dst = SRC(dst_reg);
                } else {
                    dbg("UnExpected Function Return Type");
                    exit(EXIT_FAILURE);
                }
                dst = SRC(dst_reg);
            }
        }
        ret_inst = new LLIR_RET(has_retvalue, dst);
    } else {
        ret_inst = new LLIR_RET(has_retvalue, SRC());
    }
    cur_basicblock->basic_block.push_back(ret_inst); // 将指令加入基本块
    cur_scope_elements->push_back(cur_basicblock); // return属于跳转指令, 该基本块结束
    cur_basicblock = new BasicBlock;
    cout << "exit visitReturnStmt" << endl;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitExp(SysYParser::ExpContext *ctx) {
    return ctx->addExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitCond(SysYParser::CondContext *ctx) {
    return nullptr;
}

// finished
// 进入这个产生式并不知道他将作为左值还是右值
// 所以在这里我们仅返回存储使用的变量的地址
antlrcpp::Any ASTVisitor::visitLVal(SysYParser::LValContext *ctx) {
    cout << "enter LVal" << endl;
    Variable *variable = cur_scope->resolve(ctx->Identifier()->getText());
    assert(variable != nullptr);
    /* // 暂不处理数组的情况
    vector<SRC> arr_dims;
    for (auto dim: ctx->exp()) {
        arr_dims.push_back(dim->accept(this));
    }

    if (variable->type.is_const) {
        if (variable->type.is_array == false) {
            CTValue *ctv = new CTValue(variable->type.decl_type, variable->int_scalar, variable->float_scalar);
            ret = SRC(ctv);
        } else {

        }
    } else {
        if (variable->type.is_array == false) {

        } else {

        }
    }
    */
    cout << "exit visitLVal" << endl;
    if (variable->type.is_array == false) {
        VirtReg *reg = new VirtReg(variable->var_idx, variable->type.decl_type);
        return SRC(reg);
    }
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) {
    return ctx->exp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) {
    SRC src = ctx->lVal()->accept(this);
    VirtReg *src_reg = src.ToVirtReg();
    VirtReg *dst_reg = new VirtReg(var_idx++, src_reg->type);
    SRC dst = SRC(dst_reg);
    LLIR_LOAD *load_inst = new LLIR_LOAD(dst, src);
    cur_basicblock->basic_block.push_back(load_inst);
    return dst;
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) {
    cout << "enter visitPrimaryExp3" << endl;
    SRC src = ctx->number()->accept(this);
    cout << "exit visitPrimaryExp3" << endl;
    return src;
}

// finished
antlrcpp::Any ASTVisitor::visitNumber1(SysYParser::Number1Context *ctx) {
    int32_t int_literal = parseNum(ctx->IntLiteral()->getText().c_str());
    CTValue *ret = new CTValue(TypeInt, int_literal, int_literal);
    return SRC(ret);
}

// finished
antlrcpp::Any ASTVisitor::visitNumber2(SysYParser::Number2Context *ctx) {
    float float_literal = 0;
    sscanf(ctx->FloatLiteral()->getText().c_str(), "%a", &float_literal);
    CTValue *ret = new CTValue(TypeFloat, float_literal, float_literal);
    return SRC(ret);
}

// finished
antlrcpp::Any ASTVisitor::visitUnary1(SysYParser::Unary1Context *ctx) {
    cout << "enter visitUnary1" << endl;
    SRC ret = ctx->primaryExp()->accept(this);
    dbg(ret.ToString());
    cout << "exit visitUnary1" << endl;
    return ret;
}

// finished
antlrcpp::Any ASTVisitor::visitUnary2(SysYParser::Unary2Context *ctx) {
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitUnary3(SysYParser::Unary3Context *ctx) {
    cout << "enter unary3" << endl;
    string op = ctx->unaryOp()->getText();
    SRC src = ctx->unaryExp()->accept(this);
    if (CTValue *ctv = src.ToCTValue(); ctv != nullptr) {
        // 当`src`是`CTValue`时
        // 可以直接计算, 无关所处状态
        if (op == "-") {
            CTValue *uop_ctv = new CTValue(ctv->type, -ctv->int_value, -ctv->float_value);
            return SRC(uop_ctv);
        } else if (op == "+") {
            return SRC(ctv);
        } else {
            CTValue *uop_ctv = new CTValue(TypeInt, !ctv->int_value, !ctv->float_value);
            return SRC(uop_ctv);
        }
    } else {
        VirtReg *reg = src.ToVirtReg();
        if (op == "-" || op == "!") {
            DeclType _type = reg->type;
            CTValue *zero = new CTValue(_type, 0, 0);
            VirtReg *dst = new VirtReg(var_idx++, _type);
            LLIR_BIN *bin_inst = new LLIR_BIN(SUB, dst, SRC(zero), SRC(reg));
            cur_basicblock->basic_block.push_back(bin_inst);
            return SRC(dst);
        } else {
            return SRC(reg);
        }
    }
}

// finished
antlrcpp::Any ASTVisitor::visitUnaryOp(SysYParser::UnaryOpContext *ctx) {
    dbg("Program should never reach Function visitListConstInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitFuncRParams(SysYParser::FuncRParamsContext *ctx) {
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitFuncRParam(SysYParser::FuncRParamContext *ctx) {
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitMul1(SysYParser::Mul1Context *ctx) {
    return ctx->unaryExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitMul2(SysYParser::Mul2Context *ctx) {
    cout << "enter visitMul2" << endl;
    string op = ctx->children[1]->getText();
    SRC lhs = ctx->mulExp()->accept(this);
    SRC rhs = ctx->unaryExp()->accept(this);
    // 当两个操作数都是`CTValue`
    if (CTValue *ctv1 = lhs.ToCTValue(), *ctv2 = rhs.ToCTValue(); ctv1 != nullptr && ctv2 != nullptr) {
        DeclType _type = ctv1->type;
        int imul = 0;
        float fmul = 0;
        if (op == "*") {
            imul = ctv1->int_value * ctv2->int_value;
            fmul = ctv1->float_value * ctv2->float_value;
            if (ctv1->type == TypeFloat || ctv2->type == TypeFloat) {
                imul = fmul;
            }
        } else if (op == "/") {
            imul = ctv1->int_value / ctv2->int_value;
            fmul = ctv1->float_value / ctv2->float_value;
            if (ctv1->type == TypeFloat || ctv2->type == TypeFloat) {
                imul = fmul;
            }
        } else {
            imul = ctv1->int_value % ctv2->int_value;
            fmul = imul;
        }
        if (ctv1->type != ctv2->type) {
            _type = TypeFloat;
        }
        CTValue *mul = new CTValue(_type, imul, fmul);
        dbg(mul->ToString());
        cout << "exit visitMul2 with 2 CTValue" << endl;
        return SRC(mul);
    } else { // 当其中至少有一个是`VirtReg`
        // 暂不处理类型不匹配情况
        VirtReg *reg1 = lhs.ToVirtReg();
        VirtReg *reg2 = rhs.ToVirtReg();
        SRC dst = SRC(new VirtReg(var_idx++, reg1->type));
        LLIR_BIN *bop_inst = nullptr;
        LLIR_FBIN *fbop_inst = nullptr;
        if (!ctv1 && ctv2 && reg1 && !reg2) { // lhs -> VirtReg, rhs -> CTValue
            if (reg1->type != ctv2->type) {

            }
            if (reg1->type == TypeInt) {
                bop_inst = new LLIR_BIN(StrToBinOp(op), dst, SRC(reg1), SRC(ctv2));
            } else {
                fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, SRC(reg1), SRC(ctv2));
            }
        } else if (ctv1 && !ctv2 && !reg1 && reg2) { // lhs -> CTValue, rhs -> VirtReg
            if (ctv1->type != reg2->type) {

            }
            if (reg1->type == TypeInt) {
                bop_inst = new LLIR_BIN(StrToBinOp(op), dst, SRC(ctv1), SRC(reg2));
            } else {
                fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, SRC(ctv1), SRC(reg2));
            }
        } else if (!ctv1 && !ctv2 && reg1 && reg2) { // lhs -> VirtReg, rhs -> VirtReg
            if (reg1->type != reg2->type) {

            }
            if (reg1->type == TypeInt) {
                bop_inst = new LLIR_BIN(StrToBinOp(op), dst, SRC(reg1), SRC(reg2));
            } else {
                fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, SRC(reg1), SRC(reg2));
            }
        }
        if (bop_inst != nullptr) {
            cur_basicblock->basic_block.push_back(bop_inst);
        } else {
            cur_basicblock->basic_block.push_back(fbop_inst);
        }
        return dst;
    }
}

// finished
antlrcpp::Any ASTVisitor::visitAdd1(SysYParser::Add1Context *ctx) {
    return ctx->mulExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitAdd2(SysYParser::Add2Context *ctx) {
    string op = ctx->children[1]->getText();
    SRC lhs = ctx->addExp()->accept(this);
    SRC rhs = ctx->mulExp()->accept(this);
    // 当两个操作数都是`CTValue`
    if (CTValue *ctv1 = lhs.ToCTValue(), *ctv2 = rhs.ToCTValue(); ctv1 != nullptr && ctv2 != nullptr) {
        DeclType _type = ctv1->type;
        int iadd = 0;
        float fadd = 0;
        if (op == "+") {
            iadd = ctv1->int_value + ctv2->int_value;
            fadd = ctv1->float_value + ctv2->float_value;
            if (ctv1->type == TypeFloat || ctv2->type == TypeFloat) {
                iadd = fadd;
            }
        } else if (op == "-") {
            iadd = ctv1->int_value - ctv2->int_value;
            fadd = ctv1->float_value - ctv2->float_value;
            if (ctv1->type == TypeFloat || ctv2->type == TypeFloat) {
                iadd = fadd;
            }
        }
        if (ctv1->type != ctv2->type) {
            _type = TypeFloat;
        }
        CTValue *add = new CTValue(_type, iadd, fadd);
        dbg(add->ToString());
        return SRC(add);
    } else { // 当其中至少有一个是`VirtReg`
        // 暂不处理类型不匹配情况
        VirtReg *reg1 = lhs.ToVirtReg();
        VirtReg *reg2 = rhs.ToVirtReg();
        SRC dst = SRC(new VirtReg(var_idx++, reg1->type));
        LLIR_BIN *bop_inst = nullptr;
        LLIR_FBIN *fbop_inst = nullptr;
        if (!ctv1 && ctv2 && reg1 && !reg2) { // lhs -> VirtReg, rhs -> CTValue
            if (reg1->type != ctv2->type) {

            }
            if (reg1->type == TypeInt) {
                bop_inst = new LLIR_BIN(StrToBinOp(op), dst, SRC(reg1), SRC(ctv2));
            } else {
                fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, SRC(reg1), SRC(ctv2));
            }
        } else if (ctv1 && !ctv2 && !reg1 && reg2) { // lhs -> CTValue, rhs -> VirtReg
            if (ctv1->type != reg2->type) {

            }
            if (reg1->type == TypeInt) {
                bop_inst = new LLIR_BIN(StrToBinOp(op), dst, SRC(ctv1), SRC(reg2));
            } else {
                fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, SRC(ctv1), SRC(reg2));
            }
        } else if (!ctv1 && !ctv2 && reg1 && reg2) { // lhs -> VirtReg, rhs -> VirtReg
            if (reg1->type != reg2->type) {

            }
            if (reg1->type == TypeInt) {
                bop_inst = new LLIR_BIN(StrToBinOp(op), dst, SRC(reg1), SRC(reg2));
            } else {
                fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, SRC(reg1), SRC(reg2));
            }
        }
        if (bop_inst != nullptr) {
            cur_basicblock->basic_block.push_back(bop_inst);
        } else {
            cur_basicblock->basic_block.push_back(fbop_inst);
        }
        return dst;
    }
}

// finished
antlrcpp::Any ASTVisitor::visitRel1(SysYParser::Rel1Context *ctx) {
    return ctx->addExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitRel2(SysYParser::Rel2Context *ctx) {
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitEq1(SysYParser::Eq1Context *ctx) {
    return ctx->relExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitEq2(SysYParser::Eq2Context *ctx) {
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitLAnd1(SysYParser::LAnd1Context *ctx) {
    return ctx->eqExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitLAnd2(SysYParser::LAnd2Context *ctx) {
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitLOr1(SysYParser::LOr1Context *ctx) {
    return ctx->lAndExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitLOr2(SysYParser::LOr2Context *ctx) {
    return nullptr;
}

// finished
// 返回表达式的类型由type限定, 存在隐患
antlrcpp::Any ASTVisitor::visitConstExp(SysYParser::ConstExpContext *ctx) {
    cout << "enter constexp" << endl;
    SRC result = ctx->addExp()->accept(this);
    cout << "exit constexp" << endl;
    return result;
}

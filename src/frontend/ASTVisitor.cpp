#include "ASTVisitor.hh"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-pointer-compare"

ASTVisitor::ASTVisitor(CompUnit &_ir) : ir(_ir) {
    have_main_func = false;
    cur_type = TypeVoid;
    var_idx = 1;
    glb_var_idx = 1;
    bb_idx = 1;
    sp_idx = 1;
    init_args = false;
    loop_mode = false;
    cur_scope = ir.global_scope;
    cur_scope_elements = ir.global_scope->elements;
    cur_vartable = ir.global_scope->local_table;
    cur_basicblock = nullptr;
    cur_func = nullptr;
    // ir.global_scope->elements->push_back(glbvar_init_bb);
}

// finished
// 假定所有的数组的维度的定义都是`ConstExp`
vector<int32_t> ASTVisitor::get_array_dims(vector<SysYParser::ConstExpContext *> dims) {
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
void ASTVisitor::parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t>& ilist, vector<double>& flist) {
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

void ASTVisitor::parse_variable_init(SysYParser::ListInitvalContext *node, VarType _type, 
vector<int32_t> arr_dim, int32_t off, vector<pair<int32_t, SRC>> &initTable) {
    int32_t total_size = 1;
    for (int i = 0; i < arr_dim.size(); ++i) {
        total_size = total_size * arr_dim[i];
    }
    vector<int32_t> child_array_dims = arr_dim; // 子结点的维度
    child_array_dims.erase(child_array_dims.begin());
    int32_t cnt = 0;
    for (auto child : node->initVal()) {
        if (auto scalar_node = dynamic_cast<SysYParser::ScalarInitValContext *>(child); scalar_node != nullptr) {
            SRC value = scalar_node->exp()->accept(this);
            // dbg(value.ToString(), DeclTypeToStr(value.getType()), _type.decl_type);
            if (value.getType() != _type.decl_type) {
                SRC value_cast = SRC(new VirtReg(var_idx++, _type.decl_type));
                if (_type.decl_type == TypeInt) {
                    LLIR_FPTOSI *fti_inst = new LLIR_FPTOSI(value_cast, value);
                    cur_basicblock->basic_block.push_back(fti_inst);
                    value = value_cast;
                } else {
                    LLIR_SITOFP *itf_inst = new LLIR_SITOFP(value_cast, value);
                    cur_basicblock->basic_block.push_back(itf_inst);
                    value = value_cast;
                }
            }
            // dbg(off, value.ToString());
            initTable.push_back({off, value});
            ++off;
            ++cnt;
        } else {
            auto list_node = dynamic_cast<SysYParser::ListInitvalContext *>(child);
            parse_variable_init(list_node, _type, child_array_dims, off, initTable);
            off += total_size / arr_dim[0];
            cnt += total_size / arr_dim[0];
        }
    }
    return;
}

void ASTVisitor::generate_varinit_ir(SysYParser::InitVarDefContext *ctx, SRC addr, Variable *variable) {
    assert(ctx != nullptr);
    VarType var = variable->type;
    auto init_node = ctx->initVal();
    if (var.is_array == false) {
        auto scalar_init = dynamic_cast<SysYParser::ScalarInitValContext *>(init_node);
        SRC src = scalar_init->exp()->accept(this);
        if (src.getType() != addr.getType()) {
            VirtReg *reg_cast = new VirtReg(var_idx++, addr.getType());
            if (reg_cast->type.decl_type == TypeInt) {
                LLIR_FPTOSI *fti_inst = new LLIR_FPTOSI(SRC(reg_cast), src);
                cur_basicblock->basic_block.push_back(fti_inst);
                src = SRC(reg_cast);
            } else if (reg_cast->type.decl_type == TypeFloat) {
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(reg_cast), src);
                cur_basicblock->basic_block.push_back(itf_inst);
                src = SRC(reg_cast);
            } else {
                // dbg(DeclTypeToStr(addr->type.decl_type) + ", UnExpected Function Return Type");
                exit(EXIT_FAILURE);
            }
        }
        LLIR_STORE* store_inst = new LLIR_STORE(addr, src);
        cur_basicblock->basic_block.push_back(store_inst);
    } else {
        auto node = dynamic_cast<SysYParser::ListInitvalContext *>(init_node);
        vector<pair<int32_t, SRC>> initTable;
        parse_variable_init(node, var, var.array_dims, 0, initTable);
        int32_t init_size = initTable.size();
        VirtReg *ptr1 = new VirtReg(var_idx++, var);
        LLIR_GEP *gep_inst1 = new LLIR_GEP(ptr1, addr, SRC(new CTValue(TypeInt, 0, 0)), var);
        cur_basicblock->basic_block.push_back(gep_inst1);
        int32_t number = var.elements_number();
        if (initTable.size() < number) {
            vector<SRC> args;
            VirtReg *start_addr = new VirtReg(var_idx++, VarType(false, true, false, var.decl_type));
            LLIR_GEP *gep_addr_inst = new LLIR_GEP(start_addr, ptr1, SRC(new CTValue(TypeInt, 0, 0)), start_addr->type);
            cur_basicblock->basic_block.push_back(gep_addr_inst);
            FunctionInfo *func_info = nullptr;
            if (var.decl_type == TypeInt) {
                func_info = ir.getFunctionInfo("imemset");
            } else if (var.decl_type == TypeFloat) {
                func_info = ir.getFunctionInfo("fmemset");
            } else {
                assert(true && "UnExcepted Type for memset");
            }
            func_info->is_used = true;
            cur_func->func_info.called_funcs.insert(func_info);
            args.push_back(start_addr);
            args.push_back(new CTValue(TypeInt, 0, 0));
            args.push_back(new CTValue(TypeInt, number * 4, number * 4));
            func_info->call_count += 1;
            LLIR_CALL *call_inst = new LLIR_CALL(SRC(), args, func_info);
            cur_basicblock->basic_block.push_back(call_inst);
        }
        for (auto &&pair : initTable) {
            int32_t off = pair.first;
            SRC value = pair.second;
            VirtReg *ptr2 = new VirtReg(var_idx++, VarType(false, true, false, ptr1->type.decl_type));
            LLIR_GEP *gep_inst2 = new LLIR_GEP(ptr2, ptr1, SRC(new CTValue(TypeInt, off, off)), ptr2->type);
            cur_basicblock->basic_block.push_back(gep_inst2);
            LLIR_STORE *store_inst = new LLIR_STORE(SRC(ptr2), value);
            cur_basicblock->basic_block.push_back(store_inst);
        }
    }
}

// 暂时没有考虑float数组初始化
void ASTVisitor::local_const_list_init(SRC reg, Variable *variable) {
    VarType var = variable->type;
    VirtReg *ptr1 = new VirtReg(var_idx++, var);
    LLIR_GEP *gep_inst1 = new LLIR_GEP(ptr1, reg, SRC(new CTValue(TypeInt, 0, 0)), var);
    cur_basicblock->basic_block.push_back(gep_inst1);
    int32_t idx = 0;
    for (idx = 0; idx < variable->int_list.size(); ++idx) {
        VirtReg *ptr2 = new VirtReg(var_idx++, VarType(false, true, false, ptr1->type.decl_type));
        LLIR_GEP *gep_inst2 = new LLIR_GEP(ptr2, ptr1, SRC(new CTValue(TypeInt, idx, idx)), ptr2->type);
        cur_basicblock->basic_block.push_back(gep_inst2);
        SRC value = SRC(new CTValue(var.decl_type, variable->int_list[idx], variable->float_list[idx]));
        LLIR_STORE *store_inst = new LLIR_STORE(SRC(ptr2), value);
        cur_basicblock->basic_block.push_back(store_inst);
    }
}

void ASTVisitor::init_func_args(FunctionInfo *func_info) {
    int32_t arg_idx = 0;
    for (auto pair : func_info->func_args) {
        if (!pair.second.is_array) {
            Variable *variable = new Variable(var_idx++);
            variable->type = pair.second;
            cur_vartable->var_table.push_back(make_pair(pair.first, variable));
            VirtReg *reg = new VirtReg(variable->var_idx, variable->type.decl_type);
            LLIR_ALLOCA *alloca_inst = new LLIR_ALLOCA(reg, variable);
            cur_basicblock->basic_block.push_back(alloca_inst);
            LLIR_STORE *store_inst = new LLIR_STORE(reg, new VirtReg(arg_idx, func_info->func_args[arg_idx].second.decl_type));
            cur_basicblock->basic_block.push_back(store_inst);
        } else {
            Variable *variable = new Variable(arg_idx);
            variable->type = pair.second;
            cur_vartable->var_table.push_back(make_pair(pair.first, variable));
        }
        ++arg_idx;
    }
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
    visitChildren(ctx);
    return nullptr;
}

// finished
// 设置全局变量`type`, 这个变量仅在变量声明时起作用，结束后恢复
antlrcpp::Any ASTVisitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
    DeclType last_type = cur_type;
    cur_type = getDeclType(ctx->children[1]->getText());
    visitChildren(ctx);
    cur_type = last_type;
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
    // dbg("enter ConstDef");
    string var_name = ctx->Identifier()->getText();
    // dbg(var_name);
    if (cur_vartable->findInCurTable(var_name)) {
        // dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    bool is_array = !(ctx->constExp().size() == 0);
    DeclType type = cur_type;
    VarType const_var(true, is_array, false, type);
    // dbg(var_name, DeclTypeToStr(const_var.decl_type));
    if (const_var.is_array == true) {
        const_var.array_dims = get_array_dims(ctx->constExp());
        // dbg(const_var.array_dims);
    }
    Variable *const_variable = nullptr;
    if (const_var.is_array || const_var.decl_type == TypeFloat) {
        int32_t idx = (&cur_func->func_info != nullptr) ? var_idx++ : glb_var_idx++;
        const_variable = new Variable(idx);
    }
    else const_variable = new Variable(-1);
    // 分析`const`变量的初值
    auto init_node = ctx->constInitVal();
    if (const_var.is_array == false) {
        auto node = dynamic_cast<SysYParser::ScalarConstInitValContext *>(init_node);
        SRC scalar = node->constExp()->accept(this);
        CTValue *ctv = scalar.ToCTValue();
        assert(ctv != nullptr);
        const_variable->int_scalar = ctv->int_value;
        const_variable->float_scalar = ctv->float_value;
        // dbg(const_variable->int_scalar, const_variable->float_scalar);
    } else {
        auto node = dynamic_cast<SysYParser::ListConstInitValContext *>(init_node);
        parse_const_init(node, const_var.array_dims, const_variable->int_list, const_variable->float_list);
        // dbg(const_variable->int_list, const_variable->float_list);
    }
    const_variable->type = const_var;
    // 写入当前作用域的符号表
    if (cur_vartable->findInCurTable(var_name)) {
        // dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    auto pair = make_pair(var_name, const_variable);
    cur_vartable->var_table.push_back(pair);
    // 生成 LLIR
    if (&cur_func->func_info != nullptr && const_var.is_array == true) {
        VirtReg *reg = new VirtReg(const_variable->var_idx, const_variable->type);
        LLIR_ALLOCA *alloca_inst = new LLIR_ALLOCA(SRC(reg), const_variable);
        if (loop_mode == true) {
            alloca_insts.push_back(alloca_inst);
        } else {
            cur_basicblock->basic_block.push_back(alloca_inst);
        }
        local_const_list_init(alloca_inst->reg, const_variable);
    }
    // dbg("exit ConstDef");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) {
    // dbg("Program should never reach Function visitScalarConstInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) {
    // dbg("Program should never reach Function visitListConstInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitVarDecl(SysYParser::VarDeclContext *ctx) {
    DeclType last_type = cur_type;
    cur_type = getDeclType(ctx->children[0]->getText());
    visitChildren(ctx);
    cur_type = last_type;
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) {
    // dbg("enter UninitVarDef");
    string var_name = ctx->Identifier()->getText();
    if (cur_vartable->findInCurTable(var_name)) {
        // dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    int32_t idx = (&cur_func->func_info != nullptr) ? var_idx++ : glb_var_idx++;
    Variable *variable = new Variable(idx);
    bool is_array = !(ctx->constExp().size() == 0);
    DeclType type = cur_type;
    VarType var(false, is_array, false, type);
    // dbg(var_name, DeclTypeToStr(var.decl_type));
    if (var.is_array) {
        var.array_dims = get_array_dims(ctx->constExp());
        // dbg(var.array_dims);
    }
    variable->type = var;
    VarPair var_pair = make_pair(var_name, variable);
    cur_vartable->var_table.push_back(var_pair);
    if (&cur_func->func_info != nullptr) {
        VirtReg *reg = new VirtReg(variable->var_idx, variable->type);
        LLIR_ALLOCA *alloca_inst = new LLIR_ALLOCA(SRC(reg), variable);
        if (loop_mode == true) {
            alloca_insts.push_back(alloca_inst);
        } else {
            cur_basicblock->basic_block.push_back(alloca_inst);
        }
    }
    // we don't need init un-init-var
    // if it is global, it has been init with `0`
    // dbg("exit UninitVarDef");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
    // dbg("enter visitInitVarDef");
    // push to variable table, similar with `UninitVarDef`
    string var_name = ctx->Identifier()->getText();
    if (cur_vartable->findInCurTable(var_name)) {
        // dbg(var_name + " is in cur_vartable");
        exit(EXIT_FAILURE);
    }
    int32_t idx = (&cur_func->func_info != nullptr) ? var_idx++ : glb_var_idx++;
    Variable *variable = new Variable(idx);
    bool is_array = !(ctx->constExp().size() == 0);
    DeclType type = cur_type;
    VarType var(false, is_array, false, type);
    // dbg(var_name, DeclTypeToStr(var.decl_type));
    if (var.is_array) {
        var.array_dims = get_array_dims(ctx->constExp());
        // dbg(var.array_dims);
    }
    variable->type = var;
    VarPair var_pair = make_pair(var_name, variable);
    cur_vartable->var_table.push_back(var_pair);
    LLIR_ALLOCA *alloca_inst = nullptr;
    SRC addr = SRC();
    if (&cur_func->func_info != nullptr) {
        SRC reg = SRC(new VirtReg(variable->var_idx, variable->type));
        addr = reg;
        alloca_inst = new LLIR_ALLOCA(reg, variable);
        if (loop_mode == true) {
            alloca_insts.push_back(alloca_inst);
        } else {
            cur_basicblock->basic_block.push_back(alloca_inst);
        }
    } else {
        addr = cur_scope->resolve(var_name, &cur_func->func_info);
    }
    // parse `InitVarDef`
    // init global variable before excuting main function
    // init local variable we it first exsit
    // we make sure that all variable don't init at Variable->init_value, but get value via access memory
    // 不管是标量还是向量, 初始化时都会用到首地址 addr
    if (&cur_func->func_info != nullptr) {
        generate_varinit_ir(ctx, addr, variable);
    } else {
        glb_var_init.push_back(make_tuple(ctx, addr, variable));
    }
    // dbg("exit InitVarDef");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) {
    // dbg("Program should never reach Function visitScalarInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitListInitval(SysYParser::ListInitvalContext *ctx) {
    // dbg("Program should never reach Function visitListInitval");
    exit(EXIT_FAILURE);
}

// finished
// 函数声明分析，获取函数声明，返回类型，参数表
// 因为不存在函数内声明函数的情况，因此直接将函数定义插入函数表
antlrcpp::Any ASTVisitor::visitFuncDef(SysYParser::FuncDefContext *ctx) {
    Function *func = new Function;
    string func_name = ctx->Identifier()->getText();
    // dbg("enter visitFuncDef " + func_name);
    FunctionInfo func_info;
    func_info.is_used = false;
    func_info.call_count = 0;
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
    if (func_name == "main") {
        have_main_func = true;
        func_info.is_used = true;
    }
    func->func_info = func_info;
    // reset variable idx in function
    var_idx = func_info.func_args.size() == 0 ? 0 : func_info.func_args.size();
    sp_idx = 1;
    bb_idx = 1;
    init_args = true;
    cur_func = func;
    // when get `FunctionInfo`
    // we can push `func` to function table
    // for `recursion`
    ir.functions.push_back(func);
    // parse function body
    func->main_scope = ctx->block()->accept(this);
    func->var_idx = var_idx;
    func->bb_idx = bb_idx;
    // dbg(func->main_scope->get_last_bb()->bb_idx);
    auto last_block = func->main_scope->get_last_bb();
    int32_t lb_size = last_block->basic_block.size();
    LLIR_RET *ret_inst = new LLIR_RET((ret_type != TypeVoid), SRC(new CTValue(ret_type, 0, 0)));
    if (lb_size == 0) {
        last_block->basic_block.push_back(ret_inst);
    } else if (auto last_inst = dynamic_cast<LLIR_RET *>(last_block->lastInst()); last_inst == nullptr) {
        last_block->basic_block.push_back(ret_inst);
    }
    // push to function table
    cur_func = nullptr;
    cur_basicblock = nullptr;
    // dbg("exit visitFuncDef " + func_name);
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitFuncType(SysYParser::FuncTypeContext *ctx) {
    // dbg("Program should never reach Function visitFuncType");
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
        func_arg.decl_type = func_arg.decl_type;
        func_arg.is_array = true;
        func_arg.array_dims = get_array_dims(ctx->constExp());
        func_arg.array_dims.insert(func_arg.array_dims.begin(), -1);
    }
    // dbg(func_arg.decl_type);
    return make_pair(ctx->Identifier()->getText(), func_arg);
}

// finished
// `Block`分析
// 将该作用域下的符号表，指令保存
antlrcpp::Any ASTVisitor::visitBlock(SysYParser::BlockContext *ctx) {
    // dbg("enter visitBlock");
    // 将上个作用域的基本块push进来
    cur_scope_elements->push_back(cur_basicblock);
    if (cur_basicblock != nullptr) {
        cur_func->all_blocks.push_back(cur_basicblock);
    }
    // save `cur_scope` `cur_vartable` `cur_elements`
    Scope          *last_scope = cur_scope;
    VariableTable  *last_vartable = cur_vartable;
    vector<Info *> *last_scope_elements = cur_scope_elements;
    // process `Block` part
    Scope *block_scope = new Scope(sp_idx++);
    block_scope->local_table = new VariableTable;
    block_scope->elements = new vector<Info *>;
    block_scope->parent = last_scope;
    // cur_* point to current *
    cur_scope = block_scope;
    cur_vartable = block_scope->local_table;
    cur_scope_elements = block_scope->elements;
    cur_basicblock = new BasicBlock(bb_idx++);
    // dbg(cur_basicblock->bb_idx);
    // 初始化全局变量
    if (cur_func->func_info.func_name == "main" && glb_var_init.size() != 0) {
        for (auto elm : glb_var_init) {
            generate_varinit_ir(get<0>(elm), get<1>(elm), get<2>(elm));
        }
        glb_var_init.clear();
    }
    // 初始化函数参数
    if (init_args) {
        init_func_args(&cur_func->func_info);
        init_args = false;
    }
    visitChildren(ctx);
    // 新的基本块到右括号就结束了, push
    cur_scope_elements->push_back(cur_basicblock);
    cur_func->all_blocks.push_back(cur_basicblock);
    // dbg(cur_basicblock->bb_idx);
    // restore `cur_scope` `cur_vartable` `cur_elements`
    cur_scope = last_scope;
    cur_vartable = last_vartable;
    cur_scope_elements = last_scope_elements;
    // 新的基本块
    cur_basicblock = new BasicBlock(bb_idx++);
    // dbg(cur_basicblock->bb_idx);
    // dbg("exit visitBlock");
    return block_scope;
}

// finished
antlrcpp::Any ASTVisitor::visitBlockItem(SysYParser::BlockItemContext *ctx) {
    auto stmt_node = ctx->stmt();
    if (auto stmt_node = ctx->stmt(); stmt_node != nullptr) {
        if (auto blockstmt_node = dynamic_cast<SysYParser::BlockStmtContext *>(stmt_node); blockstmt_node != nullptr) {
            LLIR_BR *br_inst1 = new LLIR_BR(false, SRC(), cur_basicblock->bb_idx + 1, 0);
            cur_basicblock->basic_block.push_back(br_inst1);
            Scope *block_stmt = stmt_node->accept(this);
            BasicBlock *last_bb = block_stmt->get_last_bb();
            LLIR_BR *br_inst2 = new LLIR_BR(false, SRC(), cur_basicblock->bb_idx, 0);
            last_bb->basic_block.push_back(br_inst2);
            return blockstmt_node;
        } else {
            return visitChildren(ctx);
        }
    } else {
        return visitChildren(ctx);
    }
}

// finished
antlrcpp::Any ASTVisitor::visitAssignment(SysYParser::AssignmentContext *ctx) {
    // dbg("visit visitAssignment");
    SRC lhs = ctx->lVal()->accept(this);
    SRC rhs = ctx->exp()->accept(this);
    if (lhs.getType() != rhs.getType()) {
        if (rhs.getType() == TypeInt) {
            SRC cast_rhs = SRC(new VirtReg(var_idx++, TypeFloat));
            LLIR_SITOFP *itf_inst = new LLIR_SITOFP(cast_rhs, rhs);
            cur_basicblock->basic_block.push_back(itf_inst);
            rhs = cast_rhs;
        } else {
            SRC cast_rhs = SRC(new VirtReg(var_idx++, TypeInt));
            LLIR_FPTOSI *itf_inst = new LLIR_FPTOSI(cast_rhs, rhs);
            cur_basicblock->basic_block.push_back(itf_inst);
            rhs = cast_rhs;
        }
    }
    LLIR_STORE *store_inst = new LLIR_STORE(lhs, rhs);
    cur_basicblock->basic_block.push_back(store_inst);
    // dbg("exit visitAssignment");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitExpStmt(SysYParser::ExpStmtContext *ctx) {
    return visitChildren(ctx);
}

// finished
antlrcpp::Any ASTVisitor::visitBlockStmt(SysYParser::BlockStmtContext *ctx) {
    // dbg("enter visitBlockStmt");
    Scope *block_stmt = ctx->block()->accept(this);
    cur_scope->elements->push_back(block_stmt);
    // dbg("exit visitBlockStmt");
    return block_stmt;
}

// finished
antlrcpp::Any ASTVisitor::visitIfStmt(SysYParser::IfStmtContext *ctx) {
    // dbg("enter visitIfStmt");
    // store last condition
    vector<LLIR_BR *> last_lor_insts  = lor_insts;
    vector<LLIR_BR *> last_land_insts = land_insts;
    lor_insts  = vector<LLIR_BR *>();
    land_insts = vector<LLIR_BR *>();
    SRC cond = ctx->cond()->accept(this);
    // file in lor branch target
    for (auto lor_inst : lor_insts) {
        lor_inst->tar_true = bb_idx;
    }
    bool has_else = (ctx->stmt().size() > 1);
    LLIR_BR *br_if_else = new LLIR_BR(true, cond, bb_idx ,0);
    LLIR_BR *br_if2else_end = new LLIR_BR(false, SRC(), 0 , 0);
    LLIR_BR *br_else2else_end = new LLIR_BR(false, SRC(), 0 , 0);
    cur_basicblock->basic_block.push_back(br_if_else);
    // if stmt body
    // dbg("Enter If-body");
    if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()[0]); node != nullptr) {
        // dbg("Block If-Stmt Context");
        Scope *block_stmt = node->accept(this);
        block_stmt->get_last_bb()->basic_block.push_back(br_if2else_end);
    } else {
        // dbg("Other If-Stmt Context");
        cur_scope_elements->push_back(cur_basicblock);
        cur_func->all_blocks.push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope(sp_idx++);
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock(bb_idx++);

        ctx->stmt()[0]->accept(this);
        cur_basicblock->basic_block.push_back(br_if2else_end);
        cur_scope_elements->push_back(cur_basicblock);
        cur_func->all_blocks.push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock(bb_idx++);
    }
    // else stmt body
    if (has_else) {
        // dbg("Enter Else-body");
        br_if_else->tar_false = cur_basicblock->bb_idx + 1;
        // file in lor branch target
        for (auto land_inst : land_insts) {
            land_inst->tar_false = bb_idx;
        }
        if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()[1]); node != nullptr) {
            // dbg("Block Else-Stmt Context");
            Scope *block_stmt = node->accept(this);
            block_stmt->get_last_bb()->basic_block.push_back(br_else2else_end);
        } else {
            // dbg("Other Else-Stmt Context");
            cur_scope_elements->push_back(cur_basicblock);
            cur_func->all_blocks.push_back(cur_basicblock);
            Scope          *last_scope = cur_scope;
            VariableTable  *last_vartable = cur_vartable;
            vector<Info *> *last_scope_elements = cur_scope_elements;

            Scope *block_scope = new Scope(sp_idx++);
            block_scope->local_table = new VariableTable;
            block_scope->elements = new vector<Info *>;
            block_scope->parent = last_scope;
            cur_scope = block_scope;
            cur_vartable = block_scope->local_table;
            cur_scope_elements = block_scope->elements;
            cur_basicblock = new BasicBlock(bb_idx++);

            ctx->stmt()[1]->accept(this);
            cur_basicblock->basic_block.push_back(br_else2else_end);
            cur_scope_elements->push_back(cur_basicblock);
            cur_func->all_blocks.push_back(cur_basicblock);

            last_scope->elements->push_back(cur_scope);
            cur_scope = last_scope;
            cur_vartable = last_vartable;
            cur_scope_elements = last_scope_elements;
            cur_basicblock = new BasicBlock(bb_idx++);
        }
        br_if2else_end->tar_true = cur_basicblock->bb_idx;
        br_else2else_end->tar_true = cur_basicblock->bb_idx;
    } else {
        // dbg("Create a Empty Else-Stmt");
        br_if_else->tar_false = cur_basicblock->bb_idx + 1;
        // file in lor branch target
        for (auto land_inst : land_insts) {
            land_inst->tar_false = bb_idx;
        }
        cur_scope_elements->push_back(cur_basicblock);
        cur_func->all_blocks.push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope(sp_idx++);
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock(bb_idx++);

        cur_basicblock->basic_block.push_back(br_else2else_end);
        cur_scope_elements->push_back(cur_basicblock);
        cur_func->all_blocks.push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock(bb_idx++);
        br_if2else_end->tar_true = cur_basicblock->bb_idx;
        br_else2else_end->tar_true = cur_basicblock->bb_idx;
    } 
    lor_insts  = last_lor_insts;
    land_insts = last_land_insts;
    // dbg("exit visitIfStmt");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitWhileStmt(SysYParser::WhileStmtContext *ctx) {
    // dbg("enter visitWhileStmt");
    // store last condition
    vector<LLIR_BR *> last_lor_insts  = lor_insts;
    vector<LLIR_BR *> last_land_insts = land_insts;
    lor_insts  = vector<LLIR_BR *>();
    land_insts = vector<LLIR_BR *>();
    LLIR_BR *br2while_cond = new LLIR_BR(false, SRC(), cur_basicblock->bb_idx + 1, 0);
    BasicBlock *alloca_branch_bb = cur_basicblock;
    cur_scope_elements->push_back(cur_basicblock);
    cur_func->all_blocks.push_back(cur_basicblock);
    cur_basicblock = new BasicBlock(bb_idx++);
    int32_t last_continue_target = continue_target;
    continue_target = cur_basicblock->bb_idx;
    vector<LLIR_BR *> last_break_insts = break_insts;
    break_insts = vector<LLIR_BR *>();
    // while condition
    SRC cond = ctx->cond()->accept(this);
    // file in lor land branch target
    for (auto lor_inst : lor_insts) {
        // dbg("Fill in lor true target");
        lor_inst->tar_true = bb_idx;
    }
    LLIR_BR *while_br_inst = new LLIR_BR(true, cond, bb_idx, 0);
    cur_basicblock->basic_block.push_back(while_br_inst);
    bool last_loop_mode = loop_mode;
    loop_mode = true;
    if (auto node = dynamic_cast<SysYParser::BlockStmtContext *>(ctx->stmt()); node != nullptr) {
        // 在这里遇到了`Block`作为循环体的`While`
        // 但是无法在生成`Block`的中途插入跳转语句
        // 因此我们要进入`Block`中找到其最后一个`basic_block`
        // 并插入`Jump_Inst`
        // dbg("Block Stmt Context");
        // file in lor branch target
        for (auto land_inst : land_insts) {
            land_inst->tar_false = bb_idx;
        }
        Scope *block_stmt = node->accept(this);
        LLIR_BR *br_while_start = new LLIR_BR(false, SRC(), continue_target, 0);
        block_stmt->get_last_bb()->basic_block.push_back(br_while_start);
    } else {
        // dbg("Other Stmt Context");
        // file in lor branch target
        for (auto land_inst : land_insts) {
            land_inst->tar_false = bb_idx;
        }
        cur_scope_elements->push_back(cur_basicblock);
        cur_func->all_blocks.push_back(cur_basicblock);
        Scope          *last_scope = cur_scope;
        VariableTable  *last_vartable = cur_vartable;
        vector<Info *> *last_scope_elements = cur_scope_elements;

        Scope *block_scope = new Scope(sp_idx++);
        block_scope->local_table = new VariableTable;
        block_scope->elements = new vector<Info *>;
        block_scope->parent = last_scope;
        cur_scope = block_scope;
        cur_vartable = block_scope->local_table;
        cur_scope_elements = block_scope->elements;
        cur_basicblock = new BasicBlock(bb_idx++);

        ctx->stmt()->accept(this);
        LLIR_BR *br_while_start = new LLIR_BR(false, SRC(), continue_target, 0);
        cur_basicblock->basic_block.push_back(br_while_start);
        cur_scope_elements->push_back(cur_basicblock);
        cur_func->all_blocks.push_back(cur_basicblock);

        last_scope->elements->push_back(cur_scope);
        cur_scope = last_scope;
        cur_vartable = last_vartable;
        cur_scope_elements = last_scope_elements;
        cur_basicblock = new BasicBlock(bb_idx++);
    }
    loop_mode = last_loop_mode;
    if (loop_mode == false) {
        for (auto alloca_inst : alloca_insts) {
            alloca_branch_bb->basic_block.push_back(alloca_inst);
        }
        alloca_insts.resize(0);
    }
    alloca_branch_bb->basic_block.push_back(br2while_cond);
    for (int32_t idx = 0; idx < break_insts.size(); ++idx) {
        break_insts[idx]->tar_true = cur_basicblock->bb_idx;
    }
    continue_target = last_continue_target;
    break_insts = last_break_insts;
    while_br_inst->tar_false = cur_basicblock->bb_idx;
    for (auto land_inst : land_insts) {
        land_inst->tar_false = cur_basicblock->bb_idx;
    }
    lor_insts  = last_lor_insts;
    land_insts = last_land_insts;
    // dbg("exit visitWhileStmt");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitBreakStmt(SysYParser::BreakStmtContext *ctx) {
    // dbg("enter visitBreakStmt");
    LLIR_BR *jmp_inst = new LLIR_BR(false, SRC(), 0, 0);
    cur_basicblock->basic_block.push_back(jmp_inst);
    break_insts.push_back(jmp_inst);
    cur_scope_elements->push_back(cur_basicblock);
    cur_func->all_blocks.push_back(cur_basicblock);
    cur_basicblock = new BasicBlock(bb_idx++);
    // dbg("exit visitBreakStmt");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitContinueStmt(SysYParser::ContinueStmtContext *ctx) {
    // dbg("enter visitContinueStmt");
    LLIR_BR *jmp_inst = new LLIR_BR(false, SRC(), continue_target, 0);
    cur_basicblock->basic_block.push_back(jmp_inst);
    cur_scope_elements->push_back(cur_basicblock);
    cur_func->all_blocks.push_back(cur_basicblock);
    cur_basicblock = new BasicBlock(bb_idx++);
    // dbg("enter visitContinueStmt");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
    // dbg("enter visitReturnStmt");
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
            if (reg->type.decl_type != ret_type) {
                VirtReg *dst_reg = new VirtReg(var_idx++, ret_type);
                if (reg->type.decl_type == TypeInt) {
                    LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(dst_reg), SRC(dst));
                    cur_basicblock->basic_block.push_back(itf_inst);
                    dst = SRC(dst_reg);
                } else if (reg->type.decl_type == TypeFloat) {
                    LLIR_FPTOSI *fti_inst = new LLIR_FPTOSI(SRC(dst_reg), SRC(dst));
                    cur_basicblock->basic_block.push_back(fti_inst);
                    dst = SRC(dst_reg);
                } else {
                    // dbg(DeclTypeToStr(reg->type.decl_type) + ", UnExpected Function Return Type");
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
    cur_func->all_blocks.push_back(cur_basicblock);
    cur_basicblock = new BasicBlock(bb_idx++);
    // dbg(cur_basicblock->bb_idx);
    // dbg("exit visitReturnStmt");
    return nullptr;
}

// finished
antlrcpp::Any ASTVisitor::visitExp(SysYParser::ExpContext *ctx) {
    // dbg("enter visitExp");
    SRC exp = ctx->addExp()->accept(this);
    // dbg("exit visitExp");
    return exp;
}

// finished
antlrcpp::Any ASTVisitor::visitCond(SysYParser::CondContext *ctx) {
    // dbg("enter visitCond");
    SRC cond =  ctx->lOrExp()->accept(this);
    if (CTValue *ctv = cond.ToCTValue(); ctv != nullptr) {
        if (ctv->type == TypeInt) {
            ctv->int_value = (ctv->int_value != 0) ? 1 : 0;
            ctv->type = TypeBool;
        } else {
            ctv->int_value = (ctv->float_value != 0) ? 1 : 0;
            ctv->type = TypeBool;
        }
    } else {
        VirtReg *reg = cond.ToVirtReg();
        if (reg->type.decl_type != TypeBool) {
            VirtReg *cmp_dst = new VirtReg(var_idx++, TypeBool);
            if (reg->type.decl_type == TypeInt) {
                LLIR_ICMP *icmp_inst = new LLIR_ICMP(NEQ, SRC(cmp_dst), cond, SRC(new CTValue(TypeInt, 0, 0)));
                cur_basicblock->basic_block.push_back(icmp_inst);
            } else if (reg->type.decl_type == TypeFloat) {
                SRC zero_ctv = SRC(new CTValue(TypeInt, 0, 0));
                SRC zero_reg = SRC(new VirtReg(var_idx++, TypeInt));
                LLIR_LOAD *load_inst = new LLIR_LOAD(zero_reg, zero_ctv);
                cur_basicblock->basic_block.push_back(load_inst);
                SRC zero_float = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(zero_float, zero_reg);
                cur_basicblock->basic_block.push_back(itf_inst);
                LLIR_FCMP *fcmp_inst = new LLIR_FCMP(NEQ, SRC(cmp_dst), cond, zero_float);
                cur_basicblock->basic_block.push_back(fcmp_inst);
            }
            cond = SRC(cmp_dst);
        }
    }
    // dbg("exit visitCond");
    return cond;
}

// finished
// 进入这个产生式并不知道他将作为左值还是右值
// 所以在这里我们仅返回存储使用的变量的地址
antlrcpp::Any ASTVisitor::visitLVal(SysYParser::LValContext *ctx) {
    string var_name = ctx->Identifier()->getText();
    // dbg("enter LVal for " + var_name);
    SRC variable = cur_scope->resolve(var_name, &cur_func->func_info);
    assert(variable.ToCTValue() != nullptr || variable.ToVirtReg() != nullptr);
    // 常量
    if (CTValue *ctv = variable.ToCTValue(); ctv != nullptr) {
        // dbg("exit visitLVal with a constant");
        return SRC(ctv);
    } 
    VirtReg *reg = variable.reg;
    // dbg(reg->type.is_array);
    if (reg->type.is_array == false) {
        // dbg("exit visitLVal with a scalar variable");
        return SRC(reg);
    }
    // dbg(reg->reg_id, reg->global);
    SRC offset = SRC(new CTValue(TypeInt, 0, 0));
    vector<int32_t> arr_dim = reg->type.get_dims();
    int32_t size = ctx->exp().size();
    VarType type = reg->type;
    // dbg(type.is_array);
    for (int32_t idx = 0; idx < size; ++idx) {
        SRC dim = ctx->exp()[idx]->accept(this);
        VirtReg *mul_off = new VirtReg(var_idx++, VarType(TypeInt));
        LLIR_BIN *bin_inst1 = new LLIR_BIN(MUL, mul_off, dim, SRC(new CTValue(TypeInt, arr_dim[idx], arr_dim[idx])));
        cur_basicblock->basic_block.push_back(bin_inst1);
        VirtReg *add_off = new VirtReg(var_idx++, VarType(TypeInt));
        LLIR_BIN *bin_inst2 = new LLIR_BIN(ADD, add_off, mul_off, offset);
        cur_basicblock->basic_block.push_back(bin_inst2);
        offset = SRC(add_off);
        type = type.move_down();
    }
    // dbg(var_name);
    // 获取元素首地址
    VirtReg *ptr1 = new VirtReg(var_idx++, reg->type);
    LLIR_GEP *gep_inst1 = new LLIR_GEP(SRC(ptr1), variable, SRC(new CTValue(TypeInt, 0, 0)), reg->type);
    cur_basicblock->basic_block.push_back(gep_inst1);
    VirtReg *ptr2 = new VirtReg(var_idx++, VarType(false, type.is_array, false, reg->type.decl_type));
    // dbg(type.is_array);
    LLIR_GEP *gep_inst2 = new LLIR_GEP(ptr2, ptr1, offset, ptr2->type);
    cur_basicblock->basic_block.push_back(gep_inst2);
    // dbg(gep_inst2->ToString(), gep_inst2->dst.reg->type.is_array);
    // dbg(reg->ToString());
    // dbg( "exit visitLVal with a variable or array");
    return SRC(ptr2);
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) {
    return ctx->exp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) {
    // dbg("enter visitPrimaryExp2");
    SRC src = ctx->lVal()->accept(this);
    SRC dst;
    if (CTValue *ctv = src.ToCTValue(); ctv != nullptr) {
        dst = SRC(ctv);
    } else {
        VirtReg *src_reg = src.ToVirtReg();
        VarType src_type = src_reg->type;
        if (src_type.is_array && src_reg->type.array_dims.size() == 0) {
            dst = SRC(src_reg);
        } else {
            VirtReg *dst_reg = new VirtReg(var_idx++, src_reg->type);
            dst = SRC(dst_reg);
            LLIR_LOAD *load_inst = new LLIR_LOAD(dst, src);
            cur_basicblock->basic_block.push_back(load_inst);
        }
    }
    // dbg("exit visitPrimaryExp2");
    return dst;
}

// finished
antlrcpp::Any ASTVisitor::visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) {
    // dbg("enter visitPrimaryExp3");
    SRC src = ctx->number()->accept(this);
    // dbg(src.ToString());
    // dbg("exit visitPrimaryExp3");
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
    double float_literal = 0;
    sscanf(ctx->FloatLiteral()->getText().c_str(), "%lf", &float_literal);
    CTValue *ret = new CTValue(TypeFloat, float_literal, float_literal);
    // dbg(ret->float_value);
    return SRC(ret);
}

// finished
antlrcpp::Any ASTVisitor::visitUnary1(SysYParser::Unary1Context *ctx) {
    // dbg("enter visitUnary1");
    SRC ret = ctx->primaryExp()->accept(this);
    // dbg("exit visitUnary1");
    return ret;
}

// finished
antlrcpp::Any ASTVisitor::visitUnary2(SysYParser::Unary2Context *ctx) {
    // dbg("enter visitUnary2");
    string func_name = ctx->Identifier()->getText();
    vector<SRC> args;
    if (func_name == "starttime") {
        func_name = "_sysy_starttime";
        args.push_back(SRC(new CTValue(TypeInt, ctx->start->getLine(), 0)));
    } else if (func_name == "stoptime") {
        func_name = "_sysy_stoptime";
        args.push_back(SRC(new CTValue(TypeInt, ctx->start->getLine(), 0)));
    } else if (ctx->funcRParams()) {
        args = ctx->funcRParams()->accept(this).as<vector<SRC>>();
    }
    FunctionInfo *func_info = ir.getFunctionInfo(func_name);
    func_info->is_used = true;
    cur_func->func_info.called_funcs.insert(func_info);
    int32_t rsize = args.size();
    int32_t fsize = func_info->func_args.size();
    if (rsize != fsize) {
        // dbg("Unmatched Function Args Num");
        exit(EXIT_FAILURE);
    }
    for (int32_t idx = 0; idx < rsize; ++idx) {
        auto rarg = args[idx];
        auto farg = func_info->func_args[idx];
        // dbg(rarg.ToString());
        if (rarg.getType() != farg.second.decl_type) {
            if (farg.second.is_array) {
                // dbg("Unmatch at %d Array Arg Type", idx);
                exit(EXIT_FAILURE);
            } 
            if (farg.second.decl_type == TypeInt) {
                SRC arg_cast = SRC(new VirtReg(var_idx++, TypeInt));
                LLIR_FPTOSI *fti_inst = new LLIR_FPTOSI(arg_cast, rarg);
                cur_basicblock->basic_block.push_back(fti_inst);
                args[idx] = arg_cast;
            } else {
                SRC arg_cast = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(arg_cast, rarg);
                cur_basicblock->basic_block.push_back(itf_inst);
                args[idx] = arg_cast;
            }
        }
    }
    SRC dst;
    if (func_info->return_type != TypeVoid) {
        dst = SRC(new VirtReg(var_idx++, VarType(func_info->return_type)));
    }
    func_info->call_count += 1;
    LLIR_CALL *call_inst = new LLIR_CALL(dst, args, func_info);
    cur_basicblock->basic_block.push_back(call_inst);
    // dbg("exit visitUnary2");
    return dst;
}

// finished
antlrcpp::Any ASTVisitor::visitUnary3(SysYParser::Unary3Context *ctx) {
    // dbg("enter visitUnary3");
    string op = ctx->unaryOp()->getText();
    SRC src = ctx->unaryExp()->accept(this);
    if (CTValue *ctv = src.ToCTValue(); ctv != nullptr) {
        // 当`src`是`CTValue`时
        // 可以直接计算, 无关所处状态
        if (op == "-") {
            CTValue *uop_ctv = new CTValue(ctv->type, -ctv->int_value, -ctv->float_value);
            // dbg("exit visitUnary3 with CTValue SUB");
            return SRC(uop_ctv);
        } else if (op == "+") {
            // dbg("exit visitUnary3 with CTValue ADD");
            return SRC(ctv);
        } else {
            CTValue *uop_ctv = new CTValue(TypeBool, !ctv->int_value, !ctv->float_value);
            // dbg("exit visitUnary3 with CTValue NEG");
            return SRC(uop_ctv);
        }
    } else {
        VirtReg *reg = src.ToVirtReg();
        VarType _type = reg->type;
        if (op == "-") {
            if (_type.decl_type == TypeBool) {
                VirtReg *zext_dst = new VirtReg(var_idx++, VarType(TypeInt));
                LLIR_ZEXT *zext_inst = new LLIR_ZEXT(SRC(zext_dst), SRC(reg));
                cur_basicblock->basic_block.push_back(zext_inst);
                reg = zext_dst;
                _type = VarType(TypeInt);
            }
            SRC ret;
            if (_type.decl_type == TypeInt) {
                VirtReg *dst = new VirtReg(var_idx++, _type);
                LLIR_BIN *bin_inst = new LLIR_BIN(SUB, dst, SRC(new CTValue(_type.decl_type, 0, 0)), SRC(reg));
                cur_basicblock->basic_block.push_back(bin_inst);
                ret = SRC(dst);
            } else if (_type.decl_type == TypeFloat) {
                SRC zero_ctv = SRC(new CTValue(TypeInt, 0, 0));
                SRC zero_float = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(zero_float, new CTValue(TypeInt, 0, 0));
                cur_basicblock->basic_block.push_back(itf_inst);
                VirtReg *dst = new VirtReg(var_idx++, _type);
                LLIR_FBIN *bin_inst = new LLIR_FBIN(SUB, dst, zero_float, SRC(reg));
                cur_basicblock->basic_block.push_back(bin_inst);
                ret = SRC(dst);
            }
            // dbg("exit visitUnary3 with VirtReg SUB");
            return ret;
        } else if (op == "+") {
            // dbg("exit visitUnary3 with VirtReg ADD");
            return SRC(reg);
        } else { // op == "!"
            if (reg->type.decl_type != TypeBool) {
                if (reg->type.decl_type == TypeInt) {
                    VirtReg *icmp_dst = new VirtReg(var_idx++, VarType(TypeBool));
                    LLIR_ICMP *icmp_inst = new LLIR_ICMP(NEQ, SRC(icmp_dst), SRC(reg), SRC(new CTValue(_type.decl_type, 0, 0)));
                    cur_basicblock->basic_block.push_back(icmp_inst);
                    reg = icmp_dst;
                } else if (reg->type.decl_type == TypeFloat) {
                    SRC zero_ctv = SRC(new CTValue(TypeInt, 0, 0));
                    SRC zero_float = SRC(new VirtReg(var_idx++, TypeFloat));
                    LLIR_SITOFP *itf_inst = new LLIR_SITOFP(zero_float, new CTValue(TypeInt, 0, 0));
                    cur_basicblock->basic_block.push_back(itf_inst);
                    VirtReg *fcmp_dst = new VirtReg(var_idx++, VarType(TypeBool));
                    LLIR_FCMP *icmp_inst = new LLIR_FCMP(NEQ, SRC(fcmp_dst), SRC(reg), zero_float);
                    cur_basicblock->basic_block.push_back(icmp_inst);
                    reg = fcmp_dst;
                }
            }
            VirtReg *dst = new VirtReg(var_idx++, VarType(TypeBool));
            LLIR_XOR *xor_inst = new LLIR_XOR(dst, SRC(reg));
            cur_basicblock->basic_block.push_back(xor_inst);
            // dbg("exit visitUnary3 with VirtReg NEG");
            return SRC(dst);
        }
    }
}

// finished
antlrcpp::Any ASTVisitor::visitUnaryOp(SysYParser::UnaryOpContext *ctx) {
    // dbg("Program should never reach Function visitListConstInitVal");
    exit(EXIT_FAILURE);
}

// finished
antlrcpp::Any ASTVisitor::visitFuncRParams(SysYParser::FuncRParamsContext *ctx) {
    vector<SRC> args;
    for (auto param : ctx->funcRParam()) {
        SRC param_src = param->accept(this);
        args.push_back(param_src);
    }
    return args;
}

// finished
antlrcpp::Any ASTVisitor::visitFuncRParam(SysYParser::FuncRParamContext *ctx) {
    return ctx->exp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitMul1(SysYParser::Mul1Context *ctx) {
    return ctx->unaryExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitMul2(SysYParser::Mul2Context *ctx) {
    // dbg("enter visitMul2");
    string op = ctx->children[1]->getText();
    SRC lhs = ctx->mulExp()->accept(this);
    SRC rhs = ctx->unaryExp()->accept(this);
    DeclType type = (lhs.getType() == rhs.getType()) ? lhs.getType() : TypeFloat;
    // 当两个操作数都是`CTValue`
    if (CTValue *ctv1 = lhs.ToCTValue(), *ctv2 = rhs.ToCTValue(); ctv1 != nullptr && ctv2 != nullptr) {
        int imul = 0;
        float fmul = 0;
        if (op == "*") {
            fmul = ctv1->float_value * ctv2->float_value;
            imul = (type == TypeInt) ? ctv1->int_value * ctv2->int_value : fmul;
        } else if (op == "/") {
            fmul = ctv1->float_value / ctv2->float_value;
            imul = (type == TypeInt) ? ctv1->int_value / ctv2->int_value : fmul;
        } else {
            imul = ctv1->int_value % ctv2->int_value;
            fmul = imul;
        }
        CTValue *mul = new CTValue(type, imul, fmul);
        // dbg(mul->ToString());
        // dbg("exit visitMul2 with 2 CTValue");
        return SRC(mul);
    } else { // 当其中至少有一个是`VirtReg`
        // dbg("exit visitMul2 with at least 1 VirtReg");
        if (lhs.getType() == rhs.getType()) {
            SRC dst = SRC(new VirtReg(var_idx++, type));
            if (type == TypeInt) {
                LLIR_BIN *bop_inst = new LLIR_BIN(StrToBinOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(bop_inst);
            } else {
                LLIR_FBIN *fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(fbop_inst);
            }
            return dst;
        } else {
            if (lhs.getType() == TypeInt) {
                SRC cast_lhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_lhs), lhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                lhs = cast_lhs;
            } else if (rhs.getType() == TypeInt) {
                SRC cast_rhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_rhs), rhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                rhs = cast_rhs;
            }
            SRC dst = SRC(new VirtReg(var_idx++, type));
            LLIR_FBIN *fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, lhs, rhs);
            cur_basicblock->basic_block.push_back(fbop_inst);
            return dst;
        }
    }
}

// finished
antlrcpp::Any ASTVisitor::visitAdd1(SysYParser::Add1Context *ctx) {
    return ctx->mulExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitAdd2(SysYParser::Add2Context *ctx) {
    // dbg("enter visitAdd2");
    string op = ctx->children[1]->getText();
    SRC lhs = ctx->addExp()->accept(this);
    SRC rhs = ctx->mulExp()->accept(this);
    // dbg(lhs.ToString(), rhs.ToString());
    DeclType type = (lhs.getType() == rhs.getType()) ? lhs.getType() : TypeFloat;
    // 当两个操作数都是`CTValue`
    if (CTValue *ctv1 = lhs.ToCTValue(), *ctv2 = rhs.ToCTValue(); ctv1 != nullptr && ctv2 != nullptr) {
        int iadd = 0;
        float fadd = 0;
        if (op == "+") {
            iadd = ctv1->int_value + ctv2->int_value;
            if (type == TypeInt) {
                fadd  = iadd;
            } else {
                fadd = ctv1->float_value + ctv2->float_value;
            }
        } else if (op == "-") {
            iadd = ctv1->int_value - ctv2->int_value;
            if (type == TypeInt) {
                fadd  = iadd;
            } else {
                fadd = ctv1->float_value - ctv2->float_value;
            }
        }
        CTValue *add = new CTValue(type, iadd, fadd);
        // dbg(add->ToString());
        // dbg("exit visitAdd2 with 2 CTValue");
        return SRC(add);
    } else { // 当其中至少有一个是`VirtReg`
        // dbg("exit visitAdd2 with at least 1 VirtReg");
        if (lhs.getType() == rhs.getType()) {
            SRC dst = SRC(new VirtReg(var_idx++, type));
            if (type == TypeInt) {
                LLIR_BIN *bop_inst = new LLIR_BIN(StrToBinOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(bop_inst);
            } else {
                LLIR_FBIN *fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(fbop_inst);
            }
            return dst;
        } else {
            if (lhs.getType() == TypeInt) {
                SRC cast_lhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_lhs), lhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                lhs = cast_lhs;
            } else if (rhs.getType() == TypeInt) {
                SRC cast_rhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_rhs), rhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                rhs = cast_rhs;
            }
            SRC dst = SRC(new VirtReg(var_idx++, type));
            LLIR_FBIN *fbop_inst = new LLIR_FBIN(StrToBinOp(op), dst, lhs, rhs);
            cur_basicblock->basic_block.push_back(fbop_inst);
            return dst;
        }
    }
}

// finished
antlrcpp::Any ASTVisitor::visitRel1(SysYParser::Rel1Context *ctx) {
    return ctx->addExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitRel2(SysYParser::Rel2Context *ctx) {
    // dbg("enter visitRel2");
    // dbg(ctx->getText());
    string op = ctx->children[1]->getText();
    SRC lhs = ctx->relExp()->accept(this);
    SRC rhs = ctx->addExp()->accept(this);
    if (op == ">") {
        op = "<";
        std::swap(lhs, rhs);
    } else if (op == ">=") {
        op = "<=";
        std::swap(lhs, rhs);
    }
    // 当两个操作数都是`CTValue`
    // dbg("enter process visitRel2");
    if (CTValue *ctv1 = lhs.ToCTValue(), *ctv2 = rhs.ToCTValue(); ctv1 != nullptr && ctv2 != nullptr) {
        int irel = 0;
        float frel = 0;
        if (op == "<") {
            irel = ctv1->int_value < ctv2->int_value;
            frel = ctv1->float_value < ctv2->float_value;
        } else if (op == "<=") {
            irel = ctv1->int_value <= ctv2->int_value;
            frel = ctv1->float_value <= ctv2->float_value;
        }
        CTValue *rel = new CTValue(TypeBool, irel, frel);
        // dbg(rel->ToString());
        // dbg("exit visitRel2 with 2 CTValue");
        return SRC(rel);
    } else { // 当其中至少有一个是`VirtReg`
        // dbg("exit visitRel2 with at least 1 VirtReg");
        if (lhs.getType() == TypeBool) {
            SRC zext_src = SRC(new VirtReg(var_idx++, VarType(TypeInt)));
            LLIR_ZEXT *zext_inst = new LLIR_ZEXT(zext_src, lhs);
            cur_basicblock->basic_block.push_back(zext_inst);
            lhs = zext_src;
        }
        if (rhs.getType() == TypeBool) {
            SRC zext_src = SRC(new VirtReg(var_idx++, VarType(TypeInt)));
            LLIR_ZEXT *zext_inst = new LLIR_ZEXT(zext_src, rhs);
            cur_basicblock->basic_block.push_back(zext_inst);
            rhs = zext_src;
        }
        if (lhs.getType() == rhs.getType()) {
            SRC dst = SRC(new VirtReg(var_idx++, TypeBool));
            if (lhs.getType() == TypeInt) {
                LLIR_ICMP *icmp_inst = new LLIR_ICMP(StrToRelOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(icmp_inst);
            } else {
                LLIR_FCMP *fcmp_inst = new LLIR_FCMP(StrToRelOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(fcmp_inst);
            }
            return dst;
        } else {
            if (lhs.getType() == TypeInt) {
                SRC cast_lhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_lhs), lhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                lhs = cast_lhs;
            } else if (rhs.getType() == TypeInt) {
                SRC cast_rhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_rhs), rhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                rhs = cast_rhs;
            }
            SRC dst = SRC(new VirtReg(var_idx++, TypeBool));
            LLIR_FCMP *fcmp_inst = new LLIR_FCMP(StrToRelOp(op), dst, lhs, rhs);
            cur_basicblock->basic_block.push_back(fcmp_inst);
            return dst;
        }
    }
}

// finished
antlrcpp::Any ASTVisitor::visitEq1(SysYParser::Eq1Context *ctx) {
    return ctx->relExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitEq2(SysYParser::Eq2Context *ctx) {
    // dbg("enter visitEq2");
    // dbg(ctx->getText());
    string op = ctx->children[1]->getText();
    SRC lhs = ctx->eqExp()->accept(this);
    SRC rhs = ctx->relExp()->accept(this);
    // dbg(op, lhs.ToString(), rhs.ToString());
    // 当两个操作数都是`CTValue`
    // dbg("enter process visitEq2");
    if (CTValue *ctv1 = lhs.ToCTValue(), *ctv2 = rhs.ToCTValue(); ctv1 != nullptr && ctv2 != nullptr) {
        int ieq = 0;
        float feq = 0;
        if (op == "==") {
            ieq = ctv1->int_value == ctv2->int_value;
            feq = ctv1->float_value == ctv2->float_value;
        } else if (op == "!=") {
            ieq = ctv1->int_value != ctv2->int_value;
            feq = ctv1->float_value != ctv2->float_value;
        }
        CTValue *eq = new CTValue(TypeBool, ieq, feq);
        // dbg(eq->ToString());
        // dbg("exit visitEq2 with 2 CTValue");
        return SRC(eq);
    } else { // 当其中至少有一个是`VirtReg`
        // dbg("exit visitEq2 with at least 1 VirtReg");
        if (lhs.getType() == TypeBool) {
            SRC zext_src = SRC(new VirtReg(var_idx++, VarType(TypeInt)));
            LLIR_ZEXT *zext_inst = new LLIR_ZEXT(zext_src, lhs);
            cur_basicblock->basic_block.push_back(zext_inst);
            lhs = zext_src;
        }
        if (rhs.getType() == TypeBool) {
            SRC zext_src = SRC(new VirtReg(var_idx++, VarType(TypeInt)));
            LLIR_ZEXT *zext_inst = new LLIR_ZEXT(zext_src, rhs);
            cur_basicblock->basic_block.push_back(zext_inst);
            rhs = zext_src;
        }
        if (lhs.getType() == rhs.getType()) {
            SRC dst = SRC(new VirtReg(var_idx++, TypeBool));
            if (lhs.getType() == TypeInt) {
                LLIR_ICMP *icmp_inst = new LLIR_ICMP(StrToRelOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(icmp_inst);
            } else {
                LLIR_FCMP *fcmp_inst = new LLIR_FCMP(StrToRelOp(op), dst, lhs, rhs);
                cur_basicblock->basic_block.push_back(fcmp_inst);
            }
            return dst;
        } else {
            if (lhs.getType() == TypeInt) {
                SRC cast_lhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_lhs), lhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                lhs = cast_lhs;
            } else if (rhs.getType() == TypeInt) {
                SRC cast_rhs = SRC(new VirtReg(var_idx++, TypeFloat));
                LLIR_SITOFP *itf_inst = new LLIR_SITOFP(SRC(cast_rhs), rhs);
                cur_basicblock->basic_block.push_back(itf_inst);
                rhs = cast_rhs;
            }
            SRC dst = SRC(new VirtReg(var_idx++, TypeBool));
            LLIR_FCMP *fcmp_inst = new LLIR_FCMP(StrToRelOp(op), dst, lhs, rhs);
            cur_basicblock->basic_block.push_back(fcmp_inst);
            return dst;
        }
    }
}

// finished
antlrcpp::Any ASTVisitor::visitLAnd1(SysYParser::LAnd1Context *ctx) {
    return ctx->eqExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitLAnd2(SysYParser::LAnd2Context *ctx) {
    // dbg("enter visitLAnd2");
    SRC dst;
    dst = ctx->lAndExp()->accept(this);
    if (dst.getType() != TypeBool) {
        if (dst.getType() == TypeInt) {
            SRC icmp_dst = SRC(new VirtReg(var_idx++, VarType(TypeBool)));
            LLIR_ICMP *icmp_inst = new LLIR_ICMP(NEQ, icmp_dst, dst, SRC(new CTValue(TypeInt, 0, 0)));
            cur_basicblock->basic_block.push_back(icmp_inst);
            dst = icmp_dst;
        } else if (dst.getType() == TypeFloat) {
            SRC fcmp_dst = SRC(new VirtReg(var_idx++, VarType(TypeBool)));
            SRC zero_float = SRC(new VirtReg(var_idx++, TypeFloat));
            LLIR_SITOFP *itf_inst = new LLIR_SITOFP(zero_float, new CTValue(TypeInt, 0, 0));
            cur_basicblock->basic_block.push_back(itf_inst);
            LLIR_FCMP *fcmp_inst = new LLIR_FCMP(NEQ, fcmp_dst, dst, zero_float);
            cur_basicblock->basic_block.push_back(fcmp_inst);
            dst = fcmp_dst;
        }
    }
    LLIR_BR *br_inst = new LLIR_BR(true, dst, 0, 0);
    cur_basicblock->basic_block.push_back(br_inst);
    cur_scope_elements->push_back(cur_basicblock);
    cur_func->all_blocks.push_back(cur_basicblock);
    cur_basicblock = new BasicBlock(bb_idx++);
    br_inst->tar_true = cur_basicblock->bb_idx;
    dst = ctx->eqExp()->accept(this);
    land_insts.push_back(br_inst);
    // dbg("exit visitLAnd2");
    return dst;
}

// finished
antlrcpp::Any ASTVisitor::visitLOr1(SysYParser::LOr1Context *ctx) {
    return ctx->lAndExp()->accept(this);
}

// finished
antlrcpp::Any ASTVisitor::visitLOr2(SysYParser::LOr2Context *ctx) {
    // dbg("enter visitLOr2");
    // store last condition
    vector<LLIR_BR *> last_land_insts = land_insts;
    land_insts = vector<LLIR_BR *>();
    SRC dst;
    dst = ctx->lOrExp()->accept(this);
    for (auto land_inst : land_insts) {
        land_inst->tar_false = bb_idx;
    }
    land_insts = last_land_insts;
    if (dst.getType() != TypeBool) {
        if (dst.getType() == TypeInt) {
            SRC icmp_dst = SRC(new VirtReg(var_idx++, VarType(TypeBool)));
            LLIR_ICMP *icmp_inst = new LLIR_ICMP(NEQ, icmp_dst, dst, SRC(new CTValue(TypeInt, 0, 0)));
            cur_basicblock->basic_block.push_back(icmp_inst);
            dst = icmp_dst;
        } else if (dst.getType() == TypeFloat) {
            SRC fcmp_dst = SRC(new VirtReg(var_idx++, VarType(TypeBool)));
            SRC zero_float = SRC(new VirtReg(var_idx++, TypeFloat));
            LLIR_SITOFP *itf_inst = new LLIR_SITOFP(zero_float, new CTValue(TypeInt, 0, 0));
            cur_basicblock->basic_block.push_back(itf_inst);
            LLIR_FCMP *fcmp_inst = new LLIR_FCMP(NEQ, fcmp_dst, dst, zero_float);
            cur_basicblock->basic_block.push_back(fcmp_inst);
            dst = fcmp_dst;
        }
    }
    LLIR_BR *br_inst = new LLIR_BR(true, dst, 0, 0);
    cur_basicblock->basic_block.push_back(br_inst);
    cur_scope_elements->push_back(cur_basicblock);
    cur_func->all_blocks.push_back(cur_basicblock);
    cur_basicblock = new BasicBlock(bb_idx++);
    br_inst->tar_false = cur_basicblock->bb_idx;
    dst = ctx->lAndExp()->accept(this);
    lor_insts.push_back(br_inst);
    // dbg("exit visitLOr2");
    return dst;
}

// finished
antlrcpp::Any ASTVisitor::visitConstExp(SysYParser::ConstExpContext *ctx) {
    // dbg("enter constexp");
    SRC result = ctx->addExp()->accept(this);
    // dbg("exit constexp");
    return result;
}

#pragma clang diagnostic pop

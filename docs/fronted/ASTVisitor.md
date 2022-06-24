## ASTVisitor

```cpp
class ASTVisitor : public SysYBaseVisitor {
// Some Useful Defination
private:
    enum CompileMode { normal , compile_time, condition  } mode;
    DeclType type;
    int32_t whole_var_idx;
    CompUnit &ir;
    vector<Info *> *cur_scope_elements;
    Scope          *cur_scope;
    VariableTable  *cur_vartable;
    BasicBlock     *cur_basicblock;
    BasicBlock     *glbvar_init_bb;
    int32_t continue_target;
    vector<JumpInst *> break_insts;
public:
    bool have_main_func;
};
```
`ASTVisitor`, 由`SysYBaseVisitor`派生而来, 用于遍历语法树生成`IR`
`mode`记录当前所处的状态, 编译期状态, 条件状态, 其他状态
`type`记录当前声明的变量的类型
`ir`记录保存中间表示
`cur_scope_elements`指向当前作用域元素
`cur_scope`指向当前作用域
`cur_variable`指向当前作用域的符号表
`cur_basicblock`指向当前基本块
`glbvar_init_bb`指向全局变量的初始化块
`continue_target`记录`continue`语句的跳转目标
`break_insts`记录`break`语句用于后续填写`break`目标
`have_main_func`标记文件中是否有`main`函数

##### Function Describation
* vector<int32_t> get_array_dims(vector<SysYParser::ConstExpContext *> dims);
    我们在程序中保证只会在遇到数组定义时使用该函数
    遍历数组定义时首先将当前`type`切换为`TypeInt`
    然后遍历每一维的AST节点, 由于节点是`ConstExpContext`, 我们保证可以在求出每一维的值
    最后还原`type`, 返回得到的数组维度`vector`

* void parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t>& list)
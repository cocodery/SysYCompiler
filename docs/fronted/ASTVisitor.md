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
* `ASTVisitor`, 由`SysYBaseVisitor`派生而来, 用于遍历语法树生成`IR`
* `mode`记录当前所处的状态, 编译期状态, 条件状态, 其他状态
* `type`记录当前声明的变量的类型
* `ir`记录保存中间表示
* `cur_scope_elements`指向当前作用域元素
* `cur_scope`指向当前作用域
* `cur_variable`指向当前作用域的符号表
* `cur_basicblock`指向当前基本块
* `glbvar_init_bb`指向全局变量的初始化块
* `continue_target`记录`continue`语句的跳转目标
* `break_insts`记录`break`语句用于后续填写`break`目标
* `have_main_func`标记文件中是否有`main`函数

##### Function Describation
* vector<int32_t> get_array_dims(vector<SysYParser::ConstExpContext *> dims);
    - 获得声明的数组各维度的大小
    - 我们在程序中保证只会在遇到数组定义时使用该函数
    - 遍历数组定义时首先将当前`type`切换为`TypeInt`
    - 然后遍历每一维的AST节点, 由于节点是`ConstExpContext`, 我们保证可以在求出每一维的值
    - 最后还原`type`, 返回得到的数组维度`vector`

```antlr
constInitVal
    : constExp # scalarConstInitVal
    | '{' (constInitVal (',' constInitVal)* )? '}' # listConstInitVal
    ;
```
* void parse_const_init(SysYParser::ListConstInitValContext *node, const vector<int32_t> &array_dims, vector<int32_t/float>& list);
    - 获得常量数组声明的初值, 分为`int32_t`类型和`float`类型, 在这里我们统一说明
    - 首先求出当前初始化维度的大小`total_size`, 若`total_size == 0`则递归终止
    - 定义当前维度下已初始化的个数`cnt = 0`
    - 对当前维度下的节点进行遍历
    - - 如果是`ScalarConstInitValContext`, 计算该节点的值并填入`list`, `cnt += 1`
    - - 如果是`ListConstInitValContext`, 调用本函数递归的对子维度进行处理, `cnt += sub_arr_size`
    - 如果当前维度为填充满, 则补充填入`0`

```antlr
constDecl : 'const' bType constDef (',' constDef)* ';' ;
```
* antlrcpp::Any visitConstDecl(SysYParser::ConstDeclContext *ctx);
    - 获得当前的变量的声明类型
    - 遍历当前行下的常量声明


```antlr
constDef : Identifier ('[' constExp ']')* '=' constInitVal;
```
* antlrcpp::Any visitConstDef(SysYParser::ConstDefContext *ctx);
    - 获取常量名, 判断是否存在于当前作用域符号表下, 若存在则退出程序
    - 常量类型`VarType const_var`, 如果是数组需调用`get_array_dims`得到数组维度
    - 对常量初值进行分析
    - - 如果是标量, 直接获得初值
    - - 如果是向量，调用`parse_const_init`获得初值
    - 将常量名和其定义填入符号表

```antlr
constExp : addExp ;
```
* antlrcpp::Any visitConstExp(SysYParser::ConstExpContext *ctx);
    - 切换`mode = compile_time`
    - 分析子节点得到表达式的值
    - 恢复`mode`, 根据`type`返回得到的值

```antlr
funcDef : funcType Identifier '(' (funcFParams)? ')' block;
```
* antlrcpp::Any visitFuncDef(SysYParser::FuncDefContext *ctx);
    - 对函数声明和定义进行分析, 并将函数填入函数表
    - 首先对函数声明进行分析, 调用`ctx->funcFParams()->accept(this)`分析参数
    - 接着调用`ctx->block()->accept(this)`对函数体进行分析
    - 将函数填入函数表, 当前基本块交还`glbvar_init_bb`

```antlr
funcFParams : funcFParam (',' funcFParam)*;
```
* antlrcpp::Any visitFuncFParams(SysYParser::FuncFParamsContext *ctx);
    - 对函数参数进行分析
    - 调用`arg->accept(this)`将参数组装到列表
    - 返回参数列表

```antlr
funcFParam : bType Identifier ('[' ']' ('[' constExp ']')* )?;
```
* antlrcpp::Any ASTVisitor::visitFuncFParam(SysYParser::FuncFParamContext *ctx);
    - 对单个参数进行分析, 只保存参数类型
    - 如果是数组类型, 第一维度置为`-1`


```antlr
number
    : IntLiteral #number1
    | FloatLiteral #number2
    ; 
```
* antlrcpp::Any ASTVisitor::visitNumber1(SysYParser::Number1Context *ctx);
* antlrcpp::Any ASTVisitor::visitNumber2(SysYParser::Number2Context *ctx);
    - 分析数字常量的使用, 将其放入`CTValue`中
    - 类型设定为其定义的类型, 两个初值均使用定义的初值, 会隐式转换

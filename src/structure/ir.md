## IR

#### CTValue
```cpp
class CTValue {
public:
    DeclType type;
    int32_t int_value;
    float float_value;
};

CTValue operator + (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue lhs, CTValue rhs);
CTValue operator * (CTValue lhs, CTValue rhs);
CTValue operator / (CTValue lhs, CTValue rhs);
CTValue operator % (CTValue lhs, CTValue rhs);
CTValue operator - (CTValue rhs);
```
编译期可以计算出来的值, 例如数字 / 用常量声明的常量等
并对运算符进行重载, 可以自动进行类型转换

#### BasicBlock
```cpp
class BasicBlock: public Info {
public:
    vector<Inst *> basic_block; 
};
```
`BasicBlock`顾名思义就是基本块
它用来记录当前基本块内的的指令
当然目前没有进行到数据流分析的阶段
暂时没有加入基本块的前驱和后继

#### Scode
```cpp
class Scope: public Info {
public:
    VariableTable  *local_table;
    vector<Info *> *elements; // `Info` -> `Scope` or `BasicBlock`
    Scope* parent;
};
```
``Scope``用于保存一个作用域内的内容
当前作用域的符号表`local_table`, 作用域下的内容`elements`, 父作用域`parent`
当前作用域下的符号表的意义就不需要多说了
作用域下的内容, 即基本块或者子作用域, 我们的代码保证这里不会出现其他类型
父作用域主要用来查询父作用域的符号表来对本作用域使用的变量进行定位

#### Function
```cpp
class Function {
public:
    FunctionInfo func_info;
    Scope *main_scope;
};
```
有了前面的数据结构基础
对函数的定义就比较简单了
函数只需要`FunctionInfo`记录函数名和参数, `Scope`记录函数体的内容即可

#### LibFunction
```cpp
class LibFunction {
public:
    FunctionInfo libfunc_info;
    bool is_used;
};
```
这里记录了`SysY`的库函数的函数名和参数, 以及该函数是否被使用

#### CompUnit
```cpp
class CompUnit: public Info {
public:
    Scope *global_scope;
    vector<Function *> functions;
    LibFunction lib_functions[11];
};
```
`CompUnit`则是整个文件的变量, 函数相关信息
`global_scope`仅会记录全局符号表, 将函数的父作用域设置为它
若不设置成`Scope`而是`VariableTable`则会对访问全局符号表造成一些不必要的麻烦
`functions`和`lib_functions[11]`就不在此赘述了
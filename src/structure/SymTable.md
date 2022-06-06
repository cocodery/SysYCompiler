## SymTable

#### VarType
```cpp
class VarType {
public:
    bool is_const;
    bool is_array;
    bool is_args;
    bool is_init;
    vector<int32_t> array_dims;
    DeclType decl_type;
    int32_t int_scalar;
    float float_scalar;
    vector<int32_t> int_list;
    vector<float> float_list;
};
```
很明显的可以看出, `VarType`是用于记录某个变量在声明时的相关信息
is const ? is array ? is argument ? is init ?
and also its decl type
if it is array tpye, `array_dims` will record related infomation
and if it is inited, the init value is store in `int_scalar` or `float_scalar` or `int_list` or `float_list`
in fact it is a bad design because it waste a lot of space
but it is easy to use and understand, so we will not optimize this

#### Variable
```cpp
class Variable {
public:
    int var_idx;
    VarType type;
};
```
`Variable`记录了某个声明的变量的类型和编号
这个编号可能有用可能没用
后期的相关优化有可能会使用

#### VariableTable
```cpp
using VarPair = pair<string, Variable *>;
class VariableTable {
public:
    vector<VarPair> var_table;
}
```
`VarPair`用于封装变量名和变量的信息
这将为后续在符号表中对变量进行查询提供便利
我们设计了`findInCurTable`和`getInCurTable`用于判断查询的变量是否在符号表中和从符号表中取出变量

#### FunctionInfo
```cpp
class FunctionInfo {
public:
    string func_name;
    DeclType return_type;
    vector<VarType> func_args;
};
```
`FunctionInfo`用于记录函数名, 返回类型, 参数列表的信息
这将后续与`Function`结合使用
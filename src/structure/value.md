## Value

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

#### IRValue
```cpp
class IRValue {
public:
    VarType type;
    VirtReg reg;
    bool is_left_value;
};
```
编译期不能计算的值, 例如表达式计算出来的值
用于记录该值的类型, 存储的虚拟寄存器号, 是否为左值
并可以判断是否可以被赋值

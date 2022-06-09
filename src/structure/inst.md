## Inst

#### VirtReg
```cpp
class VirtReg: public Info {
public:
    int32_t reg_id;
};
```
虚拟寄存器, 目前用于记录寄存器号

#### UnaryOp
```cpp
class UnaryOp {
public:
    enum Type {
        NOT, NEG, POS,
    } unary_type;
};
```
封装单目运算符

#### BinaryOp
```cpp
class BinaryOP {
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR, LSH, RSH, 
    } bin_op;
};
```
封装双目运算符

## Inst

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
class BinaryOp {
    enum Type {
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR, LSH, RSH, 
    } bin_op;
};
```
封装双目运算符

#### ReturnInst
```cpp
class ReturnInst: public Inst {
public:
    bool has_retvalue;
    VirtReg dst;
};
```
return指令中间表示
是否具有返回值, 返回值所在的寄存器

#### LoadNumber
```cpp
class LoadNumber: public Inst {
public:
    CTValue src;
    VirtReg dst;
};
```
load常量中间表示
加载的常量, 目的虚拟寄存器

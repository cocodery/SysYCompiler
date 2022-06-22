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
        ADD, SUB, MUL, DIV, MOD, LTH, LEQ, EQU, NEQ, AND, ORR,
    } bin_op;
};
```
封装双目运算符

#### UnaryOpInst
```cpp
class UnaryOpInst: public Inst {
public:
    UnaryOp op;
    VirtReg dst, src;
};
```
UnaryOpInst 指令
包括单目操作符、目的寄存器、源寄存器

#### BinaryOpInst
```cpp
class BinaryOpInst: public Inst {
public:
    BinaryOp op;
    VirtReg dst, src1, src2;
    bool need_cast;
};
```
BinaryOpInst 指令
包括双目操作符、目的寄存器、源寄存器1、源寄存器2
是否需要类型转换 (暂时没用到, 后期应该会改

#### ReturnInst
```cpp
class ReturnInst: public Inst {
public:
    bool has_retvalue;
    VirtReg dst;
};
```
ReturnInst 指令
包括是否具有返回值, 返回值所在的寄存器
属于跳转语句, 可以划分基本块

#### LoadNumber
```cpp
// load a constant to register
// ld.c reg.X, Y
class LoadNumber: public Inst {
public:
    CTValue src;
    VirtReg dst;
};
```
LoadNumber 指令
包括加载的常量, 加载到的寄存器
将常量加载道对应的寄存器

#### StoreMem
```cpp
// load value in register to address in register
// st [reg.X], reg.Y
class StoreMem: public Inst {
public:
    VirtReg dst, src;
};
```
StoreMem 指令
包括保存的地址, 保存值所在的寄存器
将右值寄存器中的值加载到左值寄存器对应的地址

#### LoadAddress
```cpp
// load an address of variable to register
// ld.a reg.X, addr
class LoadAddress: public Inst {
public:
    VirtReg dst;
    Variable *variable;
};
```
LoadAddress 指令
包括加载到的寄存器, 加载的变量
将所需变量的地址装入寄存器

#### LoadValue
```cpp
// load value in [reg.Y] to reg.X
// ld.v reg.X, [reg.Y]
class LoadValue: public Inst {
public:
    VirtReg dst, src;
};
```
LoadValue 指令
包括加载到的寄存器, 加载值的地址
将右值寄存器中的地址对应的值加载到左值寄存器

#### LoadOffset
```cpp
// add array offset to addr
// cur_dim_size is `size`, `off` is offset num
// ld.o reg.X, reg.Y, reg.Z * size
class LoadOffset: public Inst {
public:
    VirtReg dst, addr, off;
    int32_t size;
};
```
LoadOffset 指令
包括保存偏移后地址的寄存器, 被计算偏移的地址
由偏移的个数和偏移的维度组成的偏移量
将地址与偏移量相加得到的地址存入目的寄存器

#### JzeroInst
```cpp
class JzeroInst: public Inst {
public:
    VirtReg cond;
    int32_t bb_idx;
};
```
JzeroInst 指令
包括条件跳转的值, 和跳转到的地址
若条件的值为`0`则跳转

#### JnzroInst
```cpp
class JnzroInst: public Inst {
public:
    VirtReg cond;
    int32_t bb_idx;
};
```
JnzroInst 指令
包括条件跳转的值, 和跳转到的地址
若条件的值不为`0`则跳转

#### JumpInst
```cpp
class JumpInst: public Inst {
public:
    int32_t bb_idx;
};
```
JnzroInst 指令
包括和跳转到的地址
无条件跳转

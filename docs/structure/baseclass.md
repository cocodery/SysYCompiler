## BaseClass

#### Info
```cpp
class Info {
public:
    virtual ~Info() { }
};
```
`Info`基类, 主要用于派生`BasicBlock`和`Scope`

#### Inst
```cpp
class Inst: public Info {
public:
    virtual ~Inst() { }
};
```
`Inst`基类, 主要用于派生各种指令类

#### VirtReg
```cpp
class VirtReg: public Info {
public:
    int32_t reg_id;
};
```
虚拟寄存器, 目前用于记录寄存器号

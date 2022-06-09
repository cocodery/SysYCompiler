## BaseClass

#### Info
```cpp
class Info {
public:
    virtual ~Info() { }
};
```
基类

#### Inst
```cpp
class Inst: public Info {
public:
    virtual ~Inst() { }
};
```
基类

#### VirtReg
```cpp
class VirtReg: public Info {
public:
    int32_t reg_id;
};
```
虚拟寄存器, 目前用于记录寄存器号
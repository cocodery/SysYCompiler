- CodeGen
注意事项：数组全是指针，标量全是变量

1. LiveUse生成：如果一个函数的参数没有在函数体内被使用，可能会出问题（因为LiveUse和LiveDef中都没有它，但是传参的时候要用到）
2. 减法取负优化 - 浮点可能出问题
3. 暂时没有考虑函数参数超过四个的情况
4. cmp指令没有交换，后期或许可以优化
5. fcmp指令返回值没分配寄存器，可能是个坑
6. mov整数的方式或许可以优化 (灵活的操作数2):
```
https://developer.arm.com/documentation/dui0068/b/ARM-Instruction-Reference/ARM-general-data-processing-instructions/Flexible-second-operand?lang=en
```
7. param中的str长度(128)
8. mod
9. ltorg
10. borrow逻辑：得找没有被当前语句使用的寄存器才行
11. 以下内容可根据参数个数和返回值的情况进行优化
```c
#define LOCAL_VARS_SAVE_MEMCPY_REGISTERS {r0, r1, r2, lr}
#define LOCAL_VARS_LOAD_MEMCPY_REGISTERS {r0, lr}
```
12. call时候的寄存器压栈：有些寄存器里存的变量已经在LiveInterval的末期，不用压栈
13. call时候移动参数：有时候没有冲突，不用交换
14. 改变了AddSub的逻辑，现在dst和src大多数情况下可以相同
- IR
1. LLIR_RET指令：对于如下代码段
```c
float main(){
    return 3.0;
}
```
生成的LLIR_RET指令中，`ret_inst->ret_value.ctv->int_value`的值是3，但`ret_inst->ret_value.ctv->float_value`的值是0
- 前端
1. 非const全局变量初始化优化
2. 常量传播可以节省后面寄存器分配，一定要做

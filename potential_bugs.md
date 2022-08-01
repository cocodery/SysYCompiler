- CodeGen
1. LiveUse生成：如果一个函数的参数没有在函数体内被使用，可能会出问题（因为LiveUse和LiveDef中都没有它，但是传参的时候要用到）
- IR
1. LLIR_RET指令：对于如下代码段
```c
float main(){
    return 3.0;
}
```
生成的LLIR_RET指令中，`ret_inst->ret_value.ctv->int_value`的值是3，但`ret_inst->ret_value.ctv->float_value`的值是0

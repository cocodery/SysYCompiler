### Environment Config
* **1. OS**
    > Ubuntu 20.04 , or other Linux (develop environment)

    > Windows

* **2. Packages needed**
    > build-essential / git / cmake
    > make / antlr / clang / llvm /
    > ..

* **3. Test Platform**
    > Raspberrypi

    > LLVM-Interpreter

* **4. Test via LLVM-Interpreter**
    > We can see that `run` and `test` commands are all use llvm-link. 
    > And we can see no `sylib.ll` here. 
    > we need to compile `sylib.c` in `compiler2022/公开样例与运行时库/` and move to root directory

### Usages
* Make Commands
    > make build
        - compile this project
    
    > make run 
        - run main.sy in root directory

    > make test TEST=%test-case-name%
        - run test-case in compiler2022/公开样例与运行时库/functional

    > make gdb / make lldb
        - use gdb or lldb to debug

### Commit Message Rules

* **1. Add files** : 
    Add a new file in directory or add a new function in a file.
    > **Add File xxx, Add: Func xxx**
    xxx is directory name or function name.

    > Remember to writing notes for the new file or function

* **2. Delete files** : 
    Delete a existing file in directory or delete a function in a file.
    > **Del File xxx, Del: Func xxx**
    > xxx is directory name or function name. And witre reason why delete this file or function in message.

    > don't use in most situation

* **3. Modify files** :
    Modify a function in some file.
    > **Mod File xxx, Mod Func xxx**
    not need to modify the arguments type and return type, just optimize the performance of these function.


**TIPS:** if your commit involves multiple message, you can use **&&** to connect them.

**Docs Standard**
> 不要使用中文逗号, 用英文逗号和空格代替, ", "

> please decribe the data structure you design, about what it do, what main functions it has, and whether have some key algorithm at `README` in that directory

**Log**
> **2021.09.09** : can parser `SysY2021`

> **~ 2022.2.15**: add functions to parse `ConstDef`

> **~** : add structures like `Function`, `SymTable`, .. for parser

> **2022.3.22**: can parser `SysY2022v1`

> **~ 2022.4.22**: add functions to debug `FuncDecl` and `Global ConstDef`

> **~ 2022.4.28**: add functions to parse local `ConstDef`

> **~ 2022.4.30**: add functions to parse `ReturnStmt` 

> **~2022.6.17**: basicly finish all hand-made ir

> **~2022.7.10**: rebuild and use llvm-ir, support basic type, no control flow

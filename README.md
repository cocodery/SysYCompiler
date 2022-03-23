### Environment Config
* **1. OS**
    > Ubuntu 20.04 , or other Linux (develop environment)
    > Windows

* **2. Packages needed**
    > build-essential
    > git
    > cmake
    > antlr
    > ..

### Usages
* 
    > cd SysYCompiler
    > mkdir build && cd build
    > cmake ..
    > make -j
    > ./compiler -S -o [dst_file] [src_file]

### Commit Message Rules

* **1. Add files** : 
    > Add a new file in directory or add a new function in a file.

    > **Add File xxx, Add: Func xxx**
    xxx is directory name or function name.

    > Remember to writing notes for the new file or function

* **2. Delete files** : 
    > Delete a existing file in directory or delete a function in a file.

    > **Del File xxx, Del: Func xxx**
    > xxx is directory name or function name. And witre reason why delete this file or function in message.

    > don't use in most situation

* **3. Modify files** :
    > Modify a function in some file.

    > **Mod File xxx, Mod Func xxx**
    not need to modify the arguments type and return type, just optimize the performance of these function.


**TIPS:** if your commit refer to not one commit message above, you can use **&&** to connect them.

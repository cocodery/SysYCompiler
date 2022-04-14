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

* **3. Test Platform**
    > Raspberrypi

    > Qemu

* **4. Qemu Config**
    > * sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk bison flex texinfo gperf libtool patchutils bc  zlib1g-dev libexpat-dev pkg-config  libglib2.0-dev libpixman-1-dev tmux python3 python3-pip
    > * sudo apt install gcc-arm-linux-gnueabi
    > * wget https://download.qemu.org/qemu-5.0.0.tar.xz
    > * tar xvJf qemu-5.0.0.tar.xz
    > * cd qemu-5.0.0

### Usages
* Make Commands
    > cd SysYCompiler

    > make build
    
    > make run

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


**TIPS:** if your commit refer to not one commit message above, you can use **&&** to connect them.

# RT-Thread Nano Template移植

## xmake操作

 1、编译

```c
xmake
```

2、根据xmake配置生成makefile

```c
 xmake project -k makefile
```

3、切换到debug模式编译

```c
xmake f -m debug   // 切换为debug模式 
xmake    //编译
```

4、编译且输出警告信息

```text
xmake -w 或 xmake --warning
```

5、看详细的编译参数信息

```c
xmake -v 或 xmake --verbose
```

6、编译以及获取出错时xmake的调试栈信息

```c
xmake -v --backtrace
```

## RT-Thread nano 自动初始化段添加

```c
 /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))
  
    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */

           /* section information for finsh shell */
    . = ALIGN(4);
    __fsymtab_start = .;
    KEEP(*(FSymTab))
    __fsymtab_end = .; */
 
    . = ALIGN(4);
    __vsymtab_start = .;
    KEEP(*(VSymTab))
    __vsymtab_end = .; 

    /* section information for initial. */
    . = ALIGN(4);
    __rt_init_start = .;
    KEEP(*(SORT(.rti_fn*)))
    __rt_init_end = .;
  
  } >FLASH

```

RT-Thread nano在gcc下自动初始化段不能失效，原因是gcc的自定义段需要在链接脚本中添加自定义段

添加的自定义段如下：

```c
         /* section information for finsh shell */
    . = ALIGN(4);
    __fsymtab_start = .;
    KEEP(*(FSymTab))
    __fsymtab_end = .; */
 
    . = ALIGN(4);
    __vsymtab_start = .;
    KEEP(*(VSymTab))
    __vsymtab_end = .; 

    /* section information for initial. */
    . = ALIGN(4);
    __rt_init_start = .;
    KEEP(*(SORT(.rti_fn*)))
    __rt_init_end = .;
```


注意！！!

注意！！!

注意！！!

xmake.lua是工程构建文件，不能删除


## xmake编译需要注意的问题

1、xmake.lua 如果是其他工程拷贝过来的，可以删除.xmake文件夹和build文件夹，或者执行 `xmake f -c`强制清除配置缓存，主要是.xmake缓存了上一个工程的配置信息等，会编译失败

2、修改链接地址后不生效，生成反汇编如果不生效的话，先 `xmake clean`清除上一次编译缓存的obj文件，再执行xmake编译


## 编译问题解决

STM32cubeIDE F4移植RTT系统 context_gcc.s 报错_鸽子兄弟的博客-CSDN博客
[https://blog.csdn.net/LongQingyun1314/article/details/105200315](https://blog.csdn.net/LongQingyun1314/article/details/105200315)



# 1、context_gcc.s 报错cortex-m4/context_gcc.S:119: Error: thumb conditional instruction should be in IT block -- `moveq r4,#0x01'
参考：[https://blog.csdn.net/LongQingyun1314/article/details/105200315](https://blog.csdn.net/LongQingyun1314/article/details/105200315)

# 2、自定义外部SDRAM段之后生成bin文件过大问题解决

参考：[https://shequ.stmicroelectronics.cn/thread-632170-1-1.html](https://shequ.stmicroelectronics.cn/thread-632170-1-1.html)
总结来说就是bin文件时按照地址信息顺序生成代码段和可加载数据段的文件，自定义的SDRAM段默认属性时LOAD，也就是可加载的。程序链接时将该段人为可加载的数据段链接进程序造成bin文件特别大。所以修改段的默认属性为`NOLOAD(`不可加载)即可：

```c
  .extsdram (NOLOAD) :
  {
      . = ALIGN(4);
      KEEP(*(.extsdram)) 
      . = ALIGN(4);
  } >SDRAM 
```

# 3、启动文件一般都是bl main，跳到执行main，这样是不能启动RT-Thread
查看RT-Thread源码启动流程就知道RT-Thread是在`entry函数`中初始化启动，所以需要将启动文件的bl main修改`bl entry`

# 4、STM32CubeIDE新建空的STM32工程生成的启动文件有问题

问题1：在设置完栈之后就调用SystemInit

```c
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

  bl  SystemInit
```
对比STM32CubeMX生成CubeIDE工程的启动文件都是在`bl __libc_init_array`之前调用  `bl  SystemInit`  

```c
/* Call the clock system intitialization function.*/
  bl  SystemInit   
/* Call static constructors */
    bl __libc_init_array
```
改成这样之后就能启动RT-Thread，但一显示lvgl界面就死机了，LTDC和DMA2D使用了中断。

问题2：生成的启动文件中断向量表有问题

```c
  .word	OTG_HS_IRQHandler            			/* USB On The Go HS global interrupt                                  */
  .word	DCMI_IRQHandler              			/* DCMI global interrupt                                              */
  .word	CRYP_IRQHandler              			/* CRYP crypto global interrupt                               
```
同样对比STM32CubeMX生成的启动文件，正确应该是这样的：

```c
  .word     OTG_HS_IRQHandler                 /* USB OTG HS                   */                   
  .word     DCMI_IRQHandler                   /* DCMI                         */                   
  .word     0                                 /* Reserved                     */                   
  .word     HASH_RNG_IRQHandler               /* Hash and Rng                 */
```
修改完中断向量后RT-Thread就正确运行了。

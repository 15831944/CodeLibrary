# <cassert\>(assert.h)头文件

##概述 
assert.h是C标准函数库中的头文件.其中定义了assert()宏用于程序调试.  
在C标准函数库中,它是个非常特别的头文件,你可以将它引入数次以获得不同的效果,此效果依引入时是否以定义NDEBUG而定.  

##成员
*assert宏*:  
>   void assert (int expression);  
>   assert宏()是一个诊断宏,用于动态辨识程序的逻辑错误条件.  
>   如果宏的参数求值结果为非零值,则不做任何操作(no action)；如果是零值,用宽字符打印诊断消息,然后调用abort().  
>   诊断消息包括：
>   >   源文件名字(在stdlib.h中声明的宏__FILE__的值)  
>   >   所在的源文件的行号(在stdlib.h中声明的宏__LINE__的值)  
>   >   所在的函数名(在stdlib.h中声明的宏__func__的值),这是C99新增的特性  
>   >   求值结果为0的表达式  

>   诊断信息的显示目标依赖于被调用程序的类型.如果是控制台程序,诊断信息显示在stderr设备；如果是基于窗口的程序,assert()产生一个Windows MessageBox来显示诊断信息.  
>   通常控制台的信息如下:  
>   >   Assertion failed: expression, file filename, line line number  

>   程序可以屏蔽掉所有的assert()而无需修改源代码.这只需要在命令行调用C语言的编译器时添加宏定义的命令行选项,定义DNDEBUG宏;也可以在源程序程序引入<assert.h>之前就使用#define NDEBUG来定义宏.被屏蔽的assert()甚至不对传递给它的参数表达式求值,因此使用assert()时其参数表达式不能有副作用(side-effects).  
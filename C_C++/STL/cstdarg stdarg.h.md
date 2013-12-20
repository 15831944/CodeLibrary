#<cstdarg\> (stdarg.h)

##概述
stdarg.h是C语言中C标准库的头文件,stdarg是由stdandard(标准)arguments(参数)简化而来,主要目的为让函数能够接收不定量参数.  

##成员
类型  
**va_list**:  
>   类型va_list用来保存可变参数的信息,供宏va_start,va_arg和va_end使用.  
>   此类型的实现,在各个库的实现中可能不一样.  
>   va_list被va_start初始化之后,必须使用va_end结束.  

宏  
**va_start**:  
>   void va_start (va_list ap, paramN)  
>   使用paramN之后的参数,初始化ap.  

**va_arg**:  
>   type va_arg (va_list ap, type)  
>   获取ap的下一个参数,获取的参数描述为type所指的类型.并返回  
>   每次调用,都会改变ap的状态,并使下一次调用,返回下一个参数.  
>   va_arg不能判定返回的参数是否为传入的最后一个参数.  

**va_end**:  
>   void va_end (va_list ap);  
>   结束可变参数ap.  
>   若调用了va_start,则应在函数返回之前调用va_end.  


##可变参数说明
可变参数函数的参数数量是可变动的,它使用省略号来忽略之后的参数.  
    int check(int a, double b, ...);  
可定参数函数最少要有一个命名的参数.  


##参考
*   <http://www.cplusplus.com/reference/cstdarg/>
*   <https://zh.wikipedia.org/wiki/Stdarg.h>



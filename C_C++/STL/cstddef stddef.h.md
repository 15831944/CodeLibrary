#<cstddef\> (stddef.h)

##概述
stddef.h是C标准函数库中的头文件,定义了若干常见的类型与宏.  

##成员
类型  
**ptrdiff_t**:  
>   指针减法的结果.  
>   它是一个有符号整形.  

**size_t**:  
>   无符号整数型.  

宏:  
**offsetof**:  
>   offsetof (type,member)  
>   返回结构体或枚举成员的偏移量(以字节为单位).  
>   返回结果为size_t类型.  
>   起点为0.  

**NULL**:  
>   空指针.  

##参考
*   <http://www.cplusplus.com/reference/cstddef/>
*   <https://zh.wikipedia.org/wiki/Stddef.h>
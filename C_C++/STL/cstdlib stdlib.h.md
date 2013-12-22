#<cstdlib\> (stdlib.h)

##概述
stdlib.h是C标准函数库的头文件,声明了数值与字符串转换函数,伪随机数生成函数,动态内存分配函数,进程控制函数等公共函数.  

##成员
字符串转换  
**atof**:字符串转换为浮点数.  
>   double atof (const char* str);  
>   字符串开头的空格会被丢弃,从第一个非空格开始转换,然后尽可能多的转换浮点数,最后一个合法的浮点字符之后的字符也会被丢弃.  
>   如果浮点数超出范围,则函数的行为是不确定的.  
>   如果转换失败,返回0.  

**atoi**:字符串转换为整型.  
>   字符串开头的空格会被丢弃,从第一个非空格开始转换,可以有正/负符号,以10进制转换.  
>   若转换失败,返回0.  

**atol**:字符串转换为长整型.  
>   long int atol ( const char * str );  

**strtod**:字符串转换为浮点数.  
>   double strtod (const char* str, char** endptr);  
>   如果endptr不为空,其将指向数值字符之后的第一个字符.  
>   字符串开头的空格会被丢弃,从第一个非空格开始转换,然后尽可能多的转换浮点数,最后一个合法的浮点字符之后的字符也会被丢弃.  

**strtol**:字符串转换为整型.  
>   long int strtol (const char* str, char** endptr, int base);  
>   如果endptr不为空,其将指向数值字符之后的第一个字符.  
>   base可以指定进制,若为0,则为10进制,其值可为2-32或0,字符串可以选择的添加前缀如"0"\"0x"\"0X".  

**strtoul**:字符串转换为整型.  
>   unsigned long int strtoul (const char* str, char** endptr, int base);  
>   转换为无符号整型.  

伪随机数生成器  
**rand**:生成随机数.  
>   返回的随机数值为0-RAND_MAX.  

**srand**:初始化伪随机数生成器.  
>   void srand (unsigned int seed);  

动态内存分配  
**calloc**:  
>   void* calloc (size_t num, size_t size);  
>   分配num块大小为size的内存,其被初始化为0.  
>   若成功,返回分配的内存块,否则,返回空.  

**free**:  
>   void free (void* ptr);  
>   释放由malloc\calloc\realloc分配的内存,并使其可被再次分配.  
>   如果ptr不是指向由上述三函数分配的内存,则行为不确定.  
>   如果ptr为空,则函数什么也不做.  

**malloc**:  
>   void* malloc (size_t size);  
>   分配size大小的内存块,分配的内存没有初始化.  

**realloc**:  
>   void* realloc (void* ptr, size_t size);  
>   分配size大小的内存块,并将ptr指向的内存复制到新的内存块.  
>   如果ptr为空,则函数行为同malloc.  

环境相关  
**abort**:中止当前进程.  
>   void abort (void);  
>   函数会产生SIGABRT信号.  
>   此时不会执行由atexit\at_quick_exit函数设置的函数.  

**atexit**:设置程序退出时执行的函数.  
>   int atexit (void (*func)(void));
>   如果设置多个函数,则最后设置的函数,最先执行.  
>   单个函数可以多次设置.  
>   如果函数在exit之后调用,则行为依赖于实现.  

**exit**:结束程序.  
>   void exit (int status);  

**getenv**:获取环境变量.  
>   char* getenv (const char* name);  
>   返回name指定的环境变量值.  
>   由此函数返回的字符串不能被改变.  

**system**:执行系统命令.  
>   int system (const char* command);  

整型算术  
**abs**:求绝对值.  
>   int abs (int n);  

**div**:整数除法.  
>   div_t div (int numer, int denom);  
>   通过div_t结构返回numer/denom的商和余数.  
>   div_t结构值有两个成员:quot, rem.  

多字节字符  
**mblen**:获取多字节字符的长度.  
>   int mblen (const char* pmb, size_t max);  

##参考
*   <http://www.cplusplus.com/reference/cstdlib/>
*   <https://zh.wikipedia.org/zh-hk/Stdlib.h>

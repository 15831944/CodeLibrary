#<cstring\> (string.h)

##概述
string.h是C语言中C标准库的头文件,其中包含了宏定义、常量以及函数和类型的声明,涉及的内容除了字符串处理之外,还包括大量的内存处理函数:因此,string.h这个命名是不恰当的.  
在string.h中定义的函数十分常用,作为C标准库的一部分,它们被强制要求可以在任何支持C语言的平台上运行.但是,部分函数存在一些安全隐患,例如缓存溢出等,导致程序员宁愿使用一些更安全的函数而放弃一定的可移植性.同时,这些字符串函数只能处理ASCII字符集或兼容ASCII的字符集,如ISO-8859-1:在处理存在多字节字符的字符集,如UTF-8时,会产生一个警告,指出对字符串"长度"的计算是以字节而不是以Unicode字符为单位.非ASCII兼容字符集的字符串处理函数一般位于wchar.h中.  

##成员
复制  
**memcpy**:内存块拷贝.  
>   void * memcpy ( void * destination, const void * source, size_t num );  
>   进行二进制的数据拷贝,并且总是复制num字节的数据.  
>   为了避免溢出,destination和source应该至少有num字节,并且不要重叠.  

**memmove**:移动内存块.  
>   void * memmove ( void * destination, const void * source, size_t num );  
>   destination和source应该至少num字节大小.  
>   移动操作使用了一个中间缓存,以充许destination和source重叠.  

**strcpy**:复制字符串.  
>   char * strcpy ( char * destination, const char * source );  
>   复制source到destination,'\0'也会被复制.  
>   destination应该足够大,以容纳source的内容,并且不能重叠.  

**strncpy**:复制字符串.  
>   char * strncpy ( char * destination, const char * source, size_t num );  
>   复制source开头的num个字符到destination中,如果在复制num个字符之前,遇到了source的'\0',则destination后面的字符以0替换.   
>   '\0'不会自动添加,如果source比destination长,此时,destination就不再是一个正确的C字符串了.  
>   destination和source不能重叠.  

连接  
**strcat**:连接字符串.  
>   char * strcat ( char * destination, const char * source );  
>   将source连接到destination后面,destination的'\0'被source的第一个字符覆盖,并在连接完后,添加一个'\0'.  
>   destination和source不能重叠.  

**strncat**:连接字符串.  
>   char * strncat ( char * destination, const char * source, size_t num );  
>   将source中最多num个字符连接到destination上,并且在后面添加'\0'.  

比较  
**memcmp**:比较内存块.  
>   int memcmp ( const void * ptr1, const void * ptr2, size_t num );  
>   比较ptr1和ptr2的前num个字节的内容,不论是否遇到'\n'.  

**strcmp**:比较字符串.  
>   int strcmp ( const char * str1, const char * str2 );  
>   直到比较出大小,或遇到'\0'时结束.  

**strcoll**:比较两个字符串.  
>   int strcoll ( const char * str1, const char * str2 );  
>   
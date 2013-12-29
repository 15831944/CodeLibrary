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
>   根据LC_COLLATE比较字符串.在比较出大小或遇到'\0'时结束.  

**strncmp**:比较两个字符串.  
>   int strncmp ( const char * str1, const char * str2, size_t num );  
>   比较两个字符串,最多比较num个字符.  

**strxfrm**:转换字符串.  
>   size_t strxfrm ( char * destination, const char * source, size_t num );  
>   使用locale转换字符串,并将前num个字符复制到destination中.  
>   返回转换之后的字符串长度.  
>   destination和source不能重叠.  

**memchr**:查找字符.  
>   void * memchr (       void * ptr, int value, size_t num );  
>   在前num个字节中查找value的值,value被解释为unsigned char.  
>   返回找到的字符的地址.  

**strchr**:查找字符.  
>   char * strchr (       char * str, int character );  
>   返回找到的字符地址,若没找到,返回null.  

**strcspn**:获取在指定字符串中的字符的索引.  
>   size_t strcspn ( const char * str1, const char * str2 );  
>   在str1中查找,任何一个str2中的字符,返回其索引.  

**strpbrk**:获取在指定字符串中的字符的地址.  
>   char * strpbrk (       char * str1, const char * str2 );  
>   在str1中查找,任何一个str2中的字符,返回其地址.  

**strrchr**:从字符串中返回最后出现的字符.  
>   char * strrchr (       char * str, int character );  
>   返回str中,character最后出现的字符地址.  

**strspn**:返回字符串的范围.  
>   size_t strspn ( const char * str1, const char * str2 );  
>   返回str1中字符,连续是str2中的长度.  

**strstr**:返回子字符串的位置.  
>   char * strstr (       char * str1, const char * str2 );  
>   返回str1中,str2字符串第一次出现的位置,不包括'\0'.  

**strtok**:分隔字符串,并存入tokens中.  
>   char * strtok ( char * str, const char * delimiters );  
>   此函数一系列的调用,将str用delimiters中任意的一个字符分隔,并在每次调用时依次返回,除第一次调用外,传入str,之后都传入NULL,若没有字符串可供返回,返回NULL.  

其它  
**memset**:填充内存.  
>   void * memset ( void * ptr, int value, size_t num );  
>   将ptr的前num字符用value(解释为unsigned char)填充.  

**strerror**:返回错误字符串.  
>   char * strerror ( int errnum );  
>   返回的字符串为静态分配的.  

**strlen**:返回字符串长度.  
>   size_t strlen ( const char * str );  
>   通过'\0'标志字符串,不包括'\0'.  

宏  
**NULL**:  

类型  
**size_t**;  



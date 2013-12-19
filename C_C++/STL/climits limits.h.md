#<climits\> (limits.h)

##概述
limits.h是C标准函数库中的头文件,定义了整数类型的一些极限值.  

##成员
宏  
    CHAR_BIT    字节的最小位数：8  
    SCHAR_MIN   有符号字符类型的最小值：-127  
    SCHAR_MAX   有符号字符类型的最大值：+127  
    UCHAR_MAX   无符号字符类型的最大值：255  
    CHAR_MIN    字符类型的最小值: SCHAR_MIN 或者 0  
    CHAR_MAX    字符类型的最大值: SCHAR_MAX 或者 UCHAR_MAX  
    MB_LEN_MAX  多字节字符在任何locale中都适用的最长字节数：1  
    SHRT_MIN    短整型最小值：-32767,即-(2^15 - 1)  
    SHRT_MAX    短整型最大值：+32767,即 2^15 - 1  
    USHRT_MAX   无符号短整型最大值：65535 ,即 2^16 - 1  
    INT_MIN     整型最小值：-32767 ,即 -(2^15 - 1)  
    INT_MAX     整型最大值：+32767 ,即2^15 - 1   
    UINT_MAX    无符号整型最大值：65535 ,即2^16 - 1  
    LONG_MIN    长整型最小值：-2147483647 ,即-(2^31 - 1)  
    LONG_MAX    长整型最大值：+2147483647 ,即2^31 - 1  
    ULONG_MAX   无符号长整型最大值：4294967295 ,即2^32 - 1  
    LLONG_MIN   长长整型最小值：-9223372036854775807 ,即-(2^63 - 1)  
    LLONG_MAX   长长整型最大值：+9223372036854775807 ,即2^63 - 1  
    ULLONG_MAX  无符号长长整型最大值：18446744073709551615 ,即2^64- 1  


##参考
*   <http://www.cplusplus.com/reference/climits/>
*   <http://zh.wikipedia.org/zh-cn/Limits.h>
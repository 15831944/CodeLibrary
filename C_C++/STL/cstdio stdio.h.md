#<cstdio\> (stdio.h)

##概述
C语言为文件输入输出提供了许多标准库函数.这些库函数构成了C标准库头文件<stdio.h\>的主体.它们的功能大多数都继承自麦克·列斯克在70年代早期于贝尔实验室所编写的一个"可移植的I/O程序库".  
按照现在的视角来看,C语言的I/O功能相当底层:C语言将所有的文件操作符都抽象成了字节流(如标准输入流和标准输出流).不像其他早期的编程语言,C语言没有对于从文件随机位置读取的直接支持:为了从一个文件的中间部分读取内容,程序员需要创建一个文件流,定向到文件的中间部分,然后从文件流中按顺序读取一个又一个的字节.  
在cstdio库中流由FILE指针处理,一个FILE指针唯一的标识了一个流,并且做为参数传入到流的处理函数中.  
有三个标准流:stdin\stdout\stderr,它们会在程序中自动被创建.  

每个流都有一些属性,定义了对它们可以使用什么函数,和怎样处理数据.大多数属性是在fopen函数中定义的,这些属性如下:  
*   Read/Write: 是否有读或写的权限;  
*   Text/Binary: 文本文件或二进制文件;  
*   Buffer: 流有三种缓存:fully buffered\line buffered\unbuffered,fully buffered是在缓存充满之后才开始读或写,line buffered在遇到换行符时读写,unbuffered的流的读写是直接实时的.  
*   Orientation: 一个流可以是byte-oriented或wide-oriented,在cstdio中定义的流大都是byte-oritened.  

每个流都有一些标志,以标识流的当前状态:  
*   Error indicator: 在发生错误时被置上,可以通过ferror()函数检查,可以通过clearerr\freopen\rewind函数重置.  
*   End-Of-File indicator: 标识已经遇到了end-of-file,它可以通过feof()函数检查,可以通过clearerr\freopen函数重置,或者通过重新定位函数rewind\fseek\fsetpos改变.  
*   Position indicator: 指示流中下了个读/写的位置,可以通过fgetpos()函数返回,可能通过rewind\fseek\fsetpos改变.  

##成员
文件操作  
**remove**:删除文件  
>   int remove ( const char * filename );  
>   删除filename指定的文件,需要具有此文件的权限.  

**rename**:重命名文件  
>   int rename ( const char * oldname, const char * newname );
>   将文件或目录的名称由oldname改变为newname.  
>   如果newname指定的文件已经存在,则可能覆盖原文件或失败,这依赖于实现.  
>   如果成功,返回0;  

**tmpfile**:打开一个临时文件  
>   FILE * tmpfile ( void );  
>   使用wb+模式创建一个二进制文件,这个文件的文件名与所有已经存在的文件名不同.  
>   这个临时文件在流关闭或程序停止时自动删除,如果程序异常的关闭,则临时文件是否被删除,依赖于实现.  
>   如果成功,返回一个文件指针,反之,返回NULL.  

**tmpnam**:获取一个临时文件名  
>   char * tmpnam ( char * str );  
>   返回一个与已经存在文件的文件名不同的文件名.  
>   如果参数str为空,则要返回的文件名保存在一个内部的静态数组中,此静态数组中的内容,会在下一次调用此函数时被覆盖.  
>   如果str不为空,则其长度就最少为L_tmpnam,以保存文件名.  
>   如果失败,返回空指针,否则返回str或一个静态数组.  

文件访问:　
**fclose**:关闭文件  
>   int fclose ( FILE * stream );  
>   关闭对应的文件,并且分离.  
>   此流对应的内部缓存会被分离和刷新:还没有写入文件的缓存会被写入,还没有读入的缓存会被丢弃.  
>   即使调用失败,此文件指针,与不会再与文件和其缓存关联.  

**fflush**:刷新流  
>   int fflush ( FILE * stream );  
>   




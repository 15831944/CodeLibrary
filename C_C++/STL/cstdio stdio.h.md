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
>   刷新指定流,如果指定的打开的流在做写入操作,则任何未写入文件的数据都会被写入.  
>   如果参数stream为空,则所有流都会被刷新.  
>   如果是其它情况,则其行为是未定义的.  
>   如果成功返回0.  

**fopen**:打开文件  
>   FILE * fopen ( const char * filename, const char * mode );  
>   文件打开的模式:  
>   * "r":只读模式打开,文件必须存在.  
>   * "w":创建一个新的文件,或打开已存在的文件,此时文件以前的内容会被清除.  
>   * "a":打开一个文件以写入,打开后在文件尾部,改变指针位置的操作会被忽略,如果文件不存在则创建.  
>   * "r+":以更新的方式打开一个文件,文件必须存在.  
>   * "w+":创建一个文件或打开已存在的文件,以更新模式打开,此时文件以前的内容会被清除.  
>   * "a+":以更新的方式打开一个文件,所有的写入操作都会在尾部,即使之前使用了位置操作.  
>   以上都是针对文本文件的,若是二进制文件,则需要添加"b".  

**freopen**:重新打开一个文件,以新的文件名或模式
>   FILE * freopen ( const char * filename, const char * mode, FILE * stream );  
>   如果filename指定了,则先关闭stream参数其对应的文件.  
>   如果filename为空,则以新的模式打开stream对应的文件.  
>   成功后,错误标志都会被重置,就像是调用了clearerr.  

**setbuf**:设置流的缓存  
>   void setbuf ( FILE * stream, char * buffer );  
>   设置流读写的缓存,若buffer不为空,则流变为fully buffered,若buffer为空,则流变为unbuffered.  
>   此函数应该在文件打开之后调用,以任何读写操作之前.  
>   函数假定buffer最小有BUFSIZ字节的大小.  

**setvbuf**:设置流的缓存和打开模式  
>   int setvbuf ( FILE * stream, char * buffer, int mode, size_t size );  
>   设置流的缓存和缓存大小,还有缓存的模式.  
>   此函数应该在文件打开之后调用,以任何读写操作之前.  
>   参数mode值:  
>   * _IOFBF: full buffering  
>   * _IOLBF: line buffering  
>   * _IONBF: no buffering  

格式化输入输出  
**fprintf**:格式化输出数据到流.  
>   int fprintf ( FILE * stream, const char * format, ... );  

**fscanf**:格式化输入.  
>   int fscanf ( FILE * stream, const char * format, ... );  
>   从流中读取格式化数据,并保存在后面的可变参数中.  

**printf**:格式化输出到stdout.  
>   int printf ( const char * format, ... );  

**scanf**:从stdin中格式化输入.  
>   int scanf ( const char * format, ... );  

**sprintf**:格式化输出到字符串.  
>   int sprintf ( char * str, const char * format, ... );  
>   str应该足够大以容纳新构成的内容.  
>   '\0'会自己添加到最后.  

**sscanf**:从字符串中格式化输入.. 
>   int sscanf ( const char * s, const char * format, ...);  

**vfprintf**:格式化输出到文件..
>   int vfprintf ( FILE * stream, const char * format, va_list arg );  
>   类似fprintf,但用参数arg代替可变参数.  
>   在调用前后,arg需要用va_start和va_end初始化和释放.  

**vprintf**:格式化输出到stdout.  
>   int vprintf ( const char * format, va_list arg );  

**vsprintf**:格式化输出到字符串.  
>   int vsprintf (char * s, const char * format, va_list arg );  

字符输入输出  
**fgetc**:从流中获取字符.  
>   int fgetc ( FILE * stream );  
>   返回流中的一个字符,如果遇到end-of-file,则返回EOF,并设置end-of-file标志.  

**fgets**:从流中获取字符串.  
>   char * fgets ( char * str, int num, FILE * stream );  
>   从流中读取n-1个字符并保存在str中,如果遇到newline或end-of-file,则保存已经读取的字符.  
>   '\0'会被自动添加.  
>   如果在读取字符之前遇到了end-of-file,则返回空,str的内容不变.  

**fputc**:将字符写入流中.  
>   int fputc ( int character, FILE * stream );  
>   如果成功,返回已写入的字符,失败,则返回EOF.  

**fputs**:将字符串写入流中.  
>   int fputs ( const char * str, FILE * stream );  
>   将str中的内容输出到流中,直到遇到'\0','\0'不会被输出.  
>   如果成功,一个非负数返回,失败,则返回EOF.  

**getc**:从流中获取字符.  
>   int getc ( FILE * stream );  
>   同fgetc().  

**getchar**:从stdin中获取字符.  
>   int getchar ( void );  

**gets**:从stdin中获取字符串.  
>   char * gets ( char * str );
>   从stdin中获取字符串,直到遇到newline或end-of-file.  
>   newline不会被写入str.  
>   '\0'会被自己添加.  

**putc**:输出字符到流中.  
>   int putc ( int character, FILE * stream );  

**putchar**:输出字符到stdout.  
>   int putchar ( int character );  

**puts**:输出字符串到stdout.  
>   int puts ( const char * str );  

直接输入输出  
**fread**:从流中读取数据块.  
>   size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );  
>   从流中读取count个数据块,每个数据块size大小,并保存在ptr中.  
>   ptr最小size*count大小.  
>   返回总共读取的元素个数.  

**fwrite**:写入数据块到流中.  
>   size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );  
>   写入count个数据块,每个数据块size大小.  
>   返回一共写入的元素个数.  

文件位置操作  
**fgetpos**:获取流的当前位置.  
>   int fgetpos ( FILE * stream, fpos_t * pos );  
>   将文件的当前位置保存在pos中.  
>   若成功返回0.  

**fseek**:设置流的当前位置.  
>   int fseek ( FILE * stream, long int offset, int origin );  
>   origin的取值:  
>   * SEEK_SET:文件开头.  
>   * SEEK_CUR:文件当前位置.  
>   * SEEK_END:文件的结尾.  
>   如果是文本文件,则origin值必须为SEEK_SET.  
>   如果成功,返回0.  

错误处理  
**clearerr**:清除错误标志.  
>   void clearerr ( FILE * stream );  
>   重置所有的错误标志和eof标志.  

**feof**:检查end-of-file标志.  
>   int feof ( FILE * stream );  
>   检查流的end-of-file标志是否已经置上.  
>   如果标志已置上,返回一个非0数,否则,返回0.  

**ferror**:检查error标志.  
>   int ferror ( FILE * stream );  

**perror**:打印错误信息.  
>   void perror ( const char * str );  

宏  
**BUFSIZ**  
**EOF**  
**FILENAME_MAX**  
**FOPEN_MAX**  
**L_tmpnam**  
**NULL**  
**TMP_MAX**  

类型  
**FILE**  
**fpos_t**  
**size_t**  


##参考
*   <http://www.cplusplus.com/reference/cstdio/>
*   <https://zh.wikipedia.org/zh-hk/Stdio.h>

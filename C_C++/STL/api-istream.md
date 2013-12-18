#api-istream

头文件:<istream> <iostream>  
定义:typedef basic_istream<char> istream;  
综述:


##成员函数(public)
*std::istream::operator>>*:  
>   对于输入流而言,它就是一个数据提取操作;  
>   它使用了多种类型进行重载:arithmetic types \stream buffers \manipulators  

*std::istream::gcount*:  
>   streamsize gcount() const;  
>   返回最近一次的unformatted input operations(无格式输入操作)提取的字符数.  
>   unformatted input operations包括:get, getline, ignore, peek, read, readsome, putback 和 unget.  

*std::istream::get*:  
>   [1] int get();  
>   [2] istream& get (char& c);  
>   [3] istream& get (char* s, streamsize n);  
>   [4] istream& get (char* s, streamsize n, char delim);  
>   [5] istream& get (streambuf& sb);  
>   [6] istream& get (streambuf& sb, char delim);  

>   从流中提取字符,并做为无格式的输入;  
>   [1]和[2]提取单个字符;  
>   [3]和[4]提取最多n-1个字符,或者遇到了定界字符(是'\n'或delim),定界符不会被提取,它会保留下来并作为下一次提取的字符;'\0'会自己添加到提取的字符串后面,即使提取的字符串为空.  
>   [5]和[6]从输入流中提取字符串并将其副本输入到sb中,直到遇到内部错误或遇到定界符(是'\n'或delim),
>   这些方法在遇到end-of-file符时也会结束,如果遇到了此字符,会置eofbit标志.  
>   除了[1]会返回提取的字符或EOF,其它方法都会返回*this.  

*std::istream::getline*:  
>   [1] istream& getline (char* s, streamsize n );  
>   [2] istream& getline (char* s, streamsize n, char delim );
>   方法从输入流中提取字符串,并做为无格式的输入保存在s中,直到遇到定界符('\n'或delin),或者已经有n个字符写入到s中(包括'\0').  
>   定界符会被提取,但不写入到s中,直接丢弃.  
>   这些方法在遇到end-of-file符时也会结束,如果遇到了此字符,会置eofbit标志.  
>   '\0'会自己添加到提取的字符串后面,即使提取的字符串为空.  
>   在如下情况,failbit标志会被置上:(1)没有提取到字符;(2)在已经有n-1个字符被写入s中时,定界符还没有被遇上;  

*std::istream::ignore*:  
>   istream& ignore (streamsize n = 1, int delim = EOF);  
>   提取字符并且将它们丢弃,直到有n个字符被提取,或者遇到定界符;  
>   这些方法在遇到end-of-file符时也会结束,如果遇到了此字符,会置eofbit标志.  

*std::istream::peek*:  
>   int peek();  
>   获取流中的下一个字符,但是提取出来.  
>   如果内部的任何标志已经被置上,则其返回EOF.  

*std::istream::read*:  
>   istream& read (char* s, streamsize n);  
>   从流中提取n个字符并将其保存在s中;  
>   此方法只是简单的复制数据,而不去检查字符.  
>   在提取n个字符之前,流结束了,如遇到end-of-file标志,会将已经提取的字符保存,并置上eofbit和failbit标志.  

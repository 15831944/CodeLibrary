#api-stringstream

头文件:<sstream>
定义:typedef basic_stringstream<char> stringstream;
综述:
    stringstream是一个输入/输出字符串数据流；继承自iostream； 
    stringstream对象使用一个string buffer来保存字符序列,这个序列可以通过str()方法直接访问,此方法返回一个string对象； 
    
##成员
    除了string buffer,此类的对象还有继承自ios_base,ios和istream的成员;  
    > flags,setf,unsetf 
    > width
    > precision
    > getloc,imbue
    > fill
    > rdstate,setstate,clear
    > exceptions
    > register_callback
    > iword,pword,xalloc
    > tie
    > rdbuf
    > gcount

##成员类型
    > char_type   char
    > traits_type char_traits<char>
    > allocator_type  allocator<char>
    > int_type    int
    > pos_type    streampos
    > off_type    streamoff

##成员函数(public)
>    _(constructor)_:
    > [1] explicit stringstream (ios_base::openmode which = ios_base::in | ios_base::out);  (C++98/C++11)  
    > [2] explicit stringstream (const string& str, ios_base::openmode which = ios_base::in | ios_base::out);  (C++98/C++11)  
>    **str**:  
    > [1] string str() const;  
    > [2] void str (const string& s);  
    > 函数[1]返回sstream对象中当前内容的副本;  
    > 函数[2]将参数s设置为当前对象中的内容,抛弃之前所有的内容;  
    > 实际上,此文件是调用内部的string buffer的str()函数;
    
##其它
    stringstream不是线程安全的,并发的访问会引发数据竞争.  
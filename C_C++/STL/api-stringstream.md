#api-stringstream

ͷ�ļ�:<sstream>  
����:typedef basic_stringstream<char> stringstream;  
����:
stringstream��һ������/����ַ����������̳���iostream��  
stringstream����ʹ��һ��string buffer�������ַ�����,������п���ͨ��str()����ֱ�ӷ���,�˷�������һ��string����  
    
##��Ա
����string buffer,����Ķ����м̳���ios_base,ios��istream�ĳ�Ա;  
>   flags,setf,unsetf 
>   width
>   precision
>   getloc,imbue
>   fill
>   rdstate,setstate,clear
>   exceptions
>   register_callback
>   iword,pword,xalloc
>   tie
>   rdbuf
>   gcount

##��Ա����
>   char_type   char
>   traits_type char_traits<char>
>   allocator_type  allocator<char>
>   int_type    int
>   pos_type    streampos
>   off_type    streamoff

##��Ա����(public)
(*constructor*):  
>   [1] explicit stringstream (ios_base::openmode which = ios_base::in | ios_base::out);  (C++98/C++11)  
>   [2] explicit stringstream (const string& str, ios_base::openmode which = ios_base::in | ios_base::out);  (C++98/C++11)  

*std::stringstream::str*:  
>   [1] string str() const;  
>   [2] void str (const string& s);  
>   ����[1]����sstream�����е�ǰ���ݵĸ���;  
>   ����[2]������s����Ϊ��ǰ�����е�����,����֮ǰ���е�����;  
>   ʵ����,���ļ��ǵ����ڲ���string buffer��str()����;
    
*std::istream::operator>>*:  
*std::istream::gcount*:  
*std::istream::get*:  
*std::istream::getline*:  
*std::istream::ignore*:  
*std::istream::peek*:  
*std::istream::read*:  

##����
    stringstream�����̰߳�ȫ��,�����ķ��ʻ��������ݾ���.  
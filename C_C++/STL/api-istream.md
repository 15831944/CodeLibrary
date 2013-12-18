#api-istream

ͷ�ļ�:<istream> <iostream>  
����:typedef basic_istream<char> istream;  
����:


##��Ա����(public)
*std::istream::operator>>*:  
>   ��������������,������һ��������ȡ����;  
>   ��ʹ���˶������ͽ�������:arithmetic types \stream buffers \manipulators  

*std::istream::gcount*:  
>   streamsize gcount() const;  
>   �������һ�ε�unformatted input operations(�޸�ʽ�������)��ȡ���ַ���.  
>   unformatted input operations����:get, getline, ignore, peek, read, readsome, putback �� unget.  

*std::istream::get*:  
>   [1] int get();  
>   [2] istream& get (char& c);  
>   [3] istream& get (char* s, streamsize n);  
>   [4] istream& get (char* s, streamsize n, char delim);  
>   [5] istream& get (streambuf& sb);  
>   [6] istream& get (streambuf& sb, char delim);  

>   ��������ȡ�ַ�,����Ϊ�޸�ʽ������;  
>   [1]��[2]��ȡ�����ַ�;  
>   [3]��[4]��ȡ���n-1���ַ�,���������˶����ַ�(��'\n'��delim),��������ᱻ��ȡ,���ᱣ����������Ϊ��һ����ȡ���ַ�;'\0'���Լ���ӵ���ȡ���ַ�������,��ʹ��ȡ���ַ���Ϊ��.  
>   [5]��[6]������������ȡ�ַ��������丱�����뵽sb��,ֱ�������ڲ���������������(��'\n'��delim),
>   ��Щ����������end-of-file��ʱҲ�����,��������˴��ַ�,����eofbit��־.  
>   ����[1]�᷵����ȡ���ַ���EOF,�����������᷵��*this.  

*std::istream::getline*:  
>   [1] istream& getline (char* s, streamsize n );  
>   [2] istream& getline (char* s, streamsize n, char delim );
>   ����������������ȡ�ַ���,����Ϊ�޸�ʽ�����뱣����s��,ֱ�����������('\n'��delin),�����Ѿ���n���ַ�д�뵽s��(����'\0').  
>   ������ᱻ��ȡ,����д�뵽s��,ֱ�Ӷ���.  
>   ��Щ����������end-of-file��ʱҲ�����,��������˴��ַ�,����eofbit��־.  
>   '\0'���Լ���ӵ���ȡ���ַ�������,��ʹ��ȡ���ַ���Ϊ��.  
>   ���������,failbit��־�ᱻ����:(1)û����ȡ���ַ�;(2)���Ѿ���n-1���ַ���д��s��ʱ,�������û�б�����;  

*std::istream::ignore*:  
>   istream& ignore (streamsize n = 1, int delim = EOF);  
>   ��ȡ�ַ����ҽ����Ƕ���,ֱ����n���ַ�����ȡ,�������������;  
>   ��Щ����������end-of-file��ʱҲ�����,��������˴��ַ�,����eofbit��־.  

*std::istream::peek*:  
>   int peek();  
>   ��ȡ���е���һ���ַ�,������ȡ����.  
>   ����ڲ����κα�־�Ѿ�������,���䷵��EOF.  

*std::istream::read*:  
>   istream& read (char* s, streamsize n);  
>   ��������ȡn���ַ������䱣����s��;  
>   �˷���ֻ�Ǽ򵥵ĸ�������,����ȥ����ַ�.  
>   ����ȡn���ַ�֮ǰ,��������,������end-of-file��־,�Ὣ�Ѿ���ȡ���ַ�����,������eofbit��failbit��־.  

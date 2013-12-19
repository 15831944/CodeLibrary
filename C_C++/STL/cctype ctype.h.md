#<cctype\> (ctype.h)

##概述
此头文件定义了一些为字符分类和转换的函数.  

##成员
*isalnum*:  
>   int isalnum ( int c );  
>   是否为英文字母或数字字符.  
>   是否为字母依赖于locale的设置.  

*isalpha*:  
>   int isalpha ( int c );  
>   是否为英文字母字符.  

*iscntrl*:  
>   int iscntrl ( int c );  
>   是否为控制字符.  

*isdigit*:  
>   int isdigit ( int c );  
>   是否为数字字符.  

*isgraph*:  
>   int isgraph ( int c );  
>   是否为可显示字符.  

*islower*:  
>   int islower ( int c );  
>   是否为小写字母字符.  

*isprint*:  
>   int isprint ( int c );  
>   是否为可打印字符.  

*ispunct*:  
>   int ispunct ( int c );  
>   是否为标点符号字符.  

*isspace*:  
>   int isspace ( int c );  
>   是否为空白字符.  

*isupper*:  
>   int isupper ( int c );  
>   是否为大写字母字符.  

*isxdigit*:  
>   int isxdigit ( int c );  
>   是否为十六进制字符.  

*tolower*:  
>   int tolower ( int c );  
>   转换为小写字符.  

*toupper*:  
>   int toupper ( int c );  
>   转换为大写字符.  

##ASCII字符集的情况
<table border="1"><tbody>
<tr><th>ASCII values</th><th>characters</th><th>iscntrl</th><th>isblank</th><th>isspace</th><th>isupper</th><th>islower</th><th>isalpha</th><th>isdigit</th><th>isxdigit</th><th>isalnum</th><th>ispunct</th><th>isgraph</th><th>isprint</th></tr>
<tr><td>0x00 .. 0x08</td><td>NUL, (other control codes)</td>
<td>x</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
<tr><td>0x09</td><td>tab (<tt>'\t'</tt>)</td>
<td>x</td><td>x</td><td>x</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
<tr><td>0x0A .. 0x0D</td><td>(white-space control codes: <tt>'\f'</tt>,<tt>'\v'</tt>,<tt>'\n'</tt>,<tt>'\r'</tt>)</td>
<td>x</td><td> </td><td>x</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
<tr><td>0x0E .. 0x1F</td><td>(other control codes)</td>
<td>x</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
<tr><td>0x20</td><td>space (<tt>' '</tt>)</td>
<td> </td><td>x</td><td>x</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td>x</td></tr>
<tr><td>0x21 .. 0x2F</td><td><tt>!"#$%&amp;'()*+,-./</tt></td>
<td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td>x</td></tr>
<tr><td>0x30 .. 0x39</td><td><tt>0123456789</tt></td>
<td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td>x</td><td> </td><td>x</td><td>x</td></tr>
<tr><td>0x3a .. 0x40</td><td><tt>:;&lt;=&gt;?@</tt></td>
<td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td>x</td></tr>
<tr><td>0x41 .. 0x46</td><td><tt>ABCDEF</tt></td>
<td> </td><td> </td><td> </td><td>x</td><td> </td><td>x</td><td> </td><td>x</td><td>x</td><td> </td><td>x</td><td>x</td></tr>
<tr><td>0x47 .. 0x5A</td><td><tt>GHIJKLMNOPQRSTUVWXYZ</tt></td>
<td> </td><td> </td><td> </td><td>x</td><td> </td><td>x</td><td> </td><td> </td><td>x</td><td> </td><td>x</td><td>x</td></tr>
<tr><td>0x5B .. 0x60</td><td><tt>[\]^_`</tt></td>
<td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td>x</td></tr>
<tr><td>0x61 .. 0x66</td><td><tt>abcdef</tt></td>
<td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td> </td><td>x</td><td>x</td><td> </td><td>x</td><td>x</td></tr>
<tr><td>0x67 .. 0x7A</td><td><tt>ghijklmnopqrstuvwxyz</tt></td>
<td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td> </td><td> </td><td>x</td><td> </td><td>x</td><td>x</td></tr>
<tr><td>0x7B .. 0x7E</td><td><tt>{|}~</tt></td>
<td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td>x</td><td>x</td><td>x</td></tr>
<tr><td>0x7F</td><td>(DEL)</td>
<td>x</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
</tbody></table>
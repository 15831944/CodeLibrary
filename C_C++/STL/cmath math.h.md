#<cmath\> (math.h)

##概述
math.h是C标准函数库中的头文件.提供用于常用高级数学运算的运算函数.  

##成员
###三角函数类  
**cos**:余弦函数  
    double cos (double x);  (C90/C99/C++98/C++11);  
    float cosf (float x);  (C99);  
    long double cosl (long double x);  (C99)  
    float cos (float x);  (C++98)  
    long double cos (long double x);  (C++98)  
    其中x为角度的弧度值,一弧度为 180/PI .  

**sin**:正弦函数  
    double sin(double x);  

**tan**:正切函数  
    double tan (double x);  

**acos**:反余弦函数  
    double acos (double x);  
    其中x为余弦值,返回对应角度的弧度值.  

**acos**:反余弦函数  
    double acos (double x);  

**asin**:反正弦函数  
    double asin (double x);  

**atan**:反正切(主值)函数  
    double atan (double x);  

**atan2**:反正切(主值)函数  
    double atan2(double y, double x);  

**cosh**:双曲线余弦函数  
    double cosh (double x);  

**sinh**:双曲线正弦函数  
    double sinh (double x);  

**tanh**:双曲线正切函数  
    double tanh  (double x);  

###指数对数类
**exp**:指数函数  
    double exp (double x);  
    返回以e为底数,x为指数的值: e^x;   

**log**:自然对数函数  
    double log (double x);  
    返回以e为底的对数;  

**log10**:常用对数函数  
    double log10 (double x);  
    返回以10为底的对数;  

**pow**:  
    double pow (double base, double exponent);  
    返回指数base^exponent的值;  

**sqrt**:  
    double sqrt (double x);  
    返回x的平方根.  
    如果x为负数,则会产生domain error.  

###取整与取余数
**modf**:  
    double modf (double x, double* intpart);  
    将x的整数部分通过指针intpart返回,小数部分通过函数返回值返回.  

**fmod**:  
    double fmod (double numer, double denom);  
    返回两参数相除(numer/denom)的余数.  

###取整函数
**ceil**:向上取整  
    double ceil (double x);  
    返回不小于x的最小整数;  

**floor**:向下取整  
    double floor (double x);  
    返回不大于x的最大整数;  

###绝对值
**abs**:
    double abs (double x);

**fabs**:
    double fabs (double x);

##参考
*   <http://www.cplusplus.com/reference/cmath/>
*   <http://zh.wikipedia.org/zh-cn/Math.h>
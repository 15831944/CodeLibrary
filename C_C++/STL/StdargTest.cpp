#include <stdarg.h>
#include <iostream>

void getNum(int num, ...)
{
    std::cout << "num: " << num << std::endl;
    va_list ap;
    va_start(ap, num);
    for(; num--;){
        int n = va_arg(ap, int);
        std::cout << n << std::endl;
    }
    va_end(ap);
}

int main()
{
    getNum(2, 1, 2);
    getNum(1, 1, 1);
    getNum(3, 1, 1);
    return 0;

}
#include <csignal>
#include <iostream>
#include <windows.h>
#include <process.h>

void my_handler(int i)
{
    Sleep(100); 
    std::cout << "my handle." << GetCurrentThreadId() << std::endl;
}

unsigned int __stdcall ThreadFun(void *pPM)
{
    std::cout << "raise start." << GetCurrentThreadId() << std::endl;
    int r = raise(SIGINT);
    std::cout << "raise end." << r << std::endl;

    return 0;
}

int main()
{

    void (*prev_handler)(int);
    prev_handler = signal (SIGINT, my_handler);
    
    std::cout << "_beginthreadex start." << GetCurrentThreadId() << std::endl;
    _beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
    std::cout << "_beginthreadex end." << std::endl;

    Sleep(1000); 
    std::cout << "main end" << std::endl;
    return 0;
}
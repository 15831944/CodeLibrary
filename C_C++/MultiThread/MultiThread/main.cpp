#include <Windows.h>
#include <process.h>
#include <iostream>

using namespace std;

volatile long g_nLoginCount; //登录次数
unsigned int __stdcall Fun(void *pPM); //线程函数
const int THREAD_NUM = 50; //启动线程数

CRITICAL_SECTION CrtThreadDone;

unsigned int __stdcall ThreadFun(void *pPM)
{
    //InterlockedIncrement((LPLONG)&g_nLoginCount);  
    int i = 10;
    while(i--){
        Sleep(10); 

        EnterCriticalSection(&CrtThreadDone);
        g_nLoginCount++;
        LeaveCriticalSection(&CrtThreadDone);
    }

    
    return 0;
}

int main()
{
    InitializeCriticalSection(&CrtThreadDone);

    int num= 20;
    while (num--)
    {    
        g_nLoginCount = 0;
        int i;
        HANDLE  handle[THREAD_NUM];
        for (i = 0; i < THREAD_NUM; i++)
            handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);

        WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
        printf("有%d个用户登录后记录结果是%d\n", THREAD_NUM, g_nLoginCount);
    }

    int c;
    cin >> c;
    return 0;
}
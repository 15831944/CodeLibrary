#include <Windows.h>
#include <process.h>
#include <iostream>
#include "ProductConsum.h"

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

int TestThreadFun()
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
    return 0;
}


long g_nNum; //全局资源
unsigned int __stdcall Fun(void *pPM); //线程函数
const int THREAD_NUM_FUN = 10; //子线程个数
CRITICAL_SECTION  g_csThreadParameter, g_csThreadCode;  
HANDLE  g_hThreadEvent;  
int TestFun()
{
    InitializeCriticalSectionAndSpinCount(&g_csThreadParameter, 1000);  
    InitializeCriticalSectionAndSpinCount(&g_csThreadCode, 1000);  
    g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);   

    g_nNum = 0;
    HANDLE  handle[THREAD_NUM];
    
    int i = 0;
    while (i < THREAD_NUM_FUN) 
    {
        EnterCriticalSection(&g_csThreadParameter);//进入子线程序号关键区域  
        handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
        WaitForSingleObject(g_hThreadEvent, INFINITE); //等待事件被触发  
        i++;//等子线程接收到参数时主线程可能改变了这个i的值
    }
    //保证子线程已全部运行结束
    WaitForMultipleObjects(THREAD_NUM_FUN, handle, TRUE, INFINITE);  

    DeleteCriticalSection(&g_csThreadCode);  
    DeleteCriticalSection(&g_csThreadParameter);  
    CloseHandle(g_hThreadEvent);  
    return 0;
}

unsigned int __stdcall Fun(void *pPM)
{
//由于创建线程是要一定的开销的，所以新线程并不能第一时间执行到这来
    Sleep(10);
    int nThreadNum = *(int *)pPM; //子线程获取参数
    LeaveCriticalSection(&g_csThreadParameter);//离开子线程序号关键区域  
    SetEvent(g_hThreadEvent); //触发事件  
    Sleep(50);//some work should to do

    EnterCriticalSection(&g_csThreadCode);//进入各子线程互斥区域  
    g_nNum++;  //处理全局资源
    Sleep(10);//some work should to do
    printf("线程编号为%d  全局资源值为%d\n", nThreadNum, g_nNum);
    LeaveCriticalSection(&g_csThreadCode);//离开各子线程互斥区域  

    return 0;
}


HANDLE  g_hThreadEventFS;
//快线程
unsigned int __stdcall FastThreadFun(void *pPM)
{
    Sleep(10); //用这个来保证各线程调用等待函数的次序有一定的随机性
    printf("%s 启动\n", (PSTR)pPM);
    WaitForSingleObject(g_hThreadEventFS, INFINITE);
    printf("%s 等到事件被触发 顺利结束\n", (PSTR)pPM);
    return 0;
}
//慢线程
unsigned int __stdcall SlowThreadFun(void *pPM)
{
    Sleep(100);
    printf("%s 启动\n", (PSTR)pPM);
    WaitForSingleObject(g_hThreadEventFS, INFINITE);
    printf("%s 等到事件被触发 顺利结束\n", (PSTR)pPM);
    return 0;
}
int TestFastSlow()
{
    printf("  使用PluseEvent()函数\n");
    printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

    BOOL bManualReset = TRUE;
    //创建事件 第二个参数手动置位TRUE，自动置位FALSE
    g_hThreadEventFS = CreateEvent(NULL, bManualReset, FALSE, NULL);
    if (bManualReset == TRUE)
        printf("当前使用手动置位事件\n");
    else
        printf("当前使用自动置位事件\n");

    char szFastThreadName[5][30] = {"快线程1000", "快线程1001", "快线程1002", "快线程1003", "快线程1004"};
    char szSlowThreadName[2][30] = {"慢线程196", "慢线程197"};

    int i;
    for (i = 0; i < 5; i++)
        _beginthreadex(NULL, 0, FastThreadFun, szFastThreadName[i], 0, NULL);
    for (i = 0; i < 2; i++)
        _beginthreadex(NULL, 0, SlowThreadFun, szSlowThreadName[i], 0, NULL);
    
    Sleep(50); //保证快线程已经全部启动
    printf("现在主线程触发一个事件脉冲 - PulseEvent()\n");
    PulseEvent(g_hThreadEventFS);//调用PulseEvent()就相当于同时调用下面二句
    //SetEvent(g_hThreadEvent);
    //ResetEvent(g_hThreadEvent);
    
    Sleep(3000); 
    printf("时间到，主线程结束运行\n");
    CloseHandle(g_hThreadEventFS);
    return 0;
}


long g_nNum_Mutex;
unsigned int __stdcall Fun(void *pPM);
const int THREAD_NUM_MUTEX = 10;
//互斥量与关键段
HANDLE  g_hThreadParameter;
CRITICAL_SECTION g_csThreadCodeMutex;


unsigned int __stdcall FunMutex(void *pPM)
{
    int nThreadNum = *(int *)pPM;
    ReleaseMutex(g_hThreadParameter);//触发互斥量
    
    Sleep(50);//some work should to do

    EnterCriticalSection(&g_csThreadCodeMutex);
    g_nNum_Mutex++;
    Sleep(0);//some work should to do
    printf("线程编号为%d  全局资源值为%d\n", nThreadNum, g_nNum_Mutex);
    LeaveCriticalSection(&g_csThreadCodeMutex);
    return 0;
}

int TestFunMutex()
{
    printf("     经典线程同步 互斥量Mutex\n");
    printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");
    
    //初始化互斥量与关键段 第二个参数为TRUE表示互斥量为创建线程所有
    g_hThreadParameter = CreateMutex(NULL, FALSE, NULL);
    InitializeCriticalSection(&g_csThreadCodeMutex);

    HANDLE  handle[THREAD_NUM_MUTEX];    
    g_nNum_Mutex = 0;    
    int i = 0;
    while (i < THREAD_NUM_MUTEX) 
    {
        handle[i] = (HANDLE)_beginthreadex(NULL, 0, FunMutex, &i, 0, NULL);
        WaitForSingleObject(g_hThreadParameter, INFINITE); //等待互斥量被触发
        i++;
    }
    WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
    
    //销毁互斥量和关键段
    CloseHandle(g_hThreadParameter);
    DeleteCriticalSection(&g_csThreadCode);
    for (i = 0; i < THREAD_NUM_MUTEX; i++)
        CloseHandle(handle[i]);
    return 0;
}


long g_nNumSemaphore;
const int THREAD_NUM_Semaphore = 10;
//信号量与关键段
HANDLE            g_hThreadParameterSemaphore;
CRITICAL_SECTION  g_csThreadCodeSemaphore;

unsigned int __stdcall FunSemaphore(void *pPM)
{
	int nThreadNum = *(int *)pPM;
	ReleaseSemaphore(g_hThreadParameterSemaphore, 1, NULL);//信号量++

	Sleep(50);//some work should to do

	EnterCriticalSection(&g_csThreadCodeSemaphore);
	++g_nNum;
	Sleep(0);//some work should to do
	printf("线程编号为%d  全局资源值为%d\n", nThreadNum, g_nNum);
	LeaveCriticalSection(&g_csThreadCodeSemaphore);
	return 0;
}

int TestFunSemaphore()
{
	printf("     经典线程同步 信号量Semaphore\n");
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

	//初始化信号量和关键段
	g_hThreadParameterSemaphore = CreateSemaphore(NULL, 0, 1, NULL);//当前0个资源，最大允许1个同时访问
	InitializeCriticalSection(&g_csThreadCodeSemaphore);

	HANDLE  handle[THREAD_NUM_Semaphore];	
	g_nNum = 0;
	int i = 0;
	while (i < THREAD_NUM_Semaphore) 
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, FunSemaphore, &i, 0, NULL);
		WaitForSingleObject(g_hThreadParameterSemaphore, INFINITE);//等待信号量>0
		++i;
	}
	WaitForMultipleObjects(THREAD_NUM_Semaphore, handle, TRUE, INFINITE);
	
	//销毁信号量和关键段
	DeleteCriticalSection(&g_csThreadCodeSemaphore);
	CloseHandle(g_hThreadParameterSemaphore);
	for (i = 0; i < THREAD_NUM_Semaphore; i++)
		CloseHandle(handle[i]);
	return 0;
}


int main()
{
    //TestThreadFun();
    //TestFun();
    //TestFastSlow();
    //TestFunMutex();
    //TestFunSemaphore();
    ProductConsum();

    int c;
    cin >> c;
    return 0;
}
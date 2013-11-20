#include <Windows.h>
#include <process.h>
#include <iostream>
#include "ProductConsum.h"

using namespace std;

volatile long g_nLoginCount; //��¼����
unsigned int __stdcall Fun(void *pPM); //�̺߳���
const int THREAD_NUM = 50; //�����߳���

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
        printf("��%d���û���¼���¼�����%d\n", THREAD_NUM, g_nLoginCount);
    }
    return 0;
}


long g_nNum; //ȫ����Դ
unsigned int __stdcall Fun(void *pPM); //�̺߳���
const int THREAD_NUM_FUN = 10; //���̸߳���
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
        EnterCriticalSection(&g_csThreadParameter);//�������߳���Źؼ�����  
        handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
        WaitForSingleObject(g_hThreadEvent, INFINITE); //�ȴ��¼�������  
        i++;//�����߳̽��յ�����ʱ���߳̿��ܸı������i��ֵ
    }
    //��֤���߳���ȫ�����н���
    WaitForMultipleObjects(THREAD_NUM_FUN, handle, TRUE, INFINITE);  

    DeleteCriticalSection(&g_csThreadCode);  
    DeleteCriticalSection(&g_csThreadParameter);  
    CloseHandle(g_hThreadEvent);  
    return 0;
}

unsigned int __stdcall Fun(void *pPM)
{
//���ڴ����߳���Ҫһ���Ŀ����ģ��������̲߳����ܵ�һʱ��ִ�е�����
    Sleep(10);
    int nThreadNum = *(int *)pPM; //���̻߳�ȡ����
    LeaveCriticalSection(&g_csThreadParameter);//�뿪���߳���Źؼ�����  
    SetEvent(g_hThreadEvent); //�����¼�  
    Sleep(50);//some work should to do

    EnterCriticalSection(&g_csThreadCode);//��������̻߳�������  
    g_nNum++;  //����ȫ����Դ
    Sleep(10);//some work should to do
    printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);
    LeaveCriticalSection(&g_csThreadCode);//�뿪�����̻߳�������  

    return 0;
}


HANDLE  g_hThreadEventFS;
//���߳�
unsigned int __stdcall FastThreadFun(void *pPM)
{
    Sleep(10); //���������֤���̵߳��õȴ������Ĵ�����һ���������
    printf("%s ����\n", (PSTR)pPM);
    WaitForSingleObject(g_hThreadEventFS, INFINITE);
    printf("%s �ȵ��¼������� ˳������\n", (PSTR)pPM);
    return 0;
}
//���߳�
unsigned int __stdcall SlowThreadFun(void *pPM)
{
    Sleep(100);
    printf("%s ����\n", (PSTR)pPM);
    WaitForSingleObject(g_hThreadEventFS, INFINITE);
    printf("%s �ȵ��¼������� ˳������\n", (PSTR)pPM);
    return 0;
}
int TestFastSlow()
{
    printf("  ʹ��PluseEvent()����\n");
    printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

    BOOL bManualReset = TRUE;
    //�����¼� �ڶ��������ֶ���λTRUE���Զ���λFALSE
    g_hThreadEventFS = CreateEvent(NULL, bManualReset, FALSE, NULL);
    if (bManualReset == TRUE)
        printf("��ǰʹ���ֶ���λ�¼�\n");
    else
        printf("��ǰʹ���Զ���λ�¼�\n");

    char szFastThreadName[5][30] = {"���߳�1000", "���߳�1001", "���߳�1002", "���߳�1003", "���߳�1004"};
    char szSlowThreadName[2][30] = {"���߳�196", "���߳�197"};

    int i;
    for (i = 0; i < 5; i++)
        _beginthreadex(NULL, 0, FastThreadFun, szFastThreadName[i], 0, NULL);
    for (i = 0; i < 2; i++)
        _beginthreadex(NULL, 0, SlowThreadFun, szSlowThreadName[i], 0, NULL);
    
    Sleep(50); //��֤���߳��Ѿ�ȫ������
    printf("�������̴߳���һ���¼����� - PulseEvent()\n");
    PulseEvent(g_hThreadEventFS);//����PulseEvent()���൱��ͬʱ�����������
    //SetEvent(g_hThreadEvent);
    //ResetEvent(g_hThreadEvent);
    
    Sleep(3000); 
    printf("ʱ�䵽�����߳̽�������\n");
    CloseHandle(g_hThreadEventFS);
    return 0;
}


long g_nNum_Mutex;
unsigned int __stdcall Fun(void *pPM);
const int THREAD_NUM_MUTEX = 10;
//��������ؼ���
HANDLE  g_hThreadParameter;
CRITICAL_SECTION g_csThreadCodeMutex;


unsigned int __stdcall FunMutex(void *pPM)
{
    int nThreadNum = *(int *)pPM;
    ReleaseMutex(g_hThreadParameter);//����������
    
    Sleep(50);//some work should to do

    EnterCriticalSection(&g_csThreadCodeMutex);
    g_nNum_Mutex++;
    Sleep(0);//some work should to do
    printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum_Mutex);
    LeaveCriticalSection(&g_csThreadCodeMutex);
    return 0;
}

int TestFunMutex()
{
    printf("     �����߳�ͬ�� ������Mutex\n");
    printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");
    
    //��ʼ����������ؼ��� �ڶ�������ΪTRUE��ʾ������Ϊ�����߳�����
    g_hThreadParameter = CreateMutex(NULL, FALSE, NULL);
    InitializeCriticalSection(&g_csThreadCodeMutex);

    HANDLE  handle[THREAD_NUM_MUTEX];    
    g_nNum_Mutex = 0;    
    int i = 0;
    while (i < THREAD_NUM_MUTEX) 
    {
        handle[i] = (HANDLE)_beginthreadex(NULL, 0, FunMutex, &i, 0, NULL);
        WaitForSingleObject(g_hThreadParameter, INFINITE); //�ȴ�������������
        i++;
    }
    WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
    
    //���ٻ������͹ؼ���
    CloseHandle(g_hThreadParameter);
    DeleteCriticalSection(&g_csThreadCode);
    for (i = 0; i < THREAD_NUM_MUTEX; i++)
        CloseHandle(handle[i]);
    return 0;
}


long g_nNumSemaphore;
const int THREAD_NUM_Semaphore = 10;
//�ź�����ؼ���
HANDLE            g_hThreadParameterSemaphore;
CRITICAL_SECTION  g_csThreadCodeSemaphore;

unsigned int __stdcall FunSemaphore(void *pPM)
{
	int nThreadNum = *(int *)pPM;
	ReleaseSemaphore(g_hThreadParameterSemaphore, 1, NULL);//�ź���++

	Sleep(50);//some work should to do

	EnterCriticalSection(&g_csThreadCodeSemaphore);
	++g_nNum;
	Sleep(0);//some work should to do
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);
	LeaveCriticalSection(&g_csThreadCodeSemaphore);
	return 0;
}

int TestFunSemaphore()
{
	printf("     �����߳�ͬ�� �ź���Semaphore\n");
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

	//��ʼ���ź����͹ؼ���
	g_hThreadParameterSemaphore = CreateSemaphore(NULL, 0, 1, NULL);//��ǰ0����Դ���������1��ͬʱ����
	InitializeCriticalSection(&g_csThreadCodeSemaphore);

	HANDLE  handle[THREAD_NUM_Semaphore];	
	g_nNum = 0;
	int i = 0;
	while (i < THREAD_NUM_Semaphore) 
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, FunSemaphore, &i, 0, NULL);
		WaitForSingleObject(g_hThreadParameterSemaphore, INFINITE);//�ȴ��ź���>0
		++i;
	}
	WaitForMultipleObjects(THREAD_NUM_Semaphore, handle, TRUE, INFINITE);
	
	//�����ź����͹ؼ���
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
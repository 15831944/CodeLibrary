#include <iostream>
#include <process.h>
#include "typedef.h"
#include "tester.h"
#include "timer.h"

using namespace std;

const int ThreadCount = 50;
const int TestActionLoopCount = 10000;

double TimerLock[ThreadCount];
double TimerNoLock[ThreadCount];

struct ThreadParam{
    LockTester* tester;
    int index;
};

unsigned int __stdcall DoAction(void *pPM)
{
    double timeBeg = GetCurrentTimerFlag();
    ThreadParam* param = static_cast<ThreadParam*>(pPM);
    int index = param->index;
    LockTester* tester = param->tester;
    for(int idx = 0; idx != TestActionLoopCount; ++idx){
        tester->DoAction(index);
    }
    double timeEnd = GetCurrentTimerFlag();
    TimerLock[index] = timeEnd - timeBeg;
    return 0;
}

unsigned int __stdcall DoAction_NoLock(void *pPM)
{
    double timeBeg = GetCurrentTimerFlag();
    ThreadParam* param = static_cast<ThreadParam*>(pPM);
    int index = param->index;
    LockTester* tester = param->tester;
    for(int idx = 0; idx != TestActionLoopCount; ++idx){
        tester->Lock();
        tester->DoAction_NoLock(index);
        tester->Unlock();
    }
    double timeEnd = GetCurrentTimerFlag();
    TimerNoLock[index] = timeEnd - timeBeg;
    return 0;
}

int main()
{
    InitTimer();
    LockTester tester;
    tester.Init(ThreadCount);
    HANDLE handle[ThreadCount];

    cout << "begin create thread" << endl;
    double timeBeg = GetCurrentTimerFlag();
    for(int idx = 0; idx != ThreadCount; ++idx){
        ThreadParam* parm = new ThreadParam();
        parm->index = idx;
        parm->tester = &tester;
        handle[idx] = (HANDLE)_beginthreadex(NULL, 0, DoAction, parm, 0, NULL);
    }
    WaitForMultipleObjects(ThreadCount, handle, TRUE, INFINITE);
    double timeEnd = GetCurrentTimerFlag();
    cout.precision(10);
    cout << "wati for thread. timeBeg=" << timeBeg << ",timeEnd=" << timeEnd << ",diff=" << (timeEnd - timeBeg) << endl;
    
    double all = 0;
    double pre = 0;
    for(int idx = 0; idx != ThreadCount; ++idx){
        CloseHandle(handle[idx]);
        all += TimerLock[idx];
    }
    pre = all/(ThreadCount * TestActionLoopCount);
    cout << "lock, all=" << all << ",pre=" << pre << endl;

    cout << "begin create thread no lock" << endl;
    timeBeg = GetCurrentTimerFlag();
    for(int idx = 0; idx != ThreadCount; ++idx){
        ThreadParam* parm = new ThreadParam();
        parm->index = idx;
        parm->tester = &tester;
        handle[idx] = (HANDLE)_beginthreadex(NULL, 0, DoAction_NoLock, parm, 0, NULL);
    }
    WaitForMultipleObjects(ThreadCount, handle, TRUE, INFINITE);
    timeEnd = GetCurrentTimerFlag();
    cout << "wati for thread. timeBeg=" << timeBeg << ",timeEnd=" << timeEnd << ",diff=" << (timeEnd - timeBeg) << endl;
    
    double allNo = 0;
    double preNo = 0;
    for(int idx = 0; idx != ThreadCount; ++idx){
        CloseHandle(handle[idx]);
        allNo += TimerNoLock[idx];
    }
    preNo = allNo/(ThreadCount * TestActionLoopCount);
    cout << "no lock, allNo=" << allNo << ",preNo=" << preNo << endl;
    cout << "no lock : lock = " << allNo / all << endl;

    char c;
    cout << "exit." << endl;
    cin >> c;

    return 1;
}
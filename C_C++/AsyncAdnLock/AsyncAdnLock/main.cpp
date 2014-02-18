#include <iostream>
#include <process.h>
#include "typedef.h"
#include "tester.h"
#include "timer.h"

using namespace std;

const int TestActionLoopCount = 10000;

unsigned int __stdcall DoAction(void *pPM)
{
    double timeBeg = GetCurrentTimerFlag();
    LockItem* item = static_cast<LockItem*>(pPM);
    for(int idx = 0; idx != TestActionLoopCount; ++idx){
        item->Lock();
        item->DoAction();
        item->Unlock();
    }
    double timeEnd = GetCurrentTimerFlag();
    cout << "count: " << item->GetCount() << ",diff=" << (timeEnd - timeBeg) 
            << ",pre=" << (timeEnd - timeBeg) / TestActionLoopCount << endl;
    return 0;
}

int main()
{
    InitTimer();


    const int ThreadCount = 20;
    LockTester tester;
    tester.Init(ThreadCount);

    cout << "begin create thread" << endl;
    double timeBeg = GetCurrentTimerFlag();
    HANDLE handle[ThreadCount];
    for(int idx = 0; idx != ThreadCount; ++idx){
        handle[idx] = (HANDLE)_beginthreadex(NULL, 0, DoAction, tester.GetItem(idx), 0, NULL);
    }
    double timeEnd = GetCurrentTimerFlag();
    cout.precision(10);
    cout << "wati for thread. timeBeg=" << timeBeg << ",timeEnd=" << timeEnd << ",diff=" << (timeEnd - timeBeg) << endl;
    WaitForMultipleObjects(ThreadCount, handle, TRUE, INFINITE);
    cout << "end" << endl;

    char c;
    cout << "exit." << endl;
    cin >> c;

    return 1;
}
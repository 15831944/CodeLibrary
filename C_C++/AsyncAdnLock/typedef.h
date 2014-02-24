#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include <Windows.h>

class LockItem{
public:
    LockItem():count(0){
        InitializeCriticalSectionAndSpinCount(&cri, 1000);
    };
    virtual ~LockItem(){
        DeleteCriticalSection(&cri);
    };

    void Lock(){EnterCriticalSection(&cri);};
    void Unlock(){LeaveCriticalSection(&cri);};
    void DoAction(){
        int i = 300;
        while(i--)++count;
    };
    int GetCount(){return count;};
private:
    int count;
    CRITICAL_SECTION cri;
};

#endif
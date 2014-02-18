#ifndef _TESTER_H
#define _TESTER_H

#include "typedef.h"
#include <vector>
#include <Windows.h>
#include <assert.h>

class LockTester{
public:
    LockTester(){
        InitializeCriticalSectionAndSpinCount(&cri, 1000);
    };
    virtual ~LockTester(){
        DeleteCriticalSection(&cri);
    };
    void Init(const int count){
        for(int i = 0; i != count; ++i){
            LockItem* p = new LockItem();
            items.push_back(p);
        }
    };
    void Lock(){EnterCriticalSection(&cri);};
    void Unlock(){LeaveCriticalSection(&cri);};

    LockItem* GetItem(const int idx){
        assert(idx >= 0 && idx < items.size());
        return items[idx];
    };

    void DoAction(const int idx){
        LockItem* pItem = GetItem(idx);
        pItem->Lock();
        pItem->DoAction();
        pItem->Unlock();
    };

    void DoAction_NoLock(const int idx){
        LockItem* pItem = GetItem(idx);
        pItem->DoAction();
    };
private:
    std::vector<LockItem*> items;
    CRITICAL_SECTION cri;
};

#endif
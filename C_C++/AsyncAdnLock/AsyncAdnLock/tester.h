#ifndef _TESTER_H
#define _TESTER_H

#include "typedef.h"
#include <vector>

class LockTester{
public:
    LockTester(){

    };
    virtual ~LockTester(){
        
    };
    void Init(const int count){
        for(int i = 0; i != count; ++i){
            LockItem* p = new LockItem();
            items.push_back(p);
        }
    };
    LockItem* GetItem(const int idx){
        return items[idx];
    }

private:
    std::vector<LockItem*> items;

};

#endif
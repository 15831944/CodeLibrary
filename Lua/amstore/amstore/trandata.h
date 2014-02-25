#ifndef _AM_TRANDATA_H
#define _AM_TRANDATA_H

#include "sys/sync.h"
#include "config.h"

//��������ݱ�����
//TΪҪ��������ͣ�����Ҫһ��һ�� TransactionIdx_T tranIdx ��Ա.
template<class T, size_t N = 1000> class TranData{
    
public:
    TranData():index(0)
    {
        pDataArr = new T[N];
    };

    virtual ~TranData()
    {
        criSect.Lock();
        delete [] pDataArr;
        criSect.Unlock();
    };

    T* NewTranData(TransactionIdx_T tranIdx)
    {
        criSect.Lock();
        T* pRet = pDataArr + index;
        pRet->tranIdx = tranIdx;
        index = (index + 1) % N;
        criSect.Unlock();
        return pRet;
    };

    T* GetTranData(TransactionIdx_T tranIdx)
    {
        T* pRet = NULL;
        criSect.Lock();
        int curIdx = 0;
        for(int idx = 0; idx < N; ++idx){
            curIdx = (index + N - idx) % N;
            if((pDataArr + curIdx)->tranIdx == tranIdx){
                pRet = pDataArr + curIdx;
                break;
            }
        }
        criSect.Unlock();
        return pRet;
    };
private:
    T* pDataArr;
    CCriticalSection criSect;
    int32_t index;
};

#endif
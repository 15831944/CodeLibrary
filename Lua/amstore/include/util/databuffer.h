#ifndef _DATABUFFER_H
#define _DATABUFFER_H

#include <list>
#include "sys/sync.h"

//数据缓冲模板，主要目的是为了避免new操作的频繁使用
//T为要服务的类型,其需要有一个成员T* pBuffNext
//为每次重新分配的对象个数
template<class T, size_t N = 1000> class DataBuffer
{
public:
    DataBuffer():m_lpHead(NULL), m_lpTail(NULL), m_freeCount(0){};

    virtual ~DataBuffer()
    {
        m_criSect.Lock();
        for(std::list<T*>::iterator it = m_arrList.begin(); it != m_arrList.end(); ++it){
            T *pArr = *it;
            delete [] pArr;
        }
        m_criSect.Unlock();
    };

public:
    T* GetData(const size_t count = 1)
    {
        if(count <= 0) return NULL;

        m_criSect.Lock();

        if(m_freeCount < count + 1) IncBuffer();

        T *lpNodes = m_lpHead;
        T *lpTail = lpNodes;
        m_lpHead = m_lpHead->pBuffNext;

        for (int i = 1; i < count; ++i){
            lpTail = m_lpHead;
            m_lpHead = m_lpHead->pBuffNext;
        }

        lpTail->pBuffNext = NULL;
        m_freeCount -= count;

        m_criSect.Unlock();

        return lpNodes;
    };

    int RecycleData(T* pNote)
    {
        int count = 0;
        m_criSect.Lock();
        while(pNote){
            m_lpTail->pBuffNext = pNote;
            m_lpTail = m_lpTail->pBuffNext;
            m_freeCount += 1;
            count += 1;
            pNote = pNote->pBuffNext;
        }
        m_lpTail->pBuffNext = NULL;
        m_criSect.Unlock();
        return count;
    };

    int EnsureBuffer(const size_t count){
        do{
            if(m_freeCount < count + 1){
                IncBuffer();
            }else{
                break;
            }
        }while(true)
        return count;
    }

private:
    int IncBuffer()
    {
        T* pArr = new T[N];

        int idx = 0;
        if(!m_lpHead){
            m_lpHead = pArr + idx;
            m_lpTail = pArr + idx;
            m_lpTail->pBuffNext = NULL;
            idx++;
        }

        for(; idx < N; ++idx){
            m_lpTail->pBuffNext = pArr + idx;
            m_lpTail = m_lpTail->pBuffNext;
            m_lpTail->pBuffNext = NULL;
        }

        m_arrList.push_back(pArr);
        m_freeCount += N;

        return N;
    };

private:
    T* m_lpHead;
    T* m_lpTail;
    CCriticalSection m_criSect;
    std::list<T*> m_arrList;
    size_t m_freeCount;
};

#endif
#ifndef _SYNC_H
#define _SYNC_H

#include "sysdef.h"
#include "types.h"
#include "typedef.h"

class CSyncObject;
class CSemaphore;
class CMutex;
class CEvent;
class CCriticalSection;
class CSingleLock;

//同步对象基类
class CTLSyncObject
{
public:
    CTLSyncObject(LPCTSTR pstrName);

    // Attributes
public:
    operator HANDLE() const;
    HANDLE  m_hObject;

    // 加锁
    //param dwTimeOut 超时时间,单位毫秒。INFINITE表示不设超时时间
    //return TRUE加锁成功 FALSE加锁失败
    virtual BOOL Lock(DWORD dwTimeout = INFINITE);

    // 解锁
    //return TRUE解锁成功 FALSE解锁失败
    virtual BOOL Unlock() = 0;

    // Implementation
public:
    virtual ~CTLSyncObject();
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif
    friend class CSingleLock;
};


//信号量类
class CSemaphore : public CTLSyncObject
{
public:
    CSemaphore(int32_t lInitialCount = 1, int32_t lMaxCount = 1, LPCTSTR pstrName=NULL);
    // Implementation
public:
    virtual ~CSemaphore();
    virtual BOOL Unlock();
    virtual BOOL Unlock(int32_t lCount, int32_t* lprevCount = NULL);
};


//! 互斥量类
class CMutex : public CTLSyncObject
{
public:
    CMutex(BOOL bInitiallyOwn = FALSE, LPCTSTR lpszName = NULL);
#if defined(__LINUX) || defined(__MAC)
    virtual BOOL Lock(DWORD dwTimeout = INFINITE);
#endif
    // Implementation
public:
    virtual ~CMutex();
    BOOL Unlock();
};


// 事件类
class CEvent : public CTLSyncObject
{
public:
    CEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE, LPCTSTR lpszNAme = NULL);

    // Operations
public:
    BOOL SetEvent();
    BOOL PulseEvent();
    BOOL ResetEvent();
    BOOL Unlock();

    // Implementation
public:
    virtual ~CEvent();
};

// CCriticalSection
#if defined(__LINUX) || defined(__MAC)
typedef struct _RTL_CRITICAL_SECTION {
    DWORD OwningThread;            //!< 记录线程ID
    HANDLE LockSemaphore;        //!< 信号量对象
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;
#endif


// 临界区类
class CCriticalSection : public CTLSyncObject
{
    // Constructor
public:
    CCriticalSection();

    // Attributes
public:
    operator CRITICAL_SECTION*();
    CRITICAL_SECTION m_sect;

    // Operations
public:
    BOOL Unlock();

    // 加锁
    //return TRUE加锁成功 FALSE加锁失败
    BOOL Lock();

    // 加锁
    //param dwTimeOut 超时时间
    //return TRUE加锁成功 FALSE加锁失败
    BOOL Lock(DWORD dwTimeout);

    // Implementation
public:
    virtual ~CCriticalSection();
};


//! 锁对象类
/*! 
*  
*/

class CSingleLock
{
    // Constructors
public:
    CSingleLock(CTLSyncObject* pObject, BOOL bInitialLock = FALSE);

    // Operations
public:
    //! 加锁
    //dwTimeOut 超时时间,单位毫秒。INFINITE表示不设超时时间
    //TRUE加锁成功 FALSE加锁失败
    BOOL Lock(DWORD dwTimeOut = INFINITE);

    // 解锁
    //return TRUE解锁成功 FALSE解锁失败
    BOOL Unlock();

    // Implementation
public:
    ~CSingleLock();

protected:
    CTLSyncObject*    m_pObject;        // 同步对象
    BOOL            m_bAcquired;    // 是否被占用标记
};


//读写锁。允许多个线程读，一个线程写。并且写线程优先。
class CReadWriteLock
{
public:
    CReadWriteLock();
    ~CReadWriteLock();

public:
    BOOL ReadLock(DWORD dwTimeOut = INFINITE);
    BOOL WriteLock(DWORD dwTimeOut = INFINITE);
    void Unlock();

private:
    bool _waiting_for_lock(DWORD dwTimeOut, short& nWaitingCount);
private:
    short    m_nLockStatus;                /* 锁状态值 */
    short    m_nReadLockCount;            /* 读锁计数 */
    short    m_nReadWaitingCount;        /* 读等待计数 */
    short    m_nWriteWaitingCount;        /* 写等待计数 */

    HANDLE                m_evEvent;        /* 通知事件 Event */
    CCriticalSection    m_stLock;        /* 锁对象 */
};


// 原子操作，变量递增
//param lpAddend 变量指针
//return *lpAddend + 1
//remarks *lpAddend = *lpAddend + 1
integer TL_InterlockedIncrement (integer volatile *lpAddend);

// 原子操作，变量递减
//param lpAddend 变量指针
//return *lpAddend - 1
//remarks *lpAddend = *lpAddend - 1
integer TL_InterlockedDecrement (integer volatile *lpAddend);

// 原子操作，变量赋值
//param Target    目标变量
//param Value    赋值变量
//return Value
//remarks *Target = Value
integer TL_InterlockedExchange (integer volatile *Target, integer Value);

// 原子操作，变量相加
//param Addend    目标变量
//param Value    加操作变量
//return *Addend初始值
//remarks *Addend = *Addend + Value
integer TL_InterlockedExchangeAdd (integer volatile *Addend, integer Value);

// 原子操作，变量比较
//param Destination    目标变量
//param Exchange    赋值变量
//param Comperand    比较变量
//return *Destination初始值
//remarks 如Comperand==*Destination,则执行*Destination=Exchange
integer TL_InterlockedCompareExchange (integer volatile *Destination, integer Exchange, integer Comperand);

// 原子操作，位比较并赋值
//param Base    目标变量
//param Bit        目标位
//return *Base & (1<<Bit)
//remarks 如(! (*Base & (1<<Bit)) ) 则 *Base = (*Base | (1<<Bit))
BOOL TL_InterlockedBitTestAndSet(integer volatile *Base, integer Bit);


// 原子操作，指针比较
//param Destination    目标变量
//param Exchange    赋值变量
//param Comperand    比较变量
//return *Destination初始值
//remarks 如Comperand==*Destination,则执行*Destination=Exchange
PVOID  TL_InterlockedCompareExchangePointer(PVOID volatile *Destination, PVOID ExChange, PVOID Comperand);


// 原子操作，变量赋值
//param Target    目标变量
//param Value    赋值变量
//return Value
//remarks *Target = Value
PVOID TL_InterlockedExchangePointer(integer volatile *Target, integer Value);

// 创建事件对象
//param bManualReset    复位方式
// <ul>
// <li> 复位方式
//    <ol>
//    <li> TRUE        必须用TL_ResetEvent手工恢复到无信号状态
//    <li> FALSE    事件被等待线程释放后，自动恢复到无信号状态
//    </ol>
// </ul>
//param bInitialState    初始状态，如TRUE初始为有信号状态，否则为无信号状态
//param lpName            事件对象命名，可以是无名
//return 事件对象句柄，如失败返回NULL
HANDLE TL_CreateEvent(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName);

// 设置事件信号
//param hEvent    事件对象句柄
//return 如操作成功返回非零值，否则返回0
BOOL TL_SetEvent(HANDLE hEvent);

// 重置事件到无信号状态
//param hEvent    事件对象句柄
//return 如操作成功返回非零值，否则返回0
BOOL TL_ResetEvent(HANDLE hEvent);

// 销毁事件信号
//param hEvent    事件对象句柄
//return 如操作成功返回非零值，否则返回0
BOOL TL_DestoryEvent(HANDLE hEvent);

// 销毁对象句柄
//param hObject    对象句柄
//return 如操作成功返回非零值，否则返回0
BOOL TL_CloseHandle(HANDLE hObject);


// 等待事件信号
//param hHandle    对象句柄
//param dwMilliseconds 超时时间，单位毫秒。-1为不超时。
//return 有信号状态返回WAIT_OBJECT_0，超时返回WAIT_TIMEOUT
DWORD TL_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

//信号状态返回值定义
#ifndef WAIT_OBJECT_0
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)
#define WAIT_OBJECT_0                    ((STATUS_WAIT_0 ) + 0 )    //!< 有信号状态返回
#define WAIT_FAILED                     (DWORD)0xFFFFFFFF            //!< 信号状态失败
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)
#define WAIT_TIMEOUT                        STATUS_TIMEOUT            //!< 超时返回
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (HANDLE)-1        //!< 无效对象句柄
#endif

#endif
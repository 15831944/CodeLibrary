#ifndef _SYNC_H
#define _SYNC_H

#include "sysdef.h"
#include "types.h"

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


class CSyncer{
public:
    CSyncer(CTLSyncObject& sync):_sync(sync){
        _sync.Lock();
    }

    virtual ~CSyncer(){
        _sync.Unlock();
    }
private:
    CTLSyncObject& _sync;
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


// 锁对象类
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


/////////////////////////////////////////////////////////////////////////////////////////
//线程相关

//线程工作函数返回值定义
#ifdef WIN32
#define THREAD_RETURN DWORD		//!< Windows线程函数返回值
#else
#define THREAD_RETURN void*		//!< Linux线程函数返回值
#endif



//!
//线程工作函数定义
//如：THREAD_RETURN DemoProc(void* pParam);
typedef THREAD_RETURN ( *PTL_THREAD_START_ROUTINE)( void* lpThreadParameter );
typedef PTL_THREAD_START_ROUTINE LPTL_THREAD_START_ROUTINE;


//创建线程
//param 工作函数指针、工作函数参数
//return 返回 线程ID
HANDLE TL_CreateThread(LPTL_THREAD_START_ROUTINE lpStartAddress,  void* lpParameter, LPDWORD lpThreadId);


// 等待线程退出
//param hThread 线程句柄
//param dwWaitTime 等待退出的时间(毫秒)
//return 句柄无效或者成功退出返回true，超时返回false
BOOL TL_WaitThread(HANDLE hThread,	DWORD  dwWaitTime = INFINITE);


//等待线程退出
//param hThread 线程句柄
//param bExit   线程退出的标志。函数等待这个值变成 true ，就退出。认为线程已经退出。
//param dwWaitTime 等待退出的时间(毫秒)
//return 句柄无效或者成功退出返回true，超时调用 TL_TerminateThread 杀死线程，并返回false
//remarks 这个函数不使用 WaitForSingleObject ，因为在动态库里面退出时，这个函数可能会死锁
bool TL_WaitThread(HANDLE hThread,	volatile bool  &bExit, DWORD  dwWaitTime = INFINITE);

//强行结束线程
//param hThread 线程句柄
//return 无
void TL_TerminateThread(HANDLE hThread);


//取得线程ID
//return 线程ID
DWORD TL_GetCurrentThreadId();


//取得线程
//return 线程
HANDLE TL_GetCurrentThread();


//取得进程ID
//\return 进程ID
DWORD TL_GetCurrentProcessId();


// 取父进程ID 传入0时取当前进程父进程
DWORD TL_GetParentProcessID(DWORD dwProcessID = 0);


//休眠
//param dwMilliseconds 休眠时间，单位毫秒
//return 无
void TL_Sleep( DWORD dwMilliseconds );

//////////////////////////////////////////////////////////////////////////////////////////
//!线程的类操作模型

#if defined(__LINUX) || defined(__MAC)
#define HAVE_PTHREAD_H
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#else
#ifdef __WINDOWS
#include <windows.h>
#include <process.h>
#endif
#endif

#if defined HAVE_PTHREAD_H
#define CRFPP_USE_THREAD 1
#endif

#if(defined(_WIN32) && ! defined (__CYGWIN__))
#define CRFPP_USE_THREAD 1
#define BEGINTHREAD(src, stack, func, arg, flag, id) \
    (HANDLE)_beginthreadex((void *)(src),(unsigned)(stack), \
    (unsigned(_stdcall *)(void *))(func),(void *)(arg), \
    (unsigned)(flag),(unsigned *)(id))
#endif


//线程类
class CTL_Thread
{
public:
    CTL_Thread() 
    {
#ifdef HAVE_PTHREAD_H
        hnd_ = 0;
#else
        hnd_ = NULL;
#endif
        m_bExit = false;
        m_bRuning = false;
    }
    virtual ~CTL_Thread() {};

public:

    //线程函数
    static THREAD_RETURN Wrapper(void *ptr);

    //线程过程，派生实现
    virtual void Run()
    {
        while(m_bRuning)
        {
            TL_Sleep(1000);
        }
    }

    //启动线程
    void Start();

    virtual void Stop(DWORD dwTimeout)
    {
        m_bRuning = false;
        Join(dwTimeout);
    }

    // 等待线程结束
    //remarks 如果达到超时时间没有退出，强制杀死线程
    void Join(DWORD dwTimeout = INFINITE);

    /////////////////////////////////////////////////////////////////////////////////////
private:
#ifdef HAVE_PTHREAD_H
    pthread_t hnd_;		//!< Linux线程类型
#else
#ifdef _WIN32
    HANDLE  hnd_;		//!< Windows线程句柄
#endif
#endif
    volatile bool  m_bExit;		//线程是否已经退出。 = true 表示已经退出
protected:
    volatile bool m_bRuning;
};

#endif
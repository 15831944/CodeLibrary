#if (defined(__LINUX)|| defined(__MAC))
#include <pthread.h>
#include "mt_linux.h"
#endif

#include "sync.h"

#ifndef UNUSED
#ifdef _DEBUG
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif
#define UNUSED_ALWAYS(x) x
#endif

//////////////////////////CTLSyncObject//////////////////////////
CTLSyncObject::CTLSyncObject(LPCTSTR pstrName)
{
    UNUSED(pstrName);   // unused in release builds

    m_hObject = NULL;

#ifdef _DEBUG
    //    m_strName = pstrName;
#endif
}

CTLSyncObject::~CTLSyncObject()
{
    if (m_hObject != NULL)
    {
        ::CloseHandle(m_hObject);
        m_hObject = NULL;
    }
}

BOOL CTLSyncObject::Lock(DWORD dwTimeout)
{
    if (TL_WaitForSingleObject(m_hObject, dwTimeout) == WAIT_OBJECT_0)
        return TRUE;
    else
        return FALSE;
}

#ifdef _DEBUG
void CTLSyncObject::AssertValid() const
{
}
#endif

CTLSyncObject::operator HANDLE() const
{ 
    return m_hObject;
}

//////////////////////////CSemaphore//////////////////////////
CSemaphore::CSemaphore(int32_t lInitialCount, int32_t lMaxCount,
    LPCTSTR pstrName)
    :  CTLSyncObject(pstrName)
{
    ASSERT(lMaxCount > 0);
    ASSERT(lInitialCount <= lMaxCount);

    m_hObject = ::CreateSemaphore(NULL, lInitialCount, lMaxCount, pstrName);
}

CSemaphore::~CSemaphore()
{
}

BOOL CSemaphore::Unlock(int32_t lCount, int32_t* lpPrevCount /* =NULL */)
{
    return ::ReleaseSemaphore(m_hObject, lCount, lpPrevCount);
}

BOOL CSemaphore::Unlock() { return Unlock(1, NULL); }


//////////////////////////CMutex//////////////////////////
CMutex::CMutex(BOOL bInitiallyOwn, LPCTSTR pstrName): CTLSyncObject(pstrName)
{
    m_hObject = ::CreateMutex(NULL, bInitiallyOwn, pstrName);
}

CMutex::~CMutex()
{
#if defined(__LINUX) || defined(__MAC)
    if (m_hObject != NULL)
    {
        ::DestoryMutex(m_hObject);
        m_hObject = NULL;
    }
#endif
}

BOOL CMutex::Unlock()
{
    return ::ReleaseMutex(m_hObject);
}

#if defined(__LINUX) || defined(__MAC)
BOOL CMutex::Lock(DWORD dwTimeout)
{
    return ::LockMutex(m_hObject, dwTimeout);
}
#endif


//////////////////////////CEvent//////////////////////////
CEvent::CEvent(BOOL bInitiallyOwn, BOOL bManualReset, LPCTSTR pstrName) : CTLSyncObject(pstrName)
{
    m_hObject = TL_CreateEvent(bManualReset,
        bInitiallyOwn, pstrName);
}

CEvent::~CEvent()
{
#if defined(__LINUX) || defined(__MAC)
    if (m_hObject != NULL)
    {
        TL_DestoryEvent(m_hObject);
        m_hObject = NULL;
    }
#endif
}

BOOL CEvent::Unlock()
{
    return TRUE;
}

BOOL CEvent::SetEvent()
{ ASSERT(m_hObject != NULL); return TL_SetEvent(m_hObject); }
BOOL CEvent::PulseEvent()
{ ASSERT(m_hObject != NULL); return ::PulseEvent(m_hObject); }
BOOL CEvent::ResetEvent()
{ ASSERT(m_hObject != NULL); return TL_ResetEvent(m_hObject); }


//////////////////////////CCriticalSection//////////////////////////
CCriticalSection::CCriticalSection() : CTLSyncObject(NULL)
{
    ::InitializeCriticalSection(&m_sect);
}

CCriticalSection::operator CRITICAL_SECTION*()
{
    return (CRITICAL_SECTION*) &m_sect;
}

CCriticalSection::~CCriticalSection()
{
    ::DeleteCriticalSection(&m_sect);
}

BOOL CCriticalSection::Lock()
{
    ::EnterCriticalSection(&m_sect);
    return TRUE;
}


//brief CCriticalSection::Lock 加锁，支持超时

//param dwTimeout 超时时间，单位毫秒，-1不超时
//return  
BOOL CCriticalSection::Lock(DWORD dwTimeout)
{
    if(dwTimeout == (DWORD)-1)
        return Lock();
    DWORD dwCurrentSlice = 0;
    while(!::TryEnterCriticalSection(&m_sect))
    {
        if(dwCurrentSlice < dwTimeout)
        {
            TL_Sleep(100);
            dwCurrentSlice += 100;
        }
        else
        {
            //超时
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CCriticalSection::Unlock()
{
    ::LeaveCriticalSection(&m_sect);
    return TRUE;
}


//////////////////////////CSingleLock//////////////////////////
CSingleLock::CSingleLock(CTLSyncObject* pObject, BOOL bInitialLock)
{
    m_pObject = pObject;
    m_bAcquired = FALSE;
    if (bInitialLock) Lock();
}

BOOL CSingleLock::Lock(DWORD dwTimeOut /* = INFINITE */)
{
    ASSERT(!m_bAcquired);
    if (m_pObject) m_bAcquired = m_pObject->Lock(dwTimeOut);
    return m_bAcquired;
}

BOOL CSingleLock::Unlock()
{
    if (m_bAcquired && m_pObject) m_bAcquired = !m_pObject->Unlock();

    // successfully unlocking means it isn't acquired
    return !m_bAcquired;
}

CSingleLock::~CSingleLock()
{ Unlock(); }


//////////////////////////CReadWriteLock//////////////////////////
//读写锁。允许多个线程读，一个线程写。并且写线程优先。
#define RWLOCK_IDLE				0x0000	/* 空闲 */
#define RWLOCK_READ				0x0001	/* 读锁 */
#define RWLOCK_WRITE			0x0002	/* 写锁 */

CReadWriteLock::CReadWriteLock()
{
    m_nLockStatus        = RWLOCK_IDLE;
    m_nReadLockCount     = 0;
    m_nReadWaitingCount  = 0;
    m_nWriteWaitingCount = 0;

    //假设当前有多个读锁请求正在等待写锁释放,那么当写锁被释放时,所有这些读锁都应该有机会获得执行.
    m_evEvent = TL_CreateEvent(TRUE, FALSE, NULL);
}

CReadWriteLock::~CReadWriteLock()
{
#if defined(__LINUX) || defined(__MAC)
    TL_DestoryEvent(m_evEvent);
#else
    TL_CloseHandle(m_evEvent);
#endif
}

BOOL CReadWriteLock::ReadLock(DWORD dwTimeOut)
{
    bool isWaitReturn = false;
    while(1)
    {
        m_stLock.Lock();

        if(isWaitReturn)
        {
            m_nReadWaitingCount--;		//等待事件返回,重新竞争锁.
        }

        if(m_nLockStatus == RWLOCK_IDLE)
        {
            m_nLockStatus = RWLOCK_READ;	//空闲状态,直接得到控制权
            m_nReadLockCount++;
            m_stLock.Unlock();
            break;
        }
        else if(m_nLockStatus == RWLOCK_READ)
        {
            if(m_nWriteWaitingCount > 0)
            {
                if(!_waiting_for_lock(dwTimeOut, m_nReadWaitingCount))
                {
                    return FALSE;
                }
                isWaitReturn = true;		//等待返回,继续尝试加锁.
            }
            else
            {	
                m_nReadLockCount++;		//得到读锁,计数+1
                m_stLock.Unlock();
                break;
            }
        }
        else if(m_nLockStatus == RWLOCK_WRITE)
        {
            if(!_waiting_for_lock(dwTimeOut, m_nReadWaitingCount))
            {
                return FALSE;
            }
            isWaitReturn = true;		//等待返回,继续尝试加锁.
        }
        else
        {
            assert(0);
            break;
        }
    }
    return TRUE;
}

BOOL CReadWriteLock::WriteLock(DWORD dwTimeOut)
{
    bool isWaitReturn = false;
    while(1)
    {
        m_stLock.Lock();
        if(isWaitReturn)
        {
            m_nWriteWaitingCount--;		//等待事件返回,重新竞争锁.
        }
        if(m_nLockStatus == RWLOCK_IDLE)
        {
            m_nLockStatus = RWLOCK_WRITE;	//空闲状态,直接得到控制权
            m_stLock.Unlock();
            break;
        }
        else
        {
            if(!_waiting_for_lock(dwTimeOut, m_nWriteWaitingCount))
            {
                return FALSE;
            }
            isWaitReturn = true;
        }
    }
    return TRUE;
}

bool CReadWriteLock::_waiting_for_lock(DWORD dwTimeOut, short& nWaitingCount)
{
    nWaitingCount++;

    TL_ResetEvent(m_evEvent);
    m_stLock.Unlock();

    //虽然 LeaveCriticalSection() 和 WaitForSingleObject() 之间有一个时间窗口,
    //但是由于windows平台的事件信号是不会丢失的,所以没有问题.
    if(TL_WaitForSingleObject(m_evEvent, dwTimeOut) == WAIT_TIMEOUT)
    {
        m_stLock.Lock();
        nWaitingCount--;
        m_stLock.Unlock();
        return false;
    }
    return true;
}

void CReadWriteLock::Unlock()
{
    m_stLock.Lock();

    if(m_nReadLockCount > 0)
    {
        /* 读锁解锁 */
        m_nReadLockCount--;

        if(0 == m_nReadLockCount)
        {
            m_nLockStatus = RWLOCK_IDLE;

            /* 释放 */
            if( m_nWriteWaitingCount > 0 || m_nReadWaitingCount > 0 )
            { 
                //此时有锁请求正在等待,激活所有等待的线程.(手动事件).
                //使这些请求重新竞争锁.
                TL_SetEvent(m_evEvent);
            }
            else
            {
                //空闲
            }
        }
        else
        {
            //还有读锁 
        }
    }
    else
    {
        m_nLockStatus = RWLOCK_IDLE;

        //写锁解锁
        if( m_nWriteWaitingCount > 0 || m_nReadWaitingCount > 0 )
        {
            //如果在占有互斥量_stLock的情况下,触发事件,那么可能会使一些锁请求不能得到竞争机会.
            //假设调用unlock时,另一个线程正好调用rlock或者wlock.如果不释放互斥量,只有之前已经等待的锁请求有机会获得锁控制权.
            TL_SetEvent(m_evEvent);
        }
        else
        {
            /* 空闲 */
        }
    }
    m_stLock.Unlock();
}


#ifdef __WINDOWS
integer TL_InterlockedIncrement (integer volatile *lpAddend)
{
    return ::InterlockedIncrement(lpAddend);
}

integer TL_InterlockedDecrement (integer volatile *lpAddend)
{
    return ::InterlockedDecrement (lpAddend);
}

integer TL_InterlockedExchange(integer volatile *Target, integer Value)
{
    return ::InterlockedExchange(Target, Value);
}

integer TL_InterlockedExchangeAdd (integer volatile *Addend, integer Value)
{
    return ::InterlockedExchangeAdd(Addend, Value);
}

integer TL_InterlockedCompareExchange(integer volatile *Destination, integer Exchange, integer Comperand)
{
    return ::InterlockedCompareExchange(Destination, Exchange, Comperand);
}

BOOL TL_InterlockedBitTestAndSet(integer volatile *Base, integer Bit)
{
    return ::InterlockedBitTestAndSet(Base, Bit);
}

PVOID TL_InterlockedCompareExchangePointer(PVOID volatile *Destination, PVOID ExChange, PVOID Comperand)
{
    return ::InterlockedCompareExchangePointer(Destination, ExChange, Comperand);
}

PVOID TL_InterlockedExchangePointer(integer volatile* Target, integer Value)
{
    return InterlockedExchangePointer(Target, Value);
}

HANDLE TL_CreateEvent(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName)
{
    return ::CreateEvent(NULL, bManualReset, bInitialState, lpName);
}

BOOL TL_SetEvent(HANDLE hEvent)
{
    return ::SetEvent(hEvent);
}

BOOL TL_ResetEvent(HANDLE hEvent)
{
    return ::ResetEvent(hEvent);
}

BOOL TL_CloseHandle(HANDLE hObject)
{
    return ::CloseHandle(hObject);
}

DWORD TL_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
{
    return ::WaitForSingleObject(hHandle, dwMilliseconds);
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//线程相关实现
#ifdef __WINDOWS

#ifndef WINBASEAPI
#define WINBASEAPI
#endif
#ifndef WINAPI
#define WINAPI
#endif

WINBASEAPI VOID WINAPI Sleep(__in DWORD dwMilliseconds);


WINBASEAPI DWORD WINAPI GetCurrentThreadId(VOID);

WINBASEAPI DWORD WINAPI GetCurrentProcessId(VOID);

void TL_Sleep(DWORD dwMilliseconds)
{
    ::Sleep(dwMilliseconds);
}


//创建线程
HANDLE TL_CreateThread(
    LPTL_THREAD_START_ROUTINE lpStartAddress,
    void* lpParameter,
    LPDWORD lpThreadId)
{
    return ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, (void*)lpParameter, 0, lpThreadId);
}

//等待线程退出
BOOL TL_WaitThread(HANDLE hThread, DWORD  dwWaitTime/* = INFINITE*/)
{
    if(hThread == NULL) return TRUE;
    return ::WaitForSingleObject(hThread, dwWaitTime) == WAIT_OBJECT_0;
}

bool TL_WaitThread(HANDLE hThread,	volatile bool &bExit, DWORD dwWaitTime)
{
    if(hThread == NULL)
    {
        return true;
    }
    DWORD dwExitCode = 0;
    ::GetExitCodeThread(hThread, &dwExitCode);
    if(dwExitCode != STILL_ACTIVE)
    {
        return true;
    }

    DWORD dwSleep = 0;
    while(!bExit && dwSleep < dwWaitTime)
    {
        TL_Sleep(20);
        dwSleep += 20;
    }
    TL_TerminateThread(hThread);
    return bExit;
}

void TL_TerminateThread(HANDLE hThread)
{
    if(hThread == NULL)
        return;
    ::TerminateThread(hThread, 0);
}

DWORD TL_GetCurrentThreadId()
{
    return (DWORD)::GetCurrentThreadId();
}

HANDLE TL_GetCurrentThread()
{
    return ::GetCurrentThread();
}

DWORD TL_GetCurrentProcessId()
{
    return (DWORD)::GetCurrentProcessId();
}


THREAD_RETURN CTL_Thread::Wrapper(void *ptr)
{
    CTL_Thread *p = static_cast<CTL_Thread *>(ptr);
    p->Run();
    p->m_bExit = true;
    return 0;
}


void CTL_Thread::Start()
{
    m_bRuning = true;
    DWORD id;
    hnd_ = TL_CreateThread(&CTL_Thread::Wrapper, this, &id);
}

void CTL_Thread::Join(DWORD dwTimeout)
{
    if(hnd_)
    {
        TL_WaitThread(hnd_, m_bExit, dwTimeout);
        CloseHandle(hnd_);
        hnd_ = NULL;
    }
}

#endif //WIN32
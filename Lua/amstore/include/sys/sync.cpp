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


//brief CCriticalSection::Lock ������֧�ֳ�ʱ

//param dwTimeout ��ʱʱ�䣬��λ���룬-1����ʱ
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
			//��ʱ
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
//��д�����������̶߳���һ���߳�д������д�߳����ȡ�
#define RWLOCK_IDLE				0x0000	/* ���� */
#define RWLOCK_READ				0x0001	/* ���� */
#define RWLOCK_WRITE			0x0002	/* д�� */

CReadWriteLock::CReadWriteLock()
{
	m_nLockStatus        = RWLOCK_IDLE;
	m_nReadLockCount     = 0;
	m_nReadWaitingCount  = 0;
	m_nWriteWaitingCount = 0;

	//���赱ǰ�ж�������������ڵȴ�д���ͷ�,��ô��д�����ͷ�ʱ,������Щ������Ӧ���л�����ִ��.
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
			m_nReadWaitingCount--;		//�ȴ��¼�����,���¾�����.
		}

		if(m_nLockStatus == RWLOCK_IDLE)
		{
			m_nLockStatus = RWLOCK_READ;	//����״̬,ֱ�ӵõ�����Ȩ
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
				isWaitReturn = true;		//�ȴ�����,�������Լ���.
			}
			else
			{	
				m_nReadLockCount++;		//�õ�����,����+1
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
			isWaitReturn = true;		//�ȴ�����,�������Լ���.
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
			m_nWriteWaitingCount--;		//�ȴ��¼�����,���¾�����.
		}
		if(m_nLockStatus == RWLOCK_IDLE)
		{
			m_nLockStatus = RWLOCK_WRITE;	//����״̬,ֱ�ӵõ�����Ȩ
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

	//��Ȼ LeaveCriticalSection() �� WaitForSingleObject() ֮����һ��ʱ�䴰��,
	//��������windowsƽ̨���¼��ź��ǲ��ᶪʧ��,����û������.
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
		/* �������� */
		m_nReadLockCount--;

		if(0 == m_nReadLockCount)
		{
			m_nLockStatus = RWLOCK_IDLE;

			/* �ͷ� */
			if( m_nWriteWaitingCount > 0 || m_nReadWaitingCount > 0 )
			{ 
				//��ʱ�����������ڵȴ�,�������еȴ����߳�.(�ֶ��¼�).
				//ʹ��Щ�������¾�����.
				TL_SetEvent(m_evEvent);
			}
			else
			{
				//����
			}
		}
		else
		{
			//���ж��� 
		}
	}
	else
	{
		m_nLockStatus = RWLOCK_IDLE;

		//д������
		if( m_nWriteWaitingCount > 0 || m_nReadWaitingCount > 0 )
		{
			//�����ռ�л�����_stLock�������,�����¼�,��ô���ܻ�ʹһЩ�������ܵõ���������.
			//�������unlockʱ,��һ���߳����õ���rlock����wlock.������ͷŻ�����,ֻ��֮ǰ�Ѿ��ȴ����������л�����������Ȩ.
			TL_SetEvent(m_evEvent);
		}
		else
		{
			/* ���� */
		}
	}
	m_stLock.Unlock();
}

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

//ͬ���������
class CTLSyncObject
{
public:
    CTLSyncObject(LPCTSTR pstrName);

    // Attributes
public:
    operator HANDLE() const;
    HANDLE  m_hObject;

    // ����
    //param dwTimeOut ��ʱʱ��,��λ���롣INFINITE��ʾ���賬ʱʱ��
    //return TRUE�����ɹ� FALSE����ʧ��
    virtual BOOL Lock(DWORD dwTimeout = INFINITE);

    // ����
    //return TRUE�����ɹ� FALSE����ʧ��
    virtual BOOL Unlock() = 0;

    // Implementation
public:
    virtual ~CTLSyncObject();
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif
    friend class CSingleLock;
};


//�ź�����
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


//! ��������
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


// �¼���
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
    DWORD OwningThread;            //!< ��¼�߳�ID
    HANDLE LockSemaphore;        //!< �ź�������
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;
#endif


// �ٽ�����
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

    // ����
    //return TRUE�����ɹ� FALSE����ʧ��
    BOOL Lock();

    // ����
    //param dwTimeOut ��ʱʱ��
    //return TRUE�����ɹ� FALSE����ʧ��
    BOOL Lock(DWORD dwTimeout);

    // Implementation
public:
    virtual ~CCriticalSection();
};


//! ��������
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
    //! ����
    //dwTimeOut ��ʱʱ��,��λ���롣INFINITE��ʾ���賬ʱʱ��
    //TRUE�����ɹ� FALSE����ʧ��
    BOOL Lock(DWORD dwTimeOut = INFINITE);

    // ����
    //return TRUE�����ɹ� FALSE����ʧ��
    BOOL Unlock();

    // Implementation
public:
    ~CSingleLock();

protected:
    CTLSyncObject*    m_pObject;        // ͬ������
    BOOL            m_bAcquired;    // �Ƿ�ռ�ñ��
};


//��д�����������̶߳���һ���߳�д������д�߳����ȡ�
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
    short    m_nLockStatus;                /* ��״ֵ̬ */
    short    m_nReadLockCount;            /* �������� */
    short    m_nReadWaitingCount;        /* ���ȴ����� */
    short    m_nWriteWaitingCount;        /* д�ȴ����� */

    HANDLE                m_evEvent;        /* ֪ͨ�¼� Event */
    CCriticalSection    m_stLock;        /* ������ */
};


// ԭ�Ӳ�������������
//param lpAddend ����ָ��
//return *lpAddend + 1
//remarks *lpAddend = *lpAddend + 1
integer TL_InterlockedIncrement (integer volatile *lpAddend);

// ԭ�Ӳ����������ݼ�
//param lpAddend ����ָ��
//return *lpAddend - 1
//remarks *lpAddend = *lpAddend - 1
integer TL_InterlockedDecrement (integer volatile *lpAddend);

// ԭ�Ӳ�����������ֵ
//param Target    Ŀ�����
//param Value    ��ֵ����
//return Value
//remarks *Target = Value
integer TL_InterlockedExchange (integer volatile *Target, integer Value);

// ԭ�Ӳ������������
//param Addend    Ŀ�����
//param Value    �Ӳ�������
//return *Addend��ʼֵ
//remarks *Addend = *Addend + Value
integer TL_InterlockedExchangeAdd (integer volatile *Addend, integer Value);

// ԭ�Ӳ����������Ƚ�
//param Destination    Ŀ�����
//param Exchange    ��ֵ����
//param Comperand    �Ƚϱ���
//return *Destination��ʼֵ
//remarks ��Comperand==*Destination,��ִ��*Destination=Exchange
integer TL_InterlockedCompareExchange (integer volatile *Destination, integer Exchange, integer Comperand);

// ԭ�Ӳ�����λ�Ƚϲ���ֵ
//param Base    Ŀ�����
//param Bit        Ŀ��λ
//return *Base & (1<<Bit)
//remarks ��(! (*Base & (1<<Bit)) ) �� *Base = (*Base | (1<<Bit))
BOOL TL_InterlockedBitTestAndSet(integer volatile *Base, integer Bit);


// ԭ�Ӳ�����ָ��Ƚ�
//param Destination    Ŀ�����
//param Exchange    ��ֵ����
//param Comperand    �Ƚϱ���
//return *Destination��ʼֵ
//remarks ��Comperand==*Destination,��ִ��*Destination=Exchange
PVOID  TL_InterlockedCompareExchangePointer(PVOID volatile *Destination, PVOID ExChange, PVOID Comperand);


// ԭ�Ӳ�����������ֵ
//param Target    Ŀ�����
//param Value    ��ֵ����
//return Value
//remarks *Target = Value
PVOID TL_InterlockedExchangePointer(integer volatile *Target, integer Value);

// �����¼�����
//param bManualReset    ��λ��ʽ
// <ul>
// <li> ��λ��ʽ
//    <ol>
//    <li> TRUE        ������TL_ResetEvent�ֹ��ָ������ź�״̬
//    <li> FALSE    �¼����ȴ��߳��ͷź��Զ��ָ������ź�״̬
//    </ol>
// </ul>
//param bInitialState    ��ʼ״̬����TRUE��ʼΪ���ź�״̬������Ϊ���ź�״̬
//param lpName            �¼���������������������
//return �¼�����������ʧ�ܷ���NULL
HANDLE TL_CreateEvent(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName);

// �����¼��ź�
//param hEvent    �¼�������
//return ������ɹ����ط���ֵ�����򷵻�0
BOOL TL_SetEvent(HANDLE hEvent);

// �����¼������ź�״̬
//param hEvent    �¼�������
//return ������ɹ����ط���ֵ�����򷵻�0
BOOL TL_ResetEvent(HANDLE hEvent);

// �����¼��ź�
//param hEvent    �¼�������
//return ������ɹ����ط���ֵ�����򷵻�0
BOOL TL_DestoryEvent(HANDLE hEvent);

// ���ٶ�����
//param hObject    ������
//return ������ɹ����ط���ֵ�����򷵻�0
BOOL TL_CloseHandle(HANDLE hObject);


// �ȴ��¼��ź�
//param hHandle    ������
//param dwMilliseconds ��ʱʱ�䣬��λ���롣-1Ϊ����ʱ��
//return ���ź�״̬����WAIT_OBJECT_0����ʱ����WAIT_TIMEOUT
DWORD TL_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

//�ź�״̬����ֵ����
#ifndef WAIT_OBJECT_0
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)
#define WAIT_OBJECT_0                    ((STATUS_WAIT_0 ) + 0 )    //!< ���ź�״̬����
#define WAIT_FAILED                     (DWORD)0xFFFFFFFF            //!< �ź�״̬ʧ��
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)
#define WAIT_TIMEOUT                        STATUS_TIMEOUT            //!< ��ʱ����
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (HANDLE)-1        //!< ��Ч������
#endif

#endif
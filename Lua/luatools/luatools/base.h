/**********************************************************************
* 基本的操作，有一些对操作系统接口的封装
* Auth: ganlong
* Build: 2013.07.23
**********************************************************************/
#ifndef _BASE_H
#define _BASE_H

#pragma once

#include <cassert>

#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32)
#include <windows.h>
#else
#include <linux/kernel.h>
#endif


#define MIN(x, y) min(x, y)
#define MAX(x, y) max(x, y)

#pragma warning(disable:4996)

//----------字符操作相关---------------
#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32)
#define snprintf _snprintf
#endif

//----------线程同步相关---------------
#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32)
    typedef CRITICAL_SECTION CriticalSection;
    #define InitCriticalSection(x) InitializeCriticalSection(x)
    #define CriticalSectionLock(x) EnterCriticalSection((x))
    #define CriticalSectionUnLock(x) LeaveCriticalSection((x))


    //typedef HANDLE Locker;
    //Locker CreateLocker(){return CreateEvent(NULL, TRUE, FALSE, NULL);}
    //void CloseLocker(Locker hld){CloseHandle(hld);}
    //void GetLocker(Locker lck){WaitForSingleObject(lck, 0); SetEvent(lck);}
    //void ReleaseLocker(Locker lck){ResetEvent(lck);};

#else
    #define int CriticalSection
    #define InitCriticalSection(x) (x)
    #define CriticalSectionLock(x) (x)
    #define CriticalSectionUnLock(x) (x)


    //typedef int Locker;
    //Locker CreateLocker(){return 0;}
    //void CloseLocker(Locker hld){return true;}
    //void GetLocker(Locker lck){}
    //void ReleaseLocker(Locker lck){};
#endif

#define ASSERT assert

#endif
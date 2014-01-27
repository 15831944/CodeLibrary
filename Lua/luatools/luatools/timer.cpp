#include "timer.h"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <ostream>
#include <stdlib.h>

TimerStarter TimerStarter::instance;

LARGE_INTEGER nFreq;
LARGE_INTEGER nBeginTime;
LARGE_INTEGER nEndTime;


static time_t InnerTimer;
static double TimerFlag = 0;

void InitTimer()
{
    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);
    TimerFlag = (double)(nBeginTime.QuadPart)/(double)nFreq.QuadPart;
}

void GetTimeOnNano(TimerNano *timer)
{
    #pragma warning( disable : 4996 )
    static struct tm *local;
    InnerTimer = time(NULL);
    local = localtime(&InnerTimer);

    QueryPerformanceCounter(&nBeginTime);

    timer->year = local->tm_year + 1900;
    timer->mon = local->tm_mon + 1;
    timer->day = local->tm_mday;
    timer->hour = local->tm_hour;
    timer->min = local->tm_min;
    timer->sec = local->tm_sec;

    TimerFlag = (double)(nBeginTime.QuadPart)/(double)nFreq.QuadPart;

    //std::cout.precision(20);
    //std::cout << nBeginTime.QuadPart << " " << d1  << " freq:" << nFreq.QuadPart << std::endl;

    double d1 = TimerFlag;
    double val = 1.0;
    double d2 = modf(d1, &val);
    long d3 = static_cast<long>(1000000 * (d2 ));
    long d4 = static_cast<long>(1000000000 * (d2 ));

    timer->mill = static_cast<int>(1000 * d2);
    timer->micro = static_cast<int>(d3 % 1000);
    timer->nano = static_cast<int>(d4 % 1000);
}

void PrintTimeNano(TimerNano& obj)
{
    std::cout << obj.year << "-" << obj.mon << "-" << obj.day << " " << obj.hour << ":" << obj.min
        << ":" << obj.sec << " " << obj.mill << " " << obj.micro << " " << obj.nano << "\n";
}

double GetCurrentTimerFlag()
{
    QueryPerformanceCounter(&nBeginTime);
    TimerFlag = (double)(nBeginTime.QuadPart)/(double)nFreq.QuadPart;

    return TimerFlag;
}

void TestTimer()
{
    TimerNano curTimer;
    memset(&curTimer, 0, sizeof(curTimer));

    GetTimeOnNano(&curTimer);
    
    std::cout << curTimer.year << "-" << curTimer.mon << "-" << curTimer.day << " " << curTimer.hour << ":" << curTimer.min
        << ":" << curTimer.sec << " " << curTimer.mill << " " << curTimer.micro << " " << curTimer.nano << std::endl;
}
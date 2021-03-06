#include <Windows.h>

#include <stdio.h>
#include <time.h>

int main()
{
    struct timeval tmval;
    int i;
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
        
    for (i = 0; i < 4; ++i)
    {
        

        GetLocalTime(&wtm);
        tm.tm_year     = wtm.wYear - 1900;
        tm.tm_mon     = wtm.wMonth - 1;
        tm.tm_mday     = wtm.wDay;
        tm.tm_hour     = wtm.wHour;
        tm.tm_min     = wtm.wMinute;
        tm.tm_sec     = wtm.wSecond;
        tm. tm_isdst    = -1;
        clock = mktime(&tm);
        tmval.tv_sec = clock;
        tmval.tv_usec = wtm.wMilliseconds * 1000;
        
        printf("%d\t%d\n", tmval.tv_usec, tmval.tv_sec);
        Sleep(1);
    }
    
    return 0;
}
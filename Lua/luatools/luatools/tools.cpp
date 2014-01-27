#include "tools.h"
#include "timer.h"
#include "luacode.h"

void InitTools()
{
    InitTimer();
}

//获取当前时间，精确到微秒
//@return {year, mon, day, hour, min, sec, mill, micro}
LIB_API GetTimeOnMicrosecond(lua_State* L)
{
    TimerNano timer;
    GetTimeOnNano(&timer);

    PINT(L, timer.year);
    PINT(L, timer.mon);
    PINT(L, timer.day);
    PINT(L, timer.hour);
    PINT(L, timer.min);
    PINT(L, timer.sec);
    PINT(L, timer.mill);
    PINT(L, timer.micro);

    return 8;
}


LIB_API GetCurTimerFlag(lua_State* L)
{
    double flag = GetCurrentTimerFlag();
    PN(L, flag);
    return 1;
}


LIB_API GetCurThreadId(lua_State* L)
{
    DWORD id = GetCurrentThreadId();
    PINT(L, id);
    return 1;
}

LIB_API CurrentThreadSleep(lua_State* L)
{
    int timer = PAI(L, 1);
    Sleep(timer);

    return 0;
}
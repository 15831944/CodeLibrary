#ifndef _EXPORT_H
#define _EXPORT_H

#include "luaunit.h"
#include "Timer.h"

CACHE_LIB_API GetTimeFlag(lua_State* L)
{
    double flag = GetCurrentTimerFlag();
    PN(L, flag);
    return 1;
}

#endif
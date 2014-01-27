/**********************************************************************
* cache库对外提供的公共方法 entrust
* Auth: ganlong
* Build: 2013.08.06
**********************************************************************/
#ifndef _TOOLS_H
#define _TOOLS_H

#pragma once

#include "luautil.h"
#include "base.h"

void InitTools();

LIB_API GetTimeOnMicrosecond(lua_State* L);

LIB_API GetCurTimerFlag(lua_State* L);

LIB_API GetCurThreadId(lua_State* L);

LIB_API CurrentThreadSleep(lua_State* L);

#endif
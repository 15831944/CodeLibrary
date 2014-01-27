                            /**********************************************************************
* 封装与Lua交互的相关头文件和类型
* Auth: ganlong
* Build: 2013.07.23
**********************************************************************/
#ifndef _LUA_UTIL_H
#define _LUA_UTIL_H

#pragma once

#include <lua.hpp>
#include "base.h"

#define LIB_API extern "C" int

#define PAI(L, n) (long)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //取整数的参数

#define PAC(L, n) (char)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //取单字符的参数

#define PAS(L, n) (lua_isstring(L, n) ? luaL_checkstring((L), (n)) : "")            //取字符串参数

#define PASL(L, n) (lua_isstring(L, n) ? lua_strlen(L, n) : 0)              //取字符串的长度

#define PAD(L, n) (double)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0.0)          //取浮点型参数

#define CS(f, L, n) memcpy(f, PAS(L, n), MIN(sizeof(f), PASL(L, n)))

#define PN(L, f) lua_pushnumber(L, f)    

#define PINT(L, f) lua_pushinteger(L, f)

#define PS(L, f) lua_pushlstring(L, f, MIN(sizeof(f), strlen(f)))

#define PLS(L, f, n) lua_pushlstring(L, f, n)

#endif
#ifndef _CACHE_LUAUTIL_H
#define _CACHE_LUAUTIL_H

#pragma once

#include <lua.hpp>

#define CACHE_LIB_API extern "C" int

#define PAI(L, n) (long)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //取整数的参数

#define PAC(L, n) (char)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //取单字符的参数

#define PAS(L, n) (lua_isstring(L, n) ? luaL_checkstring((L), (n)) : "")            //取字符串参数

#define PASL(L, n) (lua_isstring(L, n) ? lua_strlen(L, n) : 0)              //取字符串的长度

#define PAD(L, n) (double)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0.0)          //取浮点型参数

#define CS(f, L, n) memcpy(f, PAS(L, n), MIN(sizeof(f), PASL(L, n)))

#define PN(L, f) lua_pushnumber(L, f)

#define PS(L, f) lua_pushlstring(L, f, sizeof(f))

#define PLS(L, f, n) lua_pushlstring(L, f, n)

#endif
                            /**********************************************************************
* ��װ��Lua���������ͷ�ļ�������
* Auth: ganlong
* Build: 2013.07.23
**********************************************************************/
#ifndef _LUA_UTIL_H
#define _LUA_UTIL_H

#pragma once

#include <lua.hpp>
#include "base.h"

#define LIB_API extern "C" int

#define PAI(L, n) (long)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //ȡ�����Ĳ���

#define PAC(L, n) (char)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //ȡ���ַ��Ĳ���

#define PAS(L, n) (lua_isstring(L, n) ? luaL_checkstring((L), (n)) : "")            //ȡ�ַ�������

#define PASL(L, n) (lua_isstring(L, n) ? lua_strlen(L, n) : 0)              //ȡ�ַ����ĳ���

#define PAD(L, n) (double)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0.0)          //ȡ�����Ͳ���

#define CS(f, L, n) memcpy(f, PAS(L, n), MIN(sizeof(f), PASL(L, n)))

#define PN(L, f) lua_pushnumber(L, f)    

#define PINT(L, f) lua_pushinteger(L, f)

#define PS(L, f) lua_pushlstring(L, f, MIN(sizeof(f), strlen(f)))

#define PLS(L, f, n) lua_pushlstring(L, f, n)

#endif
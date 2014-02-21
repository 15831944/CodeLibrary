#ifndef _LUAUTIL_H
#define _LUAUTIL_H

#define LUA_LIB_API extern "C" int

#ifndef PAI
#define PAI(L, n) (long)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //取整数的参数
#endif

#ifndef PAC
#define PAC(L, n) (char)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0)           //取单字符的参数
#endif

#ifndef PAS
#define PAS(L, n) (lua_isstring(L, n) ? luaL_checkstring((L), (n)) : "")            //取字符串参数
#endif

#ifndef PASL
#define PASL(L, n) (lua_isstring(L, n) ? lua_strlen(L, n) : 0)              //取字符串的长度
#endif

#ifndef PAD
#define PAD(L, n) (double)(lua_isnumber(L, n) ? luaL_checknumber((L), (n)) : 0.0)          //取浮点型参数
#endif

#ifndef CS
#define CS(f, L, n) memcpy(f, PAS(L, n), MIN(sizeof(f), PASL(L, n))); f[sizeof(f) - 1] = 0
#endif

#ifndef PN
#define PN(L, f) lua_pushnumber(L, f)    
#endif

#ifndef PINT
#define PINT(L, f) lua_pushinteger(L, f)
#endif

#ifndef PS
#define PS(L, f) lua_pushlstring(L, f, MIN(sizeof(f), strlen(f)))
#endif

#ifndef PLS
#define PLS(L, f, n) lua_pushlstring(L, f, n)
#endif

#endif
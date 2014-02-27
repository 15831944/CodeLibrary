#ifndef _AM_TOOLS_H
#define _AM_TOOLS_H

#include "luautil/luna.h"
#include "luautil/luautil.h"
#include "util/serialgen.h"

void InitTools();

LUA_LIB_API getBaseDataVersionNumber(lua_State* L);

LUA_LIB_API setBaseDataVersionNumber(lua_State* L);

LUA_LIB_API incBaseDataVersionNumber(lua_State* L);

#endif
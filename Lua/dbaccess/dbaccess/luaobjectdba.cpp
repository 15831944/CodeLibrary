#include <baratol.h>
#include "luaobjectdba.h"

#ifdef _SUPPORT_ADOLIB
const char CLuaAdoLibDBA::className[] = "LuaAdoDBA";
Lunar<CLuaAdoLibDBA>::RegType CLuaAdoLibDBA::methods[] = 
{
	method(CLuaAdoLibDBA, funcproxy),
	method(CLuaAdoLibDBA, open),
	method(CLuaAdoLibDBA, isopen),
	method(CLuaAdoLibDBA, close),
	method(CLuaAdoLibDBA, exec),
	method(CLuaAdoLibDBA, exec_cb),
	method(CLuaAdoLibDBA, exec_cb_by_row),
	method(CLuaAdoLibDBA, begin_transaction),
	method(CLuaAdoLibDBA, commit_transaction),
	method(CLuaAdoLibDBA, rollback_transaction),
	{0, 0}
};
#endif

int  RegistDba(lua_State* L)
{
	#ifdef _SUPPORT_ADOLIB
		Lunar<CLuaAdoLibDBA>::Register(L);
	#endif
	return 0;
}
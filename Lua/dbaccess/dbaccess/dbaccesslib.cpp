#include <lua/luna.h>
#include <baratol.h>
#include "inc/commdef.h"

#include "luaobjectdba.h"

extern "C"
{	
	HEXIN_LUA_API int luaopen_dbaccess(lua_State* L)
	{
		InitAdoManager();
		RegistDba(L);

		return 0;
	}
}
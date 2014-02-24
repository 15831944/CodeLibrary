#include "luautil/luautil.h"
#include "luaobject.h"
#include "unitdata.h"

extern "C"
{    
    __declspec(dllexport) int luaopen_amstore(lua_State* L)
    {
        RegistLuaObject(L);
        RegistUnitBase(L);
        return 0;
    }
}
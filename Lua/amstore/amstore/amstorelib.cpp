#include "luautil/luautil.h"
#include "luaobject.h"

extern "C"
{    
    __declspec(dllexport) int luaopen_amstore(lua_State* L)
    {
        RegistLuaObject(L);
        return 0;
    }
}
#include "luautil/luautil.h"
#include "luaobject.h"
#include "unitdata.h"
#include "tools.h"

extern "C"
{    

    static luaL_Reg mylibs[] = { 
        {"GetUnitBaseByOperId", GetUnitBaseByOperId},
        {"GetUnitBaseByOperName", GetUnitBaseByOperName},
        {"GetFastUnitBaseByOperName", GetFastUnitBaseByOperName},

        {"getBaseDataVersionNumber", getBaseDataVersionNumber},
        {"setBaseDataVersionNumber", setBaseDataVersionNumber},
        {"incBaseDataVersionNumber", incBaseDataVersionNumber},

        {0, 0}
    };

    __declspec(dllexport) int luaopen_amstore(lua_State* L)
    {
        RegistLuaObject(L);
        RegistUnitBase(L);

        luaL_register(L, "amstore", mylibs);

        return 1;
    }
}
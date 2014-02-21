#include "luaobject.h"

const char LuaObjDemo::className[] = "LuaObjDemo";
Lunar<LuaObjDemo>::RegType LuaObjDemo::methods[] = 
{
    method(LuaObjDemo, funcproxy),
    method(LuaObjDemo, get_count),
    {0, 0}
};


int RegistLuaObject(lua_State* L)
{
    Lunar<LuaObjDemo>::Register(L);

    return 0;
}
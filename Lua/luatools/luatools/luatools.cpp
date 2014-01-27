#include <iostream>
#include "luautil.h"

static luaL_Reg mylibs[] = {

    {NULL, NULL}
}; 

extern "C" __declspec(dllexport)
    int luaopen_luatools(lua_State* L) 
{
    luaL_register(L, "luatools", mylibs);
    return 1;
}
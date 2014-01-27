#include <iostream>
#include <lua.hpp>

#include "boot2.h"
#include "boot1.h"

static luaL_Reg mylibs[] = {
    {NULL, NULL}
}; 

extern "C" __declspec(dllexport)
    int luaopen_dllboot(lua_State* L) 
{
    std::cout << "luaopen_dllboot" << std::endl;
    luaL_register(L, "dllboot", mylibs);
    return 1;
}
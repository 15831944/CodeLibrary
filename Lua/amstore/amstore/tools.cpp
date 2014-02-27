#include "tools.h"

SerialGenerater serialGenBDVN;

void InitTools()
{

}

LUA_LIB_API getBaseDataVersionNumber(lua_State* L)
{
    PINT(L, serialGenBDVN.GetCurSerial());
    return 1;
}

LUA_LIB_API setBaseDataVersionNumber(lua_State* L)
{
    int i = 0;
    i = PAI(L, 1);
    serialGenBDVN.InitSerial(i);

    PINT(L, 0);
    return 1;
}

LUA_LIB_API incBaseDataVersionNumber(lua_State* L)
{
    PINT(L, serialGenBDVN.Generate());
    return 1;
}
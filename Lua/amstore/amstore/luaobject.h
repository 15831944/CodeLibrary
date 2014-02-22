#ifndef _LUAOBJECT_H
#define _LUAOBJECT_H

#include <iostream>
#include "luautil/luna.h"
#include "luautil/luautil.h"
#include "sys/typedef.h"

int RegistLuaObject(lua_State* L);

using namespace std;

class LuaObjDemo
{
public:
    static const char className[];
    typedef Lunar<LuaObjDemo>::RegType RegType;
    static RegType methods[];
    //MakeFuncProxy();
public:
    LuaObjDemo(): counter(0){
        cout << "LuaObjDemo construct" << endl;
    }
    LuaObjDemo(lua_State* L): counter(0){
        cout << "LuaObjDemo construct lua" << endl;
    }
    virtual ~LuaObjDemo(){
        cout << "LuaObjDemo destruct" << endl;
    }
    static LuaObjDemo* Create(lua_State* L){
        cout << "LuaObjDemo::Create" << endl;
        return new LuaObjDemo(L);
    }

    int get_count(lua_State* L){
        PN(L, ++counter);
        return 1;
    }
private:
    int32_t counter;
};

#endif
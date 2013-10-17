/**********************************************************************
* cache library 导出函数
* Auth: ganlong
* Build: 2013.07.23
**********************************************************************/
#include "Luaunit.h"
#include "Export.h"


//luaL_Reg结构体的第一个字段为字符串，在注册时用于通知Lua该函数的名字。
//第一个字段为C函数指针。
//结构体数组中的最后一个元素的两个字段均为NULL，用于提示Lua注册函数已经到达数组的末尾。
static luaL_Reg mylibs[] = { 
    {"GetTimeFlag", GetTimeFlag},

    {NULL, NULL}                                            
}; 


//该C库的唯一入口函数。其函数签名等同于上面的注册函数。见如下几点说明：
//1. 我们可以将该函数简单的理解为模块的工厂函数。
//2. 其函数名必须为luaopen_xxx，其中xxx表示library名称。Lua代码require "xxx"需要与之对应。
//3. 在luaL_register的调用中，其第一个字符串参数为模块名"xxx"，第二个参数为待注册函数的数组。
//4. 需要强调的是，所有需要用到"xxx"的代码，不论C还是Lua，都必须保持一致，这是Lua的约定，
//   否则将无法调用。
extern "C" __declspec(dllexport)
    int luaopen_TimerLib(lua_State* L) 
{
    const char* libName = "TimerLib";
    luaL_register(L, libName, mylibs);
    return 1;
}
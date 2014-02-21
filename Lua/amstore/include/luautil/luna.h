
#ifndef __luna_h__
#define __luna_h__

#include "lua.hpp"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <string>

#include <iostream>
using namespace std;

#ifndef _WIN32
#define _snprintf snprintf
#endif

template <typename T> class Lunar {
    typedef struct { T *pT; } userdataType;
public:
    typedef int (T::*mfp)(lua_State *L);
    typedef struct { const char *name; mfp mfunc; } RegType;

    static void Register(lua_State *L) {
        //���Ѿ��д�mematable����ֱ���˳�
        luaL_getmetatable (L, T::className);
        if(!lua_isnil(L, -1))
        {
            return;
        }

        lua_newtable(L);
        int methods = lua_gettop(L);

        //methods
        //��T::className��ע�������metatable����
        luaL_newmetatable(L, T::className);
        int metatable = lua_gettop(L);

        //methods,metatable
        //��T::className��LUA_GLOBALSINDEX����methods����
        //Lua5.2��û��LUA_GLOBALSINDEX
        // store method table in globals so that
        // scripts can add functions written in Lua.
        lua_pushvalue(L, methods);
        //methods,metatable,methods
        set(L, LUA_GLOBALSINDEX, T::className);

        //methods,metatable
        // hide metatable from Lua getmetatable()
        lua_pushvalue(L, methods);
        //methods,metatable,methods
        //����metatable.__metatable = methods
        set(L, metatable, "__metatable");

        lua_pushvalue(L, methods);
        //����metatable.__index = methods
        set(L, metatable, "__index");

        lua_pushcfunction(L, tostring_T);
        //����metatable.__tostring = tostring_T
        set(L, metatable, "__tostring");

        lua_pushcfunction(L, gc_T);
        //����metatable.__gc = gc_T
        set(L, metatable, "__gc");

        //methods,metatable
        lua_newtable(L);                // mt for method table

        //methods,metatable,mt
        lua_pushcfunction(L, new_T);

        //methods,metatable,mt,new_T
        lua_pushvalue(L, -1);           // dup new_T function

        //methods,metatable,mt,new_T,new_T
        //����methods.new = new_T
        set(L, methods, "new");         // add new_T to method table

        //methods,metatable,mt,new_T
        //����methods.__call = new_T
        set(L, -3, "__call");           // mt.__call = new_T

        //methods,metatable,mt
        //����methods��metatableΪmt
        lua_setmetatable(L, methods); 
        //methods,metatable

        // fill method table with methods from class T
        //��T::methods���õ�methods��
        for (RegType *l = T::methods; l->name; l++) {
            lua_pushstring(L, l->name);
            lua_pushlightuserdata(L, (void*)l);
            lua_pushcclosure(L, thunk, 1);
            lua_settable(L, methods);
        }

        //methods,metatable
        lua_pop(L, 2);  // drop metatable and method table
    }

    // push onto the Lua stack a userdata containing a pointer to T object
    static int push(lua_State *L, T *obj, bool gc=false) {
        if (!obj) { lua_pushnil(L); return 0; }
        luaL_getmetatable(L, T::className);  // lookup metatable in Lua registry
        //mt
        if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", T::className);
        cout << "push top 1=" << lua_gettop(L) << endl;
        //mt
        int mt = lua_gettop(L);
        cout << "push top 2=" << lua_gettop(L) << endl;
        subtable(L, mt, "userdata", "v");
        //mt,table
        cout << "push top 3=" << lua_gettop(L) << endl;
        //mt,table
        userdataType *ud =
            static_cast<userdataType*>(pushuserdata(L, obj, sizeof(userdataType)));
        //mt,table(userdata),ud
        cout << "push top 4=" << lua_gettop(L) << endl;
        if (ud) {
            cout << "push ud is not nil" << endl;
            ud->pT = obj;  // store pointer to object in userdata
            lua_pushvalue(L, mt);
            //mt,table(userdata),ud,mt
            cout << "push top 5=" << lua_gettop(L) << endl;
            lua_setmetatable(L, -2);
            //mt,table(userdata),ud
            cout << "push top 5.1=" << lua_gettop(L) << endl;
            if (gc == false) {
                cout << "push gc is false" << endl;
                lua_checkstack(L, 3);
                subtable(L, mt, "do not trash", "k");
                lua_pushvalue(L, -2);
                lua_pushboolean(L, 1);
                lua_settable(L, -3);
                lua_pop(L, 1);
            }
        }
        cout << "push top 6=" << lua_gettop(L) << endl;
        lua_replace(L, mt);
        //ud,table(userdata)
        cout << "push top 7=" << lua_gettop(L) << endl;
        lua_settop(L, mt);
        //ud
        cout << "push top 8=" << lua_gettop(L) << endl;
        return mt;  // index of userdata containing pointer to T object
    }

    // get userdata from Lua stack and return pointer to T object
    static T *check(lua_State *L, int narg) {
        if(lua_isnil(L, narg))
            return NULL;
        userdataType *ud =
            static_cast<userdataType*>(luaL_checkudata(L, narg, T::className));
        //if(!ud) luaL_typerror(L, narg, T::className);
        if(!ud) return NULL;
        return ud->pT;  // pointer to T object
    }

private:
    Lunar();  // hide default constructor

    // member function dispatcher
    static int thunk(lua_State *L) {
        // stack has userdata, followed by method args
        T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
        lua_remove(L, 1);  // remove self so member function args start at index 1
        // get member function from upvalue
        RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
        return (obj->*(l->mfunc))(L);  // call member function
    }

    // create a new T object and
    // push onto the Lua stack a userdata containing a pointer to T object
    static int new_T(lua_State *L) {
        lua_remove(L, 1);   // use classname:new(), instead of classname.new()
        T *obj = new T(L);  // call constructor for T objects
        cout << "new_T top 1=" << lua_gettop(L) << endl;
        push(L, obj, true); // gc_T will delete this object
        cout << "new_T top 2=" << lua_gettop(L) << endl;
        return 1;           // userdata containing pointer to T object
    }

    // garbage collection metamethod
    static int gc_T(lua_State *L) {
        if (luaL_getmetafield(L, 1, "do not trash")) {
            lua_pushvalue(L, 1);  // dup userdata
            lua_gettable(L, -2);
            if (!lua_isnil(L, -1)) return 0;  // do not delete object
        }
        userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
        T *obj = ud->pT;
        if (obj) delete obj;  // call destructor for T objects
        return 0;
    }

    static int tostring_T (lua_State *L) {
        char buff[32];
        userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
        T *obj = ud->pT;
        _snprintf(buff, sizeof(buff), "%p", obj);
        lua_pushfstring(L, "%s (%s)", T::className, buff);
        return 1;
    }

    static void set(lua_State *L, int table_index, const char *key) {
        lua_pushstring(L, key);
        lua_insert(L, -2);  // swap value and key
        lua_settable(L, table_index);
    }

    static void weaktable(lua_State *L, const char *mode) {
        //mt
        lua_newtable(L);
        //mt,table
        cout << "weaktable top 1.00=" << lua_gettop(L) << endl;
        lua_pushvalue(L, -1);  // table is its own metatable
        cout << "weaktable top 1.0=" << lua_gettop(L) << endl;
        //mt,table,table
        //����table��metatableΪ���Լ�
        lua_setmetatable(L, -2);
        cout << "weaktable top 1.1=" << lua_gettop(L) << endl;
        //mt,table
        lua_pushliteral(L, "__mode");
        //mt,table,"__mode"
        lua_pushstring(L, mode);
        //mt,table,"__mode",mode
        lua_settable(L, -3);   // metatable.__mode = mode
        cout << "weaktable top 2=" << lua_gettop(L) << endl;
    }

    //subtable(L, mt, "userdata", "v");
    //subtable(L, mt, "do not trash", "k");
    static void subtable(lua_State *L, int tindex, const char *name, const char *mode) {
        //mt
        lua_pushstring(L, name);
        cout << "subtable top 1=" << lua_gettop(L) << endl;
        //mt,name
        lua_gettable(L, tindex);
        //mt,mt.name
        cout << "subtable top 2=" << lua_gettop(L) << endl;
        if (lua_isnil(L, -1)) {
            cout << "subtable is nil" << endl;
            lua_pop(L, 1);
            //mt
            lua_checkstack(L, 3);
            weaktable(L, mode);
            //mt,table
            cout << "subtable top 3=" << lua_gettop(L) << ",tindex=" << tindex << endl;
            lua_pushstring(L, name);
            //mt,table,name
            lua_pushvalue(L, -2);
            //mt,table,name,table
            lua_settable(L, tindex);
            //mt,table
            cout << "subtable top 4=" << lua_gettop(L) << endl;
        }
    }

    static void *pushuserdata(lua_State *L, void *key, size_t sz) {
        //mt,table(userdata)
        void *ud = 0;
        cout << "pushuserdata top 0.2=" << lua_gettop(L) << ",key=" << key << endl;
        lua_pushlightuserdata(L, key);
        cout << "pushuserdata top 0.1=" << lua_gettop(L) << endl;
        //mt,table(userdata),key
        lua_gettable(L, -2);     // lookup[key]
        //mt,table,table.key
        cout << "pushuserdata top 1.00=" << lua_gettop(L) << endl;
        if (lua_isnil(L, -1)) {
            cout << "pushuserdata is nil" << endl;
            lua_pop(L, 1);         // drop nil
            //mt,table
            lua_checkstack(L, 3);
            ud = lua_newuserdata(L, sz);  // create new userdata
            cout << "pushuserdata top 1.0=" << lua_gettop(L) << endl;
            //mt,table(userdata),ud
            lua_pushlightuserdata(L, key);
            //mt,table(userdata),ud,key
            cout << "pushuserdata top 1.1=" << lua_gettop(L) << endl;
            lua_pushvalue(L, -2);  // dup userdata
            //mt,table(userdata),ud,key,ud
            cout << "pushuserdata top 1.2=" << lua_gettop(L) << endl;
            lua_settable(L, -4);   // lookup[key] = userdata
            //mt,table(userdata),ud
            cout << "pushuserdata top 2=" << lua_gettop(L) << endl;
        }
        return ud;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////
#define method(class, name) {#name, &class::name}

#define methodL(class, name) {#name, &class::name##L}


//����lua�����ӿ�
#define DECLARE_INTER(name) \
    virtual int name(lua_State* L) {return 0;}

//������Ҫʵ�ֵ����ƺͺ����б����
#define DECLARE_LUA_EXPORT(classname) \
    typedef Lunar<classname>::RegType RegType; \
    static const char className[];    \
    static RegType methods[];    

#define PAI(L, n) (long)(lua_isnumber(L, n) \
    ? luaL_checknumber((L), (n)) : 0)    //ȡ�����Ĳ���

#define PAS(L, n) (lua_isstring(L, n) \
    ? luaL_checkstring((L), (n)) : "")        //ȡ�ַ�������

#define PASL(L, n) (lua_isstring(L, n) \
    ? lua_strlen(L, n) : 0)        //ȡ�ַ����ĳ���

#define PAD(L, n) (double)(lua_isnumber(L, n) \
    ? luaL_checknumber((L), (n)) : 0.0f) //ȡ�����Ͳ���



//����lua���л�����ջ�ĸ�������ֹ
//������������
class  Lua_SaveState
{
public:
    Lua_SaveState(lua_State* L, int nNeedDels = 0)
        :m_L(L),top(0)
    {
        top = lua_gettop(m_L) - nNeedDels;
    }
    ~Lua_SaveState()
    {
        lua_settop(m_L, top);
    }
protected:
    lua_State* m_L;
    int        top;
};

//�Զ����ɰ�װ��ĺ궨��
#define MakeRefLuaObject(refclass, objclass,  baseclass) \
class objclass : public baseclass, public EnableRefPtr<objclass> \
{ \
public:\
    objclass* get_this(){return this;}\
};\
    typedef CTLRefPtr<objclass> refclass;

//������֧�����ö����װ�ĺ궨��
#define MakeFuncProxy() \
    int funcproxy(lua_State* L) {\
    std::string strName = PAS(L, 1);\
    lua_remove(L, 1);\
    for(RegType *l = methods; l->name; l++) {\
    if(strName == l->name){\
    return (this->*(l->mfunc))(L);  \
    }\
    }\
    return 0;\
}
//��������֧�������޸�
/*
class CTEST
{
public:
typedef Lunar<CLuaHexinPacket>::RegType RegType;    //�̶�����
MakeFuncProxy();    //���庯�����ô�����
};
methods������
method(CTEST, funcproxy),
*/

/////////////////////////////////////////////////////////////////////////////////////////////////
//lua�ű�������Ϣ��ջ��Ϣ���
/*
*
*/
#define LEVELS1    12    /* size of the first part of the stack */
#define LEVELS2    10    /* size of the second part of the stack */

inline int errormessage(lua_State *L)
{
    int level = 0;  /* skip level 0 (it's this function) */
    int firstpart = 1;  /* still before eventual `...' */    
    lua_Debug ar;
    if (!lua_isstring(L, 1))
        return lua_gettop(L);
    lua_settop(L, 1);
    lua_pushliteral(L, "\r\n");
    lua_pushliteral(L, "���ö�ջ:\r\n");
    while (lua_getstack(L, level++, &ar)) 
    {
        char buff[10];
        if (level > LEVELS1 && firstpart) 
        {
            /* no more than `LEVELS2' more levels? */
            if (!lua_getstack(L, level+LEVELS2, &ar))
                level--;  /* keep going */
            else 
            {
                lua_pushliteral(L, "       ...\r\n");  /* too many levels */
                while (lua_getstack(L, level+LEVELS2, &ar))  /* find last levels */
                    level++;                
            }
            firstpart = 0;
            continue;
        }

#if 1
        sprintf_s(buff, sizeof(buff),  "%4d-  ", level-1);
#else
        _snprintf(buff, sizeof(buff),  "%4d-  ", level-1);
#endif

        lua_pushstring(L, buff);
        lua_getinfo(L, "Snl", &ar);
        lua_pushfstring(L, "%s:", ar.short_src);
        if (ar.currentline > 0)
            lua_pushfstring(L, "%d:", ar.currentline);
        switch (*ar.namewhat) 
        {
        case 'g':  /* global */ 
        case 'l':  /* local */
        case 'f':  /* field */
        case 'm':  /* method */
            lua_pushfstring(L, " �ں��� `%s'", ar.name);
            break;
        default: 
            {
                if (*ar.what == 'm')  /* main? */
                    lua_pushfstring(L, " in main chunk");
                else if (*ar.what == 'C')  /* C function? */
                    lua_pushfstring(L, "%s", ar.short_src);
                else
                    lua_pushfstring(L, " �ں��� <%s:%d>", ar.short_src, ar.linedefined);
            }

        }
        lua_pushliteral(L, "\r\n");
        lua_concat(L, lua_gettop(L));
    }
    lua_concat(L, lua_gettop(L));     

    time_t t = time(NULL);
    struct tm* t2 = localtime(&t);

    char szTime[128];
    _snprintf(szTime, sizeof(szTime), 
        "%04d%02d%02d %02d:%02d:%02d\r\n", 
        t2->tm_year + 1900, t2->tm_mon + 1, t2->tm_mday, t2->tm_hour, t2->tm_min, t2->tm_sec);

    FILE* pf = fopen("scripterr.log", "a+b");
    if(pf)
    {
        fwrite(szTime, 1, strlen(szTime), pf);
        fwrite(lua_tostring(L, -1), 1, lua_strlen(L, -1), pf);
        fclose(pf);
    }
    return 0;
}

#endif


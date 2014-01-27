#ifndef _LUA_OBJECT_DBA_
#define _LUA_OBJECT_DBA_

#include "hexinpacket.h"
#include "lua/luna.h"
#include "hdfile.h"
#pragma pack(1)
using namespace baratol;

int  RegistDba(lua_State* L);

#ifdef _SUPPORT_ADOLIB
#include "adolib/adolib.h"
#include "luaobject.h"
#include "adomanager.h"

class CLuaAdoLibDBA
{
public:
	static const char className[];				//类名
	typedef Lunar<CLuaAdoLibDBA>::RegType RegType;
	static Lunar<CLuaAdoLibDBA>::RegType methods[];	//需要导出的函数
	MakeFuncProxy();
protected:
	long m_lDBSession;
	AdoManager *pAdoMag;
public:
	CLuaAdoLibDBA()
	{
		m_lDBSession = 0;
		pAdoMag = &(AdoManager::GetInstance());
	}
	CLuaAdoLibDBA(lua_State* L)
	{
		m_lDBSession = 0;
		pAdoMag = &(AdoManager::GetInstance());
	}
	~CLuaAdoLibDBA()
	{
		pAdoMag->FreeItem(m_lDBSession);
	}

	int open(lua_State* L)
	{
		pAdoMag->FreeItem(m_lDBSession);
		AdoItem* item = pAdoMag->CreateItem(PAS(L, 1), PAI(L, 2), PAI(L, 3));
		m_lDBSession = (long)item;
		lua_pushboolean(L, m_lDBSession != 0);
		lua_pushstring(L, pAdoMag->GetSessionError(m_lDBSession));
		return 2;
	}

	int  isopen(lua_State* L)
	{
		lua_pushboolean(L, m_lDBSession != 0);
		return 1;
	}

	int  close(lua_State* L)
	{
		lua_pushboolean(L, pAdoMag->FreeItem(m_lDBSession));
		m_lDBSession = 0;
		return 1;
	}

	int  exec(lua_State* L)
	{
		if(m_lDBSession == 0)
		{
			lua_pushnil(L);
			lua_pushstring(L, "database is not connected");
			return 2;
		}

		CHDFile* pdata = new CHDFile;
		if(!pAdoMag->RunCmdHDFile(m_lDBSession, PAS(L, 1), *pdata))
		{
			delete pdata;
			lua_pushnil(L);
			lua_pushstring(L, pAdoMag->GetSessionError(m_lDBSession));
			return 2;
		}

		CLuaHDFile* pluahdfile = new CLuaHDFile(pdata, true);
		Lunar<CLuaHDFile>::push(L, pluahdfile, true);
		lua_pushstring(L, pAdoMag->GetSessionError(m_lDBSession));
		return 2;
	}

	static void Callback(void* pUserData, int nType, const char* pszTabname, int nIndex, const char* pszColname, CTLValue* pValue)
	{		
		AdoLuaCallbackStr* pCB = (AdoLuaCallbackStr *)pUserData;
		lua_State* L = pCB->m_L;
		int   idx = pCB->m_idx;
		if(idx == 0)
			return;

		lua_pushvalue(L,  idx);
		lua_pushnumber(L, nType);
		lua_pushstring(L, pszTabname);
		lua_pushnumber(L, nIndex);
		lua_pushstring(L, pszColname ? pszColname : "");
		if(pValue)
		{
			pValue->SetOutputFormat(TF_PRE_MASK, TF_PRE_MASK);

			if(pValue->IsString())
				lua_pushstring(L, (LPCTSTR)(pValue ? pValue->GetString() : ""));
			else
				lua_pushnumber(L, pValue->GetDouble());
		}
		else
			lua_pushstring(L, "");

		lua_pcall(L, 5, -1, 0);
		return;

	}
	int exec_cb(lua_State* L)
	{
		AdoLuaCallbackStr cb(L, 2);
		if(m_lDBSession == 0)
		{
			lua_pushboolean(L, 0);
			lua_pushstring(L, "database is not connected");
			return 2;
		}
		int32 nAffect = 0;
		BOOL bRtn = pAdoMag->RunCmdCallback(m_lDBSession, PAS(L, 1), Callback, &cb, &nAffect);
		lua_pushboolean(L, bRtn);		
		lua_pushstring(L, pAdoMag->GetSessionError(m_lDBSession));
		lua_pushnumber(L, nAffect);
		return 3;
	}

	static void CallbackByRow(void* pUserData, int nType, const char* pszTabname, int nIndex, const char* pszColname, CTLValue* pValue)
	{
		AdoLuaCallbackStr* pCB = (AdoLuaCallbackStr *)pUserData;
		lua_State* L = pCB->m_L;
		int   idx = pCB->m_idx;
		if(idx == 0)
			return;

		if(nType == 2)
		{
			lua_newtable(L);
			return;
		}
		else if(nType == 3)
		{
			if(pValue)
			{
				if(pValue->IsNull())
				{
					lua_pushstring(L, "");
				}
				else if(pValue->IsString())
					lua_pushstring(L, (LPCTSTR)(pValue ? pValue->GetString() : ""));
				else
					lua_pushnumber(L, pValue->GetDouble());
			}
			else
				lua_pushstring(L, "");

			lua_setfield(L, -2, pszColname);
			return;
		}
		else if(nType == 4)
		{
			lua_pushvalue(L,  idx);		
			lua_pushstring(L, pszTabname);
			lua_pushnumber(L, nIndex);
			lua_pushvalue(L, -4);
			lua_pcall(L, 3, -1, 0);
			lua_pop(L, 1);
			return;
		}
		return;	
	}

	int exec_cb_by_row(lua_State* L)
	{
		AdoLuaCallbackStr cb(L, 2);

		long lsession = PAI(L,3);
        if (lsession==0)lsession = pAdoMag->GetIdleConnection();

		if(lsession == 0)
		{
			lua_pushboolean(L, 0);
			lua_pushstring(L, "database is not connected");
			return 2;
		}
		int32 nAffect = 0;
		BOOL bRtn = pAdoMag->RunCmdCallback(lsession, PAS(L, 1), CallbackByRow, &cb, &nAffect);
		lua_pushboolean(L, bRtn);		
		lua_pushstring(L, pAdoMag->GetSessionError(lsession));
		lua_pushnumber(L, nAffect);
		return 3;
	}

	int begin_transaction(lua_State* L)
	{
		TAdoItemId itemId = pAdoMag->BeginTransaction();
		lua_pushnumber(L, itemId);
		return 1;
	}

	int commit_transaction(lua_State* L)
	{
		TAdoItemId itemId = PAI(L, 1);
		int iRes = pAdoMag->CommitTransaction(itemId);
		lua_pushnumber(L, iRes);
		return 1;
	}

	int rollback_transaction(lua_State* L)
	{
		TAdoItemId itemId = PAI(L, 1);
		int iRes = pAdoMag->RollbackTransaction(itemId);
		lua_pushnumber(L, iRes);
		return 1;
	}
};

#endif
#pragma pack()

#endif
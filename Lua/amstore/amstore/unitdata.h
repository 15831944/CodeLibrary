#ifndef _AM_UNITDATA_H
#define _AM_UNITDATA_H

#include <list>
#include <hash_map>
#include <string>
#include "luautil/luna.h"
#include "luautil/luautil.h"
#include "util/serialgen.h"
#include "sys/sync.h"
#include "config.h"
#include "typedef.h"
#include "loger.h"
#include "business.h"
#include "unitasset.h"

using std::list;
using std::hash_map;
using std::string;

class UnitData{
public:
    static const char className[];
    typedef Lunar<UnitData>::RegType RegType;
    static RegType methods[];
public:
    UnitData():unit_id(0){
    }
    UnitData(lua_State* L){
        new (this) UnitData();
    }
    virtual ~UnitData(){
    }

    //��Ҫȷ������֮�󣬶�Ӧ��UnitData�ĵ�ַ����仯����lua�лᱣ����userdate
    static UnitData* Create(lua_State* L){
        LOG(INFO) << "[UnitData::Create]start." << LOG_END_FLAG;
        UnitId_T id = PAI(L, 1);
        LOG(INFO) << "[UnitData::Create]id=" << id << LOG_END_FLAG;
        UnitData* pNew = GetByUnitId(id);
        if(!pNew){
            pNew = InitUnitData(id);
        }

        if(!pNew){
            LOG(ERROR) << "[UnitData::Create]end." << LOG_END_FLAG;
        }
        LOG(INFO) << "[UnitData::Create]end.pNew=" << pNew << LOG_END_FLAG;
        return pNew;
    }

    int AddUnitBaseData(lua_State* L);
    int GetUnitBaseData(lua_State* L);
    int AddFundBaseData(lua_State* L);
    int GetFundBaseData(lua_State* L);

    int NewTransaction(lua_State* L);
    int LockSaveInfo(lua_State* L);
    int UnlockSaveInfo(lua_State* L);

    int BusiInitData(lua_State* L);
    int BusiAddData(lua_State* L);
    int BusiGetAllData(lua_State* L);
    int BusiReleaseData(lua_State* L);
    int BusiRollbackData(lua_State* L);
    int BusiInitDbSaveInfo(lua_State* L);
    int BusiGetSaveInfo(lua_State* L);
    int BusiDoneSaveInfo(lua_State* L);

    int AssetInitData(lua_State* L);
    int AssetUpdateByEntrust(lua_State* L);
    int AssetUpdateByBusiness(lua_State* L);
    int AssetGetData(lua_State* L);
    int AssetReleaseData(lua_State* L);
    int AssetRollbackData(lua_State* L);
    int AssetInitDbSaveInfo(lua_State* L);
    int AssetGetSaveInfo(lua_State* L);
    int AssetDoneSaveInfo(lua_State* L);
private:
    //����unitid��ȡUnitData*�����޷���NULL
    static UnitData* GetByUnitId(const UnitId_T id);
    static UnitData* InitUnitData(const UnitId_T id);
    static UnitData* ConnectOperData(const OperatorId_T operId, const OperatorName_T operName, UnitData* pData);
    static UnitData* GetByOper(const OperatorId_T operId);
    static UnitData* GetByOper(const OperatorName_T operName);

    //unitid��UnitDataָ��Ĺ���
    static hash_map<UnitId_T, UnitData*> _id2Data;
    //OperatorId��UnitDataָ��Ĺ���
    static hash_map<OperatorId_T, UnitData*> _operId2Data;
    //OperatorName��UnitDataָ��Ĺ���
    static hash_map<string, UnitData*> _operName2Data;
    //UnitData���б��ڳ��������ڼ䣬�����Ԫ�ش���֮�󲻻ᱻ�ͷź�ɾ�����Ա����ⲿ���е���ָ���userdata��Ч
    static list<UnitData*> _DataList;
    static CReadWriteLock _rwLock;
private:
    int Init(UnitId_T unitId);
private:
    UnitId_T unit_id;
    UnitBaseData *pUnitBaseData;
    FundBaseData *pFundBaseData;
    SerialGenerater serialGenTran;
    BusinessData* pBusiData;
    UnitAssetData* pAssetDate;
    CCriticalSection criSectSI;

    friend int GetUnitBaseByOperId_F(lua_State* L);
    friend int GetUnitBaseByOperName_F(lua_State* L);
    friend int GetFastUnitBaseByOperName_F(lua_State* L);
};


LUA_LIB_API GetUnitBaseByOperId(lua_State* L);
LUA_LIB_API GetUnitBaseByOperName(lua_State* L);
LUA_LIB_API GetFastUnitBaseByOperName(lua_State* L);

int RegistUnitBase(lua_State* L);

#endif
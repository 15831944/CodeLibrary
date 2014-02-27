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
#include "entrust.h"
#include "unitstock.h"

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

    //需要确保创建之后，对应的UnitData的地址不会变化，因lua中会保存其userdate
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

    int EntInitData(lua_State* L);
    int EntAddData(lua_State* L);
    int EntGetEntByEntId(lua_State* L);
    int EntGetEntByRepId(lua_State* L);
    int EntGetAllData(lua_State* L);
    int EntGetDataByEntStatus(lua_State* L);
    int EntUpdateEntToRevokeInner(lua_State* L);
    int EntUpdateEntStatus(lua_State* L);
    int EntUpdateEntByRevOrder(lua_State* L);
    int EntUpdateEntByReport(lua_State* L);
    int EntUpdateRevByReport(lua_State* L);
    int EntUpdateEntByWaste(lua_State* L);
    int EntUpdateEntByRevoke(lua_State* L);
    int EntUpdateEntByRevokeWaste(lua_State* L);
    int EntUpdateRevByRevokeWaste(lua_State* L);
    int EntUpdateEntByBusiness(lua_State* L);
    int EntReleaseData(lua_State* L);
    int EntRollbackData(lua_State* L);
    int EntInitDbSaveInfo(lua_State* L);
    int EntGetSaveInfo(lua_State* L);
    int EntDoneSaveInfo(lua_State* L);

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

    int StockInitData(lua_State* L);
    int StockGetDataByCode(lua_State* L);
    int StockGetAllData(lua_State* L);
    int StockUpdateDataByEntrust(lua_State* L);
    int StockUpdateDataByBusiness(lua_State* L);
    int StockRollbackData(lua_State* L);
    int StockReleaseData(lua_State* L);
    int StockInitDbSaveInfo(lua_State* L);
    int StockGetSaveInfo(lua_State* L);
    int StockDoneSaveInfo(lua_State* L);

private:
    //根据unitid获取UnitData*，若无返回NULL
    static UnitData* GetByUnitId(const UnitId_T id);
    static UnitData* InitUnitData(const UnitId_T id);
    static UnitData* ConnectOperData(const OperatorId_T operId, const OperatorName_T operName, UnitData* pData);
    static UnitData* GetByOper(const OperatorId_T operId);
    static UnitData* GetByOper(const OperatorName_T operName);

    //unitid到UnitData指针的关联
    static hash_map<UnitId_T, UnitData*> _id2Data;
    //OperatorId到UnitData指针的关联
    static hash_map<OperatorId_T, UnitData*> _operId2Data;
    //OperatorName到UnitData指针的关联
    static hash_map<string, UnitData*> _operName2Data;
    //UnitData的列表，在程序运行期间，里面的元素创建之后不会被释放和删除，以保存外部持有的其指针或userdata有效
    static list<UnitData*> _DataList;
    static CReadWriteLock _rwLock;
private:
    int Init(UnitId_T unitId);
private:
    UnitId_T unit_id;
    UnitBaseData *pUnitBaseData;
    FundBaseData *pFundBaseData;
    SerialGenerater serialGenTran;
    EntrustData* pEntData;
    BusinessData* pBusiData;
    AssetData* pAssetData;
    StockData* pStockData;
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
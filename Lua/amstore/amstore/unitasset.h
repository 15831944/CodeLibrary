#ifndef _AM_UNITASSET_H
#define _AM_UNITASSET_H

#pragma once

#include <list>
#include <vector>
#include <algorithm>
#include "tradedate.h"
#include "luautil/luautil.h"
#include "sys/sync.h"
#include "typedef.h"
#include "luacode.h"
#include "loger.h"
#include "trandata.h"
#include "dbsaver.h"

using std::list;
using std::vector;
using std::find_if;

class AssetData : public TradeData{
public:
    AssetData()
    {
        pDbSav = new DBSaver(this);
    };
    virtual ~AssetData(){};
    virtual int Init()
    {
        return 0;
    };
public:
    UnitAsset* InitData(const UnitAsset* pData);
    UnitAsset* GetData(CurrencyCode_T curCode, UnitAsset* pData);
    int UpdateByEntrust(const UnitAsset* pData, const TransactionIdx_T tranIdx);
    int UpdateByBusiness(const UnitAsset* pData, const TransactionIdx_T tranIdx);
    int ReleaseData();
    int RollbackData(const TransactionIdx_T tranId);
public:
    virtual int InitSaveInfo(){return pDbSav->InitSaveInfo();};
    virtual int GetSaveInfo(std::list<DBWriteItem*>& pList){return pDbSav->GetSaveInfo(pList);};
    virtual int DoneSaveInfo(){return pDbSav->DoneSaveInfo();};
    virtual int GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update);
    virtual int DoneDbUpdata(std::list<DBWriteItem*>& pList);
    virtual bool Lock(int mill = 0){return criSect.Lock(mill);};
    virtual void Unlock(){criSect.Unlock();};
private:
    TranData<TranAsset, TRAN_DATA_COUNT> pTranData;
    CCriticalSection criSect;
    vector<UnitAsset*> pAssetList;
    DBSaver* pDbSav;
};

typedef struct AssetFunctorEqual
{
    AssetFunctorEqual(const CurrencyCode_T aData) : curCode(aData) { }; 
    bool operator()(UnitAsset *p)
    { 
        LOG(INFO) << "[AssetFunctorEqual]currency_code=" << p->currency_code << "," << curCode << LOG_END_FLAG;
        return strcmp(curCode.c_str(), p->currency_code) == 0; 
    };
    std::string curCode;
}AssetFunctorEqual;

#endif
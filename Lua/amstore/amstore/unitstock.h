#ifndef _AM_UNITSTOCK_H
#define _AM_UNITSTOCK_H

#include <list>
#include <hash_map>
#include <algorithm>
#include "tradedate.h"
#include "luautil/luautil.h"
#include "util/databuffer.h"
#include "sys/sync.h"
#include "typedef.h"
#include "luacode.h"
#include "loger.h"
#include "trandata.h"
#include "dbsaver.h"

using std::list;
using std::hash_map;
using std::find_if;
using std::string;
using std::copy_if;

class StockData : public TradeData{
public:
    StockData()
    {
        pDbSav = new DBSaver(this);
    };
    virtual ~StockData(){};
    virtual int Init()
    {
        return 0;
    };
public:
    int InitData(const UnitStock* pData);
    int GetDataByCode(const BS_T bs, const StockCode_T code, UnitStock* pUnitStock);
    int GetAllData(list<UnitStock*>& pList);
    int UpdateDataByEntrust(const UnitStock* pData, const TransactionIdx_T tranIdx);
    int UpdateDataByBusiness(const UnitStock* pData, const TransactionIdx_T tranIdx);
    int ReleaseData();
    int RollbackData(const TransactionIdx_T tranId);
public:
    virtual int InitSaveInfo(){return pDbSav->InitSaveInfo();};
    virtual int GetSaveInfo(std::list<DBWriteItem*>& pList){return pDbSav->GetSaveInfo(pList);};
    virtual int DoneSaveInfo(){return pDbSav->DoneSaveInfo();};
    virtual bool Lock(int mill = 0){return criSect.Lock(mill);};
    virtual void Unlock(){criSect.Unlock();};
public:
    virtual int GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update);
    virtual int DoneDbUpdata(std::list<DBWriteItem*>& pList);
private:
    inline UnitStock* GetDataByCode_Inner(const BS_T bs, const StockCode_T code);
private:
    DataBuffer<UnitStock> pDataBuffer;
    TranData<TranStock, TRAN_DATA_COUNT> pTranData;
    list<UnitStock*> pStockList;
    hash_map<string, UnitStock*> pCodeMapB;
    hash_map<string, UnitStock*> pCodeMapS;
    CCriticalSection criSect;
    DBSaver* pDbSav;
};

#endif
#ifndef _AM_BUSINESS_H
#define _AM_BUSINESS_H

#pragma once

#include <list>
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
using std::find_if;

//成交数据
//只有添加，查询所有，数据保存，事务回滚 操作
class BusinessData : public TradeData{
public:
    BusinessData()
    {
        pDbSav = new DBSaver(this);
    };
    virtual ~BusinessData(){};
    virtual int Init()
    {
        return 0;
    };
public:
    Business* InitData(const Business* pData);
    Business* AddData(const Business* pData, const TransactionIdx_T tranIdx);
    int GetAllData(list<Business*>& rList);
    int ReleaseData();
    int RollbackData(const TransactionIdx_T tranId);

private:
    Business* AddData_Inner(const Business* pData, const OperateType opType, const TransactionIdx_T tranIdx);

public:
    virtual int InitSaveInfo(){return pDbSav->InitSaveInfo();};
    virtual int GetSaveInfo(std::list<DBWriteItem*>& pList){return pDbSav->GetSaveInfo(pList);};
    virtual int DoneSaveInfo(){return pDbSav->DoneSaveInfo();};
    virtual int GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update);
    virtual int DoneDbUpdata(std::list<DBWriteItem*>& pList);
    virtual bool Lock(int mill = 0){return criSect.Lock(mill);};
    virtual void Unlock(){criSect.Unlock();};
private:
    DataBuffer<Business> pDataBuffer;
    TranData<TranBusiness, TRAN_DATA_COUNT> pTranData;
    list<Business*> pBusiList;
    CCriticalSection criSect;
    DBSaver* pDbSav;
};

typedef struct BusiFunctorEqual
{
    BusiFunctorEqual(BusinessId_T n) : business_id(n) { }; 
    bool operator()(Business *p)
    { 
        LOG(INFO) << "[BusiFunctorEqual]business_id=" << p->business_id << "," << business_id << LOG_END_FLAG;
        return (business_id == p->business_id); 
    };
    BusinessId_T business_id;
}BusiFunctorEqual;


#endif
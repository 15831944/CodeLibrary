#ifndef _AM_ENTRUST_H
#define _AM_ENTRUST_H

#pragma once

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

//委托数据
//支持操作：
//  添加，查询所有，查询指定entId、repId，按委托状态查询，委托数据更新，数据保存，事务回滚
class EntrustData : public TradeData{
public:
    EntrustData()
    {
        pDbSav = new DBSaver(this);
    };
    virtual ~EntrustData(){};
    virtual int Init()
    {
        return 0;
    };
public:
    int InitData(const Entrust* pData);
    int AddData(const Entrust* pData, const TransactionIdx_T tranIdx);
    int GetDataByEntId(const EntrustId_T entId, Entrust* pDataOut);
    int GetDataByRepId(const ReportId_T repId, Entrust* pDataOut);
    int GetDataByEntStatus(const string entStatus, list<Entrust*>& pListOut);
    int GetAllData(list<Entrust*>& pListOut);
    int UpdateEntToRevokeInner(const EntrustId_T entId, const TransactionIdx_T tranIdx);
    int UpdateEntStatus(const EntrustId_T entId, const EntStatus_T status, const TransactionIdx_T tranIdx);
    int UpdateEntByRevOrder(const EntrustId_T entId, const EntrustId_T cancelId, const TransactionIdx_T tranIdx);
    int UpdateEntByReport(const EntrustId_T entId, const ReportId_T repId, const ExtReportId_T exrepId, const TransactionIdx_T tranIdx);
    int UpdateRevByReport(const EntrustId_T entId, const ReportId_T repId, const ExtReportId_T exrepId, const TransactionIdx_T tranIdx);
    int UpdateEntByWaste(const EntrustId_T entId, const string remark, const TransactionIdx_T tranIdx);
    int UpdateEntByRevoke(Entrust* pData, const TransactionIdx_T tranIdx);
    int UpdateEntByRevokeWaste(const EntrustId_T entId, const string remark, const TransactionIdx_T tranIdx);
    int UpdateRevByRevokeWaste(const EntrustId_T entId, const string remark, const TransactionIdx_T tranIdx);
    int UpdateEntByBusiness(Entrust* pData, const TransactionIdx_T tranIdx);
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
    int AddData_Inner(const Entrust* pData, const OperateType opType, const TransactionIdx_T tranIdx);
    inline Entrust* GetByEntId_Inner(const EntrustId_T entId);
    inline int UpdateEntStatus_Inner(Entrust* pData);
private:
    DataBuffer<Entrust> pDataBuffer;
    TranData<TranEntrust, TRAN_DATA_COUNT> pTranData;
    list<Entrust*> pEntList;
    hash_map<EntrustId_T, Entrust*> pEntIdMap;
    hash_map<ReportId_T, Entrust*> pRepIdMap;
    CCriticalSection criSect;
    DBSaver* pDbSav;
};

typedef struct EntFunctorEqualStatus
{
    EntFunctorEqualStatus(string v) : status(v) { }; 
    bool operator()(Entrust *p)
    { 
        LOG(INFO) << "[EntFunctorEqualStatus]entrust_id=" << p->entrust_id << ",entrust_status=" << p->entrust_status 
                << ",status=" << status << LOG_END_FLAG;
        return strchr(status.c_str(), p->entrust_status) != NULL; 
    };
    string status;
}EntFunctorEqualStatus;

typedef struct EntFunctorEqual
{
    EntFunctorEqual(EntrustId_T n) : entrust_id(n) { }; 
    bool operator()(Entrust *p)
    { 
        LOG(INFO) << "[BusiFunctorEqual]entrust_id=" << p->entrust_id << "," << entrust_id << LOG_END_FLAG;
        return (entrust_id == p->entrust_id); 
    };
    EntrustId_T entrust_id;
}EntFunctorEqual;

#endif
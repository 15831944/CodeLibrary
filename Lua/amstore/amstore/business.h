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

using std::list;
using std::find_if;

//成交数据
//只有添加，查询所有，数据保存，事务回滚 操作
class BusinessData : public TradeData{
public:
    BusinessData(){};
    virtual ~BusinessData(){};

public:
    Business* InitData(const Business* pData, const TransactionIdx_T tranIdx);
    Business* AddData(const Business* pData, const TransactionIdx_T tranIdx);
    int GetAllData(std::list<Business*>& rList);
    int ReleaseData();
    int RollbackData(const TransactionIdx_T tranId);

private:
    Business* AddData_Inner(const Business* pData, const OperateType opType, const TransactionIdx_T tranIdx);

private:
    DataBuffer<Business> pDataBuffer;
    TranData<TranBusiness, 2000> pTranData;
    std::list<Business*> pBusiList;
    CCriticalSection criSect;
};

typedef struct BusinessFinder
{
    BusinessFinder(BusinessId_T n) : business_id(n) { }; 
    bool operator()(Business *p) { return (business_id == p->business_id); };
    BusinessId_T business_id;
}BusinessFinder;

#endif
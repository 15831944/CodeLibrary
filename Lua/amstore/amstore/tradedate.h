#ifndef _AM_TRADEDATA_H
#define _AM_TRADEDATA_H

#include <list>
#include "typedef.h"
#include "sys/sync.h"

//交易数据基类,一个子帐户的交易数据
//基本功能有：
//  添加、获取...
//  事务的回滚
//  交易数据保存，通过字符串保存，保存后根据version_id来更新标志
//  线程安全
class TradeData{
public:
    TradeData(){}
    virtual ~TradeData(){}
    virtual int Init() = 0;
public:
    virtual int GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update) = 0;
    virtual int DoneDbUpdata(std::list<DBWriteItem*>& pList) = 0;

    virtual int InitSaveInfo() = 0;
    virtual int GetSaveInfo(std::list<DBWriteItem*>& pList) = 0;
    virtual int DoneSaveInfo() = 0;

    virtual bool Lock(int mill = 0) = 0;
    virtual void Unlock() = 0;
};

#endif
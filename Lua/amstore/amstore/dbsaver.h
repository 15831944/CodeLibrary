#ifndef _AM_DBSAVER_H
#define _AM_DBSAVER_H

#include <list>
#include "config.h"
#include "luautil/luautil.h"
#include "sys/sync.h"
#include "typedef.h"
#include "loger.h"
#include "tradedate.h"

//������SQL��ʽ���浽���ݿ�
//֧�ֵĲ����У�
//  ��ʼ�����������Ϣ
//  ��ȡSQL��ʽ�ı�����Ϣ
//  ������ɺ�ȷ�ϲ���
class DBSaver
{
public:
    DBSaver(TradeData* const p):pTradeData(p){};

    virtual ~DBSaver(){
        ReleaseItem();
    };

    int InitSaveInfo(){
        LOG(INFO) << "[DBSaver::InitSaveInfo]start." << LOG_END_FLAG;
        int iRes, iInsert, iUpdate;

        criSect.Lock();
        ReleaseItem();
        iRes = pTradeData->GetDbUpdateInfo(pList, &iInsert, &iUpdate);
        criSect.Unlock();

        LOG(INFO) << "[DBSaver::InitSaveInfo]end.iRes=" << iRes << ",size=" << pList.size() << LOG_END_FLAG;
        return iRes;
    };

    int GetSaveInfo(std::list<DBWriteItem*>& pListOut){
        LOG(INFO) << "[DBSaver::GetSaveInfo]start.size=" << pList.size() << LOG_END_FLAG;
        pListOut.clear();
        criSect.Lock();
        pListOut.assign(pList.begin(), pList.end());
        criSect.Unlock();
        LOG(INFO) << "[DBSaver::GetSaveInfo]end." << LOG_END_FLAG;
        return pList.size();
    };

    int DoneSaveInfo(){
        LOG(INFO) << "[DBSaver::DoneSaveInfo]start." << LOG_END_FLAG;
        int iRes, iInsert, iUpdate;

        criSect.Lock();
        iRes = pTradeData->DoneDbUpdata(pList);
        criSect.Unlock();

        LOG(INFO) << "[DBSaver::DoneSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
        return iRes;
    };
private:
    void ReleaseItem(){
        for(std::list<DBWriteItem*>::iterator it = pList.begin(); it != pList.end(); it++){
            DBWriteItem* p = static_cast<DBWriteItem*>(*it);
            delete p;
        }
        pList.clear();
    };
private:
    std::list<DBWriteItem*> pList;
    TradeData* const pTradeData;
    CCriticalSection criSect;
};

#endif
#ifndef _AM_DBSAVER_H
#define _AM_DBSAVER_H

#include <list>
#include "config.h"
#include "luautil/luautil.h"
#include "typedef.h"
#include "loger.h"

//数据以SQL形式保存到数据库
//支持的操作有：
//  初始化待保存的信息
//  获取SQL形式的保存信息
//  保存完成后确认操作
template <typename I, typename D>
class DBSaver
{
public:
    DBSaver():itemIndex(0){};

    virtual ~DBSaver(){
        ReleaseItem();
    };

    int InitSaveInfo(){
        LOG(INFO) << "[DBSaver::InitSaveInfo]start." << LOG_END_FLAG;
        int iRes, iInsert, iUpdate;

        ReleaseItem();

        //iRes = Store::getInstance().getDBNeedToUpdate(unitId, &pList, pCacheList, size, &iInsert, &iUpdate);
        iRes = Store::getInstance().getDBNeedToUpdate(unitId, &pList, &iInsert, &iUpdate);
        itemIndex = 0;

        LOG(INFO) << "[DBSaver::InitSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
        return iRes;
    };

    Item* GetNext(){
        LOG(INFO) << "[DBSaver::GetNext]start.itemIndex=" << itemIndex << ",size=" << pList.size() << LOG_END_FLAG;
        ItemBase* pRes = NULL;
        if(itemIndex >= 0 && itemIndex < pList.size()){
            pRes = pList[itemIndex];
            itemIndex++;
        }
        LOG(INFO) << "[DBSaver::GetNext]end." << LOG_END_FLAG;
        return (Item*)pRes;
    };

    int DoneSaveInfo(const int unitId){
        LOG(INFO) << "[DBSaver::DoneSaveInfo]start." << LOG_END_FLAG;
        int iRes, iInsert, iUpdate;

        iRes = Store::getInstance().doneDBUpdate(unitId, &pList);

        LOG(INFO) << "[DBSaver::DoneSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
        return iRes;
    };
private:
    void ReleaseItem(){
        for(std::list<ItemBase*>::iterator it = pList.begin(); it != pList.end(); it++){
            I* p = (I*)(*it);
            delete p;
        }
        pList.clear();
    };
private:
    std::list<DBWriteItem*> pList;
};

#endif
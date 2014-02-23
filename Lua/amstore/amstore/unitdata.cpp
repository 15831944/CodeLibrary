#include "unitdata.h"

map<UnitId_T, UnitData*> UnitData::_id2Data;
list<UnitData*> UnitData::_DataList;
CReadWriteLock UnitData::_rwLock;

UnitData* UnitData::GetByUnitId(const UnitId_T id){
    UnitData* pRet = NULL;

    _rwLock.ReadLock();
    if(_id2Data.find(id) != _id2Data.end()){
        pRet = _id2Data.at(id);
    }else{
        //
    }
    _rwLock.Unlock();

    return pRet;
}

UnitData* UnitData::InitUnitData(const UnitId_T id){
    UnitData* pRet = new UnitData();
    pRet->unit_id = id;

    _rwLock.WriteLock();
    if(_id2Data.find(id) == _id2Data.end()){
        _DataList.push_back(pRet);
        _id2Data[id] = pRet;
    }else{
        delete pRet;
        pRet = _id2Data.at(id);
    }
    _rwLock.Unlock();

    return pRet;
}
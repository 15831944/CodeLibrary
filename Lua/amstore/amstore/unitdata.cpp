#include "unitdata.h"

map<UnitId_T, UnitData*> UnitData::_id2Data;
list<UnitData*> UnitData::_DataList;

UnitData* UnitData::GetByUnitId(const UnitId_T id){
    UnitData* pRet = NULL;
    if(_id2Data.find(id) != _id2Data.end()){
        pRet = _id2Data.at(id);
    }else{
        //
    }
    return pRet;
}

UnitData* UnitData::InitUnitData(const UnitId_T id){
    UnitData* pRet = new UnitData();
    pRet->unit_id = id;
    _DataList.push_back(pRet);
    _id2Data[id] = pRet;
    return pRet;
}
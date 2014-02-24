#include "unitdata.h"
#include "luacode.h"

hash_map<UnitId_T, UnitData*> UnitData::_id2Data;
hash_map<OperatorId_T, UnitData*> UnitData::_operId2Data;
hash_map<string, UnitData*> UnitData::_operName2Data;
list<UnitData*> UnitData::_DataList;
CReadWriteLock UnitData::_rwLock;

UnitData* UnitData::GetByUnitId(const UnitId_T id)
{
    LOG(INFO) << "[UnitData::GetByUnitId]start. id = " << id << LOG_END_FLAG;
    UnitData* pRet = NULL;

    _rwLock.ReadLock();
    if(_id2Data.find(id) != _id2Data.end()){
        LOG(INFO) << "[UnitData::GetByUnitId]already has unit data." << LOG_END_FLAG;
        pRet = _id2Data.at(id);
    }else{
        //
    }
    _rwLock.Unlock();

    LOG(INFO) << "[UnitData::GetByUnitId]end. id = " << id << ",pRet=" << pRet << LOG_END_FLAG;
    return pRet;
}

UnitData* UnitData::InitUnitData(const UnitId_T id)
{
    LOG(INFO) << "[UnitData::InitUnitData]start. unitId = " << id << LOG_END_FLAG;
    UnitData* pRet = NULL;
    try{
        pRet = new UnitData();
        pRet->Init(id);
    }catch(const std::bad_alloc& ex){
        LOG(ERROR) << "[UnitData::InitUnitData]init data faild. unitId = " << id << ",msg=" << ex.what() << LOG_END_FLAG;
    }

    _rwLock.WriteLock();
    if(_id2Data.find(id) == _id2Data.end()){
        LOG(INFO) << "[UnitData::InitUnitData]not found unitdata." << LOG_END_FLAG;
        _DataList.push_back(pRet);
        _id2Data[id] = pRet;
    }else{
        LOG(INFO) << "[UnitData::InitUnitData]already has unitdata." << LOG_END_FLAG;
        delete pRet;
        pRet = _id2Data.at(id);
    }
    _rwLock.Unlock();

    LOG(INFO) << "[UnitData::InitUnitData]end. unitId = " << id << ",pRet=" << pRet << LOG_END_FLAG;
    return pRet;
}

UnitData* UnitData::ConnectOperData(const OperatorId_T operId, const OperatorName_T operName, UnitData* pData)
{
    _rwLock.WriteLock();
    if(_operId2Data.find(operId) == _operId2Data.end()){
        _operId2Data[operId] = pData;
        _operName2Data[operName] = pData;
    }else{
        //
    }
    _rwLock.Unlock();
    return pData;
}

UnitData* UnitData::GetByOper(const OperatorId_T operId)
{
    UnitData* pRet = NULL;

    _rwLock.ReadLock();
    if(_operId2Data.find(operId) != _operId2Data.end()){
        pRet = _operId2Data.at(operId);
    }else{
        //
    }
    _rwLock.Unlock();

    return pRet;
}

UnitData* UnitData::GetByOper(const OperatorName_T operName)
{
    UnitData* pRet = NULL;

    _rwLock.ReadLock();
    if(_operName2Data.find(operName) != _operName2Data.end()){
        pRet = _operName2Data.at(operName);
    }else{
        //
    }
    _rwLock.Unlock();

    return pRet;
}

int UnitData::Init(UnitId_T unitId)
{
    unit_id = unitId;
    pUnitBaseData = new UnitBaseData();
    pFundBaseData = new FundBaseData();

    memset(pUnitBaseData, 0, sizeof(*pUnitBaseData));
    memset(pFundBaseData, 0, sizeof(*pFundBaseData));

    pUnitBaseData->unit_id = unitId;
    return 0;
}


int UnitData::AddUnitBaseData(lua_State* L)
{
    LOG(INFO) << "[UnitData::AddUnitBaseData]start." << LOG_END_FLAG;
    int iRet = 1;

    memset(pUnitBaseData, 0, sizeof(UnitBaseData));
    LUAUNPACK_UNITBASE(L, pUnitBaseData);

    LOG(INFO) << "[UnitData::AddUnitBaseData]unitId=" << pUnitBaseData->unit_id << ", operator_id=" << pUnitBaseData->operator_id
        << ", operator_name=" << pUnitBaseData->operator_name << LOG_END_FLAG;

    ConnectOperData(pUnitBaseData->operator_id, pUnitBaseData->operator_name, this);

    PINT(L, 0);

    LOG(INFO) << "[UnitData::AddUnitBaseData]end." << LOG_END_FLAG;
    return iRet;
}


int UnitData::GetUnitBaseData(lua_State* L)
{
    LOG(INFO) << "[UnitData::GetUnitBaseData]start.unit_id = " << unit_id << LOG_END_FLAG;
    int iRes = 2;
    PINT(L, 0);
    LUAPACK_UNITBASE(L, pUnitBaseData);
    LOG(INFO) << "[UnitData::GetUnitBaseData]end.unit_id = " << unit_id << LOG_END_FLAG;
    return iRes;
}


int UnitData::AddFundBaseData(lua_State* L)
{
    LOG(INFO) << "[UnitData::AddFundBaseData]start." << LOG_END_FLAG;
    int iRet = 1;

    memset(pFundBaseData, 0, sizeof(pFundBaseData));
    LUAUNPACK_FUNDLINK(L, pFundBaseData);

    LOG(INFO) << "[UnitData::AddFundBaseData]:unit_id = " << unit_id << ",fund_id = " <<pFundBaseData->fund_id << ",cts_version = "
        << pFundBaseData->cts_version << LOG_END_FLAG;

    PINT(L, 0);
    LOG(INFO) << "[UnitData::AddFundBaseData]end." << LOG_END_FLAG;
    return iRet;
}


int UnitData::GetFundBaseData(lua_State* L)
{
    LOG(INFO) << "[UnitData::GetFundBaseData]start.unit_id = " << unit_id << LOG_END_FLAG;
    int iRes = 2;
    LUAPACK_FUNDLINK(L, pFundBaseData);
    PINT(L, 0);
    LOG(INFO) << "[UnitData::GetFundBaseData]end.unit_id = " << unit_id << ",fund_id = " <<pFundBaseData->fund_id << ",cts_version = "
        << pFundBaseData->cts_version << LOG_END_FLAG;
    return iRes;
}


int GetUnitBaseByOperId_F(lua_State* L)
{
    int iRes = 0;
    OperatorId_T operId = PAI(L, 1);
    UnitData* pUnitData = UnitData::GetByOper(operId);
    if(pUnitData){
        PINT(L, 0);
        LUAPACK_UNITBASE(L, pUnitData->pUnitBaseData);
        LOG(INFO) << "[GetUnitBaseByOperId_F]:success,unit_id = " << pUnitData->pUnitBaseData->unit_id 
            << ",operator_id = " << pUnitData->pUnitBaseData->operator_id
            << ",operator_name = " << pUnitData->pUnitBaseData->operator_name << LOG_END_FLAG;
        iRes = 2;
    }else{
        iRes = 1;
        iRes = 1;
        PINT(L, -1);
    }
    
    return iRes;
}

LUA_LIB_API GetUnitBaseByOperId(lua_State* L)
{
    return GetUnitBaseByOperId_F(L);
}


int GetUnitBaseByOperName_F(lua_State* L)
{
    LOG(INFO) << "[GetUnitBaseByOperName]start." << LOG_END_FLAG;
    int iRes = 0;
    OperatorId_T operId = PAI(L, 1);
    UnitData* pUnitData = UnitData::GetByOper(operId);
    if(pUnitData){
        LUAPACK_UNITBASE(L, pUnitData->pUnitBaseData);
        LOG(INFO) << "[GetUnitBaseByOperName]:success,unit_id = " << pUnitData->pUnitBaseData->unit_id 
            << ",operator_id = " << pUnitData->pUnitBaseData->operator_id
            << ",operator_name = " << pUnitData->pUnitBaseData->operator_name << LOG_END_FLAG;
        iRes = 2;
        PINT(L, 0);
    }else{
        iRes = 1;
        PINT(L, -1);
    }
    LOG(INFO) << "[GetUnitBaseByOperName]start.iRes=" << iRes << LOG_END_FLAG;
    return iRes;
}

LUA_LIB_API GetUnitBaseByOperName(lua_State* L)
{
    return GetUnitBaseByOperName_F(L);
}


int GetFastUnitBaseByOperName_F(lua_State* L)
{
    int iRes = 0;
    OperatorId_T operId = PAI(L, 1);
    UnitData* pUnitData = UnitData::GetByOper(operId);
    if(pUnitData){
        PINT(L, pUnitData->unit_id); 
        PINT(L, pUnitData->pUnitBaseData->operator_id); 
        PINT(L, pUnitData->pUnitBaseData->fund_id); 
        LOG(INFO) << "[GetFastUnitBaseByOperName_F]:success,unit_id = " << pUnitData->unit_id 
            << ",operator_id = " << pUnitData->pUnitBaseData->operator_id
            << ",fund_id = " << pUnitData->pUnitBaseData->fund_id << LOG_END_FLAG;
        iRes = 3;
    }
    return iRes;
}

LUA_LIB_API GetFastUnitBaseByOperName(lua_State* L)
{
    return GetFastUnitBaseByOperName_F(L);
}


const char UnitData::className[] = "UnitData";
Lunar<UnitData>::RegType UnitData::methods[] = 
{
    method(UnitData, AddUnitBaseData),
    method(UnitData, GetUnitBaseData),
    method(UnitData, AddFundBaseData),
    method(UnitData, GetFundBaseData),
    {0, 0}
};

static luaL_Reg mylibs[] = { 
    {"GetUnitBaseByOperId", GetUnitBaseByOperId},
    {"GetUnitBaseByOperName", GetUnitBaseByOperName},
    {"GetFastUnitBaseByOperName", GetFastUnitBaseByOperName},
    {0, 0}
};


int RegistUnitBase(lua_State* L)
{
    Lunar<UnitData>::Register(L);

    luaL_register(L, "amstore.unitdata", mylibs);

    return 0;
}
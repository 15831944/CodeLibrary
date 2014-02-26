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
    pBusiData = new BusinessData();
    pBusiData->Init();
    pAssetDate = new UnitAssetData();
    pAssetDate->Init();

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
    PINT(L, 0);
    LUAPACK_FUNDLINK(L, pFundBaseData);
    LOG(INFO) << "[UnitData::GetFundBaseData]end.unit_id = " << unit_id << ",fund_id = " <<pFundBaseData->fund_id << ",cts_version = "
        << pFundBaseData->cts_version << LOG_END_FLAG;
    return iRes;
}

int UnitData::NewTransaction(lua_State* L)
{
    LOG(INFO) << "[UnitData::NewTransaction]start." << LOG_END_FLAG;
    TransactionIdx_T tranIdx = serialGenTran.Generate();
    PN(L, tranIdx);
    LOG(INFO) << "[UnitData::NewTransaction]end.tranIdx=" << tranIdx << LOG_END_FLAG;
    return 1;
}

int UnitData::LockSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::LockSaveInfo]start." << LOG_END_FLAG;
    int ok = 0;
    if(criSectSI.Lock(60000)){
        PN(L, 0);
        ok = 1;
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::LockSaveInfo]end.ok=" << ok << LOG_END_FLAG;
    return 1;
}

int UnitData::UnlockSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::UnlockSaveInfo]start." << LOG_END_FLAG;
    criSectSI.Unlock();
    PN(L, 0);
    LOG(INFO) << "[UnitData::UnlockSaveInfo]end." << LOG_END_FLAG;
    return 1;
}

int UnitData::BusiInitData(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiInitData]start." << LOG_END_FLAG;
    int iRes = 0;
    Business* pBusi = new Business();
    memset(pBusi, 0, sizeof(Business));

    LUAUNPACK_BUSINESS(L, pBusi);

    LOG(INFO) << "[UnitData::BusiInitData]unitId=" << pBusi->unit_id << ", business_id=" << pBusi->business_id 
        << ", entrustdirection_code=" << pBusi->entrustdirection_code << LOG_END_FLAG;

    Business* pRes = pBusiData->InitData(pBusi);
    if(pRes){
        PN(L, 0);
        iRes = 1;
    }else{
        PN(L, -1);
        iRes = 1;
    }
    delete pBusi;

    LOG(INFO) << "[UnitData::BusiInitData]end." << LOG_END_FLAG;
    return iRes;
}

int UnitData::BusiAddData(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiAddData]start." << LOG_END_FLAG;
    int iRes = 0;
    Business* pBusi = new Business();
    memset(pBusi, 0, sizeof(Business));

    LUAUNPACK_BUSINESS(L, pBusi);
    TransactionIdx_T tranIdx = PAI(L, 29);

    LOG(INFO) << "[UnitData::BusiAddData]unitId=" << pBusi->unit_id << ", business_id=" << pBusi->business_id 
        << ", entrustdirection_code=" << pBusi->entrustdirection_code << ",tranIdx=" << tranIdx << LOG_END_FLAG;

    Business* pRes = pBusiData->AddData(pBusi, tranIdx);
    if(pRes){
        PN(L, 0);
        iRes = 1;
    }else{
        PN(L, -1);
        iRes = 1;
    }
    delete pBusi;

    LOG(INFO) << "[UnitData::BusiAddData]end." << LOG_END_FLAG;
    return iRes;
}

int UnitData::BusiGetAllData(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiGetAllData]start." << LOG_END_FLAG;
    int iRes = 0;
    int count = 0;

    list<Business*> aList;
    //因为aList中将要保存pBusiData里面的Business*数据
    pBusiData->Lock();
    iRes = pBusiData->GetAllData(aList);

    if(iRes >= 0){
        PN(L, 0);
        lua_newtable(L);
        for(list<Business*>::iterator it = aList.begin(); it != aList.end(); ++it){
            Business* pBusi = *it;
            PINT(L, ++count); 
            LUAPACK_BUSINESS_TABLE(L, pBusi);
            lua_settable(L, -3);
        }
        iRes = 2;
    }else{
        PN(L, -1);
        iRes = 1;
    }
    pBusiData->Unlock();

    LOG(INFO) << "[UnitData::BusiGetAllData]end. iRes=" << iRes << ", count=" << count << LOG_END_FLAG;
    return iRes;
}

int UnitData::BusiReleaseData(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiReleaseData]start." << LOG_END_FLAG;
    int iRes = pBusiData->ReleaseData();
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::BusiReleaseData]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int UnitData::BusiRollbackData(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiRollbackData]start." << LOG_END_FLAG;
    TransactionIdx_T tranIdx = PAI(L, 1);

    LOG(INFO) << "[UnitData::BusiRollbackData]tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRes = pBusiData->RollbackData(tranIdx);
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::BusiRollbackData]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int UnitData::BusiInitDbSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiInitDbSaveInfo]start." << LOG_END_FLAG;
    int iRes = pBusiData->InitSaveInfo();
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::BusiInitDbSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int UnitData::BusiGetSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiGetSaveInfo]start." << LOG_END_FLAG;
    list<DBWriteItem*> aList;
    int iRes = pBusiData->GetSaveInfo(aList);
    int count = 0;
    if(iRes >= 0){
        LOG(INFO) << "[UnitData::BusiGetSaveInfo]size=" << aList.size() << LOG_END_FLAG;
        PN(L, 0);
        lua_newtable(L);
        for(list<DBWriteItem*>::iterator it = aList.begin(); it != aList.end(); ++it){
            DBWriteItem* pItem = *it;
            PINT(L, ++count); 
            LUAPACK_DBWRITEITEM_ITEM_TABLE(L, pItem);
            lua_settable(L, -3);
        }
        iRes = 2;
    }else{
        PN(L, -1);
        iRes = 1;
    }
    LOG(INFO) << "[UnitData::BusiGetSaveInfo]end.iRes=" << iRes << ",count=" << count << LOG_END_FLAG;
    return iRes;
}

int UnitData::BusiDoneSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::BusiDoneSaveInfo]start." << LOG_END_FLAG;
    int iRes = pBusiData->DoneSaveInfo();
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::BusiDoneSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetInitData(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetInitData]start." << LOG_END_FLAG;
    int iRes = 0;
    UnitAsset* pAsset = new UnitAsset();
    memset(pAsset, 0, sizeof(UnitAsset));

    LUAUNPACK_UNITASSET(L, pAsset);
    UnitAsset* pRet = pAssetDate->InitData(pAsset);
    if(pRet){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    delete pAsset;

    LOG(INFO) << "[UnitData::AssetInitData]end." << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetUpdateByEntrust(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetUpdateByEntrust]start." << LOG_END_FLAG;
    UnitAsset* pAsset = new UnitAsset();
    memset(pAsset, 0, sizeof(UnitAsset));

    CS(pAsset->currency_code, L, 1); 
    pAsset->prebuy_balance = PAD(L, 2);
    pAsset->presale_balance = PAD(L, 3);
    pAsset->prebuy_fee = PAD(L, 4);
    pAsset->presale_fee = PAD(L, 5);
    TransactionIdx_T tranIdx = PAI(L, 6);

    LOG(INFO) << "[UnitData::AssetUpdateByEntrust]currency_code=" << pAsset->currency_code << ",prebuy_balance=" << pAsset->prebuy_balance
        << "presale_balance=" << pAsset->presale_balance << ",prebuy_fee=" << pAsset->prebuy_fee
        << "presale_fee=" << pAsset->presale_fee << ",tranIdx=" << tranIdx << LOG_END_FLAG;

    int iRet = pAssetDate->UpdateByEntrust(pAsset, tranIdx);
    if(iRet >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }

    LOG(INFO) << "[UnitData::AssetUpdateByEntrust]end." << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetUpdateByBusiness(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetUpdateByBusiness]start." << LOG_END_FLAG;
    UnitAsset* pAsset = new UnitAsset();
    memset(pAsset, 0, sizeof(UnitAsset));

    CS(pAsset->currency_code, L, 1); 
    pAsset->current_cash = PAD(L, 2);
    pAsset->prebuy_balance = PAD(L, 3);
    pAsset->presale_balance = PAD(L, 4);
    pAsset->prebuy_fee = PAD(L, 5);
    pAsset->presale_fee = PAD(L, 6);
    TransactionIdx_T tranIdx = PAI(L, 7);

    LOG(INFO) << "[UnitData::AssetUpdateByBusiness]currency_code=" << pAsset->currency_code 
        << ",current_cash=" << pAsset->current_cash << ",prebuy_balance=" << pAsset->prebuy_balance
        << "presale_balance=" << pAsset->presale_balance << ",prebuy_fee=" << pAsset->prebuy_fee
        << "presale_fee=" << pAsset->presale_fee << ",tranIdx=" << tranIdx << LOG_END_FLAG;

    int iRet = pAssetDate->UpdateByBusiness(pAsset, tranIdx);
    if(iRet >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }

    LOG(INFO) << "[UnitData::AssetUpdateByBusiness]end." << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetGetData(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetGetData]start." << LOG_END_FLAG;
    CurrencyCode_T curCode = {0};
    CS(curCode, L, 1);
    UnitAsset* pAsset = new UnitAsset();
    memset(pAsset, 0, sizeof(UnitAsset));

    int iRet = 0;
    UnitAsset* pRet = pAssetDate->GetData(curCode, pAsset);

    LOG(INFO) << "[UnitData::AssetGetData]curCode=" << curCode << ",current_cash=" << pAsset->current_cash
        << ",prebuy_fee=" << pAsset->prebuy_fee << ",prebuy_balance=" << pAsset->prebuy_balance << LOG_END_FLAG;

    if(pRet){
        PN(L, 0);
        LUAPACK_UNITASSET_TABLE(L, pAsset);
        iRet = 2;
    }else{
        PN(L, -1);
        iRet = 1;
    }
    LOG(INFO) << "[UnitData::AssetGetData]end." << LOG_END_FLAG;
    return iRet;
}

int UnitData::AssetReleaseData(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetReleaseData]start." << LOG_END_FLAG;
    int iRet = pAssetDate->ReleaseData();
    if(iRet >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }

    LOG(INFO) << "[UnitData::AssetReleaseData]end." << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetRollbackData(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetRollbackData]start." << LOG_END_FLAG;
    TransactionIdx_T tranIdx = PAI(L, 1);

    LOG(INFO) << "[UnitData::AssetRollbackData]tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRes = pAssetDate->RollbackData(tranIdx);
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::AssetRollbackData]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetInitDbSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetInitDbSaveInfo]start." << LOG_END_FLAG;
    int iRes = pAssetDate->InitSaveInfo();
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::AssetInitDbSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int UnitData::AssetGetSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetGetSaveInfo]start." << LOG_END_FLAG;
    list<DBWriteItem*> aList;
    int iRes = pAssetDate->GetSaveInfo(aList);
    int count = 0;
    if(iRes >= 0){
        LOG(INFO) << "[UnitData::AssetGetSaveInfo]size=" << aList.size() << LOG_END_FLAG;
        PN(L, 0);
        lua_newtable(L);
        for(list<DBWriteItem*>::iterator it = aList.begin(); it != aList.end(); ++it){
            DBWriteItem* pItem = *it;
            PINT(L, ++count); 
            LUAPACK_DBWRITEITEM_ITEM_TABLE(L, pItem);
            lua_settable(L, -3);
        }
        iRes = 2;
    }else{
        PN(L, -1);
        iRes = 1;
    }
    LOG(INFO) << "[UnitData::AssetGetSaveInfo]end.iRes=" << iRes << ",count=" << count << LOG_END_FLAG;
    return iRes;
}

int UnitData::AssetDoneSaveInfo(lua_State* L)
{
    LOG(INFO) << "[UnitData::AssetDoneSaveInfo]start." << LOG_END_FLAG;
    int iRes = pAssetDate->DoneSaveInfo();
    if(iRes >= 0){
        PN(L, 0);
    }else{
        PN(L, -1);
    }
    LOG(INFO) << "[UnitData::AssetDoneSaveInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return 1;
}

int GetUnitBaseByOperId_F(lua_State* L)
{
    LOG(INFO) << "[GetUnitBaseByOperId_F]start." << LOG_END_FLAG;
    int iRes = 0;
    OperatorId_T operId = PAI(L, 1);

    LOG(INFO) << "[GetUnitBaseByOperId_F]operId=" << operId << LOG_END_FLAG;

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
        PINT(L, -1);
    }
    
    LOG(INFO) << "[GetUnitBaseByOperId_F]end.iRes=" << iRes << LOG_END_FLAG;
    return iRes;
}

LUA_LIB_API GetUnitBaseByOperId(lua_State* L)
{
    return GetUnitBaseByOperId_F(L);
}


int GetUnitBaseByOperName_F(lua_State* L)
{
    LOG(INFO) << "[GetUnitBaseByOperName_F]start." << LOG_END_FLAG;
    int iRes = 0;
    OperatorName_T operName = {0};
    CS(operName, L, 1);

    LOG(INFO) << "[GetUnitBaseByOperName_F]operName=" << operName << LOG_END_FLAG;
    UnitData* pUnitData = UnitData::GetByOper(operName);
    if(pUnitData){
        PINT(L, 0);
        LUAPACK_UNITBASE(L, pUnitData->pUnitBaseData);
        LOG(INFO) << "[GetUnitBaseByOperName_F]:success,unit_id = " << pUnitData->pUnitBaseData->unit_id 
            << ",operator_id = " << pUnitData->pUnitBaseData->operator_id
            << ",operator_name = " << pUnitData->pUnitBaseData->operator_name << LOG_END_FLAG;
        iRes = 2;
    }else{
        PINT(L, -1);
        iRes = 1;
    }
    LOG(INFO) << "[GetUnitBaseByOperName_F]start.iRes=" << iRes << LOG_END_FLAG;
    return iRes;
}

LUA_LIB_API GetUnitBaseByOperName(lua_State* L)
{
    return GetUnitBaseByOperName_F(L);
}


int GetFastUnitBaseByOperName_F(lua_State* L)
{
    LOG(INFO) << "[GetFastUnitBaseByOperName_F]start." << LOG_END_FLAG;
    int iRes = 0;
    OperatorName_T operName = {0};
    CS(operName, L, 1);

    LOG(INFO) << "[GetFastUnitBaseByOperName_F]operName=" << operName << LOG_END_FLAG;
    UnitData* pUnitData = UnitData::GetByOper(operName);
    if(pUnitData){
        PINT(L, pUnitData->unit_id); 
        PINT(L, pUnitData->pUnitBaseData->operator_id); 
        PINT(L, pUnitData->pUnitBaseData->fund_id); 
        LOG(INFO) << "[GetFastUnitBaseByOperName_F]:success,unit_id = " << pUnitData->unit_id 
            << ",operator_id = " << pUnitData->pUnitBaseData->operator_id
            << ",fund_id = " << pUnitData->pUnitBaseData->fund_id << LOG_END_FLAG;
        iRes = 3;
    }
    LOG(INFO) << "[GetFastUnitBaseByOperName_F]end.iRes=" << iRes << LOG_END_FLAG;
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

    method(UnitData, NewTransaction),
    method(UnitData, LockSaveInfo),
    method(UnitData, UnlockSaveInfo),

    method(UnitData, BusiInitData),
    method(UnitData, BusiAddData),
    method(UnitData, BusiGetAllData),
    method(UnitData, BusiReleaseData),
    method(UnitData, BusiRollbackData),
    method(UnitData, BusiInitDbSaveInfo),
    method(UnitData, BusiGetSaveInfo),
    method(UnitData, BusiDoneSaveInfo),

    method(UnitData, AssetInitData),
    method(UnitData, AssetUpdateByEntrust),
    method(UnitData, AssetUpdateByBusiness),
    method(UnitData, AssetGetData),
    method(UnitData, AssetReleaseData),
    method(UnitData, AssetRollbackData),
    method(UnitData, AssetInitDbSaveInfo),
    method(UnitData, AssetGetSaveInfo),
    method(UnitData, AssetDoneSaveInfo),

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

    luaL_register(L, "amstore", mylibs);

    return 0;
}
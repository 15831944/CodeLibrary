#include "unitasset.h"

UnitAsset* UnitAssetData::InitData(const UnitAsset* pData)
{
    LOG(INFO) << "[UnitAssetData::InitData]start.pAssetList.size=" << pAssetList.size()
        << ",currency_code=" << pData->currency_code << ",current_cash=" << pData->current_cash 
        << ",prebuy_balance=" << pData->prebuy_balance << ",presale_balance=" << pData->presale_balance 
        << ",prebuy_fee=" << pData->prebuy_fee << ",presale_fee=" << pData->presale_fee << LOG_END_FLAG;
    
    UnitAsset* pNewAsset = new UnitAsset();
    memcpy(pNewAsset, pData, sizeof(*pNewAsset));
    pNewAsset->opType = OperateTypeInit;

    criSect.Lock();
    vector<UnitAsset*>::iterator it = std::find_if(pAssetList.begin(), pAssetList.end(), AssetFunctorEqual(pData->currency_code));
    if(it == pAssetList.end()){
        pAssetList.push_back(pNewAsset);
    }else{
        memcpy(*it, pNewAsset, sizeof(*pNewAsset));
        delete pNewAsset;
        pNewAsset = NULL;

        LOG(ERROR) << "[UnitAssetData::InitData]already has the data." << LOG_END_FLAG;
    }
    criSect.Unlock();

    LOG(INFO) << "[UnitAssetData::InitData]end.pAssetList.size=" << pAssetList.size() << LOG_END_FLAG;
    return pNewAsset;
}

UnitAsset* UnitAssetData::GetData(CurrencyCode_T curCode, UnitAsset* pData)
{
    LOG(INFO) << "[UnitAssetData::GetData]start.pAssetList.size=" << pAssetList.size() << LOG_END_FLAG;

    UnitAsset* pRet = NULL;
    criSect.Lock();
    vector<UnitAsset*>::iterator it = std::find_if(pAssetList.begin(), pAssetList.end(), AssetFunctorEqual(curCode));
    if(it != pAssetList.end()){
        memcpy(pData, *it, sizeof(*pData));
        pRet = pData;
    }else{
        LOG(ERROR) << "[UnitAssetData::InitData]data not found.curCode=" << curCode << LOG_END_FLAG;
    }
    criSect.Unlock();

    LOG(INFO) << "[UnitAssetData::GetData]end.pAssetList.size=" << pAssetList.size() << LOG_END_FLAG;
    return pRet;
}

int UnitAssetData::UpdateByEntrust(const UnitAsset* pData, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[UnitAssetData::UpdateByEntrust]start.currency_code=" << pData->currency_code << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    TranAsset* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[UnitAssetData::UpdateByEntrust]get new tran data faild." << LOG_END_FLAG;
        return -1;
    }

    LOG(INFO) << "[UnitAssetData::UpdateByEntrust]pNewTran=" << pNewTran << LOG_END_FLAG;
    UnitAsset* pAsset = NULL;
    criSect.Lock();
    vector<UnitAsset*>::iterator it = std::find_if(pAssetList.begin(), pAssetList.end(), AssetFunctorEqual(pData->currency_code));
    if(it != pAssetList.end()){
        pAsset = *it;

        pAsset->prebuy_balance += pData->prebuy_balance;
        pAsset->presale_balance += pData->presale_balance;
        pAsset->prebuy_fee += pData->prebuy_fee;
        pAsset->presale_fee += pData->presale_fee;
        if(OperateTypeInit == pAsset->opType){
            pAsset->opType = OperateTypeUpdate;
        }
        pAsset->versionIdx++;

        memcpy(pNewTran, pData, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        memcpy(pNewTran->currency_code, pAsset->currency_code, sizeof(pNewTran->currency_code));

        LOG(INFO) << "[UnitAssetData::UpdateByEntrust]pNewTran=currency_code=" << pNewTran->currency_code 
        << ",current_cash=" << pNewTran->current_cash << ",prebuy_balance=" << pNewTran->prebuy_balance 
        << ",presale_balance=" << pNewTran->presale_balance << ",prebuy_fee=" << pNewTran->prebuy_fee
        << ",presale_fee=" << pNewTran->presale_fee << LOG_END_FLAG;
    }else{
        LOG(ERROR) << "[UnitAssetData::UpdateByEntrust]asset not found." << LOG_END_FLAG;
    }
    criSect.Unlock();

    LOG(INFO) << "[UnitAssetData::UpdateByEntrust]end.currency_code=" << pNewTran->currency_code 
        << ",current_cash=" << pNewTran->current_cash << LOG_END_FLAG;
    return 0;
}

int UnitAssetData::UpdateByBusiness(const UnitAsset* pData, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[UnitAssetData::UpdateByBusiness]start.currency_code=" << pData->currency_code << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    TranAsset* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[UnitAssetData::UpdateByBusiness]get new tran data faild." << LOG_END_FLAG;
        return -1;
    }

    LOG(INFO) << "[UnitAssetData::UpdateByBusiness]pNewTran=" << pNewTran << LOG_END_FLAG;
    UnitAsset* pAsset = NULL;
    criSect.Lock();
    vector<UnitAsset*>::iterator it = std::find_if(pAssetList.begin(), pAssetList.end(), AssetFunctorEqual(pData->currency_code));
    if(it != pAssetList.end()){
        pAsset = *it;

        pAsset->current_cash += pData->current_cash;
        pAsset->prebuy_balance += pData->prebuy_balance;
        pAsset->presale_balance += pData->presale_balance;
        pAsset->prebuy_fee += pData->prebuy_fee;
        pAsset->presale_fee += pData->presale_fee;

        if(OperateTypeInit == pAsset->opType){
            pAsset->opType = OperateTypeUpdate;
        }
        pAsset->versionIdx++;

        memcpy(pNewTran, pData, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        memcpy(pNewTran->currency_code, pAsset->currency_code, sizeof(pNewTran->currency_code));

        LOG(INFO) << "[UnitAssetData::UpdateByBusiness]pNewTran=currency_code=" << pNewTran->currency_code 
        << ",current_cash=" << pNewTran->current_cash << ",prebuy_balance=" << pNewTran->prebuy_balance 
        << ",presale_balance=" << pNewTran->presale_balance << ",prebuy_fee=" << pNewTran->prebuy_fee
        << ",presale_fee=" << pNewTran->presale_fee << LOG_END_FLAG;
    }else{
        LOG(ERROR) << "[UnitAssetData::UpdateByBusiness]asset not found." << LOG_END_FLAG;
    }
    criSect.Unlock();

    LOG(INFO) << "[UnitAssetData::UpdateByBusiness]end.currency_code=" << pNewTran->currency_code 
        << ",current_cash=" << pNewTran->current_cash << LOG_END_FLAG;
    return 0;
}

int UnitAssetData::ReleaseData()
{
    LOG(INFO) << "[UnitAssetData::ReleaseData]start.pAssetList.size=" << pAssetList.size() << LOG_END_FLAG;
    int count = 0;

    criSect.Lock();
    for(std::vector<UnitAsset*>::iterator it = pAssetList.begin(); it != pAssetList.end(); ++it){
        UnitAsset* pAsset = *it;
        delete pAsset;
    }
    pAssetList.clear();
    criSect.Unlock();

    LOG(INFO) << "[UnitAssetData::ReleaseData]end.count=" << count << "pAssetList.size=" << pAssetList.size() << LOG_END_FLAG;
    return count;
}

int UnitAssetData::RollbackData(const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[UnitAssetData::RollbackData]start.tranIdx=" << tranIdx << LOG_END_FLAG;
    TranAsset* pTran = pTranData.GetTranData(tranIdx);
    if(!pTran){
        LOG(ERROR) << "[BusinessData::RollbackData]get tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    LOG(INFO) << "[UnitAssetData::RollbackData]pTran=" << pTran << "currency_code=" << pTran->currency_code 
        << ",current_cash=" << pTran->current_cash << ",prebuy_balance=" << pTran->prebuy_balance 
        << ",presale_balance=" << pTran->presale_balance << ",prebuy_fee=" << pTran->prebuy_fee
        << ",presale_fee=" << pTran->presale_fee << LOG_END_FLAG;
    vector<UnitAsset*>::iterator it = std::find_if(pAssetList.begin(), pAssetList.end(), AssetFunctorEqual(pTran->currency_code));
    if(it != pAssetList.end()){
        UnitAsset* pAsset = *it;

        pAsset->current_cash -= pTran->current_cash;
        pAsset->prebuy_balance -= pTran->prebuy_balance;
        pAsset->presale_balance -= pTran->presale_balance;
        pAsset->prebuy_fee -= pTran->prebuy_fee;
        pAsset->presale_fee -= pTran->presale_fee;

        if(OperateTypeInit == pAsset->opType){
            pAsset->opType = OperateTypeUpdate;
        }
        pAsset->versionIdx++;
    }else{
        LOG(ERROR) << "[BusinessData::RollbackData]get unitasset faild." << LOG_END_FLAG;
        return -1;
    }
    LOG(INFO) << "[BusinessData::RollbackData]end." << LOG_END_FLAG;
    return 0;
}

int UnitAssetData::GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update)
{
    LOG(INFO) << "[UnitAssetData::GetDbUpdateInfo]start.pAssetList.size=" << pAssetList.size() << LOG_END_FLAG;

    int uCount = 0;
    criSect.Lock();
    for(vector<UnitAsset*>::iterator it = pAssetList.begin(); it != pAssetList.end(); ++it){
        UnitAsset* pAsset = *it;
        if(OperateTypeUpdate == pAsset->opType){
            DBWriteAsset* pNew = new DBWriteAsset();
            memcpy(pNew->currency_code, pAsset->currency_code, sizeof(pNew->currency_code));
            pNew->versionIdx = pAsset->versionIdx;

            std::stringstream ss;
            ss << "update unitasset set begin_cash = "
<< pAsset->begin_cash << ", current_cash = " << pAsset->current_cash << ", prebuy_balance = " 
<< pAsset->prebuy_balance << ", presale_balance = " << pAsset->presale_balance << ", input_balance = "
<< pAsset->input_balance << ", output_balance = " << pAsset->output_balance << ", input_total = "
<< pAsset->input_total << ", output_total = " << pAsset->output_total << ", prebuy_fee = "
<< pAsset->prebuy_fee << ", presale_fee = " << pAsset->presale_fee 
<< " where busin_date = " << pAsset->busin_date << " and unit_id = " << pAsset->unit_id 
<< " and currency_code = '" << pAsset->currency_code << "'";
            pNew->sql.assign(ss.str().c_str());
            pList.push_back(pNew);

            LOG(INFO) << "[UnitAssetData::GetDbUpdateInfo]update: " << pNew->sql << LOG_END_FLAG;

            ++uCount;
        }
    }
    criSect.Unlock();

    *insert = 0;
    *update = uCount;

    LOG(INFO) << "[UnitAssetData::GetDbUpdateInfo]end.uCount=" << uCount << LOG_END_FLAG;
    return uCount;
}

int UnitAssetData::DoneDbUpdata(list<DBWriteItem*>& pList)
{
    LOG(INFO) << "[UnitAssetData::DoneDbUpdata]start.size=" << pList.size() << LOG_END_FLAG;
    criSect.Lock();
    for(list<DBWriteItem*>::iterator it = pList.begin(); it != pList.end(); it++){
        DBWriteAsset* pDBAsset = static_cast<DBWriteAsset*>(*it);
        UnitAsset* pAssetItem = NULL;

        vector<UnitAsset*>::iterator i = std::find_if(pAssetList.begin(), pAssetList.end(), AssetFunctorEqual(pDBAsset->currency_code));
        if(i != pAssetList.end()){
            pAssetItem = *i;

            if(pAssetItem->versionIdx == pDBAsset->versionIdx){
                pAssetItem->opType = OperateTypeInit;

                LOG(INFO) << "[UnitAssetData::DoneDbUpdata]OperateTypeInit=currency_code=" << pAssetItem->currency_code << LOG_END_FLAG;
            }else{
                if(pAssetItem->opType == OperateTypeInsert){
                    pAssetItem->opType = OperateTypeUpdate;
                }
            }
        }else{
            LOG(ERROR) << "[UnitAssetData::DoneDbUpdata]get asset faild.currency_code=" << pDBAsset->currency_code << LOG_END_FLAG;
        }
    }
    criSect.Unlock();
    LOG(INFO) << "[UnitAssetData::DoneDbUpdata]end." << LOG_END_FLAG;
    return 0;
}
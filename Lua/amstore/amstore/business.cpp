#include "business.h"


Business* BusinessData::InitData(const Business* pData, const TransactionIdx_T tranIdx)
{
    return AddData_Inner(pData, OperateTypeInit, tranIdx);
}

Business* BusinessData::AddData(const Business* pData, const TransactionIdx_T tranIdx)
{
    return AddData_Inner(pData, OperateTypeInsert, tranIdx);
}

int BusinessData::GetAllData(list<Business*>& rList)
{
    criSect.Lock();
    rList.clear();
    rList.assign(pBusiList.begin(), pBusiList.end());
    criSect.Unlock();
    return rList.size();
}

int BusinessData::ReleaseData()
{
    int count = 0;
    criSect.Lock();
    Business* pBusiHead = NULL;
    Business* pBusiTail = NULL;
    for(std::list<Business*>::iterator it = pBusiList.begin(); it != pBusiList.end(); ++it){
        Business* pBusi = *it;
        if(pBusiHead){
            pBusiTail->pBuffNext = pBusi;
            pBusiTail = pBusi;
        }else{
            pBusiHead = pBusi;
            pBusiTail = pBusiHead;
        }
    }

    pBusiTail->pBuffNext = NULL;
    if(pBusiHead){
        pDataBuffer.RecycleData(pBusiHead);
    }
    count = pBusiList.size();
    pBusiList.clear();
    criSect.Unlock();
    return count;
}

int BusinessData::RollbackData(const TransactionIdx_T tranIdx)
{
    LOG(ERROR) << "[BusinessData::RollbackData]start.tranIdx=" << tranIdx << LOG_END_FLAG;
    TranBusiness* pNewTran = pTranData.GetTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::RollbackData]get tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    std::list<Business*>::reverse_iterator it = std::find_if(pBusiList.rbegin(), pBusiList.rend(), BusinessFinder(pNewTran->business_id));
    if(it.base() != pBusiList.end()){
        pBusiList.erase(it.base());
        Business* p = *(it.base());
        p->pBuffNext = NULL;
        pDataBuffer.RecycleData(p);
    }else{
        LOG(ERROR) << "[BusinessData::RollbackData]get business faild.business_id=" << pNewTran->business_id << LOG_END_FLAG;
        return -1;
    }
    LOG(ERROR) << "[BusinessData::RollbackData]end." << LOG_END_FLAG;
    return 0;
}

Business* BusinessData::AddData_Inner(const Business* pData, const OperateType opType, const TransactionIdx_T tranIdx)
{
    Business* pNewBusi = pDataBuffer.GetData();
    if(!pNewBusi){
        LOG(ERROR) << "[BusinessData::AddData_Inner]get new business faild." << LOG_END_FLAG;
        return NULL;
    }

    TranBusiness* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::AddData_Inner]get new tran data faild." << LOG_END_FLAG;
        pDataBuffer.RecycleData(pNewBusi);
        return NULL;
    }

    memcpy(pNewBusi, pData, sizeof(*pNewBusi));
    pNewBusi->opType = opType;
    pNewTran->business_id = pData->business_id;

    criSect.Lock();
    pBusiList.push_back(pNewBusi);
    criSect.Unlock();

    return pNewBusi;
}
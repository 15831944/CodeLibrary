#include "business.h"
#include <sstream>

Business* BusinessData::InitData(const Business* pData)
{
    return AddData_Inner(pData, OperateTypeInit, 0);
}

Business* BusinessData::AddData(const Business* pData, const TransactionIdx_T tranIdx)
{
    return AddData_Inner(pData, OperateTypeInsert, tranIdx);
}

int BusinessData::GetAllData(list<Business*>& rList)
{
    LOG(INFO) << "[BusinessData::RollbackData]start.pBusiList.size=" << pBusiList.size() << LOG_END_FLAG;
    criSect.Lock();
    rList.clear();
    rList.assign(pBusiList.begin(), pBusiList.end());
    criSect.Unlock();
    LOG(INFO) << "[BusinessData::RollbackData]start.rList.size=" << rList.size() << LOG_END_FLAG;
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
    LOG(INFO) << "[BusinessData::RollbackData]start.tranIdx=" << tranIdx << LOG_END_FLAG;
    TranBusiness* pNewTran = pTranData.GetTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::RollbackData]get tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    LOG(INFO) << "[BusinessData::RollbackData]pBusiList.size=" << pBusiList.size() << LOG_END_FLAG;
    list<Business*>::reverse_iterator it = std::find_if(pBusiList.rbegin(), pBusiList.rend(), BusiFunctorEqual(pNewTran->business_id));

    if(it != pBusiList.rend()){
        //LOG(INFO) << "[BusinessData::RollbackData]erase.business_id=" << (*(it.base()))->business_id << LOG_END_FLAG;
        list<Business*>::iterator i = --it.base();
        Business* p = *i;
        pBusiList.erase(i);
        p->pBuffNext = NULL;
        pDataBuffer.RecycleData(p);
    }else{
        LOG(ERROR) << "[BusinessData::RollbackData]get business faild.business_id=" << pNewTran->business_id << LOG_END_FLAG;
        return -1;
    }
    LOG(INFO) << "[BusinessData::RollbackData]end." << LOG_END_FLAG;
    return 0;
}

Business* BusinessData::AddData_Inner(const Business* pData, const OperateType opType, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[BusinessData::AddData_Inner]start." << LOG_END_FLAG;
    Business* pNewBusi = pDataBuffer.GetData();
    if(!pNewBusi){
        LOG(ERROR) << "[BusinessData::AddData_Inner]get new business faild." << LOG_END_FLAG;
        return NULL;
    }

    TranBusiness* pNewTran = NULL;
    if(tranIdx > 0){
        pNewTran = pTranData.NewTranData(tranIdx);
        if(!pNewTran){
            LOG(ERROR) << "[BusinessData::AddData_Inner]get new tran data faild." << LOG_END_FLAG;
            pDataBuffer.RecycleData(pNewBusi);
            return NULL;
        }
    }
    

    memcpy(pNewBusi, pData, sizeof(*pNewBusi));
    pNewBusi->opType = opType;
    pNewBusi->versionIdx++;

    if(pNewTran) pNewTran->business_id = pData->business_id;

    criSect.Lock();
    pBusiList.push_back(pNewBusi);
    criSect.Unlock();

    LOG(INFO) << "[BusinessData::AddData_Inner]end.pBusiList.size()" << pBusiList.size() << LOG_END_FLAG;
    return pNewBusi;
}


int BusinessData::GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update)
{
    LOG(INFO) << "[BusinessData::GetDbUpdateInfo]start." << LOG_END_FLAG;

    int iCount = 0;
    int iRes = 0;

    criSect.Lock();
    for(list<Business*>::iterator it = pBusiList.begin(); it != pBusiList.end(); ++it){
        Business* pBusi = *it;
        if(OperateTypeInsert == pBusi->opType){
            DBWriteBusi* pDbBusi = new DBWriteBusi();
            pDbBusi->business_id = pBusi->business_id;
            pDbBusi->versionIdx = pBusi->versionIdx;
            std::stringstream ss;
            ss << "insert into business(busin_date, business_id, entrust_id, unit_id, combi_code, "
    << "stock_code, entrustdirection_code, business_price, business_amount, business_time, "
    << "jy_fee, js_fee, yh_fee, gh_fee, qt_fee, "
    << "yj_fee, jg_fee, zg_fee, js2_fee, operator_id, "
    << "batchop_id, current_cash, balance_excludefee, current_amount, businclass_code, "
    << "fund_id, balance, time_stamp, remarks, ext_business_id) values( "
    << pBusi->busin_date << ", " << pBusi->business_id << ", " << pBusi->entrust_id << ", " << pBusi->unit_id << ", '" << pBusi->combi_code << "', '"
    << pBusi->stock_code << "', " << pBusi->entrustdirection_code << ", " << pBusi->business_price << ", " << pBusi->business_amount << ", " << pBusi->business_time << ", "
    << pBusi->jy_fee << ", " << pBusi->js_fee << ", " << pBusi->yh_fee<< ", " << pBusi->gh_fee << ", " << pBusi->qt_fee << ", "
    << pBusi->yj_fee << ", " << pBusi->jg_fee << ", " << pBusi->zg_fee << ", " << pBusi->js2_fee << ", " << pBusi->operator_id << ", "
    << pBusi->batchop_id << ", " << pBusi->current_cash << ", " << pBusi->balance_excludefee << ", " << pBusi->current_amount << ", " << pBusi->businclass_code << ", "
    << pBusi->fund_id << ", " << pBusi->balance << ", systimestamp, 'trade normal', " << pBusi->ext_business_id << ")";
                    pDbBusi->sql.assign(ss.str().c_str());
                    pList.push_back(pDbBusi);

            LOG(INFO) << "[BusinessData::GetDbUpdateInfo]insert: " << pDbBusi->sql << LOG_END_FLAG;
            ++iCount;

            if(iCount >= MAX_DB_WRITE_COUNT){
                LOG(INFO) << "[BusinessData::GetDbUpdateInfo]equal MAX_DB_WRITE_COUNT: all=" << pBusiList.size() << LOG_END_FLAG;
                break;
            }
        }
    }
    criSect.Unlock();

    *insert = iCount;
    *update = 0;
    iRes = iCount;
    LOG(INFO) << "[BusinessData::GetDbUpdateInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return iRes;
}

int BusinessData::DoneDbUpdata(std::list<DBWriteItem*>& pList)
{
    LOG(INFO) << "[BusinessData::DoneDbUpdata]start.size=" << pList.size() << LOG_END_FLAG;

    criSect.Lock();
    for(list<DBWriteItem*>::iterator it = pList.begin(); it != pList.end(); it++){
        DBWriteBusi* pDbBusi = static_cast<DBWriteBusi*>(*it);
        Business* pBusiItem = NULL;

        std::list<Business*>::reverse_iterator rit = std::find_if(pBusiList.rbegin(), pBusiList.rend(), BusiFunctorEqual(pDbBusi->business_id));
        if(rit != pBusiList.rend()){
            list<Business*>::iterator i = --rit.base();
            pBusiItem = *i;

            if(pBusiItem->versionIdx == pDbBusi->versionIdx){
                pBusiItem->opType = OperateTypeInit;

                LOG(INFO) << "[BusinessStore::doneDBUpdate]OperateTypeInit=entIt=" << pBusiItem->business_id << LOG_END_FLAG;
            }else{
                if(pBusiItem->opType == OperateTypeInsert){
                    pBusiItem->opType = OperateTypeUpdate;
                }
            }
        }else{
            LOG(ERROR) << "[BusinessData::DoneDbUpdata]get business faild.business_id=" << pDbBusi->business_id << LOG_END_FLAG;
        }
    }
    criSect.Unlock();

    LOG(INFO) << "[BusinessData::DoneDbUpdata]end." << LOG_END_FLAG;
    return 0;
}
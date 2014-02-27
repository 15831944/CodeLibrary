#include "entrust.h"

int EntrustData::InitData(const Entrust* pData)
{
    return AddData_Inner(pData, OperateTypeInit, 0);
}

int EntrustData::AddData(const Entrust* pData, const TransactionIdx_T tranIdx)
{
    return AddData_Inner(pData, OperateTypeInsert, tranIdx);
}

int EntrustData::AddData_Inner(const Entrust* pData, const OperateType opType, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::AddData_Inner]start.tranIdx=" << tranIdx << LOG_END_FLAG;
    Entrust* pNewEnt = pDataBuffer.GetData();
    if(!pNewEnt){
        LOG(ERROR) << "[EntrustData::AddData_Inner]get new entrust faild." << LOG_END_FLAG;
        return -1;
    }

    TranEntrust* pNewTran = NULL;
    if(tranIdx > 0){
        pNewTran = pTranData.NewTranData(tranIdx);
        if(!pNewTran){
            LOG(ERROR) << "[BusinessData::AddData_Inner]get new tran data faild." << LOG_END_FLAG;
            pDataBuffer.RecycleData(pNewEnt);
            return -2;
        }
    }

    memcpy(pNewEnt, pData, sizeof(*pNewEnt));
    pNewEnt->opType = opType;
    pNewEnt->versionIdx++;

    if(pNewTran){
        pNewTran->entrust_id = pData->entrust_id;
        pNewTran->opType = opType;          //恢复时根据opType来选择操作类型
    }

    criSect.Lock();
    pEntList.push_back(pNewEnt);
    pEntIdMap[pNewEnt->entrust_id] = pNewEnt;
    pRepIdMap[pNewEnt->report_id] = pNewEnt;
    criSect.Unlock();

    LOG(INFO) << "[EntrustData::AddData_Inner]end." << LOG_END_FLAG;
    return 0;
}

Entrust* EntrustData::GetByEntId_Inner(const EntrustId_T entId)
{
    Entrust* pRet = NULL;
    if(pEntIdMap.find(entId) != pEntIdMap.end()){
        pRet = pEntIdMap.at(entId);
    }else{
        LOG(WARNING) << "[UnitData::GetByUnitId]not found data.entId=" << entId << LOG_END_FLAG;
    }
    return pRet;
}

int EntrustData::UpdateEntStatus_Inner(Entrust* pEnt)
{
    if(pEnt->amount == pEnt->cancel_amount){
        pEnt->entrust_status = EntrustStatus_YiChe;
    }else if(pEnt->amount == pEnt->business_amount){
        pEnt->entrust_status = EntrustStatus_YiCheng;
    }else if(pEnt->amount == pEnt->cancel_amount + pEnt->business_amount){
        pEnt->entrust_status = EntrustStatus_BuChe;
    }else if((pEnt->cancel_amount + pEnt->business_amount) == 0){
        pEnt->entrust_status = EntrustStatus_YiBao;
    }else if(pEnt->business_amount == 0){
        pEnt->entrust_status = EntrustStatus_BuChe;
    }else if(pEnt->cancel_amount == 0){
        pEnt->entrust_status = EntrustStatus_BuCheng;
    }else{
        pEnt->entrust_status = EntrustStatus_BuChe;
    }
    return 0;
}

int EntrustData::GetDataByEntId(const EntrustId_T entId, Entrust* pDataOut)
{
    LOG(INFO) << "[EntrustData::GetDataByEntId]start.entId=" << entId << LOG_END_FLAG;
    int iRet = 0;
    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        memcpy(pDataOut, pEnt, sizeof(*pDataOut));
    }else{
        iRet = -1;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::GetDataByEntId]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::GetDataByRepId(const ReportId_T repId, Entrust* pDataOut)
{
    LOG(INFO) << "[EntrustData::GetDataByRepId]start.repId=" << repId << LOG_END_FLAG;
    int iRet = 0;
    criSect.Lock();
    if(pRepIdMap.find(repId) != pRepIdMap.end()){
        Entrust* pEnt = pRepIdMap.at(repId);
        if(pEnt){
            memcpy(pDataOut, pRepIdMap.at(repId), sizeof(*pDataOut));
        }else{
            iRet = -2;
        }
    }else{
        LOG(WARNING) << "[UnitData::GetDataByRepId]not found data.repId=" << repId << LOG_END_FLAG;
        iRet = -1;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::GetDataByRepId]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::GetDataByEntStatus(const std::string entStatus, list<Entrust*>& pListOut)
{
    LOG(INFO) << "[EntrustData::GetDataByEntStatus]start.entStatus=" << entStatus << LOG_END_FLAG;
    int iRet = 0;
    criSect.Lock();
    copy_if(pEntList.begin(), pEntList.end(), pListOut.begin(), EntFunctorEqualStatus(entStatus));
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::GetDataByEntStatus]end.pListOut.size=" << pListOut.size() << LOG_END_FLAG;
    return iRet;
}

int EntrustData::GetAllData(list<Entrust*>& pListOut)
{
    LOG(INFO) << "[EntrustData::GetAllData]start." << LOG_END_FLAG;
    int iRet = 0;
    criSect.Lock();
    copy(pEntList.begin(), pEntList.end(), pListOut.begin());
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::GetAllData]end.pListOut.size=" << pListOut.size() << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntToRevokeInner(const EntrustId_T entId, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntToRevokeInner]start.entId=" << entId << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntToRevokeInner]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->cancel_flag = EntrustCancelFlag_Inner;
        pEnt->entrust_status = EntrustStatus_YiChe;
        pEnt->cancel_amount = pEnt->amount;
        pEnt->prebuy_balance = 0;
        pEnt->presale_balance = 0;
        pEnt->prebuy_fee = 0;
        pEnt->presale_fee = 0;
        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntToRevokeInner]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntStatus(const EntrustId_T entId, const EntStatus_T status, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntToRevokeInner]start.entId=" << entId << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntToRevokeInner]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->entrust_status = status;
        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntToRevokeInner]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntByRevOrder(const EntrustId_T entId, const EntrustId_T cancelId, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntByRevOrder]start.entId=" << entId << ",cancelId=" << cancelId 
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntByRevOrder]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->entrust_status = EntrustStatus_DaiChe;
        pEnt->cancel_id = cancelId;

        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntByRevOrder]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntByReport(const EntrustId_T entId, const ReportId_T repId, const ExtReportId_T exrepId, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntByReport]start.entId=" << entId << ",repId=" << repId << ",exrepId=" << exrepId 
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntByReport]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->report_id = repId;
        pEnt->exreport_id = exrepId;
        pEnt->entrust_status = EntrustStatus_YiBao;

        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntByReport]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateRevByReport(const EntrustId_T entId, const ReportId_T repId, const ExtReportId_T exrepId, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateRevByReport]start.entId=" << entId << ",repId=" << repId << ",exrepId=" << exrepId 
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateRevByReport]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->report_id = repId;
        pEnt->exreport_id = exrepId;
        pEnt->entrust_status = EntrustStatus_CheRen;

        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateRevByReport]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntByWaste(const EntrustId_T entId, const string remark, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntByWaste]start.entId=" << entId << ",remark=" << remark
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntByWaste]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->entrust_status = EntrustStatus_FeiDan;
        snprintf(pEnt->remarks, sizeof(pEnt->remarks), "%s", remark.c_str());

        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntByWaste]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntByRevoke(Entrust* pData, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntByRevoke]start.entrust_id=" << pData->entrust_id 
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[EntrustData::UpdateEntByRevoke]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(pData->entrust_id);
    if(pEnt){
        LOG(INFO) << "[EntrustData::UpdateEntByRevoke]ent. amount=" << pEnt->amount << ",cancel_amount=" << pEnt->cancel_amount
            << ",business_amount=" << pEnt->business_amount << LOG_END_FLAG;

        if(pEnt->amount == pEnt->cancel_amount + pData->cancel_amount){
            pEnt->entrust_status = EntrustStatus_YiChe;
            pEnt->cancel_amount = pEnt->cancel_amount + pData->cancel_amount;

            pData->prebuy_frozen_balance = 0 - pEnt->prebuy_frozen_balance;
            pData->prebuy_balance = 0 - pEnt->prebuy_balance;
            pData->presale_balance = 0 - pEnt->presale_balance;
            pData->prebuy_fee = 0 - pEnt->prebuy_fee;
            pData->presale_fee = 0 - pEnt->presale_fee;

            pEnt->prebuy_frozen_balance = 0;
            pEnt->prebuy_balance = 0;
            pEnt->presale_balance = 0;
            pEnt->prebuy_fee = 0;
            pEnt->presale_fee = 0;
        }else if(pEnt->amount == (pEnt->business_amount + pEnt->cancel_amount + pData->cancel_amount)){
            pEnt->entrust_status = EntrustStatus_BuChe;
            pEnt->cancel_amount = pEnt->cancel_amount + pData->cancel_amount;

            pData->prebuy_frozen_balance = 0 - pEnt->prebuy_frozen_balance;
            pData->prebuy_balance = 0 - pEnt->prebuy_balance;
            pData->presale_balance = 0 - pEnt->presale_balance;
            pData->prebuy_fee = 0 - pEnt->prebuy_fee;
            pData->presale_fee = 0 - pEnt->presale_fee;

            pEnt->prebuy_frozen_balance = 0;
            pEnt->prebuy_balance = 0;
            pEnt->presale_balance = 0;
            pEnt->prebuy_fee = 0;
            pEnt->presale_fee = 0;
        }else if(pEnt->amount > (pEnt->business_amount + pEnt->cancel_amount + pData->cancel_amount)){
            pEnt->entrust_status = EntrustStatus_BuChe;
            pEnt->cancel_amount = pEnt->cancel_amount + pData->cancel_amount;

            pEnt->prebuy_frozen_balance += pData->prebuy_frozen_balance;
            pEnt->prebuy_balance += pData->prebuy_balance;
            pEnt->presale_balance += pData->presale_balance;
            pEnt->prebuy_fee += pData->prebuy_fee;
            pEnt->presale_fee += pData->presale_fee;
        }else{
            iRet = -3;
        }

        if(iRet == 0){
            if(OperateTypeInit == pEnt->opType){
                pEnt->opType = OperateTypeUpdate;
            }
            pEnt->versionIdx++;

            memcpy(pNewTran, pData, sizeof(*pNewTran));
            pNewTran->tranIdx = tranIdx;
            pNewTran->opType = OperateTypeDiff;
        }
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntByRevoke]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntByRevokeWaste(const EntrustId_T entId, const string remark, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntByRevokeWaste]start.entId=" << entId << ",remark=" << remark
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntByRevokeWaste]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        UpdateEntStatus_Inner(pEnt);
    
        snprintf(pEnt->remarks, sizeof(pEnt->remarks), "%s", remark.c_str());

        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntByRevokeWaste]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateRevByRevokeWaste(const EntrustId_T entId, const string remark, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateRevByRevokeWaste]start.entId=" << entId << ",remark=" << remark
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateRevByRevokeWaste]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(entId);
    if(pEnt){
        pEnt->entrust_status = EntrustStatus_CheFei;
        snprintf(pEnt->remarks, sizeof(pEnt->remarks), "%s", remark.c_str());

        if(OperateTypeInit == pEnt->opType){
            pEnt->opType = OperateTypeUpdate;
        }
        pEnt->versionIdx++;

        memcpy(pNewTran, pEnt, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateRevByRevokeWaste]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::UpdateEntByBusiness(Entrust* pData, const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::UpdateEntByBusiness]start.entrust_id=" << pData->entrust_id
        << ",tranIdx=" << tranIdx << LOG_END_FLAG;
    int iRet = 0;

    TranEntrust* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[BusinessData::UpdateEntByBusiness]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(pData->entrust_id);
    if(pEnt){
        //全部成交或全部成交和撤单，即当前成交为最后一笔，全部解冻
        if( (pEnt->amount == pEnt->business_amount + pData->business_amount) 
            || (pEnt->amount == pEnt->business_amount + pEnt->cancel_amount + pData->business_amount) ){
            pData->prebuy_balance = 0 - pEnt->prebuy_balance;
            pData->presale_balance = 0 - pEnt->presale_balance;
            pData->prebuy_fee = 0 - pEnt->prebuy_fee;
            pData->presale_fee = 0 - pEnt->presale_fee;
            pData->prebuy_frozen_balance = 0 - pEnt->prebuy_frozen_balance;

            pEnt->business_amount += pData->business_amount;
            pEnt->business_balance += pData->business_balance;
            pEnt->entrust_status = EntrustStatus_YiCheng;
            pEnt->prebuy_balance = 0;
            pEnt->presale_balance = 0;
            pEnt->prebuy_fee = 0;
            pEnt->presale_fee = 0;
            pEnt->prebuy_frozen_balance = 0;
            iRet = 1; 
        //部分成交
        }else if(pEnt->amount >= pEnt->business_amount + pData->business_amount){
            pEnt->business_amount += pData->business_amount;
            pEnt->business_balance += pData->business_balance;
            pEnt->entrust_status = EntrustStatus_BuCheng;
            pEnt->prebuy_balance += pData->prebuy_balance;
            pEnt->presale_balance += pData->presale_balance;
            pEnt->prebuy_fee += pData->prebuy_fee;
            pEnt->presale_fee += pData->presale_fee;
            pEnt->prebuy_frozen_balance += pData->prebuy_frozen_balance;
            iRet = 2; 
        }else{
            iRet = -3; 
        }

        if(iRet == 0){
            if(OperateTypeInit == pEnt->opType){
                pEnt->opType = OperateTypeUpdate;
            }
            pEnt->versionIdx++;

            memcpy(pNewTran, pData, sizeof(*pNewTran));
            pNewTran->tranIdx = tranIdx;
            pNewTran->opType = OperateTypeDiff;
        }
    }else{
        iRet = -2;
    }
    criSect.Unlock();
    LOG(INFO) << "[EntrustData::UpdateEntByBusiness]end.iRet=" << iRet << LOG_END_FLAG;
    return iRet;
}

int EntrustData::ReleaseData()
{
    int count = 0;
    criSect.Lock();
    Entrust* pEntHead = NULL;
    Entrust* pEntTail = NULL;
    for(std::list<Entrust*>::iterator it = pEntList.begin(); it != pEntList.end(); ++it){
        Entrust* pEnt = *it;
        if(pEntHead){
            pEntTail->pBuffNext = pEnt;
            pEntTail = pEnt;
        }else{
            pEntHead = pEnt;
            pEntTail = pEntHead;
        }
    }

    pEntTail->pBuffNext = NULL;
    if(pEntHead){
        pDataBuffer.RecycleData(pEntHead);
    }
    count = pEntList.size();
    pEntList.clear();
    pEntIdMap.clear();
    pRepIdMap.clear();
    criSect.Unlock();
    return count;
}

int EntrustData::RollbackData(const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[EntrustData::RollbackData]start.tranIdx=" << tranIdx << LOG_END_FLAG;
    TranEntrust* pTran = pTranData.GetTranData(tranIdx);
    if(!pTran){
        LOG(ERROR) << "[EntrustData::RollbackData]get tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    int iRet = 0;
    LOG(INFO) << "[EntrustData::RollbackData]pBusiList.size=" << pEntList.size() << LOG_END_FLAG;
    criSect.Lock();
    Entrust* pEnt = GetByEntId_Inner(pTran->entrust_id);
    if(pEnt){
        if(pTran->opType == OperateTypeUpdate){
            VersionIdx_T iVerIdx = pEnt->versionIdx;
            OperateType opType = pEnt->opType;
            memcpy(pEnt, pTran, sizeof(*pEnt));

            pEnt->versionIdx = iVerIdx + 1;
            if(opType == OperateTypeInit){
                pEnt->opType = OperateTypeUpdate;
            }
        }else if(pTran->opType == OperateTypeInsert){
            list<Entrust*>::iterator it = find_if(pEntList.begin(), pEntList.end(), EntFunctorEqual(pTran->entrust_id));
            if(it != pEntList.end()){
                pEntList.erase(it);
            }else{
                LOG(ERROR) << "[EntrustData::RollbackData]ent not found.pEntList.entrust_id=" << pTran->entrust_id << LOG_END_FLAG;
            }

            hash_map<ReportId_T, Entrust*>::iterator ite = pEntIdMap.find(pEnt->entrust_id);
            if(ite != pEntIdMap.end()){
                pEntIdMap.erase(ite);
            }else{
                LOG(ERROR) << "[EntrustData::RollbackData]ent not found.pEntIdMap.entrust_id=" << pTran->entrust_id << LOG_END_FLAG;
            }
            
            hash_map<EntrustId_T, Entrust*>::iterator itr = pRepIdMap.find(pEnt->entrust_id);
            if(itr != pEntIdMap.end()){
                pRepIdMap.erase(itr);
            }else{
                LOG(ERROR) << "[EntrustData::RollbackData]ent not found.pRepIdMap.entrust_id=" << pTran->entrust_id << LOG_END_FLAG;
            }

            pEnt->pBuffNext = NULL;
            pDataBuffer.RecycleData(pEnt);
            
        }else if(pTran->opType == OperateTypeDiff){
            pEnt->business_amount -= pTran->business_amount;
            pEnt->business_balance -= pTran->business_balance;
            pEnt->prebuy_balance -= pTran->prebuy_balance;
            pEnt->presale_balance -= pTran->presale_balance;
            pEnt->prebuy_fee -= pTran->prebuy_fee;
            pEnt->presale_fee -= pTran->presale_fee;
            pEnt->prebuy_frozen_balance -= pTran->prebuy_frozen_balance;
            UpdateEntStatus_Inner(pEnt);
        }
    }else{
        LOG(ERROR) << "[EntrustData::RollbackData]get business faild.entrust_id=" << pTran->entrust_id << LOG_END_FLAG;
        iRet = -1;
    }
    criSect.Unlock();

    LOG(INFO) << "[EntrustData::RollbackData]end." << LOG_END_FLAG;
    return iRet;
}


int EntrustData::GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update)
{
    LOG(INFO) << "[EntrustData::GetDbUpdateInfo]start." << LOG_END_FLAG;

    int iCount = 0;
    int uCount = 0;
    int iRes = 0;

    criSect.Lock();
    for(list<Entrust*>::iterator it = pEntList.begin(); it != pEntList.end(); ++it){
        Entrust* pEnt = *it;
        if(OperateTypeInsert == pEnt->opType){
            DBWriteEnt* pDbEnt = new DBWriteEnt();
            pDbEnt->entrust_id = pEnt->entrust_id;
            pDbEnt->versionIdx = pEnt->versionIdx;
            std::stringstream ss;
            ss << "insert into entrust(busin_date, batchop_id, entrust_id, report_id, unit_id, "
"combi_code, stock_code, entrustdirection_code, price_type, price, "
"amount, balance, prebuy_frozen_balance, prebuy_balance, presale_balance, prebuy_fee, "
"presale_fee, entrust_status, entrust_time, cancel_flag, cancel_id, "
"cancel_amount, business_amount, business_balance, remarks, operator_id, "
"ip, mac, time_stamp, exreport_id, fund_id, fee) values ("
<< pEnt->busin_date << "," << pEnt->batchop_id << "," << pEnt->entrust_id << ", " << pEnt->report_id << "," << pEnt->unit_id << ","
<< "'" << pEnt->combi_code << "', '" << pEnt->stock_code << "', " << pEnt->entrustdirection_code << ", '" << pEnt->price_type << "', " << pEnt->price << ", "
<< pEnt->amount << ", " << pEnt->balance << ", " << pEnt->prebuy_frozen_balance << ", " << pEnt->prebuy_balance << ", " << pEnt->presale_balance << ", " << pEnt->prebuy_fee << ", "
<< pEnt->presale_fee << ", '" << pEnt->entrust_status << "', " << pEnt->entrust_time << ", '" << pEnt->cancel_flag << "', " << pEnt->cancel_id << ", "
<< pEnt->cancel_amount << ", " << pEnt->business_amount << ", " << pEnt->business_balance << ", '" << pEnt->remarks << "', " << pEnt->operator_id << ", '"
<< pEnt->ip << "', '" << pEnt->mac << "', systimestamp, " << pEnt->exreport_id << ", " 
<< pEnt->fund_id << ", " << pEnt->fee << ")";
            pDbEnt->sql.assign(ss.str().c_str());
            pList.push_back(pDbEnt);

            LOG(INFO) << "[EntrustData::GetDbUpdateInfo]insert: " << pDbEnt->sql << LOG_END_FLAG;
            ++iCount;
        }else if(OperateTypeUpdate == pEnt->opType){
            DBWriteEnt* pDbEnt = new DBWriteEnt();
            pDbEnt->entrust_id = pEnt->entrust_id;
            pDbEnt->versionIdx = pEnt->versionIdx;
            std::stringstream ss;
            ss << "update entrust set busin_date = " << pEnt->busin_date << ", batchop_id = " << pEnt->batchop_id
<< ", report_id = " << pEnt->report_id << ", combi_code = '" << pEnt->combi_code << "' , stock_code = '" << pEnt->stock_code
<< "', entrustdirection_code = " << pEnt->entrustdirection_code << ", price_type = '" << pEnt->price_type 
<< "', price = " << pEnt->price << ", amount = " << pEnt->amount << ", balance = " << pEnt->balance 
<< ", prebuy_frozen_balance = " << pEnt->prebuy_frozen_balance << ", prebuy_balance = " << pEnt->prebuy_balance
<< ", presale_balance = " << pEnt->presale_balance << ", prebuy_fee = " << pEnt->prebuy_fee
<< ", presale_fee = " << pEnt->presale_fee << ", entrust_status = '" << pEnt->entrust_status << "', entrust_time = " << pEnt->entrust_time
<< ", cancel_flag = '" << pEnt->cancel_flag << "', cancel_id = " << pEnt->cancel_id << ", cancel_amount = " << pEnt->cancel_amount
<< ", business_amount = " << pEnt->business_amount << ", business_balance = " << pEnt->business_balance << ", remarks = '" << pEnt->remarks
<< "', operator_id = " << pEnt->operator_id << ", ip = '" << pEnt->ip << "', mac = '" << pEnt->mac << "', exreport_id = " << pEnt->exreport_id
<< " where unit_id = " << pEnt->unit_id << " and entrust_id = " << pEnt->entrust_id;
            pDbEnt->sql.assign(ss.str().c_str());
            pList.push_back(pDbEnt);

            LOG(INFO) << "[EntrustData::GetDbUpdateInfo]update: " << pDbEnt->sql << LOG_END_FLAG;
            ++uCount;

            
        }

        if(iCount + uCount >= MAX_DB_WRITE_COUNT){
            LOG(INFO) << "[EntrustData::GetDbUpdateInfo]equal MAX_DB_WRITE_COUNT: all=" << pEntList.size() << LOG_END_FLAG;
            break;
        }
    }
    criSect.Unlock();

    *insert = iCount;
    *update = uCount;
    iRes = iCount + uCount;
    LOG(INFO) << "[EntrustData::GetDbUpdateInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return iRes;
}

int EntrustData::DoneDbUpdata(std::list<DBWriteItem*>& pList)
{
    LOG(INFO) << "[EntrustData::DoneDbUpdata]start.size=" << pList.size() << LOG_END_FLAG;

    criSect.Lock();
    for(list<DBWriteItem*>::iterator it = pList.begin(); it != pList.end(); it++){
        DBWriteEnt* pDbEnt = static_cast<DBWriteEnt*>(*it);
        Entrust* pEntItem = NULL;

        std::list<Entrust*>::reverse_iterator rit = std::find_if(pEntList.rbegin(), pEntList.rend(), EntFunctorEqual(pDbEnt->entrust_id));
        if(rit != pEntList.rend()){
            list<Entrust*>::iterator i = --rit.base();
            pEntItem = *i;

            if(pEntItem->versionIdx == pDbEnt->versionIdx){
                pEntItem->opType = OperateTypeInit;

                LOG(INFO) << "[EntrustData::DoneDbUpdata]OperateTypeInit=entrust_id=" << pEntItem->entrust_id << LOG_END_FLAG;
            }else{
                if(pEntItem->opType == OperateTypeInsert){
                    pEntItem->opType = OperateTypeUpdate;
                }
            }
        }else{
            LOG(ERROR) << "[EntrustData::DoneDbUpdata]get business faild.entrust_id=" << pEntItem->entrust_id << LOG_END_FLAG;
        }
    }
    criSect.Unlock();

    LOG(INFO) << "[EntrustData::DoneDbUpdata]end." << LOG_END_FLAG;
    return 0;
}
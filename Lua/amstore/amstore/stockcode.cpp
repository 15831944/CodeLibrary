#include "unitstock.h"

int StockData::InitData(const UnitStock* pData)
{
    UnitStock* pNewStock = new UnitStock();
    memcpy(pNewStock, pData, sizeof(*pNewStock));
    pNewStock->opType = OperateTypeInit;
    pNewStock->versionIdx++;

    criSect.Lock();
    pStockList.push_back(pNewStock);
    pCodeMapB[pNewStock->stock_code] = pNewStock;
    criSect.Unlock();
    return 0;
}

int StockData::GetDataByCode(const BS_T bs, const StockCode_T code, UnitStock* pUnitStock)
{
    int iRet = 0;
    criSect.Lock();
    UnitStock* pStock = GetDataByCode_Inner(bs, code);
    if(pStock){
        memcpy(pUnitStock, pStock, sizeof(*pUnitStock));
    }else{
        iRet = -1;
    }
    criSect.Unlock();
    return iRet;
}

int StockData::GetAllData(list<UnitStock*>& pList)
{
    criSect.Lock();
    copy(pStockList.begin(), pStockList.end(), pList.begin());
    criSect.Unlock();
    return 0;
}

int StockData::UpdateDataByEntrust(const UnitStock* pData, const TransactionIdx_T tranIdx)
{
    int iRet = 0;

    TranStock* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[StockData::UpdateDataByEntrust]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    UnitStock* pStock = GetDataByCode_Inner(pData->bs, pData->stock_code);
    if(pStock){
        pStock->prebuy_amount += pStock->prebuy_amount;
        pStock->presale_amount += pStock->presale_amount;
        pStock->prebuy_balance += pStock->prebuy_balance;
        pStock->presale_balance += pStock->presale_balance;

        if(OperateTypeInit == pStock->opType){
            pStock->opType = OperateTypeUpdate;
        }
        pStock->versionIdx++;

        memcpy(pNewTran, pStock, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;

        iRet = 1;
    }else{
        UnitStock *pNewStock = new UnitStock();
        memcpy(pNewStock, pData, sizeof(*pNewStock));

        pNewStock->opType = OperateTypeInsert;
        pNewStock->versionIdx++;

        memcpy(pNewTran, pNewStock, sizeof(*pNewTran));

        iRet = 2;
    }
    criSect.Unlock();
    return iRet;
}

int StockData::UpdateDataByBusiness(const UnitStock* pData, const TransactionIdx_T tranIdx)
{
    int iRet = 0;

    TranStock* pNewTran = pTranData.NewTranData(tranIdx);
    if(!pNewTran){
        LOG(ERROR) << "[StockData::UpdateDataByBusiness]get new tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    criSect.Lock();
    UnitStock* pStock = GetDataByCode_Inner(pData->bs, pData->stock_code);
    if(pStock){
        pStock->current_amount += pData->current_amount;
        pStock->prebuy_amount += pData->prebuy_amount;
        pStock->presale_amount += pData->presale_amount;
        pStock->prebuy_balance += pData->prebuy_balance;
        pStock->presale_balance += pData->presale_balance;
        pStock->buy_amount += pData->buy_amount;
        pStock->sale_amount += pData->sale_amount;
        pStock->buy_balance += pData->buy_balance;
        pStock->sale_balance += pData->sale_balance;
        pStock->buy_fee += pData->buy_fee;
        pStock->sale_fee += pData->sale_fee;

        if(OperateTypeInit == pStock->opType){
            pStock->opType = OperateTypeUpdate;
        }
        pStock->versionIdx++;

        memcpy(pNewTran, pStock, sizeof(*pNewTran));
        pNewTran->tranIdx = tranIdx;
        pNewTran->opType = OperateTypeUpdate;

        iRet = 1;
    }else{
        UnitStock *pNewStock = new UnitStock();
        memcpy(pNewStock, pData, sizeof(*pNewStock));

        pNewStock->opType = OperateTypeInsert;
        pNewStock->versionIdx++;

        memcpy(pNewTran, pNewStock, sizeof(*pNewTran));

        iRet = 2;
    }
    criSect.Unlock();
    return iRet;
}

int StockData::ReleaseData()
{
    int count = 0;
    criSect.Lock();
    Entrust* pEntHead = NULL;
    Entrust* pEntTail = NULL;
    for(std::list<UnitStock*>::iterator it = pStockList.begin(); it != pStockList.end(); ++it){
        UnitStock* pStock = *it;
        delete pStock;
    }
    pStockList.clear();
    pCodeMapB.clear();
    pCodeMapS.clear();
    criSect.Unlock();
    return count;
}

int StockData::RollbackData(const TransactionIdx_T tranIdx)
{
    LOG(INFO) << "[StockData::RollbackData]start.tranIdx=" << tranIdx << LOG_END_FLAG;
    TranStock* pTran = pTranData.GetTranData(tranIdx);
    if(!pTran){
        LOG(ERROR) << "[StockData::RollbackData]get tran data faild.tranIdx=" << tranIdx << LOG_END_FLAG;
        return -1;
    }

    LOG(INFO) << "[StockData::RollbackData]pTran=" << pTran << "stock_code=" << pTran->stock_code 
        << ",current_amount=" << pTran->current_amount << ",prebuy_amount=" << pTran->prebuy_amount 
        << ",presale_amount=" << pTran->presale_amount << ",prebuy_balance=" << pTran->prebuy_balance
        << ",presale_balance=" << pTran->presale_balance << LOG_END_FLAG;

    UnitStock* pStock = GetDataByCode_Inner(pTran->bs, pTran->stock_code);
    if(pStock){
        pStock->current_amount -= pTran->current_amount;
        pStock->prebuy_amount -= pTran->prebuy_amount;
        pStock->presale_amount -= pTran->presale_amount;
        pStock->prebuy_balance -= pTran->prebuy_balance;
        pStock->presale_balance -= pTran->presale_balance;
        pStock->buy_amount -= pTran->buy_amount;
        pStock->sale_amount -= pTran->sale_amount;
        pStock->buy_balance -= pTran->buy_balance;
        pStock->sale_balance -= pTran->sale_balance;
        pStock->buy_fee -= pTran->buy_fee;
        pStock->sale_fee -= pTran->sale_fee;

        if(OperateTypeInit == pStock->opType){
            pStock->opType = OperateTypeUpdate;
        }
        pStock->versionIdx++;
    }else{
        LOG(ERROR) << "[StockData::RollbackData]get unitasset faild." << LOG_END_FLAG;
        return -1;
    }
    LOG(INFO) << "[StockData::RollbackData]end." << LOG_END_FLAG;
    return 0;
}

UnitStock* StockData::GetDataByCode_Inner(const BS_T bs, const StockCode_T code)
{
    UnitStock* pRet = NULL;
    hash_map<string, UnitStock*>::iterator it = pCodeMapB.find(code);
    if(it != pCodeMapB.end()){
        pRet = pCodeMapB.at(code);
    }else{
        LOG(WARNING) << "[StockData::GetDataByCode_Inner]not found.code=" << code << LOG_END_FLAG;
    }
    return pRet;
}


int StockData::GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update)
{
    LOG(INFO) << "[StockData::GetDbUpdateInfo]start." << LOG_END_FLAG;

    int iCount = 0;
    int uCount = 0;
    int iRes = 0;

    criSect.Lock();
    for(list<UnitStock*>::iterator it = pStockList.begin(); it != pStockList.end(); ++it){
        UnitStock* pStock = *it;
        if(OperateTypeInsert == pStock->opType){
            DBWriteStock* pDbStock = new DBWriteStock();
            pDbStock->bs = pStock->bs;
            snprintf(pDbStock->stock_code, sizeof(pDbStock->stock_code), "%s", pStock->stock_code);
            pDbStock->versionIdx = pStock->versionIdx;
            std::stringstream ss;
            ss << "insert into unitstock(busin_date, unit_id, stock_code, bs, begin_amount, "
<< "begin_cost, begin_total_profit, begin_total_buyfee, begin_total_salefee, current_amount, "
<< "prebuy_amount, presale_amount, prebuy_balance, presale_balance, buy_amount, "
<< "sale_amount, buy_balance, sale_balance, buy_fee, sale_fee, dividend_balance, bonusshare_amount) values( "
<< pStock->busin_date << "," << pStock->unit_id << ",'" << pStock->stock_code << "','" << pStock->bs << "'," << pStock->begin_amount << ","
<< pStock->begin_cost << "," << pStock->begin_total_profit << "," << pStock->begin_total_buyfee << "," << pStock->begin_total_salefee << "," 
<< pStock->current_amount << "," << pStock->prebuy_amount << "," << pStock->presale_amount << "," << pStock->prebuy_balance << "," 
<< pStock->presale_balance << "," << pStock->buy_amount << "," << pStock->sale_amount << "," << pStock->buy_balance << "," 
<< pStock->sale_balance << "," << pStock->buy_fee << "," << pStock->sale_fee << ", 0, 0)";
            pDbStock->sql.assign(ss.str().c_str());
            pList.push_back(pDbStock);

            LOG(INFO) << "[StockData::GetDbUpdateInfo]insert: " << pDbStock->sql << LOG_END_FLAG;
            ++iCount;
        }else if(OperateTypeUpdate == pStock->opType){
            DBWriteStock* pDbStock = new DBWriteStock();
            pDbStock->bs = pStock->bs;
            snprintf(pDbStock->stock_code, sizeof(pDbStock->stock_code), "%s", pStock->stock_code);
            pDbStock->versionIdx = pStock->versionIdx;
            std::stringstream ss;
            ss << "update unitstock set begin_amount = " << pStock->begin_amount << ", begin_cost = "
<< pStock->begin_cost << ", begin_total_profit = " << pStock->begin_total_profit << ", begin_total_buyfee = "
<< pStock->begin_total_buyfee << ", begin_total_salefee = " << pStock->begin_total_salefee << ", current_amount = "
<< pStock->current_amount << ", prebuy_amount = " << pStock->prebuy_amount << ", presale_amount = "
<< pStock->presale_amount << ", prebuy_balance = " << pStock->prebuy_balance << ", presale_balance = "
<< pStock->buy_balance << ", buy_amount = " << pStock->buy_amount << ", sale_amount = "
<< pStock->sale_amount << ", buy_balance = " << pStock->buy_balance << ", sale_balance = "
<< pStock->sale_balance << ", buy_fee = " << pStock->buy_fee << ", sale_fee = "
<< pStock->sale_fee << " where busin_date = " << pStock->busin_date << " and unit_id = " << pStock->unit_id
<< " and stock_code = '" << pStock->stock_code << "' and bs = '" << pStock->bs << "'";
            pDbStock->sql.assign(ss.str().c_str());
            pList.push_back(pDbStock);

            LOG(INFO) << "[StockData::GetDbUpdateInfo]update: " << pDbStock->sql << LOG_END_FLAG;
            ++uCount;
        }

        if(iCount + uCount >= MAX_DB_WRITE_COUNT){
            LOG(INFO) << "[StockData::GetDbUpdateInfo]equal MAX_DB_WRITE_COUNT: all=" << pStockList.size() << LOG_END_FLAG;
            break;
        }
    }
    criSect.Unlock();

    *insert = iCount;
    *update = uCount;
    iRes = iCount + uCount;
    LOG(INFO) << "[StockData::GetDbUpdateInfo]end.iRes=" << iRes << LOG_END_FLAG;
    return iRes;
}

int StockData::DoneDbUpdata(std::list<DBWriteItem*>& pList)
{
    LOG(INFO) << "[StockData::DoneDbUpdata]start.size=" << pList.size() << LOG_END_FLAG;

    criSect.Lock();
    for(list<DBWriteItem*>::iterator it = pList.begin(); it != pList.end(); it++){
        DBWriteStock* pDbStock = static_cast<DBWriteStock*>(*it);
        UnitStock* pStockItem = GetDataByCode_Inner(pDbStock->bs, pDbStock->stock_code);
        if(pStockItem){
            if(pStockItem->versionIdx == pDbStock->versionIdx){
                pStockItem->opType = OperateTypeInit;

                LOG(INFO) << "[StockData::DoneDbUpdata]OperateTypeInit=stock_code=" << pStockItem->stock_code << LOG_END_FLAG;
            }else{
                if(pStockItem->opType == OperateTypeInsert){
                    pStockItem->opType = OperateTypeUpdate;
                }
            }
        }else{
            LOG(ERROR) << "[StockData::DoneDbUpdata]get business faild.stock_code=" << pStockItem->stock_code << LOG_END_FLAG;
        }
    }
    criSect.Unlock();

    LOG(INFO) << "[StockData::DoneDbUpdata]end." << LOG_END_FLAG;
    return 0;
}
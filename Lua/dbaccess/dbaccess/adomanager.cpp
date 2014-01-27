#include <baratol.h>
#include "adomanager.h"
#include "adolib/adolib.h"
#include "adolib/adodblib.h"

#include "adolib/adodbaccess.h"
#ifdef __WINDOWS
	#include "adolib/otl_db_oracle.h"
#endif
extern "C" {
	#include <mysql/mysql.h>
}

AdoManager* G_AdoMang = NULL;

int InitAdoManager()
{
	if(NULL == G_AdoMang){
		G_AdoMang = new AdoManager();
	}
	return 0;
}

//AdoManager实现--begin
AdoManager& AdoManager::GetInstance()
{
	return *G_AdoMang;
}

AdoItem* AdoManager::CreateItem(const CHAR* pszConnection, int nType, int nTimeOut)
{
	CBaseData* pLib;
	if (nType == TYPE_DB_SQLSERVER){
		pLib = new CAdoDBLibSql(nTimeOut);
	}else if (nType == TYPE_DB_ORACLE){
#ifdef __WINDOWS
		pLib = new CAdoDBLibOra();
#else		//??? linux下连oracle对象未实现 Add by Mini.J @20110128
		pLib = NULL;
		assert(false);
#endif
	}else if (nType == TYPE_DB_MYSQL){
		pLib = new CMysqlLib();
	}else{
		assert(false);
	}

	// 连接数据库
	BOOL bOpen = FALSE;
	bOpen = pLib->Open(pszConnection);

	AdoItem* pItem = NULL;
	if (bOpen){
		baratol::CSingleLock sLock(lock);
		sLock.Lock();
		pItem = new AdoItem();
		pItem->pDb = pLib;
		pItem->status = AIS_Open;
		pItemList->AddTail(pItem);
		sLock.Unlock();
	}else{
		baratol::CSingleLock sLock(lockStr);
		sLock.Lock();
		(*strLastError) = pLib->GetLastError();
		sLock.Unlock();
		delete pLib;
		return 0;
	}
	return pItem;
}

int AdoManager::FreeItem(const TAdoItemId iItemId)
{
	BOOL bRet = FALSE;
	POSITION pos;
	baratol::CSingleLock sLock(lock);
	sLock.Lock();
	pos = pItemList->Find((void *)iItemId);
	if(pos != NULL){
		pItemList->RemoveAt(pos);	
		AdoItem *pItem = (AdoItem *)iItemId;
		CBaseData* pData = pItem->pDb;
		delete pData;
		delete pItem;
		bRet = TRUE;
	}
	sLock.Unlock();
	return bRet;		
}

TAdoItemId GetIdleConnection()
{
	TAdoItemId res = 0;
	baratol::CSingleLock sLock(lock);
	sLock.Lock();

	POSITION pos = pItemList->GetHeadPosition();
	while(pos != NULL) {
		AdoItem* pItem = (AdoItem *)pItemList->GetNext(pos);
		if(pItem != NULL){
			if (pItem->status == AIS_Open){
                res = (TAdoItemId)pItem;
				break;
            }
        }
	}

	//..todo若获取失败，可回收垃圾链接或再创建一个链接，并返回

	sLock.Unlock();
	return res;	
}

TAdoItemId AdoManager::BeginTransaction()
{
	TAdoItemId res = 0;
	baratol::CSingleLock sLock(lock);
	sLock.Lock();

	POSITION pos = pItemList->GetHeadPosition();
	while(pos != NULL) {
		AdoItem* pItem = (AdoItem *)pItemList->GetNext(pos);
		if(pItem != NULL){
			if (pItem->status == AIS_Open){
                pItem->status = AIS_Begin;
                res = (TAdoItemId)pItem;
				break;
            }
        }
	}

	//..todo若获取失败，可回收垃圾链接或再创建一个链接，并返回

	sLock.Unlock();
	return res;
}

int AdoManager::CommitTransaction(const TAdoItemId iItemId)
{
	int iRet = 0;
	POSITION pos;
	baratol::CSingleLock sLock(lock);
	sLock.Lock();
	pos = pItemList->Find((void *)iItemId);
	if(pos != NULL){
		AdoItem *pItem = (AdoItem *)iItemId;
		CBaseData* pData = pItem->pDb;
        pItem->status = AIS_Open;
	}else{
		iRet = 1;
	}
	sLock.Unlock();
	return iRet;	
}

int AdoManager::RollbackTransaction(const TAdoItemId iItemId)
{
	return CommitTransaction(iItemId);
}

BOOL AdoManager::RunCmdHDFile(const TAdoItemId iItemId, const TCHAR* strCommand, CHDFile& hdfile)
{
	baratol::CSingleLock sLock(lock);
	sLock.Lock();
	if(!pItemList->Find((void *)iItemId))		// 多线程可能出问题 Add by Mini.J @20110124
		return FALSE;
	sLock.Unlock();

	AdoItem *pItem = (AdoItem *)iItemId;
	CBaseData* pData = pItem->pDb;
	return pData->RunCmd(strCommand, hdfile);
}

BOOL AdoManager::RunCmdCallback(const TAdoItemId iItemId, const TCHAR* strCommand, pAdoCallback pf, void* pUserData, int32* pAffectRows)
{
	baratol::CSingleLock sLock(lock);
	sLock.Lock();
	if(!pItemList->Find((void *)iItemId))		// 多线程可能出问题 Add by Mini.J @20110124
		return FALSE;
	sLock.Unlock();

	AdoItem *pItem = (AdoItem *)iItemId;
	CBaseData* pData = pItem->pDb;
	return pData->RunCmdCallback(strCommand, pf, pUserData, pAffectRows);
}

const char* AdoManager::GetSessionError(const TAdoItemId iItemId)
{
	if(!pItemList->Find((void *)iItemId))
		return (*strLastError);

	AdoItem *pItem = (AdoItem *)iItemId;
	CBaseData* pData = pItem->pDb;
	return pData->GetLastError();	
}
//AdoManager实现--end
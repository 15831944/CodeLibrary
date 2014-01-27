#ifndef _ADO_MANAGER_
#define _ADO_MANAGER_

#ifdef _SUPPORT_ADOLIB

#include <baratol.h>
#include "adolib/adolib.h"

typedef enum TAdoItemStatus{
    AIS_Init = 0,
	AIS_Open = 1,
	AIS_Begin = 2,
	AIS_Close = 3
}AdoItemStatus;

class AdoItem
{
public:
	AdoItem():pDb(NULL), status(AIS_Init){};

	CBaseData* pDb;
	AdoItemStatus status;
};

typedef long TAdoItemId;

class AdoManager
{
public:
	AdoManager(){
		pItemList = new baratol::CPtrList();
		lock = new baratol::CMutex();
		strLastError = new baratol::CString();
		lockStr = new baratol::CMutex();
	}

	~AdoManager(){
		if (pItemList != NULL){
			pItemList->RemoveAll();
			delete pItemList;
		}
		if (lock != NULL)delete lock;
		if (strLastError != NULL)delete strLastError;
	}
	static AdoManager& GetInstance();
	
	AdoItem* CreateItem(const CHAR* pszConnection, int nType, int nTimeOut = 0);
	int FreeItem(const TAdoItemId iItemId);

	TAdoItemId GetIdleConnection();
	TAdoItemId BeginTransaction();
	int CommitTransaction(const TAdoItemId iItemId);
	int RollbackTransaction(const TAdoItemId iItemId);

	BOOL RunCmdHDFile(const TAdoItemId iItemId, const TCHAR* strCommand, CHDFile& hdfile);
	BOOL RunCmdCallback(const TAdoItemId iItemId, const TCHAR* strCommand, pAdoCallback pf, void* pUserData, int32* pAffectRows);
	const char* GetSessionError(const TAdoItemId iItemId);
private:
	baratol::CPtrList* pItemList;
	baratol::CMutex* lock;
	baratol::CString* strLastError;
	baratol::CMutex* lockStr;
};

int InitAdoManager();

#endif //_SUPPORT_ADOLIB
#endif //_ADO_MANAGER_
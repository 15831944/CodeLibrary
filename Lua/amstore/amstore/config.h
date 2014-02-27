#ifndef _AM_CONFIG_H
#define _AM_CONFIG_H

#include "luautil/luautil.h"
#include "sys/types.h"

const int32_t MAX_DB_WRITE_COUNT = 500;
const int32_t TRAN_DATA_COUNT = 2000;


const char EntrustCancelFlag_No = '0';       //正常委托
const char EntrustCancelFlag_Normal = '1';   //外部撤单
const char EntrustCancelFlag_Inner = '2';     //内部撤单

const char EntrustStatus_WeiBao = '1';
const char EntrustStatus_DaiBao = '2';
const char EntrustStatus_ZhengBao = '3';
const char EntrustStatus_YiBao = '4';
const char EntrustStatus_FeiDan = '5';
const char EntrustStatus_BuCheng = '6';
const char EntrustStatus_YiCheng = '7';
const char EntrustStatus_BuChe = '8';
const char EntrustStatus_YiChe = '9';
const char EntrustStatus_DaiChe = 'a';
const char EntrustStatus_WeiChe = 'A';
const char EntrustStatus_DaiChe_R = 'B';
const char EntrustStatus_ZhengChe = 'C';
const char EntrustStatus_CheRen = 'D';
const char EntrustStatus_CheFei = 'E';
const char EntrustStatus_YiChe_R = 'F';


const int32_t UNITCODE_LEN = 32;
const int32_t UNITNAME_LEN = 64;
const int32_t FUNDCODE_LEN = 32;
const int32_t FUNDNAME_LEN = 64;
const int32_t PASSWORD_LEN = 32;
const int32_t BROKEROFFICE_CODE_LEN = 32;
const int32_t BROKER_CODE_LEN = 32;
const int32_t BROKER_NAME_LEN = 64;
const int32_t IP_LEN = 32;
const int32_t OPERATOR_NAME_LEN = 64;
const int32_t INSTITUTION_NAME_LEN = 64;
const int32_t CTS_VERSION_LEN = 64;
const int32_t STOCKACCOUNT_LEN = 64;

typedef int32_t GenericId_T;
typedef int32_t UnitId_T;
typedef int32_t FundId_T;
typedef int32_t InstitutionId_T;
typedef int32_t OperatorId_T;
typedef int32_t TransactionIdx_T;
typedef int32_t VersionIdx_T;
typedef int32_t BusinessId_T;
typedef int32_t ExtBusinessId_T;
typedef int32_t EntrustId_T;
typedef int32_t ReportId_T;
typedef int32_t ExtReportId_T;
typedef int32_t MarketCode_T;

typedef char UnitCode_T[UNITCODE_LEN];
typedef char UnitName_T[UNITNAME_LEN];
typedef char FundCode_T[FUNDCODE_LEN];
typedef char FundName_T[FUNDNAME_LEN];
typedef char Password_T[PASSWORD_LEN];
typedef char BrokerofficeCode_T[BROKEROFFICE_CODE_LEN];
typedef char BrokerCode_T[BROKER_CODE_LEN];
typedef char BrokerName_T[BROKER_NAME_LEN];
typedef char OperatorName_T[OPERATOR_NAME_LEN];
typedef char IP_T[IP_LEN];
typedef char InstitutionName_T[INSTITUTION_NAME_LEN];
typedef char CtsVersion_T[CTS_VERSION_LEN];
typedef char StockAccount_T[STOCKACCOUNT_LEN];

typedef double Currency_T;
typedef double Price_T;
typedef int32_t Amount_T;
typedef int32_t EntDirCode_T;
typedef char PriceType_T;
typedef char EntStatus_T;
typedef char CombiCode_T[129];
typedef char StockCode_T[65];
typedef char StockSpell_T[65];
typedef char StockFullName_T[265];
typedef char ReportCode_T[65];
typedef char StockName_T[265];
typedef char StockTypeName_T[265];
typedef char MarketName_T[265];
typedef char Summary_T[4001];
typedef char MAC_T[31];
typedef char BS_T;
typedef char CurrencyCode_T[4];
typedef char Remarks_T[513];

#endif
#ifndef _AM_TYPEDEF_H
#define _AM_TYPEDEF_H

#include "sys/types.h"
#include "config.h"

typedef enum TOperateType{
    OperateTypeInit = 0,
    OperateTypeUpdate = 1,
    OperateTypeInsert = 2
}OperateType;

typedef struct TUnitBaseData
{
    UnitId_T unit_id;
    UnitCode_T unit_code;
    UnitName_T unit_name;
    FundId_T fund_id;
    FundCode_T fund_code;
    FundName_T fund_name;
    int32_t busin_date;
    int32_t prebusin_date;
    BrokerofficeCode_T brokeroffice_code;
    InstitutionId_T institution_id;
    InstitutionName_T institution_name;
    OperatorId_T operator_id;
    OperatorName_T operator_name;
} UnitBaseData;


typedef struct TFundBaseData
{
    FundId_T fund_id;
    Password_T trade_password;
    Password_T comm_password;
    BrokerofficeCode_T brokeroffice_code;
    BrokerCode_T broker_code;
    IP_T trade_ip;
    int32_t trade_port;
    IP_T query_ip;
    int32_t query_port;
    BrokerName_T broker_name;
    CtsVersion_T cts_version;
    StockAccount_T stockaccount_sh;
    StockAccount_T stockaccount_sz;
} FundBaseData;

typedef struct Entrust{
    int32_t busin_date;
    EntrustId_T entrust_id;
    UnitId_T unit_id;
    EntrustId_T batchop_id;
    ReportId_T report_id;
    CombiCode_T combi_code;
    StockCode_T stock_code;
    EntDirCode_T entrustdirection_code;
    PriceType_T price_type;
    Price_T price;
    Amount_T amount;
    Currency_T balance;
    Currency_T prebuy_balance;
    Currency_T presale_balance;
    Currency_T prebuy_fee;
    Currency_T presale_fee;
    Currency_T prebuy_frozen_balance;
    EntStatus_T entrust_status;
    int32_t entrust_time;
    char cancel_flag;
    EntrustId_T cancel_id;
    Amount_T cancel_amount;
    Amount_T business_amount;
    Currency_T business_balance;
    Remarks_T remarks;
    OperatorId_T operator_id;
    struct tm ent_time_stamp;       //调用时不传递
    IP_T ip;
    MAC_T mac;
    ExtReportId_T exreport_id;
    MarketCode_T market_code;
    FundId_T fund_id;
    Currency_T fee;

    OperateType opType;
    VersionIdx_T versionIdx;
    TransactionIdx_T tranIdx;
    struct Entrust* pBuffNext;
}Entrust;

typedef struct TBusiness{
    int32_t busin_date;
    BusinessId_T batchop_id;
    BusinessId_T business_id;
    ExtBusinessId_T ext_business_id;
    int businclass_code;
    FundId_T fund_id;
    UnitId_T unit_id;
    CombiCode_T combi_code;
    StockCode_T stock_code;
    Currency_T balance;
    Currency_T current_cash;
    Currency_T balance_excludefee;
    Amount_T business_amount;
    Amount_T current_amount;
    Price_T business_price;
    int32_t business_time;
    EntrustId_T entrust_id;
    EntDirCode_T entrustdirection_code;
    Currency_T jy_fee;
    Currency_T js_fee;
    Currency_T yh_fee;
    Currency_T gh_fee;
    Currency_T qt_fee;
    Currency_T yj_fee;
    Currency_T jg_fee;
    Currency_T zg_fee;
    Currency_T js2_fee;
    OperatorId_T operator_id;
    struct tm time_stamp;          //调用时不传递
    Summary_T remarks;           //调用时不传递

    OperateType opType;
    VersionIdx_T versionIdx;
    struct TBusiness* pBuffNext;
} Business;

typedef struct UnitAsset{
    int32_t busin_date;
    UnitId_T unit_id;
    CurrencyCode_T currency_code;
    Currency_T begin_cash;
    Currency_T current_cash;
    Currency_T prebuy_balance;
    Currency_T prebuy_fee;
    Currency_T presale_balance;
    Currency_T presale_fee;
    Currency_T input_balance;
    Currency_T output_balance;
    Currency_T input_total;
    Currency_T output_total;

    OperateType opType;
    VersionIdx_T versionIdx;
    TransactionIdx_T tranIdx;
};

typedef struct TranBusiness{
    TransactionIdx_T tranIdx;
    BusinessId_T business_id;
}TranBusiness;

typedef UnitAsset TranAsset;

typedef Entrust TranEntrust;

typedef struct DBWriteItem{
    int versionIdx;
    std::string sql;

    DBWriteItem():versionIdx(0){}
}DBWriteItem;

typedef struct DBWriteBusi : public DBWriteItem{
    BusinessId_T business_id;

    DBWriteBusi():business_id(0){}
}DBWriteBusi;

typedef struct DBWriteAsset: public DBWriteItem{
    CurrencyCode_T currency_code;

    DBWriteAsset(){memset(currency_code, 0, sizeof(currency_code));};
}DBWriteAsset;

#endif
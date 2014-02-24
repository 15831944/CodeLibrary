#ifndef _AM_TYPEDEF_H
#define _AM_TYPEDEF_H

#include "sys/types.h"
#include "config.h"

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


#endif
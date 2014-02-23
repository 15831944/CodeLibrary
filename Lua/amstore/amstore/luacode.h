#ifndef _AM_LUACODE_H
#define _AM_LUACODE_H

#include "luautil/luautil.h"
#include "config.h"
#include "typedef.h"

//UnitBaseData
#define LUAUNPACK_UNITBASE(L, p) \
    (p)->unit_id = PAI(L,1); \
    CS((p)->unit_code, L, 2); \
    CS((p)->unit_name, L, 3); \
    (p)->fund_id = PAI(L,4); \
    CS((p)->fund_code, L, 5); \
    CS((p)->fund_name, L, 6); \
    (p)->busin_date = PAI(L,7); \
    (p)->prebusin_date = PAI(L,8); \
    CS((p)->brokeroffice_code, L, 9); \
    (p)->institution_id = PAI(L,10); \
    CS((p)->institution_name, L, 11); \
    (p)->operator_id = PAI(L,12); \
    CS((p)->operator_name, L, 13)

#define LUAPACK_UNITBASE(L, p) \
    PINT(L, (p)->unit_id); \
    PS(L, (p)->unit_code); \
    PS(L, (p)->unit_name); \
    PINT(L, (p)->fund_id); \
    PS(L, (p)->fund_code); \
    PS(L, (p)->fund_name); \
    PINT(L, (p)->busin_date); \
    PINT(L, (p)->prebusin_date); \
    PS(L, (p)->brokeroffice_code); \
    PINT(L, (p)->institution_id); \
    PS(L, (p)->institution_name); \
    PINT(L, (p)->operator_id); \
    PS(L, (p)->operator_name)


//FundBaseData
#define LUAUNPACK_FUNDLINK(L, p) \
    (p)->fund_id = PAI(L,2); \
    CS((p)->trade_password, L, 3); \
    CS((p)->comm_password, L, 4); \
    CS((p)->brokeroffice_code, L, 5); \
    CS((p)->broker_code, L, 6); \
    CS((p)->trade_ip, L, 7); \
    (p)->trade_port = PAI(L,8); \
    CS((p)->query_ip, L, 9); \
    (p)->query_port = PAI(L,10); \
    CS((p)->broker_name, L, 11); \
    CS((p)->cts_version, L, 12); \
    CS((p)->stockaccount_sh, L, 13); \
    CS((p)->stockaccount_sz, L, 14)

#define LUAPACK_FUNDLINK(L, p) \
    PINT(L, (p)->fund_id); \
    PS(L, (p)->trade_password); \
    PS(L, (p)->comm_password); \
    PS(L, (p)->brokeroffice_code); \
    PS(L, (p)->broker_code); \
    PS(L, (p)->trade_ip); \
    PINT(L, (p)->trade_port); \
    PS(L, (p)->query_ip); \
    PINT(L, (p)->query_port); \
    PS(L, (p)->broker_name); \
    PS(L, (p)->cts_version); \
    PS(L, (p)->stockaccount_sh); \
    PS(L, (p)->stockaccount_sz)


#endif
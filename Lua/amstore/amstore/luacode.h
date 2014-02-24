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

#define LUAPACK_UNITBASE(L, p) lua_newtable(L); \
     PINT(L, 1); PINT(L, (p)->unit_id); lua_settable(L, -3); \
     PINT(L, 2); PS(L, (p)->unit_code); lua_settable(L, -3); \
     PINT(L, 3); PS(L, (p)->unit_name); lua_settable(L, -3); \
     PINT(L, 4); PINT(L, (p)->fund_id); lua_settable(L, -3); \
     PINT(L, 5); PS(L, (p)->fund_code); lua_settable(L, -3); \
     PINT(L, 6); PS(L, (p)->fund_name); lua_settable(L, -3); \
     PINT(L, 7); PINT(L, (p)->busin_date); lua_settable(L, -3); \
     PINT(L, 8); PINT(L, (p)->prebusin_date); lua_settable(L, -3); \
     PINT(L, 9); PS(L, (p)->brokeroffice_code); lua_settable(L, -3); \
     PINT(L, 10); PINT(L, (p)->institution_id); lua_settable(L, -3); \
     PINT(L, 11); PS(L, (p)->institution_name); lua_settable(L, -3); \
     PINT(L, 12); PINT(L, (p)->operator_id); lua_settable(L, -3); \
     PINT(L, 13); PS(L, (p)->operator_name); lua_settable(L, -3)


//FundBaseData
#define LUAUNPACK_FUNDLINK(L, p) \
    (p)->fund_id = PAI(L,1); \
    CS((p)->trade_password, L, 2); \
    CS((p)->comm_password, L, 3); \
    CS((p)->brokeroffice_code, L, 4); \
    CS((p)->broker_code, L, 5); \
    CS((p)->trade_ip, L, 6); \
    (p)->trade_port = PAI(L,7); \
    CS((p)->query_ip, L, 8); \
    (p)->query_port = PAI(L,9); \
    CS((p)->broker_name, L, 10); \
    CS((p)->cts_version, L, 11); \
    CS((p)->stockaccount_sh, L, 12); \
    CS((p)->stockaccount_sz, L, 13)

#define LUAPACK_FUNDLINK(L, p) lua_newtable(L); \
    PINT(L, 1); PINT(L, (p)->fund_id); lua_newtable(L); \
    PINT(L, 2); PS(L, (p)->trade_password); lua_newtable(L); \
    PINT(L, 3); PS(L, (p)->comm_password); lua_newtable(L); \
    PINT(L, 4); PS(L, (p)->brokeroffice_code); lua_newtable(L); \
    PINT(L, 5); PS(L, (p)->broker_code); lua_newtable(L); \
    PINT(L, 6); PS(L, (p)->trade_ip); lua_newtable(L); \
    PINT(L, 7); PINT(L, (p)->trade_port); lua_newtable(L); \
    PINT(L, 8); PS(L, (p)->query_ip); lua_newtable(L); \
    PINT(L, 9); PINT(L, (p)->query_port); lua_newtable(L); \
    PINT(L, 10); PS(L, (p)->broker_name); lua_newtable(L); \
    PINT(L, 11); PS(L, (p)->cts_version); lua_newtable(L); \
    PINT(L, 12); PS(L, (p)->stockaccount_sh); lua_newtable(L); \
    PINT(L, 13); PS(L, (p)->stockaccount_sz); lua_newtable(L)


#endif
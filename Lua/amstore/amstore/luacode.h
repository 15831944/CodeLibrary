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
    PINT(L, 1); PINT(L, (p)->fund_id); lua_settable(L, -3); \
    PINT(L, 2); PS(L, (p)->trade_password); lua_settable(L, -3); \
    PINT(L, 3); PS(L, (p)->comm_password); lua_settable(L, -3); \
    PINT(L, 4); PS(L, (p)->brokeroffice_code); lua_settable(L, -3); \
    PINT(L, 5); PS(L, (p)->broker_code); lua_settable(L, -3); \
    PINT(L, 6); PS(L, (p)->trade_ip); lua_settable(L, -3); \
    PINT(L, 7); PINT(L, (p)->trade_port); lua_settable(L, -3); \
    PINT(L, 8); PS(L, (p)->query_ip); lua_settable(L, -3); \
    PINT(L, 9); PINT(L, (p)->query_port); lua_settable(L, -3); \
    PINT(L, 10); PS(L, (p)->broker_name); lua_settable(L, -3); \
    PINT(L, 11); PS(L, (p)->cts_version); lua_settable(L, -3); \
    PINT(L, 12); PS(L, (p)->stockaccount_sh); lua_settable(L, -3); \
    PINT(L, 13); PS(L, (p)->stockaccount_sz); lua_settable(L, -3)


//----entrust----
#define LUAPACK_ENTRUST_TABLE(L, p) lua_newtable(L); \
    PINT(L, 1); PINT(L, (p)->busin_date); lua_settable(L, -3); \
    PINT(L, 2); PINT(L, (p)->entrust_id); lua_settable(L, -3); \
    PINT(L, 3); PINT(L, (p)->unit_id); lua_settable(L, -3); \
    PINT(L, 4); PINT(L, (p)->batchop_id); lua_settable(L, -3); \
    PINT(L, 5); PINT(L, (p)->report_id); lua_settable(L, -3); \
    PINT(L, 6); PS(L, (p)->combi_code); lua_settable(L, -3); \
    PINT(L, 7); PS(L, (p)->stock_code); lua_settable(L, -3); \
    PINT(L, 8); PINT(L, (p)->entrustdirection_code); lua_settable(L, -3); \
    PINT(L, 9); PINT(L, (int)((p)->price_type)); lua_settable(L, -3); \
    PINT(L, 10); PN(L, (p)->price); lua_settable(L, -3); \
    PINT(L, 11); PN(L, (p)->amount); lua_settable(L, -3); \
    PINT(L, 12); PN(L, (p)->balance); lua_settable(L, -3); \
    PINT(L, 13); PN(L, (p)->prebuy_balance); lua_settable(L, -3); \
    PINT(L, 14); PN(L, (p)->presale_balance); lua_settable(L, -3); \
    PINT(L, 15); PN(L, (p)->prebuy_fee); lua_settable(L, -3); \
    PINT(L, 16); PN(L, (p)->presale_fee); lua_settable(L, -3); \
    PINT(L, 17); PN(L, (p)->prebuy_frozen_balance); lua_settable(L, -3); \
    PINT(L, 18); PINT(L, (int)((p)->entrust_status)); lua_settable(L, -3); \
    PINT(L, 19); PINT(L, (p)->entrust_time); lua_settable(L, -3); \
    PINT(L, 20); PINT(L, (int)((p)->cancel_flag)); lua_settable(L, -3); \
    PINT(L, 21); PINT(L, (p)->cancel_id); lua_settable(L, -3); \
    PINT(L, 22); PINT(L, (p)->cancel_amount); lua_settable(L, -3); \
    PINT(L, 23); PINT(L, (p)->business_amount); lua_settable(L, -3); \
    PINT(L, 24); PN(L, (p)->business_balance); lua_settable(L, -3); \
    PINT(L, 25); PS(L, (p)->remarks); lua_settable(L, -3); \
    PINT(L, 26); PINT(L, (p)->operator_id); lua_settable(L, -3); \
    PINT(L, 27); PS(L, (p)->ip); lua_settable(L, -3); \
    PINT(L, 28); PS(L, (p)->mac); lua_settable(L, -3); \
    PINT(L, 29); PINT(L, (p)->exreport_id); lua_settable(L, -3); \
    PINT(L, 30); PINT(L, (p)->market_code); lua_settable(L, -3); \
    PINT(L, 31); PINT(L, (p)->fund_id); lua_settable(L, -3); \
    PINT(L, 32); PN(L, (p)->fee); lua_settable(L, -3)

#define LUAUNPACK_ENTRUST(L, p) \
    (p)->busin_date = PAI(L, 1); \
    (p)->entrust_id = PAI(L, 2); \
    (p)->unit_id = PAI(L, 3); \
    (p)->batchop_id = PAI(L, 4); \
    (p)->report_id = PAI(L, 5); \
    CS((p)->combi_code, L, 6); \
    CS((p)->stock_code, L, 7); \
    (p)->entrustdirection_code = PAI(L, 8); \
    (p)->price_type = PAC(L, 9); \
    (p)->price = PAD(L, 10); \
    (p)->amount = PAI(L, 11); \
    (p)->balance = PAD(L, 12); \
    (p)->prebuy_balance = PAD(L, 13); \
    (p)->presale_balance = PAD(L, 14); \
    (p)->prebuy_fee = PAD(L, 15); \
    (p)->presale_fee = PAD(L, 16); \
    (p)->prebuy_frozen_balance = PAD(L, 17); \
    (p)->entrust_status = PAC(L, 18); \
    (p)->entrust_time = PAI(L, 19); \
    (p)->cancel_flag = PAC(L, 20); \
    (p)->cancel_id = PAI(L, 21); \
    (p)->cancel_amount = PAI(L, 22); \
    (p)->business_amount = PAI(L, 23); \
    (p)->business_balance = PAD(L, 24); \
    CS((p)->remarks, L, 25); \
    (p)->operator_id = PAI(L, 26); \
    CS((p)->ip, L, 27); \
    CS((p)->mac, L, 28); \
    (p)->exreport_id = PAI(L, 29); \
    (p)->market_code = PAI(L, 30); \
    (p)->fund_id = PAI(L, 31); \
    (p)->fee = PAD(L, 32)

//----business----
#define LUAPACK_BUSINESS_TABLE(L, p) lua_newtable(L); \
    PINT(L, 1); PINT(L, (p)->busin_date); lua_settable(L, -3); \
    PINT(L, 2); PINT(L, (p)->batchop_id); lua_settable(L, -3); \
    PINT(L, 3); PINT(L, (p)->business_id); lua_settable(L, -3); \
    PINT(L, 4); PINT(L, (p)->businclass_code); lua_settable(L, -3); \
    PINT(L, 5); PINT(L, (p)->fund_id); lua_settable(L, -3); \
    PINT(L, 6); PINT(L, (p)->unit_id); lua_settable(L, -3); \
    PINT(L, 7); PS(L, (p)->combi_code); lua_settable(L, -3); \
    PINT(L, 8); PS(L, (p)->stock_code); lua_settable(L, -3); \
    PINT(L, 9); PN(L, (p)->balance); lua_settable(L, -3); \
    PINT(L, 10); PN(L, (p)->current_cash); lua_settable(L, -3); \
    PINT(L, 11); PN(L, (p)->balance_excludefee); lua_settable(L, -3); \
    PINT(L, 12); PINT(L, (p)->business_amount); lua_settable(L, -3); \
    PINT(L, 13); PINT(L, (p)->current_amount); lua_settable(L, -3); \
    PINT(L, 14); PN(L, (p)->business_price); lua_settable(L, -3); \
    PINT(L, 15); PINT(L, (p)->business_time); lua_settable(L, -3); \
    PINT(L, 16); PINT(L, (p)->entrust_id); lua_settable(L, -3); \
    PINT(L, 17); PINT(L, (p)->entrustdirection_code); lua_settable(L, -3); \
    PINT(L, 18); PN(L, (p)->jy_fee); lua_settable(L, -3); \
    PINT(L, 19); PN(L, (p)->js_fee); lua_settable(L, -3); \
    PINT(L, 20); PN(L, (p)->yh_fee); lua_settable(L, -3); \
    PINT(L, 21); PN(L, (p)->gh_fee); lua_settable(L, -3); \
    PINT(L, 22); PN(L, (p)->qt_fee); lua_settable(L, -3); \
    PINT(L, 23); PN(L, (p)->yj_fee); lua_settable(L, -3); \
    PINT(L, 24); PN(L, (p)->jg_fee); lua_settable(L, -3); \
    PINT(L, 25); PN(L, (p)->zg_fee); lua_settable(L, -3); \
    PINT(L, 26); PN(L, (p)->js2_fee); lua_settable(L, -3); \
    PINT(L, 27); PINT(L, (p)->operator_id); lua_settable(L, -3); \
    PINT(L, 28); PINT(L, (p)->ext_business_id); lua_settable(L, -3)

#define LUAUNPACK_BUSINESS(L, p) \
    (p)->busin_date = PAI(L, 1); \
    (p)->batchop_id = PAI(L, 2); \
    (p)->business_id = PAI(L, 3); \
    (p)->businclass_code = PAI(L, 4); \
    (p)->fund_id = PAI(L, 5); \
    (p)->unit_id = PAI(L, 6); \
    CS((p)->combi_code, L, 7); \
    CS((p)->stock_code, L, 8); \
    (p)->balance = PAD(L, 9); \
    (p)->current_cash = PAD(L, 10); \
    (p)->balance_excludefee = PAD(L, 11); \
    (p)->business_amount = PAI(L, 12); \
    (p)->current_amount = PAI(L, 13); \
    (p)->business_price = PAD(L, 14); \
    (p)->business_time = PAI(L, 15); \
    (p)->entrust_id = PAI(L, 16); \
    (p)->entrustdirection_code = PAI(L, 17); \
    (p)->jy_fee = PAD(L, 18); \
    (p)->js_fee = PAD(L, 19); \
    (p)->yh_fee = PAD(L, 20); \
    (p)->gh_fee = PAD(L, 21); \
    (p)->qt_fee = PAD(L, 22); \
    (p)->yj_fee = PAD(L, 23); \
    (p)->jg_fee = PAD(L, 24); \
    (p)->zg_fee = PAD(L, 25); \
    (p)->js2_fee = PAD(L, 26); \
    (p)->operator_id = PAI(L, 27); \
    (p)->ext_business_id = PAI(L, 28)

#define LUAPACK_BUSINESS_UPDATE_ITEM_TABLE(L, p) lua_newtable(L); \
    PINT(L, 1); PINT(L, (p)->unit_id); lua_settable(L, -3); \
    PINT(L, 2); PINT(L, (p)->business_id); lua_settable(L, -3); \
    PINT(L, 3); PINT(L, (p)->seesion_id); lua_settable(L, -3); \
    PINT(L, 4); PLS(L, (p)->sql.c_str(), (p)->sql.length()); lua_settable(L, -3)

//----unitasset----
#define LUAUNPACK_UNITASSET(L, p) \
    (p)->busin_date = PAI(L, 1); \
    (p)->unit_id = PAI(L, 2); \
    CS((p)->currency_code, L, 3); \
    (p)->begin_cash = PAD(L, 4); \
    (p)->current_cash = PAD(L, 5); \
    (p)->prebuy_balance = PAD(L, 6); \
    (p)->prebuy_fee = PAD(L, 7); \
    (p)->presale_balance = PAD(L, 8); \
    (p)->presale_fee = PAD(L, 9); \
    (p)->input_balance = PAD(L, 10); \
    (p)->output_balance = PAD(L, 11); \
    (p)->input_total = PAD(L, 12); \
    (p)->output_total = PAD(L, 13)

#define LUAPACK_UNITASSET_TABLE(L, p) lua_newtable(L); \
    PINT(L, 1); PINT(L, (p)->busin_date); lua_settable(L, -3); \
    PINT(L, 2); PINT(L, (p)->unit_id); lua_settable(L, -3); \
    PINT(L, 3); PS(L, (p)->currency_code); lua_settable(L, -3); \
    PINT(L, 4); PN(L, (p)->begin_cash); lua_settable(L, -3); \
    PINT(L, 5); PN(L, (p)->current_cash); lua_settable(L, -3); \
    PINT(L, 6); PN(L, (p)->prebuy_balance); lua_settable(L, -3); \
    PINT(L, 7); PN(L, (p)->prebuy_fee); lua_settable(L, -3); \
    PINT(L, 8); PN(L, (p)->presale_balance); lua_settable(L, -3); \
    PINT(L, 9); PN(L, (p)->presale_fee); lua_settable(L, -3); \
    PINT(L, 10); PN(L, (p)->input_balance); lua_settable(L, -3); \
    PINT(L, 11); PN(L, (p)->output_balance); lua_settable(L, -3); \
    PINT(L, 12); PN(L, (p)->input_total); lua_settable(L, -3); \
    PINT(L, 13); PN(L, (p)->output_total); lua_settable(L, -3)

//----unitstock----
#define LUAPACK_UNITSTOCK_TABLE(L, p) lua_newtable(L); \
    PINT(L, 1); PINT(L, (p)->busin_date); lua_settable(L, -3); \
    PINT(L, 2); PINT(L, (p)->unit_id); lua_settable(L, -3); \
    PINT(L, 3); PS(L, (p)->stock_code); lua_settable(L, -3); \
    PINT(L, 4); PINT(L, (int)((p)->bs)); lua_settable(L, -3); \
    PINT(L, 5); PINT(L, (p)->begin_amount); lua_settable(L, -3); \
    PINT(L, 6); PN(L, (p)->begin_cost); lua_settable(L, -3); \
    PINT(L, 7); PN(L, (p)->begin_total_profit); lua_settable(L, -3); \
    PINT(L, 8); PN(L, (p)->begin_total_buyfee); lua_settable(L, -3); \
    PINT(L, 9); PN(L, (p)->begin_total_salefee); lua_settable(L, -3); \
    PINT(L, 10); PINT(L, (p)->current_amount); lua_settable(L, -3); \
    PINT(L, 11); PINT(L, (p)->prebuy_amount); lua_settable(L, -3); \
    PINT(L, 12); PINT(L, (p)->presale_amount); lua_settable(L, -3); \
    PINT(L, 13); PN(L, (p)->prebuy_balance); lua_settable(L, -3); \
    PINT(L, 14); PN(L, (p)->presale_balance); lua_settable(L, -3); \
    PINT(L, 15); PINT(L, (p)->buy_amount); lua_settable(L, -3); \
    PINT(L, 16); PINT(L, (p)->sale_amount); lua_settable(L, -3); \
    PINT(L, 17); PN(L, (p)->buy_balance); lua_settable(L, -3); \
    PINT(L, 18); PN(L, (p)->sale_balance); lua_settable(L, -3); \
    PINT(L, 19); PN(L, (p)->buy_fee); lua_settable(L, -3); \
    PINT(L, 20); PN(L, (p)->sale_fee); lua_settable(L, -3)

#define LUAUNPACK_UNITSTOCK(L, p) \
    (p)->busin_date = PAI(L, 1); \
    (p)->unit_id = PAI(L, 2); \
    CS((p)->stock_code, L, 3); \
    (p)->bs = PAC(L, 4); \
    (p)->begin_amount = PAI(L, 5); \
    (p)->begin_cost = PAD(L, 6); \
    (p)->begin_total_profit = PAD(L, 7); \
    (p)->begin_total_buyfee = PAD(L, 8); \
    (p)->begin_total_salefee = PAD(L, 9); \
    (p)->current_amount = PAI(L, 10); \
    (p)->prebuy_amount = PAI(L, 11); \
    (p)->presale_amount = PAI(L, 12); \
    (p)->prebuy_balance = PAD(L, 13); \
    (p)->presale_balance = PAD(L, 14); \
    (p)->buy_amount = PAI(L, 15); \
    (p)->sale_amount = PAI(L, 16); \
    (p)->buy_balance = PAD(L, 17); \
    (p)->sale_balance = PAD(L, 18); \
    (p)->buy_fee = PAD(L, 19); \
    (p)->sale_fee = PAD(L, 20)

#define LUAPACK_DBWRITEITEM_ITEM_TABLE(L, p) lua_newtable(L); \
    PINT(L, 1); PLS(L, (p)->sql.c_str(), (p)->sql.length()); lua_settable(L, -3)

#endif
module('testamstore', package.seeall);

package.cpath = ".\\?.dll";

local amstore = require("amstore");
require('hx');
require('hexin.threadex');
local util = hexin.util;
local thread = hexin.thread;

print("type(amstore)=" .. type(amstore));

print("amstore.GetUnitBaseByOperId=" .. type(amstore.GetUnitBaseByOperId));

function GenTableStr(tbl)
    local msgstr;
    for k, v in pairs(tbl) do
        if type(v) ~= 'table' then
            if msgstr then
                msgstr = string.format("%s %s", msgstr, k .. '=' .. tostring(v) .. " " .. type(v));
            else
                msgstr = string.format( k .. '=' .. tostring(v) .. " " .. type(v));
            end
        else
            if msgstr then
                msgstr = msgstr .. ' ' .. k .. '={';
            else
                msgstr = k .. '={';
            end
            msgstr = string.format("%s%s", msgstr, GenTableStr(v) .. '}');
        end
    end
    return msgstr or "";
end

local OperIdBase = 110011;
local OperNameBase = "OperNameBase";
local unitbase = {
    unit_code = "702002002",
    unit_name = "unit_name",
    fund_id = 702002,
    fund_code = "fund_code",
    fund_name = "fund_name",
    busin_date = 20140202,
    prebusin_date = 20140201,
    brokeroffice_code = "brokeroffice_code",
    institution_id = 702,
    institution_name = "institution_name",
    operator_id = OperIdBase,
    operator_name = OperNameBase .. OperIdBase,
};

local FundBase = {
    fund_id = 702002,
    trade_password = "trade_password",
    comm_password = "comm_password", 
    brokeroffice_code = "brokeroffice_code", 
    broker_code = "broker_code",
    trade_ip = "trade_ip",
    trade_port = 777,
    query_ip = "query_ip",
    query_port = 888,
    broker_name = "broker_name",
    cts_version = "cts_version",
    stockaccount_sh = "stockaccount_sh",
    stockaccount_sz = "stockaccount_sz",
};

local Business = {
    busin_date = 20140202, 
    batchop_id = 1, 
    business_id = 1, 
    businclass_code = "businclass_code", 
    fund_id = 702002,
    unit_id = 702002002, 
    combi_code = "combi_code", 
    stock_code = "stock_code", 
    balance = 22, 
    current_cash = 33,
    balance_excludefee = 1, 
    business_amount = 100, 
    current_amount = 200,  
    business_price = 11,  
    business_time = 121212,
    entrust_id = 1, 
    entrustdirection_code = 1201, 
    jy_fee = 1,  
    js_fee = 1,  
    yh_fee = 1,
    gh_fee = 1, 
    qt_fee = 1, 
    yj_fee = 1, 
    jg_fee = 1,  
    zg_fee = 1,
    js2_fee = 1, 
    operator_id = OperIdBase,
    ext_business_id = 1,
};

local UnitAsset = {
    busin_date = 20140202, 
    unit_id = 702002002, 
    currency_code = "cny", 
    begin_cash = 10000, 
    current_cash = 10000,
    prebuy_balance = 0, 
    prebuy_fee = 0, 
    presale_balance = 0, 
    presale_fee = 0, 
    input_balance = 0,
    output_balance = 0, 
    input_total = 0, 
    output_total = 0,
};

local function TestAddUnitBaseData(unitId, unitbase)
    local ud = UnitData.create(unitId);
    
    if not ud then
        print("==error==TestAddUnitBaseData.create error");
        return;
    end
    
    local res = ud:AddUnitBaseData(unitId, unitbase.unit_code,unitbase.unit_name,unitbase.fund_id,
    unitbase.fund_code,unitbase.fund_name,unitbase.busin_date,unitbase.prebusin_date,
    unitbase.brokeroffice_code,unitbase.institution_id,unitbase.institution_name,
    unitbase.operator_id,unitbase.operator_name);
    
    if res ~= 0 then
        print("==error==TestAddUnitBaseData.res=" .. res);
    end
end

local function TestGetUnitBaseData(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestGetUnitBaseData.create error");
        return;
    end
    
    local iRet, data = ud:GetUnitBaseData();
    
    if iRet == 0 then
        local idx = 1;
        data.unit_id = data[idx]; idx = idx + 1;
        data.unit_code = data[idx]; idx = idx + 1;
        data.unit_name = data[idx]; idx = idx + 1;
        data.fund_id = data[idx]; idx = idx + 1;
        data.fund_code = data[idx]; idx = idx + 1;
        data.fund_name = data[idx]; idx = idx + 1;
        data.busin_date = data[idx]; idx = idx + 1;
        data.prebusin_date = data[idx]; idx = idx + 1;
        data.brokeroffice_code = data[idx]; idx = idx + 1;
        data.institution_id = data[idx]; idx = idx + 1;
        data.institution_name = data[idx]; idx = idx + 1;
        data.operator_id = data[idx]; idx = idx + 1;
        data.operator_name = data[idx]; idx = idx + 1;
            
        if data.unit_id ~= unitId or data.unit_code ~= unitbase.unit_code
            or data.unit_name ~= unitbase.unit_name or data.fund_id ~= unitbase.fund_id 
            or data.fund_code ~= unitbase.fund_code or data.fund_name ~= unitbase.fund_name 
            or data.busin_date ~= unitbase.busin_date or data.prebusin_date ~= unitbase.prebusin_date 
            or data.brokeroffice_code ~= unitbase.brokeroffice_code or data.institution_id ~= unitbase.institution_id 
            or data.institution_name ~= unitbase.institution_name then
            print("==error==TestGetUnitBaseData.data=" .. GenTableStr(data) .. ",unitId=" .. unitId);
        end
    else
        print("==error==TestGetUnitBaseData.iRet=" .. iRetd);     
    end
end

local function TestGetUnitBaseByOperId(operId)
    local iRet, data = amstore.GetUnitBaseByOperId(operId);

    if iRet == 0 then
        local idx = 1;
        data.unit_id = data[idx]; idx = idx + 1;
        data.unit_code = data[idx]; idx = idx + 1;
        data.unit_name = data[idx]; idx = idx + 1;
        data.fund_id = data[idx]; idx = idx + 1;
        data.fund_code = data[idx]; idx = idx + 1;
        data.fund_name = data[idx]; idx = idx + 1;
        data.busin_date = data[idx]; idx = idx + 1;
        data.prebusin_date = data[idx]; idx = idx + 1;
        data.brokeroffice_code = data[idx]; idx = idx + 1;
        data.institution_id = data[idx]; idx = idx + 1;
        data.institution_name = data[idx]; idx = idx + 1;
        data.operator_id = data[idx]; idx = idx + 1;
        data.operator_name = data[idx]; idx = idx + 1;
            
        if data.operator_id ~= operId or data.unit_code ~= unitbase.unit_code
            or data.unit_name ~= unitbase.unit_name or data.fund_id ~= unitbase.fund_id 
            or data.fund_code ~= unitbase.fund_code or data.fund_name ~= unitbase.fund_name 
            or data.busin_date ~= unitbase.busin_date or data.prebusin_date ~= unitbase.prebusin_date 
            or data.brokeroffice_code ~= unitbase.brokeroffice_code or data.institution_id ~= unitbase.institution_id 
            or data.institution_name ~= unitbase.institution_name then
            print("==error==TestGetUnitBaseByOperId.data=" .. GenTableStr(data) .. ",unitId=" .. unitId);
        end
    else
        print("==error==TestGetUnitBaseByOperId.iRet=" .. iRetd);     
    end
end

local function TestGetUnitBaseByOperName(operName)
    local iRet, data = amstore.GetUnitBaseByOperName(operName);

    if iRet == 0 then
        local idx = 1;
        data.unit_id = data[idx]; idx = idx + 1;
        data.unit_code = data[idx]; idx = idx + 1;
        data.unit_name = data[idx]; idx = idx + 1;
        data.fund_id = data[idx]; idx = idx + 1;
        data.fund_code = data[idx]; idx = idx + 1;
        data.fund_name = data[idx]; idx = idx + 1;
        data.busin_date = data[idx]; idx = idx + 1;
        data.prebusin_date = data[idx]; idx = idx + 1;
        data.brokeroffice_code = data[idx]; idx = idx + 1;
        data.institution_id = data[idx]; idx = idx + 1;
        data.institution_name = data[idx]; idx = idx + 1;
        data.operator_id = data[idx]; idx = idx + 1;
        data.operator_name = data[idx]; idx = idx + 1;
            
        if data.operator_name ~= operName or data.unit_code ~= unitbase.unit_code
            or data.unit_name ~= unitbase.unit_name or data.fund_id ~= unitbase.fund_id 
            or data.fund_code ~= unitbase.fund_code or data.fund_name ~= unitbase.fund_name 
            or data.busin_date ~= unitbase.busin_date or data.prebusin_date ~= unitbase.prebusin_date 
            or data.brokeroffice_code ~= unitbase.brokeroffice_code or data.institution_id ~= unitbase.institution_id 
            or data.institution_name ~= unitbase.institution_name then
            print("==error==TestGetUnitBaseByOperName.data=" .. GenTableStr(data) .. ",unitId=" .. unitId);
        end
    else
        print("==error==TestGetUnitBaseByOperName.iRet=", iRetd);     
    end
end


local function TestGetFastUnitBaseByOperName(operName)
    local unit_id, operator_id,fund_id = amstore.GetFastUnitBaseByOperName(operName);
    unit_id = tonumber(unit_id) or-1;
    if unit_id <= 0 then
         print("==error==TestGetFastUnitBaseByOperName.unit_id=" .. unit_id);            
    end
end

local function TestAddFundBaseData(unitId, data)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestGetUnitBaseData.create error");
        return;
    end

    local iRet = ud:AddFundBaseData(data.fund_id,data.trade_password,
    data.comm_password, data.brokeroffice_code, data.broker_code,data.trade_ip,
    data.trade_port,data.query_ip,data.query_port,
    data.broker_name,data.cts_version,data.stockaccount_sh,data.stockaccount_sz);

    if iRet ~= 0 then
        print("==error==TestAddFundBaseData.iRet=", iRetd);     
    end
end

local function TestGetFundBaseData(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestGetFundBaseData.create error");
        return;
    end

    local iRet, data = ud:GetFundBaseData(unitId);

    if iRet == 0 then
        local idx = 1;
        data.fund_id = data[idx]; idx = idx + 1;
        data.trade_password = data[idx]; idx = idx + 1;
        data.comm_password = data[idx]; idx = idx + 1;
        data.brokeroffice_code = data[idx]; idx = idx + 1;
        data.broker_code = data[idx]; idx = idx + 1;
        data.trade_ip = data[idx]; idx = idx + 1;
        data.trade_port = data[idx]; idx = idx + 1;
        data.query_ip = data[idx]; idx = idx + 1;
        data.query_port = data[idx]; idx = idx + 1;
        data.broker_name = data[idx]; idx = idx + 1;
        data.cts_version = data[idx]; idx = idx + 1;
        data.stockaccount_sh = data[idx]; idx = idx + 1;
        data.stockaccount_sz = data[idx]; idx = idx + 1;
            
        if data.fund_id ~= FundBase.fund_id or data.trade_password ~= FundBase.trade_password
            or data.comm_password ~= FundBase.comm_password or data.brokeroffice_code ~= FundBase.brokeroffice_code 
            or data.broker_code ~= FundBase.broker_code or data.trade_ip ~= FundBase.trade_ip 
            or data.trade_port ~= FundBase.trade_port or data.query_ip ~= FundBase.query_ip 
            or data.query_port ~= FundBase.query_port or data.broker_name ~= FundBase.broker_name 
            or data.cts_version ~= FundBase.cts_version or data.stockaccount_sh ~= FundBase.stockaccount_sh 
            or data.stockaccount_sz ~= FundBase.stockaccount_sz 
            then
            print("==error==TestGetFundBaseData.data=" .. GenTableStr(data) .. ",unitId=" .. unitId);
        end
    else
        print("==error==TestGetFundBaseData.iRet=", iRetd);     
    end
end

local function TestInitBusiness(unitId, curData)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestInitBusiness.create error");
        return;
    end

    local iRet, data = ud:BusiInitData(curData.busin_date, curData.batchop_id, curData.business_id, curData.businclass_code, curData.fund_id,
                curData.unit_id, curData.combi_code, curData.stock_code, curData.balance, curData.current_cash,
                curData.balance_excludefee, curData.business_amount, curData.current_amount,  curData.business_price,  curData.business_time,
                curData.entrust_id, curData.entrustdirection_code, curData.jy_fee,  curData.js_fee,  curData.yh_fee,
                curData.gh_fee, curData.qt_fee, curData.yj_fee, curData.jg_fee,  curData.zg_fee,
                curData.js2_fee, curData.operator_id, curData.ext_business_id);
    if iRet < 0 then
        print("==error==TestInitBusiness.iRet=", iRetd);
    end
end

local function TestAddBusiness(unitId, curData, tranIdx)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAddBusiness.create error");
        return;
    end

    local iRet, data = ud:BusiAddData(curData.busin_date, curData.batchop_id, curData.business_id, curData.businclass_code, curData.fund_id,
                curData.unit_id, curData.combi_code, curData.stock_code, curData.balance, curData.current_cash,
                curData.balance_excludefee, curData.business_amount, curData.current_amount,  curData.business_price,  curData.business_time,
                curData.entrust_id, curData.entrustdirection_code, curData.jy_fee,  curData.js_fee,  curData.yh_fee,
                curData.gh_fee, curData.qt_fee, curData.yj_fee, curData.jg_fee,  curData.zg_fee,
                curData.js2_fee, curData.operator_id, curData.ext_business_id, tranIdx);
    if iRet < 0 then
        print("==error==TestAddBusiness.iRet=", iRetd);
    end
end

local function TestGetAllBusiness(unitId, count)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestInitBusiness.create error");
        return;
    end

    local iRet, data = ud:BusiGetAllData();
    if iRet < 0 or type(data) ~= "table" or #data ~= count then
        print("==error==TestGetAllBusiness.iRet=", iRet, #data);
    end
end

local function TestBusiReleaseData(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==BusiReleaseData.create error");
        return;
    end

    local iRet = ud:BusiReleaseData();
    if iRet < 0 then
        print("==error==BusiReleaseData.iRet=", iRetd);
    end

    TestGetAllBusiness(unitId, 0);
end

local function TestNewTransaction(unitId, tranIdx)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestNewTransaction.create error");
        return;
    end

    local iRet = ud:NewTransaction();
    if iRet < 0 then
        print("==error==TestNewTransaction.iRet=", iRetd);
    end

    return iRet;
end

local function TestBusiRollbackData(unitId, tranIdx)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestBusiRollbackData.create error");
        return;
    end

    local iRet = ud:BusiRollbackData(tranIdx);
    if iRet < 0 then
        print("==error==TestBusiRollbackData.iRet=", iRetd);
    end

    return iRet;
end

local function TestLockSaveInfo(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestLockSaveInfo.create error");
        return;
    end

    local iRet = ud:LockSaveInfo();

    local iRet = ud:BusiInitDbSaveInfo();
    return iRet;
end

local function TestUnlockSaveInfo(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestUnlockSaveInfo.create error");
        return;
    end

    local iRet = ud:UnlockSaveInfo();
    return iRet;
end

local function TestBusiGetSaveInfo(unitId, flag)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==BusiGetSaveInfo.create error");
        return;
    end

    flag = flag or 1;

    local iRet, data = ud:BusiGetSaveInfo();
    if iRet < 0 or ((type(data) ~= "table" or #data <= 0) and flag ~= 0) then
        print("==error==TestBusiGetSaveInfo.iRet=", iRet, flag);
    end

    for idx = 1, #data do
        --print("sql=", data[idx][1]);
    end

    return iRet;
end

local function TestBusiDoneSaveInfo(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestBusiDoneSaveInfo.create error");
        return;
    end

    local iRet, data = ud:BusiDoneSaveInfo();
    if iRet < 0  then
        print("==error==TestBusiDoneSaveInfo.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetInitData(unitId, curData)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetInitData.create error");
        return;
    end

    local iRet = ud:AssetInitData(curData.busin_date, curData.unit_id, curData.currency_code, curData.begin_cash, curData.current_cash,
                curData.prebuy_balance, curData.prebuy_fee, curData.presale_balance, curData.presale_fee, curData.input_balance,
                curData.output_balance, curData.input_total, curData.output_total);
    if iRet < 0  then
        print("==error==TestAssetInitData.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetUpdateByEntrust(unitId, data, tranIdx)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetUpdateByEntrust.create error");
        return;
    end

    local iRet = ud:AssetUpdateByEntrust(data.currency_code, data.prebuy_balance, data.presale_balance,
        data.prebuy_fee, data.presale_fee, tranIdx);
    if iRet < 0  then
        print("==error==TestAssetUpdateByEntrust.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetUpdateByBusiness(unitId, data, tranIdx)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetUpdateByBusiness.create error");
        return;
    end

    local iRet = ud:AssetUpdateByBusiness(data.currency_code, data.current_cash, data.prebuy_balance, data.presale_balance,
        data.prebuy_fee, data.presale_fee, tranIdx);
    if iRet < 0  then
        print("==error==TestAssetUpdateByBusiness.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetGetData(unitId, curCode)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetGetData.create error");
        return;
    end

    local iRet, data = ud:AssetGetData(curCode);
    if iRet < 0 or type(data) ~= "table" or #data <= 0 then
        print("==error==TestAssetGetData.iRet=", iRet);
    end

    return data;
end

local function TestAssetReleaseData(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetReleaseData.create error");
        return;
    end

    local iRet = ud:AssetReleaseData(curCode);
    if iRet < 0 then
        print("==error==TestAssetReleaseData.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetRollbackData(unitId, tranIdx)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetRollbackData.create error");
        return;
    end

    local iRet = ud:AssetRollbackData(tranIdx);
    if iRet < 0 then
        print("==error==TestAssetRollbackData.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetInitDbSaveInfo(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetInitDbSaveInfo.create error");
        return;
    end

    local iRet = ud:AssetInitDbSaveInfo();
    if iRet < 0 then
        print("==error==TestAssetInitDbSaveInfo.iRet=", iRet);
    end

    return iRet;
end

local function TestAssetGetSaveInfo(unitId, flag)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetGetSaveInfo.create error");
        return;
    end

    local iRet, data = ud:AssetGetSaveInfo();
    if iRet < 0 or ((type(data) ~= "table" or #data <= 0) and flag ~= 0) then
        print("==error==TestAssetGetSaveInfo.iRet=", iRet, flag);
    end

    return iRet;
end

local function TestAssetDoneSaveInfo(unitId)
    local ud = UnitData.create(unitId);
    if not ud then
        print("==error==TestAssetDoneSaveInfo.create error");
        return;
    end

    local iRet = ud:AssetDoneSaveInfo();
    if iRet < 0 then
        print("==error==TestAssetDoneSaveInfo.iRet=", iRet);
    end

    return iRet;
end

function Test()
    for idx = 1, 10 do
        unitbase.operator_id = OperIdBase + idx;
        unitbase.operator_name = OperNameBase .. unitbase.operator_id;
        TestAddUnitBaseData(idx, unitbase);
        TestGetUnitBaseData(idx);    
        TestGetUnitBaseByOperId(unitbase.operator_id);
        TestGetUnitBaseByOperName(unitbase.operator_name);
        TestGetFastUnitBaseByOperName(unitbase.operator_name);
        TestAddFundBaseData(idx, FundBase);
        TestGetFundBaseData(idx);

        Business.unit_id = idx;
        Business.business_id = idx;
        TestInitBusiness(idx, Business);
        Business.business_id = idx + 1000;
        local tranIdx = TestNewTransaction(idx);
        TestAddBusiness(idx, Business, tranIdx);
        TestBusiRollbackData(idx, tranIdx);
        TestGetAllBusiness(idx, 1);

        Business.business_id = idx + 2000;
        tranIdx = TestNewTransaction(idx);
        TestAddBusiness(idx, Business, tranIdx);
        TestLockSaveInfo(idx);
        TestBusiGetSaveInfo(idx);
        TestBusiDoneSaveInfo(idx);
        TestUnlockSaveInfo(idx);

        TestLockSaveInfo(idx);
        TestBusiGetSaveInfo(idx, 0);
        TestBusiDoneSaveInfo(idx);
        TestUnlockSaveInfo(idx);

        TestBusiReleaseData(idx);

        TestAssetInitData(idx, UnitAsset);

        local UnitAssetUpdate = {
            currency_code = "cny", 
            current_cash = 0,
            prebuy_balance = 10, 
            prebuy_fee = 10, 
            presale_balance = 10, 
            presale_fee = 10, 
        };
        tranIdx = TestNewTransaction(idx);
        TestAssetUpdateByEntrust(idx, UnitAssetUpdate, tranIdx);
        
        UnitAssetUpdate = {
            currency_code = "cny", 
            current_cash = 100,
            prebuy_balance = -10, 
            prebuy_fee = -10, 
            presale_balance = -10, 
            presale_fee = -10, 
        };
        TestAssetUpdateByBusiness(idx, UnitAssetUpdate, tranIdx);
        local asset = TestAssetGetData(idx, "cny");

        if asset then 
            if asset[5] ~= UnitAsset.current_cash + UnitAssetUpdate.current_cash then
                print("TestAssetGetData.current_cash=" .. asset[5]);
            end
        else
            print("TestAssetGetData.current_cash not found");
        end

        UnitAssetUpdate = {
            currency_code = "cny", 
            current_cash = 0,
            prebuy_balance = 10, 
            prebuy_fee = 10, 
            presale_balance = 10, 
            presale_fee = 10, 
        };
        tranIdx = TestNewTransaction(idx);
        TestAssetUpdateByEntrust(idx, UnitAssetUpdate, tranIdx);
        UnitAssetUpdate = {
            currency_code = "cny", 
            current_cash = 100,
            prebuy_balance = -10, 
            prebuy_fee = -10, 
            presale_balance = -10, 
            presale_fee = -10, 
        };
        tranIdx = TestNewTransaction(idx);
        TestAssetUpdateByBusiness(idx, UnitAssetUpdate, tranIdx);
        TestAssetRollbackData(idx, tranIdx);

        local asset = TestAssetGetData(idx, "cny");

        if asset then 
            if asset[5] ~= UnitAsset.current_cash + 100 then
                print("==TestAssetGetData.current_cash=" .. asset[5]);
            end
        else
            print("==TestAssetGetData.current_cash not found");
        end

        TestLockSaveInfo(idx);
        TestAssetInitDbSaveInfo(idx);
        TestAssetGetSaveInfo(idx);
        TestAssetDoneSaveInfo(idx);
        TestUnlockSaveInfo(idx);

        TestAssetReleaseData(idx);
    end

    for idx = 702001001, 702001011 do
        unitbase.operator_id = OperIdBase + idx;
        unitbase.operator_name = OperNameBase .. unitbase.operator_id;
        TestAddUnitBaseData(idx, unitbase);
        TestGetUnitBaseData(idx);    
        TestGetUnitBaseByOperId(unitbase.operator_id);
        TestGetUnitBaseByOperName(unitbase.operator_name);
        TestGetFastUnitBaseByOperName(unitbase.operator_name);
        TestAddFundBaseData(idx, FundBase);
        TestGetFundBaseData(idx);

        Business.unit_id = idx;
        Business.business_id = idx;
        TestInitBusiness(idx, Business);
        Business.business_id = idx + 1000;
        TestAddBusiness(idx, Business);
        TestGetAllBusiness(idx, 2);

        TestBusiReleaseData(idx);
    end
end

--hexin.thread.run_main_func(function()
--    while true do
--        --Test();
--    end
--end);    
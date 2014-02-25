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

local function TestAddUnitBaseData(unitId, unitbase)
    local ud = UnitData.create(unitId);
    
    if not ud then
        print("==error==TestAddUnitBaseData.create error");
        return;
    end
    
    print(type(ud), type(ud.AddUnitBaseData));
    
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
    end
end

--hexin.thread.run_main_func(function()
--    while true do
--        --Test();
--    end
--end);    
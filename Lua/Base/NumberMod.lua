package.cpath = ".\\?.dll"

local numTools = require("numtools");

local dataTest = require("NumberModTest");


--返回n1 % n2，其中n1和n2可以浮点数
function NumMod(n1, n2)
    b = numTools.ModFloat(n1, n2, 2)
    
    if b ~= 0 then
        print(b, n1, n2);
    end
    
    return  b;
end


local function  TestNumMod()
    local a = NumMod(15.87, 0.01);
    print(a)

    local count = 0;
    local countError = 0;
    local res;
    local resError ={};
    
    for i = 1, #dataTest do
    --for i = 1, 1 do
        res = NumMod(dataTest[i], 0.01);
        if res ~= 0 then
            resError[#resError + 1] = i;
            countError = countError + 1;
        end
        count = count + 1;
    end
    
    print("#resError=" .. #resError .. ", count=" .. count .. ", countError=" .. countError);
end

TestNumMod();
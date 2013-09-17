local  function BuildData()
    local fileName = "NumberModTest.lua";
    local file = io.open(fileName, "w");
    local context = {};
   
    context[#context + 1] = "module('NumberModTest', package.seeall);  \n";
    context[#context + 1] =  "local data = {}; \n";

    local idx = 1;
    for num = 0.01, 1000, 0.01 do
        num = tostring(num);
        local i = string.find(num, '%.') or 0;
        num = string.sub(num, 1, i + 2);
        
        context[#context + 1] =  "data[" .. idx .. "] = " .. num .. "; \n";
        idx = idx + 1;
    end

    context[#context + 1] = "return data; \n";
    context = table.concat(context, "");

    print(context);

    file:write(context);
    file:close();
end

BuildData();
package.cpath = ".\\?.dll";

require('hx');
require('hexin.threadex');
local util = hexin.util;
local thread = hexin.thread;

local testamstore = require("testamstore");

print("testamstore start");
--hexin.thread.start_threads('testamstore', 10, {}); 
testamstore.Test();
print("testamstore end");

--while true do
--    thread.sleep(1000);
--end
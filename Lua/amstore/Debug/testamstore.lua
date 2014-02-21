package.cpath = ".\\?.dll";

local amstore = require("amstore");

local objDemo = LuaObjDemo:new();
print(type(objDemo));
print(objDemo);
print(objDemo:get_count());

local objDemo = LuaObjDemo:new();
print(type(objDemo));
print(objDemo);
print(objDemo:get_count());
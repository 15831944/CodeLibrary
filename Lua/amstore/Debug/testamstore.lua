package.cpath = ".\\?.dll";

local amstore = require("amstore");

local objDemo = LuaObjDemo:create();
print(type(objDemo));
print(objDemo);
print(objDemo:get_count());

local objDemo = LuaObjDemo:create();
print(type(objDemo));
print(objDemo);
print(objDemo:get_count());

local objDemo = LuaObjDemo();
print(type(objDemo));
print(objDemo);
print(objDemo:get_count());
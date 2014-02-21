#include "boot1.h"

Boot1 Boot1::_Instance_1("class static member 1");

static Boot1 GInstance_cpp_1("global static variable (cpp_1)");

Boot1 Boot1::_Instance_2("class static member 2");

static Boot1 GInstance_cpp_2("global static variable (cpp_2)");

Boot1 Boot1::_Instance_3("class static member 3");
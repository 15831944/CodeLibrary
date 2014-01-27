#include "boot1.h"

static Boot1 GInstance_1("global static variable (cpp 1)");

Boot1 Boot1::_Instance("class static member");

static Boot1 GInstance_2("global static variable (cpp 2)");
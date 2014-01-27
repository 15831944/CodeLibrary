#include "boot2.h"

static Boot2 GInstance_1("global static variable (cpp 1)");

Boot2 Boot2::_Instance("class static member");

static Boot2 GInstance_2("global static variable (cpp 2)");
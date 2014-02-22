#ifndef _AM_LOGER_H
#define _AM_LOGER_H

#define GLOG_NO_ABBREVIATED_SEVERITIES

#include "glog/logging.h"
#include "glog/log_severity.h"

#define FILE_LOG_NAME "am"

#define LOG_END_FLAG ""

void InitLoger();

#endif
#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#pragma once

#include "sysdef.h"

/*32 位和 64 位数据模型
ILP32   LP64    LLP64
char        8       8       8
short       16      16      16
int         32      32      32
long        32      64      32
long long   64      64      64
指针        32      64      64

64位Linux使用LP64模型，64位Windows使用LLP64模型
*/

typedef char              int8_t;
typedef unsigned char     uint8_t;

typedef short             int16_t;
typedef unsigned short    uint16_t;

#if defined(_LP64)
typedef int                 int32_t;
typedef unsigned int        uint32_t;
typedef long long           integer;
typedef unsigned long long  uinteger;
#else
typedef long                int32_t;
typedef unsigned long       uint32_t;
typedef long                integer;
typedef unsigned long       uinteger;
#endif

typedef long long           int64_t;
typedef unsigned long long  uint64_t;
//typedef void*               intptr_t;

#endif
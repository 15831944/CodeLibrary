#ifndef _SYSDEF_H
#define _SYSDEF_H

#pragma once

#if defined(_WIN32) || defined (_WIN64)
#define __WINDOWS		// windowsƽ̨
#elif defined (__MAC)
//#define _DARWIN_C_SOURCE			// macosƽ̨
#else
#ifndef __LINUX
#define __LINUX			// linuxƽ̨
#endif
#endif

#define __NS_LONTOKEN_START	namespace lontoken{
#define __NS_LONTOKEN_END	}

#if (defined(__LINUX) || defined(__MAC))
#include <assert.h>
#define ASSERT assert
#elif (defined(__WINDOWS))
#include <cassert>
#include <Windows.h>
#define ASSERT assert
#define snprintf _snprintf
#endif 

#endif
/* 核新API库基础类型定义 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include "sysdef.h"

//32 位和 64 位数据模型
//  ILP32   LP64    LLP64
//  char        8       8       8
//  short       16      16      16
//  int         32      32      32
//  long        32      64      32
//  long long   64      64      64
//  指针        32      64      64
//
//64位Linux使用LP64模型，64位Windows使用LLP64模型


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

#ifdef _WIN32
#include <tchar.h>
#else
#include "tchar.h"
#endif


#ifdef __cplusplus
extern "C"{
#endif

//
#ifndef OK
#define	OK 0
#endif

#ifndef _AFX
struct __POSITION { };
typedef __POSITION*	POSITION;
#endif

#ifdef WIN32

#include <Windows.h>
#include <io.h>

#else	//!WIN32

#ifndef __RPC_FAR
#define __RPC_FAR
#endif

#ifndef HANDLE
typedef  void* HANDLE;
#endif

#define small char
typedef unsigned char byte;
typedef unsigned char boolean;


#ifndef _HYPER_DEFINED
#define _HYPER_DEFINED

#if !defined(__RPC_DOS__) && !defined(__RPC_WIN16__) && !defined(__RPC_MAC__) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)) && defined(_MSC_VER)
#define  hyper           __int64
#define MIDL_uhyper  unsigned __int64
#elif defined(__GNUC__)
#define hyper			long long
#define MIDL_uhyper	unsigned long long
#else
typedef double  hyper;
typedef double MIDL_uhyper;
#endif

#endif // _HYPER_DEFINED

//
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif // !FALSE
//
//
#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif
//
#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char BYTE;
#endif // !_BYTE_DEFINED
//
#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short WORD;
#endif // !_WORD_DEFINED
//
typedef unsigned int UINT;

typedef int INT;

typedef long BOOL;
//
#ifndef _LONG_DEFINED
#define _LONG_DEFINED
typedef long LONG;
#endif // !_LONG_DEFINED
//
#ifndef _WPARAM_DEFINED
#define _WPARAM_DEFINED
typedef UINT WPARAM;
#endif // _WPARAM_DEFINED
//
#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
#ifdef __LP64_BARATOL
typedef unsigned int DWORD;
#else
typedef unsigned long DWORD;
#endif
#endif // !_DWORD_DEFINED
//
#ifndef _LPARAM_DEFINED
#define _LPARAM_DEFINED
typedef LONG LPARAM;
#endif // !_LPARAM_DEFINED
//
#ifndef _LRESULT_DEFINED
#define _LRESULT_DEFINED
typedef LONG LRESULT;
#endif // !_LRESULT_DEFINED
//
#ifndef _LPWORD_DEFINED
#define _LPWORD_DEFINED
typedef WORD __RPC_FAR *LPWORD;
#endif // !_LPWORD_DEFINED
//
#ifndef _LPDWORD_DEFINED
#define _LPDWORD_DEFINED
typedef DWORD __RPC_FAR *LPDWORD;
#endif // !_LPDWORD_DEFINED
//
typedef char CHAR;

typedef /* [string] */ CHAR *LPSTR;

typedef /* [string] */ const CHAR *LPCSTR;
//
#ifndef _WCHAR_DEFINED
#define _WCHAR_DEFINED
#ifdef __WINDOWS
typedef wchar_t WCHAR;
#else
#ifdef _UNICODE
typedef wchar_t WCHAR;
#else
typedef unsigned short WCHAR;
#endif
#endif
#endif // !_WCHAR_DEFINED


#ifdef _UNICODE
typedef WCHAR TCHAR;
#else
typedef CHAR TCHAR;
#endif

//
typedef /* [string] */ WCHAR *LPWSTR;

typedef /* [string] */ TCHAR *LPTSTR;

typedef /* [string] */ const WCHAR *LPCWSTR;

typedef /* [string] */ const TCHAR *LPCTSTR;
//
#ifndef _COLORREF_DEFINED
#define _COLORREF_DEFINED
typedef DWORD COLORREF;
#endif // !_COLORREF_DEFINED
//
#ifndef _LPCOLORREF_DEFINED
#define _LPCOLORREF_DEFINED
typedef DWORD __RPC_FAR *LPCOLORREF;
#endif // !_LPCOLORREF_DEFINED
//
typedef HANDLE *LPHANDLE;

typedef struct  _RECTL
    {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
    }	RECTL;

typedef struct _RECTL *PRECTL;

typedef struct _RECTL *LPRECTL;

typedef struct  tagPOINT
    {
    LONG x;
    LONG y;
    }	POINT;

typedef struct tagPOINT *PPOINT;

typedef struct tagPOINT *LPPOINT;

typedef struct  _POINTL
    {
    LONG x;
    LONG y;
    }	POINTL;

typedef struct _POINTL *PPOINTL;

//
typedef struct  tagSIZE
    {
    LONG cx;
    LONG cy;
    }	SIZE;

typedef struct tagSIZE *PSIZE;

typedef struct tagSIZE *LPSIZE;

typedef struct  tagSIZEL
    {
    LONG cx;
    LONG cy;
    }	SIZEL;

typedef struct tagSIZEL *PSIZEL;

typedef struct tagSIZEL *LPSIZEL;

typedef const RECTL LPCRECTL;

typedef void VOID;

typedef void *PVOID;

typedef void *LPVOID;

typedef float FLOAT;

typedef struct  tagRECT
    {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
    }	RECT;

typedef struct tagRECT *PRECT;

typedef struct tagRECT *LPRECT;

typedef const RECT *LPCRECT;

typedef unsigned char UCHAR;

typedef short SHORT;

typedef unsigned short USHORT;

typedef DWORD ULONG;

typedef double DOUBLE;

#ifndef _DWORDLONG_
typedef MIDL_uhyper DWORDLONG;
typedef DWORDLONG __RPC_FAR *PDWORDLONG;
#endif // !_DWORDLONG_
//
#ifndef _ULONGLONG_
typedef hyper LONGLONG;

typedef MIDL_uhyper ULONGLONG;

typedef LONGLONG *PLONGLONG;

typedef ULONGLONG *PULONGLONG;

#endif // _ULONGLONG_

typedef struct  _LARGE_INTEGER
    {
    LONGLONG QuadPart;
    }	LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef struct  _ULARGE_INTEGER
    {
    ULONGLONG QuadPart;
    }	ULARGE_INTEGER;

//
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef LONG HRESULT;
#endif // !_HRESULT_DEFINED
//
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct  _GUID
    {
    DWORD Data1;
    WORD Data2;
    WORD Data3;
    BYTE Data4[ 8 ];
    }	GUID;
#endif // !GUID_DEFINED
//
#if !defined( __LPGUID_DEFINED__ )
#define __LPGUID_DEFINED__
typedef GUID *LPGUID;
#endif // !__LPGUID_DEFINED__
//


#define CONST			const
#ifdef __LP64_BARATOL
typedef unsigned int DWORD;
#else
typedef unsigned long DWORD;
#endif
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL	            *PBOOL;
typedef BOOL	            *LPBOOL;
typedef BYTE	            *PBYTE;
typedef BYTE	            *LPBYTE;
typedef int		            *PINT;
typedef int		            *LPINT;
typedef WORD	            *PWORD;
typedef WORD	            *LPWORD;
typedef long	            *LPLONG;
typedef DWORD		        *PDWORD;
typedef DWORD	            *LPDWORD;
typedef void	            *LPVOID;
typedef const void	        *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

/* Types use for passing & returning polymorphic values */
typedef UINT WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;

//
#define _W64
typedef _W64 int INT_PTR, *PINT_PTR;
typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;

typedef _W64 long LONG_PTR, *PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

//#define __int3264   __int32 		//32-bit on 32-bit paltforms, 64-bit on 64-bit platforms
typedef int		__int32;	//always 32-bit

typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef LONG_PTR SSIZE_T, *PSSIZE_T;

////////////////////////////////////////////////////////////////////////////
//for 64 bits
typedef long long	LONG64;	
typedef long long	INT64;	
typedef long long	__int64;	

typedef unsigned long long	ULONG64;	
typedef unsigned long long	UINT64;	
typedef unsigned long long	__uint64;	
////////////////////////////////////////////////////////////////////////////

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef _MAX_PATH
 #define _MAX_PATH   256 /* max. length of full pathname */
#endif

#define __min(a,b)            (((a) < (b)) ? (a) : (b))
#define __max(a,b)            (((a) > (b)) ? (a) : (b))

#define MAX __max
#define MIN __min

#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif

#ifndef MAKEWORD
#define MAKEWORD(low,high)	((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))
#endif

#ifndef __time32_t
#define __time32_t long
#endif

#endif //WIN32

#ifndef MAX
#define MAX max
#endif
#ifndef MIN
#define MIN min
#endif

#ifndef LOWORD
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#endif

#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))
#endif

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//属性位操作函数
#define TLSetOptions(dwOptions, dwValue, dwMask) dwOptions &= ~dwMask; dwOptions |= (dwValue & dwMask)
//属性位获取函数
#define TLGetOptions(dwOptions, dwMask) (dwOptions & dwMask)

#ifdef __cplusplus
}
#endif

#endif	//__TYPES_H__
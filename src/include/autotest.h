//*---------------------------------------------------------------------------------
//|  Title:    AUTOTEST.H
//|		Copyright (c) Microsoft Corporation
//|
//|  Notes:
//|      To create a retail version of this file (eg: the version which
//|      ships in the SDK), remove those items bracketed by:
//|            #ifndef _RETAIL
//|            #endif /* _RETAIL */
//|      and
//|            #ifdef QUIKTEST
//|            #endif /* QUIKTEST */
//|
//|   This file contains constants and prototypes required to compile an
//|   Auto Test DLL.
//*---------------------------------------------------------------------------------
#ifndef AUTOTEST_DEFS
#define AUTOTEST_DEFS

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>



#ifdef INHOUSE30
#include <wchar.h>
#endif

#include "sql.h"
#include "sqlext.h"

extern HINSTANCE  hLoadedInst;

#ifndef _TCHAR_DEFINED
typedef char TCHAR;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

// Defines for FETCH_RESUME rmved from SQL*.H in ODBC3.0, needed for tests
#ifndef SQL_FD_FETCH_RESUME
#define SQL_FD_FETCH_RESUME                 0x00000040L
#endif

#ifndef SQL_FETCH_RESUME
#define SQL_FETCH_RESUME                7    /* Not supported by 2.0+ drivers */
#endif



#ifdef INHOUSE20

#ifndef TEXT
#define TEXT(quote) quote
#endif

#ifndef WIN32
typedef LPSTR LPTSTR;
#endif


#ifndef WIN32
typedef LPCSTR LPCTSTR;
#endif

#ifndef SQLRETURN
#define SQLRETURN RETCODE
#endif

#ifndef _SQLCHAR_DEFINED
typedef unsigned char SQLCHAR;
#define _SQLCHAR_DEFINED
#endif /* !_SQLCHAR_DEFINED */

#ifndef _SQLTCHAR_DEFINED
typedef SQLCHAR  SQLTCHAR;
#define _SQLTCHAR_DEFINED
#endif /* !_SQLTCHAR_DEFINED */

#ifndef SQL_C_TCHAR
#define SQL_C_TCHAR	SQL_C_CHAR
#endif


#endif

#ifdef _UNICODE

#undef	lstrcpy
#undef	lstrcat
#undef	lstrcmp
#undef	lstrcmpi
#undef	strstr
#undef	strdup
#undef	strtok


#define	strtok	_tcstok
#define	strdup	_tcsdup
#define	strstr	_tcsstr
#define	lstrcpy	_tcscpy
#define	lstrcat	_tcscat
#define	lstrcmpi	_tcsicmp
#define	lstrcmp	_tcscmp
#undef	wsprintf
#define	wsprintf	_stprintf
#undef	wvsprintf
#define	wvsprintf	_vstprintf

#endif

#ifdef __cplusplus
extern "C" {
#endif   /* __cplusplus */

//FOR WIN95 UNICODE SUPPORT


//END win95 unicode support

// This define the setjmp buffers for the different platforms
//   NOTE: Can not use setjmp.h because of C++ restriction
#ifndef _JMP_BUF_DEFINED
#ifndef WIN32
typedef  int  		jmp_buf[9];
#else
#if 	defined (_M_ALPHA)
typedef  double 	jmp_buf[24];
#elif defined (_M_MRX000)
typedef  double		jmp_buf[16];
#elif defined (_M_PPC)
typedef  double		jmp_buf[32];
#elif defined (_M_IA64)
typedef __declspec(align(16)) struct _SETJMPFLOAT128 {
    __int64 LowPart;
    __int64 HighPart;
} SETJMPFLOAT128;
typedef SETJMPFLOAT128 jmp_buf[33];
#else
typedef  int  		jmp_buf[16];
#endif
#endif
#define _JMP_BUF_DEFINED
#endif

#ifndef _RETAIL
#include <setjmp.h>
#endif   /* _RETAIL  */




//----------------------------------------------------------------------------------
//    Defines and Macros
//----------------------------------------------------------------------------------
#define TEST_ABORTED                (-1)

#define AUTO_MAX_TEST_NAME          35
#define AUTO_MAX_TESTCASE_NAME      35
#define AUTO_MAX_TESTDESC_NAME      75

#define MAXFLUSH                   300
#define MAX_USER_INFO               50
#define MAX_KEYWORD_LEN            149

/*$$ not needed already (collides with another definition)
// --
#ifdef QUIKTEST
//-- Begin Retail only
#ifdef VG_MAC
#define EXTFUNCDECL                                             CDECL
#ifdef VG_PPC
#define EXTFUN                                                  CDECL
#else
#define EXTFUN                                                  PASCAL
#endif
#else
#elif defined(WIN32)
#define EXTFUNCDECL                 _stdcall
#define EXTFUN                      _stdcall
#else
#define EXTFUNCDECL
#define EXTFUN                      __export PASCAL
#endif


#define InitTest(lps)                                             \
{  lps->cErrors=0; }
#define AbortTest(lps)                                            \
{ lps->cErrors=TEST_ABORTED; }

#ifndef VG_UNIX
#define     AllocateMemory(cb)   \
					 (GlobalLock(GlobalAlloc((GMEM_FIXED | GMEM_ZEROINIT), (cb))))

#ifdef WIN32
#define     ReleaseMemory(lp)    \
					 (GlobalUnlock((HGLOBAL)GlobalHandle(lp)), (BOOL)GlobalFree((HGLOBAL)GlobalHandle(lp)))
#else
#define     ReleaseMemory(lp)    \
					 (GlobalUnlock(((HGLOBAL)LOWORD(GlobalHandle(SELECTOROF(lp))))), \
					 (BOOL)GlobalFree(((HGLOBAL)LOWORD(GlobalHandle(SELECTOROF(lp))))))
#endif
#else
#define ReleaseMemory(lp) free((void*)lp)
#define AllocateMemory(cb) calloc((size_t)1, (size_t)cb)
#endif

#define NumItems(s) (sizeof(s) / sizeof(s[0]))

// Following will access bit number pos in a bit array and return
//    TRUE if it is set, FALSE if it is not
#define CQBITS (sizeof(UINT) * 8)
#define getqbit(lpa, pos)  \
	(lpa[((pos) / CQBITS)] & (1 << ((pos) - (CQBITS * ((pos) / CQBITS)))))
#define GETBIT(p1,p2) getqbit(p1,(p2)-1)

// -- End retail only
#endif  / * QUIKTEST * /
// --
$$*/


//
// This structure contains the information found in the .INI file for a
// data source.  The filled out structure is in turn passed to AutoTestFunc
// to drive the individual tests.
//
typedef struct tagSERVERINFO {
	HWND        hwnd;                      // Output edit window
	TCHAR        szLogFile[_MAX_PATH];      // Output log file
	HENV        henv;                      // .EXE's henv
	HDBC        hdbc;                      // .EXE's hdbc
	HSTMT       hstmt;                     // .EXE's hstmt

	// The following items are gathered from the .INI file and may be defined
	//    via the "Manage Test Sources" menu item from ODBC Test
	TCHAR        szSource[SQL_MAX_DSN_LENGTH+1];
	TCHAR        szValidServer0[SQL_MAX_DSN_LENGTH+1];
	TCHAR        szValidLogin0[MAX_USER_INFO+1];
	TCHAR        szValidPassword0[MAX_USER_INFO+1];
	TCHAR        szKeywords[MAX_KEYWORD_LEN+1];

	// Following are used for run-time
	UINT *  rglMask;                   // Run test mask
	int         failed;                    // Track failures on a test case basis
	int         cErrors;                   // Count of errors
	BOOL        fDebug;                    // TRUE if debugging is to be enabled
	BOOL        fScreen;                   // TRUE if test output goes to screen
	BOOL        fLog;                      // TRUE if test output goes to log
	BOOL        fIsolate;                  // TRUE to isolate output
	UDWORD      vCursorLib;                // Value for SQL_ODBC_CURSOR on SQLSetConnectOption
	HINSTANCE   hLoadedInst;               // Instance handle of loaded test

	// Following are used for buffering output to edit window
	TCHAR        szBuff[MAXFLUSH];          // Hold temporary results
	UINT        cBuff;                     // Number of chars in szBuff

#ifndef _RETAIL
	// Following are for INHOUSE.H stuff, remove for retail
	int         cStateErrors;
	HENV *  usrHenv;                   // User's HENV
	HDBC *  usrHdbc;                   // User's HDBC
	int         itestcase;
	LPTSTR       sztestcase;
	jmp_buf		env;	                     // For aborts
	UINT *  rglMaskEntire;             // Contains the entire run bitmask regardless of isolated mode
	DWORD       dwDriver;                  // Bitmask of the Driver we are Currently on
	UWORD       uODBCVer;                  // Current odbc Version of Driver being tested
	UWORD       uDriverVer;                // Current Driver Version
	UDWORD		uOdbcBehav;					// Odbc Behavior Attribute
#endif  /* _RETAIL */
// [AlexR] 01.06.1999 - additional field for the server version (Hydra,Sphinx,...)
	int		nServerVer;
	} SERVERINFO;
typedef SERVERINFO * lpSERVERINFO;


//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------
BOOL szLogPrintf(lpSERVERINFO lps, BOOL fForce, LPTSTR szFmt, ...);
int szMessageBox(HWND hwnd, UINT style, LPTSTR szTitle, LPTSTR szFmt, ...);
LPTSTR WINAPI GetRCString(HINSTANCE hInst, LPTSTR buf, int cbbuf, UINT ids);


#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------
// IDS string constants
// These constants are used to define the range in which the .h file is allowed to
// create ids strings.  They are calculated by the idsmunge.exe tool found in the
// tests\autotest\tools\idsmunge directory.  These can be expanded when needed by
// simply adding the needed ids strings and re-running the tool.
//----------------------------------------------------------------------------------

#define IDS_INHOUS20                                          0
#define IDS_IHTESTS                                           200
#define IDS_USER                                              2000

//Originally in testobj.h for use with IHForceKeyBit function
#define MSK_KEY_FILLTABLE_AUTO	0x00000000	// Fill MakeTable AutoMakeTable default
#define MSK_KEY_LOCAL			0x00000001	// Mask bit set by keyword LOCALIZATION
#define MSK_KEY_ODBC3BEHAV		0x00000002	// Set on ODBC3 behavior (default is 2.x behavior)
#define MSK_KEY_NOSTATECHK		0x00000004	// Turn off state checking for autotests
#define MSK_KEY_FSMAP			0x00000008	// Turn on mapping of SQLExtendedFetch to SQLFetchScroll
#define MSK_KEY_FILLTABLE_NULL	0x00010000	// Fill MakeTable with nulls when row = column,
                                             //              keyword FILLTABLE_NULL
#define MSK_KEY_FILLTABLE_CHAR	0x00020000	// Fill MakeTable with unique chars,
                                             //              keyword FILLTABLE_CHAR
#define MSK_KEY_UNATTENDED		0x00000010	// Running unattended - don't interrupt
#define MSK_KEY_SHOWSQL			0x00000020

#endif

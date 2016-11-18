//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	GTRCOMM.C
//|
//| Purpose:
//|		Header file for utility functions.
//*---------------------------------------------------------------------------------
#ifndef GTRCOMM_DEFS
#define GTRCOMM_DEFS

#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include "portable.h"
#include "standard.h"
#include "bitarray.h"
#include "gatortst.h"
#include "strings.h"
#include "sql.h"
#include "sqlext.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------

#ifndef WIN32
#define LPTSTR LPSTR
#define LPCTSTR	LPSTR
#define TCHAR	char
#endif

//Unicode specific macros
#define NEXTCHAR(str) ((str)+=sizeof(TCHAR))
#ifndef SQLTCHAR
#define SQLTCHAR	TCHAR
#endif

//@INSTALLER
#define SQL_FALSE			0
#define SQL_TRUE			1

//The following are Stmt states to signify what state
// the current hstmt is in.
#define STMT_UNALLOCATED	0
#define STMT_ALLOCATED		1
#define STMT_PREPARED		2
#define STMT_EXECUTED		3
#define STMT_CURSOR			4
#define STMT_NEEDDATA		5
#define STMT_ASYNC			6

//For Column Binding
#define BIND_INVALID					0x0000
#define CWB_FIRST_TIME				0x0001
#define CWB_REBIND					0x0002
#define CWB_ROWCHANGE				0x0004
#define CWB_UNBIND					0x0008
#define CWB_NOCHANGE					0x0010


#define GetNumItemsInArray(rgname) (sizeof((rgname)) / sizeof((rgname[0])))


// Window ids used for create window and stat bar descriptions
#define IDW_OUTWIN		idsOutputWinDesc
#define IDW_TOP			idsInputEditWinDesc
#define IDW_BOTTOM		idsOutputEditWinDesc
#define IDW_STMTS			idsStmtWinDesc
#define IDW_ENVS			idsEnvWinDesc


#define IDD_OPEN_FILE				1700
#define IDD_SAVE_FILE				1701

#define TINYBUFF						64
#define SMALLBUFF						128
#define MEDBUFF						256
#define LARGEBUFF						512
#define XLARGEBUFF					1024

#define CALLED_FROM_MENU			65000

//To flag whether a hdesc was implicitly or
//explicitly allocated
#define EXPLICIT_ALLOC				TRUE
#define IMPLICIT_ALLOC				FALSE

#define RC_NOTSUCCESSFUL(rc) ((rc) != SQL_SUCCESS && (rc) != SQL_SUCCESS_WITH_INFO)
#define RC_SUCCESSFUL(rc) ((rc) == SQL_SUCCESS || (rc) == SQL_SUCCESS_WITH_INFO)
#define RC_SUCCESSFUL_NDF(rc) (RC_SUCCESSFUL(rc) || ((rc) == SQL_NO_DATA_FOUND))
#define RC_STILLEXECUTING(rc) ((rc) == SQL_STILL_EXECUTING)
#define RC_ERROR(rc) ((rc) == SQL_ERROR)

#define GETCIPOINTER(x) GetWindowLongPtr(x, 0)
#define SETCIPOINTER(x, y) SetWindowLongPtr(x, 0, (LONG_PTR)(y))

#define SIZEOF(x) (sizeof((x)) / sizeof(TCHAR))

#define ABS(a) (((a) < 0) ? ((a) * -1) : (a))

#define API_NUM_CORE_LVL1_OFFSET (SQL_EXT_API_START - SQL_NUM_FUNCTIONS)

typedef signed char		 			STINYINT;
typedef unsigned char		 		UTINYINT;
typedef RECT *					lpRECT;

#define DFT_ODBC_HNDLS				30

#define MAXOUTBUFF					300
#define MAXRETURNCODE            35
#define DFTMSGBUFF					75
#ifdef WIN32
#define MAX_ALLOCATED_BUFFER		MAXVIRTMEM
#else
#define MAX_ALLOCATED_BUFFER		((UINT)-1)
#endif

// These two #defines go together
#define DFT_BUFF_LEN_CHARS		3
#define DFT_BUFF_LEN_MAX		999
// These two do too
#define DFT_PARM_LEN_CHARS		3
#define DFT_PARM_LEN_MAX		999
#define DFT_BIND_BUFF_CHARS	5

#define OUTWIN_WIDTH				600
#define OUTWIN_HEIGHT			250
#define ENV_WIDTH					165

//
// Turn on asserts by default for debug, non-retail builds
//
//#if defined( _DEBUG ) && !defined( _RETAIL ) && !defined( _SHOWASSERT )
//#define _SHOWASSERT
//#endif

#ifdef _SHOWASSERT
#define ONDEBUG(funcrtn)			funcrtn
VOID INTFUN DoAssertCheck(BOOL fCond, LPTSTR szMsg, LPTSTR szFile, int iLine);
#define ASSERTCHECK(cond,msg) \
{ \
	const TCHAR * szmsg = (LPTSTR)msg; \
	DoAssertCheck(cond, (LPTSTR)szmsg, (LPTSTR)vszFile, __LINE__); \
}
#else
#define ONDEBUG(funcrtn)
#define ASSERTCHECK(cond,msg)
#endif


#define MSG_SEV0				0
#define MSG_SEV1				1
#define MSG_SEV2				2
#define MSG_SEV3				3

#define GETOUTPUTWINDOW(ci) ((ci) ? ci->hwndOut : hwndOutWin)

#define DFT_ETO_FLAGS (ETO_OPAQUE | ETO_CLIPPED)

#define FREE_PTR(ptr) \
{                     \
	if(ptr)            \
		ReleaseMemory(ptr); \
	ptr = NULL;            \
}


// Resets the given edit window, if that window was the focus window, its new value
//	is stored again for later
#define  DORESETEDITWINDOW(focuswin, addrhwnd, id, style,x,y,nWidth,nHeight) \
{                                                  \
	BOOL fFocus=(focuswin == addrhwnd);             \
	ResetEditWindow(&addrhwnd, hInst, id, style,x,y,nWidth,nHeight);   \
	if(fFocus) focuswin = addrhwnd;                 \
}


#ifdef WIN32
#define GETADDRINFO(ptr)		(ptr)
#define ADDRESSINFO			TEXT("0x%08p")
#else
#define GETADDRINFO(ptr)	HIWORD(ptr), LOWORD(ptr)
#define ADDRESSINFO			TEXT("0x%04X:0x%04X")
#endif

#define szHENV		TEXT("henv")
#define szHDBC		TEXT("hdbc")
#define szHSTMT	TEXT("hstmt")
#define szHDESC	TEXT("hdesc")
#define szNULL		TEXT("<null>")

//descriptor types, these will be set in the descriptor record for
//easy, fast comparison.
#define DESC_ARD	100
#define DESC_APD	101
#define DESC_IRD	102
#define DESC_IPD	103


//Used by routines which set descriptor fields
#define NOT_USED	1

//Used in usrwndp.c and gtrcomm.c
#define USE_C_DFT_ARRAY		1

//*------------------------------------------------------------------------
//| USROPTIONS
//|	Used to track the user options for the global application.  Note that
//|	some options may be specified on a per connection window basis.
//*------------------------------------------------------------------------
typedef struct tagUSROPTIONS {
	// Flags
	BOOL				fAutoAsync;				// TRUE if we're executing statements in auto mode
	BOOL				fAsyncLog;				// TRUE if auto async statements should be logged
	BOOL				fDriverMenus;			// TRUE if we're disabling unsupported commands
	BOOL				fAutoErrors;			// TRUE if we should automatically check for errors
	BOOL				fNullChecking;			// TRUE if we should verify NULL terminators
	BOOL				fBufferChecking;		// TRUE if we should do buffer overwrite checking
	BOOL				fVersionNumbers;		// TRUE if constants should show version numbers
	BOOL				fUseCursorLib;			// Value for SQL_ODBC_CURSORS
	BOOL				fBindColZero;			// TRUE if we need to bind column 0 (i.e. bookmark col)
	UINT				uLogLevel;				// Logging level for filter
	BOOL				fDMTrace;				// TRUE if the driver manager tracing is enabled
	BOOL				fTrace;					// TRUE if we're tracing at the moment
	TCHAR				szLog[_MAX_PATH];		// The name of the file to log to

	// Limits
	UWORD				uDftBuffLen;			// The default buffer length to use (Gator default is 300)
	UWORD				uDftParmLen;			// How much data should be logged for parameters

	// Dipslay information
	BOOL				fMaximize;				// True if we should maximize on startup
	BOOL				fMinimize;				// True if we need to restore default window size
	TCHAR				szFont[LF_FACESIZE+1];	// The font face to use
	int				iPtSize;					// User defined point size
	int				iFontHeight;			// The font height used in the LOGFONT struct
	int				x;							// The horizontal position of window when last closed
	int				y;							// The vertical position of window when last closed
	int				nWidth;					// The window width when last closed
	int				nHeight;					// The window height when last closed

	BOOL				fOutputMaximize;		// True if we need to maximize the Child windows on startup.
	BOOL				fOutputMinimize;// True if we need to minimize the Child windows on startup.
	int				xOutput;					// The horizontal position of the output window when last closed
	int				yOutput;					// The vertical position of the output window when last closed
	int				nWidthOutput;			// The output window's width when last closed
	int				nHeightOutput;			// The output window's height when last closed

	// Working vars
	BOOL				fResetFont;				// Flag to determine if font should be reset
	HWND				hwndFocus;				// Used on font reset to track focus window

	// Rowset information
	UWORD				uReservedRows;			// For extra rows in rowset, used for SetPos
	UWORD				uDftBindBuff;			// Default buffer for Bind Col All
	BOOL				fShowStatus;			// True if status column should be displayed
	BOOL				fShowPcbs;				// True if all pcb values should be displayed
	BOOL				fShowColumns;			// True if we can call DescribeCol for column names
	BOOL				fAutoDisplay;			// True if gator should automatically display
													//		the rowset after a Fetch/FetchScroll/ExtendedFetch

	// Misc info
													// The inital data source for connect
	TCHAR				szInitialDSN[SQL_MAX_DSN_LENGTH+1];
	TCHAR				szInitialUID[35];		// Initial user id
	BOOL				fAllocEnv;				// True if we need to allocate new hEnv for this Full Connect
	int				nODBCVer;				// Which menu to load based on ODBC Version
	BOOL				fODBC30;					// Are we running ODBC 3.0 Gator?
	BOOL				fODBCBehavior;			// Do we set ODBC 3.0 Behavior Env. Attr.
	BOOL				fRecord;					// True if user is recording keystrokes
	TCHAR				szRecLog[_MAX_PATH];	// The name of the file to log keystroks to
	FILE				*fhRecordFile;			// File handle to Record log file
	BOOL				fPlayBack;				// True if user is playing back keystrokes
	} USROPTIONS, * lpUSROPTIONS;


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN TruncateEditContents(HWND hwnd);
UINT CDECL szPrintf(lpUSROPTIONS lpUsrOpts, HWND hwnd, UINT uSev,
			BOOL fCheckLog, BOOL fFormat, LPTSTR szFmt, ...);
void INTFUN fBusy(BOOL flag);
LPTSTR INTFUN GetGtrString(LPTSTR buf, int cbbuf, UINT ids);
LPSTR INTFUN GetGtrStringA(LPSTR buf, int cbbuf, UINT ids);
VOID INTFUN GetDSNs(HWND hwnd);
HFONT INTFUN CreateEditFont();
HFONT INTFUN CreateColumnFont();
VOID INTFUN Draw3DEdges(HWND hwnd, HDC hdc, RECT * lpRect);
VOID INTFUN RefreshWindow(HWND hwnd);
int INTFUN GetSqlFuncOffset(UWORD funcid);
int INTFUN GetAPINumIndex(UWORD funcid);
int INTFUN GetApiNum(UWORD dex);
BOOL INTFUN IsODBC30Api(UWORD nAPINum);
LPTSTR GetRetCodeName(RETCODE rc, LPTSTR str);
LPTSTR GetInstallerRetCodeName(BOOL fRc, LPTSTR str);
LPTSTR GetInstallerRetCodeName30(RETCODE rc, LPTSTR str);
BOOL INTFUN fAbortCheck(HWND hwndOut);
int INTFUN InvokeHelp(HWND hwnd, LPCTSTR szHelpFile, LPCTSTR szKeyword);
HWND INTFUN CreateEditWindow(HWND hwndParent, HINSTANCE hInst, UINT id, DWORD uStyle,
									  	int x,int y,int nWidth,int nHeight);
VOID INTFUN DoResetWrap();
VOID INTFUN ToolsUserOptions(HWND hwnd, lpUSROPTIONS lpUsrOpts);
BOOL INTFUN SaveGetUserOptions(lpUSROPTIONS lpUsrOpts, BOOL fSave);
UINT EXTFUN Hook3d(HWND	hDlg, UINT	msg, WPARAM	wParam, LPARAM	lParam);

//Bug #361
BOOL INTFUN IsVarLengthDataType(SWORD fCType);

VOID INTFUN AddNodeToToolBar(UINT cNum,HANDLE handle,HWND hwnd, LPTSTR szHandle);
DWORD INTFUN DropNodeFromToolBar(HWND hwnd,DWORD dwInd);
VOID INTFUN FindComboRowHeight(HWND hwnd);

BOOL INTFUN DisplayErrors(HWND hwnd,HENV henv,HDBC hdbc,BOOL fMsgBox,
	int idsTitle);
BOOL INTFUN DisplayErrors30(HWND hwnd,HENV henv,HDBC hdbc,BOOL fMsgBox,
	int idsTitle);
DWORD	INTFUN FindIndexInDropDown(HWND hwnd,HANDLE handle,UINT cNum,LPTSTR szHandle);
LPTSTR INTFUN HandleToText(LPTSTR szAddress,SQLHANDLE handle);
INT INTFUN TextToHandle(LPTSTR szAddress,SQLHANDLE *phandle);

BOOL INTFUN IsCharData(SWORD fCType);
BOOL INTFUN IsVarLenData(SWORD fCType);

BOOL INTFUN IsSigned(LPTSTR str,BOOL *pfPos);

int INTFUN GetTrueWindowTextLength(HWND hwnd,int id);
SWORD INTFUN GetCTypeIndex(BOOL fUseCDft,LPTSTR szfCType);
SWORD INTFUN GetSQLTypeIndex(LPTSTR szSQLType,BOOL fUseCDft);
LPTSTR INTFUN ConvertToUnicode(LPSTR szStrA,LPTSTR szStrW, WORD cbStrW);
LPSTR INTFUN ConvertToANSI(LPTSTR szStrW,LPSTR szStrA, WORD cbStrA);
LRESULT INTFUN GtrSendMessage(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
BOOL INTFUN GtrSetWindowText(HWND hWnd,PTR lpStr);
LRESULT INTFUN GtrSendDlgItemMessage(HWND hDlg,int nIDDlgItem,UINT Msg,WPARAM wParam,LPARAM lParam);
BOOL INTFUN GtrSetDlgItemText(HWND hDlg,int nIDDlgItem,PTR lpStr);


BOOL EXTFUN IsWindowsWOW(void);
BOOL EXTFUN IsWin32s(void);
BOOL EXTFUN IsWindowsMIPS(void);
BOOL EXTFUN IsWindowsDEC(void);
BOOL EXTFUN IsWindowsIntel(void);
LPTSTR EXTFUN DLLGetEnv (LPTSTR lpszVariableName);
BOOL EXTFUN IsChicago(void);
BOOL INTFUN Is2XDriver(HDBC hdbc);

VOID INTFUN ResetEditWindow(HWND * lphwnd, HINSTANCE hInst,UINT id, DWORD uStyle,int x,int y,int nWidth,int nHeight);

LPTSTR INTFUN FindGatorIni(LPTSTR szPath);
VOID INTFUN InitFont(LPLOGFONT lf,LPTSTR szFont);
int  CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,int FontType,LPARAM lParam);

#endif

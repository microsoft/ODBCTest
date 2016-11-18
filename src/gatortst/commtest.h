//*---------------------------------------------------------------------------------
//|  Title:		COMMTEST.H
//|		Copyright (c) Microsoft Corporation
//|
//|	This file contains the prototypes and external defs for the test API.
//*---------------------------------------------------------------------------------

#ifndef COMMTEST_DEFS
#define COMMTEST_DEFS

//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define VszFile static TCHAR vszFile[]=TEXT(__FILE__)

#ifndef UNICODE
#define GATORTST_ORGFILENAME "GTRTST32.DLL\0"
#else
#define GATORTST_ORGFILENAME "GTRTS32W.DLL\0"
#endif


#define CTL3D_MESSAGES(hDlg, msg, wParam, lParam)
#define MAXTESTNAME						  64
#define MAXTESTDESC	   				  70
#define MAXTESTSOURCE					  50
#define MAXLISTBOXSIZE 					 100
#define MAXOUTBUFF	  					 200
#define MAXFILELEN	  			 _MAX_PATH
#define MAXTESTBUFF						4000
#define MAXTITLE							  35

#ifndef EXE_INVOKED
// Common Dialog Ids
#define IDT_PROMPT							 2999
#define IDD_BROWSE							 3000
#define IDD_GETLOG							 3001
#define IDC_APPEND							 3002
#define IDC_OPEN_TREE                   3101
#define IDE_OPEN_FILENAME               3102
#define IDC_OPEN_FILELIST               3103
#define IDC_OPEN_FILETYPE               3104
#define IDC_OPEN_DRIVES                 3105
#define IDCK_OPEN_APPEND                3106
#define IDD_OPEN_FILETREE               3107
#define IDC_OPEN_DIR_LIST               3108
#define IDD_OPEN_DIR                    3109


// Resource string ids
#define idsAutoTestNotInIni  				15000
#define idsAutoTestLoadFailed				15001
#define idsAutoTestGetProcFailed			15002
#define idsAutoTestAlreadyInstalled		15003
#define idsRemoveAutoTest					15004
#define idsAutoTestNotValid				15005
#define idsAutoTestNameInvalid			15006
#define idsDialogNotLoaded					15007
#define idsMustSelectSourceNTest			15008
#define idsNowExecuting						15009
#define idsAutoTestSource					15010
#define idsTimeStarted						15011
#define idsTimeFinished						15012
#define idsAborted							15013
#define idsTestCompleted					15014
#define idsTimeTotal							15015
#define idsNoFailures						15016
#define idsTestFailure 						15017
#define idsTestFailures						15018
#define idsGrandTotalErrors				15019
#define idsTestCase							15020
#define idsIniFileNotFound					15021
#define idsLogFileNotValid					15022
#define idsDeleteGroup						15023
#define idsReplaceGroup						15024
#define idsNowExecutingGroup				15025
#define idsGroupGrandTotal					15026
#define idsBufferOverflow					15027
#define idsLogFilter							15028
#define idsGroupNameInUse					15029
#define idsAutoNameInUse					15030
#define idsLogTitle							15031
#define idsOverWriteFile					15032
#define idsDefault							15033
#define idsGetNewRunList					15034
#define idsDeleteRunList					15035
#define idsRunListFilter					15036
#define idsFileExtMustBeRls				15037
#define idsDeleteSource						15038
#define idsSourceNameInUse					15039
#define idsReplaceSource					15040
#define idsTestCaseCountMismatch			15041
#define idsTestCaseUpdatedRun				15042
#define idsTestCaseUpdated					15043
#define idsTestCaseNotCorrect				15044
#define idsTruncated							15045
#define idsDeleteFileNotFound				15046
#define idsNoDSNsInstalled					15047
#define idsLibraryFileNotFound			15048
#define idsRlsPrompt							15049
#define idsKeywords							15050


// For adding/removing auto tests
#define ADD_AT_NEW_IN_LIST		 	1
#define ADD_AT_ALREADY_IN_LIST	2
#define ADD_AT_PATH_DIFFERS		4

// For szLogPrintf errors
#define SLP_BUFFER_OVERFLOW	1
#define SLP_LOG_FILE_ERROR		2
#define SLP_EDIT_WIN_ERROR		4

// Prototypes for the
typedef BOOL (PASCAL *lpAUTOTESTNAME)(LPTSTR, UINT *);
typedef BOOL (PASCAL *lpAUTOTESTDESC)(UWORD, LPTSTR, LPTSTR);
typedef void (PASCAL *lpAUTOTESTFUNC)(lpSERVERINFO);

// The following structure is used to keep track of Auto Test DLL's
typedef struct tagAUTOTESTINFO {
	BOOL					fLoaded;					// TRUE if text descriptions have been loaded
	HINSTANCE			hInst;					// Loaded instance handle from LoadLibrary
	TCHAR					szTestName[MAXTESTNAME];
	TCHAR					szPath[MAXFILELEN];	// Full path name of the test DLL
	UINT					cbTestCases;			// How many tests are there
	lpAUTOTESTNAME		lpTestName;				// Proc address for AutoTestName
	lpAUTOTESTDESC		lpTestDesc;				// Proc address for AutoTestDesc
	lpAUTOTESTFUNC		lpTestFunc;				//	Proc address for AutoTestFunc
	struct tagAUTOTESTINFO * next;		// Next node
	struct tagAUTOTESTINFO * prev;		// Prev node
	} AUTOTESTINFO;
typedef AUTOTESTINFO * lpAUTOTESTINFO;
#endif


//------------------------------------------------------------------------------------------
// .INI macros
//------------------------------------------------------------------------------------------
#define INI_STRING_NOT_FOUND		0
#define INI_DATA_SOURCE				1
#define INI_AUTO_TEST				2
#define INI_GROUP_NAME				4

// General use
#define GetLastRunList(outbuff, cboutbuff)														\
	(GtrGetPrivateProfileString(szSTARTUP, szLASTRUNLIST, NULL, outbuff, cboutbuff, g_szGatorIni))
#define SetLastRunList(list)                                              				\
	(GtrWritePrivateProfileString(szSTARTUP, szLASTRUNLIST, list, g_szGatorIni))

// For auto tests
#define GetDftFromDir(outbuff, cboutbuff)															\
	(GtrGetPrivateProfileString(szSTARTUP, szDFTFROMDIR, NULL, outbuff, cboutbuff, g_szGatorIni))
#define SetDftFromDir(newdir)																			\
	(GtrWritePrivateProfileString(szSTARTUP, szDFTFROMDIR, newdir, g_szGatorIni))
#define GetInstalledTestList(outbuff, cboutbuff)												\
	(GtrGetPrivateProfileString(szAUTOTESTS, NULL, NULL, outbuff, cboutbuff, g_szGatorIni))
#define AddTestNameToIni(test,cnt)																	\
	(GtrWritePrivateProfileString(szAUTOTESTS, test, cnt, g_szGatorIni))
#define GetTestNameFromIni(test, outbuff, cboutbuff)											\
	(GtrGetPrivateProfileString(szAUTOTESTS, test, NULL, outbuff, cboutbuff, g_szGatorIni))
#define GetTestCaseCount(test)																		\
	GtrGetPrivateProfileInt(szAUTOTESTS, test, 0, g_szGatorIni)
#define GetTestDLLPath(test, outbuff, cboutbuff)												\
	(GtrGetPrivateProfileString(test, szDLL, NULL, outbuff, cboutbuff, g_szGatorIni))
#define RemoveTestSectionFromIni(test)																\
	(GtrWritePrivateProfileString(test, NULL, NULL, g_szGatorIni))
#define RemoveTestNameFromIni(test)																	\
	(GtrWritePrivateProfileString(szAUTOTESTS, test, NULL, g_szGatorIni))
#define SetTestDLLPath(test, path)																	\
	(GtrWritePrivateProfileString(test, szDLL, path, g_szGatorIni))

// For data sources
#define GetTestSources(outbuff, cboutbuff)														\
	(GtrGetPrivateProfileString(szSQLDRIVERS, NULL, NULL, outbuff, cboutbuff, g_szGatorIni))
#define GetSourceName(name, outbuff, cboutbuff)                                     \
	(GtrGetPrivateProfileString(szSQLDRIVERS, name, NULL, outbuff, cboutbuff, g_szGatorIni))
#define GetServer0(dsn, buff, cbbuff)																\
	(GtrGetPrivateProfileString(dsn, szSERVER0, NULL, buff, cbbuff, g_szGatorIni))
//#define GetServer1(dsn, buff, cbbuff)
//	(GtrGetPrivateProfileString(dsn, szSERVER1, NULL, buff, cbbuff, g_szGatorIni))
#define GetLogin0(dsn, buff, cbbuff)																\
	(GtrGetPrivateProfileString(dsn, szLOGIN0, NULL, buff, cbbuff, g_szGatorIni))
//#define GetLogin1(dsn, buff, cbbuff)
//	(GtrGetPrivateProfileString(dsn, szLOGIN1, NULL, buff, cbbuff, g_szGatorIni))
#define GetPassword0(dsn, buff, cbbuff)															\
	(GtrGetPrivateProfileString(dsn, szPASSWORD0, NULL, buff, cbbuff, g_szGatorIni))
//#define GetPassword1(dsn, buff, cbbuff)
//	(GtrGetPrivateProfileString(dsn, szPASSWORD1, NULL, buff, cbbuff, g_szGatorIni))
#define GetKeywords(dsn, buff, cbbuff)																\
	(GtrGetPrivateProfileString(dsn, szKEYWORDS, NULL, buff, cbbuff, g_szGatorIni))
#define SetSourceName(source)                                                       \
	(GtrWritePrivateProfileString(szSQLDRIVERS, source, szINSTALLED, g_szGatorIni))
#define RemoveSourceName(source)                                                    \
	{(GtrWritePrivateProfileString(szSQLDRIVERS, source, NULL, g_szGatorIni));				\
	 (GtrWritePrivateProfileString(source, NULL, NULL, g_szGatorIni));}
#define SetServer0(source, server)                                                  \
	(GtrWritePrivateProfileString(source, szSERVER0, server, g_szGatorIni))
#define SetLogin0(source, login)                                                    \
	(GtrWritePrivateProfileString(source, szLOGIN0, login, g_szGatorIni))
#define SetPassword0(source, pwd)                                                   \
	(GtrWritePrivateProfileString(source, szPASSWORD0, pwd, g_szGatorIni))
#define SetKeywords(source, keys)																	\
	(GtrWritePrivateProfileString(source, szKEYWORDS, keys, g_szGatorIni))
//#define SetServer1(source, server)
//	(GtrWritePrivateProfileString(source, szSERVER1, server, g_szGatorIni))
//#define SetLogin1(source, login)
//	(GtrWritePrivateProfileString(source, szLOGIN1, login, g_szGatorIni))
//#define SetPassword1(source, pwd)
//	(GtrWritePrivateProfileString(source, szPASSWORD1, pwd, g_szGatorIni))


// For Groups
#define GetGroupList(outbuff, cboutbuff)															\
	(GtrGetPrivateProfileString(szGROUPS, NULL, NULL, outbuff, cboutbuff, g_szGatorIni))
#define GetGroupAutoTests(group, outbuff, cboutbuff)											\
	(GtrGetPrivateProfileString(group, NULL, NULL, outbuff, cboutbuff, g_szGatorIni))
#define GetGroupNameFromIni(group, outbuff, cboutbuff)										\
	(GtrGetPrivateProfileString(szGROUPS, group, NULL, outbuff, cboutbuff, g_szGatorIni))
#define AddGroupToIni(group)																			\
	(GtrWritePrivateProfileString(szGROUPS, group, szINSTALLED, g_szGatorIni))
#define AddGroupAutoTest(group, autotest)															\
	(GtrWritePrivateProfileString(group, autotest, szINSTALLED, g_szGatorIni))
#define RemoveGroupAutoTest(group, autotest)														\
	(GtrWritePrivateProfileString(group, autotest, NULL, g_szGatorIni))
#define RemoveGroupFromIni(group)																	\
	(GtrWritePrivateProfileString(szGROUPS, group, NULL, g_szGatorIni))
#define RemoveGroup(group)																				\
	(GtrWritePrivateProfileString(group, NULL, NULL, g_szGatorIni))

// Debug constants
#ifndef QUIKTEST
#ifdef _MEM_CHECK
void * EXTFUNCDECL DebugGetMemory(DWORD size, LPTSTR szFile, int cbLine);
void EXTFUN DebugReleaseMemory(LPVOID ptr, LPTSTR szFile, int cbLine);
#define AllocateMemory(size) DebugGetMemory(size, (LPTSTR)vszFile, __LINE__)
#define ReleaseMemory(ptr) DebugReleaseMemory(ptr, (LPTSTR)vszFile, __LINE__)
#else
#define AllocateMemory(size) DoGetMemory(size)
#define ReleaseMemory(ptr) DoReleaseMemory(ptr)
#endif
#endif


#if defined( _DEBUG ) && defined( _SHOWASSERT )
#define ONDEBUG(funcrtn)			funcrtn
#define ASSERTCHECK(cond,msg)                                                                     	\
	{                                                                                               \
	if(cond) {                                                                                      \
		TCHAR tmp[200];                                                                        			\
		const TCHAR szAssertMsg[] = "Assertion failure: %s, %s, %d";												\
		wsprintf((LPTSTR)tmp, szAssertMsg, (LPTSTR)msg, (LPTSTR)vszFile, __LINE__);							\
		MessageBox(GetActiveWindow(), (LPTSTR)tmp, "error", MB_ICONEXCLAMATION | MB_OK);					\
		}                                                                                            \
	}
#else
#define ONDEBUG(funcrtn)
#define ASSERTCHECK(cond,msg)
#endif


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
#ifndef COMMTEST_C_DEFS
extern TCHAR szErrBuff[MAXOUTBUFF];
extern TCHAR szLogFilter[MAXOUTBUFF];
extern TCHAR szLogTitle[MAXTITLE];
/*
extern LPTSTR szErrTitle;
extern LPTSTR szTestTitle;
extern LPTSTR szGATORININAME;
extern LPTSTR szSTARTUP;
extern LPTSTR szLASTRUNLIST;
extern LPTSTR szDFTFROMDIR;
extern LPTSTR szAUTOTESTS;
extern LPTSTR szDLL;
extern LPTSTR szINSTALLED;
extern LPTSTR szSQLDRIVERS;
extern LPTSTR szSERVER0;
extern LPTSTR szLOGIN0;
extern LPTSTR szPASSWORD0;
extern LPTSTR szSERVER1;
extern LPTSTR szLOGIN1;
extern LPTSTR szPASSWORD1;
extern LPTSTR szGROUPS;
extern LPTSTR szLASTRUN;*/
#endif

// Note:  The following strings were being placed in the code segment
//				but this DLL got so large that multiple code segments were
//				being allocated and hence the offsets got screwed up
//
// Generic strings
static TCHAR szErrTitle[]			=	TEXT("Error");
static TCHAR szTestTitle[]			=	TEXT("ODBC Test");
static TCHAR szNewLine[]			=	TEXT("\r\n");

// GATOR.INI sections and entries
#ifndef WIN32
static TCHAR szGATORININAME[]		=	TEXT("GATOR.INI");
#else
#ifdef _UNICODE
static TCHAR szGATORININAME[]		=	TEXT("GATOR32W.INI");
#else
static TCHAR szGATORININAME[]		=	TEXT("GATOR32.INI");
#endif
#endif
static TCHAR szSTARTUP[] 			=  TEXT("startup");
static TCHAR szLASTRUNLIST[]		=  TEXT("Last Run List");
static TCHAR szDFTFROMDIR[] 		=  TEXT("Default From Dir");
static TCHAR szAUTOTESTS[]			=	TEXT("Auto Tests");
static TCHAR szDLL[]				= TEXT("DLL");
static TCHAR szINSTALLED[]			=	TEXT("Installed");
static TCHAR szSQLDRIVERS[]		=	TEXT("SQL_DRIVERS");
static TCHAR szSERVER0[]			=	TEXT("SERVER0");
static TCHAR szLOGIN0[]				=	TEXT("LOGIN0");
static TCHAR szPASSWORD0[]			=	TEXT("PASSWORD0");
static TCHAR szKEYWORDS[]			= 	TEXT("KEYWORDS");
static TCHAR szGROUPS[]				=	TEXT("GROUPS");
static TCHAR szLASTRUN[]			=	TEXT("Last Run");
static TCHAR szODBCTESTHANDLE[]	=	TEXT("ODBC Test Handles");


//----------------------------------------------------------------------------------
//		Functions
//----------------------------------------------------------------------------------

// AUTOTESTINFO functions
lpAUTOTESTINFO AddAutoTest(lpAUTOTESTINFO * head, LPTSTR szName, LPTSTR szPath,
			BOOL fUnload, int * rtnstatus);
lpAUTOTESTINFO LoadAutoTestDLL(lpAUTOTESTINFO ati, LPTSTR szName);
VOID UnLoadAutoTestDLL(lpAUTOTESTINFO ati);
lpAUTOTESTINFO FindAutoTestNode(lpAUTOTESTINFO head, LPTSTR szName);
void DeleteAutoTestNode(lpAUTOTESTINFO * head, lpAUTOTESTINFO ati);
void DeleteAutoTestList(lpAUTOTESTINFO head);
BOOL INTFUN VerifyTestCaseCount(LPTSTR szAutoTest, UINT *pldcnt, BOOL fRun);
int INTFUN RefreshInstalledTestList(HWND hDlg, int id, lpAUTOTESTINFO * lpHead);
UINT INTFUN GetIniNameUsage(LPTSTR szName);


// Output functions
//BOOL EXTFUN DoszWrite(HWND hwnd, LPTSTR str, BOOL flag);
//BOOL EXTFUN DumpLogInfo(LPTSTR szLogFile, LPTSTR str);
//UINT EXTFUNCDECL CDECL szLogPrintf(HWND hwnd, LPTSTR szFile, LPTSTR szFmt, ...);
//VOID EXPORT CDECL szTimePrintf(HWND hWnd, LPTSTR szFmt, ...);
BOOL EXTFUN DoszWrite(lpSERVERINFO lps, LPTSTR str, BOOL fForce, BOOL fNew);
BOOL EXTFUNCDECL szLogPrintf(lpSERVERINFO lps, BOOL fForce, LPTSTR szFmt, ...);
int EXTFUNCDECL szMessageBox(HWND hwnd, UINT style, LPTSTR szTitle, LPTSTR szFmt, ...);

// Dialog and utility functions
BOOL EXTFUN FAbortCheck();
TIMEINFO * EXTFUNCDECL CElapsed(lpTIMEINFO start, lpTIMEINFO end, lpTIMEINFO elapsed);
void EXTFUN CenterDialog(HWND hdlg);
LPTSTR INTFUN FindFileDir(LPTSTR szFile, LPTSTR szPath);
BOOL EXTFUN GetLogFile(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
			LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL * lpfAppend);
LPTSTR EXTFUN GetFilter(LPTSTR szOut, int cbOut, UINT ids);
BOOL EXTFUN BrowseDirs(HWND hDlg, HINSTANCE hInst,
					LPCSTR lpszPrompt, LPTSTR lpszDrive);
UINT EXTFUN CommDlgHook(HWND	hdlg,
						UINT	msg,
						WPARAM	wParam,
						LPARAM	lParam);
void Busy(int flag);
UINT EnumerateListBox(HWND hwndParent, HWND hwndList, unsigned msg);
LPTSTR BuildSelList(HWND hwndList);
BOOL IsValidTestName(LPTSTR szName);
UINT	INTFUN CountNullArray(LPTSTR instr);
BOOL INTFUN DoSetFile(HWND hwnd, LPCTSTR szInFile);
LPTSTR EXTFUN GetString(LPTSTR buf, int cbbuf, UINT ids);


// Memory functions
void * EXTFUNCDECL DoGetMemory(DWORD size);
void EXTFUN DoReleaseMemory(LPVOID ptr);

//Platform determining functions
BOOL EXTFUN IsWindowsWOW(void);
BOOL EXTFUN IsWin32s(void);
BOOL EXTFUN IsWindowsMIPS(void);
BOOL EXTFUN IsWindowsDEC(void);
BOOL EXTFUN IsWindowsIntel(void);
LPTSTR EXTFUN DLLGetEnv (LPTSTR lpszVariableName);
BOOL EXTFUN IsChicago(void);
VOID INTFUN SetupFileInfo(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
		LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL * lpfAppend,
		OPENFILENAME *lpofn);

VOID EXTFUN SetGatorIniPath(LPTSTR szGatorIni);

int GtrMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
BOOL GatorSetWindowText(HWND hWnd,LPCTSTR lpString);
DWORD GtrSearchPath(LPCTSTR lpPath,LPCTSTR lpFileName,LPCTSTR lpExtensions, DWORD nBufferLength,
				 LPTSTR lpBuffer,LPTSTR *lpFilePart);
HINSTANCE GtrLoadLibrary( LPCTSTR lpLibFileName);
HANDLE GtrCreateFile(LPCTSTR lpFileName,DWORD dwDesiredAccess,DWORD dwSharedMode,LPSECURITY_ATTRIBUTES
				  lpSecurityAttributes,DWORD dwCreationDistribution,DWORD FlagsAndAttributes,
				  HANDLE hTemplateFile);
DWORD GtrFormatMessage(DWORD dwFlags,LPCVOID lpSource, DWORD dwMessageid, DWORD dwLanguageid,
					LPTSTR lpBuffer,DWORD nSize,va_list * Arguments);
int GtrLoadString(HINSTANCE hInstance,UINT uID, LPTSTR lpBuffer, int nBufferMax);
BOOL GtrWritePrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpString,
							   LPCTSTR lpFileName);
DWORD GtrGetPrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault,
							 LPTSTR lpReturnedString, DWORD nSize,LPCTSTR lpFileName);
UINT GtrGetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName, INT nDefault,LPCTSTR lpFileName);
//int _openA(const wchar_t *filename,int oflag,int pmode);


#endif

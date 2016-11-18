//*---------------------------------------------------------------------------------
//|  Title:		RUNTEST.H
//|		Copyright (c) Microsoft, All rights reserved
//|
//|	This file prototypes for running autotests from the command line
//*---------------------------------------------------------------------------------
#ifndef RUNTEST_DEFS
#define RUNTEST_DEFS
#include "autotest.h"
#include "portable.h"

#define MAXTESTNAME	TINYBUFF
#define MAX_TESTS		50


// #define's
// General use
#define GetLastRunList(outbuff, cboutbuff)														\
	(GtrGetPrivateProfileString(szSTARTUP, szLASTRUNLIST, NULL, outbuff, cboutbuff, szGATORINI))
#define SetLastRunList(list)                                              				\
	(GtrWritePrivateProfileString(szSTARTUP, szLASTRUNLIST, list, szGATORINI))

// For auto tests
#define GetDftFromDir(outbuff, cboutbuff)															\
	(GtrGetPrivateProfileString(szSTARTUP, szDFTFROMDIR, NULL, outbuff, cboutbuff, szGATORINI))
#define SetDftFromDir(newdir)																			\
	(GtrWritePrivateProfileString(szSTARTUP, szDFTFROMDIR, newdir, szGATORINI))
#define GetInstalledTestList(outbuff, cboutbuff)												\
	(GtrGetPrivateProfileString(szAUTOTESTS, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define AddTestNameToIni(test,cnt)																	\
	(GtrWritePrivateProfileString(szAUTOTESTS, test, cnt, szGATORINI))
#define GetTestNameFromIni(test, outbuff, cboutbuff)											\
	(GtrGetPrivateProfileString(szAUTOTESTS, test, NULL, outbuff, cboutbuff, szGATORINI))
#define GetTestCaseCount(test)																		\
	GetPrivateProfileInt(szAUTOTESTS, test, 0, szGATORINI)
#define GetTestDLLPath(test, outbuff, cboutbuff)												\
	(GtrGetPrivateProfileString(test, szDLL, NULL, outbuff, cboutbuff, szGATORINI))
#define RemoveTestSectionFromIni(test)																\
	(GtrWritePrivateProfileString(test, NULL, NULL, szGATORINI))
#define RemoveTestNameFromIni(test)																	\
	(GtrWritePrivateProfileString(szAUTOTESTS, test, NULL, szGATORINI))
#define SetTestDLLPath(test, path)																	\
	(GtrWritePrivateProfileString(test, szDLL, path, szGATORINI))

#define GetTestSources(outbuff, cboutbuff)														\
	(GtrGetPrivateProfileString(szSQLDRIVERS, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define GetSourceName(name, outbuff, cboutbuff)                                     \
	(GtrGetPrivateProfileString(szSQLDRIVERS, name, NULL, outbuff, cboutbuff, szGATORINI))
#define GetServer0(dsn, buff, cbbuff)																\
	(GtrGetPrivateProfileString(dsn, szSERVER0, NULL, buff, cbbuff, szGATORINI))
#define GetLogin0(dsn, buff, cbbuff)																\
	(GtrGetPrivateProfileString(dsn, szLOGIN0, NULL, buff, cbbuff, szGATORINI))
//#define GetLogin1(dsn, buff, cbbuff)
//	(GtrGetPrivateProfileString(dsn, szLOGIN1, NULL, buff, cbbuff, szGATORINI))
#define GetPassword0(dsn, buff, cbbuff)															\
	(GtrGetPrivateProfileString(dsn, szPASSWORD0, NULL, buff, cbbuff, szGATORINI))
//#define GetPassword1(dsn, buff, cbbuff)
//	(GtrGetPrivateProfileString(dsn, szPASSWORD1, NULL, buff, cbbuff, szGATORINI))
#define GetKeywords(dsn, buff, cbbuff)																\
	(GtrGetPrivateProfileString(dsn, szKEYWORDS, NULL, buff, cbbuff, szGATORINI))
#define SetSourceName(source)                                                       \
	(GtrWritePrivateProfileString(szSQLDRIVERS, source, szINSTALLED, szGATORINI))
#define RemoveSourceName(source)                                                    \
	{(GtrWritePrivateProfileString(szSQLDRIVERS, source, NULL, szGATORINI));				\
	 (GtrWritePrivateProfileString(source, NULL, NULL, szGATORINI));}
#define SetServer0(source, server)                                                  \
	(GtrWritePrivateProfileString(source, szSERVER0, server, szGATORINI))
#define SetLogin0(source, login)                                                    \
	(GtrWritePrivateProfileString(source, szLOGIN0, login, szGATORINI))
#define SetPassword0(source, pwd)                                                   \
	(GtrWritePrivateProfileString(source, szPASSWORD0, pwd, szGATORINI))
#define SetKeywords(source, keys)																	\
	(GtrWritePrivateProfileString(source, szKEYWORDS, keys, szGATORINI))
//#define SetServer1(source, server)
//	(GtrWritePrivateProfileString(source, szSERVER1, server, szGATORINI))
//#define SetLogin1(source, login)
//	(GtrWritePrivateProfileString(source, szLOGIN1, login, szGATORINI))
//#define SetPassword1(source, pwd)
//	(GtrWritePrivateProfileString(source, szPASSWORD1, pwd, szGATORINI))


// For Groups
#define GetGroupList(outbuff, cboutbuff)															\
	(GtrGetPrivateProfileString(szGROUPS, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define GetGroupAutoTests(group, outbuff, cboutbuff)											\
	(GtrGetPrivateProfileString(group, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define GetGroupNameFromIni(group, outbuff, cboutbuff)										\
	(GtrGetPrivateProfileString(szGROUPS, group, NULL, outbuff, cboutbuff, szGATORINI))
#define AddGroupToIni(group)																			\
	(GtrWritePrivateProfileString(szGROUPS, group, szINSTALLED, szGATORINI))
#define AddGroupAutoTest(group, autotest)															\
	(GtrWritePrivateProfileString(group, autotest, szINSTALLED, szGATORINI))
#define RemoveGroupAutoTest(group, autotest)														\
	(GtrWritePrivateProfileString(group, autotest, NULL, szGATORINI))
#define RemoveGroupFromIni(group)																	\
	(GtrWritePrivateProfileString(szGROUPS, group, NULL, szGATORINI))
#define RemoveGroup(group)																				\
	(GtrWritePrivateProfileString(group, NULL, NULL, szGATORINI))


// Prototypes for the
typedef BOOL (PASCAL *lpAUTOTESTNAME)(LPTSTR, UINT *);
typedef BOOL (PASCAL *lpAUTOTESTDESC)(UWORD, LPTSTR, LPTSTR);
typedef void (PASCAL *lpAUTOTESTFUNC)(lpSERVERINFO);

// The following structure is used to keep track of Auto Test DLL's
typedef struct tagAUTOTESTINFO {
	BOOL					fLoaded;					// TRUE if text descriptions have been loaded
	HINSTANCE			hInst;					// Loaded instance handle from LoadLibrary
	TCHAR					szTestName[MAXTESTNAME];
	TCHAR					szPath[_MAX_PATH];	// Full path name of the test DLL
	UINT					cbTestCases;			// How many tests are there
	lpAUTOTESTNAME		lpTestName;				// Proc address for AutoTestName
	lpAUTOTESTDESC		lpTestDesc;				// Proc address for AutoTestDesc
	lpAUTOTESTFUNC		lpTestFunc;				//	Proc address for AutoTestFunc
	struct tagAUTOTESTINFO * next;		// Next node
	struct tagAUTOTESTINFO * prev;		// Prev node
	} AUTOTESTINFO;
typedef AUTOTESTINFO * lpAUTOTESTINFO;

typedef struct tagTEST
{
	TCHAR	szTest[MEDBUFF];
	UINT	rglMask;

} TEST;
typedef TEST *lpTEST;


typedef struct tagLVLINDEX {
	UINT		fOpt;								// Bitmask for operation
	UINT		group;							// Group index
	UINT		autot;							// Auto test within group
	UINT		testc;							// Test within auto test
	} LVLINDEX;
typedef LVLINDEX * lpLVLINDEX;


// Typedef for the strings
typedef struct tagARRAYLPTSTR {
	LPTSTR			str;
	} ARRAYLPTSTR;
typedef ARRAYLPTSTR * lpARRAYLPTSTR;


// This structure is used to the attributes which all levels
// at a level have in common
typedef struct tagUDLLEVELDESC {
	UINT										fx;				// Free space width
	UINT										bx;				// Bitmap width
	UINT										by;				// Bitmap height
	UINT										ux;				// User text width (pixels)
	HBITMAP									hMap;				// Handle to bitmap for this level
	} UDLLEVELDESC;
typedef UDLLEVELDESC * lpUDLLEVELDESC;


// Information on an actual physical level including all user test and
// selection/expansion information. Next is an array of items below this level
typedef struct tagUDLINFOLEVEL {
	UINT										level;			// Which level is this (0 based)
	UINT										cItems;			// How many items at this level
	BOOL										fLoaded;			// TRUE if szItems have been set
	lpARRAYLPTSTR							szItems;			// Array of pointers to item data
	UINT *								aSelected;		// Bitmask of selected item
	UINT *								aPartial;		// Upper level partial select
	UINT *								aExpanded;		// Bitmask of expanded items
	UDWORD									cNextItems;		// How many items at next level
	UINT										pdex;				// Parent index
	struct tagUDLINFOLEVEL *		prev;				// Owner level, NULL for parent
	struct tagUDLINFOLEVEL * * 	next;				// Array of levels below this
	} UDLINFOLEVEL;
typedef UDLINFOLEVEL * lpUDLINFOLEVEL;

// Information about the box including everything required for drawing the
// elements and points to the correct structures
typedef struct tagUDLINFOBOX {
	HINSTANCE				hInst;			// Instance handle of this owner
	HWND						hwnd;				// Window handle of control
	HWND						hwndOwner;		// Owner window on create
	HWND						hwndScroll;		// Handle of scrollbar
	BOOL						fhwndScroll;	// TRUE if enabled
	HWND						hwndBox;			// Handle of client draw area
	UINT						cLevels;			// How many levels are there?
	UINT						dx;				// Width of hwndBox
	UINT						dy;				// Height of hwndBox
	UINT						ly;				// Line height max
	LVLINDEX					top;				// Top level information
	UDWORD					cVisible;		// Total rows that will fit in box
	UINT						cCurRelative;	// Current realtive row number
	UINT						cScrollPos;		// Relative to over-all scroll
	UINT						cRange;			// Range for scroll bar
	HFONT						hFont;			// The font to use for drawing
	UINT						cx;				// Average width of character
	UINT						cy;				// Height of average character
	UINT						cbExtra;			// White space to use for drawing
	HBITMAP					hPlus;			// More levels to be seen
	HBITMAP					hMinus;			// No more levels to be seen
	UINT						ex;				// Expansion indicator width
	UDWORD					cTotItems;		// Total items for all levels
	UDWORD					cTotVisible;	// Total visible items for all levels
	lpUDLLEVELDESC			lpdesc;			// Array of level descriptors
	lpUDLINFOLEVEL			lplvl;			// First level descriptor
	} UDLINFOBOX;
typedef UDLINFOBOX * lpUDLINFOBOX;

// Following structure is used to track the selected auto tests
typedef struct tagRUNTESTLIST {
	BOOL						fRun;								// TRUE if we should run
	BOOL						fAppend;							// TRUE if we should append to existing log files
	HINSTANCE				hInst;							// Instance handle of owner
	HWND						hRunDlg;							// Window handle of Run Auto Tests
	LPTSTR					szSource;						// Dbl-null list of sources
	TCHAR						szRunList[_MAX_PATH];		// Name of Run List we're using
	lpSERVERINFO			rs;								// Pointer to struct to file out
	lpAUTOTESTINFO			head;								// Auto test list for run-info
	lpUDLINFOBOX			lpbox;							// The user drawn list of groups
	} RUNTESTLIST;
typedef RUNTESTLIST *  lpRUNTESTLIST;


UINT INTFUN GetNumSources(LPTSTR lpSrcs,LPTSTR szSources);

void INTFUN FillServerInfo(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,HWND hwnd, HINSTANCE hInst,
				HENV henv, HDBC hdbc, HSTMT hstmt);

void INTFUN SetAutoTestBits(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,int cCnt,
	LPTSTR szName,LPTSTR szTestCase);

void INTFUN ProcessSection(FILE *fp,LPTSTR pSection,LPTSTR szSources);

void INTFUN ProcessEntry(lpSERVERINFO lpSI,LPTSTR pTok,
	lpTEST rgAutoTest,UINT *puNumTests);

UINT INTFUN HandleRunList(lpSERVERINFO lpSI,LPTSTR szRunList,
	LPTSTR szSources,lpTEST rgAutoTest);

void INTFUN RunAutoTests(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,UINT uNumTests,
	LPTSTR szAutoTest,LPTSTR szTestCase,TEST rgAutoTests[],LPTSTR szSources,
	HWND hwnd);

UINT INTFUN ProcessCmdLine(LPTSTR lpszCmdLine,LPTSTR szAutoTest,LPTSTR szTestCase,
		LPTSTR szKeyWords,lpSERVERINFO lpSI,LPTSTR szSources,TEST rgAutoTests[]);

BOOL INTFUN RunFromCmdLine(lpRUNTESTLIST prst,LPTSTR lpszCmdLine,HWND hwnd);

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

#endif
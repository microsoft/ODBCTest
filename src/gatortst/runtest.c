//*---------------------------------------------------------------------------------
//|	File:		RUNTEST.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module contains the functions used to load and run a test DLL.
//*---------------------------------------------------------------------------------
#include "gtrhdrs.h"
#include "file.h"
#include <dlgs.h>

#pragma warning(disable:4996)
#pragma warning(disable:4995)
//----------------------------------------------------------------------------------
//		Macros and defines
//----------------------------------------------------------------------------------
#define MAXRUNLIST	35

#define USER_RESET_AUTO_TESTS			WM_USER + 170
#define USER_RESET_TEST_SOURCES		WM_USER + 171
#define USER_RESET_BUTTON_STATUS		WM_USER + 172
#define USER_GET_OPTIONS				WM_USER + 173
#define USER_RESET_DFT_RUNLIST		WM_USER + 174
#define USER_SET_BUTTON_STATUS		WM_USER + 175


#define GetLastRun(group,autot,buff,cbbuff,file)												\
	(GtrGetPrivateProfileString(group,autot,NULL,buff,cbbuff,file))
#define SetLastRun(group,autot,dexes,file)														\
	(GtrWritePrivateProfileString(group,autot,dexes,file))
#define GetLastRunDebug(runlist)																		\
	(GtrGetPrivateProfileInt(szOPTIONINFO, szDEBUG, FALSE, runlist))
#define GetLastRunScreen(runlist)																	\
	(GtrGetPrivateProfileInt(szOPTIONINFO, szSCREEN, FALSE, runlist))
#define GetLastLogFile(runlist)																		\
	(GtrGetPrivateProfileInt(szOPTIONINFO, szLOGFILE, FALSE, runlist))
#define GetLastLogFileName(buff, cbbuff, runlist)												\
	(GtrGetPrivateProfileString(szOPTIONINFO, szLOGFILENAME, NULL, buff, cbbuff, runlist))
#define GetLastIsolate(runlist)																		\
	(GtrGetPrivateProfileInt(szOPTIONINFO, szISOLATE, FALSE, runlist))
#define GetLastCursorLib(runlist)																	\
	(GtrGetPrivateProfileInt(szOPTIONINFO, szCURSORLIB, FALSE, runlist))
#define GetLastRunSources(outbuff, cboutbuff,runlist)											\
	(GtrGetPrivateProfileString(szSQLDRIVERS, NULL, NULL, outbuff, cboutbuff, runlist))
#define SetLastRunDebug(outbuff, runlist)															\
	(GtrWritePrivateProfileString(szOPTIONINFO, szDEBUG, outbuff, runlist))
#define SetLastRunScreen(outbuff, runlist)														\
	(GtrWritePrivateProfileString(szOPTIONINFO, szSCREEN, outbuff, runlist))
#define SetLastLogFile(outbuff, runlist)                                            \
	(GtrWritePrivateProfileString(szOPTIONINFO, szLOGFILE, outbuff, runlist))
#define SetLastLogFileName(outbuff, runlist)														\
	(GtrWritePrivateProfileString(szOPTIONINFO, szLOGFILENAME, outbuff, runlist))
#define SetLastIsolate(outbuff, runlist)															\
	(GtrWritePrivateProfileString(szOPTIONINFO, szISOLATE, outbuff, runlist))
#define SetLastCursorLib(outbuff, runlist)														\
	(GtrWritePrivateProfileString(szOPTIONINFO, szCURSORLIB, outbuff, runlist))
#define AddSourceToRunList(outbuff, runlist)														\
	(GtrWritePrivateProfileString(szSQLDRIVERS, outbuff, TEXT("src"), runlist))


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE 	hThisInst;
extern TCHAR 		g_szGatorIni[MAXFILELEN];

// For .INI macros
dCSEG(TCHAR) szRUNLISTEXT[]				= TEXT("*.rls");
dCSEG(TCHAR) szRLSEXT[]						= TEXT("rls");
dCSEG(TCHAR) szOPTIONINFO[] 				= TEXT("Options");
dCSEG(TCHAR) szDEBUG[] 						= TEXT("Debug");
dCSEG(TCHAR) szSCREEN[] 					= TEXT("Screen");
dCSEG(TCHAR) szLOGFILE[] 					= TEXT("Log File");
dCSEG(TCHAR) szLOGFILENAME[] 				= TEXT("Log File Name");
dCSEG(TCHAR) szISOLATE[] 					= TEXT("Isolate");
dCSEG(TCHAR) szCURSORLIB[] 				= TEXT("Cursor Library");
dCSEG(TCHAR) szALL[]							= TEXT("All");

TCHAR szDbgBuff[XLARGEBUFF];

//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN RunGroup(lpRUNTESTLIST rtl, LPTSTR src, lpUDLINFOLEVEL lpgrp,
		UINT pdex, UINT * lpcGrandTotal);
VOID INTFUN RunAuto(lpRUNTESTLIST rtl, LPTSTR src,
		lpUDLINFOLEVEL lpato, UINT pdex, UINT * lpcAutoErrors);
void EXTFUN SelectTests(HWND hwnd, HINSTANCE hInst, RUNTESTLIST * rtl);
LRESULT EXTFUN SelectTestsWndProc(HWND	 hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void INTFUN CreateTestList(lpUDLINFOBOX * lplpbox, HWND hDlg, HINSTANCE hInst,
			int x, int y, int dx, int dy, lpRUNTESTLIST rtl);
BOOL INTFUN GetTestGroups(lpUDLINFOBOX lpbox, lpRUNTESTLIST rtl,HWND hwnd,HWND);
UDLINFOLEVEL * EXTFUNCDECL GetAutoTests(lpUDLINFOLEVEL lpgrp, UINT pdex, LPTSTR szGroup,
						lpRUNTESTLIST rtl,HWND);
UDLINFOLEVEL * EXTFUNCDECL GetTestCases(lpUDLINFOLEVEL lpato, UINT pdex, LPTSTR szAutoTest,
						lpRUNTESTLIST rtl);
VOID INTFUN ResetForRunList(BOOL fSet, HWND hDlg, LPTSTR szRunList, lpRUNTESTLIST rtl,
									 HWND);
BOOL INTFUN GetSetLastRunInfo(BOOL fSet, lpUDLINFOLEVEL lptst, LPTSTR szRunList,
			LPTSTR szGroup, LPTSTR szAuto);
VOID INTFUN ResetForDefault(lpRUNTESTLIST rtl, HWND hDlg);
VOID INTFUN SetDftOptions(lpRUNTESTLIST rtl, HWND hDlg);
VOID INTFUN HandleRunLists(lpRUNTESTLIST rtl);
BOOL EXTFUN RunListsWndProc(HWND	 hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL INTFUN GetNewRunList(HWND hDlg, LPTSTR szList, LPTSTR szFullPath);
UINT EXTFUN NewRunListHook(HWND	hDlg, UINT	msg, WPARAM	wParam, LPARAM	lParam);
BOOL INTFUN DftRunListChangedByTab(lpRUNTESTLIST rtl, LPTSTR szList);
VOID INTFUN GtrGetLastError(HWND hwnd,LPTSTR lpszMsg);


__inline VOID INTFUN DbgMsg(HANDLE hConsole,LPTSTR szMsg)
{
	DWORD	dwChars=0;

	WriteConsole(hConsole,
					szMsg,
					lstrlen(szMsg),
					&dwChars,
					NULL);

} //DbgMsg()


//*------------------------------------------------------------------------
//|  RunAutoTest:
//|		Brutally abridged code at this point
//|	Parms:
//|		hwnd			Parent window handle
//|		hInst			Hinstance for dialog
//|		henv			Pass a valid environment handle or NULL
//|		hdbc			Pass default connection handle or NULL
//|		hstmt			The statement handle
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
void EXTFUN RunAutoTest(HWND hwnd, HINSTANCE hInst,
				HENV henv, HDBC hdbc, HSTMT hstmt,lpRUNTESTLIST lpRtl,
				HANDLE hConsole)
{
	RUNTESTLIST				rtl;
	lpRUNTESTLIST			lpRunList=&rtl;
	SERVERINFO				rs;
	lpSERVERINFO			lpRS=NULL;
	UINT						dex;
	LPTSTR					str;
	UINT						cGrandTotal=0;
	lpUDLINFOLEVEL			lpgrp;
	static TCHAR			szOutBuff[100];
	UINT						cbMaxTests=10;
	UINT						cbTotalErrors=0;
	static TCHAR			szLogFile[_MAX_PATH];

#ifdef _DEBUG
#define szDEBUGFILE			"C:\\TMP\\MEM"
		DeleteFile((LPTSTR)szDEBUGFILE);
#endif

	//Set gator32.ini path if odbctest hasn't set it
	SetGatorIniPath(g_szGatorIni);

	if (lpRtl)
	{
		lpRunList=lpRtl;
		lpRunList->fRun=TRUE;
	}


	if (!lpRtl)
	{
		// Init structure for display then do it
		memset(lpRunList, 0, sizeof(RUNTESTLIST));
		memset(&rs, 0, sizeof(SERVERINFO));

		lstrcpy(rs.szLogFile,szLogFile);

		lpRunList->hInst = hInst;
		rs.hwnd = hwnd;
		lpRunList->rs = &rs;
		lpRunList->fAppend = TRUE;
		SelectTests(hwnd, hInst,lpRunList);
	}

	//save log file for a rainy day
	lstrcpy(szLogFile,lpRunList->rs->szLogFile);

	if(!lpRunList->fRun)
	{
		if(lpRunList->szSource)
			ReleaseMemory(lpRunList->szSource);

		goto exit01;
	}

	if(!lpRunList->szSource || !(*lpRunList->szSource))
		goto exit01;


	// At this point we have the data sources to run against and a list
	// of groups, auto tests, and testcases to use for running.
	// Let's do it.
	Busy(TRUE);
	lpgrp = lpRunList->lpbox->lplvl;
	str   = lpRunList->szSource;
	lpRS  = lpRunList->rs;

	// Loop through each source
	while(*str)
	{
		// First get this data source's information
		lstrcpy(lpRS->szSource, str);
		GetServer0(lpRS->szSource, lpRS->szValidServer0, sizeof(lpRS->szValidServer0));
		GetLogin0(lpRS->szSource, lpRS->szValidLogin0, sizeof(lpRS->szValidLogin0));
		GetPassword0(lpRS->szSource, lpRS->szValidPassword0, sizeof(lpRS->szValidPassword0));
		GetKeywords(lpRS->szSource, lpRS->szKeywords, sizeof(lpRS->szKeywords));
		lpRS->hwnd = hwnd;
		lpRS->henv = henv;
		lpRS->hdbc = hdbc;
		lpRS->hstmt = hstmt;
		lpRS->cErrors = 0;

      // Now run each group which has something chosen
      cGrandTotal = 0;
		for(dex=0;  dex<lpgrp->cItems;  dex++)
		{
			if(BitGet(lpgrp->aSelected, dex) ||	BitGet(lpgrp->aPartial, dex))
				RunGroup(lpRunList, str, lpgrp, dex, &cGrandTotal);
		}

		// Move on to the next list
		str += lstrlen(str) + 1;
	}

	ReleaseMemory(lpRunList->szSource);

exit01:
	UDLFreeBox(lpRunList->lpbox);
	DeleteAutoTestList(lpRunList->head);
	Busy(FALSE);
}


//*------------------------------------------------------------------------
//|  RunGroup:
//|		Executes a group by running each auto test in the group against
//|		the given source.
//|	Parms:
//|		rtl			Run test list information
//|		src			Data source to run against
//|		lpgrp			Group level information
//|		pdex			Which group to run against
//|		lpcGTotal	Pointer to grand total indicator for groups
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID INTFUN RunGroup(lpRUNTESTLIST rtl, LPTSTR src, lpUDLINFOLEVEL lpgrp, UINT pdex, UINT * lpcGrandTotal)
{
	lpSERVERINFO			rs=rtl->rs;
	lpUDLINFOLEVEL			lpato;
	LPTSTR						szOutBuff;
	UINT						ato;
	UINT						cAutoErrors=0;

	szOutBuff = (LPTSTR)AllocateMemory(MAXOUTBUFF);
	if(!szOutBuff)
		return;

	// Print out the header for this group
	szLogPrintf(rs, TRUE,
					GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsNowExecutingGroup),
					lpgrp->szItems[pdex].str,
					src);

	// Print out the keywords for this group
	szLogPrintf(rs, TRUE,
					GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsKeywords),
					rs->szKeywords);

	lpato = lpgrp->next[pdex];
	for(ato=0;  ato<lpato->cItems;  ato++) {
		cAutoErrors = 0;
		if(BitGet(lpato->aSelected, ato) ||
			BitGet(lpato->aPartial, ato))
			RunAuto(rtl, src, lpato, ato, &cAutoErrors);
		*lpcGrandTotal += cAutoErrors;
		}

	szLogPrintf(rs, TRUE,
					GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsGroupGrandTotal),
					*lpcGrandTotal,
					lpgrp->szItems[pdex].str);
	ReleaseMemory(szOutBuff);
}


//*------------------------------------------------------------------------
//|  RunAuto:
//|		Given an auto test, run each test case chosen within it.
//|	Parms:
//|		rtl					Run test list information
//|		src					Data source to run against
//|		lpato					Autotest to run
//|		pdex					Which auto test to run against
//|		lpcAutoErrors		Pointer to grand total indicator for groups
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID INTFUN RunAuto(lpRUNTESTLIST rtl, LPTSTR src,
		lpUDLINFOLEVEL lpato, UINT pdex, UINT * lpcAutoErrors)
{
	lpSERVERINFO			rs=rtl->rs;
	lpUDLINFOLEVEL			lptst;
	lpAUTOTESTINFO			ati;
	LPTSTR					szOutBuff;
	UINT						dex;
	int						rtn;
	LPTSTR					szAutoTest=lpato->szItems[pdex].str;
	int						iState;

#ifndef _LOGTIME_OFF
	TIMEINFO					strtime, endtime, elapsed;
	TCHAR						szTime[MAXOUTBUFF];
#endif

	//Set gator32.ini path if odbctest hasn't set it
	SetGatorIniPath(g_szGatorIni);

   if(!(ati = FindAutoTestNode(rtl->head, szAutoTest))) {
   	if(!(ati = AddAutoTest(&rtl->head, szAutoTest, NULL, FALSE, &rtn)))
   		return;
   	}
	else if(!LoadAutoTestDLL(ati, ati->szTestName))
		return;

	// Verify the count of test cases registered matches DLL
	if(!VerifyTestCaseCount(szAutoTest, &(ati->cbTestCases), TRUE))
		return;

	// Allocate memory for output and set up jump table
	szOutBuff = (LPTSTR)AllocateMemory(MAXOUTBUFF);
	if(!szOutBuff)
		return;

	// Set the jump address. If an inhouse function fails and aborts, we will
	// return to this point of execution with a non zero return status.
	// The process will terminate and exit gracefully.
	if((iState=setjmp(rs->env)) != 0) 	// If 0, its just the original setting
	{
		rs->cErrors = -1;
		goto aborted;
	}

	szLogPrintf(rs, TRUE,
					GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsNowExecuting),
					szAutoTest);
	szLogPrintf(rs, TRUE,
					GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsAutoTestSource),
					src);

#ifndef _LOGTIME_OFF
	GetTime(&strtime);

	szLogPrintf(rs, TRUE,
					GetString(szOutBuff, MAXOUTBUFF, idsTimeStarted),
					szGetTime(szTime,strtime));
#endif // _LOGTIME_OFF

   // This function has been called to execute exactly one auto test.
   // Go through each test in this auto test.
	rs->cErrors = 0;
	rs->hLoadedInst = ati->hInst;
	lptst = lpato->next[pdex];

	// Allocate memory for the rglMaskEntire field which will contain the
	//		entire bitmask of tests to run.
	rs->rglMaskEntire = (UINT *)AllocateMemory(MINREQUIREDINTS(lptst->cItems) * sizeof(UINT *));
	if(!rs->rglMaskEntire)
		goto aborted;

	memcpy(rs->rglMaskEntire, lptst->aSelected,
			(size_t)(MINREQUIREDINTS(lptst->cItems) * sizeof(UINT)));

	if(!rs->fIsolate)
	{
		// The non-isolated case means we will only call AutoTestFunc once
		// with the entire bitmask.  This is quite easy for us to do.
		rs->rglMask = lptst->aSelected;
		(*ati->lpTestFunc)(rs);
	}
	else
	{
		// The non-isolated case means that we can only set one bit at a
		// time and must therefore go through each one
		rs->rglMask = (UINT *)AllocateMemory(MINREQUIREDINTS(lptst->cItems) * sizeof(UINT *));
		if(!rs->rglMask)
			goto aborted;

		for(dex=0;  dex<lptst->cItems;  dex++)
		{
			if(BitGet(lptst->aSelected, dex))
			{
				if(FAbortCheck())
				{
					szLogPrintf(rs, TRUE,
								GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsAborted));
					goto aborted;
				}
				else
				{
					// Read to run each test.  Run them in isolated mode which means that
					// we set each bit and call for one at a time.
					BitSet(rs->rglMask, dex);
					(*ati->lpTestFunc)(rs);
					BitClear(rs->rglMask, dex);
				}
			}
		}

		ReleaseMemory(rs->rglMask);

	} // End of test case selected

	UnLoadAutoTestDLL(ati);

#ifndef _LOGTIME_OFF
	GetTime(&endtime);
	szLogPrintf(rs, TRUE,
					GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsTimeFinished),
					szGetTime((LPTSTR)szTime,endtime));
#endif // _LOGTIME_OFF

	switch(rs->cErrors) {
		case -1:
			szLogPrintf(rs, TRUE,
				GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsAborted));
			break;

		case 0:
			szLogPrintf(rs, TRUE,
				GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsTestCompleted),
				ati->szTestName);
			szLogPrintf(rs, TRUE,
				GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsNoFailures),
				ati->szTestName);
			break;

		default:
			szLogPrintf(rs, TRUE,
				GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsTestCompleted),
				ati->szTestName);
			szLogPrintf(rs, TRUE,
				GetString((LPTSTR)szOutBuff, MAXOUTBUFF, (rs->cErrors == 1) ? idsTestFailure : idsTestFailures),
				rs->cErrors);
			*lpcAutoErrors += rs->cErrors;
			break;
		}

#ifndef _LOGTIME_OFF
	CElapsed(&strtime, &endtime, &elapsed);
	szLogPrintf(rs, TRUE,
				GetString((LPTSTR)szOutBuff, MAXOUTBUFF, idsTimeTotal),
				szGetTime(szTime,elapsed));
#endif // _LOGTIME_OFF

aborted:
	ReleaseMemory(szOutBuff);

	if(rs->rglMaskEntire)
		ReleaseMemory(rs->rglMaskEntire);

}


//*------------------------------------------------------------------------
//|  SelectTests:
//|		This function will envoke our test selection dialog to display
//|		to the user a list of valid tests and groups to run.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL*
//|							*The hInstance of this loaded library must be used
//|							to load the dialog.
//|		rtl			The structure to return selection info in
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
void EXTFUN SelectTests(HWND hwnd, HINSTANCE hInst, RUNTESTLIST * rtl)
{
#ifndef WIN32
	DLGPROC 				dlgproc;
	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)SelectTestsWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_RUN_TESTS), hwnd, dlgproc,
			(LPARAM)(RUNTESTLIST *)rtl))
	{
		szMessageBox(hwnd,
						MB_OK,
						szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_RUN_TESTS");
	}

	FreeProcInstance((FARPROC) dlgproc);

#else

	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_RUN_TESTS), hwnd, SelectTestsWndProc,
			(LPARAM)(RUNTESTLIST *)rtl))
	{

		szMessageBox(hwnd,
						MB_OK,
						szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						TEXT("IDD_RUN_TESTS"));
	}
#endif

}




//*------------------------------------------------------------------------
//|  SelectTestsWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
LRESULT EXTFUN SelectTestsWndProc(HWND	 hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RUNTESTLIST		*rtl;

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			{
			TCHAR		szTemp[_MAX_PATH]=TEXT("");

			rtl = (RUNTESTLIST *)lParam;
			SETWINDOWLONG(hDlg, rtl);

			rtl->fRun = FALSE;
			rtl->head = NULL;
			rtl->hRunDlg = hDlg;
			CenterDialog(hDlg);

			if (rtl && rtl->rs)
			{
				if (*rtl->rs->szLogFile)
				{
					lstrcpy(szTemp, rtl->rs->szLogFile);
					DoSetFile(GetDlgItem(hDlg, IDT_LOG), (LPCTSTR)szTemp);
				}
				else
					GetText(GetDlgItem(hDlg, IDT_LOG), rtl->rs->szLogFile);
			}

			CheckDlgButton(hDlg, IDX_SCREEN, TRUE);
			CheckDlgButton(hDlg, IDX_CURSOR_LIB, GRAYED_BUTTON);
			SendMessage(hDlg, USER_RESET_TEST_SOURCES, 0, 0L);

			CreateTestList(&rtl->lpbox, hDlg, hThisInst, 7, 18, 177, 70, rtl);
			SendMessage(hDlg, USER_RESET_RUNLISTS, 0, 0L);
			SendMessage(hDlg, USER_SET_BUTTON_STATUS, 0, 0L);

         SetFocus(rtl->lpbox->hwndBox);
         }
			return FALSE;

		//
		// Set status of Run button based on selected items.  The
		//	UDL_SELECTION_CHANGED messages is sent by the custom control
		//	the other is sent internally to this dlgproc
		//
		case UDL_SELECTION_CHANGED:
		case USER_SET_BUTTON_STATUS:
			{
			int		iSources=0;
			int		iTests=0;
			BOOL		fEnabled=TRUE;

			rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);

			iSources = (int)SendDlgItemMessage(hDlg, IDL_SOURCES,
									LB_GETSELCOUNT, 0, 0L);
			if(iSources == CB_ERR)
				iSources = 0;

			iTests = UDLHowManySelections(rtl->lpbox->lplvl) +
							UDLHowManyPartials(rtl->lpbox->lplvl);

			EnableWindow(GetDlgItem(hDlg, IDOK),
									(iTests && iSources) ? TRUE : FALSE);
			}
			return FALSE;

		// User message to reset the data source items
		case USER_RESET_TEST_SOURCES:
			{
			LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDL_SOURCES);

			rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);

			// First the list we have in the .INI file
			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
			if(str) {
				SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
				SendMessage(hwnd, LB_ADDSTRING,
						0, (LPARAM)(LPTSTR)szODBCTESTHANDLE);
				if(GetTestSources(str, MAXTESTBUFF))
					while(*astr) {
						SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)astr);
						astr += lstrlen(astr) + 1;
						}
				}
			ReleaseMemory(str);
			}
			return TRUE;

		// User message will cause a list of all .rls files to be generated
		case USER_RESET_RUNLISTS:
			{
			TCHAR		szDft[25];
			TCHAR		szLastFile[MAXRUNLIST];
			DWORD		dex=0;
			HWND 		hwnd=GetDlgItem(hDlg, IDC_RUNLISTS);

			rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);
			SendMessage(hwnd, CB_RESETCONTENT, 0, 0L);
			SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)GetString(szDft, sizeof(szDft)/sizeof(szDft[0]), idsDefault));
			SendMessage(hwnd, CB_DIR, DDL_READWRITE, (LPARAM)(LPTSTR)szRUNLISTEXT);
			if(GetLastRunList(szLastFile, sizeof(szLastFile)/sizeof(szLastFile[0])) &&
				FindFileDir((LPTSTR)szLastFile, rtl->szRunList)) {
				ResetForRunList(FALSE, hDlg, rtl->szRunList, rtl,NULL);
				SendMessage(hwnd, CB_SETCURSEL,
						(WPARAM)SendMessage(hwnd, CB_FINDSTRING, 0, (LPARAM)(LPTSTR)szLastFile), 0L);
				}
			else {
				lstrcpy(szLastFile, szDft);														//		or use <default>
				SendMessage(hwnd, CB_SETCURSEL, 0, 0L);
				}
			SendMessage(hDlg, USER_SET_BUTTON_STATUS, 0, 0L);
			}
			return TRUE;


		case USER_GET_OPTIONS:
			{
			UINT	fChecked;

			rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);

			rtl->rs->fDebug = IsDlgButtonChecked(hDlg, IDX_DEBUG);
			rtl->rs->fScreen = IsDlgButtonChecked(hDlg, IDX_SCREEN);
			rtl->rs->fLog = IsDlgButtonChecked(hDlg, IDX_LOG);
//.			GetText(GetDlgItem(hDlg, IDT_LOG), rtl->rs->szLogFile);
			rtl->rs->fIsolate = IsDlgButtonChecked(hDlg, IDX_ISOLATE);
			fChecked = IsDlgButtonChecked(hDlg, IDX_CURSOR_LIB);
			if(!fChecked)
				rtl->rs->vCursorLib = SQL_CUR_USE_DRIVER;
			else if(fChecked == GRAYED_BUTTON)
				rtl->rs->vCursorLib = SQL_CUR_USE_IF_NEEDED;
			else
				rtl->rs->vCursorLib = SQL_CUR_USE_ODBC;
			}
			return TRUE;

		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam, lParam)) {
				case IDX_LOG:
					{
					int	tmp = IsDlgButtonChecked(hDlg, IDX_LOG);
					EnableWindow(GetDlgItem(hDlg, IDB_LOG), tmp);
					EnableWindow(GetDlgItem(hDlg, IDT_LOG), tmp);
					}
					return TRUE;

				case IDB_LOG:
					{
					TCHAR				szTemp[XLARGEBUFF]=TEXT("");

					rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);

					DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_OPEN), hDlg, OpenDlgProc,
							(LPARAM)(RUNTESTLIST *)rtl);

					lstrcpy(szTemp,rtl->rs->szLogFile);
					DoSetFile(GetDlgItem(hDlg, IDT_LOG), (LPCTSTR)szTemp);

					}
					return TRUE;

				case IDB_RUNLISTS:
					{

					rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);
					SendMessage(hDlg, USER_GET_OPTIONS, 0, 0L);
					HandleRunLists(rtl);
					}
					return TRUE;

				// Grab the values and let them run
				case IDOK:
					{
					TCHAR		szList[MAXRUNLIST];
					WPARAM	dex;
					TCHAR		szMode[SMALLBUFF];
					FILE		*fp=NULL;

					rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);
					SendMessage(hDlg, USER_GET_OPTIONS, 0, 0L);

					if (rtl->fAppend)
						lstrcpy(szMode,TEXT("a"));
					else
						lstrcpy(szMode,TEXT("w"));

					//Is is a valid file name?
					fp=_tfopen(rtl->rs->szLogFile,szMode);

					if (fp)
						fclose(fp);

					if(rtl->rs->fLog && !rtl->fAppend)
						DeleteFile(rtl->rs->szLogFile);
					rtl->szSource = BuildSelList(GetDlgItem(hDlg, IDL_SOURCES));
					rtl->fRun = TRUE;
					dex = (WPARAM)SendDlgItemMessage(hDlg, IDC_RUNLISTS, CB_GETCURSEL, 0, 0L);
					if(dex > 0) {
						SendDlgItemMessage(hDlg, IDC_RUNLISTS, CB_GETLBTEXT, dex, (LPARAM)(LPTSTR)szList);
						SetLastRunList(szList);
						}
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					}
					return TRUE;

            case IDCANCEL:
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;
				}

			switch(GET_WM_COMMAND_CODE(wParam, lParam)) {
				// If user changes their run list selection ,reset controls
				case CBN_SELCHANGE:
					if(GET_WM_COMMAND_ID(wParam, lPara) == IDC_RUNLISTS)
						SendMessage(hDlg, USER_RESET_DFT_RUNLIST, 0, 0L);
					SendMessage(hDlg, USER_SET_BUTTON_STATUS, 0, 0L);
					break;
				}
			return FALSE;			// End WM_COMMAND

		// Change controls based on the new default run list
		case USER_RESET_DFT_RUNLIST:
			{
			TCHAR		szList[MAXRUNLIST];
			HWND		hwnd = GetDlgItem(hDlg, IDC_RUNLISTS);

			rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);
			if(SendMessage(hwnd, CB_GETCURSEL, 0, 0L) > 0) {	// not <default>
				GetCBText(hwnd, szList);
				FindFileDir((LPTSTR)szList, rtl->szRunList);
				ResetForRunList(FALSE, hDlg, rtl->szRunList, rtl,NULL);
				}
			else
				ResetForDefault(rtl, hDlg);
         }
         return TRUE;

		// Must unregister 3D stuff and end dialog
		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;

		default:
			return FALSE;
		}
}




//*------------------------------------------------------------------------
//|  CreateTestList:
//|		Create our output box and build the outline list of
//|		groups, auto tests, and test cases.
//|	Parms:
//|		lplpbox				Return pointer for created box
//|		hDlg					Parent window
//|		hInst					Instance handle
//|		x						Top left
//|		y						Top
//|		dx						Width
//|		dy						Height
//|		rtl					Run-time list info
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN CreateTestList(lpUDLINFOBOX * lplpbox, HWND hDlg, HINSTANCE hInst,
			int x, int y, int dx, int dy, lpRUNTESTLIST rtl)
{
	lpUDLINFOBOX	lpbox;

	UDLInit(hInst);
	lpbox = UDLCreateBox(3, hDlg, hInst, x, y, dx, dy);

	UDLCreateLevelDesc(lpbox, 0, hInst, (LPTSTR)MAKEINTRESOURCE(IDBT_GROUP), DFTBITMAPWIDTH, DFTBITMAPHEIGHT);
	UDLCreateLevelDesc(lpbox, 1, hInst, (LPTSTR)MAKEINTRESOURCE(IDBT_AUTOT), DFTBITMAPWIDTH, DFTBITMAPHEIGHT);
	UDLCreateLevelDesc(lpbox, 2, hInst, (LPTSTR)MAKEINTRESOURCE(IDBT_TESTC), DFTBITMAPWIDTH, DFTBITMAPHEIGHT);

	GetTestGroups(lpbox, rtl,hDlg,NULL);

	*lplpbox = lpbox;
	UDLAdjustScrollBar(lpbox);
}


//*------------------------------------------------------------------------
//|  GetTestGroups:
//|		Retrieve all groups from our .ini file and continue building
//|		the outline.
//|	Parms:
//|		lpbox					The box we have created
//|		rtl					For test chain and DLL's
//|	Returns:
//|		TRUE if successful, FALSE on failure
//*------------------------------------------------------------------------
BOOL INTFUN GetTestGroups(lpUDLINFOBOX lpbox, lpRUNTESTLIST rtl,HWND hwnd,HWND hConsole)
{
	LPTSTR				szIniStr;
	LPTSTR				str;
	UINT				cItems;
	UINT				dex,cdex;
	int				len;
	lpUDLINFOLEVEL	lplvl;

	if (hConsole)
		DbgMsg(hConsole,TEXT("GetTestGroups()\r\n"));

	//Set gator32.ini path if odbctest hasn't set it
	SetGatorIniPath(g_szGatorIni);

	if (hConsole)
	{
		DbgMsg(hConsole,g_szGatorIni);
		DbgMsg(hConsole,TEXT("\n\r"));
	}

	str = szIniStr = (LPTSTR)AllocateMemory(MAXTESTBUFF);
	if(!str)
		return FALSE;
	lstrcpy(str, szALL);
	len = lstrlen(str)+1;
	str += len;
	GetGroupList(str, (MAXTESTBUFF - len));

	if (hConsole)
	{
		DbgMsg(hConsole,TEXT("Group list: "));
		DbgMsg(hConsole,str);
		DbgMsg(hConsole,TEXT("\n\r"));
	}

	str = szIniStr;
	cItems = CountNullArray(szIniStr);

	lplvl = lpbox->lplvl = UDLCreateLevel(NULL, 0, GROUP_LEVEL, cItems, FALSE);
	for(dex=0;  dex<MINREQUIREDINTS(lplvl->cItems);  dex++)
		lplvl->aExpanded[dex] = (UINT)-1;

	lplvl->fLoaded = TRUE;
   cdex=0;

   for(dex=0;  dex<cItems;  dex++)
	{
		// Create a level descriptor for the auto tests below this group
   	lplvl->next[cdex] = GetAutoTests(lplvl, cdex, str, rtl,hConsole);

		if(lplvl->next[cdex])
		{
   	  	// Set the display text for group (dex)
			UDLSetLevelText(lplvl, cdex, str);
			++cdex;
		}

		str += lstrlen(str) + 1;
	}

	// Check for empty groups and free their memory
	if(cdex != cItems)
		lplvl->cItems = cdex;

	// Finish initializing items
	lpbox->top.fOpt = UDL_GROUP_ON_TOP;
	lpbox->top.group = lpbox->top.autot = lpbox->top.testc = 0;
	lpbox->cTotVisible = UDLVisibleItems(lpbox);
	lpbox->cCurRelative = 0;

	ReleaseMemory(szIniStr);
	return TRUE;
}


//*------------------------------------------------------------------------
//|  GetAutoTests:
//|		Given a group, enumerate all tests for the group and all
//|		of it's test cases.
//|	Parms:
//|		lpgrp					The group which owns us
//|		pdex					The parent index of the group
//|		szGroup				The group we're interested in
//|		rtl					The run-time list
//|	Returns:
//|		Pointer to the new level, NULL on error
//*------------------------------------------------------------------------
UDLINFOLEVEL * EXTFUNCDECL GetAutoTests(lpUDLINFOLEVEL lpgrp, UINT pdex, LPTSTR szGroup,
						lpRUNTESTLIST rtl,HWND hConsole)
{
	lpUDLINFOLEVEL		lplvl;
	LPTSTR					szIniStr;
	LPTSTR					str;
	UINT					cItems;
	UINT					dex;


	if (hConsole)
		DbgMsg(hConsole,TEXT("GetAutoTests()\r\n"));

	str = szIniStr = (LPTSTR)AllocateMemory(MAXTESTBUFF);
	if(!str)
		goto exit01;

	if(_tcscmp(szGroup, (LPTSTR)szALL))
	{
		if(!GetGroupAutoTests(szGroup, szIniStr, MAXTESTBUFF))
			goto exit01;

	}
	else
	{
		if(!GetInstalledTestList(str, MAXTESTBUFF))
			goto exit01;

	}

	cItems = CountNullArray(szIniStr);

	lplvl = UDLCreateLevel(lpgrp, pdex, AUTOT_LEVEL, cItems, FALSE);
	lplvl->fLoaded = TRUE;

	if (hConsole)
		DbgMsg(hConsole,TEXT("Installed Autotests: \r\n"));

   for(dex=0;  dex<cItems;  dex++)
	{
		if (hConsole)
		{
			DbgMsg(hConsole,str);
			DbgMsg(hConsole,TEXT("\r\n"));
		}

		// Create a level descriptor for the auto tests below this group
   	lplvl->next[dex] = GetTestCases(lplvl, dex, str, rtl);

   	// Set the display text for group (dex)
		UDLSetLevelText(lplvl, dex, str);
		str += (lstrlen(str) + 1);// * sizeof(TCHAR);
	}

	ReleaseMemory(szIniStr);

	return lplvl;

exit01:								// Error
	ReleaseMemory(szIniStr);
	return NULL;
}



//*------------------------------------------------------------------------
//|  GetTestCases:
//|		Given an auto test name, retrieve all of it's descriptions
//|		using the AutoTestDesc function.
//|	Parms:
//|		lpato					The auto test which owns us
//|		pdex					The parent index of the auto test
//|		szAutoTest			The auto test we're interested in
//|		rtl					The run-time list
//|	Returns:
//|		Pointer to the new level, NULL on error
//*------------------------------------------------------------------------
UDLINFOLEVEL * EXTFUNCDECL GetTestCases(lpUDLINFOLEVEL lpato, UINT pdex, LPTSTR szAutoTest,
						lpRUNTESTLIST rtl)
{
	lpUDLINFOLEVEL		lplvl;
	UINT					cTestCases;
//	lpAUTOTESTINFO		ati;
//	int					rtn;

//	if(!(ati = AddAutoTest(&rtl->head, szAutoTest, NULL, TRUE, &rtn)))
//		return NULL;
	if(!(cTestCases = GetTestCaseCount(szAutoTest)))
		return NULL;

	lplvl = UDLCreateLevel(lpato, pdex, TESTC_LEVEL, cTestCases, TRUE);
	lplvl->fLoaded = FALSE;

	return lplvl;
}



//*------------------------------------------------------------------------
//|  ResetForRunList:
//|		Given a run list, set the defaults according to the description
//|		contained in szRunList.  Don't call this function for <default>
//|	Parms:
//|		fSet					TRUE if we are setting defaults, else getting
//|		hDlg					Window handle of the dialog to change
//|		szRunList			File which contains our values
//|		lprtl					The run list information
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN ResetForRunList(BOOL fSet, HWND hDlg, LPTSTR szRunList, lpRUNTESTLIST rtl,HWND hConsole)
{
	UINT					gdex, adex;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;
	lpSERVERINFO		rs=rtl->rs;
	LPTSTR					szStr, aStr;
	RECT					rect;

	if (hConsole)
		DbgMsg(hConsole,TEXT("ResetForRunList()\r\n"));

	Busy(TRUE);
	if(fSet)
		DeleteFile(szRunList);

	// Go through each group and retrieve defaults for within
	lpgrp = rtl->lpbox->lplvl;
	for(gdex=0;  gdex<lpgrp->cItems;  gdex++)
	{
		lpato = lpgrp->next[gdex];

		for(adex=0;  adex<lpato->cItems;  adex++)
		{
			lptst = lpato->next[adex];
			GetSetLastRunInfo(fSet, lptst, szRunList,
						lpgrp->szItems[gdex].str, lpato->szItems[adex].str);
			if(!fSet)
				UDLAdjustPartial(lpato, adex);
		}
	}

	if (rtl->lpbox->hwndBox)
	{
		GetClientRect(rtl->lpbox->hwndBox, &rect);
		InvalidateRect(rtl->lpbox->hwndBox, &rect, TRUE);
	}

	// If we are setting values, simply write everything out to the file
	if(fSet)
	{
		TCHAR		tmp[45];
		WPARAM	dex, cdex, maxdex;

		// Write out the group information
		SetLastRunDebug(_itot(rs->fDebug, tmp, 10), szRunList);
		SetLastRunScreen(_itot(rs->fScreen, tmp, 10), szRunList);
		SetLastLogFile(_itot(rs->fLog, tmp, 10), szRunList);
		if(rs->fLog)
			SetLastLogFileName(rs->szLogFile, szRunList);
		SetLastIsolate(_itot(rs->fIsolate, tmp, 10), szRunList);
		SetLastCursorLib(_itot((int)rs->vCursorLib, tmp, 10), szRunList);

		// Now get sources.  Note we should probably allocate an array of ints
		//		and get each one instead of checking each, but this isn't crucial code
		maxdex = (WPARAM)SendDlgItemMessage(hDlg, IDL_SOURCES, LB_GETCOUNT, 0, 0L);
		for(cdex=0;  cdex<maxdex;  cdex++)
		{
			dex = (WPARAM)SendDlgItemMessage(hDlg, IDL_SOURCES, LB_GETSEL, cdex, 0L);

			if(dex != LB_ERR && dex > 0)
			{
				SendDlgItemMessage(hDlg, IDL_SOURCES, LB_GETTEXT, cdex, (LPARAM)(LPTSTR)tmp);
				AddSourceToRunList(tmp,szRunList);
			}
		}
	}
	// Else we are getting values and updating the display accordingly
	else
	{
		// Retrieve options from RunList
		rs = rtl->rs;
		rs->fDebug = GetLastRunDebug(szRunList);
		if (hConsole)
		{
			wsprintf(szDbgBuff,TEXT("rs->fDebug = %d\r\n"),rs->fDebug);
			DbgMsg(hConsole,szDbgBuff);
		}


	   rs->fScreen = GetLastRunScreen(szRunList);
		if (hConsole)
		{
			wsprintf(szDbgBuff,TEXT("rs->fScreen = %d\r\n"),rs->fScreen);
			DbgMsg(hConsole,szDbgBuff);
		}

		if(rs->fLog = GetLastLogFile(szRunList))
	   	GetLastLogFileName(rs->szLogFile, sizeof(rs->szLogFile), szRunList);
   	else
   		*rs->szLogFile = TEXT('\0');

		if (hConsole)
		{
			DbgMsg(hConsole,TEXT("rs->szLogFile = "));
			DbgMsg(hConsole,rs->szLogFile);
			DbgMsg(hConsole,TEXT("\r\n"));
		}

		//   	rs->szLog = (rs->fLog) ? rs->szLogFile : NULL;
	   rs->fIsolate = GetLastIsolate(szRunList);
		if (hConsole)
		{
			wsprintf(szDbgBuff,TEXT("rs->fIsolate = %d\r\n"),rs->fIsolate);
			DbgMsg(hConsole,szDbgBuff);
		}

		rs->vCursorLib = (UDWORD)GetLastCursorLib(szRunList);
		if (hDlg)
			SetDftOptions(rtl, hDlg);

/*   	// Set defaults
		CheckDlgButton(hDlg, IDX_LOG, rs->fLog);
		if(rs->fLog)
			SetWindowText(GetDlgItem(hDlg, IDT_LOG), rs->szLogFile);
		EnableWindow(GetDlgItem(hDlg, IDB_LOG), rs->fLog);
		EnableWindow(GetDlgItem(hDlg, IDT_LOG), rs->fLog);
		CheckDlgButton(hDlg, IDX_SCREEN, rs->fScreen);
		CheckDlgButton(hDlg, IDX_DEBUG, rs->fDebug);
		CheckDlgButton(hDlg, IDX_ISOLATE, rs->fIsolate);
		if(rs->vCursorLib == SQL_CUR_USE_DRIVER)
			fCheck = FALSE;
		else if(rs->vCursorLib == SQL_CUR_USE_IF_NEEDED)
			fCheck = GRAYED_BUTTON;
		else
			fCheck = TRUE;
		CheckDlgButton(hDlg, IDX_CURSOR_LIB, fCheck);
*/
		// Select sources
		aStr = szStr = (LPTSTR)AllocateMemory(MAXTESTBUFF);

		if(szStr)
		{
			DWORD		dex;
			if (hDlg)
				SendDlgItemMessage(hDlg, IDL_SOURCES, LB_SETSEL, FALSE, MAKELPARAM(-1,0));

			if (hConsole)
				DbgMsg(hConsole,TEXT("Sources = \r\n"));

			if(GetLastRunSources(szStr, MAXTESTBUFF, szRunList))
			{
				while(*aStr)
				{
					if (hConsole)
					{
						DbgMsg(hConsole,aStr);
						DbgMsg(hConsole,TEXT("\r\n"));
					}

					if (hDlg)
					{
						dex = (DWORD)SendDlgItemMessage(hDlg, IDL_SOURCES, LB_FINDSTRING, 0, (LPARAM)aStr);
						//fixed bug# 2943
						if (dex == LB_ERR)
							SendDlgItemMessage(hDlg,IDL_SOURCES, LB_ADDSTRING,0,(LPARAM)(LPCTSTR)aStr);
						if(dex != LB_ERR)
							SendDlgItemMessage(hDlg, IDL_SOURCES, LB_SETSEL, TRUE, MAKELPARAM(dex, 0));
					}

					aStr += lstrlen(aStr) + 1;
				}
			}
		}

		//if hDlg is null then we called this routine from Loader.exe
		if (!hDlg)
		{
			rtl->szSource = (LPTSTR)AllocateMemory(MAXTESTBUFF+2);
			memcpy(rtl->szSource,szStr,MAXTESTBUFF);
		}

		ReleaseMemory(szStr);

	}	// End of getting values

	Busy(FALSE);
}


//*------------------------------------------------------------------------
//|  GetSetLastRunInfo:
//|		Given a group, auto test, and run list file, set default selections
//|		for the group.  In the runlist is a section for each group which
//|		has an entry for each auto test.  Example:
//|				[group]
//|				auto1=2,2346,232
//|		Where the value contains first the number of UINTs that were last
//|		used to track the test (could change if user adds/deletes test
//|		cases), followed by one bitmask per array for defaults.
//|	Parms:
//|		fSet					TRUE if we should set values based on current,
//|										else means to get them
//|		lptst					The test structure to handle
//|		szRunList			Name of file which has run information
//|		szGroup				Name of group to load
//|		szAuto				Auto test within group
//|	Returns:
//|		TRUE if we found the group information, FALSE if we did not
//*------------------------------------------------------------------------
BOOL INTFUN GetSetLastRunInfo(BOOL fSet, lpUDLINFOLEVEL lptst, LPTSTR szRunList,
			LPTSTR szGroup, LPTSTR szAuto)
{
	UINT			dex;
	UINT			cItems;
	TCHAR			szStr[MAXOUTBUFF];
	LPTSTR		str=szStr;
	LPTSTR		lpFmt=TEXT("%u,");

	cItems = MINREQUIREDINTS(lptst->cItems);				// How many items in array
	memset(str, 0, MAXOUTBUFF);
	if(fSet) {
		wsprintf(str, lpFmt,cItems);
		str += lstrlen(str);
		for(dex=0;  dex<cItems;  dex++) {
			wsprintf(str, lpFmt,lptst->aSelected[dex]);
			str += lstrlen(str);
			}
		--str;
		*str = TEXT('\0');
		return(SetLastRun(szGroup, szAuto, szStr, szRunList));
		}
	else {
		if(GetLastRun(szGroup, szAuto, szStr, MAXOUTBUFF, szRunList)) {
			while(str = _tcschr(str, TEXT(',')))
				*str++ = TEXT('\0');
			str = szStr;
			cItems = min(cItems, (UINT)_ttoi(str));
			str += lstrlen(str) + 1;
			for(dex=0;  dex<cItems;  dex++) {
				lptst->aSelected[dex] = (UINT)_ttol(str);
				str += lstrlen(str) + 1;
				}
			return TRUE;											// Items were selected
			}
		else {
			for(dex=0;  dex<cItems; dex++)
				lptst->aSelected[dex] = 0;
			}
		}
	return FALSE;
}


//*------------------------------------------------------------------------
//|  ResetForDefault:
//|		If the user selects <Default>, then we need to give them the
//|		default values.
//|	Parms:
//|		rtl					The runlist structure to use
//|		hDlg					Dialog window handle being updated
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN ResetForDefault(lpRUNTESTLIST rtl, HWND hDlg)
{
	UINT					gdex, adex, tdex, maxdex;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;
	lpSERVERINFO		rs=rtl->rs;
	RECT					rect;

	// No selected groups or tests
	lpgrp = rtl->lpbox->lplvl;
	for(gdex=0;  gdex<lpgrp->cItems;  gdex++) {
		lpato = lpgrp->next[gdex];
		for(adex=0;  adex<lpato->cItems;  adex++) {
			lptst = lpato->next[adex];
			maxdex = MINREQUIREDINTS(lptst->cItems);
			for(tdex=0;  tdex<maxdex;  tdex++)
				lptst->aSelected[tdex] = 0;
			UDLAdjustPartial(lpato, adex);
			}
		}
	GetClientRect(rtl->lpbox->hwndBox, &rect);
	InvalidateRect(rtl->lpbox->hwndBox, &rect, TRUE);

	// No selected test sources
	SendDlgItemMessage(hDlg, IDL_SOURCES, LB_SETSEL, FALSE, MAKELPARAM(-1,0));

	// Set output options and run-time options
	rs = rtl->rs;
	rs->fDebug = FALSE;
	rs->fScreen = TRUE;
	rs->fLog = FALSE;
	rs->fIsolate = FALSE;
	rs->vCursorLib = SQL_CUR_USE_IF_NEEDED;
	SetDftOptions(rtl, hDlg);

	// Remove the last run entry forcing <default> for next time
	SetLastRunList(TEXT(""));
}




//*------------------------------------------------------------------------
//|  SetDftOptions:
//|		Given a runtestlist and a dialog handle, set the controls based
//|		on the data structre.
//|	Parms:
//|		rtl					The runlist structure to use
//|		hDlg					Dialog window handle being updated
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN SetDftOptions(lpRUNTESTLIST rtl, HWND hDlg)
{
	lpSERVERINFO		rs=rtl->rs;
	UINT					fCheck;

	CheckDlgButton(hDlg, IDX_LOG, rs->fLog);
	if(rs->fLog)
		SetWindowText(GetDlgItem(hDlg, IDT_LOG), rs->szLogFile);
	EnableWindow(GetDlgItem(hDlg, IDB_LOG), rs->fLog);
	EnableWindow(GetDlgItem(hDlg, IDT_LOG), rs->fLog);
	CheckDlgButton(hDlg, IDX_SCREEN, rs->fScreen);
	CheckDlgButton(hDlg, IDX_DEBUG, rs->fDebug);
	CheckDlgButton(hDlg, IDX_ISOLATE, rs->fIsolate);
	if(rs->vCursorLib == SQL_CUR_USE_DRIVER)
		fCheck = FALSE;
	else if(rs->vCursorLib == SQL_CUR_USE_IF_NEEDED)
		fCheck = GRAYED_BUTTON;
	else
		fCheck = TRUE;
	CheckDlgButton(hDlg, IDX_CURSOR_LIB, fCheck);
}


//*------------------------------------------------------------------------
//|  HandleRunLists:
//|		This function will display the Run Lists dialog box which a user
//|		can use to define Run Lists or remove old ones.  Note that this
//|		function will update the Run Auto Test dialog directly.
//|	Parms:
//|		rtl				The running list
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID INTFUN HandleRunLists(lpRUNTESTLIST rtl)
{
	DLGPROC 				dlgproc;

	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)RunListsWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_RUN_LISTS), rtl->hRunDlg, dlgproc,
			(LPARAM)(LPTSTR)rtl))
		szMessageBox(rtl->hRunDlg,
						MB_OK,
						(LPTSTR)szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_RUN_LISTS");
	FreeProcInstance((FARPROC) dlgproc);
}




//*------------------------------------------------------------------------
//|  RunListsWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN RunListsWndProc(HWND	 hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	lpRUNTESTLIST			rtl;

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			{
			TCHAR	szList[MAXRUNLIST];
			TCHAR	szDir[_MAX_PATH];
			WPARAM dex;

			rtl = (lpRUNTESTLIST)lParam;
			SETWINDOWLONG(hDlg, rtl);
			CenterDialog(hDlg);
			if((dex = (WPARAM)SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_GETCURSEL, 0, 0L)) > 0) {
				SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_GETLBTEXT, dex, (LPARAM)(LPTSTR)szList);
				SetWindowText(GetDlgItem(hDlg, IDT_CURRENT), (LPTSTR)szList);
				}

			// Find current directory and set value in dialog
			GetCurrentDirectory(sizeof(szDir)/sizeof(szDir[0]), szDir);
			DoSetFile(GetDlgItem(hDlg, IDT_FROM), (LPCTSTR)szDir);

			SendMessage(hDlg, USER_RESET_RUNLISTS, 0, 0L);
			}
			return TRUE;

		// User message resets button status based on our state
		case USER_RESET_BUTTON_STATUS:
			{
			TCHAR 		szList[MAXRUNLIST];
			TCHAR		szTmp[MAXRUNLIST];
			rtl = (lpRUNTESTLIST)GETWINDOWLONG(hDlg);
			GetText(GetDlgItem(hDlg, IDT_CURRENT), szList);
			GetLBText(GetDlgItem(hDlg, IDL_RUNLISTS), szTmp);
			EnableWindow(GetDlgItem(hDlg, IDB_SAVE),
				(SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_GETCURSEL, 0, 0L) > 0) &&
				(lstrcmpi(szList, szTmp) == 0));
			EnableWindow(GetDlgItem(hDlg, IDB_DELETERL),
				!(LB_ERR == SendDlgItemMessage(hDlg, IDL_RUNLISTS, LB_GETCURSEL, 0, 0L)));
			}
			return TRUE;

		// User message will cause a list of all .rls files to be generated
		case USER_RESET_RUNLISTS:
			{
			HWND 		hwnd=GetDlgItem(hDlg, IDL_RUNLISTS);
			TCHAR		szList[MAXRUNLIST];
			WPARAM	dex;
			rtl = (lpRUNTESTLIST)GETWINDOWLONG(hDlg);
			SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
			SendMessage(hwnd, LB_DIR, DDL_READWRITE, (LPARAM)(LPTSTR)szRUNLISTEXT);
			GetText(GetDlgItem(hDlg, IDT_CURRENT), szList);
			dex = (WPARAM)SendDlgItemMessage(hDlg, IDL_RUNLISTS, LB_FINDSTRING, 0, (LPARAM)(LPTSTR)szList);
			if(dex != LB_ERR)
				SendDlgItemMessage(hDlg, IDL_RUNLISTS, LB_SETCURSEL, dex, 0L);
			SendMessage(hDlg, USER_RESET_BUTTON_STATUS, 0, 0L);
			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam, lParam)) {
				// Save simply rewrites the current file
				case IDB_SAVE:
					{
					TCHAR		szList[MAXRUNLIST];
					rtl = (lpRUNTESTLIST)GETWINDOWLONG(hDlg);
					GetText(GetDlgItem(hDlg, IDT_CURRENT), szList);
					ResetForRunList(TRUE, rtl->hRunDlg, (LPTSTR)rtl->szRunList, rtl,NULL);
					}
					return TRUE;

				// Save As will save a copy with a new name
				case IDB_SAVEAS:
					{
					TCHAR szName[MAXRUNLIST];
					WPARAM 		dex;
					rtl = (lpRUNTESTLIST)GETWINDOWLONG(hDlg);
					if(GetNewRunList(hDlg, szName, (LPTSTR)rtl->szRunList)) {
						ResetForRunList(TRUE, rtl->hRunDlg, (LPTSTR)rtl->szRunList, rtl,NULL);
						dex = (WPARAM)SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_FINDSTRING, 1, (LPARAM)(LPTSTR)szName);
						if(dex == LB_ERR)
							dex = (WPARAM)SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szName);
						SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_SETCURSEL, dex, 0L);
						}
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					}
					return TRUE;

				// Delete the group by deleting the file
				case IDB_DELETERL:
					{
					TCHAR 	szList[MAXRUNLIST];
					TCHAR  szTmp[MAXRUNLIST];
					WPARAM	dex;
					rtl = (lpRUNTESTLIST)GETWINDOWLONG(hDlg);
					GetLBText(GetDlgItem(hDlg, IDL_RUNLISTS), szList);
					if(IDOK == szMessageBox(hDlg,
							MB_ICONQUESTION | MB_OKCANCEL,
							(LPTSTR)szTestTitle,
							GetString(szErrBuff, MAXOUTBUFF, idsDeleteRunList),
							(LPTSTR)szList)) {
						TCHAR		szDelRunList[MAXFILELEN];
						GetText(GetDlgItem(hDlg, IDT_CURRENT), szTmp);
						if(lstrcmpi(szList, szTmp) == 0) {
							SetWindowText(GetDlgItem(hDlg, IDT_CURRENT),
									GetString(szTmp,
                                              sizeof(szTmp)/sizeof(szTmp[0]), idsDefault));
							SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_SETCURSEL, 0, 0L);
							SendMessage(rtl->hRunDlg, USER_RESET_DFT_RUNLIST, 0, 0L);
							}
						if(FindFileDir(szList, szDelRunList)) {
							DeleteFile(szDelRunList);
							dex = (WPARAM)SendDlgItemMessage(hDlg, IDL_RUNLISTS, LB_FINDSTRING,
												0, (LPARAM)(LPTSTR)szList);
							SendDlgItemMessage(hDlg, IDL_RUNLISTS, LB_DELETESTRING, dex, 0L);
							SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_DELETESTRING,
										(WPARAM)SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_FINDSTRING, 0,
										(LPARAM)(LPTSTR)szList), 0L);
							GetCBText(GetDlgItem(rtl->hRunDlg, IDC_RUNLISTS), szList);
							FindFileDir(szList, rtl->szRunList);
							}
						else
							szMessageBox(hDlg,
									MB_ICONEXCLAMATION | MB_OK,
									(LPTSTR)szTestTitle,
									GetString(szErrBuff, MAXOUTBUFF, idsDeleteFileNotFound),
									(LPTSTR)szList);
						SendMessage(hDlg, USER_RESET_BUTTON_STATUS, 0, 0L);
						}
					}
					return TRUE;

				case IDB_FROM:
					{
					TCHAR		szDir[_MAX_PATH];
					TCHAR		szPrompt[MAXOUTBUFF];

					GetCurrentDirectory(sizeof(szDir)/sizeof(szDir[0]), szDir);
					GetString(szPrompt, sizeof(szPrompt)/sizeof(szPrompt[0]), idsRlsPrompt);
					if(BrowseDirs(hDlg, hThisInst, (LPCSTR)szPrompt,szDir))
						{
						DoSetFile(GetDlgItem(hDlg, IDT_FROM), (LPCTSTR)szDir);
						SendMessage(hDlg, USER_RESET_RUNLISTS, 0, 0L);
						}
					}
					return TRUE;


            case IDCANCEL:
				{
					// close button: fixed bug #2943
					TCHAR szName[MAXRUNLIST];
					WPARAM dex, cnt, ndex;


					rtl = (lpRUNTESTLIST)GETWINDOWLONG(hDlg);
					cnt = (WPARAM)SendDlgItemMessage(hDlg,IDL_RUNLISTS,LB_GETCOUNT,0,0L);
					if( cnt != LB_ERR)
					{
						for(ndex=0; ndex <cnt;ndex++)
						{
							SendDlgItemMessage(hDlg,IDL_RUNLISTS,LB_GETTEXT,ndex,(LPARAM)(LPCTSTR)szName);
							dex = (WPARAM)SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_FINDSTRING, 1, (LPARAM)(LPTSTR)szName);
							if(dex == LB_ERR)
								dex = (WPARAM)SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szName);
							SendDlgItemMessage(rtl->hRunDlg, IDC_RUNLISTS, CB_SETCURSEL, 0, 0L);
						}
					}
					SendMessage(hDlg, USER_RESET_TEST_SOURCES, 0, 0L);
					SendMessage(hDlg, USER_RESET_RUNLISTS, 0, 0L);
					SendMessage(hDlg, WM_CLOSE, 0, 0L);

				}

					return TRUE;
			}
			// Now look for changes in selection
			switch(GET_WM_COMMAND_CODE(wParam, lParam)) {
				case LBN_SELCHANGE:
					SendMessage(hDlg, USER_RESET_BUTTON_STATUS, 0, 0L);
					break;
				}
			return FALSE;

		// Must unregister 3D stuff and end dialog
		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;

		default:
			return FALSE;
		}
}



//*---------------------------------------------------------------------------------
//| GetNewRunList:
//|	Prompts the user for a new name for a run list.  This function uses a
//|	customized version of the GetSaveFileName dialog which disables the
//|	directory box forcing user to leave in same dir.
//| Parms:
//|	hDlg					Parent dialog window
//|	szList				For returning new name
//|	szFullPath			Used for writing to current directory
//| Returns:
//|	TRUE if we get a new name, FALSE else
//*---------------------------------------------------------------------------------
BOOL INTFUN GetNewRunList(HWND hDlg, LPTSTR szList, LPTSTR szFullPath)
{
	OPENFILENAME	ofn;
	TCHAR				szTmp[MAXFILELEN];
	TCHAR				szTmpFile[MAXRUNLIST];
	TCHAR				szRunListFilter[35];
	TCHAR				szTitle[50];

	GetString(szTitle, sizeof(szTitle)/sizeof(szTitle[0]), idsGetNewRunList);
	GetFilter(szRunListFilter,
              sizeof(szRunListFilter)/sizeof(szRunListFilter[0]), idsRunListFilter);
	memset((LPTSTR)szTmpFile, 0, MAXRUNLIST);
	memset((LPTSTR)szTmp, 0, MAXFILELEN);
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize      = sizeof(OPENFILENAME);
	ofn.hwndOwner        = hDlg;
	ofn.lpstrTitle			= szTitle;
	ofn.lpstrFilter		= szRunListFilter;
	ofn.lpstrFile			= szTmp;
	ofn.nMaxFile			= sizeof(szTmp);
	ofn.lpstrFileTitle	= szTmpFile;
	ofn.nMaxFileTitle		= sizeof(szTmpFile);
	ofn.lpstrDefExt		= (LPTSTR)szRLSEXT;
	ofn.Flags				= OFN_ENABLEHOOK |
								  OFN_HIDEREADONLY |
								  OFN_NOREADONLYRETURN |
								  OFN_OVERWRITEPROMPT |
								  OFN_PATHMUSTEXIST;
	ofn.lpfnHook			= (LPOFNHOOKPROC)NewRunListHook;
	if(GetSaveFileName(&ofn)) {
		lstrcpy(szList, szTmpFile);
		lstrcpy(szFullPath, szTmp);
		return TRUE;
		}
	return FALSE;				// Canceled or whatever
}


//*---------------------------------------------------------------------------------
//| NewRunListHook:
//|	This hook function will take care of 3d affect and will make it impossible
//|	for the user to change directories.
//| Parms:
//|	hDlg							Window handle
//|	msg							Window message
//|	wParam						Standard value
//|	lParam						Standard value
//| Returns:
//|	TRUE if we handled message, FALSE otherwise
//*---------------------------------------------------------------------------------
UINT EXTFUN NewRunListHook(HWND	hDlg, UINT	msg, WPARAM	wParam, LPARAM	lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			{
			CenterDialog(hDlg);
			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam, lParam)) {
				case IDOK:
					{
					TCHAR	szFile[_MAX_PATH];
					TCHAR	szExt[_MAX_EXT];

					GetText(GetDlgItem(hDlg,edt1), szFile);
					_tsplitpath(szFile, NULL, NULL, NULL, szExt);			// Ext includes '.'
					if(*szExt &&                                       // An exstension to check
						lstrcmpi(&szExt[1], szRLSEXT) != 0) {
						szMessageBox(hDlg,
								MB_ICONINFORMATION | MB_OK,
								(LPTSTR)szErrTitle,
								GetString(szErrBuff, MAXOUTBUFF, idsFileExtMustBeRls));
						return TRUE;
						}
					}
					break;
				}
			break;

		case WM_CLOSE:
			break;

		default:
			break;
	}

	return FALSE;
}




//*---------------------------------------------------------------------------------
//| DftRunListChangedByTab:
//|	Unfortunatley, CBN_SELENDOK only tracks mouse clicks and the ENTER key.
//|	The user may still choose a new item and TAB to change selection.  The only
//|	way to fix this is to track a kill focus and see if the selection has
//|	changed.
//| Parms:
//|	rtl					The run test list with our current value
//|	szList				The current selection
//| Returns:
//|	TRUE if we get a new name, FALSE else
//*---------------------------------------------------------------------------------
BOOL INTFUN DftRunListChangedByTab(lpRUNTESTLIST rtl, LPTSTR szList)
{
	LPTSTR		str, lstr;
	str = lstr = rtl->szRunList;
	while(str = _tcschr(lstr, '\\'))
		lstr = str + 1;
	if(lstrcmpi(lstr, szList) != 0)
		return TRUE;
	else
		return FALSE;
}

//*------------------------------------------------------------------------
//|  AbortRun:
//|		This was added as an entry point to the Inhouse 2.0 library since
//|		longjmp cannot be called from C++ code.
//|	Parms:
//|		henv		Environment to jump back to
//|		iState	State returned to the environment
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID EXTFUN AbortRun(jmp_buf henv, int iState)
{
	longjmp(henv, iState);
}

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	RUNTEST.C
//|
//| Purpose:
//|		This module contains functions for running autotests from the command
//|	line.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "runtest.h"
#pragma warning(disable:4996)
#pragma warning(disable:4995)
static TCHAR vszFile[] = TEXT(__FILE__);

// Note:  The following strings were being placed in the code segment
//				but this DLL got so large that multiple code segments were
//				being allocated and hence the offsets got screwed up
//
// Generic strings
static TCHAR szErrTitle[]			=  TEXT("Error");
static TCHAR szTestTitle[]			=  TEXT("ODBC Test");
static TCHAR szNewLine[]			=  TEXT("\r\n");

TCHAR szGATORINI[_MAX_PATH];

// GATOR.INI sections and entries
#ifndef WIN32
static TCHAR szGATORINIFILE[]		=  TEXT("GATOR.INI");
#else
#ifdef _UNICODE
static TCHAR szGATORINIFILE[]		=  TEXT("GATOR32W.INI");
#else
static TCHAR szGATORINIFILE[]		=  TEXT("GATOR32.INI");
#endif
#endif
static TCHAR szSTARTUP[] 			=  TEXT("startup");
static TCHAR szLASTRUNLIST[]		=  TEXT("Last Run List");
static TCHAR szDFTFROMDIR[] 		=  TEXT("Default From Dir");
static TCHAR szAUTOTESTS[]			=  TEXT("Auto Tests");
static TCHAR	szDLL[]				=  TEXT("DLL");
static TCHAR szINSTALLED[]			=  TEXT("Installed");
static TCHAR szSQLDRIVERS[]		=  TEXT("SQL_DRIVERS");
static TCHAR szSERVER0[]			=  TEXT("SERVER0");
static TCHAR szLOGIN0[]				=  TEXT("LOGIN0");
static TCHAR szPASSWORD0[]			=  TEXT("PASSWORD0");
static TCHAR szKEYWORDS[]			=  TEXT("KEYWORDS");
static TCHAR szGROUPS[]				=  TEXT("GROUPS");
static TCHAR szLASTRUN[]			=  TEXT("Last Run");
static TCHAR szEOS[]					=  TEXT("\0");
dCSEG(TCHAR) szMANAGESOURCES[]	= TEXT("ManageSources");
dCSEG(TCHAR) szMANAGETESTS[]		= TEXT("ManageTests");
dCSEG(TCHAR) szMANAGEGROUPS[]		= TEXT("ManageGroups");
dCSEG(TCHAR) szRUNAUTOTEST[]		= TEXT("RunAutoTest");
dCSEG(TCHAR) szEMPTY[]				= TEXT("\0");
dCSEG(TCHAR) szDLLToken[]			= TEXT("DLL");
dCSEG(TCHAR) szAutoTestToken[]	= TEXT("TEST");
dCSEG(TCHAR) szTestCaseToken[]	= TEXT("CASE");
dCSEG(TCHAR) szKeyWordToken[]		= TEXT("KEYWORDS");
dCSEG(TCHAR) szDSNToken[]			= TEXT("DSN");
dCSEG(TCHAR) szLOGToken[]			= TEXT("LOG");
dCSEG(TCHAR) szIsolateToken[]		= TEXT("ISOLATE");
dCSEG(TCHAR) szDebugToken[]		= TEXT("DEBUG");
dCSEG(TCHAR) szCurslibToken[]		= TEXT("CURSLIB");
dCSEG(TCHAR) szRLSTToken[]			= TEXT("RUNLIST");
dCSEG(TCHAR) szAppendToken[]		= TEXT("APPEND");


extern HINSTANCE 			hInst;
extern TCHAR 				szOutBuff[MAXOUTBUFF];

void INTFUN RunTest(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,UINT cItems,
	UINT *lpcAutoErrors)
{
	UINT dex;
	UINT *rglMask=NULL;

#ifndef _LOGTIME_OFF
	TIMEINFO					strtime, endtime, elapsed;
	TCHAR						szTime[MAXOUTBUFF];
#endif

	TCHAR szOutBuff[MAXOUTBUFF];


	// Set the jump address. If an inhouse function fails and aborts, we will
	// return to this point of execution with a non zero return status.
	// The process will terminate and exit gracefully.
	int iState;
	if((iState=setjmp(lpSI->env)) != 0) {    	// If 0, its just the original setting
		lpSI->cErrors = -1;
		goto aborted;
		}
	szLogPrintf(lpSI, TRUE,
					GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsNowExecuting),
					lpati->szTestName);
	szLogPrintf(lpSI, TRUE,
					GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsAutoTestSource),
					lpSI->szSource);

#ifndef _LOGTIME_OFF
	GetTime(&strtime);
	szLogPrintf(lpSI, TRUE,
					GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsTimeStarted),
					szGetTime(szTime,strtime));
#endif // _LOGTIME_OFF



	// Allocate memory for the rglMaskEntire field which will contain the
	//		entire bitmask of tests to run.
	lpSI->rglMaskEntire = (UINT *)AllocateMemory(MINREQUIREDINTS(cItems) * sizeof(UINT *));
	if(!lpSI->rglMaskEntire)
		goto aborted;
	memset(lpSI->rglMaskEntire, 1,
			(size_t)(MINREQUIREDINTS(cItems) * sizeof(UINT)));
	if(!lpSI->fIsolate) {
		// The non-isolated case means we will only call AutoTestFunc once
		// with the entire bitmask.  This is quite easy for us to do.
		(*lpati->lpTestFunc)(lpSI);
		}
	else {
		// The non-isolated case means that we can only set one bit at a
		// time and must therefore go through each one
		rglMask = (UINT *)AllocateMemory(MINREQUIREDINTS(cItems) * sizeof(UINT *));

		if(!rglMask)
			goto aborted;

		//Save current rglMask
		*rglMask = *lpSI->rglMask;

		//Clear out lpSI->rglMask
		for(dex=0;  dex < cItems;  dex++)
			BitClear(lpSI->rglMask, dex);

		for(dex=0;  dex < cItems;  dex++) {
			if(BitGet(rglMask, dex)) {
				if(FAbortCheck()) {
					szLogPrintf(lpSI, TRUE,
								GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsAborted));
					goto aborted;
					}
				else {
					// Read to run each test.  Run them in isolated mode which means that
					// we set each bit and call for one at a time.
					BitSet(lpSI->rglMask, dex);
					(*lpati->lpTestFunc)(lpSI);
					BitClear(lpSI->rglMask, dex);
					}
				}
			}
		FREE_PTR(rglMask);
		} // End of test case selected

#ifndef _LOGTIME_OFF
	GetTime(&endtime);
	szLogPrintf(lpSI, TRUE,
					GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsTimeFinished),
					szGetTime(szTime,endtime));
#endif // _LOGTIME_OFF

	switch(lpSI->cErrors) {
		case -1:
			szLogPrintf(lpSI, TRUE,
				GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsAborted));
			break;

		case 0:
			szLogPrintf(lpSI, TRUE,
				GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsTestCompleted),
				lpati->szTestName);
			szLogPrintf(lpSI, TRUE,
				GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsNoFailures),
				lpati->szTestName);
			break;

		default:
			szLogPrintf(lpSI, TRUE,
				GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsTestCompleted),
				lpati->szTestName);
			szLogPrintf(lpSI, TRUE,
				GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, (lpSI->cErrors == 1) ? idsTestFailure : idsTestFailures),
				lpSI->cErrors);
			*lpcAutoErrors += lpSI->cErrors;
			break;
		}

#ifndef _LOGTIME_OFF
	CElapsed(&strtime, &endtime, &elapsed);
	szLogPrintf(lpSI, TRUE,
				GetRCString(hInst,(LPTSTR)szOutBuff, MAXOUTBUFF, idsTimeTotal),
				szGetTime(szTime,elapsed));
#endif // _LOGTIME_OFF

aborted:
	FREE_PTR(lpSI->rglMaskEntire);

}

//*---------------------------------------------------------------------------------
//| DelFile:
//|	Invoke the OpenFile function with OF_DELETE option.  Note that if there is
//|	an error on deletion, no messages are displayed by this function.  It might
//|	be possible that the OpenFile function does so however.
//| Parms:
//|	szFile				Full path name of file to delete
//| Returns:
//|	TRUE if file was deleted, FALSE on error, in which case the file may or may
//|		not be gone
//*---------------------------------------------------------------------------------
BOOL EXTFUN DelFile(LPTSTR szFile)
{
#ifndef WIN32
	OFSTRUCT			ofs;

	memset(&ofs, 0, sizeof(OFSTRUCT));
	ofs.cBytes = sizeof(OFSTRUCT);
	lstrcpy(ofs.szPathName, szFile);

	if(HFILE_ERROR == OpenFile(szFile, &ofs, OF_DELETE))
		return FALSE;
	else
		return TRUE;

#else
	return(DeleteFile(szFile));
#endif
}


UINT INTFUN GetNumSources(LPTSTR lpSrcs,LPTSTR szSources)
{
	UINT i=0;
	TCHAR *pComma=NULL;

	lstrcpy(szSources,lpSrcs);

	while (pComma=_tcsstr(lpSrcs,TEXT(",")))
		{
		lpSrcs=pComma+1;
		i++;
		}

	if (lpSrcs)
		i++;

	return(i);

} //GetNumSources()



void INTFUN FillServerInfo(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,HWND hwnd, HINSTANCE hInst,
				HENV henv, HDBC hdbc, HSTMT hstmt)
{


   GetServer0(lpSI->szSource,		lpSI->szValidServer0, 	sizeof(lpSI->szValidServer0));
	GetLogin0(lpSI->szSource,		lpSI->szValidLogin0, 	sizeof(lpSI->szValidLogin0));
	GetPassword0(lpSI->szSource,	lpSI->szValidPassword0, sizeof(lpSI->szValidPassword0));
	GetKeywords(lpSI->szSource,	lpSI->szKeywords, 		sizeof(lpSI->szKeywords));
	lpSI->hwnd = hwnd;
	lpSI->henv = henv;
	lpSI->hdbc = hdbc;
	lpSI->hstmt = hstmt;
	lpSI->cErrors = 0;
   lpSI->hLoadedInst=lpati->hInst;


} //FillServerInfo()


void INTFUN SetAutoTestBits(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,int cCnt,
	LPTSTR szName,LPTSTR szTestCase)
{
	int nIndex;
	TCHAR	szDesc[MEDBUFF];

	if (!lstrcmpi(szTestCase,szEOS))
		{
		for (nIndex=1;nIndex <= cCnt;nIndex++)
			BitSet(lpSI->rglMask,nIndex-1);
		}
	else
		{
		//Search for Specified Test Case to run
		for (nIndex=1;nIndex <= cCnt;nIndex++)
			{
			(*lpati->lpTestDesc)((UWORD)nIndex, szName, szDesc);
			if (!lstrcmpi(szName,szTestCase))
				BitSet(lpSI->rglMask,nIndex-1);
			}
		}

}


void INTFUN ProcessSection(FILE *fp,LPTSTR pSection,LPTSTR szSources)
{
	TCHAR	szLine[LARGEBUFF],
			*pTok=NULL;

	if (pTok=_tcstok(pSection,TEXT("[]")))
		{
		//Get All DSN's
		if (!_tcscmp(pTok,TEXT("SQL_DRIVERS")))
			{
			while (_fgetts(szLine,LARGEBUFF,fp))
				{
				if (pTok=_tcstok(szLine,TEXT("=")))
					{
					if (!_tcscmp(szSources,szEOS))
						lstrcpy(szSources,pTok);
					else
						{
						lstrcat(szSources,TEXT(","));
						lstrcat(szSources,szLine);
						}
					}

				} //end while()

			} //if (!_tcscmp(pTok...
		}

}

void INTFUN ProcessEntry(lpSERVERINFO lpSI,LPTSTR pTok,
	lpTEST rgAutoTest,UINT *puNumTests)
{
	TCHAR	*szEntry=_tcsdup(pTok);
	LONG	lNumSel=0;
	UINT	iNum=0;

	if (pTok=_tcstok(NULL,TEXT("=,")))
		{
		TCHAR *pTmp=_tcsdup(pTok);

		if (lNumSel=_ttol(pTok))
			{
			//Get Selected tests
			if (pTok=_tcstok(NULL,TEXT("=,")))
				{
				if (iNum=_ttoi(pTok))
					{
					lstrcpy(rgAutoTest[*puNumTests].szTest,szEntry);
					rgAutoTest[*puNumTests].rglMask=iNum;
					(*puNumTests)++;
					}
				}
			}

		} //if (pTok=_tcstok...

	if (!_tcscmp(szEntry,TEXT("Debug")))
		lpSI->fDebug=lNumSel;
	else if (!_tcscmp(szEntry,TEXT("Screen")))
		lpSI->fScreen=lNumSel;
	else if (!_tcscmp(szEntry,TEXT("Log File")))
		lpSI->fLog=lNumSel;
	else if (!_tcscmp(szEntry,TEXT("Log File Name")))
		lstrcpy(lpSI->szLogFile,pTok);
	else if (!_tcscmp(szEntry,TEXT("Isolate")))
		lpSI->fIsolate=lNumSel;
	else if (!_tcscmp(szEntry,TEXT("Cursor Library")))
		lpSI->vCursorLib=lNumSel;


} //ProcessEntry()



UINT INTFUN HandleRunList(lpSERVERINFO lpSI,LPTSTR szRunList,
	LPTSTR szSources,lpTEST rgAutoTest)
{
	FILE	*fp=NULL;
	TCHAR	szLine[LARGEBUFF],
			*pTok=NULL,
			*pLine=NULL,
			*pSection=NULL,
			*pDSN=NULL;

	UINT	uNumTests=0;

	CHAR	szRunListA[LARGEBUFF];

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,szRunList,-1,szRunListA,LARGEBUFF,NULL,NULL);
#else
	strcpy(szRunListA,szRunList);
#endif


	if (fp = fopen(szRunListA,"r"))
		{
		while (_fgetts(szLine,LARGEBUFF,fp))
			{
			if (pTok=_tcsstr(szLine,TEXT("\n")))
				lstrcpyn(pTok,szEOS,sizeof(TCHAR));

			pLine=_tcsdup(szLine);

			if (pSection=_tcsstr(szLine,TEXT("[")))
				ProcessSection(fp,pSection,szSources);
			else if (pTok=_tcstok(szLine,TEXT("=,")))
				ProcessEntry(lpSI, pTok, rgAutoTest,&uNumTests);

 			} //end while

		fclose(fp);

		} //if (fp = _tfopen...

	return(uNumTests);

} //HandleRunList()


void INTFUN RunAutoTests(lpSERVERINFO lpSI,lpAUTOTESTINFO lpati,UINT uNumTests,
	LPTSTR szAutoTest,LPTSTR szTestCase,TEST rgAutoTests[],LPTSTR szSources,
	HWND hwnd)
{
	UINT	cAutoErrors=0,
			iTests=0,
			cCnt=0;

	TCHAR	szPath[_MAX_PATH],
			szTestName[_MAX_PATH];
	LPTSTR lpszTok=NULL;
	BOOL	fRLS=FALSE;


	//check if we are processing a Run List
	fRLS=_tcscmp(rgAutoTests[iTests].szTest,szEOS);

	//Loop through all specified Autotests
	for (iTests=0;iTests < uNumTests;iTests++)
		{
		if (!fRLS)
			lstrcpy(lpati->szTestName,szAutoTest);
		else
			lstrcpy(lpati->szTestName,rgAutoTests[iTests].szTest);

		lstrcpy(szTestName,lpati->szTestName);
		// Next line isn't necessary, remove at a later date
		//lstrcat(szTestName, TEXT(" Auto Test"));
		szPath[0] = '\0';
		GetTestDLLPath(szTestName, szPath, sizeof(szPath)/sizeof(szPath[0]));

		// need to bail if the DLL wasn't found
		if (!lstrlen(szPath))
			{
			wsprintf( szOutBuff, TEXT("The DLL specified by the entry for %s does not exist"), szTestName);
			szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					TEXT("Unable to locate or load DLL"),
					(LPTSTR) szOutBuff,
					NULL);
			continue;
			}
		lstrcpy(lpati->szPath,szPath);
		if (!LoadAutoTestDLL(lpati, szPath))
			continue;

		//Get Number of Tests in Autotest
		(*lpati->lpTestName)(szAutoTest,&cCnt);

		lpSI->rglMask = (UINT *)AllocateMemory(MINREQUIREDINTS(cCnt) * sizeof(UINT *));

		//set rglMask
		if (fRLS)
			SetAutoTestBits(lpSI,lpati,cCnt,szAutoTest,szTestCase);
		else
			*lpSI->rglMask=rgAutoTests[iTests].rglMask;

		//loop through all sources (i.e. DSN's)
		lpszTok=_tcstok(szSources,TEXT(","));
		while (lpszTok)
			{
			EatWhiteSpace(lpszTok);
			lstrcpy(lpSI->szSource,lpszTok);

			FillServerInfo(lpSI,lpati,hwnd,hInst,NULL,NULL,NULL);
			RunTest(lpSI,lpati,cCnt,&cAutoErrors);
#if 0
			// Hopeless attempt to update the screen
			if (lpSI->fScreen)
				{
				ShowWindow(lpSI->hwnd, SW_SHOWMAXIMIZED);
				UpdateWindow(lpSI->hwnd);
				}
			RunTest(lpSI,lpati,cCnt,&cAutoErrors);
			if (lpSI->fScreen)
				{
				UpdateWindow(lpSI->hwnd);
				}
#endif

			if(fRLS)
				break;

			lpszTok = NULL;
			lpszTok=_tcstok(NULL,TEXT(","));
			}

		FREE_PTR(lpSI->rglMask);
		lpSI->rglMask=NULL;

		UnLoadAutoTestDLL(lpati);

		} //for (iTests=0;...


} //RunAutoTests()




UINT INTFUN ProcessCmdLine(LPTSTR lpszCmdLine,LPTSTR szAutoTest,LPTSTR szTestCase,
		LPTSTR szKeyWords,lpSERVERINFO lpSI,LPTSTR szSources,TEST rgAutoTests[])
{
	LPTSTR	lpszTok=NULL,
			lpstr=NULL,
			szCmdLine=NULL;

	TCHAR	szLOG[_MAX_PATH]=TEXT(""),
			szPath[_MAX_PATH]=TEXT(""),
			szRunList[_MAX_PATH]=TEXT("");

	short	nAutoLen=0,
			nCaseLen=0,
			nKeyLen=0,
			nDSNLen=0,
			nLOGLen=0,
			nIsolateLen=0,
			nCursLen=0,
			nRLSTLen=0;

	BOOL fAppend=FALSE;
	BOOL fRLS = FALSE;

	//can be running either thru runlist or separate cmdline options

	UINT	uNumTests=0; //default is 1 AutoTest

	//make a copy of the command line
	szCmdLine=_tcsdup(lpszCmdLine);

	//Get Lengths of Tokens
  	nAutoLen=(short)lstrlen(szAutoTestToken);
   nCaseLen=(short)lstrlen(szTestCaseToken);
   nKeyLen=(short)lstrlen(szKeyWordToken);
	nDSNLen=(short)lstrlen(szDSNToken);
	nLOGLen=(short)lstrlen(szLOGToken);
	nCursLen=(short)lstrlen(szCurslibToken);
	nRLSTLen=(short)lstrlen(szRLSTToken);

	lpszTok=_tcstok(szCmdLine, TEXT("/"));

	while (lpszTok)
		{
		if (lpstr=_tcsstr(lpszTok, szAutoTestToken))
		{
			lstrcpy(szAutoTest,&lpstr[nAutoLen]);
			if ( !fRLS)
			{

				lstrcpy(rgAutoTests[uNumTests].szTest,szAutoTest);
				rgAutoTests[uNumTests].rglMask=1;
				(uNumTests)++;
			}
		}
		else if (lpstr=_tcsstr(lpszTok, szTestCaseToken))
			lstrcpy(szTestCase,&lpstr[nCaseLen]);
		else if (lpstr=_tcsstr(lpszTok, szKeyWordToken))
			lstrcpy(szKeyWords,&lpstr[nKeyLen]);
		else if (lpstr=_tcsstr(lpszTok, szDSNToken))
		{
			lstrcpy(szSources,&lpstr[nDSNLen]);
			lstrcat(szSources,TEXT(","));
		}
		else if (lpstr=_tcsstr(lpszTok, szLOGToken))
			{
			lstrcpy(lpSI->szLogFile,&lpstr[nLOGLen]);
			lpSI->fLog=TRUE;
			}
		else if (lpstr=_tcsstr(lpszTok, szAppendToken))
			fAppend=TRUE;
		else if (lpstr=_tcsstr(lpszTok, szIsolateToken))
			lpSI->fIsolate=TRUE;
		else if (lpstr=_tcsstr(lpszTok, szDebugToken))
			lpSI->fDebug=TRUE;
		else if (lpstr=_tcsstr(lpszTok, szCurslibToken))
			{
			//Set Cursor Lib load options
			TCHAR szCurs[SMALLBUFF];
			lstrcpy(szCurs,&lpstr[nCursLen]);

			EatWhiteSpace(szCurs);
			if (!_tcscmp(szCurs,TEXT("SQL_CUR_USE_DRIVER")))
				lpSI->vCursorLib=SQL_CUR_USE_DRIVER;
			else if (!_tcscmp(szCurs,TEXT("SQL_CUR_USE_IF_NEEDED")))
				lpSI->vCursorLib=SQL_CUR_USE_IF_NEEDED;
			else if (!_tcscmp(szCurs,TEXT("SQL_CUR_USE_ODBC")))
				lpSI->vCursorLib=SQL_CUR_USE_ODBC;
			}
		else if (lpstr=_tcsstr(lpszTok, szRLSTToken))
			{
			//Process the Run List
			lstrcpy(szRunList,&lpstr[nRLSTLen]);
			EatWhiteSpace(szRunList);
			uNumTests=HandleRunList(lpSI,szRunList,szSources,rgAutoTests);
			fRLS = TRUE;
			}
		lpszTok=_tcstok(NULL,TEXT("/"));

	} //while (lpszTok)

	EatWhiteSpace(szSources);
	//EatWhiteSpace(szAutoTest);
	EatWhiteSpace(szTestCase);
	EatWhiteSpace(szKeyWords);
	EatWhiteSpace(lpSI->szLogFile);

	//if user did not specify APPEND then delete log file
	if (!fAppend)
		DelFile(lpSI->szLogFile);

	return(uNumTests);

} //ProcessCmdLine()



//*-----------------------------------------------------------------------
//| RunFromCmdLine()
//|		This function Parse command line for User info.
//| 	(i.e. MSQPRF32.EXE /DSN: ms_desktop /UID: ronf /PWD: Admin)
//| Parameters:
//|		lpszCmdLine				Command line parameters
//| Return Value:
//|		None
//*-----------------------------------------------------------------------

BOOL INTFUN RunFromCmdLine(lpRUNTESTLIST prst,LPTSTR lpszCmdLine,HWND hwnd)
{
	TCHAR 		szAutoTest[MEDBUFF]=TEXT(""),
					szTestCase[MEDBUFF]=TEXT(""),
					szKeyWords[LARGEBUFF]=TEXT(""),
					szSources[XLARGEBUFF]=TEXT("");


	SERVERINFO 		SI;
	AUTOTESTINFO 	ati;
	TEST 				rgAutoTests[MAX_TESTS];
	UINT				uNumTests=0;

	//Clear out ARGS
	memset(prst,0,sizeof(RUNTESTLIST));
	memset(&SI,0,sizeof(SERVERINFO));
	memset(&ati,0,sizeof(AUTOTESTINFO));
	memset(&rgAutoTests,0,sizeof(TEST) * MAX_TESTS);

	//ConvertToUnicode(lpszCmdLine,tszCmdLine, sizeof(tszCmdLine));

	uNumTests=ProcessCmdLine(lpszCmdLine,szAutoTest,szTestCase,szKeyWords,
		&SI,szSources,rgAutoTests);

   if(!FindFileDir((LPTSTR)szGATORINIFILE, szGATORINI))
   	lstrcpy(szGATORINI,szGATORINIFILE);		// Just use GATOR.INI, no path

	RunAutoTests(&SI,&ati,uNumTests,szAutoTest,szTestCase,rgAutoTests,
		szSources,hwnd);

	return(FALSE);

} //RunFromCmdLine()

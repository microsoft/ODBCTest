//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHINST.C
//|
//| Purpose:
//|		This module contains the function handlers for the Installer Menu
//|		Note that each function can be found in the order that it appears
//|		on the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "odbcinst.h"
#include "fhinst.h"
#include "resource.h"
#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR				szOutBuff[MAXOUTBUFF];
extern HINSTANCE 		hInst;								// For this app
extern HWND				hwndClient;							// For MDI children
extern HWND				hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS	lpUsrOptions;						// Global user options
lpINSTALLER				lpInst;

dCSEG(TCHAR)			szODBCINST_COREKEY[] =		TEXT("SOFTWARE\\ODBC\\ODBCINST.INI\\ODBC Core");
dCSEG(TCHAR)			szODBCINST_BASE_HKEY[] =	TEXT("SOFTWARE\\ODBC\\ODBCINST.INI");
dCSEG(TCHAR)			szTRANSLATOR_HKEY[] =		TEXT("SOFTWARE\\ODBC\\ODBCINST.INI\\MS Code Page Translator");
dCSEG(TCHAR)			szODBCINI[] =					TEXT("ODBC.INI");
dCSEG(TCHAR)			szEOSs[] =						TEXT("\0");
dCSEG(TCHAR)			szSEMICOLON[] =				TEXT(";");
#define INSTALL		TRUE
#define REMOVE			FALSE



//*---------------------------------------------------------------------------------
//| DoubleNullTerm:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

LPTSTR INTFUN DoubleNullTerm(LPTSTR lpInStr)
{
	UINT		uLen=0,
				uCharLen=sizeof(TCHAR);
	LPTSTR	lpToken=NULL,
				szDup=NULL;

	if (!lpInStr)
		return(NULL);

	//if there are no semi-colons, do not parse
	if (!_tcschr(lpInStr,TEXT(';')))
		return(lpInStr);

	uLen=lstrlen(lpInStr);
	szDup=AllocateMemory(uLen+(uCharLen*2));
	lstrcpy(szDup,lpInStr);
	lpToken=_tcstok(szDup,szSEMICOLON);

	while (lpToken)
		lpToken=_tcstok(NULL,szSEMICOLON);

	if (!_tcsnccmp(&szDup[uLen-1],szSEMICOLON,uCharLen))
		memcpy(&szDup[uLen-1],szEOSs,uCharLen);

	return(szDup);

} //DoubleNullTerm()


//*---------------------------------------------------------------------------------
//| DisplayUsageCounts:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN DisplayUsageCounts(HWND hwndOut,DWORD dwPrevUsageCount,DWORD dwCurrUsageCount,
	LPTSTR szFunc)
{
	TCHAR	szPrev[MEDBUFF]=TEXT(""),
			szCurr[MEDBUFF]=TEXT(""),
			szFuncCount[LARGEBUFF];

	if (idsPrevUsageCount)
		GetGtrString(szPrev, MEDBUFF, idsPrevUsageCount);

	if (idsCurrUsageCount)
		GetGtrString(szCurr, MEDBUFF, idsCurrUsageCount);

	if (szFunc)
		wsprintf(szFuncCount,TEXT("\t\t\t\t\t\t\t\r\n%s:\r\n"),szFunc);

	szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,
				szFuncCount, dwPrevUsageCount);

	szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,
				szPrev, dwPrevUsageCount);

	szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,
				szCurr, dwCurrUsageCount);

} //DisplayUsageCounts()


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

RETCODE INTFUN ValidateRegistryKey(LPCTSTR lpcKey)
{
#ifdef WIN32

	LONG lRc=0;
	HKEY	 hkeySubKey;

	if ((lRc=RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpcKey,0, KEY_READ | KEY_WRITE,
		&hkeySubKey)) != ERROR_SUCCESS)
		return(FALSE);

	RegCloseKey(HKEY_LOCAL_MACHINE);

#endif

	return(TRUE);

} //ValidateRegistryKey()


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

DWORD INTFUN CheckUsageCount(LPCTSTR lpcKey)
{
	DWORD	dwUsageCount;
#ifdef WIN32
	DWORD	cbUCount = sizeof(dwUsageCount);
	LONG lRc=0;
	HKEY	 hkeySubKey;

	if ((lRc=RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpcKey,0, KEY_READ | KEY_WRITE,
		&hkeySubKey)) != ERROR_SUCCESS)
  		{
  //		szWrite("ERROR - No CORE Key found, and DriverManager was installed!",TRUE);
		return(0);
		}

	lRc = RegQueryValueEx(hkeySubKey,TEXT("UsageCount"),NULL,NULL,
						(LPBYTE) &dwUsageCount,&cbUCount);

	if (lRc != ERROR_SUCCESS)
		dwUsageCount=0;

	RegCloseKey(HKEY_LOCAL_MACHINE);

#endif

	return(dwUsageCount);

} // SQLCheckUsageCount()





//*---------------------------------------------------------------------------------
//| DisplayMsgWithKey:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN DisplayMsgWithKey(HWND hwndOut, UINT idsMsg,LPCTSTR szKey,UINT idsFunc)
{
	TCHAR szOut[LARGEBUFF]=TEXT("");
	TCHAR szFunc[LARGEBUFF]=TEXT("");

	if (idsFunc)
		GetGtrString(szFunc, LARGEBUFF, idsFunc);

	if (idsMsg)
		GetGtrString(szOut, LARGEBUFF, idsMsg);

	if (idsFunc)
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,
					szOut, szKey,szFunc);
	else
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,
					szOut, szKey);


} //DisplayMsgWithKey()


//*---------------------------------------------------------------------------------
//| CheckRemoveStatus:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN CheckRemoveStatus(HWND hwndOut,BOOL fSuccess,DWORD dwUsageCount,DWORD dwBeforeUsageCount,
		LPTSTR szRegKey,BOOL fKeyExists,UINT idsFunc)
{

	if (fSuccess)
		{
		if (dwUsageCount == 0)
			{
			if (fKeyExists)
				DisplayMsgWithKey(hwndOut, idsKeyNotDeleted,szRegKey,idsFunc);
			else
				DisplayMsgWithKey(hwndOut, idsKeySuccessfullyDeleted,szRegKey,idsFunc);
			}
		}
	else
		{
		if (dwBeforeUsageCount == 0)
			DisplayMsgWithKey(hwndOut, idsUsageCount0,szRegKey,idsFunc);
		if (!fKeyExists)
			DisplayMsgWithKey(hwndOut, idsRegKeyNotExist,szRegKey,idsFunc);
		}

} //CheckRemoveStatus()


//*---------------------------------------------------------------------------------
//| CheckInstallStatus:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN CheckInstallStatus(HWND hwndOut,BOOL fSuccess,DWORD dwUsageCount,DWORD dwBeforeUsageCount,
		LPTSTR szRegKey,BOOL fKeyExists,UINT idsFunc)
{

	if (fSuccess)
		{
		if (dwUsageCount == 0)
			{
			if (fKeyExists)
				DisplayMsgWithKey(hwndOut, idsUsageCountNotIncremented,szRegKey,idsFunc);
			else
				DisplayMsgWithKey(hwndOut, idsKeyNotCreated,szRegKey,idsFunc);
			}
		}

} //CheckInstallStatus()


//*---------------------------------------------------------------------------------
//| VerifyTranslatorCounts:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN VerifyTranslatorCounts(HWND hwndOut,BOOL fSuccess,DWORD dwBeforeUsageCount,
	BOOL fBeforeKeyExists,UINT idsFunc,BOOL fInstall)
{
#ifdef WIN32
	BOOL 	fKeyExists=ValidateRegistryKey(szTRANSLATOR_HKEY);
	DWORD	dwAfterUsageCount=CheckUsageCount(szTRANSLATOR_HKEY);

	if (fInstall)
		CheckInstallStatus(hwndOut,fSuccess,dwAfterUsageCount,
			dwBeforeUsageCount,(LPTSTR)szTRANSLATOR_HKEY,	fKeyExists,idsFunc);
	else
		CheckRemoveStatus(hwndOut,fSuccess,dwAfterUsageCount,
			dwBeforeUsageCount,(LPTSTR)szTRANSLATOR_HKEY,	fKeyExists,idsFunc);

	DisplayUsageCounts(hwndOut,dwBeforeUsageCount,dwAfterUsageCount,TEXT("Translator Usage Counts"));
#endif

} //VerifyTranslatorCounts()


//*---------------------------------------------------------------------------------
//| VerifyDMCounts:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN VerifyDMCounts(HWND hwndOut,BOOL fSuccess,DWORD dwBeforeUsageCount,
	BOOL fBeforeKeyExists,UINT idsFunc,BOOL fInstall)
{
#ifdef WIN32
	BOOL fKeyExists=ValidateRegistryKey(szODBCINST_COREKEY);
	DWORD	dwAfterUsageCount=CheckUsageCount(szODBCINST_COREKEY);

	if (fInstall)
		CheckInstallStatus(hwndOut,fSuccess,dwAfterUsageCount,
			dwBeforeUsageCount,(LPTSTR)szODBCINST_COREKEY,fKeyExists,idsFunc);
	else
		CheckRemoveStatus(hwndOut,fSuccess,dwAfterUsageCount,
			dwBeforeUsageCount,(LPTSTR)szODBCINST_COREKEY,fKeyExists,idsFunc);

	DisplayUsageCounts(hwndOut,dwBeforeUsageCount,dwAfterUsageCount,TEXT("Driver Manager Usage Counts"));
#endif

} //VerifyDMCounts()

//*---------------------------------------------------------------------------------
//| VerifyDriverCounts:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN VerifyDriverCounts(HWND hwndOut,BOOL fSuccess,LPTSTR szDriverKey,
	DWORD dwBeforeUsageCount,BOOL fBeforeKeyExists,UINT idsFunc,BOOL fInstall)
{
#ifdef WIN32
	BOOL fKeyExists=ValidateRegistryKey(szDriverKey);
	DWORD	dwAfterUsageCount=CheckUsageCount(szDriverKey);

	if (fInstall)
		CheckInstallStatus(hwndOut,fSuccess,dwAfterUsageCount,
			dwBeforeUsageCount,szDriverKey,fKeyExists,idsFunc);
	else
		CheckRemoveStatus(hwndOut,fSuccess,dwAfterUsageCount,
			dwBeforeUsageCount,szDriverKey,fKeyExists,idsFunc);

	DisplayUsageCounts(hwndOut,dwBeforeUsageCount,dwAfterUsageCount,TEXT("Driver Usage Counts"));

#endif

} //VerifyDriverCounts()



//*---------------------------------------------------------------------------------
//| BuildDriverRegKey:
//| Parms:
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

LPTSTR INTFUN BuildDriverRegKey(LPTSTR szRegKey,LPTSTR szDriver)
{
	//Manufactur the registry key
	lstrcpy(szRegKey,szODBCINST_BASE_HKEY);
	lstrcat(szRegKey,TEXT("\\"));
	lstrcat(szRegKey,szDriver);

	return(szRegKey);

} //BuildDriverRegKey()

//*---------------------------------------------------------------------------------
//| BuildTransRegKey:
//| Parms:
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

LPTSTR INTFUN BuildTransRegKey(LPTSTR szRegKey,LPTSTR szDriver)
{
	//Manufactur the registry key
	lstrcpy(szRegKey,szTRANSLATOR_HKEY);
	lstrcat(szRegKey,TEXT("\\"));
	lstrcat(szRegKey,szDriver);

	return(szRegKey);

} //BuildDriverRegKey()



//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLRemoveDriver(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE,
								fBeforeKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);
	TCHAR						szRegKey[LARGEBUFF]=TEXT("");
	WORD						fRemoveDSN=*(WORD *)lpParms[1]->lpData;
	BOOL						fRemove=(BOOL)fRemoveDSN;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	//Manufactur the registry key
	if (!LPPARAMNULL(lpParms, 0))
		{
		lstrcpy(szRegKey,szODBCINST_BASE_HKEY);
		lstrcat(szRegKey,TEXT("\\"));
		lstrcat(szRegKey,(LPTSTR)lpParms[0]->lpData);
		dwBeforeUsageCount=CheckUsageCount(szRegKey);
		}

	fSuccess=(BOOL)SQLRemoveDriver(lpParms[0]->fNull ? NULL:	(LPCTSTR)lpParms[0]->lpData,			//lpszDriver
									fRemove,	//fRemoveDSN
									(DWORD *)lpParms[2]->lpData);	//lpdwUsageCount

	if (!LPPARAMNULL(lpParms, 0))
		VerifyDriverCounts(hwndOut,fSuccess,szRegKey,dwBeforeUsageCount,
			fBeforeKeyExists,idsSQLRemoveDriver,REMOVE);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLRemoveDriverManager(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE,
								fBeforeKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);

#ifdef WIN32
	fKeyExists=ValidateRegistryKey(szODBCINST_COREKEY);
	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szODBCINST_COREKEY);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLRemoveDriverManager((DWORD *)lpParms[0]->lpData);	//dwUsageCount

	VerifyDMCounts(hwndOut,fSuccess,dwBeforeUsageCount,
		fBeforeKeyExists,idsSQLRemoveDriverManager,REMOVE);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);
#endif
	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLInstallTranslator(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE,
								fBeforeKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);
	TCHAR						szRegKey[LARGEBUFF]=TEXT("");
	LPTSTR					szString=NULL;

	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szTRANSLATOR_HKEY);

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 										// szPathOut
						(*(WORD *)LPPARAMDATA(lpParms, 4)) * sizeof(TCHAR), 		//	  based on cbConnStrOutMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	szString=DoubleNullTerm((LPTSTR)lpParms[1]->lpData);

	fSuccess=(BOOL)SQLInstallTranslator(
									NULL,
									(LPTSTR)szString,
									(LPCTSTR)lpParms[2]->lpData,			//szPathIn
									(LPTSTR)lpParms[3]->lpData,				//szPathOut
									*(WORD *)lpParms[4]->lpData,		//cbPathOutMax
									(WORD *)lpParms[5]->lpData,		//pcbPathOut
									*(WORD *)lpParms[6]->lpData,		//fRequest
									(LPDWORD)lpParms[7]->lpData);			//dwUsageCount

	VerifyTranslatorCounts(hwndOut,fSuccess,dwBeforeUsageCount,
		fBeforeKeyExists,idsSQLInstallTranslator,INSTALL);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLInstallTranslatorEx:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLInstallTranslatorEx(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE,
								fBeforeKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);
	TCHAR						szRegKey[LARGEBUFF]=TEXT("");
	LPTSTR					szString=NULL;

	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szTRANSLATOR_HKEY);

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 										// szPathOut
						(*(WORD *)LPPARAMDATA(lpParms, 3)) * sizeof(TCHAR), 		//	  based on cbConnStrOutMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	szString=DoubleNullTerm((LPTSTR)lpParms[0]->lpData);

	fSuccess=(BOOL)SQLInstallTranslatorEx(
									(LPTSTR)szString,
									(LPCTSTR)lpParms[1]->lpData,			//szPathIn
									(LPTSTR)lpParms[2]->lpData,				//szPathOut
									*(WORD *)lpParms[3]->lpData,		//cbPathOutMax
									(WORD *)lpParms[4]->lpData,		//pcbPathOut
									*(WORD *)lpParms[5]->lpData,		//fRequest
									(LPDWORD)lpParms[6]->lpData);			//dwUsageCount

	VerifyTranslatorCounts(hwndOut,fSuccess,dwBeforeUsageCount,
		fBeforeKeyExists,idsSQLInstallTranslator,INSTALL);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[2], lpParms[4], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLInstallerError:
//|	This function handles calls to SQLInstallerError
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLInstallerError(STD_FH_PARMS)
{
	RETCODE rc=SQL_SUCCESS;
	WORD iError=*(WORD *)lpParms[0]->lpData;
	WORD cbErrorMsgMax=*(WORD *)lpParms[3]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 									// szErrorMsg
						*(WORD *)LPPARAMDATA(lpParms, 3) * sizeof(TCHAR), 		//	based on cbErrorMsgMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLInstallerError(iError,								//iError
								(DWORD *)lpParms[1]->lpData,		//pfErrorCode
								(LPTSTR)lpParms[2]->lpData,		//szErrorMsg
								cbErrorMsgMax,							//cbErrorMsgMax
								(WORD *)lpParms[4]->lpData);		//pcbErrorMsg


	// Log return code information
	LOGRETURNCODE(NULL, ci,rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[2], lpParms[4], TRUE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	return(rc);

} //lpSQLInstallerError()


//*---------------------------------------------------------------------------------
//| lpSQLRemoveTranslator:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLRemoveTranslator(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE,
								fBeforeKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);

#ifdef WIN32

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szTRANSLATOR_HKEY);

	fSuccess=(BOOL)SQLRemoveTranslator((LPCTSTR)lpParms[0]->lpData,	//lpszTranslator
												(LPDWORD)lpParms[1]->lpData);	//lpdwUsageCount

	VerifyTranslatorCounts(hwndOut,fSuccess,dwBeforeUsageCount,
		fBeforeKeyExists,idsSQLRemoveTranslator,REMOVE);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

#endif

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLConfigDriver(STD_FH_PARMS)
{
	BOOL	fSuccess=FALSE;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[4], 													// szConnStrOut
						(*(WORD *)LPPARAMDATA(lpParms, 5)) * sizeof(TCHAR), 		//	  based on cbConnStrOutMax
						lpUsrOptions->fBufferChecking);


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLConfigDriver(LPPARAMNULL(lpParms, 0) ? NULL : lpInst->hwndParent,	//hwndParent
									*(WORD *)lpParms[1]->lpData,									//fRequest
									(LPCTSTR)lpParms[2]->lpData,										//lpszDriver
									(LPCTSTR)lpParms[3]->lpData,										//lpszArgs
									(LPTSTR)lpParms[4]->lpData,											//lpszMsg
									*(WORD *)lpParms[5]->lpData,									//cbMsgMax
									(WORD *)lpParms[6]->lpData);									//pcbMsgOut

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[4], lpParms[6], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLConfigDataSource(STD_FH_PARMS)
{
	BOOL		fSuccess=FALSE;
	LPTSTR	lpstr=NULL;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	lpstr=DoubleNullTerm((LPTSTR)lpParms[3]->lpData);

	fSuccess=(BOOL)SQLConfigDataSource(
									LPPARAMNULL(lpParms, 0) ? NULL : lpInst->hwndParent,	//hwndParent
									*(WORD *)lpParms[1]->lpData,								//fRequest
									(LPCTSTR)lpParms[2]->lpData,									//lpszDriver
									(LPCTSTR)lpstr);									//lpszAttributes

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}
//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLCreateDataSource(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=(BOOL)SQLCreateDataSource(
									LPPARAMNULL(lpParms, 0) ? NULL : lpInst->hwndParent,	//hwndParent
									(LPTSTR)lpParms[1]->lpData);									//lpszDSN

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetInstalledDrivers(STD_FH_PARMS)
{
	BOOL fSuccess=FALSE;
	WORD cbBufMax=*(WORD *)lpParms[1]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[0], 											// lpszBuf
						cbBufMax * sizeof(TCHAR),
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=(BOOL)SQLGetInstalledDrivers(
									(LPTSTR)lpParms[0]->lpData,				//lpszBuf
									*(WORD *)lpParms[1]->lpData,		//cbBufMax
									(WORD *)lpParms[2]->lpData);		//pcbBufOut

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[0], lpParms[2], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetPrivateProfileString(STD_FH_PARMS)
{
	BOOL		fSuccess=FALSE;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 											// lpszReturnBuffer
						(*(SWORD *)LPPARAMDATA(lpParms, 4)) * (SWORD)sizeof(TCHAR), 		//	cchReturnBuffer
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=(BOOL)SQLGetPrivateProfileString(
						(LPCTSTR)lpParms[0]->lpData,	//lpszSection
						(LPCTSTR)lpParms[1]->lpData,	//lpszKey
						(LPCTSTR)lpParms[2]->lpData,	//lpszDefault
						(LPTSTR)lpParms[3]->lpData,	//lpszReturnBuffer
						*(DWORD *)lpParms[4]->lpData,	//cchReturnBuffer
						(LPTSTR)szODBCINI);				//lpszFile

		// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[4], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}


//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLInstallDriver(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);
	LPTSTR						lpstr=NULL;
	LPTSTR						lpToken=NULL;
	WORD						cbPathMax=*(WORD *)lpParms[3]->lpData,
								cbActual=0;
	TCHAR						szRegKey[LARGEBUFF]=TEXT("");
	LPTSTR					lpszInfFile=(LPTSTR)lpParms[0]->lpData;

	if (!LPPARAMNULL(lpParms, 1))
		{
		if (lpstr=DoubleNullTerm((LPTSTR)lpParms[1]->lpData))
			BuildDriverRegKey(szRegKey,lpstr);
		}

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 											// lpszPath
						cbPathMax * sizeof(TCHAR),
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szRegKey);

	fSuccess=(BOOL)SQLInstallDriver(
									lpParms[0]->fNull ? NULL :  lpszInfFile,
									(LPTSTR)lpstr ? lpstr : lpParms[1]->lpData,	//lpszDriver
									(LPTSTR)lpParms[2]->lpData,						//lpszPath
									cbPathMax,
									(WORD *)lpParms[4]->lpData);	//pcbPathOut

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	fKeyExists=ValidateRegistryKey(szRegKey);

	dwAfterUsageCount=CheckUsageCount((LPCTSTR)szRegKey);

	CheckInstallStatus(hwndOut,fSuccess,dwAfterUsageCount,
		dwBeforeUsageCount,szRegKey,fKeyExists,idsSQLInstallDriver);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[2], lpParms[4], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	FREE_PTR(lpstr);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLInstallDriverManager(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE,
								fBeforeKeyExists=TRUE;
	DWORD						dwUsageCount=0,
								dwBeforeUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[0], 											// lpszPath
						(*(WORD *)lpParms[1]->lpData) * sizeof(TCHAR), 		//	  based on cbPathMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szODBCINST_COREKEY);

	fSuccess=(BOOL)SQLInstallDriverManager(
									(LPTSTR)lpParms[0]->lpData,		//lpszPath
									*(WORD *)lpParms[1]->lpData,		//cbPathMax
									(WORD *)lpParms[2]->lpData);		//pcbPathOut


	VerifyDMCounts(hwndOut,fSuccess,dwBeforeUsageCount,
		fBeforeKeyExists,idsSQLInstallDriverManager,INSTALL);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[0], lpParms[2], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLManageDataSources(STD_FH_PARMS)
{
	BOOL		fSuccess=FALSE;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLManageDataSources(
			LPPARAMNULL(lpParms, 0) ? NULL : lpInst->hwndParent); //hwndParent

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLRemoveDefaultDataSource(STD_FH_PARMS)
{
	BOOL		fSuccess=FALSE;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLRemoveDefaultDataSource();

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLRemoveDSNFromIni(STD_FH_PARMS)
{
	BOOL			fSuccess=FALSE;
	LPCTSTR		lpszDSN=(LPCTSTR)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLRemoveDSNFromIni(lpParms[0]->fNull ? NULL : lpszDSN);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

RETCODE INTFUN lpSQLValidDSN(STD_FH_PARMS)
{
	BOOL		fSuccess=FALSE;
	UINT		idsStr=idsValidDSN;
	TCHAR		szOut[MEDBUFF]=TEXT("");
	HWND		hwndOut = GETOUTPUTWINDOW(ci);
	LPCTSTR	lpszDSN=(LPCTSTR)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLValidDSN(lpParms[0]->fNull ? NULL:lpszDSN);

	if (!fSuccess)
		idsStr=idsInvalidDSN;

	GetGtrString(szOut, MEDBUFF, idsStr);
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,szOut);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLWriteDSNToIni(STD_FH_PARMS)
{
	BOOL			fSuccess=FALSE;
	LPCTSTR		lpszDSN=(LPCTSTR)lpParms[0]->lpData,
					lpszDriver=(LPCTSTR)lpParms[1]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	fSuccess=(BOOL)SQLWriteDSNToIni(
									lpParms[0]->fNull ? NULL : lpszDSN,
									lpParms[1]->fNull ? NULL : lpszDriver);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}



//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLWritePrivateProfileString(STD_FH_PARMS)
{
	BOOL		fSuccess=FALSE;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=(BOOL)SQLWritePrivateProfileString(
						lpParms[0]->fNull ? NULL : (LPCTSTR)lpParms[0]->lpData,	//lpszSection
						lpParms[1]->fNull ? NULL : (LPCTSTR)lpParms[1]->lpData,	//lpszKey
						lpParms[2]->fNull ? NULL : (LPCTSTR)lpParms[2]->lpData,	//lpszString
						(LPTSTR)szODBCINI);							//lpszFile

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);
}

//*---------------------------------------------------------------------------------
//| lpSQLFreeStmt:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetTranslator(STD_FH_PARMS)
{
	RETCODE			rc=SQL_SUCCESS;
	BOOL				fSuccess=FALSE;
	HWND				hwndOut = GETOUTPUTWINDOW(ci);
	WORD				cbNameMax=*(WORD *)lpParms[2]->lpData;
	WORD				cbNameOut=12;
	TCHAR				szPath[300]=TEXT("");
	WORD				cbPathMax=*(WORD *)lpParms[5]->lpData;
	WORD				*pcbPathOut=(WORD *)lpParms[6]->lpData;
	DWORD				*pvOption=lpParms[7]->lpData;
	LPTSTR			lpszBuff=NULL;

	if (lpParms[1]->lpData)
	{
		lpszBuff=AllocateMemory(cbNameMax);

		//Make a copy of lpszName
		lstrcpyn(lpszBuff,(LPTSTR)lpParms[1]->lpData,cbNameMax-1);

		//We need to reallocate lpszName, because it is an
		//Input/Output arg. and it was allocated by ParameterBuilder()
		//solely as an Input Arg.
		FREE_PTR(lpParms[1]->lpData);

		// Allocate the correct size buffer
		BUILDOUTPUTPARM(lpParms[1], 							// lpszName
							(cbNameMax) * sizeof(TCHAR), 								//	based on cbNameMax
							lpUsrOptions->fBufferChecking);

		//copy what the user entered
		lstrcpy(lpParms[1]->lpData,lpszBuff);
	}

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[4], 											// lpszPath
						(*(WORD *)LPPARAMDATA(lpParms, 5)) * sizeof(TCHAR), 		//	based on cbPathMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=(BOOL)SQLGetTranslator(
									lpParms[0]->fNull ? NULL : lpInst->hwndParent,	//hwndParent
									(LPTSTR)lpParms[1]->lpData,
									cbNameMax,														//cbNameMax
									(WORD *)lpParms[3]->lpData,								//pcbNameOut
									(LPTSTR)lpParms[4]->lpData,								//lpszPath
									cbPathMax,														//cbPathMax
									pcbPathOut,														//pcbPathOut
									pvOption);														//pvOption

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[4], lpParms[6], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	FREE_PTR(lpszBuff);

	return(rc);
}


//*------------------------------------------------------------------------
//| GetInstallerInfoWndProc:
//|	Dialog proc prompts for connection info, writes values to global
//|	variables szDSN, szUID, and szPWD.
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
BOOL EXTFUN GetInstallerInfoWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static lpINSTALLER lpInst=NULL;

	switch(msg) {
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
			{
			HWND			hCntrl=NULL;
			DWORD			dwDex=0;
			LPTSTR		lpInstallDriverOpt=TEXT("ODBC_INSTALL_DRIVER"),
							lpRemoveDriverOpt=TEXT("ODBC_REMOVE_DRIVER");

			CenterDialog(hDlg);

			lpInst=(lpINSTALLER)lParam;

			CheckDlgButton(hDlg, IDCK_CD_HWNDPARENT, TRUE);

			// Set limits
			SendDlgItemMessage(hDlg, IDCB_CD_SZDRIVER, EM_LIMITTEXT, DFT_TEXT_LEN, 0L);
			SendDlgItemMessage(hDlg, IDCB_CD_SZARGS, EM_LIMITTEXT, DFT_TEXT_LEN, 0L);
			SendDlgItemMessage(hDlg, IDE_CD_CBMSGMAX, EM_LIMITTEXT, DFT_TEXT_LEN, 0L);
			SetDlgItemText(hDlg, IDCB_CD_SZDRIVER, szNULL);
			SetDlgItemText(hDlg, IDCB_CD_SZARGS, szNULL);
			SetDlgItemText(hDlg, IDE_CD_CBMSGMAX, szNULL);

			dwDex=(DWORD)SendDlgItemMessage(hDlg, IDCB_CD_FREQUEST, CB_ADDSTRING, (WPARAM)0L, (LPARAM)lpInstallDriverOpt);
			SendDlgItemMessage(hDlg, IDCB_CD_FREQUEST, CB_SETCURSEL, (WPARAM)dwDex, 0L);

			dwDex=(DWORD)SendDlgItemMessage(hDlg, IDCB_CD_FREQUEST, CB_ADDSTRING, (WPARAM)0L, (LPARAM)lpRemoveDriverOpt);
			SendDlgItemMessage(hDlg, IDCB_CD_FREQUEST, CB_SETCURSEL, (WPARAM)dwDex, 0L);


			}

			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				case IDOK:
					{
					// Retrieve values
					WPARAM		dex=0;
					HWND			hwnd=GetDlgItem(hDlg, IDCB_CD_FREQUEST);

					if(CB_ERR == (dex = (WPARAM)SendMessage(hwnd, CB_GETCURSEL, 0, 0L)))
						GetDlgItemText(hDlg, IDC_DATA_SOURCE,
									lpInst->szDriver,
									tsizeof(lpInst->szDriver));
					else
						SendMessage(hwnd, CB_GETLBTEXT, dex,
									(LPARAM)(LPTSTR)lpInst->szDriver);

					GetDlgItemText(hDlg, IDCB_CD_SZARGS, lpInst->szArgs,
									tsizeof(lpInst->szArgs));

					lpInst->fRequest=(WORD)IsDlgButtonChecked(hDlg, IDCK_CD_HWNDPARENT);

					EndDialog(hDlg, IDOK);
					}

					return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}

			if(GET_WM_COMMAND_CODE(wParam, lParam) ==	CBN_DBLCLK)
				SendMessage(hDlg, WM_COMMAND, IDOK, 0L);

			return FALSE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}

	return FALSE;
}



//*---------------------------------------------------------------------------------
//| lpSQLInstallDriverEx:
//|	This function calls SQLInstallDriverEx
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLInstallDriverEx(STD_FH_PARMS)
{
	BOOL						fSuccess=FALSE,
								fKeyExists=TRUE;
	DWORD						dwBeforeUsageCount=0,
								dwAfterUsageCount=0;
	HWND						hwndOut = GETOUTPUTWINDOW(ci);
	LPTSTR					lpstr=NULL;
	LPTSTR					lpToken=NULL;
	TCHAR						szRegKey[LARGEBUFF]=TEXT("");
	WORD						cbPathOutMax=*(WORD *)lpParms[3]->lpData;
	WORD						fRequest=*(WORD *)lpParms[5]->lpData;

	if (!lpParms[0]->fNull)
	{
		if (lpstr=DoubleNullTerm((LPTSTR)lpParms[0]->lpData))
			BuildDriverRegKey(szRegKey,lpstr);
	}

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 									// lpszPathOut
						(*(WORD *)lpParms[3]->lpData) * sizeof(TCHAR),				//	  based on cbPathOutMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	dwBeforeUsageCount=CheckUsageCount((LPCTSTR)szRegKey);

	fSuccess=SQLInstallDriverEx((LPTSTR)lpstr ? lpstr : lpParms[0]->lpData,				//lpszDriver
									lpParms[1]->fNull ? NULL :  lpParms[1]->lpData,				//lpsPathIn
									lpParms[2]->fNull ? NULL :  (LPTSTR)lpParms[2]->lpData,	//lpszPathOut
									cbPathOutMax,
									lpParms[4]->fNull ? NULL :  (WORD *)lpParms[4]->lpData,	//pcbPathOut
									fRequest,
									lpParms[6]->fNull ? NULL :  (DWORD *)lpParms[6]->lpData);//lpdwUsageCount

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	fKeyExists=ValidateRegistryKey(szRegKey);

	dwAfterUsageCount=CheckUsageCount((LPCTSTR)szRegKey);

	CheckInstallStatus(hwndOut,fSuccess,dwAfterUsageCount,
		dwBeforeUsageCount,szRegKey,fKeyExists,idsSQLInstallDriver);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[1], lpParms[4], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	// Do not Free if lpstr is the same as lpData.
	if(lpstr != lpParms[0]->lpData)
		FREE_PTR(lpstr);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);

} //lpSQLInstallDriverEx()


//*---------------------------------------------------------------------------------
//| lpSQLPostInstallerError:
//|	This function calls SQLPostInstallerError
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLPostInstallerError(STD_FH_PARMS)
{
	RETCODE	rc=SQL_SUCCESS;
	DWORD		fErrorCode	 =*(DWORD *)lpParms[0]->lpData;
	LPTSTR	lpszErrorMsg =(LPTSTR)lpParms[1]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	rc=SQLPostInstallerError(fErrorCode,lpParms[1]->fNull ? NULL : lpszErrorMsg);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	return(rc);

} //lpSQLPostInstallerError()



//*---------------------------------------------------------------------------------
//| lpSQLReadFileDSN:
//|	This function calls SQLReadFileDSN
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLReadFileDSN(STD_FH_PARMS)
{
	BOOL		fSuccess=SQL_TRUE;
	HWND		hwndOut = GETOUTPUTWINDOW(ci);
	LPCTSTR	lpszFileName=(LPCTSTR)lpParms[0]->lpData;
	LPCTSTR	lpszAppName =(LPCTSTR)lpParms[1]->lpData;
	LPCTSTR	lpszKeyName =(LPCTSTR)lpParms[2]->lpData;
	WORD		cbString		=*(WORD *)lpParms[4]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 									// lpszString
						(*(WORD *)LPPARAMDATA(lpParms, 4))* sizeof(TCHAR), 		//	  based on cbString
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=SQLReadFileDSN(lpParms[0]->fNull ? NULL : lpszFileName,
							lpParms[1]->fNull ? NULL : lpszAppName,
							lpParms[2]->fNull ? NULL : lpszKeyName,
							lpParms[3]->fNull ? NULL : (LPTSTR)lpParms[3]->lpData, //szString
							cbString,
							lpParms[5]->fNull ? NULL : (WORD *)lpParms[5]->lpData);	//pcbString

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);

} //lpSQLReadFileDSN()

//*---------------------------------------------------------------------------------
//| lpSQLWriteFileDSN:
//|	This function calls SQLWriteFileDSN
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLWriteFileDSN(STD_FH_PARMS)
{
	BOOL		fSuccess=SQL_TRUE;
	HWND		hwndOut = GETOUTPUTWINDOW(ci);
	LPCTSTR	lpszFileName=(LPCTSTR)lpParms[0]->lpData;
	LPCTSTR	lpszAppName =(LPCTSTR)lpParms[1]->lpData;
	LPCTSTR	lpszKeyName =(LPCTSTR)lpParms[2]->lpData;
	LPCTSTR	lpszString =(LPCTSTR)lpParms[3]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=SQLWriteFileDSN(	lpParms[0]->fNull ? NULL : lpszFileName,
								lpParms[1]->fNull ? NULL : lpszAppName,
								lpParms[2]->fNull ? NULL : lpszKeyName,
								lpParms[3]->fNull ? NULL : lpszString);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);

} //lpSQLWriteFileDSN()


//*---------------------------------------------------------------------------------
//| lpSQLGetConfigMode:
//|	This function calls SQLGetConfigMode
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetConfigMode(STD_FH_PARMS)
{
	BOOL		fSuccess=SQL_TRUE;
	HWND		hwndOut = GETOUTPUTWINDOW(ci);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=SQLGetConfigMode(lpParms[0]->fNull ? NULL : (UWORD *)lpParms[0]->lpData);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);

} //lpSQLGetConfigMode()

//*---------------------------------------------------------------------------------
//| lpSQLSetConfigMode:
//|	This function calls SQLGetConfigMode
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetConfigMode(STD_FH_PARMS)
{
	BOOL		fSuccess=SQL_TRUE;
	HWND		hwndOut = GETOUTPUTWINDOW(ci);
	UWORD		wConfigMode=*(UWORD *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	fSuccess=SQLSetConfigMode(wConfigMode);

	// Log return code information
	LOGINSTRETURNCODE(NULL, ci, fSuccess);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, TRUE);

	return(fSuccess ? SQL_SUCCESS : SQL_ERROR);

} //lpSQLSetConfigMode()

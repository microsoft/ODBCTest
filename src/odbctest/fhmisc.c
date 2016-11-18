//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHMISC.C
//|
//| Purpose:
//|		This module contains the function handlers for the Misc menu.
//|		Note that each function can be found in the order that it appears
//|		on the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "resource.h"
#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define NUM_ODBC2_ALL_FUNCTIONS		100


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern lpDFTARRAY					lpdftGetFunct;
extern UINT							ciGetFunct;
extern lpDFTARRAY					lpdftTrueFalse;
extern lpDFTARRAY					lpdftSQLTrueFalse;
extern UINT							ciTrueFalse;

dCSEG(TCHAR)	szGetFuncLogging[]				= TEXT("\t\t\t\tOut:\t\t\t%-60s%s\r\n");
dCSEG(TCHAR)	szGetFuncDataLogging[]			= TEXT("\t\t\t\t\t\t\t\t\t\t%-60s%s\r\n");
dCSEG(TCHAR)	szGetFuncTitle[]					= TEXT("\r\nGet Functions All:\r\n");
dCSEG(TCHAR)	szGetFuncName[]					= TEXT("\t\t\t\t%-60s");
dCSEG(TCHAR)	szGetFuncData[]					= TEXT("%-s\r\n");
dCSEG(TCHAR)	szGetFuncLabel[]					= TEXT("\t\t\t\t%-60s%-s\r\n");
dCSEG(TCHAR) szGetFuncError[]						= TEXT("\t%s returned %s \r\n");

//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN GetUpdatedStatusFlags(lpCONNECTIONINFO ci);          // From FHCONN.C




//*---------------------------------------------------------------------------------
//| GetAllFunctions:
//| Parms:
//|	pfExists				Does function Exist
//|	fFunction			ODBC API
//| Returns:
//|	TRUE					if we need to get all functions
//|	FALSE					if we don't
//*---------------------------------------------------------------------------------

_inline BOOL INTFUN GetAllFunctions(UWORD *pfExists, UWORD fFunction)
{
	if (pfExists)
	{
		if ((fFunction == SQL_API_ALL_FUNCTIONS) ||
			 (fFunction == SQL_API_ODBC3_ALL_FUNCTIONS))
			 return TRUE;
	}

	return(FALSE);

} //GetAllFunctions()




//*---------------------------------------------------------------------------------
//| lpSQLTransact:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLTransact(STD_FH_PARMS)
{
	RETCODE					rc;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	HDBC						hdbc=NULL;
	HENV						henv=NULL;

	if (lpParms[0]->lpData)
		henv=*(HENV *)lpParms[0]->lpData;

	if (lpParms[1]->lpData)
		hdbc=*(HDBC *)lpParms[1]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLTransact(henv,hdbc,
					  *(UWORD *)lpParms[2]->lpData);				// fCompletionType

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,
					cParms, ci, FALSE);

   //  Check for errors
	if (henv)
	{
		AUTOLOGERROREI(ci,lpActiveEnv,rc,henv);
	}
	else if (hdbc)
	{
		AUTOLOGERRORCI(ci,rc,hdbc);
	}

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLEndTran:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLEndTran(STD_FH_PARMS)
{
	SQLRETURN				rc;
	SQLHENV					henv=NULL;
	SQLHDBC					hdbc=NULL;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	UWORD 					fHandleType = *(UWORD *)(lpParms[IPAR_ET_FHANDLETYPE]->lpData),
								fCompletionType=*(UWORD *)(lpParms[IPAR_ET_FCOMPLETIONTYPE]->lpData);
	SQLHANDLE				hHandle=NULL;

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[IPAR_ET_HHANDLE]->lpData)
		hHandle=*(SQLHANDLE *)lpParms[IPAR_ET_HHANDLE]->lpData;

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc= SQLEndTran(fHandleType,
						hHandle,
						fCompletionType);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

   //  Check for errors
	if ((fHandleType == SQL_HANDLE_ENV) ||
		 (fHandleType == SQL_HANDLE_SENV))
	{
		AUTOLOGERROREI(ci,lpActiveEnv,rc,hHandle);
	}
	else
	{
		AUTOLOGERRORCI(ci,rc,hHandle);
	}

	return rc;

} //lpSQLEndTran()



//*---------------------------------------------------------------------------------
//| lpSQLCancel:
//|	If successful, any statements pending on the HSTMT must be cleared from
//|	the list of auto-executing statements.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLCancel(STD_FH_PARMS)
{
	RETCODE				rc;
	HSTMT					hstmt=NULL;
	lpSTATEMENTINFO	lpStmt=NULL;


	if (ci && ci->lpCurStmt)
	{
		lpStmt=ci->lpCurStmt;
		hstmt=lpStmt->hstmt;
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLCancel((lpParms[0]->fNull) ? NULL : hstmt);		// hstmt

	// Log return code information
	if(!(uAttrib & FH_NO_LOG))
		LogReturnCode(NULL, ci, rc);

	// If successful and the previous function did not return
	//SQL_STILL_EXECUTING, then we need to destroy old results set
	if (RC_SUCCESSFUL(rc) && (lpStmt && !lpStmt->fStillExecuting))
	{
		if(lpStmt->lpResults)
		{
			DestroyResultWindow((lpRESULTWININFO)lpStmt->lpResults,
				TRUE);
			lpStmt->lpResults = NULL;
		}
	}

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLError:
//|	Invoke the SQLError function for the given handles.  Note that no automatic
//|	logging of error will take place since this function does not return errors.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLError(STD_FH_PARMS)
{
	RETCODE					rc=SQL_SUCCESS;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	HENV						henv=lpActiveEnv ? lpActiveEnv->henv:NULL;
	UDWORD					cbSQLState=6*sizeof(TCHAR);

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 							// szSqlState
						cbSQLState,								// 5 for state + 1 for NULL
						lpUsrOptions->fBufferChecking);

	BUILDOUTPUTPARM(lpParms[5], 								// szErrorMsg
						(*(SWORD *)lpParms[6]->lpData) * sizeof(TCHAR), 		//	  based on cbErrorMsgMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLError((lpParms[0]->fNull) ? SQL_NULL_HENV : henv,		// henv
					  (lpParms[1]->fNull) ? SQL_NULL_HDBC : ci->hdbc,	// hdbc
					  (lpParms[2]->fNull) ? SQL_NULL_HSTMT : 				// hstmt
					  							ci->lpCurStmt->hstmt,
					  (TCHAR *)lpParms[3]->lpData,						// szSqlState
					  (SDWORD *)lpParms[4]->lpData,						// pfNativeError
					  (TCHAR *)lpParms[5]->lpData,						// szErrorMsg
					  *(SWORD *)lpParms[6]->lpData,						// cbErrorMsgMax
					  (SWORD *)lpParms[7]->lpData);						// pcbErrorMsg

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[5], lpParms[7], TRUE);

  	// Check szSqlState for null termination
	CHECKNULLTERM(ci, lpParms[3], NULL, cbSQLState);

  	// Check szErrorMsg for null termination
	CHECKNULLTERM(ci, lpParms[5], lpParms[7], TRUE);

	// Buffer modification checking
	if(rc != SQL_NO_DATA_FOUND)
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	TRUNCATIONCHECK(lpParms[5]->fNull, lpParms[7]->fNull, rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[5]->lpData, 									// szErrorMsg
				*(SWORD *)lpParms[6]->lpData, 						// cbErrorMsgMax
				*(SWORD *)lpParms[7]->lpData, 						// *pcbErrorMsg
				(LPTSTR)lpParms[7]->szName);										// "pcbErrorMsg"

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLGetFunctions:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetFunctions(STD_FH_PARMS)
{
	RETCODE		rc=SQL_SUCCESS;
	UWORD			uFunction=*(UWORD *)lpParms[1]->lpData;
	UWORD			*pfExists=NULL;
	HDBC			hdbc=NULL;
	BOOL			f2Funcs=(uFunction == SQL_API_ALL_FUNCTIONS),
					f3Funcs=(uFunction == SQL_API_ODBC3_ALL_FUNCTIONS),
					fSupported=SQL_FALSE;
	SWORD			uArySize=0;
	SWORD			uNumFuncs=1,
					iFunc,
					nApiNum=0,
					nApiIdx=0;
	TCHAR			szTemp[MEDBUFF]=TEXT(""),
					szSupported[XLARGEBUFF]=TEXT(""),
					szAPI[MEDBUFF]=TEXT(""),
					szLabel[MEDBUFF]=TEXT("");
	HWND			hwndOut = (ci) ? ci->hwndOut : hwndOutWin;

	if (f2Funcs)
		uNumFuncs=NUM_ODBC2_ALL_FUNCTIONS;
	else if (f3Funcs)
		uNumFuncs=SQL_API_ODBC3_ALL_FUNCTIONS;

	//set array size
	uArySize=uNumFuncs*sizeof(UWORD);

	pfExists=(UWORD *)AllocateMemory(uArySize);

	ASSERTCHECK(pfExists, TEXT("lpSQLGetFunctions(pfExist) AllocateMemory failed"));

	if (!pfExists)
		return(SQL_ERROR);

	//Untouch the buffer
	InitUnModified(pfExists,uArySize);

	if (ci && !lpParms[0]->fNull)
		hdbc=ci->hdbc;

	if (!lpUsrOptions->fODBC30)
		lstrcpy(lpParms[2]->szName,TEXT("pfExists"));

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetFunctions(hdbc,
								uFunction,
								lpParms[2]->fNull ? NULL : pfExists);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	if (!lpParms[2]->fNull)
	{

		if (!GetAllFunctions(pfExists,uFunction))
		{
			//update lpData so that Gator can log it correctly
			if (!lpParms[2]->fNull && lpParms[2]->lpData)
				*(UWORD *)lpParms[2]->lpData=*pfExists;

			LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

			// Null termination and buffer modification checking
			OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);
		}
		else
		{
			if (RC_SUCCESSFUL(rc) && pfExists)
			{
				if (f3Funcs)
					lstrcpy(szLabel,TEXT("SQL_FUNC_EXISTS(SupportedPtr,API Constant)"));
				else
					lstrcpy(szLabel,TEXT("pfExists[API Constant]"));

				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						(LPTSTR)szGetFuncLogging,TEXT("Function"),szLabel);

				//Loop through all functions and output the results
 				for (iFunc=0;iFunc < NUM_APIS;iFunc++)
				{
					//Get API Number if it exists
					nApiNum=(SWORD)GetApiNum(iFunc);

					if (nApiNum > 0)
					{
						if (f2Funcs && IsODBC30Api(nApiNum))
							continue;
						else
						{
							//valid API for the uFunction arg
							lstrcpy(szAPI,rgApiList[iFunc].szAPIName);

							fSupported=(f3Funcs ? SQL_FUNC_EXISTS(pfExists,nApiNum) : pfExists[nApiNum]);

							wsprintf(szTemp,TEXT("%s = %d"),szAPI,nApiNum);
						}

					}
					else
						continue;

					if (fSupported == SQL_TRUE)
						lstrcpy(szSupported,lpdftSQLTrueFalse[SQL_TRUE].szOpt);
					else if (fSupported == SQL_FALSE)
						lstrcpy(szSupported,lpdftSQLTrueFalse[SQL_FALSE].szOpt);
					else
						_itot(fSupported,szSupported,10);

					// Now we start printing information
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
								(LPTSTR)szGetFuncDataLogging,szTemp,szSupported);

					//if we are in 2.X mode verify that the array entry for this API
					//is SQL_FALSE, if it isn't then display a TST message
					if (f2Funcs && IsODBC30Api(nApiNum) && (fSupported == SQL_TRUE))
					{
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
										GetGtrString(szOutBuff, MAXOUTBUFF, TST1023),
										rgApiList[iFunc].szAPIName,rgApiList[iFunc].szAPIName);
					}

				} // for loop
			}
		}
	}
	else
		LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	//  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	FREE_PTR(pfExists);

	return rc;
}


//*---------------------------------------------------------------------------------
//| MiscGetFunctionsAll:
//|	This function will enumerate all functions to the given output window.
//| Parms:
//|	ci					Connection info record
//|	uFunctionID		SQLGetFunctions' uFunctionId Parameter
//| Returns:
//|	RETCODE
//*---------------------------------------------------------------------------------
RETCODE INTFUN MiscGetFunctionsAll(lpCONNECTIONINFO ci,UWORD uFunctionId)
{
	HDBC		hdbc = NULL;
	RETCODE	rc=SQL_SUCCESS;
	HWND		hwndOut = (ci) ? ci->hwndOut : hwndOutWin;
	TCHAR		szTemp[MEDBUFF]=TEXT(""),
				szBuff[XLARGEBUFF]=TEXT(""),
				szLabel[MEDBUFF]=TEXT("");
	UWORD		iFunc;
	UWORD		nApiNum=0;
	SDWORD		dwDex=0;
	BOOL		fExists=SQL_FALSE,
				fPrint30=SQL_FALSE;
	UWORD		uwSupported=FALSE;

	if (uFunctionId == SQL_API_ODBC3_ALL_FUNCTIONS)
		fPrint30=TRUE;
	else if (uFunctionId == SQL_API_ALL_FUNCTIONS)
		fPrint30=FALSE;
	else if (uFunctionId == CALLED_FROM_MENU)
	{
		//This function was called from the menu
		fPrint30=lpUsrOptions->fODBC30;
	}

	fBusy(TRUE);

	if(!ci)
		return(SQL_ERROR);

	hdbc = ci->hdbc;

	if (uFunctionId == CALLED_FROM_MENU)
	{
		// Print the header since this was called from the menu
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,(LPTSTR)szGetFuncTitle);
	}

	if (lpUsrOptions->fODBC30)
		lstrcpy(szLabel,TEXT("*SupportedPtr"));
	else
		lstrcpy(szLabel,TEXT("*pfExists"));

	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
				(LPTSTR)szGetFuncLabel, TEXT("Function"), szLabel);

	//Loop through all functions and output the results
 	for (iFunc=0;iFunc < NUM_APIS;iFunc++)
	{
		dwDex = GetApiNum(iFunc);
		if ( dwDex == -1 )
			continue ;
		else
			nApiNum = (UWORD )dwDex;
		//display function name and constant value
		wsprintf(szTemp,TEXT("%s = %d"),rgApiList[iFunc].szAPIName,nApiNum);
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					(LPTSTR)szGetFuncName,szTemp);

		if (RC_SUCCESSFUL(rc=SQLGetFunctions(hdbc,nApiNum,&uwSupported)))
		{

			if (nApiNum >= 0)
			{
				//if we are in 2.X mode don't display a 3.0 API
				if (!fPrint30 && IsODBC30Api(nApiNum))
					continue;

				// Now we start printing information
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
							(LPTSTR)szGetFuncData,
							lpdftSQLTrueFalse[(uwSupported ? 1 : 0)].szOpt);
			}
		}
		else
		{
			//send CRLF
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						TEXT("\r\n\t\t"));

			ToolOptionsAllErrors(ci, NULL, hdbc, NULL,FALSE,rc);
		}

		if(fAbortCheck(hwndOut))
			goto done;

	}

done:
	// Now clear any errors that might still be in the error que
	while(SQLError(NULL, ci->hdbc, NULL, szTemp, NULL, szBuff, MAXOUTBUFF, NULL) != SQL_NO_DATA_FOUND);

	fBusy(FALSE);

	return(rc);
}

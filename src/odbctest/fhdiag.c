//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHSTMT.C
//|
//| Purpose:
//|		This module contains the function handlers for the Statement menu.
//|		Note that each function can be found in the order that it appears
//|		on the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define MAX_TIMESTAMP_CHAR_LEN		47			// Size of the character buffer required for the largest timestamp value
#define DEX_C_DEFAULT					8			// Index of SQL_C_DEFAULT in the BindParameter fCType combo box
#define DEX_DFT_ROWSET_VALUE			1			// Index of <rowset value> in the BindParameter rgbValue & pcbValue combo boxes



//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR							szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options

extern lpDFTARRAY					lpdftParamType;
extern lpDFTARRAY					lpdftCTypes;
extern lpDFTARRAY					lpdftSqlTypes;
extern lpDFTARRAY					lpdftNullable;
extern UINT							ciCTypes;
extern UINT 						ciSqlTypes;
extern UINT							ciNullable;



//*---------------------------------------------------------------------------------
//| lpSQLGetDiagField:
//|	Invoke the SQLGetDiagField function for the given handles.  Note that no automatic
//|	logging of error will take place since this function does not return errors.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetDiagField(STD_FH_PARMS)
{
	SQLRETURN		rc=SQL_SUCCESS;

	SWORD				fHandleType=*(SWORD *)lpParms[IPAR_GDF_FHANDLETYPE]->lpData;
	SQLHANDLE		hHandle=NULL;
	SWORD				irecRecNumber=*(SWORD *)lpParms[IPAR_GDF_IRECRECNUMBER]->lpData;
	SWORD				fDiagIdentifier =*(SWORD *)lpParms[IPAR_GDF_FDIAGIDENTIFIER]->lpData;
	SWORD				cbBufferLength=*(SWORD *)lpParms[IPAR_GDF_CBBUFFERLENGTH]->lpData;

	if (lpParms[IPAR_GDF_HHANDLE]->lpData)
		hHandle=*(SQLHANDLE *)lpParms[IPAR_GDF_HHANDLE]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[IPAR_GDF_RGBDIAGINFO], 											// szErrorMsg
						(*(SWORD *)lpParms[IPAR_GDF_CBBUFFERLENGTH]->lpData) * sizeof(TCHAR), 				//	  based on cbBufferLength
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc=SQLGetDiagField(fHandleType,
					hHandle,
					irecRecNumber,
					fDiagIdentifier,
					lpParms[IPAR_GDF_RGBDIAGINFO]->fNull ? NULL : 	lpParms[IPAR_GDF_RGBDIAGINFO]->lpData,
					cbBufferLength,
					lpParms[IPAR_GDF_PCBSTRINGLENGTH]->fNull ? NULL : lpParms[IPAR_GDF_PCBSTRINGLENGTH]->lpData);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[IPAR_GDF_RGBDIAGINFO], lpParms[IPAR_GDF_PCBSTRINGLENGTH], TRUE);

	// Null termination and buffer modification checking
	if ((rc != SQL_NO_DATA_FOUND) &&
		IsCharData(lpParms[IPAR_GDF_RGBDIAGINFO]->fCType))
	{
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);
		CHECKNULLTERM(ci, lpParms[IPAR_GDF_RGBDIAGINFO], lpParms[IPAR_GDF_PCBSTRINGLENGTH], TRUE);
		// Verify any truncation which may have occured
		TRUNCATIONCHECK(lpParms[IPAR_GDF_RGBDIAGINFO]->fNull, lpParms[IPAR_GDF_PCBSTRINGLENGTH]->fNull, rc, GETOUTPUTWINDOW(ci),
					(LPTSTR)lpParms[IPAR_GDF_RGBDIAGINFO]->lpData, 					// rgbDiagInfo
					*(SWORD *)lpParms[IPAR_GDF_CBBUFFERLENGTH]->lpData, 				// cbBufferLength
					*(SWORD *)lpParms[IPAR_GDF_PCBSTRINGLENGTH]->lpData, 				// *pcbStringLength
					(LPTSTR)lpParms[IPAR_GDF_PCBSTRINGLENGTH]->szName);					// "pcbStringLength"
	}

	return rc;

} //lpSQLGetDiagField()


//*---------------------------------------------------------------------------------
//| lpSQLGetDiagRec:
//|	Invoke the SQLError function for the given handles.  Note that no automatic
//|	logging of error will take place since this function does not return errors.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

SQLRETURN INTFUN lpSQLGetDiagRec(STD_FH_PARMS)
{
	SQLRETURN		rc=SQL_SUCCESS;
	SQLHANDLE		hHandle=NULL;
	SWORD				fHandleType=*(SWORD *)lpParms[IPAR_GDR_FHANDLETYPE]->lpData;
	SWORD				irecRecNumber=*(SWORD *) lpParms[IPAR_GDR_IRECRECNUMBER]->lpData;
	SDWORD			*pfNativeError=(SDWORD *)lpParms[IPAR_GDR_PFNATIVEERROR]->lpData;
	SWORD				cbBufferLength=*(SWORD *)lpParms[IPAR_GDR_CBBUFFERLENGTH]->lpData;
	SWORD				*pcbTextLength=(SWORD *) lpParms[IPAR_GDR_PCBTEXTLENGTH]->lpData;

	if (lpParms[IPAR_GDR_HHANDLE]->lpData)
		hHandle=*(SQLHANDLE *)lpParms[IPAR_GDR_HHANDLE]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[IPAR_GDR_SZSQLSTATE], 	// szSQLState
						(6*sizeof(TCHAR)),					//2 chrs for Class, 3 for subclass
						lpUsrOptions->fBufferChecking);

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[IPAR_GDR_SZMESSAGETEXT],// szMessageText
						cbBufferLength * sizeof(TCHAR),						//		based on cbBufferLength
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc=SQLGetDiagRec(fHandleType,
						hHandle,
						irecRecNumber,
						lpParms[IPAR_GDR_SZSQLSTATE]->fNull ? NULL : lpParms[IPAR_GDR_SZSQLSTATE]->lpData,
						lpParms[IPAR_GDR_PFNATIVEERROR]->fNull? NULL : pfNativeError,
						lpParms[IPAR_GDR_SZMESSAGETEXT]->fNull ? NULL : lpParms[IPAR_GDR_SZMESSAGETEXT]->lpData,
						cbBufferLength,
						lpParms[IPAR_GDR_PCBTEXTLENGTH]->fNull ? NULL : pcbTextLength);


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[IPAR_GDR_SZMESSAGETEXT], lpParms[IPAR_GDR_PCBTEXTLENGTH], TRUE);

	// Null termination and buffer modification checking
	if (rc != SQL_NO_DATA_FOUND)
	{
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

		CHECKNULLTERM(ci, lpParms[IPAR_GDR_SZMESSAGETEXT], lpParms[IPAR_GDR_PCBTEXTLENGTH], TRUE);

		// Verify any truncation which may have occured
		TRUNCATIONCHECK(lpParms[IPAR_GDR_SZMESSAGETEXT]->fNull, lpParms[IPAR_GDR_PCBTEXTLENGTH]->fNull, rc, GETOUTPUTWINDOW(ci),
					(LPTSTR)lpParms[IPAR_GDR_SZMESSAGETEXT]->lpData, 					// szMessageText
					*(SWORD *)lpParms[IPAR_GDR_CBBUFFERLENGTH]->lpData, 					// cbBufferLength
					*(SWORD *)lpParms[IPAR_GDR_PCBTEXTLENGTH]->lpData, 					// *pcbTextLength
					(LPTSTR)lpParms[IPAR_GDR_PCBTEXTLENGTH]->szName);							// "pcbTextLength"
	}

	return rc;

} //lpSQLGetDiagRec()

//*---------------------------------------------------------------------------------
//| DisplayDiagnostics:
//|	This function retrieves and display all errors in the error queue
//| Parms:
//|	rcAct					Actual RC
//|	fHandleType			Handle type
//|	hHandle				Actual handle
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID INTFUN DisplayDiagnostics(RETCODE rcAct,SWORD fHandleType,SQLHANDLE hHandle,
										 LPTSTR szAPI)
{
	SQLRETURN		rc=SQL_SUCCESS;
	SDWORD			fNativeError=0;
	SWORD				cbBufferLength=300;
	SWORD				cbTextLength=0;
	TCHAR				szMessageText[LARGEBUFF]=TEXT("");
	SWORD				irecRecNumber;
	TCHAR				szRtn[SMALLBUFF]=TEXT(""),
						szSQLState[TINYBUFF]=TEXT(""),
						szMsg[LARGEBUFF]=TEXT(""),
						szBuff[MEDBUFF]=TEXT("");


	// Retrieve all errors
	for (irecRecNumber=1;RC_SUCCESSFUL(rc);irecRecNumber++)
	{
		rc=SQLGetDiagRec(fHandleType,
							hHandle,
							irecRecNumber,
							szSQLState,
							&fNativeError,
							szMessageText,
							tsizeof(szMessageText),
							&cbTextLength);

		if (RC_SUCCESSFUL(rc))
		{
			lstrcpy(szBuff,szAPI);
			//Get RC in text form
			wsprintf(szMsg,TEXT("%s: SQLState: %s Message: %s"),
				GetRetCodeName(rcAct, szRtn),	szSQLState,szMessageText);
		}
		else
		{
		//	lstrcpy(szBuff,TEXT("SQLGetDiagRec"));
			lstrcpy(szMsg,TEXT("No Descriptor information available!"));
		}

		//Display Error
		szMessageBox(GetActiveWindow(), MB_ICONEXCLAMATION | MB_OK,
			 szBuff, szMsg);


	} //end for



} //DisplayDiagnostics()


//*---------------------------------------------------------------------------------
//| ClearErrorQueue:
//|	This function attempts to clear the specified (hhandleType) error queue
//| Parms:
//|	fHandleType				Handle Type
//|	hHandle					handle
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN ClearErrorQueue(SQLSMALLINT fHandleType,SQLHANDLE hHandle)
{

	switch(fHandleType)
	{

		case	SQL_HANDLE_SENV:
		case	SQL_HANDLE_ENV:
			SQLGetEnvAttr(hHandle,SQL_ATTR_ODBC_VERSION,NULL,0,NULL);
			break;
		case	SQL_HANDLE_DBC:
			SQLGetInfo(hHandle,SQL_ACTIVE_CONNECTIONS,NULL,0,NULL);
			break;
		case	SQL_HANDLE_STMT:
			SQLGetStmtOption(hHandle,SQL_QUERY_TIMEOUT,NULL);
			break;
		case	SQL_HANDLE_DESC:
			SQLGetDescField(hHandle,0,SQL_DESC_ALLOC_TYPE,NULL,0,NULL);
			break;

	}

} //ClearErrorQueue()


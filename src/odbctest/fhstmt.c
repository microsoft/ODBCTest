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
extern TCHAR						szSQL_CONCUR_READ_ONLY[];
extern TCHAR						szSQL_CONCUR_LOCK[];
extern TCHAR						szSQL_CONCUR_ROWVER[];
extern TCHAR						szSQL_CONCUR_VALUES[];
extern TCHAR						szSQL_CURSOR_FORWARD_ONLY[];
extern TCHAR						szSQL_CURSOR_KEYSET_DRIVEN[];
extern TCHAR						szSQL_CURSOR_DYNAMIC[];
extern TCHAR						szSQL_CURSOR_STATIC[];


dCSEG(TCHAR) szRow[]						= TEXT(" Row,");
dCSEG(TCHAR) szRowsetHeader[]			= TEXT(" Hdbc, Hstmt, Column, Row,");
dCSEG(TCHAR) szFourNA[]					= TEXT(" N/A, N/A, N/A, N/A,");
dCSEG(TCHAR) szROWWISE[]				= TEXT("row-wise");
dCSEG(TCHAR) szCOLWISE[]				= TEXT("column-wise");
dCSEG(TCHAR) szShowParamTitle[]		= TEXT("\r\nShow Parameters for hstmt#%d: \r\n");
dCSEG(TCHAR) szpirowNull[]				= TEXT("\t\t\t\tpirow = NULL \r\n");
dCSEG(TCHAR) szpirowValue[]			= TEXT("\t\t\t\t*pirow = %lu \r\n");
dCSEG(TCHAR) szShowParamSubtitle[]	= TEXT("\t\t\t\tipar,%s fParamType, fCTypes, fSqlType, cbColDef, ibScale, cbValueMax, *pcbValue, SQL_LEN_DATA_AT_EXEC,%s *rgbValue\r\n");
dCSEG(TCHAR) szrgbVal[]					= TEXT("rgbValue");
dCSEG(TCHAR) szRowsetTitle[]			= TEXT(" %u, %u, %lu, %lu,");
dCSEG(TCHAR) szShowParamData[]		= TEXT("\t\t\t\t%d,%s %s, %s, %s, %lu, %d, %ld, %s, %s,%s %s \r\n");
dCSEG(TCHAR) szINVALID_ATTR[]			= TEXT("%d (An Invalid Value)");
dCSEG(TCHAR) szMEMALLOC_ERROR[]		= TEXT("Memory allocation error re-allocating to a smaller size.");

//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN GetUpdatedStatusFlags(lpCONNECTIONINFO ci);          	// From FHCONN.C
UDWORD INTFUN SumBoundCols(lpSTATEMENTINFO lpStmt);					// From FHRSLT.C
VOID INTFUN CheckRowsetCache(lpCONNECTIONINFO lpci);




//*---------------------------------------------------------------------------------
//| DoAllocStmt:
//|	This function should be called after an HSTMT has been successfully allocated.
//|	It will add that hstmt to the combo-box list of hstmts and insert it into the
//|	linked list of statements.
//| Parms:
//|	ci							Current connection information (always NULL)
//|	hstmt						The successfully allocated hstmt
//| Returns:
//|	A pointer to the statement info structure if successful, else NULL
//*---------------------------------------------------------------------------------
lpSTATEMENTINFO INTFUN DoAllocStmt(lpCONNECTIONINFO ci, HSTMT hstmt)
{
	lpSTATEMENTINFO	lpState=NULL;
	lpENVIRONMENTINFO	lpEi=GetActiveEnvNode();
	SQLHDESC					hdesc=NULL;

	// Attempt to allocate a new linked list for the statement information
	lpState = (lpSTATEMENTINFO)AllocateMemory(sizeof(STATEMENTINFO));
	if(!lpState)
	{
		SQLFreeStmt(hstmt, SQL_DROP);
		szMessageBox(hwndClient,
				MB_ICONSTOP | MB_OK,
				(LPTSTR)TEXT("Error"),
				GetGtrString(szOutBuff, MAXOUTBUFF, idsFreeHandleDueToMemory));
		return NULL;
	}

	// Set information, then add to the linked list
	ci->uState |= STATE_ALLOCATED_HSTMT;
	lpState->hstmt = hstmt;

	if (lpEi)
		lpState->uStmtNum = ++lpEi->cCurStmtNum;

	if(ci->lpStmt)
	{
		// Insert at end of list
		ci->lpStmt->prev->next = lpState;
		lpState->prev = ci->lpStmt->prev;
		lpState->next = ci->lpStmt;
		ci->lpStmt->prev = lpState;
	}
	else
	{
		// Only one in the list
		ci->lpStmt = lpState;
		lpState->next = lpState;
		lpState->prev = lpState;
	}

	ci->lpCurStmt = lpState;
	lpState->lpCI=ci;

	++ci->cStmts;

	// Need to set default values for statement info (note allocated memory
	//		is init to zero, so only non zero fields need be set)
	lpState->fColBinding = TRUE;
	lpState->uCursorType = SQL_CURSOR_FORWARD_ONLY;
	lpState->uConcurType = SQL_CONCUR_READ_ONLY;
	lpState->uRowSetSize =
	lpState->uEFRowSetSize =
	lpState->uFSFRowSetSize = ci->uRowSetSize;
	lpState->uReservedRows = lpUsrOptions->uReservedRows;
	lpState->uCurrentRow = 1;						// Default current row is 1 based
	lpState->cParamRows = 1;						// Default number of rows, changed by calling SQLParamOptions
	lpState->fParamOpt = FALSE;					// Set to TRUE once SQLParamOptions is called
	lpState->fpirowNull = TRUE;					// Set to FALSE if call to SQLParamOptions has VALID pirow
	lpState->pirow = 0;								// clear the pirow used by SQLParamOptions
	lpState->fSetPos = FALSE;						// clear the SQL_NEED_DATA flag for SQLSetPos
	lpState->ParamDataCType = SQL_C_CHAR;		// Default C Type for SQLPutData calls without preceeding SQLParamData
	lpState->fAutoDisplay=lpUsrOptions->fAutoDisplay;

	// Finally add the new node to the mini-tool bar of hstmts
	AddNodeToToolBar(lpState->uStmtNum,(HANDLE)hstmt,ci->hwndStmts,szHSTMT);

	//Allocate and fill ARD
	hdesc=GetDescHandle(lpState,SQL_ATTR_APP_ROW_DESC);

	if (hdesc && (lpState->lpARD=DoAllocDescriptor(ci,hdesc,FALSE)))
	{
		lstrcpy(lpState->lpARD->szName,TEXT("Implicit ARD"));
		//sumit
		lpState->lpARD->uwAllocType = SQL_DESC_ALLOC_AUTO;
		lpState->lpARD->uwDescType=DESC_ARD;
		FillDescriptorInfo(ci,lpState->lpARD);
	}

	//Allocate and fill APD
	hdesc=NULL;
	hdesc=GetDescHandle(lpState,SQL_ATTR_APP_PARAM_DESC);

	if (hdesc && (lpState->lpAPD=DoAllocDescriptor(ci,hdesc,FALSE)))
	{
		lstrcpy(lpState->lpAPD->szName,TEXT("Implicit APD"));
		//sumit
		lpState->lpAPD->uwAllocType = SQL_DESC_ALLOC_AUTO;
		lpState->lpAPD->uwDescType=DESC_APD;
		FillDescriptorInfo(ci,lpState->lpAPD);
	}

	//Allocate and fill IRD
	hdesc=NULL;
	hdesc=GetDescHandle(lpState,SQL_ATTR_IMP_ROW_DESC);

	if (hdesc && (lpState->lpIRD=DoAllocDescriptor(ci,hdesc,FALSE)))
	{
		lstrcpy(lpState->lpIRD->szName,TEXT("Implicit IRD"));
		lpState->lpIRD->uwAllocType = SQL_DESC_ALLOC_AUTO;
		lpState->lpIRD->uwDescType=DESC_IRD;
		FillDescriptorInfo(ci,lpState->lpIRD);
	}

	//Allocate and fill IPD
	hdesc=NULL;
	hdesc=GetDescHandle(lpState,SQL_ATTR_IMP_PARAM_DESC);
	if (hdesc && (lpState->lpIPD=DoAllocDescriptor(ci,hdesc,FALSE)))
	{
		lstrcpy(lpState->lpIPD->szName,TEXT("Implicit IPD"));
		lpState->lpIPD->uwAllocType = SQL_DESC_ALLOC_AUTO;
		lpState->lpIPD->uwDescType=DESC_IPD;
		FillDescriptorInfo(ci,lpState->lpIPD);
	}

	return lpState;
}


//*---------------------------------------------------------------------------------
//| DoDropStmt:
//|	This function set states and free statement nodes after an hstmt has been
//|	successfully drop using SQLFreeStmt.
//| Parms:
//|	ci							Current connection information
//| Returns:
//|	A pointer to the statement info structure if successful, else NULL
//*---------------------------------------------------------------------------------
VOID INTFUN DoDropStmt(lpCONNECTIONINFO ci,lpSTATEMENTINFO lpState)
{
	DWORD		dwIndex=0;

	dwIndex=FindIndexInDropDown(ci->hwndStmts,lpState->hstmt,lpState->uStmtNum,szHSTMT);

	// Remove the hstmt from the combo box
	dwIndex=DropNodeFromToolBar(ci->hwndStmts,dwIndex);

	// Destroy the results window if present
	if(lpState->lpResults)
		DestroyResultWindow((lpRESULTWININFO)lpState->lpResults,TRUE);

	if(lpState->next != lpState)
	{
		// More than one node
		lpState->prev->next = lpState->next;
		lpState->next->prev = lpState->prev;

		// Removing head node?
		if(ci->lpStmt == lpState)
			ci->lpStmt = lpState->next;
	}
	else
	{
		// Removing only node
		ci->lpStmt = NULL;
	}

	if (ci->cStmts > 0)
		--ci->cStmts;

	// Free all bound columns and parameter nodes, then the statement info itself
	while(lpState->lpVars)
		FreeBoundColumn(lpState, lpState->lpVars->next);

	while(lpState->lpParams)
		FreeParameter(lpState, lpState->lpParams->next);

	FREE_PTR(lpState);

	// Now set the current selection
	SetCurrentHstmtFromList(ci);

	if (ci->lpCurStmt)
	{
		dwIndex=FindIndexInDropDown(ci->hwndStmts,ci->lpCurStmt->hstmt,
					ci->lpCurStmt->uStmtNum,szHSTMT);
	}
	else
		dwIndex=0;

	SendMessage(ci->hwndStmts, CB_SETCURSEL, (WPARAM)dwIndex, 0L);

	// Change state if there are no more allocated statements
	if(!ci->lpStmt)
		ci->uState &= ~STATE_ALLOCATED_HSTMT;

	// Refresh the toolbar so that some buttons get disabled correctly
	RefreshToolbar();
}


//*---------------------------------------------------------------------------------
//| DropAllStmts:
//|	This function will free all of the hstmts for a connection window.
//|	The caller determines if each hstmt must be dropped (won't be the
//|	case when SQLDisconnect is successful).
//| Parms:
//|	lpci						Connection window to free
//|	fNeedToFree				TRUE if we must try to free each hstmt
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN DropAllStmts(lpCONNECTIONINFO lpci, BOOL fNeedToFree)
{
	RETCODE 		rc;

	if(!lpci || !lpci->lpStmt)
		return TRUE;

	while(lpci->lpStmt)
	{
		SendMessage(lpci->hwndStmts, CB_SETCURSEL, (WPARAM)1, 0L);  	// Head node
		SetCurrentHstmtFromList(lpci);											//  is now current statement

		if(fNeedToFree)
		{
			SQLCancel(lpci->lpCurStmt->hstmt);

			//For ODBC 3.0 use SQLFreeHandle()
			if (lpUsrOptions->nODBCVer >= IDR_ODBCVER_30)
				rc = SQLFreeHandle(SQL_HANDLE_STMT,lpci->lpCurStmt->hstmt);
			else
				rc = SQLFreeStmt(lpci->lpCurStmt->hstmt, SQL_DROP);

			if(RC_NOTSUCCESSFUL(rc))
				return FALSE;
		}

		DoDropStmt(lpci,lpci->lpCurStmt);
	}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| lpSQLAllocStmt:
//|	The list of valid statement handles must be updated after successful
//|	completion of this function.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLAllocStmt(STD_FH_PARMS)
{
	RETCODE				rc;
	HSTMT					hstmt=NULL;
	HDBC					hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLAllocStmt(hdbc,
							(lpParms[1]->fNull) ? NULL : &hstmt);		// phstmt

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	// If successful, add the new hstmt to the combo box and to our linked list
	if(rc == SQL_SUCCESS && DoAllocStmt(ci, hstmt)) {
		return SQL_SUCCESS;
    }
	else {
		return SQL_ERROR;
    }
}


//*---------------------------------------------------------------------------------
//| lpSQLPrepare:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLPrepare(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLPrepare(hstmt,
						 (LPTSTR)lpParms[1]->lpData,							// szSQLStr
						 *(SDWORD *)lpParms[2]->lpData);            // cbSQLStr

	if (RC_SUCCESSFUL(rc))
		ci->lpStmt->wCurState=STMT_PREPARED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}






//*---------------------------------------------------------------------------------
//| lpSQLNumParams:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLNumParams(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLNumParams(hstmt,
						 (SWORD *)lpParms[1]->lpData);						// pcpar

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLDescribeParam:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLDescribeParam(STD_FH_PARMS)
{
	RETCODE		rc=SQL_SUCCESS;
	HWND			hwndOut = GETOUTPUTWINDOW(ci);
	HSTMT		hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLDescribeParam(hstmt,
						 *(UWORD *)lpParms[1]->lpData,							// ipar
						 (SWORD *)lpParms[2]->lpData,							// pfSqlType
						 (SQLULEN *)lpParms[3]->lpData,							// cbColDef
						 (SWORD *)lpParms[4]->lpData,							// pibScale
						 (SWORD *)lpParms[5]->lpData);							// pfNullable

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,
					cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

  	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLExecute:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLExecute(STD_FH_PARMS)
{
	RETCODE	rc;
	lpSTATEMENTINFO lpStmt=NULL;
	HSTMT		hstmt=NULL;

	if (ci)
		lpStmt=ci->lpCurStmt;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLExecute(hstmt);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Check for SQL_NEED_DATA.  If it is, then we need to clear the fSetPos flag in the StatementInfo structure
	if(rc == SQL_NEED_DATA && lpStmt)
		lpStmt->fSetPos = FALSE;

	// If successful, check for possible changes to rowset, for example if we
	//		just executed a positioned delete
	if(RC_SUCCESSFUL(rc))
	{
		lpStmt->wCurState=STMT_EXECUTED;
		CheckRowsetCache(ci);
	}

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLExecDirect:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLExecDirect(STD_FH_PARMS)
{
	RETCODE				rc;
	lpSTATEMENTINFO	lpStmt=NULL;
	HSTMT					hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	if (ci)
		lpStmt=ci->lpCurStmt;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	 rc = SQLExecDirect(hstmt,
						 (LPTSTR)lpParms[1]->lpData,							// szSQLStr
						 *(SDWORD *)lpParms[2]->lpData);           // cbSQLStr

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Check for SQL_NEED_DATA.  If it is, then we need to clear the fSetPos flag in the StatementInfo structure
	if(rc == SQL_NEED_DATA && lpStmt)
			lpStmt->fSetPos = FALSE;

	// If successful, check for possible changes to rowset, for example if we
	//		just executed a positioned delete
	if(RC_SUCCESSFUL(rc))
	{
		lpStmt->wCurState=STMT_EXECUTED;
		CheckRowsetCache(ci);
	}

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLSetCursorName:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetCursorName(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLSetCursorName(hstmt,
						 			(TCHAR *)lpParms[1]->lpData,            				// szCursor
						 			*(SWORD *)lpParms[2]->lpData);            			// cbCursor

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLGetCursorName:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetCursorName(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[1], 												// pvParam
						((SDWORD)*(SWORD *)lpParms[2]->lpData) * sizeof(TCHAR), 	//	  based on cbCursorMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetCursorName 	(hstmt,
						 			(TCHAR *)lpParms[1]->lpData,				// szCursor
						 			*(SWORD *)lpParms[2]->lpData,				// cbCursorMax
						 			(SWORD *)LPPARAMDATA(lpParms,3));				// pcbCursor

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[1], lpParms[3], TRUE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	if(!lpParms[1]->fNull){
		TRUNCATIONCHECK(lpParms[1]->fNull, lpParms[3]->fNull, rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[1]->lpData,
				(SDWORD)*(SWORD *)lpParms[2]->lpData,
				(SDWORD)*(SWORD *)lpParms[3]->lpData,
				(LPTSTR)lpParms[3]->szName);
		}

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLParamData:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLParamData(STD_FH_PARMS)
{
	RETCODE				rc;
	lpPARAMETERINFO	lpParam=NULL;
	lpSTATEMENTINFO	lpSi=NULL;
	lpBOUNDVARS			lpBV=NULL;
	UDWORD				idx;
	HWND					hwndOut;
	TCHAR					szTemp[180];
	UINT_PTR				rgbTemp;
	BOOL					fFound=FALSE;
	UINT_PTR				udRowPtr;
	HSTMT					hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	hwndOut = GETOUTPUTWINDOW(ci);

	if ((ci) && (ci->lpCurStmt))
		lpSi = ci->lpCurStmt;


	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[1], 							// prgbValue
						sizeof(UDWORD),						//	size of a pointer
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLParamData(hstmt,
						 (PTR *)lpParms[1]->lpData);            					// prgbValue

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	if(!(lpParms[1]->lpData))
	{
		LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);
	}
	else if(!(uAttrib & FH_NO_LOG))
		LogParametersSpecial(szFuncName, lpParms,
									cParms, ci, FALSE,
									PRM_32HDL, 0, 1);

	// Null termination and buffer modification checking
	// Do not do if rc=SQL_SUCCESS -- the buffer is untouched in this case
	if(rc != SQL_SUCCESS)
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Find the corresponding lpPARAMETERINFO structure and store it's pointer
	if (rc == SQL_NEED_DATA)
	{
		// If rgbValue is NULL, assume a C Type of SQL_C_CHAR
		if(!(lpParms[1]->lpData))
		{
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
					GetGtrString(szTemp, MAXOUTBUFF, TST1007));
			lpSi->ParamDataCType = SQL_C_CHAR;
		}
		else
		{
			// Check to see if the SQL_NEED_DATA came from SQLSetPos or (SQLExecute or SQLExecDirect)
			if(lpSi->fSetPos)
			{
				// The SQL_NEED_DATA came from a call to SQLSetPos
				rgbTemp = (UINT_PTR)(*(PTR *)lpParms[1]->lpData);

				// See if the bound memory here is column-wise bound or row-wise bound
				// It is column-wise binding
				if((lpSi->fColBinding) && (lpSi->lpVars))
				{
					// Search the linked list of bound vars, seeing if the rgbValue is in that bound memory
					lpBV = lpSi->lpVars;
					for(idx=0;idx<(UDWORD)lpSi->uBoundNodes;idx++)
					{
						if((rgbTemp >= (UINT_PTR)lpBV->rgbValue) &&
							(rgbTemp <= (UINT_PTR)((LPTSTR)lpBV->rgbValue + lpBV->cbTotMem)))
						{
							// We found it
							lpSi->ParamDataCType = lpBV->fCType;
							fFound = TRUE;
							break;
						}

						lpBV = lpBV->next;
					}

					if(fFound)
					{
						fFound = FALSE;
						udRowPtr = (UINT_PTR)lpBV->rgbValue;

						// Now check through each row of that bound column looking for the correct row
						for(idx=0;idx<lpSi->uRowSetSize;idx++)
						{
							if(rgbTemp == udRowPtr)
							{
								szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
										GetGtrString(szTemp, MAXOUTBUFF, TST1013),
										(UDWORD)lpBV->icol, (UDWORD)(idx + 1));
								fFound = TRUE;
								break;
							}

							udRowPtr += (UINT_PTR)lpBV->cbElement;
						}

						if(!fFound)			// We didn't ever find the right row within the bound column
							ASSERTCHECK(TRUE, TEXT("Unable to find bound memory needing data."));
					}
					else						// We didn't ever find the right bound column
						ASSERTCHECK(TRUE, TEXT("Unable to find bound memory needing data."));
				}

				// It is row-wise binding
				else if ((!lpSi->fColBinding) && (lpSi->lpRowWise))
				{
					;
				}
            // If neither of the above cases were true, something is wrong here
				else
					ASSERTCHECK(TRUE, TEXT("Unable to find bound memory needing data."));
			}
			else if (lpSi->lpParams)
			{
				lpParam = lpSi->lpParams;

				for(idx=0;idx<(lpSi->cParams);idx++)
				{
					if((UDWORD *)lpParam->rgbValue == (UDWORD *)(*(PTR *)lpParms[1]->lpData))
					{
						lpSi->ParamDataCType = lpParam->fCType;
						break;
					}
					else
						lpParam = lpParam->next;
				}

				szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
						GetGtrString(szTemp, MAXOUTBUFF, TST1004),
						lpSi->lpCurParam ? lpSi->lpCurParam->ipar : lpParam->ipar);

			}
		}

		//Move to next parameter
		if (lpSi->lpCurParam && lpSi->lpCurParam->next)
			lpSi->lpCurParam=lpSi->lpCurParam->next;
		else
			lpSi->lpCurParam=NULL;

	} //if((rc == SQL_NEED_DATA) && ((ci) && (ci->lpCurStmt)))
	else if (RC_SUCCESSFUL(rc))
	{
		//reset current parameter if SUCCESS
		if (lpSi->lpParams)
			lpSi->lpCurParam=lpSi->lpParams;
		else
			lpSi->lpCurParam=NULL;

	}

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLPutData:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLPutData(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLPutData(hstmt,
						(PTR)lpParms[1]->lpData,           	// rgbValue
						*(SDWORD *)lpParms[2]->lpData);		// cbValue

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLParamOptions:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLParamOptions(STD_FH_PARMS)
{
	RETCODE		rc;
	UWORD			cParam;
	UDWORD		cNewRows=*(UDWORD *)lpParms[1]->lpData;		//	Number of rows we hope to set using ParamOptions
	UDWORD		cOldRows;
	UDWORD		cTemp;
	//UDWORD *pirowTmp=NULL;
    SQLULEN *pirowTmp=NULL;
	lpPARAMETERINFO	lpPI;
	BOOL			frgbValue=TRUE;
	BOOL			fpcbValue;
	BOOL			ffMacro;
	HWND			hwndOut;
	BOOL			fRowsetParam = FALSE;
	UINT			idx;
	lpBOUNDVARS			lpBVTmp = NULL;
	lpSTATEMENTINFO	lpSiTmp = NULL;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	hwndOut = GETOUTPUTWINDOW(ci);

	// First we need to check for some stuff relating to parameters bound into rowsets
	if((ci) && (ci->lpCurStmt))
	{
		if(ci->lpCurStmt->lpParams)
		{
			lpPI = ci->lpCurStmt->lpParams;

			for(idx=0; idx<(UINT)ci->lpCurStmt->cParams; idx++)
			{
				if(lpPI->frgbRowset || lpPI->fpcbRowset)
				{
					// If we are here, we know that we have a parameter bound to a rowset
					if((!lpPI->fColBinding) && (cNewRows > 1))
					{
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
										GetGtrString(szOutBuff, MAXOUTBUFF, TST0018));
						return SQL_ERROR;
					}

					// Now we need to make sure that with cNewRows, a parameter is not off the end of the rowset
					// First we need to find the right
					lpBVTmp = (lpBOUNDVARS)lpPI->lpRowset;
					lpSiTmp = (lpSTATEMENTINFO)lpBVTmp->lpStmt;

					if((!lpSiTmp) || ((lpPI->iRow + cNewRows - 1) > (lpSiTmp->uRowSetSize)))
					{
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
										GetGtrString(szOutBuff, MAXOUTBUFF, TST0019));
						return SQL_ERROR;
					}
				}

				lpPI = lpPI->next;
			}
		}
	}

	// If the new pirow is > the old pirow, then try to allocate memory first,
	// if it is less than the old pirow, allocate memory after the call
	// if the memory re-allocation doesn't work, then return SQL_ERROR
	if((ci) && (ci->lpCurStmt) && (ci->lpCurStmt->lpParams))
	{
		cOldRows = ci->lpCurStmt->cParamRows;

		if(cNewRows > cOldRows)
		{
			lpPI = ci->lpCurStmt->lpParams;
			cParam = ci->lpCurStmt->cParams;

			while(cParam--)
			{
				// First try to re-alloc the rgbValue buffer
				if(!lpPI->frgbRowset)
				{
					if(lpPI->rgbValue)
					{
						if((cNewRows * lpPI->cBytes) > MAX_ALLOCATED_BUFFER)
						{
							szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
										GetGtrString(szOutBuff, MAXOUTBUFF, TST0008),
										(MAX_ALLOCATED_BUFFER-1L));
							return SQL_ERROR;
						}

						frgbValue = ReAllocAdjMem(lpPI->rgbValue, (cOldRows * lpPI->cBytes), (cNewRows * lpPI->cBytes));

						if(!frgbValue)
							szMessageBox(hwndOut,MB_ICONINFORMATION | MB_OK, (LPTSTR)TEXT("Error"),
									GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
									(cNewRows * lpPI->cBytes), TEXT("rgbValue's array"));
					}
				}

				// Next try to re-alloc the pcbValue buffer
				if(!lpPI->fpcbRowset)
				{
					if(frgbValue)
					{
						if(lpPI->pcbValue)
						{
							if((cNewRows * sizeof(SDWORD)) > MAX_ALLOCATED_BUFFER)
							{
								szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
											GetGtrString(szOutBuff, MAXOUTBUFF, TST0008),
											(MAX_ALLOCATED_BUFFER-1L));
								return SQL_ERROR;
							}

							fpcbValue = ReAllocAdjMem(lpPI->pcbValue, (cOldRows * sizeof(SDWORD)), (cNewRows * (sizeof(SDWORD))));
							ffMacro = ReAllocAdjMem(lpPI->lpfMacro, (cOldRows * sizeof(BOOL)), (cNewRows * (sizeof(BOOL))));

							if((!fpcbValue) || (!ffMacro))
								szMessageBox(hwndOut,MB_ICONINFORMATION | MB_OK, (LPTSTR)TEXT("Error"),
										GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
										(cNewRows * sizeof(SDWORD)), TEXT("pcbValue's array"));
						}
					}
				}

				// Make sure none of the re-alloc's failed for this parameter
				// If one of them did, we need to undo the ones we have already done
				if((!frgbValue) || (!fpcbValue) || (!ffMacro))
				{
					// Walk back through the linked list of parameters and re-allocate the memory
					// back to its original size
					while(cParam < ci->lpCurStmt->cParams)
					{
						if((lpPI->rgbValue) && (!lpPI->frgbRowset))
						{
							if(!ReAllocAdjMem(lpPI->rgbValue, (cNewRows * lpPI->cBytes), (cOldRows * lpPI->cBytes)))
							{
								ASSERTCHECK(TRUE, szMEMALLOC_ERROR);
							}
						}

						if((lpPI->pcbValue) && (!lpPI->fpcbRowset))
						{
							if(!ReAllocAdjMem(lpPI->pcbValue, (cNewRows * sizeof(SDWORD)), (cOldRows * sizeof(SDWORD))))
							{
								ASSERTCHECK(TRUE, szMEMALLOC_ERROR);
							}

							if(!ReAllocAdjMem(lpPI->lpfMacro, (cNewRows * sizeof(BOOL)), (cOldRows * sizeof(BOOL))))
							{
								ASSERTCHECK(TRUE, szMEMALLOC_ERROR);
							}
						}

						lpPI = lpPI->prev;      // Decrement to the previous parameter in the linked list
						cParam++;					// Increment the count of which parameter we are on
					}

					return SQL_ERROR;
				}
				else									// The memory re-allocation worked
					lpPI = lpPI->next;			// Increment to the next parameter in the linked list
			}
		}
	}

	// Invoke function
	// First check to see if pirow is a null pointer and set the temporary pointer accordingly
	if(!lpParms[2]->fNull)                                                	// if pirow is not a NULL pointer
	{
		if((ci) && (ci->lpCurStmt))
			(UDWORD *)pirowTmp = &(ci->lpCurStmt->pirow);
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc = SQLParamOptions(hstmt,
					 			*(SQLULEN *)lpParms[1]->lpData,	// crow
					 			pirowTmp);								// pirow

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   // Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Now we check for 2 cases:
	//		1) if the call was successful and cNewRows is less than cOldRows (we need to do the memory adjustment)
	//		2) if the call was unsuccessful and cNewRows is greater than cOldRows (we need to rollback the memory adjustment)
	//	The variables cNewRows and cOldRows are already set for the first case
	if((ci) && (ci->lpCurStmt) && (ci->lpCurStmt->lpParams))
	{
		lpPI = ci->lpCurStmt->lpParams;
		cParam = ci->lpCurStmt->cParams;

		if((((rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO)) && (cNewRows < cOldRows)) ||
			((rc == SQL_ERROR) && (cNewRows > cOldRows)))
		{
			if (rc == SQL_ERROR)
			{
				// We need to reset the memory to the size that it was before the call to SQLParamOptions
				cTemp = cNewRows;
				cNewRows = cOldRows;
				cOldRows = cTemp;
			}

			// Reallocate memory to the smaller size
			while(cParam--)
			{
				if((lpPI->rgbValue) && (!lpPI->frgbRowset))
					if(!ReAllocAdjMem(lpPI->rgbValue, (cOldRows * lpPI->cBytes), (cNewRows * lpPI->cBytes)))
						ASSERTCHECK(TRUE, szMEMALLOC_ERROR szMEMALLOC_ERROR);

				if((lpPI->pcbValue) && (!lpPI->fpcbRowset))
					if(!ReAllocAdjMem(lpPI->pcbValue, (cOldRows * sizeof(SDWORD)), (cNewRows * sizeof(SDWORD))))
						ASSERTCHECK(TRUE, szMEMALLOC_ERROR);

				lpPI = lpPI->next;			// Increment to the next parameter in the linked list
			}
		}
	}

	// Now set the number of rows in the StatementInfo structure, along with the fParamOpt flag(since it has been called)
	if((rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO))
	{
		ci->lpCurStmt->cParamRows = cNewRows;
		ci->lpCurStmt->fParamOpt = TRUE;

		if(!lpParms[2]->fNull)                                              	// if pirow is a NULL pointer
			ci->lpCurStmt->fpirowNull = FALSE;													// Set the flag in the StatementInfo struct
		else
			ci->lpCurStmt->fpirowNull = TRUE;													// Set the flag in the StatementInfo struct
	}

	return rc;
}

//*---------------------------------------------------------------------------------
//| FindSINode:
//|	This function finds the STATEMENTINFO node assocated with 'hstmt'
//| Parms:
//|	lpCI		pointer to CONNECTIONINFO linked list
//|	hstmt		Handle of node to locate
//| Returns:
//|	The Node if found, otherwise, NULL
//*---------------------------------------------------------------------------------

lpSTATEMENTINFO INTFUN FindSINode(lpCONNECTIONINFO lpCI,HSTMT hstmt)
{
	lpSTATEMENTINFO	lpStmt=NULL;
	SWORD					iStmt;

	for (iStmt=0,lpStmt=lpCI->lpStmt;iStmt < lpCI->cStmts;lpStmt=lpStmt->next,iStmt++)
	{
		if (hstmt == lpStmt->hstmt)
			break;
	}

	return(lpStmt);

} //FindSINode()


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
RETCODE INTFUN lpSQLFreeStmt(STD_FH_PARMS)
{
	RETCODE				rc;
	UWORD					fOption = *(UWORD *)(lpParms[1]->lpData);
	lpSTATEMENTINFO	lpState=NULL;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	//Find the correct StatementInfo node, we can't
	//assume the the user selected the current statement
	if (ci && ci->lpStmt)
		lpState=FindSINode(ci,hstmt);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, 	cParms, ci, TRUE);

	// Invoke function
	rc = SQLFreeStmt(hstmt,fOption);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// If successful, add the new hstmt to the combo box and to our linked list
	if(rc == SQL_SUCCESS)
	{
		switch(fOption)
		{
			// Destroy the results window if present
			case SQL_CLOSE:
				if(lpState->lpResults)
				{
					DestroyResultWindow((lpRESULTWININFO)lpState->lpResults,TRUE);
					lpState->lpResults = NULL;
				}
				break;

			// Drop means we need to remove hstmt from combo-box
			case SQL_DROP:
				DoDropStmt(ci,lpState);
				break;

			// Need to free all bound column nodes
			case SQL_UNBIND:
				if(lpState->lpResults)
				{
					DestroyResultWindow((lpRESULTWININFO)lpState->lpResults,FALSE);
					lpState->lpResults = NULL;
				}

				while(lpState->lpVars)
					FreeBoundColumn(lpState, lpState->lpVars->next);
				break;

			case SQL_RESET_PARAMS:
				while(lpState->lpParams)
					FreeParameter(lpState, lpState->lpParams->next);
				break;
			}
		}

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLCloseCursor:
//|	This function frees resources associated with a statment handle. It is
//|	equivolent to tcalling SQLFreeStmt(SQL_CLOSE)
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLCloseCursor(STD_FH_PARMS)
{
	RETCODE				rc;
	lpSTATEMENTINFO	lpState=((ci) ? ci->lpCurStmt : NULL);
	HSTMT					hstmt=NULL;

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[IPAR_CC_HSTMT]->lpData)
		hstmt=*(HSTMT *)lpParms[IPAR_CC_HSTMT]->lpData;

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms,cParms, ci, TRUE);

	rc = SQLCloseCursor(hstmt);

	// If successful, add the new hstmt to the combo box and to our linked list
	if(rc == SQL_SUCCESS)
	{
		if(lpState->lpResults)
		{
			DestroyResultWindow((lpRESULTWININFO)lpState->lpResults,TRUE);
			lpState->lpResults = NULL;
		}
	}

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;

} //lpSQLCloseCursor()




//*---------------------------------------------------------------------------------
//| lpFillParam:
//|	This function allows the user to set the rgbValue and pcbValue of a
//|	parameter set by SQLBindParameter.
//| Parms:
//|	ci							Current connection information (always NULL)
//| Returns:
//|	Always returns SQL_SUCCESS
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpFillParam(STD_FH_PARMS)
{
	return SQL_SUCCESS;
}

//*---------------------------------------------------------------------------------
//| StatementShowParams:
//|	This function will return the result descriptor for all parameters associated
//|	with a given hstmt.
//| Parms:
//|	ci							Current connection information (always NULL)
//| Returns:
//|
//*---------------------------------------------------------------------------------
VOID INTFUN StatementShowParams(lpCONNECTIONINFO ci)
{
	UWORD			ipar;
	UDWORD		iRow;
	HWND			hwndOut;
	TCHAR			szParamTypeBuf[40];
	TCHAR			szCTypeBuf[40];
	TCHAR			szSqlTypeBuf[40];
	TCHAR			szMacroBuf[10];
	lpDFTARRAY	lpDftParam=lpdftParamType;
	lpDFTARRAY	lpDftC=lpdftCTypes;
	lpDFTARRAY	lpDftSql=lpdftSqlTypes;
	lpPARAMETERINFO	lpParam=NULL;
	TCHAR			szpcbValue[40];
	TCHAR			Temp[30];

	PTR			lpszValue;
	BOOL			fFormat=FALSE;
	SDWORD		dLength;
	SDWORD		pcbValueTmp;

	BOOL			fRowset=FALSE;
	BOOL			fParamOpt=FALSE;
	TCHAR			szRowTmp[15];
	TCHAR			szRowsetHdr[50];
	UINT			cbChar=sizeof(TCHAR);


	if(!((ci) && (ci->lpCurStmt)))
		return;
	if(!(ci->lpCurStmt->lpParams))
		return;

	fBusy(TRUE);
	lpParam = ci->lpCurStmt->lpParams;
	hwndOut = GETOUTPUTWINDOW(ci);

	// Walk the list of parameters and see if any are from rowsets
	for(ipar=0;ipar<(ci->lpCurStmt->cParams);ipar++){
		if(lpParam->frgbRowset || lpParam->fpcbRowset){
			fRowset = TRUE;
			break;
			}
		lpParam = lpParam->next;
		}
	lpParam = ci->lpCurStmt->lpParams;

	// Check and see if ParamOptions has been called on this hstmt
	if(ci->lpCurStmt->fParamOpt)
		fParamOpt = TRUE;

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			(LPTSTR)szShowParamTitle, ci->lpCurStmt->uStmtNum);

	// Print the value of pirow if ParamOptions has been called
	if(fParamOpt)
		if(ci->lpCurStmt->fpirowNull)
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						(LPTSTR)szpirowNull);
		else
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						(LPTSTR)szpirowValue, ci->lpCurStmt->pirow);

	// Determine what the secondary header will look like -- affected by fParamOpt & fRowset
	if(fParamOpt)
		lstrcpy(szRowTmp, szRow);
	else
		szRowTmp[0] = TEXT('\0');

	if(fRowset)
		lstrcpy(szRowsetHdr, szRowsetHeader);
	else
		szRowsetHdr[0] = TEXT('\0');

	// Print the secondary header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			(LPTSTR)szShowParamSubtitle,
			szRowTmp, szRowsetHdr);

   for (ipar=0;ipar<(ci->lpCurStmt->cParams);ipar++){
		// Make sure the user hasn't aborted
		if(fAbortCheck(hwndOut))
			goto exit01;

		// Find fParamType in default array
		if(lpParam->dwParamDex != CB_ERR)
			wsprintf(szParamTypeBuf, TEXT("%s=%d"),lpDftParam[lpParam->dwParamDex].szOpt, lpDftParam[lpParam->dwParamDex].fOpt);
		else
		{
			FormatValueBasedOnType(szParamTypeBuf,
										sizeof(szParamTypeBuf)/sizeof(szParamTypeBuf[0]),
										&(lpParam->fParamType),
										SQL_C_USHORT,
										sizeof(USHORT),
										hwndOut);
		}

		// Find fCType in default array
		wsprintf(szCTypeBuf, TEXT("%s=%d"),lpDftC[lpParam->dwCDex].szOpt, lpDftC[lpParam->dwCDex].fOpt);

		// Find fSqlType in default array
		wsprintf(szSqlTypeBuf, TEXT("%s=%d"),lpDftSql[lpParam->dwSqlDex].szOpt, lpDftSql[lpParam->dwSqlDex].fOpt);

		// Figure out the size of the buffer needed for the string representing rgbValue
		if(!lpParam->rgbValue)
			dLength = 5;										// size of "NULL"
	   else if(lpParam->fCType == SQL_C_BINARY)
	   	dLength = ((lpParam->cBytes * 2) + 3)*cbChar;	// Two characters for every binary byte plus '0x' and null terminator
	   else if (IsCharData(lpParam->fCType))
			dLength = (lpParam->cBytes + cbChar);          // Extra character for the null terminator
		else
	   	dLength = 64;										// This makes the buffer big enough for the largest timestamp

		lpszValue = (PTR)AllocateMemory(dLength);
		if(!lpszValue) {
			szMessageBox(hwndOut,
					MB_ICONINFORMATION | MB_OK,
					TEXT("Error"),
					GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
					dLength, (LPTSTR)szrgbVal);
			goto exit01;
			}

		// Repeat for each element only changing the rgbValue (for multiple elements -- using SQLParamOptions)
		for(iRow=0;iRow<ci->lpCurStmt->cParamRows;iRow++){
			// Now format pcbValue correctly
			if(!(lpParam->pcbValue))
				lstrcpy(szpcbValue,TEXT("NULL"));
			else{
				switch(*(SDWORD *)(lpParam->pcbValue + iRow)){
					case SQL_NTS:
						lstrcpy(szpcbValue, GetGtrString((LPTSTR)Temp, 30, idsSQL_NTS));
						break;
					case SQL_NULL_DATA:
						lstrcpy(szpcbValue, GetGtrString((LPTSTR)Temp, 30, idsSQL_NULL_DATA));
						break;
					case SQL_DATA_AT_EXEC:
						lstrcpy(szpcbValue, GetGtrString((LPTSTR)Temp, 30, idsSQL_DATA_AT_EXEC));
						break;
					case SQL_DEFAULT_PARAM:
						lstrcpy(szpcbValue, GetGtrString((LPTSTR)Temp, 30, idsSQL_DEFAULT_PARAM));
						break;
					default:
						// The pcbValue is a number -- see if the macro was invoked
						// If it was, then undo it to display the real number
						if(*(lpParam->lpfMacro + iRow))
							pcbValueTmp = UNDO_LEN_DATA_AT_EXEC(*(SDWORD *)(lpParam->pcbValue + iRow));
						else
							pcbValueTmp = *(SDWORD *)(lpParam->pcbValue + iRow);
						wsprintf(szpcbValue, TEXT("%ld"), pcbValueTmp);
						break;
					}
				}

			// Format the SQL_LEN_DATA_AT_EXEC flag correctly
			if(*(lpParam->lpfMacro + iRow))
				lstrcpy(szMacroBuf, TEXT("TRUE"));
			else
				lstrcpy(szMacroBuf, TEXT("FALSE"));


			// Now format rgbValue correctly
			if(!(lpParam->rgbValue))
			{
				lstrcpy(lpszValue, TEXT("NULL"));
				fFormat = TRUE;
			}
			else
				fFormat = FormatValueBasedOnType(lpszValue, dLength,
								((TCHAR *)lpParam->rgbValue + (lpParam->cBytes * (iRow))),
								lpParam->fCType, lpParam->cBytes,hwndOut);

			if(!fFormat)
				lstrcpy(lpszValue, TEXT(""));

			// Now format iRow and the rowset information if it is needed
			if(fParamOpt)
				wsprintf(szRowTmp, TEXT(" %lu,"), (iRow + 1));
			else
				szRowTmp[0] = TEXT('\0');

			if(fRowset){
				if((lpParam->frgbRowset) || (lpParam->fpcbRowset))
					wsprintf(szRowsetHdr, szRowsetTitle, lpParam->iHdbc, lpParam->iHstmt,
								lpParam->iCol, (lpParam->iRow + iRow));
				else
					// This is the case where we have parameters from rowset values, but this is an ordinary
					// parameter and we don't want to print out bogus information
					lstrcpy(szRowsetHdr, szFourNA);
				}
			else
				szRowsetHdr[0] = TEXT('\0');

			// Now print the information for one column
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					  (LPTSTR)szShowParamData,
						lpParam->ipar, (LPTSTR)szRowTmp, (LPTSTR)szParamTypeBuf, (LPTSTR)szCTypeBuf,
						(LPTSTR)szSqlTypeBuf, lpParam->cbColDef, lpParam->ibScale,
						lpParam->cbValueMax, szpcbValue, szMacroBuf, szRowsetHdr, lpszValue);
			}

		// Advance parameter pointer to the next parameter
		lpParam=lpParam->next;

		// Now clear the buffers
		FREE_PTR(lpszValue);
   	}

	// Now print an extra line for clarity
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			  TEXT("\r\n"));
exit01:
	fBusy(FALSE);
	return;
}



//*---------------------------------------------------------------------------------
//| FreeParameter:
//|	This function will free the given parameter node from the list of parameters.
//| Parms:
//|	lpState					The statement node to keep track of head
//|	lpParam					Pointer to the parameter node to free
//| Returns:
//|	TRUE if the node is successfully added/updated
//*---------------------------------------------------------------------------------
BOOL INTFUN FreeParameter(lpSTATEMENTINFO lpState, lpPARAMETERINFO lpParam)
{
	lpBOUNDVARS		lpBVOld;
	lpROWWISEINFO	lpRWOld;

	// Removing the head node?
	if(lpParam == lpState->lpParams) {
		if(lpState->lpParams == lpState->lpParams->next)			// Only one node anyway
			lpState->lpParams = NULL;
		else
			lpState->lpParams = lpState->lpParams->next;			// Pass the buck
		}

	// Remove the node from the list then free its memory
	lpParam->next->prev = lpParam->prev;
	lpParam->prev->next = lpParam->next;

	// Release rgbValue memory if necessary, checking for rgbValue's that come from rowsets
	// and correctly decrementing lock counts and releasing all orphaned memory necessary
	if(lpParam->rgbValue){									// If a previous rgbValue pointer exists
		if(lpParam->frgbRowset){							// The value is from a rowset
			if(lpParam->fColBinding){						// It is column-wise bound memory
				lpBVOld = lpParam->lpRowset;
				if(--(lpBVOld->uRgbLockCnt) == 0){		// If the lock count goes to zero
					FreeAdjMem(lpBVOld->rgbValue);
					if((lpBVOld->uRgbLockCnt == 0) && (lpBVOld->uPcbLockCnt == 0))
						FREE_PTR(lpParam->lpRowset);
					}
				}
			else{													// It is row-wise bound memory
				lpRWOld = lpParam->lpRowset;
				if(--(lpRWOld->uRgbLockCnt) == 0){
					FreeAdjMem(lpRWOld->rgbValue);
					FREE_PTR(lpParam->lpRowset);
					}
				}
			}
		else														// The value is not from a rowset
			FreeAdjMem(lpParam->rgbValue);
		}

	// Release pcbValue memory if necessary, checking for pcbValue's that come from rowsets
	// and correctly decrementing lock counts and releasing all orphaned memory necessary
	if(lpParam->pcbValue){									// If a previous pcbValue pointer exists
		if(lpParam->fpcbRowset){							// The value is from a rowset
			if(lpParam->fColBinding){						// It is column-wise bound memory
				lpBVOld = lpParam->lpRowset;
				if(--(lpBVOld->uPcbLockCnt) == 0){			// If the lock count goes to zero
					FreeAdjMem(lpBVOld->pcbValue);
					if((lpBVOld->uRgbLockCnt == 0) && (lpBVOld->uPcbLockCnt == 0))
						FREE_PTR(lpParam->lpRowset);
					}
				}
			else{														// It is row-wise bound memory
				lpRWOld = lpParam->lpRowset;
				if(--(lpRWOld->uRgbLockCnt) == 0){
					FreeAdjMem(lpRWOld->rgbValue);
					FREE_PTR(lpParam->lpRowset);
					}
				}
			}
		else
			FreeAdjMem(lpParam->pcbValue);
		}

	FREE_PTR(lpParam);

	// Reset the number of params to 0
	lpState->cParams = 0;

	return TRUE;
}



//*---------------------------------------------------------------------------------
//| CheckRowsetCache:
//|	If a rowset is present with an rgfRowStatus array, then we also have a
//|	cache of those values present.  When we successfully execute a statement,
//|	we check that cache to see if there are differences.  If there were, then
//|	the rowset needs to be repainted.
//| Parms:
//|	lpci						Connection info
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN CheckRowsetCache(lpCONNECTIONINFO lpci)
{
	lpSTATEMENTINFO 	lpStmt = lpci->lpStmt;
	SWORD					iStmts = lpci->cStmts;


	// Note we could save the cursor name and go looking for it to find
	//		the exact statement which may have been affected by this
	//		execute, but this is a test tool and that would be a little
	//		overkill.
	while(iStmts--)
	{
		if(lpStmt->lpResults && lpStmt->rgfRowCache && lpStmt->rgfRowStatus)
		{
			if(memcmp(lpStmt->rgfRowCache, lpStmt->rgfRowStatus,
							(size_t)(sizeof(UWORD) * TOT_ROWSET_SIZE(lpStmt))))
				RefreshWindow(((lpRESULTWININFO)lpStmt->lpResults)->hwnd);
		}

		lpStmt = lpStmt->next;
	}
}


//*---------------------------------------------------------------------------------
//| ChangeRowsetSize:
//|	Attempt to update to a new rowset size (either via SetStmtOption or through
//|	SetScrollOption).  This requires reallocating all bound memory.  So if this
//|	fails we need a recovery plan.
//| Parms:
//|	lpci					Connection info
//|	lpStmt				The statement with bound vars to adjust
//|	uNewRowsetSize		The new size
//|	fResetStmtOpt		TRUE if we should reset SQL_ROWSET_SIZE, FALSE if called
//|								from SQLSetScrollOptions
//|	fEFChanged			TRUE if we need to change the size of the ExtendedFetch rowset
//|							FALSE if we need to change the size of the FetchScroll/Fetch rowset
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ChangeRowsetSize(lpCONNECTIONINFO lpci,
					lpSTATEMENTINFO lpStmt, UDWORD uNewRowsetSize,
					BOOL fResetStmtOpt,UDWORD uSQLApi)
{
	UDWORD	cbOldRowSet;
	int		idsStr=0;
	RETCODE	rc=SQL_SUCCESS;
	BOOL		fUse25= (uSQLApi == SQL_ROWSET_SIZE) ;
	BOOL		f25 = (!lpci->lpActiveEnv->fODBC3XEnv );


	if (fUse25 || f25)
	{
		// The rowset size for SQLExtendedFetch
		// has changed.
		// Save old value, and set new one
		cbOldRowSet = lpStmt->uEFRowSetSize;
		lpStmt->uEFRowSetSize = uNewRowsetSize;
		idsStr=idsErrChgRowsetSize;
	}
	if ( !fUse25)
	{
		// The rowset size for SQLFetchScroll/SQLFetch
		// has changed.
		// Save old value, and set new one
		cbOldRowSet = lpStmt->uFSFRowSetSize;
		lpStmt->uFSFRowSetSize = uNewRowsetSize;
		idsStr=idsErrChgArraySize;
	}

	// Try to reset all bound columns, if not successful,
	//		warn user and reset rowset size
	if(!ResetStmtBVMem(lpStmt, GETOUTPUTWINDOW(lpci), cbOldRowSet))
	{
		if(fResetStmtOpt)
		{
			szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV0, TRUE, TRUE,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsStr),
					lpStmt->uStmtNum, uNewRowsetSize, cbOldRowSet);


			if (fUse25 || f25)
				SQLSetStmtOption(lpStmt->hstmt,SQL_ROWSET_SIZE,	cbOldRowSet);
			if (!fUse25)
				rc=SQLSetStmtAttr(lpStmt->hstmt,SQL_ATTR_ROW_ARRAY_SIZE,(PTR)(LONG_PTR)cbOldRowSet,0);
		}
		else
		{
			szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV0, TRUE, TRUE,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsErrDoingScrollOptions),
					lpStmt->uStmtNum, uNewRowsetSize, cbOldRowSet);
		}

		if (fUse25 || f25)
			lpStmt->uEFRowSetSize = cbOldRowSet;
		if(!fUse25)
			lpStmt->uFSFRowSetSize = cbOldRowSet;

		return FALSE;
	}
	else
		// It add worked, so commit any decreases required
		CommitBVChanges(lpStmt);

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| ChangeBindType:
//|	Try to change the bind type based on the handles passed in.  We might issue
//|	TST0010 if this cannot be done at the current point in time.  If the user
//|	is attempting to change the bind type while there are bound parameters in
//|	the rowset, then issue TST0017 since this would blow those parameters out
//|	of the water.
//| Parms:
//|	hwnd						Where to write errors if there are any
//|	lpci						Connection info.  NULL if only lpStmt is to be used
//|	lpInStmt					Statement info. NULL if all statements on lpci
//|	uNewVal					The new value for SQL_BIND_TYPE
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ChangeBindType(HWND hwnd, lpCONNECTIONINFO lpci,
						lpSTATEMENTINFO lpInStmt, UDWORD uNewVal)
{
	lpSTATEMENTINFO		lpStmt=lpInStmt;

	//
	// Check single statment (SQLSetStmtOption) first
	//
	if(lpInStmt) {
		// If there are no changes, there is nothing to do
		if(uNewVal == lpStmt->uRowWiseSize)
			return TRUE;
		// If we're doing row-wise binding and they're changing the row size
		//	to the valid sum of bound cols, there is no problem
		else if(!lpInStmt->fColBinding &&
				  uNewVal == SumBoundCols(lpInStmt))
			;
		// Else they're making a change which would cause a re-bind to occur
		//	and we cannot do this
		else if(lpInStmt->lpVars)
			goto err0010;
		// If we have bound parameters, then this change could cause them to
		//	become invalid, disallow
		else if(IsBoundParamsOnRowset(lpInStmt))
			goto err0017;

		// We made it
		ChangeBindTypeDetails(lpStmt, uNewVal);
		}

	//
	// Check all statements on connection (SQLSetConnectOption).
	//	The individual checks remain the same as above, we just spreading
	//	them over each statment and holding off until all are valid
	//	before changing any of them.
	//
	else {
		SWORD			cStmts=lpci->cStmts;
		BOOL			fChange=FALSE;

		lpStmt = lpci->lpStmt;
		while(cStmts--) {
			if(uNewVal != lpStmt->uRowWiseSize)
				fChange = TRUE;

			if(!lpStmt->fColBinding &&							// Row-wise binding
				uNewVal == SumBoundCols(lpStmt))				//		and setting row-wise to bind total
				;

			else if(uNewVal != lpStmt->uRowWiseSize &&
						lpStmt->lpVars)
				goto err0010;

			else if(IsBoundParamsOnRowset(lpInStmt))
				goto err0017;

			lpStmt = lpStmt->next;
			}

		// If there were no changes, it doesn't hurt anything
		if(!fChange)
			return TRUE;

		// There were no statements with bound cols, so reset each statement
		cStmts=lpci->cStmts;
		lpStmt = lpci->lpStmt;
		while(cStmts--) {
			ChangeBindTypeDetails(lpStmt, uNewVal);
			lpStmt = lpStmt->next;
			}
		lpci->uRowWiseSize = uNewVal;
		}

	return TRUE;


// Following label is for error
err0010:
	szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
			GetGtrString(szOutBuff, MAXOUTBUFF, TST0010),
			(!uNewVal) ? (LPTSTR)szCOLWISE : (LPTSTR)szROWWISE);
	return FALSE;

err0017:
	szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, FALSE,
			GetGtrString(szOutBuff, MAXOUTBUFF, TST0017));
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| GetAttribute:
//|	This function retrieves the specified Attribute either from the hdbc or hstmt
//| Parms:
//|	hwnd		window handle
//|	lpCI		Connection info.
//|	sdAttr	Attribute to be retrieved
//| Returns:
//|	sdValue	Attribute value
//*---------------------------------------------------------------------------------

SDWORD INTFUN GetAttribute(HWND hwnd,lpCONNECTIONINFO lpCI,SDWORD sdAttr)
{
	SDWORD	sdValue=0;
	RETCODE	rc=SQL_SUCCESS;
	LPTSTR	lpAPI=NULL;
	TCHAR		szRtn[MEDBUFF]=TEXT("");
	HSTMT		hstmt=NULL;
	HDBC		hdbc=NULL;

	hdbc=lpCI->hdbc;

	if (lpCI->lpCurStmt)
		hstmt=lpCI->lpCurStmt->hstmt;

	if (lpCI->lpCurStmt)
	{
		rc=SQLGetStmtAttr(hstmt,
								sdAttr,
								&sdValue,
								sizeof(sdValue),
								NULL);
		lpAPI=TEXT("SQLGetStmtAttr");
	}
	else
	{
		rc=SQLGetConnectAttr(hdbc,
								sdAttr,
								&sdValue,
								sizeof(sdValue),
								NULL);
		lpAPI=TEXT("SQLGetConnectAttr");
	}

	if (RC_NOTSUCCESSFUL(rc))
	{
		szMessageBox(hwnd, MB_OK,
						 NULL, TEXT("%s returned %s"),lpAPI,GetRetCodeName(rc, szRtn),
						 TEXT("Current Cursor Settings"));
		ToolOptionsAllErrors(lpCI, NULL,hdbc,hstmt,NULL,rc);

		return(-1);
	}

	return(sdValue);

} //GetAttribute()




//*---------------------------------------------------------------------------------
//| GetCurrentCursorSettings:
//|	This tool will display the current CURSOR settings.
//| Parms:
//|	lpCI		Connection info.
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN GetCurrentCursorSettings(lpCONNECTIONINFO lpCI)
{
	SDWORD				sdConCur=0,
							udType=0,
							udRowArraySize=0,
							udRowSetSize=0;
	TCHAR					szConCur[LARGEBUFF],
							szType[LARGEBUFF];
	HWND					hwnd = GetEditWindow();

	//Get current concurrency
	switch (GetAttribute(hwnd,lpCI,SQL_ATTR_CONCURRENCY))
	{
		case SQL_CONCUR_READ_ONLY:
			lstrcpy(szConCur,szSQL_CONCUR_READ_ONLY);
			break;
		case SQL_CONCUR_LOCK:
			lstrcpy(szConCur,szSQL_CONCUR_LOCK);
			break;
		case SQL_CONCUR_ROWVER:
			lstrcpy(szConCur,szSQL_CONCUR_ROWVER);
			break;
		case SQL_CONCUR_VALUES:
			lstrcpy(szConCur,szSQL_CONCUR_VALUES);
			break;
		default:
			wsprintf(szConCur,szINVALID_ATTR,sdConCur);

	}

	//Get current cursor type
	switch (GetAttribute(hwnd,lpCI,SQL_ATTR_CURSOR_TYPE))
	{
		case SQL_CURSOR_FORWARD_ONLY:
			lstrcpy(szType,szSQL_CURSOR_FORWARD_ONLY);
			break;
		case SQL_CURSOR_KEYSET_DRIVEN:
			lstrcpy(szType,szSQL_CURSOR_KEYSET_DRIVEN);
			break;
		case SQL_CURSOR_DYNAMIC:
			lstrcpy(szType,szSQL_CURSOR_DYNAMIC);
			break;
		case SQL_CURSOR_STATIC:
			lstrcpy(szType,szSQL_CURSOR_STATIC);
			break;
		default:
			wsprintf(szType,szINVALID_ATTR,udType);
	}

	//Get current rowset size
	udRowArraySize=GetAttribute(hwnd,lpCI,SQL_ATTR_ROW_ARRAY_SIZE);

	//Get current rowset size
	udRowSetSize=GetAttribute(hwnd,lpCI,SQL_ROWSET_SIZE);

	szMessageBox(hwnd, MB_OK,
		TEXT("Current Cursor Settings"),
		TEXT("Cursor Type:\t\t%s\r\nConcurrency:\t\t%s\r\nRowset Size:\r\n\tSQLExtendedFetch:\t%d\r\n\tSQLFetch/SQLFetchScroll:\t%d\r\n"),
		szType,szConCur,udRowSetSize,udRowArraySize);

}

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
#define MIN_ALLOC_SIZE				(sizeof(TIMESTAMP_STRUCT))

#define ROLLBACK_SIZE					 15

#define GREALLOCFLAGS				(GHND)


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR							szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern HFONT						hEditFont;

// Following refer to default arrays which must be used out of context of
//		the parameter arrays
extern lpDFTARRAY					lpdftSqlTypes;
extern lpDFTARRAY					lpdftNullable;
extern lpDFTARRAY					lpdftCTypes;
extern UINT 						ciSqlTypes;
extern UINT							ciNullable;
extern UINT							ciCTypes;
extern UINT							fUnicode;

static TCHAR szQUOTE[]					= TEXT("\"");
dCSEG(TCHAR) szBOUND[]					= TEXT("BOUND");
dCSEG(TCHAR) szERROR[]					= TEXT("ERROR");
dCSEG(TCHAR) szTRUNC[]					= TEXT("TRUNC: ");
dCSEG(TCHAR) szNODATA[]					= TEXT("NO DATA");
dCSEG(TCHAR) szNULLVAL[]				= TEXT("<Null>");
dCSEG(TCHAR) szInvalidOpt[]			= TEXT("Invalid fOption: %d");
dCSEG(TCHAR) szDescColTitle[]			= TEXT("\r\nDescribe Column All: \r\n");
dCSEG(TCHAR) szNumResColFail[]		= TEXT("\t\t\t\tSQLNumResultCols returned %s. \r\n");
dCSEG(TCHAR) szNoColsToDesc[]			= TEXT("\t\t\t\tThere are no columns to describe. \r\n");
dCSEG(TCHAR) szDescColSubtitle[]		= TEXT("\t\t\t\t\t\t\t\t\ticol, szColName, *pcbColName, *pfSqlType, *pcbColDef, *pibScale, *pfNullable \r\n");
dCSEG(TCHAR) szDescColFailed[]		= TEXT("\t\t\t\tSQLDescribeCol returned %s. \r\n");
dCSEG(TCHAR) szDescColData[]			= TEXT("\t\t\t\t\t\t\t\t\t\t\t%d, %s, %d, %s, %lu, %d, %s \r\n");
dCSEG(TCHAR) szGetDataRowTitle[]		= TEXT("\r\nGet Data Row:\r\n");
dCSEG(TCHAR) szGetDataAllTitle[]		= TEXT("\r\nGet Data All:\r\n");
dCSEG(TCHAR) szShowBoundColsTitle[]	= TEXT("\t\t\t\t\t\t\t\t\ticol, fCType, cbValueMax, pcbValue, rgbValue\r\n");
dCSEG(TCHAR) szShowBoundColsData[]	= TEXT("\t\t\t\t\t\t\t\t\t\t\t%u, %s, %ld, ");
dCSEG(TCHAR) szBINDCOLUMN[]			= TEXT("SQLBindCol");


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN GetUpdatedStatusFlags(lpCONNECTIONINFO ci);          // From FHCONN.C
PTR AdjustStmtBVMem(UWORD uTask, lpSTATEMENTINFO lpStmt, lpBOUNDVARS lpbv,
			SDWORD cbValueMax, SDWORD * lpcbTotMem,
			UNALIGNED SDWORD * * lppcbValue,
			HWND hwndOut);
BOOL INTFUN AddCommitRollbackBVNode(lpSTATEMENTINFO lpStmt, LPVOID lpData,
						SDWORD cbOldSize, SDWORD cbNewSize);
BOOL INTFUN RollbackBVChanges(lpSTATEMENTINFO lpStmt);
VOID INTFUN FreeRollbackBV(lpSTATEMENTINFO lpStmt);
BOOL INTFUN AddBoundColumn(lpSTATEMENTINFO lpStmt, UWORD icol, SWORD fCType,
					SDWORD cbElement, PTR rgbValue, SDWORD cbTotValueMax, SDWORD * pcbValue,
					UWORD uType);
UDWORD INTFUN SumBoundCols(lpSTATEMENTINFO lpStmt);
VOID INTFUN DoCreateResults(lpCONNECTIONINFO lpci, lpSTATEMENTINFO lpStmt,
					HINSTANCE hInst, HENV henv,RETCODE rc);



//*---------------------------------------------------------------------------------
//| DropNestedHandles:
//|	This function drops nested stmt handes on fetch
//| Parms:
//|	lpStmt			pointer to SI struct
//| Returns:
//*---------------------------------------------------------------------------------
RETCODE INTFUN DropNestedHandles(lpCONNECTIONINFO ci, lpSTATEMENTINFO lpStmt)
{
	// drop child stmts
	if (lpStmt && lpStmt->lpStmtChildren)
	{
		while (lpStmt->lpStmtChildren)
		{
			RETCODE 		rc;
			lpSTATEMENTINFO lpStmtChild = lpStmt->lpStmtChildren;

			//For ODBC >= 3.0 use SQLFreeHandle()
			if (lpUsrOptions->nODBCVer >= IDR_ODBCVER_30)
				rc = SQLFreeHandle(SQL_HANDLE_STMT, lpStmtChild->hstmt);
			else
				rc = SQLFreeStmt(lpStmtChild->hstmt, SQL_DROP);

			if (RC_NOTSUCCESSFUL(rc))
				return rc;

			DoDropStmt(ci, lpStmtChild);
		}

		lpStmt->lpStmtChildren = NULL;
	}
	return SQL_SUCCESS;
}

//*---------------------------------------------------------------------------------
//| DisplayRowset:
//|	This function determines if the rowset should be displayed
//| Parms:
//|	lpStmt			pointer to SI struct
//|	rc					RETCODE of Fetch
//| Returns:
//|	TRUE if the Rowset should be display, otherwise FALSE
//*---------------------------------------------------------------------------------
BOOL INTFUN DisplayRowset(lpSTATEMENTINFO lpStmt,RETCODE rc)
{
	BOOL			fRowStatusPtr=FALSE;
	lpBOUNDVARS	lpbv=NULL;
	lpCONNECTIONINFO lpci = (lpStmt) ? lpStmt->lpCI :NULL;


	if (!lpStmt || (rc == SQL_ERROR) || ( rc == SQL_STILL_EXECUTING) )
		return(FALSE);

	lpbv=lpStmt->lpVars;

	fRowStatusPtr=(lpStmt->lpARD && lpStmt->lpARD->rgArrayStatusPtr);

	//	backward compatibility
	if (lpci && ! (lpci->lpActiveEnv->fODBC3XEnv ))
		fRowStatusPtr = (lpStmt && lpStmt->rgfRowStatus);


	if (lpStmt->fAutoDisplay)
	{
		if ((fRowStatusPtr || lpbv))
		{
			return(TRUE);
		}
	}

	return(FALSE);

} //DisplayRowset()


//*---------------------------------------------------------------------------------
//| SetupRowsetBuffers:
//|		This function untouches the rowset buffers before each call to
//|	SQLExtendedFetch,SQLFetchScroll, or SQLFetch and sets the correct
//|	bound vars.
//| Parms:
//|	lpStmt		Statementinfo record
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN SetupRowsetBuffers(lpSTATEMENTINFO lpStmt,HWND hwndOut)
{
	lpBOUNDVARS			lpbv=NULL;
	UINT					ibv,
							cCols=lpStmt->uBoundCols;
	PTR					rgbValue=NULL;

	//Check for bound columns
	if (!lpStmt->lpVars)
		return;

	lpbv=lpStmt->lpVars;

	for (ibv=0;ibv < cCols;ibv++)
	{
		if (lpbv->uType == CWB_UNBIND)
		{
			if (lpbv->pcbValue)
			{
				FreeAdjMem(lpbv->pcbValue);
				lpbv->pcbValue=NULL;
			}

			// Row-wise with bound node
			if (!lpStmt->fColBinding && lpbv && lpStmt->uBoundCols - 1)
			{
				// and rows left after this one gone
				AdjustStmtBVMem(CWB_UNBIND, lpStmt, lpbv,					// Change the size
									lpbv->cbElement, &lpbv->cbTotMem,
									NULL, hwndOut);
			}

			if(lpStmt->lpVars && lpbv)
			{
				lpBOUNDVARS lpCurr=lpbv->next;
				//pass in the node before the node that is to
				//be freed.
				FreeBoundColumn(lpStmt, lpbv);
				lpbv=lpCurr;
			}

			FreeRollbackBV(lpStmt);

		}

		if (lpbv->uType != CWB_FIRST_TIME)
		{
			//copy all "New" binding information to the used vars
			if (lpbv->fCTypeNew)
			{
				lpbv->fCType=lpbv->fCTypeNew;
				lpbv->fCTypeNew=0;
			}

			if (lpbv->cbElementNew)
			{
				lpbv->cbElement=lpbv->cbElementNew;
				lpbv->cbElementNew=0;
			}

			if (lpbv->cbTotMemNew)
			{
				lpbv->cbTotMem=lpbv->cbTotMemNew;
				lpbv->cbTotMemNew=0;
			}

			if (lpbv->rgbValueNew)
			{
				if (lpbv->rgbValue)
					FreeAdjMem(lpbv->rgbValue);

				lpbv->rgbValue=lpbv->rgbValueNew;
				lpbv->rgbValueNew=NULL;
			}

			if (lpbv->pcbValueNew)
			{
				if (lpbv->pcbValue)
					FreeAdjMem(lpbv->pcbValue);

				lpbv->pcbValue=lpbv->pcbValueNew;
				lpbv->pcbValueNew=NULL;
			}
		}

		//Untouch the bound buffers
		InitUnModified(lpbv->rgbValue,lpbv->cbTotMem);

		lpbv=lpbv->next;
	}

} //SetupRowsetBuffers()


//*---------------------------------------------------------------------------------
//| lpSQLNumResultCols:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLNumResultCols(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLNumResultCols(hstmt,
								(SWORD *)lpParms[1]->lpData);	// pccol

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	//  Check for errors
	AUTOLOGERRORSI(ci,rc,hstmt);


	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLDescribeCol:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLDescribeCol(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 										// szColName
						(*(SWORD *)lpParms[3]->lpData)* sizeof(TCHAR), 				//	based on cbColNameMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLDescribeCol  (hstmt,
						 		*(UWORD *)lpParms[1]->lpData,            				// icol
						 		(LPTSTR)lpParms[2]->lpData,                   		// szColName
						 		*(SWORD *)lpParms[3]->lpData,            				// cbColNameMax
						 		(SWORD *)lpParms[4]->lpData,          					// pcbColName
						 		(SWORD *)lpParms[5]->lpData,          					// pfSqlType
						 		(SQLULEN *)lpParms[6]->lpData,          				// pcbColDef
						 		(SWORD *)lpParms[7]->lpData,          					// pibScale
						 		(SWORD *)lpParms[8]->lpData);          				// pfNullable

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[2], lpParms[4], TRUE);

	// Check szColName for null termination
	CHECKNULLTERM(ci, lpParms[2], lpParms[4], TRUE);

	// Buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	TRUNCATIONCHECK(lpParms[2]->fNull, lpParms[4]->fNull,
				rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[2]->lpData,
				*(SWORD *)lpParms[3]->lpData, *(SWORD *)lpParms[4]->lpData,
				(LPTSTR)lpParms[4]->szName);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| LogColumnAttributes:
//|		This function takes care of the special logging that SQLColAttributes and
//|	SQLColAttribute require.
//| Parms:
//|	szFuncName				Name of SQL API
//|	cParms					# of Parms for this API
//|	ci							Current connection information
//|	lpParms					Pointer to the paramter array to use for the request
//|	uAttrib					attribute of parm
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN LogColumnAttributes(LPTSTR szFuncName, UINT cParms, lpCONNECTIONINFO ci, lpPARAMITEM * lpParms, UDWORD uAttrib)
{
	UINT			dex;
	BOOL			fLogged=FALSE;

	// Set the output parameters depending on descriptor (fDescType)
	if(IsCharData(lpParms[3]->fCType))
	{
		// log rgbDesc and pcbDesc
		lpParms[3]->uParmType |= PT_LOG_OUT;
		lpParms[5]->uParmType |= PT_LOG_OUT;
		lpParms[6]->uParmType &= ~PT_LOG_OUT;
	}
	else
	{
		// log pfDesc only
		lpParms[3]->uParmType &= ~PT_LOG_OUT;
		lpParms[5]->uParmType &= ~PT_LOG_OUT;
		lpParms[6]->uParmType |= PT_LOG_OUT;
	}

	// Log output parameters
	// Check to see if the value returned was a handle, bitmask, or linked array --
	// If it is, then we need to format the output for those special cases
	for(dex=0;  dex<(lpParms[2]->cDfts);  dex++)
	{
		// Find the correct fOption in the default array
		if(((SDWORD)*(UWORD *)lpParms[2]->lpData) == lpParms[2]->lpDfts[dex].fOpt)
		{
			// If we get here, we know which element in the default array the fOption is
			// Clear the prefix bits in uValInfo
			if((lpParms[2]->lpDfts[dex].uValInfo == PRM_32HDL) 				// the fDescType rgbDesc is a handle
         	|| (lpParms[2]->lpDfts[dex].lpLink))	// the fDescType rgbDesc is from a linked array (or bitmask)
         {
         	if(!(uAttrib & FH_NO_LOG))
				{
	         	LogParametersSpecial(szFuncName, lpParms,
											cParms, ci, FALSE,
											lpParms[2]->lpDfts[dex].uValInfo, 2, 6);
					fLogged=TRUE;
				}
			}
			else
			{
				LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);
				fLogged=TRUE;
			}
		}
	}

	if(!fLogged)	// fOption was not in the default array -- driver defined type
		LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

} //LogColumnAttributes()


//*---------------------------------------------------------------------------------
//| lpSQLColAttributes:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLColAttributes(STD_FH_PARMS)
{
	RETCODE		rc;
	UWORD			fDescType=*(UWORD *)lpParms[2]->lpData;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 													// rgbDesc
						(*(SWORD *)lpParms[4]->lpData) * sizeof(TCHAR), 				//	based on cbDescMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLColAttributes   (hstmt,
						 			*(UWORD *)lpParms[1]->lpData,            				// icol
						 			fDescType,									            				// fDescType
						 			(VOID *)lpParms[3]->lpData,            				// rgbDesc
						 			*(SWORD *)lpParms[4]->lpData,         					// cbDescMax
						 			(SWORD *)lpParms[5]->lpData,         					// pcbDesc
						 			(SQLPOINTER)lpParms[6]->lpData);        					// pfDesc

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	//sumit LogColumnAttributes(szFuncName,cParms,ci,lpParms,uAttrib);

	LOGPARAMETERS(szFuncName,lpParms,cParms,ci,FALSE);

	// Check each *string* buffer
	if(lpParms[3]->fCType != SQL_C_SLONG)
		UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Check for Null termination
	switch(fDescType)
	{
		case SQL_COLUMN_LABEL:
		case SQL_COLUMN_NAME:
		case SQL_COLUMN_TABLE_NAME:
		case SQL_COLUMN_OWNER_NAME:
		case SQL_COLUMN_QUALIFIER_NAME:
		case SQL_COLUMN_TYPE_NAME:
			CHECKNULLTERM(ci, lpParms[3], lpParms[5], TRUE);
			break;
	}

	// Buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	TRUNCATIONCHECK(lpParms[3]->fNull, lpParms[5]->fNull, rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[3]->lpData,
				*(SWORD *)lpParms[4]->lpData, *(SWORD *)lpParms[5]->lpData,
				(LPTSTR)lpParms[5]->szName);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLColAttribute:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLColAttribute(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 								// rgbCharacterAttribute
						((INT_PTR)lpParms[4]) , //	based on cbBufferLength
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLColAttribute	(hstmt,
						 		*(UWORD *)lpParms[1]->lpData,    // iColumnNumber
						 		*(UWORD *)lpParms[2]->lpData,		// fFieldIdentifier
						 		(PTR)lpParms[3]->lpData,         // rgbCharacterAttribute
						 		*(SWORD *)lpParms[4]->lpData,    // cbBufferLength
						 		(SWORD *)lpParms[5]->lpData,		// pcbStringLength
						 		(PTR)lpParms[6]->lpData);			// pvNumericAttribute

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	//sumit LogColumnAttributes(szFuncName,cParms,ci,lpParms,uAttrib);

	LOGPARAMETERS(szFuncName,lpParms,cParms,ci,FALSE);

	// Check each *string* buffer
	if(lpParms[3]->fCType != SQL_C_SLONG)
		UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	TRUNCATIONCHECK(lpParms[3]->fNull, lpParms[5]->fNull, rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[3]->lpData,
				*(SWORD *)lpParms[4]->lpData, *(SWORD *)lpParms[5]->lpData,
				(LPTSTR)lpParms[5]->szName);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}



//*---------------------------------------------------------------------------------
//| lpSQLRowCount:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLRowCount(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLRowCount(hstmt,
						 (SQLLEN *)lpParms[1]->lpData);					// pcrow

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
//| DoBindColumn:
//|	This function performs the internal bookkeeping on the bound column.
//|	(i.e. Finds the bound node, allocates the bound column,etc.
//| Parms:
//|	lpCI					Current connectioinfo record
//|	icol					Column to be bound
//|	prgbValue			Address of rgbValue
//|	ppcbValue			Address of pcbValue
//|	uType					State of Bound Var
//|	cbValueMax			cbValueMax
//|	cbValueMaxUsed		cbValueMax actually used in binding
//|	fCType				C Type of bound column
//|	frgbValueNull		Did user check the rgbValue box
//|	fpcbValueNull		Did user check the pcbValue box
//|	fFieldID				DESC field ID
//| Returns:
//|	lpbv					Pointer to bound var node
//*---------------------------------------------------------------------------------

lpBOUNDVARS INTFUN DoBindColumn(lpCONNECTIONINFO lpCI,UWORD icol,PTR *prgbValue,
										UNALIGNED SDWORD **ppcbValue,UWORD *puType,
										SDWORD cbValueMax,SDWORD *pcbValueMaxUsed,SWORD fCType,
										BOOL frgbValueNull,BOOL fpcbValueNull,SQLSMALLINT fFieldID,
										SDWORD *pcbTotValueMax)
{
	HWND						hwndOut=GETOUTPUTWINDOW(lpCI);
	PTR						rgbValue=NULL;
	UNALIGNED SDWORD		*pcbValue=NULL;
	lpBOUNDVARS				lpbv=NULL;
	lpSTATEMENTINFO		lpStmt=lpCI ? lpCI->lpCurStmt : NULL;

	// We have a valid statement and a non-null HDBC
	// Look for the node in question
	lpbv = FindBoundVar(lpStmt->lpVars,	lpStmt->uBoundCols, icol);

   //if (lpbv && !fpcbValueNull)
   //	pcbValue = lpbv->pcbValue;

	if (fFieldID==SQL_DESC_OCTET_LENGTH_PTR)
	{
		//force the pcbValue to be realloc'd in this
		//case
		if (pcbValue)
			FreeAdjMem(pcbValue);
	}

	// Check flag and allocate mem if required
	if (lpStmt->fColBinding && (!fpcbValueNull && !pcbValue))
		pcbValue = (SDWORD *)AllocateAdjMem(sizeof(SDWORD));

	switch(fFieldID)
	{
		case SQL_DESC_TYPE:
		case SQL_DESC_OCTET_LENGTH:
		case SQL_DESC_LENGTH:
		case SQL_DESC_PRECISION:
		case SQL_DESC_SCALE:
		case SQL_DESC_DATETIME_INTERVAL_CODE:
//		case SQL_DESC_INTERVAL_CODE:
//		case SQL_DESC_DATETIME_INTERVAL:
//		case SQL_DESC_CHARACTER_SET_CATALOG:
//		case SQL_DESC_CHARACTER_SET_SCHEMA:
//		case SQL_DESC_CHARACTER_SET_NAME:
			//Free the rgbValue and set it to NULL as per
			//   spec.
			if (lpbv)
			{
				if (lpbv->rgbValue)
					FreeAdjMem(lpbv->rgbValue);

				lpbv->rgbValue=NULL;
			}
			break;
	}

	if(frgbValueNull)				// NULL rgbValue means unbind
	{
		*puType = CWB_UNBIND;
	}
	else if (lpbv && lpbv->fBound)	// There was a bound-col node	and it was bound
		*puType = CWB_REBIND;			// Just a change in binding
	else if (lpStmt->lpRowWise)		// We're adding a new column to existing
		*puType = CWB_REBIND;
	else if (!lpbv ||						// There wasn't a bound node
   	(lpbv && !lpbv->rgbValue))		// 	or there was but no memory
   	*puType = CWB_FIRST_TIME;

	// If we are unbinding (rgbValue or pcbValue) check for TST0013 error
	if(!lpStmt->fColBinding && (frgbValueNull || fpcbValueNull))
	{
		if(lpbv && lpStmt->lpVars->prev != lpbv)
		{
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST0013));
			return(NULL);
		}
	}

	// TST0014: Don't let user rebind a column if there are parameters bound to that memory
	if(IsBoundParamsOnRowset(lpStmt))
	{
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
						GetGtrString(szOutBuff, MAXOUTBUFF, TST0014));
		return(NULL);
	}

	// TST0015: If in row-wise and user unbinds either rgb/pcb, then they
	//				will blow any bound params out of the water.
	if(!lpStmt->fColBinding &&	IsBoundParamsOnRowset(lpStmt))
	{
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
						GetGtrString(szOutBuff, MAXOUTBUFF, TST0015));
		return(NULL);
	}


	// Now allocate/adjust memory based on current condition
   if(*puType != CWB_UNBIND)
   {
   	// Find the cbValueMax to use.  If they don't match, user specified an
   	//	incorrect size for the type.
   	*pcbValueMaxUsed = (DWORD)GetSizeOfCType(fCType, (UDWORD)cbValueMax);

   	if(*pcbValueMaxUsed != cbValueMax)
		{
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST1015),
							*pcbValueMaxUsed, cbValueMax);
		}
   	else if (*pcbValueMaxUsed < 0)
   	{
			// Cannot allow 0 or negative number for variable length data
			//	types, since that would not be a valid buffer.
	   	*pcbValueMaxUsed = max(*pcbValueMaxUsed, 1);

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST1019),
							*pcbValueMaxUsed, cbValueMax);
   	}

		//it should be value to have a cbValueMax of 0 and have
		//a valid rgbValue
		if (!*pcbValueMaxUsed)
			*pcbValueMaxUsed=lpUsrOptions->uDftBindBuff;

	   rgbValue = AdjustStmtBVMem(*puType, lpStmt, lpbv,
								*pcbValueMaxUsed, pcbTotValueMax,
								(fpcbValueNull) ? NULL : &pcbValue,
								hwndOut);

		if(!rgbValue)
			return(NULL);

	}

	*prgbValue=rgbValue;
	*ppcbValue=pcbValue;

	return(lpbv);

} //DoBindColumn()



//*---------------------------------------------------------------------------------
//| UpdateBoundColumn:
//|	This function updates (commits the changes) the BoundVar record with rgbValue
//|	and pcbValue if SQLBindCol() returned SQL_SUCCESS.
//| Parms:
//|	lpCI					Current connectioinfo record
//|	icol					Column to be bound
//|	fCType				C Type
//|	rgbValue				data ptr
//|	pcbValue				pointer to cbValue
//|	uType					State of Bound Var
//|	lpbv					Pointer to bound var
//|	cbUsedValueMax		cbValueMax that is used for this binding
//|	rc						Return Code from SQLBindCol()
//| Returns:
//|	TRUE if SUCCESSFUL otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN UpdateBoundColumn(lpCONNECTIONINFO lpCI,UWORD icol,SWORD fCType,
										PTR rgbValue,UNALIGNED SQLLEN *pcbValue,UWORD uType,
										lpBOUNDVARS lpbv,SDWORD cbUsedValueMax,RETCODE rc,
										SDWORD *pcbTotValueMax)
{
	lpSTATEMENTINFO	lpStmt=lpCI ? lpCI->lpCurStmt : NULL;
	HWND					hwndOut=GETOUTPUTWINDOW(lpCI);
	BOOL					fSuccessful=TRUE;

	if(RC_SUCCESSFUL(rc))
	{

		// Now check rest of options
		if (uType != CWB_UNBIND && !AddBoundColumn(lpStmt, icol, fCType, cbUsedValueMax,
			 			rgbValue, *pcbTotValueMax, (SDWORD *)pcbValue,uType))
		{
			RollbackBVChanges(lpStmt);
			fSuccessful=FALSE;
		}
		else if (lpbv)
		{
			lpbv = FindBoundVar(lpStmt->lpVars,	lpStmt->uBoundCols, icol);
			lpbv->uType = uType;

			// It add worked, so commit any decreases required
			CommitBVChanges(lpStmt);
		}

	}
	else
	{
		// If not successful, then roll back any changes
		fSuccessful=FALSE;
		RollbackBVChanges(lpStmt);
	}

	// Free any node information we still have
	FreeRollbackBV(lpStmt);

	return(fSuccessful);

} //UpdateBoundColumn()


//*---------------------------------------------------------------------------------
//| lpSQLBindCol:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
static SQLUINTEGER dataAtFetch = SQL_DATA_AT_FETCH;
RETCODE INTFUN lpSQLBindCol(STD_FH_PARMS)
{
	RETCODE					rc;
	lpBOUNDVARS				lpbv=NULL;
	UWORD						uType=BIND_INVALID;					// Assume invalid handle
	lpSTATEMENTINFO		lpStmt=ci ? ci->lpCurStmt : NULL;

	// Due to the intense amount of processing required for this function, gather
	//		parameters as local variables
	BOOL						fNullHstmt=lpParms[0]->fNull;
	UWORD						icol=*(UWORD *)lpParms[1]->lpData;
	SWORD						fCType=*(SWORD *)lpParms[2]->lpData;
	BOOL						frgbValueNull=lpParms[3]->fNull;
	SDWORD					    cbValueMax = *(SDWORD *)lpParms[4]->lpData;
	BOOL						fpcbValueNull=lpParms[5]->fNull;
	SDWORD					    cbValueMaxUsed=cbValueMax,
								cbTotValueMax=0;
	PTR						    rgbValue=NULL;
	UNALIGNED INT_PTR		    *pcbValue=NULL;
	SQLHDESC					hdesc=NULL;
	HSTMT						hstmt=NULL;

	if (ci && ci->lpCurDesc)
		hdesc=ci->lpCurDesc->hdesc;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

    if (lpParms[6]->fNull)
    { // Setting Data-at-Fetch
        // Log input parameters
        LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);
        // Invoke function
        rc = SQLBindCol(hstmt,
                        icol,
                        fCType,
                        NULL,
                        NULL,
                        &dataAtFetch);
        LOGRETURNCODE(NULL, ci, rc);
        AUTOLOGERRORSI(ci, rc, hstmt);
        return rc;
    }

	// The memory allocated for the pcbValue is not adjustable.  We'll free
	//	it here and take care of it through the rest of this function handler
	FREE_PTR(lpParms[5]->lpData);
	lpParms[5]->lpData = NULL;
	lpParms[5]->cBytes = 0;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	if (hdesc)
	{
		//Set the appropriate descriptor fields
		SetDescriptorInfo(ci,
				hdesc,
				*(UWORD *)lpParms[1]->lpData,		// SQL_DESC_COUNT (ARD and IRD)
				NOT_USED,								// SQL_DESC_PARAMETER_TYPE (ARD)
				*(SWORD *)lpParms[2]->lpData,		// SQL_DESC_TYPE (ARD)
				NOT_USED,								// SQL_DESC_TYPE (IRD)
				NOT_USED,								// SQL_DESC_SCALE (IRD)
				lpParms[3]->lpData,					// SQL_DESC_DATA_PTR (ARD)
				NOT_USED,								// SQL_DESC_LENGTH and/or SQL_DESC_PRECISION (APD)
				*(SDWORD *)lpParms[4]->lpData,	// SQL_DESC_OCTET_LENGTH (ARD)
				lpParms[5]->lpData,					// SQL_DESC_OCTET_LENGTH_PTR (ARD)
				SQL_API_SQLBINDCOL);
	}

	if (lpStmt && !fNullHstmt)
	{
		// Figure out which type of binding we're doing
		lpbv=DoBindColumn(ci,icol,&rgbValue,&(UNALIGNED SDWORD *)pcbValue,&uType,
								cbValueMax,&cbValueMaxUsed,fCType,
								frgbValueNull,fpcbValueNull,0,
								&cbTotValueMax);

		//set these parameter so they are logged correctly
		lpParms[3]->lpData=rgbValue;
		lpParms[5]->lpData=pcbValue;

   	if (uType != CWB_UNBIND && !rgbValue)
			return SQL_ERROR;

	}

	// Invoke function
	rc = SQLBindCol(hstmt,
						 icol,
						 fCType,
						 rgbValue,
						 cbValueMax,				// cbValueMax (whatever user specified, internally use cbUsedValueMax)
						 (SQLLEN *)pcbValue);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// If successful, we need to update the bound column information
	UpdateBoundColumn(ci,
                     icol,
                     fCType,
                     rgbValue,
                     pcbValue,
                     uType,
					 lpbv,
                     cbValueMaxUsed,
                     rc,
                     &cbTotValueMax);

	//  Check for errors
	AUTOLOGERRORSI(ci,rc,hstmt);

	//Reset parameters to what they were before so that
	//gator doesn't free rgbValue and pcbValue
	lpParms[3]->lpData=NULL;
	lpParms[5]->lpData=NULL;

	return rc;

} //lpSQLBindCol()


//*---------------------------------------------------------------------------------
//| lpSQLFetch:
//|	Fetched values must be displayed upon success.  Note this will mean adding
//|	the newly fetched data to the display.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLFetch(STD_FH_PARMS)
{
	RETCODE				rc;
	lpSTATEMENTINFO	lpStmt=NULL;
	HSTMT					hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	if (ci && ci->lpCurStmt)
	{
		lpStmt=ci->lpCurStmt;

		//Untouch bufffers before next call to SQLFetchScroll
		//Since we don't want to display old data.For SQLFetchScroll
		//we don't necessarily know how many rows were fetched because
		//we don't have an rgfRowStatus as an argument.
		SetupRowsetBuffers(lpStmt,	GETOUTPUTWINDOW(ci));
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// drop nested handles
	if (lpStmt)
	{
		rc = DropNestedHandles(ci, lpStmt);
	}

	// Invoke function
	rc = SQLFetch(hstmt);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	if (RC_SUCCESSFUL_NDF(rc))
	{
		//Now update uBoundNodes since all bound columns have been fetched
		lpStmt->uBoundNodes=lpStmt->uBoundCols;
		//Set the Fetch API
		lpStmt->wFetchAPI=SQL_API_SQLFETCH;
		lpStmt->uRowSetSize=lpStmt->uFSFRowSetSize;

		if (lpStmt->lpIRD)
			lpStmt->rgfRowStatus = lpStmt->lpIRD->rgArrayStatusPtr;
	}

	//only show rowset window if Gator is in 3.0 mode
	// and there are bound columns or there is a
	// array status ptr in the IRD.
	//Set the Fetch API
	if (DisplayRowset(lpStmt, rc))
	{
		if (lpUsrOptions->fODBC30 && SumBoundCols(lpStmt))
			DoCreateResults(ci, lpStmt, hInst, ci->lpActiveEnv->henv,rc);
	}

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLExtendedFetch:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLExtendedFetch(STD_FH_PARMS)
{
	RETCODE				rc;
	lpBOUNDVARS			lpbv=NULL;
	lpSTATEMENTINFO	lpStmt=NULL;
	static UWORD		*rgfRowStatus=NULL;
	UDWORD				uRowSet=1;				// Default of one row
	UDWORD				i=0;

	static UDWORD		iBytes=0;
	HSTMT					hstmt=NULL;
	lpVOID				OldParamValue=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	if (ci && ci->lpCurStmt)
	{
		lpStmt=ci->lpCurStmt;

		if (lpStmt->lpVars)
			lpbv=lpStmt->lpVars;
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Check for proper row-wise buffer settings before calling
	if(lpStmt && !lpStmt->fColBinding && lpbv)
	{
		UDWORD 		uSum = SumBoundCols(lpStmt);

		if(lpStmt->uRowWiseSize != uSum)
		{
			szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST0012),
							lpStmt->uRowWiseSize,
							uSum);
			return SQL_ERROR;
		}
	}

	// If an array status is required for the call, allocate one
	if(ci && ci->lpCurStmt)
	{
		ci->lpCurStmt->uRowSetSize=ci->lpCurStmt->uEFRowSetSize;
		uRowSet = TOT_ROWSET_SIZE(ci->lpCurStmt);

		SetupRowsetBuffers(ci->lpCurStmt,GETOUTPUTWINDOW(ci));
	}




	if(	ci && ci->lpCurStmt &&
		!lpParms[4]->fNull && (!rgfRowStatus || (iBytes < (sizeof(UWORD) * uRowSet) )))
	{
		iBytes = (sizeof(UWORD) * uRowSet);
		if ( lpStmt->rgfRowStatus)
		{
			FREE_PTR(lpStmt->rgfRowStatus);
			//so that when the descriptors are freed IRD status does not point to an invalid location.
			if (lpStmt->lpIRD)
				lpStmt->lpIRD->rgArrayStatusPtr = NULL;
		}

		rgfRowStatus = AllocateMemory(iBytes);
		if(!rgfRowStatus)
		{
			szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)TEXT("Error"),
					GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
					iBytes, (LPTSTR)TEXT("rgfRowStatus"));
		}
		else
		{

			OldParamValue=lpParms[4]->lpData;
			lpParms[4]->lpData=rgfRowStatus;
			InitUnModified(rgfRowStatus,iBytes);

		}
	}
	else
	{
		OldParamValue=lpParms[4]->lpData;
		if (rgfRowStatus)
			lpParms[4]->lpData=rgfRowStatus;
	}


	// drop nested handles
	if (lpStmt)
	{
		rc = DropNestedHandles(ci, lpStmt);
	}

	// Invoke function
	rc = SQLExtendedFetch(hstmt,
								 *(UWORD *)lpParms[1]->lpData,		// fFetchType
								 *(SDWORD *)lpParms[2]->lpData,		// irow
								 (SQLULEN *)lpParms[3]->lpData,		// pcrow
								 rgfRowStatus);							// rgfRowStatus

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Buffer modification checking
	if (rc != SQL_NO_DATA_FOUND && rc != SQL_STILL_EXECUTING)
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	if (RC_SUCCESSFUL_NDF(rc))
	{
		//Now update uBoundNodes since all bound columns have been fetched
		lpStmt->uBoundNodes=lpStmt->uBoundCols;

		//Set the Fetch API
		lpStmt->wFetchAPI=SQL_API_SQLEXTENDEDFETCH;
		lpStmt->uRowSetSize=lpStmt->uEFRowSetSize;

		if (lpStmt->lpIRD)
			lpStmt->rgfRowStatus = lpStmt->lpIRD->rgArrayStatusPtr;
	}

	// Create results set window if successful
	if (DisplayRowset(lpStmt, rc))
	{
		// Reset rgfRowStatus based on this successful and create/refresh
		if(lpStmt->rgfRowStatus)
		{
			 FREE_PTR(lpStmt->rgfRowStatus);
			//so that when the descriptors are freed IRD status
			//does not point to an invalid location.
			 if (	lpStmt->lpIRD)
				 lpStmt->lpIRD->rgArrayStatusPtr = NULL;

		}

		lpStmt->rgfRowStatus = rgfRowStatus;

		DoCreateResults(ci, lpStmt, hInst, ci->lpActiveEnv->henv,rc);

		// If rgfRowStatus was available, then cache it to detect updates
		//	made to rowset via SetPos (ADD, DELETE, UPDATE)
		if(rgfRowStatus)
		{
			FREE_PTR(lpStmt->rgfRowCache);
			lpStmt->rgfRowCache = AllocateMemory(iBytes);
			if(lpStmt->rgfRowCache)
				memcpy(lpStmt->rgfRowCache, lpStmt->rgfRowStatus, (size_t)iBytes);
		}
	}
	else
	{
		if ( rc != SQL_STILL_EXECUTING)
		{
			FREE_PTR(rgfRowStatus);
			if (lpStmt && lpStmt->rgfRowStatus)
				lpStmt->rgfRowStatus = NULL;
			if (lpStmt && lpStmt->lpIRD)
				 lpStmt->lpIRD->rgArrayStatusPtr = NULL;
		}
	}

	lpParms[4]->lpData=OldParamValue;

	return rc;
}



//*---------------------------------------------------------------------------------
//| lpSQLFetchScroll:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLFetchScroll(STD_FH_PARMS)
{
	RETCODE				rc;
	lpSTATEMENTINFO	lpStmt=NULL;
	SQLHANDLE			hHandle=NULL;
	HSTMT					hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	if (ci && ci->lpCurStmt)
		lpStmt=ci->lpCurStmt;

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms,	cParms, ci, TRUE);


	//Untouch bufffers before next call to SQLFetchScroll
	//Since we don't want to display old data.For SQLFetchScroll
	//we don't necessarily know how many rows were fetched because
	//we don't have an rgfRowStatus as an argument.
	if (lpStmt)
		SetupRowsetBuffers(lpStmt,GETOUTPUTWINDOW(ci));

	// Check for proper row-wise buffer settings before calling
	if(lpStmt && !lpStmt->fColBinding && lpStmt->lpVars)
	{
		UDWORD 		uSum = SumBoundCols(lpStmt);

		if(lpStmt->uRowWiseSize != uSum)
		{
			szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST0012),
							lpStmt->uRowWiseSize,uSum);
			return SQL_ERROR;
		}
	}

	// drop nested handles
	if (lpStmt)
	{
		rc = DropNestedHandles(ci, lpStmt);
	}

	// Invoke function
	rc = SQLFetchScroll(hstmt,
								*(UWORD *)lpParms[IPAR_FS_FFETCHORIENTATION]->lpData,			// fFetchType
								*(SDWORD *)lpParms[IPAR_FS_FFETCHOFFSET]->lpData);		// irow

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	if (RC_SUCCESSFUL_NDF(rc))
	{
		//Now update uBoundNodes since all bound columns have been fetched
		lpStmt->uBoundNodes=lpStmt->uBoundCols;
		//Set the Fetch API
		lpStmt->wFetchAPI=SQL_API_SQLFETCHSCROLL;
		lpStmt->uRowSetSize=lpStmt->uFSFRowSetSize;

		if (lpStmt->lpIRD)
			lpStmt->rgfRowStatus = lpStmt->lpIRD->rgArrayStatusPtr;
	}

	// Create results set window if successful and there are bound
	// columns or RowStatusPtr has been defined and the user wants
	// it automatically displayed.
	if (DisplayRowset(lpStmt, rc))
		DoCreateResults(ci, lpStmt, hInst, ci->lpActiveEnv->henv,rc);

	return rc;

} //lpSQLFetchScroll()



//*---------------------------------------------------------------------------------
//| lpSQLSetPos:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetPos(STD_FH_PARMS)
{
	RETCODE				rc;
	lpRESULTWININFO	lprw;
	UWORD					irow = *(UWORD *)lpParms[1]->lpData;
	UWORD					fOption = *(UWORD *)lpParms[2]->lpData;
	HSTMT					hstmt=NULL;
	lpSTATEMENTINFO	lpStmt=NULL;

	if (ci && ci->lpCurStmt)
		lpStmt=ci->lpCurStmt;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLSetPos(hstmt,
						 irow,
						 fOption,
						 *(UWORD *)lpParms[3]->lpData);		// fLock

	// If successful, we may need to refresh the view
	if(RC_SUCCESSFUL(rc))
	{
		lpStmt->uCurrentRow = irow;

		ONDEBUG(wsprintf(szOutBuff, szInvalidOpt, fOption));
		ASSERTCHECK((fOption <= SQL_POSITION && fOption >= SQL_ADD), szOutBuff);

		//  Refresh if we have bound data and hence a rowset to paint
		lprw = ((lpRESULTWININFO)lpStmt->lpResults);
		if(lprw)
		{
			RefreshWindow(lprw->hwnd);
			SetMiniBarText(lprw, NULL);
		}
	}

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Check for SQL_NEED_DATA and set the fSetPos flag accordingly
	if(rc == SQL_NEED_DATA && lpStmt)
		lpStmt->fSetPos = TRUE;

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLBulkOperations:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLBulkOperations(STD_FH_PARMS)
{
	RETCODE				rc;
	lpRESULTWININFO	lprw;
	UWORD					fOperation = *(UWORD *)lpParms[1]->lpData;
	HSTMT					hstmt=NULL;
	lpSTATEMENTINFO	lpStmt=NULL;

	if (ci && ci->lpCurStmt)
		lpStmt=ci->lpCurStmt;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLBulkOperations(hstmt, fOperation);

	// If successful, we may need to refresh the view
	if(RC_SUCCESSFUL(rc))
	{
		ONDEBUG(wsprintf(szOutBuff, szInvalidOpt, fOperation));
		ASSERTCHECK((fOperation <= SQL_POSITION && fOperation >= SQL_ADD), szOutBuff);

		//  Refresh if we have bound data and hence a rowset to paint
		lprw = ((lpRESULTWININFO)lpStmt->lpResults);
		if(lprw)
		{
			RefreshWindow(lprw->hwnd);
			SetMiniBarText(lprw, NULL);
		}
	}

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Check for SQL_NEED_DATA and set the fSetPos flag accordingly
	if(rc == SQL_NEED_DATA && lpStmt)
		lpStmt->fSetPos = TRUE;

	return rc;

} // lpSQLBulkOperations()


//*---------------------------------------------------------------------------------
//| lpSQLGetData:
//|	Need to set the C type for rgbValue so it is logged correctly.  All output
//|	goes to the edit output window.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetData(STD_FH_PARMS)
{
	RETCODE		rc;
	SWORD			fCType = *(SWORD *)lpParms[2]->lpData;
	SWORD			icol	= *(UWORD *)lpParms[1]->lpData;
	//Bug #320
	INT_PTR		*pcbValue=NULL,
					cbValueMax=*(SDWORD *)lpParms[4]->lpData;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Check the fCType for SQL_C_DEFAULT -- if it is, set C Type to SQL_C_BINARY
	if (lpParms[0]->fNull || (fCType == SQL_C_DEFAULT))
		fCType = SQL_C_BINARY;					// Doesn't matter since the hstmt isn't valid anyway

	// Build output parameters
	lpParms[3]->fCType = fCType;
	BUILDOUTPUTPARM(lpParms[3],				// Allocate rgbValue
						cbValueMax ,					//  based on cbValueMax
						lpUsrOptions->fBufferChecking);

	//Bug #320. Added for readability
	pcbValue=(INT_PTR *)lpParms[5]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetData(hstmt,
						 icol,
						 *(SWORD *)lpParms[2]->lpData,	// fCType
						 (PTR)lpParms[3]->lpData,			// rgbValue
						 cbValueMax,
						 pcbValue);

	if (pcbValue && *pcbValue > 0 && (UDWORD)*pcbValue < lpParms[3]->cBytes)
		lpParms[3]->cBytes=(DWORD)*pcbValue;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Check Null termination
	if (IsCharData(fCType))
		CHECKNULLTERM(ci, lpParms[3], lpParms[5], TRUE);

	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	if (IsCharData(fCType))
	{
		TRUNCATIONCHECK(lpParms[3]->fNull, lpParms[5]->fNull,
					rc, GETOUTPUTWINDOW(ci),
					(LPTSTR)lpParms[3]->lpData, 												// rgbValue is buffer to check
					*(SDWORD *)lpParms[4]->lpData, 									// cbValueMax
					*(SDWORD *)lpParms[5]->lpData,									// pcbValueMax
					(LPTSTR)lpParms[5]->szName);
	}

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLGetNestedHandle:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetNestedHandle(STD_FH_PARMS)
{
	RETCODE		rc=0;
	HSTMT			hstmt = NULL;
	SQLHANDLE		hOutHandle = NULL;
	lpCONNECTIONINFO	lpCi=ci;
	HWND					hwndOut = GETOUTPUTWINDOW(ci);


	if (lpParms[0]->lpData)
		hstmt = *(HSTMT *)lpParms[0]->lpData;
	SWORD			icol = *(UWORD *)lpParms[1]->lpData;

	BUILDOUTPUTPARM(lpParms[2], 							// Allocate OutputHandlePtr
		sizeof(UDWORD),						//  based on cbValueMax
		lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetNestedHandle(hstmt,
        icol,	// nColParam
		lpParms[2]->fNull ? SQL_NULL_HANDLE : lpParms[2]->lpData);		// nested handle

	if (lpParms[2]->lpData)
		hOutHandle = *(SQLHANDLE *)lpParms[2]->lpData;

	lpParms[2]->fHandle = PT_PTR_PTR;

	if (RC_SUCCESSFUL(rc))
	{
		lpSTATEMENTINFO	lpState = DoAllocStmt(lpCi, hOutHandle);

		// add to parents list of child stmts
		lpSTATEMENTINFO lpStateParent = FindSINode(ci, hstmt);

		if(lpStateParent)
		{
			lpState->lpStmtParent = lpStateParent;
			
			if (lpStateParent->lpStmtChildren)
			{
				// Insert at end of list
				lpStateParent->lpStmtChildren->prevChild->nextChild = lpState;
				lpState->prevChild = lpStateParent->lpStmtChildren->prevChild;
				lpState->nextChild = lpStateParent->lpStmtChildren;
				lpStateParent->lpStmtChildren->prevChild = lpState;
			}
			else
			{
				// Only one in the list
				lpStateParent->lpStmtChildren = lpState;
				lpState->nextChild = lpState;
				lpState->prevChild = lpState;
			}
		}
	}
	
	if (lpCi)
		lpCi->hstmtCurr= hOutHandle;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	//If SQL_ERROR then the DM should have changed the
	//output handle to SQL_NULL_HANDLE
	if (RC_ERROR(rc) && lpParms[2]->lpData)
	{
		if (*(SQLHANDLE *)lpParms[2]->lpData != SQL_NULL_HANDLE)
		{
			TCHAR str[LARGEBUFF] = TEXT(""),
				szMsg[LARGEBUFF] = TEXT("");

			lstrcpy(str, lpParms[2]->szName);
			// Remove the '&' and ':' chars
			RemoveDlgFormat(str);

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
				GetGtrString(szMsg, sizeof(szMsg) / sizeof(szMsg[0]), TST1022), str);
		}

	}

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	//  Check for errors
	AUTOLOGERRORSI(ci, rc, hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLMoreResults:
//|	Successful completion means we have a new results set to display. This will
//|	cause the current results set to be destroyed.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLMoreResults(STD_FH_PARMS)
{
	RETCODE				rc;
	lpSTATEMENTINFO	lpStmt=((ci) && (ci->lpCurStmt)) ? ci->lpCurStmt : NULL;
	HSTMT					hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

  	// Invoke function
	rc = SQLMoreResults(hstmt);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// If successful, then we need to destroy old results set
	if(RC_SUCCESSFUL(rc) || rc == SQL_NO_DATA_FOUND)
	{
		if(lpStmt->lpResults)
		{
			DestroyResultWindow((lpRESULTWININFO)lpStmt->lpResults,TRUE);
			lpStmt->lpResults = NULL;
		}
	}

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLNextColumn:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLNextColumn(STD_FH_PARMS)
{
	RETCODE		rc=0;
	HSTMT			hstmt = NULL;

	if (lpParms[0]->lpData)
		hstmt = *(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLNextColumn(hstmt,
        (SQLSMALLINT *)lpParms[1]->lpData);	// pnColParam

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	//  Check for errors
	AUTOLOGERRORSI(ci, rc, hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLSetScrollOptions:
//|	If this function is successful, then attempt to adjust the rowset size
//|	which could mean reallocating bound memory.  ChangeRowsetSize will do this
//|	for us.  If it fails, then call SetScrollOptions to reset the value back
//|	to the old value.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetScrollOptions(STD_FH_PARMS)
{
	RETCODE		rc;
	UWORD			fConcurrency=*(UWORD *)lpParms[1]->lpData;
	SDWORD		crowKeyset=*(SDWORD *)lpParms[2]->lpData;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);


	// Invoke function
	rc = SQLSetScrollOptions(hstmt,
						 			fConcurrency,
						 			crowKeyset,
						 			*(UWORD *)lpParms[3]->lpData);		// crowRowset

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// If it was successful, we may need to reset the rowset size
	if(RC_SUCCESSFUL(rc))
	{
		UDWORD		uNewRowsetSize;						// Must use UDWORD to match SQL_ROWSET_SIZE
		UDWORD		uOldRowsetSize;

		// Try to change the bound rowsize, if this fails, then we must
		//	change back to the old size.
		uNewRowsetSize = (UDWORD)*(UWORD *)lpParms[3]->lpData;
		uOldRowsetSize = ci->lpCurStmt->uRowSetSize;
		if(!ChangeRowsetSize(ci, ci->lpCurStmt, uNewRowsetSize, FALSE,SQL_ROWSET_SIZE))
			SQLSetScrollOptions(ci->lpCurStmt->hstmt,
						 			fConcurrency,            												// fConcurrency
						 			crowKeyset,            													// crowKeyset
						 			(UWORD)uOldRowsetSize);
		else
		{
			// When successful, update our stored information and reset the
			//	tool bar description
			ci->lpCurStmt->uCursorType = (UDWORD)(ABS(crowKeyset));
			ci->lpCurStmt->uConcurType = fConcurrency;
			if(ci->lpCurStmt->lpResults)
				SetMiniBarText(((lpRESULTWININFO)ci->lpCurStmt->lpResults), NULL);
		}
	}

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);
	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
		GetGtrString(szOutBuff, MAXOUTBUFF, idsScrollOptionsObsolete));

	return rc;
}


//*---------------------------------------------------------------------------------
//| ResultsDescribeColAll:
//|	This function will return the result descriptor for all columns in a result
//|	set using the number of result columns (SQLNumResultCols) and SQLDescribeCol.
//| Parms:
//|	ci							Current connection information (always NULL)
//| Returns:
//|
//*---------------------------------------------------------------------------------
VOID INTFUN ResultsDescribeColAll(lpCONNECTIONINFO ci)
{
	UINT			idx;
	TCHAR			szTemp[100];
	TCHAR			szSqlTypeBuf[100];
	TCHAR			szNullableBuf[100];
   RETCODE		rc;
	SWORD			pcol;
	HWND			hwndOut;
	lpDFTARRAY	lpDftSql=lpdftSqlTypes;
	lpDFTARRAY	lpDftNull=lpdftNullable;
	UINT			cSqlTypes=ciSqlTypes;
	UINT			cNullable=ciNullable;

	HSTMT			hstmt=NULL;
	UWORD			icol,
					dexStart;
	SQLTCHAR		*szColName;
	SWORD			pcbColName;
	SWORD			pfSqlType;
	SQLULEN		pcbColDef;
	SWORD			pibScale;
	SWORD			pfNullable;

	BOOL			fFlushed=TRUE,
					fBookMarksOn=FALSE;
	LPTSTR		szOutBuff;
	LPTSTR		str;
	BOOL			fNoErrors=TRUE;
	lpSTATEMENTINFO lpStmt=NULL;


	if (!ci)
		return;

	szColName = (SQLTCHAR *)AllocateMemory(lpUsrOptions->uDftBuffLen);
	str = szOutBuff = (LPTSTR)AllocateMemory(lpUsrOptions->uDftBuffLen);

	fBusy(TRUE);

	if((ci) && (ci->lpCurStmt))
	{
		lpStmt=ci->lpCurStmt;
		hstmt = lpStmt->hstmt;
	}

	hwndOut = GETOUTPUTWINDOW(ci);

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			(LPTSTR)szDescColTitle);

	// First find out how many result columns
	rc = SQLNumResultCols(hstmt,&pcol);

	if(RC_NOTSUCCESSFUL(rc)){
		GetRetCodeName(rc, (LPTSTR)szTemp);
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					(LPTSTR)szNumResColFail, szTemp);
		ToolOptionsAllErrors(ci, NULL,NULL,hstmt,NULL,rc);
		goto exit01;
		}

   // Make sure there are columns to display
   if(pcol == 0){
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					(LPTSTR)szNoColsToDesc);
		goto exit01;
   	}

	// Print the secondary header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			(LPTSTR)szDescColSubtitle);

	//set starting index
	if (lpUsrOptions->fBindColZero && (lpStmt->uBookMarkAttr > SQL_UB_OFF))
	{
		fBookMarksOn=TRUE;
		dexStart=0;
	}
	else
		dexStart=1;

   // For each column, get a column description and allocate a buffer for it
   for (icol=dexStart;icol<=(UWORD)pcol;icol++)
	{
		// Make sure the user hasn't aborted
		if(fAbortCheck(hwndOut))
			goto exit01;

		rc = SQLDescribeCol(hstmt, icol, szColName,
									(SWORD)lpUsrOptions->uDftBuffLen, &pcbColName,
									&pfSqlType, &pcbColDef, &pibScale, &pfNullable);

      if(RC_NOTSUCCESSFUL(rc)){
			GetRetCodeName(rc, (LPTSTR)szTemp);
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						(LPTSTR)szDescColFailed, szTemp);
			ToolOptionsAllErrors(ci, NULL,NULL,hstmt,NULL,rc);
      	goto exit01;
		   }

		//Set the Bookmark column's name to <bookmark col>
		if (fBookMarksOn && !*szColName && icol==0)
			lstrcpy(szColName,TEXT("<bookmark column>"));


		// Find pfSqlType in default array
		for(idx=0;idx<cSqlTypes;idx++){
			if(pfSqlType == (SWORD)lpDftSql[idx].fOpt){
				wsprintf(szSqlTypeBuf, TEXT("%s=%d"), lpDftSql[idx].szOpt, pfSqlType);
				break;
				}
			}

		// Find pfNullable in default array
		for(idx=0;idx<cNullable;idx++){
			if(pfNullable == (SWORD)lpDftNull[idx].fOpt){
				wsprintf(szNullableBuf, TEXT("%s=%d"), lpDftNull[idx].szOpt, pfNullable);
				break;
				}
			}

		// Now print the information for one column
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
				  (LPTSTR)szDescColData,
					icol, szColName, pcbColName, szSqlTypeBuf,
					pcbColDef, pibScale, szNullableBuf);

		// Now clear the buffers
		szSqlTypeBuf[0] = TEXT('\0');
		szNullableBuf[0] = TEXT('\0');
   	}

	// Now print an extra line for clarity
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			  TEXT("\r\n"));

exit01:
 	FREE_PTR(szOutBuff);
	FREE_PTR(szColName);

	fBusy(FALSE);
}


//*---------------------------------------------------------------------------------
//| DoCheckRtnRow:
//|	User just did a SQLGetData.  This function will check the return code and
//|	display any required messages.
//| Parms:
//|	ci							Current connection information
//|	rc							Return code from SQLGetData
//|	hstmt						Statement handle
//|	uBoundCols				How many bound cols do you have
//|	hwndOut					Where to write output
//|	rgbValue					Pointer to the data (string data)
//|	cbValue					The returned pcbValue
//|	fSqlType					The sql type of the native data
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID INTFUN DoCheckRtnRow(lpCONNECTIONINFO lpci, RETCODE rc,
					HSTMT hstmt, UINT uBoundCols, HWND hwndOut,
					PTR rgbValue, SQLLEN cbValue, SWORD fSqlType)
{
	LPTSTR	szErrBnd;
	TCHAR		szState[6];
	LPSTR		str = (LPSTR)rgbValue;
	TCHAR		szStrW[XLARGEBUFF*2]=TEXT("");

	// If there are errors, we could have tried to get a bound column
	//		which is not valid under 1.0.  Indicate this by writing out
	//		BOUND.  The other option is a real error which will be denoted
	//		by ERROR.
	if(rc == SQL_ERROR) {
		szErrBnd = (LPTSTR)szERROR;                     	// Assume an error

		// Check if this is a bound column notice
		rc = SQLError(NULL, NULL, hstmt, szState, NULL, NULL, 0, NULL);
		if(RC_SUCCESSFUL(rc))
			if(lstrcmpi(szState, TEXT("S1002")) == 0 && uBoundCols)
				szErrBnd = (LPTSTR)szBOUND;

		// It's either BOUND or ERROR, so tell user
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				TRUE, FALSE, szErrBnd);
		}
	// Tell user there was no data to get.
	else if(rc == SQL_NO_DATA_FOUND) {
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				TRUE, FALSE, (LPTSTR)szNODATA);
		}
	else
	{
		// Look for any errors which we can overcome.  For example, if truncation
		//		occurs, we'll still display just with an extra message
		if(rc == SQL_SUCCESS_WITH_INFO)
		{
			rc = SQLError(NULL, NULL, hstmt, szState, NULL, NULL, 0, NULL);

			if(RC_SUCCESSFUL(rc))
			{
				if(lstrcmpi(szState,TEXT("01004")) == 0)			// Truncation
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
							TRUE, FALSE, (LPTSTR)szTRUNC);
				else
					ToolOptionsAllErrors(lpci, NULL,NULL,hstmt,NULL,rc);
			}
			else
				ToolOptionsAllErrors(lpci, NULL,NULL,hstmt,NULL,rc);
		}

		// Now print out the value
		if(cbValue == SQL_NULL_DATA)
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
					TRUE, FALSE, (LPTSTR)szNULLVAL);
		else
		{
			BOOL fIsChar= (fSqlType == SQL_CHAR || fSqlType == SQL_WCHAR);

			// Dump the current value
			if (fIsChar || (fSqlType == SQL_BINARY))
			{
				TCHAR szPrefix[TINYBUFF];

				//set prefix
				if (fSqlType == SQL_BINARY)
					lstrcpy(szPrefix,TEXT("0x"));
				else
					lstrcpy(szPrefix,TEXT("\""));

				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,szPrefix);
			}

			ConvertToUnicode(str,szStrW,sizeof(szStrW)/sizeof(szStrW[0]));

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, szStrW);

			if (fIsChar)
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, (LPTSTR)TEXT("\""));
		}
	}
}


//*---------------------------------------------------------------------------------
//| ResultsGetDataRow:
//|	For the current row, figure out how many columns there are (SQLNumResultsCols)
//|	and then do a SQLGetData on that row only.  No calls to SQLDescribeCol are done,
//|	all binding is done to SQL_C_CHAR at a length of 300.
//| Parms:
//|	ci							Current connection information
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID INTFUN ResultsGetRowSet(lpCONNECTIONINFO lpci)
{
	lpSTATEMENTINFO	lpStmt=NULL;
	BOOL					fRowStatusPtr=FALSE;
	lpBOUNDVARS			lpbv=NULL;

	if (lpci && lpci->lpStmt)
	{
		lpStmt=lpci->lpStmt;
		lpbv=lpStmt->lpVars;
	}

	// Create results set window if successful and there are bound
	// columns
	if (lpbv)
	{
		if (lpStmt->lpIRD)
			lpStmt->rgfRowStatus = lpStmt->lpIRD->rgArrayStatusPtr;

		DoCreateResults(lpci, lpStmt, hInst, lpci->lpActiveEnv->henv,SQL_SUCCESS);
	}

} //ResultsGetRowSet()


//*---------------------------------------------------------------------------------
//| ResultsGetDataRow:
//|	For the current row, figure out how many columns there are (SQLNumResultsCols)
//|	and then do a SQLGetData on that row only.  No calls to SQLDescribeCol are done,
//|	all binding is done to SQL_C_CHAR at a length of 300.
//| Parms:
//|	ci							Current connection information
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
#define DUMP_INFO \
	{																		\
	GetApiName(uDex, szOutBuff);                          \
	LogReturnCode((LPTSTR)szOutBuff, lpci, rc);            \
	ToolOptionsAllErrors(lpci, lpci->lpActiveEnv->henv, lpci->hdbc, hstmt,FALSE,rc);  \
	}

VOID INTFUN ResultsGetDataRow(lpCONNECTIONINFO lpci)
{
	RETCODE				rc;
	HSTMT					hstmt=NULL;
	HWND					hwndOut;
	SWORD					cCols=0;
	SWORD					dex;
	UINT					uDex;
	SQLLEN				cbValue;
	UDWORD				udRows=1;								// Cheat since we only do one row
	LPTSTR					str=NULL;
	UINT					uHighestBoundCol=0;

	if(!lpci)
		return;

	str = (LPTSTR)AllocateMemory(MAXOUTBUFF+1);
	if(!str)
		return;

	fBusy(TRUE);

	// Initialize local variables
	if(lpci && lpci->lpCurStmt) {
		hstmt = lpci->lpCurStmt->hstmt;
		uHighestBoundCol = lpci->lpCurStmt->uHighestBoundCol;
		}
	hwndOut = GETOUTPUTWINDOW(lpci);

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
			TRUE, TRUE, (LPTSTR)szGetDataRowTitle);

	// Get meta data information
	uDex = GetSqlFuncOffset(SQL_API_SQLNUMRESULTCOLS);
	rc = SQLNumResultCols(hstmt, &cCols);

	if(RC_NOTSUCCESSFUL(rc))
		goto exit01;

	// If cCols = 0, assume this is a non-select statement and just
	//	tell user how many rows were affected.
	else if (cCols == 0) {
		SQLLEN		sdRows=0;

		uDex = GetSqlFuncOffset(SQL_API_SQLROWCOUNT);
		rc = SQLRowCount(hstmt, &sdRows);

		if(RC_NOTSUCCESSFUL(rc))
			goto exit01;
		if(rc == SQL_SUCCESS_WITH_INFO) {
			DUMP_INFO;
			}
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				TRUE, TRUE,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsIUDAffectedRows),
				sdRows);
		}
	// else we have a results set to show the user
	else {
		// Find the highest bound column, if there is one, and tell user
		if(uHighestBoundCol > 0)
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1,
					TRUE, TRUE,
					GetGtrString(szOutBuff, MAXOUTBUFF, TST1005),
					uHighestBoundCol);

	   // For each column, get a column description and allocate a buffer for it
		uDex = GetSqlFuncOffset(SQL_API_SQLGETDATA);
	   for(dex=0;  dex<cCols;  dex++) {
			if(fAbortCheck(hwndOut))
				goto freemem;

			while((rc = SQLGetData(hstmt,
						(UWORD)(dex+1),
						SQL_C_CHAR,
						(PTR)str,
						MAXOUTBUFF,
						&cbValue)) == SQL_STILL_EXECUTING){
				if(fAbortCheck(hwndOut))
					goto exit01;
				}

			DoCheckRtnRow(lpci, rc, hstmt, uHighestBoundCol, hwndOut,
						str, (SDWORD)cbValue, SQL_C_CHAR);


			// Now do column separator or flush line if required
			if(dex < (cCols - 1))
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
						TRUE, TRUE, (LPTSTR)TEXT(", "));
			}

		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				TRUE, TRUE, (LPTSTR)TEXT("\r\n"));

		// Tell user how many rows were fetched
		// Bug #273 if udRows is either 0 or > 1 pluralize "row"
		//               if cCols is either 0 or > 1 pluralize "column"
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				TRUE, TRUE, GetGtrString(szOutBuff, sizeof(szOutBuff), idsRowCount),
				udRows, (!udRows) || (udRows>1) ? TEXT("s") : TEXT(""),
				cCols, (!cCols) || (cCols>1) ? TEXT("s") : TEXT(""));
		}	// end of a selection


	// Free allocated memory and return
	goto freemem;

	// Following is catch all for errors
exit01:
	DUMP_INFO;

	// Free all allocated memory
freemem:
	FREE_PTR(str);
	fBusy(FALSE);
}



//*---------------------------------------------------------------------------------
//| ResultsGetDataAll:
//|	This function will retrieve all data pending on an hstmt according to
//|	the following:
//|			SQLNumResultsCols
//|				SQLDescribeCol for each column in the results
//|				SQLFetch for each row
//|					SQLGetData for each column in current row
//|				SQLMoreResults to get next results
//| Parms:
//|	ci							Current connection information
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
#define MAXCOLNAME 					100
#define MAX_DFT_GETDATA_BUFF		100
#define MAXGETDATABUFF ((UWORD)(-1))

typedef struct tagGETINFOALL {
	TCHAR				szCol[MAXCOLNAME];			// Column name for display
	SWORD				fSqlType;						// For GetData call
	SQLULEN			cbValueMax;						// How much memory to allocate
	PTR				rgbValue;						// Pointer to memory
	} GETINFOALL;
typedef GETINFOALL * lpGETINFOALL;

VOID INTFUN ResultsGetDataAll(lpCONNECTIONINFO lpci)
{
	RETCODE				rc,
							rcSetPos=SQL_SUCCESS;
	HSTMT					hstmt=NULL;
	HWND					hwndOut;
	SWORD					cCols=0;
	SWORD					dex;
	UINT					uDex;
	SQLLEN				cbValue;
	UDWORD				udRows=0;
	UWORD					fMoreResultsSupported=FALSE;
	lpGETINFOALL		lpgi=NULL;
	LPTSTR				str=NULL;
	UINT					uHighestBoundCol=0;
	UINT					cbChar=sizeof(TCHAR);
	//SDWORD				nOldArraySize=0,
	//						nOldRowsetSize=0;
    SQLLEN nOldArraySize=0, nOldRowsetSize=0;

	if(!lpci)
		return;

	str = (LPTSTR)AllocateMemory(MAXOUTBUFF);
	if(!str)
		return;

	fBusy(TRUE);

	// Initialize local variables
	if(lpci && lpci->lpCurStmt)
	{
		hstmt = lpci->lpCurStmt->hstmt;
		uHighestBoundCol = lpci->lpCurStmt->uHighestBoundCol;
	}

	hwndOut = GETOUTPUTWINDOW(lpci);

	//Reset rowset sizes to 1
	rc=SQLGetStmtAttr(hstmt,SQL_ATTR_ROW_ARRAY_SIZE,&nOldArraySize,sizeof(nOldArraySize),NULL);
	rc=SQLGetStmtAttr(hstmt,SQL_ROWSET_SIZE,&nOldRowsetSize,sizeof(nOldArraySize),NULL);
	rc=SQLSetStmtAttr(hstmt,SQL_ATTR_ROW_ARRAY_SIZE,(PTR)1,0);
	rc=SQLSetStmtAttr(hstmt,SQL_ROWSET_SIZE,(PTR)1,0);

	// Check to see if SQLMoreResults is supported by this driver
	rc = SQLGetFunctions(lpci->hdbc, SQL_API_SQLMORERESULTS, &fMoreResultsSupported);

	if(RC_NOTSUCCESSFUL(rc))
		fMoreResultsSupported = FALSE;

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
			TRUE, TRUE, (LPTSTR)szGetDataAllTitle);

   // For each results set, get the meta data back then retrieve the results
   rc = SQL_SUCCESS;
   while(RC_SUCCESSFUL(rc))
	{

		// Get meta data information
		uDex = GetSqlFuncOffset(SQL_API_SQLNUMRESULTCOLS);
		rc = SQLNumResultCols(hstmt, &cCols);

		if(RC_NOTSUCCESSFUL(rc))
			goto exit01;
		else if (cCols == 0)
		{
			// since cCols == 0, we can assume this is a non-select statement and just
			//	tell user how many rows were affected.
			SQLLEN		sdRows=0;

			uDex = GetSqlFuncOffset(SQL_API_SQLROWCOUNT);
			rc = SQLRowCount(hstmt, &sdRows);

			if(RC_NOTSUCCESSFUL(rc))
				goto exit01;
			if(rc == SQL_SUCCESS_WITH_INFO)
			{
				DUMP_INFO;
			}

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
					TRUE, TRUE,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsIUDAffectedRows),
					sdRows);

		   // If we cannot get any more results sets, we must exit
			if(!fMoreResultsSupported)
	   		goto freemem;
		}
		// else we have a results set to show the user
		else
		{
			DWORD		dwReqdMem;

			dwReqdMem = sizeof(GETINFOALL) * (DWORD)cCols;	// Explicit promotion required
		   lpgi = (lpGETINFOALL)AllocateMemory(dwReqdMem);
		   if(!lpgi)
		   	goto freemem;

			// Find the highest bound column, if there is one, and tell user
			if(uHighestBoundCol > 0)
			{
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
						TRUE, TRUE,
						GetGtrString(szOutBuff, MAXOUTBUFF, TST1005),
						uHighestBoundCol);
			}

		   // For each column, get a column description and allocate a buffer for it
		   for(dex=0;  dex<cCols;  dex++)
			{
				uDex = GetSqlFuncOffset(SQL_API_SQLDESCRIBECOL);
		   	rc = SQLDescribeCol(hstmt,
		   				(UWORD)(dex+1),
		   				(LPTSTR)lpgi[dex].szCol,
		   				MAXCOLNAME, NULL,
		   				&lpgi[dex].fSqlType,
		   				&lpgi[dex].cbValueMax,
		   				NULL, NULL);

		   	if(RC_NOTSUCCESSFUL(rc))
		   		goto exit01;

				switch(lpgi[dex].fSqlType)
				{
					case SQL_BINARY:
					case SQL_VARBINARY:
					case SQL_LONGVARBINARY:
						// Binary types must allow for twice as much room for the char display
						if(lpgi[dex].cbValueMax==0)
							//Handle MAX 
							lpgi[dex].cbValueMax = 8000;
						else {
				   			lpgi[dex].cbValueMax *= (2 * cbChar)+cbChar;
				   			lpgi[dex].fSqlType = SQL_BINARY;
						}
						break;

					case SQL_CHAR:
					case SQL_VARCHAR:
					case SQL_LONGVARCHAR:
					case SQL_WCHAR:
					case SQL_WVARCHAR:
					case SQL_WLONGVARCHAR:
					{
						lpgi[dex].fSqlType = (fUnicode ? SQL_WCHAR : SQL_CHAR);
						// Worst case, each Unicode char maps to a double-byte char
						// Prevent overflow if value is half a gig or larger
						if (lpgi[dex].cbValueMax < 0x7fffffff)
						{
							lpgi[dex].cbValueMax *= 2;
							lpgi[dex].cbValueMax+=cbChar;
						}
						else
							lpgi[dex].cbValueMax = 0xffffffff;

					}

					break;

					default:
						// For other types, use a default buffer size
		   			lpgi[dex].cbValueMax = MAX_DFT_GETDATA_BUFF;

				} //switch(fSqlType)

				lpgi[dex].cbValueMax = min(lpgi[dex].cbValueMax, MAXGETDATABUFF);
		   	lpgi[dex].rgbValue = (PTR)AllocateMemory(lpgi[dex].cbValueMax);

				if(!lpgi[dex].rgbValue)
		   		goto exit01;

				// Print out the column header
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
							TRUE, TRUE, TEXT("\"%s\""), (LPTSTR)lpgi[dex].szCol);
				if(dex < (cCols - 1))
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
							TRUE, TRUE, TEXT(", "));

		   } //for(dex=0;  dex<cCols;  dex++)

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
					TRUE, TRUE, TEXT("\r\n"));


			// Now fetch each row and display the results
			uDex = GetSqlFuncOffset(SQL_API_SQLFETCH);
	   	while((rc = SQLFetch(hstmt)) == SQL_STILL_EXECUTING)
			{
	   		if(fAbortCheck(hwndOut))
	   			goto exit01;
	   	}

			//This loop was added for the SQLSetPos case
			while (RC_SUCCESSFUL(rc))
			{
				++udRows;
				uDex = GetSqlFuncOffset(SQL_API_SQLGETDATA);

				for(dex=0;  dex<cCols;  dex++)
				{
					if(fAbortCheck(hwndOut))
						goto freemem;

					while((rc = SQLGetData(hstmt,
	   						(UWORD)(dex+1),
	   						SQL_C_CHAR,
	   						lpgi[dex].rgbValue,
	   						lpgi[dex].cbValueMax,
	   						&cbValue)) == SQL_STILL_EXECUTING)
					{
			   		if(fAbortCheck(hwndOut))
			   			goto exit01;
	   			}

	   			DoCheckRtnRow(lpci, rc, hstmt, uHighestBoundCol, hwndOut,
	   						lpgi[dex].rgbValue, cbValue,
	   						lpgi[dex].fSqlType);

					// Now do column separator or flush line if required
	   			if(dex < (cCols - 1))
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
								TRUE, TRUE, (LPTSTR)TEXT(", "));

   			} //for(dex=0;  dex<cCols;  dex++)

				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
						TRUE, TRUE, (LPTSTR)TEXT("\r\n"));

	   		if(RC_NOTSUCCESSFUL(rc) && rc != SQL_NO_DATA_FOUND)
	   			goto exit01;

	   		// Get next row
				uDex = GetSqlFuncOffset(SQL_API_SQLFETCH);
		   	while((rc = SQLFetch(hstmt)) == SQL_STILL_EXECUTING)
				{
		   		if(fAbortCheck(hwndOut))
		   			goto exit01;
		   	}

			} //while (RC_SUCCESSFUL(rc))


	   	if(rc != SQL_NO_DATA_FOUND)
	   		goto exit01;

			// Tell user how many rows were fetched
			// Bug #273 if udRows is either 0 or > 1 pluralize "row"
			//               if cCols is either 0 or > 1 pluralize "column"
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
					TRUE, TRUE, GetGtrString(szOutBuff, sizeof(szOutBuff), idsRowCount),
					udRows, (!udRows) || (udRows>1) ? TEXT("s") : TEXT(""),
					cCols, (!cCols) || (cCols>1) ? TEXT("s") : TEXT(""));

			udRows = 0;
		}	// end of a selection

		if(fAbortCheck(hwndOut))
			goto freemem;


   	// If SQLMoreResults is supported by the driver, look for an
   	//		add'l results set.
   	if(fMoreResultsSupported)
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLMORERESULTS);

	   	while((rc = SQLMoreResults(hstmt)) == SQL_STILL_EXECUTING)
			{
	   		if(fAbortCheck(hwndOut))
	   			goto exit01;
	   	}

			if(RC_NOTSUCCESSFUL(rc) && rc != SQL_NO_DATA_FOUND)
	   		goto exit01;

			if(rc == SQL_SUCCESS_WITH_INFO)
			{
				DUMP_INFO;
			}
	   	else if(rc != SQL_NO_DATA_FOUND && lpgi)
			{
	   		// There was another results set, so free all allocated memory
	   		//		for last results set and get the next

				for(dex=0;  dex<cCols;  dex++)
				{
					FREE_PTR(lpgi[dex].rgbValue);
				}

				FREE_PTR(lpgi);
				lpgi = NULL;
			}

		} //if(fMoreResultsSupported)

	} //while(RC_SUCCESSFUL(rc))

   // Cancel and free if we got all results and MoreResults is not supported
	if(!fMoreResultsSupported)
	   SQLFreeStmt(hstmt, SQL_CLOSE);

	//Reset rowset sizes
	rc=SQLSetStmtAttr(hstmt,SQL_ATTR_ROW_ARRAY_SIZE,(PTR)(LONG_PTR)nOldArraySize,sizeof(nOldArraySize));
	rc=SQLSetStmtAttr(hstmt,SQL_ROWSET_SIZE,(PTR)(LONG_PTR)nOldRowsetSize,sizeof(nOldArraySize));

	// Free allocated memory and return
	goto freemem;

	// Following is catch all for errors
exit01:
	DUMP_INFO;

	// Free all allocated memory
freemem:

	FREE_PTR(str);
	if(lpgi)
	{
		for(dex=0;  dex<cCols;  dex++)
		{
			FREE_PTR(lpgi[dex].rgbValue);
		}

		FREE_PTR(lpgi);
	}
	fBusy(FALSE);
}


//*---------------------------------------------------------------------------------
//| FindBoundVar:
//|	Returns the BOUNDVARS node which corresponds to the icol required.
//| Parms:
//|	lpHead					First bound var node to look at
//|	cnt						How many bound nodes are there
//|	icol						Which column are we searching for
//| Returns:
//|	Pointer to the node if found, NULL else
//*---------------------------------------------------------------------------------
lpBOUNDVARS FindBoundVar(lpBOUNDVARS lpHead, UINT cnt, UWORD icol)
{
	lpBOUNDVARS lpbv=lpHead;

	if(!lpHead || !cnt)
		return NULL;

	// We have count of nodes, just look for the target and return it
	while(cnt--)
		if(lpbv->icol != icol)
			lpbv = lpbv->next;
		else
			return lpbv;

	return NULL;				// Wasn't found
}


//*---------------------------------------------------------------------------------
//| AdjustStmtBVMem:
//|	Based on uType, allocate or expand memory for a bound column node.
//|	We'll handle logging of rollback nodes as well.
//| Parms:
//|	uTask						What operation to take?
//|	lpStmt					The statement information
//|	lpbv						Bound var if adjusting existing
//|	cbValueMax				Max size of one element,
//|									Old rowset size for CWB_ROWCHANGE
//|	lprlbv					Pointer to rollback array
//|	prlbvdex					Pointer to index for rollback array
//| Returns:
//|	Valid pointer on success, NULL if no memory was allocated.  Caller
//|		must not issue error messages
//*---------------------------------------------------------------------------------
#define CHECK_MEM_BOUNDS(cbTot, hwnd) 											\
{																							\
	if(cbTot > MAX_ALLOCATED_BUFFER || cbTot <= 0) {    					\
		szPrintf(lpUsrOptions, hwnd,                          			\
					MSG_SEV0,                                       		\
					TRUE, TRUE,                                     		\
					GetGtrString(szOutBuff, MAXOUTBUFF, TST0008),   		\
					(MAX_ALLOCATED_BUFFER-1L));                     		\
		return NULL;                                             		\
		}                                                        		\
}
#define CHECK_OVERFLOW(cbRowset, cbValMax, cbTot, hwnd)					\
{                                                                    \
	if(((cbTot) / (cbValMax)) != (cbRowset)) {                        \
		szPrintf(lpUsrOptions, hwnd,                          			\
					MSG_SEV0,                                       		\
					TRUE, TRUE,                                     		\
					GetGtrString(szOutBuff, MAXOUTBUFF, TST0008),   		\
					(MAX_ALLOCATED_BUFFER-1L));                     		\
		return NULL;                                             		\
		}                                                              \
}
PTR AdjustStmtBVMem(UWORD uTask, lpSTATEMENTINFO lpStmt, lpBOUNDVARS lpbv,
			SDWORD cbValueMax, SDWORD * lpcbTotMem,
			UNALIGNED SDWORD * * lppcbValue, HWND hwndOut)
{
	SDWORD		cbTotMem=0;
	SDWORD		cbTotPcb=0;
	SDWORD		cbGrandTotal=0;
	PTR			rgbValue=NULL;
	PTR			rtnPtr=NULL;

	if(!lpStmt)
		return NULL;

	// We're allocating/adjusting memory
	if(lpStmt->fColBinding)
	{
		UDWORD	uSize,
					uRowSetSize=0;

		if (lpStmt->uEFRowSetSize > lpStmt->uFSFRowSetSize)
			uRowSetSize = lpStmt->uEFRowSetSize;
		else
			uRowSetSize = lpStmt->uFSFRowSetSize;

		uRowSetSize+=lpStmt->uReservedRows;

		// Sizes based on separate blocks of memory for column-wise //sumit
		if (lpbv && (lpbv->cbElement > cbValueMax))
			cbValueMax = lpbv->cbElement;

		uSize = (UDWORD)cbValueMax;
		cbTotMem = uSize * uRowSetSize;

		CHECK_OVERFLOW(uRowSetSize, uSize, (UDWORD)(cbTotMem), hwndOut);
		if(lppcbValue)
		{
			// If not null
			cbTotPcb = sizeof(SDWORD) * uRowSetSize;
			CHECK_OVERFLOW(uRowSetSize, sizeof(SDWORD), (UDWORD)(cbTotPcb), hwndOut);
		}

		cbGrandTotal = max(cbTotMem, cbTotPcb);

		// Do not try to allocate more than we can handle
		if(uTask != CWB_ROWCHANGE)
			CHECK_MEM_BOUNDS(cbGrandTotal, hwndOut);
	}


	//
	// Handle column-wise binding first.
	//
	if(lpStmt->fColBinding)
	{
		switch(uTask)
		{
			//Since the bound memory on a rebind is not used until the next
			//data fetch we will consider it a first time binding.
			//For first time, simply allocate the memory for both the data
			//		and the pcb value array.
			case CWB_REBIND:
			case CWB_FIRST_TIME:
				if(rtnPtr = rgbValue = AllocateAdjMem(cbTotMem))
				{
					if(lppcbValue &&										// User has valid pcbValue array
						sizeof(SDWORD) < cbTotPcb)
					{
						// But not large enough
						if(*lppcbValue)
							FreeAdjMem(*lppcbValue);				// Free old memory, not enough

						*lppcbValue = (SDWORD *)AllocateAdjMem(cbTotPcb);

						if(!*lppcbValue)
						{
							FreeAdjMem(rgbValue);
							goto nomem;
						}
					}
				}
				else
					goto nomem;

				if(lppcbValue && *lppcbValue)
					memset(*lppcbValue, 0, (size_t)cbTotPcb);

				break;

			// This task will only change the pcbValue due to the new rowset
			//		size.  CBW_REBIND should be used to adjust the rgbValue
			//		first.  Note that since we know how large an SDWORD is,
			//		cbValueMax contains our old rowset size for this task.
			case CWB_ROWCHANGE:
				{
				SDWORD		cbOld = (cbValueMax * sizeof(SDWORD)); // cbValueMax is overidden to old rowset size

				rtnPtr = rgbValue = lpbv->rgbValue;

				// If more memory is required, try to adjust the bound memory to a larger
				//		value.  If successful, add this change to the rollback list.
				if(cbTotMem > lpbv->cbTotMem)
				{
					// We required more than we have
					if(!ReAllocAdjMem(rgbValue, lpbv->cbTotMem, cbTotMem))
						goto nomem;

					// Add this to the rollback list.  If this doesn't work,
					//		undo the change immediately
					if(!AddCommitRollbackBVNode(lpStmt, rgbValue, lpbv->cbTotMem, cbTotMem))
					{
						ReAllocAdjMem(rgbValue, cbTotMem,
											lpbv->cbTotMem); 				// Free change as well
						goto nomem;
					}
				}

				// We need less memory.  Since we haven't finished yet, add a commit
				//		node so that if everything else is successful, we'll shrink any
				//		uneeded memory.
				else
				{
					if(!AddCommitRollbackBVNode(lpStmt, rgbValue, lpbv->cbTotMem, cbTotMem))
						goto nomem;
				}

				lpbv->cbTotMem = cbTotMem;

				rtnPtr = (PTR)TRUE;											// nomem takes care of failure
				if(*lppcbValue)
				{
					// User has valid pcb array
					// We need more memory for the pcb value array.  Allocate more and
					//		add a rollback node.
					if(cbOld < cbTotPcb)
					{                             // Need more cb values
						if(!ReAllocAdjMem(*lppcbValue,
										cbOld,										// From oldrowset * sizeof
										cbTotPcb))									// To new required size
							goto nomem;

						// Add the changed pcb allocation to the rollback list.  If
						//		this fails, reverse the allocate immediately
						if(!AddCommitRollbackBVNode(lpStmt, (LPVOID)*lppcbValue, cbOld, cbTotPcb))
						{
							ReAllocAdjMem(*lppcbValue, cbTotPcb, cbOld);
							goto nomem;
						}
					}
					// We need less memory.  Record a commit node and wait for success
					else
					{
						if(!AddCommitRollbackBVNode(lpStmt, rgbValue, lpbv->cbTotMem, cbTotMem))
							goto nomem;
					}
				}
			}
			break;
			}

		// Save values and return
		ASSERTCHECK((!cbTotMem), TEXT("Should not be setting total to 0"));
		*lpcbTotMem = cbTotMem;
		return rtnPtr;
		}		// End of column-wise

	//
	// Now do row-wise
	//
	else {
		switch(uTask) {
			// First time will require larger block of memory, so alloc/realloc
			//	main pointer
			case CWB_FIRST_TIME:
				{
				SDWORD		cbSum;

				// Figure out total required mem
				cbSum = cbValueMax + (lppcbValue ? sizeof(SDWORD) : 0);
				cbGrandTotal = cbSum * TOT_ROWSET_SIZE(lpStmt);
				CHECK_OVERFLOW(TOT_ROWSET_SIZE(lpStmt), (UDWORD)cbSum,
								cbGrandTotal, hwndOut);
				CHECK_MEM_BOUNDS(cbGrandTotal, hwndOut);

				// Need a row-wise node to start
				if(!lpStmt->lpRowWise) {
					lpStmt->lpRowWise = (lpROWWISEINFO)AllocateMemory(sizeof(ROWWISEINFO));
					if(!lpStmt->lpRowWise)
						goto nomem;
					}

				// We already have total mem reuqired (rgb & pcb) so allocate
				if(rgbValue = AllocateAdjMem(cbGrandTotal)) {
					lpStmt->lpRowWise->rgbValue = rtnPtr = rgbValue;

					// If we want a pcbValue, it is located immediately after
					//	the rgb element for this column
					if(lppcbValue) {
						LPBYTE lpAddr=rgbValue;

						lpAddr += cbValueMax;
						*lppcbValue = (SDWORD *)lpAddr;
						}
					}
				else
					goto nomem;
				}
				break;


			// Rebinds are allowed only if it is the last column which is being
			//	rebound.  Prior columns could cause an overlap which is an error.
			case CWB_REBIND:
				{
				SDWORD		cbTotal;

				rtnPtr = rgbValue = lpStmt->lpRowWise->rgbValue;

				// See if there is any change for this bound node.  If not,
				//	let it proceede
				if(lpbv && cbValueMax == lpbv->cbElement)
					if((lpbv->pcbValue && lppcbValue && *lppcbValue) ||
					  	(!lpbv->pcbValue && !lppcbValue))
					return rtnPtr;

				// Check column order; you may not rebind a column unless it is
				//	the last in the list, or overlap could occur
				if(lpbv && lpStmt->lpVars->prev != lpbv) {
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
									GetGtrString(szOutBuff, MAXOUTBUFF, TST0011));
					return NULL;
					}

				// Figure out total required mem
				cbTotal = SumBoundCols(lpStmt);								// Size of one old row
				if(lpbv) {
					cbTotal -= lpbv->cbElement;								// Take out this column
					if(lpbv->pcbValue)
						cbTotal -= sizeof(SDWORD);								// And pcb if specified
					}

				// Add in the rebound sizes
				cbTotal += cbValueMax;
				cbTotal += (lppcbValue ? sizeof(SDWORD) : 0);
				cbGrandTotal = cbTotal * TOT_ROWSET_SIZE(lpStmt);
				CHECK_OVERFLOW(TOT_ROWSET_SIZE(lpStmt), cbTotal, (UDWORD)(cbGrandTotal), hwndOut);
				CHECK_MEM_BOUNDS(cbGrandTotal, hwndOut);

				// If we need more memory to fulfill this request, then try to make
				//	memory larger.
				if(cbGrandTotal > lpStmt->lpRowWise->cbTotMem) {
					if(!ReAllocAdjMem(rgbValue, lpStmt->lpRowWise->cbTotMem, cbGrandTotal))
						goto nomem;
					// Add this to the rollback list.  If this doesn't work,
					//		undo the change immediately
					if(!AddCommitRollbackBVNode(lpStmt, rgbValue,
									lpStmt->lpRowWise->cbTotMem, cbGrandTotal)) {
						ReAllocAdjMem(rgbValue, cbGrandTotal,
											lpStmt->lpRowWise->cbTotMem); 				// Free change as well
						goto nomem;
						}
					}
				// We need less memory.  Since we haven't finished yet, add a commit
				//		node so that if everything else is successful, we'll shrink any
				//		uneeded memory.
				else {
					if(!AddCommitRollbackBVNode(lpStmt, rgbValue,
									lpStmt->lpRowWise->cbTotMem, cbGrandTotal))
						goto nomem;
					}

            // If there is no lpbv, then we just added a new node and must therefore
            //	set our pointers for return.
            if(!lpbv) {
            	LPBYTE		lpAddr=rgbValue;
            	lpAddr += cbTotal - sizeof(SDWORD) - cbValueMax;
            	rtnPtr = (PTR)lpAddr;

            	if(lppcbValue) {
            		lpAddr = rgbValue;
            		lpAddr += cbTotal - sizeof(SDWORD);
            		*lppcbValue = (SDWORD *)lpAddr;
            		}
            	}
            }
            break;


			// Rowchange is used to change the rowset size.
			case CWB_ROWCHANGE:
				{
				SDWORD		sdSum=SumBoundCols(lpStmt);
				SDWORD		cbOldRowSetSize=cbValueMax;				// cbValueMax is overidden to old rowset size by caller
				SDWORD		cbFrom=(lpStmt->lpRowWise->cbTotMem * cbOldRowSetSize);			// From old total mem
				SDWORD		cbTo=(sdSum * TOT_ROWSET_SIZE(lpStmt));

				CHECK_OVERFLOW((UDWORD)(cbTo), (SDWORD)(sdSum),
								TOT_ROWSET_SIZE(lpStmt), hwndOut);

				CHECK_MEM_BOUNDS(cbTo, hwndOut);

				rtnPtr = rgbValue = lpStmt->lpRowWise->rgbValue;
				cbGrandTotal = cbTo;

            // We need more memory if we've increased rowset size
				if(cbOldRowSetSize < (SDWORD)TOT_ROWSET_SIZE(lpStmt)) {
					if(!ReAllocAdjMem(rgbValue, cbFrom, cbTo))
						goto nomem;

					// Add this change to the rollback list in case we are forced
					//	to back it out.
					if(!AddCommitRollbackBVNode(lpStmt,
								rgbValue, cbFrom, cbTo)) {
						ReAllocAdjMem(rgbValue, cbTo, cbFrom);			// Back out change
						goto nomem;
						}
					}
				// We don't need as much memory
				else {
					if(!AddCommitRollbackBVNode(lpStmt, rgbValue, cbFrom, cbTo))
						goto nomem;
					}
				}
				break;


			// Unbind is only called for row-wise binding.  We need to take
			//	the current lpbv and remove it from the memory.
			case CWB_UNBIND:
				{
				SDWORD		cbTotal;

				rtnPtr = rgbValue = lpStmt->lpRowWise->rgbValue;

				// Figure out total required mem
				cbTotal = SumBoundCols(lpStmt);							// Size of one old row
				cbTotal -= lpbv->cbElement;								// Take out this column
				if(lpbv->pcbValue)
					cbTotal -= sizeof(SDWORD);								// And pcb if specified

				cbGrandTotal = cbTotal * TOT_ROWSET_SIZE(lpStmt);

				// We need less memory.
				ReAllocAdjMem(rgbValue, lpStmt->lpRowWise->cbTotMem, cbGrandTotal);
				}
				break;
			}	// end of switch on types

		ASSERTCHECK((!cbGrandTotal), TEXT("Should not be setting total to 0"));
		lpStmt->lpRowWise->cbTotMem = cbGrandTotal;
		return rtnPtr;
		}	// end of row-wise binding

	ASSERTCHECK((TRUE), TEXT("Unhandled bind type"));

nomem:
	szPrintf(lpUsrOptions, hwndOut,
				MSG_SEV0,
				TRUE, TRUE,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsBindMemFailure),
				cbTotMem);
	szPrintf(lpUsrOptions, hwndOut,
				MSG_SEV0,
				TRUE, TRUE,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsAborted));
	return NULL;
}


//*---------------------------------------------------------------------------------
//| ResetStmtBVMem:
//|	Reset all bound nodes with the new rowset size.  If this fails, the user
//|	will be warned and all changes for this hstmt will be rolled back.
//| Parms:
//|	lpStmt					The statement information to hang the bound info from]
//|	hwndOut					Output window for errors
//|	cbOldRowSet				Old rowset size
//| Returns:
//|	TRUE if success, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ResetStmtBVMem(lpSTATEMENTINFO lpStmt, HWND hwndOut, UDWORD uOldRowSet)
{
	lpBOUNDVARS			lpbv=lpStmt->lpVars;
	SWORD					sCnt=0;
	BOOL					fRtn=TRUE;						// Assume it works
	SDWORD				cbTotMem;

	if(!lpStmt->uBoundCols)
		return TRUE;

	// TST0016: If there are any bound parameters on this rowset, then
	//				we cannot allow the rowset size to be changed as they
	//				could become invalid.
	if(IsBoundParamsOnRowset(lpStmt))
	{
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
						GetGtrString(szOutBuff, MAXOUTBUFF, TST0016));
		return FALSE;
	}

	// For row-wise binding, all memory is in flat address, so only one
	//	rebind is required
	if(!lpStmt->fColBinding)
	{
		if(!AdjustStmtBVMem(CWB_ROWCHANGE, lpStmt, lpbv,
						uOldRowSet, NULL,
						NULL, hwndOut))
		{
			fRtn = FALSE;
		}

		goto done;
	}

	// For column-wise binding, we must go through each bound var
	//	and realloc it
	sCnt = (SWORD)lpStmt->uBoundCols;
	while(sCnt--)
	{
		// Only bound columns are displayed
		if(lpbv->fBound)
		{
			if (!lpbv->cbElement)
				lpbv->cbElement=1;

			// Adjust the rgbValue first
			if(!AdjustStmtBVMem(CWB_ROWCHANGE, lpStmt, lpbv,
							uOldRowSet, &cbTotMem,
							&lpbv->pcbValue, hwndOut))
			{
				fRtn = FALSE;
				goto done;
			}
		}

		lpbv = lpbv->next;
		}

done:
	// If we've failed, then rollback all changes and return
	if(!fRtn)
		RollbackBVChanges(lpStmt);

	// Either way free the rollback list
	FreeRollbackBV(lpStmt);
	return fRtn;
}


//*---------------------------------------------------------------------------------
//| AddCommitRollbackBVNode:
//|	The rollback nodes are used for two purposes:
//|		ROLLBACK:	cbNewSize > cbOldSize
//|			If more memory is required than is currently allocated, then more
//|			memory is allocated and a rollback node is allocated.  This way
//|			if anything fails, the memory changes can be rolled back to the
//|			smaller size.
//|		COMMIT:		cbNewSize < cbOldSize
//|			The other case is where we have more memory than required.  Rather
//|			than shrinking the memory, we'll add a commit node which records the
//|			size changes but does nothing.  If all requests for additional memory
//|			are successful, then the shrinkage can occur.
//|	The two types are distignuished by simply comparing the new to old size.
//| Parms:
//|	lpStmt					The statement information to hang the bound info from
//|	lpData					The base pointer to adjust (rgb or pcb)
//|	cbOldSize				The old total bytes
//|	cbNewSize				New total
//| Returns:
//|	TRUE if success, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN AddCommitRollbackBVNode(lpSTATEMENTINFO lpStmt, LPVOID lpData,
						SDWORD cbOldSize, SDWORD cbNewSize)
{
	DWORD		cbCurrent;					// Number of nodes of size ROLLBACK_SIZE

	cbCurrent = (lpStmt->uRollBack) / ROLLBACK_SIZE +
					(((lpStmt->uRollBack) % ROLLBACK_SIZE) ? 1 : 0);

	// If this is a new list, allocate first block
	if(!lpStmt->lpRollBack)	{
		if(!(lpStmt->lpRollBack =
						(lpROLLBACKBV)AllocateAdjMem(sizeof(ROLLBACKBV) * ROLLBACK_SIZE)))
			return FALSE;
		}
	// Else see if we need a new block
	else if(cbCurrent * ROLLBACK_SIZE <
			  lpStmt->uRollBack + 1) {							// Need more room
		DWORD		cbBlockSize = ROLLBACK_SIZE * sizeof(ROLLBACKBV);

		if(!ReAllocAdjMem(lpStmt->lpRollBack,
					cbCurrent * cbBlockSize,
					(cbCurrent + 1) * cbBlockSize))
			return FALSE;
		}


	// Track this new node
	lpStmt->lpRollBack[lpStmt->uRollBack].lpData = lpData;
	lpStmt->lpRollBack[lpStmt->uRollBack].cbOldSize = cbOldSize;
	lpStmt->lpRollBack[lpStmt->uRollBack].cbNewSize = cbNewSize;
	++lpStmt->uRollBack;

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| RollbackBVChanges:
//|	If a failure occurs while adjusting memory, this function can be called
//|	to reverse all changes back to their original states.
//| Parms:
//|	lpStmt					The statement information to hang the bound info from
//| Returns:
//|	TRUE if success, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN RollbackBVChanges(lpSTATEMENTINFO lpStmt)
{
	lpROLLBACKBV		lprlbv;

	if(!lpStmt || !lpStmt->lpRollBack)
		return FALSE;

	// For each node, reverse the process.  Since these will be releasing
	//		memory, there should be no errors and we wouldn't know what to
	//		do with one if there was one.
	while(lpStmt->uRollBack--) {
		lprlbv = &lpStmt->lpRollBack[lpStmt->uRollBack];
		if(lprlbv->cbNewSize > lprlbv->cbOldSize) // It's a rollback node
			ReAllocAdjMem(lprlbv->lpData,				// Change rgbValue allocation
								lprlbv->cbNewSize,		// From new size
								lprlbv->cbOldSize);		// Back to old size
		}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| CommitBVChanges:
//|	If all requests for add'l memory were successful, and everything has
//|	worked correctly, this function will go through and find all changes
//|	which require less memory and make those adjustments.  Since shrinking
//|	memory should not fail, we're safe to do this at the end.  This way
//|	no memory is shrank prematuraly which could cause data loss.
//| Parms:
//|	lpStmt					The statement information to hang the bound info from
//| Returns:
//|	TRUE if success, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN CommitBVChanges(lpSTATEMENTINFO lpStmt)
{
	lpROLLBACKBV		lprlbv;

	if(!lpStmt || !lpStmt->lpRollBack)
		return FALSE;

	// For each commit node, change the size to the smaller amount.  This
	//		should never fail and if it does (why would it?) we'll ignore
	//		the failure and just keep the extra memory around.
	while(lpStmt->uRollBack--) {
		lprlbv = &lpStmt->lpRollBack[lpStmt->uRollBack];
		if(lprlbv->cbNewSize < lprlbv->cbOldSize) // It's a commit node
			ReAllocAdjMem(lprlbv->lpData,				// Change rgbValue allocation
								lprlbv->cbOldSize,		// From old size
								lprlbv->cbNewSize);		// To new, smaller size
		}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| FreeRollbackBV:
//|	Clean up the rollback nodes.
//| Parms:
//|	lpStmt					The statement information to hang the bound info from
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN FreeRollbackBV(lpSTATEMENTINFO lpStmt)
{
	if(!lpStmt || !lpStmt->lpRollBack)
		return;

	FreeAdjMem(lpStmt->lpRollBack);
	lpStmt->uRollBack = 0;
	lpStmt->lpRollBack = NULL;
}


//*---------------------------------------------------------------------------------
//| AddBoundColumn:
//|	This function will add or update a bound column node for the current
//|	statement.  If this affects the results set, it will be redrawn.
//| Parms:
//|	lpStmt					The statement information to hang the bound info from
//|	icol						Which column are we updating
//|	fCType					Type of the bound data
//|	cbElement				How be is each member
//|	rgbValue					Pointer to allocated data (NULL means unbind)
//|	cbTotValueMax			Total bytes in rgbValue (relevent for rowset>1)
//|	pcbValue					Memory for length array
//| Returns:
//|	TRUE if the node is successfully added/updated
//*---------------------------------------------------------------------------------
BOOL INTFUN AddBoundColumn(lpSTATEMENTINFO lpStmt, UWORD icol, SWORD fCType,
					SDWORD cbElement, PTR rgbValue, SDWORD cbTotValueMax, SDWORD * pcbValue,
					UWORD uType)
{
	lpBOUNDVARS			lpbv=lpStmt->lpVars;
	UINT					uHighestBoundCol;


	// If there are no nodes, then place at head of list
	if(!lpbv)
	{
		lpStmt->lpVars = lpbv = (lpBOUNDVARS)AllocateMemory(sizeof(BOUNDVARS));
		if(!lpbv)
			return FALSE;
		lpbv->next = lpbv;
		lpbv->prev = lpbv;

		//uBoundCols represents columns that have been bound but haven't been fetched
		++lpStmt->uBoundCols;

		++lpbv->uRgbLockCnt;
		++lpbv->uPcbLockCnt;
		if(lpStmt->lpRowWise)
			++lpStmt->lpRowWise->uRgbLockCnt;
	}
	// Search the list for an exising node for this column
	else
	{
		lpbv = FindBoundVar(lpStmt->lpVars, lpStmt->uBoundCols, icol);

		// If the node wasn't found, create a new one
		if(!lpbv)
		{
			UINT					ucnt=lpStmt->uBoundCols;
			lpBOUNDVARS			lptmp=lpStmt->lpVars;

			lpbv = (lpBOUNDVARS)AllocateMemory(sizeof(BOUNDVARS));
			if(!lpbv)
				return FALSE;

			++lpbv->uRgbLockCnt;				// Record our usage
			++lpbv->uPcbLockCnt;
			if(lpStmt->lpRowWise)
				++lpStmt->lpRowWise->uRgbLockCnt;

			++lpStmt->uBoundCols;

			// For column-wise, keep nodes in order
			if(lpStmt->fColBinding)
			{
				// Need to insert node as head node
				if(icol < lptmp->icol)
				{
					lpStmt->lpVars = lpbv;
					lptmp = lptmp->prev;
				}
				else
				{
					// Insert new node into the list, after the last node of a lower column number
					while(--ucnt)
					{
						if(icol > lptmp->next->icol)
							lptmp = lptmp->next;
					}
				}
			}
			// Row-wise needs to stay in order of rgb address
			else
				lptmp = lptmp->prev;

			lpbv->next = lptmp->next;
			lpbv->prev = lptmp;
			lptmp->next->prev = lpbv;
			lptmp->next = lpbv;
		}
	}

	// Set all fields of this node
	lpbv->icol = icol;
	lpbv->fBound = (rgbValue == NULL) ? FALSE : TRUE;
	lpbv->uType=uType;

	if (lpbv->uType == CWB_FIRST_TIME)
	{
		lpbv->fCType=fCType;
		lpbv->cbElement=cbElement;
		lpbv->rgbValue=rgbValue;
		lpbv->pcbValue=pcbValue;
		lpbv->cbTotMem=cbTotValueMax;
	}
	else
	{
		lpbv->fCTypeNew = fCType;
		lpbv->cbElementNew = cbElement;
		lpbv->rgbValueNew = rgbValue;
		lpbv->pcbValueNew = pcbValue;
		lpbv->cbTotMemNew = cbTotValueMax;
	}

	lpbv->lpStmt = (LPVOID)lpStmt;

	// Now look for the highest bound column and record
	lpbv=lpStmt->lpVars;
	uHighestBoundCol = lpbv->icol;
	while(lpbv->next != lpStmt->lpVars)
	{
		lpbv = lpbv->next;

		// New highest column must be bound
		if(lpbv->icol > uHighestBoundCol &&	lpbv->fBound)
			uHighestBoundCol = lpbv->icol;
	}

	lpStmt->uHighestBoundCol = uHighestBoundCol;

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| FreeBoundColumn:
//|	This function will free the given bound column node from the list of bound
//|	columns.
//| Parms:
//|	lpStmt					The statement node to keep track of head
//|	lpbv						Pointer to the bound node to free
//| Returns:
//|	The next valid node
//*---------------------------------------------------------------------------------
lpBOUNDVARS INTFUN FreeBoundColumn(lpSTATEMENTINFO lpStmt, lpBOUNDVARS lpbv)
{
	lpBOUNDVARS lpNextBv=lpbv->next, //return next valid node
					lpPrevBv=lpbv->prev;

	// Removing the head node?
	if(lpbv == lpStmt->lpVars)
	{
		if(lpStmt->lpVars == lpStmt->lpVars->next)			// Only one node anyway
			lpStmt->lpVars = NULL;
		else
			lpStmt->lpVars = lpStmt->lpVars->next;			// Pass the buck
	}

	// Remove the node from the list then free its memory
	lpPrevBv->next=lpNextBv;
	lpNextBv->prev=lpPrevBv;

	// For row-wise, we check the global lock count
	//	and only free the base pointer
	if(!lpStmt->fColBinding)
	{
		--lpStmt->lpRowWise->uRgbLockCnt;

		if(lpStmt->lpRowWise->rgbValue && !lpStmt->lpRowWise->uRgbLockCnt)
		{
			FreeAdjMem(lpStmt->lpRowWise->rgbValue);
			lpStmt->lpRowWise->rgbValue = NULL;
		}

		// If no memory is being used, then free the node
		if(!lpStmt->lpRowWise->uRgbLockCnt)
		{
			FREE_PTR(lpbv);
			FREE_PTR(lpStmt->lpRowWise);
		}
		else
			lpbv->next =
			lpbv->prev = NULL;
	}
	// For column-wise, we must check the rgb and pcb values
	//	seperately from each other
	else
	{
		// Don't actually free the bound node if there is still a lock count,
		//	meaning someone has bound a parameter to the node
		--lpbv->uRgbLockCnt;

		if(lpbv->rgbValue && !lpbv->uRgbLockCnt)
		{
			FreeAdjMem(lpbv->rgbValue);
			lpbv->rgbValue = NULL;
		}

		// Release pcb mem if it exists and no one else is using it
		--lpbv->uPcbLockCnt;
		if(lpbv->pcbValue && !lpbv->uPcbLockCnt)
		{
			FreeAdjMem(lpbv->pcbValue);
			lpbv->pcbValue = NULL;
		}

		// If no memory is being used, then free the node
		if(!lpbv->uRgbLockCnt && !lpbv->uPcbLockCnt)
		{
			FREE_PTR(lpbv);
		}
		else
		{
			// Else orphan the memory for the parameter stuff
			lpbv->next =
			lpbv->prev = NULL;
			lpbv->lpStmt = NULL;
		}
	}

	// Reduce counts of bound nodes and bound cols
	--lpStmt->uBoundNodes;

	if(lpStmt->uBoundCols)
		--lpStmt->uBoundCols;

	if(!lpStmt->uBoundCols) 				// No more bound nodes
		lpStmt->uHighestBoundCol = 0;

	return(lpNextBv);

} //FreeBoundColumn()


//*---------------------------------------------------------------------------------
//| IsBoundColumn:
//|	Returns TRUE if the specified column is a valid bound column on
//|	the current hstmt.
//| Parms:
//|	uCol						The column to check
//|	lpStmt					The statement handle
//| Returns:
//|	Pointer to bound var if found, NULL if not valid bound node
//*---------------------------------------------------------------------------------
lpBOUNDVARS IsBoundColumn(UWORD uCol, lpSTATEMENTINFO lpStmt)
{
	UINT			uNodes = lpStmt->uBoundCols;
	lpBOUNDVARS	lpbv=lpStmt->lpVars;


	// For each bound node, see if it matches
	while(uNodes) {
		ASSERTCHECK(!lpbv, TEXT("Should not have NULL bound var list"));
		if(lpbv->fBound) {
			uNodes--;

			if(lpbv->icol == uCol)
				return lpbv;
			}
		lpbv = lpbv->next;
		}

	return NULL;
}


//*---------------------------------------------------------------------------------
//| SumBoundCols:
//|	Return the sum of bound columns for a statement.
//| Parms:
//|	lpStmt					The statement handle
//| Returns:
//|	Sum of cbElement and pcbValue if not null
//*---------------------------------------------------------------------------------
UDWORD INTFUN SumBoundCols(lpSTATEMENTINFO lpStmt)
{
	UINT				uNodes = lpStmt->uBoundCols;
	lpBOUNDVARS		lpbv=lpStmt->lpVars;
	UDWORD			uSum=0;

	// For each bound node, see if it matches
	while(uNodes--) {
	   ASSERTCHECK(!lpbv, TEXT("Should not have null list"));
		if(lpbv->fBound) {
			uSum += lpbv->cbElement;
			if(lpbv->pcbValue)
				uSum += sizeof(SDWORD);
			}
		lpbv = lpbv->next;
		}

	return uSum;
}


//*---------------------------------------------------------------------------------
//| IsBoundParamsOnRowset:
//|	Given a statement node, return TRUE if there are parameters bound on the
//|	rowset.  Do this based on the lock counts.
//| Parms:
//|	lpStmt					The statement handle
//| Returns:
//|	TRUE if there are bound params, FALSE if not
//*---------------------------------------------------------------------------------
BOOL INTFUN IsBoundParamsOnRowset(lpSTATEMENTINFO lpStmt)
{
	UINT				uNodes;
	lpBOUNDVARS		lpbv;

	if(!lpStmt)
		return FALSE;

	uNodes = lpStmt->uBoundCols;
	lpbv = lpStmt->lpVars;

	if(!lpbv || !uNodes)
		return FALSE;

	// For row-wise, we keep our locks in the rowise struct and there will
	//	be one count per column bound.
	if(!lpStmt->fColBinding) {
		if(!lpStmt->lpRowWise)
			return FALSE;
		// More locks than columns means params are using memory
		else if(lpStmt->lpRowWise->uRgbLockCnt >
				  lpStmt->uBoundCols)
			return TRUE;
		}

	// For each bound node, see if there are more than 1 locks
	while(uNodes--) {
		if(lpbv->fBound) {
			if(lpbv->uRgbLockCnt > 1 ||
				lpbv->uPcbLockCnt > 1)
				return TRUE;
			}
		lpbv = lpbv->next;
		}

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| ResultsShowBoundCols:
//|	This function will walk the bound column list and dispaly all bound
//|	information.
//| Parms:
//|	ci							The connection info window to use
//|	lpbv						Pointer to the bound node to free
//|	rc							The return code specified or NO_RETCODE_CHECK
//|	uCol						Column number to display (-1 for all)
//|	fShowColHdr				TRUE to show col headers
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN ResultsShowBoundCols(lpCONNECTIONINFO ci, lpBOUNDVARS lpbv, RETCODE rc,
						UWORD uCol, BOOL fShowColHdr)
{
	HWND					hwnd=GETOUTPUTWINDOW(ci);
	UINT					dex;
	TCHAR					szCType[35];
	SWORD					sCnt=(SWORD)ci->lpCurStmt->uBoundCols;

	if(ci->lpCurStmt->uBoundCols && fShowColHdr)
		szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
					(LPTSTR)szShowBoundColsTitle);

	while(sCnt--) {
		if(fAbortCheck(hwnd))
			return;
		// Only bound columns are displayed
		if(lpbv->fBound &&										// Must be bound
			(uCol == (UWORD)-1 ||								// And either we want to display all columns
			 lpbv->icol == uCol)) {								// 	or this column matches
			// Look-up the fCType and format
			for(dex=0;  dex<ciCTypes;  dex++)
				if(lpbv->fCType == (SWORD)lpdftCTypes[dex].fOpt) {
					GetExpandedConstant(szCType, &lpdftCTypes[dex], FALSE);
					break;
					}

			szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
					(LPTSTR)szShowBoundColsData, lpbv->icol,
					szCType, lpbv->cbElement);

			// pcbValue may be NULL or valid
			if(lpbv->pcbValue) {
				SDWORD FixUnalign = *lpbv->pcbValue;
				szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
				 	TEXT("%ld, "), FixUnalign ); //*lpbv->pcbValue);
				}
			else
				szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
					TEXT("NULL, "));

			// rgbValue may be NULL or valid
			if(!lpbv->rgbValue)
				szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
					TEXT("NULL\r\n"));
			else {
				UDWORD		cbOutTot=(UDWORD)lpbv->cbTotMem;

				if(lpbv->fCType == SQL_C_BINARY)
					++cbOutTot;
				FormatValueBasedOnType(szOutBuff, MAXOUTBUFF-3,
						lpbv->rgbValue, lpbv->fCType, cbOutTot,hwnd);

				szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
						TEXT("%s\r\n"), (LPTSTR)szOutBuff);
				}

			// Look for any truncation
			if ((IsCharData(lpbv->fCType) || (lpbv->fCType == SQL_C_BINARY)) &&
				lpbv->pcbValue && rc != NO_RETCODE_CHECK)
			{

				// Check Null termination
				NullTerminationCheck(ci, TEXT("rgbValue"), lpbv->rgbValue,
					lpbv->cbElement, *lpbv->pcbValue, FALSE, 0);

				TruncationCheck(rc, hwnd,
						(LPTSTR)lpbv->rgbValue,
						lpbv->cbElement,
						*lpbv->pcbValue,
						TEXT("pcbValue"));
			}


			// No need to continue if we only print one row
			if(uCol != (UWORD)-1)
				return;
			}
		lpbv = lpbv->next;
		}
}


//*---------------------------------------------------------------------------------
//| ResultsBindColAll:
//|	Using SQLDescribeCol, go through each column and do a bind for it.
//| Parms:
//|	lpStmt					The statement to do the bind on
//| Returns:
//|	TRUE if successful, FALSE if not
//*---------------------------------------------------------------------------------
extern lpDFTARRAY  	lpdftCTypes;
extern UINT  			ciCTypes;
PARAMITEM lpmtSQLBindCol[] = {
// szName	uParmType						fCType         	dwDex	fNull	cBytes	lpData 	cDfts								lpDfts
//	------	--------------------------	-----------------	-----	-----	--------	--------	--------------------------	----------------
	NULL,		PT_HSTMT | PT_LOG_IN,		0,						0,		0,		0,			NULL,		0,									NULL, 0,	 0, 0,		NULL, NULL,
	NULL,		PT_LOG_IN,						SQL_C_USHORT,		0,		0,		0,			NULL,		0,									NULL,	0,	 0,0,			NULL, NULL,
	NULL,		PT_LOG_IN,						SQL_C_SSHORT,		0,		0,		0,			NULL,		0,									NULL,	0,	 0,0,			NULL, NULL,
	NULL,		PT_PTR_PTR |
					PT_LOG_IO,					0,						0,		0,		0,			NULL,		0,									NULL,	0,	 0,0,			NULL, NULL,
	NULL,		PT_LOG_IN,						SQL_C_SLONG,		0,		0,		0,			NULL,		0,									NULL,	0,	 0,0,			NULL, NULL,
	NULL,		PT_PTR_SDWORD |
					PT_LOG_IO,					SQL_C_SLONG,		0,		0,		0,			NULL,		0,									NULL,	0,	 0,0,			NULL, NULL,
	};
BOOL INTFUN ResultsBindColAll(lpCONNECTIONINFO lpci)
{
	lpSTATEMENTINFO	lpStmt;
	UWORD					dex,
							dexStart;
	RETCODE					rc;
	HWND					hwndOut;
	UINT					uDex;
	SWORD					cCols=0;
	SWORD					fSqlType;
	SWORD					fCType;
	SQLULEN					cbValueMax;
	BOOL					fHdr=TRUE;
	lpPARAMITEM				lplpSQLBindCol[(NumItems(lpmtSQLBindCol))];
	SWORD					fUnsigned=0;
	BOOL					fBindBkMkCol=FALSE;
	TCHAR					szDriverVer[10];
	SWORD					cbDriverVer;


	fBusy(TRUE);


	hwndOut = GETOUTPUTWINDOW(lpci);
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			GetGtrString(szOutBuff, MAXOUTBUFF, idsBindColAllTitle));

	// Free previously bound columns
	lpStmt = lpci->lpCurStmt;
	uDex = GetSqlFuncOffset(SQL_API_SQLFREESTMT);
	rc = SQLFreeStmt(lpStmt->hstmt, SQL_UNBIND);

	if(RC_NOTSUCCESSFUL(rc))
		goto exit01;

	if(lpStmt->lpResults)
	{
		DestroyResultWindow((lpRESULTWININFO)lpStmt->lpResults,
			TRUE);
		lpStmt->lpResults = NULL;
	}

	while(lpStmt->lpVars)
		FreeBoundColumn(lpStmt, lpStmt->lpVars->next);

	// Get meta data information
	uDex = GetSqlFuncOffset(SQL_API_SQLNUMRESULTCOLS);
	rc = SQLNumResultCols(lpStmt->hstmt, &cCols);
	if(RC_NOTSUCCESSFUL(rc))
 		goto exit01;

	if(cCols <= 0)
	{
		// Nothing to bind
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				TRUE, TRUE,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsNoColsToBind));
		return FALSE;
	}

	// Fill out the structure correctly
	lpmtSQLBindCol[0].lpData = &lpStmt->hstmt;
	lpmtSQLBindCol[1].lpData = &dex;
	lpmtSQLBindCol[2].cDfts = ciCTypes;
	lpmtSQLBindCol[2].lpDfts = lpdftCTypes;
	lpmtSQLBindCol[2].lpData = &fCType;
	lpmtSQLBindCol[4].lpData = &cbValueMax;
	for(dex=0;  dex<NumItems(lpmtSQLBindCol);  dex++)
		lplpSQLBindCol[dex] = (lpPARAMITEM)&lpmtSQLBindCol[dex];


	if (RC_NOTSUCCESSFUL(SQLGetInfo(lpci->hdbc,SQL_DRIVER_VER,szDriverVer,sizeof(szDriverVer),&cbDriverVer)))
		goto exit01;

	//set starting index
	if (lpUsrOptions->fBindColZero && (lpStmt->uBookMarkAttr > SQL_UB_OFF)
		&& !(_tcscmp(szDriverVer,TEXT("03")) < 0))
		dexStart=0;
	else
		dexStart=1;

   // For each column, get a column description and allocate a buffer for it
   for(dex=dexStart, rc=SQL_SUCCESS;dex<=(UWORD)cCols && rc==SQL_SUCCESS;dex++)
	{
		if(fAbortCheck(hwndOut))
			return FALSE;

		// Get the Signed'ness of column
	uDex = GetSqlFuncOffset(SQL_API_SQLCOLATTRIBUTES);
  	rc = SQLColAttributes(lpStmt->hstmt,
   				dex,
   				SQL_COLUMN_UNSIGNED,
   				(PTR ) szDriverVer,
   				0,
   				NULL,
   				(PTR)&fUnsigned);
   	if(RC_NOTSUCCESSFUL(rc))
   		goto exit01;

		// See how big it is.  We truncate anything over users default bind limit
		uDex = GetSqlFuncOffset(SQL_API_SQLDESCRIBECOL);
   	rc = SQLDescribeCol(lpStmt->hstmt,
   				dex,
   				NULL,
   				dex,
   				NULL,
   				&fSqlType,
   				&cbValueMax,
   				NULL,
   				NULL);

		if(RC_NOTSUCCESSFUL(rc))
   		goto exit01;

		//set CType to bookmark if we are binding column 0
		if (!dex)
		{
			if (lpStmt->uBookMarkAttr == SQL_UB_VARIABLE)
				fCType = SQL_C_VARBOOKMARK;
			else
				fCType = SQL_C_BOOKMARK;
		}
		else
		{
			fCType = GetCTypeFromSqlType(fSqlType,(fUnsigned == SQL_FALSE));
		 	cbValueMax = GetSizeOfCType(fCType, cbValueMax);
		}

	   if (IsCharData(fCType))
	   	cbValueMax+=sizeof(TCHAR);

	   cbValueMax = min(lpUsrOptions->uDftBindBuff, cbValueMax);

		// Now call the function handler to do the actual allocate
		uDex = GetSqlFuncOffset(SQL_API_SQLBINDCOL);
		rc = lpSQLBindCol((LPTSTR)szBINDCOLUMN, NumItems(lpmtSQLBindCol),
						lpci, lplpSQLBindCol,
						FH_NO_LOG, NULL);

		if(RC_NOTSUCCESSFUL(rc))
			break;

		// On success, show this row
		ResultsShowBoundCols(lpci, lpci->lpCurStmt->lpVars, rc, dex, fHdr);
		fHdr = FALSE;
	}

	// For row-wise binding, reset the SQL_BIND_TYPE
	if(!lpci->lpCurStmt->fColBinding)
	{
		lpSTATEMENTINFO		lpStmt=lpci->lpCurStmt;
		UDWORD					udRowSize=SumBoundCols(lpStmt);

		uDex = GetSqlFuncOffset(SQL_API_SQLSETSTMTOPTION);
		rc = SQLSetStmtOption(lpStmt->hstmt, SQL_BIND_TYPE, udRowSize);

		if(RC_SUCCESSFUL(rc) && lpStmt->uRowWiseSize != udRowSize)
		{
			lpStmt->uRowWiseSize = udRowSize;
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
						GetGtrString(szOutBuff, MAXOUTBUFF, TST1014),
						udRowSize);
		}

		if(rc != SQL_SUCCESS)
			goto exit01;
	}

	fBusy(FALSE);
	return TRUE;

	// Following is catch all for errors
exit01:
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE, TEXT("\t"));
	GetApiName(uDex-1, szOutBuff);
	LogReturnCode((LPTSTR)szOutBuff, lpci, rc);
	ToolOptionsAllErrors(lpci, lpci->lpActiveEnv->henv, lpci->hdbc, lpStmt->hstmt,FALSE,rc);
	fBusy(FALSE);
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| ResultsFetchAll:
//|	Simply call the lpSQLFetch function handler with logging until there are
//|	no more rows to fetch.  Unlike Get Data All, Fetch All will not call
//|	More Results, since you will most likely want to change your bindings
//|	before going on to a new results set.  This is consistent with v1.0.
//| Parms:
//|	lpci						The connection handle with current statement
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN ResultsFetchAll(lpCONNECTIONINFO lpci)
{
	HWND					hwndOut;
	RETCODE				rc;
	SWORD					cCols;
	HSTMT					hstmt=lpci->lpCurStmt->hstmt;
	UDWORD				udTot=0;
	UINT					uDex;
	UWORD					fMoreResultsSupported=FALSE;


	ASSERTCHECK((lpci == NULL), TEXT("lpci should not be NULL"));

	fBusy(TRUE);

	// Check to see if SQLMoreResults is supported by this driver
	rc = SQLGetFunctions(lpci->hdbc, SQL_API_SQLMORERESULTS, &fMoreResultsSupported);
	if(RC_NOTSUCCESSFUL(rc))
		fMoreResultsSupported = FALSE;

	// Do title
	hwndOut = GETOUTPUTWINDOW(lpci);
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			GetGtrString(szOutBuff, MAXOUTBUFF, idsFetchAllTitle));

	rc = SQL_SUCCESS;
	while(RC_SUCCESSFUL(rc)) {
		// Make sure the user hasn't aborted
		if(fAbortCheck(hwndOut))
			goto doabort;

		// Tell user how many columns there were
		uDex = GetSqlFuncOffset(SQL_API_SQLNUMRESULTCOLS);
		rc = SQLNumResultCols(hstmt, &cCols);
		if(RC_SUCCESSFUL(rc)) {
			// If there are no columns
			if(cCols == 0) {
				SQLLEN		sdRows=0;

				uDex = GetSqlFuncOffset(SQL_API_SQLROWCOUNT);
				rc = SQLRowCount(hstmt, &sdRows);
				if(RC_NOTSUCCESSFUL(rc))
					goto exit01;
				if(rc == SQL_SUCCESS_WITH_INFO) {
					DUMP_INFO;
					}
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
						TRUE, TRUE,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsIUDAffectedRows),
						sdRows);
			   // If we cannot get any more results sets, we must exit
				if(!fMoreResultsSupported)
		   		goto doabort;
				}
			// Process the results set
			else {
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsFetchAllCount),
						cCols);

				// Now simply fetch each row
				udTot = 0;
				uDex = GetSqlFuncOffset(SQL_API_SQLFETCH);
				while(RC_SUCCESSFUL(rc) &&
					rc != SQL_NO_DATA_FOUND) {
					if(fAbortCheck(hwndOut))
						goto doabort;

					while((rc = SQLFetch(hstmt)) == SQL_STILL_EXECUTING){
						if(fAbortCheck(hwndOut))
							goto exit01;
						}

					if(rc != SQL_SUCCESS && rc !=
						SQL_NO_DATA_FOUND) {
						GetApiName(uDex, szOutBuff);
						LogReturnCode((LPTSTR)szOutBuff, lpci, rc);
						ToolOptionsAllErrors(lpci, lpci->lpActiveEnv->henv, lpci->hdbc, hstmt,
									FALSE,rc);
						}

					if(RC_SUCCESSFUL(rc))
						++udTot;
					}

				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsFetchedRowCount),
						udTot);
				}
			}
		else
			goto exit01;


		// Now look for next results set
		if(fMoreResultsSupported) {
			uDex = GetSqlFuncOffset(SQL_API_SQLMORERESULTS);
			while((rc = SQLMoreResults(hstmt)) == SQL_STILL_EXECUTING){
				if(fAbortCheck(hwndOut))
					goto exit01;
				}

			if(RC_NOTSUCCESSFUL(rc) && rc != SQL_NO_DATA_FOUND)
				goto exit01;
			if(rc == SQL_SUCCESS_WITH_INFO) {
				DUMP_INFO;
				}
			}
		}

	// If we are all done, free the statement handle
	if(!fMoreResultsSupported)
		SQLFreeStmt(hstmt, SQL_CLOSE);

doabort:
	fBusy(FALSE);

	return;

	// Following is catch all for errors
exit01:
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE, TEXT("\t"));
	DUMP_INFO;
	fBusy(FALSE);
}


//*---------------------------------------------------------------------------------
//| DoCreateResults:
//|	Create a results set window if possible.
//| Parms:
//|	lpci						Connection info
//|	lpStmt					The statement to do the bind on
//|	hInst						Hinstance info
//|	henv						The environment handle
//|	rc							Function's RETCODE
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoCreateResults(lpCONNECTIONINFO lpci, lpSTATEMENTINFO lpStmt,
					HINSTANCE hInst, HENV henv,RETCODE rc)
{
	lpRESULTWININFO		lprw;
	BOOL						fCreateResultSet=FALSE;

	if(!lpStmt->lpResults)
	{
		fCreateResultSet=TRUE;

		// Allocate memory for a new node, then create the window
		lprw = (lpRESULTWININFO)AllocateMemory(sizeof(RESULTWININFO));
		ASSERTCHECK(lprw,TEXT("AllocateMemory for lprw Failed!!"));
		if(!lprw)
			return;

		lpStmt->lpResults=lprw;

	}
	else
		lprw=lpStmt->lpResults;



	lprw->hInst = hInst;
	lprw->hwndClient = hwndClient;
	lprw->lpci = lpci;
	lprw->lpStmt = lpStmt;
	lprw->fNoDataFound = rc==SQL_NO_DATA_FOUND;
	// Always resets to 1
	lprw->lpStmt->uCurrentRow = 1;

	FREE_PTR(lprw->colx);
	FREE_PTR(lprw->uPcbVals);
	//FREE_PTR(lprw->colpcbx);

	// Allocate memory for row widths if there are bound columns
	if (lprw->lpStmt->uBoundCols)
	{
		lprw->colx = (int *)AllocateMemory(sizeof(int)*lprw->lpStmt->uBoundCols);
		lprw->colpcbx = (int *)AllocateMemory(sizeof(int)*lprw->lpStmt->uBoundCols);
	}

	lprw->uPcbVals = (UINT *)AllocateMemory(MINREQUIREDINTS(TOT_ROWSET_SIZE(lprw->lpStmt)));

	ASSERTCHECK(lprw->uPcbVals,TEXT("AllocateMemory for lprw->uPcbVals Failed!!"));

	if(!lprw->uPcbVals)
	{
		FREE_PTR(lprw->colx);
		FREE_PTR(lprw->uPcbVals);
		FREE_PTR(lprw->colpcbx);
		return;
	}

	// Execute "create" code if:
	//		1) No rowset.
	//		2) Rowset changed in shape
	if(fCreateResultSet)
	{
		//Destroy previous window
		if (lprw && lprw->hwnd)
		{
			// Destroy window
			SendMessage(lprw->hwndClient, WM_MDIDESTROY, (WPARAM)lprw->hwnd, 0L);
			lprw->hwnd=NULL;
		}

		lpStmt->lpResults = CreateResultWindow(lpci->lpActiveEnv->hwndClient, hInst, henv,
						lpci, lpci->lpCurStmt,rc);
	}
	else
		ResetRsltWin(((lpRESULTWININFO)lpStmt->lpResults));
}


//*---------------------------------------------------------------------------------
//| lpUpdateCell:
//|	This function handler is called after the user pressed OK on the
//|	dialog.  Since the dialog wnd proc does the updates to the rowset
//|	"live", this proc need do nothing be but here for compiler purposes.
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpUpdateCell(STD_FH_PARMS)
{
	return SQL_SUCCESS;
}

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHBIND.C
//|
//| Purpose:
//|		This module contains the function handlers to handle SQLBindParameter()
//|		and SQLBindParam()
//*---------------------------------------------------------------------------------
#include "fhheader.h"

#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR				szOutBuff[MAXOUTBUFF];
extern lpUSROPTIONS		lpUsrOptions;						// Global user options


dCSEG(TCHAR) szErrorText[]				= TEXT("Error");
dCSEG(TCHAR) szInfoStruct[]			= TEXT("information structure");


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
UDWORD INTFUN SumBoundCols(lpSTATEMENTINFO lpStmt);					// From FHRSLT.C



//*---------------------------------------------------------------------------------
//| ShouldRealloc:
//|		This function determines if a realloc of the parameter's memory is needed
//|
//| Parms:
//|	lpParameter			Parameter in question
//|	fCType				C Type of parameter
//|	cbValueMax			Length of parameter's rgbValue
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

BOOL INTFUN ShouldRealloc(lpPARAMETERINFO lpParameter,SWORD fCType,SDWORD cbValueMax)
{
	BOOL fReAlloc=FALSE;

	if(lpParameter)
	{
		if(lpParameter->frgbRowset)
			fReAlloc=TRUE;
		else if ((fCType != lpParameter->fCType) || (!(lpParameter->rgbValue)))	// rgbValue's fCType changed
			fReAlloc=TRUE;
		else if ((cbValueMax != lpParameter->cbValueMax)	&&					// cbValueMax has changed and
					(IsVarLenData(fCType)))
			fReAlloc=TRUE;
	}
	else
		fReAlloc=TRUE;

	return (fReAlloc);

} //ShouldReAlloc()



//*---------------------------------------------------------------------------------
//| lpBindParameter:
//|		This function is shared by	SQLBindParameter, SQLBindParam, and SQLSetDescRec
//|
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

RETCODE INTFUN lpBindParameter(STD_FH_PARMS,
	SWORD idxHstmt,
	SWORD idxiPar,
	SWORD idxfParmType,
	SWORD idxfCType,
	SWORD idxfSQLType,
	SWORD idxcbColDef,
	SWORD idxibScale,
	SWORD idxrgbValue,
	SWORD idxcbValueMax,
	SWORD idxpcbValue,
	SWORD idxDAE,
	SWORD idxRsHdbc,
	SWORD idxRsHstmt,
	SWORD idxRsColumn,
	SWORD idxRsRow,
	SWORD	idxcbBufSize,
	SQLLEN *lpIndPtr,
	UWORD	uAPI)
{
	HWND					hwndOut;
	RETCODE				rc;
	PARAMITEM			PIrgbValue;
	PARAMITEM			PIpcbValue;
	lpPARAMITEM			lpPIrgbValue = &PIrgbValue;
	lpPARAMITEM			lpPIpcbValue = &PIpcbValue;
	SDWORD				cbMaxrgbValue = 0,
							cbBufSize = *(SDWORD *)lpParms[idxcbBufSize]->lpData;
	SDWORD				cbMaxpcbValue = 0;
	lpPARAMETERINFO	lpParameter = NULL;		// Storage for pointer to the PARAMETERINFO structure
	BOOL					fExists = FALSE;
	SWORD					fCTypeTmp;
	SWORD					fSqlTypeTmp;
	SDWORD				cbValueMaxTmp = 0;
	BOOL					fpcbNTS = FALSE;
	BOOL					fReAlloc = FALSE;
	UDWORD				cRows = 1;
	// Rowset value vars
	UWORD					ihdbc=0;
	UWORD					ihstmt=0;
	UDWORD				icol=0;
	UDWORD				irow=0;
	lpCONNECTIONINFO	lpCiList = lpConnWin;
	lpSTATEMENTINFO	lpSiList = NULL;
	lpROWWISEINFO		lpRW = NULL;
	lpBOUNDVARS			lpBV = NULL;
	lpROWWISEINFO		lpRWOld = NULL;
	lpBOUNDVARS			lpBVOld = NULL;
	BOOL					frgbTrunc = FALSE;
	BOOL					fpcbTrunc = FALSE;
	BOOL					fNullRgb=FALSE;
	BOOL					fNullPcb=FALSE;
	lpSTATEMENTINFO	lpSI=NULL;
	HSTMT					hstmt=NULL;
	BOOL					fODBC30=FALSE;
	//These are here for ease of reading parameters
	UWORD					iPar=*(UWORD *)lpParms[idxiPar]->lpData;
	SWORD					fParamType=0;
	SWORD					fCType=*(SWORD *)lpParms[idxfCType]->lpData;
	SWORD					fSQLType=0;
	UDWORD				cbColDef=*(UDWORD *)lpParms[idxcbColDef]->lpData;
	SWORD					cbPrecision=*(SWORD *)lpParms[idxcbColDef]->lpData;
	SWORD					ibScale=*(SWORD *)lpParms[idxibScale]->lpData;
	SDWORD				cbValueMax=0;
	PTR					lpPcbValue=NULL;
	PTR					lpRgbValue=NULL;
	SDWORD				idxPcbRowset=-1,
							idxRgbRowset=-1;
	BOOL					fDAEIsNull=FALSE;
	SQLHDESC				hdesc=NULL;

	switch(uAPI)
	{
		case SQL_API_SQLBINDPARAM:
		case SQL_API_SQLSETDESCREC:
			fODBC30=TRUE;
			break;
	}

	//idxcbValueMax == -1 for SQLBindParam
	if (idxcbValueMax > 0) {
        if (ci && (ci->lpCurStmt) && ci->lpCurStmt->cParamRows > 1) {
		    cbValueMax= *(SDWORD *)lpParms[idxcbBufSize]->lpData;
        } else {
		    cbValueMax= *(SDWORD *)lpParms[idxcbValueMax]->lpData;
        }
    }

	//idxfParamType == -1 for SQLBindParam && SQLSetDescRec
	if (idxfParmType > 0)
		fParamType=*(SWORD *)lpParms[idxfParmType]->lpData;

	fSQLType=*(SWORD *)lpParms[idxfSQLType]->lpData;

	//idxDAE == -1 for SQLSetDescRec
	if (idxDAE > 0)
		fDAEIsNull=lpParms[idxDAE]->fNull;

	//idxRsHdbc == -1 for SQLSetDescRec
	if (idxRsHdbc > 0)
		ihdbc = *(UWORD *)lpParms[idxRsHdbc]->lpData;

	//idxRsHstmt == -1 for SQLSetDescRec
	if (idxRsHstmt > 0)
		ihstmt = *(UWORD *)lpParms[idxRsHstmt]->lpData;

	//idxRsColumn== -1 for SQLSetDescRec
	if (idxRsColumn > 0)
		icol = *(UDWORD *)lpParms[idxRsColumn]->lpData;

	//idxRsRow == -1 for SQLSetDescRec
	if (idxRsRow > 0)
		irow = *(UDWORD *)lpParms[idxRsRow]->lpData;

	// Get output window handle for messages
	hwndOut = GETOUTPUTWINDOW(ci);

	if (ci && (ci->lpCurStmt))
		lpSI=ci->lpCurStmt;

	if (lpParms[idxHstmt]->lpData)
	{
		if (uAPI == SQL_API_SQLSETDESCREC)
			hdesc=*(SQLHDESC *)lpParms[idxHstmt]->lpData;
		else
		{
			hstmt=*(HSTMT *)lpParms[idxHstmt]->lpData;
			//get the index for the pcbValue and rgbValue <rowset value> item
			idxPcbRowset=GetPcbIndex(ROWSETVAL);
			idxRgbRowset=GetRgbIndex(ROWSETVAL);
		}

	}

	/*
	//Set the appropriate descriptor fields
	SetDescriptorInfo(ci,
			ci->lpCurDesc->hdesc,
			*(UWORD *)lpParms[idxiPar]->lpData,			// (SQL_DESC_COUNT (APD and IPD)
			fParamType,											// SQL_DESC_PARAMETER_TYPE (APD)
			*(SWORD *)lpParms[idxfCType]->lpData,		// SQL_DESC_TYPE (APD)
			*(SWORD *)lpParms[idxfSQLType]->lpData,	// SQL_DESC_TYPE (IPD)
			*(SWORD *)lpParms[idxibScale]->lpData,		// SQL_DESC_SCALE (IPD)
			lpParms[idxrgbValue]->lpData,					// SQL_DESC_DATA_PTR (APD)
			*(UDWORD *)lpParms[idxcbColDef]->lpData,	// SQL_DESC_LENGTH and/or SQL_DESC_PRECISION
			cbValueMax,											// SQL_DESC_OCTET_LENGTH (APD)
			lpParms[idxpcbValue]->lpData,		// SQL_DESC_OCTET_LENGTH_PTR and
														//		SQL_DESC_INDICATOR_PTR (APD)
			uAPI);									// SQL_API_SQLBINDPARAM or SQL_API_SQLBINDPARAMETER

	*/



	// Initialize the PARAMITEM structures for rgbValue and pcbValue
	memset(&PIrgbValue, 0, sizeof(PARAMITEM));
	memset(&PIpcbValue, 0, sizeof(PARAMITEM));
	lpPIrgbValue->szName = lpParms[idxrgbValue]->szName;
	lpPIpcbValue->szName = lpParms[idxpcbValue]->szName;

	// Check to see if the parameter already exists
	if(lpSI)
	{
		lpParameter=SearchParamList(lpSI, (*(UWORD *)lpParms[idxiPar]->lpData));
		cRows = lpSI->cParamRows;
	}

	// First determine if <rowset value> was selected out of the rgbValue combo box
	if ( (idxRgbRowset != -1) && lpParms[idxrgbValue]->dwDex == idxRgbRowset && (lpCiList || lpSiList))
	{
		// First find the hdbc and hstmt
   	while(lpCiList->uConNum != ihdbc)
   		lpCiList = lpCiList->next;

		lpSiList = lpCiList->lpStmt;
		while(lpSiList->uStmtNum != ihstmt)
			lpSiList = lpSiList->next;

		// Now that we have the correct hstmt, determine whether it uses column or row-wise binding
		if(lpSiList->fColBinding)
		{
			UDWORD	cBytes,
						i;
			BYTE		*lpData=NULL;

			// It is column-wise binding
			// We need to find the correct column
			lpBV = IsBoundColumn((UWORD)icol, lpSiList);
			// Now that we found the correct BoundVar, store the rgbValue pointer in the temporary PI structure
			lpPIrgbValue->lpData = (LPTSTR)lpBV->rgbValue + (lpBV->cbElement * (irow - 1));
			lpPIrgbValue->cBytes = lpBV->cbElement;
			fReAlloc = TRUE;													// This will cause FreeMemory to happen on an old rgbValue buffer correctly
			//need to zero out the remainder of the buffer since it has
			// CA's in it
			cBytes=lpPIrgbValue->cBytes;
			lpData=lpPIrgbValue->lpData;

			for (i=0;i < lpPIrgbValue->cBytes;i++,cBytes--)
			{
				if (lpData && lpData[i] == 0xCA)
					break;

			}
			memset(&lpData[i], 0, cBytes);

		}
		else
		{																		// It is row-wise binding
			// We cannot bind to row-wise bound data if ParamOptions has been called
			if(lpSI->fParamOpt)
			{
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST0020));
				return SQL_ERROR;
			}

			lpRW = lpSiList->lpRowWise;
			lpBV = IsBoundColumn((UWORD)icol, lpSiList);

			// Make sure the sum of the bound cols = rowsize
			if(lpSiList->uRowWiseSize != SumBoundCols(lpSiList))
			{
				wsprintf(szOutBuff, GetGtrString(szOutBuff, MAXOUTBUFF, TST0012),
							lpSiList->uRowWiseSize, SumBoundCols(lpSiList));
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, szOutBuff);
				return SQL_ERROR;
			}

			// Now that we found the correct BoundVar, store the rgbValue pointer in the temporary PI structure
			lpPIrgbValue->lpData = (LPTSTR)lpBV->rgbValue + (lpSiList->uRowWiseSize * (irow - 1));
			lpPIrgbValue->cBytes = lpBV->cbElement;
			fReAlloc = TRUE;
		}
	}
	else
	{

		// Now check to see if rgbValue's fCType changed, or if the type is SQL_C_CHAR or
		// SQL_C_BINARY and the cbValueMax changed -- if any of these are true, we will
		// need to reallocate the memory in rgbValue (also used in our parameter linked list)
		fCTypeTmp = *(SWORD *)lpParms[idxfCType]->lpData;                   // store fCType locally
		fSqlTypeTmp = *(SWORD *)lpParms[idxfSQLType]->lpData;						// store fSqlType locally

		if (!fODBC30)

			cbValueMaxTmp = (lpParms[idxcbValueMax]->lpData? *(SDWORD *)lpParms[idxcbValueMax]->lpData:0);					// store cbValueMax as a local variable
        else
			cbValueMaxTmp = (lpParms[idxpcbValue]->lpData? *(SDWORD *)lpParms[idxpcbValue]->lpData:0);					// store cbValueMax as a local variable

		if(fCTypeTmp == SQL_C_DEFAULT)													// if it's SQL_C_DEFAULT
			fCTypeTmp = GetCTypeFromSqlType(fSqlTypeTmp,TRUE);							// get the default type

		fReAlloc=ShouldRealloc(lpParameter,lpParms[idxrgbValue]->fCType,cbValueMaxTmp);

		// Set up the memory allocation parameters for rgbValue
		if(fReAlloc)
		{

		   if (!(lpParms[idxrgbValue]->lpData))													// rgbValue is a null pointer
				lpPIrgbValue->fNull = TRUE;
			else
			{
				if (IsVarLenData(fCTypeTmp))
				{
					//usr "Buffer Size" field if it larger than "cbValueMax" field
					cbMaxrgbValue = (cbBufSize > 0) ? cbBufSize : lpUsrOptions->uDftBuffLen;

					if (cbValueMaxTmp > cbBufSize)
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
										GetGtrString(szOutBuff, MAXOUTBUFF, TST1021),
										cbBufSize,cbValueMaxTmp);

				}
				else
				{
					// Give default value as the larger of cbValueMax and the size of the data
					// for the case where the C Type is a user defined type
					cbMaxrgbValue = (DWORD)GetSizeOfCType(fCTypeTmp,
					((lpParms[idxrgbValue]->cBytes < (UDWORD)cbBufSize) ?
					(UDWORD)cbBufSize : lpParms[idxrgbValue]->cBytes));	// allocate size of the data type

					if(cbMaxrgbValue != cbBufSize)
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
										GetGtrString(szOutBuff, MAXOUTBUFF, TST1015),
										cbMaxrgbValue, cbBufSize);
				}
			}

			// Now allocate adjustable memory for rgbValue
			if(!lpParms[idxrgbValue]->fNull)
			{
				// Make sure it is no larger than we can handle
				if(cbMaxrgbValue * cRows > MAX_ALLOCATED_BUFFER)
				{
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
								GetGtrString(szOutBuff, MAXOUTBUFF, TST0008),
								(MAX_ALLOCATED_BUFFER-1L));
				   lpParms[idxiPar]->dwDex = DEX_BIND_PARAM_ERROR;
					return SQL_ERROR;
				}

				lpPIrgbValue->lpData = AllocateAdjMem(cbMaxrgbValue * cRows);

				if(!lpPIrgbValue->lpData)
				{
					szMessageBox(hwndOut,MB_ICONINFORMATION | MB_OK, (LPTSTR)szErrorText,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
							cbMaxrgbValue, TEXT("rgbValue"));
					return SQL_ERROR;
				}
			}
		}
		else
		{
			// If nothing changed to cause a realloc, but
			// rgbValue changed to a null pointer, release
			// the old memory, else
			// copy the old rgbValue pointer to the temp rgbValue
		   if(lpParms[idxrgbValue]->fNull)
				FreeAdjMem(lpParameter->rgbValue);
			else
			{
				lpPIrgbValue->lpData = lpParameter->rgbValue;
				cbMaxrgbValue = lpParameter->cBytes;
			}
		}

		// Copy the data into the new rgbValue buffer
		// Types SQL_C_CHAR and SQL_C_BINARY require we copy the correct number (truncate if necessary)
		if (IsVarLenData(fCTypeTmp))
		{
			// First check if pcbValue is SQL_NTS
			if(lpParms[idxpcbValue]->lpData)
			{
				if(*(SDWORD *)lpParms[idxpcbValue]->lpData == SQL_NTS)
					fpcbNTS=TRUE;
			}

			if(lpPIrgbValue->lpData)
			{
				// if data is larger than the buffer and pcbValue = SQL_NTS
				// then truncate the data and add a null terminator
				// Bug #275 - changed ">=" to ">"
				if((lpParms[idxrgbValue]->cBytes > (UDWORD)cbMaxrgbValue) && fpcbNTS)
				{
					_fmemcpy(lpPIrgbValue->lpData, lpParms[idxrgbValue]->lpData, (size_t)(cbMaxrgbValue - 1));
					*((TCHAR *)lpPIrgbValue->lpData + (cbMaxrgbValue -1)) = TEXT('\0');
					frgbTrunc = TRUE;
				}
				// if data is larger than buffer (no SQL_NTS), just truncate the data
				// Bug #275 - changed ">=" to ">"
				else if(lpParms[idxrgbValue]->cBytes > (UDWORD)cbMaxrgbValue)
				{
					_fmemcpy(lpPIrgbValue->lpData, lpParms[idxrgbValue]->lpData, (size_t)cbMaxrgbValue);
					if(!fDAEIsNull)  // If macro is checked, we don't want error msg
						frgbTrunc = TRUE;
				}
				// if data fits in the buffer and pcbValue = SQL_NTS, just copy and add the null terminator
				else if(fpcbNTS)
				{
					_fmemcpy(lpPIrgbValue->lpData, lpParms[idxrgbValue]->lpData, (size_t)lpParms[idxrgbValue]->cBytes);
					*((TCHAR *)lpPIrgbValue->lpData + (lpParms[idxrgbValue]->cBytes)) = TEXT('\0');
				}
				// otherwise, just copy the data
				else
					_fmemcpy(lpPIrgbValue->lpData, lpParms[idxrgbValue]->lpData, (size_t)lpParms[idxrgbValue]->cBytes);
				}
			}
		else
		{
			if((lpPIrgbValue->lpData) && (lpParms[idxrgbValue]->lpData))
				_fmemcpy(lpPIrgbValue->lpData, lpParms[idxrgbValue]->lpData, (size_t)cbMaxrgbValue);

		}

		lpPIrgbValue->cBytes = cbMaxrgbValue;
	}

	// Now determine if <rowset value> was selected out of the pcbValue combo box
	if(lpParms[idxpcbValue]->dwDex == idxPcbRowset)
	{
		// First find the hdbc and hstmt
		if(!lpSiList)
		{
	   	while(lpCiList->uConNum != ihdbc)
	   		lpCiList = lpCiList->next;

			lpSiList = lpCiList->lpStmt;
			while(lpSiList->uStmtNum != ihstmt)
				lpSiList = lpSiList->next;
		}

		// Now that we have the correct hstmt, determine whether it uses column or row-wise binding
		if(lpSiList->fColBinding)
		{
			// It is column-wise binding
			// We need to find the correct column
			lpBV = IsBoundColumn((UWORD)icol, lpSiList);
			// Now that we found the correct BoundVar, store the rgbValue pointer in the temporary PI structure
			lpPIpcbValue->lpData = (SDWORD *)lpBV->pcbValue + (irow - 1);
		}
		else
		{
			// It is row-wise binding
			// We cannot bind to row-wise bound data if ParamOptions has been called
			if(lpSI->fParamOpt)
			{
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST0020));
				return SQL_ERROR;
			}

			lpRW = lpSiList->lpRowWise;
			// Make sure the sum of the bound cols = rowsize
			if(lpSiList->uRowWiseSize != SumBoundCols(lpSiList))
			{
				wsprintf(szOutBuff, GetGtrString(szOutBuff, MAXOUTBUFF, TST0012),
							lpSiList->uRowWiseSize, SumBoundCols(lpSiList));
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, szOutBuff);
				return SQL_ERROR;
			}

			// We need to find the correct column
			lpBV = IsBoundColumn((UWORD)icol, lpSiList);
			// Now that we found the correct BoundVar, store the rgbValue pointer in the temporary PI structure
			lpPIpcbValue->lpData = (TCHAR *)lpBV->pcbValue + (lpSiList->uRowWiseSize * (irow - 1));
		}
	}
	else
	{
		// Set up the memory allocation parameters for pcbValue
		if(!(lpParms[idxpcbValue]->lpData))			// pcbValue is a null pointer
			lpPIpcbValue->fNull = TRUE;
		else										// pcbValue will be an SDWORD
			cbMaxpcbValue = sizeof(SDWORD);

		// Allocate memory for pcbValue and copy data into it
		if(!lpParms[idxpcbValue]->fNull)
		{
			if(cbMaxpcbValue * cRows > MAX_ALLOCATED_BUFFER)
			{
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, TST0008),
							(MAX_ALLOCATED_BUFFER-1L));

			   lpParms[idxiPar]->dwDex = DEX_BIND_PARAM_ERROR;
				return SQL_ERROR;
			}

			lpPIpcbValue->lpData = AllocateAdjMem(cbMaxpcbValue * cRows);

			if(!lpPIpcbValue->lpData)
			{
				szMessageBox(hwndOut,MB_ICONINFORMATION | MB_OK, (LPTSTR)szErrorText,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
						cbMaxpcbValue, TEXT("pcbValue"));

				if(lpPIrgbValue->lpData)
					FreeAdjMem(lpPIrgbValue->lpData);
				return SQL_ERROR;
			}
		}

		if(lpPIpcbValue->lpData && lpParms[idxpcbValue]->lpData)
			_fmemcpy(lpPIpcbValue->lpData, lpParms[idxpcbValue]->lpData, (size_t)cbMaxpcbValue);

		// Check if pcbValue > cbValueMax.  If it is, and the macro is not checked
		// truncate it to cbValueMax
		if(lpPIpcbValue->lpData)

		{
			if(((*(SDWORD *)lpPIpcbValue->lpData) > cbBufSize) && (!fDAEIsNull))
			{
				*(SDWORD *)lpPIpcbValue->lpData = cbBufSize;
				fpcbTrunc = TRUE;
			}
		}
	}

	// Check to see if the Macro needs to be executed
	if(fDAEIsNull && lpPIpcbValue->lpData)
	{
		// SQL_LEN_DATA_AT_EXEC macro is checked, so use it on pcbValue
		*(SDWORD *)lpPIpcbValue->lpData = SQL_LEN_DATA_AT_EXEC(*(SDWORD *)lpPIpcbValue->lpData);
	}

	lpPcbValue=lpPIpcbValue ? lpPIpcbValue->lpData : NULL;
	lpRgbValue=lpPIrgbValue ? lpPIrgbValue->lpData : NULL;

	if (!lpParms[7]->lpData && lpPIrgbValue)
	{
		fNullRgb=TRUE;
		lpParms[7]->lpData=lpPIrgbValue->lpData;
	}

	//
	if( !lpParms[9]->lpData && lpPcbValue)
	{
		fNullPcb=TRUE;
		lpParms[9]->lpData=lpPIpcbValue->lpData;
	}
	//

	// Log input parameters
	// If neither rgbValue or pcbValue are <rowset value> then don't log the rowset parms
	if((lpParms[idxrgbValue]->dwDex != idxRgbRowset) && (lpParms[idxpcbValue]->dwDex != idxPcbRowset))
	{
		// Turn off the input logging flags for the rowset controls
		if (idxRsHdbc > 0)
			lpParms[idxRsHdbc]->uParmType |= PT_NONE;
		if (idxRsHstmt > 0)
			lpParms[idxRsHstmt]->uParmType |= PT_NONE;
		if (idxRsColumn > 0)
			lpParms[idxRsColumn]->uParmType |= PT_NONE;
		if (idxRsRow > 0)
			lpParms[idxRsRow]->uParmType |= PT_NONE;

		// Log input parameters
		LOGPARAMETERSSPEC(szFuncName, lpParms,cParms, ci, TRUE);

		// Turn the flags back on for next time
		if (idxRsHdbc > 0)
			lpParms[idxRsHdbc]->uParmType &= ~PT_NONE;
		if (idxRsHstmt > 0)
			lpParms[idxRsHstmt]->uParmType &= ~PT_NONE;
		if (idxRsColumn > 0)
			lpParms[idxRsColumn]->uParmType &= ~PT_NONE;
		if (idxRsRow > 0)
			lpParms[idxRsRow]->uParmType &= ~PT_NONE;

	}
	else
		LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	if (fNullRgb)
		lpParms[7]->lpData=NULL;
	if (fNullPcb)
		lpParms[9]->lpData=NULL;


	// Invoke correct ODBC API
	switch(uAPI)
	{
		case SQL_API_SQLBINDPARAMETER:
				rc = SQLBindParameter(hstmt,
											 iPar,
											 fParamType,
											 fCType,
											 fSQLType,
											 cbColDef,
											 ibScale,
											 lpRgbValue,
											 cbValueMax,
											 lpPcbValue);
		break;

		case SQL_API_SQLBINDPARAM:
				rc = SQLBindParam(hstmt,
										iPar,
										fCType,
										fSQLType,
										cbColDef,
										ibScale,
										lpRgbValue,
										lpPcbValue);
		break;

		//When there is enough time to rewrite code
		//the common functionality in lpBindParameter
		//needs to be modularized and calls for this
		//common code need to be placed in lpSQLSetDescRec
		//Rather than call lpBindParameter from lpSQLSetDescRec
		//This was done to save some time.
		case SQL_API_SQLSETDESCREC:
				//If this arg is null it means
				//the user selected <same as StrinLengthPtr>
				//for IndicatorPtr
				if (!lpIndPtr)
					lpIndPtr=lpPcbValue;


				rc = SQLSetDescRec(hdesc,
										 iPar,
										 fCType,
										 fSQLType,		//fSubType
										 cbValueMax,	//Length
										 cbPrecision,	//Precision
										 ibScale,		//Scale
										 lpRgbValue,	//DataPtr
										 lpPcbValue,	//StringLengthPtr
										 lpIndPtr);		//IndicatorPtr

				if(RC_SUCCESSFUL(rc))
				{
					SetDescriptorInfo(ci,
											hdesc,
											iPar,			// (SQL_DESC_COUNT (APD and IPD)
											fParamType,	// SQL_DESC_PARAMETER_TYPE (APD)
											fCType,		// SQL_DESC_TYPE (APD)
											fSQLType,	// SQL_DESC_TYPE (IPD)
											ibScale,		// SQL_DESC_SCALE (IPD)
											lpRgbValue,// SQL_DESC_DATA_PTR (APD)
											cbPrecision,	// SQL_DESC_LENGTH and/or SQL_DESC_PRECISION
											cbValueMax,	// SQL_DESC_OCTET_LENGTH (APD)
											lpIndPtr,		// SQL_DESC_OCTET_LENGTH_PTR and
											SQL_API_SQLSETDESCREC);	// SQL_DESC_OCTET_LENGTH (APD)

				}

		break;

	} //switch(uAPI)

	if (RC_SUCCESSFUL(rc) && lpPcbValue && (*(SDWORD *)lpPcbValue > 0))
		lpParms[idxrgbValue]->cBytes=*(UDWORD *)lpPcbValue;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Print out TST warning messages if rgbValue or pcbValue were truncated
	if(frgbTrunc)
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
				GetGtrString(szOutBuff, MAXOUTBUFF, TST1017), cbValueMaxTmp);

	if(fpcbTrunc)
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
				GetGtrString(szOutBuff, MAXOUTBUFF, TST1018), cbValueMaxTmp);

   //  Check for errors
	//
	if (uAPI == SQL_API_SQLSETDESCREC)
		AUTOLOGERRORDESC(ci,rc,hdesc);
	else
		AUTOLOGERRORSI(ci,rc,hstmt);

	if (RC_SUCCESSFUL(rc))
	{
		if(!lpParameter)
		{
			// Create PARAMETERINFO node in linked list for this hstmt
			lpParameter = (lpPARAMETERINFO)AllocateMemory(sizeof(PARAMETERINFO));

			if(!lpParameter)
			{
				szMessageBox(GetActiveWindow(),
						MB_ICONINFORMATION | MB_OK,
						TEXT("Error"),
						GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
						sizeof(PARAMETERINFO), (LPTSTR)szInfoStruct);

				if(lpRgbValue)
					FreeAdjMem(lpRgbValue);

				if(lpPcbValue)
					FreeAdjMem(lpPcbValue);

				return SQL_ERROR;
			}

			// Allocate memory for lpfMacro array
			lpParameter->lpfMacro = AllocateAdjMem(sizeof(BOOL) * cRows);

			if(!lpParameter->lpfMacro)
			{
				szMessageBox(GetActiveWindow(),
						MB_ICONINFORMATION | MB_OK,
						TEXT("Error"),
						GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
						sizeof(BOOL) * cRows, (LPTSTR)szInfoStruct);

				FREE_PTR(lpParameter);

				if(lpPIrgbValue->lpData)
					FreeAdjMem(lpPIrgbValue->lpData);

				if(lpPIpcbValue->lpData)
					FreeAdjMem(lpPIpcbValue->lpData);

				return SQL_ERROR;
			}

			// Add it to the linked list
			lpSI->cParams += 1;

			if(lpSI->lpParams)
			{			// Insert at end of list
				lpSI->lpParams->prev->next = lpParameter;
				lpParameter->prev = lpSI->lpParams->prev;
				lpParameter->next = lpSI->lpParams;
				lpSI->lpParams->prev = lpParameter;
			}
			else
			{
				// Only one in the list
				lpSI->lpParams = lpParameter;
				lpParameter->next = lpParameter;
				lpParameter->prev = lpParameter;
				lpSI->lpCurParam = lpSI->lpParams;
			}

		}

		// Now fill in the information
		lpParameter->ipar = *(UWORD *)lpParms[idxiPar]->lpData;

		if (!fODBC30)
		{
			lpParameter->fParamType = *(SWORD *)lpParms[idxfParmType]->lpData;
			lpParameter->dwParamDex = lpParms[idxfParmType]->dwDex;
		}

		lpParameter->fCType = lpParms[idxrgbValue]->fCType;								// Take the C type from the rgbValue parm
		lpParameter->dwCDex = lpParms[idxfCType]->dwDex;
		lpParameter->fSqlType = *(SWORD *)lpParms[idxfSQLType]->lpData;
		lpParameter->dwSqlDex = lpParms[idxfSQLType]->dwDex;
		lpParameter->cbColDef = *(UDWORD *)lpParms[idxcbColDef]->lpData;
		lpParameter->ibScale = *(SWORD *)lpParms[idxibScale]->lpData;
		if(lpParameter->rgbValue)
		{
			// If a previous rgbValue pointer exists
			if(lpParameter->frgbRowset)
			{
				// The value is from a rowset
				if(lpParameter->fColBinding)
				{
					// It is column-wise bound memory
					lpBVOld = lpParameter->lpRowset;

					if(--(lpBVOld->uRgbLockCnt) == 0)
					{
						// If the lock count goes to zero
						FreeAdjMem(lpBVOld->rgbValue);

						if((lpBVOld->uRgbLockCnt == 0) && (lpBVOld->uPcbLockCnt == 0))
							FREE_PTR(lpParameter->lpRowset);
					}
				}
				else
				{
					// It is row-wise bound memory
					lpRWOld = lpParameter->lpRowset;

					if(--(lpRWOld->uRgbLockCnt) == 0)
					{
						FreeAdjMem(lpRWOld->rgbValue);
						FREE_PTR(lpParameter->lpRowset);
					}
				}
			}
			else
			{
				if(fReAlloc)
					FreeAdjMem(lpParameter->rgbValue);
			}
		}

		lpParameter->rgbValue = lpPIrgbValue->lpData;

		if (lpPcbValue && (*(SDWORD *)lpPcbValue > 0))
			lpParameter->cBytes = *(SDWORD *)lpPcbValue;
		else
			lpParameter->cBytes = lpPIrgbValue->cBytes;

		if (!fODBC30)
			lpParameter->cbValueMax = *(SDWORD *)lpParms[idxcbValueMax]->lpData;
		else
			lpParameter->cbValueMax = 0;

		if(lpParameter->pcbValue)
		{
			// If a previous pcbValue pointer exists
			if(lpParameter->fpcbRowset)
			{
				// The value is from a rowset
				if(lpParameter->fColBinding)
				{
					// It is column-wise bound memory
					lpBVOld = lpParameter->lpRowset;

					if(--(lpBVOld->uPcbLockCnt) == 0)
					{
						// If the lock count goes to zero
						FreeAdjMem(lpBVOld->pcbValue);

						if((lpBVOld->uRgbLockCnt == 0) && (lpBVOld->uPcbLockCnt == 0))
							FREE_PTR(lpParameter->lpRowset);
					}
				}
				else
				{
					// It is row-wise bound memory
					lpRWOld = lpParameter->lpRowset;

					if(--(lpRWOld->uRgbLockCnt) == 0)
					{
						FreeAdjMem(lpRWOld->rgbValue);
						FREE_PTR(lpParameter->lpRowset);
					}
				}
			}
			else
				FreeAdjMem(lpParameter->pcbValue);
		}

		lpParameter->pcbValue = lpPIpcbValue->lpData;

		*(lpParameter->lpfMacro)=fDAEIsNull;

		if((idxRgbRowset != -1)&& lpParms[idxrgbValue]->dwDex == idxRgbRowset)
		{
			lpParameter->frgbRowset = TRUE;

			// Increment the lock count for rgbValue
			if(lpSiList->fColBinding)				// column-wise bound
				(lpBV->uRgbLockCnt)++;
         else											// row-wise bound
				(lpRW->uRgbLockCnt)++;
		}
		else
			lpParameter->frgbRowset = FALSE;

		if(lpParms[idxpcbValue]->dwDex == idxPcbRowset)
		{
			lpParameter->fpcbRowset = TRUE;

			// Increment the lock count for rgbValue
			if(lpSiList->fColBinding)				// column-wise bound
				(lpBV->uPcbLockCnt)++;
         else											// row-wise bound
				(lpRW->uRgbLockCnt)++;
		}
		else
			lpParameter->fpcbRowset = FALSE;

		lpParameter->iHdbc = ihdbc;
		lpParameter->iHstmt = ihstmt;
		lpParameter->iCol = icol;
		lpParameter->iRow = irow;

		if(lpSiList)
		{
			if(lpSiList->fColBinding)
			{
				lpParameter->lpRowset = lpBV;
				lpParameter->fColBinding = TRUE;
			}
			else
			{
				lpParameter->lpRowset = lpSiList->lpRowWise;
				lpParameter->fColBinding = FALSE;
			}
		}
	}
	else
	{
	   // Since BindParam did not return SUCCESS or SUCCESS_WITH_INFO, set ipar's dwDex to DEX_BIND_PARAM_ERROR
	   // This will tell the BindParameterWndProc to use the previously set values as the default instead of the
	   // values out of the linked list of valid parameters
	   lpParms[idxiPar]->dwDex = DEX_BIND_PARAM_ERROR;

	   // Free the memory we had allocated for rgbValue and pcbValue
	   if(fReAlloc)
	   {
	   	// If the memory for rgbValue was re-alloced, free it
		   if((lpPIrgbValue->lpData) &&
		   	(lpParms[idxrgbValue]->dwDex != idxRgbRowset))
		   	FreeAdjMem(lpPIrgbValue->lpData);
		 }

	   if(lpPIpcbValue->lpData)
	   {
	   	if(lpParms[idxpcbValue]->dwDex != idxPcbRowset)
	  			FreeAdjMem(lpPIpcbValue->lpData);
		}
	}

	return rc;

} //lpBindParameter()



//*---------------------------------------------------------------------------------
//| lpSQLBindParameter:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLBindParam(STD_FH_PARMS)
{

	return(lpBindParameter(szFuncName, cParms, ci,lpParms,uAttrib,lpPtr,
					IPAR_BPARAM_HSTMT,
					IPAR_BPARAM_IPAR,
					IPAR_BPARAM_FPARAMTYPE,
					IPAR_BPARAM_FCTYPE,
					IPAR_BPARAM_FSQLTYPE,
					IPAR_BPARAM_CBCOLDEF,
					IPAR_BPARAM_IBSCALE,
					IPAR_BPARAM_RGBVALUE,
					IPAR_BPARAM_CBVALUEMAX,
					IPAR_BPARAM_PCBVALUE,
					IPAR_BPARAM_DAE,
					IPAR_BPARAM_RS_HDBC,
					IPAR_BPARAM_RS_HSTMT,
					IPAR_BPARAM_COLUMN,
					IPAR_BPARAM_ROW,
					IPAR_BPARAM_BUFFERSIZE,
					NULL,
					SQL_API_SQLBINDPARAM));

} //lpSQLBindParam()


//*---------------------------------------------------------------------------------
//| lpSQLBindParameter:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLBindParameter(STD_FH_PARMS)
{

	return(lpBindParameter(szFuncName, cParms, ci,lpParms,uAttrib,lpPtr,
				IPAR_BP_HSTMT,
				IPAR_BP_IPAR,
				IPAR_BP_FPARAMTYPE,
				IPAR_BP_FCTYPE,
				IPAR_BP_FSQLTYPE,
				IPAR_BP_CBCOLDEF,
				IPAR_BP_IBSCALE,
				IPAR_BP_RGBVALUE,
				IPAR_BP_CBVALUEMAX,
				IPAR_BP_PCBVALUE,
				IPAR_BP_DAE,
				IPAR_BP_RS_HDBC,
				IPAR_BP_RS_HSTMT,
				IPAR_BP_COLUMN,
				IPAR_BP_ROW,
				IPAR_BP_BUFFERSIZE,
				NULL,
				SQL_API_SQLBINDPARAMETER));

} //lpSQLBindParameer()

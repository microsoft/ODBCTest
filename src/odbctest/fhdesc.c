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
#include "resource.h"
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
extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options

extern lpDFTARRAY					lpdftParamType;
extern lpDFTARRAY					lpdftCTypes;
extern lpDFTARRAY					lpdftSqlTypes;
extern lpDFTARRAY					lpdftNullable;
extern lpDFTARRAY					lpdftSetDescRecFields;
extern lpDFTARRAY					lpdftSetDescHdrFields;
extern UINT							cDescRecFields;
extern UINT							cDescHdrFields;
extern UINT							ciCTypes;
extern UINT 						ciSqlTypes;
extern UINT							ciNullable;

dCSEG(TCHAR) szAssertMsg[]		= TEXT("Assertion failure: %s, %s, %d");

#define DOASSERT(cond,msg) AssertCheck(cond, msg, TEXT(__FILE__), TEXT(__LINE__))


//from fhbind.c
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
	SDWORD *lpIndPtr,
	UWORD	uAPI);

BOOL INTFUN AssertCheck(BOOL fCond, LPTSTR szMsg, LPTSTR szFile, int iLine)
{
	TCHAR tmp[200];

	if(!fCond)
		return(FALSE);

	wsprintf((LPTSTR)tmp, szAssertMsg,
					(LPTSTR)szMsg, (LPTSTR)szFile, iLine);
	MessageBox(GetActiveWindow(), (LPTSTR)tmp,
					TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);

	return(TRUE);

}


//*---------------------------------------------------------------------------------
//| IsDescPointerField:
//|		This function returns TRUE if the Descriptor field ID (fFieldId) is
//|	a PTR field otherwise FALSE is returned
//| Parms:
//|	fFieldId		Descriptor Field ID
//| Returns:
//|	TRUE or FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN IsDescPointerField(SDWORD fFieldId)
{
	switch(fFieldId)
	{
		case SQL_DESC_ARRAY_STATUS_PTR:
		case SQL_DESC_BIND_OFFSET_PTR:
	#ifdef	SQL_DESC_FETCH_BOOKMARK_PTR
		case SQL_DESC_FETCH_BOOKMARK_PTR:
		case SQL_DESC_PREDICATE_OCTET_LENGTH_PTR:
		case SQL_DESC_PREDICATE_PTR:
	#endif
		case SQL_DESC_ROWS_PROCESSED_PTR:
		case SQL_DESC_DATA_PTR:
		case SQL_DESC_INDICATOR_PTR:
		case SQL_DESC_OCTET_LENGTH_PTR:
			return TRUE;
		default:
			return FALSE;
	}

} //IsDescPointerField()


//*---------------------------------------------------------------------------------
//| CopyDescInfo:
//|	This function copies Descriptor information from one DESCRIPTORHEADER structure
//|	to another.
//| Parms:
//|	lpSrcDesc	Source descriptor
//|	lpDestDesc	Destination descriptor
//| Returns:
//|	NONE
//*---------------------------------------------------------------------------------
VOID INTFUN CopyDescInfo(lpDESCRIPTORHEADER lpSrcDesc,lpDESCRIPTORHEADER lpDestDesc)
{
	UINT					iRec=0;
	lpDESCRIPTORRECORD	lpDescRec=NULL,
							lpSrcRec=NULL,
							*lplpDestRec=NULL;

//Copy DESCRIPTORHEADER


	memcpy(lpDestDesc,lpSrcDesc,sizeof(DESCRIPTORHEADER));

	lpSrcRec=lpSrcDesc ? lpSrcDesc->lpRec : NULL;

	if (lpSrcRec)
		lplpDestRec=&lpDestDesc->lpRec;

/*
	//Copy all of the Descriptor records associated with this Descriptor
	for (iRec=1;iRec < lpDestDesc->uwCount;iRec++)
		{
		*lplpDestRec = (lpDESCRIPTORRECORD)AllocateMemory(sizeof(DESCRIPTORINFO));

		memcpy(*lplpDestRec, lpSrcRec, sizeof(DESCRIPTORINFO));

		lplpDestRec=&((*lplpDestRec)->next);
		lpSrcRec=lpSrcRec->next;
		}
*/


} //CopyDescInfo()



//*---------------------------------------------------------------------------------
//| lpSQLCopyDesc:
//|	This function copies Descriptor information from one handle to another
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
SQLRETURN INTFUN lpSQLCopyDesc(STD_FH_PARMS)
{
	SQLRETURN			rc = SQL_SUCCESS;
	SQLHDESC				hSrc = NULL;
	SQLHDESC				hDest = NULL;
	HANDLE				hInvalid1=NULL,
							hInvalid2=NULL;
	lpDESCRIPTORHEADER	lpSrcDesc=NULL,
							lpDestDesc=NULL,
							lpDesc=NULL;
	SWORD					iDesc=0;
	lpENVIRONMENTINFO	lpEI=GetActiveEnv();

	//clear the error flag
	if (ci)
	{
		lpSrcDesc=lpEI->lpSrcDesc;
		lpDestDesc=lpEI->lpDestDesc;
	}

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[0]->lpData)
		hSrc=*(SQLHANDLE *)lpParms[0]->lpData;

	if (lpParms[1]->lpData)
		hDest=*(SQLHANDLE *)lpParms[1]->lpData;

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLCopyDesc(hSrc,hDest);

/*
	if (RC_SUCCESSFUL(rc))
		{

		//Find the DESCRIPTORHEADER nodes associated
		//with both Src and Dest hdesc's
		lpDesc=ci ? ci->lpDesc : NULL;

		for (iDesc=0;lpDesc && (iDesc < ci->cDescs);iDesc++)
			{
			if (lpDesc->hdesc == hSrc)
				lpSrcDesc=lpDesc;

			if (lpDesc->hdesc == hDest)
				lpDestDesc=lpDesc;

			lpDesc=lpDesc->next;

			}

		if (lpDestDesc && lpSrcDesc)
			CopyDescInfo(lpSrcDesc,lpDestDesc);

		}
  */

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	//  Check for errors
	AUTOLOGERRORDESC(ci,rc,hSrc);

	return rc;

} //lpSQLCopyDesc()



//*---------------------------------------------------------------------------------
//| lpSQLGetDescField:
//|	This function gets an individual descriptor field
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
SQLRETURN INTFUN lpSQLGetDescField(STD_FH_PARMS)
{

	SQLRETURN				rc=SQL_SUCCESS;
	SQLHDESC					hdesc=NULL;
	SWORD						iRecNum =	*(SQLSMALLINT *)lpParms[IPAR_GDF_IRECNUM]->lpData;
	SWORD						fFieldID =*(SQLSMALLINT *)lpParms[IPAR_GDF_IFIELDIDENTIFIER]->lpData;
	SDWORD					cbValueMax=	*(SQLINTEGER *)lpParms[IPAR_GDF_CBVALUEMAX]->lpData;
	SDWORD					*pcbValue=	(SQLINTEGER *)lpParms[IPAR_GDF_PCBVALUE]->lpData;
	PTR						rgbValue=lpParms[IPAR_GDF_RGBVALUE]->lpData;
	SWORD						fCType=0;
	lpSTATEMENTINFO		lpStmt=NULL;
	lpDESCRIPTORHEADER	lpDesc=NULL;

	if (ci)
	{
		if (ci->lpCurStmt)
			lpStmt=ci->lpCurStmt;
	}

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[IPAR_GDF_HDESC]->lpData)
		hdesc=*(SQLHDESC *)lpParms[IPAR_GDF_HDESC]->lpData;

	BUILDOUTPUTPARM(lpParms[IPAR_GDF_RGBVALUE], 		// Allocate rgbValue
						cbValueMax,								// based on cbValueMax
						lpUsrOptions->fBufferChecking);

	//Locate descriptor header record
	lpDesc=FindDescriptor(hdesc,ci);

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms, cParms, ci, TRUE);

	rgbValue=lpParms[IPAR_GDF_RGBVALUE]->lpData;

	rc=SQLGetDescField(hdesc,
							iRecNum,
							fFieldID,
							lpParms[IPAR_GDF_RGBVALUE]->fNull ? NULL : rgbValue,
    						cbValueMax,
    						lpParms[IPAR_GDF_PCBVALUE]->fNull ? NULL : pcbValue);

	//Set gator's Descriptor records
	if (RC_SUCCESSFUL(rc) && rgbValue)
	{
		SetDescriptorField(ci,lpStmt,lpDesc,fFieldID,
			rgbValue,pcbValue ? *pcbValue: 0,iRecNum);
	}

	switch(fFieldID)
	{
		case SQL_DESC_NAME:
		case SQL_DESC_LABEL:
		case SQL_DESC_CATALOG_NAME:
		case SQL_DESC_SCHEMA_NAME:
		case SQL_DESC_TABLE_NAME:
		case SQL_DESC_TYPE_NAME:
		case SQL_DESC_LITERAL_PREFIX:
		case SQL_DESC_LITERAL_SUFFIX:
		case SQL_DESC_LOCAL_TYPE_NAME:
		case SQL_DESC_BASE_COLUMN_NAME:
			// Check szDSN for null termination
			CHECKNULLTERM(ci, lpParms[IPAR_GDF_RGBVALUE], lpParms[IPAR_GDF_PCBVALUE], TRUE);
			break;
		case SQL_DESC_BIND_OFFSET_PTR:
		case SQL_DESC_ARRAY_STATUS_PTR:
		case SQL_DESC_ROWS_PROCESSED_PTR:
		case SQL_DESC_DATA_PTR:
		case SQL_DESC_INDICATOR_PTR:
		case SQL_DESC_OCTET_LENGTH_PTR:
			if (lpParms[IPAR_GDF_RGBVALUE])
				lpParms[IPAR_GDF_RGBVALUE]->fHandle=PT_PTR_PTR;
		default:
			;
	}


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	if (IsCharData(fCType))
	{
		TRUNCATIONCHECK(lpParms[IPAR_GDF_RGBVALUE]->fNull, lpParms[IPAR_GDF_PCBVALUE]->fNull,
					rc, GETOUTPUTWINDOW(ci),
					(LPTSTR)lpParms[IPAR_GDF_RGBVALUE]->lpData, 			// rgbValue is buffer to check
					*(SDWORD *)lpParms[IPAR_GDF_CBVALUEMAX]->lpData,	// cbValueMax
					*(SDWORD *)lpParms[IPAR_GDF_PCBVALUE]->lpData,		// pcbValueMax
					(LPTSTR)lpParms[IPAR_GDF_PCBVALUE]->lpData);
	}

	//  Check for errors
	AUTOLOGERRORDESC(ci,rc,hdesc);

	return rc;

} //lpSQLGetDescField()

//*---------------------------------------------------------------------------------
//| AddDescriptorRecord:
//|	This function allocates a new descriptor row record and adds
//|	it to the linked list of current row records
//| Parms:
//|	lpDesc		Descriptor record to process
//|	iRec			record number (i.e. descriptor row number)
//| Returns:
//|	lpRec			row descriptor record
//*---------------------------------------------------------------------------------

lpDESCRIPTORRECORD INTFUN AddDescriptorRecord(lpDESCRIPTORHEADER lpDesc, SWORD iRec)
{
	lpDESCRIPTORRECORD lpRec=NULL;

	if (!lpDesc)
		return NULL;

	//Allocate another row record
	lpRec = AllocateMemory(sizeof(DESCRIPTORRECORD));

	ASSERTCHECK(!lpRec, TEXT("Couldn't Allocate Descriptor Record!!"));

	memset(lpRec,0,sizeof(DESCRIPTORRECORD));

	//Descriptor field rec's number
	if (lpRec)
		lpRec->swRecNum=iRec;

	//Increment the number of Row records
	if (lpDesc)
	{
		lpDesc->cRecs++;
		lpDesc->swCount=iRec;
	}

	if(lpDesc->lpRec)
	{
		// Insert at end of list
		lpDesc->lpRec->prev->next = lpRec;
		lpRec->prev = lpDesc->lpRec->prev;
		lpRec->next = lpRec;
		lpDesc->lpRec->prev = lpRec;
	}
	else
	{
		// Only one in the list
		lpDesc->lpRec =
		lpRec->next =
		lpRec->prev = lpRec;
	}

	return(lpRec);

} //AddDescriptorRecord()

//*------------------------------------------------------------------------
//|  GetArrayStatusSize:
//|		This function returns the number of bytes required by the attr.
//|	for memory allocation
//|	Returns:
//|		iBytes	number of bytes to allocate
//*------------------------------------------------------------------------

UDWORD INTFUN GetArrayStatusSize(lpCONNECTIONINFO lpci)
{
	UDWORD	iBytes=0;
	UDWORD	uRowSet=1;		// Default of one row

	//Get rowset size
	if(lpci && lpci->lpCurStmt)
		uRowSet = lpci->lpCurStmt->uFSFRowSetSize + lpci->lpCurStmt->uReservedRows;

	iBytes = (sizeof(UWORD) * uRowSet);

	return(iBytes);

} //GetArrayStatusSize()

//*------------------------------------------------------------------------
//|  GetDescMemSize:
//|		This function returns the number of bytes required by the attr.
//|	for memory allocation
//|	Returns:
//|		iBytes	number of bytes to allocate
//*------------------------------------------------------------------------

UDWORD INTFUN GetDescMemSize(lpCONNECTIONINFO lpci,SDWORD sdFieldID)
{
	UDWORD	iBytes=0;

	switch (sdFieldID)
	{
		case SQL_DESC_ARRAY_STATUS_PTR:
			iBytes=GetArrayStatusSize(lpci);
			break;
		case SQL_DESC_DATA_PTR:
#ifdef SQL_DESC_OUT_OF_LINE_HEAP_PTR
		case SQL_DESC_FETCH_BOOKMARK_PTR:	//??? RGR XXX
		case SQL_DESC_OUT_OF_LINE_HEAP_PTR:
			iBytes=sizeof(PTR);
			break;
#endif
#ifdef	SQL_DESC_PREDICATE_PTR
		case SQL_DESC_PREDICATE_OCTET_LENGTH_PTR:
		case SQL_DESC_PREDICATE_PTR:
#endif
		case SQL_DESC_BIND_OFFSET_PTR:
		case SQL_DESC_OCTET_LENGTH_PTR:
		case SQL_DESC_ROWS_PROCESSED_PTR:
			iBytes=sizeof(UDWORD);
			break;
		case SQL_DESC_INDICATOR_PTR:
			iBytes=sizeof(SWORD);
			break;
	}

	return(iBytes);

} //GetDescMemSize()



//*---------------------------------------------------------------------------------
//| SetDescFieldValuePtr:
//|	This function sets the ValuePtr correctly for SQLSetDescField.
//| Parms:
//|	lpCI						pointer to connection info record
//|	fFieldID		Desc. field ID
//|	lpData					ValuePtr data
//|	fCType					C Type of Desc. Field.
//| Returns:
//|	ValuePtr					data to be passed to SQLSetDescField
//*---------------------------------------------------------------------------------

SQLPOINTER INTFUN SetDescFieldValuePtr(lpCONNECTIONINFO lpci,
													SQLSMALLINT fFieldID,
													PTR lpData,SWORD fCType)
{
	SQLPOINTER	ValuePtr=NULL;
	UDWORD		iBytes=0;

	if (!lpData)
		return(ValuePtr);

	//Check if this is a POINTER descriptor field
	if (IsDescPointerField(fFieldID))
	{
		//Get number of bytes to allocate
		iBytes=GetDescMemSize(lpci,fFieldID);

		ASSERTCHECK(iBytes,TEXT("SetDescFieldValuePtr Alloc Failed"));

		if (!iBytes)
			return NULL;

		ValuePtr=AllocateMemory(iBytes);

		*(SQLHANDLE *)lpData=ValuePtr;
	}
	else if (IsCharData(fCType))
	{
		// fOption requires a string
		ValuePtr = lpData;
	}
	else
		(INT_PTR)ValuePtr = (INT_PTR)*(SDWORD *)lpData;

	return(ValuePtr);

}



//*---------------------------------------------------------------------------------
//| lpSQLSetDescField:
//|	This function sets an individual descriptor field
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
SQLRETURN INTFUN lpSQLSetDescField(STD_FH_PARMS)
{
	SQLRETURN				rc=SQL_SUCCESS;
	lpSTATEMENTINFO		lpStmt=NULL;
	SQLSMALLINT				iRecNum =	*(SQLSMALLINT *)lpParms[IPAR_SDF_IRECNUM]->lpData;
	SQLSMALLINT				fFieldID =*(SQLSMALLINT *)lpParms[IPAR_SDF_IFIELDIDENTIFIER]->lpData;
	PTR						NewRgb=NULL,
								tmpRgb=NULL;
	UNALIGNED SDWORD		*pcbValue=NULL;
	UNALIGNED SQLLEN		*tmpPcb=NULL;
	SDWORD					*cbValuePtr =(SDWORD *)lpParms[IPAR_SDF_CBBUFFERLENGTH]->lpData;
	SQLHDESC					hdesc=NULL;
	BOOL						fNullRgbValue=lpParms[IPAR_SDF_RGBVALUE]->fNull;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	lpDESCRIPTORRECORD	lpRec=NULL;
	SWORD						uData=0;
	lpBOUNDVARS				lpbv=NULL;
	UWORD						uType=BIND_INVALID;					// Assume invalid handle
	SDWORD					cbValueMaxUsed=0,
								cbTotValueMax=0;
	BOOL						fDoBindColumn=FALSE;
	SWORD						fType=0;

	if (ci)
		lpStmt=ci->lpCurStmt;

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[IPAR_SDF_HDESC]->lpData)
		hdesc=*(SQLHDESC *)lpParms[IPAR_SDF_HDESC]->lpData;

	if (!fNullRgbValue && (fFieldID != SQL_DESC_DATA_PTR))
	{
		NewRgb=SetDescFieldValuePtr(ci,
												fFieldID,
												lpParms[IPAR_SDF_RGBVALUE]->lpData,
												lpParms[IPAR_SDF_RGBVALUE]->fCType);
	}

	//Locate descriptor header record
	lpDesc=FindDescriptor(hdesc,ci);

	if (lpDesc)
	{
		//Find the desc. record
		lpRec=FindDescriptorRecord(lpDesc,iRecNum);

		switch(fFieldID)
		{
			case SQL_DESC_DATA_PTR:
				FREE_PTR(lpParms[IPAR_SDF_RGBVALUE]->lpData);
				lpParms[IPAR_SDF_RGBVALUE]->lpData = NULL;
				lpParms[IPAR_SDF_RGBVALUE]->cBytes = 0;

				if (!lpRec->sdOctetLen)
					lpRec->sdOctetLen=*cbValuePtr;

				if (!lpRec->swConciseType)
					lpRec->swConciseType=SQL_C_DEFAULT;

				lpbv=DoBindColumn(ci,iRecNum,
									&NewRgb,
									&(UNALIGNED SDWORD *)tmpPcb,&uType,
									lpRec->sdOctetLen,&cbValueMaxUsed,
									lpRec->swConciseType,
									fNullRgbValue,fNullRgbValue,
									fFieldID,&cbTotValueMax);
				fDoBindColumn=TRUE;
				break;
		}
	}

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLSetDescField(hdesc,
						iRecNum,
						fFieldID,
						fNullRgbValue ? NULL : NewRgb,
						*cbValuePtr);

	if (fDoBindColumn)
	{
	 	// If successful, we need to update the bound column information
		UpdateBoundColumn(ci,iRecNum,
								lpRec->swConciseType,
								NewRgb,
								tmpPcb,
								uType,
								lpbv,
								cbValueMaxUsed,
								rc,
								&cbTotValueMax);
	}

	//Set gator's Descriptor records
	if (RC_SUCCESSFUL(rc) && NewRgb)
	{
		SetDescriptorField(ci,lpStmt,lpDesc,fFieldID,
			&NewRgb,cbValuePtr ? *cbValuePtr : 0,iRecNum);
	}

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	//  Check for errors
	AUTOLOGERRORDESC(ci,rc,hdesc);

	return rc;

} //lpSQLSetDescField()



//*---------------------------------------------------------------------------------
//| lpSQLGetDescRec:
//|	This function gets an individual descriptor Record
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
SQLRETURN INTFUN lpSQLGetDescRec(STD_FH_PARMS)
{
	SQLRETURN			rc=SQL_SUCCESS;
	SQLHDESC				hdesc=NULL;
	SWORD					iRecNum =		*(SWORD *)lpParms[IPAR_GDR_IRECNUM]->lpData;
	SWORD					cbName =			*(SWORD *)  lpParms[IPAR_GDR_CBNAME		]->lpData;
	SWORD					*pcbName =		(SWORD *)   lpParms[IPAR_GDR_PCBNAME	]->lpData;
	SWORD					*pfType =		(SWORD *)   lpParms[IPAR_GDR_FTYPE		]->lpData;
	SWORD					*pfSubType =	(SWORD *)   lpParms[IPAR_GDR_FSUBTYPE	]->lpData;
	SQLLEN				*pColDef =		(SQLLEN *)  lpParms[IPAR_GDR_PCOLDEF	]->lpData;
	SWORD					*pPrecision =	(SWORD *)   lpParms[IPAR_GDR_PPRECISION]->lpData;
	SWORD					*pScale =		(SWORD *)   lpParms[IPAR_GDR_PSCALE		]->lpData;
	SWORD					*pNullable =	(SWORD *)   lpParms[IPAR_GDR_PFNULLABLE]->lpData;
	lpDESCRIPTORHEADER	lpDesc=NULL;

	if (ci)
	{
		if (ci->lpCurDesc)
			lpDesc=ci->lpCurDesc;
	}

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[IPAR_GDF_HDESC]->lpData)
		hdesc=*(SQLHDESC *)lpParms[IPAR_GDF_HDESC]->lpData;


	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[IPAR_GDR_SZNAME],				// allocate szName
						lpUsrOptions->uDftBuffLen,				// Use user-defined value
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLGetDescRec(	 lpParms[IPAR_GDR_HDESC]->fNull ? NULL : hdesc,
						    iRecNum,
							 lpParms[IPAR_GDR_SZNAME]->lpData,
						    cbName,
						    lpParms[IPAR_GDR_PCBNAME]->fNull ? NULL : pcbName,
						    lpParms[IPAR_GDR_FTYPE]->fNull ? NULL : pfType,
						    lpParms[IPAR_GDR_FSUBTYPE]->fNull ? NULL : pfSubType,
						    lpParms[IPAR_GDR_PCOLDEF]->fNull ? NULL : pColDef,
						    lpParms[IPAR_GDR_PPRECISION]->fNull ? NULL : pPrecision,
						    lpParms[IPAR_GDR_PSCALE]->fNull ? NULL : pScale,
						    lpParms[IPAR_GDR_PFNULLABLE]->fNull ? NULL : pNullable);


	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[IPAR_GDR_SZNAME], lpParms[IPAR_GDR_PCBNAME], TRUE);

	// Check for Null termination
	CHECKNULLTERM(ci, lpParms[IPAR_GDR_SZNAME], lpParms[IPAR_GDR_PCBNAME], TRUE);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	//  Check for errors
	AUTOLOGERRORDESC(ci,rc,hdesc);

	return rc;

} //lpSQLGetDescRec()

//*------------------------------------------------------------------------
//|  IsSamePtr:
//|		This function returns TRUE if the StringLengthPtr should have
//|	the same address as IndicatorPtr
//| Parms:
//|	lpStrLenParm	StringLengthPtr parameter
//|	lppfIndParm		IndicatorPtr parameter
//*---------------------------------------------------------------------------------

BOOL INTFUN IsSamePtr(lpPARAMITEM lpStrLenParm,lpPARAMITEM lppfIndParm)
{
	SDWORD	*psdfInd=NULL;

	if (lppfIndParm && lppfIndParm->lpData)
		return(*(SDWORD *)lppfIndParm->lpData == SAMEAS_STRLEN);

	return(FALSE);

} //IsSamePtr()


//*---------------------------------------------------------------------------------
//| IsARD:
//|	This function returns TRUE if the hdesc belongs to an ARD.
//| Parms:
//|	lpCI						Current connection information (always NULL)
//|	hdesc			the descriptor handle in question
//| Returns:
//|	TRUE if the hdesc is an ARD, otherwise FALSE
//*---------------------------------------------------------------------------------
BOOL INTFUN IsARD(lpCONNECTIONINFO lpCI,SQLHDESC hdesc)
{
	lpDESCRIPTORHEADER lpDesc=NULL;

	if (!lpCI || !hdesc || !lpCI->lpDesc)
		return(FALSE);

	lpDesc=FindDescriptor(hdesc,lpCI);

	return(lpDesc && lpDesc->uwDescType == DESC_ARD);

}

//*---------------------------------------------------------------------------------
//| IsPD:
//|	This function returns TRUE if the hdesc belongs to a parameter descriptor
//| Parms:
//|	lpCI						Current connection information (always NULL)
//|	hdesc			the descriptor handle in question
//| Returns:
//|	TRUE if the hdesc is an APD or an IPD, otherwise FALSE
//*---------------------------------------------------------------------------------
BOOL INTFUN IsPD(lpCONNECTIONINFO lpCI,SQLHDESC hdesc)
{
	lpDESCRIPTORHEADER lpDesc=NULL;

	if (!lpCI || !hdesc || !lpCI->lpDesc)
		return(FALSE);

	lpDesc=FindDescriptor(hdesc,lpCI);
	if (!lpDesc)
		return(FALSE);

	return(lpDesc->uwDescType == DESC_APD ||
			 lpDesc->uwDescType == DESC_IPD);

}

//*---------------------------------------------------------------------------------
//| IsARD:
//|	This function returns TRUE if the hdesc belongs to an APD.
//| Parms:
//|	lpCI						Current connection information (always NULL)
//|	hdesc			the descriptor handle in question
//| Returns:
//|	TRUE if the hdesc is an APD, otherwise FALSE
//*---------------------------------------------------------------------------------
BOOL INTFUN IsAPD(lpCONNECTIONINFO lpCI,SQLHDESC hdesc)
{
	lpDESCRIPTORHEADER lpDesc=NULL;

	if (!lpCI)
		return(FALSE);

	lpDesc=FindDescriptor(hdesc,lpCI);

	return(lpDesc->uwDescType == DESC_APD);

}


//*---------------------------------------------------------------------------------
//| lpSQLSetDescRec:
//|	This function sets an individual descriptor Record
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
SQLRETURN INTFUN lpSQLSetDescRec(STD_FH_PARMS)
{
	SQLRETURN				rc=SQL_SUCCESS;
	SQLHDESC				   hdesc =NULL;
	SWORD						iRecNum = *(SWORD *)lpParms[IPAR_SDR_IRECNUM]->lpData;
	SWORD					   fType = *(SWORD *)lpParms[IPAR_SDR_FTYPE]->lpData;
	SWORD						fSubType = *(SWORD *)lpParms[IPAR_SDR_FSUBTYPE]->lpData;
	SDWORD				   cbLength = *(SDWORD *)lpParms[IPAR_SDR_CBLENGTH]->lpData;
	SWORD					   ibPrecision = *(SWORD *)lpParms[IPAR_SDR_CBCOLDEF]->lpData;
	SWORD						ibScale = *(SWORD *)lpParms[IPAR_SDR_IBSCALE]->lpData;
	SQLLEN					*pcbsdwStringLength =(SQLLEN *)lpParms[IPAR_SDR_PCBSTRINGLENGTH]->lpData;
	SDWORD							*pcbValue=NULL;
	SQLLEN					*pIndicator =	(SQLLEN *)lpParms[IPAR_SDR_PFINDICATOR]->lpData;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	lpBOUNDVARS				lpbv=NULL;
	SDWORD					cbValueMaxUsed=0;
	PTR						rgbValue=NULL,
								OldRgbValue=NULL;
	BOOL						fNullRgbValue=lpParms[IPAR_SDR_RGBVALUE]->fNull;
	BOOL						fNullStrLen=lpParms[IPAR_SDR_PCBSTRINGLENGTH]->fNull;
	BOOL						fNullPInd=lpParms[IPAR_SDR_PFINDICATOR]->fNull;
	UWORD						uType=BIND_INVALID;					// Assume invalid handle
	SDWORD					cbTotValueMax=0;
	lpSTATEMENTINFO		lpStmt=ci ? ci->lpCurStmt : NULL;
	BOOL						fARD=FALSE,
								fSamePtr=FALSE,
								fCall=TRUE;
	SDWORD					*OldStrLenPtr=NULL,
								*OldIndPtr=NULL,
                                StringLength=0;
	SQLLEN						Indicator=0;
	lpPARAMITEM				lpStrLenPtrParam=lpParms[IPAR_SDR_PCBSTRINGLENGTH],
								lpIndPtrParam=lpParms[IPAR_SDR_PFINDICATOR],
								lpDataPtrParam=lpParms[IPAR_SDR_RGBVALUE];


	//Save original addresses to be restored later
	OldRgbValue=lpDataPtrParam->lpData;
	OldStrLenPtr=lpStrLenPtrParam->lpData;
	OldIndPtr=lpIndPtrParam->lpData;

	if (ci)
	{
		if (ci->lpCurDesc)
			lpDesc=ci->lpCurDesc;

		if (ci->lpCurStmt)
			lpStmt=ci->lpCurStmt;
	}

	//See if user wants to pass an invalid valid handle
	//(i.e. pass an hstmt when the API calls for an hdesc)
	if (lpParms[IPAR_SDR_HDESC]->lpData)
		hdesc=*(SQLHDESC *)lpParms[IPAR_SDR_HDESC]->lpData;

	fSamePtr=IsSamePtr(lpStrLenPtrParam,lpIndPtrParam);

	if (lpStrLenPtrParam->lpData)
	{
		//Save contents
		StringLength=*(SDWORD *)lpStrLenPtrParam->lpData;

		//We need to alloc StringLengthPtr
		lpStrLenPtrParam->lpData=AllocateMemory(sizeof(SDWORD));
		ASSERTCHECK(lpStrLenPtrParam->lpData,TEXT("StringLengthPtr Alloc Failed"));
		if (lpStrLenPtrParam->lpData)
			*(SDWORD *)lpStrLenPtrParam->lpData=StringLength;
	}


	if (!fSamePtr && lpIndPtrParam->lpData)
	{
		Indicator=*(SDWORD *)lpIndPtrParam->lpData;

		//We need to alloc IndicatorPtr
		lpIndPtrParam->lpData=AllocateMemory(sizeof(SDWORD));
		ASSERTCHECK(lpIndPtrParam->lpData,TEXT("IndicatorPtr Alloc Failed"));
		if (lpIndPtrParam->lpData)
			*(SDWORD *)lpIndPtrParam->lpData=(SDWORD)Indicator;

	}


	if (lpStmt && hdesc)
	{
		if (fARD=IsARD(ci,hdesc))
		{
			// Figure out which type of binding we're doing
			lpbv=DoBindColumn(ci,iRecNum,&rgbValue,
								&pcbValue,&uType,
								cbLength,&cbValueMaxUsed,fType,
								fNullRgbValue,fNullStrLen,0,
								&cbTotValueMax);
			//
			if ( !rgbValue && uType != CWB_UNBIND )
				return SQL_ERROR;

			if (OldRgbValue)
			{
				//now copy the contents in case the User entered
				//an rgb "value"
				if (rgbValue)
					memcpy(rgbValue,OldRgbValue,sizeof(SDWORD));

				//set these parameter so they are logged correctly
				lpDataPtrParam->lpData=rgbValue;
			}

			//lpStrLenPtrParam->lpData=pcbsdwStringLength;

			if (!lpStrLenPtrParam->lpData)
				lpStrLenPtrParam->fNull=TRUE;


		}
		else if (IsPD(ci,hdesc))
		{
			fCall=FALSE;

			//We are doing a Bind Parameter call the code common
			//to SQLBindParameter (i.e. the code that sets up gator's
			//internal lpParameter info.
			rc=lpBindParameter(szFuncName, cParms, ci,lpParms,uAttrib,lpPtr,
								IPAR_SDR_HDESC,
								IPAR_SDR_IRECNUM,
								-1, //IPAR_BPARAM_FPARAMTYPE,
								IPAR_SDR_FTYPE,
								IPAR_SDR_FSUBTYPE,
								IPAR_SDR_CBCOLDEF,
								IPAR_SDR_IBSCALE,
								IPAR_SDR_RGBVALUE,
								IPAR_SDR_CBLENGTH,
								IPAR_SDR_PCBSTRINGLENGTH,
								-1, //IPAR_BPARAM_DAE,
								-1, //IPAR_BPARAM_RS_HDBC,
								-1, //IPAR_BPARAM_RS_HSTMT,
								-1, //IPAR_BPARAM_COLUMN,
								-1, //IPAR_BPARAM_ROW,
								IPAR_SDR_CBLENGTH,
								fSamePtr ? NULL : (SDWORD *)lpIndPtrParam->lpData,
								SQL_API_SQLSETDESCREC);
		}
	}

	if (fCall)
	{
		// Log input parameters	moved by
		LOGPARAMETERSSPEC(szFuncName, lpParms, cParms, ci, TRUE);

		rc=SQLSetDescRec(hdesc,
							 iRecNum,
							 fType,
							 fSubType,
							 cbLength,
							 ibPrecision,
							 ibScale,
							 fNullRgbValue ? NULL : rgbValue,
							 fNullStrLen ? NULL : pcbsdwStringLength,
							 fNullPInd ? NULL : pIndicator);

	}


	if (RC_SUCCESSFUL(rc) && fCall)
	{
		//Set the appropriate descriptor fields
		SetDescriptorInfo(ci,					//Connection Info List
				hdesc,								// Descriptor Handle
				iRecNum,								// SQL_DESC_COUNT (ARD and IRD)
				NOT_USED,							// SQL_DESC_PARAMETER_TYPE (ARD)
				fType,								// SQL_DESC_TYPE (ARD)
				fType,								// SQL_DESC_TYPE (IRD)
				ibScale,								// SQL_DESC_SCALE (IRD)
				lpDataPtrParam->lpData,				// SQL_DESC_DATA_PTR (ARD)
				ibPrecision,						// SQL_DESC_LENGTH and/or SQL_DESC_PRECISION (APD)
				cbLength,							// SQL_DESC_OCTET_LENGTH (ARD)
				lpIndPtrParam->lpData,				// SQL_DESC_OCTET_LENGTH_PTR (ARD)
				SQL_API_SQLSETDESCREC);
	}

	if (fARD)
	{
		// If successful, we need to update the bound column information
		UpdateBoundColumn(ci,iRecNum,fType,rgbValue,pcbsdwStringLength,uType,
										lpbv,cbValueMaxUsed,rc,&cbTotValueMax);
	}

	if (fCall)
	{
		// Log return code information
		LOGRETURNCODE(NULL, ci, rc);

		//  Check for errors
		AUTOLOGERRORDESC(ci,rc,hdesc);
	}

	//Reset parameters to what they were before so that
	//gator doesn't free rgbValue,StringLengthPtr, and
	//IndicatorPtr
	lpDataPtrParam->lpData=OldRgbValue;
	lpStrLenPtrParam->lpData=OldStrLenPtr;
	lpIndPtrParam->lpData=OldIndPtr;

	return rc;

} //lpSQLSetDescRec()

//*------------------------------------------------------------------------
//| GetDescNameWndProc:
//|	Dialog proc prompts for the name of the currently allocated Descriptor
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
LRESULT EXTFUN GetDescNameWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
			{
			lpDESCRIPTORHEADER		lpDesc = (lpDESCRIPTORHEADER)lParam;
			TCHAR						szHandle[MEDBUFF]=TEXT(""),
										szAddress[MEDBUFF]=TEXT("");
			HWND						hwndFocus=NULL;
			CenterDialog(hDlg);

			SETWINDOWLONG(hDlg, lpDesc);

			//display hdesc
			SetDlgItemText(hDlg, IDT_DESC_NAME_HANDLE,lpDesc->szHdesc);

			//If descriptor description has already been set display it
			if (*lpDesc->szName)
			{
				SetDlgItemText(hDlg, IDC_DESC_NAME_DESC,lpDesc->szName);
			}

			// Set limits
			SendDlgItemMessage(hDlg, IDC_DESC_NAME_DESC, EM_LIMITTEXT, LARGEBUFF, 0L);

			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam))
				{
				case IDOK:
					{
					UINT 						cbBytes=0;
					lpDESCRIPTORHEADER		lpDesc=NULL;

					lpDesc = (lpDESCRIPTORHEADER)GETWINDOWLONG(hDlg);

					// Retrieve values
					cbBytes=GetDlgItemText(hDlg, IDC_DESC_NAME_DESC, lpDesc->szName,
						tsizeof(lpDesc->szName));

					//if user didn't enter a name use address as name
					if (!cbBytes)
						lstrcpy(lpDesc->szName,lpDesc->szHdesc);

					EndDialog(hDlg, IDOK);
					}
					return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}

			if(GET_WM_COMMAND_CODE(wParam, lParam) ==
						CBN_DBLCLK)
				SendMessage(hDlg, WM_COMMAND, IDOK, 0L);

			return FALSE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}

	return FALSE;

} //GetDescNameWndProc()




//*---------------------------------------------------------------------------------
//| AssociateNameWithDesc:
//|	This function displays the IDD_DESC_NAME dialog
//| Parms:
//|	lpCi			Current connection information
//|	hHndl			Descriptor handle
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID INTFUN AssociateNameWithDesc(lpDESCRIPTORHEADER lpDesc)
{
	// First get the connection info, if they cancel, nothing to do
	if(IDOK != DialogBoxParam(hInst,
					MAKEINTRESOURCE(IDD_DESC_NAME),
					hwndClient,
					GetDescNameWndProc,
					(LPARAM)lpDesc))
		return;

} //AssociateNameWithDesc()




//*------------------------------------------------------------------------
//| PickHdescWndProc:
//|	Dialog proc allows the user to pick a Descriptor handle from the
//|	list of valid HDESC's
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
LRESULT EXTFUN PickHdescWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SWORD	iDesc;

	switch(msg)
	{
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
			{
			lpCONNECTIONINFO		lpCI = (lpCONNECTIONINFO)lParam;
			lpDESCRIPTORHEADER	lpDesc=NULL;
			TCHAR						szHandle[MEDBUFF]=TEXT(""),
										szAddress[MEDBUFF]=TEXT("");
			HWND						hwndFocus=NULL;
			HWND						hwndCtl=GetDlgItem(hDlg,IDC_PDH_HDESC);

			CenterDialog(hDlg);

			SETWINDOWLONG(hDlg, lpCI);

			SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)TEXT("<null pointer>"));

			if (lpCI && lpCI->lpDesc)
				lpDesc=lpCI->lpDesc;

			//display all hdescs
			for (iDesc=0;iDesc < lpCI->cDescs;iDesc++)
			{
				wsprintf(szAddress,TEXT("%s \"%s\""), lpDesc->szHdesc,lpDesc->szName);
				SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)szAddress);
				lpDesc=lpDesc->next;
			}

			SendMessage(hwndCtl, CB_SETCURSEL, 0, 0L);
			SetFocus(hwndCtl);

			// Set limits
			SendDlgItemMessage(hDlg, IDC_PDH_HDESC, EM_LIMITTEXT, MEDBUFF, 0L);

			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam))
				{
				case IDOK:
					{
					UINT 						cbBytes=0;
					lpCONNECTIONINFO		lpCI=NULL;
					lpDESCRIPTORHEADER	lpDesc=NULL;
					TCHAR						szHandle[MEDBUFF]=TEXT(""),
												szHdesc[MEDBUFF]=TEXT("");

					lpCI = (lpCONNECTIONINFO)GETWINDOWLONG(hDlg);

					// Retrieve values
					cbBytes=GetDlgItemText(hDlg, IDC_PDH_HDESC, szHandle,
									tsizeof(szHandle));

					if (lpCI && lpCI->lpDesc)
					{
						lpDesc=lpCI->lpDesc;

						for (iDesc=0;iDesc < lpCI->cDescs;iDesc++)
						{
							//Set user selection
							if (_tcsstr(szHandle,lpDesc->szHdesc))
							{
								lpCI->lpCurDesc=lpDesc;
								break;
							}
							else
								lpDesc=lpDesc->next;
						}
					}

					EndDialog(hDlg, IDOK);
					}
					return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}

			if(GET_WM_COMMAND_CODE(wParam, lParam) ==
						CBN_DBLCLK)
				SendMessage(hDlg, WM_COMMAND, IDOK, 0L);

			return FALSE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}

	return FALSE;

}


//*---------------------------------------------------------------------------------
//| PickHdesc:
//|	This function allows the user to pick an HDESC handle from the list
//|	of currently allocated HDESC's
//| Parms:
//|	lpCi			Current connection information
//|	hHndl			Descriptor handle
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID INTFUN PickHdesc(lpCONNECTIONINFO lpCI)
{


	// First get the connection info, if they cancel, nothing to do
	if(IDOK != DialogBoxParam(hInst,
					MAKEINTRESOURCE(IDD_PICK_HDESC),
					hwndClient,
					PickHdescWndProc,
					(LPARAM)lpCI))
		return;

	fBusy(TRUE);


} //PickHdesc()


//*---------------------------------------------------------------------------------
//| DoAllocDescriptor:
//|	This function allocates a new descriptor node, prompts the user to associate
//|	a name with the descriptor and then adds the node to the current linked list.
//| Parms:
//|	lpCi			Current connection information
//|	hHndl			Descriptor handle
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
lpDESCRIPTORHEADER INTFUN DoAllocDescriptor(lpCONNECTIONINFO lpCi,SQLHANDLE hHndl,
														BOOL fName)
{
	lpDESCRIPTORHEADER		lpDesc=NULL;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	TCHAR						szErrorText[MEDBUFF],
								szHndl[MEDBUFF];

	//See if we already have a record for this descriptor handle
	if (!(lpDesc=FindDescriptor(hHndl,lpCi)) || !hHndl)
	{
		// Convert handle to Text for textual comparisons
		HandleToText(szHndl,hHndl);

		// Attempt to allocate a new linked list for the statement information
		lpDesc = (lpDESCRIPTORHEADER)AllocateMemory(sizeof(DESCRIPTORHEADER));
		if(!lpDesc)
			{
 			SQLFreeHandle(SQL_HANDLE_DESC, hHndl);
			szMessageBox(hwndClient,
					MB_ICONSTOP | MB_OK,
					(LPTSTR)szErrorText,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsFreeHandleDueToMemory));
			return NULL;
			}

		// Set information, then add to the linked list
		lpDesc->hdesc = hHndl;

		lstrcpy(lpDesc->szHdesc,szHndl);

		if (lpCi)
		{
			//The current descriptor number is set at the environment
			//level so that the numbering would be start at 1 for each Environment.
			lpDesc->uDescNum = ++lpActiveEnv->cCurDescNum;
		}

		if(lpCi->lpDesc)
		{
			// Insert at end of list
			lpCi->lpDesc->prev->next = lpDesc;
			lpDesc->prev = lpCi->lpDesc->prev;
			lpDesc->next = lpCi->lpDesc;
			lpCi->lpDesc->prev = lpDesc;
		}
		else
		{
			// Only one in the list
			lpCi->lpDesc =
			lpDesc->next =
			lpDesc->prev = lpDesc;
		}

		++lpCi->cDescs;

		//link descriptor with Connection info rec.
		lpDesc->lpCI=lpCi;

		//link descriptor with Statment info rec.
		lpDesc->lpSI=lpCi->lpCurStmt;

		//Set the szName field to "Explict Desc"
		lstrcpy(lpDesc->szName,TEXT("Explict Desc"));

		//set the  defaul type to ARD since this is the way applications use it most
		//later we need to prompt the user to enetr type of DESC.

		lpDesc->uwDescType = DESC_ARD;

	} //if (!(lpDesc=FindDescriptor(lpCi,hHndl)) || !hHndl)

	if (hHndl && fName)
		AssociateNameWithDesc(lpDesc);

	return lpDesc;

} //DoAllocDescriptor()


//*---------------------------------------------------------------------------------
//| DropAllDescs:
//|	This function will free all of the hdecs for a connection window.
//|	The caller determines if each hdescs must be dropped (won't be the
//|	case when SQLDisconnect is successful).
//| Parms:
//|	lpci			Connection window to free
//|	hdesc			Descriptor handle to be freed
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoDropDesc(lpCONNECTIONINFO lpci,SQLHDESC hdesc)
{
	lpDESCRIPTORHEADER	lpDesc=lpci->lpDesc;
	lpSTATEMENTINFO		lpStmt= (lpci)?lpci->lpCurStmt:NULL;
	UINT						cDescs=lpci->cDescs;

	if (!lpDesc)
		return;

	// Now remove the node from the list
	if (cDescs > 0)
	{
		while (cDescs-- && lpDesc->hdesc != hdesc)
			lpDesc = lpDesc->next;

		if(lpDesc->next != lpDesc)
		{		// More than one node
			lpDesc->prev->next = lpDesc->next;
			lpDesc->next->prev = lpDesc->prev;
			if(lpci->lpDesc == lpDesc)			// Removing head node?
				lpci->lpDesc = lpDesc->next;
		}
		else											// Removing only node
			lpci->lpDesc = NULL;

		if (lpci->cDescs > 0)
			--lpci->cDescs;


		if(lpci->lpActiveEnv)
			--lpci->lpActiveEnv->cCurDescNum;


		//since we are going to free rowstatus ptr set the stmt rowstatus ptr to NULL
		if(lpStmt && (lpStmt->rgfRowStatus == lpDesc->rgArrayStatusPtr))
			lpStmt->rgfRowStatus = NULL;

		FREE_PTR(lpDesc->rgArrayStatusPtr);
		FREE_PTR(lpDesc->psdBindOffsetPtr);
		FREE_PTR(lpDesc->pFetchBookMarkPtr);
		FREE_PTR(lpDesc->rghDesc);
		FREE_PTR(lpDesc->pOutOfLineHeapPtr);
		FREE_PTR(lpDesc->psdPredOctLenPtr);
		FREE_PTR(lpDesc->pudRowsProcPtr);

		FREE_PTR(lpDesc);
		lpDesc=NULL;

		// Refresh the toolbar so that some buttons get disabled correctly
		RefreshToolbar();

	}

} //DoDropDesc()


//*---------------------------------------------------------------------------------
//| DropAllDescs:
//|	This function will free all of the hdecs for a connection window.
//|	The caller determines if each hdescs must be dropped (won't be the
//|	case when SQLDisconnect is successful).
//| Parms:
//|	lpci						Connection window to free
//|	fNeedToFree				TRUE if we must try to free each hstmt
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN DropAllDescs(lpCONNECTIONINFO lpci)
{
	SQLRETURN 				rc=SQL_SUCCESS;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	lpSTATEMENTINFO		lpStmt= (lpci )? lpci->lpStmt:NULL;
	SQLHDESC					hdesc=NULL;
	SWORD						cDescs=lpci->cDescs;

	if(!lpci || !lpci->lpDesc)
		return TRUE;

	lpDesc=lpci->lpDesc;

	if (cDescs <= 0)
		return(TRUE);

	while(cDescs-- && lpci->lpDesc)
	{
		hdesc=lpci->lpDesc->hdesc;
		lpDesc=lpci->lpDesc;

		//Only Free Explicitly allocated hdescs
		if ((lpDesc->uwAllocType == SQL_DESC_ALLOC_USER) && hdesc)
			rc = SQLFreeHandle(SQL_HANDLE_DESC,hdesc);

		if(RC_NOTSUCCESSFUL(rc))
			return FALSE;

		//Remove node from linked list

		if(lpStmt &&  (lpStmt->lpARD == lpDesc )	)
			lpStmt->lpARD = NULL;
		if(lpStmt &&  (lpStmt->lpAPD == lpDesc )	)
			lpStmt->lpAPD = NULL;
		if(lpStmt &&  (lpStmt->lpIRD == lpDesc )	)
			lpStmt->lpIRD = NULL;
		if(lpStmt &&  (lpStmt->lpIPD == lpDesc )	)
			lpStmt->lpIPD = NULL;
		DoDropDesc(lpci,hdesc);





	}

	return TRUE;

} //DropAllDescs



//*---------------------------------------------------------------------------------
//| FindDescriptor:
//|	This function searches lpCI, for the DESCRIPTORHEADER record associated
//|	with the hDesc
//| Parms:
//|	lpci				Connection window to search
//|	hDesc				Descriptor handle to be found
//| Returns:
//|	lpDESCRIPTORHEADER	pointer to desc. record.
//*---------------------------------------------------------------------------------

lpDESCRIPTORHEADER INTFUN FindDescriptor(SQLHDESC hdesc,lpCONNECTIONINFO lpCI)
{
	lpDESCRIPTORHEADER	lpDesc=NULL;
	SWORD						cDescs=0;

	//check pointers
	if (lpCI)
	{
		cDescs=lpCI->cDescs;

		if (lpCI->lpDesc)
			lpDesc=lpCI->lpDesc;
		else
			ASSERTCHECK(lpCI->lpDesc, "Null lpDesc");

	}
	else
		ASSERTCHECK(lpCI, "Null lpCI");

	//find the record now
	if (lpDesc && cDescs > 0)
	{
		while (cDescs--)
		{
			if (lpDesc->hdesc == hdesc)
				//return descriptor record
				return(lpDesc);

			lpDesc=lpDesc->next;
		}
	}

	return(NULL);

} //FindDescriptorRecord()



//*---------------------------------------------------------------------------------
//| FindRowDescriptor:
//|	This function searches for the lpRec specified by iRec
//| Parms:
//|	lpDesc		Descriptor record to process
//|	iRec			record number to find (i.e. descriptor row number)
//| Returns:
//|	lpRec			row descriptor record
//*---------------------------------------------------------------------------------

lpDESCRIPTORRECORD INTFUN FindRowDescriptor(lpDESCRIPTORHEADER lpDesc, UWORD iRec)
{
	lpDESCRIPTORRECORD	lpRec=NULL;
	SWORD					cRecs=0;

	//Validate Row Record
	if (!lpDesc || !lpDesc->lpRec)
		return(NULL);

	lpRec=lpDesc->lpRec;
	cRecs=lpDesc->cRecs;

	//If record we are searching for is greater than
	//number of rows in Descriptor record, it doesn't
	//exist
	if (cRecs || (iRec > cRecs))
		return(NULL);

	//Find the row record
	while (cRecs-- && lpRec)
		lpRec=lpRec->next;

	return(lpRec);

} //FindRowDescriptor()


//*---------------------------------------------------------------------------------
//| SetDescriptorInfo:
//|	This function sets the appropriate descriptor fields in lpDesc. This function
//|	is called by any API that needs to set descriptor fields.
//| Parms:
//|	ci				pointer to Current connection info record
//|	hdesc			Desc. handle to process
//|	iRec			irec is either icol(SQLBindCol) or ipar(SQLBindParameter/Param)
//|	fParamType	SQL_DESC_PARAM_TYPE field
//|	fCType		SQL_DESC_TYPE field
//|	fSQLType		SQL_DESC_TYPE field
//|	rgbValue 	SQL_DESC_DATA_PTR field
//|	cbValueMax	SQL_DESC_OCTET_LENGTH field
//|	cbColDef		SQL_DESC_LENGTH and/or SQL_DESC_PRECISION
//|	pcbValue		SQL_DESC_INDICATOR_POINTER field
//|	udAPI			API number (as defined in SQL.H) of calling function.
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN SetDescriptorInfo(lpCONNECTIONINFO lpci,
		SQLHDESC hdesc,
		UWORD iRec,
		SWORD fParamType,
		SWORD fCType,
		SWORD fSQLType,
		SWORD	ibScale,
		PTR rgbValue,
		UDWORD cbColDef,
		SDWORD cbValueMax,
		SQLLEN *pcbValue,
		UDWORD udAPI)
{
	lpDESCRIPTORHEADER	lpDesc=NULL;
	lpDESCRIPTORRECORD		lpRec=NULL;

	//Find the descriptor record for this hdesc
	if (!hdesc)
		return;

	lpDesc=FindDescriptor(hdesc,lpci);

	//Check if record exists, if it doesn't allocate it
	if (!(lpRec=FindRowDescriptor(lpDesc,iRec)))
		lpRec=AddDescriptorRecord(lpDesc,iRec);

	ASSERTCHECK(lpRec,TEXT("NULL lpRec in SetDescriptorInfo()"));

	if (!lpRec)
		return;

	if (fParamType)
		lpRec->swParmType=fParamType;

	if (rgbValue)
		lpRec->pDataPtr=rgbValue;

	if (cbValueMax)
		lpRec->sdOctetLen=cbValueMax;

	if (pcbValue)
		lpRec->psdOctLenPtr=(SDWORD *)pcbValue;

	if (fCType)
		lpRec->swType=fCType;

	if (fSQLType)
		lpRec->swType=fSQLType;

	if (cbColDef)
	{
		//lpRec->swPrecision=cbColDef;
		lpRec->udLength=cbColDef;
	}


} //SetDescriptorInfo()


//*---------------------------------------------------------------------------------
//| FormatDescField:
//|	This function formats the desc. field correctly
//| Parms:
//|	swFieldID	Desc Field to format
//|	szData				Formatted string
//|	ValuePtr				Data to format
//|	lpDft					default array to use
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN FormatDescField(SWORD swFieldID,LPTSTR szData, PTR ValuePtr,
									 lpDFTARRAY lpDft)
{

	TCHAR szOut[XLARGEBUFF]=TEXT("");

	GetExpandedConstant(szOut,lpDft,FALSE);

	switch(swFieldID)
	{
		//string fields
		case SQL_DESC_BASE_COLUMN_NAME:
		case SQL_DESC_CATALOG_NAME:
		case SQL_DESC_LABEL:
		case SQL_DESC_LITERAL_PREFIX:
		case SQL_DESC_LITERAL_SUFFIX:
		case SQL_DESC_LOCAL_TYPE_NAME:
		case SQL_DESC_NAME:
		case SQL_DESC_TABLE_NAME:
		case SQL_DESC_TYPE_NAME:
		case SQL_DESC_SCHEMA_NAME:
			wsprintf(szData,TEXT("\"%s\""), (LPTSTR)ValuePtr);
			break;

		//16 bit fields
		case SQL_DESC_ALLOC_TYPE:
		case SQL_DESC_COUNT:
		case SQL_DESC_CONCISE_TYPE:
		case SQL_DESC_DATETIME_INTERVAL_CODE:
		case SQL_DESC_FIXED_PREC_SCALE:
		case SQL_DESC_NULLABLE:
		case SQL_DESC_PARAMETER_TYPE:
		case SQL_DESC_PRECISION:
		case SQL_DESC_SCALE:
		case SQL_DESC_SEARCHABLE:
		case SQL_DESC_TYPE:
		case SQL_DESC_UNNAMED:
		case SQL_DESC_UNSIGNED:
		case SQL_DESC_UPDATABLE:
			wsprintf(szData,TEXT("%d"), *(SWORD *)ValuePtr);
			break;

		//32 bit handles
	#ifdef  SQL_DESC_BIND_OUT_OF_LINE
		case SQL_DESC_BIND_OUT_OF_LINE:
	#endif
	#ifdef	SQL_DESC_PREDICATE_OCTET_LENGTH_PTR
		case SQL_DESC_PREDICATE_OCTET_LENGTH_PTR:
		case SQL_DESC_PREDICATE_PTR:
		case SQL_DESC_FETCH_BOOKMARK_PTR:
	#endif
		case SQL_DESC_ARRAY_STATUS_PTR:
		case SQL_DESC_BIND_OFFSET_PTR:
		case SQL_DESC_ROWS_PROCESSED_PTR:
		case SQL_DESC_DATA_PTR:
		case SQL_DESC_INDICATOR_PTR:
		case SQL_DESC_OCTET_LENGTH_PTR:
			HandleToText(szData,*(SQLHANDLE *)ValuePtr);
			break;

		//32 bit fields
		case SQL_DESC_ARRAY_SIZE:
		case SQL_DESC_BIND_TYPE:
		case SQL_DESC_AUTO_UNIQUE_VALUE:
		case SQL_DESC_CASE_SENSITIVE:
		case SQL_DESC_DATETIME_INTERVAL_PRECISION:
		case SQL_DESC_DISPLAY_SIZE:
		case SQL_DESC_LENGTH:
		case SQL_DESC_OCTET_LENGTH:
			wsprintf(szData,TEXT("%d"), *(SDWORD *)ValuePtr);
			break;

	} //switch(swFieldID)

} //FormatDescField()


//*---------------------------------------------------------------------------------
//| GetDescriptorAll
//|	This function retrieves and lists all descriptor fields for all descriptors
//|	on this connection.
//| Parms:
//|	lpCI		pointer to Current connection info record
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN GetDescriptorAll(lpCONNECTIONINFO lpCI)
{
	lpDESCRIPTORHEADER	lpDesc=NULL;
	SWORD						iDesc=0;
	UINT						iField=0;
	SQLHDESC					hdesc=NULL;
	SWORD						iRecord=0;
	RETCODE					rc=SQL_SUCCESS;
	PTR						ValuePtr=NULL;
	SDWORD					sdwStringLength=0;
	SWORD						swFieldID=0;
	TCHAR						szData[XLARGEBUFF]=TEXT("");
	HWND						hwndOut = GETOUTPUTWINDOW(lpCI);
	SWORD						cDescRecs=-1;
	TCHAR		szHandle[MEDBUFF]=TEXT(""),
				szHndlStr[MEDBUFF]=TEXT("");
	SDWORD	dwDex=0;


	//check for valid Descriptor info
	if (!lpCI || !lpCI->lpDesc)
		return;

	lpDesc=lpCI->lpDesc;
	hdesc=lpDesc->hdesc;

	ValuePtr=AllocateMemory(lpUsrOptions->uDftBuffLen);

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			TEXT("\r\nGet Descriptors All:\r\n"));

	//Loop through each descriptor dumping each field
	for (iDesc=0;iDesc < lpCI->cDescs;iDesc++)
	{
		FormatDescHandle(lpCI,lpDesc,szHndlStr);

		// Print descriptor handle we are currently working on
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			TEXT("\r\n\t\t\t%s"),szHndlStr);

		// Print descriptor handle we are currently working on
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			TEXT("\r\n\t\t\tHeader:\r\n"));

		//Loop through each field in the record
		for (iField=0;iField < cDescHdrFields;iField++)
		{
			swFieldID=(SWORD)lpdftSetDescHdrFields[iField].fOpt;

			if (!IsValidDescField(swFieldID,lpDesc->uwDescType))
				continue;

			rc=SQLGetDescField(lpDesc->hdesc,
									0,
									swFieldID,
									ValuePtr,
									lpUsrOptions->uDftBuffLen,
									&sdwStringLength);

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						TEXT("\t\t\t\t\t\t\t\t%s:"),
						lpdftSetDescHdrFields[iField].szOpt);

			if (rc != SQL_SUCCESS)
				ToolOptionsAllErrors(lpCI, NULL, NULL, NULL,lpDesc->hdesc,rc);
			else
			{
				FormatDescField(swFieldID,szData,ValuePtr,
					&lpdftSetDescHdrFields[iField]);

				// Now we start printing information
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
							(LPTSTR)TEXT("%s\r\n"),
							szData);

			} //if (rc != SQL_SUCCESS)

		} //Field loop

		//Get total number of desc. records available
		rc=SQLGetDescField(lpDesc->hdesc,
								0,
								SQL_DESC_COUNT,
								&cDescRecs,
								lpUsrOptions->uDftBuffLen,
								&sdwStringLength);

		//Inform user that there are no Desc. Records
		if (!cDescRecs)
		{
			// Now we start printing information
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
						(LPTSTR)TEXT("No Descriptor Record Fields Available\r\n"));
		}

		//Loop through all other records
		for (iRecord=1; iRecord <= cDescRecs && rc != SQL_NO_DATA_FOUND;iRecord++)
		{
			// Print descriptor handle we are currently working on
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
				TEXT("\r\n\t\t\t\tRecord: #%d\r\n"),iRecord);

			//Loop through each field in the record
			for (iField=0;iField < cDescRecFields;iField++)
			{
				swFieldID=(SWORD)lpdftSetDescRecFields[iField].fOpt;

				if (!IsValidDescField(swFieldID,lpDesc->uwDescType))
					continue;

				rc=SQLGetDescField(lpDesc->hdesc,
										iRecord,
										swFieldID,
										ValuePtr,
										lpUsrOptions->uDftBuffLen,
										&sdwStringLength);

				szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
							TEXT("\t\t\t\t\t\t\t\t%s:"),
							lpdftSetDescRecFields[iField].szOpt);


				if (rc != SQL_SUCCESS)
					ToolOptionsAllErrors(lpCI, NULL, NULL, NULL,lpDesc->hdesc,rc);
				else
				{
					FormatDescField(swFieldID,szData,ValuePtr,
						&lpdftSetDescRecFields[iField]);

					// Now we start printing information
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
								(LPTSTR)TEXT("%s\r\n"),
								szData);

				} //if (rc != SQL_SUCCESS)

			} //Field loop

		} //Record loop

		lpDesc=lpDesc->next;

	} //Descriptor loop

	FREE_PTR(ValuePtr);

} //GetDescriptorAll()


//*---------------------------------------------------------------------------------
//| FindDescriptorRecord:
//|	This function attempts to locate the Descriptor record specified by swRecNum
//| Parms:
//|	lpDesc		Descriptor record to process
//|	swRecNum		record number (i.e. descriptor row number)
//| Returns:
//|	lpRec			row descriptor record
//*---------------------------------------------------------------------------------

lpDESCRIPTORRECORD INTFUN FindDescriptorRecord(lpDESCRIPTORHEADER lpDesc,SWORD swRecNum)
{
	SWORD						cRecs=0,
								iRec;
	lpDESCRIPTORRECORD	lpRec=NULL;

	if (!lpDesc)
		return(NULL);

	lpRec=lpDesc->lpRec;
	cRecs=lpDesc->cRecs;

	for (iRec=0; iRec < cRecs; iRec++)
	{
		if (lpRec->swRecNum == swRecNum)
			break;

		lpRec=lpRec->next;

	}

	//Couldn't find record so allocate one
	if (!lpRec || (lpRec && lpRec->swRecNum != swRecNum))
		lpRec=AddDescriptorRecord(lpDesc,swRecNum);

	//Return the record
	return(lpRec);

} //FindDescriptorRecord()


//*---------------------------------------------------------------------------------
//| IsValidDescField:
//|	This function TRUE if szFieldID is valid for this descriptor type
//|		otherwise FALSE.
//| Parms:
//|	swFieldID		Descriptor fieldindentifier arg.
//|	uwType			Descriptor type (i.e. DESC_ARD,DESC_APD,DESC_IRD,DESC_IPD)
//| Returns:
//|	TRUE if szFieldID is valid for this descriptor type
//|		otherwise FALSE.
//*---------------------------------------------------------------------------------

BOOL INTFUN IsValidDescField(SWORD swFieldID,UWORD uwType)
{
	switch(swFieldID)
	{
		//-=-=-=-=-=-=- Valid for all descriptors -=-=-=-=-=-=-
		case SQL_DESC_ARRAY_STATUS_PTR:
		case SQL_DESC_ALLOC_TYPE:
		case SQL_DESC_CONCISE_TYPE:
		case SQL_DESC_DATETIME_INTERVAL_CODE:
		case SQL_DESC_DATETIME_INTERVAL_PRECISION:
		case SQL_DESC_LENGTH:
		case SQL_DESC_OCTET_LENGTH:
		case SQL_DESC_PRECISION:
		case SQL_DESC_SCALE:
		case SQL_DESC_TYPE:
		{
			return(TRUE);
		}

		case SQL_DESC_COUNT:
		{
			return(uwType != DESC_IRD);
		}

		//-=-=-=-=-=-=- Valid for App. descriptors -=-=-=-=-=-=-
		case SQL_DESC_ARRAY_SIZE:
		case SQL_DESC_BIND_OFFSET_PTR:
		case SQL_DESC_BIND_TYPE:
#ifdef SQL_DESC_OUT_OF_LINE_HEAP_OCTET_LENGTH
		case SQL_DESC_OUT_OF_LINE_HEAP_OCTET_LENGTH:
		case SQL_DESC_OUT_OF_LINE_HEAP_PTR:
		case SQL_DESC_BIND_OUT_OF_LINE:
#endif
		case SQL_DESC_DATA_PTR:
		case SQL_DESC_INDICATOR_PTR:
		case SQL_DESC_OCTET_LENGTH_PTR:
		{
			return(uwType == DESC_ARD || uwType == DESC_APD);
		}

		//-=-=-=-=-=-=- Valid for Imp. descriptors -=-=-=-=-=-=-
		case SQL_DESC_ROWS_PROCESSED_PTR:
		case SQL_DESC_CASE_SENSITIVE:
		case SQL_DESC_FIXED_PREC_SCALE:
		case SQL_DESC_LOCAL_TYPE_NAME:
		case SQL_DESC_NAME:
		case SQL_DESC_NULLABLE:
		case SQL_DESC_TYPE_NAME:
		case SQL_DESC_UNNAMED:
		case SQL_DESC_UNSIGNED:
		{
			return(uwType == DESC_IRD || uwType == DESC_IPD);
		}

		//-=-=-=-=-=-=- Valid for IRDs ONLY -=-=-=-=-=-=-
		case SQL_DESC_AUTO_UNIQUE_VALUE:
		case SQL_DESC_BASE_COLUMN_NAME:
#ifdef SQL_DESC_BASE_TABLE_NAME
		case SQL_DESC_BASE_TABLE_NAME:
#endif
		case SQL_DESC_CATALOG_NAME:
		case SQL_DESC_DISPLAY_SIZE:
		case SQL_DESC_LABEL:
		case SQL_DESC_LITERAL_PREFIX:
		case SQL_DESC_LITERAL_SUFFIX:
		case SQL_DESC_SCHEMA_NAME:
		case SQL_DESC_SEARCHABLE:
		case SQL_DESC_TABLE_NAME:
		case SQL_DESC_UPDATABLE:
		{
			return(uwType == DESC_IRD);
		}

		//-=-=-=-=-=-=- Valid for IPDs ONLY -=-=-=-=-=-=-
		case SQL_DESC_PARAMETER_TYPE:
		{
			return(uwType == DESC_IPD);
		}

	}

    return FALSE;
} //IsValidDescField()


//*---------------------------------------------------------------------------------
//| FillDescriptorInfo:
//|	This function fills in (i.e. calls GetDescField) all of the fields for the
//|	specified descriptor record.
//| Parms:
//|	lpDesc	Descriptor record to fill
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN FillDescriptorInfo(lpCONNECTIONINFO lpCI,lpDESCRIPTORHEADER lpDesc)
{
	SWORD						iDesc=0;
	UINT						iField=0;
	SQLHDESC					hdesc=NULL;
	SWORD						iRecord=0;
	RETCODE					rc=SQL_SUCCESS;
	PTR						ValuePtr=NULL;
	SDWORD					sdwStringLength=0;
	SWORD						swFieldID=0;
	TCHAR						szData[XLARGEBUFF]=TEXT("");
	SWORD						cDescRecs=-1;
	UWORD						cbChar=sizeof(TCHAR);
	lpDESCRIPTORRECORD	lpRec=NULL;
	PTR						rgbValue=NULL;

	//check for valid Descriptor info
	if (!lpDesc)
		return;

	hdesc=lpDesc->hdesc;

	HandleToText(lpDesc->szHdesc,hdesc);

	ValuePtr=AllocateMemory(lpUsrOptions->uDftBuffLen);

	ASSERTCHECK(ValuePtr,TEXT("ValuePtr Alloc Failed!!"));

	if (!ValuePtr)
		return;

	//Loop through each field in the record
	for (iField=0;iField < cDescHdrFields;iField++)
	{
		swFieldID=(SWORD)lpdftSetDescHdrFields[iField].fOpt;

		//check if this field ID is valid for this type
		//of descriptor
		if (!IsValidDescField(swFieldID,lpDesc->uwDescType))
			continue;

		rc=SQLGetDescField(hdesc,
								0,
								swFieldID,
								ValuePtr,
								lpUsrOptions->uDftBuffLen,
								&sdwStringLength);

		if (RC_NOTSUCCESSFUL(rc))
			ToolOptionsAllErrors(lpCI, NULL, NULL, NULL,hdesc,rc);
		else if (ValuePtr)
		{
			//Set the descriptor record field.
			SetDescriptorField(lpCI,lpCI->lpCurStmt,lpDesc,(SDWORD)swFieldID,
									ValuePtr,sdwStringLength,iRecord);

		} //if (rc != SQL_SUCCESS)

	} //Field loop


	//Loop through all other records
	for (iRecord=1; iRecord <= lpDesc->swCount && rc != SQL_NO_DATA_FOUND;iRecord++)
	{
		//Loop through each field in the record
		for (iField=0;iField < cDescRecFields;iField++)
		{
			swFieldID=(SWORD)lpdftSetDescRecFields[iField].fOpt;

			//check if this field ID is valid for this type
			//of descriptor
			if (!IsValidDescField(swFieldID,lpDesc->uwDescType))
				continue;

			rc=SQLGetDescField(hdesc,
									iRecord,
									swFieldID,
									ValuePtr,
									lpUsrOptions->uDftBuffLen,
									&sdwStringLength);


			if (RC_NOTSUCCESSFUL(rc))
				ToolOptionsAllErrors(lpCI, NULL, NULL, NULL,hdesc,rc);
			else if (ValuePtr)
			{
				//Set the descriptor record field.
				SetDescriptorField(lpCI,lpCI->lpCurStmt,lpDesc,(SDWORD)swFieldID,
										ValuePtr,sdwStringLength,iRecord);
			}

		} //Field loop

	} //Record loop

	FREE_PTR(ValuePtr);

} //FillDescriptorInfo()


//*---------------------------------------------------------------------------------
//| SetDescriptorField:
//|	This function sets, in the lpDESCRIPTORRECORD, the field specified by
//|	swFieldID, in the lpRec struct.
//| Parms:
//|	lpRec					Descriptor record to fill
//|	swFieldID	ID of record field to set
//|	ValuePtr				pointer to data or data
//|	iRec					record number
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN SetDescriptorField(lpCONNECTIONINFO lpCI,lpSTATEMENTINFO lpStmt,
										 lpDESCRIPTORHEADER lpDesc,SDWORD sdFieldIndentifier,
										 PTR ValuePtr,SDWORD sdwStringLength,SWORD iRec)
{
	SWORD						cbChar=sizeof(TCHAR);
	lpDESCRIPTORRECORD	lpRec=NULL;

	if (!lpDesc)
		return;

	if (iRec > 0)
	{
		//Check if Descriptor record exists, if not allocate, then
		//return pointer
		if (!(lpRec=FindDescriptorRecord(lpDesc,iRec)))
			return;
	}

	switch(sdFieldIndentifier)
	{
		//-=-=-=-=-=-=-=-=- HEADER FIELDS =-=-=-=-=-=-=-=-=-=-=-=-=-=-
		case SQL_DESC_ALLOC_TYPE:
			lpDesc->uwAllocType=*(UWORD *)ValuePtr;
			break;
		case SQL_DESC_BIND_OFFSET_PTR:
			lpDesc->psdBindOffsetPtr=(PTR)*(HANDLE *)ValuePtr;
			break;
		case SQL_DESC_BIND_TYPE:
			lpDesc->sdBindType=*(SDWORD *)ValuePtr;
			break;
		case SQL_DESC_ARRAY_SIZE:
			lpDesc->udArraySize=*(UDWORD *)ValuePtr;
			ChangeRowsetSize(lpCI, lpStmt,lpDesc->udArraySize, TRUE,SQL_ATTR_ROW_ARRAY_SIZE);
			break;
		case SQL_DESC_ARRAY_STATUS_PTR:
			if (!ValuePtr)
				return;
			lpDesc->rgArrayStatusPtr=(PTR)*(HANDLE *)ValuePtr;
			break;
		case SQL_DESC_COUNT:
			lpDesc->swCount=*(SWORD *)ValuePtr;
			break;
#ifdef SQL_DESC_OUT_OF_LINE_HEAP_OCTET_LENGTH
		case SQL_DESC_OUT_OF_LINE_HEAP_OCTET_LENGTH:
			lpDesc->sdOOLHeapOctetLength=*(SDWORD *)ValuePtr;
			break;
		case SQL_DESC_OUT_OF_LINE_HEAP_PTR:
			lpDesc->pOutOfLineHeapPtr=(PTR)*(HANDLE *)ValuePtr;
			break;
#endif
		case SQL_DESC_ROWS_PROCESSED_PTR:
			lpDesc->pudRowsProcPtr=(PTR)*(HANDLE *)ValuePtr;
			break;

		//-=-=-=-=-=-=-=-=- RECORD FIELDS =-=-=-=-=-=-=-=-=-=-=-=-=-=-

		case SQL_DESC_AUTO_UNIQUE_VALUE:
			if (lpRec)
				lpRec->fAutoUniqueValue=*(BOOL *)ValuePtr;
			break;
#ifdef SQL_DESC_BIND_OUT_OF_LINE
		case SQL_DESC_BIND_OUT_OF_LINE:
			if (lpRec)
				lpRec->uwBindOutOfLine=*(UWORD *)ValuePtr;
			break;
#endif
		case SQL_DESC_BASE_COLUMN_NAME:
			if (lpRec && lpRec->pszBaseColName)
				lstrcpy(lpRec->pszBaseColName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_CASE_SENSITIVE:
			if (lpRec)
				lpRec->fCaseSensitive=*(BOOL *)ValuePtr;
			break;
		case SQL_DESC_CATALOG_NAME:
			if (lpRec && lpRec->pszCatName)
				lstrcpy(lpRec->pszCatName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_CONCISE_TYPE:
			if (lpRec)
				lpRec->swConciseType=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_DATA_PTR:
			if (lpRec)
				lpRec->pDataPtr=(PTR)*(HANDLE *)ValuePtr;
			break;
		case SQL_DESC_DATETIME_INTERVAL_CODE:
			if (lpRec)
				lpRec->swDTInterval=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_DATETIME_INTERVAL_PRECISION:
			if (lpRec)
				lpRec->sdDTIntPrec=*(SDWORD *)ValuePtr;
			break;
		case SQL_DESC_DISPLAY_SIZE:
			if (lpRec)
				lpRec->sdDisplaySize=*(SDWORD *)ValuePtr;
			break;
		case SQL_DESC_FIXED_PREC_SCALE:
			if (lpRec)
				lpRec->fFixedPrecScale=*(BOOL *)ValuePtr;
			break;
		case SQL_DESC_INDICATOR_PTR:
			if (lpRec)
				lpRec->pswIndPtr=(PTR)*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_LABEL:
			if (lpRec && lpRec->pszLabel)
				lstrcpy(lpRec->pszLabel,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_LENGTH:
			if (lpRec)
				lpRec->udLength=*(UDWORD *)ValuePtr;
			break;
		case SQL_DESC_LITERAL_PREFIX:
			if (lpRec && lpRec->pszLitPrefix)
				lstrcpy(lpRec->pszLitPrefix,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_LITERAL_SUFFIX:
			if (lpRec && lpRec->pszLitSuffix)
				lstrcpy(lpRec->pszLitSuffix,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_LOCAL_TYPE_NAME:
			if (lpRec && lpRec->pszLocTypeName)
				lstrcpy(lpRec->pszLocTypeName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_NAME:
			if (lpRec && lpRec->pszName)
				lstrcpy(lpRec->pszName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_NULLABLE:
			if (lpRec)
				lpRec->swNullable=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_OCTET_LENGTH:
			if (lpRec)
				lpRec->sdOctetLen=*(SDWORD *)ValuePtr;
			break;
		case SQL_DESC_OCTET_LENGTH_PTR:
			if (lpRec)
				lpRec->psdOctLenPtr=(PTR)(LONG_PTR)*(SDWORD *)ValuePtr;
			break;
		case SQL_DESC_PARAMETER_TYPE:
			if (lpRec)
				lpRec->swParmType=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_PRECISION:
			if (lpRec)
				lpRec->swPrecision=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_SCALE:
			if (lpRec)
				lpRec->swScale=*(SWORD *)ValuePtr;
			break;

		case SQL_DESC_SCHEMA_NAME:
			if (lpRec && lpRec->pszSchemaName)
				lstrcpy(lpRec->pszSchemaName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_SEARCHABLE:
			if (lpRec)
				lpRec->swSearchable=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_TYPE:
			if (lpRec)
				lpRec->swType=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_TABLE_NAME:
			if (lpRec && lpRec->pszTableName)
				lstrcpy(lpRec->pszTableName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_TYPE_NAME:
			if (lpRec && lpRec->pszTypeName)
				lstrcpy(lpRec->pszTypeName,(LPTSTR)ValuePtr);
			break;
		case SQL_DESC_UNNAMED:
			if (lpRec)
				lpRec->fUnNamed=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_UNSIGNED:
			if (lpRec)
				lpRec->fUnsigned=*(SWORD *)ValuePtr;
			break;
		case SQL_DESC_UPDATABLE:
			if (lpRec)
				lpRec->swUpdateable=*(SWORD *)ValuePtr;
			break;

	} //switch(FieldIndentifier)

} //SetDescriptorField()

//*---------------------------------------------------------------------------------
//| GetDescHandle:
//|	This function gets the HDESC for the specified Implicit
//|	descriptor
//| Parms:
//|	lpStmt			Statement info record
//|	udAttribute		Descriptor
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

SQLHDESC INTFUN GetDescHandle(lpSTATEMENTINFO lpStmt,UDWORD udAttribute)
{
	HSTMT			hstmt=NULL;
	SQLHDESC		hdesc=NULL;
	RETCODE		rc=SQL_SUCCESS;

	if (!lpStmt)
		return(NULL);

	hstmt=lpStmt->hstmt;

	rc=SQLGetStmtAttr(hstmt,udAttribute,&hdesc,SQL_IS_POINTER,NULL);

	if (RC_NOTSUCCESSFUL(rc))
		return(NULL);
	else
		return(hdesc);

} //GetDescHandle()

//*------------------------------------------------------------------------
//| LoadDescDefWndProc:
//|	This Wnd Proc supports the	IDD_LOAD_DESCRIPTOR_DEFAULT dialog
//| Parms:
//|	Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
LRESULT EXTFUN LoadDescDefWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
			{
				lpCONNECTIONINFO	lpCI= (lpCONNECTIONINFO)lParam;
				lpENVIRONMENTINFO lpEI=NULL;
				SWORD					cConns,
										iConn;

				CenterDialog(hDlg);

				if (!lpCI)
					return(TRUE);

				lpEI=lpCI->lpActiveEnv;

				cConns=lpEI->cConns;

				//Loop through all connections and list out all
				//of the descriptors
				for (iConn=0;iConn < cConns;iConn++)
				{
					UpdateDescComboBox(hDlg,IDC_LDD_HDESC,lpCI->cDescs,lpCI->lpDesc);

					//Next connection
					lpCI=lpCI->next;
				}

				SETWINDOWLONG(hDlg,lpCI);
			}
			return TRUE;

		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam))
				{
				case IDOK:
					{
					UINT 						cbBytes=0;
					lpCONNECTIONINFO		lpCI= (lpCONNECTIONINFO)GETWINDOWLONG(hDlg);
					TCHAR						szHDesc[LARGEBUFF]=TEXT("");
					lpDESCRIPTORHEADER	lpDesc=NULL;
					lpENVIRONMENTINFO		lpEI=NULL;
					SWORD						iDesc,
												iConn,
												cConns;
					BOOL						fFound=FALSE;
					SQLHDESC					hdesc=NULL;
					LPTSTR					pChr=NULL;


					if (!lpCI)
						return(TRUE);

					lpEI=lpCI->lpActiveEnv;
					cConns=lpEI->cConns;

					// Retrieve values
					cbBytes=GetDlgItemText(hDlg, IDC_LDD_HDESC, szHDesc,
						tsizeof(szHDesc));

					//remove the description
					if (pChr=_tcschr(szHDesc,TEXT('(')))
					{
						*pChr=TEXT('\0');

						//convert string to handle
						TextToHandle(&szHDesc[2],&hdesc);

					}

					//find descriptor record associated with szHdesc
					for (iConn=0;iConn < cConns;iConn++)
					{
						lpDesc=lpCI->lpDesc;

						for (iDesc=0;iDesc < lpCI->cDescs;iDesc++)
						{
							if (lpDesc->hdesc == hdesc)
							{
								fFound=TRUE;
								break;
							}

							lpDesc=lpDesc->next;
						}

						if (fFound)
							break;

						//Next connection
						lpCI=lpCI->next;
					}

					if (fFound)
						FillDescriptorInfo(lpCI,lpDesc);

					EndDialog(hDlg, IDOK);
					}
					return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}

			return FALSE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}

	return FALSE;

} //LoadDescDefWndProc()



//*---------------------------------------------------------------------------------
//| LoadDescriptorDefaults:
//|	This function displays the IDD_LOAD_DESCRIPTOR_DEFAULTS dialog
//|	box which allows the user to load the defaults of the selected
//|	hdesc.
//| Parms:
//|	lpCI		active connection info record
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN LoadDescriptorDefaults(lpCONNECTIONINFO lpCI)
{
	// First get the connection info, if they cancel, nothing to do
	if(IDOK != DialogBoxParam(hInst,
					MAKEINTRESOURCE(IDD_LOAD_DESCRIPTOR_DEFAULTS),
					hwndClient,
					LoadDescDefWndProc,
					(LPARAM)lpCI))
		return;

} //LoadDescriptorDefaults()


//*---------------------------------------------------------------------------------
//| FormatDescHandle:
//|	This function displays the IDD_LOAD_DESCRIPTOR_DEFAULTS dialog
//|	box which allows the user to load the defaults of the selected
//|	hdesc.
//| Parms:
//|	lpCI		active connection info record
//|	lpDesc	Descriptor Header record
//|	szHndlStr	formatted descriptor handle
//| Returns:
//|	szHndlStr	formatted descriptor handle
//*---------------------------------------------------------------------------------

LPTSTR INTFUN FormatDescHandle(lpCONNECTIONINFO lpCI,lpDESCRIPTORHEADER lpDesc,LPTSTR szHndlStr)
{
	TCHAR		szBuff[XLARGEBUFF]=TEXT(""),
				szStmtNum[MEDBUFF]=TEXT("");

	if (lpDesc->lpSI)
		wsprintf(szBuff,TEXT("hdbc %u, hstmt %u"),lpCI->uConNum,
					lpDesc->lpSI->uStmtNum);
	else
		wsprintf(szBuff,TEXT("hdbc %u"),lpCI->uConNum);

	wsprintf(szHndlStr,TEXT("Descriptor: %s (%s)\"%s\""),lpDesc->szHdesc,szBuff,lpDesc->szName);

	return(szHndlStr);

} //FormatDescHandle()

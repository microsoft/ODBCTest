//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHATTR.C
//|
//| Purpose:
//|		This module contains the function handlers for the Attr menu.
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
#define PREFIXES						PRM_CONN_PREFIX | PRM_STMT_PREFIX
#define NUM_CONNECT_OPTIONS		12



//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR				szOutBuff[MAXOUTBUFF];
extern HINSTANCE 			hInst;								// For this app
extern HWND					hwndClient;							// For MDI children
extern HWND					hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS		lpUsrOptions;						// Global user options
extern HMENU				hMenu;								// Menu handle for Trace item
extern LPTSTR				szTestHelpFile;
extern int					iApiHelpCnt, iTestHelpCnt;		// How many times have we called these?


extern lpDFTARRAY	lpdftTxnIsol;
extern UINT			ciTxnIsol;
extern lpDFTARRAY	lpdftBindType;
extern UINT			ciBindType;
extern lpDFTARRAY	lpdftConcurrency;
extern UINT			ciConcurrency;
extern lpDFTARRAY	lpdftCursorType;
extern UINT			ciCursorType;
extern lpDFTARRAY	lpdftUseBookMarks;
extern UINT			ciUseBookMarks;
extern lpDFTARRAY	lpdftBindType;
extern UINT			ciBindType;
extern lpDFTARRAY	lpdftScrollConcur;
extern UINT			ciScrollConcur;
extern lpDFTARRAY	lpdftScrollOpts;
extern UINT			ciScrollOpts;
extern lpDFTARRAY	lpdftfSetConAtr;
extern UINT			cifSetConAtr;
extern lpDFTARRAY	lpdftfSetStmtAtr;
extern UINT			cifSetStmtAtr;
extern lpDFTARRAY	lpdftCTypesNoD;
extern UINT			ciCTypesNoD;
dCSEG(TCHAR) szCURSORATRHELP[]		= TEXT("Cursor Attributes");

#define INVALID_NUMBER	-1

//*---------------------------------------------------------------------------------
//| UpdateGetStmtDefaults:
//|	Updates
//| Parms:
//|	fAttribute	SQLGetStmtAttr/Option's fAttribute
//|	lpParms				Pointer to the paramter array to use
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN UpdateGetStmtDefaults(SQLINTEGER fAttribute,lpPARAMITEM *lpParms,UWORD funcid)
{
	lpPARAMITEM *lpParmTmp=NULL;
	UINT			cParmTmp=0;
	UINT			dex=0,
					dex2=0;
	BOOL			fFoundDex=FALSE;

	if ((fAttribute != SQL_GET_BOOKMARK) && (fAttribute != SQL_ROW_NUMBER))
	{
		// If vParam is NULL, we don't propagate anything
		if(!lpParms[2]->fNull && lpParms[1]->cBytes)
		{

			// Get a pointer to the SetStmtOption parm array
			lpParmTmp = GetParmInfo(GetSqlFuncOffset(funcid), &cParmTmp);

			if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
			{
				FREE_PTR(lpParmTmp[1]->lpData);

				lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
			}

			memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
			lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
			lpParmTmp[1]->dwDex = lpParms[1]->dwDex;
			lpParmTmp[2]->fCType = lpParms[2]->fCType;

			// If fOption contains a default array, find the correct index to set vParam
			for(dex=0;  dex<(lpParms[1]->cDfts);  dex++)
			{
				if(fFoundDex)
					break;

				// Find the correct fOption in the default array
				if (((SDWORD)*(UWORD *)lpParms[1]->lpData) == lpParms[1]->lpDfts[dex].fOpt)
				{
					for(dex2=0;  dex2<(lpParms[1]->lpDfts[dex].cLinks);  dex2++)
					{
						// Find the correct fOption in the default array
						if(((SDWORD)*(UWORD *)lpParms[2]->lpData) == lpParms[1]->lpDfts[dex].lpLink[dex2].fOpt){
							lpParmTmp[2]->dwDex = (SWORD)dex2;
							fFoundDex = TRUE;
							break;
						}
					}
				}
			}

			if(!fFoundDex)
				lpParmTmp[2]->dwDex = -1;

			if (lpParms[2]->cBytes)
			{
				if(lpParmTmp[2]->cBytes != lpParms[2]->cBytes || !lpParmTmp[2]->lpData)
				{
					FREE_PTR(lpParmTmp[2]->lpData);

					lpParmTmp[2]->lpData = AllocateMemory(lpParms[2]->cBytes);
				}

				memcpy(lpParmTmp[2]->lpData, lpParms[2]->lpData, (size_t)lpParms[2]->cBytes);
				lpParmTmp[2]->cBytes = lpParms[2]->cBytes;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;
			}

		} //if(!lpParms[2]->fNull)

	}

} //UpdateGetStmtDefaults()


//*---------------------------------------------------------------------------------
//| IsAttrPointerField:
//| Parms:
//|	fOption			fOption parm of SQLSetStmtAttr
//| Returns:
//|	TRUE if fOption is Desc ptr otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN IsAttrPointerField(SQLINTEGER fOption)
{
	switch(fOption)
	{
		case SQL_ATTR_ROW_OPERATION_PTR:		//ARD
		case SQL_ATTR_ROW_STATUS_PTR:			//IRD
		case SQL_ATTR_PARAM_OPERATION_PTR:	//APD
		case SQL_ATTR_PARAM_STATUS_PTR:		//IPD
		case SQL_ATTR_ROW_BIND_OFFSET_PTR:	//ARD
		case SQL_ATTR_PARAM_BIND_OFFSET_PTR: //APD
		case SQL_ATTR_ROWS_FETCHED_PTR:		//IRD
		case SQL_ATTR_PARAMS_PROCESSED_PTR:	//IPD
		case SQL_ATTR_FETCH_BOOKMARK_PTR:
			return(TRUE);
	}

	return(FALSE);

} //IsAttrPointerField()



//*------------------------------------------------------------------------
//|  GetAttrMemSize:
//|		This function returns the number of bytes required by the attr.
//|	for memory allocation
//|	Returns:
//|		iBytes	number of bytes to allocate
//*------------------------------------------------------------------------

UDWORD INTFUN GetAttrMemSize(lpCONNECTIONINFO lpci,UINT uFieldID)
{
	UDWORD	iBytes=0;
	UDWORD	uRowSet=1;		// Default of one row

	switch (uFieldID)
	{
		case SQL_ATTR_ROW_OPERATION_PTR:
		case SQL_ATTR_ROW_STATUS_PTR:
		case SQL_ATTR_PARAM_OPERATION_PTR:
		case SQL_ATTR_PARAM_STATUS_PTR:
			iBytes=GetArrayStatusSize(lpci);
			break;
		case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
		case SQL_ATTR_ROW_BIND_OFFSET_PTR:
			iBytes=sizeof(UDWORD);
			break;
		case SQL_ATTR_FETCH_BOOKMARK_PTR:	//sets SQL_DESC_FETCH_BOOKMARK_PTR
		case SQL_ATTR_ROWS_FETCHED_PTR:		//sets SQL_DESC_ROWS_PROCESSED_PTR
			iBytes=sizeof(PTR);
			break;
		case SQL_ATTR_PARAMS_PROCESSED_PTR: //sets SQL_DESC_ROWS_PROCESSED_PTR
			iBytes=sizeof(UDWORD);

		//case SQL_ATTR_APP_ROW_DESC:
		//	case SQL_ATTR_APP_PARAM_DESC:
		//	case SQL_ATTR_IMP_ROW_DESC:
		//	case SQL_ATTR_IMP_PARAM_DESC:
		//		break;
	}

	return(iBytes);

} //GetAttrMemSize()


//*---------------------------------------------------------------------------------
//| SetRgbValue:
//| Parms:
//|	lpRgbParm				rgbValue field params
//| Returns:
//|	rgbValue
//*---------------------------------------------------------------------------------

PTR INTFUN SetRgbValue(lpCONNECTIONINFO ci,lpPARAMITEM lpRgbParm,SQLINTEGER fOption)
{
	PTR		rgbValue=NULL;
	UDWORD	iBytes=0;

	ASSERTCHECK(lpRgbParm->lpData,TEXT("lpRgbParm->lpData shouldn't be NULL"));

	if (!lpRgbParm->fNull)
	{
		switch (fOption)
		{
			case SQL_ATTR_ROW_OPERATION_PTR:		//ARD
			//case SQL_ATTR_ROW_STATUS_PTR:			//IRD
			case SQL_ATTR_PARAM_OPERATION_PTR:	//APD
			case SQL_ATTR_PARAM_STATUS_PTR:		//IPD
			case SQL_ATTR_ROW_BIND_OFFSET_PTR:	//ARD
			case SQL_ATTR_PARAM_BIND_OFFSET_PTR: //APD
			case SQL_ATTR_ROWS_FETCHED_PTR:		//IRD
			case SQL_ATTR_PARAMS_PROCESSED_PTR:	//IPD
			case SQL_ATTR_FETCH_BOOKMARK_PTR:
			case SQL_ATTR_ROW_STATUS_PTR:			//IRD
				iBytes = GetAttrMemSize(ci,fOption);

				if (lpRgbParm->lpData && (iBytes > 0))
				{
					*(SQLHANDLE *)lpRgbParm->lpData = rgbValue = AllocateMemory(iBytes);
					if(fOption == SQL_ATTR_FETCH_BOOKMARK_PTR)
						memcpy(rgbValue,lpRgbParm->lpData ,iBytes);
				}

				break;
			case SQL_ATTR_APP_ROW_DESC:
			case SQL_ATTR_APP_PARAM_DESC:
			case SQL_ATTR_IMP_ROW_DESC:
			case SQL_ATTR_IMP_PARAM_DESC:
				if (lpRgbParm->lpData )
					rgbValue = *(SQLHANDLE *)lpRgbParm->lpData;
				break;


			default:
				// fOption requires numeric (SDWORD) data
				if (lpRgbParm->lpData)
				{
					if (IsCharData(lpRgbParm->fCType))
						rgbValue =	lpRgbParm->lpData;
					else
						rgbValue =	(SQLPOINTER)(LONG_PTR)*(SDWORD *)lpRgbParm->lpData;
				}
		}
	}

	return(rgbValue);

} //SetRgbValue()


//*------------------------------------------------------------------------
//|  SetAttrDescField:
//|		This function sets the correct pointer field based on what ATTR
//|	or DESC field is being processed
//|	Returns:
//|		PTR	the pointer set
//*------------------------------------------------------------------------

SDWORD INTFUN SetAttrDescField(UINT uFieldID)
{
	switch (uFieldID)
	{
		//case SQL_ATTR_APP_ROW_DESC:
		case SQL_ATTR_ROW_OPERATION_PTR:		//ARD
			return(SQL_DESC_ARRAY_STATUS_PTR);

		case SQL_ATTR_ROW_STATUS_PTR:			//IRD
			return(SQL_DESC_ARRAY_STATUS_PTR);

		case SQL_ATTR_PARAM_OPERATION_PTR:	//APD
			return(SQL_DESC_ARRAY_STATUS_PTR);

		case SQL_ATTR_PARAM_STATUS_PTR:		//IPD
			return(SQL_DESC_ARRAY_STATUS_PTR);

		case SQL_ATTR_ROW_BIND_OFFSET_PTR:	//ARD
			return(SQL_DESC_BIND_OFFSET_PTR);

		case SQL_ATTR_PARAM_BIND_OFFSET_PTR: //APD
			return(SQL_DESC_BIND_OFFSET_PTR);

		case SQL_ATTR_ROWS_FETCHED_PTR:		//IRD
			return(SQL_DESC_ROWS_PROCESSED_PTR);

		case SQL_ATTR_PARAMS_PROCESSED_PTR:	//IPD
			return(SQL_DESC_ROWS_PROCESSED_PTR);

		case SQL_ATTR_FETCH_BOOKMARK_PTR:
			return(SQL_DESC_ROWS_PROCESSED_PTR);

		case SQL_ATTR_ROW_BIND_TYPE:		//ARD
			return(SQL_DESC_BIND_TYPE);

		case SQL_ATTR_ROW_ARRAY_SIZE:		//ARD
		case SQL_ATTR_PARAMSET_SIZE:		//APD
			return(SQL_DESC_ARRAY_SIZE);
		default:
			return(-1);
	}

} //SetAttrDescField()

//*---------------------------------------------------------------------------------
//| UpdateSetStmtDefaults:
//| Parms:
//|	szFuncName			Function Name
//|	lpParms				Pointer to the paramter array to use
//|	cParms				number of Parameters
//|	ci						Current connection information (always NULL)
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN UpdateSetStmtDefaults(lpPARAMITEM *lpParms,UWORD funcid)
{
	lpPARAMITEM *lpParmTmp=NULL;
	UINT			cParmTmp=0;


	if (lpParms[1]->cBytes <= 0)
		return;

	// Get a pointer to the GetStmtOption parm array
	lpParmTmp = GetParmInfo(GetSqlFuncOffset(funcid), &cParmTmp);

	if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
	{
		FREE_PTR(lpParmTmp[1]->lpData);

		lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
	}

	memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
	lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
	lpParmTmp[1]->dwDex = lpParms[1]->dwDex;
	lpParmTmp[2]->fCType = lpParms[2]->fCType;
	lpParmTmp[3]->dwDex = lpParms[3]->dwDex;

} //UpdateSetStmtDefaults()



//*---------------------------------------------------------------------------------
//| ProcessOptions:
//|	Processes RowSet changes and BookMarks
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN ProcessOptions(SDWORD fOption, UINT_PTR rgbValue,lpCONNECTIONINFO lpCI)
{
	lpSTATEMENTINFO		lpStmt=lpCI->lpCurStmt;

	if (!lpStmt)
		return;

	switch(fOption)
	{
		case SQL_ROWSET_SIZE:
		case SQL_ATTR_ROW_ARRAY_SIZE:
			ChangeRowsetSize(lpCI, lpStmt, (UDWORD)rgbValue, TRUE, fOption);
			break;

		case SQL_CURSOR_TYPE:
			lpStmt->uCursorType = (UDWORD)rgbValue;
			break;

		case SQL_CONCURRENCY:
			lpStmt->uConcurType = (UDWORD)rgbValue;
			break;

		case SQL_USE_BOOKMARKS:
			lpStmt->uBookMarkAttr = (UDWORD)rgbValue;
			break;

		case SQL_ATTR_APP_ROW_DESC:
			//set the current ARD
			lpStmt->lpARD=FindDescriptor((SQLHDESC)rgbValue,lpCI);
			break;

		case SQL_ATTR_APP_PARAM_DESC:
			//set the current APD
				lpStmt->lpAPD=FindDescriptor((SQLHDESC)rgbValue,lpCI);
			break;

	}

	if(lpStmt->lpResults)
		SetMiniBarText(((lpRESULTWININFO)lpStmt->lpResults), NULL);

} //ProcessOptions()





//*---------------------------------------------------------------------------------
//| LogStmtParameters:
//| Parms:
//|	szFuncName			Function Name
//|	lpParms				Pointer to the paramter array to use
//|	cParms				number of Parameters
//|	ci						Current connection information (always NULL)
//| Returns:
//|	fLogged				have the parameters been logged yet?
//*---------------------------------------------------------------------------------

static BOOL INTFUN LogStmtParameters(LPTSTR szFuncName, lpPARAMITEM *lpParms, UINT cParms,
			lpCONNECTIONINFO ci,UDWORD uAttrib)
{
	BOOL		fLogged=FALSE;
	UWORD		uOption =	lpParms[1]->fNull ? 0 : *(UWORD *)lpParms[1]->lpData;
	UDWORD	uAttribute=	lpParms[2]->fNull ? 0 : *(UDWORD *)lpParms[2]->lpData;
	UINT			dex=0,
					dex2=0;
	BOOL		fBindType= ((uOption == SQL_BIND_TYPE) || (uOption == SQL_DESC_BIND_TYPE)),
				fByColumn= uAttribute == SQL_BIND_BY_COLUMN;


	for(dex=0;  dex<(lpParms[1]->cDfts);  dex++)
	{
		// Find the correct fOption in the default array
		if (((SDWORD)*(UWORD *)lpParms[1]->lpData) == lpParms[1]->lpDfts[dex].fOpt)
		{
			// Special case SQL_BIND_TYPE since a vParam of SQL_BIND_BY_COLUMN can be logged
			// using LogParametersSpecial but all other SQL_BIND_TYPES need to be logged
			// using LogParameters
			if (!(fBindType && !fByColumn))
			{
	         // If we get here, we know which element in the default array the fOption is
	         if((lpParms[1]->lpDfts[dex].uValInfo == PRM_32HDL) 		// the fOption rgbInfoValue is a handle
	         	 	|| (lpParms[1]->lpDfts[dex].lpLink)) 					// the fOption rgbInfoValue is from a linked array (bitmask)
	        	{
	         	if(!(uAttrib & FH_NO_LOG))
					{
		         	LogParametersSpecial(szFuncName, lpParms,
												cParms, ci, FALSE,
												lpParms[1]->lpDfts[dex].uValInfo, 1, 2);
						fLogged=TRUE;
					}
				}
			}
		}
	}

	return(fLogged);

} //LogStmtParameters()


//*---------------------------------------------------------------------------------
//| lpSQLSetStmtAttr:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetStmtAttr(STD_FH_PARMS)
{
	RETCODE		rc=SQL_SUCCESS;
	lpPARAMITEM *lpParmTmp=NULL;
	UINT			cParmTmp=0;
	SQLINTEGER	fOption=*(SQLINTEGER *)lpParms[1]->lpData;
	SQLPOINTER	rgbValue=NULL;
	SQLINTEGER	cbValueMax=*(SQLINTEGER *)lpParms[3]->lpData;
	HSTMT			hstmt=NULL;
	lpSTATEMENTINFO lpStmt=NULL;
	SDWORD		sdDescFieldID=0;
	lpDESCRIPTORHEADER lpDesc=NULL;
	PTR			tmpRgb=lpParms[2]->lpData;

	if (ci && ci->lpCurStmt)
		lpStmt=ci->lpCurStmt;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	if (lpParms[2]->lpData)
		rgbValue=SetRgbValue(ci,lpParms[2],fOption);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	if (!lpParms[0]->fNull)
	{
		if ((fOption == SQL_BIND_TYPE) ||
			 (fOption == SQL_ATTR_ROW_BIND_TYPE))
		{
			// Check for TST0010
			if(!ChangeBindType(GETOUTPUTWINDOW(ci), NULL,
							ci->lpCurStmt, (UWORD)(rgbValue)))
				return SQL_ERROR;
		}
	}

	// Invoke function
	rc = SQLSetStmtAttr (hstmt,
								fOption,												// fOption
								lpParms[2]->fNull ? NULL : rgbValue,			// rgbValue
								cbValueMax);											// cbValueMax


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Update the defaults for GetStmtOptions
	if(RC_SUCCESSFUL(rc))
	{
		lpDesc=FindDescriptor(rgbValue,ci);


			switch(fOption)
			{
				case SQL_ATTR_IMP_PARAM_DESC:
					lpStmt->lpIPD=lpDesc;
					if(lpDesc)
						lpDesc->uwDescType=DESC_IPD;
					break;

				case SQL_ATTR_IMP_ROW_DESC:
					lpStmt->lpIRD=lpDesc;
					if(lpDesc)
						lpDesc->uwDescType=DESC_IRD;
					break;

				case SQL_ATTR_APP_PARAM_DESC:
					lpStmt->lpAPD=lpDesc;
					if(lpDesc)
						lpDesc->uwDescType=DESC_APD;
					break;

				case SQL_ATTR_APP_ROW_DESC:
					lpStmt->lpARD=lpDesc;
					if(lpDesc)
						lpDesc->uwDescType=DESC_ARD;
					break;
				case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
				case SQL_ATTR_PARAMSET_SIZE:
					lpDesc=lpStmt->lpAPD;
					break;
				case SQL_ATTR_PARAM_STATUS_PTR:
				case SQL_ATTR_PARAMS_PROCESSED_PTR:
					lpDesc=lpStmt->lpIPD;
				break;
				case SQL_ATTR_ROW_STATUS_PTR:
				 	lpDesc=lpStmt->lpIRD;
					if(!lpDesc)
						lpStmt->rgfRowStatus = rgbValue;
					break;

				case SQL_ATTR_ROWS_FETCHED_PTR:
					lpDesc=lpStmt->lpIRD;
					break;
				case SQL_ATTR_ROW_BIND_OFFSET_PTR:
				case SQL_ATTR_ROW_BIND_TYPE:
				case SQL_ATTR_ROW_ARRAY_SIZE:
					lpDesc=lpStmt->lpARD;
					break;

			}

			if ((sdDescFieldID=SetAttrDescField(fOption)) != -1)
			{
				if (lpDesc)
				{
					SetDescriptorField(ci,lpStmt,lpDesc,sdDescFieldID,
						&rgbValue,cbValueMax,0);

					//link descriptor record with statement info record
					lpDesc->lpSI=lpStmt;
				}

			}



		UpdateSetStmtDefaults(lpParms,SQL_API_SQLGETSTMTATTR);
		// Process any options which affect other functions
		ProcessOptions(fOption, (UINT_PTR)rgbValue, ci);
	}

	lpParms[2]->lpData=tmpRgb;

	return rc;

} //lpSQLSetStmtAttr()

//*---------------------------------------------------------------------------------
//| lpSQLGetStmtAttr:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetStmtAttr(STD_FH_PARMS)
{
	RETCODE				rc=SQL_SUCCESS;
	SQLINTEGER			fAttribute=*(SQLINTEGER *)lpParms[1]->lpData;
	SQLINTEGER			cbValueMax=*(SQLINTEGER *)lpParms[3]->lpData;
	SQLPOINTER			rgbValue=lpParms[2]->fNull ? NULL : lpParms[2]->lpData;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	HSTMT					hstmt=NULL;
	SQLHDESC				hdesc=NULL;
	lpSTATEMENTINFO	lpStmt=NULL;


	if (ci && ci->lpCurStmt)
		lpStmt=ci->lpCurStmt;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	if (!cbValueMax)
		cbValueMax=lpUsrOptions->uDftBuffLen;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 								// rgbValue
						cbValueMax , 	//	  based on cbValue
						lpUsrOptions->fBufferChecking);

	rgbValue=lpParms[2]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetStmtAttr (hstmt,
								fAttribute,																	// fAttribute
								rgbValue,
								cbValueMax,																	// cbValue
								lpParms[4]->fNull ? NULL: lpParms[4]->lpData);					// pcbValue

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	if (RC_SUCCESSFUL(rc) && rgbValue)
	{
		//Get the Implicitly allocated descriptors and let the user
		//name them
		if ((fAttribute == SQL_ATTR_IMP_PARAM_DESC) ||
			 (fAttribute == SQL_ATTR_IMP_ROW_DESC) ||
			 (fAttribute == SQL_ATTR_APP_PARAM_DESC) ||
			 (fAttribute == SQL_ATTR_APP_ROW_DESC))
		{
			hdesc=*(SQLHDESC *)rgbValue;

			lpDesc=DoAllocDescriptor(ci,hdesc,TRUE);

			switch(fAttribute)
			{
				case SQL_ATTR_IMP_PARAM_DESC:
					lpStmt->lpIPD=lpDesc;
					break;

				case SQL_ATTR_IMP_ROW_DESC:
					lpStmt->lpIRD=lpDesc;
					break;

				case SQL_ATTR_APP_PARAM_DESC:
					lpStmt->lpAPD=lpDesc;
					break;

				case SQL_ATTR_APP_ROW_DESC:
					lpStmt->lpARD=lpDesc;
					break;
			}
		}

	}

	// Log output parameters
	// Check to see if the value returned was a handle,bitmask, or linked array --
	// If it is, then we need to format the output for those special cases
	if (!LogStmtParameters(szFuncName,lpParms,cParms,ci,uAttrib))
		LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Update the defaults for SetStmtOptions
	if(RC_SUCCESSFUL(rc))
		UpdateGetStmtDefaults(fAttribute,lpParms,SQL_API_SQLGETSTMTATTR);


	return rc;

} //lpSQLGetStmtAttr()


//*---------------------------------------------------------------------------------
//| lpSQLSetStmtOption:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetStmtOption(STD_FH_PARMS)
{
	RETCODE		rc;
	UINT_PTR		vParamTemp;
	BOOL			fNullHstmt=lpParms[0]->fNull;
	UWORD			fOption=*(UWORD *)lpParms[1]->lpData;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Set vParam with the correct UDWORD (data or pointer) depending on the data type required by fOption
	if (IsCharData(lpParms[2]->fCType))
		vParamTemp = (UINT_PTR)lpParms[2]->lpData; 			// fOption requires character data
	else
		vParamTemp = *(UDWORD *)lpParms[2]->lpData;  // fOption requires numeric (UDWORD) data

	// Before calling, check for error conditions which preclude us from continuing
	if(!fNullHstmt && fOption == SQL_BIND_TYPE)
	{
		// Check for TST0010
		if(!ChangeBindType(GETOUTPUTWINDOW(ci), NULL,
						ci->lpCurStmt, (UDWORD)vParamTemp))
			return SQL_ERROR;
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLSetStmtOption (hstmt, fOption,vParamTemp);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Update the defaults for GetStmtOptions
	if(RC_SUCCESSFUL(rc))
		UpdateSetStmtDefaults(lpParms,SQL_API_SQLSETSTMTOPTION);


	// Process any options which affect other functions
	if(RC_SUCCESSFUL(rc))
		ProcessOptions(fOption, vParamTemp,ci);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLGetStmtOption:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetStmtOption(STD_FH_PARMS)
{
	RETCODE		rc;
	UWORD			fOption = *(UWORD *)lpParms[1]->lpData;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 								// pvParam
						lpUsrOptions->uDftBuffLen ,				//	Max size for our output buffer
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetStmtOption (hstmt,fOption,
						 (PTR)lpParms[2]->lpData);		// vParam

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	// Check to see if the value returned was a handle,bitmask, or linked array --
	// If it is, then we need to format the output for those special cases
	if (!LogStmtParameters(szFuncName,lpParms,cParms,ci,uAttrib))
		LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	// Update the defaults for SetStmtOptions
	if(RC_SUCCESSFUL(rc))
		UpdateGetStmtDefaults(fOption,lpParms,SQL_API_SQLSETSTMTOPTION);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLSetConnectOption:
//|	Get information for both driver specific items and non-driver items.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
static VOID INTFUN LogConnectParameters(PTSTR szFuncName, lpPARAMITEM *lpParms, UINT cParms,
		lpCONNECTIONINFO ci,UINT iParfOption,UINT iParpvParam,UDWORD uAttrib)
{
	UINT			dex;
	BOOL			fLogged=FALSE;
	UDWORD		uValInfoTmp;

	// Check to see if the value returned was a handle,bitmask, or linked array --
	// If it is, then we need to format the output for those special cases
	for(dex=0;  dex < (lpParms[iParfOption]->cDfts);  dex++)
	{
		// Find the correct fOption in the default array
		if(((SDWORD)*(UWORD *)lpParms[iParfOption]->lpData) == lpParms[iParfOption]->lpDfts[dex].fOpt)
		{
			// If we get here, we know which element in the default array the fOption is
			// Clear the prefix bits in uValInfo
			uValInfoTmp = lpParms[iParfOption]->lpDfts[dex].uValInfo;		// Get the uValInfo of the selected fOption
	   	uValInfoTmp &= ~PREFIXES;                          // Clear bits for the Conn: & Stmt: prefixes

			if (	uValInfoTmp != PRM_STR &&
				 ((uValInfoTmp == PRM_32HDL) 				// the fOption rgbInfoValue is a handle
         	|| (lpParms[iParfOption]->lpDfts[dex].lpLink))	)// the fOption rgbInfoValue is from a linked array (bitmask)

			{
         	if(!(uAttrib & FH_NO_LOG))
				{
	         	LogParametersSpecial(szFuncName, lpParms,
											cParms, ci, FALSE,
											uValInfoTmp, iParfOption, iParpvParam);
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

} //LogConnectParameters()


//*---------------------------------------------------------------------------------
//| lpSQLSetConnectOption:
//|	Get information for both driver specific items and non-driver items.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetConnectOption(STD_FH_PARMS)
{
	RETCODE		rc;
	SQLULEN     vParamTemp;
	UDWORD		uValInfoTmp;
	lpPARAMITEM *lpParmTmp;
	UINT			cParmTmp;
	UWORD			fOption=*(UWORD *)lpParms[1]->lpData;
	lpSTATEMENTINFO lpStmt=NULL;
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	if (ci)
		lpStmt=ci->lpCurStmt;

	// Set vParam with the correct UDWORD (data or pointer) depending on the data type required by fOption
	if (fOption == SQL_QUIET_MODE)
	{
		lpParms[2]->fHandle=PT_HHANDLE;

		if(lpParms[2]->fNull)
			vParamTemp = (SQLULEN)NULL;												// Same as NULL, but 32-bit needs a UDWORD (NULL won't compile as 0)
		else if (ci)
			vParamTemp = (SQLULEN)ci->hwndOut;
		else
			vParamTemp = (SQLULEN)hwndOutWin;
	}
	else if (IsCharData(lpParms[2]->fCType))
	{
		vParamTemp = (SQLULEN)lpParms[2]->lpData; 			// fOption requires character data
	}
	else
	{
		if (lpParms[2]->lpData)
			vParamTemp = *(UDWORD *)lpParms[2]->lpData;  // fOption requires numeric (UDWORD) data
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Before calling, make sure there are no errors which preclude us
	if(!hdbc && fOption == SQL_BIND_TYPE)
	{
		if(!ChangeBindType(GETOUTPUTWINDOW(ci), ci,NULL, (UDWORD)vParamTemp))
			return SQL_ERROR;
	}

	// Invoke function
	rc = SQLSetConnectOption (hdbc,fOption,vParamTemp);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	// Set default values for GetConnectOptions and/or Get/SetStatementOptions
   // First check for a user-defined value
	if(RC_SUCCESSFUL(rc))
	{
		if(lpParms[1]->dwDex == -1)			// If it is, treat it like a Conn: prefix
			uValInfoTmp = PRM_CONN_PREFIX;
		else
		{											// If it is a known value, parse uValInfo for the prefixes
		   uValInfoTmp = lpParms[1]->lpDfts[lpParms[1]->dwDex].uValInfo;	// Get the uValInfo of the selected fOption
	   	uValInfoTmp &= PREFIXES;                                       // Clear all bits except the Conn: & Stmt: prefixes
	   }

	   switch(uValInfoTmp)
		{
	      case PRM_CONN_PREFIX:
				// Update the defaults for GetConnectOptions
				lpParmTmp = GetParmInfo(GetSqlFuncOffset(SQL_API_SQLGETCONNECTOPTION), &cParmTmp);	// Get a pointer to the GetConnectOption parm array
				if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
				{
					FREE_PTR(lpParmTmp[1]->lpData);
					lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
				}

				memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
				lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
				lpParmTmp[1]->dwDex = lpParms[1]->dwDex;
				lpParmTmp[2]->fCType = lpParms[2]->fCType;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;
	      	break;
	      case PRM_STMT_PREFIX:
	      	// Update the defaults for Get & SetStmtOptions
				lpParmTmp = GetParmInfo(GetSqlFuncOffset(SQL_API_SQLSETSTMTOPTION), &cParmTmp);	// Get a pointer to the SetStmtOption parm array
				if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
				{
					FREE_PTR(lpParmTmp[1]->lpData);
					lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
				}

				memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
				lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
				lpParmTmp[1]->dwDex = (lpParms[1]->dwDex - NUM_CONNECT_OPTIONS);
				lpParmTmp[2]->fCType = lpParms[2]->fCType;
				lpParmTmp[2]->dwDex = lpParms[2]->dwDex;
				if(lpParmTmp[2]->cBytes != lpParms[2]->cBytes || !lpParmTmp[2]->lpData)
				{
					FREE_PTR(lpParmTmp[2]->lpData);
					lpParmTmp[2]->lpData = AllocateMemory(lpParms[2]->cBytes);
				}

				memcpy(lpParmTmp[2]->lpData, lpParms[2]->lpData, (size_t)lpParms[2]->cBytes);
				lpParmTmp[2]->cBytes = lpParms[2]->cBytes;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;

				lpParmTmp = GetParmInfo(GetSqlFuncOffset(SQL_API_SQLGETSTMTOPTION), &cParmTmp);	// Get a pointer to the GetStmtOption parm array
				if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
				{
					FREE_PTR(lpParmTmp[1]->lpData);
					lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
				}

				memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
				lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
				lpParmTmp[1]->dwDex = (lpParms[1]->dwDex - NUM_CONNECT_OPTIONS);
				lpParmTmp[2]->fCType = lpParms[2]->fCType;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;
	      	break;
	   	}

		// Handle specific options which affect other parts of Gator
		switch(fOption)
		{
			// Tracing affects our Trace Menu item
			case SQL_OPT_TRACE:
				lpUsrOptions->fDMTrace = (BOOL)vParamTemp;
				CheckMenuItem(hMenu, IDM_TRACE,
							(lpUsrOptions->fDMTrace) ? MF_CHECKED : MF_UNCHECKED);
				RefreshToolbar();
				break;

			// Need user's file name for Trace dialog
			case SQL_OPT_TRACEFILE:
				lstrcpy(lpUsrOptions->szLog, (LPTSTR)vParamTemp);
				break;

			// Set bookmark status on all statements
			case SQL_USE_BOOKMARKS:
				if (lpStmt)
					lpStmt->uBookMarkAttr = (UDWORD)vParamTemp;
				break;

			// Change to rowset means we need to rebind bound memory
			case SQL_ROWSET_SIZE:
				{
				if(ci->lpCurStmt)
				{
					int					dex=0, rdex=0;
					UDWORD				cbOldConnRowSet=ci->uRowSetSize;
					lpSTATEMENTINFO	lpStmt;
					UDWORD *		lprgbcbOldRowSet;			// Need array item for each stmt

					// Allocate an array to store old rowset sizes
					lprgbcbOldRowSet = (UDWORD *)AllocateMemory(sizeof(UDWORD) * ci->cStmts);
					if(!lprgbcbOldRowSet)
						goto resetrowset;							// No memory, must reset

					// Save old rowset size
					ci->uRowSetSize = (UDWORD)vParamTemp;

					// Loop through each statement and reset their parms
					lpStmt = ci->lpCurStmt;

					for(dex=0;  dex<ci->cStmts;  dex++)
					{
						// Store old rowset size for this stmt
						lprgbcbOldRowSet[dex] = lpStmt->uRowSetSize;
						lpStmt->uRowSetSize = (UDWORD)vParamTemp;

						// Try to reset all bound columns, if not successful,
						//		warn user and reset rowset size
						if(!ResetStmtBVMem(lpStmt, GETOUTPUTWINDOW(ci), lprgbcbOldRowSet[dex]))
							goto resetrowset;

						lpStmt = lpStmt->next;
					}


					// It add worked, so commit any decreases required
					CommitBVChanges(lpStmt);

               FREE_PTR(lprgbcbOldRowSet);


					break;

resetrowset:   //
					//  Reset each individual HSTMT on this HDBC to its old size
					//
					for(rdex=0;  rdex<dex+1;  rdex++)
					{
						szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
								GetGtrString(szOutBuff, MAXOUTBUFF, idsErrChgRowsetSize),
								lpStmt->uStmtNum, vParamTemp, lprgbcbOldRowSet[dex]);

						rc = SQLSetStmtOption(lpStmt->hstmt,
	               				SQL_ROWSET_SIZE,
	               				lprgbcbOldRowSet[dex]);

	               lpStmt->uRowSetSize = lprgbcbOldRowSet[dex];
	               lpStmt = lpStmt->prev;
					}


					//
					// Now warn about the overall connection handle and reset it
					//
					szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsErrChgConnRowsetSize),
							ci->uConNum, vParamTemp, cbOldConnRowSet);

					ci->uRowSetSize = cbOldConnRowSet;
					rc = SQLSetConnectOption(ci->hdbc,
										SQL_ROWSET_SIZE,
										cbOldConnRowSet);

					FREE_PTR(lprgbcbOldRowSet);
					}
				}
				break;
			}
		}

	return rc;

} //lpSQLSetConnectOption()

//*---------------------------------------------------------------------------------
//| lpSQLSetConnectAttr:
//|	This replaces lpSQLSetConnectOption.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetConnectAttr(STD_FH_PARMS)
{
#define	PREFIXES		PRM_CONN_PREFIX | PRM_STMT_PREFIX
#define	NUM_CONNECT_OPTIONS		12

	RETCODE		rc;
	UDWORD		uValInfoTmp;
	lpPARAMITEM *lpParmTmp;
	UINT			cParmTmp;
	BOOL			fNullHdbc=lpParms[0]->fNull;
	SQLINTEGER	fAttribute=*(SQLINTEGER *)lpParms[1]->lpData;
	SQLPOINTER	rgbValue=NULL;
	SQLINTEGER	cbValue=*(SQLINTEGER *)lpParms[3]->lpData;
	lpSTATEMENTINFO lpStmt=NULL;
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	if (ci)
		lpStmt=ci->lpStmt;

	// Set vParam with the correct UDWORD (data or pointer) depending on the data type required by fOption
	if(fAttribute == SQL_ATTR_QUIET_MODE && !lpParms[2]->fNull)
	{
		rgbValue=lpParms[2]->fHandle ? GETOUTPUTWINDOW(ci):NULL;
		lpParms[2]->lpData=&rgbValue;
	}
	else if (lpParms[2]->lpData)
		rgbValue=SetRgbValue(ci,lpParms[2],fAttribute);


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Before calling, make sure there are no errors which preclude us
	if(!fNullHdbc && fAttribute == SQL_BIND_TYPE)
	{
		if(!ChangeBindType(GETOUTPUTWINDOW(ci), ci,
						NULL, (UDWORD)(UINT_PTR)rgbValue))
			return SQL_ERROR;
	}

	// Invoke function
	rc=SQLSetConnectAttr(hdbc,
							fAttribute,
							lpParms[2]->fNull ? NULL : rgbValue,
							cbValue);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	// Set default values for GetConnectOptions and/or Get/SetStatementOptions
   // First check for a user-defined value
	if(RC_SUCCESSFUL(rc))
	{
		if(lpParms[1]->dwDex == -1)			// If it is, treat it like a Conn: prefix
			uValInfoTmp = PRM_CONN_PREFIX;
		else
		{
			// If it is a known value, parse uValInfo for the prefixes
		   uValInfoTmp = lpParms[1]->lpDfts[lpParms[1]->dwDex].uValInfo;	// Get the uValInfo of the selected fOption
	   	uValInfoTmp &= PREFIXES;                                       // Clear all bits except the Conn: & Stmt: prefixes
	   }

	   switch(uValInfoTmp)
		{
	      case PRM_CONN_PREFIX:
				// Update the defaults for GetConnectOptions
				lpParmTmp = GetParmInfo(GetSqlFuncOffset(SQL_API_SQLGETCONNECTATTR), &cParmTmp);	// Get a pointer to the GetConnectOption parm array
				if (!lpParmTmp)
					break;

				if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
				{
					FREE_PTR(lpParmTmp[1]->lpData);
					lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
				}

				memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
				lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
				lpParmTmp[1]->dwDex = lpParms[1]->dwDex;
				lpParmTmp[2]->fCType = lpParms[2]->fCType;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;
	      	break;
	      case PRM_STMT_PREFIX:
	      	// Update the defaults for Get & SetStmtOptions
				lpParmTmp = GetParmInfo(GetSqlFuncOffset(SQL_API_SQLSETSTMTATTR), &cParmTmp);	// Get a pointer to the SetStmtOption parm array
				if (!lpParmTmp)
					break;

				if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
				{
					FREE_PTR(lpParmTmp[1]->lpData);

					lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
				}

				memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
				lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
				lpParmTmp[1]->dwDex = (lpParms[1]->dwDex - NUM_CONNECT_OPTIONS);
				lpParmTmp[2]->fCType = lpParms[2]->fCType;
				lpParmTmp[2]->dwDex = lpParms[2]->dwDex;

				if(lpParmTmp[2]->cBytes != lpParms[2]->cBytes || !lpParmTmp[2]->lpData)
				{
					FREE_PTR(lpParmTmp[2]->lpData);

					lpParmTmp[2]->lpData = AllocateMemory(lpParms[2]->cBytes);
				}

				memcpy(lpParmTmp[2]->lpData, lpParms[2]->lpData, (size_t)lpParms[2]->cBytes);
				lpParmTmp[2]->cBytes = lpParms[2]->cBytes;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;

				lpParmTmp = GetParmInfo(GetSqlFuncOffset(SQL_API_SQLGETSTMTATTR), &cParmTmp);	// Get a pointer to the GetStmtOption parm array

				if (!lpParmTmp)
					break;

				if(lpParmTmp[1]->cBytes != lpParms[1]->cBytes || !lpParmTmp[1]->lpData)
				{
					FREE_PTR(lpParmTmp[1]->lpData);

					lpParmTmp[1]->lpData = AllocateMemory(lpParms[1]->cBytes);
				}

				memcpy(lpParmTmp[1]->lpData, lpParms[1]->lpData, (size_t)lpParms[1]->cBytes);
				lpParmTmp[1]->cBytes = lpParms[1]->cBytes;
				lpParmTmp[1]->dwDex = (lpParms[1]->dwDex - NUM_CONNECT_OPTIONS);
				lpParmTmp[2]->fCType = lpParms[2]->fCType;
				lpParmTmp[3]->dwDex = lpParms[3]->dwDex;
	      	break;
	   	}

		// Handle specific options which affect other parts of Gator
		switch(fAttribute)
		{
			// Tracing affects our Trace Menu item
			case SQL_OPT_TRACE:
				lpUsrOptions->fDMTrace = rgbValue != NULL;
				CheckMenuItem(hMenu, IDM_TRACE,
							(lpUsrOptions->fDMTrace) ? MF_CHECKED : MF_UNCHECKED);
				RefreshToolbar();
				break;

			// Need user's file name for Trace dialog
			case SQL_OPT_TRACEFILE:
				lstrcpy(lpUsrOptions->szLog, (LPTSTR)rgbValue);
				break;

			// Set bookmark status on all statements
			case SQL_USE_BOOKMARKS:
				if (lpStmt)
					lpStmt->uBookMarkAttr = (UDWORD)(UINT_PTR)rgbValue;
				break;
			// Change to rowset means we need to rebind bound memory
			case SQL_ROWSET_SIZE:
				{
				if(ci->lpCurStmt)
				{
					int					dex=0, rdex=0;
					UDWORD				cbOldConnRowSet=ci->uRowSetSize;
					lpSTATEMENTINFO	lpStmt;
					UDWORD *		lprgbcbOldRowSet;			// Need array item for each stmt

					// Allocate an array to store old rowset sizes
					lprgbcbOldRowSet = (UDWORD *)AllocateMemory(sizeof(UDWORD) * ci->cStmts);
					if(!lprgbcbOldRowSet)
						goto resetrowset;							// No memory, must reset

					// Save old rowset size
					ci->uRowSetSize = (UDWORD)(UINT_PTR)rgbValue;

					// Loop through each statement and reset their parms
					lpStmt = ci->lpCurStmt;

					for(dex=0;  dex<ci->cStmts;  dex++) {
						// Store old rowset size for this stmt
						lprgbcbOldRowSet[dex] = lpStmt->uRowSetSize;
						lpStmt->uRowSetSize = (UDWORD)(UINT_PTR)rgbValue;

						// Try to reset all bound columns, if not successful,
						//		warn user and reset rowset size
						if(!ResetStmtBVMem(lpStmt, GETOUTPUTWINDOW(ci), lprgbcbOldRowSet[dex]))
							goto resetrowset;

						lpStmt = lpStmt->next;
						}


					// It add worked, so commit any decreases required
					CommitBVChanges(lpStmt);

               FREE_PTR(lprgbcbOldRowSet);


					break;

resetrowset:   //
					//  Reset each individual HSTMT on this HDBC to its old size
					//
					for(rdex=0;  rdex<dex+1;  rdex++) {
						szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
								GetGtrString(szOutBuff, MAXOUTBUFF, idsErrChgRowsetSize),
								lpStmt->uStmtNum, (UDWORD)(UINT_PTR)rgbValue, lprgbcbOldRowSet[dex]);
	               rc = SQLSetStmtOption(lpStmt->hstmt,
	               				SQL_ROWSET_SIZE,
	               				lprgbcbOldRowSet[dex]);

	               lpStmt->uRowSetSize = lprgbcbOldRowSet[dex];
	               lpStmt = lpStmt->prev;
	               }


					//
					// Now warn about the overall connection handle and reset it
					//
					szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsErrChgConnRowsetSize),
							ci->uConNum, (UDWORD)(UINT_PTR)rgbValue, cbOldConnRowSet);

					ci->uRowSetSize = cbOldConnRowSet;
					rc = SQLSetConnectOption(ci->hdbc,
										SQL_ROWSET_SIZE,
										cbOldConnRowSet);

					FREE_PTR(lprgbcbOldRowSet);
				}
			}
			break;
		}
	} else {
        if (lpParms[1]->dwDex == -1) {
            lpParms[1]->dwDex = 0;
        }
    }

	if (fAttribute==SQL_ATTR_QUIET_MODE)
	{
		//reset lpData(rgbValue) so that gator won't try to
		//free a static addr.
		lpParms[2]->lpData=NULL;
	}

	return rc;

} //lpSQLSetConnectAttr()


//*---------------------------------------------------------------------------------
//| UpdateDefaults:
//|	This function Updates the SetConnectOption/Attr controls based on what the
//|	user just did in SQLGetConnectOption/Attr
//| Parms:
//|	uAPI						Either SQL_API_SQLSETCONNECTOPTION or SQL_API_SQLSETCONNECTATTR
//|	lpParms					Pointer to the paramter array to use for the request
//|	iParfOption				Parameter number for fOption/fAttribute control
//|	iParpvParam				Parameter number for pvParam/rgbValue control
//|	iParParameterType		Parameter number for ParameterType control
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN UpdateDefaults(UWORD uAPI,lpPARAMITEM *lpParms,UINT iParfOption,
			UINT iParpvParam,UINT iParParameterType)
{
	lpPARAMITEM *lpParmTmp=NULL;
	UINT			cParmTmp;
	BOOL			fFoundDex=FALSE;
	UINT			dex,dex2;

	if(!lpParms[iParpvParam]->fNull)
	{

		lpParmTmp = GetParmInfo(GetSqlFuncOffset(uAPI), &cParmTmp);	// Get a pointer to the SetConnectOption parm array

		if (!lpParmTmp)
			return;

		if(lpParmTmp[iParfOption]->cBytes != lpParms[iParfOption]->cBytes || !lpParmTmp[iParfOption]->lpData)
		{
			FREE_PTR(lpParmTmp[iParfOption]->lpData);

			lpParmTmp[iParfOption]->lpData = AllocateMemory(lpParms[iParfOption]->cBytes);
		}

		memcpy(lpParmTmp[iParfOption]->lpData, lpParms[iParfOption]->lpData, (size_t)lpParms[iParfOption]->cBytes);
		lpParmTmp[iParfOption]->cBytes = lpParms[iParfOption]->cBytes;
		lpParmTmp[iParfOption]->dwDex = lpParms[iParfOption]->dwDex;
		lpParmTmp[iParpvParam]->fCType = lpParms[iParpvParam]->fCType;

		// If fOption contains a default array, find the correct index to set vParam
		for(dex=0;  dex<(lpParms[iParfOption]->cDfts);  dex++)
		{
			if(fFoundDex)
				break;

			// Find the correct fOption in the default array
			if(((SDWORD)*(UWORD *)lpParms[iParfOption]->lpData) == lpParms[iParfOption]->lpDfts[dex].fOpt)
			{
				for(dex2=0;  dex2<(lpParms[iParfOption]->lpDfts[dex].cLinks);  dex2++)
				{
					// Find the correct fOption in the default array
					if(((SDWORD)*(UWORD *)lpParms[iParpvParam]->lpData) == lpParms[iParfOption]->lpDfts[dex].lpLink[dex2].fOpt)
					{
						lpParmTmp[iParpvParam]->dwDex = (SWORD)dex2;
						fFoundDex = TRUE;
						break;
					}
				}
			}
		}

		if(!fFoundDex)
			lpParmTmp[iParpvParam]->dwDex = -1;

		if(lpParmTmp[iParpvParam]->cBytes != lpParms[iParpvParam]->cBytes || !lpParmTmp[iParpvParam]->lpData)
		{
			FREE_PTR(lpParmTmp[iParpvParam]->lpData);

			lpParmTmp[iParpvParam]->lpData = AllocateMemory(lpParms[iParpvParam]->cBytes);
		}

		memcpy(lpParmTmp[iParpvParam]->lpData, lpParms[iParpvParam]->lpData, (size_t)lpParms[iParpvParam]->cBytes);
		lpParmTmp[iParpvParam]->cBytes = lpParms[iParpvParam]->cBytes;
		lpParmTmp[iParParameterType]->dwDex = lpParms[iParParameterType]->dwDex;
	}

} //UpdateDefaults()



//*---------------------------------------------------------------------------------
//| lpSQLGetConnectOption:
//|	Get information for both driver specific items and non-driver items.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetConnectOption(STD_FH_PARMS)
{
	RETCODE		rc;
	UWORD			fOption=*(UWORD *)lpParms[IPAR_GCO_FOPTION]->lpData;
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[IPAR_GCO_PVPARAM],			// pvParam
						lpUsrOptions->uDftBuffLen ,				// Use user-defined value
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetConnectOption (hdbc,
							 fOption,
							 (lpParms[IPAR_GCO_PVPARAM]->fNull) ? NULL : (PTR)lpParms[IPAR_GCO_PVPARAM]->lpData);	// vParam


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Check szDSN for null termination
/*
	switch (fOption)
	{
		case SQL_CURRENT_QUALIFIER:
		case SQL_OPT_TRACEFILE:
		case SQL_TRANSLATE_DLL:
			CHECKNULLTERM(ci, lpParms[2], lpParms[4], TRUE);
			break;
	}

*/

	// Log output parameters
	LogConnectParameters(szFuncName,lpParms,cParms,ci,IPAR_GCO_FOPTION,
						IPAR_GCO_PVPARAM,uAttrib);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	// Update the defaults for SetConnectOptions
	// If vParam is NULL, we don't propagate anything
	if(RC_SUCCESSFUL(rc))
		UpdateDefaults(SQL_API_SQLSETCONNECTOPTION,lpParms,IPAR_GCO_FOPTION,
			IPAR_GCO_PVPARAM,IPAR_GCO_PARAMETER_TYPE);


	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLGetConnectAttr:
//|	This replaces lpSQLGetConnectOption.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetConnectAttr(STD_FH_PARMS)
{
	RETCODE		rc;
	SQLINTEGER	fAttribute=*(SQLINTEGER *)lpParms[IPAR_GCA_FATTRIBUTE]->lpData;
	SQLINTEGER	cbValue=*(SQLINTEGER *)lpParms[IPAR_GCA_CBVALUE]->lpData;
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[IPAR_GCA_RGBVALUE], 			// rgbValue
						lpUsrOptions->uDftBuffLen ,
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetConnectAttr(hdbc,
								 fAttribute,																	// fAttribute
								 lpParms[IPAR_GCA_RGBVALUE]->fNull ? NULL:
											(SQLPOINTER)lpParms[IPAR_GCA_RGBVALUE]->lpData, 		// rgbValue
							    *(SQLINTEGER *)lpParms[IPAR_GCA_CBVALUE]->lpData,					// cbValue
							    lpParms[IPAR_GCA_PCBVALUE]->fNull ? NULL :
											(SQLINTEGER *)lpParms[IPAR_GCA_PCBVALUE]->lpData); //pcbValue

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LogConnectParameters(szFuncName,lpParms,cParms,ci,IPAR_GCA_FATTRIBUTE,
						IPAR_GCA_RGBVALUE,uAttrib);


	if (IsCharData(lpParms[IPAR_GCA_RGBVALUE]->fCType))
	{
		// Null termination and buffer modification checking
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);
	}

   //  Check for errors
	AUTOLOGERRORCI(ci,rc,hdbc);

	// Update the defaults for SetConnectOptions
	// If vParam is NULL, we don't propagate anything
	if(RC_SUCCESSFUL(rc))
		UpdateDefaults(SQL_API_SQLSETCONNECTATTR,lpParms,IPAR_SCA_FATTRIBUTE,
			IPAR_SCA_RGBVALUE,IPAR_SCA_PARAMETER_TYPE);

	return rc;

} //lpSQLGetConnectAttr()

//*---------------------------------------------------------------------------------
//| lpSQLSetEnvAttr:
//|	This function sets attributes set on the Environment Level.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSetEnvAttr(STD_FH_PARMS)
{
	RETCODE					rc;
	SQLINTEGER				fAttribute=*(SQLINTEGER *)lpParms[1]->lpData;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	SQLHENV					henv = NULL;
	SQLPOINTER				rgbValue=NULL;
	SQLINTEGER				cbValue=*(SQLINTEGER *)lpParms[3]->lpData;
	SQLHDBC					hdbc=NULL;
	SQLHSTMT					hstmt=NULL;

	//get handle
	if (lpParms[0]->lpData)
		henv=*(HENV *)lpParms[0]->lpData;

	if (!lpParms[2]->fNull)
	{
		if (IsCharData(lpParms[2]->fCType))
			rgbValue = lpParms[2]->lpData; 			// fOption requires character data
		else
			rgbValue = (SQLPOINTER)(LONG_PTR)*(UDWORD *)lpParms[2]->lpData;  // fOption requires numeric (UDWORD) data
	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLSetEnvAttr(henv,
						 	fAttribute,							  		// fAttribute
							lpParms[2]->fNull ? NULL :rgbValue,	// rgbValue
    						cbValue);									// cbValue

	if (RC_SUCCESSFUL(rc))
	{
		if (fAttribute == SQL_ATTR_OUTPUT_NTS)
			lpActiveEnv->fNTS=(UWORD)rgbValue == SQL_TRUE;

	}

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	if (rc != SQL_INVALID_HANDLE)
		AUTOLOGERROREI(ci,lpActiveEnv,rc,henv);

	return rc;

} //lpSQLSetEnvAttr()


//*---------------------------------------------------------------------------------
//| lpSQLGetEnvAttr:
//|	This function retrieves attributes set on the Environment Level.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetEnvAttr(STD_FH_PARMS)
{
	RETCODE					rc;
	SQLINTEGER				fAttribute=*(SQLINTEGER *)lpParms[1]->lpData;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	SQLHENV					henv=NULL;
	SQLPOINTER				rgbValue=NULL;
	SQLINTEGER				cbValueMax=*(SQLINTEGER *)lpParms[3]->lpData;
	SQLINTEGER 				*pcbValue=lpParms[4]->lpData;

	//get handle
	if (lpParms[0]->lpData)
		henv=*(HENV *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 													// rgbValue
						(*(SWORD *)lpParms[3]->lpData), 				//	  based on cbValueMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rgbValue=lpParms[2]->lpData;

	// Invoke function
	rc = SQLGetEnvAttr(henv,
						 	fAttribute,
    						lpParms[2]->fNull ? NULL : rgbValue,
    						cbValueMax,
    						lpParms[4]->fNull ? NULL : pcbValue);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

   //  Check for errors
	if (rc != SQL_INVALID_HANDLE)
		AUTOLOGERROREI(ci,lpActiveEnv,rc,henv);

	return rc;

} //lpSQLGetEnvAttr()


//*------------------------------------------------------------------------
//| LoadDfltArray:
//|	This function loads the Combobox specified by uID with the default
//|	array, lpdfts.
//| Parms:
//|	hstmt		current hstmt
//|	hDlg		handle to Dlg Wnd Proc
//|	sdAttr	Attribute to be retrieved
//|	uID		ID of Combo-box
//|	ciItems	number of items in lpdfts
//|	lpdfts	default array.
//| Returns:
//|	Current Attribute settings
//*------------------------------------------------------------------------
SDWORD INTFUN LoadDfltArray(HSTMT hstmt,HWND hDlg,SDWORD sdAttr,UINT uID,UINT ciItems,lpDFTARRAY lpdfts)
{
	UINT		i,
				idx=0;
	HWND		hwndCtl=GetDlgItem(hDlg,uID);
	DWORD		dwDex=0;
	TCHAR		szBuff[LARGEBUFF]=TEXT("");
	//SDWORD	nAttr=0;
    SQLLEN nAttr=0;
	RETCODE	rc=SQL_SUCCESS;
	BOOL		fFound=FALSE;

	//Get current atttribute value
	rc=SQLGetStmtAttr(hstmt,sdAttr,&nAttr,sizeof(nAttr),NULL);
	if (RC_NOTSUCCESSFUL(rc))
		nAttr=0;

	for (i=0;i < ciItems;i++)
	{
		if (lpdfts[i].fOpt == nAttr)
		{
			fFound=TRUE;
			idx=i;
		}

		//tack on the constant value
		wsprintf(szBuff,TEXT("%s=%d"),lpdfts[i].szOpt,lpdfts[i].fOpt);

		//Add string
		dwDex=(DWORD)SendMessage(hwndCtl,CB_ADDSTRING,0L,(LPARAM)szBuff);

		//Set Data value
		if(dwDex != CB_ERR)
			SendMessage(hwndCtl,CB_SETITEMDATA,(WPARAM)dwDex, (LPARAM)lpdfts[i].fOpt);
	}

	//Value wasn't in combo-box need to add it
	if (!fFound)
	{
		//Add string
		dwDex=(DWORD)SendMessage(hwndCtl,CB_ADDSTRING,0L,(LPARAM)_ultot((SDWORD)nAttr,szBuff,10));

		//Set Data value
		if(dwDex != CB_ERR)
		{
			idx=dwDex;
			SendMessage(hwndCtl,CB_SETITEMDATA,(WPARAM)dwDex, (LPARAM)nAttr);
		}
	}

	//Set current selection
	SendMessage(hwndCtl,CB_SETCURSEL,(WPARAM)idx, (LPARAM)0);

	return((SDWORD)nAttr);

} //LoadDfltArray()


//*------------------------------------------------------------------------
//| SetStmtAttr:
//|	This function sets the appropriate statement attribute
//| Parms:
//|	hstmt		HSTMT
//|	szAttr	Statement Attribute in string form
//|	fAttr		Statement Attribute
//|	nValue	Attribute Value
//|	dwDex		index (into the default array) of Attributes
//| Returns:
//|	Nothing
//*------------------------------------------------------------------------

VOID INTFUN SetStmtAttr(HSTMT hstmt,LPTSTR szAttr,INT fAttr,INT nValue,SDWORD dwDex)
{
PARAMITEM lpmtSQLSetStmtAttr[] = {
   TEXT("&StatementHandle:"),		PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,		NULL,		0,	0,0,		NULL,NULL,
   TEXT("&Attribute:"),				PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,		NULL,		0,	0,0,		NULL,NULL,
   TEXT("&ValuePtr:"),				PT_LOG_IN,						0,						0,						0,		0,			NULL,		0,		NULL,		0,	0,0,		NULL,NULL,
   TEXT("&StringLength:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,		NULL,		0,	0,0,		NULL,NULL,
   TEXT("fAttribute &Type:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,		NULL,		0,	0,0,		NULL,NULL,
	};

	UINT					uNumItems=NumItems(lpmtSQLSetStmtAttr);
	lpPARAMITEM			lplpSQLSetStmtAttr[(NumItems(lpmtSQLSetStmtAttr))];
	UINT					idx;
	SDWORD				sCType=SQL_C_SLONG;
	SDWORD				cbValueMax=SQL_IS_INTEGER;
	SWORD					ndx=0;

	//Find index for "fAttribute Type"
	ndx=GetCTypeIndex(!USE_C_DFT_ARRAY,TEXT("SQL_C_SLONG"));

	lpmtSQLSetStmtAttr[0].lpData = &hstmt;

	lpmtSQLSetStmtAttr[1].cDfts  = cifSetStmtAtr;
	lpmtSQLSetStmtAttr[1].lpDfts = lpdftfSetStmtAtr;

	lpmtSQLSetStmtAttr[1].dwDex = (SWORD)dwDex;
	lpmtSQLSetStmtAttr[1].lpData = &fAttr;
	lpmtSQLSetStmtAttr[1].cBytes = (UDWORD)sizeof(fAttr);

	lpmtSQLSetStmtAttr[2].lpData = &nValue;
	lpmtSQLSetStmtAttr[2].fCType = SQL_C_SLONG;
	lpmtSQLSetStmtAttr[2].cBytes = (UDWORD)sizeof(nValue);

	lpmtSQLSetStmtAttr[3].lpData = &cbValueMax;
	lpmtSQLSetStmtAttr[3].cBytes = sizeof(cbValueMax);

	lpmtSQLSetStmtAttr[4].cDfts  = ciCTypesNoD;
	lpmtSQLSetStmtAttr[4].lpDfts = lpdftCTypesNoD;

	lpmtSQLSetStmtAttr[4].dwDex = ndx;
	lpmtSQLSetStmtAttr[4].fCType = SQL_C_SLONG;
	lpmtSQLSetStmtAttr[4].lpData = &sCType;
	lpmtSQLSetStmtAttr[4].cBytes = (UDWORD)sizeof(sCType);

	for(idx=0; idx < uNumItems; idx++)
		lplpSQLSetStmtAttr[idx] = (lpPARAMITEM)&lpmtSQLSetStmtAttr[idx];

	// Now call SQLSetStmtAttr function handler
	lpSQLSetStmtAttr(TEXT("SQLSetStmtAttr"), uNumItems,
								lpActiveConnWin, lplpSQLSetStmtAttr, FALSE, NULL);


} //SetStmtAttr()

//*------------------------------------------------------------------------
//| SetConnectAttr:
//|	This function sets the appropriate connection attribute
//| Parms:
//|	hdbc		HDBC
//|	szAttr	Statement Attribute in string form
//|	fAttr		Statement Attribute
//|	nValue	Attribute Value
//|	dwDex		index (into the default array) of Attributes
//| Returns:
//|	Nothing
//*------------------------------------------------------------------------

VOID INTFUN SetConnectAttr(HDBC hdbc,LPTSTR szAttr,INT fAttr,INT nValue,SDWORD dwDex)
{
PARAMITEM lpmtSQLSetConnectAttr[] = {
   TEXT("&ConnectionHandle:"),PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,	NULL,	0,	0,0,		NULL,NULL,
   TEXT("&Attribute:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,	NULL,	0,	0,0,		NULL,NULL,
	TEXT("&ValuePtr:"),			PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,	NULL,	0,	0,0,		NULL,NULL,
   TEXT("&StringLength:"),		PT_LOG_IN,						SQL_C_SLONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,	NULL,	0,	0,0,		NULL,NULL,
   TEXT("fAttribute &Type:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,	NULL,	0,	0,0,		NULL,NULL,
	};

	UINT					uNumItems=NumItems(lpmtSQLSetConnectAttr);
	lpPARAMITEM			lplpSQLSetConnectAttr[(NumItems(lpmtSQLSetConnectAttr))];
	UINT					idx;
	SDWORD				sCType=SQL_C_SLONG;
	SDWORD				cbValueMax=SQL_IS_INTEGER;
	SWORD					ndx=0;

	//Find index for "fAttribute Type"
	ndx=GetCTypeIndex(!USE_C_DFT_ARRAY,TEXT("SQL_C_SLONG"));

	lpmtSQLSetConnectAttr[0].lpData = &hdbc;

	lpmtSQLSetConnectAttr[1].cDfts  = cifSetConAtr;
	lpmtSQLSetConnectAttr[1].lpDfts = lpdftfSetConAtr;

	lpmtSQLSetConnectAttr[1].dwDex = (SWORD)dwDex;
	lpmtSQLSetConnectAttr[1].lpData = &fAttr;
	lpmtSQLSetConnectAttr[1].cBytes = (UDWORD)sizeof(fAttr);

	lpmtSQLSetConnectAttr[2].lpData = &nValue;
	lpmtSQLSetConnectAttr[2].fCType = SQL_C_SLONG;
	lpmtSQLSetConnectAttr[2].cBytes = (UDWORD)sizeof(nValue);

	lpmtSQLSetConnectAttr[3].lpData = &cbValueMax;
	lpmtSQLSetConnectAttr[3].cBytes = sizeof(cbValueMax);

	lpmtSQLSetConnectAttr[4].cDfts  = ciCTypesNoD;
	lpmtSQLSetConnectAttr[4].lpDfts = lpdftCTypesNoD;

	lpmtSQLSetConnectAttr[4].dwDex = ndx;
	lpmtSQLSetConnectAttr[4].fCType = SQL_C_SLONG;
	lpmtSQLSetConnectAttr[4].lpData = &sCType;
	lpmtSQLSetConnectAttr[4].cBytes = (UDWORD)sizeof(sCType);

	for(idx=0; idx < uNumItems; idx++)
		lplpSQLSetConnectAttr[idx] = (lpPARAMITEM)&lpmtSQLSetConnectAttr[idx];

	// Now call SQLSetConnectAttr function handler
	lpSQLSetConnectAttr(TEXT("SQLSetConnectAttr"), uNumItems,
								lpActiveConnWin, lplpSQLSetConnectAttr, FALSE, NULL);


} //SetConnectAttr()


//*------------------------------------------------------------------------
//| ValidateField:
//|	This function validates the fields for the Cursor Attributes
//|	dialog box
//| Parms:
//|	hDlg		Dialog's HWND
//|	hwndCtl	Control's HWND
//|	nField	Field ID
//|	szField	Field Name
//|	fAddNum	Should the new number be added to combo-box.
//| Returns:
//|	-1 if Field entry is not valid otherwise return the user entered
//|	number.
//*------------------------------------------------------------------------

SDWORD INTFUN ValidateField(HWND hDlg,HWND hwndCtl,UINT nField,LPTSTR szField,
									 BOOL fAddNum)
{
	TCHAR		szNum[MEDBUFF];
	SDWORD	nNum=0;
	DWORD		dwDex=0;

	GetDlgItemText(hDlg,nField,szNum,tsizeof(szNum));
	if (!IsValidInt(SQL_C_ULONG,szNum, TRUE, szField))
	{
		SetFocus(hwndCtl);
		return(INVALID_NUMBER);
	}
	else  {
        nNum=_ttol(szNum);
        if (fAddNum)
        {
            //add number to combo-box
            dwDex=(DWORD)SendMessage(hwndCtl,CB_ADDSTRING,0L,(LPARAM)szNum);

            //Set Data value
            if(dwDex != CB_ERR)
                SendMessage(hwndCtl,CB_SETITEMDATA,(WPARAM)dwDex, (LPARAM)nNum);

            //Set current selection
            SendMessage(hwndCtl,CB_SETCURSEL,(WPARAM)dwDex, (LPARAM)0);
        }
    }

    return(nNum);
} //ValidateField()


//*------------------------------------------------------------------------
//| CursorAttribWndProc:
//|	This WndProc supports the "Cursor Attributes" dialog box
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
LRESULT EXTFUN CursorAttribWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SDWORD nOldBindType=0;
	static SDWORD nOldCursorType=0;
	static SDWORD nOldConcurrency=0;
	static SDWORD nOldUseBookMarks=0;
	//static SDWORD nOldArraySize=0;
	//static SDWORD nOldRowsetSize=0o
    static SQLLEN nOldArraySize=0;
    static SQLLEN nOldRowsetSize=0;


	switch(msg)
	{
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
		{
			SDWORD	iHdbc=0,
						iHstmt=0,
						idx;
			HWND		hwndHandle=GetDlgItem(hDlg,IDCB_CA_HANDLE);
			RETCODE	rc=SQL_SUCCESS;
			HSTMT		hstmt=NULL;

			CenterDialog(hDlg);

			//Load handles combo-box
			if (!lpActiveConnWin || !lpActiveConnWin->lpCurStmt)
				return(FALSE);

			LoadConnStmtHandles(hwndHandle,lpActiveConnWin,&iHdbc,&iHstmt,1);

			hstmt=lpActiveConnWin->lpCurStmt->hstmt;

			idx=(iHstmt ? iHstmt : iHdbc);

			//Set current selection
			SendMessage(hwndHandle,CB_SETCURSEL,(WPARAM)idx,0L);

			//Load default arrays
			nOldBindType=LoadDfltArray(hstmt,hDlg,SQL_ATTR_ROW_BIND_TYPE,IDCB_CA_BIND_TYPE,ciBindType,lpdftBindType);
			nOldCursorType=LoadDfltArray(hstmt,hDlg,SQL_ATTR_CURSOR_TYPE,IDCB_CA_CURSOR_TYPE,ciCursorType,lpdftCursorType);
			nOldConcurrency=LoadDfltArray(hstmt,hDlg,SQL_ATTR_CONCURRENCY,IDCB_CA_CONCURRENCY,ciConcurrency,lpdftConcurrency);
			nOldUseBookMarks=LoadDfltArray(hstmt,hDlg,SQL_ATTR_USE_BOOKMARKS,IDCB_CA_USE_BOOKMARKS,ciUseBookMarks,lpdftUseBookMarks);

			rc=SQLGetStmtAttr(hstmt,SQL_ATTR_ROW_ARRAY_SIZE,&nOldArraySize,sizeof(nOldArraySize),NULL);
			if (RC_NOTSUCCESSFUL(rc))
				nOldArraySize=1;

			SetDlgItemInt(hDlg, IDE_CA_ROW_ARRAY_SIZE,(SDWORD)nOldArraySize,FALSE);

			rc=SQLGetStmtAttr(hstmt,SQL_ROWSET_SIZE,&nOldRowsetSize,sizeof(nOldRowsetSize),NULL);
			if (RC_NOTSUCCESSFUL(rc))
				nOldRowsetSize=1;

			SetDlgItemInt(hDlg, IDE_CA_ROWSET_SIZE,(SDWORD)nOldRowsetSize,FALSE);

		}

		return TRUE;


		case WM_COMMAND:
			{
				SDWORD code=GET_WM_COMMAND_CODE(wParam, lParam);
				SDWORD id=GET_WM_COMMAND_ID(wParam,lParam);

			switch(id)
				{
				case IDOK:
					{
					UINT 						cbBytes=0;
					ULONG_PTR						dwDex=0,
												dwCurSel=0;
					HWND						hwndCtl=NULL;
					INT						nArraySize=0,
												nRowsetSize=0,
												nBindType=0,
												nCursorType=0,
												nUseBookMarks=0,
												nConcurrency=0;
					BOOL						fStmtAttr=TRUE;
					HANDLE					hHandle=NULL;
					TCHAR						szText[LARGEBUFF];
					SDWORD					iBindType,
												iCursorType,
												iConcurrency,
												iUseBookMarks;

					//retrieve results

					//Get Handle
					hwndCtl=GetDlgItem(hDlg,IDCB_CA_HANDLE);

					dwCurSel=(DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0L,0L);

					dwDex=SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)dwCurSel,0L);

					if (dwDex != CB_ERR)
						hHandle=(HANDLE)dwDex;

					GetDlgItemText(hDlg,IDCB_CA_HANDLE,szText,tsizeof(szText));
					fStmtAttr=(_tcsstr(szText,TEXT("hstmt ")) ? TRUE : FALSE);

					hwndCtl=GetDlgItem(hDlg,IDCB_CA_BIND_TYPE);
					iBindType=(DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0L,0L);
					nBindType=(DWORD)SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)iBindType, 0L);
					if (nBindType == CB_ERR)
					{
						nBindType=ValidateField(hDlg,hwndCtl,IDCB_CA_BIND_TYPE, TEXT("Bind Type"),
														TRUE);
						if (nBindType == INVALID_NUMBER)
							return(FALSE);
					}

					hwndCtl=GetDlgItem(hDlg,IDCB_CA_CURSOR_TYPE);
					iCursorType=(DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0L,0L);
					nCursorType=(DWORD)SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)iCursorType, 0L);

					hwndCtl=GetDlgItem(hDlg,IDCB_CA_CONCURRENCY);
					iConcurrency=(DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0L,0L);
					nConcurrency=(DWORD)SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)iConcurrency, 0L);

					hwndCtl=GetDlgItem(hDlg,IDCB_CA_USE_BOOKMARKS);
					iUseBookMarks=(DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0L,0L);
					nUseBookMarks=(DWORD)SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)iUseBookMarks, 0L);

					hwndCtl=GetDlgItem(hDlg,IDE_CA_ROW_ARRAY_SIZE);
					nArraySize=GetDlgItemInt(hDlg, IDE_CA_ROW_ARRAY_SIZE,NULL,TRUE);
					nArraySize=ValidateField(hDlg,hwndCtl,IDE_CA_ROW_ARRAY_SIZE, TEXT("Row Array Size"),
														TRUE);
					if (nArraySize == INVALID_NUMBER)
						return(FALSE);

					hwndCtl=GetDlgItem(hDlg,IDE_CA_ROWSET_SIZE);
					nRowsetSize=GetDlgItemInt(hDlg, IDE_CA_ROWSET_SIZE,NULL,TRUE);
					nRowsetSize=ValidateField(hDlg,hwndCtl,IDE_CA_ROWSET_SIZE, TEXT("Rowset Size"),
														TRUE);
					if (nRowsetSize == INVALID_NUMBER)
						return(FALSE);

					EndDialog(hDlg, IDOK);

					if (fStmtAttr)
					{
//						if (nOldBindType != nBindType)
							SetStmtAttr(hHandle,TEXT("SQL_ATTR_ROW_BIND_TYPE"),
										SQL_ATTR_ROW_BIND_TYPE,nBindType,iBindType);
//						if (nOldCursorType != nCursorType)
							SetStmtAttr(hHandle,TEXT("SQL_ATTR_CURSOR_TYPE"),
										SQL_ATTR_CURSOR_TYPE,nCursorType,iCursorType);
//						if (nOldConcurrency != nConcurrency)
							SetStmtAttr(hHandle,TEXT("SQL_ATTR_CONCURRENCY"),
										SQL_ATTR_CONCURRENCY,nConcurrency,iConcurrency);
//						if (nOldUseBookMarks != nUseBookMarks)
							SetStmtAttr(hHandle,TEXT("SQL_ATTR_USE_BOOKMARKS"),
										SQL_ATTR_USE_BOOKMARKS,nUseBookMarks,iUseBookMarks);
						if (nOldArraySize != nArraySize)
							SetStmtAttr(hHandle,TEXT("SQL_ATTR_ROW_ARRAY_SIZE"),
										SQL_ATTR_ROW_ARRAY_SIZE,nArraySize,0);
						if (nOldRowsetSize != nRowsetSize)
							SetStmtAttr(hHandle,TEXT("SQL_ROWSET_SIZE"),
										SQL_ROWSET_SIZE,nRowsetSize,0);
					}
					else
					{
//						if (nOldBindType != nBindType)
							SetConnectAttr(hHandle,TEXT("SQL_ATTR_ROW_BIND_TYPE"),
										SQL_ATTR_ROW_BIND_TYPE,nBindType,iBindType);
//						if (nOldCursorType != nCursorType)
							SetConnectAttr(hHandle,TEXT("SQL_ATTR_CURSOR_TYPE"),
										SQL_ATTR_CURSOR_TYPE,nCursorType,iCursorType);
//						if (nOldConcurrency != nConcurrency)
							SetConnectAttr(hHandle,TEXT("SQL_ATTR_CONCURRENCY"),
										SQL_ATTR_CONCURRENCY,nConcurrency,iConcurrency);
//						if (nOldUseBookMarks != nUseBookMarks)
							SetConnectAttr(hHandle,TEXT("SQL_ATTR_USE_BOOKMARKS"),
										SQL_ATTR_USE_BOOKMARKS,nUseBookMarks,iUseBookMarks);
						if (nOldArraySize != nArraySize)
							SetConnectAttr(hHandle,TEXT("SQL_ATTR_ROW_ARRAY_SIZE"),
											SQL_ATTR_ROW_ARRAY_SIZE,nArraySize,0);

						if (nOldRowsetSize != nRowsetSize)
							SetConnectAttr(hHandle,TEXT("SQL_ROWSET_SIZE"),
											SQL_ROWSET_SIZE,nRowsetSize,0);
					}

					}
					return TRUE;

				// Help key will search the ODBCAPI.HLP file
				case IDHELP:
					PostMessage(hDlg, USER_FUNCTION_HELP, 0, 0L);
					return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}

			if(code ==CBN_DBLCLK)
				SendMessage(hDlg, WM_COMMAND, IDOK, 0L);
			}
			return FALSE;

		case USER_FUNCTION_HELP:
			{
		//	iTestHelpCnt += InvokeHelp(hDlg, (LPCTSTR)szTestHelpFile,
		//			(LPCTSTR)szCURSORATRHELP);
			}
            return TRUE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}

	return FALSE;

} //CursorAttribWndProc()


//*---------------------------------------------------------------------------------
//| SetCursorAttributes:
//|	This function displays the IDD_CURSOR_ATTR dialog
//| Parms:
//|	lpCi			Current connection information
//|	hHndl			Descriptor handle
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID INTFUN SetCursorAttributes(lpSTATEMENTINFO lpStmt)
{
	// First get the connection info, if they cancel, nothing to do
	if(IDOK != DialogBoxParam(hInst,
					MAKEINTRESOURCE(IDD_CURSOR_ATTR),
					hwndClient,
					CursorAttribWndProc,
					(LPARAM)lpStmt))
		return;

} //SetCursorAttributes()

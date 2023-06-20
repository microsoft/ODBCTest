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
extern lpDFTARRAY					lpdftNullable;
extern lpHEADENV					lpHeadEnv;				//Pointer to Global Env structure

//*---------------------------------------------------------------------------------
//| lpSQLAllocHandle:
//|	This is a generic function for allocating handles. It replaces the
//|	individual functions SQLAllocEnv(),SQLAllocConnect(),and SQLAllocStatement().
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLAllocHandle(STD_FH_PARMS)
{
	RETCODE				rc=SQL_ERROR;
	UWORD					fHandleType = *(UWORD *)(lpParms[0]->lpData);
	lpCONNECTIONINFO	lpCi=ci;
	lpSTATEMENTINFO	lpState=((ci) ? ci->lpCurStmt : NULL);
	lpENVIRONMENTINFO	lpActiveEnv=GetActiveEnv();
	SQLHENV				henv=NULL;
	SQLHDBC				hdbc=NULL;
	SQLHSTMT				hstmt=NULL;
	SQLHDESC				hdesc=NULL;
	SQLHANDLE			hInHandle=NULL,
							hOutHandle=NULL;
	HWND					hwndOut=GETOUTPUTWINDOW(ci);
	BOOL					fDescHandle=FALSE;

	if (lpParms[1]->lpData)
		hInHandle=*(SQLHANDLE *)lpParms[1]->lpData;

	BUILDOUTPUTPARM(lpParms[2], 							// Allocate OutputHandlePtr
						sizeof(SQLHANDLE),					//  based on cbValueMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	rc = SQLAllocHandle(fHandleType,	hInHandle,
    							lpParms[2]->fNull ? SQL_NULL_HANDLE : lpParms[2]->lpData);

	if (lpParms[2]->lpData)
		hOutHandle=*(SQLHANDLE *)lpParms[2]->lpData;

	lpParms[2]->fHandle = PT_PTR_PTR;

	switch (fHandleType)
	{
		case SQL_HANDLE_ENV:
		case SQL_HANDLE_SENV:
			if (RC_SUCCESSFUL(rc))
				lpActiveEnv=DoAllocEnv(hOutHandle);
			if(lpActiveEnv)
				lpActiveEnv->henvCurr=hInHandle;
			break;
		case SQL_HANDLE_DBC:

			// If successful, create a new connection window for logging
			if (RC_SUCCESSFUL(rc))
			{
				lpCi = CreateConnectionWindow(hwndClient,hInst,lpActiveEnv);
				InitConnectionInfo(lpCi,lpActiveEnv);

				if(lpCi)
				{
					lpCi->hdbcCurr=lpCi->hdbc = hOutHandle;
					lpCi->uState = STATE_ALLOCATED_HDBC;
				}
			}

			if (lpCi)
				lpCi->hdbcCurr=hInHandle;

			break;
		case SQL_HANDLE_STMT:
			if (RC_SUCCESSFUL(rc))
				lpState=DoAllocStmt(lpCi, hOutHandle);

			if (lpCi)
				lpCi->hstmtCurr=hInHandle;
			break;
		case SQL_HANDLE_DESC:
			if (RC_SUCCESSFUL(rc))
				DoAllocDescriptor(ci,hOutHandle,TRUE);

			if (lpCi)
				lpCi->hdescCurr=hInHandle;
			break;

	} //switch (fHandleType)

	if (RC_NOTSUCCESSFUL(rc))
	{
		//reset henv and hdbc
		switch (fHandleType)
		{
			case SQL_HANDLE_SENV:
			case SQL_HANDLE_ENV:
			case SQL_HANDLE_DBC:
				if (lpActiveEnv)
					lpActiveEnv->henvCurr=henv=hInHandle;
				break;
			case SQL_HANDLE_STMT:
			case SQL_HANDLE_DESC:
				if (lpCi)
					lpCi->hdbcCurr=hdbc=hInHandle;
				break;
		}
	}

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	//If SQL_ERROR then the DM should have changed the
	//output handle to SQL_NULL_HANDLE
	if (RC_ERROR(rc) && lpParms[2]->lpData)
	{
		if (*(SQLHANDLE *)lpParms[2]->lpData != SQL_NULL_HANDLE)
		{
			TCHAR str[LARGEBUFF]=TEXT(""),
					szMsg[LARGEBUFF]=TEXT("");

			lstrcpy(str, lpParms[2]->szName);
			// Remove the '&' and ':' chars
			RemoveDlgFormat(str);

			szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1022),	str);
		}

	}

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   if(lpUsrOptions->fAutoErrors && (rc != SQL_INVALID_HANDLE))
	{
		ToolOptionsAllErrors(ci, lpActiveEnv ? lpActiveEnv->henv : NULL,
									hdbc,hstmt, hdesc,rc);
	}

	return(rc);

} //lpSQLAllocHandle()


//*---------------------------------------------------------------------------------
//| lpSQLFreeHandle:
//|	A successful free means we must destroy the statement node and remove the
//|	statement from our list and combo-box.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLFreeHandle(STD_FH_PARMS)
{
	RETCODE				rc=SQL_SUCCESS;
	UWORD					fHandleType = *(UWORD *)(lpParms[0]->lpData);
	lpSTATEMENTINFO	lpState=((ci) ? ci->lpCurStmt : NULL);
	SQLHANDLE			hInHandle=NULL;
	lpENVIRONMENTINFO	lpActiveEnv=GetActiveEnv();
	TCHAR					szTmpAddr[30];
	HDBC					hdbc=NULL;
	WORD					wIndex=0;
	lpDESCRIPTORHEADER	lpARD = NULL,
								lpAPD = NULL,
								lpIPD = NULL,
								lpIRD = NULL;

	if (lpParms[1]->lpData)
		hInHandle=*(SQLHANDLE *)lpParms[1]->lpData;

	//Find the correct StatementInfo node, we can't
	//assume the the user selected the current statement
	if (ci && ci->lpStmt)
		lpState=FindSINode(ci,hInHandle);

	if (lpState)
	{
		lpARD = lpState->lpARD;
		lpAPD = lpState->lpAPD;
		lpIPD = lpState->lpIPD;
		lpIRD = lpState->lpIRD;
	}

	// Log input parameters
	LOGPARAMETERSSPEC(szFuncName, lpParms,cParms, ci, TRUE);

	rc=SQLFreeHandle(fHandleType,hInHandle);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// If successful, add the new hstmt to the combo box and to our linked list
	switch(fHandleType)
	{
		case SQL_HANDLE_ENV:
		case SQL_HANDLE_SENV:
				AUTOLOGERROREI(ci,lpActiveEnv,rc,hInHandle);

				if (RC_SUCCESSFUL(rc))
					DoFreeEnvNode(lpActiveEnv);

				break;
		case SQL_HANDLE_DBC:
				if (RC_SUCCESSFUL(rc))
				{

					if(ci)
						DestroyConnectionWindow(ci);
					ci = NULL;

					// Don't use normal logging, since we don't know what window to
					//		log to.
					if(lpParms[1]->fNull)
						lstrcpy(szTmpAddr, TEXT("NULL"));
					else
						wsprintf(szTmpAddr, ADDRESSINFO, GETADDRINFO(hInHandle));


					szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsLogFreeHandle),
							(LPTSTR)szTmpAddr);


					// Log return code information
					LOGRETURNCODE(NULL, ci, rc);

					//Search for the first non-null henv
					wIndex=(WORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCOUNT, 0, 0L);

					if (wIndex)
						wIndex=(WORD)SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL, (WPARAM)1, 0L);
				}

				AUTOLOGERRORCI(ci,rc,hInHandle);

				break;

		case SQL_HANDLE_STMT:
			if (RC_SUCCESSFUL(rc)){
				//	first drop all the implicit descriptors on this hstmt
				//explicit ones are allocated on a per connection basis hence we do not worry about them
				if(lpARD)
					DoDropDesc(ci,lpARD->hdesc);

				if(lpAPD)
					DoDropDesc(ci,lpAPD->hdesc);

				if(lpIPD)
					DoDropDesc(ci,lpIPD->hdesc);

				if(lpIRD)
					DoDropDesc(ci,lpIRD->hdesc);

				DoDropStmt(ci,lpState);
			}

			AUTOLOGERRORSI(ci,rc,hInHandle);
			break;

		case SQL_HANDLE_DESC:
			if (RC_SUCCESSFUL(rc))
			{
				//if this is an an explicitly allocated descriptor being freed
				//reset the statement descriptors to the implicitly allocated ones
				if( lpARD && (lpARD->uwAllocType != SQL_DESC_ALLOC_AUTO)
					&& lpARD->hdesc == hInHandle)
				{
					lpState->lpARD = FindImplicitDescriptor(ci,DESC_ARD);
					if (ci->lpCurDesc->hdesc == hInHandle)
						ci->lpCurDesc = lpState->lpARD;
				}

				if( (lpAPD && lpAPD->uwAllocType != SQL_DESC_ALLOC_AUTO)
					&& lpAPD->hdesc == hInHandle)
				{
					lpState->lpAPD = FindImplicitDescriptor(ci,DESC_APD);
					if (ci->lpCurDesc->hdesc == hInHandle)
						ci->lpCurDesc = lpState->lpARD;
				}
				DoDropDesc(ci,hInHandle);
			}

			AUTOLOGERRORDESC(ci,rc,hInHandle);
			break;
	}

	return rc;
}


//*---------------------------------------------------------------------------------
//| FindImplicitDescriptor:
//|	This function searches lpCI, for the implicit DESCRIPTORHEADER record associated
//|	with the hDesc
//| Parms:
//|	lpci				Connection window to search
//|	hDesc				Descriptor handle to be found
//| Returns:
//|	lpDESCRIPTORHEADER	pointer to desc. record.
//*---------------------------------------------------------------------------------

lpDESCRIPTORHEADER INTFUN FindImplicitDescriptor(lpCONNECTIONINFO lpCI,UWORD	uwDescType)
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
			if (lpDesc->uwAllocType == SQL_DESC_ALLOC_AUTO
				&& lpDesc->uwDescType == uwDescType)
				//return descriptor record
				return(lpDesc);

			lpDesc=lpDesc->next;
		}
	}

	return(NULL);

}



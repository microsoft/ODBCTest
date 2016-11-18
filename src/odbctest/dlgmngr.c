//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGMNGR.C
//|
//| Purpose:
//|		This module implements the Dialog Manager component, including:
//|				Dialog Builder			Builds dialog template
//|				Window Proc				Generic window proc handles all dialogs
//|				Parm Builder			Gathers window input and puts in memory
//|		The functions in this module will also call the function handlers,
//|		each of which is located in a separate file specific to its
//|		menu group.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "blddata.h"
#include "resource.h"
#pragma warning(disable:4996)
//sumit
#define UCHAR_BYTE_COUNT					1

//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define DEX_ALLOC_CONNECT				0
#define DROPPED_RESET					2 //Need to reset length
#define ODBC_VER_30						3
#define ODBC_VER_25						2

#define USER_FUNCTION_HELP				(WM_USER + 400)


// If you want to get more detailed spy-typ dialogs messages, uncomment the
//		following line
//#define _DLGDEBUG
#ifdef _DLGDEBUG
#define ONDLGDEBUG(func)	func
#else
#define ONDLGDEBUG(func)
#endif



//----------------------------------------------------------------------------------
//	Globals
//
//		Note the global strings would normally be placed in a resource file, but
//		since they are used in a low memory environment, we don't want a
//		LoadString to fail.
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE 			hInst;
extern HWND					hwndClient;
extern HWND					hwndStatBar;
extern TCHAR 				szOutBuff[MAXOUTBUFF];
extern lpUSROPTIONS		lpUsrOptions;
extern BOOL					fNoUNICODE;

extern LPTSTR				szAPIHelpFile;
extern LPTSTR				szTestHelpFile;
extern int					iApiHelpCnt;				// How many times have we loaded this help file?
extern int					iTestHelpCnt;
extern BOOL					fUnicode;
extern TCHAR				szNULLPTR[];
extern TCHAR				szVALIDPTR[];

dCSEG(TCHAR)		 				szFREEMEM[]  = TEXT("Could not open dialog box for function '%s'. \
\r\n\r\nThis most often occurs due to low memory.  Please reduce used memory \
by closing any open applications or unnecessary windows and try request again.");


dCSEG(TCHAR) szHEXLDR[]								= TEXT("0X");
dCSEG(TCHAR) szVALID[]								= TEXT("VALID");
dCSEG(TCHAR) szZERO[] 								= TEXT("0");
dCSEG(TCHAR) szONE[]									= TEXT("1");
dCSEG(TCHAR) szZEROPOINTZERO[]					= TEXT("0.0");
dCSEG(TCHAR) szEMPTYSTR[] 							= TEXT("");
dCSEG(TCHAR) szBLANK[] 								= TEXT(" ");
dCSEG(TCHAR) szBINARYSTR[]							= TEXT("0x00");
dCSEG(TCHAR) szDFTDATE[] 							= TEXT("1993-12-31");
dCSEG(TCHAR) szDFTTIME[] 							= TEXT("12:31:15");
dCSEG(TCHAR) szDFTTIMESTAMP[] 					= TEXT("1993-12-31 12:31:15");
dCSEG(TCHAR) szUPDATECELLHELP[] 					= TEXT("update cell function tool");
dCSEG(TCHAR) szFILLPARAMHELP[] 					= TEXT("fill param function tool");
dCSEG(TCHAR) szDlgMngr[]							= TEXT("Dialog Manager");
dCSEG(TCHAR) szFillParameter[]					= TEXT("FillParam");
dCSEG(TCHAR) szUpdateCell[]						= TEXT("UpdateCell");
dCSEG(TCHAR) szInvalidFCType[]					= TEXT("Invalid fCType %d");
dCSEG(TCHAR) szCTypeNotFound[]					= TEXT("fSqlType %d not found, returning SQL_C_DEFAULT");
dCSEG(TCHAR) szDftHenv[]							= TEXT("<henv>");
dCSEG(TCHAR) szDftHdbc[]							= TEXT("<hdbc>");
dCSEG(TCHAR) szDftHstmt[]							= TEXT("<hstmt>");
dCSEG(TCHAR) szDftHdesc[]							= TEXT("<hdesc>");
dCSEG(TCHAR) szValidPointer[]						= TEXT("<valid pointer>");




// Following refer to default arrays which must be used out of context of
//		the parameter arrays
extern lpDFTARRAY					lpdftSqlTypes;
extern lpDFTARRAY					lpdftCTypes;
extern UINT 						ciSqlTypes;
extern UINT							ciCTypes;

// Following contains default ids for combo boxes
typedef struct tagCOMBOBOXSTYLES {
	LPTSTR		szType;							// combo item string
	UDWORD		uDftType;						// Default combo item
	UINT			idsVal;							// id of the text string to represent type
	SDWORD		fItem;							// The item data for pattern matching, 0 if N/A
	} COMBOBOXSTYLES;
dCSEG(COMBOBOXSTYLES) DftComboBoxStyles[] = {
		//	szType						uDftType						idsVal							fItem
		//	----------------------- -----------------------	-------------------------	----------------------------
/*0*/	TEXT("CBO_DFT_INPUT_WIN"),			CBO_DFT_INPUT_WIN,		idsInputWindow,				0,
/*1*/	TEXT("CBO_DFT_NULL"),				CBO_DFT_NULL,				idsNullPointer,				0,
/*2*/	TEXT("CBO_DFT_EMPTY"),				CBO_DFT_EMPTY,				idsEmptyString,				0,
/*3*/	TEXT("CBO_DFT_ROWSET_VALUE"),		CBO_DFT_ROWSET_VALUE,	idsRowsetValue,				0,
/*4*/	TEXT("CBO_DFT_SQL_NTS"),			CBO_DFT_SQL_NTS,			idsSQL_NTS,						SQL_NTS,
/*5*/	TEXT("CBO_DFT_NULL_DATA"),			CBO_DFT_NULL_DATA,		idsSQL_NULL_DATA,				SQL_NULL_DATA,
/*6*/	TEXT("CBO_DFT_DATA_AT_EXEC"),		CBO_DFT_DATA_AT_EXEC,	idsSQL_DATA_AT_EXEC,			SQL_DATA_AT_EXEC,
/*7*/	TEXT("CBO_DFT_DEFAULT_PARAM"),	CBO_DFT_DEFAULT_PARAM,	idsSQL_DEFAULT_PARAM,		SQL_DEFAULT_PARAM,
/*8*/	TEXT("CBO_DFT_ODBC_HANDLE"),		CBO_DFT_ODBC_HANDLE,		0,									0,
/*9*/	TEXT("CBO_DFT_NULL_HANDLE"),		CBO_DFT_NULL_HANDLE,		idsDFTNULL,						SQL_NULL_HANDLE,
/*10*/TEXT("CBO_DFT_VALID_POINTER"),	CBO_DFT_VALID_POINTER,	idsValidPointer,				0,
/*11*/TEXT("CBO_DFT_INTERVAL_VALUE"),	CBO_DFT_INTERVAL_VALUE,	idsIntervalValue,				0,
/*12*/TEXT("CBO_DFT_NULL_HENV"),			CBO_DFT_NULL_HENV,		idsDFTNULLHENV,				SQL_NULL_HENV,
/*13*/TEXT("CBO_DFT_NULL_HDBC"),			CBO_DFT_NULL_HDBC,		idsDFTNULLHDBC,				SQL_NULL_HDBC,
/*14*/TEXT("CBO_DFT_NULL_HSTMT"),		CBO_DFT_NULL_HSTMT,		idsDFTNULLHSTMT,				SQL_NULL_HSTMT,
/*15*/TEXT("CBO_DFT_NULL_HDESC"),		CBO_DFT_NULL_HDESC,		idsDFTNULLHDESC,				SQL_NULL_HDESC,

};


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN DialogWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL INTFUN HandleDlgInit(HWND hDlg, lpDIALOGDESC lpDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL INTFUN HandleControlMessage(HWND hDlg, lpDIALOGDESC lpDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL INTFUN UpdateComboBox(BOOL fEditUpdated, BOOL fInit,
				HWND hDlg, lpDIALOGDESC lpDlg,
				lpCONTROL lpCtl, lpPARAMITEM lpParm,
				WPARAM wParam, LPARAM lParam, UINT pdex);
BOOL INTFUN UpdateCkBoxStatus(HWND hDlg, lpDIALOGDESC lpDlg, lpCONTROL lpCtl,
			lpPARAMITEM lpParm, BOOL fState);
BOOL INTFUN CalculateLength(HWND hDlg, HWND hwndCtl, lpCONTROL lpCtl,lpPARAMITEM lpParm);
VOID INTFUN UpdateLengthItem(HWND hwnd, WORD wID,LPCTSTR szBuff,SWORD fCType,BOOL fNTS);
VOID INTFUN SetGenericDefaultByType(HWND hwnd, SWORD fCType);


//*---------------------------------------------------------------------------------
//| DialogManager:
//|	This function is a router which will build a dialog template and handle
//|	gathering the parameters.
//| Parms:
//|	ci							Connection Window to use, NULL if there isn't one
//|	dex						Index to the function to call
//|	uRunOptions				Bitmask indicates what we should run
//|									DLG_DISPLAY_ONLY		- User cannot update parameters
//|									DLG_CANCEL_IS_SKIP	- Change text of Cancel to Skip
//|									DLG_TIME_OUT			- Display is timed
//|	uTimeOut					When DLG_TIME_OUT is specified, indicates how many
//|									milliseconds the dialog should be displayed before
//|									it is dismissed
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN DialogManager(lpCONNECTIONINFO ci, int dex, UDWORD uRunOptions,
			UINT uTimeOut)
{
	HWND					fhwnd=GetFocus();
	lpDIALOGDESC		lpDlg;
	lpFUNCHNDLR			lpFuncHndlr;
	RETCODE				rc;

	// Build dialog if required
	lpDlg = DialogBuilder(dex);

	if(!lpDlg)
	{
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					NULL,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsCouldNotCreateDialog));
		return FALSE;
	}

	lpDlg->ci = ci;
	lpDlg->uRunOptions = uRunOptions;
	lpDlg->iFuncDex = dex;

	// Display the dialog
	lpDlg->fCancelled = (BOOL)DialogBoxParam(hInst,
													MAKEINTRESOURCE(rgApiList[dex].iDD),
													(ci) ? ci->hwnd : hwndClient,
													DialogWndProc,
													(LPARAM)(lpDIALOGDESC)lpDlg);

	// Check for error on dialog load
	if(lpDlg->fCancelled == -1)
		goto exit01;

	// Based on user choice and run-time options, call the function handler
	//		after setting cursor to hourglass
	if(!lpDlg->fCancelled)
	{
		// Didn't cancel dialog
		if(lpFuncHndlr = GetFunctionHandler(dex))
		{
			// There is a function handler present
			fBusy(TRUE);
			rc = (*lpFuncHndlr)(lpDlg->szFuncName, lpDlg->cParms,
						ci, lpDlg->lpParms,
						0, NULL);

			// Set the focus to the appropriate window.
			// The SQLAllocConnect and SQLExtendedFetch functions create new MDI
			//	windows and therefore if they were successful, take no further actions
			if ((dex == GetSqlFuncOffset(SQL_API_SQLALLOCCONNECT) ||
				  dex == GetSqlFuncOffset(SQL_API_SQLALLOCHANDLE)  ||
				  dex == GetSqlFuncOffset(SQL_API_SQLFETCHSCROLL)  ||
				  dex == GetSqlFuncOffset(SQL_API_SQLFREESTMT)	  ||
				  dex == GetSqlFuncOffset(SQL_API_SQLFREEHANDLE)  ||
				  dex == GetSqlFuncOffset(SQL_API_SQLFETCH)		  ||
			     dex == GetSqlFuncOffset(SQL_API_SQLEXTENDEDFETCH)) &&
				 RC_SUCCESSFUL(rc))								//	either was successful
			{
				;	//		so take no action
			}
			else if(lpActiveConnWin && fhwnd)
			{				// Put cursor in input window
				if(fhwnd != lpActiveConnWin->hwndStmts &&
					fhwnd != lpActiveConnWin->hwndIn &&
					fhwnd != lpActiveConnWin->hwndOut)
				{
					ConnWinMoveWindowFocus(lpActiveConnWin);	// Connection, window, but not in focus, so make it so
				}
				else
				{

					if(fhwnd == lpActiveConnWin->hwndOut)
							SetFocus(lpActiveConnWin->hwndIn);
					else
							SetFocus(fhwnd);
				}

			}
			else
				SetFocus(hwndOutWin);							// No connection window, use output window

			fBusy(FALSE);
			}
	}
	else
		SetFocus(fhwnd);

	FREE_PTR(lpDlg->lpTemplate);								// Free mem for next time

   return TRUE;

exit01:
	// Most likely cause is loss of memory.  Show error then free
	// our memory and exit since it could be invalid
	szMessageBox(GetActiveWindow(),
			MB_ICONEXCLAMATION | MB_OK,
			(LPTSTR)szDlgMngr,
			(LPTSTR)szFREEMEM,
			(LPTSTR)lpDlg->szFuncName);
	DestroyDialogs((UINT)dex);
	return FALSE;
}


//*------------------------------------------------------------------------
//|  DialogWndProc:
//|		This is the generic dialog procedure which handles all functions.
//|		Note that the lpDIALOGDESC structure we are currently handling
//|		is stored in the extra bytes of the dialog, such that if we ever
//|		desired to have multiple threads running dialogs (eg: background
//|		execution of a script), this code doesn't change.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
LRESULT EXTFUN DialogWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//------------------------------------------------------------------------------------
		// WM_INITDIALOG:    Minimum behavior is to register the 3d stuff, and center the
		//							dialog.  If there is no user proc, we'll fill in the defaults
		//							based on the data style.  Optionally user may have their own
		//							WndProc to handle extra items, such as adding DSN names.
		//------------------------------------------------------------------------------------
	case WM_INITDIALOG:
	{
		lpDIALOGDESC		lpDlg;

		// Following is minimum functionality
		fBusy(TRUE);
		lpDlg = (lpDIALOGDESC)lParam;
		SETWINDOWLONG(hDlg, lpDlg);						// Store pointer with window
		lpDlg->fCancelled = FALSE;							// Start out non-cancelled

		CenterDialog(hDlg);
		SetStatBarText(hwndStatBar, lpDlg->szFuncName, BLACK_TEXT);

		// Now set defaults for each control
		HandleDlgInit(hDlg, lpDlg, msg, wParam, lParam);

		fBusy(FALSE);
	}
	return TRUE;

	//------------------------------------------------------------------------------------
	// USER_FUNCTION_HELP:		This message is received either when the user presses
	//									the Help button or from the filter proc for F1
	//------------------------------------------------------------------------------------
	case USER_FUNCTION_HELP:
	{
		lpDIALOGDESC	lpDlg = (lpDIALOGDESC)GETWINDOWLONG(hDlg);
		LPCTSTR			szHelpItem = (LPCTSTR)lpDlg->szFuncName;

		if (!_tcscmp(szHelpItem, szFillParameter))
			szHelpItem = szFILLPARAMHELP;
		else if (!_tcscmp(szHelpItem, szUpdateCell))
			szHelpItem = szUPDATECELLHELP;

		/*if(szHelpItem == lpDlg->szFuncName)
		//	iApiHelpCnt += InvokeHelp(hDlg, szAPIHelpFile, szHelpItem);
		else
		//	iTestHelpCnt += InvokeHelp(hDlg, szTestHelpFile, szHelpItem);
		}*/
		return TRUE;




		//------------------------------------------------------------------------------------
		// WM_COMMAND:   		Message is sent for lots of stuff.  We'll track changes in
		//							controls, button clicks, and notification messages.
		//------------------------------------------------------------------------------------
	case WM_COMMAND:
	{
		WORD				id = GET_WM_COMMAND_ID(wParam, lParam);
		WORD				code = GET_WM_COMMAND_CODE(wParam, lParam);
		lpDIALOGDESC	lpDlg = (lpDIALOGDESC)GETWINDOWLONG(hDlg);

		// Now check for our buttons
		switch (id)
		{
			/*
			case WM_LBUTTONDOWN:
			{
			lpCONNECTIONINFO		lpCI=lpDlg ? lpDlg->ci : NULL;
			lpSTATEMENTINFO		lpSI=lpCI ? lpCI->lpCurStmt : NULL;
			lpDESCRIPTORRECORD		lpRow=NULL;
			SQLHDESC					hAPD=NULL;
			UDWORD					*pudOctetLen=NULL;
			UWORD						iPar=0;
			RETCODE					rc=SQL_SUCCESS;
			TCHAR						szMsg[SMALLBUFF]=TEXT("");

			if (lpSI)
			{
			if (lpSI->lpAPD)
			{
			hAPD=lpSI->lpAPD->hdesc;

			if (lpRow=lpSI->lpAPD->lpRow)
			pudOctetLen=&(lpRow->udOctetLen);

			}

			if(lpSI->lpCurParam)
			iPar=lpSI->lpCurParam->ipar;
			}

			//User depressed the "Get Descriptor" button from the SQLBindParam
			//  Dialog box.

			rc=SQLGetDescField(hAPD,iPar, SQL_DESC_OCTET_LENGTH,
			pudOctetLen,sizeof(UDWORD),NULL);

			if (RC_SUCCESSFUL(rc))
			{
			//Display cbValueMax(SQL_DESC_OCTET_LENGTH) retrieved
			wsprintf(szMsg,TEXT("SQL_DESC_OCTET_LENGTH=%l"),*pudOctetLen);

			szMessageBox(GetActiveWindow(),
			MB_ICONEXCLAMATION | MB_OK,
			NULL,
			szMsg,
			TEXT("SQLGetDescField"));
			}
			else
			{
			//Display error
			DisplayDiagnostics(rc,SQL_HANDLE_DESC,hAPD,TEXT("SQLGetDescField"));
			}

			}
			break;

			*/

			// Validate all parameters, then call parameter builder
		case IDOK:
		{
			HWND				hFocus, hOK;
			BOOL				fFound = FALSE;
			BOOL				fNoError = TRUE;
			lpCONTROL		lpCtl = NULL;
			UINT				idx;

			// If the user just hits Enter on the keyboard, we won't get
			//		an update message for any controls and things like calculated
			//		length won't get done whereas clicking OK would cause
			//		them to be done.  To fix this, force focus change to OK
			//		button.
			if (hFocus = GetFocus())
				if (hFocus != (hOK = GetDlgItem(hDlg, IDOK)))
					SetFocus(hOK);

			// Now validate the data entered, if successful store and return
			if (ValidateParameters(hDlg, lpDlg))
			{
				// Look through the control array for a UsrWndProc and call it if it has one
				lpCtl = *(lpDlg->lpCtl);

				for (idx = 0; idx < lpDlg->cCtlItems; idx++)
				{
					if ((lpCtl[idx]).lpUsrWndProc)
					{
						lpCtl = &lpCtl[idx];
						fFound = TRUE;
						break;
					}
				}

				if (fFound)							// User has their own WndProc
					fNoError = (*lpCtl->lpUsrWndProc)(hDlg, USER_IDOK,
					wParam, lParam, (lpVOID)lpDlg, IDOK);

				// Now build the parameters
				if (fNoError)
				{
					if (ParameterBuilder(hDlg, lpDlg))
						// Tell caller of the user's answer and close
						EndDialog(hDlg, (lpDlg->fCancelled = FALSE));
				}

			}
		}

		return TRUE;

		// Escape needs to set run status for caller
		case IDCANCEL:
			EndDialog(hDlg, (lpDlg->fCancelled = TRUE));
			return TRUE;

			// Help key will search the ODBCAPI.HLP file
		case IDHELP:
			PostMessage(hDlg, USER_FUNCTION_HELP, 0, 0L);
			return TRUE;

		}		// End of button clicks


		// Now look at notification messages
		switch (code)
		{
			// Edit codes
		case EN_KILLFOCUS:
		case EN_UPDATE:
			// Combo box codes
		case CBN_DROPDOWN:
		case CBN_EDITCHANGE:
		case CBN_EDITUPDATE:
		case CBN_KILLFOCUS:
		case CBN_SELCHANGE:
		case CBN_CLOSEUP:
			if (!lpDlg->fCancelled)						// Haven't already cancelled
				if (HandleControlMessage(hDlg, lpDlg, msg, wParam, lParam))
					return TRUE;					// Default behavior overriden
			return FALSE;

		}		// End of notification messages

		// At this point it was not one of the default buttons or a notification code,
		//		last possibility is a control message we want to process (eg: a check-box)
		if (id >= DLG_BASE_BEGIN && id <= DLG_BASE_END)
			if (HandleControlMessage(hDlg, lpDlg, msg, wParam, lParam))
				return TRUE;
	}			// End of WM_COMMAND
	return FALSE;

	default:
		return FALSE;
	}

	}
}

//*---------------------------------------------------------------------------------
//| IsODBCHandleCtl:
//| Parms:
//|	uParmType		Parameter type
//| Returns:
//|	TRUE	if this parameter is an ODBC Handle Control (i.e. combo-box)
//|	FALSE if not
//*---------------------------------------------------------------------------------

BOOL __stdcall IsODBCHandleCtl(UDWORD uParmType)
{
	return(uParmType & PT_HENV ||
		 uParmType & PT_HDBC ||
		 uParmType & PT_HSTMT ||
		 uParmType & PT_HDESC ||
		 uParmType & PT_HHANDLE ||
		 uParmType & PT_MULT_HDESC ||
		 uParmType & PT_HWND);

} 


//*---------------------------------------------------------------------------------
//| AddHandle:
//|	 This function adds szHandle to the control (hwndCtl) and then addes the
//|	data item hHandle
//| Parms:
//|	hwndCtl			handle to control
//|	hHandle			handle data to be set
//|	szStr				Additional info.
//|	szClientTitle	title info to be added to handle string
//|	szDesc			Descriptor description
//| Returns:
//|	FALSE if not
//*---------------------------------------------------------------------------------

SDWORD __stdcall AddHandle(HWND	 hwndCtl, SQLHANDLE hHandle, LPTSTR szStr, UINT uNum, LPTSTR szClientTitle, LPTSTR szDesc)
{
	TCHAR		szHandle[MEDBUFF]=TEXT(""),
				szHndlStr[MEDBUFF]=TEXT("");
	SDWORD	dwDex=0;

	HandleToText(szHandle,hHandle);

	if (hHandle==SQL_NULL_HANDLE)
		lstrcpy(szHndlStr,szStr);
	else if (szDesc && szClientTitle)
		wsprintf(szHndlStr,TEXT("%s %d: %s (%s)\"%s\""),szStr,uNum,szHandle,szClientTitle,szDesc);
	else if (szClientTitle)
		wsprintf(szHndlStr,TEXT("%s %d: %s (%s)"),szStr,uNum,szHandle,szClientTitle);
	else
		wsprintf(szHndlStr,TEXT("%s %d: %s"),szStr,uNum,szHandle);

	dwDex=(DWORD)SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)szHndlStr);

	if(dwDex != CB_ERR)
		SendMessage(hwndCtl, CB_SETITEMDATA,(WPARAM)dwDex, (LPARAM)hHandle);

	//return the index where the handle was inserted
	return(dwDex);

} //AddHandle()


//*---------------------------------------------------------------------------------
//| LoadConnStmtHandles:
//|	This function loads all Connection and Statement handles
//| Parms:
//|	hwnd			window handle to combo-box
//|	lpEI			pointer to env. info record
//|	piHdbc		pointer to index of current HDBC
//|	piHstmt		pointer to index of current HSTMT
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN LoadConnStmtHandles(HWND hwnd,lpCONNECTIONINFO lpCI,SDWORD *piHdbc,SDWORD *piHstmt,
											SWORD cConns)
{
	lpSTATEMENTINFO	lpSI=NULL;
	SWORD					iConn,
							iStmt;
	SDWORD				iItem=0;

	for (iConn=0;iConn < cConns;iConn++)
	{
		iItem=AddHandle(hwnd,lpCI->hdbc,TEXT("hdbc"),lpCI->uConNum,NULL,NULL);

		*piHdbc=iItem;

		//Load all hstmts
		if (lpCI->lpStmt)
		{
			lpSI=lpCI->lpStmt;

			for (iStmt=0;iStmt < lpCI->cStmts;iStmt++)
			{
				iItem=AddHandle(hwnd,lpSI->hstmt,TEXT("hstmt"),lpSI->uStmtNum,NULL,NULL);

				if (lpSI == lpCI->lpCurStmt)
					*piHstmt=iItem;

				lpSI=lpSI->next;
			}

		} //if (lpStmt)

		lpCI=lpCI->next;
	}

} //LoadConnStmtHandles()


//*---------------------------------------------------------------------------------
//| LoadODBCHandles:
//| Parms:
//|	lpEI			pointer to env. info record
//|	hwndCtl		window handle to combo-box
//|	uParmType	identifies paramter type.
//| Returns:
//|	Return index of handle to be set as the current selection
//*---------------------------------------------------------------------------------

SDWORD INTFUN LoadODBCHandles(lpENVIRONMENTINFO lpEI,HWND hwndCtl,UDWORD uParmType)
{
	DWORD		dwDex=0;
	lpCONNECTIONINFO		lpCI=NULL;
	lpSTATEMENTINFO		lpSI=NULL;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	SWORD						cConns=0,
								cStmts=0,
								cDescs=0,
								iConn;
	SDWORD					iItem=1,
								iHenv=0,
								iHdbc=0,
								iHstmt=0,
								iHdesc=0,
								iDesc,
								dwErr=0;
	BOOL						fSet=FALSE;
	TCHAR						szBuff[MEDBUFF]=TEXT("");

	if (!lpEI)
		return(0);

	//Load henv
	iItem=AddHandle(hwndCtl,lpEI->henv,TEXT("henv"),lpEI->uEnvNum,NULL,NULL);

	if (!iHenv)
		iHenv=iItem;

	if (lpActiveConnWin)
		LoadConnStmtHandles(hwndCtl,lpActiveConnWin,&iHdbc,&iHstmt,1);

	//Are we processing SQLCopyDesc?
	if (uParmType & PT_MULT_HDESC)
	{
		//Yes! We need to load all descriptors from
		//all connections
		cConns=lpEI->cConns;
		lpCI=lpEI->lpConn;
	}
	else
	{
		//No!! we only want descriptors from the
		//current connection
		if (lpCI=lpActiveConnWin)
			cConns=1;
	}

	for (iConn=0;iConn < cConns;iConn++)
	{
		//Load all hdesc's
		if (lpCI && lpCI->lpDesc)
		{
			lpDesc=lpCI->lpDesc;

			for (iDesc=0;iDesc < lpCI->cDescs;iDesc++)
			{
				if (lpDesc->lpSI)
					wsprintf(szBuff,TEXT("hdbc %u, hstmt %u"),lpCI->uConNum,
									lpDesc->lpSI->uStmtNum);
				else
					wsprintf(szBuff,TEXT("hdbc %u"),lpCI->uConNum);

				iItem=AddHandle(hwndCtl,lpDesc->hdesc,TEXT("hdesc"),lpDesc->uDescNum,szBuff,
							lpDesc->szName);

				//Use flag fSet becuase iItem may be 0 which is
				// a valid hdesc
				if (!fSet)
				{
					//just pick the first hdesc
					iHdesc=iItem;
					fSet=TRUE;
				}

				lpDesc=lpDesc->next;
			}

		} //if (lpDesc)

		if(lpCI)
			lpCI=lpCI->next;
	}


	//Now set current select for this combo-box (i.e. if this is an
	//API that requires an HSTMT, then make the current hstmt the current
	//selection.
	if (uParmType & PT_HENV)
		iItem=iHenv;
	else if (uParmType & PT_HDBC)
		iItem=iHdbc;
	else if (uParmType & PT_HSTMT)
		iItem=iHstmt;
	else if (uParmType & PT_HDESC)
		iItem=iHdesc;
	else
		iItem=iHenv;

	//Return index of handle to be current selection
	return(iItem);

} //LoadODBCHandles()



//*---------------------------------------------------------------------------------
//| HandleDlgInit:
//|	Handle the WM_INITDIALOG message.
//| Parms:
//|	hDlg						Dialog window handle
//|	lpDlg						The dialog description
//|	msg						For user WndProc
//|	wParam					For user WndProc
//|	lParam					For user WndProc
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN HandleDlgInit(HWND hDlg, lpDIALOGDESC lpDlg, UINT msg,
		WPARAM wParam, LPARAM lParam)
{
	lpCONTROL 			lpCtl;
	lpPARAMITEM			lpParm;
	UINT					dex, uiCtldex;
	UDWORD				wCtlTypeTmp;
	UINT					len=0;
	UINT					cDfts=0;
	SDWORD				dwErrIdx=0;

	// Set defaults for each control
	for(dex=0;  dex<lpDlg->cCtlItems;  dex++) {
		lpCtl = lpDlg->lpCtl[dex];							// Get current control
		uiCtldex = dex;
		if(lpCtl->iParmNum != -1)
		{
			lpParm = lpDlg->lpParms[lpCtl->iParmNum];	// Parameter description

			//Clear NOTMODIFIED flag
			lpParm->uParmType &= ~PT_OUT_NOTMODIFIED;
		}

		// Either user did not have their own WndProc, or they want the default behavior
		switch(lpCtl->type) {
			case DLG_COMBO:
				{
				UINT					pdex;
				static TCHAR		szTmp[MAXEXPANDEDCONSTANT];
				TCHAR					szCalc[25];
				DWORD					adex, dwDex;
				LPTSTR				szAddStr;
				DWORD					dwSel, dwLen=0;
				HWND					hwndIn=(lpActiveConnWin) ? lpActiveConnWin->hwndIn : hwndOutWin;
				HWND					hwndCtl=GetDlgItem(hDlg, lpCtl->wID);
				lpCONNECTIONINFO	lpCI=lpDlg ? lpDlg->ci : NULL;
				lpENVIRONMENTINFO	lpEnv=GetActiveEnv();
				SQLHENV				henv=lpEnv ? lpEnv->henv : NULL;
				SQLHDBC				hdbc=NULL;
				SQLHSTMT				hstmt=NULL;
				SQLHDESC				hdesc=NULL;

				if (lpCI)
				{
					hdbc=lpCI->hdbc;

					if (lpCI->lpCurStmt)
						hstmt=lpCI->lpCurStmt->hstmt;

					if (lpCI->lpDesc)
						hdesc=lpCI->lpDesc->hdesc;

				}

				// The following indicates that a calcuated length has been added to the
				//	combo box when the string was loaded and it was an <input window>.  If
				//	so, save the length value and add it after the defaults
				szCalc[0] = TEXT('\0');
				if(lpCtl->lDataStyle & (CBO_INWIN_LEN | CBO_DFT_SHOW_CALLEN) &&
					SendMessage(hwndCtl, CB_GETCOUNT, 0, 0L))
				{
					if(CB_ERR != SendMessage(hwndCtl, CB_GETLBTEXT,
													0, (LPARAM)(LPTSTR)szCalc))
					{
						dwLen = (DWORD)SendMessage(hwndCtl, CB_GETITEMDATA, 0, 0L);
						SendMessage(hwndCtl, CB_RESETCONTENT, 0, 0L);
					}
					else
						szCalc[0] = TEXT('\0');
				}

				// Hunt for default CBO_DFT_xxx styles and add them, always at the beginning
				for(pdex=0;  pdex<NumItems(DftComboBoxStyles);  pdex++)
				{
					if(lpCtl->lDataStyle & DftComboBoxStyles[pdex].uDftType)
					{
						if (!DftComboBoxStyles[pdex].idsVal)
							continue;

						szAddStr = GetGtrString(szOutBuff,
											MAXOUTBUFF, DftComboBoxStyles[pdex].idsVal);

						// In the case where a selection exists for input window. change text
						if(DftComboBoxStyles[pdex].uDftType & CBO_DFT_INPUT_WIN)
						{
							dwSel = (DWORD)SendMessage(hwndIn, EM_GETSEL, 0, 0L);
							if(HIWORD(dwSel) != LOWORD(dwSel))
								szAddStr = GetGtrString(szOutBuff, MAXOUTBUFF, idsInputWindowSel);
						}

						// Now that we have the string, add it.  In addition, for those
						//	values which have an integer equivalent, set the item data to
						//	that constant so that pattern matching works correctly
						dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)szAddStr);
						if(dwDex != CB_ERR &&
							DftComboBoxStyles[pdex].fItem)
							SendMessage(hwndCtl, CB_SETITEMDATA,
										(WPARAM)dwDex, (LPARAM)DftComboBoxStyles[pdex].fItem);
					}
				}


				//If we are processing a ODBC handle combo-box, load it with all
				//available handles
				if (IsODBCHandleCtl(lpParm->uParmType))
				{
					SDWORD dwCurSel=0;
					lpParm->fHandle=PT_HHANDLE;

					dwCurSel=LoadODBCHandles(lpEnv,hwndCtl,lpParm->uParmType);

					if (lpParm->dwDex)
						dwCurSel=lpParm->dwDex;

					dwErrIdx=(DWORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_SETCURSEL, (WPARAM)dwCurSel, 0L);

					if (dwErrIdx == CB_ERR)
						dwErrIdx=(DWORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_SETCURSEL, 0L, 0L);

					continue;
				}

				// Check for a default array.  Add the expanded version of the text
				//		to the combo-box, and store the fOpt with the item to allow
				//		faster pattern matching on integers
				if(lpParm->lpDfts)
				{
					for(pdex=0; pdex<lpParm->cDfts;  pdex++)
					{
						// Temp var required for following to work (wouldn't work directly)
						GetExpandedConstant(szTmp, &lpParm->lpDfts[pdex],
								lpUsrOptions->fVersionNumbers);
						dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING,
											0, (LPARAM)(LPTSTR)szTmp);

						SendMessage(hwndCtl, CB_SETITEMDATA,
											(WPARAM)dwDex,
											(LPARAM)lpParm->lpDfts[pdex].fOpt);
					}
				}

				// Check for that calculated length and if present, add it to the end
				if(*szCalc)
				{
					dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING,
								0, (LPARAM)(LPTSTR)szCalc);
					SendMessage(hwndCtl, CB_SETITEMDATA, (WPARAM)dwDex,
											(LPARAM)dwLen);
				}

				// If INDEX_DFT_ZERO is specified for the dwDex, then the the value should
				//		be set to 0 for a default.  Or, if there is to be a show calculated
				//		length, we need to add a place holder for first time through
				if(lpParm->dwDex == DEX_DFT_ZERO ||							// Want zero for a default
					 (lpCtl->lDataStyle & CBO_DFT_SHOW_CALLEN &&
					 (szCalc[0] == TEXT('\0'))))
				{

					adex = (DWORD)SendMessage(hwndCtl,
								CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szZERO);
					SendMessage(hwndCtl, CB_SETCURSEL, (WPARAM)adex, 0L);
				}

				// For case of a cb value derived from an input window string, just use
				//	the last selection, length gets overriden every time
				dwDex = (DWORD)SendMessage(hwndCtl, CB_GETCOUNT, 0, 0L) - 1;

				cDfts=GetDftStyleCount(lpCtl->lDataStyle);


				if(lpCtl->lDataStyle & CBO_INWIN_LEN &&
					(lpParm->dwDex == CB_ERR ||
					 lpParm->dwDex == (SWORD)dwDex))
				{
					lpParm->dwDex = (SWORD)dwDex;
					SendMessage(hwndCtl, CB_SETCURSEL, (WPARAM)lpParm->dwDex, 0L);
				}

				// Set the current selection based on last run
				else if(lpParm->dwDex != CB_ERR									     &&  // There was a value selection
					((UINT)lpParm->dwDex < cDfts ||	// and user selected a CBO_DFT last time
				   (lpParm->lpDfts &&															// 	or Default array is present
					 (UINT)lpParm->dwDex < lpParm->cDfts)))								//			and was used last time
				{
					dwErrIdx=(DWORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_SETCURSEL, (WPARAM)lpParm->dwDex, 0L);
					if (dwErrIdx == CB_ERR)
						dwErrIdx=(DWORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_SETCURSEL, 0L, 0L);
				}
				else if(lpParm->lpData)
				{
					// User had their own data so add it to the edit box
					if (IsCharData(lpParm->fCType))
					{
						BOOL fNTS=FALSE;
						lpPARAMITEM lpParmPcb=NULL;

						//check if the user selected SQL_NTS, if they did don't change it.
						if (lpParm && lpParm->lpParmpcb)
						{
							lpParmPcb=lpParm->lpParmpcb;
							if (lpParmPcb->lpData)
								fNTS=*(SWORD *)(lpParmPcb->lpData) == SQL_NTS;
						}

						SetWindowText(GetDlgItem(hDlg, lpCtl->wID), (LPTSTR)lpParm->lpData);
						if(lpCtl->wLengthID)
						{
							len=(UDWORD)lstrlen((LPTSTR)lpParm->lpData) * sizeof(TCHAR);
							wsprintf(szOutBuff, TEXT("%lu"), len);
							UpdateLengthItem(hDlg,lpCtl->wLengthID, (LPTSTR)szOutBuff,lpParm->fCType,fNTS);
						}
					}
					else
					{
						GetCharValueFromParm(szOutBuff, sizeof(szOutBuff), lpParm,
								FALSE, NULL, NULL, NULL, NULL, 0,GETOUTPUTWINDOW(lpCI));

						// If the calculated length is to be shown, make it an item, not just text
						if(lpCtl->lDataStyle & CBO_DFT_SHOW_CALLEN)
							UpdateLengthItem(hDlg, lpCtl->wID, szOutBuff,lpParm->fCType,FALSE);
						// Else just put the data in as though it was typed text
						else
							SetWindowText(GetDlgItem(hDlg, lpCtl->wID), (LPTSTR)szOutBuff);
					}
				}
				// By default take first item
				else
					SendDlgItemMessage(hDlg, lpCtl->wID, CB_SETCURSEL, (WPARAM)lpParm->dwDex, 0L);

				// Invoke user WndProc and set linked arrays if required
				UpdateComboBox(FALSE, TRUE,
								hDlg, lpDlg,
								lpCtl, lpParm,
								0, 0L, uiCtldex);
				}
				break;

			// For edit, check the paramter type to determine maximum length
			case DLG_EDIT:
			{
				SendDlgItemMessage(hDlg, lpCtl->wID, EM_LIMITTEXT, lpUsrOptions->uDftBuffLen, 0L);

				// If this is the first time this function is called, set default
				//		value accordingly
				if(lpParm->dwDex == DEX_DFT_ONE || lpParm->dwDex == DEX_DFT_ZERO)
					SetWindowText(GetDlgItem(hDlg, lpCtl->wID),
								(lpParm->dwDex == DEX_DFT_ONE) ? (LPTSTR)szONE : (LPTSTR)szZERO);
				// If there are previous values, use them
				else if(lpParm->lpData)
				{
					if (IsCharData(lpParm->fCType))
					{
						SetWindowText(GetDlgItem(hDlg, lpCtl->wID), (LPTSTR)lpParm->lpData);
					}
					else
					{
						GetCharValueFromParm(szOutBuff, sizeof(szOutBuff), lpParm,
								FALSE, NULL, NULL, NULL, NULL, 0,GETOUTPUTWINDOW(lpDlg->ci));

						SetWindowText(GetDlgItem(hDlg, lpCtl->wID), (LPTSTR)szOutBuff);
					}
				}
				// Else use the default length
				else
				{
					TCHAR	szTmp[10];

					wsprintf(szTmp, TEXT("%u"), lpUsrOptions->uDftBuffLen);
					SetWindowText(GetDlgItem(hDlg, lpCtl->wID), (LPTSTR)szTmp);
				}
			}
			break;

			// For check box, check for NULL indicator and set accordingly
			case DLG_CKBOX:
				// For the case of an ordinary checkbox (PT_CHK), we don't want the normal checkbox handling
				wCtlTypeTmp = lpParm->uParmType & PT_CHK;
				if(!wCtlTypeTmp){
				// For the case where fNull was set only because a valid handle was
				//		not available, reverse this now
				if(lpParm->dwDex == DEX_FORCED_NULL) {
					lpParm->fNull = FALSE;
					lpParm->dwDex = 0;					// Clear for next time
					}
				UpdateCkBoxStatus(hDlg, lpDlg, lpCtl, lpParm, lpParm->fNull);
				CheckDlgButton(hDlg, lpCtl->wID, lpParm->fNull);
				}
				break;

			case DLG_BUTTON:
				SetWindowText(GetDlgItem(hDlg, lpCtl->wID),TEXT("..."));
				break;

			}



		// Now check for user defaults, WM_INITDIALOG
		if(lpCtl->lpUsrWndProc)							// User has their own WndProc
			if((*lpCtl->lpUsrWndProc)(hDlg, msg, wParam, lParam, (lpVOID)lpDlg, dex))
				continue;									// Move on to next control
		}  // End loop through each control
	return TRUE;
}


//*---------------------------------------------------------------------------------
//| SetHandle:
//|	This function returns a Valid Handle or NULL handle depending on what it is
//|	passed.
//| Parms:
//|	phHandle				Pointer to an ODBC handle that was used in API call, In the case of
//|								invalid handle testing
//|	phActHandle			Pointer to actual ODBC handle that was used in API call, In the case of
//|								invalid handle testing
//| Returns:
//|	handle
//*---------------------------------------------------------------------------------

__inline SQLHANDLE INTFUN SetHandle(SQLHANDLE *phHandle,SQLHANDLE *phActHandle)
{
	SQLHANDLE hHandle=NULL;

	if (phActHandle && *phActHandle)
			hHandle= *phActHandle;
	else if (phHandle && *phHandle)
		hHandle = *phHandle;

	return(hHandle);

} //SetHandle()


//*---------------------------------------------------------------------------------
//| FormatHandle:
//|	This function will take the pointer of the given type and format a string
//|	suitable for display as a logged parameter or for dialog text.
//| Parms:
//|	szOut						User's return buffer
//|	fNull						TRUE if the handle choice is NULL
//|	uParmType				The parameter type
//|	szFieldName				Used to format the prompt/output text
//|	phenv						Pointer to environment handle, NULL if not allocated
//|	phdbc						Pointer to connection handle...
//|	pstmt						Pointer to statement handle...
//|	uNum						Hstmt number if applicable
//|	hActHandle				Actual handle that was used in API call, In the case of
//|								invalid handle testing
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN FormatHandle(LPTSTR szOut, BOOL fNull, UDWORD uParmType,
				LPTSTR szFieldName,
				HENV * phenv, HDBC * phdbc,
				HSTMT * phstmt, SQLHDESC * phdesc,
				UINT uNum,SQLHANDLE *phActHandle)
{
	BOOL			fShowNum=FALSE;
	SQLHANDLE	handle=NULL;
	BOOL			fDisable=FALSE;
	BOOL			fValid=FALSE,							// TRUE indicates use of 'VALID'
					fHdesc=FALSE,
					fHenv=FALSE,
					fHdbc=FALSE,
					fHstmt=FALSE,
					fHandle=FALSE,
					fPtr=FALSE;

	// For PT_xxx values, there are three possibilities:
	//		o Valid ODBC handle, format in hex
	//		o NULL
	//		o Valid output handle, VALID

	if (uParmType & PT_HDESC || uParmType & PT_MULT_HDESC)
		fHdesc=TRUE;
	else if (uParmType & PT_HENV)
		fHenv=TRUE;
	else if (uParmType & PT_HDBC)
		fHdbc=TRUE;
	else if (uParmType & PT_HSTMT)
		fHstmt=TRUE;
	else if (uParmType & PT_HOUTPUT)
		fPtr=TRUE;
	else if (uParmType & PT_HHANDLE)
		fHandle=TRUE;
	else
	{
		fPtr=TRUE;
	}

	if (!fNull)
	{
		if (fHdesc)
			handle=SetHandle(phdesc,phActHandle);
		else if (fHenv)
			handle=SetHandle(phenv,phActHandle);
		else if (fHdbc)
			handle=SetHandle(phdbc,phActHandle);
		else if (fHstmt)
		{
			if (handle=SetHandle(phstmt,phActHandle))
				fShowNum = TRUE;
		}
		else if (fHandle)
		{
			handle=SetHandle(NULL,phActHandle);
		}
		else if (fPtr)
		{
			fValid=TRUE;

			if (phActHandle)
				handle=phActHandle;

		}

		//if handle is null we need to disable handle control
		if (!handle && !fValid)
		{
			fDisable = TRUE;
			fNull = TRUE;
		}
	}


	// At this point, the following is true:
	//		o fNull indicates if we want NULL
	//		o If we have a valid handle, it is stored in handle
	//		o fShowNum contains TRUE if uNum should be part of the string
	*szOut = TEXT('\0');
	if(szFieldName)
		wsprintf(szOut, TEXT("%s "), (LPTSTR)szFieldName);

	if(fNull)
	{
		if (fHenv)
			lstrcat(szOut, TEXT("SQL_NULL_HENV"));
		else if (fHdbc)
			lstrcat(szOut, TEXT("SQL_NULL_HDBC"));
		else if (fHstmt)
			lstrcat(szOut, TEXT("SQL_NULL_HSTMT"));
		else if (fHdesc)
			lstrcat(szOut, TEXT("SQL_NULL_HDESC"));
		else
			lstrcat(szOut, TEXT("SQL_NULL_HANDLE"));
	}
	else if (handle)
		HandleToText(szOut,handle);
	else if (fValid)
		lstrcat(szOut, TEXT("VALID"));
	else
	{
		TCHAR szBuff[XLARGEBUFF]=TEXT("");

		szOut += lstrlen(szOut);
			HandleToText(szBuff,handle);

		// First set is for showing a number
		if(fShowNum)
			wsprintf(szOut, TEXT("#%u %s"), uNum, szBuff);
		else
			lstrcpy(szOut,szBuff);

	}

	return fDisable;
}


//*---------------------------------------------------------------------------------
//| UpdateCkBoxStatus:
//|	Changes the text of the combo box to match the new state passed in.
//| Parms:
//|	hDlg						Dialog window handle
//|	lpDlg						The dialog description
//|	lpCtl						Pointer to the control information
//|	lpParm					Pointer to the parameter information
//|	fState					TRUE if the check-box is set (NULL), FALSE else (VALID)
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN UpdateCkBoxStatus(HWND hDlg, lpDIALOGDESC lpDlg, lpCONTROL lpCtl,
			lpPARAMITEM lpParm, BOOL fState)
{
	BOOL						fDisable=FALSE;			// Change to TRUE if this parm can only be NULL
	BOOL						fShowNum=FALSE;			// TRUE only for valid allocated HSTMT
	TCHAR 						szTmp[50];
	UINT						num=0;
	lpCONNECTIONINFO		ci=lpDlg->ci;
	lpSTATEMENTINFO		lpStmt=NULL;
	HWND						hwnd = GetDlgItem(hDlg, lpCtl->wID);
	RECT						rect;
	HENV						henv=NULL;
	HDBC						hdbc=NULL;
	HSTMT						hstmt=NULL;
	SQLHDESC					hdesc=NULL;
	TCHAR						szHandleText[MEDBUFF]=TEXT("");
	LPTSTR					pColon=NULL;

	*szTmp = TEXT('\0');

	//set henv
	henv=GetActiveEnvHandle();

	if (ci)
	{
		hdbc=ci->hdbc;

		if (ci->lpCurStmt)
		{
			lpStmt=ci->lpCurStmt;
			hstmt=lpStmt->hstmt;

			if (lpStmt->lpARD)
				hdesc=lpStmt->lpARD->hdesc;

			num = lpStmt->uStmtNum;
		}
	}

	fDisable = FormatHandle(szTmp, fState, lpDlg->lpParms[lpCtl->iParmNum]->uParmType,
							lpCtl->szText,&henv,&hdbc,&hstmt,hdesc,num,NULL);

	// At this point, szTmp either has NULL or VALID, optionally a valid
	//		ODBC handle number
	GetWindowText(hwnd, szHandleText,MEDBUFF);

	// Search for ':' and truncate
	if (pColon=_tcschr(szHandleText, TEXT(':')))
	{
		lstrcpy(pColon+1,TEXT(" "));
		lstrcpy(pColon+2,szTmp);

		SetWindowText(hwnd, szHandleText);
		EnableWindow(hwnd, !fDisable);

		// Changing the text will erase the focus rect and cause problems later.
		//		Force the focus rect to be repainted to avoid this.
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);

		// If we had to disable the control but the user did not explicity choose NULL,
		//		then fNull must be set to TRUE so that NULL is passed correctly.  However,
		//		on the next call if there is a valid handle available, we do not want the
		//		NULL to carry over as the default.  Record this info now
		if(fDisable && !lpParm->fNull) {
			lpParm->fNull = TRUE;
			lpParm->dwDex = DEX_FORCED_NULL;				// Indicates fNull can be reversed on next init
			}
	}

	return TRUE;
}



//*---------------------------------------------------------------------------------
//| GetIndexFromControlID:
//|	Given the id of a control, this function will return the index of that
//|	control.
//| Parms:
//|	lpDlg						The dialog description
//|	uID						The id of the control to look for
//| Returns:
//|	The index.  If the return value is >= cCtlItems, then the item was not found.
//*---------------------------------------------------------------------------------
UINT INTFUN GetIndexFromControlID(lpDIALOGDESC lpDlg, UINT uID)
{
	UINT		dex;

	// Find out which control is it based on its ID
	for(dex=0;  dex<lpDlg->cCtlItems;  dex++)
		if(lpDlg->lpCtl[dex]->wID == uID)
			break;

	return dex;
}


//*---------------------------------------------------------------------------------
//| DoEditBoxUpdate:
//|	Handle an update to an edit box
//| Parms:
//|	fInit						TRUE if this is called on WM_INITDIALOG
//|	lpDlg						The dialog descriptor
//|	hDlg						Dialog window handle
//|	lpCtl						Control struct
//|	lpParm					Parameter struct
//|	wParam					From the message which caused this
//|	lParam					From the message which caused this
//|	uiCtlDex					Index for the control
//| Returns:
//|	TRUE if we have handled everything, FALSE if caller should break for
//|		default behavior
//*---------------------------------------------------------------------------------
BOOL INTFUN DoEditBoxUpdate(BOOL fInit, lpDIALOGDESC lpDlg, HWND hDlg,
					lpCONTROL lpCtl, lpPARAMITEM lpParm,
					WPARAM wParam, LPARAM lParam,
					UINT uiCtlDex)
{
	DWORD				dwLen;
	HWND				hwndCtl=GetDlgItem(hDlg, lpCtl->wID);
	HWND				hwndLenCtl=GetDlgItem(hDlg, lpCtl->wLengthID);
	LPTSTR				str = NULL;
	TCHAR				szFieldName[MAXEXPANDEDCONSTANT];
	TCHAR				szTmp[20];

	// Calculated length is done when user updates and edit field
	//	and tabs off
	if(lpCtl->lDataStyle & EDT_CALCULATED_LEN &&
		IsWindowEnabled(hwndLenCtl))
	{
		// For var length data, find length
		if(IsVarLengthDataType(lpParm->fCType)) //Bug #361
		{
			str = GetEditWindowString(hwndCtl,&dwLen,WM_GETTEXT);
			if(!str)
				return FALSE;

			if (IsHexString(str))
			{
				lstrcpy(szFieldName, lpParm->szName);
				RemoveDlgFormat(szFieldName);
	      	if(!IsValidHex(lpParm->fCType, str,
	      				TRUE, (LPTSTR)szFieldName, &dwLen))
	      		return FALSE;
	     	}
			else
			{
					--dwLen;										// Remove count of the NULL
			}
		}
		else
			// Fixed width is always the same
			dwLen = (DWORD)GetSizeOfCType(lpParm->fCType, sizeof(TIMESTAMP_STRUCT));

		wsprintf(szTmp, TEXT("%lu"), dwLen);
		UpdateLengthItem(hDlg, lpCtl->wLengthID, szTmp,lpParm->fCType,FALSE);
		FREE_PTR(str);
	}

    return TRUE;
}

//*---------------------------------------------------------------------------------
//| HandleControlMessage:
//|	For user-defined controls, this function handles a WM_COMMAND message.
//| Parms:
//|	hDlg						Dialog window handle
//|	lpDlg						The dialog description
//|	msg						Message from DlgProc
//|	wParam					from DlgProc
//|	lParam					from DlgProc
//| Returns:
//|	TRUE if we have handled everything, FALSE if caller should break for
//|		default behavior
//*---------------------------------------------------------------------------------
BOOL INTFUN HandleControlMessage(HWND hDlg, lpDIALOGDESC lpDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	UINT					uUsrMsg = 0;
	WORD 					id   = GET_WM_COMMAND_ID(wParam, lParam);
	WORD 					code = GET_WM_COMMAND_CODE(wParam, lParam);
	HWND 					hwnd = GET_WM_COMMAND_HWND(wParam, lParam);
	lpCONTROL			lpCtl;
	lpPARAMITEM			lpParm;
	UINT 					uiCtlDex;
	static TCHAR			szText[MAXSTATTEXT];

	// Search of the control item based on the control id
	uiCtlDex = GetIndexFromControlID(lpDlg, id);

	//Not one of our controls
	if(uiCtlDex >= lpDlg->cCtlItems)
		return FALSE;

	lpCtl = lpDlg->lpCtl[uiCtlDex];
	lpParm = lpDlg->lpParms[lpCtl->iParmNum];

	// Based on type, figure out what to do
	switch(lpCtl->type)
	{
		// Getting WM_COMMAND for a check box means that the check status has changed,
		//		therefore we need to change the text
		case DLG_CKBOX:
			UpdateCkBoxStatus(hDlg, lpDlg, lpCtl, lpParm, IsDlgButtonChecked(hDlg, lpCtl->wID));
			break;

		// For an edit box, just change the select text when we're notified of a setfocus
		//		therefore we need to change the text
		case DLG_EDIT:
			switch(code)
			{
				//
				// When a change message is received, simply record the fact.  This
				//		will trigger a update to be handled when focus is lost
				//
				case EN_UPDATE:
				case EN_CHANGE:
					lpCtl->fUpdated = TRUE;							// Mark field as udpated

					if(lpCtl->lpUsrWndProc)							// User has their own WndProc
						(*lpCtl->lpUsrWndProc)(hDlg, USER_EN_UPDATE_CONTENTS,
								wParam, lParam, (lpVOID)lpDlg, uiCtlDex);
					break;

				//
				// Losing focus in an edit window is only important if the contents have
				//		been updated.  Action is to call user defined WndProc if it exists
				//		for the control.
				//
				case EN_KILLFOCUS:
				{
					if(lpCtl->fUpdated)
					{
						lpCtl->fUpdated = FALSE;
						DoEditBoxUpdate(FALSE, lpDlg, hDlg, lpCtl, lpParm, wParam,
										lParam, uiCtlDex);

						if(lpCtl->lpUsrWndProc)							// User has their own WndProc
							(*lpCtl->lpUsrWndProc)(hDlg, USER_EN_KILL_FOCUS,
									wParam, lParam, (lpVOID)lpDlg, uiCtlDex);
					}
				}
				break;
			}
			break;


		// Combo box notification messages must do things like check for
		//		calculated lengths, search for keywords, etc...
		case DLG_COMBO:
		{
			switch(code)
			{
				//
				// We'll use this message to adjust the size of the drop-down.
				//		If user has their own text in the edit box, then remember
				//		this for later so we can replace it
				//
				case CBN_DROPDOWN:
				{
					ONDLGDEBUG(OutputDebugString(TEXT("CBN_DROPDOWN\r\n")));
					AdjustDropDownBox(GetDlgItem(hDlg, lpCtl->wID));
					if(SendDlgItemMessage(hDlg, lpCtl->wID, CB_GETCURSEL, 0, 0L) ==
						CB_ERR)
						lpCtl->fUpdated = DROPPED_RESET;
				}
				break;

				//
				// When a change message is received, simply record the fact.  This
				//		will trigger a update to be handled when focus is lost
				//
				case CBN_EDITCHANGE:
				case CBN_EDITUPDATE:
					lpCtl->fUpdated = TRUE;					// Mark field as udpated
					break;

				//
				// When we loose focus, check for properties which must be handled
				//		such as linked fields and pattern matchin
				//
				case CBN_KILLFOCUS:
				{
					DWORD			dwLen;
					LPTSTR		szSave;
						HWND 			hwnd = GetDlgItem(hDlg, lpCtl->wID);
					BOOL			fHandledMsg=FALSE;

					ONDLGDEBUG(OutputDebugString(TEXT("CBN_KILLFOCUS\r\n")));

					// Only care if there was an update
					if(!lpCtl->fUpdated)
					{
						// Now call the UserWndProc if one exists, sending it the killfocus message
						if(lpCtl->lpUsrWndProc)							// User has their own WndProc
							fHandledMsg=(*lpCtl->lpUsrWndProc)(hDlg, USER_CBN_KILL_FOCUS,
									wParam, lParam, (lpVOID)lpDlg, uiCtlDex);
						if (fHandledMsg)
							return(FALSE);
					}

					// Attempt to update the combo-box.  Unfortunately, there is no way for
					//		us to keep focus on control, since pressing the Cancel button will
					//		cause this function to be executed and you may never cancel
					if(!UpdateComboBox(TRUE,FALSE, hDlg, lpDlg, lpCtl, lpParm, wParam, lParam, uiCtlDex))
						return FALSE;

					// If the combo-box was dropped, then the current selection could
					//		have been changed to a value that would not show user text.
					//		This state is trapped with DROPPED_RESET, so if this happens
					//		remove current selection and put user text back in the edit box
					if(lpCtl->fUpdated == DROPPED_RESET)
					{
						HWND 		hwnd = GetDlgItem(hDlg, lpCtl->wID);

						dwLen = GetTrueWindowTextLength(hwnd,-1);
						if(dwLen)
						{
								if(szSave = (LPTSTR)AllocateMemory(dwLen + sizeof(TCHAR))) {
								GetText(hwnd, szSave);
								SendMessage(hwnd, CB_SETCURSEL, (WPARAM)-1, 0L);
								SetWindowText(hwnd, szSave);
							}
						}
					}

					lpCtl->fUpdated = FALSE;
				}

					// Now call the UserWndProc if one exists, sending it the killfocus message
					if(lpCtl->lpUsrWndProc)							// User has their own WndProc
						(*lpCtl->lpUsrWndProc)(hDlg, USER_CBN_KILL_FOCUS,
								wParam, lParam, (lpVOID)lpDlg, uiCtlDex);
					break;

				//
				// This message is recevied for both the case when the combo-box is
				//	dropped and when it is not.  If it is not dropped, then we need
				//	to do an update so that linked arrays get reset.  If it is dropped,
				//	however, we do not want to do an update yet or it causes problems
				//	for calculated length.
				//
				case CBN_SELCHANGE:
					{
					ONDLGDEBUG(OutputDebugString(TEXT("CBN_SELCHANGE\r\n")));
					if(lpCtl->fUpdated != DROPPED_RESET)
						UpdateComboBox(lpCtl->fUpdated, FALSE, hDlg, lpDlg,
									lpCtl, lpParm, wParam, lParam,
									uiCtlDex);
					else
						lpCtl->fUpdated = TRUE;
					}
					break;


#ifdef _DLGDEBUG
				case CBN_CLOSEUP:
					ONDLGDEBUG(OutputDebugString(TEXT("CBN_CLOSEUP\r\n")));
					break;

				case CBN_SELENDCANCEL:
					ONDLGDEBUG(OutputDebugString(TEXT("CBN_SELENDCANCEL\r\n")));
					break;

				case CBN_SELENDOK:
					ONDLGDEBUG(OutputDebugString(TEXT("CBN_SELENDOK\r\n")));
					break;
#endif

				default:
					break;
				}
         } // End of DLG_COMBO
         break;
		}

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| UpdateComboBox:
//|	This function will handle the default behavior required when the contents
//|	of a combo-box have been changed, either by changing the selection or by
//|	typing a new value.
//| Parms:
//|	fEditUpdated			TRUE if the field has been updated
//|	fInit						TRUE if being called for init purposes
//|	hDlg						Dialog window handle
//|	lpDlg						The dialog descriptor
//|	lpCtl						Control information
//|	lpParm					Parameter array for the control
//|	wParam					For user WndProc
//|	lParam					For user WndProc
//|	pdex						Control index for user WndProc
//| Returns:
//|	TRUE if we were successful, FALSE on failure
//*---------------------------------------------------------------------------------
BOOL INTFUN UpdateComboBox(BOOL fEditUpdated, BOOL fInit,
				HWND hDlg, lpDIALOGDESC lpDlg,
				lpCONTROL lpCtl, lpPARAMITEM lpParm,
				WPARAM wParam, LPARAM lParam, UINT pdex)
{
	HWND			hwndID = GetDlgItem(hDlg, lpCtl->wID);
	UDWORD		lDataStyle = lpCtl->lDataStyle;
	BOOL			fRtn=TRUE;
	BOOL			fChoice=FALSE;
	TCHAR			szText[MEDBUFF]=TEXT("0");
	DWORD			dwCurSel;
	DWORD			dwCount=0;
	BOOL			fCalcLen=lDataStyle & CBO_CALCULATED_LEN;


	//
	// Handle caculated length fields provided there is a link to work with
	//

	dwCount = lpParm->cDfts ? lpParm->cDfts : GetDftStyleCount(lDataStyle);

	if (fCalcLen && lpCtl->wLengthID)
	{
		// Do not update length when a CBO_DFT style was selected
		dwCurSel = (DWORD)SendMessage(hwndID, CB_GETCURSEL, 0, 0L);
		if (lpCtl->fUpdated != DROPPED_RESET &&
		 	 dwCurSel != CB_ERR && (dwCurSel < dwCount))
		{
			switch(IsChoiceCBODefault(hwndID, lpCtl, NULL))
			{
				// If these two are chosen (but not on init), then set length
				//		to zero
				case CBO_DFT_EMPTY:
				case CBO_DFT_NULL:
					if(!fInit) 									// Don't reset on init
						UpdateLengthItem(hDlg, lpCtl->wLengthID, szZERO,lpParm->fCType,FALSE);
					break;

				// For this case, the calculated length can be anything, so we will just leave it
				case CBO_DFT_ROWSET_VALUE:
					break;

				// For this case we'll calculate the length based on the selected text,
				//		always done even on init
				case CBO_DFT_INPUT_WIN:
					{
					DWORD		dsel, len;
					TCHAR		szTmp[30];
					HWND		hwnd = (lpDlg->ci) ? lpDlg->ci->hwndIn : hwndOutWin;
					LPTSTR		str;
					UDWORD	dwLen;
					UDWORD	cbChar=sizeof(TCHAR),
								cbCnt=UCHAR_BYTE_COUNT*cbChar;


					// Retrieve both text size and selection size
					len = GetTrueWindowTextLength(hwnd,-1);
					dsel = (DWORD)SendMessage(hwnd, EM_GETSEL, 0, 0L);

					if(HIWORD(dsel) != LOWORD(dsel))		// Selection override text size
						len = (HIWORD(dsel) - LOWORD(dsel))*sizeof(TCHAR);

					// Now get the text so that we can check for a hex string and calculate the length accordingly
					str = (LPTSTR)AllocateMemory(len + sizeof(TCHAR));
               if(str)
					{
						str = GetInputWindowString(hwnd, NULL);								// Get the string

						if(IsHexString(str))
						{
							// Calculate it's real character length
							if (fUnicode)
	               		IsValidHex(SQL_C_WCHAR, str, FALSE, NULL, &dwLen);
							else
	               		IsValidHex(SQL_C_CHAR, str, FALSE, NULL, &dwLen);
						}
	               else
	               	dwLen = len;

						wsprintf(szTmp, TEXT("%lu"), dwLen/cbCnt);
						UpdateLengthItem(hDlg, lpCtl->wLengthID, szTmp,lpParm->fCType,FALSE);
						FREE_PTR(str);
					}
					else
					{
						// If there was a problem allocating memory, use the string length
						wsprintf(szTmp, TEXT("%lu"), len);
						UpdateLengthItem(hDlg, lpCtl->wLengthID, szTmp,lpParm->fCType,FALSE);
					}
				}
				break;
			}
		}
		else if(!fInit)
		{
			//We only update length item if parameter is char data
			if (lpParm && IsCharData(lpParm->fCType))
			{
				// Don't proceede on init
				CalculateLength(hDlg, hwndID, lpCtl,lpParm);
			}
			// Substitue <empty string> for no text when option enabled
			if(lDataStyle & CBO_DFT_EMPTY)
			{
				// If there is no text in edit box, we'll want to use the special
				//	value of <empty string>.  Figure out its index based on other defaults
				if(!GetTrueWindowTextLength(hwndID,-1))
				{
					if(lDataStyle & CBO_DFT_INPUT_WIN)
						SendMessage(hwndID, CB_SETCURSEL, 1, 0L);
					else
						SendMessage(hwndID, CB_SETCURSEL, 0, 0L);
				}
			}
		}
	}


	//
	// Pattern matching will look for a constant based on a fragment (ignoring version numbers)
	//
	if(fEditUpdated &&									// Only required when user enters fragment
		lDataStyle & CBO_PATTERN_MATCHING) {
		static TCHAR  	szSearch[MAXEXPANDEDCONSTANT];
		static TCHAR		szVal[MAXEXPANDEDCONSTANT];
		lpPARAMITEM		lpParm=NULL;
		DWORD				dex = 0;
		DWORD				cnt;
		DWORD				dwMatch=(DWORD)-1;
		LPTSTR				str;
		int				len, iSearchLen;
		BOOL				fIntCheck=FALSE;
		SDWORD			fOpt, fItem;
		int				idx;

		// Find the lpParm based on control id
		if(lpCtl->iParmNum != -1)
			lpParm = lpDlg->lpParms[lpCtl->iParmNum];
		ASSERTCHECK((!lpParm), TEXT("lpParm must not be NULL!"));

		// Get user fragment
		*szSearch = TEXT('\0');
		GetDlgItemText(hDlg, lpCtl->wID, (LPTSTR)szSearch, tsizeof(szSearch));
		_tcsupr(szSearch);													// Ignore case


		// If number is an integer, then we must convert it and look at the
		//	item data for each item to match it up.
		if(IsValidInt(lpParm->fCType, szSearch, FALSE, NULL))
		{
			fIntCheck = TRUE;
			fOpt = (SDWORD)_ttol(szSearch);
		}

		// To do searching, we must have a parameter to look at,
		//	integers are handled on else, and it
		//	cannot be a hex constant as noted by "0X"
		if(lpParm)
		{
			// If the first character is '=', they're looking for a numeric constant
			//	so leave it intact. If not, then truncate search at '='
			if (!fIntCheck &&	*szSearch != TEXT('=') &&(str = _tcschr(szSearch, TEXT('='))))
				*str = TEXT('\0');

			iSearchLen = lstrlen(szSearch);

			cnt = (DWORD)SendMessage(hwndID, CB_GETCOUNT, 0, 0L);
			if(cnt != CB_ERR)
			{
				// Loop through each one.  Rules are:
				//		1)  If exact string constant match found, use it
				//		2)  If no exact, then use closest fragment
				// Start the loop after the default styles which don't have
				// a value (we don't want to pattern match to them)
				for(idx=0; idx<NumItems(DftComboBoxStyles); idx++)
				{
					if(DftComboBoxStyles[idx].fItem == 0 &&
						DftComboBoxStyles[idx].uDftType & lDataStyle)
						dex++;
				}

				for(;  dex<cnt;  dex++)
				{
					if(CB_ERR != SendMessage(hwndID, CB_GETLBTEXT,
							(WPARAM)dex, (LPARAM)(LPTSTR)szVal))
					{
						// Check integer case first.  The fOpt was stored with the item
						//	so retrieve and simply compare it
						if(fIntCheck)
						{
							fItem = (DWORD)SendMessage(hwndID, CB_GETITEMDATA,
											(WPARAM)dex, 0L);
							if(fItem == fOpt)
							{
								dwMatch = dex;								// Exact match found
								dex = cnt;
							}
						}
						// Now to non-integer check
						else
						{
							// Look for exact pattern match to start with
							_tcsupr(szVal);
							if(str = _tcschr(szVal, TEXT('=')))
							{
								// Only text constants count
								len = (int)(str - szVal);
								if(len == iSearchLen)
								{
									// Don't bother if not same size
									if(_tcsnccmp(szVal, szSearch, len) == 0)
									{
										// We have an exact constant match, use it
										dwMatch = dex;
										dex = cnt;										// Force end of loop
									}
								}
							}

							// No exact match, so look for partial match
							if(str = _tcschr(szVal, TEXT('(')))					// Ignore versions
								*str = TEXT('\0');

							// Search for fragment in current value
							if(dwMatch == (DWORD)-1 && iSearchLen)
								if(_tcsstr(szVal, szSearch))
									dwMatch = dex;								// Save in case no exact match found
							}		// non-integer match
						}  // valid char return
					}

					// We've gone through list, if there was an exact match, dwMatch has
					//		its index.  If no exact but a partial, dwMatch has index.
					if(dwMatch != (DWORD)-1){
						SendMessage(hwndID, CB_SETCURSEL, (WPARAM)dwMatch, 0L);
						lpCtl->fUpdated = FALSE;
						}
					else if(!fIntCheck) {
						lpCtl->fUpdated = FALSE;
						fRtn = FALSE;				// Make them specify a new value
						}
				}
			}
		}

	//
	// The user wnd proc must be executed after patern matching so that character
	//		strings used for pattern matching are not taken literally causing an error
	//
	if(lpCtl->lpUsrWndProc)							// User has their own WndProc
		(*lpCtl->lpUsrWndProc)(hDlg, USER_CBN_UPDATE_CONTENTS,
				wParam, lParam, (lpVOID)lpDlg, pdex);


	//
	// Check for a linked combo box which must be updated (example: Set/GetConnectOption)
	//
	if(lDataStyle & CBO_LINKED_FIELD &&
		lpCtl->wLengthID)
	{
		DWORD					idex;
		HWND					hLink=GetDlgItem(hDlg, lpCtl->wLengthID);
		lpCONTROL			lpLinkedCtl;
		lpPARAMITEM			lpLinkedParm;
		UINT					ldex;
		TCHAR					szDftText[MAXEXPANDEDCONSTANT];
		BOOL					found= FALSE;
		DWORD					ndex =0;




		// Find the linked control
		ldex = GetIndexFromControlID(lpDlg, lpCtl->wLengthID);
		lpLinkedCtl = lpDlg->lpCtl[ldex];
		lpLinkedParm = lpDlg->lpParms[lpLinkedCtl->iParmNum];


		//fixed bug # 3008,get the default value in the linked field
		if ( CB_ERR != (ndex = (DWORD)SendMessage(hLink,CB_GETCURSEL,0,0L)) )
			SendMessage(hLink,CB_GETLBTEXT, (WPARAM)ndex,(LPARAM)(LPCSTR)szDftText);


		SendMessage(hLink, CB_RESETCONTENT, 0, 0L);
		idex = (DWORD)SendMessage(hwndID, CB_GETCURSEL, 0, 0L);
		if(idex != CB_ERR)
		{
			// There is a selection to look at
			// Use the current selection to find the linked array of defaults
			lpDFTARRAY			lpDft=lpParm->lpDfts[idex].lpLink;
			UINT					dex;

			// If there is a default array
			if(lpDft)
			{
				UINT				cLinks=lpParm->lpDfts[idex].cLinks;
				DWORD				dwDex;
				static TCHAR	szTmp[MAXEXPANDEDCONSTANT];

				// Don't add on startup, since control will init with list.
				if(!fInit)
				{
					for(dex=0;  dex<cLinks;  dex++)
					{
						// Add both the constant and its value for pattern matching purposes
						GetExpandedConstant(szTmp, &lpDft[dex], lpUsrOptions->fVersionNumbers);
						dwDex = (DWORD)SendMessage(hLink, CB_ADDSTRING,0, (LPARAM)(LPTSTR)szTmp);
						SendMessage(hLink, CB_SETITEMDATA,(WPARAM)dwDex,(LPARAM)lpDft[dex].fOpt);

						// bug # 3008 if the defaut string is in the linked field highlight it.
						if(CB_ERR != ndex)
						{
							if (szTmp && szDftText && !lstrcmp((LPCTSTR)szTmp,(LPCTSTR)szDftText))
								if(CB_ERR != SendMessage(hLink,CB_SETCURSEL,(WPARAM)ndex,0L) )
									found = TRUE;
						}

					}

					if (!found)
						SendMessage(hLink, CB_SETCURSEL, 0, 0L);

				}

				// Given the linked array, associate it with the linked control
				lpLinkedParm->lpDfts = lpDft;
				lpLinkedParm->cDfts = cLinks;

				// Default array, pattern matching is done
				lpLinkedCtl->lDataStyle |= CBO_PATTERN_MATCHING;
			}
			else
			{
				// No array, so find a suitable value
				if(ldex < lpDlg->cCtlItems)
				{
					// Valid control
					lpCtl = lpDlg->lpCtl[ldex];						// Linked control structure
					SetGenericDefaultByType(hLink, lpDlg->lpParms[(lpCtl->iParmNum)]->fCType);
				}

				// There is no linked array
				lpLinkedParm->lpDfts = NULL;
				lpLinkedParm->cDfts = 0;

				// No default array, no pattern matching
				lpLinkedCtl->lDataStyle &= ~CBO_PATTERN_MATCHING;
			}
		}
		// There is no selection to look at, probably meaning a driver defined value.
		//		so pattern matching won't be required.
		else
			lpLinkedCtl->lDataStyle &= ~CBO_PATTERN_MATCHING;

	}


	return fRtn;
}


//*---------------------------------------------------------------------------------
//| CalculateLength:
//|	Causes the related item's value to be set as the calculated length of
//|	an item which has been changed.
//| Parms:
//|	hDlg						Dialog window handle
//|	hwndCtl					Window handle of control
//|	lpCtl						Control information
//| Returns:
//|	TRUE if we were successful, FALSE on failure
//*---------------------------------------------------------------------------------
#define UCHAR_BYTE_COUNT 1
BOOL INTFUN CalculateLength(HWND hDlg, HWND hwndCtl, lpCONTROL lpCtl,lpPARAMITEM lpParm)
{
	UDWORD	dwDex=0;
	TCHAR		szTmp[SMALLBUFF];
	UDWORD	dwLen=0;
	LPTSTR	str=NULL;
	UINT		uMsg=!lpCtl->fUpdated ? CB_GETLBTEXTLEN : WM_GETTEXT;
	UDWORD	cbChar=sizeof(TCHAR),
				cbCnt=UCHAR_BYTE_COUNT*cbChar;




	if (!(str=GetEditWindowString(hwndCtl,&dwLen,uMsg)))
	{
		ASSERTCHECK(str,TEXT("CalculateLength: str == NULL"));
		return(FALSE);
	}

	if(IsHexString(str))
	{
		// Calculate it's real character length
		if (fUnicode)
	        IsValidHex(SQL_C_WCHAR, str, FALSE, NULL, &dwLen);
		else
			IsValidHex(SQL_C_CHAR, str, FALSE, NULL, &dwLen);
	}

	wsprintf(szTmp, TEXT("%lu"), dwLen/cbCnt);
	UpdateLengthItem(hDlg, lpCtl->wLengthID, szTmp,lpParm->fCType,FALSE);

	return(TRUE);

}

//*---------------------------------------------------------------------------------
//| UpdateLengthItem:
//|	Update the length value which is stored as the last item in the combo
//|	box whose handle is passed in.
//| Parms:
//|	hwnd						Combo-box window handle
//|	wID						ID of parameter
//|	szBuff					The character string to show as length
//|	fCType					C type of parameter
//|	fNTS						TRUE if NTS needs to be displayed
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN UpdateLengthItem(HWND hwnd, WORD wID,LPCTSTR szBuff,SWORD fCType,BOOL fNTS)
{
	DWORD		dwCount=0,
				dwDex=0,
				dwFind=0;
	SDWORD	sdwData=0;
	SDWORD	dwLen=_ttol(szBuff);
	TCHAR		szFindString[LARGEBUFF]=TEXT("");
	HWND		hwndCtl=GetDlgItem(hwnd,wID);

	//if the current cbValue has been set to SQL_NTS (default)
	//then we still want to add the real length of the string
	//to the cbValue Combobox but we want the current selection
	//to remain SQL_NTS.
	dwDex=(DWORD)SendMessage(hwndCtl, CB_GETCURSEL, 0L,0L);
	sdwData=(DWORD)SendMessage(hwndCtl, CB_GETITEMDATA, dwDex,0L);

 	dwCount= (DWORD)SendMessage(hwndCtl, CB_GETCOUNT, 0L,0L);
	dwFind = (DWORD)SendMessage(hwndCtl,CB_GETLBTEXT,dwCount-1,(LPARAM)(LPTSTR)szFindString);

	if (!_tcsstr(szFindString,TEXT("SQL")))
		SendMessage(hwndCtl, CB_DELETESTRING, (WPARAM)dwCount-1, 0L);

	dwFind = (DWORD)SendMessage(hwndCtl, CB_FINDSTRING, (WPARAM)-1, (LPARAM)(LPTSTR)szBuff);

	if (dwFind != CB_ERR)
		SendMessage(hwndCtl, CB_DELETESTRING, (WPARAM)dwFind, 0L);
	else
		dwFind=0;

	dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szBuff);

	SendMessage(hwndCtl, CB_SETITEMDATA,(WPARAM)dwDex, (LPARAM)dwLen);

	//Don't select new item if it is currently SQL_NTS
	if (!(sdwData ==SQL_NTS || fNTS))
		SendMessage(hwndCtl, CB_SETCURSEL, (WPARAM)dwDex, 0L);

}


//*---------------------------------------------------------------------------------
//| SetGenericDefaultByType:
//|	This function will set a valid default based on the type.
//| Parms:
//|	hwnd						Window handle
//|	fCType					The type to use
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN SetGenericDefaultByType(HWND hwnd, SWORD fCType)
{
	switch(fCType) {
		case SQL_C_SHORT:
		case SQL_C_SSHORT:
		case SQL_C_USHORT:
		case SQL_C_LONG:
		case SQL_C_SLONG:
		case SQL_C_ULONG:
		case SQL_C_TINYINT:
		case SQL_C_UTINYINT:
		case SQL_C_STINYINT:
		case SQL_C_BIT:
		case SQL_C_GUID:
			SetWindowText(hwnd, (LPTSTR)szZERO);
			break;

		case SQL_C_FLOAT:
		case SQL_C_DOUBLE:
		case SQL_C_NUMERIC:
		case SQL_C_UBIGINT:
		case SQL_C_SBIGINT:
			SetWindowText(hwnd, (LPTSTR)szZEROPOINTZERO);
			break;

		case SQL_C_CHAR:
		case SQL_C_WCHAR:
			SetWindowText(hwnd, (LPTSTR)szEMPTYSTR);
			break;

		case SQL_C_BINARY:
			SetWindowText(hwnd, (LPTSTR)szBINARYSTR);
			break;

		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
			SetWindowText(hwnd, (LPTSTR)szDFTDATE);
			break;

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
			SetWindowText(hwnd, (LPTSTR)szDFTTIME);
			break;

		case SQL_C_INTERVAL_YEAR:
		case SQL_C_INTERVAL_YEAR_TO_MONTH:
		case SQL_C_INTERVAL_MONTH:
		case SQL_C_INTERVAL_DAY:
		case SQL_C_INTERVAL_DAY_TO_HOUR:
		case SQL_C_INTERVAL_DAY_TO_MINUTE:
		case SQL_C_INTERVAL_DAY_TO_SECOND:
		case SQL_C_INTERVAL_HOUR:
		case SQL_C_INTERVAL_HOUR_TO_MINUTE:
		case SQL_C_INTERVAL_HOUR_TO_SECOND:
		case SQL_C_INTERVAL_MINUTE:
		case SQL_C_INTERVAL_MINUTE_TO_SECOND:
		case SQL_C_INTERVAL_SECOND:
		case SQL_C_TIMESTAMP:
		case SQL_C_TYPE_TIMESTAMP:
			SetWindowText(hwnd, (LPTSTR)szDFTTIMESTAMP);
			break;

		default:
			ONDEBUG(wsprintf(szOutBuff, szInvalidFCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}
}




//*---------------------------------------------------------------------------------
//| IsChoiceCBODefault:
//|	This function gets the value from a cb control and determines whether it is
//|	any of the default (defined) CBO_DFT_xxx values.
//| Parms:
//|	hwnd						The handle of the control
//|	lpCtl						The control descriptor
//|	lpParm					pointer to record of current Paramter
//| Returns:
//|	CBO_DFT_xxx if the user has selected a default value, else 0
//*---------------------------------------------------------------------------------
UDWORD INTFUN IsChoiceCBODefault(HWND hwnd, lpCONTROL lpCtl,lpPARAMITEM	lpParm)
{
	SWORD			sdex;
	SWORD			dex, adex,
					sNumItems=0;
	UDWORD		tmpflag = lpCtl->lDataStyle;			// Only calculate offset once
	LPTSTR		lpstr=NULL;
	DWORD			dwLen=0;
	SDWORD		sdData=0;

	lpstr=GetEditWindowString(hwnd,&dwLen,WM_GETTEXT);

	sdex = (SWORD)SendMessage(hwnd, CB_GETCURSEL, 0, 0L);
	if (sdex != CB_ERR)
		sdData=(DWORD)SendMessage(hwnd, CB_GETITEMDATA, sdex, 0L);

	// Store the index
	if(lpParm)
		lpParm->dwDex = sdex;

	if(sdex == CB_ERR)
		return 0;

	switch(sdData)
	{
		case SQL_NTS:
			return(CBO_DFT_SQL_NTS);
		case SQL_NULL_DATA:
			return(CBO_DFT_NULL_DATA);
		case SQL_DATA_AT_EXEC:
			return(CBO_DFT_DATA_AT_EXEC);
		case NULLPTR:
			return(CBO_DFT_NULL);
		case EMPTYSTR:
			return(CBO_DFT_EMPTY);
		case VALIDPTR:
			return(CBO_DFT_VALID_POINTER);
		case SQL_DEFAULT_PARAM:
			return(CBO_DFT_DEFAULT_PARAM);
		case ROWSETVAL:
			return(CBO_DFT_ROWSET_VALUE);
		case INTERVALVAL:
			return(CBO_DFT_INTERVAL_VALUE);
	}

	//This was added so the gator can support the adding
	//of <null pointer> to individual items such as
	//SQL_ACCESS_MODE
	if (_tcsstr(lpstr,szNULLPTR))
		return(CBO_DFT_NULL);
	else if (_tcsstr(lpstr,szVALIDPTR))
		return(CBO_DFT_VALID_POINTER);

	//the following need special processing because a string
	//is displayed to the user but we only need the actual
	//descriptor handle
	if (!_tcsncmp(lpstr,TEXT("hdesc:"),_tcsclen(TEXT("hdesc:"))))
			return(CBO_DFT_ODBC_HANDLE);


	// Go through the style array used to populate default values and
	//		look for those values which are present in control to find
	//		the value which corresponds
	sNumItems=NumItems(DftComboBoxStyles);

	for(dex=0, adex=0; dex < sNumItems;  dex++)
	{
		if(tmpflag & DftComboBoxStyles[dex].uDftType)
		{
			if ((DftComboBoxStyles[dex].uDftType == CBO_DFT_ODBC_HANDLE) ||
				 (adex++ == sdex))
				return DftComboBoxStyles[dex].uDftType;
		}
	}

	FREE_PTR(lpstr);

	return 0;
}



//*---------------------------------------------------------------------------------
//| GetDftStyleCount:
//|	Based on a data style, returns the count of reserved default types in the
//|	control.
//| Parms:
//|	UDWORD					uStyle
//| Returns:
//|	Count of reserved types
//*---------------------------------------------------------------------------------
UINT INTFUN GetDftStyleCount(UDWORD uStyle)
{
	short int	cnt=0;
	UINT			dex;
	for(dex=0;  dex<NumItems(DftComboBoxStyles);  dex++)
		if(DftComboBoxStyles[dex].uDftType & uStyle)
			++cnt;
	return cnt;
}


//*---------------------------------------------------------------------------------
//| GetExpandedConstant:
//|	Given a default value structure, this function will format the value
//|	and the name of the constant and return it in the user's buffer based
//|	on type.
//| Parms:
//|	szOut						Output for the string
//|	lpDft						Pointer to the default array value
//|	fAddVersion				TRUE if version should be added to constant
//| Returns:
//|	Pointer to szOut
//*---------------------------------------------------------------------------------
LPTSTR INTFUN GetExpandedConstant(LPTSTR szOut, lpDFTARRAY lpDft, BOOL fAddVersion)
{
	TCHAR		szFmt[25];
	SWORD		type = lpDft->fCType;
	BOOL		fHandle=	lpDft->uValInfo & PRM_HWND ||
							lpDft->uValInfo & PRM_HENV ||
							lpDft->uValInfo & PRM_HDBC ||
							lpDft->uValInfo & PRM_HSTMT ||
							lpDft->uValInfo & PRM_HDESC;


	// First check if it is a HWND (only used by SetConnectOption - SQL_QUIET_MODE currently)
	if ((lpDft->fOpt == EMPTYSTR) ||
		(lpDft->fOpt == INTERVALVAL) ||
		(lpDft->fOpt == ROWSETVAL) ||
		(lpDft->fOpt == NULLPTR) ||
		(lpDft->fOpt == VALIDPTR) ||
		(lpDft->fOpt == SAMEAS_STRLEN))
	{
		wsprintf(szOut, TEXT("%s"), lpDft->szOpt);
	}
	else
	{
		// Check for special format condition required for Get/SetConnectOptions
		if(lpDft->uValInfo & PRM_CONN_PREFIX)
			lstrcpy(szFmt, TEXT("Conn: "));
		else if(lpDft->uValInfo & PRM_STMT_PREFIX)
			lstrcpy(szFmt, TEXT("Stmt: "));
		else
			szFmt[0] = TEXT('\0');

		// Added the constant name and value, followed by ODBC version
		switch(type) {
			case SQL_C_USHORT:
				lstrcat(szFmt, TEXT("%s=%u"));
				wsprintf(szOut, szFmt, (LPTSTR)lpDft->szOpt, (UWORD)lpDft->fOpt);
				break;

			case SQL_C_SSHORT:
				lstrcat(szFmt, TEXT("%s=%d"));
				wsprintf(szOut, szFmt, (LPTSTR)lpDft->szOpt, (SWORD)lpDft->fOpt);
				break;

			case SQL_C_ULONG:
				lstrcat(szFmt, TEXT("%s=%lu"));
				wsprintf(szOut, szFmt, (LPTSTR)lpDft->szOpt, (UDWORD)lpDft->fOpt);
				break;

			case SQL_C_SLONG:
				lstrcat(szFmt, TEXT("%s=%ld"));
				wsprintf(szOut, szFmt, (LPTSTR)lpDft->szOpt, (SDWORD)lpDft->fOpt);
				break;

			default:
				ONDEBUG(wsprintf(szOutBuff, szInvalidFCType, type));
				ASSERTCHECK(TRUE, szOutBuff);
				break;
			}

		// Add version if desired
		if(fAddVersion)
		{
			wsprintf(szFmt, TEXT("  (%u.%u)"), (UWORD)LOWORD(lpDft->uVersion), (UWORD)HIWORD(lpDft->uVersion));
			lstrcat(szOut, szFmt);
		}
	}

	return szOut;
}


//*---------------------------------------------------------------------------------
//| GetCTypeFromSqlType:
//|	This function returns the equivalent C type of a given Sql type.
//| Parms:
//|	fSqlType			The Sql type to convert
//|	fSigned			Is column signed or unsigned
//| Returns:
//|	The fCType if valid fSqlType, else SQL_C_DEFAULT
//*---------------------------------------------------------------------------------
SWORD INTFUN GetCTypeFromSqlType(SWORD fSqlType,BOOL fSigned)
{

	// No pointer involved, so simply record generic info
	switch(fSqlType) {
		case SQL_CHAR:
		case SQL_VARCHAR:
		case SQL_LONGVARCHAR:
		case SQL_DECIMAL:
			return SQL_C_CHAR;

		case SQL_NUMERIC:
			return SQL_C_CHAR;

		case SQL_BIGINT:
			return (fSigned ? SQL_C_SBIGINT : SQL_C_UBIGINT);

		case SQL_WCHAR:
		case SQL_WVARCHAR:
		case SQL_WLONGVARCHAR:
			return SQL_C_WCHAR;

		case SQL_BIT:
			return SQL_C_BIT;

		case SQL_TINYINT:
			return (fSigned ? SQL_C_STINYINT : SQL_C_UTINYINT);

		case SQL_SMALLINT:
			return (fSigned ? SQL_C_SSHORT : SQL_C_SSHORT);

		case SQL_INTEGER:
			return (fSigned ? SQL_C_SLONG : SQL_C_ULONG);

		case SQL_REAL:
			return SQL_C_FLOAT;

		case SQL_FLOAT:
		case SQL_DOUBLE:
			return SQL_C_DOUBLE;

		case SQL_GUID:
			return SQL_C_GUID;

		case SQL_BINARY:
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:
			return SQL_C_BINARY;

		case SQL_DATE:
		case SQL_TYPE_DATE:
			return SQL_C_DATE;

		case SQL_TIME:
		case SQL_TYPE_TIME:
			return SQL_C_TIME;

		case SQL_TIMESTAMP:
		case SQL_TYPE_TIMESTAMP:
			return SQL_C_TIMESTAMP;

		case SQL_INTERVAL_DAY:
			return SQL_C_INTERVAL_DAY;

		case SQL_INTERVAL_DAY_TO_HOUR:
			return SQL_C_INTERVAL_DAY_TO_HOUR;

		case SQL_INTERVAL_DAY_TO_MINUTE:
			return SQL_C_INTERVAL_DAY_TO_MINUTE;

		case SQL_INTERVAL_DAY_TO_SECOND:
			return SQL_C_INTERVAL_DAY_TO_SECOND;

		case SQL_INTERVAL_HOUR:
			return SQL_C_INTERVAL_HOUR;

		case SQL_INTERVAL_HOUR_TO_MINUTE:
			return SQL_C_INTERVAL_HOUR_TO_MINUTE;

		case SQL_INTERVAL_HOUR_TO_SECOND:
			return SQL_C_INTERVAL_HOUR_TO_SECOND;

		case SQL_INTERVAL_MINUTE:
			return SQL_C_INTERVAL_MINUTE;

		case SQL_INTERVAL_MINUTE_TO_SECOND:
			return SQL_C_INTERVAL_MINUTE_TO_SECOND;

		case SQL_INTERVAL_MONTH:
			return SQL_C_INTERVAL_MONTH;

		case SQL_INTERVAL_SECOND:
			return SQL_C_INTERVAL_SECOND;

		case SQL_INTERVAL_YEAR:
			return SQL_C_INTERVAL_YEAR;

		case SQL_INTERVAL_YEAR_TO_MONTH:
			return SQL_C_INTERVAL_YEAR_TO_MONTH;

		default:
			ONDEBUG(wsprintf(szOutBuff, szCTypeNotFound, fSqlType));
			ASSERTCHECK(TRUE, szOutBuff);
			return SQL_C_DEFAULT;
		}
}

//Bug #318, Bug #323
//*---------------------------------------------------------------------------------
//| GetDisplaySizeBasedOnType:
//|	Return the correct display size based on the C Type.
//| Parms:
//|	fCType					The C type
//|	cbMax						The input size
//| Returns:
//|	The correct display length
//*---------------------------------------------------------------------------------
UDWORD INTFUN GetDisplaySizeBasedOnType(SWORD fCType, UDWORD cbMax)
{

	int cbChar=sizeof(TCHAR);

	switch(fCType) {
		case SQL_C_SHORT:
		case SQL_C_SSHORT:
			return(DS_SHORT+cbChar);

		case SQL_C_USHORT:
			return(DS_SHORT);

		case SQL_C_LONG:
		case SQL_C_SLONG:
			return(DS_LONG+cbChar);

		case SQL_C_ULONG:
			return(DS_LONG);

		case SQL_C_TINYINT:
		case SQL_C_STINYINT:
			return(DS_TINYINT+cbChar);

		case SQL_C_UTINYINT:
			return(DS_TINYINT);

		case SQL_C_BIT:
			return(DS_BIT+cbChar);

		case SQL_C_FLOAT:
			return(DS_FLOAT);

		case SQL_C_GUID:
			return DS_GUID;

		case SQL_C_DOUBLE:
			return(DS_DOUBLE);

		case SQL_C_CHAR:
		case SQL_C_WCHAR:
			return ((cbMax+1)*cbChar);
			break;

		case SQL_C_DEFAULT:
		case SQL_C_BINARY:
	  		return((cbMax * (2*cbChar)) + (3*cbChar));		// Two characters for every binary byte plus '0x'

		case SQL_C_UBIGINT:
		case SQL_C_SBIGINT:
			return(DS_BIGINT);

		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
   		return(DS_DATE);

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
			return(DS_TIME);

		case SQL_C_TIMESTAMP:
		case SQL_C_TYPE_TIMESTAMP:
			return(DS_TIMESTAMP);

		case SQL_C_NUMERIC:
			return(cbMax+(2*cbChar));

		case SQL_INTERVAL_YEAR:
		case SQL_INTERVAL_MONTH:
		case SQL_INTERVAL_YEAR_TO_MONTH:
		case SQL_INTERVAL_DAY:
		case SQL_INTERVAL_HOUR:
		case SQL_INTERVAL_MINUTE:
		case SQL_INTERVAL_SECOND:
		case SQL_INTERVAL_DAY_TO_HOUR:
		case SQL_INTERVAL_DAY_TO_MINUTE:
		case SQL_INTERVAL_DAY_TO_SECOND:
		case SQL_INTERVAL_HOUR_TO_MINUTE:
		case SQL_INTERVAL_HOUR_TO_SECOND:
		case SQL_INTERVAL_MINUTE_TO_SECOND:
			return(DS_INTERVAL_TYPE * cbChar);

		default:
			ONDEBUG(wsprintf(szOutBuff, szInvalidFCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			return cbMax;
		}

}


//*---------------------------------------------------------------------------------
//| GetSizeOfCType:
//|	Return the correct cbValueMax for the c type, which will be a fixed value
//|	for fixed size types, and the input value for varying length.
//| Parms:
//|	fCType					The C type
//|	cbMax						The input size
//| Returns:
//|	The correct length
//*---------------------------------------------------------------------------------
SIZE_T INTFUN GetSizeOfCType(SWORD fCType, SIZE_T cbMax)
{
	switch(fCType) {
		case SQL_C_SHORT:
		case SQL_C_SSHORT:
		case SQL_C_USHORT:
			return(sizeof(SWORD));

		case SQL_C_LONG:
		case SQL_C_SLONG:
		case SQL_C_ULONG:
			return(sizeof(SLONG));

		case SQL_C_TINYINT:
		case SQL_C_UTINYINT:
		case SQL_C_STINYINT:
			return(sizeof(STINYINT));

		case SQL_C_BIT:
			return(sizeof(UCHAR));

		case SQL_C_FLOAT:
			return(sizeof(SFLOAT));

		case SQL_C_GUID:
			return (sizeof(UUID));

		case SQL_C_DOUBLE:
			return(sizeof(SDOUBLE));

		case SQL_C_UBIGINT:
		case SQL_C_SBIGINT:
			return(sizeof(DWORDLONG));

		case SQL_C_DEFAULT:
		case SQL_C_CHAR:
		case SQL_C_BINARY:
		case SQL_C_WCHAR:
			return cbMax;

		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
   		return(sizeof(DATE_STRUCT));

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
			return(sizeof(TIME_STRUCT));

		case SQL_C_TIMESTAMP:
		case SQL_C_TYPE_TIMESTAMP:
			return(sizeof(TIMESTAMP_STRUCT));

		case SQL_C_NUMERIC:
			return(sizeof(SQL_NUMERIC_STRUCT));

		case SQL_INTERVAL_DAY:
		case SQL_INTERVAL_DAY_TO_HOUR:
		case SQL_INTERVAL_DAY_TO_MINUTE:
		case SQL_INTERVAL_DAY_TO_SECOND:
		case SQL_INTERVAL_HOUR:
		case SQL_INTERVAL_HOUR_TO_MINUTE:
		case SQL_INTERVAL_HOUR_TO_SECOND:
		case SQL_INTERVAL_MINUTE:
		case SQL_INTERVAL_MINUTE_TO_SECOND:
		case SQL_INTERVAL_MONTH:
		case SQL_INTERVAL_SECOND:
		case SQL_INTERVAL_YEAR:
		case SQL_INTERVAL_YEAR_TO_MONTH:
			return(sizeof(SQL_INTERVAL_STRUCT));

		default:
			ONDEBUG(wsprintf(szOutBuff, szInvalidFCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			return cbMax;
		}
}

//*---------------------------------------------------------------------------------
//| GetEditWindowString:
//|	Retrieve text from an edit window returning a pointer to the string
//|	as well as the length of the text including null terminator.
//| Parms:
//|	hwnd						Edit window handle
//|	pdwLen					Return the length of text
//| Returns:
//|	Pointer to the string if successful, else NULL
//*---------------------------------------------------------------------------------
LPTSTR INTFUN GetEditWindowString(HWND hwnd, DWORD * pdwLen,UINT uMsg)
{
	LPTSTR 		str;
	DWORD			dwLen;

	*pdwLen = 0;

	dwLen = GetTrueWindowTextLength(hwnd,-1);
	//dwLen+=(sizeof(TCHAR)*2);
	str = AllocateMemory(dwLen + sizeof(TCHAR) * 2);

	if(!str)
		return NULL;

	SendMessage(hwnd, uMsg,(WPARAM)dwLen, (LPARAM)(LPTSTR)str);
	*pdwLen = dwLen;
	return str;
}

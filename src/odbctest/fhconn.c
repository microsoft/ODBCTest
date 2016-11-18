//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHCONN.C
//|
//| Purpose:
//|		This module contains the function handlers for the connection menu.
//|		Note that each function can be found in the order that it appears
//|		on the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "resource.h"
#include "paramidx.h"
#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
#define DFT_UID_LEN					63


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
UINT									cConnects=0;						// Count of connections

TCHAR									szPWD[DFT_UID_LEN+1];

extern TCHAR							szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndFrame;							// Main window (for menu access)
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern lpDFTARRAY					lpdftGetInfo;
extern UINT							ciInfoTypes;
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern HMENU						hMenu;								// Menu handle for Trace item
extern LPTSTR				szTestHelpFile;
extern int					iApiHelpCnt, iTestHelpCnt;		// How many times have we called these?
extern lpHEADENV					lpHeadEnv;
extern TCHAR						szSQL_OV_ODBC3[];
extern TCHAR						szSQL_OV_ODBC2[];
extern TCHAR						szSQL_ATTR_ODBC_VERSION[];


dCSEG(TCHAR) szDSNStr[]							= TEXT("DSN=%s;");
dCSEG(TCHAR) szUIDPWD[]							= TEXT("UID=%s;PWD=%s;");
dCSEG(TCHAR) szFullConnect[]					= TEXT("Full Connect:\r\n");
dCSEG(TCHAR) szDataSourcesAll[]				= TEXT("\r\nData Sources All: \r\n");
dCSEG(TCHAR) szDataSourcesSub[]				= TEXT("\t\t\t\t\t\t\t\t\tszDSN, *pcbDSN, szDescription, *pcbDescription\r\n");
dCSEG(TCHAR) szDataSourcesItemStr[]			= TEXT("\t\t\t\t\t\t\t\t\t\t\t%s, %d, %s, %d\r\n");
dCSEG(TCHAR) szDriversAll[]					= TEXT("\r\nDrivers All: \r\n");
dCSEG(TCHAR) szDriversSub[]					= TEXT("\t\t\t\t\t\t\t\t\tszDriverDesc, *pcbDriverDesc, szDriverAttributes, *pcbDrvrAttr\r\n");
dCSEG(TCHAR) szDriversItemStr[]				= TEXT("\t\t\t\t\t\t\t\t\t\t\t%s, %d, %s, %d\r\n");
dCSEG(TCHAR) szGetInfoAll[]					= TEXT("\r\nGet Info All:\r\n");
dCSEG(TCHAR) szGetInfoSub[]					= TEXT("\t\t\t\t\t\t\t\t\tfInfoType, pcbInfoValue, rgbInfoValue \r\n");
dCSEG(TCHAR) szNineTab[]						= TEXT("\t\t\t\t\t\t\t\t\t");
dCSEG(TCHAR) szThreeTab[]						= TEXT("\t\t\t");

//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN GetConnectInfoWndProc(HWND hDlg, UINT msg,
						WPARAM wParam, LPARAM lParam);


//*---------------------------------------------------------------------------------
//| DoConnect:
//|	Call this function from SQLConnect, SQLDriverConnect, or SQLBrowseConnect
//|	when a successful connection has been made.  It will query the driver
//|	for connection information and update the window title.
//| Parms:
//|	ci			Current connection info. record
//|	lpEI		Current Environment info record
//|	szDSN		Data source name
//|	szUID		User ID
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoConnect(lpCONNECTIONINFO lpCI,lpENVIRONMENTINFO lpEI,
							 LPTSTR szDSN, LPTSTR szUID)
{
	TCHAR		szOut[MAXOUTBUFF]=TEXT(""),
				szUsr[MEDBUFF]=TEXT(""),
				szDataSource[MEDBUFF]=TEXT("");

	// Change our state and defaults
	lpCI->uState |= STATE_CONNECTED_HDBC;
	lpCI->uRowSetSize = SQL_ROWSET_SIZE_DEFAULT;

	GetGtrString(szOut, MAXOUTBUFF, idsConnected);

	if (!szUID)
		lstrcpy(szUsr,TEXT("<Unknown UID>"));
	else
		lstrcpy(szUsr,szUID);

	if (!szDSN)
		lstrcpy(szDataSource,TEXT("<Unknown DSN>"));
	else
		lstrcpy(szDataSource,szDSN);

	wsprintf((LPTSTR)lpCI->szClientTitle, szOut, lpEI->uEnvNum,
				lpCI->uConNum,szUsr,szDataSource);

	SetWindowText(lpCI->hwnd, (LPTSTR)lpCI->szClientTitle);

	lstrcpy(lpCI->szUser,szUsr);
	lstrcpy(lpCI->szServer,szDataSource);

}


//*---------------------------------------------------------------------------------
//| DoDisonnect:
//|	This function will set states and change the connection window title for
//|	a successful disconnect.
//| Parms:
//|	lpci						Current connection informaiton (always NULL)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoDisconnect(lpCONNECTIONINFO lpci)
{
	if(lpci->uState & STATE_CONNECTED_HDBC) {
		--cConnects;						// One less to track

		*lpci->szServer = TEXT('\0');
		wsprintf((LPTSTR)lpci->szClientTitle,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsUnconnected),
					lpci->uConNum);

		SetWindowText(lpci->hwnd, (LPTSTR)lpci->szClientTitle);

		// Free the supported function array
		FREE_PTR(lpci->puSupportedFunctions);
		}

	// Change our state
	lpci->uState &= ~STATE_CONNECTED_HDBC;
	lpci->uState &= ~STATE_BROWSING_HDBC;

	// Refresh the toolbar so that some buttons get disabled correctly
	RefreshToolbar();
}



//*---------------------------------------------------------------------------------
//| lpSQLAllocConnect:
//|	Allocate an connection handle.  A successfully allocated handle will cause
//|	a new Connection Window to appear.  If successful, then the output will
//|	be logged in the output window.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLAllocConnect(STD_FH_PARMS)
{
	RETCODE					rc;
	HDBC						hdbc=NULL;								// For temporary use
	lpCONNECTIONINFO		lpci=NULL;								// Can't use value passed in, need new window
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	HENV						henv=NULL;

	if (lpParms[0]->lpData)
		henv=*(HENV *)lpParms[0]->lpData;

	// Invoke the function first so that if successful we can log input and output in window
    rc = SQLAllocConnect(henv, lpParms[1]->fNull ? NULL : &hdbc);	// phdbc

	// If successful, create a new connection window for logging
	if (RC_SUCCESSFUL(rc))
	{
		lpci = CreateConnectionWindow(hwndClient, hInst,lpActiveEnv);

		// If successful, add the new connection to the list, else free memory and fail
		InitConnectionInfo(lpci,lpActiveEnv);

		if(lpci)
		{
			lpci->hdbc = hdbc;
			lpci->uState = STATE_ALLOCATED_HDBC;
		}

	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, lpci, TRUE);

	// Log return code
	LOGRETURNCODE(NULL, lpci, rc);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(lpci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORCI(lpci,rc,hdbc);
   AUTOLOGERROREI(lpci,lpActiveEnv,rc,henv);

	return rc;
}

//*---------------------------------------------------------------------------------
//| ExtractConnectionInfo:
//|	This function extracts the DSN and UID from szConnStrOut
//| Parms:
//|	szConnStrOut			completed connection string
//|	szDSN						extracted DSN
//|	szUID						extracted UID
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

VOID INTFUN ExtractConnectionInfo(LPTSTR szConnStrOut,LPTSTR szDSN,LPTSTR szUID)
{
	LPTSTR		pDSN=NULL,
					pUID=NULL,
					pSemiColon=NULL,
					szDSNSTR=TEXT("DSN"),
					szSERVERSTR=TEXT("Server"),
					szUIDSTR=TEXT("UID"),
					szSEMICOLON=TEXT(";"),
					pTok=NULL,
					pOut=NULL;
	TCHAR			chEOS=TEXT('\0');
	SWORD			cbDSN=0;

	ASSERTCHECK(szConnStrOut,TEXT("Invalid szConnStrOut"));
	if (!szConnStrOut)
		return;

	//Make local copy of connection string
	pOut=_tcsdup(szConnStrOut);

	//tokenize it
	pTok=_tcstok(pOut,TEXT(";="));

	//extract the DSN or Server and UID
	while (pTok)
	{
		if (!lstrcmp(pTok,szDSNSTR) ||
			!lstrcmp(pTok,szSERVERSTR))
		{
			if (pTok=_tcstok(NULL,TEXT(";=")))
				lstrcpy(szDSN,pTok);
		}
		else if (!lstrcmp(pTok,szUIDSTR))
		{
			if (pTok=_tcstok(NULL,TEXT(";=")))
				lstrcpy(szUID,pTok);
		}
		else
			pTok=_tcstok(NULL,TEXT(";="));

	}

} //ExtractConnectionInfo()


//*---------------------------------------------------------------------------------
//| lpSQLConnect:
//|	Connect to a server.  If SQL_SUCCESS is returned, this function will:
//|			o  Change state to STATE_CONNECTED_HDBC
//|			o  Get the user name
//|			o  Update the window title
//|			o  Get current flags and set status bar info
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLConnect(STD_FH_PARMS)
{
	RETCODE		rc;
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLConnect(hdbc,
						 (LPTSTR)lpParms[1]->lpData,		// szDSN
						 *(SWORD *)lpParms[2]->lpData,	// cbDSN
						 (LPTSTR)lpParms[3]->lpData,		// szUID
						 *(SWORD *)lpParms[4]->lpData,	// cbUID
						 (LPTSTR)lpParms[5]->lpData,		// szAuthStr
						 *(SWORD *)lpParms[6]->lpData);	// cbAuthStr

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	// If successful, update window with user name and server
	if(RC_SUCCESSFUL(rc))
		DoConnect(ci,ci->lpActiveEnv,
				(LPTSTR)lpParms[1]->lpData,		//szDSN
				(LPTSTR)lpParms[3]->lpData);		//szUID

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLBrowseConnect:
//|	Connect to a via several levels.  If this call is successful, then
//|	our state is changed to indicate we are browsing the connection.  See
//|	lpSQLConnect for related notes upon success.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLBrowseConnect(STD_FH_PARMS)
{
	RETCODE		rc;
	TCHAR			szDSN[MEDBUFF]=TEXT(""),
					szUID[MEDBUFF]=TEXT("");
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Build output parameters
	BUILDOUTPUTPARM(lpParms[3], (*(SWORD *)lpParms[4]->lpData) * (SDWORD)sizeof(TCHAR),
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLBrowseConnect(hdbc,
						 (LPTSTR)lpParms[1]->lpData,							// szConnStrIn
						 *(SWORD *)lpParms[2]->lpData,            // cbConnStrIn
						 (LPTSTR)lpParms[3]->lpData,                   // szConnStrOut
						 *(SWORD *)lpParms[4]->lpData,            // cbConnStrOutMax
						 (SWORD *)lpParms[5]->lpData);            // pcbConnStrOut


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,
					cParms, ci, FALSE);

	// If successful, update window with user name and server
	if(RC_SUCCESSFUL(rc) || rc == SQL_NEED_DATA)
	{
		// Change our state
		if(rc == SQL_NEED_DATA)
			ci->uState |= STATE_BROWSING_HDBC;
		else
		{									// No longer browsing, but connected
			ci->uState &= ~STATE_BROWSING_HDBC;
			ci->uState |= STATE_CONNECTED_HDBC;

			//extract szDSN and szUID
			ExtractConnectionInfo((LPTSTR)lpParms[3]->lpData,szDSN,szUID);

			DoConnect(ci,ci->lpActiveEnv,szDSN,szUID);
		}

		// Verify any truncation which occured
		TRUNCATIONCHECK(lpParms[3]->fNull, lpParms[5]->fNull,
					rc, GETOUTPUTWINDOW(ci),
					(LPTSTR)lpParms[3]->lpData,
					*(SWORD *)lpParms[4]->lpData, *(SWORD *)lpParms[5]->lpData,
					(LPTSTR)lpParms[5]->szName);
		}

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Check for Null termination
	CHECKNULLTERM(ci, lpParms[3], lpParms[5], TRUE);

	// Buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	return rc;
}



//*---------------------------------------------------------------------------------
//| lpSQLDriverConnect:
//|	Connect possibly interactively.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLDriverConnect(STD_FH_PARMS)
{
	RETCODE		rc;
	HWND			hwnd=GETOUTPUTWINDOW(ci);
	UINT			len=0,
					cbChar=sizeof(TCHAR);
	TCHAR			szDSN[MEDBUFF]=TEXT(""),
					szUID[MEDBUFF]=TEXT("");
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[4], 													// szConnStrOut
						(*(SWORD *)lpParms[5]->lpData) * (UWORD)sizeof(TCHAR), 		//	  based on cbConnStrOutMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLDriverConnect(hdbc,
						 (lpParms[1]->fNull) ? NULL : hwnd,				// hwnd
						 (LPTSTR)lpParms[2]->lpData,							// szConnStrIn
						 *(SWORD *)lpParms[3]->lpData,				// cbConnStrIn
						 (lpParms[4]->fNull) ? NULL : (LPTSTR)lpParms[4]->lpData,// szConnStrOut
						 *(SWORD *)lpParms[5]->lpData,				// cbConnStrOutMax
						 (SWORD *)lpParms[6]->lpData,					// pcbConnStrOut
						 *(UWORD *)lpParms[7]->lpData);				// fDriverCompletion

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// If successful, update window with user name and server
	if(RC_SUCCESSFUL(rc) )
	{
		//extract szDSN and szUID
		if(lpParms[4]->lpData )
			ExtractConnectionInfo((LPTSTR)lpParms[4]->lpData,szDSN,szUID);
		DoConnect(ci,ci->lpActiveEnv,szDSN,szUID);
	}

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[4], lpParms[6], TRUE);

	// Check szConnStrOut for null termination
	CHECKNULLTERM(ci, lpParms[4], lpParms[6], TRUE);

	// Buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// szConnStrOut will become szConnStrIn for next time
	if(RC_SUCCESSFUL(rc))
	{
		// Valid szConnStrOut and valid szConnStrIn
		if (lpParms[4]->lpData)
		{
			FREE_PTR(lpParms[2]->lpData);

			len=lstrlen(lpParms[4]->lpData)*cbChar;

			lpParms[2]->lpData = (LPTSTR)AllocateMemory(len+cbChar);
			lstrcpy((LPTSTR)lpParms[2]->lpData, (LPTSTR)lpParms[4]->lpData);
	  		lpParms[2]->dwDex = CB_ERR;
			// Update cbConnStrIn to the new length also
	  		*(SWORD *)lpParms[3]->lpData = (SWORD)len;
	  		lpParms[3]->dwDex = CB_ERR;
	  	}
	}

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLDisonnect:
//|	Disconnect from a server.  If SQL_SUCCESS is returned, this function will:
//|			o  Remove state of STATE_CONNECTED_HDBC
//|			o  Update the window title to reflect new status
//|			o  Get current flags and set status bar info
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLDisconnect(STD_FH_PARMS)
{
	RETCODE	rc;
	HDBC		hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLDisconnect(hdbc);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// If successful, update window with user name and server
	if(RC_SUCCESSFUL(rc))
	{
		DropAllStmts(ci, FALSE);
		DoDisconnect(ci);
	}

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	return rc;
}



//*---------------------------------------------------------------------------------
//| lpSQLFreeConnect:
//|	Free a connection handle.  If not successful, log the information to the
//|	connection window.  If successful, kill off the connection window.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLFreeConnect(STD_FH_PARMS)
{
	RETCODE					rc;
	TCHAR						szTmpAddr[30];
	lpENVIRONMENTINFO		lpEi=GetActiveEnvNode();
	WORD						wIndex=0;
	HDBC						hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Invoke the function first so that if not successful we can log input and output in window
	rc = SQLFreeConnect(hdbc);

	// If successful, then the window is going away
	if(rc == SQL_SUCCESS && ci)
	{
		DestroyConnectionWindow(ci);
		ci = NULL;
	}

	// Don't use normal logging, since we don't know what window to
	//		log to.
	if(lpParms[0]->fNull)
		lstrcpy(szTmpAddr, TEXT("NULL"));
	else
		wsprintf(szTmpAddr, ADDRESSINFO, GETADDRINFO(hdbc));
	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(ci), MSG_SEV0, TRUE, TRUE,
			GetGtrString(szOutBuff, MAXOUTBUFF, idsLogFreeConnect),
			(LPTSTR)szTmpAddr);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	return rc;
}





//*---------------------------------------------------------------------------------
//| lpSQLDataSources:
//|	Enumerate the data sources on the system.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLDataSources(STD_FH_PARMS)
{
	RETCODE					rc;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnvNode();
	SQLHENV					henv=NULL;

	if (lpParms[0]->lpData)
		henv=*(HENV *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 													// szDSN
						(*(SWORD *)lpParms[3]->lpData) * (UWORD)sizeof(TCHAR), 		//	  based on cbDSNMax
						lpUsrOptions->fBufferChecking);
	BUILDOUTPUTPARM(lpParms[5], 													// szDescription
						(*(SWORD *)lpParms[6]->lpData) * (UWORD)sizeof(TCHAR), 		//	  based on cbDescriptionMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLDataSources(henv,
						 *(UWORD *)lpParms[1]->lpData,				// fDirection
						 (SQLTCHAR *)lpParms[2]->lpData,					// szDSN,
						 *(SWORD *)lpParms[3]->lpData,				// cbDSNMax
						 (SWORD *)lpParms[4]->lpData,					// pcbDSN
						 (SQLTCHAR *)lpParms[5]->lpData,					// szDescription
						 *(SWORD *)lpParms[6]->lpData,				// cbDescriptionMax
						 (SWORD *)lpParms[7]->lpData);				// pcbDescription

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,
					cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[5], lpParms[7], TRUE);

	// Check szDSN for null termination
	CHECKNULLTERM(ci, lpParms[5], lpParms[7], TRUE);

	// Check szDescription for null termination
	CHECKNULLTERM(ci, lpParms[2], lpParms[4], TRUE);

	// Buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	TRUNCATIONCHECK(lpParms[2]->fNull, lpParms[4]->fNull,
				rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[2]->lpData,
				*(SWORD *)lpParms[3]->lpData, *(SWORD *)lpParms[4]->lpData,
				(LPTSTR)lpParms[4]->szName);
	TRUNCATIONCHECK(lpParms[5]->fNull, lpParms[7]->fNull,
				rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[5]->lpData,
				*(SWORD *)lpParms[6]->lpData, *(SWORD *)lpParms[7]->lpData,
				(LPTSTR)lpParms[7]->szName);

   //  Check for errors
   AUTOLOGERROREI(ci,lpActiveEnv,rc,henv);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLDrivers:
//|	Enumerate the data sources on the system.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLDrivers(STD_FH_PARMS)
{
	RETCODE					rc;
	SQLHENV					henv=NULL;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnvNode();

	if (lpParms[0]->lpData)
		henv=*(HENV *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 													// szDSN
						(*(SWORD *)lpParms[3]->lpData) * (SDWORD)sizeof(TCHAR), 		//	  based on cbDriverDescMax
						lpUsrOptions->fBufferChecking);
	BUILDOUTPUTPARM(lpParms[5], 													// szDescription
						(*(SWORD *)lpParms[6]->lpData) * (SDWORD)sizeof(TCHAR), 		//	  based on cbDrvrAttrMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	// Invoke function
	rc = SQLDrivers(henv,
						 *(UWORD *)lpParms[1]->lpData,				// fDirection
						 (SQLTCHAR *)lpParms[2]->lpData,					// szDriverDesc,
						 *(SWORD *)lpParms[3]->lpData,				// cbDriverDescMax
						 (SWORD *)lpParms[4]->lpData,					// pcbDriverDesc
						 (SQLTCHAR *)lpParms[5]->lpData,					// szDriverAttributes
						 *(SWORD *)lpParms[6]->lpData,				// cbDrvrAttrMax
						 (SWORD *)lpParms[7]->lpData);				// pcbDrvrAttr

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms,
					cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[2], lpParms[4], TRUE);
	UNUSEDBUFFERCHECK(ci, lpParms[5], lpParms[7], TRUE);

	// Check szDriverDesc for null termination
	CHECKNULLTERM(ci, lpParms[2], lpParms[4], TRUE);

  	// Check szDriverAttributes for null termination
	CHECKNULLTERM(ci, lpParms[5], lpParms[7], TRUE);

	// Buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	TRUNCATIONCHECK(lpParms[2]->fNull, lpParms[4]->fNull,
				rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[2]->lpData,
				*(SWORD *)lpParms[3]->lpData, *(SWORD *)lpParms[4]->lpData,
				(LPTSTR)lpParms[4]->szName);
	TRUNCATIONCHECK(lpParms[5]->fNull, lpParms[7]->fNull,
				rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[5]->lpData,
				*(SWORD *)lpParms[6]->lpData, *(SWORD *)lpParms[7]->lpData,
				(LPTSTR)lpParms[7]->szName);

   //  Check for errors
   AUTOLOGERROREI(ci,lpActiveEnv,rc,henv);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLGetInfo:
//|	Get information for both driver specific items and non-driver items.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

RETCODE INTFUN lpSQLGetInfo(STD_FH_PARMS)
{
	RETCODE			rc;
	UINT				dex;
	lpDFTARRAY		lpDfts=(*lpParms)->lpDfts;
	BOOL				fLogged=FALSE,
						fDriverHstmt=FALSE,
						fDriverHdesc=FALSE;
	SWORD				wInfoValue=0,
						wInfoValueMax=0;
	SDWORD			dwInfoValue=0,
						dwInfoValueMax=0;
	SQLHDBC			hdbc=NULL;
	SWORD				fInfoType=lpParms[1]->lpData ? *(SWORD *)lpParms[1]->lpData : 0;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[2], 													// rgbInfoValue
						(*(SWORD *)lpParms[3]->lpData) , 				//	based on cbInfoValueMax
						lpUsrOptions->fBufferChecking);

	// if fOption is SQL_DRIVER_HSTMT, we must pass the current hstmt as a parameter in rgbInfoValue
	if(fInfoType == SQL_DRIVER_HSTMT)
	{
		fDriverHstmt=TRUE;

		if((ci) && (ci->lpCurStmt))
		{
			if(lpParms[2]->lpData)
				*(HSTMT *)lpParms[2]->lpData = ci->lpCurStmt->hstmt;
		}
		else
			lpParms[2]->fNull=TRUE;

	}

	// if fOption is SQL_DRIVER_HDESC, we must pass the current hstmt as a parameter in rgbInfoValue
	if ((fInfoType == SQL_DRIVER_HDESC) && !lpParms[2]->fNull)
	{
		fDriverHdesc=TRUE;

		PickHdesc(ci);

		if((ci) && (ci->lpCurDesc))
		{
			if(lpParms[2]->lpData)
				*(SQLHDESC *)lpParms[2]->lpData = ci->lpCurDesc->hdesc;
		}
		else
			lpParms[2]->fNull=TRUE;

	}

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetInfo (hdbc,
						 fInfoType,
						 lpParms[2]->fNull ? NULL : (PTR)lpParms[2]->lpData,     // rgbInfoValue
						 *(SQLSMALLINT *)lpParms[3]->lpData,					// cbInfoValueMax
						 (SQLSMALLINT *)lpParms[4]->lpData);					// pcbInfoValue

	if (lpParms[2] && RC_SUCCESSFUL(rc) && lpParms[4]->lpData) {
		lpParms[2]->cBytes= *(UWORD *)lpParms[4]->lpData;
        if (lpParms[2]->fCType == SQL_C_CHAR) {
            (lpParms[2]->cBytes)++;
        }
    }

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	//Untouch rgbInfoValue in case of error Bug #530
	if (RC_NOTSUCCESSFUL(rc) && (fDriverHstmt || fDriverHdesc) && lpParms[3]->lpData)
		InitUnModified(lpParms[2]->lpData, *(SWORD *)lpParms[3]->lpData + 1);


	// Log output parameters
	// Check to see if the value returned was a handle,bitmask, or linked array --
	// If it is, then we need to format the output for those special cases
	for(dex=0;  dex<(lpParms[1]->cDfts);  dex++)
	{
		// Find the correct fOption in the default array
		if ((SDWORD)fInfoType == lpParms[1]->lpDfts[dex].fOpt)
		{
         // If we get here, we know which element in the default array the fOption is
         if((lpParms[1]->lpDfts[dex].uValInfo == PRM_32HDL) 	// the fOption rgbInfoValue is a handle
         	 				|| (lpParms[1]->lpDfts[dex].lpLink))	// the fOption rgbInfoValue is from a linked array (bitmask)
			{
         	if(!(uAttrib & FH_NO_LOG) && !fLogged)
				{
	         	LogParametersSpecial(szFuncName, lpParms,
											cParms, ci, FALSE,
											lpParms[1]->lpDfts[dex].uValInfo, 1, 2);
					fLogged=TRUE;
				}
			}
		}
	}

	if(!fLogged)	// fInfoType was not in the default array -- driver defined type
		LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// We needed to special case SQL_DRIVER_HSTMT since we overwrite the rgbValue buffer as an input parameter
	if (!((fInfoType == SQL_DRIVER_HSTMT) && (rc != SQL_SUCCESS)))
	{
		// Check each *string* buffer (i.e. check rgbValue's)
		UNUSEDBUFFERCHECK(ci, lpParms[2], lpParms[4], TRUE);

		//Check the pcbValues, any non-string output parameter
		// Null termination and buffer modification checking
		OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);
	}

	// Verify any truncation which may have occured
	if (IsCharData(lpParms[2]->fCType))
	{
		if (lpParms[4]->lpData)
		{
			wInfoValue=*(SWORD *)lpParms[4]->lpData;
			dwInfoValue=(SDWORD)wInfoValue;
		}

		if (lpParms[3]->lpData)
		{
			wInfoValueMax=*(SWORD *)lpParms[3]->lpData;
			dwInfoValueMax=wInfoValueMax;
		}

		TRUNCATIONCHECK(lpParms[2]->fNull, lpParms[4]->fNull, rc, GETOUTPUTWINDOW(ci),
				(LPTSTR)lpParms[2]->lpData,
				dwInfoValueMax,
				dwInfoValue,
				(LPTSTR)lpParms[4]->szName);
	}

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	return rc;
}

//*---------------------------------------------------------------------------------
//| SetODBCBehavior:
//|	This function sets the appropriate behavior 3.0 or 2.X.
//| Parms:
//|	lpci			Current connection informaiton (always NULL)
//|	henv			Current environment handle.
//|	szODBCVer	String to display upon connection
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN SetODBCBehavior(lpCONNECTIONINFO lpci, HENV henv,LPTSTR szODBCVer)
{
	RETCODE	rc=SQL_SUCCESS;
	UDWORD	udVer;
	TCHAR		szOption[MEDBUFF];

	//If default behavior selected don't set env attr
	if (lpUsrOptions->fODBCBehavior == IDR_FC_DEFAULT)
		return;

	if (lpUsrOptions->fODBCBehavior == IDR_FC_ODBC30)
	{
		lstrcpy(szOption,szSQL_OV_ODBC3);
		udVer=SQL_OV_ODBC3;
	}
	else
	{
		lstrcpy(szOption,szSQL_OV_ODBC2);
		udVer=SQL_OV_ODBC2;
	}

	rc = SQLSetEnvAttr(henv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)(LONG_PTR)udVer,0);

	if (RC_SUCCESSFUL(rc))
	{
		wsprintf(szODBCVer,TEXT("\tEnv. Attr. %s set to %s\r\n\r\n"),
			szSQL_ATTR_ODBC_VERSION,szOption);
	}
	else
		ToolOptionsAllErrors(lpci, henv, NULL, NULL,FALSE,rc);

} //SetODBCBehavior()


//*---------------------------------------------------------------------------------
//| LoadDiscriptorInfo:
//|	This function loads the implicitly allocated descriptors with the Driver's
//|	information
//| Parms:
//|	None
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------

VOID INTFUN LoadDescriptor(HSTMT hstmt,lpDESCRIPTORHEADER lpDesc, DWORD dwAttribute)
{
	UWORD		cbName=0;
	UWORD		cCount=0;
	RETCODE	rc=SQL_SUCCESS;


	if (RC_SUCCESSFUL(rc=SQLGetStmtAttr(hstmt,dwAttribute, &lpDesc->hdesc,0,NULL)))
	{
		//Get number of descriptor records available
		if RC_NOTSUCCESSFUL(rc=SQLGetDescField(lpDesc->hdesc,0,SQL_DESC_COUNT,&cCount,0,NULL))
			return;

		//Get Header Fields
		rc=SQLGetDescField(lpDesc->hdesc,0,SQL_DESC_ALLOC_TYPE,&lpDesc->uwAllocType,0,NULL);
		rc=SQLGetDescField(lpDesc->hdesc,0,SQL_DESC_ARRAY_SIZE,&lpDesc->udArraySize,0,NULL);
		rc=SQLGetDescField(lpDesc->hdesc,0,SQL_DESC_BIND_OFFSET_PTR,lpDesc->psdBindOffsetPtr,0,NULL);



		while (cCount--)
		{

		}
	}

} //LoadDescriptor()



//*---------------------------------------------------------------------------------
//| LoadDiscriptorInfo:
//|	This function loads the implicitly allocated descriptors with the Driver's
//|	information
//| Parms:
//|	None
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------

VOID INTFUN LoadDiscriptorInfo(lpCONNECTIONINFO lpci)
{
	lpSTATEMENTINFO		lpStmt=NULL;
	lpDESCRIPTORHEADER	lpARD=NULL,
								lpAPD=NULL,
								lpIRD=NULL,
								lpIPD=NULL;
	RETCODE					rc=SQL_SUCCESS;
	HSTMT						hstmt=NULL;
	DWORD						cCount=0;

	//if there are no descriptors then return
	if (!lpci || !lpci->lpCurStmt || !lpci->lpDesc)
		return;

	lpStmt=lpci->lpCurStmt;
	hstmt=lpStmt->hstmt;
	lpARD=lpStmt->lpARD;
	lpAPD=lpStmt->lpAPD;
	lpIRD=lpStmt->lpIRD;
	lpIPD=lpStmt->lpIPD;

	LoadDescriptor(hstmt,lpARD, SQL_ATTR_APP_ROW_DESC);
	LoadDescriptor(hstmt,lpAPD, SQL_ATTR_APP_PARAM_DESC);
	LoadDescriptor(hstmt,lpIRD, SQL_ATTR_IMP_ROW_DESC);
	LoadDescriptor(hstmt,lpIPD, SQL_ATTR_IMP_PARAM_DESC);

} //LoadDescriptorInfo()


//*---------------------------------------------------------------------------------
//| ConnectFullConnect:
//|	This function will do the following:
//|				1)  Allocate an henv if not already done
//|				2)  Allocate an hdbc
//|				3)  Prompt user for DSN (via SQLDriverConnect if supported,
//|							SQLConnect if not)
//|				4)  Upon successful connection, allocate an hstmt
//| Parms:
//|	None
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN ConnectFullConnect(void)
{
	UINT						uDex;
	RETCODE					rc=SQL_SUCCESS, drc;
	HENV						henv=NULL;
	HDBC						hdbc=NULL;
	HSTMT						hstmt=NULL;
	lpCONNECTIONINFO		lpci=NULL;
	TCHAR						szConnStrIn[(SQL_MAX_DSN_LENGTH + (2 * DFT_UID_LEN) + 20)];
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnvNode();
	lpDIALOGDESC			lpDlg;
	int						nAPI;
	TCHAR						szODBCVer[LARGEBUFF]=TEXT("");
	TCHAR						szDSN[MEDBUFF]=TEXT(""),
								szUID[MEDBUFF]=TEXT("");
	BOOL						fErrors=FALSE;
	lpSTATEMENTINFO		lpStmt=NULL;
	RETCODE					rcCL=SQL_SUCCESS;
	BOOL						fCL=FALSE,
								fCLLoadFailed=FALSE,
								fAutoErrors=lpUsrOptions->fAutoErrors,
								fUse30APIs=TRUE;

	//automatically display errors
	lpUsrOptions->fAutoErrors=TRUE;

	// First get the connection info, if they cancel, nothing to do
	if(IDOK != DialogBox(hInst,
					MAKEINTRESOURCE(IDD_FULL_CONNECT),
					hwndClient,
					GetConnectInfoWndProc))
		return;

	fBusy(TRUE);

	fUse30APIs=((lpUsrOptions->fODBCBehavior > IDR_FC_ODBC2X) &&
				   (lpUsrOptions->nODBCVer != IDR_ODBCVER_25));

	if (lpHeadEnv && lpHeadEnv->lpHead)
	{
		lpActiveEnv=lpHeadEnv->lpHead;
		SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL, 1, 0L);
	}

	if (lpActiveEnv)
		henv=lpActiveEnv->henv;

	lpci = CreateConnectionWindow(hwndClient, hInst,lpActiveEnv);

	if (!lpci)
		goto exit01;

	// Allocate an henv if user wants to allocate one or
	// if there are no hEnv's allocated
	if (lpUsrOptions->fAllocEnv || !lpHeadEnv->cEnvs)
	{

		//For ODBC 3.0 use SQLAllocHandle()
		if (fUse30APIs)
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLALLOCHANDLE);


			rc = SQLAllocHandle(SQL_HANDLE_ENV,NULL,&henv);
		}
		else
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLALLOCENV);

			rc=SQLAllocEnv(&henv);
		}

		if (RC_SUCCESSFUL(rc))
		{
			//create new Environment info node.
			if (!(lpActiveEnv=DoAllocEnv(henv)))
				goto exit01;
			lpci->lpActiveEnv=lpActiveEnv;

		}
	}

	if (lpUsrOptions->fAllocEnv)
	{
		// ODBC behavior on new environment
		SetODBCBehavior(lpci, henv,szODBCVer);

		// See Env. Attr. is set
		if (RC_NOTSUCCESSFUL(rc=SQLGetEnvAttr(henv,SQL_ATTR_OUTPUT_NTS,&lpActiveEnv->fNTS,300,NULL)))
			lpActiveEnv->fNTS=TRUE;
	}

	//For ODBC 3.0 use SQLAllocHandle()
	if (fUse30APIs)
	{
		uDex = GetSqlFuncOffset(SQL_API_SQLALLOCHANDLE);
		rc = SQLAllocHandle(SQL_HANDLE_DBC,henv,&hdbc);
	}
	else
	{
		uDex = GetSqlFuncOffset(SQL_API_SQLALLOCCONNECT);
		rc = SQLAllocConnect(henv, &hdbc);
		nAPI=API_SQLALLOCCONNECT;
	}

	if(RC_NOTSUCCESSFUL(rc))
		goto exit01;

	// If successful, continue with full connect
	if(lpci)
	{
		if (lpActiveEnv)
			lpActiveEnv->lpCurConn=lpci;

		InitConnectionInfo(lpci,lpActiveEnv);

		lpci->hdbc = hdbc;
		lpci->uState = STATE_ALLOCATED_HDBC;

		//
		// User can set their option for loading the cursor lib.  The default
		// option is SQL_CUR_USE_DRIVER, and this option is therefore ignored.
		//
		if(lpUsrOptions->fUseCursorLib)
		{
			//For ODBC 3.0 use SQLAllocHandle()
			if (fUse30APIs)
			{
				rcCL = SQLSetConnectAttr(lpci->hdbc,
											SQL_ODBC_CURSORS,
											(SQLPOINTER)(LONG_PTR)(lpUsrOptions->fUseCursorLib - 1),
											0);
				nAPI=API_SQLSETCONNECTATTR;
			}
			else
			{
				rcCL = SQLSetConnectOption(lpci->hdbc,
											SQL_ODBC_CURSORS,
											(UDWORD)(lpUsrOptions->fUseCursorLib - 1));
				nAPI=API_SQLSETCONNECTOPTION;
			}

			if (RC_NOTSUCCESSFUL(rcCL))
					AUTOLOGERRORCI(lpci,rcCL,lpci->hdbc);

			if (lpUsrOptions->fRecord)
			{
				lpDlg = DialogBuilder(nAPI - 1);
			}

		}



	}
	else
		goto exit01;


	// Now do a connection.  We'll try to do a DriverConnect first which
	//		will allow you to prompted for a user-id if you did not first
	//		enter one.  If this function is not supported, try SQLConnect
	wsprintf(szConnStrIn, (LPTSTR)szDSNStr, (LPTSTR)lpUsrOptions->szInitialDSN);
	if(*lpUsrOptions->szInitialUID || *szPWD)
	{
		wsprintf(szOutBuff, (LPTSTR)szUIDPWD,
					(LPTSTR)lpUsrOptions->szInitialUID, (LPTSTR)szPWD);
		lstrcat(szConnStrIn, szOutBuff);
	}

	uDex = GetSqlFuncOffset(SQL_API_SQLDRIVERCONNECT);
	drc = rc = SQLDriverConnect(hdbc,hwndClient,szConnStrIn,SQL_NTS,
						NULL, 0, NULL,	SQL_DRIVER_COMPLETE);

	if(rc == SQL_NO_DATA_FOUND)
	{
		//Decrement connection number
		if (lpActiveEnv->cConns > 0)
			lpActiveEnv->cConns--;

		if (lpci->uConNum > 0)
			lpci->uConNum--;

		goto exit01;
	}

	AUTOLOGERRORCI(lpci,rc,hdbc);

	if(RC_SUCCESSFUL(rc))
	{

		//extract szDSN and szUID
		ExtractConnectionInfo(szConnStrIn,szDSN,szUID);

		DoConnect(lpci,lpActiveEnv,szDSN,szUID);

		nAPI=API_SQLDRIVERCONNECT;
	}
	else
	{

		if (RC_NOTSUCCESSFUL(drc))
		{
			rc = drc;	// Reset for logging
			goto exit01;
		}
		else
		{
			// SQLDriverConnect was not supported, so let's do SQLConnect
			uDex = GetSqlFuncOffset(SQL_API_SQLCONNECT);
			rc = SQLConnect(lpci->hdbc,
									lpUsrOptions->szInitialDSN, SQL_NTS,
									lpUsrOptions->szInitialUID, SQL_NTS,
									szPWD, SQL_NTS);
			nAPI=API_SQLCONNECT;

			AUTOLOGERRORCI(lpci,rc,lpci->hdbc);

		}
	}

	//Search for "Cursor library not used" message
	if (rc == SQL_SUCCESS_WITH_INFO)
	{
		SDWORD				fNativeError;
		SWORD					cbErrorMsg;
		TCHAR					szSqlState[6];
		TCHAR					szErrMsg[SQL_MAX_MESSAGE_LENGTH];
		SQLSMALLINT			RecNumber=1;
		RETCODE				rcErr=SQL_SUCCESS;

		// Look for all errors on each ODBC handle
		rcErr = SQLGetDiagRec(SQL_HANDLE_DBC,hdbc,RecNumber,szSqlState,&fNativeError,
								szErrMsg,tsizeof(szErrMsg),&cbErrorMsg);

		if (RC_NOTSUCCESSFUL(rcErr))
		{
			// Look for all errors on each ODBC handle
			rcErr = SQLError(NULL, hdbc, NULL, szSqlState,
							&fNativeError, szErrMsg,
                            sizeof(szErrMsg)/sizeof(szErrMsg[0]), &cbErrorMsg);
		}


		while(RC_SUCCESSFUL(rcErr))
		{

			if (_tcsstr(szErrMsg,TEXT("Cursor library not used")))
			{
				fCLLoadFailed=TRUE;
				break;
			}

			//Get Next Record
			RecNumber++;

			rcErr = SQLGetDiagRec(SQL_HANDLE_DBC,hdbc,RecNumber,szSqlState,&fNativeError,
									szErrMsg,tsizeof(szErrMsg),&cbErrorMsg);

			if (RC_NOTSUCCESSFUL(rcErr))
			{
				// Look for all errors on each ODBC handle
				rcErr = SQLError(NULL, hdbc, NULL, szSqlState,
								&fNativeError, szErrMsg,
                                sizeof(szErrMsg)/sizeof(szErrMsg[0]), &cbErrorMsg);
			}

		}

	}

	if(RC_NOTSUCCESSFUL(rc))
		goto exit01;
	if(rc == SQL_SUCCESS_WITH_INFO)
		szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV2, TRUE, TRUE, (LPTSTR)szFullConnect);

	GetSupportedFunctions(lpci->hdbc, lpci);

	//Record Event
	if (lpUsrOptions->fRecord)
	{
		lpDlg = DialogBuilder(nAPI - 1);
	}

	//Display cursor lib option
	if (RC_SUCCESSFUL(rcCL))
	{
		TCHAR szBuff[SMALLBUFF],
				szBuff2[SMALLBUFF];

		switch(IDR_CL_DEFAULT + lpUsrOptions->fUseCursorLib )
		{
			case IDR_CL_USE_IF_NEEDED:
				lstrcpy(szBuff,TEXT("Use If Needed"));
				break;
			case IDR_CL_USE_ODBC:
				if (fCLLoadFailed)
					lstrcpy(szBuff,TEXT("Use Driver(Cursor Library Load Failed!!)"));
				else
					lstrcpy(szBuff,TEXT("Use ODBC"));
				break;
			case IDR_CL_USE_DRIVER:
				lstrcpy(szBuff,TEXT("Use Driver"));
				break;
			case IDR_CL_DEFAULT:
				lstrcpy(szBuff,TEXT("Default"));
				break;
		}

		wsprintf(szBuff2,TEXT("\r\n\tFull Connect(%s)\r\n\r\n"),szBuff);
		szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV1, TRUE, TRUE,
				szBuff2);
	}

	//Display ODBC version set
	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV1, TRUE, TRUE,
			szODBCVer);

	// Now do the hstmt
   uDex = GetSqlFuncOffset(SQL_API_SQLALLOCSTMT);

	//For ODBC 3.0 use SQLAllocHandle()
	if (fUse30APIs)
	{
		rc = SQLAllocHandle(SQL_HANDLE_STMT,hdbc, &hstmt);
	}
	else
	{
	   rc = SQLAllocStmt(hdbc, &hstmt);
	}

   if(rc != SQL_SUCCESS)
   	goto exit01;

	//See if descriptors are available
   if(!DoAllocStmt(lpci, hstmt))				// Add statement node
   	goto exit01;

	if (lpStmt=lpci->lpCurStmt)
	{
		if (!lpStmt->uFSFRowSetSize)
			lpStmt->uFSFRowSetSize=1;

		if (!lpStmt->uEFRowSetSize)
			lpStmt->uEFRowSetSize=1;
	}

	if (fCL)
	{
		//Inform user that the Cursor Library was successfully loaded
		szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV0, TRUE, TRUE,
			(LPTSTR)TEXT("\tCursor Library successfully loaded!!\r\n\r\n"));

	}

   // Tell user they are connected
	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpci), MSG_SEV1, TRUE, TRUE,
					GetGtrString(szOutBuff, sizeof(szOutBuff), idsSuccessfullyConnected),
					(LPTSTR)lpci->szServer);
	SetFocus(lpci->hwndIn);
	++cConnects;

	// Set the text after connection
	GetConnWinnDescText(lpci, szOutBuff, MAXOUTBUFF);
	SetStatBarText(hwndStatBar, szOutBuff, BLACK_TEXT);
	RefreshToolbar();
	//Reset user options
	lpUsrOptions->fAutoErrors=fAutoErrors;
	fBusy(FALSE);
   return;

	// This label is executed when a failure occurs.  uDex has the index of the
	//		function which failed.  Echo return code and function name
exit01:
	if(uDex != (UWORD)-1)
	{
		HDBC hdbctmp=lpci ? lpci->hdbc:NULL;
		GetApiName(uDex, szOutBuff);
		LogReturnCode((LPTSTR)szOutBuff, NULL, rc);
		LogErrorsToOutputWindow(NULL, henv, hdbctmp, NULL,NULL);
	}

	ConnectFullDisconnect(lpci,drc);					// Undo any steps up until now
	lpci = NULL;

	//reset user options
	lpUsrOptions->fAutoErrors=fAutoErrors;

	RefreshToolbar();
	fBusy(FALSE);
}


//*---------------------------------------------------------------------------------
//| ConnectFullDisconnect:
//|	This function will do the following:
//|				1) Free all allocated hstmts belonging to a connection
//|				2) Disconnect from the server
//|				3) Free the hdbc and connection window
//|				4) If there are no more connections, free the henv
//| Parms:
//|	lpci					The connection window to free
//|	rcDriverConnect	If SQLDriverConnect failed we don't want to
//|							try to free hstmt's or hdesc's and we don't
//|							want to do a SQLDisconnect
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ConnectFullDisconnect(lpCONNECTIONINFO lpci,RETCODE rcDriverConnect)
{
	UINT						uDex;
	RETCODE					rc=SQL_SUCCESS;
	lpENVIRONMENTINFO		lpEi=NULL;
	HENV						henv=NULL;
	BOOL		fUse30APIs=((lpUsrOptions->fODBCBehavior > IDR_FC_ODBC2X) &&
								(lpUsrOptions->nODBCVer != IDR_ODBCVER_25));

	if (!lpci)
		return(TRUE);

	lpEi=lpci->lpActiveEnv;

	if (lpEi)
		henv = lpEi->henv;

	fBusy(TRUE);

	if (RC_SUCCESSFUL(rcDriverConnect))
	{

		// Free all allocated hdescs just in case some explicitly
		//allocated hdescs are associated with hstmt's
		if(lpci)
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLFREEHANDLE);

			if (!DropAllDescs(lpci))
				goto exit01;
		}

		// Free all allocated hstmts
		if(lpci)
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLFREESTMT);
			if(!DropAllStmts(lpci, TRUE))
				goto exit01;
		}

		// Disconnect from the server
		if(lpci)
		{
			// Disconnect only if we are connected.  If we were browsing it will be
			//		freed in the following section
			if(lpci->uState & STATE_CONNECTED_HDBC ||
				lpci->uState & STATE_BROWSING_HDBC)
			{
				uDex = GetSqlFuncOffset(SQL_API_SQLDISCONNECT);

				rc = SQLDisconnect(lpci->hdbc);

				if(RC_NOTSUCCESSFUL(rc))
					goto exit01;
				DoDisconnect(lpci);						// Change title, set state
			}
		}

	}

	// Free the connection window by freeing the hdbc.  Note that freeing the
	//		connection will remove any affects from SQLBrowseConnect as well.
	if(lpci)
	{
		if(lpci->hdbc)
		{
			//For ODBC 3.0 use SQLFreeHandle()
			if (fUse30APIs)
			{
				uDex = GetSqlFuncOffset(SQL_API_SQLFREEHANDLE);
				rc = SQLFreeHandle(SQL_HANDLE_DBC,lpci->hdbc);
			}
			else
			{
				uDex = GetSqlFuncOffset(SQL_API_SQLFREECONNECT);
				rc = SQLFreeConnect(lpci->hdbc);
			}

			if(rc != SQL_SUCCESS)
				goto exit01;
		}

		lpci->uState &= ~STATE_BROWSING_HDBC;
		DestroyConnectionWindow(lpci);

	}


	// Free henv if there are no allocated hdbcs
	if (lpEi && !lpEi->cConns)
	{
		if (fUse30APIs)
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLFREEHANDLE);
			rc = SQLFreeHandle(SQL_HANDLE_ENV,lpEi->henv);
		}
		else
		{
			uDex = GetSqlFuncOffset(SQL_API_SQLFREEENV);
			rc = SQLFreeEnv(lpEi->henv);
		}

		if(RC_NOTSUCCESSFUL(rc))
			goto exit01;
		else
			DoFreeEnvNode(lpEi);
	}



	RefreshToolbar();
	fBusy(FALSE);
	return TRUE;

	// This label is executed when a failure occurs.  uDex has the index of the
	//		function which failed.  Echo return code and function name
exit01:
	GetApiName(uDex, szOutBuff);
	LogReturnCode((LPTSTR)szOutBuff, lpci, rc);
	RefreshToolbar();
	fBusy(FALSE);
	szMessageBox(hwndClient,
		MB_ICONEXCLAMATION | MB_OK,
		NULL,
		GetGtrString(szOutBuff, MAXOUTBUFF, idsErrorOnFullDisconnect));

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| ConnectDataSourcesAll:
//|	This function will enumerate all data sources to the given output window.
//| Parms:
//|	hwnd						Output window
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN ConnectDataSourcesAll( lpCONNECTIONINFO ci)
{
	RETCODE			rc;
	SQLTCHAR *		lpszDSN;
	SQLTCHAR *		lpszDesc;
	SWORD				cbDSN;
	SWORD				cbDesc;
	SWORD				cbSize=lpUsrOptions->uDftBuffLen;
	HWND				hwndOut;
	HENV				henv=NULL;

	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnvNode();

	if (lpActiveEnv)
		henv=lpActiveEnv->henv;

	hwndOut = GETOUTPUTWINDOW(ci);

	// Allocate memory for the DSN and Description
	lpszDSN = (SQLTCHAR *)AllocateMemory(cbSize);
	lpszDesc = (SQLTCHAR *)AllocateMemory(cbSize);
	if(!(lpszDSN && lpszDesc))
		goto exit01;

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
			(LPTSTR)szDataSourcesAll);

	// Print the secondary header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
				(LPTSTR)szDataSourcesSub);

	rc = SQLDataSources(henv,SQL_FETCH_FIRST,lpszDSN,cbSize,&cbDSN,
								lpszDesc,cbSize,&cbDesc);

   while(rc != SQL_NO_DATA_FOUND)
	{
		// Make sure the user hasn't aborted
		if(fAbortCheck(hwndOut))
			goto exit01;

      if(rc != SQL_SUCCESS)
		{
			AUTOLOGERROREI(ci,lpActiveEnv,rc,henv);
      	goto exit01;
	   }

		// Now print the information for one column
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
				  (LPTSTR)szDataSourcesItemStr,
					lpszDSN, cbDSN, lpszDesc, cbDesc);

		// Now clear the buffers
		*lpszDSN = TEXT('\0');
		*lpszDesc = TEXT('\0');

		// Now make the next call
		rc = SQLDataSources(henv,SQL_FETCH_NEXT,lpszDSN,cbSize,&cbDSN,
									lpszDesc,cbSize,&cbDesc);
   	}

	// Now print an extra line for clarity
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
			  (LPTSTR)TEXT("\r\n"));

exit01:
 	FREE_PTR(lpszDSN);
	FREE_PTR(lpszDesc);

	fBusy(FALSE);
}


//*---------------------------------------------------------------------------------
//| ConnectDriversAll:
//|	This function will enumerate all data sources to the given output window.
//| Parms:
//|	hwnd						Output window
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN ConnectDriversAll(lpCONNECTIONINFO ci)
{
	RETCODE			rc;
	SQLTCHAR *		lpDriverDesc;
	SQLTCHAR *		lpDriverAttr;
	SQLTCHAR *		lpDrvrAttrBuff;
	SWORD				cbDriverDesc;
	SWORD				cbDriverAttr;
	SWORD				cbSize=lpUsrOptions->uDftBuffLen;
	HWND				hwndOut;
	HENV				henv=NULL;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnvNode();

	if (lpActiveEnv)
		henv=lpActiveEnv->henv;

	hwndOut = GETOUTPUTWINDOW(ci);

	// Allocate memory for the Description and Attributes
	lpDriverDesc = (SQLTCHAR *)AllocateMemory(cbSize);
	lpDriverAttr = (SQLTCHAR *)AllocateMemory(cbSize);
	lpDrvrAttrBuff = (SQLTCHAR *)AllocateMemory(cbSize);
	if(!(lpDriverDesc && lpDriverAttr && lpDrvrAttrBuff))
		goto exit01;

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
			(LPTSTR)szDriversAll);

	// Print the secondary header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
				(LPTSTR)szDriversSub);

	rc = SQLDrivers(henv,SQL_FETCH_FIRST,lpDriverDesc,cbSize,&cbDriverDesc,
							lpDriverAttr,cbSize,&cbDriverAttr);

   while(rc != SQL_NO_DATA_FOUND){
		// Make sure the user hasn't aborted
		if(fAbortCheck(hwndOut))
			goto exit01;

      if(rc != SQL_SUCCESS){
			ToolOptionsAllErrors(NULL, henv, NULL, NULL,FALSE,rc);
      	goto exit01;
		   }

		// Format the szDriverAttributes
		FormatValueBasedOnType(lpDrvrAttrBuff,(DWORD)cbSize,lpDriverAttr,
										SQL_C_DOUBLESTR,(UDWORD)cbDriverAttr,hwndOut);

		// Now print the information for one column
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
				  (LPTSTR)szDriversItemStr,
					lpDriverDesc, cbDriverDesc, lpDrvrAttrBuff, cbDriverAttr);

		// Now clear the buffers
		*lpDriverDesc = TEXT('\0');
		*lpDriverAttr = TEXT('\0');
		*lpDrvrAttrBuff = TEXT('\0');

		// Now make the next call
		rc = SQLDrivers(henv,SQL_FETCH_NEXT,lpDriverDesc,cbSize,&cbDriverDesc,
								lpDriverAttr,cbSize,&cbDriverAttr);
   	}

	// Now print an extra line for clarity
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE,
			  (LPTSTR)TEXT("\r\n"));

exit01:
 	FREE_PTR(lpDriverDesc);
	FREE_PTR(lpDriverAttr);
	FREE_PTR(lpDrvrAttrBuff);

	fBusy(FALSE);
}



//*---------------------------------------------------------------------------------
//| ConnectGetInfoAll:
//|	This function will return all the information about a driver and data source
//|	associated with an hdbc.
//| Parms:
//|	henv						Environment variable to use if allocated
//|	hwnd						Output window
//| Returns:
//|	TRUE if successful, FALSE if not successful.
//*---------------------------------------------------------------------------------
VOID INTFUN ConnectGetInfoAll(lpCONNECTIONINFO ci)
{
	lpDFTARRAY		lpDfts=lpdftGetInfo;
	UINT				cDfts=ciInfoTypes;
	UWORD				cbExtra=sizeof(TCHAR)*2;
	RETCODE			rc=SQL_SUCCESS;
	PTR				rgbInfoValue = (PTR)AllocateMemory(lpUsrOptions->uDftBuffLen+cbExtra);
	SWORD				cbInfoValue=0;
	UWORD				idx=0;
	PTR				pidx=&idx;
	TCHAR				szTemp[100]=TEXT("");
	BOOL				fFlushed,fNoErrors=TRUE;
	HWND				hwndOut=GETOUTPUTWINDOW(ci);
	LPTSTR			szBigOlBuff = (LPTSTR)AllocateMemory(XLARGEBUFF);
	LPTSTR			str=szBigOlBuff;
	UINT				uLen=0;

	if (!ci)
		return;

	fBusy(TRUE);

	*szBigOlBuff = TEXT('\0');

	// Print the header
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,(LPTSTR)szGetInfoAll);
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,(LPTSTR)szGetInfoSub);

	// Indent the first line the correct amount
	lstrcat(szBigOlBuff,TEXT("\t\t\t\t\t\t\t\t\t"));
	str+=lstrlen(szBigOlBuff);

	// Loop through every possible fInfoType value
	for(idx=0;idx<cDfts;idx++)
	{
		// Make sure the user hasn't aborted
		if(fAbortCheck(hwndOut))
			goto exit01;

		// if fOption is SQL_DRIVER_HSTMT, we must pass the current hstmt as a parameter in rgbInfoValue
		if (lpDfts[idx].fOpt == SQL_DRIVER_HSTMT && ci->lpCurStmt)
			*(HSTMT *)rgbInfoValue = ci->lpCurStmt->hstmt;

		// if fOption is SQL_DRIVER_HDESC, we must pass the current hstmt as a parameter in rgbInfoValue
		if (lpDfts[idx].fOpt == SQL_DRIVER_HDESC && ci->lpDesc)
			*(SQLHDESC *)rgbInfoValue = ci->lpDesc->hdesc;

		//Bug #464 Since SQL_OJ_CAPABILITIES = 65003 util 3.0
		//this kludge is needed.
		if (idx==115)
			rc = SQLGetInfo(ci->hdbc, SQL_OJ_CAPABILITIES, rgbInfoValue,	lpUsrOptions->uDftBuffLen,	&cbInfoValue);
		else
			rc = SQLGetInfo(ci->hdbc, (UWORD)lpDfts[idx].fOpt, rgbInfoValue,	lpUsrOptions->uDftBuffLen,	&cbInfoValue);

      // Now we start printing information
		// First we add enough tabs
		uLen=wsprintf(str, TEXT("%s=%u, "), (LPTSTR)lpDfts[idx].szOpt, lpDfts[idx].fOpt);
		str+=uLen;

		if(RC_SUCCESSFUL(rc))
		{

			// print cbInfoValue
			uLen=wsprintf(str,TEXT("%d, "), cbInfoValue);
			str+=uLen;

			// format rgbInfoValue according to it's type
         if((lpDfts[idx].uValInfo == PRM_32HDL) ||	// the rgbInfoValue is a handle
         	(lpDfts[idx].lpLink))						// the rgbInfoValue is from a linked array (bitmask)
			{
				fNoErrors = FormatSpecialParameters(lpDfts, cDfts, (PTR)&lpDfts[idx].fOpt, rgbInfoValue, hwndOut,
																lpDfts[idx].uValInfo, &szBigOlBuff, &str, &fFlushed,
																TEXT("rgbInfoValue"));
				str+=lstrlen(str);
			}
			else
			{
				switch(lpDfts[idx].uValInfo)
				{
					case PRM_16BIT:
						uLen=wsprintf(str, TEXT("%u"), *(UWORD *)rgbInfoValue);
						break;

					case PRM_32BIT:
						uLen=wsprintf(str, TEXT("%lu"), *(UDWORD *)rgbInfoValue);
						break;

					case PRM_STR:
						uLen=wsprintf(str, TEXT("\"%s\""), (LPTSTR)rgbInfoValue);
						break;
				}

				str+=uLen;
			}

			if(fNoErrors)			// No special formatting errors were printed, so we need to flush the buffer
			{
				lstrcpy(str,TEXT("\r\n"));
				str+=lstrlen(TEXT("\r\n"));
			}

		}
		else
		{
			// Either SQL_ERROR or SQL_INVALID_HANDLE was returned
			TCHAR szMsg[MAXOUTBUFF]=TEXT("");

			str -= 2; 				// Back up past the comma because pcbValue was blank
			GetRetCodeName(rc, (LPTSTR)szTemp);

			while(SQLError(NULL, ci->hdbc, NULL, szTemp, NULL, szMsg, MAXOUTBUFF, NULL) != SQL_NO_DATA_FOUND)
			{
				uLen=wsprintf(str,TEXT(", Error: %s Msg: %s. \r\n"),szTemp,szMsg);
				str+=uLen;
			}
		}

		// Force it to flush if it hasn't been
		szBufferParmInfo(&fFlushed, szBigOlBuff, hwndOut);
		str = szBigOlBuff;
		*szBigOlBuff = TEXT('\0');

		if(fNoErrors)		// If no error was printed, indent the entire amount
			lstrcpy(str,(LPTSTR)szNineTab);
		else
			lstrcpy(str,(LPTSTR)szThreeTab);

		str += lstrlen(szBigOlBuff);

		fNoErrors=TRUE;
   }

	// Now print an extra line for clarity
	szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
			  (LPTSTR)TEXT("\r\n"));

	// Now clear any errors that might still be in the error que
	while(SQLError(NULL, ci->hdbc, NULL, szTemp, NULL, szBigOlBuff, MAXOUTBUFF, NULL) != SQL_NO_DATA_FOUND);

exit01:
	FREE_PTR(rgbInfoValue);
	FREE_PTR(szBigOlBuff);
	fBusy(FALSE);
}


//*------------------------------------------------------------------------
//| EnableDisableODBCBehaviorGroup:
//|	This routine disables the ODBC Behavior
//|	Group box if the user is connecting on an existing HENV,if the
//|	user is requesting a new HENV this group box is enabled.
//| Parms:
//|	hDlg		handle to Dialog box
//|	fEnable	TRUE if we need to enable, FALSE if we don't
//| Returns:
//|	Nothing
//*------------------------------------------------------------------------
VOID INTFUN EnableDisableODBCBehaviorGroup(HWND hDlg,BOOL fEnable)
{
	//if we connecting using an existing henv, then
	//Disable the odbc behavior group box
	EnableWindow(GetDlgItem(hDlg,IDG_ODBC_BEHAVIOR_GROUP),fEnable);
	EnableWindow(GetDlgItem(hDlg,IDR_FC_DEFAULT),fEnable);
	EnableWindow(GetDlgItem(hDlg,IDR_FC_ODBC2X),fEnable);
	EnableWindow(GetDlgItem(hDlg,IDR_FC_ODBC30),fEnable);
}


//*------------------------------------------------------------------------
//| GetConnectInfoWndProc:
//|	Dialog proc prompts for connection info, writes values to global
//|	variables szDSN, szUID, and szPWD.
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
LRESULT EXTFUN GetConnectInfoWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
			{
			SDWORD		dwDex;
			lpENVIRONMENTINFO lpActiveEnv=GetActiveEnv();
			BOOL			fEnable=TRUE;

			CenterDialog(hDlg);

			//Clear "New hEnv" check box if there is an active Env already
			if (!lpActiveEnv)
				fEnable=FALSE;

			CheckDlgButton(hDlg, IDC_NEWHENV, !fEnable);
			EnableWindow(GetDlgItem(hDlg,IDC_NEWHENV),fEnable);

			//Check ODBC Behavior
			CheckRadioButton(hDlg, IDR_FC_DEFAULT, IDR_FC_ODBC30,
								lpUsrOptions->fODBCBehavior);

			//Check Cursor Library button
			CheckRadioButton(hDlg, IDR_CL_DEFAULT, IDR_CL_USE_DRIVER,
								(IDR_CL_DEFAULT + lpUsrOptions->fUseCursorLib));

			EnableDisableODBCBehaviorGroup(hDlg,!fEnable);

 			// Set limits
			SendDlgItemMessage(hDlg, IDE_USERID, EM_LIMITTEXT, DFT_UID_LEN, 0L);
			SendDlgItemMessage(hDlg, IDE_PASSWORD, EM_LIMITTEXT, DFT_UID_LEN, 0L);

			// Then defaults
			GetDSNs(GetDlgItem(hDlg, IDC_DATA_SOURCE));
			dwDex = (DWORD)SendDlgItemMessage(hDlg, IDC_DATA_SOURCE,
								CB_FINDSTRINGEXACT, 0,
								(LPARAM)(LPTSTR)lpUsrOptions->szInitialDSN);
			if(dwDex == CB_ERR)
				dwDex = 0;
			SendDlgItemMessage(hDlg, IDC_DATA_SOURCE, CB_SETCURSEL, (WPARAM)dwDex, 0L);
			SetDlgItemText(hDlg, IDE_USERID, lpUsrOptions->szInitialUID);
			SetDlgItemText(hDlg, IDE_PASSWORD, szPWD);
			}
			return TRUE;


		case WM_COMMAND:
		{
			UINT	id= GET_WM_COMMAND_ID(wParam,lParam),
					code=GET_WM_COMMAND_CODE(wParam, lParam);

			switch(id)
			{
				case IDOK:
				{
					// Retrieve values
					WPARAM		dex;
					HWND			hwnd=GetDlgItem(hDlg, IDC_DATA_SOURCE);

					//Assume 3.0
					lpUsrOptions->fODBC30=TRUE;

					// Check ODBC Behavior buttons
					if (IsDlgButtonChecked(hDlg, IDR_FC_DEFAULT))
					{
						lpUsrOptions->fODBC30=FALSE;
						lpUsrOptions->fODBCBehavior = IDR_FC_DEFAULT;
					}
					else if (IsDlgButtonChecked(hDlg, IDR_FC_ODBC2X))
					{
						lpUsrOptions->fODBC30=FALSE;
						lpUsrOptions->fODBCBehavior = IDR_FC_ODBC2X;
					}
					else
					{
						lpUsrOptions->fODBC30=TRUE;
						lpUsrOptions->fODBCBehavior = IDR_FC_ODBC30;
					}

					// Chec cursor lib option by looking at buttons
					for(dex=IDR_CL_DEFAULT;  dex<=IDR_CL_USE_DRIVER;  dex++)
					{
						if(IsDlgButtonChecked(hDlg, (int)dex))
						{
							lpUsrOptions->fUseCursorLib = (int)dex % 10;
							break;
						}
					}

					if(CB_ERR == (dex = (WPARAM)SendMessage(hwnd, CB_GETCURSEL, 0, 0L)))
						GetDlgItemText(hDlg, IDC_DATA_SOURCE,
									lpUsrOptions->szInitialDSN,
									tsizeof(lpUsrOptions->szInitialDSN));
					else
						SendMessage(hwnd, CB_GETLBTEXT, dex,
									(LPARAM)(LPTSTR)lpUsrOptions->szInitialDSN);
					GetDlgItemText(hDlg, IDE_USERID,
									lpUsrOptions->szInitialUID,
									tsizeof(lpUsrOptions->szInitialUID));
					GetDlgItemText(hDlg, IDE_PASSWORD, szPWD, tsizeof(szPWD));

					lpUsrOptions->fAllocEnv=IsDlgButtonChecked(hDlg, IDC_NEWHENV);

               SaveGetUserOptions(lpUsrOptions, TRUE);

					EndDialog(hDlg, IDOK);
				}
				return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
            case IDHELP:
                    PostMessage(hDlg, USER_FUNCTION_HELP, 0, 0L);
                    return TRUE;
			}

			switch(code)
			{
				case BN_CLICKED:
					EnableDisableODBCBehaviorGroup(hDlg,IsDlgButtonChecked(hDlg,IDC_NEWHENV));
					break;
				case CBN_DBLCLK:
					SendMessage(hDlg, WM_COMMAND, IDOK, 0L);
					break;
			}


		}
		return FALSE;

		case USER_FUNCTION_HELP:
			{
		//	iTestHelpCnt += InvokeHelp(hDlg, (LPCTSTR)szTestHelpFile,
		//			(LPCTSTR)TEXT("Full Connect"));
			}
            return TRUE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}

	return FALSE;
}




//*---------------------------------------------------------------------------------
//| lpSQLNativeSql:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLNativeSql(STD_FH_PARMS)
{
	RETCODE		rc;
	HDBC			hdbc=NULL;

	if (lpParms[0]->lpData)
		hdbc=*(HDBC *)lpParms[0]->lpData;

	// Allocate memory as required, set string pointers to NULL if required
	BUILDOUTPUTPARM(lpParms[3], 													// szSqlStr
						(*(SDWORD *)lpParms[4]->lpData) * (UWORD)sizeof(TCHAR), 	//	  based on cbSqlStrMax
						lpUsrOptions->fBufferChecking);

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);


	// Invoke function
	rc = SQLNativeSql(hdbc,
						 (TCHAR *)lpParms[1]->lpData,					// szSqlStrIn,
						 *(SDWORD *)lpParms[2]->lpData,				// cbSQlStrIn
						 (TCHAR *)lpParms[3]->lpData,					// szSqlStr
						 *(SDWORD *)lpParms[4]->lpData,				// cbSqlStrMax
						 (SDWORD *)lpParms[5]->lpData);				// pcbSqlStr

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	// Log output parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, FALSE);

	// Check each *string* buffer
	UNUSEDBUFFERCHECK(ci, lpParms[3], lpParms[5], TRUE);

	// Null termination and buffer modification checking
	OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, TRUE);

	// Verify any truncation which may have occured
	// If szSqlStr is NULL, we don't need to truncation check it since it will
	// return SQL_SUCCESS in this case (instead of SQL_SUCCESS_WITH_INFO and 01004)
	TRUNCATIONCHECK(lpParms[3]->fNull, lpParms[5]->fNull, rc, GETOUTPUTWINDOW(ci),
		(LPTSTR)lpParms[3]->lpData,
		*(SDWORD *)lpParms[4]->lpData,
		*(SDWORD *)lpParms[5]->lpData,
		(LPTSTR)lpParms[5]->szName);

   //  Check for errors
   AUTOLOGERRORCI(ci,rc,hdbc);

	return rc;
}

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	OPTWIN.C
//|
//| Purpose:
//|		This module contains functions that support the Tools\Options
//|	tabbed dialogs
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "resource.h"
#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define MAX_VERSION_LEN				10

#define DFT_GATOR_LENGTH			300
#define DFT_PARM_LENGTH				75
#define MIN_PARM_LENGTH				50
#define DFT_BIND_BUFF				254
#define MAX_BIND_BUFF				65534
#define C_PAGES						5

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE				hInst;
extern lpUSROPTIONS			lpUsrOptions;
extern HWND						hwndStatBar;
extern HWND						hwndOutWin;
extern HWND						hwndClient;
extern HMENU 					hMenu;
extern HFONT					hEditFont;					// For edit windows and rowset windows
extern HFONT					hColumnFont;				// For columns in rowset windows
extern int						iTestHelpCnt;			// How many times have we loaded this help file?
extern LPTSTR					szTestHelpFile;
extern lpCONNECTIONINFO		lpConnWin;					// Head node for connection windows
static TCHAR					szFont[LF_FACESIZE+1];
static int						iPtSize;
static int						iHeight;



dCSEG(TCHAR) szHELP_OPT_USER[]                      = TEXT("User Options");
dCSEG(TCHAR) szHELP_OPT_BUFLEN[]                    = TEXT("Buffer Lengths");
dCSEG(TCHAR) szHELP_OPT_MISC[]                      = TEXT("Miscellaneous");
dCSEG(TCHAR) szHELP_OPT_ROWSET[]                    = TEXT("Rowset Options");
dCSEG(TCHAR) szHELP_OPT_AUTOMATED[]                 = TEXT("Automated Options");
dCSEG(TCHAR) szHELP_OPT_VERSION[]                   = TEXT("ODBC Menu Version");
dCSEG(TCHAR) szU[] 									= TEXT("%u");
dCSEG(TCHAR) szFONTNAMESIZE[]						= TEXT("%s, %d");

TCHAR 			szOutBuff[MAXOUTBUFF];

BOOL INTFUN TabbedDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,int iTab);



//*---------------------------------------------------------------------------------
//| DoResetResults:
//|	Goes through each results set and resets it.
//| Parms:
//|	Nothing
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoResetResults(VOID)
{
	lpCONNECTIONINFO		lpConn=lpConnWin;
	lpSTATEMENTINFO		lpStmt;

 	// Reset the results windows on all connection handles
	while(lpConn)
	{
		lpStmt = lpConn->lpStmt;				// Head node of statement handles

		while(lpStmt)
		{
			if(lpStmt->lpResults)
				ResetRsltWin((lpRESULTWININFO)(lpStmt->lpResults));

			lpStmt = lpStmt->next;
			if(lpStmt == lpConn->lpStmt)
				lpStmt = NULL;
		}

		lpConn = lpConn->next;
		if(lpConn == lpConnWin)
			lpConn = NULL;
		}
}


//*------------------------------------------------------------------------
//| AdjustMenu:
//|	This is the dialog proc for the Tools, User Options dialog.
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

VOID EXTFUN AdjustMenu(lpUSROPTIONS lpUsrOpts,HMENU hMenu)
{
   switch (lpUsrOpts->nODBCVer)
      {
      case MENU25:
			//ModifyMenu(hMenu,IDM_EDIT_CLEAR,MF_BYCOMMAND,IDM_EDIT_CLEAR,"Clea&r...");
			//BOOL RemoveMenu(hMenu,uPosition,MF_BYCOMMAND);
	        break;
      case MENU30:
         break;
		case MENUALL:
         break;
      }

} //AdjustMenu()



//*------------------------------------------------------------------------
//| TabMenuVerDlgProc:
//|	This is the dialog proc for the Tools, User Options tabbed
//|	dialog. It handles the "ODBC Menu Version" Tab
//|
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

LRESULT EXTFUN TabMenuVerDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	return TabbedDlgProc (hDlg, message, wParam, lParam, IDTD_USER_MENU_VER);

} //TabMenuVerDlgProc()


//*------------------------------------------------------------------------
//| TabAutOptionsDlgProc:
//|	This is the dialog proc for the Tools, User Options tabbed
//|	dialog. It handles the "Automated Options" Tab
//|
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

LRESULT EXTFUN TabAutOptionsDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{

	return TabbedDlgProc (hDlg, message, wParam, lParam, IDTD_USER_AUT_OPTIONS);

} //TabAutOptionsDlgProc()


//*------------------------------------------------------------------------
//| TabBufferLenDlgProc:
//|	This is the dialog proc for the Tools, User Options tabbed
//|	dialog. It handles the "Buffer Length" Tab
//|
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

LRESULT EXTFUN TabBufferLenDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	return TabbedDlgProc(hDlg, message, wParam, lParam, IDTD_USER_BUFFER_LEN);

} //TabBufferLenDlgProc()


//*------------------------------------------------------------------------
//| TabRowsetOptions:
//|	This is the dialog proc for the Tools, User Options tabbed
//|	dialog. It handles the "Buffer Length" Tab
//|
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

LRESULT EXTFUN TabMiscDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	return TabbedDlgProc(hDlg, message, wParam, lParam, IDTD_USER_MISC);

} //TabRowsetOptions()


//*------------------------------------------------------------------------
//| TabRowsetOptions:
//|	This is the dialog proc for the Tools, User Options tabbed
//|	dialog. It handles the "Buffer Length" Tab
//|
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

LRESULT EXTFUN TabRowsetOptions(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	return TabbedDlgProc(hDlg, message, wParam, lParam, IDTD_USER_ROWSET_OPTIONS);

} //TabRowsetOptions()


//*------------------------------------------------------------------------
//
// FUNCTION: CreatePropertySheet (HWND)
//
// PURPOSE: Creates the property sheet dialog
//
// COMMENTS:
//	  This function creates the property sheet.
//*------------------------------------------------------------------------

static DLGPROC pfnTabProc[C_PAGES] =
{
	TabAutOptionsDlgProc,	//IDTD_USER_AUT_OPTIONS
	TabMenuVerDlgProc,		//IDTD_USER_MENU_VER
	TabBufferLenDlgProc,		//IDTD_USER_BUFFER_LEN
	TabMiscDlgProc,			//IDTD_USER_MISC
	TabRowsetOptions,			//IDTD_USER_ROWSET_OPTIONS
};


//*------------------------------------------------------------------------
//
// FUNCTION: CreatePropertySheet (HWND)
//
// PURPOSE: Creates the property sheet dialog
//
// COMMENTS:
//	  This function creates the property sheet.
//*------------------------------------------------------------------------


INT_PTR INTFUN CreatePropertySheet (HWND hwndOwner,lpUSROPTIONS lpUsrOptions)
{
	PROPSHEETPAGE psp[C_PAGES];
	PROPSHEETHEADER psh;
	int i;

	// For each page, fill out a PROPSHEETPAGE structure.
	for (i = 0; i < C_PAGES; i++)
	{
		psp[i].dwSize = sizeof(PROPSHEETPAGE);		// Set the size of this structure.
		psp[i].dwFlags = PSP_HASHELP;
		psp[i].hInstance = hInst;					// The instance associated with this application
		psp[i].pszTemplate = MAKEINTRESOURCE(IDTD_TABS_BASE+i);	  // The dialog template to use
		psp[i].pszIcon = NULL;							// Don't use a special icon in the caption.
		psp[i].pfnDlgProc = pfnTabProc[i];			// The dialog procedure that handles the page
		psp[i].lParam = (INT_PTR)lpUsrOptions;
	}

	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE | PSH_HASHELP | PSH_NOAPPLYNOW;
	// Specify the parent window.
	psh.hwndParent = GetDesktopWindow() == hwndClient ? NULL : hwndClient;
	// The instance associated with this application
	psh.hInstance = hInst;
	psh.pszCaption = TEXT("User Options");
	// The number of pages in this wizard
	psh.nPages = C_PAGES;
	psh.nStartPage = 0;
	psh.ppsp = (LPCPROPSHEETPAGE) &psp;

	// Create and run the wizard.
	return (PropertySheet(&psh));
}

//*---------------------------------------------------------------------------------
//| ToolsUserOptions:
//|	This function will invoke the user options dialog which allows the user
//|	to set certain run-time aspects of the app.
//| Parms:
//|	hwnd					The parent window to use for the display
//|	lpUsrOpts			The user options structure to fill out
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN ToolsUserOptions(HWND hwnd, lpUSROPTIONS lpUsrOpts)
{
	lpUsrOpts->hwndFocus = GetFocus();

	CreatePropertySheet(hwnd, lpUsrOpts);

	// Reset focus to previous value to avoid disappearing edit cursor
	if(lpUsrOpts->hwndFocus)
		SetFocus(lpUsrOpts->hwndFocus);

} //ToolsUserOptions


//*---------------------------------------------------------------------------------
//| DoResetFont:
//|	Reset each object which uses our default font.
//| Parms:
//|	hwnd					The parent window to use for the display
//|	lpUsrOpts			The user options structure to fill out
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoResetFont(HWND hwnd, lpUSROPTIONS lpUsrOpts)
{
	HFONT						hNewFont;
	lpCONNECTIONINFO		lpConn=lpConnWin;
	lpSTATEMENTINFO		lpStmt;
	int						x = 0,
								y = 0,
								nWidth = 0,
								nHeight = 0; //BUGBUG: never intialized

	// Destroy and recreate the column font based on new face and size
	hNewFont = CreateColumnFont();
	if(hNewFont)
	{
		DeleteObject(hColumnFont);
		hColumnFont = hNewFont;
	}

	// Now destroy and recreate the default edit window font
	hNewFont = CreateEditFont();
	if(hNewFont)
	{
		DeleteObject(hEditFont);
		hEditFont = hNewFont;

		DORESETEDITWINDOW(lpUsrOpts->hwndFocus, hwndOutWin, IDW_OUTWIN, 0,
					x,y,nWidth,nHeight);

		// Reset the results windows on all connection handles
		while(lpConn)
		{
			lpStmt = lpConn->lpStmt;				// Head node of statement handles

			while(lpStmt)
			{
				if(lpStmt->lpResults)
					ResetRsltWin((lpRESULTWININFO)(lpStmt->lpResults));

				lpStmt = lpStmt->next;
				if(lpStmt == lpConn->lpStmt)
					lpStmt = NULL;
			}

			// Reset the edit windows
			DORESETEDITWINDOW(lpUsrOpts->hwndFocus, lpConn->hwndIn, IDW_TOP, 0,
						x,y,nWidth,nHeight);
			DORESETEDITWINDOW(lpUsrOpts->hwndFocus, lpConn->hwndOut, IDW_BOTTOM, WS_BORDER,
						x,y,nWidth,nHeight);

			lpConn = lpConn->next;
			if(lpConn == lpConnWin)
				lpConn = NULL;
		}
	}
	else
	{
		szMessageBox(hwnd,
				MB_ICONINFORMATION | MB_OK,
				NULL,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsCreateFontFailed),
				(LPTSTR)lpUsrOpts->szFont,
				abs(lpUsrOpts->iPtSize));
	}
}



//*------------------------------------------------------------------------
//| GetDialogInfo:
//|	This function retrieves the info from the individual Tab dialogs.
//| Parms:
//|	hDlg			handle to dialog
//|	lpUsrOpts	pointer to User Options struct.
//|	iTab			Tab ID
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
BOOL INTFUN GetDialogInfo (HWND hDlg,lpUSROPTIONS lpUsrOpts,int iTab)
{
	int dex;

	switch(iTab)
	{
		case IDTD_USER_MENU_VER:
		{
			// Chec ODBC Version number and Load Appropriate Menu
			for(dex=FIRST_VERSION_NUM;  dex<=LAST_VERSION_NUM;  dex++)
			{
				if(IsDlgButtonChecked(hDlg, dex))
				{
					lpUsrOpts->nODBCVer = dex;
               AdjustMenu(lpUsrOpts,hMenu);
					break;
				}
			}
		}
		break;

		case IDTD_USER_AUT_OPTIONS:
		{
			// Retrieve automated flags
			lpUsrOpts->fDriverMenus = IsDlgButtonChecked(hDlg, IDX_DRIVER_MENUS);
			lpUsrOpts->fAutoErrors = IsDlgButtonChecked(hDlg, IDX_AUTO_ERROR_CHECK);
			lpUsrOpts->fNullChecking = IsDlgButtonChecked(hDlg, IDX_NULL_CHECKING);
			lpUsrOpts->fBufferChecking = IsDlgButtonChecked(hDlg, IDX_BUFFER_CHECKING);
			lpUsrOpts->fVersionNumbers = IsDlgButtonChecked(hDlg, IDX_VERSION_NUMBERS);
			lpUsrOpts->fBindColZero = IsDlgButtonChecked(hDlg, IDX_BINDCOL_ZERO);
		}
		break;

		case IDTD_USER_BUFFER_LEN:
		{
			UWORD				iTmp;
			UDWORD			udTmp;
			BOOL				fValid;
			UINT				cbChar=sizeof(TCHAR);


			// First validate & range check the edit boxes, save if valid
			iTmp = (UWORD)GetDlgItemInt(hDlg,IDE_DFT_BUFF_LEN, &fValid, FALSE);
			if((!fValid) || (iTmp == 0))
			{
				szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION,
					NULL,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsDftBuffLenInvalid));
					SetFocus(GetDlgItem(hDlg,IDE_DFT_BUFF_LEN));
					return FALSE;
			}
			else
				lpUsrOpts->uDftBuffLen = iTmp*cbChar;

			iTmp = (UWORD)GetDlgItemInt(hDlg,IDE_DFT_PARM_LEN, &fValid, FALSE);
			if((!fValid) || (iTmp < MIN_PARM_LENGTH))
			{
				szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION,
					NULL,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsDftParmLenInvalid));
					SetFocus(GetDlgItem(hDlg,IDE_DFT_PARM_LEN));
					return FALSE;
			}
			else
				lpUsrOpts->uDftParmLen = iTmp*cbChar;

			udTmp = (UDWORD)GetDlgItemInt(hDlg,IDE_BIND_ALL_BUFFER, &fValid, FALSE);
			if((!fValid) || (udTmp == 0) || (udTmp > MAX_BIND_BUFF))
			{
				szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION,
					NULL,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsBindBuffInvalid));
					SetFocus(GetDlgItem(hDlg,IDE_BIND_ALL_BUFFER));
					return FALSE;
			}
			else
				lpUsrOpts->uDftBindBuff = (UWORD)udTmp*cbChar;
		}
		break;

		case IDTD_USER_MISC:
		{
			// Current selection is warning level
			lpUsrOpts->uLogLevel = (UINT)SendDlgItemMessage(hDlg, IDC_LOG_LEVEL,
							CB_GETCURSEL, 0, 0L);

			if(lpUsrOpts->uLogLevel == (UINT)(CB_ERR))
				lpUsrOpts->uLogLevel = MSG_SEV2;

			// Save temporary font information into lpUsrOptions structure
			lstrcpy(lpUsrOpts->szFont, szFont);
			lpUsrOpts->iPtSize = iPtSize;
			lpUsrOpts->iFontHeight = iHeight;

			SaveGetUserOptions(lpUsrOpts, TRUE);

			// Reset fonts if any changes were made
	  		if(lpUsrOpts->fResetFont)
	  			DoResetFont(hDlg, lpUsrOpts);
		}
		break;

		case IDTD_USER_ROWSET_OPTIONS:
		{
			UWORD uOldRows=0;

			// Retrieve flags
			lpUsrOpts->fShowStatus = IsDlgButtonChecked(hDlg, IDX_SHOW_STATUS);
			lpUsrOpts->fShowPcbs = IsDlgButtonChecked(hDlg, IDX_SHOW_PCBS);
			lpUsrOpts->fShowColumns = IsDlgButtonChecked(hDlg, IDX_COLUMN_NAMES);
			lpUsrOpts->fAutoDisplay = IsDlgButtonChecked(hDlg, IDC_ROWSET_OPT_DISPLAY_AUTO);

			// Use macro to retrieve user value or take default
			uOldRows = lpUsrOpts->uReservedRows;
			GetDlgInt(hDlg, IDE_RESERVED_ROWS, lpUsrOpts->uReservedRows, 0);
			if(uOldRows != lpUsrOpts->uReservedRows)
				szMessageBox(GetActiveWindow(),
							MB_ICONINFORMATION | MB_OK,
							(LPTSTR)szHELP_OPT_ROWSET,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsRRUntilNextTime));

			SaveGetUserOptions(lpUsrOpts, TRUE);
			DoResetResults();
		}

	} //switch(iTab)

    return TRUE;
} //GetDialogInfo()


//*------------------------------------------------------------------------
//| TabbedDlgProc:
//|	This is the dialog proc for the Tools, Options dialog.
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------

BOOL INTFUN TabbedDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,int iTab)
{
	switch(msg)
	{
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
			{
			TCHAR				szTemp[LF_FACESIZE + 5];
			HWND				hwndL;
			int				dex;
			UINT				cbChar=sizeof(TCHAR);
			lpUSROPTIONS	lpUsrOpts =(lpUSROPTIONS)((LPPROPSHEETPAGE)lParam)->lParam;

			SETWINDOWLONG (hDlg, lParam);

			lpUsrOpts->fResetFont = FALSE;

			CenterDialog(hDlg);


			// Do auto options first
			CheckDlgButton(hDlg, IDX_DRIVER_MENUS, lpUsrOpts->fDriverMenus);
			CheckDlgButton(hDlg, IDX_AUTO_ERROR_CHECK, lpUsrOpts->fAutoErrors);
			CheckDlgButton(hDlg, IDX_NULL_CHECKING, lpUsrOpts->fNullChecking);
			CheckDlgButton(hDlg, IDX_BUFFER_CHECKING, lpUsrOpts->fBufferChecking);
			CheckDlgButton(hDlg, IDX_VERSION_NUMBERS, lpUsrOpts->fVersionNumbers);
			CheckDlgButton(hDlg, IDX_BINDCOL_ZERO, lpUsrOpts->fBindColZero);
			CheckDlgButton(hDlg, IDC_ROWSET_OPT_DISPLAY_AUTO, lpUsrOpts->fAutoDisplay);

			CheckRadioButton(hDlg, FIRST_VERSION_NUM, LAST_VERSION_NUM,
								(FIRST_VERSION_NUM+(lpUsrOpts->nODBCVer % 100))-1); //Bug #679

			// Now for the limits
			wsprintf((LPTSTR)szTemp, szU, lpUsrOpts->uDftBuffLen/cbChar);
			SendDlgItemMessage(hDlg, IDE_DFT_BUFF_LEN, EM_LIMITTEXT, DFT_BUFF_LEN_CHARS, 0L);
			SetWindowText(GetDlgItem(hDlg, IDE_DFT_BUFF_LEN), (LPTSTR)szTemp);

			wsprintf((LPTSTR)szTemp, szU, lpUsrOpts->uDftParmLen/cbChar);
			SendDlgItemMessage(hDlg, IDE_DFT_PARM_LEN, EM_LIMITTEXT, DFT_PARM_LEN_CHARS, 0L);
			SetWindowText(GetDlgItem(hDlg, IDE_DFT_PARM_LEN), (LPTSTR)szTemp);

			wsprintf((LPTSTR)szTemp, szU, lpUsrOpts->uDftBindBuff/cbChar);
			SendDlgItemMessage(hDlg, IDE_BIND_ALL_BUFFER, EM_LIMITTEXT, DFT_BIND_BUFF_CHARS, 0L);
			SetWindowText(GetDlgItem(hDlg, IDE_BIND_ALL_BUFFER), (LPTSTR)szTemp);

			hwndL = GetDlgItem(hDlg, IDC_LOG_LEVEL);
			for(dex=0;  dex<MSG_SEV2+1;  dex++)
				SendMessage(hwndL, CB_ADDSTRING, 0,
							(LPARAM)(LPTSTR)GetGtrString(szTemp,
                                                         sizeof(szTemp)/sizeof(szTemp[0]), (USER_OPT_BASE + dex)));
			SendMessage(hwndL, CB_SETCURSEL, (WPARAM)lpUsrOpts->uLogLevel, 0L);

			// Copy font name and point size to temporary variables and set static text
			lstrcpy(szFont, lpUsrOpts->szFont);
			iPtSize = lpUsrOpts->iPtSize;
			iHeight = lpUsrOpts->iFontHeight;

			wsprintf((LPTSTR)szTemp, szFONTNAMESIZE, szFont, iPtSize * (-1));
			SetDlgItemText(hDlg, IDT_FONTNAME, (LPTSTR)szTemp);

			// Tell user in stat bar what we're doing
			SetStatBarText(hwndStatBar,
							GetGtrString(szOutBuff, MAXOUTBUFF, IDM_TOOLS_USER_OPTIONS),
							BLACK_TEXT);

			// Do auto options first
			CheckDlgButton(hDlg, IDX_SHOW_STATUS, lpUsrOpts->fShowStatus);
			CheckDlgButton(hDlg, IDX_SHOW_PCBS, lpUsrOpts->fShowPcbs);
			CheckDlgButton(hDlg, IDX_COLUMN_NAMES, lpUsrOpts->fShowColumns);

			// Now for the limits
			wsprintf((LPTSTR)szTemp, szU, lpUsrOpts->uReservedRows);
			SendDlgItemMessage(hDlg, IDE_RESERVED_ROWS, EM_LIMITTEXT, 1, 0L);
			SetWindowText(GetDlgItem(hDlg, IDE_RESERVED_ROWS), (LPTSTR)szTemp);

			// Tell user in stat bar what we're doing
			SetStatBarText(hwndStatBar,
							GetGtrString(szOutBuff, MAXOUTBUFF, IDM_USER_ROWSET_OPTIONS),
							BLACK_TEXT);

			}
			return TRUE;

			case WM_NOTIFY:
			{
				switch (((NMHDR *) lParam)->code)
				{
					case PSN_KILLACTIVE:
					{
						LPPROPSHEETPAGE psp = (LPPROPSHEETPAGE)GETWINDOWLONG (hDlg);
						lpUSROPTIONS	lpUsrOpts =(lpUSROPTIONS)psp->lParam;

						GetDialogInfo (hDlg, lpUsrOpts, iTab);
						return FALSE;
					}

					case PSN_RESET:
						SetWindowLong (hDlg, DWLP_MSGRESULT, FALSE);
					break;

					case PSN_SETACTIVE:
					{
						LPPROPSHEETPAGE psp = (LPPROPSHEETPAGE)GETWINDOWLONG (hDlg);
						lpUSROPTIONS	lpUsrOpts =(lpUSROPTIONS)psp->lParam;

						// init controls with connection string values
						//SetDialogInfo (hDlg, lpDlgStruct, iTab);
						return FALSE;
					}
					break;

					case PSN_APPLY:
					{
						LPPROPSHEETPAGE psp = (LPPROPSHEETPAGE)GETWINDOWLONG (hDlg);
						lpUSROPTIONS	lpUsrOpts =(lpUSROPTIONS)psp->lParam;

						return FALSE;
					}
					break;

					// Display help dialog
					case PSN_HELP:
					{
						LPPROPSHEETPAGE psp			= (LPPROPSHEETPAGE)GETWINDOWLONG (hDlg);
						lpUSROPTIONS	lpUsrOpts	=(lpUSROPTIONS)psp->lParam;
						LPCTSTR			ptszKeyword	= szHELP_OPT_ROWSET;	//Default

						//Display the help for the active tab...
						switch(iTab)
						{
							case IDTD_USER_AUT_OPTIONS:
								ptszKeyword = szHELP_OPT_AUTOMATED;
								break;

							case IDTD_USER_MENU_VER:
								ptszKeyword = szHELP_OPT_VERSION;
								break;

							case IDTD_USER_BUFFER_LEN:
								ptszKeyword = szHELP_OPT_BUFLEN;
								break;

							case IDTD_USER_MISC:
								ptszKeyword = szHELP_OPT_MISC;
								break;

							case IDTD_USER_ROWSET_OPTIONS:
								ptszKeyword = szHELP_OPT_ROWSET;
								break;

							default:
								ASSERTCHECK(FALSE, TEXT("Unknown Option Tab"));
								break;
						};

						//Invoke Help
				//		iTestHelpCnt += InvokeHelp(hDlg, (LPCTSTR)szTestHelpFile, ptszKeyword);
					}

				} //switch (((NMHDR FAR *) lParam)->code)
			}
			break;

		//--------------------------------------------------------------------
		// USER_FUNCTION_HELP:   This is sent by our hook proc to tell us
		//								the user pressed F1=Help on a menu
		//--------------------------------------------------------------------
		case USER_FUNCTION_HELP:
			{
	//		iTestHelpCnt += InvokeHelp(hDlg, (LPCTSTR)szTestHelpFile,
	//				(LPCTSTR)szHELP_OPT_USER);
			}
			return TRUE;


		case WM_COMMAND:
			{
			LPPROPSHEETPAGE psp = (LPPROPSHEETPAGE)GETWINDOWLONG (hDlg);
			lpUSROPTIONS	 lpUsrOpts = (lpUSROPTIONS)psp->lParam;

			switch(GET_WM_COMMAND_ID(wParam,lParam))
			{
            case IDCANCEL:
           		SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;

				//
				// Just reroute since we're handling this already
				//
				case IDHELP:
					SendMessage(hDlg, USER_FUNCTION_HELP, 0, 0L);
					return TRUE;


            //
            // Prompt user for a font using a customized dialog box
            //
            case IDB_FONTS:
					{
					CHOOSEFONT		cf;
					LOGFONT			lf;
					TCHAR				szTemp[LF_FACESIZE + 5];
					LPPROPSHEETPAGE psp = (LPPROPSHEETPAGE)GETWINDOWLONG (hDlg);
					lpUSROPTIONS	 lpUsrOpts = (lpUSROPTIONS)psp->lParam;

					memset(&cf, 0, sizeof(CHOOSEFONT));
					memset(&lf, 0, sizeof(LOGFONT));

					cf.lStructSize	= sizeof(CHOOSEFONT);
					cf.hwndOwner	= hDlg;
					cf.lpLogFont	= &lf;
					cf.Flags			= CF_SCREENFONTS | CF_ENABLEHOOK | CF_ENABLETEMPLATE |
											CF_LIMITSIZE | CF_INITTOLOGFONTSTRUCT;
					cf.lpfnHook		= (LPOFNHOOKPROC)Hook3d;
					cf.lpTemplateName	= MAKEINTRESOURCE(IDD_PICKFONT);
					cf.hInstance	= hInst;
					cf.nSizeMin		= 5;
					cf.nSizeMax		= 24;

					lstrcpy(lf.lfFaceName, szFont);
					lf.lfHeight = iHeight;

					if(ChooseFont(&cf))
					{
						lpUsrOpts->fResetFont = TRUE;
						lstrcpy(szFont, cf.lpLogFont->lfFaceName);
						iPtSize = -1 * ((SWORD)cf.iPointSize / 10);
						iHeight = lf.lfHeight;

						wsprintf((LPTSTR)szTemp, szFONTNAMESIZE, szFont, iPtSize * (-1));
						SetDlgItemText(hDlg, IDT_FONTNAME, (LPTSTR)szTemp);
					}

					return TRUE;
				}
			}

			}
			return TRUE;

		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;
	}
	return FALSE;
}

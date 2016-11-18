//*---------------------------------------------------------------------------------
//|	File:		SRCPICK.C
//|		Copyright (c) Microsoft Corporation
//|
//|	Purpose:
//|		This module contains the functions which will add/remove sources
//|		used for running auto tests.
//*---------------------------------------------------------------------------------
#include "gtrhdrs.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define USER_RESET_SOURCES				WM_USER + 156
#define USER_SAVE_SOURCE				WM_USER + 157
#define USER_RESET_VALUES				WM_USER + 158
#define IDT_SOURCE						IDT_GROUP

#define MAXTESTBUFF							4000				// ~300 test DLL's possible


typedef struct tagNEWSOURCEINFO {
	BOOL			fStat;
	HINSTANCE	hInst;
	TCHAR			szNewSource[SQL_MAX_DSN_LENGTH+1];
	} NEWSOURCEINFO;


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE		hThisInst;						// This libraries instance
extern TCHAR 		g_szGatorIni[MAXFILELEN];



//----------------------------------------------------------------------------------
//		Local function prototypes
//----------------------------------------------------------------------------------
BOOL EXTFUN ManageSourcesWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
VOID INTFUN InitToDataSources(HWND hwnd0);
BOOL INTFUN GetNewSourceName(HWND hwnd, HINSTANCE hInst, LPTSTR szNewSource);
BOOL EXTFUN GetNewSourceNameWndProc(HWND		hDlg,
										unsigned msg,
										WORD		wParam,
										LONG		lParam);




//*------------------------------------------------------------------------
//|  ManageSources:
//|		This function will invoke the Manage Sources dialog box which
//|		the user may use to edit sources for testing.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL*
//|							*The hInstance of this loaded library must be used
//|							to load the dialog.
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
void EXTFUN ManageSources(HWND hwnd, HINSTANCE hInst)
{
	DLGPROC 				dlgproc;
	HINSTANCE * 	lpHINST=(HINSTANCE *)&hInst;

	//Set gator32.ini path if odbctest hasn't set it
	SetGatorIniPath(g_szGatorIni);

	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)ManageSourcesWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_MANAGE_SOURCES), hwnd, dlgproc,
			(LPARAM)(HINSTANCE *)lpHINST))
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_MANAGE_SOURCES");
	FreeProcInstance((FARPROC) dlgproc);
}




//*------------------------------------------------------------------------
//|  ManageSourcesWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN ManageSourcesWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE	* lpHINST;
	static int					cSources;
	static TCHAR					szTmpName[SQL_MAX_DSN_LENGTH+1];

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			{
			int				iCnt;

			Busy(TRUE);
			lpHINST = (HINSTANCE *)lParam;
			CenterDialog(hDlg);

			// Defaults for sources are gathered form SQLDataSources.
			//		If no DSNs are installed, user may not procede.
			InitToDataSources(GetDlgItem(hDlg, IDC_DSN0));
			iCnt = (int)SendDlgItemMessage(hDlg, IDC_DSN0, CB_GETCOUNT, 0, 0L);
			if(iCnt == CB_ERR || iCnt == 0) {
				szMessageBox(hDlg,
								MB_ICONINFORMATION | MB_OK,
								(LPTSTR)szErrTitle,
								GetString(szErrBuff, MAXOUTBUFF, idsNoDSNsInstalled));
				SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
				EnableWindow(GetDlgItem(hDlg, IDB_NEW), FALSE);
				}
			else {
				SendDlgItemMessage(hDlg, IDC_DSN0, CB_SETCURSEL, 0, 0L);

				// Set further restrictions
				SendDlgItemMessage(hDlg, IDC_DSN0, CB_LIMITTEXT, SQL_MAX_DSN_LENGTH, 0L);
				SendDlgItemMessage(hDlg, IDE_UID0, EM_LIMITTEXT, MAX_USER_INFO, 0L);
				SendDlgItemMessage(hDlg, IDE_PWD0, EM_LIMITTEXT, MAX_USER_INFO, 0L);
				SendDlgItemMessage(hDlg, IDE_KEYWORDS, EM_LIMITTEXT, MAX_KEYWORD_LEN, 0L);

				// Now set the default lists
				SendMessage(hDlg, USER_RESET_SOURCES, 0, 0L);
				}

			Busy(FALSE);
			}
			return TRUE;


		//  User message to enable/disable buttons based on counts
		case USER_SET_BUTTON_STATE:
			{
			UINT		dex;
			EnableWindow(GetDlgItem(hDlg, IDB_DELETE), cSources);
			EnableWindow(GetDlgItem(hDlg, IDT_TEST_SOURCES), cSources);
			EnableWindow(GetDlgItem(hDlg, IDC_SOURCES), cSources);

			for(dex=FIRST_SRC_CONTROL;  dex<=LAST_SRC_CONTROL;  dex++)
				EnableWindow(GetDlgItem(hDlg, dex), cSources);
			if(!cSources)
				SetFocus(GetDlgItem(hDlg, IDB_NEW));
			}
			return TRUE;


		// User message to refresh the group list
		case USER_RESET_SOURCES:
			{
			static 	LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDC_SOURCES);

			cSources = 0;
			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
			if(!astr)
				return TRUE;
			SendMessage(hwnd, CB_RESETCONTENT, 0, 0L);
			if(GetTestSources(str, MAXTESTBUFF))
				while(*str) {
					SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)str);
					str += lstrlen(str) + 1;
					++cSources;
					}
			ReleaseMemory(astr);
			SendMessage(hwnd, CB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_RESET_VALUES, 0, 0L);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
			}
			return TRUE;

		// User message to reset the values for the current source
		case USER_RESET_VALUES:
			{
			static TCHAR 		szStr[MAX_USER_INFO+1];
         TCHAR					szSource[SQL_MAX_DSN_LENGTH+1];
         TCHAR					szKeys[MAX_KEYWORD_LEN+1];
         WPARAM				dex;

         *szStr = *szSource = *szKeys = '\0';

			GetCBText(GetDlgItem(hDlg, IDC_SOURCES), szSource);

			// Given source, find other fields and set controls
			GetServer0(szSource, szStr, sizeof(szStr));
			dex = (WPARAM)SendDlgItemMessage(hDlg, IDC_DSN0, CB_FINDSTRING, 0, (LPARAM)(LPTSTR)szStr);
			if(dex != LB_ERR)
				SendDlgItemMessage(hDlg, IDC_DSN0, CB_SETCURSEL, dex, 0L);
			GetLogin0(szSource, szStr, sizeof(szStr));
			SetWindowText(GetDlgItem(hDlg, IDE_UID0), szStr);
			GetPassword0(szSource, szStr, sizeof(szStr));
			SetWindowText(GetDlgItem(hDlg, IDE_PWD0), szStr);
			GetKeywords(szSource, szKeys, sizeof(szKeys)/sizeof(szKeys[0]));
			SetWindowText(GetDlgItem(hDlg, IDE_KEYWORDS), szKeys);
			}
			return TRUE;

		// Sent when we need to save our source
		case USER_SAVE_SOURCE:
			{
			static TCHAR 		szStr[MAX_USER_INFO+1];
         TCHAR					szSource[SQL_MAX_DSN_LENGTH+1];
         TCHAR					szKeys[MAX_KEYWORD_LEN+1];

         *szStr = *szSource = *szKeys = '\0';

			GetCBText(GetDlgItem(hDlg, IDC_SOURCES), szSource);

			if(*szSource) {
				// Do the source 0
				GetCBText(GetDlgItem(hDlg, IDC_DSN0), szStr);
				SetServer0(szSource, szStr);
				GetText(GetDlgItem(hDlg, IDE_UID0), szStr);
				SetLogin0(szSource, szStr);
				GetText(GetDlgItem(hDlg, IDE_PWD0), szStr);
				SetPassword0(szSource, szStr);
				GetText(GetDlgItem(hDlg, IDE_KEYWORDS), szKeys);
				SetKeywords(szSource, szKeys);
				}
			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				case IDB_NEW:
					{
					TCHAR		szTmpName[SQL_MAX_DSN_LENGTH+1];
					int		iDex=0;

					SendMessage(hDlg, USER_SAVE_SOURCE, 0, 0L);
					if(GetNewSourceName(hDlg, *lpHINST, szTmpName)) {
						++cSources;
						iDex = (int)SendDlgItemMessage(hDlg, IDC_SOURCES, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szTmpName);
						if(iDex != CB_ERR)
							SendDlgItemMessage(hDlg, IDC_SOURCES, CB_SETCURSEL,
									(WPARAM)iDex, 0L);
						SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
						SendMessage(hDlg, USER_RESET_VALUES, 0, 0L);
						}
					}
					return TRUE;

				// For delete, get name of test and verify user wants to remove it
				case IDB_DELETE:
					{
					TCHAR		szTmpName[SQL_MAX_DSN_LENGTH+1];
					SendMessage(hDlg, USER_SAVE_SOURCE, 0, 0L);
					GetCBText(GetDlgItem(hDlg, IDC_SOURCES), szTmpName);
					if(IDOK == szMessageBox(hDlg,
									MB_ICONQUESTION | MB_OKCANCEL,
									(LPTSTR)szTestTitle,
									GetString(szErrBuff, MAXOUTBUFF, idsDeleteSource),
									szTmpName)) {
						RemoveSourceName(szTmpName);
						SendMessage(hDlg, USER_RESET_SOURCES, 0, 0L);
						}
					}
					return TRUE;

            case IDCANCEL:
            	SendMessage(hDlg, USER_SAVE_SOURCE, 0, 0L);
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;
				}
			switch(GET_WM_COMMAND_CODE(wParam,lParam)) {
				case CBN_SELCHANGE:
					{
					if(GET_WM_COMMAND_ID(wParam, lParam) == IDC_SOURCES) {
						SendMessage(hDlg, USER_RESET_VALUES, 0, 0L);
						}
					else
						SendMessage(hDlg, USER_SAVE_SOURCE, 0, 0L);
					}
					return TRUE;

				// If an edit field has changed, save the DSN
				case EN_KILLFOCUS:
					{
					HWND			hwnd = GetDlgItem(hDlg, GET_WM_COMMAND_ID(wParam, lParam));

					if(SendMessage(hwnd, EM_GETMODIFY, 0, 0L))
						SendMessage(hDlg, USER_SAVE_SOURCE, 0, 0L);
					}
					break;

/*				case CBN_KILLFOCUS:
					SendMessage(hDlg, USER_SAVE_SOURCE, 0, 0L);
					break;
*/				}
			return FALSE;

		// Must unregister 3D stuff and end dialog
		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;

		default:
			return FALSE;
		}
}


//*------------------------------------------------------------------------
//|  InitToDataSources:
//|		Use the SQLDataSource function to find our defaults.
//|	Parms:
//|		hwnd0			First window to populate
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN InitToDataSources(HWND hwnd0)//, HWND hwnd1)
{
	HENV		henv;
	RETCODE	rc;
	SQLTCHAR	szDSN[SQL_MAX_DSN_LENGTH + 1];

	if(SQL_SUCCESS != SQLAllocEnv(&henv))
		return;

	rc = SQLDataSources(henv, SQL_FETCH_FIRST,
				szDSN, sizeof(szDSN)/sizeof(szDSN[0]), NULL,
				NULL, 0, NULL);

	while(rc == SQL_SUCCESS)
	{
		SendMessage(hwnd0, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szDSN);

		rc = SQLDataSources(henv, SQL_FETCH_NEXT,
					szDSN, sizeof(szDSN)/sizeof(szDSN[0]), NULL,
					NULL, 0, NULL);
	}

	SQLFreeEnv(henv);
}


//*------------------------------------------------------------------------
//|  GetNewSourceName:
//|		This function will prompt the user for a new group name.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL
//|	Returns:
//|		TRUE if a new name has been added to our list
//*------------------------------------------------------------------------
BOOL INTFUN GetNewSourceName(HWND hwnd, HINSTANCE hInst, LPTSTR szNewSource)
{
	DLGPROC 				dlgproc;
	HINSTANCE * 	lpHINST=(HINSTANCE *)&hInst;
	NEWSOURCEINFO		ng;

	ng.hInst = hInst;
	ng.szNewSource[0] = '\0';
	ng.fStat = 0;

	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)GetNewSourceNameWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_NEW_SOURCE), hwnd, dlgproc,
			(LPARAM)(NEWSOURCEINFO *)&ng))
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_NEW_SOURCE");
	FreeProcInstance((FARPROC) dlgproc);
	if(ng.fStat) {
		lstrcpy(szNewSource, ng.szNewSource);
		return TRUE;
		}
	else
		return FALSE;
}




//*------------------------------------------------------------------------
//|  GetNewSourceNameWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN GetNewSourceNameWndProc(HWND		hDlg,
										unsigned msg,
										WORD		wParam,
										LONG		lParam)
{
	static NEWSOURCEINFO *	ng;

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			Busy(TRUE);
			ng = (NEWSOURCEINFO *)(LONG_PTR)lParam;
			CenterDialog(hDlg);
			SendDlgItemMessage(hDlg, IDE_SOURCE, EM_LIMITTEXT, SQL_MAX_DSN_LENGTH, 0L);
			Busy(FALSE);
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				case IDOK:
					{
					TCHAR  szExists[SQL_MAX_DSN_LENGTH+1];
					GetText(GetDlgItem(hDlg, IDE_SOURCE), szExists);
					if(IsValidTestName(szExists)) {
						switch(GetIniNameUsage(szExists)) {
							case INI_AUTO_TEST:
							case INI_GROUP_NAME:
								szMessageBox(hDlg,
										MB_OK | MB_ICONINFORMATION,
										(LPTSTR)szTestTitle,
										GetString(szErrBuff, MAXOUTBUFF, idsSourceNameInUse),
										szExists);
								return TRUE;

							case INI_DATA_SOURCE:
								if(IDOK != szMessageBox(hDlg,
										MB_ICONQUESTION | MB_OKCANCEL,
										(LPTSTR)szTestTitle,
										GetString(szErrBuff, MAXOUTBUFF, idsReplaceSource),
										szExists))
									return TRUE;
								break;
							}
						// Either it didn't exist or they wanted to replace it
						RemoveSourceName(szExists);
						SetSourceName(szExists);
						lstrcpy(ng->szNewSource, szExists);
						ng->fStat = TRUE;
						SendMessage(hDlg, WM_CLOSE, 0, 0L);
						}
					else
						szMessageBox(hDlg,
								MB_ICONINFORMATION | MB_OK,
								(LPTSTR)szErrTitle,
								GetString((LPTSTR)szErrBuff, MAXOUTBUFF, idsAutoTestNameInvalid),
								szExists);
					}
					return TRUE;

            case IDCANCEL:
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;
				}
			return FALSE;

		// Must unregister 3D stuff and end dialog
		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;

		default:
			return FALSE;
		}
}

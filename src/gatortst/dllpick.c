//*---------------------------------------------------------------------------------
//|	File:		DLLPICK.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module contains the functions which can be used to add/remove
//|		test DLL's from gator.ini.
//*---------------------------------------------------------------------------------
#include "gtrhdrs.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define MAXBUFF								256
#define MAXTESTBUFF							4000				// ~300 test DLL's possible

#define USER_SET_BUTTON_STATE			WM_USER + 151
#define USER_RESET_INSTALLED			WM_USER + 152
#define USER_RESET_AVAILABLE			WM_USER + 153
#define USER_ADD_ITEM 					WM_USER + 154
#define USER_REMOVE_ITEM				WM_USER + 155


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE		hThisInst;						// This libraries instance
extern TCHAR 		g_szGatorIni[MAXFILELEN];
dCSEG(TCHAR)	szDLL_EXT[]	=	TEXT("\\*.DLL");
dCSEG(TCHAR)	szFileSep[]	=	TEXT("\\");
dCSEG(TCHAR)	szEOS[]		=	TEXT("");


//----------------------------------------------------------------------------------
//		Local function prototypes
//----------------------------------------------------------------------------------
VOID INTFUN RemoveTestsFromGroups(LPTSTR szTest);


//*------------------------------------------------------------------------
//|  ManageTests:
//|		This function displays the Manage Tests dialog box which allows
//|		the user to add and remove auto test dll's from gator.ini.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL*
//|							*The hInstance of this loaded library must be used
//|							to load the dialog.
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
void EXTFUN ManageTests(HWND hwnd, HINSTANCE hInst)
{
	DLGPROC 				dlgproc;
	HINSTANCE * 	lpHINST=(HINSTANCE *)&hInst;
	TCHAR					szCurDir[MAXFILELEN];

   GetCurrentDirectory(MAXFILELEN, szCurDir);

	//Set gator32.ini path if odbctest hasn't set it
	SetGatorIniPath(g_szGatorIni);

	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)ManageTestsWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_MANAGE_TESTS), hwnd, dlgproc,
			(LPARAM)(HINSTANCE *)lpHINST))
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_RUN_TESTS");
	FreeProcInstance((FARPROC) dlgproc);
	SetCurrentDirectory(szCurDir);
}


//*---------------------------------------------------------------------------------
//| SetTestPathInfo:
//|	Based on current selection, set the static text info.
//| Parms:
//|	hDlg							Dialog handle
//|	hwndList						Handle of list box with current sel
//|	head							Head node of autotests
//| Returns:
//|	The auto test node which matches the selection
//*---------------------------------------------------------------------------------
lpAUTOTESTINFO SetTestPathInfo(HWND hDlg, HWND hwndList, lpAUTOTESTINFO	head)
{
	lpAUTOTESTINFO		ati=NULL;
	TCHAR					szTmpName[MAXTESTNAME];

	SendMessage(hwndList, LB_GETTEXT,
		(WPARAM)SendMessage(hwndList, LB_GETCURSEL, 0, 0L),
		(LPARAM)(LPTSTR)szTmpName);
	ati = FindAutoTestNode(head, szTmpName);
	if(ati) {
		SetWindowText(GetDlgItem(hDlg, IDT_NAME), ati->szTestName);
		DoSetFile(GetDlgItem(hDlg, IDT_DLL), (LPCTSTR)ati->szPath);
		}
	return ati;
}


//*------------------------------------------------------------------------
//|  ManageTestsWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN ManageTestsWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static lpAUTOTESTINFO	head;
	static lpAUTOTESTINFO	ati;
	static HINSTANCE	* lpHINST;
	static TCHAR					szFrom[MAXFILELEN];
	static BOOL					fRefresh;
	static int					cbAvail, cbInstall;
	static TCHAR					szTmpName[MAXTESTNAME];

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			lpHINST = (HINSTANCE *)lParam;
			head = ati = NULL;
			CenterDialog(hDlg);

			// Set defaults and limits for controls
			if(!*szFrom)
				if(!GetDftFromDir(szFrom, sizeof(szFrom)/sizeof(TCHAR)))
					GetCurrentDirectory(sizeof(szFrom)/sizeof(TCHAR), szFrom);

			DoSetFile(GetDlgItem(hDlg, IDT_FROM), (LPCTSTR)szFrom);

			// Now set the default lists
			SendMessage(hDlg, USER_RESET_INSTALLED, 0, 0L);
			SendMessage(hDlg, USER_RESET_AVAILABLE, 0, 0L);
			return TRUE;


		//  User message to enable/disable buttons based on counts
		case USER_SET_BUTTON_STATE:
			EnableWindow(GetDlgItem(hDlg, IDB_ADD), cbAvail);
			EnableWindow(GetDlgItem(hDlg, IDB_REMOVE), cbInstall);
			if(!cbInstall) {
				SetWindowText(GetDlgItem(hDlg, IDT_NAME), szEOS);
				SetWindowText(GetDlgItem(hDlg, IDT_DLL), szEOS);
				}
			return TRUE;

		// User message to refresh both the installed and available lists
		case USER_RESET_INSTALLED:
			{
			fRefresh = FALSE;
			cbInstall = RefreshInstalledTestList(hDlg, IDL_INSTALLED, &head);

			if(cbInstall) {
				SendDlgItemMessage(hDlg, IDL_INSTALLED, LB_SETCURSEL,
							0, 0L);
				ati = SetTestPathInfo(hDlg, GetDlgItem(hDlg, IDL_INSTALLED), head);
				}
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
			}
			return TRUE;


		// Now for the available list of DLLs
		case USER_RESET_AVAILABLE:
			{
			static	TCHAR 		szFromDir[MAXFILELEN];
			HWND		hwnd;

			lstrcpy(szFromDir, szFrom);
			lstrcat(szFromDir, szDLL_EXT);
			hwnd = GetDlgItem(hDlg, IDL_AVAILABLE);
			SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
			SendMessage(hwnd, LB_DIR,
						DDL_READWRITE, (LPARAM)(LPTSTR)szFromDir);
			cbAvail = (int)SendMessage(hwnd, LB_GETCOUNT, 0, 0L);
			SendMessage(hwnd, LB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
			}
			return TRUE;


		// User message sent by EnumerateListBox for each selected item to add
		//		Make sure the item is not alreay in the .INI file
		case USER_ADD_ITEM:
			{
			LPTSTR		szFileName=(LPTSTR)lParam;
			static 	TCHAR szPath[MAXFILELEN];
			static	TCHAR szName[MAXTESTNAME];
			int		rtnstatus;

			Busy(TRUE);
			lstrcpy(szPath, szFrom);				// Get full path of selected file
			lstrcat(szPath, szFileSep);
			lstrcat(szPath, szFileName);
			*szName = '\0';						// Not yet known

			// Add based on path.  Verify path on return to catch case where another test
			// with that name was already installed
			if((ati = AddAutoTest(&head, szName, szPath, TRUE, &rtnstatus))) {
				if(rtnstatus & ADD_AT_PATH_DIFFERS ||
					rtnstatus & ADD_AT_ALREADY_IN_LIST)
					szMessageBox(hDlg,
									MB_ICONINFORMATION | MB_OK,
									(LPTSTR)szErrTitle,
									GetString(szErrBuff, MAXOUTBUFF, idsAutoTestAlreadyInstalled),
									szName);
				else {
					// Not installed yet, make sure it isn't in use elsewhere
					TCHAR	tmp[5];
					UINT fUsage = GetIniNameUsage(szName);
					if(fUsage == INI_DATA_SOURCE || fUsage == INI_GROUP_NAME) {
							szMessageBox(hDlg,
									MB_OK | MB_ICONINFORMATION,
									(LPTSTR)szTestTitle,
									GetString(szErrBuff, MAXOUTBUFF, idsAutoNameInUse),
									ati->szTestName);
							DeleteAutoTestNode(&head, ati);
							Busy(FALSE);
							return TRUE;
						}
					AddTestNameToIni((LPTSTR)szName, _itot(ati->cbTestCases, tmp, 10));
					SetTestDLLPath((LPTSTR)szName, ati->szPath);
					fRefresh = TRUE;					// Need to refresh when all done
					}
				}
			Busy(FALSE);
			}
			return TRUE;

		// User message sent by EnumerateListBox for each selected item to remove.
		// Use name to verify it is still in .INI, if so make sure we can delete
		// it.  To delete, remove name from [Auto Tests], its section, and from list.
		case USER_REMOVE_ITEM:
			{
			LPTSTR		szName=(LPTSTR)lParam;
			if(INI_AUTO_TEST == GetIniNameUsage(szName)) {
				RemoveTestSectionFromIni(szName);
				RemoveTestNameFromIni(szName);
				RemoveTestsFromGroups(szName);
				DeleteAutoTestNode(&head, FindAutoTestNode(head, szName));
				fRefresh = TRUE;
				}
			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				case IDB_FROM:
					if(BrowseDirs(hDlg, hThisInst, NULL, szFrom)) {
						DoSetFile(GetDlgItem(hDlg, IDT_FROM), (LPCTSTR)szFrom);
						SendMessage(hDlg, USER_RESET_AVAILABLE, 0, 0L);
						SetDftFromDir(szFrom);
						}
					return TRUE;

				case IDB_ADD:
					EnumerateListBox(hDlg, GetDlgItem(hDlg, IDL_AVAILABLE), USER_ADD_ITEM);
					if(fRefresh)
						SendMessage(hDlg, USER_RESET_INSTALLED, 0, 0L);
					return TRUE;

				case IDB_REMOVE:
					EnumerateListBox(hDlg, GetDlgItem(hDlg, IDL_INSTALLED), USER_REMOVE_ITEM);
					if(fRefresh)
						SendMessage(hDlg, USER_RESET_INSTALLED, 0, 0L);
					return TRUE;

            case IDCANCEL:
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;
				}
			switch(GET_WM_COMMAND_CODE(wParam,lParam)) {
				case LBN_SELCHANGE:		// List box selection has changed
					{
					UINT	wID = GET_WM_COMMAND_ID(wParam,lParam);
					if(wID == IDL_INSTALLED)
						ati = SetTestPathInfo(hDlg, GetDlgItem(hDlg, IDL_INSTALLED), head);
					else
						return FALSE;
					}
					return TRUE;
				}
			return FALSE;

		// Must unregister 3D stuff and end dialog
		case WM_CLOSE:
			DeleteAutoTestList(head);
			EndDialog(hDlg, FALSE);
			return TRUE;

		default:
			return FALSE;
		}
}



//*------------------------------------------------------------------------
//|  RemoveTestsFromGroups:
//|		When a test is removed, we need to remove it from all groups
//|		as well to keep integrity in place.
//|	Parms:
//|		szTest				The test being removed
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN RemoveTestsFromGroups(LPTSTR szTest)
{
	LPTSTR 		astr, str;

	astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
	if(!str)
		return;

	if(GetGroupList(str, MAXTESTBUFF)) {
		while(*astr) {
			RemoveGroupAutoTest(astr, szTest);
			astr += lstrlen(astr) + 1;
			}
		}

   ReleaseMemory(str);
}

//*---------------------------------------------------------------------------------
//|	File:		GRPPICK.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module contains the functions which can be used to add/remove
//|		groups as well as auto tests for groups.
//*---------------------------------------------------------------------------------
#include "gtrhdrs.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define MAXBUFF								256
#define MAXTESTBUFF							4000				// ~300 test DLL's possible


typedef struct tagNEWGROUPINFO {
	BOOL			fStat;
	HINSTANCE	hInst;
	TCHAR			szNewGroup[AUTO_MAX_TEST_NAME];
	} NEWGROUPINFO;


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE		hThisInst;						// This libraries instance
extern TCHAR 		g_szGatorIni[MAXFILELEN];



//----------------------------------------------------------------------------------
//		Local function prototypes
//----------------------------------------------------------------------------------
BOOL INTFUN GetNewGroupName(HWND hwnd, HINSTANCE hInst, LPTSTR szNewGroup);




//*------------------------------------------------------------------------
//|  ManageGroups:
//|		This function will invoke the Manage Groups dialog box which
//|		the user may use to edit groups.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL*
//|							*The hInstance of this loaded library must be used
//|							to load the dialog.
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
void EXTFUN ManageGroups(HWND hwnd, HINSTANCE hInst)
{
	DLGPROC 				dlgproc;
	HINSTANCE * 	lpHINST=(HINSTANCE *)&hInst;

	//Set gator32.ini path if odbctest hasn't set it
	SetGatorIniPath(g_szGatorIni);

	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)ManageGroupsWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_MANAGE_GROUPS), hwnd, dlgproc,
			(LPARAM)(HINSTANCE *)lpHINST))
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_RUN_GROUPS");
	FreeProcInstance((FARPROC) dlgproc);
}




//*------------------------------------------------------------------------
//|  ManageGroupsWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN ManageGroupsWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE	* lpHINST;
	static int					cbAvail, cbInstall, cbGroups;
	static TCHAR					szTmpName[AUTO_MAX_TEST_NAME];

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			Busy(TRUE);
			lpHINST = (HINSTANCE *)lParam;
			CenterDialog(hDlg);

			// Now set the default lists
			SendMessage(hDlg, USER_RESET_GROUPS, 0, 0L);
			Busy(FALSE);
			return TRUE;


		//  User message to enable/disable buttons based on counts
		case USER_SET_BUTTON_STATE:
			// Select groups only when present
			EnableWindow(GetDlgItem(hDlg, IDT_GROUP), cbGroups);
			EnableWindow(GetDlgItem(hDlg, IDC_GROUPS), cbGroups);

			// Set installed items
			EnableWindow(GetDlgItem(hDlg, IDT_GROUP_INSTALLED), cbInstall && cbGroups);
			EnableWindow(GetDlgItem(hDlg, IDL_INSTALLED), cbInstall && cbGroups);

			// Set available items
			EnableWindow(GetDlgItem(hDlg, IDT_GROUP_AVAIL), cbAvail && cbGroups);
			EnableWindow(GetDlgItem(hDlg, IDL_AVAILABLE), cbAvail && cbGroups);

			// Buttons enabled on if there are groups
			EnableWindow(GetDlgItem(hDlg, IDB_ADD), cbAvail && cbGroups);
			EnableWindow(GetDlgItem(hDlg, IDB_REMOVE), cbInstall && cbGroups);
			EnableWindow(GetDlgItem(hDlg, IDB_DELETE), cbGroups);
			return TRUE;


		// User message to refresh the group list
		case USER_RESET_GROUPS:
			{
			static 	LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDC_GROUPS);

			cbGroups = 0;
			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
			if(!astr)
				return TRUE;
			SendMessage(hwnd, CB_RESETCONTENT, 0, 0L);
			if(GetGroupList(str, MAXTESTBUFF))
				while(*str) {
					SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)str);
					str += lstrlen(str) + 1;
					++cbGroups;
					}
			ReleaseMemory(astr);
			SendMessage(hwnd, CB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_RESET_INSTALLED, 0, 0L);
			}
			return TRUE;

		// User message to refresh both the installed and available lists
		case USER_RESET_INSTALLED:
			{
			static 	LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDL_INSTALLED);
			LPTSTR		gstr=szTmpName;				// No pun intended

			cbInstall = 0;
			SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
			if(lParam)
				gstr = (LPTSTR)lParam;
			else
				GetText(GetDlgItem(hDlg, IDC_GROUPS), gstr);
			if(!*gstr) {
				SendMessage(hDlg, USER_RESET_AVAILABLE, 0, 0L);
				return TRUE;
				}
			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
			if(!astr)
				return TRUE;
			if(GetGroupAutoTests(gstr, str, MAXTESTBUFF))
				while(*str) {
					SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)(LPTSTR)str);
					str += lstrlen(str) + 1;
					++cbInstall;
					}
			ReleaseMemory(astr);
			SendMessage(hwnd, LB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_RESET_AVAILABLE, 0, 0L);
			}
			return TRUE;

		// Now for the available list of DLLs, the Installed list must be
		//		done before this message as we use it to exclude items
		case USER_RESET_AVAILABLE:
			{
			static 	LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDL_AVAILABLE);
			HWND		hwndx=GetDlgItem(hDlg, IDL_INSTALLED);

			cbAvail = 0;
			SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
			if(!astr)
				return TRUE;
			if(GetInstalledTestList(str, MAXTESTBUFF))
				while(*str) {
					if(LB_ERR == SendMessage(hwndx, LB_FINDSTRING, 0, (LPARAM)(LPTSTR)str))
						SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)(LPTSTR)str);
					str += lstrlen(str) + 1;
					++cbAvail;
					}
			ReleaseMemory(astr);
			SendMessage(hwnd, LB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
			}
			return TRUE;


		// User message sent by EnumerateListBox for each selected item to add
		//		Make sure the item is not alreay in the .INI file
		case USER_ADD_ITEM:
			{
			GetText(GetDlgItem(hDlg, IDC_GROUPS), szTmpName);

			// Move from available to installed
			SendDlgItemMessage(hDlg, IDL_INSTALLED, LB_ADDSTRING, 0, lParam);
			SendDlgItemMessage(hDlg, IDL_AVAILABLE, LB_DELETESTRING, wParam, 0L);
			++cbInstall;
			--cbAvail;

			// Add auto test to the group
			AddGroupAutoTest(szTmpName, (LPTSTR)lParam);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);

			// If there are no more left, don't leave focus on disabled button
			if(!cbAvail)
				SetFocus(GetDlgItem(hDlg, IDCANCEL));
			}
			return TRUE;


		// User message sent by EnumerateListBox for each selected item to remove.
		// Use name to verify it is still in .INI, if so make sure we can delete
		// it.  To delete, remove name from [Auto Tests], its section, and from list.
		case USER_REMOVE_ITEM:
			{
			GetText(GetDlgItem(hDlg, IDC_GROUPS), szTmpName);

			// Move from available to installed
			SendDlgItemMessage(hDlg, IDL_AVAILABLE, LB_ADDSTRING, 0, lParam);
			SendDlgItemMessage(hDlg, IDL_INSTALLED, LB_DELETESTRING, wParam, 0L);
			++cbAvail;
			--cbInstall;

			// Add auto test to the group
			RemoveGroupAutoTest(szTmpName, (LPTSTR)lParam);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);

			// If there are no more left, don't leave focus on disabled button
			if(!cbInstall)
				SetFocus(GetDlgItem(hDlg, IDCANCEL));
			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				case IDB_NEW:
					{
					if(GetNewGroupName(hDlg, *lpHINST, szTmpName)) {
						++cbGroups;
						SendDlgItemMessage(hDlg, IDC_GROUPS, CB_SETCURSEL,
								(WPARAM)SendDlgItemMessage(hDlg, IDC_GROUPS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szTmpName),
								0L);
						SendMessage(hDlg, USER_RESET_INSTALLED, 0, 0L);
						}
					}
					return TRUE;

				case IDB_DELETE:
					{
					GetText(GetDlgItem(hDlg, IDC_GROUPS), szTmpName);
					if(IDOK == szMessageBox(hDlg,
									MB_ICONQUESTION | MB_OKCANCEL,
									(LPTSTR)szTestTitle,
									GetString(szErrBuff, MAXOUTBUFF, idsDeleteGroup),
									szTmpName)) {
						RemoveGroup(szTmpName);
						RemoveGroupFromIni(szTmpName);
						SendMessage(hDlg, USER_RESET_GROUPS, 0, 0L);
						}
					}
					return TRUE;

				case IDB_ADD:
					EnumerateListBox(hDlg, GetDlgItem(hDlg, IDL_AVAILABLE), USER_ADD_ITEM);
					return TRUE;

				case IDB_REMOVE:
					EnumerateListBox(hDlg, GetDlgItem(hDlg, IDL_INSTALLED), USER_REMOVE_ITEM);
					return TRUE;

            case IDCANCEL:
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;
				}
			switch(GET_WM_COMMAND_CODE(wParam,lParam)) {
/*				case CBN_EDITCHANGE:
				case CBN_EDITUPDATE:
					{
					HWND	hwnd=GET_WM_COMMAND_HWND(wParam,lParam);
					SendMessage(hwnd, CB_GETLBTEXT,
							(WPARAM)SendMessage(hwnd, CB_GETCURSEL, 0, 0L),
							(LPARAM)(LPTSTR)szTmpName);
					SetWindowText(hwnd, (LPTSTR)szTmpName);
					}
					return TRUE;
*/
				case CBN_SELCHANGE:		// List box selection has changed
					{
					if(GET_WM_COMMAND_ID(wParam,lParam) == IDC_GROUPS) {
						HWND hwnd=GetDlgItem(hDlg, IDC_GROUPS);
						SendMessage(hwnd, CB_GETLBTEXT,
							(WPARAM)SendMessage(hwnd, CB_GETCURSEL, 0, 0L),
							(LPARAM)(LPTSTR)szTmpName);
						SendMessage(hDlg, USER_RESET_INSTALLED, 0, (LPARAM)(LPTSTR)szTmpName);
						}
					else
						return FALSE;
					}
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


//*------------------------------------------------------------------------
//|  GetNewGroupName:
//|		This function will prompt the user for a new group name.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL
//|	Returns:
//|		TRUE if a new name has been added to our list
//*------------------------------------------------------------------------
BOOL INTFUN GetNewGroupName(HWND hwnd, HINSTANCE hInst, LPTSTR szNewGroup)
{
BOOL EXTFUN GetNewGroupNameWndProc(HWND		hDlg,
										unsigned msg,
										WORD		wParam,
										LONG		lParam);
	DLGPROC 				dlgproc;
	HINSTANCE * 	lpHINST=(HINSTANCE *)&hInst;
	NEWGROUPINFO		ng;

	ng.hInst = hInst;
	ng.szNewGroup[0] = '\0';
	ng.fStat = 0;

	dlgproc = (DLGPROC)MakeProcInstance((FARPROC)GetNewGroupNameWndProc, hThisInst);
	if(-1 == DialogBoxParam(hThisInst, MAKEINTRESOURCE(IDD_NEW_GROUP), hwnd, dlgproc,
			(LPARAM)(NEWGROUPINFO *)&ng))
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_NEW_GROUP");
	FreeProcInstance((FARPROC) dlgproc);
	if(ng.fStat) {
		lstrcpy(szNewGroup, ng.szNewGroup);
		return TRUE;
		}
	else
		return FALSE;
}




//*------------------------------------------------------------------------
//|  GetNewGroupNameWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN GetNewGroupNameWndProc(HWND		hDlg,
										unsigned msg,
										WORD		wParam,
										LONG		lParam)
{
	static NEWGROUPINFO *	ng;

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			Busy(TRUE);
			ng = (NEWGROUPINFO *)(LONG_PTR)lParam;
			CenterDialog(hDlg);
			SendDlgItemMessage(hDlg, IDE_GROUP, EM_LIMITTEXT, AUTO_MAX_TEST_NAME, 0L);
			Busy(FALSE);
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				case IDOK:
					{
					TCHAR  szExists[AUTO_MAX_TEST_NAME];
					GetText(GetDlgItem(hDlg, IDE_GROUP), szExists);
					if(IsValidTestName(szExists)) {
						switch(GetIniNameUsage(szExists)) {
							case INI_DATA_SOURCE:
							case INI_AUTO_TEST:
								szMessageBox(hDlg,
										MB_OK | MB_ICONINFORMATION,
										(LPTSTR)szTestTitle,
										GetString(szErrBuff, MAXOUTBUFF, idsGroupNameInUse),
										szExists);
								return TRUE;

							case INI_GROUP_NAME:
								if(IDOK != szMessageBox(hDlg,
										MB_ICONQUESTION | MB_OKCANCEL,
										(LPTSTR)szTestTitle,
										GetString(szErrBuff, MAXOUTBUFF, idsReplaceGroup),
										szExists))
									return TRUE;
								break;
							}
						// Either it didn't exist or they wanted to replace it
						RemoveGroup(szExists);
						AddGroupToIni(szExists);
						lstrcpy(ng->szNewGroup, szExists);
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

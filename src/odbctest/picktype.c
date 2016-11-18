//*---------------------------------------------------------------------------------
//|	File:		PICKTYPE.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module contains the functions which can be used to add/remove
//|		data types for MakeTable()
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "resource.h"
#include "table.h"
#include "picktype.h"
#pragma warning(disable:4996)
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

extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndFrame;							// Main window (for menu access)
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern lpDFTARRAY					lpdftGetInfo;
extern UINT							ciInfoTypes;
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern HMENU						hMenu;								// Menu handle for Trace item
static int							cTypes=0;

static TCHAR szTYPES[]				=	TEXT("TYPES");
static TCHAR szGATORININAME[]		=	TEXT("GATOR32.INI");
static TCHAR szSTARTUP[] 			=  TEXT("startup");
static TCHAR szLASTRUNLIST[]		=  TEXT("Last Run List");
static TCHAR szDFTFROMDIR[] 		=  TEXT("Default From Dir");
static TCHAR szAUTOTESTS[]			=	TEXT("Auto Tests");
static TCHAR szDLL[]					=	TEXT("DLL");
static TCHAR szINSTALLED[]			=	TEXT("Installed");
static TCHAR szSQLDRIVERS[]		=	TEXT("SQL_DRIVERS");
static TCHAR szSERVER0[]			=	TEXT("SERVER0");
static TCHAR szLOGIN0[]				=	TEXT("LOGIN0");
static TCHAR szPASSWORD0[]			=	TEXT("PASSWORD0");
static TCHAR szKEYWORDS[]			= 	TEXT("KEYWORDS");
static TCHAR szGROUPS[]				=	TEXT("GROUPS");
static TCHAR szLASTRUN[]			=	TEXT("Last Run");
static TCHAR szODBCTESTHANDLE[]	=	TEXT("ODBC Test Handles");
static TCHAR szErrTitle[]			=	TEXT("Error");
static TCHAR szTestTitle[]			=	TEXT("ODBC Test");

extern TCHAR 		szGATORINI[_MAX_PATH];
TCHAR szErrBuff[MAXOUTBUFF];


//*---------------------------------------------------------------------------------
//| Busy:
//|	Call this function to set the cursor when doing long work items.
//|
//|	Note:  Because this function is saving the old cursor in the DSEG of this
//|			 DLL, it would be possible to lose a custom cursor.  Therefore this
//|			 function should only be used to display either the hourglass or
//|			 the standard arrow cursor.
//| Parms:
//|	flag							TRUE to display HourGlass, FALSE for arrow
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
/*void Busy(int flag)
{
	static HCURSOR	hCursor;						// Save old cursor handle

	ShowCursor(FALSE);
	if(flag)
		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	else
		SetCursor(hCursor);
	ShowCursor(TRUE);
}*/


//*---------------------------------------------------------------------------------
//| BuildSelList:
//|	This function will build a doubally null terminated list of selected
//|	items and return a pointer to it.
//| Parms:
//|	hwndList						List box handle
//| Returns:
//|	Total items processed
//*---------------------------------------------------------------------------------
/*LPTSTR BuildSelList(HWND hwndList)
{
	LPTSTR			szStr, str;
	WPARAM		cbCnt;
	int			dex;
   int *	xIndex;

	if(!(cbCnt = (WPARAM)SendMessage(hwndList, LB_GETSELCOUNT, 0, 0L)))
   	return NULL;
	if(!(xIndex = (int *)AllocateMemory(sizeof(int) * cbCnt)))
		return NULL;
	if(!(szStr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF)))
		return NULL;
	SendMessage(hwndList, LB_GETSELITEMS, cbCnt, (LPARAM)(int *)xIndex);
	for(dex=0;  dex<(int)cbCnt;  dex++) {
		SendMessage(hwndList, LB_GETTEXT, xIndex[dex], (LPARAM)(LPTSTR)str);
		str += lstrlen(str) + 1;
		}
	FREE_PTR(xIndex);
	return szStr;
}*/

//*---------------------------------------------------------------------------------
//| EnumerateListBox:
//|	This function will go through each selected item in a multiple selection
//|	list box and send the user defined message to the parent window.  The
//|	message sent to the parent is:
//|			wParam:		Index of selected item
//|			lParam:		Pointer to the retrieved text value
//| Parms:
//|	hwndParent					Parent dialog handle
//|	hwndList						Handle of the list box to enumerate
//|	msg							User defined message to send to parent
//| Returns:
//|	Total items processed
//*---------------------------------------------------------------------------------
/*UINT EnumerateListBox(HWND hwndParent, HWND hwndList, unsigned msg)
{
	static TCHAR szItem[MAXLISTBOXSIZE];
	WPARAM		cbCnt;
	int			dex;
   int *	xIndex;
   UINT			cbTotal=0;

	if(!(cbCnt = (WPARAM)SendMessage(hwndList, LB_GETSELCOUNT, 0, 0L)))
   	return TRUE;
	if(!(xIndex = (int *)AllocateMemory(sizeof(int) * cbCnt)))
		return TRUE;
	SendMessage(hwndList, LB_GETSELITEMS, cbCnt, (LPARAM)(int *)xIndex);
	for(dex=(int)cbCnt-1;  dex>=0;  --dex) {
		SendMessage(hwndList, LB_GETTEXT, xIndex[dex], (LPARAM)(LPTSTR)szItem);
		SendMessage(hwndParent, msg, (WPARAM)xIndex[dex], (LPARAM)(LPTSTR)szItem);
		}
	FREE_PTR(xIndex);
	return cbTotal;
}*/


//*------------------------------------------------------------------------
//|  GetIniNameUsage:
//|		This function takes and name and returns one of the following
//|		flags to indicate its usage:
//|				INI_STRING_NOT_FOUND
//|				INI_DATA_SOURCE
//|				INI_AUTO_TEST
//|				INI_GROUP_NAME
//|  Parms:
//|		szName			The auto test name
//|  Returns:
//|		One of the above values
//*------------------------------------------------------------------------
UINT INTFUN GetIniNameUsagee(LPTSTR szName)
{
	TCHAR		szBuf[MAXOUTBUFF];

	if(GetSourceName(szName, szBuf, sizeof(szBuf)/sizeof(szBuf[0])))
		return INI_DATA_SOURCE;
	else if(GetTestNameFromIni(szName, szBuf, sizeof(szBuf)/sizeof(szBuf[0])))
		return INI_AUTO_TEST;
	else if(GetTypeNameFromIni(szName, szBuf, sizeof(szBuf)/sizeof(szBuf[0])))
		return INI_GROUP_NAME;
	else
		return INI_STRING_NOT_FOUND;
}

//*------------------------------------------------------------------------
//|  GetNewTypeNameWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
BOOL EXTFUN GetNewTypeNameWndProc(HWND		hDlg,
										unsigned msg,
										WORD		wParam,
										LONG		lParam)
{
	static NEWGROUPINFO *ng;

	switch(msg)
	{
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
			switch(GET_WM_COMMAND_ID(wParam,lParam))
			{
				case IDOK:
					{
					TCHAR  szExists[AUTO_MAX_TEST_NAME];
					GetText(GetDlgItem(hDlg, IDE_GROUP), szExists);
					switch(GetIniNameUsagee(szExists))
					{
						case INI_DATA_SOURCE:
						case INI_AUTO_TEST:
							szMessageBox(hDlg,
									MB_OK | MB_ICONINFORMATION,
									(LPTSTR)szTestTitle,
									GetGtrString(szErrBuff, MAXOUTBUFF, idsGroupNameInUse),
									szExists);
							return TRUE;

						case INI_GROUP_NAME:
							if(IDOK != szMessageBox(hDlg,
									MB_ICONQUESTION | MB_OKCANCEL,
									(LPTSTR)szTestTitle,
									GetGtrString(szErrBuff, MAXOUTBUFF, idsReplaceGroup),
									szExists))
								return TRUE;
							break;
					}

					// Either it didn't exist or they wanted to replace it
					RemoveType(szExists);
					AddTypeToIni(szExists);
					lstrcpy(ng->szNewGroup, szExists);
					ng->fStat = TRUE;
					SendMessage(hDlg, WM_CLOSE, 0, 0L);

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

//*------------------------------------------------------------------------
//|  GetNewTypeName:
//|		This function will prompt the user for a new group name.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL
//|	Returns:
//|		TRUE if a new name has been added to our list
//*------------------------------------------------------------------------
BOOL INTFUN GetNewTypeName(HWND hwnd, HINSTANCE hInst, LPTSTR szNewType)
{
	NEWGROUPINFO		ng;

	ng.hInst = hInst;
	ng.szNewGroup[0] = '\0';
	ng.fStat = 0;


	if (-1 == DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_NEW_GROUP), hwnd,
			(DLGPROC)GetNewTypeNameWndProc,(LPARAM)(NEWGROUPINFO *)&ng))
	{
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetGtrString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_NEW_GROUP");
	}

	if(ng.fStat)
	{
		lstrcpy(szNewType, ng.szNewGroup);
		return TRUE;
	}
	else
		return FALSE;

}

//*------------------------------------------------------------------------
//|  AddSQLType:
//|		This routine adds a SQL Datatype to the List box
//|	Parms:
//|		wSQLType		SQL Datatype
//|		rgFields		FIELDINFO records
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN AddSQLType(HWND hwndCtl,SDWORD sdwSQLType,lpFIELDINFO rgFields,
							HDBC *phdbc, HSTMT *phstmt)
{
	SWORD wSQLType=(SWORD)sdwSQLType;

	GetTypeInfo(phdbc,phstmt,rgFields,wSQLType);

	//SendMessage(hwndCtl, LB_ADDSTRING, 0, (LPARAM)(LPTSTR)str);

} //AddSQLType()


//*------------------------------------------------------------------------
//|  PickTypesWndProc:
//|		Our dialog proc which is used to handle messages received.
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Depends on messages
//*------------------------------------------------------------------------
LRESULT EXTFUN PickTypesWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static lpFIELDINFO rgFields;
	static int			cbAvail, cbInstall, cbTypes;
	static TCHAR		szTmpName[AUTO_MAX_TEST_NAME];

	switch(msg) {
		//
		// Register 3d stuff, get our instance handle and set defaults text and
		//	control items
		//
		case WM_INITDIALOG:
			Busy(TRUE);
			rgFields = (lpFIELDINFO)lParam;
			CenterDialog(hDlg);

			// Now set the default lists
			SendMessage(hDlg, USER_RESET_TYPES, 0, 0L);
			Busy(FALSE);
			return TRUE;


		//  User message to enable/disable buttons based on counts
		case USER_SET_BUTTON_STATE:
			// Select groups only when present
			EnableWindow(GetDlgItem(hDlg, IDT_TYPE), cbTypes);
			EnableWindow(GetDlgItem(hDlg, IDCB_TYPES_LIST), cbTypes);

			// Set installed items
			EnableWindow(GetDlgItem(hDlg, IDT_TYPES_INSTALLED), cbInstall && cbTypes);
			EnableWindow(GetDlgItem(hDlg, IDL_INSTALLED), cbInstall && cbTypes);

			// Set available items
			EnableWindow(GetDlgItem(hDlg, IDT_TYPES_AVAIL), cbAvail && cbTypes);
			EnableWindow(GetDlgItem(hDlg, IDL_AVAILABLE), cbAvail && cbTypes);

			// Buttons enabled on if there are groups
			EnableWindow(GetDlgItem(hDlg, IDB_ADD), cbAvail && cbTypes);
			EnableWindow(GetDlgItem(hDlg, IDB_REMOVE), cbInstall && cbTypes);
			EnableWindow(GetDlgItem(hDlg, IDB_DELETE), cbTypes);
			return TRUE;


		// User message to refresh the group list
		case USER_RESET_TYPES:
			{
			static 	LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDCB_TYPES_LIST);

			cbTypes = 0;
			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
			if(!astr)
				return TRUE;
			SendMessage(hwnd, CB_RESETCONTENT, 0, 0L);

			if(GetTypeList(str, MAXTESTBUFF))
			{
				while(*str)
				{
					SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)str);
					str += lstrlen(str) + 1;
					++cbTypes;
				}
			}

			FREE_PTR(astr);
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
			HDBC		hdbc=NULL;
			HSTMT		hstmt=NULL;

			if (!lpActiveConnWin)
				return(TRUE);

			hdbc=lpActiveConnWin->hdbc;

			if (RC_NOTSUCCESSFUL(SQLAllocStmt(hdbc,&hstmt)))
				return(TRUE);

			cbInstall = 0;
			SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
			if(lParam)
				gstr = (LPTSTR)lParam;
			else
				GetText(GetDlgItem(hDlg, IDCB_TYPES_LIST), gstr);

			if(!*gstr)
			{
				SendMessage(hDlg, USER_RESET_AVAILABLE, 0, 0L);
				return TRUE;
			}

			astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);

			if(!astr)
				return TRUE;

			if(GetTypeAutoTests(gstr, str, MAXTESTBUFF))
			{
				while(*str)
				{
					AddSQLType(hwnd,_ttoi(str),rgFields,&hdbc,&hstmt);
					str += lstrlen(str) + 1;
					++cbInstall;
				}
			}

			FREE_PTR(astr);
			SendMessage(hwnd, LB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_RESET_AVAILABLE, 0, 0L);
			SQLFreeStmt(hstmt,SQL_DROP);

			}
			return TRUE;

		// Now for the available list of DLLs, the Installed list must be
		//		done before this message as we use it to exclude items
		case USER_RESET_AVAILABLE:
			{
			static 	LPTSTR		str, astr;
			HWND		hwnd=GetDlgItem(hDlg, IDL_AVAILABLE);
			HWND		hwndx=GetDlgItem(hDlg, IDL_INSTALLED);
			HDBC		hdbc=NULL;
			HSTMT		hstmt=NULL;
			int		iType=0;
			DWORD		dex=0;


			cbAvail = 0;
			SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);

			if (!lpActiveConnWin)
				return(TRUE);

			hdbc=lpActiveConnWin->hdbc;

			if (RC_NOTSUCCESSFUL(SQLAllocStmt(hdbc,&hstmt)))
				return(TRUE);

			if (cTypes=GetTypeInfo(&hdbc,&hstmt,rgFields,3))
			{
				for (iType=0;iType < cTypes;iType++)
				{
					if(LB_ERR == SendMessage(hwndx, LB_FINDSTRING, 0,
						(LPARAM)(LPTSTR)rgFields[iType].szType))
					{
						dex=(DWORD)SendMessage(hwnd, LB_ADDSTRING, 0,
							(LPARAM)(LPTSTR)rgFields[iType].szType);

						//Store the DataType
						SendMessage(hwnd, LB_SETITEMDATA, dex,(LPARAM)rgFields[iType].wSQLType);
					}
					++cbAvail;
				}
			}

			SendMessage(hwnd, LB_SETCURSEL, 0, 0L);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);
			SQLFreeStmt(hstmt,SQL_DROP);
			}
			return TRUE;


		// User message sent by EnumerateListBox for each selected item to add
		//		Make sure the item is not alreay in the .INI file
		case USER_ADD_ITEM:
			{
				HWND	hwndCtl=GetDlgItem(hDlg, IDCB_TYPES_LIST);
				DWORD	dex=0;
				int	iType=0;
				SWORD	wSQLType=0;

			GetText(hwndCtl, szTmpName);

			dex=(DWORD)SendDlgItemMessage(hDlg, IDL_AVAILABLE, LB_GETCURSEL, 0,0L);

			wSQLType=(SWORD)SendDlgItemMessage(hDlg,IDL_AVAILABLE, LB_GETITEMDATA, dex,0L);

			for (iType=0;iType < cTypes;iType++)
			{
				if (wSQLType == rgFields[iType].wSQLType)
					rgFields[iType].fUse=TRUE;
			}

			// Move from available to installed
			SendDlgItemMessage(hDlg, IDL_INSTALLED, LB_ADDSTRING, 0, lParam);
			SendDlgItemMessage(hDlg, IDL_AVAILABLE, LB_DELETESTRING, wParam, 0L);
			++cbInstall;
			--cbAvail;

			// Add auto test to the group
			AddTypeAutoTest(szTmpName, (LPTSTR)lParam);
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
			GetText(GetDlgItem(hDlg, IDCB_TYPES_LIST), szTmpName);

			// Move from available to installed
			SendDlgItemMessage(hDlg, IDL_AVAILABLE, LB_ADDSTRING, 0, lParam);
			SendDlgItemMessage(hDlg, IDL_INSTALLED, LB_DELETESTRING, wParam, 0L);
			++cbAvail;
			--cbInstall;

			// Add auto test to the group
			RemoveTypeAutoTest(szTmpName, (LPTSTR)lParam);
			SendMessage(hDlg, USER_SET_BUTTON_STATE, 0, 0L);

			// If there are no more left, don't leave focus on disabled button
			if(!cbInstall)
				SetFocus(GetDlgItem(hDlg, IDCANCEL));
			}
			return TRUE;


		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam))
			{
				case IDB_NEW:
				{
					if(GetNewTypeName(hDlg, hInst, szTmpName))
					{
						++cbTypes;
						SendDlgItemMessage(hDlg, IDCB_TYPES_LIST, CB_SETCURSEL,
								(WPARAM)SendDlgItemMessage(hDlg, IDCB_TYPES_LIST, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szTmpName),
								0L);
						SendMessage(hDlg, USER_RESET_INSTALLED, 0, 0L);
					}
				}
				return TRUE;

				case IDB_DELETE:
					{
					GetText(GetDlgItem(hDlg, IDCB_TYPES_LIST), szTmpName);
					if(IDOK == szMessageBox(hDlg,
									MB_ICONQUESTION | MB_OKCANCEL,
									(LPTSTR)szTestTitle,
									GetGtrString(szErrBuff, MAXOUTBUFF, idsDeleteType),
									szTmpName)) {
						RemoveType(szTmpName);
						RemoveTypeFromIni(szTmpName);
						SendMessage(hDlg, USER_RESET_TYPES, 0, 0L);
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
					if(GET_WM_COMMAND_ID(wParam,lParam) == IDCB_TYPES_LIST) {
						HWND hwnd=GetDlgItem(hDlg, IDCB_TYPES_LIST);
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
//|  PickTypes:
//|		This function will invoke the Manage Types dialog box which
//|		the user may use to edit groups.
//|	Parms:
//|		hwnd			Handle of the parent
//|		hInst			Instance of this DLL*
//|							*The hInstance of this loaded library must be used
//|							to load the dialog.
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
int EXTFUN PickTypes(HWND hwnd, HINSTANCE hInst,lpFIELDINFO rgFields)
{
	lstrcpy(szGATORINI, szGATORININAME);							// Just use GATOR.INI, no path

	if(-1 == DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_MANAGE_TYPES), hwnd, PickTypesWndProc,
			(LPARAM)rgFields))
	{
		szMessageBox(hwnd,
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetGtrString(szErrBuff, MAXOUTBUFF, idsDialogNotLoaded),
						"IDD_MANAGE_TYPES");
	}

	return (cTypes);

} //PickTypes()

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	CONNWIN.C
//|
//| Purpose:
//|		This module contains the window procedure for the Connection Windows,
//|		as well as utility routines used to track connections.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#pragma warning(disable:4996)

//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define STMT_WIDTH		200
#define STMT_HEIGHT		 24



//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE				hInst;
extern HFONT					hEditFont;
extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HWND						hwndStatBar;
extern HWND						hwndOutWin;
extern HWND						hwndFrame;
extern lpUSROPTIONS			lpUsrOptions;		// Global user options
extern HWND						hwndCurMDIChild;  // Current active MDI child at all times
extern lpRESULTWININFO		lpActiveResultWin;

extern lpHEADENV					lpHeadEnv;					//Pointer to Global Env linked list


HICON									hConnectIcon=NULL;
dCSEG(TCHAR) szCONNWINCLASS[]		= TEXT("GATOR:CONNWIN");
dCSEG(TCHAR) szOPENFILTER[]		= TEXT("Text Files (*.txt;*.log;*.sql)\0*.txt;*.log;*.sql\0All Files (*.*)\0*.*\0");
dCSEG(TCHAR) szOPENEXTDFT[]		= TEXT("txt");
dCSEG(TCHAR) szUNKNOWN[]			= TEXT("<unknown>");
dCSEG(TCHAR) szInvalidhwnd[]		= TEXT("Invalid hwnd!");

lpCONNECTIONINFO		lpConnWin;					// Head node for connection windows
lpCONNECTIONINFO		lpActiveConnWin;			// The currently active connection window
UINT						giStmtHeight;				// Based on font of combo-box



//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN ConnectWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND INTFUN GetEditWindow();
void INTFUN ClearOutputWindow(lpCONNECTIONINFO ci);
VOID INTFUN SetCurrentHstmtFromList(lpCONNECTIONINFO lpci);


//*---------------------------------------------------------------------------------
//| InitConnectionInfo:
//|	This function will add a new CONNECTIONINFO node to our linked
//|	list of connections,set the title info, and initialize the
//|	hstmt drop down. This code was moved from CreateConnectionWindow
//|	because the call to CreateConnectionWindow was moved to a point
//|	before the henv is allocated.
//| Parms:
//|	ci				connection info record
//|	lpEi			environment info record
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN InitConnectionInfo(lpCONNECTIONINFO ci,lpENVIRONMENTINFO lpEi)
{
	lpENVIRONMENTINFO				lpActiveEi=GetActiveEnv();

	GetGtrString(szOutBuff, MAXOUTBUFF, idsUnconnected);
	wsprintf((LPTSTR)ci->szClientTitle,szOutBuff,ci->uConNum);

	if(lpEi->lpConn)
	{				// Insert at end of list
			lpEi->lpConn->prev->next = ci;
			ci->prev =lpEi->lpConn->prev;
			ci->next = lpEi->lpConn;
			lpEi->lpConn->prev = ci;
	}
	else
	{
		// Only connection at this time
			lpEi->lpConn = ci;
			ci->next = ci;
			ci->prev = ci;
	}

	lpEi->cConns++;
	lpEi->lpCurConn=ci;
	lpConnWin=lpEi->lpConn;

	// Add NULL hstmt
	SendMessage(ci->hwndStmts, CB_ADDSTRING, 0, (LPARAM)TEXT("SQL_NULL_HSTMT"));
	SendMessage(ci->hwndStmts, CB_SETCURSEL, 0, 0L);

} //InitConnectionInfo()



//*---------------------------------------------------------------------------------
//| CreateConnectionWindow:
//|	This function will add a new connection window to the client area.
//| Parms:
//|	hwndClient				Client window handle
//|	hInst						For use in CreateWindow call
//| Returns:
//|	Window handle of the status bar if successful, NULL on error
//*---------------------------------------------------------------------------------
CONNECTIONINFO * CreateConnectionWindow(HWND hwndClient, HINSTANCE hInst,
													 	lpENVIRONMENTINFO lpActiveEi)
{
	MDICREATESTRUCT		mdicreate;
	lpCONNECTIONINFO		ci=NULL;
	BOOL						fZoom;
	LRESULT					lResult=0;
	UINT						cNext=(lpActiveEi? lpActiveEi->cConns:0);

 	// Allocate memory for a new node, then create the window
	ci = (lpCONNECTIONINFO)AllocateMemory(sizeof(CONNECTIONINFO));
	if(!ci)
		return NULL;

	//set connection number
	ci->uConNum = ++cNext;


	// Determine if child windows are currently maximized and match this current window
	fZoom = IsZoomed(hwndCurMDIChild);

	mdicreate.szClass = szCONNWINCLASS;
	mdicreate.szTitle = (LPTSTR)ci->szClientTitle;
	mdicreate.hOwner  = hInst;
	mdicreate.x			= CW_USEDEFAULT;
	mdicreate.y			= CW_USEDEFAULT;
	mdicreate.cx		= CW_USEDEFAULT;
	mdicreate.cy		= CW_USEDEFAULT;
	mdicreate.style	= (fZoom) ? WS_MAXIMIZE : 0;
	mdicreate.lParam	= (LPARAM)ci;

	// If successful, add the new connection to the list, else free memory and fail

	lResult=SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)&mdicreate);

	if (!lResult)
	{
		szMessageBox(hwndClient,
					MB_ICONEXCLAMATION | MB_OK,
					NULL,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsConnWinCreateFailure));
		FREE_PTR(ci);
		return NULL;
	}

	//set back pointer
	ci->lpActiveEnv=lpActiveEi;

	return ci;
}



//*---------------------------------------------------------------------------------
//| DestroyConnectionWindow:
//|	This function will clean up all memory allocated to a connection window,
//|	then destroy it.
//| Parms:
//|	ci							The node to delete
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DestroyConnectionWindow(lpCONNECTIONINFO ci)
{
	lpENVIRONMENTINFO lpEi=ci->lpActiveEnv;
	WORD wIndex=0;

	// Remove the node from the linked list
	if(ci->next != ci)
	{
		// More than one node
		if (ci->prev)
			ci->prev->next = ci->next;

		if (ci->next)
			ci->next->prev = ci->prev;

		if(lpEi->lpConn == ci)				// Removing head node?
		{
			lpEi->lpConn = ci->next;
			lpEi->lpCurConn = ci->next;
		}
	}
	else
	{
		lpEi->lpCurConn = NULL;
		lpEi->lpConn = NULL;
		lpConnWin=NULL;
	}

	//one less connection
	if (lpEi->cConns > 0)
		lpEi->cConns--;

	// Destroy window
	SendMessage(lpEi->hwndClient, WM_MDIDESTROY, (WPARAM)ci->hwnd, 0L);
	FREE_PTR(ci);

	// if there are no connection windows open but there is a valid henv
	// set the henv as the current selecttion
	wIndex=(WORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCOUNT, 0, 0L);
	if (!lpEi->cConns && wIndex > 0)
		SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL,wIndex-1, 0L);

} //DestroyConnectionWindow()




//*---------------------------------------------------------------------------------
//| ConnectWndProc:
//|	Handles any messages that must be processed by the window.
//|
//|	Define DEBUG_MSG_PUMP if you want to see every message that comes through.
//|	Unlike tools like Spy, this is a great way to track down messages that occur
//|	before a window is created (before Spy can see them).
//*---------------------------------------------------------------------------------
LRESULT EXTFUN ConnectWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

//#define DEBUG_MSG_PUMP
#if defined(_DEBUG) && defined(DEBUG_MSG_PUMP)
{
TCHAR sztmp[200];
lpCONNECTIONINFO ci=(lpCONNECTIONINFO)GETCIPOINTER(hwnd);
wsprintf(sztmp, TEXT("ci: 0x%08X\thwnd: %08X\tmsg: %08X\twParam: %08X\tlParam: %08X\r\n"),
		ci, hwnd, msg, wParam, lParam);
OutputDebugString(sztmp);
}
#endif


	switch(msg) {
		//--------------------------------------------------------------------
		// WM_CREATE:		Process to retrieve the CONNECTIONINFO structure
		//						and create the child windows required.
		//--------------------------------------------------------------------
		case WM_CREATE:
			{
			lpCONNECTIONINFO			ci;
			CREATESTRUCT * 		cs;
			MDICREATESTRUCT * 	mdi;
			lpENVIRONMENTINFO			lpEi=NULL;
			int	x=0,
					y=0,
					nWidth=0,
					nHeight=0;

			cs = (CREATESTRUCT *)lParam;
			mdi = (MDICREATESTRUCT *)cs->lpCreateParams;
			ci = (lpCONNECTIONINFO)mdi->lParam;
			SETCIPOINTER(hwnd, ci);

			// Now create windows
			ci->hwnd = hwnd;
			ci->hwndIn = CreateEditWindow(ci->hwnd, hInst, IDW_TOP, 0,
										x,y,nWidth,nHeight);

			ci->hwndOut = CreateEditWindow(ci->hwnd,hInst,
										IDW_BOTTOM, WS_BORDER,
										x,y,nWidth,nHeight);

			ci->hwndStmts = CreateWindow(TEXT("COMBOBOX"), NULL,
						WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
						CBS_DROPDOWNLIST,
						0, 0, STMT_WIDTH, 0,
						hwnd, (HMENU)(IDW_STMTS), hInst, NULL);

			if(!ci->hwndIn || !ci->hwndOut || !ci->hwndStmts) // Verify it worked,
				return -1;                                     //		let Windows destroy children

			lpEi=GetActiveEnv();

			ci->lpActiveEnv=lpEi;

			PostMessage(hwnd, USER_MOD_MDI_CHILD_MENU, 0, 0L);
			}
			return 0;


		//--------------------------------------------------------------------
		// USER_MOD_MDI_CHILD_MENU:
		//--------------------------------------------------------------------
		case USER_MOD_MDI_CHILD_MENU:
			//Bug #218 Removed the SC_CLOSE processing
			return 0;


		//--------------------------------------------------------------------
		// WM_MENUSELECT:   This message is sent when a menu item is
		//							selected.  Show the description in the status bar.
		//--------------------------------------------------------------------
		case WM_MENUSELECT:
			{
			DoMenuSelect(GET_WM_MENUSELECT_ID(wParam, lParam),			// menu id
							 GET_WM_MENUSELECT_FLAGS(wParam, lParam),		// flags
							 GET_WM_MENUSELECT_HMENU(wParam, lParam));	// hMenu
			}
			break;


		//--------------------------------------------------------------------
		// WM_SIZE:			Use this message to move the child windows to
		//						correct locations.  Note that we must break to
		//						allow DefMDIWndProc to handle min/max messages
		//--------------------------------------------------------------------
		case WM_SIZE:
			{
			lpCONNECTIONINFO			ci;
			WORD							dx = LOWORD(lParam);
			WORD							dy = HIWORD(lParam);
			UINT							cyExtra = (2 * GetSystemMetrics(SM_CYBORDER));
			UINT							yToolBar;
			UINT							yEdit;
			UINT							yBotWin;
			RECT							rect;
			int							iCBStmtHeight;

			ci = (lpCONNECTIONINFO)GETCIPOINTER(hwnd);
			HideDropDowns(ci);

			// Figure overall dimensions
			FindComboRowHeight(ci->hwndStmts);
			GetWindowRect(ci->hwndStmts, &rect);
			iCBStmtHeight = rect.bottom - rect.top;
			yToolBar = (2 * cyExtra) + iCBStmtHeight;
			yEdit = dy - yToolBar;
			yBotWin = (UINT)(.6 * yEdit);

			// Calculate rect for 3d affect
			ci->rToolRect.left = 0;
			ci->rToolRect.top = 0;
			ci->rToolRect.right = dx;
			ci->rToolRect.bottom = yToolBar;

			// Move the windows
			MoveWindow(ci->hwndStmts, (dx - STMT_WIDTH) / 2,
											((yToolBar - iCBStmtHeight) / 2),
											STMT_WIDTH, iCBStmtHeight, TRUE);
			MoveWindow(ci->hwndIn, 0, yToolBar, dx, yEdit - yBotWin, TRUE);
			MoveWindow(ci->hwndOut, 0, dy - yBotWin, dx, yBotWin, TRUE);
			}
			break;


		//
		// Hande WM_MOVE so that if any drop downs are present, they are stowed
		//		away for movement.  If not, a ghost is left on the screen.
		//
		case WM_MOVE:
			{
			HideDropDowns((lpCONNECTIONINFO)GETCIPOINTER(hwnd));
			}
			break;


		//
		// Hande the WM_INITMENUPOPUP message so that when the user selects
		//		a menu, we enable/disable each item based on our current state.
		//
		case WM_INITMENUPOPUP:
			{
			if(!(BOOL)HIWORD(lParam))			// Not the system menu
				ResetMenuStatus((HMENU)wParam, (int)LOWORD(lParam), 2);
			HideDropDowns((lpCONNECTIONINFO)GETCIPOINTER(hwnd));
			}
			break;


		//--------------------------------------------------------------------
		// WM_PAINT:		Add 3d affect to tool bar.
		//--------------------------------------------------------------------
		case WM_PAINT:
			{
			lpCONNECTIONINFO		ci;
			PAINTSTRUCT				ps;
			HDC						hdc;

			ci = (lpCONNECTIONINFO)GETCIPOINTER(hwnd);
			if(hdc = BeginPaint(hwnd, &ps)) {
				Draw3DEdges(hwnd, hdc, &ci->rToolRect);
				EndPaint(hwnd, &ps);
				}
			}
			break;


		//--------------------------------------------------------------------
		// WM_COMMAND:			Process menu items.  Note that some menu items
		//							are handled by the main window procedure, in
		//							which case we simply pass them on.
		//--------------------------------------------------------------------
		case WM_COMMAND:
			{
			lpCONNECTIONINFO		ci;
			UINT						id=GET_WM_COMMAND_ID(wParam, lParam);
			UINT						code=GET_WM_COMMAND_CODE(wParam, lParam);

			ci = (lpCONNECTIONINFO)GETCIPOINTER(hwnd);

         // All menu items are handled in main.c, so pass them back to the parent
			if(id >= IDM_LOWEST_MENU &&
				id <= IDM_HIGHEST_MENU) {
				SendMessage(hwndFrame, WM_COMMAND, wParam, lParam);
				return 0;
				}

			// Look for notification codes.  In this case we're interested in
			//		the drop down messages which we must handle specially so
			//		that our hstmt list will display.
			switch(code) {
				case CBN_DROPDOWN:
					{
					HWND			hwnd=GET_WM_COMMAND_HWND(wParam, lParam);
					AdjustDropDownBox(hwnd);
					}
					break;

				// When the user changes the selection of the hstmt list, find the new current
				//		statement based on that value.
				case CBN_SELCHANGE:
					{
					SetCurrentHstmtFromList(lpActiveConnWin);
					// Refresh the toolbar (for all buttons relating to an allocated hstmt)
					RefreshToolbar();
					}
					break;

				// When the input/output edit windows or the HSTMT list get focus,
				//		put the description in the edit window.
				case CBN_SETFOCUS:
				case EN_SETFOCUS:
					{
					SetStatBarText(hwndStatBar,
									GetGtrString(szOutBuff, MAXOUTBUFF, id),
									BLACK_TEXT);
					}
					break;

				// Sent when the edit control is out of room.  We'll truncate contents
				case EN_ERRSPACE:
					{
					if(lpActiveConnWin) {
						HWND		hwnd=(id == IDW_BOTTOM) ? lpActiveConnWin->hwndOut :
											lpActiveConnWin->hwndIn;

						ASSERTCHECK((!lpActiveConnWin->hwndOut), szInvalidhwnd);
						TruncateEditContents(hwnd);
						}
					}
					break;

				// Refresh Save As.  Don't assume there is an lpActiveConnWin
				//	since we get these while changing windows on Font and Word Wrap
				case EN_UPDATE:
				case EN_CHANGE:
					{
					if(id == IDW_TOP && lpActiveConnWin)
						RefreshToolbarSaveAs(lpActiveConnWin->hwndIn);
					}
					break;
				}
			}
			break;

		//--------------------------------------------------------------------
		// WM_MDIACTIVATE:	This message is sent for the window losing
		//							focus, as well as when we gain focus.
		//--------------------------------------------------------------------
		case WM_MDIACTIVATE:
			{
			lpENVIRONMENTINFO lpActiveEnv=GetActiveEnv();
			DWORD					dwIndex=0;

			*szOutBuff = TEXT('\0');
#ifndef WIN32
			if(wParam)
			{
				// Activate
				lpActiveConnWin = (lpCONNECTIONINFO)GETCIPOINTER((HWND)LOWORD(lParam));
			}
#else
			if((HWND)lParam == hwnd)
			{
				lpActiveConnWin = (lpCONNECTIONINFO)GETCIPOINTER(hwnd);
			}
#endif
			else
			{
				lpActiveConnWin = NULL;
				lpActiveResultWin = NULL;
			}

			if (lpActiveConnWin && !lpActiveEnv)
			{
				if (!(lpActiveEnv=lpActiveConnWin->lpActiveEnv))
				{
					//If we fail to obtain a valid Env. node use
					//the Global Head node.
					if (lpHeadEnv)
						lpActiveEnv=lpHeadEnv->lpHead;
				}

			}

			if (lpActiveEnv)
				lpActiveEnv->lpCurConn=lpActiveConnWin;

			HideDropDowns(lpActiveConnWin);
			if(lpActiveConnWin)
			{
				hwndCurMDIChild = lpActiveConnWin->hwnd;			// Save current active child
				SetFocus(hwndCurMDIChild);
				SetFocus(lpActiveConnWin->hwndIn);
				GetConnWinnDescText(lpActiveConnWin, szOutBuff, MAXOUTBUFF);
			}
			else
				hwndCurMDIChild = NULL;									// No current active child

			//Find and highlight the correct henv in the henv window
			if (lpActiveEnv)
				{
				dwIndex=FindIndexInDropDown(lpHeadEnv->hwndEnvs,lpActiveEnv->henv,
					lpActiveEnv->uEnvNum,szHENV);
				SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL, dwIndex, 0L);
				}

			SetStatBarText(hwndStatBar, szOutBuff, BLACK_TEXT);
			RefreshToolbar();
			}
			return 0;


		//--------------------------------------------------------------------
		// WM_SYSCOMMAND:		Need to track this message in case the user
		//							closes the window via the system menu.  Handle
		//							this scenario as a Full Disconnect.
		//--------------------------------------------------------------------
		case WM_SYSCOMMAND:
			{
			//Bug #218 Added SC_CLOSE processing
			if ((wParam == IDM_FULL_DISCONNECT) || (wParam == SC_CLOSE)){
				SendMessage(hwndFrame, WM_COMMAND, PORT_WM_COMMAND_SEND(IDM_FULL_DISCONNECT,0L));
				return 0;
				}

			HideDropDowns((lpCONNECTIONINFO)GETCIPOINTER(hwndFrame));
			}
			break;


		default:
			break;
		}

	//
	// If we haven't already processed the message, do default behavior.
	//
	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}





//*---------------------------------------------------------------------------------
//| GetEditWindow:
//|	This function will get the current focus window and verify that it is
//|	an edit window which is valid for one of the Edit menu items.
//| Parms:
//|	None.
//| Returns:
//|	Edit window handle if valid, NULL if not valid
//*---------------------------------------------------------------------------------
HWND INTFUN GetEditWindow()
{
	HWND 		hwnd;
	TCHAR		szClass[20];

	if(hwnd = GetFocus()) {
		GetClassName(hwnd, (LPTSTR)szClass, sizeof(szClass)/sizeof(szClass[0]));
		if(!lstrcmpi(szClass, TEXT("EDIT")))
			return hwnd;
		}
	return NULL;
}




//*---------------------------------------------------------------------------------
//| ClearOutputWindow:
//|	Allows user to clear the contents of the input or output window of the
//|	current connection window.
//| Parms:
//|	ci						Connection window to work with
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
void INTFUN ClearOutputWindow(lpCONNECTIONINFO ci)
{
	BOOL EXTFUN ClearOutWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	DLGPROC  	dlgproc;
	HWND			hwnd = GetFocus();

	// If there is no current connection window, then we want to clear
	// the General Output Window
	if(!lpActiveConnWin){
		SendMessage(hwndOutWin, WM_SETTEXT, 0, (LPARAM)TEXT(""));
				RefreshToolbarSaveAs(hwndOutWin);
				}
	// Display the dialog
	else {
		dlgproc = (DLGPROC)MakeProcInstance((FARPROC)ClearOutWndProc, hInst);
		if(-1 == DialogBoxParam(ci->lpActiveEnv->hInst,
				MAKEINTRESOURCE(IDD_CLEAR_EDIT),
				ci->hwnd,
				dlgproc, (LPARAM)(lpCONNECTIONINFO)ci))
			szMessageBox(ci->hwnd,
							 MB_ICONEXCLAMATION | MB_OK,
							 NULL,
							 GetGtrString(szOutBuff, MAXOUTBUFF, idsDialogNotFound),
							 (LPTSTR)TEXT("Clear Window"));
		FreeProcInstance((FARPROC) dlgproc);
		}

	if(hwnd)
		SetFocus(hwnd);
}



//*------------------------------------------------------------------------
//| ClearOutWndProc:
//|	Handle messages for the clear output window dialog proc.
//| Parms:
//|	in			Standard window parms
//| Returns:
//|	Depends on message
//*------------------------------------------------------------------------
BOOL EXTFUN ClearOutWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static lpCONNECTIONINFO ci;
	static BOOL			fTop=TRUE, fBottom=TRUE;

	switch(msg) {
		case WM_INITDIALOG:
			ci = (lpCONNECTIONINFO)lParam;
			CenterDialog(hDlg);
			CheckDlgButton(hDlg, IDX_INPUT, fTop);
			CheckDlgButton(hDlg, IDX_OUTPUT, fBottom);
			return TRUE;

		case WM_COMMAND:
			switch(GET_WM_COMMAND_ID(wParam,lParam)) {
				// For OK, simply see which boxes are checked and clear the
				//	corresponding window.
				case IDOK:
					{
					if(fTop = IsDlgButtonChecked(hDlg, IDX_INPUT))
						SendMessage(ci->hwndIn,WM_SETTEXT,0,
								(LPARAM)(LPTSTR)TEXT(""));
					if(fBottom = IsDlgButtonChecked(hDlg, IDX_OUTPUT))
						SendMessage(ci->hwndOut,WM_SETTEXT,0,
								(LPARAM)(LPTSTR)TEXT(""));
					RefreshToolbarSaveAs(lpActiveConnWin->hwndIn);
					SendMessage(hDlg, WM_CLOSE, 0, 0L);
					}
					return TRUE;

            case IDCANCEL:
           		SendMessage(hDlg, WM_CLOSE, 0, 0L);
					return TRUE;
				}
			return TRUE;

		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;
		}
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| SetCurrentHstmtFromList:
//|	This function will find and set the current hstmt node based on the current
//|	selection in the hstmt list of the given connection window.
//| Parms:
//|	lpci						The connection window to set
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN SetCurrentHstmtFromList(lpCONNECTIONINFO lpci)
{
	SWORD wIndex=(SWORD)SendMessage(lpci->hwndStmts, CB_GETCURSEL, 0, 0L);

	if (wIndex == CB_ERR)
		wIndex=(SWORD)SendMessage(lpci->hwndStmts, CB_GETCOUNT, 0, 0L);

	// Assum head node to start
	lpci->lpCurStmt = lpci->lpStmt;

	// An error or index==0 means NULL
	if(!wIndex || wIndex == CB_ERR)
		lpci->lpCurStmt = NULL;
	// Else index is node number in order from head
	else while(--wIndex)
		lpci->lpCurStmt = lpci->lpCurStmt->next;
}


//*---------------------------------------------------------------------------------
//| SetCurrentStmt:
//|	Reset the list current HSTMT based on the desired value from the user.
//| Parms:
//|	lpci						The connection window to adjust
//|	lpTarget					The statment that should be current
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN SetCurrentStmt(lpCONNECTIONINFO lpci, lpSTATEMENTINFO lpTarget)
{
	WPARAM				iPos=1;
	lpSTATEMENTINFO	lpStmt;

	// Make the active statement windows HSTMT the active
	//	handle so that the correct one is dropped.
	for(lpStmt = lpci->lpStmt;
			lpStmt && lpStmt != lpTarget;
			lpStmt = lpStmt->next)
		iPos++;
	SendMessage(lpci->hwndStmts, CB_SETCURSEL, iPos, 0L);
	SetCurrentHstmtFromList(lpci);
}


//*---------------------------------------------------------------------------------
//| AdjustDropDownBox:
//|	This function will adjust the drop-down of a combo-box to exact fit the
//|	contents up to a max of 10 entries.
//| Parms:
//|	hwnd						Window handle of the combo box
//|	uMax						Maximum rows allowed in drop-down
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN AdjustDropDownBox(HWND hwnd)
{
#define CB_MAX_ENTRIES		10

	int		dyMin;
	UINT		dy;
	RECT		rect;
	UINT		uEntries;

	FindComboRowHeight(hwnd);

	GetWindowRect(hwnd, &rect);
	dyMin = rect.bottom - rect.top + 1;						// Height of edit box on top
	uEntries = (UINT)SendMessage(hwnd, CB_GETCOUNT, 0, 0L) + 5;
	uEntries = min(CB_MAX_ENTRIES, uEntries);				// Find total entries
   dy = (uEntries * (giStmtHeight)) + dyMin;				// Calculate height of rect
	SetWindowPos(hwnd,
				(HWND)NULL, 0, 0, 								// Parms ignored
				rect.right - rect.left,
				dy,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
}

//*---------------------------------------------------------------------------------
//| HideDropDowns:
//|	This function will close up and tool-bar combo box drop downs
//| Parms:
//|	lpci						The Connection window to close
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN HideDropDowns(lpCONNECTIONINFO lpci)
{
	if(!lpci)
		return;
	SendMessage(lpci->hwndStmts, CB_SHOWDROPDOWN, FALSE, 0L);
}


//*---------------------------------------------------------------------------------
//| ConnWinMoveWindowFocus:
//|	This function will change the focus window for a connection window.
//| Parms:
//|	lpci						The Connection window to modify
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN ConnWinMoveWindowFocus(lpCONNECTIONINFO lpci)
{
	lpSTATEMENTINFO	lpStmt=NULL;
	lpRESULTWININFO	lpRslt=NULL;
	HWND	fHwnd = GetFocus(),
			hwndRslts=NULL;

	if (lpci && lpci->lpCurStmt)
	{
		lpStmt=lpci->lpCurStmt;
		lpRslt=(lpRESULTWININFO)(lpStmt->lpResults);

		if (lpRslt)
			hwndRslts=lpRslt->hwnd;
	}

	if(fHwnd == lpci->hwndIn)
		SetFocus(lpci->hwndOut);
	else if(fHwnd == lpci->hwndOut)
		SetFocus(lpci->hwndStmts);
	else if (fHwnd == hwndRslts)
		SetFocus(hwndRslts);
	else
		SetFocus(lpci->hwndIn);
}


//*---------------------------------------------------------------------------------
//| GetConnWinnDescText:
//|	This function will format a text string suitable for display in the
//|	status window for a connection window.
//| Parms:
//|	lpci						The Connection window to look at
//|	szOut						Where should the string be returned
//|	cbOut						How much room to write
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN GetConnWinnDescText(lpCONNECTIONINFO lpci, LPTSTR szOut, int cbOut)
{
	TCHAR		szFmt[MAXOUTBUFF];

	if(lpci->uState & STATE_CONNECTED_HDBC)
		wsprintf(szOutBuff, GetGtrString(szFmt, sizeof(szFmt)/sizeof(szFmt[0]), idsConnWinDesc),
					(*lpci->szUser) ? (LPTSTR)lpci->szUser : (LPTSTR)szUNKNOWN,
					(LPTSTR)lpci->szServer,
					lpci->lpActiveEnv->uEnvNum,
					GETADDRINFO(lpci->lpActiveEnv->henv),
					GETADDRINFO(lpci->hdbc));
	else
		GetGtrString(szOutBuff, sizeof(szOutBuff), idsConnWinDescNoConn);
}



//*---------------------------------------------------------------------------------
//| OpenFileToInputWindow:
//|	Allows user to clear the contents of the input or output window of the
//|	current connection window.
//| Parms:
//|	hwndIn				The window handle to use for the file
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
void INTFUN OpenFileToInputWindow(HWND	hwndIn)
{
	DWORD 				cb;
	OPENFILENAME 	ofn;
	UDWORD 			MaxDataSize;
	HANDLE 			hFile;
	HWND 				hFocus = GetFocus();
	UDWORD 			startend;
	UDWORD 			length;
	LPTSTR 			pbBuf;
	CHAR*			szBuf;
	static TCHAR		szFile[_MAX_PATH];

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.hwndOwner 			= hwndIn;
	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hInstance			= hInst;
	ofn.lpstrFilter 		= szOPENFILTER;
	ofn.lpstrDefExt 		= szOPENEXTDFT;
	ofn.nFilterIndex 		= 1;
	ofn.lpstrFile 			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.Flags 				= OFN_FILEMUSTEXIST |
									OFN_HIDEREADONLY |
									OFN_ENABLEHOOK |
									OFN_ENABLETEMPLATE;
	ofn.lpfnHook			= (LPOFNHOOKPROC)Hook3d;
	ofn.lpTemplateName	= MAKEINTRESOURCE(IDD_OPEN_FILE);

	if(GetOpenFileName(&ofn))
	{
		// Figure out how much room we have
		startend = (UDWORD)SendMessage(hwndIn, EM_GETSEL, 0, 0L);
		length = GetTrueWindowTextLength(hwndIn,-1);

		MaxDataSize = MAX_EDIT_WINDOW_TEXT - length - LOWORD(startend) + HIWORD(startend);

		if(MaxDataSize)
		{
			pbBuf = AllocateMemory(MaxDataSize);
			szBuf = AllocateMemory(MaxDataSize);

			if(pbBuf)
			{
				if ((hFile = GtrOpenFile(hwndIn,szFile,OPEN_EXISTING,
					READFROMFILE,idsErrOpenFileAlloc))!= INVALID_HANDLE_VALUE)
				{
                    ReadFile(hFile, szBuf, MaxDataSize, &cb, NULL);
					OemToCharA(szBuf,szBuf);
#ifdef _UNICODE
					MultiByteToWideChar(CP_ACP,0,szBuf,-1,pbBuf,MaxDataSize/2);
#else
					lstrcpy(pbBuf,szBuf);
#endif

					if(cb)
						SendMessage(hwndIn, EM_REPLACESEL, 0, (LPARAM)pbBuf);
					CloseHandle(hFile);
				}

				FREE_PTR(pbBuf);
				FREE_PTR(szBuf);
			}

		} //if (MaxDataSize)
		else
		{
			// There is no room to add the new file
			szMessageBox(hwndIn,
						MB_ICONEXCLAMATION | MB_OK,
						NULL,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsErrOpenFileRoom),
						(LPTSTR)szFile);
		}

	}

	if(hFocus)
		SetFocus(hFocus);

   return;
}


//*---------------------------------------------------------------------------------
//| SaveFileInInputWindow:
//|	Allows user to clear the contents of the input or output window of the
//|	current connection window.
//| Parms:
//|	hwndIn				The window handle with contents to save
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
void INTFUN SaveFileInInputWindow(HWND hwndIn)
{
	UINT 				cb;
	OPENFILENAME 	ofn;
	HANDLE 			hFile;
	HWND 				hFocus = GetFocus();
	UWORD 			length;
	LPTSTR 			pbBuf;
	static TCHAR	szFile[_MAX_PATH];
	int				cExtra=sizeof(TCHAR) * 2;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.hwndOwner 			= hwndIn;
	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hInstance			= hInst;
	ofn.lpstrFilter 		= szOPENFILTER;
	ofn.lpstrDefExt 		= szOPENEXTDFT;
	ofn.nFilterIndex 		= 1;
	ofn.lpstrFile 			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.Flags 				= OFN_PATHMUSTEXIST |
									OFN_NOREADONLYRETURN |
									OFN_OVERWRITEPROMPT |
									OFN_HIDEREADONLY |
									OFN_ENABLEHOOK |
									OFN_ENABLETEMPLATE;
	ofn.lpfnHook			= (LPOFNHOOKPROC)Hook3d;
	ofn.lpTemplateName	= MAKEINTRESOURCE(IDD_SAVE_FILE);

	if(GetSaveFileName(&ofn))
	{
		// Figure out how much room we have
		length = (UWORD)GetTrueWindowTextLength(hwndIn,-1);

		ASSERTCHECK((!length), (LPTSTR)TEXT("Cannot save 0 bytes!  Should not have gotten this far."));

		pbBuf = AllocateMemory(length + cExtra);

		if(pbBuf)
		{
			length = (UWORD)SendMessage(hwndIn, WM_GETTEXT,
								(WPARAM)(length + sizeof(TCHAR)), (LPARAM)pbBuf);

			if ((hFile = GtrOpenFile(hwndIn,szFile,CREATE_ALWAYS,
				WRITETOFILE,idsErrSaveFile)) != INVALID_HANDLE_VALUE)
			{
				cb = GtrWriteFile(hwndIn,hFile,pbBuf,length *sizeof(TCHAR));

				GtrCloseFile(hFile);
			}

			FREE_PTR(pbBuf);
		}

		// We ran out of memory, tell user of our abort
		else
			szMessageBox(hwndIn,
						MB_ICONEXCLAMATION | MB_OK,
						NULL,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsErrSaveFileAlloc),
						(LPTSTR)szFile);

	}  // End successful get save file

	if(hFocus)
		SetFocus(hFocus);

   return;
}

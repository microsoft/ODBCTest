//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	OUTWIN.C
//|
//| Purpose:
//|		This module contains code to handle generic output messages which
//|		will include:
//|			1) Function output when no connnection window is present
//|			2) Auto test output when no connection window is present
//|			3) Output for functions which do not require a connection window,
//|					example: SQLDataSources
//*---------------------------------------------------------------------------------
#include "fhheader.h"

//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define GETOUTWINDOW(x) GetWindowLong(x, 0)
#define SETOUTWINDOW(x, y) SetWindowLong(x, 0, (LONG)y)



//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE		hInst;
extern TCHAR			szOutBuff[MAXOUTBUFF];
extern HWND				hwndClient;
extern HWND				hwndStatBar;
extern HWND				hwndCurMDIChild;  			// Current active MDI child at all times
extern TCHAR 			szWNDPOSSECTION[];
extern lpHEADENV		lpHeadEnv;
extern lpUSROPTIONS	lpUsrOptions;

static TCHAR szOUTWINCLASS[]	= TEXT("GATOR:OUTWIN");


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN OutWinWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwndOutWin;


//*---------------------------------------------------------------------------------
//| CreateOutWinWindow:
//|	This function will create the generic output window used as described above.
//| Parms:
//|	hwndClient				Client window handle
//|	hInst						For use in CreateWindow call
//| Returns:
//|	Window handle of the status bar if successful, NULL on error
//*---------------------------------------------------------------------------------
HWND INTFUN CreateOutWinWindow(HWND hwndClient, HINSTANCE hInst)
{
	MDICREATESTRUCT				mdicreate;
	TCHAR								szTitle[30];

	// Allocate memory for a new node
	GetGtrString(szTitle, sizeof(szTitle)/sizeof(szTitle[0]), idsOutWinTitle);

	//Setup output window with default size
	mdicreate.szClass = szOUTWINCLASS;
	mdicreate.szTitle = szTitle;
	mdicreate.hOwner  = hInst;
	mdicreate.x			= 0;
	mdicreate.y			= 0;
	mdicreate.cx		= OUTWIN_WIDTH;
	mdicreate.cy		= OUTWIN_HEIGHT;
	mdicreate.style	= 0;
	mdicreate.lParam	= (LPARAM)(NULL);

	//Create the Output window with default size
	return (HWND)SendMessage(hwndClient, WM_MDICREATE, 0,
			(LPARAM)(LPMDICREATESTRUCT)&mdicreate);

}



//*---------------------------------------------------------------------------------
//| DestroyOutWinWindow:
//|	This function cleans up the output window by destroying it.
//| Parms:
//|	hwndClient					Window handle of the client area
//|	hwnd							Window handle of the output window
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DestroyOutWinWindow(HWND hwndClient, HWND hwnd)
{
	// Destroy window
	SendMessage(hwndClient, WM_MDIDESTROY, (WPARAM)hwnd, 0L);
	hwndOutWin = NULL;
}



//*---------------------------------------------------------------------------------
//| OutWinWndProc:
//|	Handles any messages that must be processed by the window.
//*---------------------------------------------------------------------------------
LRESULT EXTFUN OutWinWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		//--------------------------------------------------------------------
		// WM_CREATE:		Create the output edit window and remove the
		//						system menu Close option
		//--------------------------------------------------------------------
		case WM_CREATE:
			{
			HWND hOUT=NULL;

			hOUT = CreateEditWindow(hwnd, hInst, IDW_OUTWIN, 0,
					lpUsrOptions->xOutput,
					lpUsrOptions->yOutput,
					lpUsrOptions->nWidthOutput,
					lpUsrOptions->nHeightOutput);

			hwndOutWin=hOUT;

			if(!hwndOutWin)
				return -1;
			PostMessage(hwnd, USER_MOD_MDI_CHILD_MENU, 0, 0L);
			}
			return 0;


		//--------------------------------------------------------------------
		// USER_MOD_MDI_CHILD_MENU:
		//						The default handling of the MDI child system menu
		//						is to replace the system version with a special
		//						version during WM_CREATE.  For this reason, we
		//						cannot modify it until after that message.
		//--------------------------------------------------------------------
		case USER_MOD_MDI_CHILD_MENU:
			{
			HMENU			hMenu;

			// Remove the Close system menu item for the child, as well as its separator
			if(hMenu = GetSystemMenu(hwnd, FALSE)) {
				RemoveMenu(hMenu, IDM_SYSTEM_CLOSE_POS, MF_BYPOSITION);
				RemoveMenu(hMenu, IDM_SYSTEM_CLOSE_POS, MF_BYPOSITION);
				DrawMenuBar(hwnd);
				}
			}
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
			WORD	dx = LOWORD(lParam);
			WORD	dy = HIWORD(lParam);

			// Simply move the edit window to fill the entire MDI child
			MoveWindow(hwndOutWin, 0, 0, dx, dy, TRUE);
			}
			break;


		//--------------------------------------------------------------------
		// Hande the WM_INITMENUPOPUP message so that when the user selects
		//		a menu, we enable/disable each item based on our current state.
		//--------------------------------------------------------------------
		case WM_INITMENUPOPUP:
			{
			if(!(BOOL)HIWORD(lParam))			// Not the system menu
				ResetMenuStatus((HMENU)wParam, (int)LOWORD(lParam), 2);
			}
			break;


		//--------------------------------------------------------------------
		// WM_SYSCOMMAND:		We don't want anyone to close the Output window.
		//							The Close menu item was deleted, but the hot-key
		//							(Ctrl+F4) is still intact.
		//--------------------------------------------------------------------
		case WM_SYSCOMMAND:
			if(wParam == SC_CLOSE)
				return 0;
			break;


		//--------------------------------------------------------------------
		// WM_COMMAND:			For stat bar text
		//--------------------------------------------------------------------
		case WM_COMMAND:
			{
			UINT						id=GET_WM_COMMAND_ID(wParam, lParam);
			UINT						code=GET_WM_COMMAND_CODE(wParam, lParam);

			// Check WM_COMMAND only to set stat bar text
			switch(code) {
				// Put in status bar description of output window
				case EN_SETFOCUS:
					SetStatBarText(hwndStatBar,
									GetGtrString(szOutBuff, MAXOUTBUFF, id),
									BLACK_TEXT);
					break;

				// Sent when the edit control is out of room.  We'll truncate contents
				case EN_ERRSPACE:
					TruncateEditContents(hwndOutWin);
					break;

				case EN_UPDATE:
				case EN_CHANGE:
					{
					RefreshToolbarSaveAs(hwndOutWin);
					}
					break;
				}
			}
			break;


		//--------------------------------------------------------------------
		// WM_MDIACTIVATE:	Just change status bar description for user.
		//--------------------------------------------------------------------
		case WM_MDIACTIVATE:
			{
#ifndef WIN32
			if(wParam) {					// Activate
#else
			if(lParam) {
#endif
				hwndCurMDIChild = GetParent(hwndOutWin);
				SetStatBarText(hwndStatBar,
									GetGtrString(szOutBuff, MAXSTATTEXT, idsOutputWinDesc),
									BLACK_TEXT);
				RefreshToolbar();
				SetFocus(hwndOutWin);

				//output window has been selected, now change hEnv Combo box
				//to display NULL
				if (lpHeadEnv)
					SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL, 0L, 0L);
				}
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

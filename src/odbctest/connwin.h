//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	CONNWIN.H
//|
//| Purpose:
//|		Header file for Connection Window information used to track connections.
//*---------------------------------------------------------------------------------
#ifndef CONNWIN_DEFS
#define CONNWIN_DEFS

#include <Windows.h>
#include "portable.h"
#include "standard.h"
#include "handles.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
extern lpCONNECTIONINFO		lpConnWin;				// Head node for connection windows
extern lpCONNECTIONINFO		lpActiveConnWin;		// The currently active connection window
extern UINT						cNextConnect;			// Current count of connections





//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
CONNECTIONINFO * CreateConnectionWindow(HWND hwndClient, HINSTANCE hInst,
													 	lpENVIRONMENTINFO lpActiveEi);
VOID INTFUN DestroyConnectionWindow(lpCONNECTIONINFO ci);
HWND INTFUN GetEditWindow();
VOID INTFUN SetCurrentHstmtFromList(lpCONNECTIONINFO lpci);
VOID INTFUN SetCurrentStmt(lpCONNECTIONINFO lpci, lpSTATEMENTINFO lpTarget);
VOID INTFUN AdjustDropDownBox(HWND hwnd);
VOID INTFUN HideDropDowns(lpCONNECTIONINFO lpci);
VOID INTFUN ConnWinMoveWindowFocus(lpCONNECTIONINFO lpci);
VOID INTFUN GetConnWinnDescText(lpCONNECTIONINFO lpci, LPTSTR szOut, int cbOut);
VOID INTFUN InitConnectionInfo(lpCONNECTIONINFO ci,lpENVIRONMENTINFO lpEi);


#endif

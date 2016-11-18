//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	OUTWIN.H
//|
//| Purpose:
//|		Header file for Output Window functions
//*---------------------------------------------------------------------------------
#ifndef OUTWIN_DEFS
#define OUTWIN_DEFS

#include <Windows.h>
#include "portable.h"
#include "standard.h"
#include "handles.h"

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
extern HWND		hwndOutWin; 							// The edit child window


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
HWND INTFUN CreateOutWinWindow(HWND hwndClient, HINSTANCE hInst);
VOID INTFUN DestroyOutWinWindow(HWND hwndClient, HWND hwnd);


#endif

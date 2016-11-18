//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	RSLTWIN.H
//|
//| Purpose:
//|		Header file for Results Window information used to track connections.
//*---------------------------------------------------------------------------------
#ifndef RSLTWIN_DEFS
#define RLSTWIN_DEFS

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




//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
RESULTWININFO * CreateResultWindow(HWND hwndClient, HINSTANCE hInst,
						HENV henv, lpCONNECTIONINFO lpci, lpSTATEMENTINFO lpStmt,
						RETCODE rc);
VOID INTFUN DestroyResultWindow(lpRESULTWININFO lprw, BOOL fFreeRowStatus);
VOID INTFUN ResetRsltWin(lpRESULTWININFO lprw);
VOID INTFUN SetMiniBarText(lpRESULTWININFO lprw, LPCTSTR szStr);



#endif

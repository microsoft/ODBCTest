//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	TOOLBAR.H
//|
//| Purpose:
//|		Header file for the status bar functions
//*---------------------------------------------------------------------------------
#ifndef TOOLBAR_DEFS
#define TOOLBAR_DEFS

#include <Windows.h>
#include "portable.h"
#include "standard.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------

// This enum gives the offsets of each button found in the toolbar bitmaps
enum TBButtons{
	TB_FILE_OPEN,
	TB_FILE_SAVEAS,
	TB_FULL_CONNECT,
	TB_FULL_DISCONNECT,
	TB_EXEC_DIRECT,
	TB_GET_DATA_ALL,
	TB_BIND_COL_EXT_FETCH,
	TB_ERROR_ALL,
	TB_FREE_STMT_CLOSE,
	TB_USER_OPTIONS,
	TB_TRACE,
	TB_RUN_AUTO_TESTS,
	TB_FETCH_FIRST,
	TB_FETCH_PRIOR,
	TB_FETCH_NEXT,
	TB_FETCH_LAST,
	TB_TOOLS_RECORD,
	TB_TOOLS_PLAYBACK
	};


//*------------------------------------------------------------------------
//| TOOLBARINFO:
//|	This structure is used to track the tool bar.
//*------------------------------------------------------------------------
typedef struct tagTOOLBARINFO {
	HWND				hwnd;						// Handle to our window
	UINT				dx;						// Width of the window at this point
	UINT				dy;						// Height of window
	RECT				rect;						// Bounding rect for text only
	} TOOLBARINFO;
typedef TOOLBARINFO * lpTOOLBARINFO;

//*------------------------------------------------------------------------
//| TBBUTTONINFO:
//|	This structure is used to track each button's information.
//*------------------------------------------------------------------------
typedef struct tagTBBUTTONINFO {
	UINT				uBtnID;					// The ID of the button
	UINT				uMenuID;					// ID of the associated menu; (-1) if no menu item associated
	UINT				uOptID;					// If no assocated menu, this ID is the ID of the assocated control (ie. IDX_ERR in the UserOpts dlg)
	UINT				uStyle;					// The type of button (Push, toggle)
	UINT				xOffset;					// The offset of the button's bitmap in the AllButtons bitmap
	BOOL				fDown;					// Flag telling if the button is in the Up or Down position; TRUE if Down
	int				idsStatus;				// The ids string number to be displayed in the status bar when button is pressed
	} TBBUTTONINFO;

//*------------------------------------------------------------------------
//| TBBUTTONINFO:
//|	This structure is used to track each button's information.
//*------------------------------------------------------------------------
typedef struct tagTBLAYOUT {
	UINT				uBtnID;					// The ID of the button
	} TBLAYOUT;


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
HWND INTFUN CreateToolBar(HWND hwndParent, HINSTANCE hInst);
VOID INTFUN SizeToolBar(HWND hwnd, UINT dx);
VOID INTFUN RefreshToolbar();
VOID INTFUN RefreshToolbarSaveAs(HWND hwnd);
VOID INTFUN DoFreeStmtClose(BOOL fNoLog);


#endif

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	STATBAR.H
//|
//| Purpose:
//|		Header file for the status bar functions
//*---------------------------------------------------------------------------------
#ifndef STATBAR_DEFS
#define STATBAR_DEFS

#include <Windows.h>
#include "portable.h"
#include "standard.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define MAXSTATTEXT		256
#define WHITE_TEXT		RGB(255,255,255)
#define BLACK_TEXT		RGB(0,0,0)
#define RED_TEXT			RGB(255,0,0)
#define GREEN_TEXT		RGB(0,255,0)
#define BLUE_TEXT			RGB(0,0,255)
#define YELLOW_TEXT 		RGB(255,255,0)
#define LTBLUE_TEXT 		RGB(0,255,255)
#define LTYELLOW_TEXT	RGB(255,255,128)
#define GRAY_TEXT			RGB(192,192,192)

#define STAT_ACCESS_MODE_READ		0x00000001L
#define STAT_AUTO_COMMIT_TRUE		0x00000002L
#define STAT_TRACING_ON				0x00000004L
#define STAT_ASYNC_ON				0x00000008L
#define STAT_BIND_TYPE_COL			0x00000010L


//*------------------------------------------------------------------------
//| STATUSWINDOWINFO:
//|	This structure is used to track all information in the status bar.
//|	A pointer to this structure is kept in the extra bytes of the
//|	window so given the status bar window handle, you can access this
//|	structure.
//*------------------------------------------------------------------------
typedef struct tagSTATUSWINDOWINFO {
	HWND				hwnd;						// Handle to our window
	UINT				dx;						// Width of the window at this point
	UINT				dy;						// Height of window
	UINT				cx;						// Average width of a TCHARacter
	UINT				cy;						// Average height of a character
	HFONT				hFont;					// The font to use for the display
	COLORREF			color;					// Color to use
	TCHAR				szStr[MAXSTATTEXT];	// The text to display
	UDWORD			uFlags;					// Bitmask indicates the value of other  items
	//  Now for the rectangles
	RECT				rect;						// Bounding rect for text only
	RECT				amRect;					// SQL_ACCESS_MODE
	RECT				acRect;					// SQL_AUTO_COMMIT
	RECT				trRect;					// SQL_OPT_TRACE
	RECT				ayRect;					// SQL_ASYNC_ENABLE
	RECT				btRect;					// SQL_BIND_TYPE
	} STATUSWINDOWINFO;
typedef STATUSWINDOWINFO * lpSTATUSWINDOWINFO;



//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
HWND INTFUN CreateStatBar(HWND hwndParent, HINSTANCE hInst);
VOID INTFUN SetStatBarText(HWND hwnd, LPTSTR str, COLORREF color);
VOID INTFUN SetStatBarFlags(HWND hwnd, UDWORD uFlags);
VOID INTFUN SizeStatBar(HWND hwnd, UINT dx);
VOID INTFUN Draw3DBorder(HWND hwnd, HDC hdc, RECT * lpRect, UINT dx);



#endif

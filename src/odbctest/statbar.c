//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	STATBAR.C
//|
//| Purpose:
//|		This module contains all functions required for the status bar
//|		which is present at the bottom of the screen.
//*---------------------------------------------------------------------------------
#include "fhheader.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define ID_STATBAR  						111
#define DFT_STAT_PT_SIZE				10
#define DFT_STAT_FONT					"MS Sans Serif"


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
lpSTATUSWINDOWINFO		lpStat;					// Global pointer to the status window
extern HFONT				hEditFont;				// Default for system, only use if required

extern lpUSROPTIONS		lpUsrOptions;
static TCHAR  szSTATBARCLASS[]	= TEXT("GATOR:STATBAR");


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN StatusWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID INTFUN SizeStatBar(HWND hwnd, UINT dx);



//*---------------------------------------------------------------------------------
//| CreateStatBar:
//|	Call this function to create a window of type status bar.  When
//|	the parent is sized, the SizeStatBar function should be called.
//| Parms:
//|	hwndParent				Parent window handle
//|	hInst						For use in CreateWindow call
//| Returns:
//|	Window handle of the status bar if successful, NULL on error
//*---------------------------------------------------------------------------------
HWND INTFUN CreateStatBar(HWND hwndParent, HINSTANCE hInst)
{
	return CreateWindow(szSTATBARCLASS,
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
			0, 0, 0, 0,
			hwndParent,
			(HMENU)(ID_STATBAR),
			hInst,
			NULL);
}




//*---------------------------------------------------------------------------------
//| StatusWndProc:
//|	Handles any messages that must be processed by the window.
//*---------------------------------------------------------------------------------
LRESULT EXTFUN StatusWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg) {
		//---------------------------------------------------------------------
		// WM_CREATE:  Allocate memory for the status window information
		//						and store it with our window handle.
		//---------------------------------------------------------------------
		case WM_CREATE:
			{
			TEXTMETRIC		tm;
			HFONT				hf;
			HDC				hdc;
			LOGFONT			lf;

			if(!(lpStat = (lpSTATUSWINDOWINFO)AllocateMemory(sizeof(STATUSWINDOWINFO))))
				return -1;
			lpStat->hwnd = hwnd;
			*lpStat->szStr = '\0';


			// Now determine size information
			hdc = GetDC(hwnd);
			if(hdc) {
				// Create a font to use
				InitFont(&lf,(LPTSTR)lpUsrOptions->szFont);
				lf.lfHeight = -MulDiv(DFT_STAT_PT_SIZE,
											GetDeviceCaps(hdc, LOGPIXELSY), 72);
				lf.lfWeight = FW_THIN;
				lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
				lpStat->hFont = CreateFontIndirect(&lf);
				if(!lpStat->hFont)
					lpStat->hFont = hEditFont;						// Take default if there is an error

				// Now size based on current font
				hf = SelectObject(hdc, lpStat->hFont);
				GetTextMetrics(hdc, &tm);
				lpStat->cx = tm.tmAveCharWidth;
				lpStat->cy = tm.tmHeight + tm.tmExternalLeading;
				lpStat->dy = lpStat->cy + (7 * GetSystemMetrics(SM_CYBORDER)) + 2;
				SelectObject(hdc,hf);
				ReleaseDC(hwnd, hdc);
				}
			}
			return FALSE;


		//---------------------------------------------------------------------
		// WM_PAINT:	  Paint the window
		//---------------------------------------------------------------------
		case WM_PAINT:
			{
			HDC							hdc;
			PAINTSTRUCT					ps;

			if(hdc = BeginPaint(hwnd, &ps)) {
				Draw3DBorder(hwnd, hdc, &lpStat->rect, lpStat->dx);
				if(*lpStat->szStr) {
					HFONT				hf;
					hf=SelectObject(hdc, lpStat->hFont);
					SetTextColor(hdc, lpStat->color);
					SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
					ExtTextOut(hdc,
								lpStat->rect.left + 3,
								lpStat->rect.top,
								DFT_ETO_FLAGS,
								&lpStat->rect,
								lpStat->szStr,
								lstrlen(lpStat->szStr),
								NULL);
					SelectObject(hdc, hf);
					}
				EndPaint(hwnd, &ps);
				}
			}
			return FALSE;


		//---------------------------------------------------------------------
		// WM_DESTORY:   Process this message to free memory and destroy font.
		//---------------------------------------------------------------------
		case WM_DESTROY:
			{
			DeleteObject(lpStat->hFont);
			FREE_PTR(lpStat);
			}
			return 0;


		default:
			break;
		}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}



//*---------------------------------------------------------------------------------
//| SetStatBarText:
//|	Will change the current status bar text and cause it to be painted.
//| Parms:
//|	hwnd						The window handle of the status bar
//|	str						The new value
//|	color						The text color to use
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN SetStatBarText(HWND hwnd, LPTSTR str, COLORREF color)
{
	if(lpStat) {
		lpStat->color = color;
		lstrcpy(lpStat->szStr, str);
		InvalidateRect(hwnd, &lpStat->rect, TRUE);
		UpdateWindow(hwnd);
		}
}




//*---------------------------------------------------------------------------------
//| SetStatBarText:
//|	Simply store the flags with the status information and then force a paint.
//| Parms:
//|	hwnd						The window handle of the status bar
//|	uFlags					New flag information
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN SetStatBarFlags(HWND hwnd, UDWORD uFlags)
{
	if(lpStat) {
		lpStat->uFlags = uFlags;
		InvalidateRect(hwnd, &lpStat->rect, TRUE);
		}
}


//*---------------------------------------------------------------------------------
//| SizeStatBar:
//|	This function will adjust the size of our status bar and move it to
//|	the new location.
//| Parms:
//|	hwnd						The window handle of the status bar
//|	dx							The width of the client window
//|	bottom					The bottom of the client window
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN SizeStatBar(HWND hwnd, UINT dx)
{
	UINT						cyExtra = (3 * GetSystemMetrics(SM_CYBORDER));
	UINT						cxExtra = cyExtra * 2;

	lpStat->dx = dx;
	lpStat->rect.left = cxExtra;
	lpStat->rect.top = cyExtra + 2;
	lpStat->rect.right = lpStat->dx - cxExtra;
	lpStat->rect.bottom = lpStat->dy - cyExtra;
}


//*---------------------------------------------------------------------------------
//| Draw3DBorder:
//|	This function will draw a 3d border around the text rectangle.
//| Parms:
//|	hwnd						The window handle
//|	hdc						Device context to draw in
//|	lpStat					The status information
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN Draw3DBorder(HWND hwnd, HDC hdc, RECT * lpRect, UINT dx)
{
	HPEN		hdPen, hlPen, hOldPen;
	int		iPenWidth = GetSystemMetrics(SM_CYBORDER);// * 2;


	// Create a dark pen for the left and top borders
	hdPen = CreatePen(PS_SOLID, iPenWidth, GetSysColor(COLOR_BTNSHADOW));
	hOldPen = SelectObject(hdc, hdPen);
	MoveTo(hdc, lpRect->left-1, lpRect->bottom+1);
	LineTo(hdc, lpRect->left-1, lpRect->top-1);
	LineTo(hdc, lpRect->right+1, lpRect->top-1);

	// Create a light pen for the right and bottom borders
	hlPen = CreatePen(PS_SOLID, iPenWidth, PORT_GETCOLOR_BTN_HIGHLIGHT);
	SelectObject(hdc, hlPen);
	LineTo(hdc, lpRect->right+1, lpRect->bottom+1);
	LineTo(hdc, lpRect->left-1, lpRect->bottom+1);

	// Use light pen for the very top line
	if(dx != (UINT)-1) {
		MoveTo(hdc, 0, 0);
		LineTo(hdc, dx, 0);
		}

	SelectObject(hdc, hOldPen);
	DeleteObject(hdPen);
	DeleteObject(hlPen);
}

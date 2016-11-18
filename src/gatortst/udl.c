//*-------------------------------------------------------------------------
//|  Title: UDL.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|  Purpose:
//|		Simple program to load and run entry points in GATORTST.DLL.
//|
//|  Notes:
//|		Structured programmers will have a heart attack when examining
//|		the gotos in this module.  Please note that special handling
//|		is required when looping through such a dynamic structure as
//|		an outline control with visible flags.  While this could be
//|		accomplished using structured programming techniques, efficiency
//|		is best served by using a more direct approach.  All gotos are
//|		used only when necessary, and only locally within a function.
//*-------------------------------------------------------------------------
#include "gtrhdrs.h"

//#define _VERBOSE_DEBUG_
//#define UDL_DEBUG


//------------------------------------------------------------------------
// Defines and macros
//------------------------------------------------------------------------
#define RELATIVEROW(mousey, lineheight) (mousey / lineheight)
#define NUMROWS(dy, ly) ((dy / ly) + ((dy % ly) ? 1 : 0))
#define ROWTOP(ly, level) (ly * level)
#define ROWBOTTOM(ly, level) ((ly * level) + ly - 1)
#define MILIMETERS 10
#define EXPANSIONWIDTH (16 * MILIMETERS)
#define REPAINTROWS(hwnd)														\
{                                                                 \
	RECT 	rect;                                                    \
	GetClientRect(hwnd, &rect);                                    \
	InvalidateRect(hwnd, &rect, TRUE);                             \
}

//------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------
static TCHAR vszFile[]			= TEXT(__FILE__);
static TCHAR szUDLClass[]		= TEXT("UDLBox");
static TCHAR szQUESTIONS[]		= TEXT("???");


//------------------------------------------------------------------------
// Local Function Prototypes
//------------------------------------------------------------------------
LRESULT EXTFUN UDLWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void INTFUN UDLPaintRows(HDC hdc, lpUDLINFOBOX lpbox, LVLINDEX * top);
void INTFUN UDLPaintItem(HDC hdc, lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lplvl,
			UINT index, RECT * lprect,lpUDLINFOLEVEL lptst);
void EXTFUN DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart);
UDWORD INTFUN UDLVisibleItems(lpUDLINFOBOX lpbox);
UDLINFOLEVEL * UDLFindRelativeRow(lpUDLINFOBOX lpbox, UINT row,
			UINT * lpdex, LVLINDEX * top);
WPARAM INTFUN UDLGetOpCode(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lplvl,
				UINT dex, UINT xPos);
void INTFUN UDLChangeRowStatus(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lplvl,
				UINT dex, UINT row, WPARAM opcode);
void INTFUN UDLSelectEntireAutot(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lpato, UINT dex, BOOL fSel);
void INTFUN UDLSelectEntireGroup(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lpgrp, UINT dex, BOOL fSel);
VOID UDLAdjustCurrentLine(lpUDLINFOBOX lpbox, LVLINDEX * top,
			BOOL fDir, UINT cRows);
HWND INTFUN UDLGetScrollWindow(HWND hwnd, HINSTANCE hInst);
VOID INTFUN FocusLine(HWND hwnd, HDC hdc, lpUDLINFOBOX lpbox);
VOID INTFUN UDLAdjustCurrentRelative(lpUDLINFOBOX lpbox, UINT fDir, UINT rows);
VOID INTFUN UDLAdjustCurrentScrollPos(lpUDLINFOBOX lpbox, UINT fDir, UINT rows);
VOID INTFUN LoadTestCaseDescs(lpUDLINFOLEVEL lptst, LPTSTR szAutoTest);





//*------------------------------------------------------------------------
//|  UDLInit:
//|		This function will register the class required for the UDL
//|		window and procedure.
//|	Parms:
//|		hInst			Instance handle for the register
//|	Returns:
//|		TRUE on success
//*------------------------------------------------------------------------
BOOL UDLInit(HANDLE hInst)
{
	WNDCLASS	wc;

   // Attempt to retrieve the class, if successful, already registered for this load
   if(GetClassInfo(hInst, szUDLClass, &wc))
   	return TRUE;

   // Not yet registered, so do so
   memset(&wc, 0, sizeof(WNDCLASS));
	wc.style = 	CS_BYTEALIGNWINDOW 	|
					CS_CLASSDC			 	|
					CS_DBLCLKS				|
					CS_HREDRAW 				|
					CS_VREDRAW;
	wc.lpfnWndProc = UDLWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(lpUDLINFOBOX);
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szUDLClass;
	RegisterClass(&wc);
	return TRUE;
}


//*------------------------------------------------------------------------
//|  UDLCreateBox:
//|		Creates a new window in the specified location.
//|	Parms:
//|		cLevels		How many levels do you have?
//|		hwnd			Owner window handle
//|		hInst			Instance handle for procs
//|	  * Following are assumed to be in dialog units
//|		nLeft			Upper left
//|		nTop			Top upper left
//|		dx				Width
//|		dy				Height
//|	Returns:
//|		Pointer to new UDLINFOBOX structure
//*------------------------------------------------------------------------
UDLINFOBOX * EXTFUNCDECL UDLCreateBox(UINT cLevels, HWND hwnd, HINSTANCE hInst,
		int nLeft, int nTop, int dx, int dy)
{
	HWND					hwndNew;
	lpUDLINFOBOX		lpbox;
	RECT					rect;
	int					cxScroll = GetSystemMetrics(SM_CXVSCROLL);

	rect.left = nLeft;
	rect.top = nTop;
	rect.right = nLeft + dx;
	rect.bottom = nTop + dy;
	MapDialogRect(hwnd, &rect);

	if(!(lpbox = (lpUDLINFOBOX)AllocateMemory(sizeof(UDLINFOBOX))))
		return NULL;
	lpbox->hInst = hInst;
	if(!(hwndNew = CreateWindow(szUDLClass, NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_GROUP |
			WS_TABSTOP | WS_VISIBLE | WS_BORDER,
			rect.left,
			rect.top,
			rect.right,
			rect.bottom,
			hwnd,
			(HMENU)(5),
			(HANDLE)hInst,
			(LPVOID)lpbox))) {
		ReleaseMemory(lpbox);
		return NULL;
		}

	lpbox->lpdesc = (lpUDLLEVELDESC)AllocateMemory(sizeof(UDLLEVELDESC) * cLevels);
	if(!lpbox->lpdesc)
		return NULL;

	GetClientRect(hwndNew, &rect);			// Adjust for dialog units
	SetWindowLongPtr(hwndNew, 0, (LONG_PTR)lpbox);
	lpbox->hwndOwner = hwnd;
	lpbox->hwndBox = hwndNew;
	lpbox->cLevels = cLevels;
	lpbox->dx = rect.right - rect.left;
	lpbox->dy = rect.bottom - rect.top;
	lpbox->cVisible = (lpbox->dy / lpbox->ly);

	lpbox->hPlus = LoadBitmap(hInst, MAKEINTRESOURCE(IDBT_PLUS));
	lpbox->hMinus = LoadBitmap(hInst, MAKEINTRESOURCE(IDBT_MINUS));
	lpbox->ex = DFTBITMAPWIDTH;

	MoveWindow(lpbox->hwndScroll,
					rect.right - cxScroll,
					0,
					cxScroll,
					lpbox->dy,
					TRUE);
	lpbox->dx -= cxScroll;

	return lpbox;
}


//*------------------------------------------------------------------------
//|  UDLFreeBox:
//|		Frees the memory for an info box.  You may call this function
//|		to free all memory, including levels if desired.
//|	Parms:
//|		lpbox			Pointer to the info box
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID EXTFUN UDLFreeBox(lpUDLINFOBOX lpbox)
{
	UINT		dex;

	if(!lpbox)
		return;

	// Level descriptors
	for(dex=0;  dex<lpbox->cLevels;  dex++)
		DeleteObject(lpbox->lpdesc[dex].hMap);
	ReleaseMemory(lpbox->lpdesc);

	// All data levels
	UDLFreeLevel(lpbox->lplvl);

	// Remainging box items
	DeleteObject(lpbox->hFont);
	DeleteObject(lpbox->hPlus);
	DeleteObject(lpbox->hMinus);
	ReleaseMemory(lpbox);
}


//*------------------------------------------------------------------------
//|  UDLCreateLevelDesc:
//|		Create a level description.  Need do only once per level.
//|		Creates a new level.  Must start at level 0 and go in order
//|		Note that user must specify their own instance handle to
//|		cover the case where the bitmap to load is not in the user's
//|		resource fork.
//|	Parms:
//|		lpbox			Owner box info
//|		level			Which level are you creating (0 based)
//|		hInst			Instance handle to use to load bitmap
//|		szBit			Name of bitmap to load
//|		bx				Bitmap width in milimeters
//|		by				Bitmap height in milimeters
//|	Returns:
//|		Returns TRUE on success, FALSE on failure
//*------------------------------------------------------------------------
BOOL EXTFUN  UDLCreateLevelDesc(
				lpUDLINFOBOX 	lpbox,
				UINT				level,
				HINSTANCE		hInst,
				LPTSTR				szBit,
				UINT				bx,
				UINT				by)
{
	if(!lpbox)
		return FALSE;

	lpbox->lpdesc[level].fx = (lpbox->cbExtra * 2) * level;
	lpbox->lpdesc[level].bx = bx;
	lpbox->lpdesc[level].by = by;
	lpbox->lpdesc[level].ux = lpbox->lpdesc[level].fx + lpbox->ex + lpbox->lpdesc[level].bx +
										lpbox->cbExtra;
	lpbox->lpdesc[level].hMap = LoadBitmap(hInst, szBit);

	return TRUE;
}



//*------------------------------------------------------------------------
//|  UDLCreateLevel:
//|		Creates a new level.  Must start at level 0 and go in order
//|	Parms:
//|		parent		Who is the owner level, NULL if this is the parent
//|		pdex			Parent index
//|		cLevel		What level is this
//|		cItems		How many items at this level?
//|		fLastLevel	Is this the last level (eg: leave next=NULL)
//|	Returns:
//|		Pointer to the new node, NULL on error
//*------------------------------------------------------------------------
UDLINFOLEVEL * EXTFUNCDECL UDLCreateLevel(lpUDLINFOLEVEL parent, UINT pdex,
		UINT cLevel, UINT cItems, BOOL fLastLevel)
{
	lpUDLINFOLEVEL		lplvl;
	int					cDex;				// Number of required ints to cover bitmask

	lplvl = (lpUDLINFOLEVEL)AllocateMemory(sizeof(UDLINFOLEVEL));
	if(!lplvl)
		return NULL;
	lplvl->level = cLevel;
	lplvl->cItems = cItems;
	lplvl->szItems = (lpARRAYLPTSTR)AllocateMemory(sizeof(lpARRAYLPTSTR) * cItems);
	cDex = MINREQUIREDINTS(cItems);
	lplvl->aSelected = (UINT *)AllocateMemory(sizeof(UINT *) * cDex);
	lplvl->aPartial = (UINT *)AllocateMemory(sizeof(UINT *) * cDex);
	lplvl->aExpanded = (UINT *)AllocateMemory(sizeof(UINT *) * cDex);
	if(!lplvl->aSelected ||
		!lplvl->aPartial  ||
		!lplvl->aExpanded)
		goto exit01;
	if(!fLastLevel)
		if(!(lplvl->next = (lpUDLINFOLEVEL *)AllocateMemory(sizeof(lpUDLINFOLEVEL) * cItems)))
			goto exit00;

	if(parent) {
		lplvl->pdex = pdex;
		lplvl->prev = parent;
		}
	return lplvl;

exit00:
	ReleaseMemory(lplvl->aSelected);
	ReleaseMemory(lplvl->aPartial);
	ReleaseMemory(lplvl->aExpanded);

exit01:
	ReleaseMemory(lplvl);
	return NULL;
}


//*------------------------------------------------------------------------
//|  UDLFreeLevel:
//|		Given a level node, this recursive function will find its way
//|		to the bottom of the levels and free from there back up,
//|		eventually getting back to the level passed in and freeing it.
//|	Parms:
//|		lplvl			The level to free
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID EXTFUN UDLFreeLevel(lpUDLINFOLEVEL lplvl)
{
	UINT				dex;

	// Nothing left to free
	if(!lplvl)
		return;

	// When the next pointer is NULL, then we have hit bottom and can free
	// the level safely
	else if(!lplvl->next) {
		UDLFreeLevelText(lplvl);
		ReleaseMemory(lplvl->szItems);
		ReleaseMemory(lplvl->aSelected);
		ReleaseMemory(lplvl->aPartial);
		ReleaseMemory(lplvl->aExpanded);
		ReleaseMemory(lplvl);
		return;
		}

	// There are more levels to free, so recurse to the next level and free,
	// finally freeing this level again
	else  {
		for(dex=0;  dex<lplvl->cItems;  dex++)
			UDLFreeLevel((lpUDLINFOLEVEL)lplvl->next[dex]);
		ReleaseMemory(lplvl->next);
		lplvl->next = NULL;
		UDLFreeLevel(lplvl);
		return;
		}
}



//*------------------------------------------------------------------------
//|  UDLSetLevelText:
//|		Allocates memory and stores the text for the given index
//|	Parms:
//|		lplvl			Pointer to the level
//|		index			Which index value is this
//|		szStr			The string to store
//|	Returns:
//|		Return TRUE on success, FALSE on error
//*------------------------------------------------------------------------
BOOL EXTFUN UDLSetLevelText(lpUDLINFOLEVEL lplvl,
			UINT index,	LPTSTR szStr)
{
	UINT len=(lstrlen(szStr) + 1) * sizeof(TCHAR);

	if(!lplvl ||
		index > lplvl->cItems)
		return FALSE;

	lplvl->szItems[index].str = (LPTSTR)AllocateMemory(len);
	if(!lplvl->szItems[index].str)
		return FALSE;
	lstrcpy(lplvl->szItems[index].str, szStr);
	return TRUE;
}


//*------------------------------------------------------------------------
//|  UDLFreeLevelText:
//|		Frees all memory for all strings at the given level
//|	Parms:
//|		lplvl			Pointer to the level
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID EXTFUN UDLFreeLevelText(lpUDLINFOLEVEL lplvl)
{
	UINT dex;
	if(!lplvl || !lplvl->fLoaded)			// No level or text was not loaded
		return;
	for(dex=0; dex<lplvl->cItems; dex++)
		ReleaseMemory(lplvl->szItems[dex].str);
	return;
}


//*------------------------------------------------------------------------
//| DoUDLVertScroll:
//|		Handle vertical scrolling messages.  When the user presses an
//|		arrow key, scrolling is required, but we do not want to adjust the
//|		focus rectangle.  For this special case, nPos will be
//|		UDL_NO_RELATIVE_SCROLL.
//| Parms:
//|		lpbox					The control information
//|		iScrollCode			Scrolling flag
//|		nPos					New position indicator
//| Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID INTFUN DoUDLVertScroll(lpUDLINFOBOX lpbox, int iScrollCode, UINT nPos)
{
	HWND hwnd=lpbox->hwnd;

	switch(iScrollCode) {
		// This message is hit when the user wants to go all the way to
		//		the top.  We'll just reset all information accordingly
		case SB_TOP:
			lpbox->top.fOpt = UDL_GROUP_ON_TOP;
			lpbox->top.group = 0;
			lpbox->top.autot = 0;
			lpbox->top.testc = 0;
			UDLAdjustCurrentRelative(lpbox, UDL_UP, lpbox->cCurRelative);
			lpbox->cScrollPos = 0;
			UDLAdjustScrollBar(lpbox);
			REPAINTROWS(hwnd);
			break;

		// Message means the End key is pressed
		case SB_BOTTOM:
			{
			UINT  uRows = lpbox->cRange - lpbox->cScrollPos;
			UDLAdjustCurrentLine(lpbox, &lpbox->top, UDL_DOWN, uRows);
			UDLAdjustCurrentScrollPos(lpbox, UDL_DOWN, uRows);
			UDLAdjustCurrentRelative(lpbox, UDL_DOWN, uRows);
			REPAINTROWS(hwnd);
			}
			break;

		// Move list down one item
		case SB_LINEUP:
			if(lpbox->fhwndScroll &&
				lpbox->cScrollPos > 0) {
				UDLAdjustCurrentLine(lpbox, &lpbox->top, UDL_UP, 1);
				UDLAdjustCurrentScrollPos(lpbox, UDL_UP, 1);
				if(nPos != UDL_NO_RELATIVE_SCROLL)
					UDLAdjustCurrentRelative(lpbox, UDL_DOWN, 1);
				REPAINTROWS(hwnd);
				}
			break;

		// Move list up one item
		case SB_LINEDOWN:
			if(lpbox->fhwndScroll &&
				lpbox->cScrollPos < lpbox->cRange) {
				UDLAdjustCurrentLine(lpbox, &lpbox->top, UDL_DOWN, 1);
				UDLAdjustCurrentScrollPos(lpbox, UDL_DOWN, 1);
				if(nPos != UDL_NO_RELATIVE_SCROLL)
					UDLAdjustCurrentRelative(lpbox, UDL_UP, 1);
				REPAINTROWS(hwnd);
				}
			break;

		// Move list down one page
		case SB_PAGEUP:
			if(lpbox->fhwndScroll &&
				lpbox->cScrollPos > 0) {
				UINT	rows=min((UINT)lpbox->cVisible - 1, lpbox->cScrollPos);

				UDLAdjustCurrentLine(lpbox, &lpbox->top, UDL_UP, (UINT)rows);
				UDLAdjustCurrentScrollPos(lpbox, UDL_UP, (UINT)rows);
				if(nPos != UDL_NO_RELATIVE_SCROLL)
					UDLAdjustCurrentRelative(lpbox, UDL_DOWN, (UINT)rows);
				REPAINTROWS(hwnd);
				}
			break;

		// Move list up one page
		case SB_PAGEDOWN:
			if(lpbox->fhwndScroll &&
				lpbox->cScrollPos < lpbox->cRange) {
				UINT	rows=min((UINT)lpbox->cVisible - 1, lpbox->cRange - lpbox->cScrollPos);

				UDLAdjustCurrentLine(lpbox, &lpbox->top, UDL_DOWN, (UINT)rows);
				UDLAdjustCurrentScrollPos(lpbox, UDL_DOWN, (UINT)rows);
				if(nPos != UDL_NO_RELATIVE_SCROLL)
					UDLAdjustCurrentRelative(lpbox, UDL_UP, (UINT)rows);
				REPAINTROWS(hwnd);
				}
			break;

		// If the user drags the thumb, move to that location in the list
		case SB_THUMBTRACK:
			{
			UINT uRows=abs(nPos - lpbox->cScrollPos);
         UINT fDir=(nPos < lpbox->cScrollPos) ? UDL_UP : UDL_DOWN;

			UDLAdjustCurrentLine(lpbox, &lpbox->top, fDir, uRows);
			UDLAdjustCurrentScrollPos(lpbox, fDir, uRows);
			UDLAdjustCurrentRelative(lpbox, fDir, uRows);
			REPAINTROWS(hwnd);
			}
			break;
		}
}



//*------------------------------------------------------------------------
//|  UDLWndProc:
//|		This function handles all of the messages received by the
//|		user box.
//|	Parms:
//|		Depends on the message
//|	Returns:
//|		TRUE if we processed the message
//*------------------------------------------------------------------------
LRESULT EXTFUN UDLWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static 		int xPos, yPos;
	static		TCHAR outbuff[150];


	switch(msg) {
		//---------------------------------------------------------------------
		// WM_CREATE is processed in order to create the output edit window
		//		in which all of our output will be seen.
		//---------------------------------------------------------------------
		case WM_CREATE:
			{
			LOGFONT						lf;
			HDC							hdc;
			CREATESTRUCT *		cs;
			lpUDLINFOBOX 				lpbox;

			cs = (CREATESTRUCT *)lParam;
			lpbox = (lpUDLINFOBOX)cs->lpCreateParams;
			lpbox->hwnd = hwnd;

			memset(&lf, 0, sizeof(LOGFONT));
			lf.lfCharSet=DEFAULT_CHARSET;
			lf.lfHeight = -9;
			lf.lfWeight = FW_NORMAL;
			lf.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
			lstrcpy(lf.lfFaceName, (LPTSTR)"MS Sans Serif");
			if(!(lpbox->hFont = CreateFontIndirect(&lf)))
				GtrMessageBox(hwnd, (LPTSTR)"Could not create font", (LPTSTR)"title", MB_OK);
			else {
				HFONT				hf;
				TEXTMETRIC		tm;

				// Get default sizes for TCHARacters and line spacing
				hdc = GetDC(hwnd);
				hf = SelectObject(hdc, lpbox->hFont);
				GetTextMetrics(hdc, &tm);
				lpbox->cx = tm.tmAveCharWidth;
				SelectObject(hdc, hf);
				ReleaseDC(hwnd, hdc);
				lpbox->cy = tm.tmHeight + tm.tmExternalLeading;
				lpbox->cbExtra = (7 * GetSystemMetrics(SM_CYBORDER));
				lpbox->ly = lpbox->cy + lpbox->cbExtra;

				// Create our scrollbar
				lpbox->hwndScroll = CreateWindowA("scrollbar",
							NULL,
							WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | SBS_VERT,
							0, 0, 0, 0,
							hwnd,
							(HMENU)(100),
							(HANDLE)lpbox->hInst,
							NULL);
				}
			}
			return FALSE;


		//---------------------------------------------------------------------
		// WM_LBUTTONDOWN is received when left mouse button is pressed
		//---------------------------------------------------------------------
		case WM_LBUTTONDOWN:
			{
			static lpUDLINFOBOX 		lpbox;
			static lpUDLINFOLEVEL	lplvl;
			static UINT					dex;
			static UINT					row;
			static WPARAM				opcode;

			lpbox = GETUDLINFOBOX(hwnd);
			if(GetFocus() != lpbox->hwndBox)
				SetFocus(lpbox->hwndBox);
			if(!lpbox->lplvl)
				return TRUE;
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			row = RELATIVEROW(yPos, lpbox->ly);
			if((lplvl = UDLFindRelativeRow(lpbox, row, &dex, &lpbox->top)))
				if((opcode = UDLGetOpCode(lpbox, lplvl, dex, xPos))) {
					UDLChangeRowStatus(lpbox, lplvl, dex, row, opcode);
					FocusLine(hwnd, NULL, lpbox);
					lpbox->cCurRelative = row;
					FocusLine(hwnd, NULL, lpbox);
					}
			}
			return FALSE;

		//---------------------------------------------------------------------
		// WM_PAINT is called when we need to paint our window.
		//---------------------------------------------------------------------
		case WM_PAINT:
			{
			static HDC					hdc;
			static PAINTSTRUCT		ps;
			static lpUDLINFOBOX 		lpbox;

			lpbox = GETUDLINFOBOX(hwnd);
			if(!lpbox->lplvl)
				return TRUE;
			hdc = BeginPaint(hwnd, &ps);
			UDLPaintRows(hdc, lpbox, &lpbox->top);
			EndPaint(hwnd, &ps);
			}
			return TRUE;

		//---------------------------------------------------------------------
		// WM_VSCROLL:			Handle vertical scroll messages
		//---------------------------------------------------------------------
		case WM_VSCROLL:
			{
			lpUDLINFOBOX 		lpbox;

			lpbox = GETUDLINFOBOX(hwnd);
			if(!lpbox->lplvl)
				return TRUE;

			DoUDLVertScroll(lpbox,
							 	GET_WM_VSCROLL_CODE(wParam, lParam),
							 	GET_WM_VSCROLL_POS(wParam, lParam));
			}
			return TRUE;

		//---------------------------------------------------------------------
		// WM_GETDLGCODE must tell Windows we want keyboard input.
		//---------------------------------------------------------------------
		case WM_GETDLGCODE:
			return DLGC_WANTARROWS | DLGC_WANTCHARS;

		//---------------------------------------------------------------------
		// WM_CHAR is sent whenever the user presses a character.
		//---------------------------------------------------------------------
		case WM_CHAR:
			{
			static lpUDLINFOBOX 		lpbox;
			static lpUDLINFOLEVEL	lplvl;
			static UINT					dex;
			static UINT					row;
			static WPARAM				opcode;

			lpbox = GETUDLINFOBOX(hwnd);
			if(!lpbox->lplvl)
				return TRUE;

			switch(wParam) {
				case ' ':											// Means select current row
					row = lpbox->cCurRelative;
					if((lplvl = UDLFindRelativeRow(lpbox, row, &dex, &lpbox->top)))
						UDLChangeRowStatus(lpbox, lplvl, dex, row, UDL_SELCHANGE);
					return TRUE;
				}
			}
			break;

		//---------------------------------------------------------------------
		// WM_KEYDOWN is sent whenever the user presses a key.
		//---------------------------------------------------------------------
		case WM_KEYDOWN:
			{
			static lpUDLINFOBOX 		lpbox;

			lpbox = GETUDLINFOBOX(hwnd);
			switch(wParam) {
				case VK_HOME:
					DoUDLVertScroll(lpbox, SB_TOP, 0);
					break;

				case VK_END:
					DoUDLVertScroll(lpbox, SB_BOTTOM, 0);
					break;

				case VK_UP:							// Pressed up arrow key
					if(lpbox->cCurRelative > 0)								// Room to move
						UDLAdjustCurrentRelative(lpbox, UDL_UP, 1);
					else
						DoUDLVertScroll(lpbox, SB_LINEUP, UDL_NO_RELATIVE_SCROLL);
					break;

				case VK_DOWN:
					{
					if(lpbox->cCurRelative + 1 < lpbox->cVisible &&
						lpbox->cCurRelative + 1 < lpbox->cTotVisible) 		// Room to move
						UDLAdjustCurrentRelative(lpbox, UDL_DOWN, 1);
					else
						DoUDLVertScroll(lpbox, SB_LINEDOWN, UDL_NO_RELATIVE_SCROLL);
					}
					break;

				case VK_PRIOR:															// Page up
					{
					if(lpbox->cCurRelative > 0)									// Room to move
						UDLAdjustCurrentRelative(lpbox, UDL_UP, lpbox->cCurRelative);
					else
						DoUDLVertScroll(lpbox, SB_PAGEUP, UDL_NO_RELATIVE_SCROLL);
					}
					break;

				case VK_NEXT:
					{
					if(lpbox->cCurRelative + 1 < lpbox->cVisible &&
						lpbox->cCurRelative + 1 < lpbox->cTotVisible) 		// Room to move
						UDLAdjustCurrentRelative(lpbox, UDL_DOWN,
								(UINT)(min(lpbox->cVisible,lpbox->cTotVisible) - lpbox->cCurRelative - 1));
					else
						DoUDLVertScroll(lpbox, SB_PAGEDOWN, UDL_NO_RELATIVE_SCROLL);
					}
					break;

				case VK_F4:												// Expand/contract
					{
					static lpUDLINFOLEVEL	lplvl;
					static UINT					dex;
					if((lplvl = UDLFindRelativeRow(lpbox, lpbox->cCurRelative, &dex, &lpbox->top)))
						UDLChangeRowStatus(lpbox, lplvl, dex, lpbox->cCurRelative, UDL_EXPAND);
               }
               break;
				}
			}
			break;

		//---------------------------------------------------------------------
		// WM_KILLFOCUS is sent whenever we gain or loose focus
		//---------------------------------------------------------------------
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			{
			static lpUDLINFOBOX 		lpbox;
			lpbox = GETUDLINFOBOX(hwnd);
			FocusLine(hwnd, NULL, lpbox);
			}
			return 0;

		default:
			break;
		}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}



//*------------------------------------------------------------------------
//|  UDLPaintRows:
//|		Function will paint all rows in the list box based on the
//|		box pointer for the window and how many are visible.
//|
//|		Note:  Ideally this code would use recursion to go through
//|				the levels thereby assuring that multiple levels
//|				could be used (see code for freeing a list).  This
//|				however is much faster and easier to code.
//|	Parms:
//|		hdc				Device context to paint in
//|		lpbox				The info to use for painting
//|		top				Information on the top row to start painting
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN UDLPaintRows(HDC hdc, lpUDLINFOBOX lpbox, LVLINDEX * top)
{
	UINT					grp, ato, tst;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;
	RECT					rect;
	HFONT					hf;
	UDWORD				cVisible=lpbox->cVisible;

	if(!lpbox->lplvl)
		return;


	// Set font and painting area defaults
	hf = SelectObject(hdc, lpbox->hFont);
	SetRect(&rect, 0, 0, lpbox->dx, lpbox->ly);

	// Use top level information to hold place
	lpgrp = lpbox->lplvl;
	if(!(lpato = lpgrp->next[top->group]))
		return;
	if(!(lptst = lpato->next[top->autot]))
		return;
	grp = top->group;
	ato = top->autot;
	tst = top->testc;

	if(top->fOpt & UDL_GROUP_ON_TOP)
		goto group;
	else if(top->fOpt & UDL_AUTOT_ON_TOP)
		goto autotest;
	else
		goto testcase;

	// Paint the group item then see if its lower levels are showing
	for(grp=0;  grp<lpgrp->cItems && (cVisible);  grp++)
	{
group:// Starting point for group on top
		UDLPaintItem(hdc, lpbox, lpgrp, grp, &rect,lptst);
		--cVisible;
		lpato = lpgrp->next[grp];

		// More to show and level is exposed
		if(cVisible && BitGet(lpgrp->aExpanded, grp))
		{
			// Paint the auto test and see if it has test cases showing
			for(ato=0;  ato<lpato->cItems && (cVisible);  ato++)
			{
autotest:	// Starting point for auto test on top
 				lptst = lpato->next[ato];

				UDLPaintItem(hdc, lpbox, lpato, ato, &rect, lptst);
				--cVisible;

				// More to show and level is exposed
				if(cVisible && BitGet(lpato->aExpanded, ato))
				{
					// Paint test case
					for(tst=0;  tst<lptst->cItems && (cVisible);  tst++)
					{
testcase:			// starting point for testcase on top
						UDLPaintItem(hdc, lpbox, lptst, tst, &rect,lptst);
						--cVisible;

					}	// end loop through tst

				} // End visible test cases

			} // end loop through ato

		} // End of visible auto tests

	}  // End loop through grp

	SelectObject(hdc, hf);				// Restore old font

	// Now paint the focus rectangle, if we have focus
	if(GetFocus() == lpbox->hwndBox)
		FocusLine(NULL, hdc, lpbox);
}



//*------------------------------------------------------------------------
//|  UDLPaintItem:
//|		Do the actualy output to the device context.  Adjust the
//|		rectangle to point to the next logical row
//|	Parms:
//|		hdc				Device context to paint in
//|		lpbox				Use for fonts and sizes
//|		lplvl				Level with information
//|		index				Which item at this level to paint?
//|		lprect			The bounding rect to paint in
//|		lptst				Auto test
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN UDLPaintItem(HDC hdc, lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lplvl,
			UINT index, RECT * lprect,lpUDLINFOLEVEL lptst)
{
	lpUDLLEVELDESC		lpdesc=&lpbox->lpdesc[lplvl->level];
	LPTSTR					szVal;
	RECT					txtrect;

	// Paint background as selected if it is so
	if(BitGet(lplvl->aSelected, index) ||
		BitGet(lplvl->aPartial, index)) {
		HBRUSH		hbr;
		SetRect(&txtrect,
				  lpdesc->fx + lpbox->ex + 2,
				  lprect->top,
				  (BitGet(lplvl->aPartial, index))
				  				? lpdesc->fx + lpbox->ex + lpdesc->bx + lpbox->cbExtra
				  				: lpbox->dx,
				  lprect->bottom);
		hbr = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		FillRect(hdc, &txtrect, hbr);
		DeleteObject(hbr);
		}

	// Paint the expansion indicator if we're not at last row
	if(lptst)
		DrawBitmap(hdc,
					  BitGet(lplvl->aExpanded, index) ? lpbox->hMinus : lpbox->hPlus,
					  lpdesc->fx + (lpbox->cbExtra / 2),
					  lprect->top + (lpbox->cbExtra / 2));

	// Paint the user bitmap
	DrawBitmap(hdc,
				  lpdesc->hMap,
				  lpdesc->fx + lpbox->ex + (lpbox->cbExtra / 2),
				  lprect->top + (lpbox->cbExtra / 2));

	// Paint the user text
	SetRect(&txtrect,
			  lpdesc->ux,
			  lprect->top,
			  lpbox->dx,
			  lprect->top + lpbox->ly);
	if(BitGet(lplvl->aSelected, index)) {
		SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
		SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
		}
	else {
		SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
		SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
		}

	szVal = lplvl->szItems[index].str;
	if(!szVal)
		szVal = szQUESTIONS;

	ExtTextOut(hdc,
					txtrect.left + 2,
					txtrect.top + 3,
					ETO_OPAQUE,
					&txtrect,
					szVal,
					lstrlen(szVal),
					(LPINT)NULL);

	// Adjust the rectangle for next time
	lprect->top += lpbox->ly;
	lprect->bottom += lpbox->ly;
}



//*------------------------------------------------------------------------
//|  DrawBitmap:
//|		Stolen out of Petzold's programming windows book.  This function
//|		will draw a bitmap at the xStart and yStart locations.
//|	Parms:
//|		hdc				Device context to paint in
//|		hBitmap			Handle to the bitmap to draw
//|		xStart			From left
//|		yStart			From top
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID EXTFUN DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart)
{
	BITMAP		bm;
	HDC			hdcMem;
	POINT			ptSize, ptOrg;

	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	SetMapMode(hdcMem, GetMapMode(hdc));

	GetObject(hBitmap, sizeof(BITMAP), (LPTSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdc, &ptSize, 1);

	ptOrg.x = 0;
	ptOrg.y = 0;
	DPtoLP(hdcMem, &ptOrg, 1);

	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y,
			hdcMem, ptOrg.x, ptOrg.y, SRCAND);

	DeleteDC(hdcMem);
}



//*------------------------------------------------------------------------
//|  UDLVisibleItems:
//|		Returns the count of the total visible items.
//|	Parms:
//|		lpbox				The info to use for painting
//|	Returns:
//|		Number of items which could be displayed if there was enough room
//*------------------------------------------------------------------------
UDWORD INTFUN UDLVisibleItems(lpUDLINFOBOX lpbox)
{
	UINT					grp, ato, tst;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;
	UDWORD				cVisible=0;

	if(!lpbox->lplvl)
		return 0;
	lpgrp = lpbox->lplvl;
	for(grp=0;  grp<lpgrp->cItems;  grp++) {
		++cVisible;
		lpato = lpgrp->next[grp];
		if(BitGet(lpgrp->aExpanded, grp)) {
			for(ato=0;  ato<lpato->cItems;  ato++) {
				++cVisible;
				lptst = lpato->next[ato];
				if(BitGet(lpato->aExpanded, ato))
					for(tst=0;  tst<lptst->cItems;  tst++)
						++cVisible;
				}
			}
		}
	return cVisible;
}



//*------------------------------------------------------------------------
//|  UDLFindRelativeRow:
//|		Returns a level pointer and index of an item which corresponds
//|		to a relative row in the list box.
//|	Parms:
//|		lpbox				The info to use for painting
//|		row				What relative row to look for
//|		lpdex				Pointer to storage to store index
//|		top				Information on top level
//|	Returns:
//|		Pointer to level information for the relative row
//*------------------------------------------------------------------------
UDLINFOLEVEL * UDLFindRelativeRow(lpUDLINFOBOX lpbox, UINT row,
			UINT * lpdex, LVLINDEX * top)
{
	UINT					grp, ato, tst;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;

	++row;			// Adjust for 0 based rows

	// Use top level information to hold place
	lpgrp = lpbox->lplvl;
	if(!(lpato = lpgrp->next[top->group]))
		return NULL;
	if(!(lptst = lpato->next[top->autot]))
		return NULL;
	grp = top->group;
	ato = top->autot;
	tst = top->testc;
	if(top->fOpt & UDL_GROUP_ON_TOP)
		goto group;
	else if(top->fOpt & UDL_AUTOT_ON_TOP)
		goto autotest;
	else
		goto testcase;

	for(grp=0;  grp<lpgrp->cItems && (row);  grp++) {
group:
		if(!--row) {
			*lpdex = grp;
			return lpgrp;
			}
		lpato = lpgrp->next[grp];
		if(BitGet(lpgrp->aExpanded, grp)) {
			for(ato=0;  ato<lpato->cItems && (row);  ato++) {
autotest:
				if(!--row) {
					*lpdex = ato;
					return lpato;
					}
				lptst = lpato->next[ato];
				if(BitGet(lpato->aExpanded, ato))
					for(tst=0;  tst<lptst->cItems && (row);  tst++)
testcase:
						if(!--row) {
							*lpdex = tst;
							return lptst;
							}
				}
			}
		}
	*lpdex = 0;					// Error condition
	return NULL;
}



//*------------------------------------------------------------------------
//|  UDLGetOpCode:
//|		Given a row, index, and mouse position, this function will
//|		return the meaning of a click at that location.
//|	Parms:
//|		lpbox				The info to use for painting
//|		lplvl				Level information
//|		dex				Which item
//|		xPos				Mouse position
//|	Returns:
//|		An opcode which describes the meaning of the mouse click
//|				0						User clicked free space
//|				UDL_EXPAND			User clicked the expansion indicator
//|				UDL_SELCHANGE		User wants to select/deselect item
//*------------------------------------------------------------------------
WPARAM INTFUN UDLGetOpCode(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lplvl,
				UINT dex, UINT xPos)
{
	lpUDLLEVELDESC		lpdesc=&lpbox->lpdesc[lplvl->level];

	if(xPos <= lpdesc->fx) 		// Click in free space means nothing
		return 0;
	else if((lplvl->next) &&								// There is exspansion indicator at this level
				(xPos <= (lpdesc->fx + lpbox->ex)))		// 	and it was clicked
		return UDL_EXPAND;
	else if(xPos <= lpbox->dx)								// Can't be on disabled scroll bar
		return UDL_SELCHANGE;								// User selection change
	else
		return 0;
}



//*------------------------------------------------------------------------
//|  UDLChangeRowStatus:
//|		Change the given row according to the opcode given.  Note this
//|		could affect other rows as well.
//|	Parms:
//|		lpbox				The info to use for painting
//|		lplvl				Level information
//|		dex				Which item
//|		row				Which relative row is it?
//|		opcode			What should we do with it?
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN UDLChangeRowStatus(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lplvl,
				UINT dex, UINT row, WPARAM opcode)
{
	RECT					rect;

	GetClientRect(lpbox->hwndBox, &rect);
	switch(opcode) {
		case UDL_SELCHANGE:
			switch(lplvl->level) {
				case UDL_GROUP:
					UDLSelectEntireGroup(lpbox, lplvl, dex, !BitGet(lplvl->aSelected, dex));
					break;

				case UDL_AUTOT:
					UDLSelectEntireAutot(lpbox, lplvl, dex, !BitGet(lplvl->aSelected, dex));
					break;

				case UDL_TESTC:
					BitFlip(lplvl->aSelected, dex);
					break;
				}
			FocusLine(lpbox->hwndBox, NULL, lpbox);
			lpbox->cCurRelative = row;
			UDLAdjustPartial(lplvl, dex);

			// Tell caller that the selection changed
			SendMessage(lpbox->hwndOwner, UDL_SELECTION_CHANGED, 0, 0L);

			break;

		case UDL_EXPAND:
			if(lplvl->next)
			{	// Make sure we can
				if(lplvl->level == AUTOT_LEVEL &&
					lplvl->next[dex] &&
					lplvl->next[dex]->fLoaded == FALSE)
					LoadTestCaseDescs(lplvl->next[dex], lplvl->szItems[dex].str);
				FocusLine(lpbox->hwndBox, NULL, lpbox);
				lpbox->cCurRelative = row;
				BitFlip(lplvl->aExpanded, dex);
				rect.top += (lpbox->ly * row);								// Only paint changed rows
				UDLAdjustScrollBar(lpbox);
				}
			else
				return;
			break;

		default:
			return;
		}
	InvalidateRect(lpbox->hwndBox, &rect, TRUE);
}


//*------------------------------------------------------------------------
//|  UDLSelectEntireGroup:
//|		This function will select the desired group and all of the
//|		auto tests and test cases under it.
//|	Parms:
//|		lpbox				The info to use for painting
//|		lpgrp				Level information
//|		dex				Which item
//|		fSel				Select or Deselect
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN UDLSelectEntireGroup(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lpgrp, UINT dex, BOOL fSel)
{
	UINT	tmp;
	lpUDLINFOLEVEL lpato;

	BitSet(lpgrp->aSelected, dex);
	lpato = lpgrp->next[dex];
	for(tmp=0;  tmp<lpato->cItems;  tmp++)
		UDLSelectEntireAutot(lpbox, lpato, tmp, fSel);
}


//*------------------------------------------------------------------------
//|  UDLSelectEntireAutot:
//|		This function will select the desired autotest and all
//|		test cases for that auto test.
//|	Parms:
//|		lpbox				The info to use for painting
//|		lpato				Level information
//|		dex				Which item
//|		fSel				Select or Deselect
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN UDLSelectEntireAutot(lpUDLINFOBOX lpbox, lpUDLINFOLEVEL lpato, UINT dex, BOOL fSel)
{
	UINT	tmp, reqd;
	lpUDLINFOLEVEL lptst;

	if (!(lptst = lpato->next[dex]))
		return;

	reqd = MINREQUIREDINTS(lptst->cItems);

	if(fSel)
	{
		BitSet(lpato->aSelected, dex);
		for(tmp=0;  tmp<reqd;  tmp++)
			lptst->aSelected[tmp] = (UINT)-1;
	}
	else
	{
		BitClear(lpato->aSelected, dex);

		for(tmp=0;  tmp<reqd;  tmp++)
			lptst->aSelected[tmp] = 0;
	}
}


//*------------------------------------------------------------------------
//|  UDLAdjustPartial:
//|		This function will adjust the partial selection bitmaks based
//|		on the value of flag.
//|	Parms:
//|		lplvl				The level changed
//|		dex				The index
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
void INTFUN UDLAdjustPartial(lpUDLINFOLEVEL lplvl, UINT dex)
{
	//  This function is a reactive function.  It will look at all
	//		conditions and set flags accordingly.
	switch(lplvl->level) {
		case UDL_GROUP:
			{
			lpUDLINFOLEVEL lpato = lplvl->next[dex];		// auto tests of this group
			UINT				cSel	= UDLHowManySelections(lpato);
			UINT				cPar 	= UDLHowManyPartials(lpato);

			if(lpato->cItems == cSel) {
				BitSet(lplvl->aSelected, dex);				// All auto tests selected,
				BitClear(lplvl->aPartial, dex);				// 	therefore no partials
				}
			else if (cSel || cPar) {							// Partial selection
				BitClear(lplvl->aSelected, dex);				// Not fully selected,
				BitSet(lplvl->aPartial, dex);					//		but partially selected
				}
			else {													// No auto tests selected
				BitClear(lplvl->aSelected, dex);
				BitClear(lplvl->aPartial, dex);
				}
			}
			break;

		// Check the status of an auto test
		case UDL_AUTOT:
			{
			lpUDLINFOLEVEL lpgrp = lplvl->prev;
			UINT				pdex  = lplvl->pdex;
			lpUDLINFOLEVEL lptst = lplvl->next[dex];		// child tests of this auto test
			UINT				cSel	= UDLHowManySelections(lptst);

			if (!lptst)
				break;

			if(lptst->cItems == cSel) {
				BitSet(lplvl->aSelected, dex);				// All test cases selected,
				BitClear(lplvl->aPartial, dex);				// 	therefore no partials
				}
			else if (cSel) {										// Partial selection
				BitClear(lplvl->aSelected, dex);				// Not fully selected,
				BitSet(lplvl->aPartial, dex);					//		but partially selected
				}
			else {													// No test cases selected
				BitClear(lplvl->aSelected, dex);
				BitClear(lplvl->aPartial, dex);
				}
			UDLAdjustPartial(lpgrp, pdex);					// Check out parent group
			}
			break;

		// Check status of test case
		case UDL_TESTC:
			UDLAdjustPartial(lplvl->prev, lplvl->pdex);	// Just check parent
			break;
		}
}


//*------------------------------------------------------------------------
//|  UDLHowManySelections:
//|		Returns count of selected items at a level
//|	Parms:
//|		lplvl				The level to check
//|	Returns:
//|		Total selected items
//*------------------------------------------------------------------------
UINT INTFUN UDLHowManySelections(lpUDLINFOLEVEL lplvl)
{
	UINT	dex;
	UINT	cnt=0;

	for(dex=0; lplvl && (dex < lplvl->cItems);  dex++)
	{
		if(BitGet(lplvl->aSelected, dex))
			++cnt;
	}

	return cnt;

}


//*------------------------------------------------------------------------
//|  UDLHowManyPartials:
//|		Returns count of partially selected items at a level
//|	Parms:
//|		lplvl				The level to check
//|	Returns:
//|		Total selected items
//*------------------------------------------------------------------------
UINT INTFUN UDLHowManyPartials(lpUDLINFOLEVEL lplvl)
{
	UINT	dex;
	UINT	cnt=0;

	for(dex=0;  dex<lplvl->cItems;  dex++)
		if(BitGet(lplvl->aPartial, dex))
			++cnt;
	return cnt;
}


//*------------------------------------------------------------------------
//|  UDLAdjustCurrentLine:
//|		Given the current position, this function will update the
//|		LVLINDEX structure to a new set of values which should be
//|		located at the top of the visible list.
//|	Parms:
//|		lpbox				The info to use for painting
//|		top				Information on top level
//|		fDir				Which direction to move (UDL_UP or UDL_DOWN)
//|		cRows				Number of rows to move
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID UDLAdjustCurrentLine(lpUDLINFOBOX lpbox, LVLINDEX * top,
			BOOL fDir, UINT cRows)
{
	UINT					grp, ato, tst;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;
	BOOL					fSkip=FALSE;						// To get initial position

	// Use top level information to hold place
	++cRows;
	lpgrp = lpbox->lplvl;
	lpato = lpgrp->next[top->group];
	lptst = lpato->next[top->autot];
	grp = top->group;
	ato = top->autot;
	tst = top->testc;

#ifdef UDL_DEBUG
{
	TCHAR	sz[200];
	wsprintf(sz, "Pre: AdjustCurrentLine: fOpt: %u, grp: %u, ato: %u, tst: %u\r\n", top->fOpt, top->group, top->autot, top->testc);
	OutputDebugString(sz);
}
#endif

	if(fDir == UDL_DOWN) {
		if(top->fOpt & UDL_GROUP_ON_TOP)
			goto groupd;
		else if(top->fOpt & UDL_AUTOT_ON_TOP)
			goto autotestd;
		else
			goto testcased;
		}
	else {
		++grp;
		++ato;
		++tst;
		if(top->fOpt & UDL_GROUP_ON_TOP) {
			fSkip = TRUE;
			goto groupu;
			}
		else if(top->fOpt & UDL_AUTOT_ON_TOP) {
			fSkip = TRUE;
			goto autotestu;
			}
		else {
//			fSkip = TRUE;	// zzz
			goto testcaseu;
			}
		}

	//
	// Move forward from current row to find next row, UDL_DOWN
	//
	for(grp=0;  grp<lpgrp->cItems && (cRows);  grp++) {
		// When starting on the next group, reset auto and test to 0
		ato = 0;
		tst = 0;
groupd:
		if(!--cRows) {
			top->fOpt = UDL_GROUP_ON_TOP;
			goto done;
			}
		lpato = lpgrp->next[grp];
		if(BitGet(lpgrp->aExpanded, grp)) {
			for(ato=0;  ato<lpato->cItems && (cRows);  ato++) {
				// When start a new auto test, start at test 0
				tst = 0;
autotestd:
				if(!--cRows) {
					top->fOpt = UDL_AUTOT_ON_TOP;
					goto done;
					}
				lptst = lpato->next[ato];
				if(BitGet(lpato->aExpanded, ato))
					for(tst=0;  tst<lptst->cItems && (cRows);  tst++)
testcased:
						if(!--cRows) {
							top->fOpt = UDL_TESTC_ON_TOP;
							goto done;
							}
				}
			}
		}

	//
	// Move backward from current row to find next row, UDL_UP
	//
	for(grp=lpgrp->cItems;  grp!=0 && (cRows);  grp--) {
groupu:
		if(fSkip) {
			--cRows;
			fSkip = FALSE;
			}
		else if(BitGet(lpgrp->aExpanded, grp-1)) {
			lpato = lpgrp->next[grp-1];
			for(ato=lpato->cItems;  ato!=0 && (cRows);  ato--) {
autotestu:
				if(fSkip) {
					--cRows;
					fSkip = FALSE;
					}
  				else if(BitGet(lpato->aExpanded, ato-1)) {
				 	lptst = lpato->next[ato-1];
					for(tst=lptst->cItems; tst!=0 && (cRows);  tst--) {
testcaseu:
						if(!--cRows) {
							top->fOpt = UDL_TESTC_ON_TOP;
							goto doneu;
							}
						}  // End of tst
					if(!--cRows) {
						top->fOpt = UDL_AUTOT_ON_TOP;
						goto doneu;
						}
					}  // End of autotest expanded
				// Wasn't a skip for test nor an expanded text, so just count it
				else {
					if(!--cRows) {
						top->fOpt = UDL_AUTOT_ON_TOP;
						goto doneu;
						}
					}
				}  // End of ato
			if(!--cRows) {
				top->fOpt = UDL_GROUP_ON_TOP;
				goto doneu;
				}
			}  // End of group expanded
		// Wasn't a skip for group, or expanded group, so just count it
		else {
			if(!--cRows) {
				top->fOpt = UDL_GROUP_ON_TOP;
				goto doneu;
				}
			}
		} // End of grp

doneu:
	if(grp!=0) --grp;
	if(ato!=0) --ato;
	if(tst!=0) --tst;

done:
	top->group = grp;
	top->autot = ato;
	top->testc = tst;

#ifdef UDL_DEBUG
{
	TCHAR	sz[200];
	wsprintf(sz, "Post: AdjustCurrentLine: fOpt: %u, grp: %u, ato: %u, tst: %u\r\n", top->fOpt, top->group, top->autot, top->testc);
	OutputDebugString(sz);
}
#endif

	return;
}


//*------------------------------------------------------------------------
//|  UDLInViewRows:
//|		Return the number of rows that are actually visible currently.
//|	Parms:
//|		lpbox				The info to use for painting
//|	Returns:
//|		Number of visible rows
//*------------------------------------------------------------------------
UINT INTFUN UDLInViewRows(lpUDLINFOBOX lpbox)
{
	UINT					grp, ato, tst;
	lpUDLINFOLEVEL		lpgrp, lpato, lptst;
	UINT					cInView=0;

	// Use top level information to hold place
	lpgrp = lpbox->lplvl;
	lpato = lpgrp->next[lpbox->top.group];
	lptst = lpato->next[lpbox->top.autot];
	grp = lpbox->top.group;
	ato = lpbox->top.autot;
	tst = lpbox->top.testc;

	if(lpbox->top.fOpt & UDL_GROUP_ON_TOP)
		goto groupd;
	else if(lpbox->top.fOpt & UDL_AUTOT_ON_TOP)
		goto autotestd;
	else
		goto testcased;

	for(grp=0;  grp<lpgrp->cItems;  grp++) {
		// When starting on the next group, reset auto and test to 0
		ato = 0;
		tst = 0;
groupd:
		lpato = lpgrp->next[grp];
		++cInView;
		if(BitGet(lpgrp->aExpanded, grp)) {
			for(ato=0;  ato<lpato->cItems;  ato++) {
				// When start a new auto test, start at test 0
				tst = 0;
autotestd:
				lptst = lpato->next[ato];
				++cInView;
				if(BitGet(lpato->aExpanded, ato)) {
					for(tst=0;  tst<lptst->cItems;  tst++)
testcased:			++cInView;
					}
				}
			}
		}


#ifdef UDL_DEBUG
{
TCHAR	sz[200];
wsprintf(sz, "\r\nUDLInViewRows:  cInView: %u\r\n", cInView);
OutputDebugString(sz);
}
#endif

	return cInView;
}


//*------------------------------------------------------------------------
//|  UDLAdjustScrollBar:
//|		Take the current state of the scrollbar and set all
//|		attributes accordingly.
//|	Parms:
//|		lpbox				The info to use for painting
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID UDLAdjustScrollBar(lpUDLINFOBOX lpbox)
{
	UDWORD			cEmpty=0, cInView=0;
	UINT				cOldRange=lpbox->cRange;

	// Adjust current relative row based on number of rows
	//		that can be seen in box
	lpbox->cTotVisible = UDLVisibleItems(lpbox);

#ifdef UDL_DEBUG
{
TCHAR	sz[200];
wsprintf(sz, "\r\nAdjustScrollBar:  cTotVisible: %lu, cVisible: %lu, cRange: %u, cScrollPos: %u\r\n", lpbox->cTotVisible, lpbox->cVisible, lpbox->cRange, lpbox->cScrollPos);
OutputDebugString(sz);
}
#endif


	// We now have at least cVisible items in view if there are that
	//	many items to show.  We can now find a range and set a valid
	//	scroll pos.
	lpbox->cRange = (int)(lpbox->cTotVisible - lpbox->cVisible);
	if(lpbox->cScrollPos > lpbox->cRange)
		lpbox->cScrollPos = lpbox->cRange;

	// If everything now fits in one box, make sure all contents
	//	are in the window
	if(lpbox->cTotVisible < lpbox->cVisible) {
		// Force the list to the top
		lpbox->top.fOpt = UDL_GROUP_ON_TOP;
		lpbox->top.group = 0;
		lpbox->top.autot = 0;
		lpbox->top.testc = 0;
		REPAINTROWS(lpbox->hwnd);

		// Make sure the relative is still on a valid row
		if(lpbox->cCurRelative > lpbox->cTotVisible)
			lpbox->cCurRelative = (UINT)lpbox->cTotVisible - 1;
		}

//	if(lpbox->cTotVisible < lpbox->cVisible  &&		// All fits in one box
//		lpbox->cCurRelative > lpbox->cTotVisible)		//    and we're too far down
//		lpbox->cCurRelative = (UINT)lpbox->cTotVisible - 1;

	//
	// Enable scrollbar if there are enough rows
	//
	if(lpbox->cTotVisible > lpbox->cVisible) {
		// If this function was called when a level was collapsed, then
		//	we may not have cVisible items actually in the show list.
		//	Adjust current position to make this true.
		if(cOldRange != lpbox->cRange) {
			cInView = UDLInViewRows(lpbox);
			if(lpbox->cVisible > cInView) {
				cEmpty = lpbox->cVisible - cInView;
				UDLAdjustCurrentLine(lpbox, &lpbox->top,
								UDL_UP, (UINT)cEmpty);
				REPAINTROWS(lpbox->hwnd);
#ifdef UDL_DEBUG
{
				TCHAR	sz[200];
				wsprintf(sz, "Adjust UDL_UP: %lu\r\n", cEmpty);
				OutputDebugString(sz);
}
#endif
				}
			}

		EnableWindow(lpbox->hwndScroll, TRUE);
		lpbox->fhwndScroll = TRUE;
		// Set range based on number of rows that cannot be seen
		SetScrollRange(lpbox->hwndScroll,
				SB_CTL,
				0,
				lpbox->cRange,
				FALSE);
		SetScrollPos(lpbox->hwndScroll,
				SB_CTL,
				lpbox->cScrollPos,
				TRUE);
#ifdef UDL_DEBUG
{
		TCHAR	sz[200];
		wsprintf(sz, "AdjustScrollBar:  cRange: %u, cScrollPos: %u\r\n", lpbox->cRange, lpbox->cScrollPos);
		OutputDebugString(sz);
}
#endif
		}
	else {
		EnableWindow(lpbox->hwndScroll, FALSE);
		lpbox->fhwndScroll = FALSE;
		lpbox->cScrollPos = 0;
		}
}


//*------------------------------------------------------------------------
//|  UDLGetScrollWindow:
//|		Returns the window handle of the scrollbar in the window.
//|	Parms:
//|		hwnd				Window handle of UDL box
//|		hInst				Instance handle
//|	Returns:
//|		Scrollbar window handle
//*------------------------------------------------------------------------
HWND INTFUN UDLGetScrollWindow(HWND hwnd, HINSTANCE hInst)
{
	BOOL EXTFUN CALLBACK UDLScrollSearch(HWND hwnd, LONG ptr);
	HWND		hwndScroll=NULL;
	DLGPROC	lpfnEnum;

	lpfnEnum = (DLGPROC)MakeProcInstance((FARPROC)UDLScrollSearch, hInst);
	EnumChildWindows(hwnd, (WNDENUMPROC)lpfnEnum, (LPARAM)&hwndScroll);
	FreeProcInstance((FARPROC)lpfnEnum);
	return hwndScroll;
}


//*------------------------------------------------------------------------
//|  UDLScrollSearch:
//|		This callback function is called for each child window.
//|		We're looking for the one of type scrollbar.
//|	Parms:
//|		hwnd				Window handle to check
//|		ptr				Pointer to storage for window
//|	Returns:
//|		TRUE if we should continue, FALSE to end
//*------------------------------------------------------------------------
BOOL EXTFUN CALLBACK UDLScrollSearch(HWND hwnd, LONG ptr)
{
	TCHAR	szCls[20];

	GetClassName(hwnd, szCls, sizeof(szCls)/sizeof(szCls[0]));
	if(lstrcmpi(szCls, (LPTSTR)"scrollbar") == 0) {
		HWND * lphwnd = (HWND *)(LONG_PTR)ptr;
		*lphwnd = hwnd;
		}

	return TRUE;
}



//*------------------------------------------------------------------------
//|  FocusLine:
//|		This function will either draw or erase the focus rectangle.
//|	Parms:
//|		hwnd				Window handle to draw in if hdc not available
//|		hdc				Device context, NULL if hwnd should be used
//|		lpbox				Box information
//|	Returns:
//|		TRUE if we should continue, FALSE to end
//*------------------------------------------------------------------------
VOID INTFUN FocusLine(HWND hwnd, HDC hdc, lpUDLINFOBOX lpbox)
{
	RECT		rect;

	if(hwnd)
		hdc = GetDC(hwnd);

	rect.left = 0;
	rect.right = lpbox->dx;
	rect.top = (lpbox->ly * lpbox->cCurRelative);
	rect.bottom = rect.top + lpbox->ly;
	DrawFocusRect(hdc, &rect);

	if(hwnd)
		ReleaseDC(hwnd, hdc);
}


//*------------------------------------------------------------------------
//|  UDLAdjustCurrentRelative:
//|		Based on current settings, this function will attempt to adjust
//|		the current relative line (the focus rect).
//|	Parms:
//|		lpbox				Box information
//|		fDir				Direction to scroll
//|		rows				How many desired rows
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID INTFUN UDLAdjustCurrentRelative(lpUDLINFOBOX lpbox, UINT fDir, UINT rows)
{
	FocusLine(lpbox->hwndBox, NULL, lpbox);
	switch(fDir) {
		case UDL_DOWN:
			if(lpbox->cCurRelative + 1 < lpbox->cVisible) {					// Room to move
				if((lpbox->cCurRelative += rows) > lpbox->cVisible)		// Too far, back up
					lpbox->cCurRelative = (UINT)lpbox->cVisible - 1;
				}
			break;

		case UDL_UP:
			if(lpbox->cCurRelative > 0) {											// Room to move
				if((int)(lpbox->cCurRelative -= rows) < 0)					// Too far, back up
					lpbox->cCurRelative = 0;
				}
			break;

		default:
			return;
		}
	FocusLine(lpbox->hwndBox, NULL, lpbox);
}


//*------------------------------------------------------------------------
//|  UDLAdjustCurrentScrollPos:
//|		Will adjust scroll bar position up or down
//|	Parms:
//|		lpbox				Box information
//|		fDir				Direction to scroll
//|		rows				How many desired rows
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------
VOID INTFUN UDLAdjustCurrentScrollPos(lpUDLINFOBOX lpbox, UINT fDir, UINT rows)
{
#ifdef UDL_DEBUG
{
TCHAR	sz[200];
wsprintf(sz, "\r\nPre: AdjustCurrentScrollPos:	cRange: %u, cScrollPos: %u, fDir: %d, rows: %u\r\n", lpbox->cRange, lpbox->cScrollPos, fDir, rows);
OutputDebugString(sz);
}
#endif

	switch(fDir) {
		case UDL_DOWN:
			if(lpbox->cScrollPos < lpbox->cRange) {							// Room to move
				if((lpbox->cScrollPos += rows) > lpbox->cRange)
					lpbox->cScrollPos = lpbox->cRange;
				}
			break;

		case UDL_UP:
			if(lpbox->cScrollPos > 0) {											// Room to move
				if((int)(lpbox->cScrollPos -= rows) < 0)
					lpbox->cScrollPos = 0;
				}
			break;

		default:
			return;
		}

#ifdef UDL_DEBUG
{
TCHAR	sz[200];
wsprintf(sz, "Post: AdjustCurrentScrollPos:	cRange: %u, cScrollPos: %u\r\n", lpbox->cRange, lpbox->cScrollPos);
OutputDebugString(sz);
}
#endif

	UDLAdjustScrollBar(lpbox);
}




//*------------------------------------------------------------------------
//|  LoadTestCaseDescs:
//|		This function will load test case descriptions.  They are not
//|		loaded by default to save run time.
//|	Parms:
//|		lptst					Pointer to the node to fill out
//|		szAutoTest			The auto test name to load
//|	Returns:
//|		Pointer to the new level, NULL on error
//*------------------------------------------------------------------------
VOID INTFUN LoadTestCaseDescs(lpUDLINFOLEVEL lptst, LPTSTR szAutoTest)
{
	lpAUTOTESTINFO		head=NULL;
	lpAUTOTESTINFO		ati;
	int					rtn;
	UWORD					dex;
	TCHAR					szTest[AUTO_MAX_TEST_NAME + AUTO_MAX_TESTDESC_NAME + 4];
	TCHAR					szDesc[AUTO_MAX_TESTDESC_NAME];

	if(!(ati = AddAutoTest(&head, szAutoTest, NULL, FALSE, &rtn)))
		return;
	VerifyTestCaseCount(szAutoTest, &(ati->cbTestCases), TRUE);
   for(dex=0;  dex<ati->cbTestCases;  dex++) {
   	// Set the display text for testcase (dex)
		if((*ati->lpTestDesc)((UWORD)(dex+1), (LPTSTR)szTest, (LPTSTR)szDesc)) {
			lstrcat(szTest, (LPTSTR)":  ");
			lstrcat(szTest, szDesc);
			UDLSetLevelText(lptst, dex, szTest);
			}
		}
	lptst->fLoaded = TRUE;
	UnLoadAutoTestDLL(ati);
	DeleteAutoTestList(head);
}

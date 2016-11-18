//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	TOOLBAR.C
//|
//| Purpose:
//|		This module contains all functions required for the tool bar
//|		which is present just below the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"

//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define ID_TOOLBAR  						112
#define TB_BTN_HEIGHT					19
#define TB_BTN_WIDTH						19
#define TB_GROSS_BTN_HEIGHT			24
#define TB_GROSS_BTN_WIDTH				23
#define TB_SEPARATOR_WIDTH				(TB_GROSS_BTN_WIDTH / 3 * 2)
#define TB_SEPARATOR						(UINT)-1
#define TB_NO_BUTTON						(UINT)-1
#define TBBS_NO_STYLE					0
#define TB_NO_MENU						(UINT)-1
#define TB_SAVE_AS_ID					1
#define HWNDENV_XOFFSET					(TB_SEPARATOR_WIDTH * 2)
#define ENV_HEIGHT						500

#define TBBS_CHECKED						0x0100
#define TBBS_INDETERMINATE				0x0200
#define TBBS_DISABLED					0x0400
#define TBBS_PRESSED						0x0800

#define ROP_DSPDxax						0x00E20746L
#define ROP_PSDPxax						0x00B8074AL


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
lpTOOLBARINFO				lpTool;					// Global pointer to the status window
extern HINSTANCE 			hInst;
extern HMENU 				hMenu;
extern HWND					hwndFrame;
extern HWND					hwndOutWin;						// Output window
extern HWND					hwndToolBar;
extern HWND					hwndStatBar;
extern HWND					hwndClient;
extern lpRESULTWININFO	lpActiveResultWin;
extern lpCONNECTIONINFO	lpActiveConnWin;	// The active connection handle
extern lpUSROPTIONS		lpUsrOptions;
extern lpHEADENV			lpHeadEnv;
extern HWND					hwndMDIOutWin;
extern TCHAR				szOutBuff[MAXOUTBUFF];
static TCHAR  				szTOOLBARCLASS[] = TEXT("GATOR:TOOLBAR");
UINT							uLastBtnID;
BOOL							fMouseDown = FALSE;
UINT							uButtonX;
UINT							uButtonY;
BOOL							fInitToolbar = TRUE;
static HDC					hDCGlyphs = NULL;
static HDC					hDCMono = NULL;
static HBRUSH				hbrDither = NULL;
static HBRUSH				hbrBtnHighlight = NULL;
static HBRUSH				hbrBtnShadow = NULL;
static HBITMAP				hBtnBmp = NULL;
static HBITMAP				hMonoBmp = NULL;
static int					gHenvXOffset=0;

dCSEG(TCHAR) szJUSTNULLTERM[]			= TEXT("");

TBBUTTONINFO TBButtonInfo[] = {
//	uBtnID						uMenuID					uOptID					uStyle			xOffset	fDown		idsStatus
// ----------------------- -------------------- -------------------- -------------- -------- -------- ----------------
	TB_FILE_OPEN,				IDM_FILE_OPEN,			0,							0,					0,			FALSE,	IDM_FILE_OPEN,
	TB_FILE_SAVEAS,			IDM_FILE_SAVEAS,		0,							0,					0,			FALSE,	IDM_FILE_SAVEAS,
	TB_FULL_CONNECT,			IDM_FULL_CONNECT,		0,							0,					0,			FALSE,	IDM_FULL_CONNECT,
	TB_FULL_DISCONNECT,		IDM_FULL_DISCONNECT,	0,							0,					0,			FALSE,	IDM_FULL_DISCONNECT,
	TB_EXEC_DIRECT,			TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_CORE_FUNCS+SQL_API_SQLEXECDIRECT,
	TB_GET_DATA_ALL,			IDM_GET_DATA_ALL,		0,							0,					0,			FALSE,	IDM_GET_DATA_ALL,
	TB_BIND_COL_EXT_FETCH,	TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_BIND_COL_EXT_FETCH,
	TB_ERROR_ALL,				IDM_ALL_ERRORS,		0,							0,					0,			FALSE,	IDM_ALL_ERRORS,
	TB_FREE_STMT_CLOSE,		TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_FREE_STMT_CLOSE,
	TB_USER_OPTIONS,			IDM_TOOLS_USER_OPTIONS,
																0,							0,					0,			FALSE,	IDM_TOOLS_USER_OPTIONS,
	TB_TRACE,					IDM_TRACE,				0,							0,					0,			FALSE,	IDM_TRACE,
	TB_RUN_AUTO_TESTS,		IDM_TOOLS_RUN_AUTO_TESTS,
																0,							0,					0,			FALSE,	IDM_TOOLS_RUN_AUTO_TESTS,
	TB_FETCH_FIRST,			TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_FETCH_FIRST,
	TB_FETCH_PRIOR,			TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_FETCH_PRIOR,
	TB_FETCH_NEXT,				TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_FETCH_NEXT,
	TB_FETCH_LAST,				TB_NO_MENU,				0,							0,					0,			FALSE,	IDM_FETCH_LAST,
	//TB_TOOLS_RECORD,			IDM_TOOLS_RECORD,		0,							0,					0,			FALSE,	IDM_TOOLS_RECORD,
	//TB_TOOLS_PLAYBACK,		IDM_TOOLS_PLAYBACK,	0,							0,					0,			FALSE,	IDM_TOOLS_PLAYBACK,
	};


//--------------------------------------------------------------------------
//	TBLayout -- This structure defines the layout of the buttons on the
//					toolbar.  TB_SEPARATOR is used to describe a separator
//					space between buttons.  The order of the buttons in this
//					structure is the order that they will appear on the toolbar.
//--------------------------------------------------------------------------
TBLAYOUT	TBLayout[] = {
	TB_FILE_OPEN,
	TB_FILE_SAVEAS,
	TB_SEPARATOR,
	TB_FULL_CONNECT,
	TB_FULL_DISCONNECT,
	TB_SEPARATOR,
	TB_EXEC_DIRECT,
	TB_GET_DATA_ALL,
	TB_BIND_COL_EXT_FETCH,
	TB_FREE_STMT_CLOSE,
	TB_ERROR_ALL,
	TB_SEPARATOR,
	TB_FETCH_FIRST,
	TB_FETCH_PRIOR,
	TB_FETCH_NEXT,
	TB_FETCH_LAST,
	TB_SEPARATOR,
	TB_USER_OPTIONS,
	TB_TRACE,
	TB_SEPARATOR,
	TB_RUN_AUTO_TESTS,
	TB_SEPARATOR,
	TB_TOOLS_RECORD,
	TB_TOOLS_PLAYBACK,
	};

//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN ToolWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID INTFUN ToolStatBar(HWND hwnd, UINT dx);
VOID INTFUN NonMenuCommandFunc(UINT uBtn);


//*---------------------------------------------------------------------------------
//| GetButtonStyle:
//|	Call this function to get the style of a button
//| Parms:
//|	uBtn						The Button Number (ID)
//| Returns:
//|	The style of the button
//*---------------------------------------------------------------------------------
UINT INTFUN GetButtonStyle(UINT uBtn)
{
	HWND			hwnd = lpActiveConnWin ? lpActiveConnWin->hwndIn : hwndOutWin;

	switch(uBtn){
		case TB_FILE_OPEN:
			return ((!lpActiveResultWin) ? TBBS_NO_STYLE : TBBS_DISABLED);

		case TB_FILE_SAVEAS:
			return (((!lpActiveResultWin) && GetTrueWindowTextLength(hwnd,-1)) ?
						TBBS_NO_STYLE : TBBS_DISABLED);

		case TB_FULL_CONNECT:
			break;

		case TB_FULL_DISCONNECT:
			return ((lpActiveConnWin) ? TBBS_NO_STYLE : TBBS_DISABLED);

		case TB_EXEC_DIRECT:
			if(lpActiveConnWin)
				return ((lpActiveConnWin->lpCurStmt) ? TBBS_NO_STYLE : TBBS_DISABLED);
			else
				return TBBS_DISABLED;

		case TB_GET_DATA_ALL:
			if(lpActiveConnWin)
				return ((lpActiveConnWin->lpCurStmt) ? TBBS_NO_STYLE : TBBS_DISABLED);
			else
				return TBBS_DISABLED;

		case TB_BIND_COL_EXT_FETCH:
 			if(lpActiveConnWin)
				return ((lpActiveConnWin->lpCurStmt) ? TBBS_NO_STYLE : TBBS_DISABLED);
			else
				return TBBS_DISABLED;

		case TB_ERROR_ALL:
			return(GetActiveEnv() ? TBBS_NO_STYLE : TBBS_DISABLED);
			break;
		case TB_FREE_STMT_CLOSE:
 			if(lpActiveConnWin)
				return ((lpActiveConnWin->lpCurStmt) ? TBBS_NO_STYLE : TBBS_DISABLED);
			else
				return TBBS_DISABLED;

		case TB_USER_OPTIONS:
			break;

		case TB_TRACE:
			return ((GetMenuState(hMenu, IDM_TRACE, MF_BYCOMMAND) == MF_CHECKED) ?
						TBBS_CHECKED : TBBS_NO_STYLE);

		case TB_RUN_AUTO_TESTS:
			break;

		case TB_TOOLS_RECORD:
			return ((GetMenuState(hMenu, IDM_TOOLS_RECORD, MF_BYCOMMAND) == MF_CHECKED) ?
						TBBS_CHECKED : TBBS_NO_STYLE);

		case TB_TOOLS_PLAYBACK:
			break;

		case TB_FETCH_FIRST:
		case TB_FETCH_PRIOR:
		case TB_FETCH_LAST:
			{
				if (lpActiveResultWin)
				{
					if (lpActiveResultWin->lpStmt == lpActiveConnWin->lpCurStmt)
					{
						if (lpActiveResultWin->lpStmt->uCursorType != SQL_CURSOR_FORWARD_ONLY)
							return(TBBS_NO_STYLE);
					}
				}

			return (TBBS_DISABLED);
			}
		case TB_FETCH_NEXT:
			{
				if (lpActiveResultWin)
				{
					if (lpActiveResultWin->lpStmt == lpActiveConnWin->lpCurStmt)
						return(TBBS_NO_STYLE);
				}

				return(TBBS_DISABLED);
			}

		default:
			break;
		}

	return TBBS_NO_STYLE;
}

//*---------------------------------------------------------------------------------
//| InvalidateButton:
//|	This function invalidates the rectangle defined by a specific button on
//|	the toolbar
//| Parms:
//|	hwnd						handle to the toolbar window
//|	uBtn						the button number
//| Returns:
//|	The number of the button (from the TBLayout struct) or (UINT)(-1)
//*---------------------------------------------------------------------------------
VOID INTFUN InvalidateButton(HWND hwnd, UINT uBtn)
{
	RECT	rc;
	UINT	idx=0;

	// Find the correct button in the TBButtonInfo struct
	while(TBButtonInfo[idx].uBtnID != uBtn)
		idx++;

	// Set up the correct rectangle for the button
	rc.left = (int)(TBButtonInfo[idx].xOffset);
	rc.top = (int)uButtonY;
	rc.right = (int)(TBButtonInfo[idx].xOffset + TB_GROSS_BTN_WIDTH - 1);
	rc.bottom = (int)(uButtonY + TB_GROSS_BTN_HEIGHT);

	// Invalidate it
	InvalidateRect(hwnd, &rc, TRUE);

	return;
}

//*---------------------------------------------------------------------------------
//| GetIndexFromBtnNumber:
//|	Call this function to get the index of a button (in the TBButtonInfo struct)
//|	from the button number
//| Parms:
//|	uBtn						The Button Number (ID)
//| Returns:
//|	The index of the button; TB_NO_BUTTON if not found
//*---------------------------------------------------------------------------------
UINT INTFUN GetIndexFromBtnNumber(UINT	uBtn)
{
	UINT	idx;
	BOOL	fFound = FALSE;

	for(idx=0; idx<NumItems(TBButtonInfo); idx++){
		if(TBButtonInfo[idx].uBtnID == uBtn){
			fFound = TRUE;
			break;
			}
		}

	if(fFound)
		return idx;
	else
		return TB_NO_BUTTON;
}

//*---------------------------------------------------------------------------------
//| RefreshToolbar:
//|	Call this function to refresh all buttons on the toolbar to their updated style
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN RefreshToolbar(VOID)
{
	UINT	idx;
	UINT	uStyle;

	if(hwndToolBar){
		for(idx=0; idx<NumItems(TBButtonInfo); idx++){
			uStyle = GetButtonStyle(TBButtonInfo[idx].uBtnID);
			if(uStyle != TBButtonInfo[idx].uStyle){
				TBButtonInfo[idx].uStyle = uStyle;
				InvalidateButton(hwndToolBar, TBButtonInfo[idx].uBtnID);
				}
			}
		}
}

//*---------------------------------------------------------------------------------
//| RefreshToolbarSaveAs:
//|	Call this function to refresh the SaveAs button based on the text in the
//|	input window
//| Parms:
//|	hwnd		The handle of the current window (window with the focus)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN RefreshToolbarSaveAs(HWND hwnd)
{
	DWORD	len=0;
	UINT	idx = TB_SAVE_AS_ID;

	len = GetTrueWindowTextLength(hwnd,-1);

	if(len > 0){
		if(TBButtonInfo[idx].uStyle & TBBS_DISABLED){
			TBButtonInfo[idx].uStyle &= ~TBBS_DISABLED;
			InvalidateButton(hwndToolBar, TBButtonInfo[idx].uBtnID);
	   	}
	 	}
	else{
		if(!(TBButtonInfo[idx].uStyle & TBBS_DISABLED)){
			TBButtonInfo[idx].uStyle |= TBBS_DISABLED;
			InvalidateButton(hwndToolBar, TBButtonInfo[idx].uBtnID);
			}
		}
}

//*---------------------------------------------------------------------------------
//| CreateToolBar:
//|	Call this function to create a window of type tool bar.  When
//|	the parent is sized, the SizeToolBar function should be called.
//| Parms:
//|	hwndParent				Parent window handle
//|	hInst						For use in CreateWindow call
//| Returns:
//|	Window handle of the status bar if successful, NULL on error
//*---------------------------------------------------------------------------------
HWND INTFUN CreateToolBar(HWND hwndParent, HINSTANCE hInst)
{
	return CreateWindow(szTOOLBARCLASS,
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
			0, 0, 0, 0,
			hwndParent,
			(HMENU)(ID_TOOLBAR),
			hInst,
			NULL);
}

//*---------------------------------------------------------------------------------
//| CreateDitherBitmap
//|	Call this function to create a dithered bitmap to be used by the DrawButton
//|	function when drawing checked buttons (in the down position)
//| Returns:
//|	Handle to the bitmap if successful
//*---------------------------------------------------------------------------------
HBITMAP INTFUN CreateDitherBitmap()
{
	HDC 			hDC;
	HBITMAP 		hbm;
	long 			patGray[8];
	COLORREF 	clr;
	int			i;
	struct  // BITMAPINFO with 16 colors
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD      bmiColors[16];
	} bmi;
	memset(&bmi, 0, sizeof(bmi));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = 8;
	bmi.bmiHeader.biHeight = 8;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 1;
	bmi.bmiHeader.biCompression = BI_RGB;

	clr = GetSysColor(COLOR_BTNFACE);
	bmi.bmiColors[0].rgbBlue = GetBValue(clr);
	bmi.bmiColors[0].rgbGreen = GetGValue(clr);
	bmi.bmiColors[0].rgbRed = GetRValue(clr);

	clr = GetSysColor(COLOR_BTNHIGHLIGHT);
	bmi.bmiColors[1].rgbBlue = GetBValue(clr);
	bmi.bmiColors[1].rgbGreen = GetGValue(clr);
	bmi.bmiColors[1].rgbRed = GetRValue(clr);

	// initialize the brushes
	for (i = 0; i < 8; i++)
	   patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;

	hDC = GetDC(NULL);
	hbm = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT,
		(LPBYTE)patGray, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hDC);

	return hbm;
}

//*---------------------------------------------------------------------------------
//| InitToolBar:
//|	Call this function to initialize all the global structure related to the toolbar
//| Parms:
//|	hwnd				Toolbar window handle
//| Returns:
//|	FALSE if we could not allocate memory for toolbar struct, TRUE if successful
//*---------------------------------------------------------------------------------
BOOL INTFUN InitToolBar(HWND hwnd)
{
	UINT		idx, idx2=0;
	UINT		cxTemp;
	UINT		cyExtra = (7 * GetSystemMetrics(SM_CYBORDER));
	UINT		cxExtra = cyExtra * 2;
	LOGBRUSH	lb;
	HBITMAP	hbmGray;
	HDC		hDCToolbar;

	if(!(lpTool = (lpTOOLBARINFO)AllocateMemory(sizeof(TOOLBARINFO))))
		return FALSE;
	lpTool->hwnd = hwnd;
	lpTool->dy = TB_GROSS_BTN_HEIGHT + (2 * cyExtra);

	// Calculate the starting coordinates of the buttons -- save them globally
	uButtonX = TB_SEPARATOR_WIDTH;
	uButtonY = cyExtra;

	// Calculate the starting x coordinate of each button
	cxTemp = uButtonX;
	for(idx=0; idx<NumItems(TBLayout); idx++){
		if(TBLayout[idx].uBtnID == TB_SEPARATOR)
			cxTemp += TB_SEPARATOR_WIDTH;
		else{
			for(idx2=0; idx2<NumItems(TBButtonInfo); idx2++){
				if(TBButtonInfo[idx2].uBtnID == TBLayout[idx].uBtnID){
					TBButtonInfo[idx2].xOffset = cxTemp;
					break;
					}
				}
			cxTemp += TB_GROSS_BTN_WIDTH;
			}
		}

	//	Initialize hDC's for disabled images
	hDCToolbar = GetDC(hwnd);
	hDCGlyphs = CreateCompatibleDC(hDCToolbar);
	hDCMono = CreateCompatibleDC(hDCToolbar);

	hBtnBmp = LoadBitmap(hInst, TEXT("TOOLBARBITMAP"));
	SelectObject(hDCGlyphs, hBtnBmp);
	hMonoBmp = CreateBitmap(20,20,1,1,NULL);
	SelectObject(hDCMono, hMonoBmp);

	hbmGray = CreateDitherBitmap();
	if (hbmGray != NULL){
		hbrDither = CreatePatternBrush(hbmGray);
		DeleteObject(hbmGray);
		}

	lb.lbStyle = BS_SOLID;
	lb.lbColor = GetSysColor(COLOR_BTNHIGHLIGHT);
	hbrBtnHighlight = CreateBrushIndirect(&lb);

	lb.lbColor = GetSysColor(COLOR_BTNSHADOW);
	hbrBtnShadow = CreateBrushIndirect(&lb);

	ReleaseDC(hwnd, hDCToolbar);

	return TRUE;
}

//*---------------------------------------------------------------------------------
//| GetBtnNumFromCoord:
//|	This function returns the number of the toolbar button (if any) that is
//|	at the given coordinates
//| Parms:
//|	cx							x-coordinate
//|	cy							y-coordinate
//| Returns:
//|	The number of the button (from the TBLayout struct) or (UINT)(-1)
//*---------------------------------------------------------------------------------
UINT INTFUN GetBtnNumFromCoord(UINT cx, UINT cy)
{
	UINT	idx;
	UINT	cxTemp=0;

	// First check the y-coordinate.  If it is not within the horizontal button area,
	// it is obviously not on a button
	if((cy < uButtonY) || (cy > (uButtonY + TB_GROSS_BTN_HEIGHT)))
		return TB_NO_BUTTON;
	// Now check the x-coordinate against each button in the TBButtonInfo struct
	for(idx=0; idx<NumItems(TBButtonInfo); idx++)
		{
		if((cx >= TBButtonInfo[idx].xOffset) && (cx < (TBButtonInfo[idx].xOffset + TB_GROSS_BTN_WIDTH)))
			return TBButtonInfo[idx].uBtnID;
     	}
	// It wasn't on a button
   return TB_NO_BUTTON;
}

//*---------------------------------------------------------------------------------
//| PatB:
//|	This function creates a bit pattern on the specified hDC
//| Parms:
//|	hDC						handle to the device context
//|	x							x-coordinate of the top-left corner of rectangle
//|	y							y-coordinate of the top-left corner of rectangle
//|	dx							width of the destination rectangle
//|	dy							height of the destination rectangle
//|	rgb						the background color
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN PatB(HDC hDC, int x, int y, int dx, int dy, COLORREF rgb)
{
	RECT	rc;

	rc.left = x;
	rc.top = y;
	rc.right = x + dx;
	rc.bottom = y + dy;

	SetBkColor(hDC, rgb);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

	return;
}

//*---------------------------------------------------------------------------------
//| CreateMask:
//|	This function creates a monochrome bitmap mask
//| Parms:
//|	iImage			The number of the button in the buttons bitmap
//|	ptOffset			The offset of the image in the hDCMono bitmap
//|	bHilite			Flag for highlight
//|	bHiliteShadow	Flag for highlight shadow
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN CreateMask(int iImage, POINT ptOffset, BOOL bHilite, BOOL bHiliteShadow)
{
	PatBlt(hDCMono, 0, 0, TB_BTN_WIDTH, TB_BTN_HEIGHT, WHITENESS);

	SetBkColor(hDCGlyphs, GetSysColor(COLOR_BTNFACE));
	BitBlt(hDCMono, ptOffset.x, ptOffset.y, TB_BTN_WIDTH, TB_BTN_HEIGHT,
				hDCGlyphs, iImage * TB_BTN_WIDTH, 0, SRCCOPY);

	if(bHilite){
		SetBkColor(hDCGlyphs, GetSysColor(COLOR_BTNHIGHLIGHT));
		BitBlt(hDCMono, ptOffset.x, ptOffset.y, TB_BTN_WIDTH, TB_BTN_HEIGHT,
					hDCGlyphs, iImage * TB_BTN_WIDTH, 0, SRCPAINT);

		if(bHiliteShadow)
			BitBlt(hDCMono, 1, 1, TB_BTN_WIDTH - 3, TB_BTN_HEIGHT - 3,
						hDCMono, 0, 0, SRCAND);
		}
}

//*---------------------------------------------------------------------------------
//| DrawButton:
//|	This function draws a button on the toolbar
//| Parms:
//|	hDC						handle to the device context
//|	x							x-coordinate of the top-left corner of rectangle
//|	y							y-coordinate of the top-left corner of rectangle
//|	dx							width of the destination rectangle
//|	dy							height of the destination rectangle
//|	rgb						the background color
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN DrawButton(HDC hDC, int x, int y, int iImage, UINT nStyle)
{
	int 		dx, dy;
	POINT		ptOffset;
	HGDIOBJ	hbrOld;
	COLORREF	crFrame = GetSysColor(COLOR_WINDOWFRAME);
	COLORREF	crBtnFace = GetSysColor(COLOR_BTNFACE);
	COLORREF crShadow = GetSysColor(COLOR_BTNSHADOW);
	COLORREF crHilite = GetSysColor(COLOR_BTNHIGHLIGHT);

	dx = TB_BTN_WIDTH;
	dy = TB_BTN_HEIGHT;

	// Draw the border around the button
	PatB(hDC, x+1, y, dx+3, 1, crFrame);
	PatB(hDC, x+1, y+2+dy+2, dx+3, 1, crFrame);
	PatB(hDC, x, y+1, 1, dy+3, crFrame);
	PatB(hDC, x+2+dx+2, y+1, 1, dy+3, crFrame);

	// Draw the interior grey
	PatB(hDC, x+2, y+2, dx, dy, crBtnFace);

	ptOffset.x = 2;
	ptOffset.y = 2;

	if(nStyle & (TBBS_PRESSED | TBBS_CHECKED)){
		// Pressed in or down or down disabled
		PatB(hDC, x+1, y+1, dx+2, 1, crShadow);
		PatB(hDC, x+1, y+2, 1, dy+1, crShadow);
		// For any depressed button, add 1 to these offsets
		ptOffset.x++;
		ptOffset.y++;
		}
	else{
		// Regular button look
		PatB(hDC, x+1, y+1, dx+2, 1, crHilite);
		PatB(hDC, x+1, y+2, 1, dy+1, crHilite);

		PatB(hDC, x+2+dx+1, y+1, 1, dy+3, crShadow);
		PatB(hDC, x+1, y+2+dy+1, dx+3, 1, crShadow);

		PatB(hDC, x+2+dx, y+2, 1, dy+1, crShadow);
		PatB(hDC, x+2, y+2+dy, dx, 1, crShadow);
		}

	if((nStyle & TBBS_PRESSED) || !(nStyle & TBBS_DISABLED)){
		// normal image version
		BitBlt(hDC, x + ptOffset.x, y + ptOffset.y, TB_BTN_WIDTH, TB_BTN_HEIGHT,
					hDCGlyphs, iImage * TB_BTN_WIDTH, 0, SRCCOPY);

		if(nStyle & TBBS_PRESSED)
			return TRUE;				// nothing more to do, the rest of the style is ignored
		}

	if(nStyle & (TBBS_DISABLED | TBBS_INDETERMINATE)){
		// disabled or indeterminate version
		CreateMask(iImage, ptOffset, TRUE, FALSE);

		SetTextColor(hDC, 0L);
		SetBkColor(hDC, (COLORREF)0x00FFFFFFL);

		if(nStyle & TBBS_DISABLED){
			// disabled - draw the highlighted shadow
			hbrOld = SelectObject(hDC, hbrBtnHighlight);
			if(hbrOld != NULL){
				// draw highlight color where we have 0's in the mask
				BitBlt(hDC, x+1, y+1, dx, dy,
							hDCMono, 0, 0, ROP_PSDPxax);
				SelectObject(hDC, hbrOld);
				}
			}

			// Always draw the shadow
			{
			hbrOld = SelectObject(hDC, hbrBtnShadow);
			if(hbrOld != NULL){
				// draw highlight color where we have 0's in the mask
				BitBlt(hDC, x+2, y+2, dx-1, dy-1,
							hDCMono, 2, 2, ROP_PSDPxax);
				SelectObject(hDC, hbrOld);
				}
			}
		}

	// if it is checked, do the dither brush avoiding the glyph
	if(nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)){
		hbrOld = SelectObject(hDC, hbrDither);
		if(hbrOld != NULL){
			ptOffset.x--;
			ptOffset.y--;
			CreateMask(iImage, ptOffset, ~(nStyle & TBBS_INDETERMINATE),
							nStyle & TBBS_DISABLED);

			SetTextColor(hDC, 0L);
			SetBkColor(hDC, (COLORREF)0x00FFFFFFL);

			// only draw the dither brush where the mask is 1's
			BitBlt(hDC, x+3, y+3, dx+1, dy+1, hDCMono, 2, 2, ROP_DSPDxax);
			SelectObject(hDC, hbrOld);
			}
		}

	return TRUE;
}

//*---------------------------------------------------------------------------------
//| ToolWndProc:
//|	Handles any messages that must be processed by the window.
//*---------------------------------------------------------------------------------
LRESULT EXTFUN ToolWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg) {


		//---------------------------------------------------------------------
		// WM_CREATE:  Allocate memory for the status window information
		//						and store it with our window handle.
		//---------------------------------------------------------------------
		case WM_CREATE:
			{
			UINT	idx;

			// Initialize the global structures for the toolbar
			if(!InitToolBar(hwnd))
				return -1;


			for(idx=0; idx<NumItems(TBButtonInfo); idx++)
				{
				//Capture the offset of the last button
				if ((int)TBButtonInfo[idx].xOffset > gHenvXOffset)
					gHenvXOffset=TBButtonInfo[idx].xOffset;

				}

			gHenvXOffset+=2;

			}
			return FALSE;

		//---------------------------------------------------------------------
		// WM_SIZE:  Allocate memory for the status window information
		//						and store it with our window handle.
		//---------------------------------------------------------------------
		case WM_SIZE:
			{
			WORD							dx = LOWORD(lParam);
			WORD							dy = HIWORD(lParam);

			if (!lpHeadEnv)
				return(FALSE);

			SendMessage(lpHeadEnv->hwndEnvs, CB_SHOWDROPDOWN, FALSE, 0L);

			}
			return FALSE;


		//--------------------------------------------------------------------
		// WM_COMMAND:			Process menu items.  Note that some menu items
		//							are handled by the main window procedure, in
		//							which case we simply pass them on.
		//--------------------------------------------------------------------
		case WM_COMMAND:
			{
			UINT						id=GET_WM_COMMAND_ID(wParam, lParam);
			UINT						code=GET_WM_COMMAND_CODE(wParam, lParam);

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
					HWND					hwnd=GET_WM_COMMAND_HWND(wParam, lParam);
					lpENVIRONMENTINFO lpActiveEnv=GetActiveEnvNode();
					lpCONNECTIONINFO	lpConnWin=NULL;
					UINT					cConnections;

					//now activate all of the connection windows linked to this EI node
					if (lpActiveEnv)
						{
						lpConnWin=lpActiveEnv->lpConn;
						cConnections=lpActiveEnv->cConns;

						while (lpConnWin && cConnections--)
							{
							SendMessage(lpConnWin->hwnd, WM_MDIACTIVATE, (WPARAM)hwnd, (LPARAM)lpConnWin->hwnd);
							lpConnWin=lpConnWin->next;
							}
						}
					else
						{
						//user selected NULL henv from combo box so bring the Output Window into focus
						if(!IsZoomed(hwndMDIOutWin))
							SendMessage(hwndClient, WM_MDIRESTORE, (WPARAM)hwndMDIOutWin, 0L);
						SetFocus(hwndMDIOutWin);
						}




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

						ASSERTCHECK((!lpActiveConnWin->hwndOut), "Invalid Hwnd");
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


		//---------------------------------------------------------------------
		// WM_PAINT:	  Paint the window
		//---------------------------------------------------------------------
		case WM_PAINT:
			{
			HDC					hdc;
			PAINTSTRUCT			ps;
			UINT					idx;
			RECT					rect,
									rc;

         GetUpdateRect(hwnd, &rect, TRUE);

			if(hdc = BeginPaint(hwnd, &ps)) {
				Draw3DEdges(hwnd, hdc, &lpTool->rect);

				for(idx=0; idx<NumItems(TBButtonInfo); idx++){
					// Figure out from the coordinates which Buttons need to be painted
					if(!((((int)uButtonY > rect.bottom) && (((int)(uButtonY + TB_GROSS_BTN_HEIGHT)) > rect.bottom)) ||
						  (((int)uButtonY < rect.top) && (((int)(uButtonY + TB_GROSS_BTN_HEIGHT)) < rect.top)) ||
						  (((int)TBButtonInfo[idx].xOffset > rect.right) && (((int)TBButtonInfo[idx].xOffset + TB_GROSS_BTN_WIDTH - 1) > rect.right)) ||
						  (((int)TBButtonInfo[idx].xOffset < rect.left) && (((int)TBButtonInfo[idx].xOffset + TB_GROSS_BTN_WIDTH - 1) < rect.left)))){
						DrawButton(hdc, (int)TBButtonInfo[idx].xOffset, (int)uButtonY,
										(int)TBButtonInfo[idx].uBtnID, TBButtonInfo[idx].uStyle);

						}
					}

				// The first time through, we need to do a refresh
				if(fInitToolbar){

					RefreshToolbar();

					// Invalidate it
					InvalidateRect(lpHeadEnv->hwndEnvs, &rc, TRUE);

					MoveWindow(lpHeadEnv->hwndEnvs,
									gHenvXOffset + HWNDENV_XOFFSET,   		// x-coord
									uButtonY,										// y-coord
									ENV_WIDTH, 										//	width
									ENV_HEIGHT,										// height
									TRUE);											// repaint flag

					ShowWindow(lpHeadEnv->hwndEnvs,SW_SHOW);

					fInitToolbar = FALSE;
					}


				EndPaint(hwnd, &ps);
				}
			}
			return FALSE;

		//---------------------------------------------------------------------
		// WM_LBUTTONDOWN:	Process this message if the left mouse button
		//							was pressed
		//---------------------------------------------------------------------
		case WM_LBUTTONDOWN:
			{
			UINT	uBtn;
			UINT	idx;

			uBtn = GetBtnNumFromCoord((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			if(uBtn != TB_NO_BUTTON){
				idx = GetIndexFromBtnNumber(uBtn);
				SetCapture(hwnd);
				uLastBtnID = uBtn;
				SetStatBarText(hwndStatBar, GetGtrString(szOutBuff, MAXOUTBUFF,
									TBButtonInfo[idx].idsStatus),	BLACK_TEXT);
				if(!(TBButtonInfo[idx].uStyle & TBBS_DISABLED)){
					fMouseDown = TRUE;
					TBButtonInfo[idx].uStyle |= TBBS_PRESSED;
					InvalidateButton(hwnd, uBtn);
					}
				}
			}
			return FALSE;


		//---------------------------------------------------------------------
		// WM_LBUTTONUP:	Process this message if the left mouse button
		//						was released
		//---------------------------------------------------------------------
		case WM_LBUTTONUP:
			{
			UINT	uBtn;
			UINT	idx;

			SetStatBarText(hwndStatBar, (LPTSTR)szJUSTNULLTERM, BLACK_TEXT);
			if(fMouseDown){
				uBtn = GetBtnNumFromCoord((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
				if((uBtn == uLastBtnID)){
					fMouseDown = FALSE;
					uLastBtnID = TB_NO_BUTTON;
					idx = GetIndexFromBtnNumber(uBtn);
					TBButtonInfo[idx].uStyle &= ~TBBS_PRESSED;
					InvalidateButton(hwnd, uBtn);
					ReleaseCapture();
					// Perform whatever action is necessary
					if(TBButtonInfo[idx].uMenuID != TB_NO_MENU)
						SendMessage(hwndFrame, WM_COMMAND, (WPARAM)(TBButtonInfo[idx].uMenuID), 0L);
					else
						NonMenuCommandFunc(TBButtonInfo[idx].uBtnID);
					}
				else{
					ReleaseCapture();
					fMouseDown = FALSE;
					uLastBtnID = TB_NO_BUTTON;
					}
				}
			else{
				ReleaseCapture();
				uLastBtnID = TB_NO_BUTTON;
				}
			}
			return FALSE;

		//---------------------------------------------------------------------
		// WM_MOUSEMOVE:	Process this message if the left mouse button
		//						was release
		//---------------------------------------------------------------------
		case WM_MOUSEMOVE:
			{
			UINT	uBtn;
			UINT	idx;

			if(fMouseDown){
				uBtn = GetBtnNumFromCoord((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
				if((uBtn == uLastBtnID)){
					idx = GetIndexFromBtnNumber(uBtn);
					if(!(TBButtonInfo[idx].uStyle & TBBS_PRESSED)){
						TBButtonInfo[idx].uStyle |= TBBS_PRESSED;
						InvalidateButton(hwnd, uBtn);
						}
					}
				else{
					idx = GetIndexFromBtnNumber(uLastBtnID);
					if(TBButtonInfo[idx].uStyle & TBBS_PRESSED){
						TBButtonInfo[idx].uStyle &= ~TBBS_PRESSED;
						InvalidateButton(hwnd, uLastBtnID);
						}
					}
				}
			}
			return FALSE;

		//---------------------------------------------------------------------
		// WM_DESTORY:   Process this message to free memory and destroy font.
		//---------------------------------------------------------------------
		case WM_DESTROY:
			{
			DeleteDC(hDCGlyphs);
			DeleteDC(hDCMono);
			DeleteObject(hBtnBmp);
			DeleteObject(hMonoBmp);
			DeleteObject(hbrDither);
			DeleteObject(hbrBtnHighlight);
			DeleteObject(hbrBtnShadow);
			FREE_PTR(lpTool);
			}
			return 0;

		default:
			break;
		}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}




//*---------------------------------------------------------------------------------
//| SizeToolBar:
//|	This function will adjust the size of our status bar and move it to
//|	the new location.
//| Parms:
//|	hwnd						The window handle of the status bar
//|	dx							The width of the client window
//|	bottom					The bottom of the client window
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN SizeToolBar(HWND hwnd, UINT dx)
{
	lpTool->dx = dx;
	lpTool->rect.left = 0;
	lpTool->rect.top = 0;
	lpTool->rect.right = dx;
	lpTool->rect.bottom = lpTool->dy;
}

//*---------------------------------------------------------------------------------
//| DoFreeStmtClose:
//|	This function does the equivalent of a SQLFreeStmt(SQL_CLOSE)
//| Parms:
//|	fLogging		Flag which determines if you want logging or not
//|						FALSE for Logging, TRUE for NO Logging
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoFreeStmtClose(BOOL fNoLog)
{
	extern lpDFTARRAY		lpdftFreeStmtOpt;
	extern UINT				ciFreeStmtOpt;
	PARAMITEM lpmtSQLFreeStmt[] = {
	// szName				uParmType						fCType         	dwDex		fNull	cBytes	lpData 	cDfts		lpDfts
	//	-----------------	--------------------------	-----------------	--------	-----	--------	--------	--------	--------
		TEXT("&hstmt:"),	PT_HSTMT | PT_FREE_PTR |
								PT_LOG_IN,						0,						0,			0,		0,			NULL,		0,			NULL,	 0,0,0,			NULL, NULL,
		TEXT("&fOption:"),PT_LOG_IN,						SQL_C_USHORT,		0,			0,		0,			NULL,		0,			NULL,	 0,0,0,			NULL, NULL,
		};

	lpPARAMITEM			lplpSQLFreeStmt[(NumItems(lpmtSQLFreeStmt))];
	UWORD					fOption = SQL_CLOSE;
	UINT					idx;

	fBusy(TRUE);

	// Fill out the PARAMITEM structure correctly
	lpmtSQLFreeStmt[0].lpData = lpActiveConnWin->lpCurStmt;
	lpmtSQLFreeStmt[1].cDfts = ciFreeStmtOpt;
	lpmtSQLFreeStmt[1].lpDfts = lpdftFreeStmtOpt;
	lpmtSQLFreeStmt[1].lpData = &fOption;
	for(idx=0; idx<NumItems(lpmtSQLFreeStmt); idx++)
		lplpSQLFreeStmt[idx] = (lpPARAMITEM)&lpmtSQLFreeStmt[idx];

	// Now call the function handler
	lpSQLFreeStmt(TEXT("SQLFreeStmt"), NumItems(lpmtSQLFreeStmt),
						lpActiveConnWin, lplpSQLFreeStmt, fNoLog, NULL);

	fBusy(FALSE);
}

//*---------------------------------------------------------------------------------
//| DoExtendedFetch:
//|		This function calls SQLExtendedFetch with the given fFetchType.
//| Parms:
//|	fLogging		Flag which determines if you want logging or not
//|						FALSE for Logging, TRUE for NO Logging
//|	fFetchType	Fetch type to perform
//|	lpSI			Statement info linked list
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoExtendedFetch(BOOL fNoLog, UWORD fFetchType,lpSTATEMENTINFO lpSI)
{
	extern lpDFTARRAY		lpdftFetchType;
	extern UINT				ciFetchType;
	PARAMITEM lpmtSQLExtendedFetch[] = {
	// szName				uParmType						fCType         	dwDex		fNull	cBytes	lpData 	cDfts		lpDfts
	//	-----------------	--------------------------	-----------------	--------	-----	--------	--------	--------	--------
		TEXT("&hstmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,			0,		0,			NULL,		0,			NULL, 0,0,0,			NULL, NULL,
		TEXT("f&FetchType:"),	PT_LOG_IN,						SQL_C_USHORT,		0,			0,		0,			NULL,		0,			NULL,	0,0,0,			NULL, NULL,
		TEXT("&irow:"),			PT_LOG_IN,						SQL_C_SLONG,		0,			0,		0,			NULL,		0,			NULL,	0,0,0,			NULL, NULL,
		TEXT("&pcrow:"),			PT_PTR_UDWORD | PT_LOG_OUT,SQL_C_ULONG,		0,			0,		0,			NULL,		0,			NULL,	0,0,0,			NULL, NULL,
		TEXT("rgfRowStatus:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,			0,		0,			NULL,		0,			NULL,	0,0,0,			NULL, NULL,
		};

	lpPARAMITEM			lplpSQLExtendedFetch[(NumItems(lpmtSQLExtendedFetch))];
	SDWORD				irow = 1;
	UDWORD				pcrow;
	UINT					idx;
	UDWORD				uRowSetSize=lpSI->uRowSetSize;
	UINT					cItems=NumItems(lpmtSQLExtendedFetch);

	// Set the untouched pattern to the output buffers
	InitUnModified(&pcrow, (UDWORD)sizeof(pcrow));

	// Fill out the PARAMITEM structure correctly
	lpmtSQLExtendedFetch[0].lpData = lpSI;
	lpmtSQLExtendedFetch[1].cDfts = ciFetchType;
	lpmtSQLExtendedFetch[1].lpDfts = lpdftFetchType;
	lpmtSQLExtendedFetch[1].lpData = &fFetchType;
	lpmtSQLExtendedFetch[2].lpData = &irow;
	lpmtSQLExtendedFetch[2].cBytes = (UDWORD)sizeof(irow);
	lpmtSQLExtendedFetch[3].lpData = &pcrow;
	lpmtSQLExtendedFetch[3].cBytes = (UDWORD)sizeof(pcrow);
	lpmtSQLExtendedFetch[4].lpData = lpSI->rgfRowStatus;
	lpmtSQLExtendedFetch[4].cBytes = (UDWORD)uRowSetSize;

	for(idx=0; idx < cItems; idx++)
		lplpSQLExtendedFetch[idx] = (lpPARAMITEM)&lpmtSQLExtendedFetch[idx];

	// Now call ExtendedFetch
	lpSQLExtendedFetch(TEXT("SQLExtendedFetch"), cItems,
								lpActiveConnWin, lplpSQLExtendedFetch, fNoLog, NULL);

} //DoExtendedFetch()


//*---------------------------------------------------------------------------------
//| DoFetchScroll:
//|		This function calls SQLExtendedFetch with the given fFetchType.
//| Parms:
//|	fLogging		Flag which determines if you want logging or not
//|						FALSE for Logging, TRUE for NO Logging
//|	fFetchType	Fetch type to perform
//|	lpSI			Statement info linked list
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoFetchScroll(BOOL fNoLog, UWORD fFetchType,lpSTATEMENTINFO lpSI)
{
	extern lpDFTARRAY		lpdftFetchType;
	extern UINT				ciFetchType;
	PARAMITEM lpmtSQLFetchScroll[] = {
	// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
	//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
		TEXT("hst&mt:"),					PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,					NULL,		0,	0,0,			NULL, NULL,
		TEXT("f&FetchOrientation:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,					NULL,		0,	0,0,			NULL, NULL,
		TEXT("fF&etchOffset:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,					NULL,		0,	0,0,			NULL, NULL,
		};

	UINT					uNumItems=NumItems(lpmtSQLFetchScroll);
	lpPARAMITEM			lplpSQLFetchScroll[(NumItems(lpmtSQLFetchScroll))];
	SDWORD				irow = 1;
	UDWORD				pcrow;
	UINT					idx;

	// Set the untouched pattern to the output buffers
	InitUnModified(&pcrow, (UDWORD)sizeof(pcrow));

	// Fill out the PARAMITEM structure correctly
	lpmtSQLFetchScroll[0].lpData = lpSI;
	lpmtSQLFetchScroll[1].cDfts  = ciFetchType;
	lpmtSQLFetchScroll[1].lpDfts = lpdftFetchType;
	lpmtSQLFetchScroll[1].lpData = &fFetchType;
	lpmtSQLFetchScroll[2].lpData = &irow;
	lpmtSQLFetchScroll[2].cBytes = (UDWORD)sizeof(irow);

	for(idx=0; idx < uNumItems; idx++)
		lplpSQLFetchScroll[idx] = (lpPARAMITEM)&lpmtSQLFetchScroll[idx];

	// Now call ExtendedFetch
	lpSQLFetchScroll(TEXT("SQLFetchScroll"), uNumItems,
								lpActiveConnWin, lplpSQLFetchScroll, fNoLog, NULL);

} //DoFetchScroll()


//*---------------------------------------------------------------------------------
//| DoFetch:
//|		This function calls SQLFetch with the given fFetchType.
//| Parms:
//|	fLogging		Flag which determines if you want logging or not
//|						FALSE for Logging, TRUE for NO Logging
//|	fFetchType	Fetch type to perform
//|	lpSI			Statement info linked list
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoFetch(BOOL fNoLog, UWORD fFetchType,lpSTATEMENTINFO lpSI)
{
	PARAMITEM lpmtSQLFetch[] = {
	// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
	//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
		TEXT("hst&mt:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,							NULL, 0,	0,0,		NULL, NULL,
		};

	lpPARAMITEM			lplpSQLFetch[(NumItems(lpmtSQLFetch))];

	// Fill out the PARAMITEM structure correctly
	lpmtSQLFetch[0].lpData = &lpSI->hstmt;

	lplpSQLFetch[0] = (lpPARAMITEM)&lpmtSQLFetch[0];

	// Now call SQLFetch
	lpSQLFetch(TEXT("SQLFetch"), NumItems(lpmtSQLFetch),
					lpActiveConnWin, lplpSQLFetch, fNoLog, NULL);

} //DoFetch()


//*---------------------------------------------------------------------------------
//| DoFetchMethod:
//|		This function calls SQLExtendedFetch, SQLFetchScroll, or SQLFetch
//|	with the given fFetchType.
//| Parms:
//|	fLogging		Flag which determines if you want logging or not
//|						FALSE for Logging, TRUE for NO Logging
//|	fFetchType	Fetch type to perform
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoFetchMethod(BOOL fNoLog, UWORD fFetchType)
{
	fBusy(TRUE);

	if (!lpActiveConnWin)
		return;

	switch(lpActiveConnWin->lpCurStmt->wFetchAPI)
	{
		case SQL_API_SQLFETCH:
			DoFetch(fNoLog,fFetchType,lpActiveConnWin->lpCurStmt);
			break;

		case SQL_API_SQLEXTENDEDFETCH:
			DoExtendedFetch(fNoLog,fFetchType,lpActiveConnWin->lpCurStmt);
			break;

		case SQL_API_SQLFETCHSCROLL:
			DoFetchScroll(fNoLog,fFetchType,lpActiveConnWin->lpCurStmt);
			break;
		default:
			;
	}

	fBusy(FALSE);

} //DoFetchMethod()


//*---------------------------------------------------------------------------------
//| NonMenuCommandFunc:
//|	This function handles the functionality of all of the toolbar buttons
//|	that are not associated with a menu item
//| Parms:
//|	uOptID					The button ID of the button that we need to handle
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN NonMenuCommandFunc(UINT uBtn)
{

	if(lpActiveResultWin)
	{
		ShowWindow(lpActiveResultWin->hwndrgb,SW_HIDE);
		ShowWindow(lpActiveResultWin->hwndpcb,SW_HIDE);
		ShowWindow(lpActiveResultWin->hwndARDCell,SW_HIDE);
	}

	switch(uBtn)
	{
		case TB_EXEC_DIRECT:
		{
			PARAMITEM lpmtSQLExecDirect[] = {
			// szName				uParmType						fCType         	dwDex		fNull	cBytes	lpData 	cDfts		lpDfts
			//	-----------------	--------------------------	-----------------	------ 	-----	--------	--------	--------	--------
				TEXT("&hstmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,			0,		0,			NULL,		0,			NULL,		0,	0,	0,			NULL, NULL,
				TEXT("&szSqlStr:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_CHAR,			0,			0,		0,			NULL,		0,			NULL,		0,	0,	0,			NULL, NULL,
				TEXT("&cbSqlStr:"),	   PT_LOG_IN,						SQL_C_SLONG,		1,			0,		0,			NULL,		0,			NULL,		0,	0,	0,			NULL, NULL,
				};

			lpPARAMITEM		lplpSQLExecDirect[(NumItems(lpmtSQLExecDirect))];
			UDWORD			cbLen;
			SDWORD			cbSqlStr = SQL_NTS;
			LPTSTR				lpSqlStr = NULL;
			UINT				idx;

			fBusy(TRUE);

			// Get the string from the input window
			lpSqlStr = GetInputWindowString(lpActiveConnWin->hwndIn, &cbLen);

			// Fill out the PARAMITEM structure correctly
			lpmtSQLExecDirect[0].lpData = lpActiveConnWin->lpCurStmt;
			lpmtSQLExecDirect[1].lpData = lpSqlStr;
			lpmtSQLExecDirect[2].lpData = &cbSqlStr;
			for(idx=0; idx<NumItems(lpmtSQLExecDirect); idx++)
				lplpSQLExecDirect[idx] = (lpPARAMITEM)&lpmtSQLExecDirect[idx];

			// Now call the function handler
			lpSQLExecDirect(TEXT("SQLExecDirect"), NumItems(lpmtSQLExecDirect),
								lpActiveConnWin, lplpSQLExecDirect, 0, NULL);

			FREE_PTR(lpSqlStr);

			fBusy(FALSE);
			}
			break;

		case TB_BIND_COL_EXT_FETCH:
			{

			//Set Fetch Method
			if (lpUsrOptions->fODBC30 && IsSupported(lpActiveConnWin,SQL_API_SQLFETCHSCROLL,TRUE))
				lpActiveConnWin->lpCurStmt->wFetchAPI=SQL_API_SQLFETCHSCROLL;
			else
				lpActiveConnWin->lpCurStmt->wFetchAPI=SQL_API_SQLEXTENDEDFETCH;

			if(ResultsBindColAll(lpActiveConnWin))
				DoFetchMethod(FALSE, SQL_FETCH_NEXT);

			}
			break;

		case TB_FREE_STMT_CLOSE:
			{
			DoFreeStmtClose(FALSE);
			}
			break;

		case TB_FETCH_FIRST:
			{
			DoFetchMethod(FALSE, SQL_FETCH_FIRST);
			}
			break;

		case TB_FETCH_PRIOR:
			{
			DoFetchMethod(FALSE, SQL_FETCH_PRIOR);
			}
			break;

		case TB_FETCH_NEXT:
			{
			DoFetchMethod(FALSE, SQL_FETCH_NEXT);
			}
			break;


		case TB_FETCH_LAST:
			{
			DoFetchMethod(FALSE, SQL_FETCH_LAST);
			}
			break;

		default:
			break;
		}
}

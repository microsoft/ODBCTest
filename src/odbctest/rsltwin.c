//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	RSLTWIN.C
//|
//| Purpose:
//|		This module contains the window procedure for the Results Windows.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "blddata.h"
#include <malloc.h>
#include <errno.h>
#include <limits.h>

#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define IDW_STATBAR		750
#define IDW_VERTBAR		751
#define IDW_HORZBAR		752

#define GETRWPOINTER(x) GetWindowLongPtr(x, 0)
#define SETRWPOINTER(x, y) SetWindowLongPtr(x, 0, (LONG_PTR)(y))

#define MAXDSPCHARWIDTH	500
#define MINDSPCHARWIDTH  10

#define MAXROWNUM			  9
#define DFTEXTRA			  2
#define PIXELFUDGE		1.2
#define ONEROW				  1
#define DFTHORZINC(rw)	(rw->rowcx - rw->statcx)
#define HEIGHTFUDGE		4
#define NUM_CB_ROWS		10

#define ROWNUMWIDTH(rw) (rw->rowcx - rw->statcx)
#define STATUS_TITLE 	((UWORD)-1)
#define NO_RGF_ARRAY		((UWORD)-2)


#define RGB_CHILD_ID		40000
#define PCB_CHILD_ID		41000
#define ARDSTATUS_CHILD_ID	42000


typedef struct tagPAINTINFO {
	HDC				hdc;
	HBRUSH			hGrayBrush;
	HFONT				hNormFont;
	HPEN				hDottedPen;
	} PAINTINFO, * lpPAINTINFO;


HBRUSH	g_hNoRowBrush;

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE					hInst;
extern HFONT						hEditFont;
extern HFONT						hColumnFont;		// Use for columns
extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HWND							hwndStatBar;
extern HWND							hwndOutWin;
extern lpUSROPTIONS				lpUsrOptions;		// Global user options
extern UINT							giStmtHeight;		// Based on font of combo-box, found in connwin.c
extern HWND							hwndCurMDIChild;  // Current active MDI child at all times
extern lpSTATUSWINDOWINFO		lpStat;				// Pointer to the main status bar, use for dimensions and font
extern lpHEADENV					lpHeadEnv;


HICON									hResultIcon=NULL;
dCSEG(TCHAR) szRSLTWINCLASS[]		= TEXT("GATOR:RSLTWIN");
dCSEG(TCHAR)	szErrRsltTitle[] 	= TEXT("Error");
dCSEG(TCHAR) szRSLTNULLVAL[]		= TEXT("<null>");
dCSEG(TCHAR) szEMPTYCOL[]			= TEXT("<empty string>");
dCSEG(TCHAR) szCOLNUM[]				= TEXT("col%u"); //Bug #401
dCSEG(TCHAR) szSTATUS[]				= TEXT("Status");
dCSEG(TCHAR) szRSLTUNKNOWN[]		= TEXT("Unknown!");
dCSEG(TCHAR) szCOLBIND[]			= TEXT("COLUMN");
dCSEG(TCHAR) szROWBIND[]			= TEXT("ROW");
dCSEG(TCHAR) szWARNING[]			= TEXT("Warning");
dCSEG(TCHAR) szBOOKMARK[]			= TEXT("Bookmark");
dCSEG(TCHAR) szROWSET[]				= TEXT("Rowset");

//
// Following table describes the valid rgfRowStatus flags
//
LPTSTR g_szRowStatus[] = {
	TEXT("SUCCESS"),
	TEXT("DELETED"),
	TEXT("UPDATED"),
	TEXT("NOROW"),
	TEXT("ADDED"),
	TEXT("ERROR"),
	TEXT("SUCCESS_WITH_INFO"),
	};
UWORD			uStatTitle = STATUS_TITLE;

typedef struct tagCBVALUE
{
	LPTSTR	szString;
	DWORD		dwValue;
} CBVALUE;

CBVALUE	g_rgPcbValues[]={
	{TEXT("SQL_NTS"),(DWORD)SQL_NTS},
	{TEXT("SQL_NULL_DATA"),(DWORD)SQL_NULL_DATA},
	{TEXT("SQL_DATA_AT_EXEC"),(DWORD)SQL_DATA_AT_EXEC},
	{TEXT("SQL_COLUMN_IGNORE"),(DWORD)SQL_COLUMN_IGNORE},
};

CBVALUE g_rgArrayStatus[] = {
	TEXT("SQL_ROW_PROCEED"),(DWORD)SQL_ROW_PROCEED,
	TEXT("SQL_ROW_IGNORE"),(DWORD)SQL_ROW_IGNORE,
	};

LPTSTR szCursorType[] = {
	TEXT("FORWARD"),
	TEXT("KEYSET"),
	TEXT("DYNAMIC"),
	TEXT("STATIC"),
	};

LPTSTR szConcurType[] = {
	TEXT("READ ONLY"),
	TEXT("LOCK"),
	TEXT("ROWVER"),
	TEXT("VALUES"),
	};



//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
LRESULT EXTFUN ResultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID INTFUN GetFontDim(HWND hwnd, HFONT hFont, int * lpicx, int * lpily,
						int * lpiTitle);
VOID INTFUN SetColWidths(HWND hwnd, lpRESULTWININFO lprw);

// Drawing/WndProc helpers
VOID INTFUN DoSize(lpRESULTWININFO rw);
VOID INTFUN DoDblClickRowset(lpRESULTWININFO lprw, int fKeyCode, int mx, int my);
VOID INTFUN DoMouseMove(lpRESULTWININFO lprw, int fKeyCode, int x, int y);
VOID INTFUN DoVertScroll(lpRESULTWININFO lprw, int iScrollCode, int iPos);
VOID INTFUN DoHorzScroll(lpRESULTWININFO lprw, int iScrollCode, int nPos);
VOID INTFUN DoKeyDownScroll(lpRESULTWININFO lprw,
					WPARAM wKeyId, LPARAM dwKeyData);

// Utility functions
VOID INTFUN GetHorzVertScrollArea(lpRESULTWININFO lprw, lpRECT lphzRect);
VOID INTFUN FindFirstLastCol(lpRESULTWININFO lprw,
					lpRECT lpuRect,
					int * piFirst, int * piLast);
VOID INTFUN FindFirstLastRow(lpRESULTWININFO lprw,
					lpRECT lpuRect,
					int * piFirst, int * piLast);

// Paint routines
VOID INTFUN DrawColumns(HDC hdc, lpRECT uRect, lpRESULTWININFO lprw,
								UWORD *rgArrayStatusPtr);
VOID INTFUN PaintColumn(lpPAINTINFO lppi, lpRECT lpRect, LPCTSTR szOutStr,
								int statcx);
VOID INTFUN DrawData(HDC hdc, lpRECT uRect, lpRESULTWININFO lprw,UWORD *rgArrayStatusPtr);
VOID INTFUN PaintData(lpPAINTINFO lppi, lpRECT lpRect, LPCTSTR szOutStr,
					int iPcbWidth, LPCTSTR szPcbStr);


lpRESULTWININFO	lpActiveResultWin;// The activated window

//*---------------------------------------------------------------------------------
//| FindColOffset:
//|	This function returns the left coord. for the Data columns in the rowset.
//|
//|	Scroll pos 0 is x=0, therefore -(scroll pos) * increment is the 0 based
//|		offset to use, add on row column which also occupies space
//| Parms:
//|	lprw		Pointer to rowset window information record
//| Returns:
//|	left coord.
//*---------------------------------------------------------------------------------

__inline int INTFUN FindColOffset(lpRESULTWININFO lprw)
{
	int iPos;

 	if (lprw->fHorzPresent)
		iPos=(-1 * GetScrollPos(lprw->hwndHorzScroll, SB_CTL) * DFTHORZINC(lprw)) + lprw->rowcx;
	else
		iPos=lprw->rowcx;

	return(iPos);

} //FindColOffset()


//*---------------------------------------------------------------------------------
//| CreateResultWindow:
//|	This function will create a results window in the client area which will be
//|	the child of a connection window (via data structures only).
//| Parms:
//|	hwndClient				Client window handle
//|	hInst						For use in CreateWindow call
//|	henv						Environment handle for application
//|	lpci						The parent CONNECTIONINFO
//|	lpStmt					The parent STATEMENTINFO
//| Returns:
//|	Window handle of the status bar if successful, NULL on error
//*---------------------------------------------------------------------------------
RESULTWININFO * CreateResultWindow(HWND hwndClient, HINSTANCE hInst,
						HENV henv, lpCONNECTIONINFO lpci, lpSTATEMENTINFO lpStmt,
						RETCODE rc)
{
	MDICREATESTRUCT		mdicreate;
	lpRESULTWININFO		lprw;
	BOOL						fZoom;
	int						length=0;

	// Allocate memory for a new node, then create the window
	lprw = (lpRESULTWININFO)AllocateMemory(sizeof(RESULTWININFO));
	if(!lprw)
		return NULL;
	lprw->hInst = hInst;
	lprw->hwndClient = hwndClient;
	lprw->lpci = lpci;
	lprw->lpStmt = lpStmt;
	lprw->fNoDataFound = rc==SQL_NO_DATA_FOUND;
	length=wsprintf((LPTSTR)lprw->szClientTitle,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsResultWinTitle),
				lpStmt->uStmtNum,
				(LPTSTR)lpci->szClientTitle);

	// Determine if child windows are currently maximized and match this current window
	fZoom = IsZoomed(hwndCurMDIChild);

	mdicreate.szClass = szRSLTWINCLASS;
	mdicreate.szTitle = (LPTSTR)lprw->szClientTitle;
	mdicreate.hOwner  = hInst;
	mdicreate.x			= CW_USEDEFAULT;
	mdicreate.y			= CW_USEDEFAULT;
	mdicreate.cx		= CW_USEDEFAULT;
	mdicreate.cy		= CW_USEDEFAULT;
	mdicreate.style	= (fZoom) ? WS_MAXIMIZE : 0;
	mdicreate.lParam	= (LPARAM)lprw;

	// If successful, add the new connection to the list, else free memory and fail
	if(SendMessage(hwndClient, WM_MDICREATE, 0,
			(LPARAM)(LPMDICREATESTRUCT)&mdicreate))
		lpStmt-> lpResults = lprw;
	else {
		szMessageBox(hwndClient,
					MB_ICONEXCLAMATION | MB_OK,
					(LPTSTR)szErrRsltTitle,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsRsltWinCreateFailure));
		FREE_PTR(lprw);
		return NULL;
		}

	return lprw;
}



//*---------------------------------------------------------------------------------
//| DestroyResultWindow:
//|	Destroy a results window and clean up data structures.
//| Parms:
//|	lprw							The node to delete
//|	fFreeRowStatus				SQL_UNBIND shouldn't free the
//|									rgfRowStatus.
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DestroyResultWindow(lpRESULTWININFO lprw, BOOL fFreeRowStatus)
{
	// Remove reference from statement structure
	lprw->lpStmt->lpResults = NULL;

	if (fFreeRowStatus)
	{
		FREE_PTR(lprw->lpStmt->rgfRowStatus);
		//set IRD to NULL because the application might try to free this later.
		if(lprw->lpStmt->lpIRD)
			lprw->lpStmt->lpIRD->rgArrayStatusPtr = NULL;
	}

	FREE_PTR(lprw->lpStmt->rgfRowCache);
	FREE_PTR(lprw->colx);
	FREE_PTR(lprw->colpcbx);
	FREE_PTR(lprw->uPcbVals);

	if(g_hNoRowBrush)
		DeleteObject(g_hNoRowBrush);

	// Destroy window
	SendMessage(lprw->hwndClient, WM_MDIDESTROY, (WPARAM)lprw->hwnd, 0L);
	FREE_PTR(lprw);
}


//*---------------------------------------------------------------------------------
//| FormatRowsetCell:
//|	Given a pointer and pcb information, format an output string for the rowset.
//| Parms:
//|	szDspBuff				Output buffer
//|	rgbValue					Pointer to data
//|	pcbValue					Pointer to cb vals
//|	fCType					The type of the cell
//|	cbElement				Size of one cell (max)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN FormatRowsetCell(LPTSTR szDspBuff,
					PTR rgbValue, UNALIGNED SDWORD * pcbValue,
					SWORD fCType, UDWORD cbElement,
					UDWORD cbDspBuff,HWND hwndOut)
{
// To fix some Unalignment problems on MIPS
	SDWORD FixUnalign=0;

	//Bug307
	if (pcbValue)
		// Use the Variable and not dereference in the if()
		FixUnalign=*pcbValue;

	if(pcbValue &&	FixUnalign == SQL_NULL_DATA)
		lstrcpy(szDspBuff, szRSLTNULLVAL);
	else if(fCType == SQL_C_WCHAR)
		lstrcpyn(szDspBuff, (LPTSTR)rgbValue,cbDspBuff/sizeof(TCHAR));
	else if(fCType == SQL_C_CHAR)
	{
		TCHAR * szStrW = (LPTSTR)AllocateMemory((XLARGEBUFF * 2) * sizeof(TCHAR));

		ConvertToUnicode((LPSTR)rgbValue, szStrW,XLARGEBUFF * 2);

		lstrcpyn(szDspBuff, szStrW,cbDspBuff/sizeof(TCHAR));

		FREE_PTR(szStrW);

	}
	else
	{
   	// Format the data.  For binary, we only want to display the pcbValue
   	UDWORD		udSize=cbElement;

		if(pcbValue &&
			FixUnalign == SQL_NO_TOTAL)						// Don't try to show everything
			udSize = MAXDSPCHARWIDTH;

   	if (IsVarLenData(fCType) && pcbValue && FixUnalign > 0)
		{
			udSize = (UDWORD)min((UDWORD)(FixUnalign), cbElement);
		}

		FormatValueBasedOnType(szDspBuff, MAXDSPCHARWIDTH,
					rgbValue, fCType, udSize,hwndOut);
	}
}


//*---------------------------------------------------------------------------------
//| GetColumnNames:
//|	Look through each bound node and attempt to get the column name.
//| Parms:
//|	lprw						Pointer to the results set
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN GetColumnNames(lpRESULTWININFO lprw)
{
	RETCODE					rc;
	lpSTATEMENTINFO		lpStmt;
	lpBOUNDVARS				lpbv;
	UINT						uCols;
	UINT						uDex=GetSqlFuncOffset(SQL_API_SQLDESCRIBECOL);

	if(!lpUsrOptions->fShowColumns)				// If this is not turned on, then don't bother
		return;

	// Init pointers and counters
	lpStmt = lprw->lpStmt;
	uCols = lpStmt->uBoundNodes;
	lpbv = lpStmt->lpVars;

	// Loop through looking for bound nodes.  When found, fill out the column
	//	name if present, else use old value.
	while(uCols--) {
		if(lpbv->fBound) {
			if(!*lpbv->szCol)
				//
				// For non-bookmark columns, use SQLDescribeCol to
				//	get the column name.  For bookmarks, use hard
				//	coded name since calling DescribeCol on col 0 is
				// not valid.
				//
				if(lpbv->icol != 0) {
					rc = SQLDescribeCol(lpStmt->hstmt,
										lpbv->icol,
										(LPTSTR)lpbv->szCol,
										sizeof(lpbv->szCol),
										NULL,
										NULL, NULL, NULL, NULL);
					if(RC_NOTSUCCESSFUL(rc))
						goto showerror;

					// Some drivers return an empty string for column names when used on
					//	an expression.  Fill the void with a indicator for this.
					if(!*lpbv->szCol)
						lstrcpy(lpbv->szCol, szEMPTYCOL);
					}
				else
					lstrcpy(lpbv->szCol, szBOOKMARK);
			}
		lpbv = lpbv->next;
		}

	return;

// Jump here on an error
showerror:
	GetApiName(uDex, szOutBuff);
	LogReturnCode((LPTSTR)szOutBuff, lprw->lpci, rc);
	ToolOptionsAllErrors(lprw->lpci, lprw->lpci->lpActiveEnv->henv,
		lprw->lpci->hdbc, lpStmt->hstmt,FALSE,SQL_SUCCESS);

	// When we fail to retrieve column names, we must go through the rest of
	// of the columns and fill out something for the name.
	++uCols;				// Start on current node
	while(uCols--) {
		if(lpbv->fBound && !*lpbv->szCol)
			wsprintf(lpbv->szCol, szCOLNUM, lpbv->icol);

		lpbv = lpbv->next;
		}
}


//*---------------------------------------------------------------------------------
//| ResetRsltWin:
//|	Given the current window and font, figure out how big each column is.
//| Parms:
//|	lprw						Pointer to the results set
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN ResetRsltWin(lpRESULTWININFO lprw)
{
	// Retrieve the column names if required
	if(lpUsrOptions->fShowColumns)
		GetColumnNames(lprw);

	// Figure out the dimensions based on the font and options
	GetFontDim(lprw->hwnd, hEditFont,
			&lprw->cx, &lprw->ly, &lprw->iTitleHeight);
	lprw->coly = lprw->ly;
	SetColWidths(lprw->hwnd, lprw);

	// Reset scrolling ranges based on any changes
	DoSize(lprw);

	SetMiniBarText(lprw, NULL);
	RefreshWindow(lprw->hwnd);
}

//*---------------------------------------------------------------------------------
//| GetStringFromCell:
//|		This function gets the string from the specified control
//|
//|	Params:
//|		hwnd		Control hwnd
//*---------------------------------------------------------------------------------

__inline LPTSTR INTFUN GetStringFromCell(HWND hwnd, DWORD *pdwLen)
{
	LPTSTR	psz=NULL;

	*pdwLen=(DWORD)GetTrueWindowTextLength(hwnd,-1);

	if (!(psz = AllocateMemory(*pdwLen + sizeof(TCHAR))))
		return(NULL);

	// fetch string from control
	GetWindowText(hwnd, psz, (*pdwLen + sizeof(TCHAR))/sizeof(TCHAR));

	return(psz);

} //GetStringFromCell()


//*---------------------------------------------------------------------------------
//| AddNewPcbValue:
//|		This function updates the screen with a new pcbValue.
//|
//|	Params:
//|		cbValueNew		New cbValue
//|		cbValueOld		Old cbValue
//|		hwndpcb			HWND to pcbValue Cell
//*---------------------------------------------------------------------------------

VOID INTFUN AddNewPcbValue(SDWORD cbValueNew,SDWORD cbValueOld,HWND hwndpcb)
{
	TCHAR		szcbValue[MEDBUFF]=TEXT(""),
				szcbOld[MEDBUFF]=TEXT("");
	DWORD		dex=0;

	//Convert new cbValue to string
	_itot(cbValueNew,szcbValue,10);

	//Convert old cbValue to string for search
	_itot(cbValueOld,szcbOld,10);

	//Find old cbValue
	dex=(DWORD)SendMessage(hwndpcb,CB_FINDSTRING,(WPARAM)0,(LPARAM)szcbOld);

	if (dex != CB_ERR)
	{
		//remove old value from CB
		dex=(DWORD)SendMessage(hwndpcb,CB_DELETESTRING,(WPARAM)dex,0L);
	}

	//Add new cbValue string
	dex=(DWORD)SendMessage(hwndpcb,CB_ADDSTRING,(WPARAM)0,(LPARAM)szcbValue);
	//Add new cbValue data
	SendMessage(hwndpcb,CB_SETITEMDATA,(WPARAM)dex,(LPARAM)cbValueNew);
	//Set new value as current selection
	dex=(DWORD)SendMessage(hwndpcb,CB_SETCURSEL,(WPARAM)dex,0L);

} //AddNewPcbValue()


//*---------------------------------------------------------------------------------
//| UpdateRgbValue:
//|		This function is called when a user modifies the data in a Rowset cell.
//|
//|	Params:
//|		lprw			Pointer to Rowset info
//*---------------------------------------------------------------------------------

VOID INTFUN UpdateRgbValue(lpRESULTWININFO lprw)
{
	lpBOUNDVARS		lpbv=NULL;
	LPTSTR			pszRgb=NULL,
						pszPcb=NULL;
	TCHAR				szDspBuff[MAXDSPCHARWIDTH]=TEXT("");
	UDWORD			cbOff;
	PTR				rgbTemp=NULL,
						rgbValue=NULL;
	SDWORD			*pcbValTmp=NULL,
						*pcbValue=NULL,
						cbValue=0;
	DWORD				cbLen=0,
						cbPcb=0,
						cbRgb=0,
						iCol=0;
	lpSTATEMENTINFO lpStmt=NULL;
	lpCONNECTIONINFO lpCI=NULL;

	if (!lprw || !lprw->lpStmt || !lprw->lpStmt->lpVars)
		goto Cleanup;

	lpStmt=lprw->lpStmt;

	if (lpStmt->lpCI)
		lpCI=lpStmt->lpCI;

	//Get string length of pcbValue
	pszPcb=GetStringFromCell(lprw->hwndpcb,&cbPcb);
	pszRgb=GetStringFromCell(lprw->hwndrgb,&cbRgb);

	//Find the correct bound var node
	iCol=lpStmt->uHighestBoundCol;

	lpbv=lpStmt->lpVars;

	//Find the correct bound var node
	iCol=lpStmt->uHighestBoundCol;
	while (iCol--)
	{
		if (lpbv->icol==lprw->iCurrCol)
			break;
		else
			lpbv=lpbv->next;
	}

	if(lpStmt->fColBinding)
		cbOff = (DWORD)GetSizeOfCType(lpbv->fCType, (UDWORD)lpbv->cbElement);
	else
		cbOff = lpStmt->uRowWiseSize;

	rgbValue = FindAddress(lpbv->rgbValue, 0,lprw->iCurrRow, cbOff);
	pcbValue = FindAddress(lpbv->pcbValue, 0,
									lprw->iCurrRow, (lpStmt->fColBinding) ? sizeof(SDWORD) : cbOff);

	if (UnModified(rgbValue,cbOff))
		*szDspBuff=TEXT('\0');
	else if (pcbValue && *pcbValue != SQL_NULL_DATA)
	{
		FormatRowsetCell(szDspBuff, rgbValue, pcbValue,
					lpbv->fCType, lpbv->cbElement,MAXDSPCHARWIDTH+1,GETOUTPUTWINDOW(lpCI));
	}

	//Did user make a change to the data
	if (*pcbValue == SQL_NULL_DATA || !lstrcmp(szDspBuff,pszRgb))
		goto Cleanup;

	//Validate entry
	if (!ValidateEditWindowByType(lprw->hwndrgb,-1,(LPTSTR)szROWSET,lpbv->fCType))
	{
		//Validation failed set text back to what it was before edit
		SetWindowText(lprw->hwndrgb,pszRgb);
		SetFocus(lprw->hwndrgb);
	}

	//see if data in cell has changed
	if (lstrcmp(szDspBuff,pszRgb))
	{

		// Check for a hex string.  Parse & build from the hex string to given data type
		if (!_tcsnicmp(pszRgb,TEXT("0X"),lstrlen(TEXT("0X"))))
		{
			//toupper the string
			_tcsupr(pszRgb);

			// Remove all spaces from the hex string
			if(!EatWhiteSpace(pszRgb))
  				goto Cleanup;

			//Do the conversion
			rgbTemp=ConvertToTypeFromHex(pszRgb,lpbv->fCType,lstrlen(pszRgb)*sizeof(TCHAR),
					&cbValue,0,0,0);

		}
		else
		{
			rgbTemp=ConvertToTypeFromChar(pszRgb,lpbv->fCType,lstrlen(pszRgb)*sizeof(TCHAR),
					&cbValue,0,0);
		}

		if(cbValue > lpbv->cbElement)
		{
			szMessageBox(lprw->hwndrgb, MB_ICONINFORMATION | MB_OK,
						(LPTSTR)szROWSET,
						 GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsrgbValueTruncated),
						 lpbv->cbElement);
		}

		//Clear out buffer
		memset(rgbValue,0,cbOff);

		//Copy new data to rgbValue
		memcpy(rgbValue, rgbTemp,(size_t)min(cbValue, lpbv->cbElement));

		//Update the screen with new pcbValue
		AddNewPcbValue(cbValue,*pcbValue,lprw->hwndpcb);

		//now update the pcbValue accordingly
		if (pcbValue)
			*pcbValue=cbValue;
	}

Cleanup:

	//hide windows
	ShowWindow(lprw->hwndrgb,SW_HIDE);
	InvalidateRect(lprw->hwnd,&lprw->rRgbCell,TRUE);

	FREE_PTR(pszRgb);
	FREE_PTR(pszPcb);

} //UpdateRgbValue()


//*---------------------------------------------------------------------------------
//| UpdatePcbValue:
//|		This function is called when a user modifies the data in a Rowset cell.
//|
//|	Params:
//|		lprw			Pointer to Rowset info
//*---------------------------------------------------------------------------------

VOID INTFUN UpdatePcbValue(lpRESULTWININFO lprw)
{
	lpBOUNDVARS		lpbv=NULL;
	DWORD				cbLen=0;
	LPTSTR			pszPcb=NULL;
	PTR				rgbValue=NULL;
	SDWORD			*pcbValue=NULL,
						cbTemp=0,
						cbValue=0;
	DWORD				dex,
						cbPcb=0;
	int				cCols;
	UDWORD			cbOff;
	lpSTATEMENTINFO	lpStmt=NULL;

	if (!lprw || !lprw->lpStmt || !lprw->lpStmt->lpVars)
		goto CleanupPcb;

	lpStmt=lprw->lpStmt;

	//Get string length of rgbValue
	pszPcb=GetStringFromCell(lprw->hwndpcb,&cbPcb);

	dex=(DWORD)SendMessage(lprw->hwndpcb,CB_GETCURSEL,0,0L);

	if (dex != CB_ERR)
		cbTemp=(DWORD)SendMessage(lprw->hwndpcb,CB_GETITEMDATA,(WPARAM)dex,0L);
	else
	{
		cbTemp=_ttoi(pszPcb);
	}

	//check if pcbValue changed
	if (!*pszPcb)
		goto CleanupPcb;

	//Find the correct bound var node
	cCols=lpStmt->uHighestBoundCol;

	lpbv=lpStmt->lpVars;

	while (cCols--)
	{
		if (lpbv->icol==lprw->iCurrCol)
			break;
		else
			lpbv=lpbv->next;
	}

	if (lpbv->pcbValue && *lpbv->pcbValue == cbTemp)
		goto CleanupPcb;

	if(lpStmt->fColBinding)
		cbOff = (DWORD)GetSizeOfCType(lpbv->fCType, (UDWORD)lpbv->cbElement);
	else
		cbOff = lpStmt->uRowWiseSize;

	pcbValue = FindAddress(lpbv->pcbValue, 0,
									lprw->iCurrRow, (lpStmt->fColBinding) ? sizeof(SDWORD) : cbOff);

	rgbValue = FindAddress(lpbv->rgbValue, 0,lprw->iCurrRow, cbOff);

	switch(cbTemp)
	{
		case SQL_NTS:
			*pcbValue = SQL_NTS;
			break;

		case SQL_COLUMN_IGNORE:
			*pcbValue = SQL_COLUMN_IGNORE;
			break;

		case SQL_NULL_DATA:
			*pcbValue = SQL_NULL_DATA;
			break;

		case SQL_DATA_AT_EXEC:
			*pcbValue = SQL_DATA_AT_EXEC;
  			break;

		default:
			*pcbValue=cbTemp;

			// If user attempts to use a pcbValue which is larger than we can
			//	hold, force it back.
			if ((SDWORD)*pcbValue > lpbv->cbElement)
			{
				szMessageBox(hwndCurMDIChild, MB_ICONINFORMATION | MB_OK,
							(LPTSTR)szROWSET,
							 GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idscbValueTruncated),
							 *pcbValue, lpbv->cbElement);
				*pcbValue = lpbv->cbElement;
			}

	}

CleanupPcb:

	ShowWindow(lprw->hwndpcb,SW_HIDE);
	InvalidateRect(lprw->hwnd,&lprw->rPcbCell,TRUE);

	FREE_PTR(pszPcb);

} //UpdatePcbValue()


//*---------------------------------------------------------------------------------
//| SetStatusValue:
//|		This function sets the value of the ArrayStatusPtr
//|
//|	Params:
//|		hwnd					hwnd to arraystatusptr combo-box
//|		dwVal					value of rgArraystatusptr for current row
//*---------------------------------------------------------------------------------

VOID INTFUN SetStatusValue(HWND hwnd,UWORD dwVal)
{
	DWORD		dex=0;
	BOOL		fFound=FALSE;
	TCHAR		szFindString[XLARGEBUFF]=TEXT("");
	int		i;

	//Verify that the current value of *rgArrStatPtr is one of the
	//defaults
	for (i=0;i < NumItems(g_rgArrayStatus);i++)
	{
		if (g_rgArrayStatus[i].dwValue == dwVal)
		{
			fFound=TRUE;
			break;
		}
	}

	//If current value not found then this is a user entered
	//value and we need to convert it to a string and add it
	//to the combo-box
	if (fFound)
		 lstrcpy(szFindString,g_rgArrayStatus[i].szString);
	else
		_ltot(dwVal,szFindString,10);

	dex=(DWORD)SendMessage(hwnd,CB_FINDSTRING,(WPARAM)0,(LPARAM)szFindString);

	if (dex != CB_ERR && !fFound)
	{
		//remove old value from CB
		dex=(DWORD)SendMessage(hwnd,CB_DELETESTRING,(WPARAM)dex,0L);

		//Add new cbValue string
		dex=(DWORD)SendMessage(hwnd,CB_ADDSTRING,(WPARAM)0,(LPARAM)szFindString);
		//Add new cbValue data
		SendMessage(hwnd,CB_SETITEMDATA,(WPARAM)dex,(LPARAM)dwVal);
	}

	//Set new value as current selection
	dex=(DWORD)SendMessage(hwnd,CB_SETCURSEL,(WPARAM)dex,0L);

} //SetStatusValue()


//*---------------------------------------------------------------------------------
//| UpdateARDCell:
//|		This function is called when a user modifies the data in a ARD Array Status
//|	cell
//|
//|	Params:
//|		lprw			Pointer to Rowset info
//*---------------------------------------------------------------------------------

VOID INTFUN UpdateARDCell(lpRESULTWININFO lprw)
{
	LPTSTR					psz=NULL;
	TCHAR						szDspBuff[MAXDSPCHARWIDTH]=TEXT("");
	lpSTATEMENTINFO		lpStmt=NULL;
	lpDESCRIPTORHEADER	lpARD=NULL;
	DWORD						dex,
								cbSz=0;
	UWORD						*pudOldVal=NULL;
	SDWORD					sdNewVal=0;

	if (!lprw || !lprw->lpStmt || !lprw->lpStmt->lpARD)
		return;

	lpARD=lprw->lpStmt->lpARD;

	//Get string length of rgbValue
	psz=GetStringFromCell(lprw->hwndARDCell,&cbSz);

	dex=(DWORD)SendMessage(lprw->hwndARDCell,CB_GETCURSEL,0,0L);

	sdNewVal=(DWORD)SendMessage(lprw->hwndARDCell,CB_GETITEMDATA,(WPARAM)dex,0L);

	pudOldVal = FindAddress(lpARD->rgArrayStatusPtr, 0,
									lprw->iCurrRow, sizeof(UWORD));

	switch(sdNewVal)
	{
		case SQL_ROW_IGNORE:
			*pudOldVal=SQL_ROW_IGNORE;
			break;
		case SQL_ROW_PROCEED:
			*pudOldVal=SQL_ROW_PROCEED;
			break;
	}

	//Set new value as current selection
	dex=(DWORD)SendMessage(lprw->hwndARDCell,CB_SETCURSEL,(WPARAM)dex,0L);

	ShowWindow(lprw->hwndARDCell,SW_HIDE);
	InvalidateRect(lprw->hwnd,&lprw->rARDCell,TRUE);

} //UpdateARDCell()


//*---------------------------------------------------------------------------------
//| ResultWndProc:
//|	Handles any messages that must be processed by the window.
//|
//|	Define DEBUG_MSG_PUMP if you want to see every message that comes through.
//|	Unlike tools like Spy, this is a great way to track down messages that occur
//|	before a window is created (before Spy can see them).
//*---------------------------------------------------------------------------------
LRESULT EXTFUN ResultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

//#define DEBUG_MSG_PUMP
#if defined(_DEBUG) && defined(DEBUG_MSG_PUMP)
{
TCHAR sztmp[200];
lpRESULTWININFO rw=(lpRESULTWININFO)GETRWPOINTER(hwnd);
wsprintf(sztmp, TEXT("rw: 0x%08X\thwnd: %04X\tmsg: %04X\twParam: %04X\tlParam: %08X\r\n"),
		rw, hwnd, msg, wParam, lParam);
OutputDebugString(sztmp);
}
#endif

	switch(msg) {
		//--------------------------------------------------------------------
		// WM_CREATE:		Process to retrieve the RESULTWININFO structure
		//						and create the child windows required.
		//--------------------------------------------------------------------
		case WM_CREATE:
			{
			lpRESULTWININFO	lprw;
			CREATESTRUCT		*cs;
			MDICREATESTRUCT	*mdi;
			UINT		iStr;
			SDWORD	nChildID=-1;
			DWORD		dex,
						dex1,
						dex2;
			TCHAR		szBuff[LARGEBUFF];


			cs = (CREATESTRUCT *)lParam;
			mdi = (MDICREATESTRUCT *)cs->lpCreateParams;
			lprw = (lpRESULTWININFO)mdi->lParam;
			SETRWPOINTER(hwnd, lprw);

			//Create ARD ArrayStatus window
			lprw->hwndARDCell=CreateWindow(
				TEXT("ComboBox"),
				NULL,
				WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP,
				0,0,0,0,
				hwnd,
				(HMENU)(ARDSTATUS_CHILD_ID),
				hInst,
				NULL);

			for (iStr=0;iStr < NumItems(g_rgArrayStatus);iStr++)
			{
				wsprintf(szBuff,TEXT("%s=%d"),g_rgArrayStatus[iStr].szString,
							g_rgArrayStatus[iStr].dwValue);

				dex=(DWORD)SendMessage(lprw->hwndARDCell,CB_ADDSTRING,0,(LPARAM)szBuff);

				dex1=(DWORD)SendMessage(lprw->hwndARDCell,CB_SETITEMDATA,dex,
									(LPARAM)g_rgArrayStatus[iStr].dwValue);

				dex2=(DWORD)SendMessage(lprw->hwndARDCell,CB_SETCURSEL,0,0L);

			}

			g_hNoRowBrush = CreateHatchBrush(HS_DIAGCROSS, GRAY_TEXT);

			// Now create windows
			lprw->hwndrgb=CreateWindow(
				TEXT("Edit"),
				NULL,
				WS_CHILD | ES_AUTOHSCROLL,
				0,0,0,0,
				hwnd,
				(HMENU)(RGB_CHILD_ID),
				hInst,
				NULL);

			//Create pcbValue window
			lprw->hwndpcb=CreateWindow(
				TEXT("ComboBox"),
				NULL,
				WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP,
				0,0,0,0,
				hwnd,
				(HMENU)(PCB_CHILD_ID),
				hInst,
				NULL);


			for (iStr=0;iStr < NumItems(g_rgPcbValues);iStr++)
			{
				wsprintf(szBuff,TEXT("%s=%d"),g_rgPcbValues[iStr].szString,
							g_rgPcbValues[iStr].dwValue);

				dex=(DWORD)SendMessage(lprw->hwndpcb,CB_ADDSTRING,0,(LPARAM)szBuff);

				dex1=(DWORD)SendMessage(lprw->hwndpcb,CB_SETITEMDATA,dex,
									(LPARAM)g_rgPcbValues[iStr].dwValue);

				dex2=(DWORD)SendMessage(lprw->hwndpcb,CB_SETCURSEL,0,0L);

			}


			lprw->hwnd = hwnd;
			lprw->hwndVertScroll = CreateWindowA("SCROLLBAR", NULL,
						WS_CHILD | WS_VISIBLE | SBS_VERT,
						0, 0, 0, 0,
						hwnd, (HMENU)(IDW_VERTBAR), hInst, NULL);
			lprw->hwndHorzScroll = CreateWindowA("SCROLLBAR", NULL,
						WS_CHILD | WS_VISIBLE | SBS_HORZ,
						0, 0, 0, 0,
						hwnd, (HMENU)(IDW_HORZBAR), hInst, NULL);

			if(!lprw->hwnd || !lprw->hwndVertScroll || !lprw->hwndHorzScroll)
				return -1;													// Let Windows kill children

			// Allocate memory for row widths if there are
			// bound columns
			if (lprw->lpStmt->uBoundNodes)
			{
				lprw->colx = (int *)AllocateMemory(sizeof(int) *
											lprw->lpStmt->uBoundNodes);
				lprw->colpcbx = (int *)AllocateMemory(sizeof(int) *
											lprw->lpStmt->uBoundNodes);
			}

			lprw->uPcbVals = (UINT *)AllocateMemory(MINREQUIREDINTS(TOT_ROWSET_SIZE(lprw->lpStmt)));

			if(!lprw->uPcbVals)
			{
				FREE_PTR(lprw->colpcbx);
				FREE_PTR(lprw->colx);
				FREE_PTR(lprw->uPcbVals);
				return -1;
			}

			// If that was successful, then figure out the column height
			if(!hColumnFont)
				hColumnFont = CreateColumnFont();
			ResetRsltWin(lprw);

			// Need to modify child system menu
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

			// Change Close to Close Statement
			if(hMenu = GetSystemMenu(hwnd, FALSE)) {
				ModifyMenu(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_STRING, IDM_FREESTMT_CLOSE,
							GetGtrString(szOutBuff, sizeof(szOutBuff), idsFREESTMTCLOSE));
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

		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			{
				HDC	hdc=(HDC)wParam;
				HWND	hwndEdit=(HWND)lParam;

//				SetBkColor(hdc,g_crBkColor);
//				SetTextColor(hdc,g_TextColor);

			}
			break;


		//--------------------------------------------------------------------
		// WM_SIZE:			Use this message to move the child windows to
		//						correct locations.  Note that we must break to
		//						allow DefMDIWndProc to handle min/max messages
		//--------------------------------------------------------------------
		case WM_SIZE:
			{
			lpRESULTWININFO			lprw;
			WORD							dx = LOWORD(lParam);
			WORD							dy = HIWORD(lParam);

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			// Store client width and height
			lprw->dx = dx;
			lprw->dy = dy;
			DoSize(lprw);
			}
			break;


		//--------------------------------------------------------------------
		// WM_VSCROLL:		Handle any vertical scroll requests.
		//--------------------------------------------------------------------
		case WM_VSCROLL:
			{
			lpRESULTWININFO			lprw;

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			DoVertScroll(lprw,
							 GET_WM_VSCROLL_CODE(wParam, lParam),
							 GET_WM_VSCROLL_POS(wParam, lParam));
			}
			break;


		//--------------------------------------------------------------------
		// WM_HSCROLL:		Handle any horizontal scroll requests.
		//--------------------------------------------------------------------
		case WM_HSCROLL:
			{
			lpRESULTWININFO			lprw;

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			DoHorzScroll(lprw,
							 GET_WM_HSCROLL_CODE(wParam, lParam),
							 GET_WM_HSCROLL_POS(wParam, lParam));
			}
			break;


		//--------------------------------------------------------------------
		// WM_KEYDOWN:		We'll scroll based on cursor keys
		//--------------------------------------------------------------------
		case WM_KEYDOWN:
			{
			lpRESULTWININFO			lprw;

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			DoKeyDownScroll(lprw, wParam, lParam);
			}
			break;


		//--------------------------------------------------------------------
		// WM_LBUTTONDBLCLK:	User has double-clicked the rowset window, see
		//							if it is a valid cell and if so, bring up hex
		//							view on it.
		//--------------------------------------------------------------------
		case WM_LBUTTONDBLCLK:
			{
			lpRESULTWININFO			lprw;

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			DoDblClickRowset(lprw, (int)wParam, LOWORD(lParam), HIWORD(lParam));
			}
			break;


		//--------------------------------------------------------------------
		// WM_MOUSEMOVE:		Use this message to print out info about current
		//							cell.
		//		NOTE:  This was *way* too slow.  I've disabled the code and
		//					left it in, just in case there is a future use for it.
		//--------------------------------------------------------------------
/*		case WM_MOUSEMOVE:
			{
			lpRESULTWININFO			lprw;

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			DoMouseMove(lprw, wParam, LOWORD(lParam), HIWORD(lParam));
			}
			break;
*/

		//
		// Hande the WM_INITMENUPOPUP message so that when the user selects
		//		a menu, we enable/disable each item based on our current state.
		//
		case WM_INITMENUPOPUP:
			{
			if(!(BOOL)HIWORD(lParam))			// Not the system menu
				ResetMenuStatus((HMENU)wParam, (int)LOWORD(lParam), 2);
			}
			break;


		//--------------------------------------------------------------------
		// WM_PAINT:		Add 3d affect to tool bar.
		//--------------------------------------------------------------------
		case WM_PAINT:
			{
			lpRESULTWININFO		lprw;
			PAINTSTRUCT				ps;
			HDC						hdc;
			RECT						updateRect;
			UWORD						*rgArrayStatusPtr=NULL;


			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);
			if(GetUpdateRect(hwnd, &updateRect, TRUE))
			{
				if(hdc = BeginPaint(hwnd, &ps))
				{
					ASSERTCHECK(lprw,TEXT("lprw cannot be NULL!!"));

					//Get the Array Status Ptr from the ARD if it
					//exists
					if (lprw->lpStmt && lprw->lpStmt->lpARD)
						rgArrayStatusPtr=lprw->lpStmt->lpARD->rgArrayStatusPtr;

					// Paint columns if required
					if(updateRect.top <= lprw->coly)
						DrawColumns(hdc, &updateRect, lprw,rgArrayStatusPtr);

					// Now paint the data
					DrawData(hdc, &updateRect, lprw,rgArrayStatusPtr);

					// Put edges on tool-bar and small rect
					Draw3DEdges(hwnd, hdc, &lprw->rToolRect);
					if(lprw->fVertPresent && lprw->fHorzPresent)
						Draw3DEdges(hwnd, hdc, &lprw->rMiniBar);

					// Now draw the mini tool-bar text if required
					if(updateRect.bottom >= lprw->rToolRect.top)
					{
						UINT					cyExtra = (4 * GetSystemMetrics(SM_CYBORDER));
						UINT					cxExtra = cyExtra * 2;
						RECT					tool;
						HFONT					hf;

						tool.left = cxExtra;
						tool.top = lprw->rToolRect.top + cyExtra;
						tool.right = lprw->rToolRect.right - cxExtra;
						tool.bottom = lprw->rToolRect.bottom - cyExtra;
						Draw3DBorder(hwnd, hdc, &tool, (UINT)-1);
						hf=SelectObject(hdc, lpStat->hFont);
						SetTextColor(hdc, BLACK_TEXT);
						SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
						ExtTextOut(hdc,
									tool.left + 3,
									tool.top,
									DFT_ETO_FLAGS,
									&tool,
									lprw->szStatText,
									lstrlen(lprw->szStatText),
									NULL);
						SelectObject(hdc, hf);
					}

					EndPaint(hwnd, &ps);
				}
			}
			}
			break;


		//--------------------------------------------------------------------
		// WM_SYSCOMMAND:		Need to track this message in case the user
		//							closes the window via the system menu.  Handle
		//							this scenario as a Free Statement close.
		//--------------------------------------------------------------------
		case WM_SYSCOMMAND:
			{
			if(wParam == IDM_FREESTMT_CLOSE) {
				SetCurrentStmt(lpActiveResultWin->lpci, lpActiveResultWin->lpStmt);
				DoFreeStmtClose(FALSE);
				return 0;
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
			lpRESULTWININFO		lprw;
			UINT						id=GET_WM_COMMAND_ID(wParam, lParam);
			UINT						code=GET_WM_COMMAND_CODE(wParam, lParam);
			int						iRow=1,
										iCol=1;

			lprw = (lpRESULTWININFO)GETRWPOINTER(hwnd);

			if (id == RGB_CHILD_ID  || id == PCB_CHILD_ID)
			{
				if (code== EN_KILLFOCUS)
					UpdateRgbValue(lprw);
				else if (code== CBN_KILLFOCUS)
					UpdatePcbValue(lprw);
			}
			else if (id == ARDSTATUS_CHILD_ID)
			{
				 if (code== CBN_KILLFOCUS)
					UpdateARDCell(lprw);
			}
			else if (id >= DLG_BASE_BEGIN && id <= DLG_BASE_END)
			{
				// Menu item is one of the ODBC API functions
				// In range of API function
				int		apid = GetSqlFuncOffset((UWORD)(id - IDM_API_BASE));

         	if(apid != -1)
         		DialogManager(lprw->lpci, apid, 0, 0);
         }

			switch(id)
			{

				// For default menu options which are in our menu range, go ahead
				//		and send back to client to handle.
				default:
					if(id >= IDM_LOWEST_MENU &&
						id <= IDM_HIGHEST_MENU)
						SendMessage(lprw->hwndClient, WM_COMMAND, wParam, lParam);
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
			TCHAR			szFmt[MAXOUTBUFF];
			lpENVIRONMENTINFO lpActiveEnv=NULL;

			*szOutBuff = TEXT('\0');
#ifndef WIN32
			if(wParam) {					// Activate
				lpActiveResultWin = (lpRESULTWININFO)GETRWPOINTER((HWND)LOWORD(lParam));
				}
#else
			if((HWND)lParam == hwnd) {
				lpActiveResultWin = (lpRESULTWININFO)GETRWPOINTER(hwnd/*(HWND)lParam*/);
				}
#endif
			else {
				lpActiveResultWin = NULL;
				lpActiveConnWin = NULL;
				}

			// If we are activating, then set the current active windows and
			//		handles.  This results window inherits its parent as an
			//		active connection
			if(lpActiveResultWin)
			{
				lpActiveConnWin = lpActiveResultWin->lpci;
				hwndCurMDIChild = lpActiveResultWin->hwnd;
				if(lpActiveResultWin->lpStmt)
				{
					wsprintf(szOutBuff,
						GetGtrString(szFmt, MAXOUTBUFF, idsResultWinDesc),
						lpActiveResultWin->lpStmt->uStmtNum);
				}

				//Display active henv
				if (lpActiveConnWin && lpActiveConnWin->lpActiveEnv)
				{
					lpActiveEnv=lpActiveConnWin->lpActiveEnv;
					SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL,lpActiveEnv->uEnvNum, 0L);
				}

			}
			else
				hwndCurMDIChild = NULL;

			RefreshToolbar();
			SetStatBarText(hwndStatBar, szOutBuff, BLACK_TEXT);
			}
			return 0;


		default:
			break;
		}

	//
	// If we haven't already processed the message, do default behavior.
	//
	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}


//*---------------------------------------------------------------------------------
//| fAnyKeyCheck:
//|	Wait for a key press to continue.
//| Parms:
//|	None
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN fAnyKeyCheck(void)
{
	MSG 			msg;

	while(GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST) &&
			msg.message != WM_KEYDOWN)
		;
}



//*---------------------------------------------------------------------------------
//| GetFontDim:
//|	Figure out the height and width of the font being used.
//| Parms:
//|	hwnd						Window handle to check
//|	hFont						The font to size
//|	lpicx						Average char width
//|	lpily						Recommended line height
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN GetFontDim(HWND hwnd, HFONT hFont, int * lpicx, int * lpily,
						int * lpiTitle)
{
	HDC				hdc;
	HFONT				hf;
	TEXTMETRIC		tm;
	int				iExtra;

	// Select font into the current device to get precise dimensions
	hdc = GetDC(hwnd);
	hf = SelectObject(hdc, hFont);

	// Return font information and size it
	GetTextMetrics(hdc, &tm);
	*lpicx = tm.tmAveCharWidth;
	*lpily = tm.tmHeight + tm.tmExternalLeading;
	iExtra = GetSystemMetrics(SM_CYBORDER);
	*lpily += (5 * iExtra);
	*lpiTitle = *lpily - (2 * iExtra);

	// Put the old font back
	SelectObject(hdc,hf);
	ReleaseDC(hwnd, hdc);
}


//*---------------------------------------------------------------------------------
//| GetColWidthBasedOnValues:
//|	Go through all of the data in the current rowset and measure them.  Return
//|	the exact pixel width required to show the largest of these.
//| Parms:
//|	hdc						Use this device context to calculate widths
//|	lprw						The rowset window information
//|	lpbv						The current bound var we're looking at
//| Returns:
//|	The exact pixel width required
//*---------------------------------------------------------------------------------
int INTFUN GetColWidthBasedOnValues(HDC hdc, lpRESULTWININFO lprw, lpBOUNDVARS lpbv)
{
	UDWORD	cbOff;					// Offset for data type
	PTR		rgbValue;
	SDWORD	*pcbValue=NULL;
	SIZE		sz;
	UDWORD	iRows;
	int		iWidth=0;
	LPTSTR	szDspBuff;
	int		i;
	int		cbBuff=(MAXDSPCHARWIDTH+1)* sizeof(TCHAR) + sizeof(TCHAR);
	lpSTATEMENTINFO lpStmt=NULL;
	lpCONNECTIONINFO lpCI=NULL;

	if (!lprw || !lprw->lpStmt || !lprw->lpStmt->lpVars)
		return(0);

	if (!(szDspBuff = (LPTSTR)AllocateMemory(cbBuff)))
		return cbBuff;

	if (lprw && lprw->lpStmt)
		lpCI=lprw->lpStmt->lpCI;

	for (i=0;i < MAXDSPCHARWIDTH;i++)
		szDspBuff[i]=TEXT('A');

	if(lprw->lpStmt->fColBinding)
		cbOff = (DWORD)GetSizeOfCType(lpbv->fCType, (UDWORD)lpbv->cbElement);
	else
		cbOff = lprw->lpStmt->uRowWiseSize;

	// For each row in this bound column, find the larget pixel width
	for(iRows=0;  iRows<TOT_ROWSET_SIZE(lprw->lpStmt);  iRows++) {
		// First find the address of the data
		rgbValue = FindAddress(lpbv->rgbValue, 0, iRows, cbOff);
	   if(lpbv->pcbValue)
      	pcbValue = (SDWORD *)FindAddress(lpbv->pcbValue, 0,
      			iRows,
      			(lprw->lpStmt->fColBinding) ? sizeof(SDWORD) : cbOff);

      // Now format the data
		FormatRowsetCell(szDspBuff, rgbValue, pcbValue,
				lpbv->fCType, lpbv->cbElement,cbBuff,GETOUTPUTWINDOW(lpCI));

		// Finally mesaure the item
		GetTextExtentPoint(hdc,
					(LPTSTR)szDspBuff, lstrlen(szDspBuff),
					(SIZE *)&sz);
		iWidth = max(sz.cx, iWidth);
		}

	FREE_PTR(szDspBuff);

	return iWidth;
}


//*---------------------------------------------------------------------------------
//| SetColWidths:
//|	For each column, use the default font to figure out its width and store
//|		this information.
//| Parms:
//|	hdc						Device context
//|	lprw						The results window to set
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN SetColWidths(HWND hwnd, lpRESULTWININFO lprw)
{
	lpBOUNDVARS		lpbv;
	HDC				hdc;
	HFONT				hf;									// Store the old font
	TCHAR				szBigRow[20];
	int				iTotChars;
	TCHAR				szRowCol[MAXTITLE];				// Row Column title
	SIZE				sz;
	UDWORD			dex;
	SDWORD			iPcbWidth=0;
	UNALIGNED SDWORD		*pcbValue;
	lpSTATEMENTINFO		lpStmt=NULL;
	lpDESCRIPTORHEADER	lpIRD=NULL,
								lpARD=NULL;
	int						nPcbWidth=0;

	lprw->fArrayStatusExists=FALSE,
	lprw->frgfRowStatusExists=FALSE;

	if (lprw->lpStmt)
	{
		lpStmt=lprw->lpStmt;
		lpARD=lpStmt->lpARD;
		lpIRD=lpStmt->lpIRD;

		switch(lpStmt->wFetchAPI)
		{
			case SQL_API_SQLEXTENDEDFETCH:
				lprw->frgfRowStatusExists=TRUE;
				lprw->fArrayStatusExists=FALSE;
				break;
			case SQL_API_SQLFETCHSCROLL:
			case SQL_API_SQLFETCH:
				//we only want to show the rgfRowStatus
				//if the Row Status array in the IRD
				//is valid
				if (lpARD && lpARD->rgArrayStatusPtr)
					lprw->fArrayStatusExists=TRUE;

				if (lpIRD && lpIRD->rgArrayStatusPtr)
					lprw->frgfRowStatusExists=TRUE;

				//backward compatibility
				else if (!lpIRD && lpStmt && lpStmt->rgfRowStatus)
					lprw->frgfRowStatusExists = TRUE	;
				break;
		}

	}

	hdc = GetDC(hwnd);
	hf = SelectObject(hdc, hColumnFont);

	// First figure out the size of the row number column
	//		NOTE:  Must cast the SIZE struct to far.  In nodebug
	//					version, segment was getting lost causing problems
	wsprintf(szBigRow, TEXT("%u"), ((UWORD)-1));
	GetTextExtentPoint(hdc,
				(LPTSTR)szBigRow, lstrlen(szBigRow),
				(SIZE *)&sz);
	lprw->rownumcolcx = lprw->rowcx = sz.cx;

	// We now have largest number, make sure there's room for "Row"
	GetGtrString(szOutBuff, sizeof(szOutBuff), idsRowColumnTitle);
	GetTextExtentPoint(hdc,
				(LPTSTR)szOutBuff, lstrlen(szOutBuff),
				(SIZE *)&sz);
	lprw->rownumcolcx = lprw->rowcx = max(sz.cx, lprw->rowcx);

	// Now look at status columns if necessary
	lprw->statcx = 0;
	if (lpUsrOptions->fShowStatus)
	{
		if (lprw->frgfRowStatusExists)
		{
			for(dex=0;  dex<NumItems(g_szRowStatus);  dex++)
			{
				GetTextExtentPoint(hdc, (LPTSTR)g_szRowStatus[dex],
						lstrlen(g_szRowStatus[dex]),
						(SIZE *)&sz);
				if(sz.cx > lprw->statcx)
					lprw->statcx = sz.cx;
			}

			lprw->statcx = (int)(lprw->statcx * 1.2);			// Add a little extra
			lprw->rowcx += lprw->statcx;
		}
	}

	//set the coord. where the editable cells begin.
	lprw->left=lprw->rowcx;

	// Now make room for the Array Status ptr in the APD
	lprw->arraystatcx = 0;

	//Only if it exists
	if (lprw->fArrayStatusExists)
	{
		if(lpUsrOptions->fShowStatus)
		{
			for(dex=0;  dex<NumItems(g_rgArrayStatus);  dex++)
			{
				GetTextExtentPoint(hdc, (LPTSTR)g_rgArrayStatus[dex].szString,
						lstrlen(g_rgArrayStatus[dex].szString),
						(SIZE *)&sz);
				if(sz.cx > lprw->arraystatcx)
					lprw->arraystatcx = sz.cx;
			}

			lprw->arraystatcx = (int)(lprw->arraystatcx * 1.2);	// Add a little extra
			lprw->rowcx += lprw->arraystatcx;
		}
	}

	// Reset font to the smaller width font
	SelectObject(hdc, hEditFont);

	// Now go through each node
	lprw->sumdx = 0;												// Will contain total width
	lpbv = lprw->lpStmt->lpVars;
	dex = 0;
	if(lpbv)
	{
		UINT			uNodes = lprw->lpStmt->uBoundNodes;
		int			iFromColWidth;

		//Get width of pcbValue Combo-box
		GetTextExtentPoint(hdc, (LPTSTR)TEXT("SQL_DATA_AT_EXEC=-6"),
						lstrlen(TEXT("SQL_DATA_AT_EXEC=-6")),
						(SIZE *)&sz);

		nPcbWidth=(int)(sz.cx*PIXELFUDGE);

		// For each bound node
		while(uNodes--)
		{
			if(lpbv->fBound )

			{
				if(lpUsrOptions->fShowPcbs && (pcbValue = lpbv->pcbValue))
				{
					// Go through each pcb value for this column and find the largest
					//	based on number of digits, assume '0' is largest width.

						lprw->colpcbx[dex] = nPcbWidth;

				}

				// Now size the actual type
				iFromColWidth = (int)GetColWidthBasedOnValues(hdc, lprw, lpbv);
				iTotChars = min(iFromColWidth, MAXDSPCHARWIDTH);


				// Given the number of expected chars, find a good width which will
				//	either be the size required for the column header or the data,
				//	whichever is larger.
				SelectObject(hdc, hColumnFont);
				if(lpUsrOptions->fShowColumns)
				{
					GetTextExtentPoint(hdc, lpbv->szCol,
							lstrlen(lpbv->szCol), (SIZE *)&sz);
				}
				else
				{
					wsprintf(szRowCol, szCOLNUM, lpbv->icol);
					GetTextExtentPoint(hdc, szRowCol,
							lstrlen(szRowCol), (SIZE *)&sz);
				}

				SelectObject(hdc, hEditFont);

				lprw->colx[dex] = max(sz.cx, (iTotChars + lprw->colpcbx[dex]));
				lprw->colx[dex] = (int)(lprw->colx[dex] * PIXELFUDGE);

				// Keep the width in the sum for scroll calculations
				lprw->sumdx += lprw->colx[dex++];
			}

			lpbv = lpbv->next;
		}
	}

	SelectObject(hdc, hf);					// Replace old font
	ReleaseDC(hwnd, hdc);					//   and then the DC
}


//*---------------------------------------------------------------------------------
//| SetMiniBarText:
//|	Change the text in the mini status bar
//| Parms:
//|	lprw						For handles and rects
//|	szStr						The new text. If NULL, use the default settings
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN SetMiniBarText(lpRESULTWININFO lprw, LPCTSTR szStr)
{
	*lprw->szStatText = TEXT('\0');

	if(szStr) {
		if(*szStr)
			lstrcpy(lprw->szStatText, szStr);
		}
	else {
		lpSTATEMENTINFO	lpStmt=lprw->lpStmt;

		wsprintf(lprw->szStatText,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsMiniStats),
				ACT_ROWSET_SIZE(lpStmt),
				lpStmt->uCurrentRow,
				lpStmt->uReservedRows,
				(lpStmt->fColBinding) ? (LPTSTR)szCOLBIND : (LPTSTR)szROWBIND,
				(LPTSTR)szCursorType[lpStmt->uCursorType],
				(LPTSTR)szConcurType[(UDWORD)(lpStmt->uConcurType-1)]);
		}

	InvalidateRect(lprw->hwnd, &lprw->rToolRect, TRUE);
}

//*---------------------------------------------------------------------------------
//| DoDblClickRowset:
//|	If user dbl-clicked a valid cell, then show the user that cell's contents
//|	in hex.
//| Parms:
//|	lprw					Results window information
//|	fKeyCode				Indicates button status
//|	x						x pixel location
//|	y						y pixel location
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoDblClickRowset(lpRESULTWININFO lprw, int fKeyCode, int mx, int my)
{
	RECT			rScrollArea;
	lpBOUNDVARS	lpbv=lprw->lpStmt->lpVars;
	int			irow;
	int			iTop, iLeft;				// for scroll position
	int			x;
	int			dex;
	int			top;
	int			*picolx;
	static int	iLastCol, iLastRow;
	PTR			rgbValue=NULL;
	SDWORD		*pcbValue=NULL;
	TCHAR			szDspBuff[MAXDSPCHARWIDTH]=TEXT("");
	UDWORD		cbMax=0;
	SDWORD		FixUnalignment;
	BOOL			fValidCell=FALSE,
					fArrayStat=FALSE;
	int			left = 0; // BUGBUG: never initialized
	lpDESCRIPTORHEADER	lpARD=lprw->lpStmt->lpARD;
	UWORD			*rgArrStatPtr=NULL;
	lpSTATEMENTINFO lpStmt=NULL;
	lpCONNECTIONINFO lpCI=NULL;

	if (!lprw || !lprw->lpStmt || !lprw->lpStmt->lpVars)
		return;

	lpStmt=lprw->lpStmt;

	if (lpStmt->lpCI)
		lpCI=lpStmt->lpCI;

	// See if point is in over our data
	GetHorzVertScrollArea(lprw, &rScrollArea);

	if(mx < rScrollArea.left ||
		mx > rScrollArea.right ||
		my < rScrollArea.top ||
		my > rScrollArea.bottom)
		return;												// Not over data

	// Figure out logical scrolling offsets
	iTop =
	iLeft = 0;
	if(lprw->fHorzPresent)
		iLeft = GetScrollPos(lprw->hwndHorzScroll, SB_CTL);

	if(lprw->fVertPresent)
		iTop = GetScrollPos(lprw->hwndVertScroll, SB_CTL);

	// Now see which cell it is
	irow = ((my - lprw->coly) / lprw->ly) + iTop;
   if((UDWORD)(irow+1) > TOT_ROWSET_SIZE(lprw->lpStmt))		// Row must be within rowset
   	return;

	x = FindColOffset(lprw);

	picolx = &lprw->colx[0];							// Pointer goes faster through array
	while(!lpbv->fBound)
		lpbv = lpbv->next;

	for(dex=0;  dex<(int)lprw->lpStmt->uBoundNodes;  dex++, ++picolx)
	{
		//Is this a valid Rowset cell?
		if(mx >= x && mx <= x + *picolx)
		{
			//Yup!!
			fValidCell=TRUE;
			break;
		}

		x += lprw->colx[dex];
		lpbv = lpbv->next;

		while(!lpbv->fBound)
			lpbv = lpbv->next;

	}

	//if this is not a valid rowset cell maybe it's the
	//Row Status Array.
	if (mx >= left)
		fArrayStat=TRUE;

	// Find first row and add in column height
	top=(irow * lprw->ly) + lprw->coly;

	//Kill current focus
	SetFocus(NULL);

	lprw->iCurrRow=irow;
	lprw->iCurrCol=lpbv->icol;

	//The following code only applies to RowSet Cells
	if (fValidCell)
	{
		if(lprw->lpStmt->fColBinding)
			cbMax = (DWORD)GetSizeOfCType(lpbv->fCType, (UDWORD)lpbv->cbElement);
		else
			cbMax = lprw->lpStmt->uRowWiseSize;

		// Find rgbValue in array
		rgbValue = FindAddress(lpbv->rgbValue, iTop,	irow, cbMax);
		pcbValue = FindAddress(lpbv->pcbValue, iTop,	irow,
							(lprw->lpStmt->fColBinding) ? sizeof(SDWORD) : cbMax);

		if (UnModified(rgbValue,cbMax))
			*szDspBuff=TEXT('\0');
		else
			FormatRowsetCell(szDspBuff, rgbValue, pcbValue,
									lpbv->fCType, lpbv->cbElement,MAXDSPCHARWIDTH+1,
									GETOUTPUTWINDOW(lpCI));

		//ShowWindow(lprw->hwndrgb,SW_HIDE);
		InvalidateRect(lprw->hwnd,&lprw->rRgbCell,TRUE);

		lprw->rRgbCell.left=x;
		lprw->rRgbCell.top=top;
		lprw->rRgbCell.right=x+(lprw->colx[dex]-lprw->colpcbx[dex]);
		lprw->rRgbCell.bottom=top-lprw->iTitleHeight+HEIGHTFUDGE;

		//User wants to edit rowset cell move edit window or combo box
		//to this location
		MoveWindow(	lprw->hwndrgb,
						lprw->rRgbCell.left,							// horizontal position
						lprw->rRgbCell.top,							// vertical position
						lprw->colx[dex]-lprw->colpcbx[dex],		// width
						lprw->iTitleHeight+HEIGHTFUDGE,			// height
						TRUE);											// repaint flag

		//Set contents of edit window to current data
		SetWindowText(lprw->hwndrgb,szDspBuff);
		ShowWindow(lprw->hwndrgb,SW_SHOW);
		SendMessage(lprw->hwndrgb,EM_SETSEL,0L,(LPARAM)-1);
		SetFocus(lprw->hwndrgb);

		FixUnalignment = *(UNALIGNED SDWORD *)pcbValue;
		wsprintf(szDspBuff, TEXT("%ld"), FixUnalignment);

		//ShowWindow(lprw->hwndrgb,SW_HIDE);
		InvalidateRect(lprw->hwnd,&lprw->rPcbCell,TRUE);
		lprw->rPcbCell.left=x+(lprw->colx[dex]-lprw->colpcbx[dex]);
		lprw->rPcbCell.top=top;
		lprw->rPcbCell.right=x+lprw->colpcbx[dex];
		lprw->rPcbCell.bottom=top-lprw->iTitleHeight*NUM_CB_ROWS;

		//User wants to edit rowset cell move edit window or combo box
		//to this location
		MoveWindow(	lprw->hwndpcb,
						lprw->rPcbCell.left,						// horizontal position
						lprw->rPcbCell.top,						// vertical position
						lprw->colpcbx[dex],							// width
						lprw->iTitleHeight*NUM_CB_ROWS,			// height
						TRUE);											// repaint flag

		ShowWindow(lprw->hwndpcb,SW_SHOW);
		AddNewPcbValue(FixUnalignment,0,lprw->hwndpcb);

	}
	else if (fArrayStat)
	{
		InvalidateRect(lprw->hwnd,&lprw->rARDCell,TRUE);

		rgArrStatPtr = FindAddress(lpARD->rgArrayStatusPtr,0,irow, sizeof(UWORD));

		lprw->rARDCell.left=left;
		lprw->rARDCell.top=top;
		lprw->rARDCell.right=left+lprw->arraystatcx;
		lprw->rARDCell.bottom=top-lprw->iTitleHeight+HEIGHTFUDGE;

		SetStatusValue(lprw->hwndARDCell,*rgArrStatPtr);

		//User wants to edit ArrayStatusPtr cell move combo box
		//to this location
		MoveWindow(	lprw->hwndARDCell,
						lprw->rARDCell.left,							// horizontal position
						lprw->rARDCell.top,							// vertical position
						lprw->arraystatcx+lprw->rownumcolcx,	// width
						lprw->iTitleHeight*NUM_CB_ROWS,			// height
						TRUE);											// repaint flag

		ShowWindow(lprw->hwndARDCell,SW_SHOW);
	}


}


//*---------------------------------------------------------------------------------
//| DoMouseMove:
//|	Look at the position and tell user what to expect.
//| Parms:
//|	lprw					Results window information
//|	fKeyCode				Indicates button status
//|	x						x pixel location
//|	y						y pixel location
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoMouseMove(lpRESULTWININFO lprw, int fKeyCode, int mx, int my)
{
	RECT			rScrollArea;
	int			icol, irow;
	int			iTop, iLeft;				// for scroll position
	int			x;
	int			dex;
	int			*picolx;
	static int	iLastCol, iLastRow;
	int			left;

	// See if point is in over our data
	GetHorzVertScrollArea(lprw, &rScrollArea);

	//Editable cells begin after rgfRowStatus
	left=rScrollArea.left-lprw->statcx;

	if(mx < left ||
		mx > rScrollArea.right ||
		my < rScrollArea.top ||
		my > rScrollArea.bottom)
		return;												// Not over data

	// Figure out logical scrolling offsets
	iTop =
	iLeft = 0;
	if(lprw->fHorzPresent)
		iTop = GetScrollPos(lprw->hwndHorzScroll, SB_CTL);
	if(lprw->fVertPresent)
		iLeft = GetScrollPos(lprw->hwndVertScroll, SB_CTL);

	// Now see which cell it is
	irow = ((my - lprw->coly) / lprw->ly) + iTop;

	x = FindColOffset(lprw);

	picolx = &lprw->colx[0];							// Pointer goes faster through array
	for(dex=0;  dex<(int)lprw->lpStmt->uBoundNodes;  dex++, ++picolx)
	{
		if(mx >= x && mx <= x + *picolx)
			break;

		x += lprw->colx[dex];
	}

	if(dex < (int)lprw->lpStmt->uBoundNodes)
		icol = dex + iLeft;

	wsprintf(szOutBuff, TEXT("Row:  %d      Col:  %d"), irow, icol);
	SetMiniBarText(lprw, (LPCTSTR)szOutBuff);
}


//*---------------------------------------------------------------------------------
//| DoSize:
//|	Figure out where each piece should be based on new size.
//| Parms:
//|	lprw					Results window information
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoSize(lpRESULTWININFO lprw)
{
	UINT							cyExtra = (2 * GetSystemMetrics(SM_CYBORDER));
	UINT							cScrollWidth = GetSystemMetrics(SM_CXHSCROLL);
	UINT							yToolBar;
	RECT							rScrollArea;
	int							irows=0;
	int							icols=0;
	int							iOrigVert=0;
	int							iOrigHorz=0;

	// Figure overall dimensions
	yToolBar = lpStat->dy;					// Use height of the main app status bar

	// Calculate rect for 3d affect
	lprw->rToolRect.left = 0;
	lprw->rToolRect.top = lprw->dy - yToolBar;
	lprw->rToolRect.right = lprw->dx;
	lprw->rToolRect.bottom = lprw->dy;

	// Determine which scroll bars are needed for display.  Do this by
	//	finding scrollable area
	lprw->fVertPresent =
	lprw->fHorzPresent = FALSE;
	GetHorzVertScrollArea(lprw, &rScrollArea);									// Assumes no scroll bars
	lprw->fHorzPresent = ((rScrollArea.right - lprw->rowcx <					// TRUE if we need scroll bar
								lprw->sumdx) != 0) ? TRUE : FALSE;

	if(lprw->fHorzPresent)
	{
		rScrollArea.bottom -= GetSystemMetrics(SM_CYHSCROLL);
		iOrigHorz = GetScrollPos(lprw->hwndHorzScroll, SB_CTL);
	}

	lprw->fVertPresent = ((UDWORD)((rScrollArea.bottom - rScrollArea.top) / lprw->ly) <
								TOT_ROWSET_SIZE(lprw->lpStmt))
								? TRUE : FALSE;

	if(lprw->fVertPresent)
	{
		rScrollArea.right -= GetSystemMetrics(SM_CXVSCROLL);
		iOrigVert = GetScrollPos(lprw->hwndVertScroll, SB_CTL);
	}

	// Now the scrollable area is valid and takes the scroll bars into account
	if(lprw->fHorzPresent)
	{
		icols = lprw->sumdx - (rScrollArea.right - rScrollArea.left);
		icols = (icols / DFTHORZINC(lprw)) + ((icols % DFTHORZINC(lprw)) ? 1 : 0);
	}

	// If there is need for a vertical scroll bar, then figure out how many
	// rows cannot fit in the display.  Set scroll range accordingly.
	if(lprw->fVertPresent)
	{
		UDWORD		uRows;

		uRows = (rScrollArea.bottom - rScrollArea.top) / lprw->ly;		// Rows that can fit
		uRows = TOT_ROWSET_SIZE(lprw->lpStmt) - uRows;

		// Windows max scroll range/pos is an int, so don't allow the user
		//	to go beyond this limit.  This is a limiation of ODBC Test that
		//	a user is very unlikely to hit (> 32767 rowset?!)
		if(uRows > UINT_MAX)
		{
			szMessageBox(lprw->hwnd,
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szWARNING,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsMaxRowsReached),
					UINT_MAX);
			irows = UINT_MAX;
		}
		else
			irows = (int)uRows;
		ASSERTCHECK(irows<=0, TEXT("Invalid irows"));
	}

	// Set values regardless of whether they are present
	SetScrollRange(lprw->hwndHorzScroll, SB_CTL, 0, icols, TRUE);
	SetScrollPos(lprw->hwndHorzScroll, SB_CTL, min(icols, iOrigHorz), TRUE);
	SetScrollRange(lprw->hwndVertScroll, SB_CTL, 0, irows, TRUE);
	SetScrollPos(lprw->hwndVertScroll, SB_CTL, min(irows, iOrigVert), TRUE);

	// Now do the mini bar
	if(lprw->fVertPresent && lprw->fHorzPresent)
	{
		lprw->rMiniBar.left = lprw->dx - cScrollWidth;
		lprw->rMiniBar.top = lprw->dy - yToolBar - cScrollWidth;
		lprw->rMiniBar.right = lprw->dx;
		lprw->rMiniBar.bottom = lprw->rMiniBar.top + cScrollWidth - 1;
	}

	// Move the windows
	if(lprw->fVertPresent)
	{
		MoveWindow(lprw->hwndVertScroll,
						lprw->dx - cScrollWidth,								// x
						0,																// y
						cScrollWidth,												// width
						lprw->dy - yToolBar -             					// height
						 	((lprw->fHorzPresent) ? cScrollWidth : 0),
						TRUE);
	}

	if(lprw->fHorzPresent)
	{
		MoveWindow(lprw->hwndHorzScroll, 0,									// x
						lprw->dy - yToolBar - 									// y
							cScrollWidth,
						lprw->dx - 													// width
							((lprw->fVertPresent) ? cScrollWidth : 0),
						cScrollWidth,												// height
						TRUE);
	}
	ShowWindow(lprw->hwndVertScroll,
					(lprw->fVertPresent) ? SW_SHOW : SW_HIDE);

	ShowWindow(lprw->hwndHorzScroll,
					(lprw->fHorzPresent) ? SW_SHOW : SW_HIDE);
}


//*---------------------------------------------------------------------------------
//| DoVertScroll:
//|	Handle vertical scroll requests.  This may include handling keyboard input.
//| Parms:
//|	lprw						Results window information
//|	iScrollCode				The to indicate what we're doing
//|	iPos						Based on iScrollCode
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoVertScroll(lpRESULTWININFO lprw, int iScrollCode, int nPos)
{
	int		iMin, iMax;
	int		iPos;
	int		iOrig;
	int		iPage;
	int		dy, y;
	RECT		rScrollArea;

	// Figure out how big a page is by looking at the available
	//	scrollable area, then find num of lines which will fit
	GetHorzVertScrollArea(lprw, &rScrollArea);

	// First find the scroll range to use relative to our data
	GetScrollRange(lprw->hwndVertScroll, SB_CTL, &iMin, &iMax);
	iPage = (rScrollArea.bottom - rScrollArea.top) / lprw->ly;
	iPos = iOrig = GetScrollPos(lprw->hwndVertScroll, SB_CTL);

	switch(iScrollCode) {
		case SB_BOTTOM:
			iPos  = iMax;
			break;

		case SB_LINEDOWN:
			iPos += ONEROW;
			break;

		case SB_LINEUP:
			iPos -= ONEROW;
			break;

		case SB_PAGEDOWN:
			iPos += iPage;
			break;

		case SB_PAGEUP:
			iPos -= iPage;
			break;

		case SB_TOP:
			iPos  = iMin;
			break;

		case SB_THUMBPOSITION:
			iPos = nPos;
			break;
		}

	// Don't leave range by accident
	if(iPos < iMin)
		iPos = iMin;
	if(iPos > iMax)
		iPos = iMax;

	// If required, scroll the window
	if(iPos != iOrig) {
		SetScrollPos(lprw->hwndVertScroll, SB_CTL, iPos, TRUE);
		rScrollArea.left = 0;					// Scroll row for vertical
		rScrollArea.right = min(lprw->sumdx + lprw->rowcx, rScrollArea.right);
		ScrollWindow(lprw->hwnd,
							0, 						// No horizontal scroll
							dy = (iOrig - iPos) * lprw->ly,
							&rScrollArea,
							&rScrollArea);
		// See if there is a partial row.  If so, we need to invalidate
		//	the extra pixels for the partial row in addition to the
		//	area uncovered by the scroll operation.
		y = (rScrollArea.bottom - rScrollArea.top) % lprw->ly;
		if(y && dy < 0) {							// Going up with partial rows exposed
			rScrollArea.top = rScrollArea.bottom + dy - y;
			InvalidateRect(lprw->hwnd, &rScrollArea, TRUE);
			}
		UpdateWindow(lprw->hwnd);
		}
}


//*---------------------------------------------------------------------------------
//| DoHorzScroll:
//|	Handle horizontal scroll requests.  This may include handling keyboard input.
//| Parms:
//|	lprw						Results window information
//|	iScrollCode				The to indicate what we're doing
//|	iPos						Based on iScrollCode
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoHorzScroll(lpRESULTWININFO lprw, int iScrollCode, int nPos)
{
	int		iMin, iMax;
	int		iPos;
	int		iOrig;
	int		iPage;
	int		dx;
	RECT		rScrollArea;

	// Figure out how big a page is by looking at the available
	//	scrollable area, then find num of lines which will fit
	GetHorzVertScrollArea(lprw, &rScrollArea);

	// First find the scroll range to use relative to our data
	GetScrollRange(lprw->hwndHorzScroll, SB_CTL, &iMin, &iMax);
	iPage = (rScrollArea.right - rScrollArea.left) / DFTHORZINC(lprw);
	iPos = iOrig = GetScrollPos(lprw->hwndHorzScroll, SB_CTL);

	switch(iScrollCode) {
		case SB_BOTTOM:
			iPos  = iMax;
			break;

		case SB_LINEDOWN:
			iPos += ONEROW;
			break;

		case SB_LINEUP:
			iPos -= ONEROW;
			break;

		case SB_PAGEDOWN:
			iPos += iPage;
			break;

		case SB_PAGEUP:
			iPos -= iPage;
			break;

		case SB_TOP:
			iPos  = iMin;
			break;

		case SB_THUMBPOSITION:
			iPos = nPos;
			break;
		}

	// Don't leave range by accident
	if(iPos < iMin)
		iPos = iMin;
	if(iPos > iMax)
		iPos = iMax;

	// If required, scroll the window
	if(iPos != iOrig) {
		SetScrollPos(lprw->hwndHorzScroll, SB_CTL, iPos, TRUE);
		rScrollArea.top = 0;						// Need to scroll column titles
		ScrollWindow(lprw->hwnd,
							dx = (iOrig - iPos) * DFTHORZINC(lprw),
							0,							// No vertical scroll
							&rScrollArea,
							&rScrollArea);
		UpdateWindow(lprw->hwnd);
		}
}


//*---------------------------------------------------------------------------------
//| IsCtrlPressed:
//|	Retrieves the state of the keybaord and looks to see if the Ctrl key is
//|	pressed.  Do this by indexing the keys virtual code and checking the
//|	hi order bit.  If set, it is pressed.
//| Parms:
//|	Nothing
//| Returns:
//|	TRUE if the Ctrl key is pressed, FALSE otherwise
//*---------------------------------------------------------------------------------
BOOL INTFUN IsCtrlPressed()
{
	return (GetKeyState(VK_CONTROL) & 0x80);
}


//*---------------------------------------------------------------------------------
//| DoKeyDownScroll:
//|	This function allows the user to scroll the results set via the keyboard.
//|	This is done by mapping each key to a corresponding scroll message.
//| Parms:
//|	lprw						Results window information
//|	wKeyId					The key code id (See WM_KEYDOWN)
//|	dwKeyData				The key modifiers (See WM_KEYDOWN)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoKeyDownScroll(lpRESULTWININFO lprw,
					WPARAM wKeyId, LPARAM dwKeyData)
{
	switch(wKeyId) {
		case VK_HOME:
			if(IsCtrlPressed())
				DoVertScroll(lprw, SB_TOP, 0);
			else
				DoHorzScroll(lprw, SB_TOP, 0);
			break;

		case VK_END:
			if(IsCtrlPressed())
				DoVertScroll(lprw, SB_BOTTOM, 0);
			else
				DoHorzScroll(lprw, SB_BOTTOM, 0);
			break;

		case VK_UP:
			DoVertScroll(lprw, SB_LINEUP, 0);
			break;

		case VK_DOWN:
			DoVertScroll(lprw, SB_LINEDOWN, 0);
			break;

		case VK_LEFT:
			if(IsCtrlPressed())
				DoHorzScroll(lprw, SB_PAGEUP, 0);
			else
				DoHorzScroll(lprw, SB_LINEUP, 0);
			break;

		case VK_RIGHT:
			if(IsCtrlPressed())
				DoHorzScroll(lprw, SB_PAGEDOWN, 0);
			else
				DoHorzScroll(lprw, SB_LINEDOWN, 0);
			break;

		case VK_PRIOR:
			DoVertScroll(lprw, SB_PAGEUP, 0);
			break;

		case VK_NEXT:
			DoVertScroll(lprw, SB_PAGEDOWN, 0);
			break;
		}
}


//*---------------------------------------------------------------------------------
//| GetHorzVertScrollArea:
//|	Given a results set, find the rectangle which bounds the scrollable area.
//|	Note that the row column and column titles are fixed in location, and
//|	therefore, are not part of the area.
//| Parms:
//|	lprw						Results window information
//|	lphzRect					The scrollable rectangle
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN GetHorzVertScrollArea(lpRESULTWININFO lprw, lpRECT lphzRect)
{
	lphzRect->left = lprw->left+1;	// Left side of editable
	lphzRect->top  = lprw->coly;		// Top picks up below column titles

	// Right and bottom are decided based on whether a scroll-bar is present.
	//		Bottom also must subtract out the mini tool bar.
	lphzRect->right = lprw->dx -
								((lprw->fVertPresent) ? GetSystemMetrics(SM_CXVSCROLL) : 0);
	lphzRect->bottom = lprw->dy -
								(lprw->rToolRect.bottom - lprw->rToolRect.top) -
								((lprw->fHorzPresent) ? GetSystemMetrics(SM_CYHSCROLL) : 0);

}


//*---------------------------------------------------------------------------------
//| FindFirstLastCol:
//|	Given an update rectangle and other information, return the first and
//|	last columns requiring painting.   Note that the row column
//|	and column titles are fixed.
//| Parms:
//|	lprw						Results window information
//|	lpuRect					The update rectangle
//|	piFirst					First column needing painting
//|	piLast					Last column
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN FindFirstLastCol(lpRESULTWININFO lprw,
					lpRECT lpuRect,
					int * piFirst, int * piLast)
{
	int				x;
	int				iCol = 0;
	UINT				dex;

	*piFirst = -1;
	*piLast = -1;

	// Find the first column by looking at the pixel location of each
	//	column and comparing it to the update rectangle.
	x = FindColOffset(lprw);

	for(dex=0;  dex<lprw->lpStmt->uBoundNodes;  dex++) {
		if(lpuRect->right > x)				// x is left side of iCol
			*piLast = iCol;
		x += lprw->colx[dex];				// Right side of iCol
		if(*piFirst == -1 &&
			lpuRect->left < x)				// This column in update region
			*piFirst = iCol;

		++iCol;
		}
	++*piLast;
}


//*---------------------------------------------------------------------------------
//| FindFirstLastRow:
//|	Given an update rectangle and other information, return the first and
//|	last rows requiring painting.  Note that the row column
//|	and column titles are fixed.  Also note that returned rows are 0 based.
//| Parms:
//|	lprw						Results window information
//|	lpuRect					The update rectangle
//|	piFirst					First column needing painting
//|	piLast					Last column
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN FindFirstLastRow(lpRESULTWININFO lprw,
					lpRECT lpuRect,
					int * piFirst, int * piLast)
{
	int	top, dy, irows, sy;
	int	cScrollWidth = GetSystemMetrics(SM_CYHSCROLL);
	int	iRowSet;


	top = max(0, lpuRect->top - lprw->coly);			// Remove column height from calculation
	*piFirst = top / lprw->ly;								// First row, zero based

	// Bottom must take into account the mini-status bar
	//	which is always present, and the horz scroll bar
	//	which might be present
	dy = lpuRect->bottom - lpuRect->top;				// Updateable area
	if(lpuRect->top < lprw->coly)							// Remove column header overlap
		dy -= (lprw->coly - lpuRect->top);
	if(lprw->fHorzPresent) {
		sy = lprw->dy - cScrollWidth;
		if(lpuRect->bottom > sy)
			dy -= (lpuRect->bottom - sy);
		}
	if(lpuRect->bottom > lprw->rToolRect.top)			// Is area over stat bar?
		dy -= (lpuRect->bottom - lprw->rToolRect.top);
	irows = (dy / lprw->ly) +
				((dy % lprw->ly) ? 1 : 0);					// Rows that will fit (include partial)
	iRowSet = (int)TOT_ROWSET_SIZE(lprw->lpStmt) -
					 ((lprw->fVertPresent) ? GetScrollPos(lprw->hwndVertScroll, SB_CTL) : 0);
	*piLast = min(*piFirst + irows, iRowSet);
}



//*---------------------------------------------------------------------------------
//| FindBoundColNum:
//|	Return the iCnt'th bound column by traversing the linked list of bound
//|	nodes for iCnt.  Only bound nodes are considered.
//| Parms:
//|	lpbv						First bound var to look at
//|	uBoundNodes				How many bound cols are there
//|	iCnt						Which do we want (start at 0)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
lpBOUNDVARS FindBoundColNum(lpBOUNDVARS lpbv, UINT uBoundNodes, int iCnt)
{
	while(lpbv && uBoundNodes--) {
		if(lpbv->fBound) {
			if(!iCnt)
				return lpbv;
			else
				--iCnt;
			}
		lpbv = lpbv->next;
		}

	return NULL;
}


//*---------------------------------------------------------------------------------
//| DrawColumns:
//|	Paint the column headers.  We'll do an optimized paint here so that only
//|	those columns which need painting will be done.
//| Parms:
//|	hdc						Device context
//|	hwnd						Window to paint
//|	uRect						The update rectangle
//|	lprw						Results window information
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DrawColumns(HDC hdc, lpRECT uRect, lpRESULTWININFO lprw,
								UWORD *rgArrayStatusPtr)
{
	int				iFirstCol, iLastCol;				// First and last columns required
	int				x;										// Use for pixel offset
	int				icol=0;								// Current column number
	RECT				colRect;								// Bounding rect for each column
	TCHAR				szRowCol[MAXTITLE];				// Row Column title
	PAINTINFO		pi;
	UINT				uBoundNodes;
	lpBOUNDVARS		lpbv;

	// Create any required brushes
	memset(&pi, 0, sizeof(PAINTINFO));
	pi.hdc = hdc;
	pi.hNormFont = hEditFont;
	pi.hGrayBrush = GetStockBrush(LTGRAY_BRUSH);

	// Locate the first and last columns to be drawn, apart from the
	//		row column which is fixed.
	FindFirstLastCol(lprw, uRect, &iFirstCol, &iLastCol);

	// Now figure out the left offset for painting
	colRect.top = 0;									// Always at 0
	colRect.bottom = lprw->coly - 1;				// Calculated height
	lpbv = lprw->lpStmt->lpVars;
	uBoundNodes = lprw->lpStmt->uBoundNodes;

	// Left offset, could be negative
	x = FindColOffset(lprw);

	while(uBoundNodes-- && icol <= iLastCol)
	{
		// Don't paint unbound columns
		if(lpbv->fBound)
		{
			if(icol >= iFirstCol)
			{
				colRect.left = x;
				colRect.right = colRect.left + lprw->colx[icol] - 1;

				// If user wants to show the column names, pass them in
				if(lpUsrOptions->fShowColumns)
				{
					PaintColumn(&pi, &colRect, lpbv->szCol, 0);
				}
				else
				{
					wsprintf(szRowCol, szCOLNUM, lpbv->icol);
					PaintColumn(&pi, &colRect, szRowCol, 0);
				}
			}

			x += lprw->colx[icol];
			icol++;
		}

		lpbv = lpbv->next;							// Keep going through list
	}

	//Paint the Array status pointer from the ARD if present
	if(rgArrayStatusPtr && uRect->left <= lprw->rowcx)
	{
		colRect.left = lprw->rownumcolcx+lprw->statcx;
		colRect.right = lprw->rowcx - 1;
		PaintColumn(&pi, &colRect, TEXT("ArrayStat"), lprw->arraystatcx);
	}

	//Draw rgfRowStatus column
	// Row column is frozen, so paint at static location if required
	if(uRect->left <= lprw->rowcx)
	{
		colRect.left = lprw->rownumcolcx;
		colRect.right = colRect.left+lprw->statcx;

		lstrcpy(szRowCol,(LPTSTR)szSTATUS);
		PaintColumn(&pi, &colRect, szRowCol, lprw->statcx);
	}

	//Draw the Row # column
	if(uRect->left <= lprw->rowcx)
	{
		GetGtrString(szRowCol, sizeof(szRowCol)/sizeof(szRowCol[0]), idsRowColumnTitle);
		colRect.left = 0;
		colRect.right = lprw->rownumcolcx;

		PaintColumn(&pi, &colRect, szRowCol, lprw->statcx);
	}

	DeleteObject(pi.hGrayBrush);
}


//*---------------------------------------------------------------------------------
//| PaintColumn:
//|	Paint a specific column in the rect provided.  This function is used for
//|	The "Row" | "Status" upper left corner, each column header, and each row number
//|	along the left hand side.  The rgfRowStatus parm indicates which we are
//|	doing at this time.
//| Parms:
//|	lppi						Paint information
//|	lpRect					Bounding rect for column
//|	szOutStr					The column title
//|	statcx					Width of status column (ignored unless (rgfRowStatus))
//|	rgfRowStatus			Indicates one of the following:
//|									NULL  			Don't ignore this value
//|									-1   				Paint the "Row", possibly "Status" columns
//|									0 to 5			Paint the valid status value
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN PaintColumn(lpPAINTINFO lppi, lpRECT lpRect, LPCTSTR szOutStr,
						int statcx)
{
	RECT			rect;
	HFONT			hOldFont;


	CopyRect(&rect, lpRect);

	// Put the 3d edges on columns
	FillRect(lppi->hdc, lpRect, lppi->hGrayBrush);

	//Paint the top of the 3d box (i.e. a single horz. white line)
	PatBlt(lppi->hdc, lpRect->left, lpRect->top,
				lpRect->right - lpRect->left, 1, WHITENESS);

	//Paint left side of 3d box (i.e. a single vert. white line
	PatBlt(lppi->hdc, lpRect->left, lpRect->top,
				1, lpRect->bottom - lpRect->top, WHITENESS);

	//Paint the bottom of the 3d box (i.e. a single horz. black line
	PatBlt(lppi->hdc, lpRect->left, lpRect->bottom,
					lpRect->right - lpRect->left, 1, BLACKNESS);
	//Paint the right side of the 3d box (i.e. a single vert. black line
	PatBlt(lppi->hdc, lpRect->right, lpRect->top,
					1, lpRect->bottom - lpRect->top, BLACKNESS);

	// Draw the text centered in the column
	SetTextColor(lppi->hdc, BLACK_TEXT);
	SetBkColor(lppi->hdc, GetSysColor(COLOR_BTNFACE));

	++rect.top;

	// Now do the actual paint
	hOldFont = SelectObject(lppi->hdc, hColumnFont);
	DrawText(lppi->hdc,
				szOutStr,
				lstrlen(szOutStr),
				&rect,
				DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SelectObject(lppi->hdc, hOldFont);
}

//*---------------------------------------------------------------------------------
//| PaintARDStatusColumn:
//|	Paint the Array Status Ptr for the current ARD.
//| Parms:
//|	lppi						Paint information
//|	lpRect					Bounding rect for column
//|	szOutStr					The column title
//|	statcx					Width of status column (ignored unless (rgfRowStatus))
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN PaintARDStatusColumn(lpPAINTINFO lppi, lpRECT lpRect, LPCTSTR szOutStr,
						int statcx)
{
	RECT		rect;

	CopyRect(&rect, lpRect);

	// Draw the text centered in the column
	if(szOutStr)
	{
		ExtTextOut(lppi->hdc,
					rect.left + DFTEXTRA,
					rect.top + DFTEXTRA,
					DFT_ETO_FLAGS,
					&rect,
					szOutStr,
					lstrlen(szOutStr),
					NULL);
	}

	// Put the 3d edges on columns
	PatBlt(lppi->hdc, lpRect->left, lpRect->bottom,
					lpRect->right - lpRect->left, 1,
					PATCOPY);
	PatBlt(lppi->hdc, lpRect->right, lpRect->top,
					1, lpRect->bottom - lpRect->top,
					PATCOPY);

} //PaintARDStatusColumn()


//*---------------------------------------------------------------------------------
//| CreateRowsetFont:
//|	Create a font based on flags.
//| Parms:
//|	lpUsrOpts				User options for font and pt size
//|	fStrike					TRUE for stirke-through font
//|	fUnderline				TRUE for underline font
//|	fItalic					TRUE for italic font
//| Returns:
//|	The font handle created or NULL on error
//*---------------------------------------------------------------------------------
HFONT INTFUN CreateRowsetFont(lpUSROPTIONS lpUsrOpts, BOOL fStrike,
						BOOL fUnderline, BOOL fItalic)
{
	HFONT			hFont;
	LOGFONT		lf;

	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = lpUsrOpts->iFontHeight;
	lf.lfWeight = FW_THIN;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
	lf.lfItalic = (BYTE)fItalic;
	lf.lfUnderline = (BYTE)fUnderline;
	lf.lfStrikeOut = (BYTE)fStrike;
	lstrcpy(lf.lfFaceName, (LPTSTR)lpUsrOpts->szFont);
	hFont = CreateFontIndirect(&lf);
	if(!hFont)
		ASSERTCHECK(TRUE, TEXT("Could not create font"));

	return hFont;
}


//*---------------------------------------------------------------------------------
//| SetAlign:
//|	Use the SetTextAlign function to set the alignment to the left or right
//|	of the specified origin used in ExtTextOut.
//| Parms:
//|	hdc						Device context
//|	fLeft						TRUE if alignment is left, FALSE for right
//| Returns:
//|	The old attribute from GetTextAlign
//*---------------------------------------------------------------------------------
UINT INTFUN SetAlign(HDC hdc, BOOL fLeft)
{
	UINT		uAlign;

	uAlign = GetTextAlign(hdc);
	uAlign &= ~(TA_RIGHT | TA_CENTER | TA_LEFT);
	uAlign |= ((fLeft) ? TA_LEFT : TA_RIGHT);

	return SetTextAlign(hdc, ((fLeft) ? TA_LEFT : TA_RIGHT));
}


//*---------------------------------------------------------------------------------
//| DrawData:
//|	Display the data required using the fonts as described in the TCM
//|	data base to reflect the rgfRowStatus flags.
//| Parms:
//|	hdc						Device context
//|	hwnd						Window to paint
//|	uRect						The update rectangle
//|	lprw						Results window information
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DrawData(HDC hdc, lpRECT uRect, lpRESULTWININFO lprw,
							UWORD *rgArrayStatusPtr)
{
	int				iFirstCol, iLastCol;				// First and last columns required
	int				iFirstRow, iLastRow;				// First and last rows to paint
	int				x;										// Use for pixel offset
	int				icol=0;								// Current column number
	int				irow=0;
	int				itop;
	TCHAR				szRowNum[MAXROWNUM];
	TCHAR				szPcbVal[20];
	LPTSTR				szPcbStr=NULL;
	RECT				colRect;								// Bounding rect for each column
	PAINTINFO		pi;									// Store GDI objects
	HBRUSH			hOldBrush;
	HPEN				hNoRowPen = NULL;					// Need light pen
	HFONT				hFont;
	HFONT				hOldFont;							// For restoring context
	HFONT				hNormFont = hEditFont;			// Use as default
	HFONT				hStrikeFont = NULL;				// For deleted rows, strike through
	HFONT				hUnderlineFont = NULL;			// For updated rows, underlined
	HFONT				hItalicFont = NULL;				// For added rows, italics
	UINT				uBoundNodes;
	lpBOUNDVARS		lpbv;
	UWORD 			uDftSuccess = NO_RGF_ARRAY;	// Use when there is no rgfRowStatus
	LPTSTR			szDspBuff;

	// Indexed into arrays
	PTR				rgbValue;
	SDWORD			*pcbValue=NULL;
	UWORD				*rgfRowStatus=NULL;
	UWORD				*rgArrayStatPtr=NULL;
	lpSTATEMENTINFO lpStmt=NULL;
	lpCONNECTIONINFO lpCI=NULL;
	BOOL					fUnmodified=FALSE;

	if (!lprw || !lprw->lpStmt || !lprw->lpStmt->lpVars)
		return;

	lpStmt=lprw->lpStmt;

	if (lpStmt->lpCI)
		lpCI=lpStmt->lpCI;

	szDspBuff = (LPTSTR)AllocateMemory(MAXDSPCHARWIDTH+1);
	if(!szDspBuff)
		return;

	// Create any required brushes
	memset(&pi, 0, sizeof(PAINTINFO));
	pi.hdc = hdc;
	pi.hNormFont = hEditFont;
	pi.hGrayBrush = GetStockBrush(LTGRAY_BRUSH);
	pi.hDottedPen = CreatePen(PS_DOT, 1, GetSysColor(COLOR_BTNFACE));

	hOldBrush = SelectObject(hdc, pi.hGrayBrush);

	// Figure out which row is on top
	itop = GetScrollPos(lprw->hwndVertScroll, SB_CTL);

	// Locate the first and last columns to be drawn, apart from the
	//		row column which is fixed.
	FindFirstLastCol(lprw, uRect, &iFirstCol, &iLastCol);

	// Determine based on update rectangle which rows need to be
	//		repainted.
	FindFirstLastRow(lprw, uRect, &iFirstRow, &iLastRow);

	// First loop around each row to paint
	for(irow = iFirstRow;  irow < iLastRow;  irow++)
	{
		// Now figure out the left offset for painting
		colRect.top = (irow * lprw->ly) +
								lprw->coly;					// Find first row and add in column height
		colRect.bottom = colRect.top +
								lprw->ly -1;				// Bottom based on line height

		// Now find row status in array if there
		if(lprw->lpStmt->rgfRowStatus)
		{
			rgfRowStatus = (UWORD *)FindAddress(lprw->lpStmt->rgfRowStatus,
									itop, irow, sizeof(UWORD));
		}
		else
			rgfRowStatus = &uDftSuccess;

		// Set background and font based on row status
		switch(*rgfRowStatus) {
			// Success is black text on white background, normal text
			case NO_RGF_ARRAY:
			case SQL_ROW_SUCCESS:
			case SQL_ROW_SUCCESS_WITH_INFO:
				hFont = hNormFont;
				SetTextColor(hdc, BLACK_TEXT);
				SetBkColor(hdc, WHITE_TEXT);
				break;

			// Deleted is strike-through font on white
			case SQL_ROW_DELETED:
				// May need to create the font
				if(!hStrikeFont)
					if(!(hStrikeFont = CreateRowsetFont(lpUsrOptions, TRUE,
								FALSE, FALSE)))
						hStrikeFont = hEditFont;		// If create fails, default
				hFont = hStrikeFont;
				SetTextColor(hdc, BLACK_TEXT);
				SetBkColor(hdc, WHITE_TEXT);
				break;

			// Black text on blue background
			case SQL_ROW_UPDATED:
				// May need to create the font
				if(!hUnderlineFont)
					if(!(hUnderlineFont = CreateRowsetFont(lpUsrOptions, FALSE,
								TRUE, FALSE)))
						hUnderlineFont = hEditFont;		// If create fails, default
				hFont = hUnderlineFont;
				SetTextColor(hdc, BLACK_TEXT);
				SetBkColor(hdc, LTBLUE_TEXT);
				break;

			// Gray text on white
			case SQL_ROW_NOROW:
				if(!g_hNoRowBrush) {
					g_hNoRowBrush = CreateHatchBrush(HS_DIAGCROSS, GRAY_TEXT);
					ASSERTCHECK((!g_hNoRowBrush), TEXT("Error creating a hatched brush"));
					}
				SetBkColor(hdc, WHITE_TEXT);
				break;

			// Black italic text on light blue
			case SQL_ROW_ADDED:
				// May need to create the font
				if(!hItalicFont)
					if(!(hItalicFont = CreateRowsetFont(lpUsrOptions, FALSE,
								FALSE, TRUE)))
						hItalicFont = hEditFont;		// If create fails, default
				hFont = hItalicFont;
				SetTextColor(hdc, BLACK_TEXT);
				SetBkColor(hdc, LTBLUE_TEXT);
				break;

			// Black text on red background
			case SQL_ROW_ERROR:
				hFont = hNormFont;
				SetTextColor(hdc, BLACK_TEXT);
				SetBkColor(hdc, RED_TEXT);
				break;

			default:
				ONDEBUG(wsprintf(szOutBuff, TEXT("Unexpected *rgfRowStatus=%u"), *rgfRowStatus));
				ASSERTCHECK(TRUE, szOutBuff);
				break;
		}

		if (fUnmodified=UnModified(rgfRowStatus,sizeof(UWORD)))
		{
			if(!g_hNoRowBrush)
			{
				g_hNoRowBrush = CreateHatchBrush(HS_DIAGCROSS, GRAY_TEXT);
				ASSERTCHECK((!g_hNoRowBrush), TEXT("Error creating a hatched brush"));
			}

			SetBkColor(hdc, WHITE_TEXT);
		}

		// Current row is identified with yellow background
		if((int)lprw->lpStmt->uCurrentRow == itop + irow + 1 ||	// This is the current row
			lprw->lpStmt->uCurrentRow == 0)								// Or all are selected (block operation)
		{
			SetBkColor(hdc, YELLOW_TEXT);
		}

		// Now loop around each column to be painted.  In this loop, irow is the
		//		logical row number for the screen, 0 always being the top row.
		//		itop is the offset into our rowset which is the top.
		x = FindColOffset(lprw);

		icol = 0;
		lpbv = lprw->lpStmt->lpVars;

		uBoundNodes = lprw->lpStmt->uBoundNodes;

		while(uBoundNodes-- && icol <= iLastCol)
		{
			UDWORD			cbMax;

			if(lpbv->fBound)
			{							// Don't paint unbound columns
				if(icol >= iFirstCol)
				{
					// Find correct offset
					if(lprw->lpStmt->fColBinding)
						cbMax = (DWORD)GetSizeOfCType(lpbv->fCType, (UDWORD)lpbv->cbElement);
					else
						cbMax = lprw->lpStmt->uRowWiseSize;

					// Find left and right of rectangle
					colRect.left = x;
					colRect.right = x + lprw->colx[icol] - 1;

					// Find rgbValue in array
					rgbValue = FindAddress(lpbv->rgbValue, itop,	irow, cbMax);

					// Find pcbValue in array if valid
					if(lpbv->pcbValue)
					{
						pcbValue = (SDWORD *)FindAddress(lpbv->pcbValue,
											itop, irow,
											(lprw->lpStmt->fColBinding) ? sizeof(SQLLEN) : cbMax);
					}
					else
						pcbValue = NULL;

					// For a no row, just fill the rectangle with a hatch pattern.
					//	Will need to clear background first since hatch brush does
					//	not do so by itself
					if(fUnmodified || (*rgfRowStatus == SQL_ROW_NOROW) || UnModified(rgbValue,cbMax))
					{
						FillRect(hdc, &colRect, g_hNoRowBrush);
						PaintData(&pi, &colRect, NULL, 0, NULL);
					}
					// Format output value, if char, just use text to avoid quotes
					else
					{
						if (lprw->fNoDataFound)
						{
							lstrcpy(szDspBuff,TEXT(""));
							szPcbStr=szDspBuff;
						}
						else
						{
							FormatRowsetCell(szDspBuff, rgbValue, pcbValue,
										lpbv->fCType, lpbv->cbElement,MAXDSPCHARWIDTH+1,
										GETOUTPUTWINDOW(lpCI));


							// If we are showing pcb values, then it could be either <null>
							//	when none are allocated, or the actual value
							if(lpUsrOptions->fShowPcbs)
							{
								if(!pcbValue)
									szPcbStr = (LPTSTR)szRSLTNULLVAL;
								else
								{
									SDWORD FixUnalignment = *(UNALIGNED SDWORD *)pcbValue;
									wsprintf(szPcbVal, TEXT("%ld"), FixUnalignment ); //*pcbValue);
									szPcbStr = szPcbVal;
								}
							}
						}

						// Now pull in fonts and brushes and paint the data
						hOldFont = SelectObject(hdc, hFont);
						PaintData(&pi, &colRect, szDspBuff, lprw->colpcbx[icol], szPcbStr);
						SelectObject(hdc, hOldFont);
					}

				}
				x += lprw->colx[icol];					// Start out next column after this one
				++icol;
			}
			lpbv = lpbv->next;							// Keep going through list
		}

		//Paint the Array status pointer from the ARD if present
		if(rgArrayStatusPtr && uRect->left <= lprw->rowcx)
		{
			rgArrayStatPtr = FindAddress(rgArrayStatusPtr, 0,
											irow, sizeof(UWORD));

			colRect.left = lprw->statcx+lprw->rownumcolcx;
			colRect.right = lprw->rowcx - 1;
			hOldFont = SelectObject(hdc, hFont);
			PaintData(&pi, &colRect, g_rgArrayStatus[*rgArrayStatPtr].szString,0,NULL);
			SelectObject(hdc, hOldFont);
		}

		// Restore brush if required
		if(fUnmodified || (*rgfRowStatus == SQL_ROW_NOROW))
			SelectObject(hdc, pi.hGrayBrush);

		// Draw rgfRowStatus column
		if (uRect->left <= lprw->rowcx)
		{
			colRect.left = lprw->rownumcolcx;
			colRect.right = lprw->rownumcolcx+lprw->statcx;

			if (lpUsrOptions->fShowStatus)
			{
				if(fUnmodified || (*rgfRowStatus == NO_RGF_ARRAY))
					lstrcpy(szRowNum,(LPTSTR)szRSLTUNKNOWN);
				else
				{
					if ((rgfRowStatus >= 0) && (*rgfRowStatus < NumItems(g_szRowStatus)))
						lstrcpy(szRowNum,g_szRowStatus[*rgfRowStatus]);
				}

				SelectObject(pi.hdc, pi.hNormFont);
			}

			PaintColumn(&pi, &colRect, szRowNum, lprw->statcx);
		}


		//Draw the Row # column
		if(uRect->left <= lprw->rowcx)
		{
			wsprintf(szRowNum, TEXT("%d"), (itop + irow + 1));		// irow is 1 based per the spec
			colRect.left = 0;
			colRect.right = lprw->rownumcolcx;

			PaintColumn(&pi, &colRect, szRowNum, lprw->statcx);
		}

	}		// End of loop through each row

	// Kill of any GDI devices we created during paint
	SelectObject(hdc, hOldBrush);
	DeleteObject(pi.hGrayBrush);
	DeleteObject(pi.hDottedPen);
	if(hStrikeFont)
		DeleteObject(hStrikeFont);
	if(hUnderlineFont)
		DeleteObject(hUnderlineFont);
	if(hItalicFont)
		DeleteObject(hItalicFont);
	FREE_PTR(szDspBuff);
}


//*---------------------------------------------------------------------------------
//| PaintData:
//|	Paint a specific column in the rect provided.
//| Parms:
//|	lppi						Paint information
//|	lpRect					Bounding rect for column
//|	szOutStr					The column title
//|	szPcbStr					The pcb value to display (NULL means no display)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN PaintData(lpPAINTINFO lppi, lpRECT lpRect, LPCTSTR szOutStr,
					int iPcbWidth, LPCTSTR szPcbStr)
{
	RECT		rect;

//#define DEBUG_BOX
#ifdef DEBUG_BOX
	DrawFocusRect(lppi->hdc, lpRect);
	fAnyKeyCheck();
	DrawFocusRect(lppi->hdc, lpRect);
#endif

	CopyRect(&rect, lpRect);
	rect.right -= iPcbWidth;

	// Draw the text centered in the column
	if(szOutStr)
		ExtTextOut(lppi->hdc,
					rect.left + DFTEXTRA,
					rect.top + DFTEXTRA,
					DFT_ETO_FLAGS,
					&rect,
					szOutStr,
					lstrlen(szOutStr),
					NULL);

	// Now paint the pcbs if required
	if(szPcbStr) {
		HPEN		hOldPen;

		CopyRect(&rect, lpRect);
		rect.left = rect.right - iPcbWidth;

#ifdef DEBUG_BOX
	DrawFocusRect(lppi->hdc, &rect);
	fAnyKeyCheck();
	DrawFocusRect(lppi->hdc, &rect);
#endif

		// Set the text alignment for the pcb Value to right aligned
		//	since numbers are usually right aligned
		SetAlign(lppi->hdc, FALSE);
		ExtTextOut(lppi->hdc,
					rect.right - DFTEXTRA,
					rect.top + DFTEXTRA,
					DFT_ETO_FLAGS,
					&rect,
					szPcbStr,
					lstrlen(szPcbStr),
					NULL);
		SetAlign(lppi->hdc, TRUE);

		// Draw the dashed line to separate data from pcb
		PatBlt(lppi->hdc, rect.left, rect.top,
						1, rect.bottom - rect.top,
						WHITENESS);								// Erase any background pixels
		hOldPen = SelectObject(lppi->hdc, lppi->hDottedPen);
		MoveTo(lppi->hdc, rect.left, rect.top);
		LineTo(lppi->hdc, rect.left, rect.bottom);
		SelectObject(lppi->hdc, hOldPen);
		}

	// Put the 3d edges on columns
	PatBlt(lppi->hdc, lpRect->left, lpRect->bottom,
					lpRect->right - lpRect->left, 1,
					PATCOPY);
	PatBlt(lppi->hdc, lpRect->right, lpRect->top,
					1, lpRect->bottom - lpRect->top,
					PATCOPY);
}

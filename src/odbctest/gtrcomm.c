//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	GTRCOMM.C
//|
//| Purpose:
//|		This module contains utility functions used by many other modules.
//|		It also contains the code for the user options dialog.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "resource.h"
#include <htmlhelp.h>
#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define szDEBUGFILE					TEXT("C:\\TMP\\MEM")
#define MAX_VERSION_LEN				10

#define DFT_GATOR_LENGTH			300
#define DFT_PARM_LENGTH				75
#define MIN_PARM_LENGTH				50
#define DFT_BIND_BUFF				254
#define MAX_BIND_BUFF				65534
#define C_PAGES						5


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE				hInst;
extern lpUSROPTIONS			lpUsrOptions;
extern HWND						hwndStatBar;
extern HWND						hwndOutWin;
extern HWND						hwndClient;
extern HMENU 					hMenu;
extern HFONT					hEditFont;					// For edit windows and rowset windows
extern HFONT					hColumnFont;				// For columns in rowset windows
extern int						iTestHelpCnt;			// How many times have we loaded this help file?
extern LPTSTR					szTestHelpFile;
extern lpCONNECTIONINFO		lpConnWin;					// Head node for connection windows
extern lpHEADENV				lpHeadEnv;
extern UINT						giStmtHeight;
extern lpCONNECTIONINFO		lpActiveConnWin;			// The currently active connection window
extern lpDFTARRAY				lpdftCTypes;
extern lpDFTARRAY				lpdftSqlTypes;
extern lpDFTARRAY				lpdftCTypesNoD;
extern UINT						ciCTypes;
extern UINT						ciCTypesNoD;
extern BOOL						fUnicode;
extern TCHAR					g_szGatorIni[_MAX_PATH];
TCHAR 			szOutBuff[MAXOUTBUFF];

// Ini file settings
#ifndef WIN32
dCSEG(TCHAR)	szINIFILE[]							= TEXT("GATOR.INI");
#else
#ifdef _UNICODE
dCSEG(TCHAR)	szINIFILE[]							= TEXT("GATOR32W.INI");
#else
dCSEG(TCHAR)	szINIFILE[]							= TEXT("GATOR32.INI");
#endif
#endif

dCSEG(TCHAR) szMSSansSerif[]						= TEXT("MS Sans Serif");
dCSEG(TCHAR) szu[] 									= TEXT("%u");
dCSEG(TCHAR) szd[] 									= TEXT("%d");
dCSEG(TCHAR) szNOTFOUND[]							= TEXT("strnf");
dCSEG(TCHAR) szAssertion[]							= TEXT("Assertion failure: %s, %s, %d");
dCSEG(TCHAR) szErr[]									= TEXT("Error");
dCSEG(TCHAR) szFreePtrAssert[]					= TEXT("Assert:  Tried to free a NULL pointer, please use the FREE_PTR macro instead:  %s, %d");
dCSEG(TCHAR) szInvalidInstance[]					= TEXT("Invalid instance handle");
dCSEG(TCHAR) szStringNotFound[]					= TEXT("String %u not found.");
dCSEG(TCHAR) szInvalidRetcode[]					= TEXT("Invalid return code: %d");
dCSEG(TCHAR) szErrorMsgStr[]						= TEXT("\t\t\t\t\t\tszErrorMsg=\"%s\"\r\n");

dCSEG(TCHAR) szSTARTUP[]							= TEXT("Startup");
dCSEG(TCHAR) szDRVRMENUS[]							= TEXT("Driver Menus");
dCSEG(TCHAR) szAUTOERRORS[]						= TEXT("Auto Errors");
dCSEG(TCHAR) szNULLCHECKING[]						= TEXT("Null Checking");
dCSEG(TCHAR) szBUFFERCHECKING[]					= TEXT("Buffer Checking");
dCSEG(TCHAR) szVERSIONNUMBERS[]					= TEXT("Version Numbers");
dCSEG(TCHAR) szUSECURSORLIB[]						= TEXT("Use Cursor Library");
dCSEG(TCHAR) szODBCBEHAVIOR[]						= TEXT("ODBC Behavior");
dCSEG(TCHAR) szBINDCOLZERO[]						= TEXT("Bind Column Zero");
dCSEG(TCHAR) szAUTODISPLAY[]						= TEXT("Auto Display Rowset");
dCSEG(TCHAR) szDFTMSGSEV[]							= TEXT("Default Logging Level");
dCSEG(TCHAR) szRESERVEDROWS[]						= TEXT("Reserved Rows");
dCSEG(TCHAR) szDFTBUFFLEN[]						= TEXT("Default Buffer");
dCSEG(TCHAR) szDFTPARMLEN[]						= TEXT("Default Parm Buff");
dCSEG(TCHAR) szMAXCONNECTS[]						= TEXT("Max Connections");
dCSEG(TCHAR) szMAXSTMTS[]							= TEXT("Max Statements");
dCSEG(TCHAR) szFONT[]								= TEXT("Font Face");
dCSEG(TCHAR) szPTSIZE[]								= TEXT("Point Size");
dCSEG(TCHAR) szFONTHEIGHT[]						= TEXT("Font Height");
dCSEG(TCHAR) szSHOWSTATUS[]						= TEXT("Show Status");
dCSEG(TCHAR) szSHOWPCBS[]							= TEXT("Show Pcbs");
dCSEG(TCHAR) szSHOWCOLUMNS[]						= TEXT("Show Columns");
dCSEG(TCHAR) szDFTBINDBUFF[]						= TEXT("Default Bind Buff");
dCSEG(TCHAR) szDFTFONT[]							= TEXT("MS Sans Serif");
dCSEG(TCHAR) szINITDSN[]							= TEXT("Initial DSN");
dCSEG(TCHAR) szINITUID[]							= TEXT("Initial UID");
dCSEG(TCHAR) szODBCVer[]							= TEXT("ODBC Version");
dCSEG(TCHAR) szMAXIMIZED[]							= TEXT("maximized");
dCSEG(TCHAR) szMINIMIZED[]							= TEXT("mimimized");
dCSEG(TCHAR) szLEFT[]								= TEXT("Window Coordinate x");
dCSEG(TCHAR) szTOP[]									= TEXT("Window Coordinate y");
dCSEG(TCHAR) szWIDTH[]								= TEXT("Window Width");
dCSEG(TCHAR) szHEIGHT[]								= TEXT("Window Height");
dCSEG(TCHAR) szOUTPUTMAXIMIZED[]					= TEXT("Output maximized");
dCSEG(TCHAR) szOUTPUTMINIMIZED[]					= TEXT("Output mimimized");
dCSEG(TCHAR) szOUTPUTLEFT[]						= TEXT("Output Window Coordinate x");
dCSEG(TCHAR) szOUTPUTTOP[]							= TEXT("Output Window Coordinate y");
dCSEG(TCHAR) szOUTPUTWIDTH[]						= TEXT("Output Window Width");
dCSEG(TCHAR) szOUTPUTHEIGHT[]						= TEXT("Output Window Height");

// Following structure used for character names of return codes
extern lpDFTARRAY 	lpdftSqlCodes;
extern lpDFTARRAY 	lpdftTrueFalse;
extern lpDFTARRAY 	lpdftInstallerErrors;
extern UINT				ciSqlCodes;
extern UINT				ciTrueFalse;
extern UINT				ciInstallerErrors;



//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------


#if defined( _DEBUG ) && !defined( _RETAIL )
//*---------------------------------------------------------------------------------
//| DoAssertCheck:
//|	This function is only defined when we want asserts.
//| Parms:
//|	fCond					If TRUE show assert, else stop
//|	szMsg					The message from the user
//|	szFile				The file it came from
//|	iLine					Line number of the assert
//| Returns:
//|	TRUE					if there is an assert else FALSE is returned
//*---------------------------------------------------------------------------------
BOOL INTFUN DoAssertCheck(BOOL fCond, LPTSTR szMsg, LPTSTR szFile, int iLine)
{
	TCHAR tmp[200];

	if(!fCond)
		return FALSE;

	wsprintf((LPTSTR)tmp, szAssertion,
					(LPTSTR)szMsg, (LPTSTR)szFile, iLine);
	GtrMessageBox(GetActiveWindow(), (LPTSTR)tmp,
					szErr, MB_ICONEXCLAMATION | MB_OK);

	return(TRUE);

}
#endif


//*---------------------------------------------------------------------------------
//| DoGtrGetMemory:
//|	This function allocates the specified amount of memory.  Note that the error
//|	messages are hard coded since they are only used when memory is very low and
//|	a LoadString could fail under such circumstances.
//| Parms:
//|	in			size						How much memory
//| Returns:
//|	Long pointer to void
//*---------------------------------------------------------------------------------
void * EXTFUNCDECL DoGtrGetMemory(SIZE_T size)
{
	LPVOID	tmp;
	const TCHAR	szOutOfMemory[] = TEXT("Error allocating memory.  Please close other applications and try your request again.");
	const TCHAR	szError[] = TEXT("Out of Memory");

	//Make size an even number
	if (size % 2)
		size+=1;

   //Bug #480 Changed GPTR to GHND
   tmp = GlobalAllocPtr(GHND, size);
   if(!tmp) 						// Serious error allocating memory
   	GtrMessageBox(GetActiveWindow(),
   				(LPTSTR)szOutOfMemory,
   				(LPTSTR)szError,
   				MB_ICONHAND | MB_SYSTEMMODAL);

   return tmp;
}


//
// Following only used in debug mode
//
#ifdef _DEBUG
void INTFUN DbgReleaseMemory(LPVOID ptr, LPTSTR szFile, UINT iLine)
{
	if(!ptr) {
		wsprintf((LPTSTR)szOutBuff,
				szFreePtrAssert,
				(LPTSTR)szFile, iLine);
		GtrMessageBox(GetActiveWindow(), (LPTSTR)szOutBuff,
					TEXT("error"), MB_ICONEXCLAMATION | MB_OK);
		}
	else
		GlobalFreePtr(ptr);
}
#endif



//*------------------------------------------------------------------------
//|  TruncateEditContents:
//|		Truncate the first 2000 chars of an edit control.
//|
//|	Parms:
//|		hwnd			Edit window to truncate
//|	Returns:
//|		TRUE if successful, FALSE on a failure
//*------------------------------------------------------------------------
VOID INTFUN TruncateEditContents(HWND hwnd)
{
	DWORD		len;
	TCHAR		szMsg[100];

	SendMessage(hwnd, EM_SETSEL, PORT_EM_SETSEL(0,2000));
	GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), idsTruncatedEditWindow);
	SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)(LPTSTR)szMsg);
	len = (DWORD)SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0L);
	SendMessage(hwnd, EM_SETSEL, PORT_EM_SETSEL(len, len));
}


//*------------------------------------------------------------------------
//|  DoszWrite:
//|		Output to the specified edit window.  Checks are done for
//|		truncation, in which case the first 2000 TCHARs are removed.
//|		This function will perform buffering of output, waiting until
//|		a new-line character is found or we run out of buffer space
//|		before writing the output.  This reduces flicker.  Note that
//|		since this buffer is stored locally, one should always flush all
//|		user output before calling with another window handle.
//|
//|	Parms:
//|		lpUsrOpts	User options
//|		hwnd			Edit window to write to
//|		str			String to write
//|	Returns:
//|		TRUE if successful, FALSE on a failure
//*------------------------------------------------------------------------
BOOL INTFUN DoszWrite2(HWND hwnd, LPTSTR str)
{
#define MAXFLUSH		500
	WORD				newlen=(WORD)lstrlen(str);
	DWORD				dwTot;
	static TCHAR	szBuff[MAXFLUSH];
	static WORD		cBuff;
	static WORD		udExtra=sizeof(TCHAR)*2;

	dwTot = newlen + cBuff + udExtra;

	// If we are finished with this line or the line is getting too large
	//   for our buffer, then flush it to the screen
	if(_tcschr(str, TEXT('\n')) || dwTot >= MAXFLUSH)
	{
		// Check for truncation, then write the string
		WORD		oldlen=(WORD)GetTrueWindowTextLength(hwnd,-1);

		if((newlen + (WORD)oldlen + cBuff + udExtra) >= MAX_EDIT_WINDOW_TEXT)
			TruncateEditContents(hwnd);

		// Set the cursor to the end of the edit window so that we
		// always append to the end of what currently exists
		SendMessage(hwnd, EM_SETSEL, (WPARAM)-2, (LPARAM)-1);

		// Flush buffer if required.  If FlushBuffer can handle everything, we're done
		if(cBuff)
		{
			if(newlen + cBuff + udExtra < MAXFLUSH)
			{
				lstrcpy(&szBuff[cBuff], str);
				cBuff += (UINT)newlen;
				str = NULL;
			}

			szBuff[cBuff] = TEXT('\0');
			SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)szBuff);
			*szBuff = TEXT('\0');
			cBuff = 0;
			}
		// Either there was no buffered text or we couldn't flush everything, so write string
		if(str)
			SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)str);
		}
	// No need to flush the line yet, so simply add it to our buffer
	else
	{
		_tcsncpy(&szBuff[cBuff], str, (MAXFLUSH-cBuff));
		cBuff += newlen;
	}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| WriteLogInfo:
//|	This function takes a buffer and writes it to the specified user
//|	log.
//| Parms:
//|	szLogFile				The name of the file to append to
//|	str						The string to dump
//| Returns:
//|	LOG_CONTINUE			No error, continue logging
//|	LOG_ERROR_CONTINUE	Error occurs but user wants to continue logging
//|	LOG_STOP					Error and user wants to quit logging
//*---------------------------------------------------------------------------------
SWORD INTFUN WriteLogInfo(HWND hwndIn,LPTSTR szLogFile, LPTSTR str)
{
	HANDLE		hf=0L;
	SWORD			swErrCode=LOG_CONTINUE;
	UINT			fNoIds=0;
	DWORD			dwAttrib=OPEN_EXISTING;

	if ((hf=GtrOpenFile(hwndIn,szLogFile,dwAttrib,
		WRITETOFILE,fNoIds)) == INVALID_HANDLE_VALUE)
	{
		TCHAR szTmpBuff[MAXOUTBUFF];
		if(IDNO == szMessageBox(GetActiveWindow(),
							MB_ICONEXCLAMATION,
							NULL,
							GetGtrString(szTmpBuff, MAXOUTBUFF, idsLogFileNotValid),
							szLogFile))
		{
			//Stop Logging
			swErrCode = LOG_STOP;
		}

		swErrCode = LOG_ERROR_CONTINUE;
	}
	else
	{
		GtrSeekFile(hf,0L,2);
		GtrWriteFile(hwndIn,hf, str, (UWORD)lstrlen(str));  			// Write wherever we are
		GtrCloseFile(hf);
	}

	return(swErrCode);

}



//*------------------------------------------------------------------------
//|  szPrintf:
//|		Works like printf but will redirect your output to an
//|		edit window and/or logfile.
//|  Parms:
//|		lpUsrOpts		User options
//|		hwnd				The edit window to write to
//|		uSev				Severity of this message
//|		fCheckLog		TRUE if this message should be logged
//|		fComment			TRUE if this string will be logged as a comment
//|		szFmt				Format string
//|		(varying)		Arguements for format string
//|  Returns:
//|		TRUE if everything was successful, FALSE if there was an error
//*------------------------------------------------------------------------
UINT CDECL szPrintf(lpUSROPTIONS lpUsrOpts, HWND hwnd, UINT uSev,
			BOOL fCheckLog, BOOL fFormat, LPTSTR szFmt, ...)
{
#define MAXOUTPUTBUFF 2000
	static TCHAR	szBuffer[MAXOUTPUTBUFF];
	va_list			marker;
	UINT				rtn=0;

	// Check request logging level before actually logging information
	if(uSev > lpUsrOpts->uLogLevel)
		return TRUE;

	// Use format and arguements as input
	if(fFormat){
		va_start(marker, szFmt);
		_vsntprintf((LPTSTR)szBuffer, MAXOUTPUTBUFF, szFmt, marker);
		va_end(marker);
	}

	// Write to the edit window
	rtn = DoszWrite2(hwnd, (fFormat) ? (LPTSTR)szBuffer : szFmt);

	// If this string should be logged, verify we are tracing at this time
	if(fCheckLog && lpUsrOpts->fTrace)
	{
		SWORD lrtn;
		lrtn = WriteLogInfo(hwnd,lpUsrOpts->szLog, (fFormat) ? (LPTSTR)szBuffer : szFmt);
		if (lrtn == LOG_STOP)
		{
			//Stop tracing
			lpUsrOpts->fTrace = FALSE;
			return FALSE;
		}
		else
			return (lrtn & rtn);
	}

	return rtn;
}


//*---------------------------------------------------------------------------------
//| fBusy:
//|	Call this function to set the cursor when doing long work items.
//|
//|	Note:  Because this function is saving the old cursor in the DSEG of this
//|			 DLL, it would be possible to lose a custom cursor.  Therefore this
//|			 function should only be used to display either the hourglass or
//|			 the standard arrow cursor.
//| Parms:
//|	flag							TRUE to display HourGlass, FALSE for arrow
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
void INTFUN fBusy(BOOL flag)
{
	static HCURSOR	hCursor;						// Save old cursor handle

	ShowCursor(FALSE);
	if(flag)
		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	else
		SetCursor(hCursor);
	ShowCursor(TRUE);
}


//*------------------------------------------------------------------------
//|  GetGtrString:
//|		This function will load a resource string from our resource
//|		fork and return a pointer to the buffer.  Note that if
//|		LoadString fails, you will pull an assert in the debug version
//|		and a short string will be returned.  The short string is used
//|		to avoid overwriting user's buffer and confusing any sprintf
//|		like function that may be using the return value.
//|	Parms:
//|		buf		Output buffer for the string being retrieved
//|		cbbuf		How large is the buffer to retrieve
//|		ids		Which string do you want?
//|	Returns:
//|		Pointer to the buffer if successful, NULL for an error.
//*------------------------------------------------------------------------
LPTSTR INTFUN GetGtrString(LPTSTR buf, int cbbuf, UINT ids)
{
	ASSERTCHECK(!hInst, szInvalidInstance);
	if(!hInst)
		return NULL;

	if(!GtrLoadString(hInst, ids, buf, cbbuf/sizeof(TCHAR))) {
		ONDEBUG(TCHAR		szBuff[MAXOUTBUFF]);
		ONDEBUG(wsprintf(szBuff, szStringNotFound, ids));
		ASSERTCHECK(TRUE, szBuff);
		return (LPTSTR)szNOTFOUND;
		}

	return buf;
}




//*------------------------------------------------------------------------
//|  GetDSNs:
//|		This function will fill a combo box with by using SQLDataSources.
//|	Parms:
//|		hwnd		Where should the entries be added
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN GetDSNs(HWND hwnd)
{
	HENV		henv;
	RETCODE	rc;
	TCHAR		szDSN[SQL_MAX_DSN_LENGTH + 1];

	if(SQL_SUCCESS != SQLAllocEnv(&henv))
		return;

	rc = SQLDataSources(henv, SQL_FETCH_FIRST,
				szDSN, sizeof(szDSN)/sizeof(szDSN[0]), NULL,
				NULL, 0, NULL);
	while(RC_SUCCESSFUL(rc)) {
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szDSN);
		rc = SQLDataSources(henv, SQL_FETCH_NEXT,
					szDSN, sizeof(szDSN)/sizeof(szDSN[0]), NULL,
					NULL, 0, NULL);
		}

	SQLFreeEnv(henv);
}



//*------------------------------------------------------------------------
//|  CreateEditFont:
//|		This function will return the handle of a small font which can
//|		be used for edit windows, status bars, etc...
//|	Parms:
//|		None.
//|	Returns:
//|		Handle to font if successful, NULL otherwise
//*------------------------------------------------------------------------
HFONT INTFUN CreateEditFont()
{
	LOGFONT		lf;


	InitFont(&lf,(LPTSTR)lpUsrOptions->szFont);
	lf.lfHeight = lpUsrOptions->iFontHeight;
	lf.lfWeight = FW_THIN;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	return CreateFontIndirect(&lf);

}


//*------------------------------------------------------------------------
//|  CreateColumnFont:
//|		Create the bold font that will be used in a rowset window.
//|	Parms:
//|		None.
//|	Returns:
//|		Handle to font if successful, NULL otherwise
//*------------------------------------------------------------------------
HFONT INTFUN CreateColumnFont()
{
	LOGFONT		lf;

	InitFont(&lf,(LPTSTR)lpUsrOptions->szFont);
	lf.lfHeight = lpUsrOptions->iFontHeight;
	lf.lfWeight = FW_BOLD;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	return CreateFontIndirect(&lf);
}


//*---------------------------------------------------------------------------------
//| Draw3DEdges:
//|	This function will give a rectangle a 3d affect by adding a light line
//|	to the top and a dark line at the bottom.
//| Parms:
//|	hwnd						The window handle
//|	hdc						Device context to draw in
//|	lpRect					Pointer to the rectangle to make 3d
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN Draw3DEdges(HWND hwnd, HDC hdc, RECT * lpRect)
{
	HPEN		hdPen, hlPen, hOldPen;
	int		iPenWidth = GetSystemMetrics(SM_CYBORDER);
	HBRUSH	hGryBrush;

	// Fill background as gray
	hGryBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	FillRect(hdc, lpRect, hGryBrush);

	// Create a dark pen for the bottom
	hdPen = CreatePen(PS_SOLID, iPenWidth, GetSysColor(COLOR_BTNSHADOW));
	hOldPen = SelectObject(hdc, hdPen);
	MoveTo(hdc, lpRect->left, lpRect->bottom - 1);
	LineTo(hdc, lpRect->right, lpRect->bottom - 1);

	// Create a light pen for the right and bottom borders
	hlPen = CreatePen(PS_SOLID, iPenWidth, GetSysColor(COLOR_BTNHIGHLIGHT));
	SelectObject(hdc, hlPen);
	MoveTo(hdc, lpRect->left, lpRect->top);
	LineTo(hdc, lpRect->right, lpRect->top);

	SelectObject(hdc, hOldPen);
	DeleteObject(hdPen);
	DeleteObject(hlPen);
	DeleteObject(hGryBrush);
}


//*---------------------------------------------------------------------------------
//| RefreshWindow:
//|	Invalidate the client area of a window to force a repaint.
//| Parms:
//|	hwnd						The window to force repaint
//| Returns:
//|	The index value or -1 for an invalid function
//*---------------------------------------------------------------------------------
VOID INTFUN RefreshWindow(HWND hwnd)
{
	RECT			rect;

	GetClientRect(hwnd, &rect);
	InvalidateRect(hwnd, &rect, TRUE);
}


//*---------------------------------------------------------------------------------
//| GetSqlFuncOffset:
//|	Given an ODBC function id (SQL_API_xxx as found in SQLEXT.H), this function
//|	returns a hashed index value guaranteed to be unique to that function.
//| Parms:
//|	funcid					The function id
//| Returns:
//|	The index value or -1 for an invalid function
//*---------------------------------------------------------------------------------

int INTFUN GetSqlFuncOffset(UWORD funcid)
{
	int nDex;

	for (nDex= 0; nDex < GetNumItemsInArray(rgApiList); nDex++)
		{
		if (funcid == rgApiList[nDex].iAPINum)
			return (rgApiList[nDex].iOrdinalNum-1);
		}

	return(-1);

}

//*---------------------------------------------------------------------------------
//| GetAPINumIndex:
//|	Given an ODBC function id (SQL_API_xxx as found in SQLEXT.H), this function
//|	returns a hashed index value guaranteed to be unique to that function.
//| Parms:
//|	funcid					The function id
//| Returns:
//|	The index value or -1 for an invalid function
//*---------------------------------------------------------------------------------

int INTFUN GetAPINumIndex(UWORD funcid)
{
	int nDex;

	for (nDex= 0; nDex < NUM_APIS; nDex++)
		{
		if (funcid == rgApiList[nDex].iAPINum)
			return (nDex);
		}

	return(-1);

}


//*---------------------------------------------------------------------------------
//| GetApiNum:
//|	Given the hashed index (found by GetSqlFuncOffset), this function returns
//|	the SQL_API_xxx number.
//| Parms:
//|	dex						The 0 based index
//| Returns:
//|	The SQL_API_xxx number or -1 on error
//*---------------------------------------------------------------------------------

int INTFUN GetApiNum(UWORD dex)
{
	if(dex > GetNumItemsInArray(rgApiList))
		return(-1);
	else
		return (rgApiList[dex].iAPINum);

}

//*---------------------------------------------------------------------------------
//| IsODBC30Api:
//|	Given the SQL_API_XXX number determine if it is a 3.0 or 2.X function
//| Parms:
//|	nApiNum		SQL_API_XXX number
//| Returns:
//|	TRUE		if 3.0 API otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN IsODBC30Api(UWORD nAPINum)
{
	UWORD dex=0;

	if(dex > GetNumItemsInArray(rgApiList))
		return(-1);
	else
	{
		dex=(UWORD)GetAPINumIndex(nAPINum);

		return (rgApiList[dex].fODBC30);
	}

} //IsODBC30Api()


//*---------------------------------------------------------------------------------
//| GetRetCodeName:
//|	Given a return code, return the character name, formatted with value.
//| Parms:
//|	rc							The return code
//|	str						User string
//| Returns:
//|	Pointer to user buffer
//*---------------------------------------------------------------------------------
LPTSTR GetRetCodeName(RETCODE rc, LPTSTR str)
{
	UINT dex;

	for(dex=0;  dex<ciSqlCodes;  dex++)
		if(rc == lpdftSqlCodes[dex].fOpt)
			return GetExpandedConstant(str, &lpdftSqlCodes[dex], FALSE);

	wsprintf(str, szInvalidRetcode, rc);
	return str;
}

//*---------------------------------------------------------------------------------
//| GetInstallerRetCodeName:
//|	Given a return code, return the character name, formatted with value.
//| Parms:
//|	fRc						The return code
//|	str						User string
//| Returns:
//|	Pointer to user buffer
//*---------------------------------------------------------------------------------
LPTSTR GetInstallerRetCodeName(BOOL fRc, LPTSTR str)
{
	UINT dex;

	for(dex=0;  dex < ciTrueFalse;  dex++)
		if(fRc == lpdftTrueFalse[dex].fOpt)
			return GetExpandedConstant(str, &lpdftTrueFalse[dex], FALSE);

	wsprintf(str, szInvalidRetcode, fRc);
	return str;
}

//*---------------------------------------------------------------------------------
//| GetInstallerRetCodeName30:
//|	Given a return code, return the character name, formatted with value.
//| Parms:
//|	rc							The return code
//|	str						User string
//| Returns:
//|	Pointer to user buffer
//*---------------------------------------------------------------------------------
LPTSTR GetInstallerRetCodeName30(RETCODE rc, LPTSTR str)
{
	UINT dex;

	for(dex=0;  dex < ciInstallerErrors;  dex++)
		if(rc == lpdftInstallerErrors[dex].fOpt)
			return GetExpandedConstant(str, &lpdftInstallerErrors[dex], FALSE);

	wsprintf(str, szInvalidRetcode, rc);
	return str;
}

//*---------------------------------------------------------------------------------
//| fAbortCheck:
//|	This function will detect the Escape key being depressed and will cancel
//|	the current request.
//| Parms:
//|	hwndOut						Output window to write any messages to
//| Returns:
//|	TRUE if an abort should take place, FALSE if not
//*---------------------------------------------------------------------------------
BOOL INTFUN fAbortCheck(HWND hwndOut)
{
	BOOL 			fAbort = FALSE;
	MSG 			msg;
	TCHAR			szBuff[DFTMSGBUFF];

	// Examine the message queue for the escape key
	while(PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		if(msg.message==WM_KEYDOWN)
			switch (msg.wParam)
				{
				case VK_ESCAPE: 	// break out of the current loop
					fAbort = TRUE;
					while(GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST) &&
						  msg.message != WM_KEYUP)
						;
					break;

				case VK_PAUSE:
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
							FALSE, FALSE, GetGtrString(szBuff,
                                                       sizeof(szBuff)/sizeof(szBuff[0]), idsPressKeyToCont));
					while(GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST) &&
							msg.message != WM_KEYDOWN)
						;
					fAbort = msg.wParam == VK_ESCAPE;
					break;
				}

	// Tell user we have aborted
	if(fAbort)
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0,
				FALSE, FALSE, GetGtrString(szBuff,
                                           sizeof(szBuff)/sizeof(szBuff[0]), idsAborted));

	return fAbort;
}


//*---------------------------------------------------------------------------------
//| InvokeHelp:
//|	Load the specified help file and return the success status.  If there was
//|	an error, then tell the user.
//| Parms:
//|	hwnd						Parent window for the help file
//|	szHelpFile				Fully qualified help file name
//|	szKeyword				Optional keyword to search for
//|									Specify NULL if contents are to be displayed
//| Returns:
//|	TRUE if an abort should take place, FALSE if not
//*---------------------------------------------------------------------------------
/*int INTFUN InvokeHelp(HWND hwnd, LPCTSTR pszHelpFile, LPCTSTR pszKeyword)
{
    HWND hHelp = NULL;
    int iRet = 1;

    if(pszKeyword)
    {
        //HtlpHelp - bring up HTMLHELP index and look for the following keyword
        hHelp = HtmlHelp(NULL, pszHelpFile, HH_DISPLAY_INDEX, (DWORD_PTR)pszKeyword);
    }
    else
    {
        //HtmlHelp - otherwise just display main help topic
        hHelp = HtmlHelp(NULL, pszHelpFile, HH_DISPLAY_TOC, (DWORD_PTR)NULL);
    }

    if(hHelp == NULL)
    {
        //Display Error...
        szMessageBox(hwnd,
                    MB_ICONINFORMATION | MB_OK,
                    NULL,
                    GetGtrString(szOutBuff, MAXOUTBUFF, idsLoadHelpFailed),
                    (LPTSTR)pszHelpFile);
        iRet = 0;
    }

    return iRet;
}
*/

//*---------------------------------------------------------------------------------
//| CreateEditWindow:
//|	This function will create a child window of type "edit", give it a small
//|	edit font, and its own private heap so that the maximum amount of text
//|	can be used.
//| Parms:
//|	hwndParent				Parent window handle
//|	hInst						For use in CreateWindow call
//|	id							Child window id
//|	uStyle					Add'l style for the window, beyond defaults
//|	x							horizontal position of window
//|	y							vertical position of window
//|	nWidth					window width
//|	nHeight					window height
//| Returns:
//|	Window handle of the window if successful, NULL on error
//*---------------------------------------------------------------------------------
HWND INTFUN CreateEditWindow(HWND hwndParent, HINSTANCE hInst, UINT id, DWORD uStyle,
    int x,int y,int nWidth,int nHeight)
{
	HWND			hwnd;
	UINT			cTabStop=5;
	DWORD			dwWrap=0;

	if(MF_CHECKED != GetMenuState(hMenu, IDM_EDIT_WRAP, MF_BYCOMMAND))
		dwWrap = ES_AUTOHSCROLL | WS_HSCROLL;

#ifndef WIN32
	{
	HGLOBAL 		hMem;

	// Allocate a global heap for the window
	hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_SHARE, 256L);
	if(!hMem)
		return NULL;
	hwnd = CreateWindow(TEXT("EDIT"),
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_MULTILINE | ES_AUTOVSCROLL | dwWrap | uStyle,
			x,
			y,
			nWidth,
			nHeight,
			hwndParent,
			(HMENU)id,
			hMem,
			NULL);
	}
#else
	// No need for heap manipulation on NT
	hwnd = CreateWindow(TEXT("EDIT"),
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_MULTILINE | ES_AUTOVSCROLL | dwWrap | uStyle,
			x,
			y,
			nWidth,
			nHeight,
			hwndParent,
			(HMENU)(ULONG_PTR)id,
			hInst,
            NULL);
#endif

  	SendMessage(hwnd, WM_SETFONT, (WPARAM)hEditFont, 0L);
	SendMessage(hwnd, EM_SETTABSTOPS, 1, (LPARAM)&cTabStop);
	SendMessage(hwnd, EM_LIMITTEXT, 0, 0);   	// Means ~60000 max under 16-bit, 4GB-1 under Win32

	return hwnd;
}


//*---------------------------------------------------------------------------------
//| ResetEditWindow:
//|	Recreate an edit window with a new font.  Perserve the old contents of the
//|	window.
//| Parms:
//|	hwnd					Window of type 'edit'
//|	hInst					The hinstance to use for create window
//|	id						The child id
//|	x							horizontal position of window
//|	y							vertical position of window
//|	nWidth					window width
//|	nHeight					window height
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN ResetEditWindow(HWND * lphwnd, HINSTANCE hInst,UINT id, DWORD	uStyle,
										int x,int y,int nWidth,int nHeight)
{
	HWND			hwndEdit;
	HWND			hwndParent=GetParent(*lphwnd);
	LPTSTR			str;
	DWORD			len, dwSel;
	RECT			rect;

	hwndEdit = CreateEditWindow(hwndParent, hInst, id, uStyle,x,y,nWidth,nHeight);
	if(hwndEdit)
	{
		len = GetTrueWindowTextLength(*lphwnd,-1);

		if(len+=sizeof(TCHAR))
		{
			str = (LPTSTR)AllocateMemory(len);
			if(str)
			{
				if(SendMessage(*lphwnd, WM_GETTEXT,
							(WPARAM)len, (LPARAM)(LPTSTR)str))
				{
					SendMessage(hwndEdit, EM_REPLACESEL,
							0, (LPARAM)(LPTSTR)str);

					// Retrieve the current selection from old window and
					//	apply it to the new window.
					dwSel = (DWORD)SendMessage(*lphwnd, EM_GETSEL, 0, 0L);
					SendMessage(hwndEdit, EM_SETSEL, 0, dwSel);
				}

				FREE_PTR(str);
			}
		}
	}

	// Free resources
	DestroyWindow(*lphwnd);

	// Now force a repaint of the window
	*lphwnd = hwndEdit;
	GetClientRect(hwndParent, &rect);
	SendMessage(hwndParent, WM_SIZE, 0,
						MAKELONG(rect.right, rect.bottom));
	InvalidateRect(hwndParent, &rect, TRUE);
}



//*---------------------------------------------------------------------------------
//| DoResetWrap:
//|	Change every edit window to use word wrap based on menu status.
//| Parms:
//|	None
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoResetWrap(void)
{
	lpCONNECTIONINFO		lpConn=lpConnWin;
	HWND						hwndFocus=GetFocus();
	int						x=0,
								y=0,
								nWidth=0,
								nHeight=0;

	DORESETEDITWINDOW(hwndFocus, hwndOutWin, IDW_OUTWIN, 0,
		x,y,nWidth,nHeight);

	// Reset the results windows on all connection handles
	while(lpConn)
	{
		// Reset the edit windows
		DORESETEDITWINDOW(hwndFocus,
					lpConn->hwndIn, IDW_TOP, 0,
					x,y,nWidth,nHeight);
		DORESETEDITWINDOW(hwndFocus,
					lpConn->hwndOut, IDW_BOTTOM, WS_BORDER,
					x,y,nWidth,nHeight);

		lpConn = lpConn->next;
		if(lpConn == lpConnWin)
			lpConn = NULL;
	}

	if(hwndFocus)
		SetFocus(hwndFocus);
}



//*---------------------------------------------------------------------------------
//| Hook3d:
//|	This hook function can be used by all of the samples which follow.  It will
//|	trap relevant messages and cause controls to be painted in 3d.
//| Parms:
//|	hDlg							Window handle
//|	msg							Window message
//|	wParam						Standard value
//|	lParam						Standard value
//| Returns:
//|	TRUE if we handled message, FALSE otherwise
//*---------------------------------------------------------------------------------
UINT EXTFUN Hook3d(HWND	hDlg, UINT	msg, WPARAM	wParam, LPARAM	lParam)
{
	if(msg == WM_INITDIALOG)
	{
		CenterDialog(hDlg);
		return TRUE;
	}

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| SaveGetUserOptions:
//|	This function will take the user options structure and write it's contents
//|	to the .INI file, or retrieve depending on the flag passed in.
//| Parms:
//|	lpUsrOpts			The user options structure to populate/save
//|	fSave					TRUE to save the values, FALSE to retrieve
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
#define WRITE_BOOL_VAL(fOpt, szEntry) \
	{ \
	wsprintf((LPTSTR)szTemp, szd, fOpt); \
	GtrWritePrivateProfileString(szSTARTUP, szEntry, (LPTSTR)szTemp, szINIFILE); \
   }
#define WRITE_UWORD_VAL(fOpt, szEntry) \
	{ \
	wsprintf((LPTSTR)szTemp, szu, fOpt); \
	GtrWritePrivateProfileString(szSTARTUP, szEntry, (LPTSTR)szTemp, szINIFILE); \
   }
BOOL INTFUN SaveGetUserOptions(lpUSROPTIONS lpUsrOpts, BOOL fSave)
{
	TCHAR		szTemp[20];
	UINT		cbChar=sizeof(TCHAR);

	if(!fSave)
	{
		lpUsrOpts->fDriverMenus = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szDRVRMENUS, TRUE, szINIFILE);
		lpUsrOpts->fAutoErrors = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szAUTOERRORS, FALSE, szINIFILE);
		lpUsrOpts->fNullChecking = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szNULLCHECKING, TRUE, szINIFILE);
		lpUsrOpts->fBufferChecking = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szBUFFERCHECKING, TRUE, szINIFILE);
		lpUsrOpts->fVersionNumbers = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szVERSIONNUMBERS, TRUE, szINIFILE);
		lpUsrOpts->fUseCursorLib = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szUSECURSORLIB, FALSE, szINIFILE);
		lpUsrOpts->fODBCBehavior = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szODBCBEHAVIOR, IDR_FC_ODBC30, szINIFILE);
		lpUsrOpts->fBindColZero = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szBINDCOLZERO, FALSE, szINIFILE);
		lpUsrOpts->fAutoDisplay = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szAUTODISPLAY, TRUE, szINIFILE);
		lpUsrOpts->uLogLevel = GtrGetPrivateProfileInt(szSTARTUP, szDFTMSGSEV, MSG_SEV2, szINIFILE);
		lpUsrOpts->uDftBuffLen = (UWORD)GtrGetPrivateProfileInt(szSTARTUP, szDFTBUFFLEN, DFT_GATOR_LENGTH, szINIFILE);
		lpUsrOpts->uDftBuffLen*=(UWORD)cbChar;
		lpUsrOpts->uDftParmLen = (UWORD)GtrGetPrivateProfileInt(szSTARTUP, szDFTPARMLEN, DFT_PARM_LENGTH, szINIFILE);
		lpUsrOpts->uDftParmLen*=(UWORD)cbChar;

		// Display information
		lpUsrOpts->fMaximize = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szMAXIMIZED, TRUE, szINIFILE);
		lpUsrOpts->fMinimize = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szMINIMIZED, TRUE, szINIFILE);
		lpUsrOpts->x = (int)GtrGetPrivateProfileInt(szSTARTUP, szLEFT, 0, szINIFILE);
		lpUsrOpts->y = (int)GtrGetPrivateProfileInt(szSTARTUP, szTOP, 0, szINIFILE);
		lpUsrOpts->nWidth = (int)GtrGetPrivateProfileInt(szSTARTUP, szWIDTH, 800, szINIFILE);
		lpUsrOpts->nHeight = (int)GtrGetPrivateProfileInt(szSTARTUP, szHEIGHT, 300, szINIFILE);

		// Display information
		lpUsrOpts->fOutputMaximize = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szOUTPUTMAXIMIZED, TRUE, szINIFILE);
		lpUsrOpts->fOutputMinimize = (BOOL)GtrGetPrivateProfileInt(szSTARTUP, szOUTPUTMINIMIZED, TRUE, szINIFILE);
		lpUsrOpts->xOutput = (int)GtrGetPrivateProfileInt(szSTARTUP, szOUTPUTLEFT, 0, szINIFILE);
		lpUsrOpts->yOutput = (int)GtrGetPrivateProfileInt(szSTARTUP, szOUTPUTTOP, 0, szINIFILE);
		lpUsrOpts->nWidthOutput = (int)GtrGetPrivateProfileInt(szSTARTUP, szOUTPUTWIDTH, 800, szINIFILE);
		lpUsrOpts->nHeightOutput = (int)GtrGetPrivateProfileInt(szSTARTUP, szOUTPUTHEIGHT, 300, szINIFILE);

		//ODBC Version
		lpUsrOpts->nODBCVer= (int)GtrGetPrivateProfileInt(szSTARTUP, szODBCVer, MENU30, szINIFILE);

		GtrGetPrivateProfileString(szSTARTUP, szFONT, (LPTSTR)szDFTFONT,
						(LPTSTR)lpUsrOpts->szFont, LF_FACESIZE, szINIFILE);
		lpUsrOpts->iPtSize = GtrGetPrivateProfileInt(szSTARTUP, szPTSIZE, -9, szINIFILE);
		lpUsrOpts->iFontHeight = GtrGetPrivateProfileInt(szSTARTUP, szFONTHEIGHT, -14, szINIFILE);

		// Rowset information
		lpUsrOpts->fShowStatus = (BOOL)GtrGetPrivateProfileInt(szSTARTUP,
												szSHOWSTATUS, TRUE, szINIFILE);
		lpUsrOpts->fShowPcbs = (BOOL)GtrGetPrivateProfileInt(szSTARTUP,
												szSHOWPCBS, TRUE, szINIFILE);
		lpUsrOpts->fShowColumns = (BOOL)GtrGetPrivateProfileInt(szSTARTUP,
												szSHOWCOLUMNS, TRUE, szINIFILE);
		lpUsrOpts->uReservedRows = (UWORD)GtrGetPrivateProfileInt(szSTARTUP,
												szRESERVEDROWS, 0, szINIFILE);
		lpUsrOpts->uDftBindBuff = (UWORD)GtrGetPrivateProfileInt(szSTARTUP,
												szDFTBINDBUFF, DFT_BIND_BUFF, szINIFILE);

		lpUsrOpts->uDftBindBuff*=(UWORD)cbChar;
		// Misc items
		GtrGetPrivateProfileString(szSTARTUP, szINITDSN, TEXT(""),
						(LPTSTR)lpUsrOpts->szInitialDSN,
						sizeof(lpUsrOpts->szInitialDSN), szINIFILE);
		GtrGetPrivateProfileString(szSTARTUP, szINITUID, TEXT(""),
						(LPTSTR)lpUsrOpts->szInitialUID,
						sizeof(lpUsrOpts->szInitialUID), szINIFILE);
	}
	else
	{
		WRITE_BOOL_VAL(lpUsrOpts->fDriverMenus, szDRVRMENUS);
		WRITE_BOOL_VAL(lpUsrOpts->fAutoErrors, szAUTOERRORS);
		WRITE_BOOL_VAL(lpUsrOpts->fNullChecking, szNULLCHECKING);
		WRITE_BOOL_VAL(lpUsrOpts->fBufferChecking, szBUFFERCHECKING);
		WRITE_BOOL_VAL(lpUsrOpts->fVersionNumbers, szVERSIONNUMBERS);
		WRITE_BOOL_VAL(lpUsrOpts->fUseCursorLib, szUSECURSORLIB);
		WRITE_BOOL_VAL(lpUsrOpts->fODBCBehavior, szODBCBEHAVIOR);
		WRITE_BOOL_VAL(lpUsrOpts->fBindColZero, szBINDCOLZERO);
		WRITE_BOOL_VAL(lpUsrOpts->fAutoDisplay, szAUTODISPLAY);
		WRITE_UWORD_VAL(lpUsrOpts->uLogLevel, szDFTMSGSEV);
		WRITE_UWORD_VAL(lpUsrOpts->uDftBuffLen/cbChar, szDFTBUFFLEN);
		WRITE_UWORD_VAL(lpUsrOpts->uDftParmLen/cbChar, szDFTPARMLEN);

		// Display information
		WRITE_BOOL_VAL(lpUsrOpts->fMaximize,szMAXIMIZED);
		WRITE_BOOL_VAL(lpUsrOpts->fMinimize,szMINIMIZED);
		GtrWritePrivateProfileString(szSTARTUP, szFONT, (LPTSTR)lpUsrOpts->szFont, szINIFILE);
		WRITE_BOOL_VAL(lpUsrOpts->iPtSize,szPTSIZE);
		WRITE_BOOL_VAL(lpUsrOpts->iFontHeight,szFONTHEIGHT);
		WRITE_BOOL_VAL(lpUsrOpts->x,szLEFT);
		WRITE_BOOL_VAL(lpUsrOpts->y,szTOP);
		WRITE_BOOL_VAL(lpUsrOpts->nWidth,szWIDTH);
		WRITE_BOOL_VAL(lpUsrOpts->nHeight,szHEIGHT);

		// Display information for Output window
		WRITE_BOOL_VAL(lpUsrOpts->fOutputMaximize,szOUTPUTMAXIMIZED);
		WRITE_BOOL_VAL(lpUsrOpts->fOutputMinimize,szOUTPUTMINIMIZED);
		WRITE_BOOL_VAL(lpUsrOpts->xOutput,szOUTPUTLEFT);
		WRITE_BOOL_VAL(lpUsrOpts->yOutput,szOUTPUTTOP);
		WRITE_BOOL_VAL(lpUsrOpts->nWidthOutput,szOUTPUTWIDTH);
		WRITE_BOOL_VAL(lpUsrOpts->nHeightOutput,szOUTPUTHEIGHT);

		//ODBC Version
		WRITE_BOOL_VAL(lpUsrOpts->nODBCVer,szODBCVer);

		// Rowset information
		WRITE_UWORD_VAL(lpUsrOpts->uReservedRows, szRESERVEDROWS);
		WRITE_UWORD_VAL(lpUsrOpts->uDftBindBuff/cbChar, szDFTBINDBUFF);
		WRITE_BOOL_VAL(lpUsrOpts->fShowStatus, szSHOWSTATUS);
		WRITE_BOOL_VAL(lpUsrOpts->fShowPcbs, szSHOWPCBS);
		WRITE_BOOL_VAL(lpUsrOpts->fShowColumns, szSHOWCOLUMNS);

		// Misc items
		GtrWritePrivateProfileString(szSTARTUP, szINITDSN,
						(LPTSTR)lpUsrOpts->szInitialDSN, szINIFILE);
		GtrWritePrivateProfileString(szSTARTUP, szINITUID,
						(LPTSTR)lpUsrOpts->szInitialUID, szINIFILE);
	}

	return TRUE;
}

//Bug #361
//*---------------------------------------------------------------------------------
//| IsVarLengthDataType:
//|	This function returns TRUE if the datatype has variable length data and
//|	FALSE otherwise.
//| Parms:
//|	fCType	Data Type
//| Returns:
//|	This function returns TRUE if the datatype has variable length data and
//|	FALSE otherwise.
//*---------------------------------------------------------------------------------

BOOL INTFUN IsVarLengthDataType(SWORD fCType)
{
	if (fCType == SQL_C_CHAR ||
		 fCType == SQL_C_WCHAR ||
		 fCType == SQL_C_BINARY ||
		 fCType == SQL_C_DEFAULT)
		return TRUE;

	return FALSE;

} //IsVarLengthData()


//*---------------------------------------------------------------------------------
//| AddNodeToToolBar:
//|	This function adds a new node to the mini-tool bar of henvs or hstmts
//| Parms:
//|	cNum		Current Handle number (count)
//|	handle	handle to be added to mini-toolbar and displayed
//|	hstmt
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN AddNodeToToolBar(UINT cNum,HANDLE handle,HWND hwnd, LPTSTR szHandle)
{
	TCHAR 		szAddress[75]=TEXT("");
	DWORD		dwIndex=0;

#ifndef WIN32
	wsprintf(szAddress, TEXT("%s %u:  0x%04X:0x%04X"), (LPSTR)szHandle,cNum, HIWORD(handle),LOWORD(handle));
#else
    {
    int cchLen;
	cchLen = wsprintf(szAddress, TEXT("%s %u:  0x%08p"), szHandle,cNum, handle);
    if (cchLen > 8 && szAddress[cchLen - 1] == 'p') {
        wsprintf(szAddress, TEXT("%s %u:  0x%08X"), szHandle,cNum, (unsigned int)handle);
    }
    }
#endif

	dwIndex = (DWORD)SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szAddress);
	if(dwIndex != CB_ERRSPACE)
		SendMessage(hwnd, CB_SETCURSEL, (WPARAM)dwIndex, 0L);

	// Refresh the toolbar so that the correct buttons get enabled
   RefreshToolbar();
}


//*---------------------------------------------------------------------------------
//| DropNodeFromToolBar:
//|	This function drops an existing node from the mini-tool bar of henvs or hstmts
//| Parms:
//|	hwnd		window handle to be dropped
//|	dwInd		index of handle to be deleted from the specified combo box
//| Returns:
//|	dwIndex	index (node number) to be deleted from linked list
//*---------------------------------------------------------------------------------

DWORD INTFUN DropNodeFromToolBar(HWND hwnd,DWORD dwInd)
{
	DWORD	dwIndex = dwInd;

	//if dwIndex == 0 then just delete the currently selected
	//handle otherwise use the value passed in (dwInd) as the
	//index of the handle to be deleted.
	if (!dwIndex)
		dwIndex=(DWORD)SendMessage(hwnd, CB_GETCURSEL, 0, 0L);

	//delete handle from "Drop Down"
	SendMessage(hwnd, CB_DELETESTRING, (WPARAM)dwIndex, 0L);

	return(dwIndex);

} //DropNodeFromToolBar()


//*---------------------------------------------------------------------------------
//| FindIndexInDropDown:
//|	This function finds an address in the specified "Drop Down"
//| Parms:
//|	hwnd				window handle of "Drop Down"
//|	szHandleName	"HENV","HSTMT","HDBC","HDESC"
//| Returns:
//|	dwIndex	index (node number) to be deleted from linked list
//*---------------------------------------------------------------------------------
DWORD	INTFUN FindIndexInDropDown(HWND hwnd,HANDLE handle,UINT cNum,LPTSTR szHandle)
{
	TCHAR 	szAddress[75];
	SDWORD	dwIndex=0;

#ifndef WIN32
	wsprintf(szAddress, TEXT("%s %u:  0x%04X:0x%04X"), (LPSTR)szHandle,cNum, HIWORD(handle),LOWORD(handle));
#else
    {
    int cchLen;
	cchLen = wsprintf(szAddress, TEXT("%s %u:  0x%08p"), szHandle,cNum, handle);
    if (cchLen > 8 && szAddress[cchLen - 1] == 'p') {
        wsprintf(szAddress, TEXT("%s %u:  0x%08X"), szHandle,cNum, (unsigned int)handle);
    }
    }
#endif

	dwIndex=(DWORD)SendMessage(hwnd, CB_FINDSTRING, (WPARAM)-1, (LPARAM)szAddress);

	return((dwIndex == CB_ERR ? 0 : dwIndex));

} //FindIndexInDropDown()





//*---------------------------------------------------------------------------------
//| FindComboRowHeight:
//|	Given a combo-box window handle, figure out how high each row in the
//|	drop down will be.  We only do this once.  The height is based on the
//|	font used in the combo-box.  If selectable fonts are ever used, this will
//|	have to be adjusted.
//| Parms:
//|	hwnd							The window handle of a combo-box
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN FindComboRowHeight(HWND hwnd)
{
	TEXTMETRIC	tm;
	HDC			hdc;

	if(!giStmtHeight)
	{
		if(hdc = GetDC(hwnd))
		{
			GetTextMetrics(hdc, &tm);
			giStmtHeight = (UINT)tm.tmHeight;
			ReleaseDC(hwnd, hdc);
		}
	}

}


//*---------------------------------------------------------------------------------
//| DisplayErrors30:
//|		This function displayes the Error Messages returned by SQLGetDiagRec and
//|	displays them in either a MessageBox or prints them.
//| Parms:
//|	hwnd			Parent Window Handle
//|	HandleType	SQL_HANDLE_ENV, etc.
//|	Handle		henv,hdbc,etc.
//|	idsTitle		ids string of title
//| Returns:
//|	TRUE if there were errors,otherwise, FALSE
//*---------------------------------------------------------------------------------

BOOL CheckErrors30(HWND hwnd,SQLSMALLINT HandleType,SQLHANDLE Handle,UINT idsTitle,
						 BOOL fMsgBox)
{
	SQLSMALLINT	RecNumber=0;
	TCHAR			szState[TINYBUFF]=TEXT("");
	TCHAR			szMsg[SQL_MAX_MESSAGE_LENGTH]=TEXT("");
	SWORD 		cbErrorMsg;
	RETCODE		rc=SQL_SUCCESS;
	TCHAR			szTitle[LARGEBUFF]=TEXT("");
	SQLINTEGER	nError=0;
	BOOL			fErrors=FALSE;

	if (idsTitle)
		GetGtrString(szTitle, sizeof(szTitle)/sizeof(szTitle[0]), idsTitle);
	else
		lstrcpy(szTitle,TEXT("SQLGetDiagRec"));


	while (RC_SUCCESSFUL(rc))
	{
		rc = SQLGetDiagRec(HandleType,Handle, RecNumber++,szState, &nError, szMsg,
			tsizeof(szMsg), &cbErrorMsg);

		if(RC_NOTSUCCESSFUL(rc) || lstrcmp(szState, TEXT("IM001")) != 0)
		{
			fErrors=TRUE;

			if (!fMsgBox)
				szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsSQLErrorMessage),
							(LPTSTR)TEXT("dbc"), (LPTSTR)szState, nError, cbErrorMsg);
		}

		if (!Handle)
			lstrcpy(szMsg,TEXT("Invalid Handle"));

		if (fMsgBox)
			szMessageBox(hwnd,MB_ICONEXCLAMATION | MB_OK,szTitle,szMsg);
		else
			szPrintf(lpUsrOptions, hwnd, MSG_SEV0,TRUE, TRUE,
				(LPTSTR)szErrorMsgStr, (LPTSTR)szMsg);
	}

	return(fErrors);

} // CheckErrors30()



//*---------------------------------------------------------------------------------
//| DisplayErrors30:
//|		This function displayes the Error Messages returned by SQLGetDiagRec and
//|	displays them in either a MessageBox or prints them.
//| Parms:
//|	hwnd		Window handle of the output window
//|	henv		Environment Handle
//|	hdbc		Connection Handle
//|	fMsgBox	TRUE if caller wants the error displayed in a
//|				Message Dialog Box
//|	szTitle	Title for Message Box
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN DisplayErrors30(HWND hwnd,HENV henv,HDBC hdbc,BOOL fMsgBox,
	int idsTitle)
{

	BOOL		fErrors=FALSE;

	//Check Errors on HENV
	fErrors += CheckErrors30(hwnd,SQL_HANDLE_ENV,henv,idsTitle,fMsgBox);
	fErrors += CheckErrors30(hwnd,SQL_HANDLE_DBC,hdbc,idsTitle,fMsgBox);

	return(fErrors);

} //DisplayErrors30()



//*---------------------------------------------------------------------------------
//| DisplayErrors:
//|		This function displayes the Error Messages returned by SQLError and
//|	displays them in either a MessageBox or prints them.
//| Parms:
//|	hwnd		Window handle of the output window
//|	henv		Environment Handle
//|	hdbc		Connection Handle
//|	fMsgBox	TRUE if caller wants the error displayed in a
//|				Message Dialog Box
//|	szTitle	Title for Message Box
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN DisplayErrors(HWND hwnd,HENV henv,HDBC hdbc,BOOL fMsgBox,
	int idsTitle)
{

	TCHAR		szState[TINYBUFF];
	SDWORD	fError=0;
	TCHAR		szMsg[SQL_MAX_MESSAGE_LENGTH]=TEXT("");
	SWORD 	cbErrorMsg;
	RETCODE	rc=SQL_SUCCESS;
	TCHAR		szTitle[LARGEBUFF]=TEXT("");
	BOOL		fErrors=FALSE;

	if (idsTitle)
		GetGtrString(szTitle, sizeof(szTitle)/sizeof(szTitle[0]), idsTitle);
	else
		lstrcpy(szTitle,TEXT("SQLError"));

	while (RC_SUCCESSFUL(rc))
		{
		rc = SQLError(henv, hdbc, NULL,szState, &fError, szMsg,
			sizeof(szMsg)/sizeof(szMsg[0]), &cbErrorMsg);

		if(RC_NOTSUCCESSFUL(rc) || lstrcmp(szState, TEXT("IM001")) != 0)
			{
			fErrors=TRUE;

			if (!fMsgBox)
				szPrintf(lpUsrOptions, hwnd, MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsSQLErrorMessage),
							(LPTSTR)TEXT("dbc"), (LPTSTR)szState, fError, cbErrorMsg);
			}

		if (!henv)
			lstrcpy(szMsg,TEXT("Invalid Handle"));

		if (fMsgBox)
			szMessageBox(hwnd,MB_ICONEXCLAMATION | MB_OK,szTitle,szMsg);
		else
			szPrintf(lpUsrOptions, hwnd, MSG_SEV0,TRUE, TRUE,
				(LPTSTR)szErrorMsgStr, (LPTSTR)szMsg);
		}

	return(fErrors);

} //DisplayErrors()


//*---------------------------------------------------------------------------------
//| HandleToText:
//|		This function formats (i.e. creates a text string) a handle
//| Parms:
//|	szAddress	Buffer to hold formatted string
//|	handle		handle to be formatted
//| Returns:
//|	szAddress	Buffer to hold formatted string
//*---------------------------------------------------------------------------------

LPTSTR INTFUN HandleToText(LPTSTR szAddress,SQLHANDLE handle)
{
	//create handle string
#ifndef WIN32
	wsprintf(szAddress, TEXT("0x%04X:0x%04X"), HIWORD(handle),LOWORD(handle));
#else
    {
    int cchLen;
	cchLen = wsprintf(szAddress, TEXT("0x%08p"),handle);
    if (cchLen > 2 && szAddress[cchLen - 1] == 'p') {
        wsprintf(szAddress, TEXT("0x%08X"), (unsigned int)handle);
    }
    }
#endif

	return(szAddress);


} //HandleToText()


//*---------------------------------------------------------------------------------
//| TextToHandle:
//|		This function converts a string containing a handle to it's binary
//|	representation
//| Parms:
//|	szAddress	Buffer to hold formatted string
//|	handle		handle to be formatted
//| Returns:
//|	return value from sscanf
//*---------------------------------------------------------------------------------

INT INTFUN TextToHandle(LPTSTR szAddress,SQLHANDLE *phandle)
{
    INT nLen;
	//create handle string
#ifndef WIN32
	nLen = _stscanf(szAddress, TEXT("0x%04X:0x%04X"), HIWORD(*phandle),LOWORD(*phandle));
#else
	nLen = _stscanf(szAddress, TEXT("%p"),phandle);
#endif

    return nLen;
} //HandleToText()


//*---------------------------------------------------------------------------------
//| IsCharData:
//|		This function returns TRUE if the fCType signifies that the data is
//|	a string type which is NULL terminated (i.e. SQL_C_CHAR)
//| Parms:
//|	fCType		C Type of data
//| Returns:
//|	TRUE	if string date otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN IsCharData(SWORD fCType)
{

	switch(fCType)
	{
		case SQL_C_CHAR:
		case SQL_C_WCHAR:
			return(TRUE);
	}

	return(FALSE);

} //IsCharData()


//*---------------------------------------------------------------------------------
//| IsVarLenData:
//|		This function returns TRUE if the fCType signifies that the data is
//|	a varable length type which is NULL terminated (i.e. SQL_C_CHAR,SQL_C_BINARY)
//| Parms:
//|	fCType		C Type of data
//| Returns:
//|	TRUE	if string date otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN IsVarLenData(SWORD fCType)
{

	switch(fCType)
	{
		case SQL_C_CHAR:
		case SQL_C_WCHAR:
		case SQL_C_BINARY:
			return(TRUE);
	}

	return(FALSE);

} //IsCharData()


//*---------------------------------------------------------------------------------
//| IsSigned:
//|		This function returns TRUE if str has a sign as the first character
//|	(i.e. "-12")
//| Parms:
//|	str		string to process
//|	pfPos		Is sign Positive
//| Returns:
//|	TRUE	if signed otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN IsSigned(LPTSTR str,BOOL *pfPos)
{
	BOOL	fPos=_tcschr(str,TEXT('+')) != NULL,
			fNeg=_tcschr(str,TEXT('-')) != NULL;

	//_tcschr is the UNICODE/ANSI generic routine for strchr
	if (fPos || fNeg)
	{
		//only set *pfPos if there is a '+'
		if (fPos)
			*pfPos=fPos;
		else if (fNeg)
			*pfPos=FALSE;

		return(TRUE);
	}

	return (FALSE);

} //IsSigned()


//*---------------------------------------------------------------------------------
//| GetTrueWindowTextLength:
//|		This function returns the length in bytes of the string, this function
//|	double the length if we are running unicode
//| Parms:
//|	hwnd	window handle
//|	id		Dialog control id for retrieveing length of
//| Returns:
//|	true length in bytes
//*---------------------------------------------------------------------------------

int INTFUN GetTrueWindowTextLength(HWND hwnd, int id)
{
	int	nLen=0,
			cBytes=sizeof(TCHAR);

	if (id == -1)
		nLen = (DWORD)SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0L);
	else
		nLen = (DWORD)SendDlgItemMessage(hwnd,id, WM_GETTEXTLENGTH, 0, 0L);

	return((nLen * cBytes));

} //GetTrueWindowTextLength()



//*---------------------------------------------------------------------------------
//| GetCTypeIndex:
//|		This function returns the index into the C Type default array for the
//|	passed in C Type. The reason that this search is done using a string
//|	is because SQL_C_DEFAULT == SQL_C_ULONG so that if the constant value
//|	of SQL_C_ULONG is passed in,as opposed to "SQL_C_LONG", the index for
//|	SQL_C_DEFAULT.
//| Parms:
//|	szCType		string containing the C Type to search for
//|	fUseCDft		TRUE if we need to search the array that contains SQL_C_DEFAULT
//|					FALSE if we search the array that doesn't contain SQL_C_DEFAULT
//| Returns:
//|	index into the default array
//*---------------------------------------------------------------------------------

SWORD INTFUN GetCTypeIndex(BOOL fUseCDft,LPTSTR szfCType)
{
	lpDFTARRAY	lpDft;
	UINT			cTypes,
					index;

	if (fUseCDft)
	{
		cTypes=ciCTypes;
		lpDft=lpdftCTypes;
	}
	else
	{
		cTypes=ciCTypesNoD;
		lpDft=lpdftCTypesNoD;

	}


	//100 is arbitrary just a place to stop for now.
	for (index=0;index < cTypes;index++)
	{
		if (!lstrcmp((LPTSTR)lpDft[index].szOpt,szfCType))
			break;
	}

	return ((SWORD)index);

} // GetCTypeIndex()



//*---------------------------------------------------------------------------------
//| GetSQLTypeIndex:
//|		This function returns the index into the SQL Type default array for the
//|	passed in C Type
//| Parms:
//|	fUseCDft		TRUE if we need to search the array that contains SQL_C_DEFAULT
//|					FALSE if we search the array that doesn't contain SQL_C_DEFAULT
//| Returns:
//|	index into the default array
//*---------------------------------------------------------------------------------

SWORD INTFUN GetSQLTypeIndex(LPTSTR szSQLType,BOOL fUseAll)
{
	SWORD index;
	//lpDFTARRAY	lpDft=fUseAll ? lpdftSqlTypes : lpdftSqlTypes;

	//100 is arbitrary just a place to stop for now.
	for (index=0;index < 100;index++)
	{
		if (!lstrcmp(lpdftSqlTypes[index].szOpt,szSQLType))
			break;
	}

	return (index);

} // GetSQLTypeIndex()


//*---------------------------------------------------------------------------------
//| ConvertToUnicode:
//|		This function converts a single byte string to a Unicode string, if needed
//| Parms:
//|	szStrA		single byte string
//|	szStrW		Unicode string
//| Returns:
//|	address of Unicode string
//*---------------------------------------------------------------------------------

LPTSTR INTFUN ConvertToUnicode(LPSTR szStrA,LPTSTR szStrW, WORD cbStrW)
{

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
							szStrA, (unsigned int)strlen(szStrA), szStrW, cbStrW/sizeof(WCHAR));
#else
	lstrcpyn(szStrW,(LPTSTR)szStrA,cbStrW-sizeof(TCHAR));
#endif

	return(szStrW);

} //ConvertToUnicode()


//*---------------------------------------------------------------------------------
//| ConvertToANSI:
//|		This function converts a Unicode string to a multi-byte string, if needed
//| Parms:
//|	szStrW		Unicode string
//|	szStrA		single byte string
//| Returns:
//|	address of multi-byte string
//*---------------------------------------------------------------------------------

LPSTR INTFUN ConvertToANSI(LPTSTR szStrW,LPSTR szStrA, WORD cbStrA)
{
	LPCSTR	szUnmappable= "?";
	BOOL		fUsedDefChar=FALSE;

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED,
							szStrW, lstrlen(szStrW), szStrA, cbStrA,
							szUnmappable,&fUsedDefChar);
#else
	lstrcpy(szStrA,szStrW);
#endif

	return(szStrA);

} //ConvertToANSI()



//*---------------------------------------------------------------------------------
//| GtrSendMessage:
//|		This function wraps windows API SendMessage() so that gator can
//|	convert to Wide Char. if necessary
//| Parms:
//|	Standard windows API parms
//| Returns:
//|	lResult
//*---------------------------------------------------------------------------------

LRESULT INTFUN GtrSendMessage(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	LRESULT lResult=0;

	if (IsChicago() || !fUnicode)
		lResult=SendMessageA(hWnd,Msg,wParam,lParam);
#ifdef UNICODE
	else
		lResult=SendMessageW(hWnd,Msg,wParam,lParam);
#endif

	return(lResult);

} //GtrSendMessageA()


//*---------------------------------------------------------------------------------
//| GtrSendDlgItemMessage:
//|		This function wraps windows API SendDlgItemMessage() so that gator can
//|	convert to Wide Char. if necessary
//| Parms:
//|	Standard windows API parms
//| Returns:
//|	lResult
//*---------------------------------------------------------------------------------

LRESULT INTFUN GtrSendDlgItemMessage(HWND hDlg,int nIDDlgItem,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	LRESULT lResult=0;

	if (IsChicago() || !fUnicode)
		lResult=SendDlgItemMessageA(hDlg,nIDDlgItem,Msg,wParam,lParam);
#ifdef UNICODE
	else
		lResult=SendDlgItemMessageW(hDlg,nIDDlgItem,Msg,wParam,lParam);
#endif

	return(lResult);

} //GtrSendDlgItemMessage()


//*---------------------------------------------------------------------------------
//| GtrSetWindowText:
//|		This function wraps windows API SetWindowText() so that gator can
//|	convert to Wide Char. if necessary
//| Parms:
//|	hDlg			handle to Dialog
//|	lpStr			Generic pointer which will be cast to either a unicode string
//|					or ANSI string.
//| Returns:
//|	result from SetWindowText
//*---------------------------------------------------------------------------------

BOOL INTFUN GtrSetWindowText(HWND hDlg,PTR lpStr)
{
	BOOL fResult=FALSE;

	if (IsChicago() || !fUnicode)
		fResult=SetWindowTextA(GetDlgItem(hDlg, IDE_DFT_BUFF_LEN), (LPSTR)lpStr);
#ifdef UNICODE
	else
		fResult=SetWindowTextW(GetDlgItem(hDlg, IDE_DFT_BUFF_LEN), (LPTSTR)lpStr);
#endif

	return(fResult);

} //GtrSetWindowText()



//*---------------------------------------------------------------------------------
//| GtrSetDlgItemText:
//|		This function wraps windows API SetDlgItemText() so that gator can
//|	convert to Wide Char. if necessary
//| Parms:
//|	hDlg			handle to Dialog
//|	lpStr			Generic pointer which will be cast to either a unicode string
//|					or ANSI string.
//| Returns:
//|	result from SetDlgItemText
//*---------------------------------------------------------------------------------

BOOL INTFUN GtrSetDlgItemText(HWND hDlg,int nIDDlgItem,PTR lpStr)
{
	BOOL fResult=FALSE;

	if (IsChicago() || !fUnicode)
		fResult=SetDlgItemTextA(hDlg, nIDDlgItem, (LPSTR)lpStr);
#ifdef UNICODE
	else
		fResult=SetDlgItemTextW(hDlg, nIDDlgItem, (LPTSTR)lpStr);
#endif

	return(fResult);

} //GtrSetDlgItemText()


//*---------------------------------------------------------------------------------
//| IsSupported:
//|		This function determines if a particular API is supported by the driver
//| Parms:
//|	lpCI				lpCONNECTIONINFO record
//|	nFunctionId		SQL API
//| Returns:
//|	result from SetDlgItemText
//*---------------------------------------------------------------------------------

BOOL INTFUN IsSupported(lpCONNECTIONINFO lpCI, SQLSMALLINT nFunctionId,
								BOOL fDontUseGetFunc)
{
	SQLSMALLINT nSupported=SQL_FALSE;
	RETCODE		rc=SQL_SUCCESS;

	//Check the Bit Mask first, if that fails then
	//call SQLGetFunctions
	if (lpCI->puSupportedFunctions)
	{
		if (lpUsrOptions->fODBC30)
		{
			if (SQL_FUNC_EXISTS(lpCI->puSupportedFunctions, nFunctionId))
				return(SQL_TRUE);
		}
		else
		{
			if (BitGet(lpCI->puSupportedFunctions, nFunctionId))
				return(SQL_TRUE);
		}
	}

	if (!fDontUseGetFunc)
		rc=SQLGetFunctions(lpCI->hdbc,nFunctionId,&nSupported);

	return(nSupported == SQL_TRUE);

} //IsSupported()


//*---------------------------------------------------------------------------------
//| Is2XDriver:
//|		This function determines if odbctest is currently connected to a ODBC 2.X
//|	driver.
//| Parms:
//|	hdbc		Connection handle
//| Returns:
//|	TRUE if Driver is 2.X, otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN Is2XDriver(HDBC hdbc)
{
	RETCODE	rc=SQL_SUCCESS;
	TCHAR		szVer[MEDBUFF]=TEXT("");
	SWORD		cbValue;

	rc=SQLGetInfo(hdbc,SQL_DRIVER_VER,szVer,sizeof(TCHAR)*3,&cbValue);
	if (RC_SUCCESSFUL(rc))
		return (!lstrcmp(szVer,TEXT("02")));
	else
		return (TRUE);

} //Is2XDriver()


//Init font
VOID INTFUN InitFont(LPLOGFONT lf,LPTSTR szFont)
{
	HDC hdc;
	HWND hwndActive;
	FONTENUMPROC lpEnumFontFamExProc;

	lpEnumFontFamExProc = (FONTENUMPROC)EnumFontFamExProc;

	memset(lf, 0, sizeof(LOGFONT));
	lstrcpy(lf->lfFaceName,szFont);
	lf->lfCharSet = DEFAULT_CHARSET;
	hwndActive=GetActiveWindow();
	hdc=GetDC(hwndActive);

	EnumFontFamilies(hdc,
							szFont,
							lpEnumFontFamExProc,
							(LONG_PTR)lf);

	ReleaseDC(hwndActive,hdc);
}
//Callback for EnumFontFamilies
int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,
												  int FontType,LPARAM lParam)
{
	LPLOGFONT lf= (LPLOGFONT)lParam;

	lf->lfCharSet = lpelfe->elfLogFont.lfCharSet;
	return FALSE;
}



//*---------------------------------------------------------------------------------
//| FindFileDir:
//|	Given the simple name of a file (just name.ext) return the full path
//|	of the file.
//| Parms:
//|	szFile				File name
//|	szPath				Full path of file, including file name
//| Returns:
//|	szPath if found, NULL otherwise
//*---------------------------------------------------------------------------------
LPTSTR INTFUN FindGatorIni(LPTSTR szPath)
{
	DWORD		fFound=FALSE;

	fFound=SearchPath(NULL,						// address of search path
							szINIFILE,				// address of gator32.ini string
							NULL,						// address of extension
							_MAX_PATH,				// size, in characters, of buffer
							szPath,					// address of buffer for found filename
							NULL);					// address of pointer to file component

	if (!fFound)
		lstrcpy(szPath,szINIFILE);

	//make call into gtrtst32.dll to set it's global path
	SetGatorIniPath(szPath);

	return szPath;
}

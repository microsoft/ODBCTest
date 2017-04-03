//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	MAIN.C
//|
//| Purpose:
//|		This module contains the main interface including initializing the
//|		application and handling menu items.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "fhinst.h"
#include "runtest.h"
#include "table.h"
#include "picktype.h"
#pragma warning(disable:4996)

//*---------------------------------------------------------------------------------
//|	Defines and Macros
//*---------------------------------------------------------------------------------
#define IDM_FIRSTCHILD			400
#define MAX_TILED_WINDOWS		10


//*---------------------------------------------------------------------------------
//|	Globals
//*---------------------------------------------------------------------------------
static TCHAR vszFile[] = TEXT(__FILE__);

//
// All global variables which are declared as extern and used in other modules
//		should have their initial declarations in the following section.  This will
//		mean they will most likely fall into the same page and given their wide
//		spread usage should reduce overall page faults required for access.
//
HWND 					hwndFrame, hwndClient, hwndOutput, hwndStatBar, hwndToolBar;
HWND					hwndMDIOutWin;						// Used only to create and delete MDI Output Window
HINSTANCE 			hInst;
HBRUSH				hWindowBrush;						// Common brush for background window color
HFONT					hEditFont=NULL;					// Common font for all edit windows
HFONT					hColumnFont=NULL;					// For column headers in results
HMENU 				hMenu=NULL;
HMENU					hSubMenus[NUM_MENUS];			// Track for status bar
UINT					uMenuHelp=0;						// Contains ids string; F1=help menu
int					iApiHelpCnt, iTestHelpCnt;		// How many times have we called these?
TCHAR 				szDirName[MAXOUTBUFF];
TCHAR					szDftFileFilter[100];
lpUSROPTIONS		lpUsrOptions=NULL;
LPTSTR				szTestHelpFile;
LPTSTR				szAPIHelpFile;
RECT 					wndRect;
LPRECT				lpWndRect=&wndRect;
RECT					OutWndRect;
RECT 					clientRect;
LPRECT				lpClientRect=&clientRect;
HWND					hwndCurMDIChild;					// Contains the current MDI child window at all times
#ifdef UNICODE
BOOL					fUnicode=TRUE;
#else
BOOL					fUnicode=FALSE;
#endif
TCHAR					g_szGatorIni[_MAX_PATH];
BOOL					g_fFileDSN=FALSE;

extern const ODBCAPI_LIST apiList[NUM_ODBC_APIS];

extern lpCONNECTIONINFO			lpActiveConnWin;	// The active connection handle
extern lpSTATUSWINDOWINFO		lpStat;
extern lpTOOLBARINFO				lpTool;
extern TCHAR 						szOutBuff[MAXOUTBUFF];
extern lpRESULTWININFO			lpActiveResultWin;
extern lpHEADENV					lpHeadEnv;
extern lpINSTALLER				lpInst;

dCSEG(TCHAR)	szFRAMECLASS[] 					= TEXT("GATOR:Frame");
static TCHAR szCONNECTCLASS[]						= TEXT("GATOR:CONNECT");
static TCHAR szOUTWINCLASS[]						= TEXT("GATOR:OUTWIN");
static TCHAR szCONNWINCLASS[]						= TEXT("GATOR:CONNWIN");
static TCHAR szRSLTWINCLASS[]						= TEXT("GATOR:RSLTWIN");
static TCHAR szSTATBARCLASS[]						= TEXT("GATOR:STATBAR");
static TCHAR szTOOLBARCLASS[]						= TEXT("GATOR:TOOLBAR");
static TCHAR szGATOR[]								= TEXT("GATOR");

dCSEG(TCHAR) szTESTHELPFILENAME[]				= TEXT("ODBCTEST.CHM");
dCSEG(TCHAR) szAPIHELPFILENAME[]					= TEXT("ODBC.CHM");
dCSEG(TCHAR) szHLPPATH[] 							= TEXT("..\\HELP\\");
dCSEG(TCHAR) szTESTHLPPATH[] 						= TEXT("..\\HELP\\");
dCSEG(TCHAR) szLOGFILTER[]							= TEXT("Log Files (*.log)\0*.log\0All Files (*.*)\0*.*\0");
dCSEG(TCHAR) szLOGEXT[]								= TEXT("log");
dCSEG(TCHAR) szDFTLOGNAME[]						= TEXT("\\SQL.LOG");
dCSEG(TCHAR) szKBFree[]								= TEXT("%u KB");
dCSEG(TCHAR) szRECDFTLOGNAME[]					= TEXT("\\RECORD.LOG");
#ifdef UNICODE
dCSEG(TCHAR)	szWindowTitle[]					= TEXT("ODBC Test (UNICODE)");
#else
dCSEG(TCHAR)	szWindowTitle[]					= TEXT("ODBC Test (ANSI)");
#endif


//*---------------------------------------------------------------------------------
//|	External functions
//*---------------------------------------------------------------------------------
#ifndef WIN32
BOOL EXPFUN IsWindowsWOW(void);
BOOL EXPFUN IsWin32s(void);
BOOL EXPFUN IsWindowsMIPS(void);
BOOL EXPFUN IsWindowsDEC(void);
BOOL EXPFUN IsWindowsIntel(void);
LPTSTR EXPFUN DLLGetEnv (LPTSTR lpszVariableName);
#endif

BOOL EXTFUN IsChicago(void);


//*---------------------------------------------------------------------------------
//|	Local functions
//*---------------------------------------------------------------------------------
void INTFUN ClearOutputWindow(lpCONNECTIONINFO ci);	// From CONNWIN.C
void INTFUN OpenFileToInputWindow(HWND);					// From CONNWIN.C
void INTFUN SaveFileInInputWindow(HWND);					// From CONNWIN.C

BOOL Init(HANDLE hInstance,   HANDLE hPrevInstance,
          LPTSTR  lpszCmdLine, int    CmdShow);
WPARAM  DoMain(HANDLE hInstance);
LRESULT EXTFUN WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID INTFUN DoHorizontalTile(HWND hwndClient);

void DisplayAbout(HWND hwnd, HINSTANCE hInst);
LRESULT EXTFUN AboutWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LPTSTR INTFUN ResolveHelpFile(LPTSTR szHelpFile, LPCTSTR szHelpName, LPCTSTR szRelativePath);
VOID INTFUN ResetMenuStatus(HMENU hMenu, int iMenu, int iCalledFrom);
UWORD * InitMenuArray(lpCONNECTIONINFO lpci);

// Following are for MDI children
LRESULT EXTFUN ConnectWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT EXTFUN ResultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT EXTFUN OutWinWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT EXTFUN StatusWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT EXTFUN ToolWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


//*------------------------------------------------------------------------
//| WinMain:
//|     Parameters:
//|         hInstance     - Handle to current Data Segment
//|         hPrevInstance - Handle to previous Data Segment (NULL if none)
//|         lpszCmdLine   - Long pointer to command line info
//|         nCmdShow      - Integer value specifying how to start app.,
//|                            (Iconic [7] or Normal [1,5])
//*------------------------------------------------------------------------
int PASCAL WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR  lpszCmdLine,
                    int    nCmdShow)
{

	TCHAR tszCmdLine[XLARGEBUFF]=TEXT("");

#ifdef _DEBUG
	DeleteFile(TEXT("\\TMP\\MEM"));
#endif


	//Check to see if odbctest was called from the command line with
	// a File DSN
	ConvertToUnicode(lpszCmdLine,tszCmdLine,
                     sizeof(tszCmdLine)/sizeof(tszCmdLine[0]));

	if (_tcsstr(tszCmdLine,TEXT(".dsn")))
		g_fFileDSN=TRUE;
	else
		g_fFileDSN=FALSE;

	//Locate gator32.ini. First look in working or launch directory
	// First look for GATOR.INI
	FindGatorIni(g_szGatorIni);

	if (Init(hInstance, hPrevInstance,tszCmdLine,nCmdShow))
		return(int)(DoMain(hInstance));
    return -1;
}

//*---------------------------------------------------------------------------------
//| GetSupportedFunctions:
//|	Gets supported functions into a bitmask
//| Parms:
//|	hdbc						Valid connection handle to look at
//|	lpAFuncs					Bit array to indicate which are supported
//| Returns:
//|	RETCODE
//*---------------------------------------------------------------------------------
RETCODE INTFUN GetSupportedFunctions(HDBC hdbc, lpCONNECTIONINFO ci)
{
	RETCODE	rc=SQL_SUCCESS;
	BOOL		fSuccess=TRUE;
	UWORD		uFunction;

	uFunction=SQL_API_ODBC3_ALL_FUNCTIONS;

	// Don't enumerate if this option is off
	if(!lpUsrOptions->fDriverMenus || !hdbc)
		return SQL_ERROR;

   // Find out which functions are supported
	if (!ci->puSupportedFunctions)
	{
		ci->puSupportedFunctions =
			(UWORD *)AllocateMemory(SQL_API_ODBC3_ALL_FUNCTIONS_SIZE * sizeof(UWORD));
	}

	rc = SQLGetFunctions(hdbc,uFunction, ci->puSupportedFunctions);

	return(rc);
}



//*---------------------------------------------------------------------------------
//| Init:
//|	This function will take care of initilization for the application.
//| Parms:
//|	hInstance				Data segment for this instance
//|	hPrevInstance			Data segment for previous instance
//|	lpszCmdLine				Command line parameters
//|	nCmdShow					Desired display method
//| Returns:
//|	TRUE		if function was successful
//|	FALSE		if there was an error
//*---------------------------------------------------------------------------------
BOOL Init(HANDLE hInstance,   HANDLE hPrevInstance,
          LPTSTR  lpszCmdLine, int    nCmdShow)
{
	WNDCLASS 	wc;
	RECT			ClientRect;
	int			dex;
	DWORD			dwIndex=0;
	RUNTESTLIST rst;
	BOOL fRunCmdLine=FALSE;

	hInst = hInstance;
#ifdef WIN32
	AdjMemInit();						// Find default page size (void used for macro)
#endif

	if(!hPrevInstance && !fRunCmdLine) {
		// Register the Frame Window first
		memset(&wc, 0, sizeof(WNDCLASS));
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(GATORICON));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szFRAMECLASS;
		if(!RegisterClass(&wc))
			return FALSE;

		// Output MDI child window
		wc.lpfnWndProc = (WNDPROC)OutWinWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(HWND);						// Store edit window handle
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(OUTWINICON));
		wc.lpszClassName = szOUTWINCLASS;
		if(!RegisterClass(&wc))
			return FALSE;

		// Connection MDI child window
		wc.lpfnWndProc = (WNDPROC)ConnectWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(lpCONNECTIONINFO);
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(CONNECTICON));
		wc.lpszClassName = szCONNWINCLASS;
		if(!RegisterClass(&wc))
			return FALSE;

		// Results MDI child window
		wc.lpfnWndProc = (WNDPROC)ResultWndProc;
		wc.style |= CS_DBLCLKS;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(lpRESULTWININFO);
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(RESULTICON));
		wc.lpszClassName = szRSLTWINCLASS;
		if(!RegisterClass(&wc))
			return FALSE;

		// Status Bar
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)StatusWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(lpSTATUSWINDOWINFO);
		wc.hIcon = NULL;
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszClassName = szSTATBARCLASS;
		if(!RegisterClass(&wc))
			return FALSE;

		// Tool Bar
		wc.lpfnWndProc = (WNDPROC)ToolWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(lpTOOLBARINFO);
		wc.hbrBackground = NULL;
		wc.lpszClassName = szTOOLBARCLASS;
		if(!RegisterClass(&wc))
			return FALSE;
		}
	// Read in user preferences
	lpUsrOptions = (lpUSROPTIONS)AllocateMemory(sizeof(USROPTIONS));
	if(!lpUsrOptions)
		return FALSE;
	memset(lpUsrOptions, 0, sizeof(USROPTIONS));
	SaveGetUserOptions(lpUsrOptions, FALSE);

	// Load menus and create a font to use based on user options
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(lpUsrOptions->nODBCVer));

	for(dex=0;  dex<NUM_MENUS;  dex++)
		hSubMenus[dex] = GetSubMenu(hMenu, dex);

	hEditFont = CreateEditFont();

	//For Maximized or minimized window, use defaults
	if (lpUsrOptions->fMaximize || lpUsrOptions->fMinimize)
		lpUsrOptions->x = lpUsrOptions->y = lpUsrOptions->nWidth =
		lpUsrOptions->nHeight = CW_USEDEFAULT;

	//Create a Frame window using saved User Options for size and position
	hwndFrame = CreateWindow(szFRAMECLASS,
			szWindowTitle,
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			lpUsrOptions->x,
			lpUsrOptions->y,
			lpUsrOptions->nWidth,
			lpUsrOptions->nHeight,
			NULL,
			(HMENU)(hMenu),
			hInstance,
			NULL);

	//Cascade output window if client area is too small for default size
	hwndClient = GetWindow(hwndFrame, GW_CHILD);
	GetClientRect(hwndFrame,(RECT *)&ClientRect);
	if (ClientRect.right < OUTWIN_WIDTH ||
		 ClientRect.bottom < OUTWIN_HEIGHT )
		PostMessage(hwndClient,WM_MDICASCADE,0,0L);

	//@INSTALLER
	lpInst=(lpINSTALLER)AllocateMemory(sizeof(INSTALLER));

	if (!lpInst && !fRunCmdLine)
		return (FALSE);
	lpInst->hwndParent=hwndClient;

	// Now create the status window
	hwndStatBar = CreateStatBar(hwndFrame, hInst);
	hwndToolBar = CreateToolBar(hwndFrame, hInst);
	SetStatBarText(hwndStatBar,
						GetGtrString(szOutBuff, MAXSTATTEXT, idsOutputWinDesc),
						BLACK_TEXT);


	//Allocate lpHeadEnv
	lpHeadEnv=(lpHEADENV)AllocateMemory(sizeof(HEADENV));

	if (!lpHeadEnv )
		return FALSE;		// let Windows destroy children
	//Create Drop down list containing
	lpHeadEnv->hwndEnvs = CreateWindow(TEXT("COMBOBOX"), NULL,
						WS_CHILD | WS_VSCROLL |
						CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT,
						50, 50, ENV_WIDTH, 90,
						hwndToolBar, (HMENU)(IDW_ENVS), hInst, NULL);

	if (!lpHeadEnv->hwndEnvs && !fRunCmdLine) {
		return FALSE;		// let Windows destroy children
    }

	// Add NULL henv
	dwIndex=(DWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)TEXT("SQL_NULL_HENV"));
	dwIndex=(DWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL, 0, 0L);


	// Resolve help files, looking for SDK first, then on path
	szTestHelpFile = (LPTSTR)AllocateMemory(_MAX_PATH);
	szAPIHelpFile = (LPTSTR)AllocateMemory(_MAX_PATH);
	if(!szTestHelpFile || !szAPIHelpFile)
		return FALSE;
	ResolveHelpFile(szAPIHelpFile, szAPIHELPFILENAME,  szHLPPATH);
	ResolveHelpFile(szTestHelpFile, szTESTHELPFILENAME,  szTESTHLPPATH);

	//Check if there are command line args.
	if (!g_fFileDSN)
	{
		if (_tcscmp(lpszCmdLine,TEXT("\0")))
	 		return(RunFromCmdLine(&rst,lpszCmdLine,hwndFrame));
	}

	//ShowWindow(hwndFrame, lpUsrOptions->fMaximize ? SW_SHOWMAXIMIZED : nCmdShow);
    ShowWindow(hwndFrame, SW_SHOWMAXIMIZED);
    UpdateWindow(hwndFrame);


	return (hwndFrame != NULL);
}

//*------------------------------------------------------------------------
//| DoMain:
//|     This is the main loop for the application:
//*------------------------------------------------------------------------
WPARAM  DoMain(HANDLE hInstance)
{
	MSG			msg;
	HANDLE		hAccel;
	BOOL			fFreed=FALSE;
	HINSTANCE	hLibInst=NULL;
    int         rc;

	hAccel = LoadAccelerators(hInstance,TEXT("BASEACCELS"));
	while((rc = GetMessage(&msg, NULL, 0, 0)) != 0)
		{
        if (rc == -1) {
            break;
        }
		if(!TranslateAccelerator(hwndFrame,hAccel,&msg) &&
			!TranslateMDISysAccel(hwndClient, &msg))
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}
		}

	DeleteObject(hEditFont);
	if(hColumnFont)
		DeleteObject(hColumnFont);

	return (WPARAM)msg.wParam;
}


//*------------------------------------------------------------------------
//| DoShutDown:
//|   Clean up all the objects we have created and used globally, then save
//|	the calling window position and destroy it to start exit process.
//*------------------------------------------------------------------------
long INTFUN DoShutDown(HWND hwnd)
{

	//Get coords for Output Window before it is destroyed.
	if (!(lpUsrOptions->fOutputMaximize = IsZoomed(hwndClient)))
		lpUsrOptions->fOutputMinimize = IsIconic(hwndClient);

   GetWindowRect(hwndClient,&OutWndRect);
	lpUsrOptions->xOutput = (OutWndRect.left < 0) ? 0 : OutWndRect.left;
   lpUsrOptions->yOutput = (OutWndRect.top < 0) ? 0 : OutWndRect.top;
   lpUsrOptions->nWidthOutput = (OutWndRect.right - OutWndRect.left < 0) ? 0 : OutWndRect.right - OutWndRect.left;
   lpUsrOptions->nHeightOutput = (OutWndRect.bottom - OutWndRect.top < 0) ? 0 : OutWndRect.bottom - OutWndRect.top;


	if (lpHeadEnv && lpHeadEnv->cEnvs)
	{
		 if (!DoDropAllEnvs(lpHeadEnv->lpHead))
			goto DontShutDown;
	}

	FREE_PTR(lpHeadEnv);

	DestroyOutWinWindow(hwndClient, hwndOutWin);

	// Free the help files if used
	if(iApiHelpCnt)
		WinHelp(hwnd, szAPIHelpFile, HELP_QUIT, 0L);
	if(iTestHelpCnt)
		WinHelp(hwnd, szTestHelpFile, HELP_QUIT, 0L);
	FREE_PTR(szTestHelpFile);
	FREE_PTR(szAPIHelpFile);

	//Save max/min status or window coordinates in UsrOptions
	if (!(lpUsrOptions->fMaximize = IsZoomed(hwnd)))
		lpUsrOptions->fMinimize = IsIconic(hwnd);

   GetWindowRect(hwnd,lpWndRect);
	lpUsrOptions->x = (wndRect.left < 0) ? 0 : wndRect.left;
   lpUsrOptions->y = (wndRect.top < 0) ? 0 : wndRect.top;
   lpUsrOptions->nWidth = (wndRect.right - wndRect.left < 0) ? 0 : wndRect.right - wndRect.left;
   lpUsrOptions->nHeight = (wndRect.bottom - wndRect.top < 0) ? 0 : wndRect.bottom - wndRect.top;
	SaveGetUserOptions(lpUsrOptions, TRUE);

	// Free the user options memory
	FREE_PTR(lpUsrOptions);

	// Now free remaining memory and kill off windows
	DestroyDialogs((UINT)-1);
	DestroyWindow(hwnd);

DontShutDown:

	return 0;
}


//*---------------------------------------------------------------------------------
//| DoMoveWindowCursor:
//|	Move the cursor based on the current location and focus.
//| Parms:
//|	None
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoMoveWindowCursor(void)
{
	if(lpActiveConnWin)
		ConnWinMoveWindowFocus(lpActiveConnWin);
	else
		SetFocus(hwndOutWin);
}


//*------------------------------------------------------------------------
//| WndProc:
//|	This is *the* window procedure.  It will handle the main application
//|	and menu commands when a user does not have an MDI child in focus.
//*------------------------------------------------------------------------
LRESULT EXTFUN WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND		hwndClient;
	static int 		dx, dy;

 	switch(msg) {

		//--------------------------------------------------------------------
		//  WM_CREATE for the main window procedure will create the MDI
		//		client window in which all child windows will be located.
		//		After this the Output Window is created.
		//--------------------------------------------------------------------
		case WM_CREATE:
			{
			CLIENTCREATESTRUCT			clientcreate;
			clientcreate.hWindowMenu = hSubMenus[IDM_WINDOW_MENU];
			clientcreate.idFirstChild = IDM_FIRSTCHILD;
			hwndClient = CreateWindow(TEXT("MDICLIENT"), NULL,
					WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_CLIPSIBLINGS,
					0, 0, 0, 0, hwnd, (HMENU)(1), hInst,
					(LPTSTR)&clientcreate);
			if(!hwndClient)
				return -1;

			// Create output window
			hwndMDIOutWin = CreateOutWinWindow(hwndClient, hInst);
			if(!hwndMDIOutWin)
				return -1;
            }
			return 0;

		//--------------------------------------------------------------------
		// Hande the WM_SIZE to change the location of the tool bar, status
		//		bar, and client window area.
		//--------------------------------------------------------------------
		case WM_SIZE:
			{
			WORD	dx,dy;


			dx = LOWORD(lParam);
			dy = HIWORD(lParam);


			MoveWindow(hwndToolBar, 0, 0, dx, lpTool->dy, TRUE);
			MoveWindow(hwndClient, 0, lpTool->dy, dx, dy - lpStat->dy - lpTool->dy, TRUE);
			MoveWindow(hwndStatBar, 0, dy - lpStat->dy, dx, lpStat->dy, TRUE);
			SizeStatBar(hwndStatBar, dx);
			SizeToolBar(hwndToolBar, dx);
			SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0L);

			}
			return 0;


		//--------------------------------------------------------------------
		//  WM_ACTIVATE is raised when the main window gains/loses focus.
		//		We'll keep track of the focus window and use it to reset the
		//		cursor when we move around.
		//--------------------------------------------------------------------
		case WM_ACTIVATE:
			{
			static HWND		hwndFocus;

			if(wParam == WA_INACTIVE)
				hwndFocus = GetFocus();
			else if(hwndFocus)
				SetFocus(hwndFocus);
			}
			return 0;


		//--------------------------------------------------------------------
		// Hande the WM_INITMENUPOPUP message so that when the user selects
		//		a menu, we enable/disable each item based on our current state.
		//--------------------------------------------------------------------
		case WM_INITMENUPOPUP:
			if(!(BOOL)HIWORD(lParam))			// Not the system menu
				ResetMenuStatus((HMENU)wParam, (int)LOWORD(lParam), 1);
			break;


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



		//--------------------------------------------------------------------
		// USER_FUNCTION_HELP:   This is sent by our hook proc to tell us
		//								the user pressed F1=Help on a menu
		//--------------------------------------------------------------------
		case USER_FUNCTION_HELP:
			{
			if(uMenuHelp) {
				GetGtrString(szOutBuff, MAXOUTBUFF, (uMenuHelp-1));
			//	iTestHelpCnt += InvokeHelp(hwnd, (LPCTSTR)szTestHelpFile,
			//			(LPCTSTR)szOutBuff);
				}
			else
				SendMessage(hwnd, WM_COMMAND, IDM_TEST_HELP, 0L);
			}
			return 0;


		//--------------------------------------------------------------------
		// WM_COMMAND:			Handle menu choices.  Note that many will be
		//							handled by a connection window.
		//--------------------------------------------------------------------
		case WM_COMMAND:
			{
			int	id=GET_WM_COMMAND_ID(wParam, lParam);
			lpENVIRONMENTINFO lpActiveEnv=NULL;

			//-----------------------------------------------------------------------
			//  Check for a SQL function
			//-----------------------------------------------------------------------
  	      if (id == IDM_FILL_PARAM)
				id=GetSqlFuncOffset((UWORD)id)+1;

			if (id >= FIRST_API && id <= NUM_ODBC_APIS)  // In range of API function
			{
      		DialogManager(lpActiveConnWin, (UWORD)id - 1, 0, 0);

				return 0;
			}

			switch(id) {
				//----------------------------------------------------------------------
				// This is our accelrator for F6=Move Window.  Switch between windows
				//----------------------------------------------------------------------
				case IDM_MOVE_WINDOW:
					DoMoveWindowCursor();
					return 0;


				//----------------------------------------------------------------------
				// This menu item is only in the system menu of a Results MDI child.
				//	When you maximize a child MDI window, the system menu commands are
				//	routed through the Frame window, so we have to look for this message
				//	both here and in rsltwin.c.
				//----------------------------------------------------------------------
				case IDM_FREESTMT_CLOSE:
					{
					SetCurrentStmt(lpActiveResultWin->lpci, lpActiveResultWin->lpStmt);
					DoFreeStmtClose(FALSE);
					}
					return 0;


				//-----------------------------------------------------------------------
				//  File menu items
				//-----------------------------------------------------------------------
				case IDM_FILE_OPEN:
					{
					OpenFileToInputWindow((lpActiveConnWin) ?
								lpActiveConnWin->hwndIn :
								hwndOutWin);
					}
					return 0;

				case IDM_FILE_SAVEAS:
					{
					SaveFileInInputWindow((lpActiveConnWin) ?
								lpActiveConnWin->hwndIn :
								hwndOutWin);
					}
					return 0;

				case IDM_FILE_USEROPTIONS:
					{
					//Save max/min status or window coordinates in UsrOptions
					if (!(lpUsrOptions->fMaximize = IsZoomed(hwnd)))
						lpUsrOptions->fMinimize = IsIconic(hwnd);

					if (!(lpUsrOptions->fOutputMaximize = IsZoomed(hwndOutput)))
						lpUsrOptions->fOutputMinimize = IsIconic(hwndOutput);

					GetWindowRect(hwndOutput,&OutWndRect);
					lpUsrOptions->xOutput = (OutWndRect.left < 0) ? 0 : OutWndRect.left;
					lpUsrOptions->yOutput = (OutWndRect.top < 0) ? 0 : OutWndRect.top;
					lpUsrOptions->nWidthOutput = (OutWndRect.right - OutWndRect.left < 0) ? 0 : OutWndRect.right - OutWndRect.left;
					lpUsrOptions->nHeightOutput = (OutWndRect.bottom - OutWndRect.top < 0) ? 0 : OutWndRect.bottom - OutWndRect.top;

					GetWindowRect(hwnd,lpWndRect);
					lpUsrOptions->x = (wndRect.left < 0) ? 0 : wndRect.left;
					lpUsrOptions->y = (wndRect.top < 0) ? 0 : wndRect.top;
					lpUsrOptions->nWidth = (wndRect.right - wndRect.left < 0) ? 0 : wndRect.right - wndRect.left;
					lpUsrOptions->nHeight = (wndRect.bottom - wndRect.top < 0) ? 0 : wndRect.bottom - wndRect.top;

					SaveGetUserOptions(lpUsrOptions, TRUE);
					}
					return 0;


				case IDM_FILE_EXIT:
					SendMessage(hwnd, WM_CLOSE, 0, 0L);
					return 0;

				//-----------------------------------------------------------------------
				//  Edit menu items
				//-----------------------------------------------------------------------
				case IDM_EDIT_UNDO:
				case IDM_EDIT_CUT:
				case IDM_EDIT_COPY:
				case IDM_EDIT_PASTE:
				case IDM_EDIT_DELETE:
					{
					HWND 		fhwnd;
					if(!(fhwnd = GetEditWindow()))
						return 0;
					SendMessage(fhwnd,
									(id == IDM_EDIT_UNDO) ? WM_UNDO :
									(id == IDM_EDIT_CUT) ? WM_CUT :
									(id == IDM_EDIT_COPY) ? WM_COPY :
									(id == IDM_EDIT_PASTE) ? WM_PASTE : WM_CLEAR,
									0, 0L);
					}
					return 0;

            case IDM_EDIT_SELECT_ALL:
            	{
            	HWND		fhwnd;

            	if(fhwnd = GetEditWindow())
						SendMessage(fhwnd, EM_SETSEL, PORT_EM_SETSEL(0, -1));
            	}
            	return 0;

				case IDM_EDIT_CLEAR:
					ClearOutputWindow(lpActiveConnWin);
					return 0;

				// Flip-flop menu state and reset accordingly
				case IDM_EDIT_WRAP:
					CheckMenuItem(hMenu, IDM_EDIT_WRAP,
									(MF_CHECKED == GetMenuState(hMenu, IDM_EDIT_WRAP, MF_BYCOMMAND)) ?
												MF_UNCHECKED : MF_CHECKED);
					DoResetWrap();
					return 0;



				//-----------------------------------------------------------------------
				//  Connect menu items
				//-----------------------------------------------------------------------
				case IDM_FULL_CONNECT:
					ConnectFullConnect();
					return 0;

				case IDM_FULL_DISCONNECT:
					ConnectFullDisconnect(lpActiveConnWin,SQL_SUCCESS);
					return 0;

				case IDM_DATA_SOURCES_ALL:
					ConnectDataSourcesAll(lpActiveConnWin);
					return 0;

				case IDM_DRIVERS_ALL:
					ConnectDriversAll(lpActiveConnWin);
					return 0;

				case IDM_GET_INFO_ALL:
					ConnectGetInfoAll(lpActiveConnWin);
					return 0;

				//----------------------------------------------------------------------
				//  Statement menu items
				//----------------------------------------------------------------------
				case IDM_SHOW_PARAMS:
					if (lpActiveConnWin)
						StatementShowParams(lpActiveConnWin);
					return 0;

				case IDM_SHOW_CURSOR_SETTINGS:
					if (lpActiveConnWin)// && lpActiveConnWin->lpCurStmt)
						GetCurrentCursorSettings(lpActiveConnWin);
					return 0;

				//----------------------------------------------------------------------
				//  Attr menu items
				//----------------------------------------------------------------------
				case IDM_SET_CURSOR_ATTR:
					if (lpActiveConnWin && lpActiveConnWin->lpCurStmt)
						SetCursorAttributes(lpActiveConnWin->lpCurStmt);
					return 0;

				//----------------------------------------------------------------------
				//  Results menu items
				//----------------------------------------------------------------------
				case IDM_DESCRIBE_COL_ALL:
					ResultsDescribeColAll(lpActiveConnWin);
					return 0;

				case IDM_FETCH_ALL:
					ResultsFetchAll(lpActiveConnWin);
					return 0;

				case IDM_SHOW_BOUND_COLS:
					szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV0, TRUE, TRUE,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsShowBoundColsTitle));
					ResultsShowBoundCols(lpActiveConnWin,
							lpActiveConnWin->lpCurStmt->lpVars, NO_RETCODE_CHECK,
							(UWORD)-1, TRUE);
					return 0;

				case IDM_BIND_COL_ALL:
					ResultsBindColAll(lpActiveConnWin);
					return 0;

				case IDM_GET_DATA_ROW:
					ResultsGetDataRow(lpActiveConnWin);
					return 0;
				case IDM_DISPLAY_ROWSET:
					if (lpActiveConnWin && lpActiveConnWin->lpCurStmt)
						ResultsGetRowSet(lpActiveConnWin);
					return 0;
				case IDM_GET_DATA_ALL:
					ResultsGetDataAll(lpActiveConnWin);
					return 0;

				case IDM_MAKE_TABLE:
					{
						TCHAR			szTbl1[LARGEBUFF]=TEXT("");
						lpFIELDINFO	rgFieldInfo=NULL;
						int			cTypes=0;

						if (!(rgFieldInfo=(lpFIELDINFO)AllocateMemory(sizeof(FIELDINFO) * MAX_TYPES)))
							return(0);

						//cTypes=PickTypes(hwnd,hInst,rgFieldInfo);

						if (lpActiveConnWin)
							MakeTable(&lpActiveConnWin->hdbc, szTbl1,rgFieldInfo,3,&cTypes);

						FREE_PTR(rgFieldInfo);

					}
					return(0);

				case IDM_ALL_ERRORS:
					{
					//Bug #643
					lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
					HENV						henv=lpActiveEnv?lpActiveEnv->henv:NULL;
					HDBC						hdbc=NULL;
					HSTMT						hstmt=NULL;
					BOOL						fErrorsFound=FALSE;
					lpDESCRIPTORHEADER	lpDesc=NULL;
					SWORD						iDesc=0;

					if (lpActiveConnWin)
					{
						hdbc=lpActiveConnWin->hdbc;
						if (lpActiveConnWin->lpCurStmt)
							hstmt=lpActiveConnWin->lpCurStmt->hstmt;
						if (lpActiveConnWin->lpDesc)
							lpDesc=lpActiveConnWin->lpDesc;

					}

					//Print out "Errors All"
					szPrintf(lpUsrOptions,GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV0,
							TRUE, FALSE, TEXT("\r\nErrors All:\r\n"));

					if (lpActiveEnv )
					{
						if ( !lpActiveConnWin ||
								(!lpActiveConnWin->hdbcCurr &&
								!lpActiveConnWin->hstmtCurr &&
								!lpActiveConnWin->hdescCurr) )
							lpActiveEnv->henvCurr=henv;
					}

					//fErrorsFound=ToolOptionsAllErrors(lpActiveConnWin,(henv) ? henv : NULL,(hdbc) ? hdbc : NULL,(hstmt) ? hstmt : NULL ,NULL,SQL_ERROR);
					fErrorsFound=ToolOptionsAllErrors(lpActiveConnWin,(henv)? henv:NULL,NULL,NULL,NULL,SQL_ERROR);
					fErrorsFound += ToolOptionsAllErrors(lpActiveConnWin, NULL,(hdbc) ? hdbc :NULL,NULL,NULL,SQL_ERROR);
					fErrorsFound += ToolOptionsAllErrors(lpActiveConnWin, NULL,NULL,(hstmt) ? hstmt:NULL,NULL,SQL_ERROR);



					// Do all descriptor handle errors
					if (lpDesc)
					{
						for (iDesc=0;iDesc < lpActiveConnWin->cDescs;iDesc++)
						{
							fErrorsFound+=EnumerateErrors30(TEXT("desc"),
													GETOUTPUTWINDOW(lpActiveConnWin),
													NULL,SQL_HANDLE_DESC, lpDesc->hdesc);

							lpDesc=lpDesc->next;
						}
					}

					if (!fErrorsFound)
					{
						//Print out "Errors All"
						szPrintf(lpUsrOptions,GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV0,
								TRUE, FALSE, TEXT("\t\tNO ERRORS FOUND!!\r\n"));
					}
					}

					return(0);
				case IDM_CLEAR_ERROR_QUEUE:
					{
					lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
					HENV						henv=lpActiveEnv?lpActiveEnv->henv:NULL;
					HDBC						hdbc=NULL;
					HSTMT						hstmt=NULL;
					BOOL						fErrorsFound=FALSE;
					lpDESCRIPTORHEADER	lpDesc=NULL;
					SWORD						iDesc=0;

					if (lpActiveConnWin)
					{
						hdbc=lpActiveConnWin->hdbc;
						if (lpActiveConnWin->lpCurStmt)
							hstmt=lpActiveConnWin->lpCurStmt->hstmt;
						if (lpActiveConnWin->lpDesc)
							lpDesc=lpActiveConnWin->lpDesc;

					}
					ClearErrorQueue(SQL_HANDLE_ENV,henv);
					ClearErrorQueue(SQL_HANDLE_DBC,hdbc);
					ClearErrorQueue(SQL_HANDLE_STMT,hstmt);

					// Do all descriptor handle errors
					if (lpActiveConnWin)
						lpDesc=lpActiveConnWin->lpDesc;

					if (lpDesc)
					{
						for (iDesc=0;iDesc < lpActiveConnWin->cDescs;iDesc++)
						{
							ClearErrorQueue(SQL_HANDLE_DESC,lpDesc->hdesc);

							lpDesc=lpDesc->next;
						}
					}

					}
					return 0;

				case IDM_GET_FUNCTIONS_ALL:
					MiscGetFunctionsAll(lpActiveConnWin,CALLED_FROM_MENU);
					break;

				case IDM_GET_DESCRIPTOR_ALL:
					GetDescriptorAll(lpActiveConnWin);
					break;

				case IDM_LOAD_DESCRIPTOR_DEFAULTS:
					LoadDescriptorDefaults(lpActiveConnWin);
					break;

				//-----------------------------------------------------------------------
				//  Tools menu items
				//-----------------------------------------------------------------------
				case IDM_TOOLS_USER_OPTIONS:
					ToolsUserOptions(hwndClient, lpUsrOptions);
					return 0;

				// Get the menu status array if driver menus are enabled
				case IDM_CONNECT_SET_DRIVER_DEP_MENUS:
					GetSupportedFunctions(lpActiveConnWin->hdbc, lpActiveConnWin);
					return 0;

				// Use the GetLogFile function from Gatortst.dll
				case IDM_TRACE:
					{
					RETCODE				rc;
					BOOL					fAppend=TRUE;					// Default is to append
					TCHAR					szTitle[25];
					lpCONNECTIONINFO	ci = lpActiveConnWin;
					HENV					henv=GetActiveEnv();
					HDBC					hdbc=NULL;
					HSTMT					hstmt=NULL;

					//get ODBC handles
					if (ci)
					{
						hdbc=ci->hdbc;

						if (ci->lpCurStmt)
							hstmt=ci->lpCurStmt->hstmt;

					}

					// Save the handle to the focus window to avoid losing the cursor
					lpUsrOptions->hwndFocus = GetFocus();

					// If not tracing already, get a file name and start
					if(!lpUsrOptions->fDMTrace)
					{
						// Set title and default log file
						GetGtrString((LPTSTR)szTitle,
                                     sizeof(szTitle)/sizeof(szTitle[0]), idsLogFileTitle);

						if(!*lpUsrOptions->szLog)
							lstrcpy(lpUsrOptions->szLog, szDFTLOGNAME);

						// Now ask user for desired file
						if(GetLogFile(hwnd, NULL, szTitle,
								(LPTSTR)szLOGFILTER, (LPTSTR)szLOGEXT, (LPTSTR)lpUsrOptions->szLog,
								OFN_NOCHANGEDIR, &fAppend))
						{
							// If user wants clean file, delete old one
							if(!fAppend)
								DeleteFile(lpUsrOptions->szLog);

							if (lpUsrOptions->fODBC30)
								// User selected a valid file, establish as trace file
								rc = SQLSetConnectAttr(NULL, SQL_OPT_TRACEFILE,
										(LPTSTR)lpUsrOptions->szLog,_MAX_PATH);
							else
								// User selected a valid file, establish as trace file
								rc = SQLSetConnectOption(NULL, SQL_OPT_TRACEFILE,
										(SQLULEN)(LPTSTR)lpUsrOptions->szLog);

							if(rc == SQL_SUCCESS)
							{
								// Now turn on tracing
								if (lpUsrOptions->fODBC30)
									rc = SQLSetConnectAttr(NULL, SQL_OPT_TRACE,
										(SQLPOINTER)(LONG_PTR)(UDWORD)SQL_OPT_TRACE_ON,0);
								else
									rc = SQLSetConnectOption(NULL, SQL_OPT_TRACE, TRUE);

								if(rc == SQL_SUCCESS)
								{
									lpUsrOptions->fDMTrace = TRUE;
									CheckMenuItem(hMenu, IDM_TRACE, MF_CHECKED);
								}
								else
								{
									szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV0, TRUE, TRUE,
											GetGtrString(szOutBuff, MAXOUTBUFF, idsOptTraceFailed));

								}

							}
							else
							{
								szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV0, TRUE, TRUE,
										GetGtrString(szOutBuff, MAXOUTBUFF, idsOptTraceFileFailed),lpUsrOptions->szLog,_MAX_PATH);
							}

							if (RC_NOTSUCCESSFUL(rc))
							{
								LogReturnCode(NULL, ci, rc);
								ToolOptionsAllErrors(ci, henv, hdbc,hstmt,FALSE,rc);
							}

						}
					}
					// Else stop tracing
					else
					{
						SQLSetConnectOption(NULL, SQL_OPT_TRACE, FALSE);
						lpUsrOptions->fDMTrace = FALSE;
						CheckMenuItem(hMenu, IDM_TRACE, MF_UNCHECKED);
					}

					// Refresh the toolbar (in case the state changed)
					RefreshToolbar();

					// Reset focus to previous value to avoid disappearing edit cursor
					if(lpUsrOptions->hwndFocus)
						SetFocus(lpUsrOptions->hwndFocus);
					}
					return 0;

				case IDM_TOOLS_MANAGE_SOURCES:
					ManageSources(hwnd, hInst);
					return 0;

				case IDM_TOOLS_MANAGE_TESTS:
					ManageTests(hwnd, hInst);
					return 0;

				case IDM_TOOLS_MANAGE_GROUPS:
					ManageGroups(hwnd, hInst);
					return 0;

				case IDM_TOOLS_RUN_AUTO_TESTS:	// No connection window, so create a test output window
					{
					HENV				henv=NULL;
					HDBC				hdbc=NULL;
					HSTMT				hstmt=NULL;

					if(lpActiveConnWin)
					{
						hdbc = lpActiveConnWin->hdbc;
						henv = lpActiveConnWin->lpActiveEnv ?
								lpActiveConnWin->lpActiveEnv->henv : NULL;

						if(lpActiveConnWin->lpCurStmt)
							hstmt = lpActiveConnWin->lpCurStmt->hstmt;
					}

					// Bring the Output Window into focus since that is where output
					//	is shown.
					if(!IsZoomed(hwndMDIOutWin))
						SendMessage(hwndClient, WM_MDIRESTORE, (WPARAM)hwndMDIOutWin, 0L);
					SetFocus(hwndMDIOutWin);

					// Invoke test via gatortst.dll
					RunAutoTest(hwndOutWin, hInst,henv,  hdbc, hstmt,NULL,NULL);
					}
					return 0;

				case IDM_TOOLS_PLAYBACK:
					{

					}
					return 0;

				case IDM_TOOLS_RECORD:
					{
					BOOL					fAppend=TRUE;					// Default is to append
					TCHAR					szTitle[25];
					lpCONNECTIONINFO	ci = lpActiveConnWin;
					HENV					henv=GetActiveEnv();
					HDBC					hdbc=NULL;
					HSTMT					hstmt=NULL;

					//get ODBC handles
					if (ci)
					{
						hdbc=ci->hdbc;

						if (ci->lpCurStmt)
							hstmt=ci->lpCurStmt->hstmt;

					}

					// Save the handle to the focus window to avoid losing the cursor
					lpUsrOptions->hwndFocus = GetFocus();

					// If not tracing already, get a file name and start
					if(!lpUsrOptions->fRecord)
					{
						lpUsrOptions->fRecord=TRUE;
						CheckMenuItem(hMenu, IDM_TOOLS_RECORD, MF_CHECKED);

						// Set title and default log file
						GetGtrString((LPTSTR)szTitle,
                                     sizeof(szTitle)/sizeof(szTitle[0]), idsRecordLogFileTitle);

						if(!*lpUsrOptions->szRecLog)
							lstrcpy(lpUsrOptions->szRecLog, szRECDFTLOGNAME);

						// Now ask user for desired file
						if(GetRecordFile(hwnd, NULL, szTitle,
								(LPTSTR)szLOGFILTER, (LPTSTR)szLOGEXT, (LPTSTR)lpUsrOptions->szRecLog,
								OFN_NOCHANGEDIR, &fAppend))
						{
							// If user wants clean file, delete old one
							if(!fAppend)
								DeleteFile(lpUsrOptions->szRecLog);

						}
					}
					// Else stop tracing
					else
					{
						lpUsrOptions->fRecord = FALSE;
						CheckMenuItem(hMenu, IDM_TOOLS_RECORD, MF_UNCHECKED);
					}

					// Refresh the toolbar (in case the state changed)
					RefreshToolbar();

					// Reset focus to previous value to avoid disappearing edit cursor
					if(lpUsrOptions->hwndFocus)
						SetFocus(lpUsrOptions->hwndFocus);
					}
					return 0;


				//-----------------------------------------------------------------------
				//  Window
				//-----------------------------------------------------------------------
				case IDM_TILE:
					SendMessage(hwndClient, WM_MDITILE, 0, 0L);
					return 0;

				case IDM_TILE_HORZ:
					DoHorizontalTile(hwndClient);
					return 0;

				case IDM_CASCADE:
					SendMessage(hwndClient, WM_MDICASCADE, 0, 0L);
					return 0;

				case IDM_ARRANGE:
					SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0L);
					return 0;

				case IDM_CLOSEALL:
					{
					// Simply walk through the list of connections and free them
					while (lpConnWin && ConnectFullDisconnect(lpConnWin->next,SQL_SUCCESS))
											;
					}
					return 0;

				//---------------------------------------------------------------
				//  Help
				//---------------------------------------------------------------
				case IDM_TEST_HELP:
				//	iTestHelpCnt += InvokeHelp(hwnd, (LPCTSTR)szTestHelpFile,
				//								NULL);
					return 0;

				case IDM_API_HELP:
			//		iApiHelpCnt += InvokeHelp(hwnd, (LPCTSTR)szAPIHelpFile,
				//								NULL);
					return 0;

				case IDM_ABOUT:
					DisplayAbout(hwnd, hInst);
					return 0;


				default:
					break;
				};
			}
			break;


		//--------------------------------------------------------------------
		//  WM_CLOSE will take care of cleaning up our global objects.
		//--------------------------------------------------------------------
		case WM_CLOSE:
			FREE_PTR(lpInst);
			return DoShutDown(hwnd);

		case WM_DESTROY:
			{
			PostQuitMessage(0);
			}
			return 0;

		default:
			break;
		}
	return DefFrameProc(hwnd, hwndClient, msg, wParam, lParam);
}



//*---------------------------------------------------------------------------------
//| DoMenuSelect:
//|	This function will evaluate the parms recieved from WM_MENUSELECT and
//|	determine the appropriate action to take with the status bar text.  If a
//|	menu is just being displayed for the first time, then we will want to
//|	load and display that item.
//| Parms:
//|	uMenu						The menu number
//|	uFlags					The MF_xxx flags or -1 if menu is exiting
//|	hMenu						The menu handle
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoMenuSelect(UINT uMenu, UINT uFlags, HMENU hMenu)
{
	TCHAR 			szMenuText[MAXOUTBUFF];
	UINT			idsStr=0;

	// We are exiting the menu, there is no menu item, or
	//	there are other reasons to display nothing
	if(uFlags == (UINT)-1 	||
		uFlags & (MF_SEPARATOR | MF_MENUBREAK | MF_MENUBARBREAK)) {
		idsStr = 0;
		uMenuHelp = 0;
		}

	// System (control) menu is raised
	else if(uFlags & MF_SYSMENU) {
		// If it is the pop-up menu, then use general menu description
		if(uFlags & MF_POPUP) {
			idsStr = 0;
			uMenuHelp = 0;
			}
		// Else it is an item on the pop-up menu
		else
			idsStr = uMenu;
		}

	// Popup flag is set when the top level popup is displayed
	else if(uFlags & MF_POPUP) {
#ifndef WIN32
		UINT			dex;

		// For 16-bit, we only get the handle of the menu
		//	and therefore must search for the correct one
		for(dex=0;  dex<NUM_MENUS;  dex++)
			if(hSubMenus[dex] == (HMENU)uMenu)
				break;
		if(dex < NUM_MENUS)
			idsStr = dex+1;
#else
		// For Win32, they just give us the offset, no problem
		idsStr = GETMENUNUM(uMenu) + 1;
#endif
		uMenuHelp = idsStr + MENU_POPUP_HELP_BEGIN;
		}

	// If the uMenu matches the child windows, use a generic string
	else if(uMenu >= MENU_MDI_BEGIN &&
			  uMenu <= MENU_MDI_END)
		idsStr = MENU_MDI_BEGIN;

	// We are on a normal menu item
	else
		idsStr = uMenu;

	// Given the ids string, display the text if there is any
	*szMenuText = '\0';
	if(idsStr)
		GetGtrString((LPTSTR)szMenuText,
                     sizeof(szMenuText)/sizeof(szMenuText[0]), idsStr);
	SetStatBarText(hwndStatBar, (LPTSTR)szMenuText, BLACK_TEXT);
}


//*---------------------------------------------------------------------------------
//| DoHorizontalTile:
//|	Find the first ten non-iconzied child windows and tile then horizontally
//|	in the client area.
//| Parms:
//|	hwndClient				Client window
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN DoHorizontalTile(HWND hwndClient)
{
	HWND			hwndFocus;
	HWND			hwndMDI;
	HWND			hwndTile[MAX_TILED_WINDOWS];
	RECT			rect;
	TCHAR			szClass[25];
	int			iHeight;
	int			dex, cnt, ndex;

	// Clear out window array, then find both the current window with
	//		focus and the MDI window which owns it.  For example, if
	//		input edit window of conn window has focus, hwndFocus is
	//		edit window, hwndFocusParent is the conn window.
	memset((void *)&hwndTile, 0, sizeof(HWND) * MAX_TILED_WINDOWS);
	hwndFocus = GetFocus();

	// Restore the current MDI child before we tile
	SendMessage(hwndClient, WM_MDIRESTORE, (WPARAM)hwndCurMDIChild, 0L);

	// Loop through each child window and find the first ten non-iconized
	for(hwndMDI = GetWindow(hwndClient, GW_CHILD), dex=0;
		 dex < MAX_TILED_WINDOWS && hwndMDI && GetParent(hwndMDI) == hwndClient;
		 hwndMDI = GetWindow(hwndMDI, GW_HWNDNEXT)) {
		GetClassName(hwndMDI, szClass, sizeof(szClass)/sizeof(szClass[0]));
		if(!_tcsnccmp(szClass, szGATOR, lstrlen(szGATOR))
			&& !IsIconic(hwndMDI))
			hwndTile[dex++] = hwndMDI;
	}

	// dex now has the count of windows to tile.  Use this value to determine
	//	height of each, then tile them.  Note the list is in reverse order
	//	(oldest at bottom)
	if(dex) {
		GetClientRect(hwndClient, &rect);
		iHeight = (rect.bottom - rect.top) / dex;
		cnt = 0;
		//Fixed bug# 219: The window with focus tiles to the top.
		for (ndex =0; ndex < dex ; ndex++)
			MoveWindow(hwndTile[ndex],
							rect.left,
							iHeight * cnt++,
							rect.right,
							iHeight,
							TRUE);
		}

	// Now set focus first to the parent to activiate it, then to
	//		the child window within it.
	if(hwndFocus)
		SetFocus(hwndFocus);
}




//*------------------------------------------------------------------------
//| DisplayAbout:
//|     Display the about box for this app.
//*------------------------------------------------------------------------
void DisplayAbout(HWND hwnd, HINSTANCE hInst)
{
	if(-1 == DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, AboutWndProc,
			(LPARAM)(HINSTANCE * )&hInst))
		szMessageBox(hwnd, MB_ICONEXCLAMATION | MB_OK,
						 NULL, GetGtrString(szOutBuff, MAXOUTBUFF, idsDialogNotFound),
						 (LPTSTR)TEXT("About"));
}



//*------------------------------------------------------------------------
//| AboutWndProc:
//|     Handle the About messages
//*------------------------------------------------------------------------
LRESULT EXTFUN AboutWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x=10,y=30;
	static HINSTANCE * 	lpHINST;

	switch(msg) {
		case WM_INITDIALOG:
			{
			TCHAR			szMem[15];
			SIZE_T			dwFree;
#ifdef WIN32
			MEMORYSTATUS		mst;
#endif

			lpHINST = (HINSTANCE *)lParam;
			CenterDialog(hDlg);

			// Calculate free memory and display
#ifndef WIN32
			dwFree = GetFreeSpace(0) / 1024;
#else
			mst.dwLength = sizeof(mst);
			GlobalMemoryStatus(&mst);
			dwFree = mst.dwAvailVirtual / 1024;
#endif
			wsprintf((LPTSTR)&szMem, szKBFree, dwFree);
			SetDlgItemText(hDlg, IDT_MEM_AVAIL, (LPTSTR)szMem);
			}
			return TRUE;

		// Use paint routine to display the bitmap
		case WM_PAINT:
			{
			HDC			hdc;
			PAINTSTRUCT	ps;
			HBITMAP		hBitmap=NULL;

			hdc = BeginPaint(hDlg, &ps);
			if(hdc) {
				hBitmap = LoadBitmap(hInst, TEXT("ABOUTBITMAP"));
				if(hBitmap) {
					PatBlt(hdc, 12, 15, 64, 64, WHITENESS);
					DrawBitmap(hdc, hBitmap, 12, 15);
					DeleteObject(hBitmap);
					}
				EndPaint(hDlg, &ps);
				}
			}
			return TRUE;


		case WM_COMMAND:
			SendMessage(hDlg, WM_CLOSE, 0, 0L);
			return TRUE;


		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;


		default:
			return FALSE;
		}
}


//*---------------------------------------------------------------------------------
//| ResolveHelpFile:
//|	This function will look for the named help file first on the path,
//|	and if not found there, it will default to the specified relative path.
//| Parms:
//|	szHelpFile				User buffer to return full file name
//|	szHelpName				Name of the help file, no path information
//|	szRelativePath			The relative path to use if not on path
//| Returns:
//|	Pointer to szHelpFile
//*---------------------------------------------------------------------------------
LPTSTR INTFUN ResolveHelpFile(LPTSTR szHelpFile, LPCTSTR szHelpName, LPCTSTR szRelativePath)
{
	if (FileExist(szHelpName))
	{
		lstrcpy(szHelpFile, szHelpName);
	}
	else
	{
		// If not on path, use default path
		lstrcpy(szHelpFile, szRelativePath);
		lstrcat(szHelpFile, szHelpName);
	}

	return szHelpFile;
}


//*---------------------------------------------------------------------------------
//| ResetMenuStatus:
//|	This function will enable/disable menu items based on which menu is
//|	about to be displayed.
//| Parms:
//|	hMenu						Handle to the menu being displayed
//|	iMenu						Index to the menu
//|	iCalledFrom				Where were we called from?
//|									1		From main WndProc
//|									2		From connection window
//|									3		Test output window
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN ChkMenuStatus(HMENU hMenu, UINT apinum, UWORD *lpa, BOOL fHenv)
{
	UINT	uEnabled = MF_ENABLED,
			uRC=0;
	int	nOrdNum=GetSqlFuncOffset((UWORD)apinum),
			nIndex;



	if(lpUsrOptions->fDriverMenus)
	{
		nIndex = GetAPINumIndex((UWORD)apinum);
		nOrdNum = rgApiList[nIndex].iOrdinalNum;
		if(!(SQL_FUNC_EXISTS(lpa, apinum)))
			uEnabled = MF_GRAYED;
	}

	uRC=EnableMenuItem(hMenu,nOrdNum,uEnabled);

}

VOID INTFUN ResetMenuStatus(HMENU hMenu, int iMenu, int iCalledFrom)
{
	// Switch on the menu number, offset if zoomed
	switch(GETMENUNUM(iMenu)) {
		case IDM_FILE_MENU:
			{
			HWND			hwnd = lpActiveConnWin ? lpActiveConnWin->hwndIn : hwndOutWin;

			// Can open any time there is an edit window to open to
			EnableMenuItem(hMenu, IDM_FILE_OPEN,
						(!lpActiveResultWin) ? MF_ENABLED : MF_GRAYED);

			// Can only save when there are contents
			EnableMenuItem(hMenu, IDM_FILE_SAVEAS,
						((!lpActiveResultWin) && GetTrueWindowTextLength(hwnd,-1)) ?
						MF_ENABLED : MF_GRAYED);
			}
			break;

		case IDM_EDIT_MENU:
			{
			HWND		fhwnd=GetEditWindow();
			UINT		fStat = (fhwnd!=NULL) ? MF_ENABLED : MF_GRAYED;
         DWORD		sel = 0;
         UINT		fTextSel;


			// All are based on the focus being an edit window
         if(fhwnd)
         	sel = (DWORD)SendMessage(fhwnd, EM_GETSEL, 0, 0L);
         fTextSel = (fStat == MF_ENABLED && HIWORD(sel) != LOWORD(sel)) ? MF_ENABLED : MF_GRAYED;

			// Need text to undo
			EnableMenuItem(hMenu, IDM_EDIT_UNDO,
						(fStat == MF_ENABLED &&
						SendMessage(fhwnd, EM_CANUNDO, 0, 0L)) ? MF_ENABLED : MF_GRAYED);
			// Need selected text
			EnableMenuItem(hMenu, IDM_EDIT_CUT, fTextSel);
			EnableMenuItem(hMenu, IDM_EDIT_COPY, fTextSel);
			// Must have format available to paste
			EnableMenuItem(hMenu, IDM_EDIT_PASTE,
						(fStat == MF_ENABLED &&
						 IsClipboardFormatAvailable(CF_TEXT)) ? MF_ENABLED : MF_GRAYED);
			// Need selected text
			EnableMenuItem(hMenu, IDM_EDIT_DELETE, fTextSel);


			// Must have an active connection window, and there must be text in one of the edit windows
			if (lpActiveConnWin)
				ModifyMenu(hMenu,IDM_EDIT_CLEAR,MF_BYCOMMAND,IDM_EDIT_CLEAR,TEXT("Clea&r..."));
			else
				ModifyMenu(hMenu,IDM_EDIT_CLEAR,MF_BYCOMMAND,IDM_EDIT_CLEAR,TEXT("Clea&r Output Window"));

			// Need to have text to select
			EnableMenuItem(hMenu, IDM_EDIT_SELECT_ALL,
						(fStat == MF_ENABLED &&
						GetTrueWindowTextLength(fhwnd,-1)) ? MF_ENABLED : MF_GRAYED);

			// Disable WordWrap if a Result Window has the focus
			EnableMenuItem(hMenu, IDM_EDIT_WRAP, (lpActiveResultWin ? MF_GRAYED : MF_ENABLED));
			}
			break;

		case IDM_ENV_MENU:
			{
			UWORD						*lpa=InitMenuArray(lpActiveConnWin);
			lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
			HENV						henv= lpActiveEnv ? lpActiveEnv->henv: NULL;

			ChkMenuStatus(hMenu, SQL_API_SQLALLOCHANDLE, lpa, TRUE);
			ChkMenuStatus(hMenu, SQL_API_SQLFREEHANDLE, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETDIAGFIELD, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETDIAGREC, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLENDTRAN, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLERROR, lpa, TRUE);
			ChkMenuStatus(hMenu, SQL_API_SQLTRANSACT, lpa, TRUE);
			// Tools
			EnableMenuItem(hMenu, IDM_ALL_ERRORS,
							(henv) ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem(hMenu, IDM_DATA_SOURCES_ALL,
							(henv) ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem(hMenu, IDM_DRIVERS_ALL,
							(henv) ? MF_ENABLED : MF_GRAYED);
			}
			break;

		case IDM_CONNECT_MENU:
			{
			UWORD						*lpa=InitMenuArray(lpActiveConnWin);
			lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
			HENV						henv= lpActiveEnv ? lpActiveEnv->henv: NULL;
			BOOL						fConnected=FALSE;
			BOOL						fEnabled,
										fDesc=MF_GRAYED;

			if (lpActiveConnWin)
			{
				fConnected=lpActiveConnWin->uState & STATE_CONNECTED_HDBC;
				fDesc=lpActiveConnWin->lpDesc != NULL ? MF_ENABLED : MF_GRAYED;
			}

			fEnabled = fConnected ? MF_ENABLED : MF_GRAYED;

			// Standard items
			ChkMenuStatus(hMenu, SQL_API_SQLALLOCCONNECT, lpa, TRUE);
			ChkMenuStatus(hMenu, SQL_API_SQLCONNECT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLBROWSECONNECT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLDRIVERCONNECT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLDISCONNECT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLFREECONNECT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLFREEENV, lpa, TRUE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETINFO, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCOPYDESC, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETDESCFIELD, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETDESCFIELD, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETDESCREC, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETDESCREC, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETFUNCTIONS, lpa, FALSE);


			// Tools
			EnableMenuItem(hMenu, IDM_FULL_DISCONNECT, (lpActiveConnWin) ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem(hMenu, IDM_MAKE_TABLE,lpActiveConnWin ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem(hMenu, IDM_GET_INFO_ALL,fEnabled);
			EnableMenuItem(hMenu, IDM_GET_FUNCTIONS_ALL,fEnabled);
			EnableMenuItem(hMenu, IDM_GET_DESCRIPTOR_ALL, fDesc);
			EnableMenuItem(hMenu, IDM_LOAD_DESCRIPTOR_DEFAULTS, fDesc);
			EnableMenuItem(hMenu, IDM_CONNECT_SET_DRIVER_DEP_MENUS,fEnabled);
			}
			break;

		case IDM_STATEMENT_MENU:
			{
			UWORD				*lpa=InitMenuArray(lpActiveConnWin);
			UINT				uEnable=MF_GRAYED;

			// Standard items
			ChkMenuStatus(hMenu, SQL_API_SQLALLOCSTMT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPREPARE, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLBINDPARAMETER, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLNUMPARAMS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLDESCRIBEPARAM, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLEXECUTE, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLEXECDIRECT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETCURSORNAME, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETCURSORNAME, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPARAMDATA, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPUTDATA, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPARAMOPTIONS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLFREESTMT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLNATIVESQL, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLBINDPARAM, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCLOSECURSOR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCANCEL, lpa, FALSE);


			// Tools
			if(lpActiveConnWin &&
				lpActiveConnWin->lpCurStmt &&
				lpActiveConnWin->lpCurStmt->lpParams)
				uEnable = MF_ENABLED;
			EnableMenuItem(hMenu, IDM_FILL_PARAM, uEnable);
			EnableMenuItem(hMenu, IDM_SHOW_PARAMS, uEnable);


			if(lpActiveConnWin)
				EnableMenuItem(hMenu, IDM_SHOW_CURSOR_SETTINGS, MF_ENABLED);
			else
				EnableMenuItem(hMenu, IDM_SHOW_CURSOR_SETTINGS, MF_GRAYED);

			}
			break;

		case IDM_ATTR_MENU:
			{
			UWORD				*lpa=InitMenuArray(lpActiveConnWin);
			UINT				uEnable=MF_GRAYED;

			// Standard items
			ChkMenuStatus(hMenu, SQL_API_SQLSETSTMTATTR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETSTMTATTR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETSTMTOPTION, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETSTMTOPTION, lpa, FALSE);

			ChkMenuStatus(hMenu, SQL_API_SQLSETCONNECTATTR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETCONNECTATTR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETCONNECTOPTION, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETCONNECTOPTION, lpa, FALSE);

			ChkMenuStatus(hMenu, SQL_API_SQLSETENVATTR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETENVATTR, lpa, FALSE);

			if(lpActiveConnWin && lpActiveConnWin->lpCurStmt)
				uEnable = MF_ENABLED;

			EnableMenuItem(hMenu, IDM_SET_CURSOR_ATTR, uEnable);
			}
			break;

		case IDM_RESULTS_MENU:
			{
			UWORD				*lpa=InitMenuArray(lpActiveConnWin);
			UINT				uEnable=MF_GRAYED;
			UINT				uDataEnable=MF_GRAYED;
			lpSTATEMENTINFO	lpStmt=NULL;
			UINT				uRowsetEnable=MF_GRAYED;

			// Standard items
			ChkMenuStatus(hMenu, SQL_API_SQLNUMRESULTCOLS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLDESCRIBECOL, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCOLATTRIBUTE, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCOLATTRIBUTES, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLROWCOUNT, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLBINDCOL, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLFETCH, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLEXTENDEDFETCH, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETPOS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETDATA, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETNESTEDHANDLE, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLMORERESULTS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLNEXTCOLUMN, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSETSCROLLOPTIONS, lpa, FALSE);
#ifdef SQL_API_SQLLOCATOR
			ChkMenuStatus(hMenu, SQL_API_SQLLOCATOR, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETLENGTH, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETPOSITION, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETSUBSTRING, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLLOCATEDUPDATE, lpa, FALSE);
#endif
			ChkMenuStatus(hMenu, SQL_API_SQLFETCHSCROLL, lpa, FALSE);


			// Tools
			//		These items require a valid HSTMT, Show Bound Cols also requires that
			//		there be bound columns
			if(lpActiveConnWin &&lpActiveConnWin->lpCurStmt)
			{
				lpStmt=lpActiveConnWin->lpCurStmt;

				if (!lpStmt->fAutoDisplay && lpStmt->uBoundNodes)
				{
					uRowsetEnable=MF_ENABLED;
				}

				uEnable = MF_ENABLED;
			 	if ((lpStmt->wCurState==STMT_PREPARED) ||
					 (lpStmt->wCurState==STMT_EXECUTED))
				{
					uDataEnable=MF_ENABLED;
				}
			}

			EnableMenuItem(hMenu, IDM_DISPLAY_ROWSET,uRowsetEnable);

			EnableMenuItem(hMenu, IDM_FETCH_ALL, uDataEnable);
			EnableMenuItem(hMenu, IDM_GET_DATA_ROW, uDataEnable);
			EnableMenuItem(hMenu, IDM_GET_DATA_ALL, uDataEnable);
			EnableMenuItem(hMenu, IDM_DESCRIBE_COL_ALL,uDataEnable);
			EnableMenuItem(hMenu, IDM_BIND_COL_ALL,uDataEnable);

			EnableMenuItem(hMenu, IDM_SHOW_BOUND_COLS,
					(uEnable == MF_ENABLED && lpActiveConnWin->lpCurStmt->uBoundCols) ? uEnable : MF_GRAYED);
			EnableMenuItem(hMenu, IDM_UPDATE_CELL,
					(uEnable == MF_ENABLED && lpActiveResultWin) ? uEnable : MF_GRAYED);
 			}
			break;

		case IDM_CATALOG_MENU:
			{
			UWORD	*lpa=InitMenuArray(lpActiveConnWin);

			// Standard items
			ChkMenuStatus(hMenu, SQL_API_SQLGETTYPEINFO, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLTABLES, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCOLUMNS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSTATISTICS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSTRUCTUREDTYPES, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSTRUCTUREDTYPECOLUMNS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPRIMARYKEYS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLFOREIGNKEYS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLTABLEPRIVILEGES, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLCOLUMNPRIVILEGES, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLSPECIALCOLUMNS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPROCEDURES, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLPROCEDURECOLUMNS, lpa, FALSE);
			}
			break;

#if ODBCVER < 0x300
		case IDM_MISC_MENU:
			{
			UINT 						*lpa=InitMenuArray(lpActiveConnWin);
			lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
			HENV						henv=	lpActiveEnv?lpActiveEnv->henv:NULL;

			// Standard items
			ChkMenuStatus(hMenu, SQL_API_SQLCANCEL, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLGETFUNCTIONS, lpa, FALSE);
			ChkMenuStatus(hMenu, SQL_API_SQLERROR, lpa, TRUE);
			ChkMenuStatus(hMenu, SQL_API_SQLTRANSACT, lpa, TRUE);
			EnableMenuItem(hMenu, IDM_GET_FUNCTIONS_ALL,
							(lpActiveConnWin &&
							(lpActiveConnWin->uState & STATE_CONNECTED_HDBC)) ? MF_ENABLED : MF_GRAYED);

			EnableMenuItem(hMenu, IDM_ALL_ERRORS,
							(henv) ? MF_ENABLED : MF_GRAYED);
			}
			break;
#endif
		case IDM_TOOLS_MENU:
			{
			}
			break;

		// Output window always present
		case IDM_WINDOW_MENU:
			{
			UINT		uWindows=(lpActiveConnWin) ? MF_ENABLED : MF_GRAYED;
			EnableMenuItem(hMenu, IDM_CLOSEALL, uWindows);
			}
			break;

		case IDM_HELP_MENU:
			{
			}
			break;

		default:
#ifdef DOASSERTWARNINGS
			ONDEBUG(wsprintf(szOutBuff, TEXT("Invalid menu index %d"), iMenu));
			ASSERTCHECK(TRUE, szOutBuff);
#endif
			break;
		}
}



//*---------------------------------------------------------------------------------
//| InitMenuArray:
//|	Returns an array of UINTs which have one bit set for each supported function.
//|	Note that if the user does not have driver dependent menus enabled, this
//|	bitmask will have every bit set so all menus are enabled.
//| Parms:
//|	lpci						The connection info to work with, may be NULL
//| Returns:
//|	The bitmask to use
//*---------------------------------------------------------------------------------
UWORD * InitMenuArray(lpCONNECTIONINFO lpci)
{
	static UWORD 	uDftMenus[SQL_API_ODBC3_ALL_FUNCTIONS_SIZE];


	memset(uDftMenus,-1,SQL_API_ODBC3_ALL_FUNCTIONS_SIZE);

	if(!lpci ||	!lpUsrOptions->fDriverMenus)
		return (UWORD *)&uDftMenus;						// Return by default all enabled

	// May not yet be connected
	if(!(lpci->uState & STATE_CONNECTED_HDBC))
		return (UWORD *)&uDftMenus;

	if(lpci->puSupportedFunctions)
		return lpci->puSupportedFunctions;
	else if (RC_SUCCESSFUL(GetSupportedFunctions(lpci->hdbc, lpci)))
		return lpci->puSupportedFunctions;

	return (UWORD *)&uDftMenus;
}

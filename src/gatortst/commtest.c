//*---------------------------------------------------------------------------------
//|	File:		COMMTEST.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module contains common functions which the main Test API requires,
//|		including a linked list routines for tracking auto tests and their
//|		information.
//*---------------------------------------------------------------------------------
#define COMMTEST_C_DEFS
#pragma warning(disable:4996)
#include "gtrhdrs.h"



//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define szDEBUGFILE			TEXT("C:\\TMP\\MEM")
#define PROC_ARCH				TEXT("PROCESSOR_ARCHITECTURE")
#define WF_WINNT				0x4000		// WinFlag mask for WOW component of NT
#define WF_CPUR4000			0x0100		// WinFlag mask for the R4000 processor
#define WF_CPUALPHA			0x0200		// WinFlag mask for the Alpha processor
#define WIN32S					0x80000000	// GetVersion mask for Win32s
#define OS_WIN32S				0
#define OS_CHICAGO			1
#define OS_WINNT				2


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE			hThisInst;
TCHAR g_szGatorIni[MAXFILELEN];
TCHAR szOutBuff[MAXOUTBUFF];
TCHAR szErrBuff[MAXOUTBUFF];
TCHAR szLogFilter[MAXOUTBUFF];
TCHAR szLogTitle[MAXTITLE];


// Hook function pointers for processing F1=Help
FARPROC	lpfnFilterProc;						// Points to our filter proc
HHOOK		hOldHook=NULL;							// Points to previous default proc


dCSEG(TCHAR) szODBCTESTAPI[]		=	TEXT("ODBC Test API");
/*
dCSEG(TCHAR) szAUTOTESTNAME[]		=	TEXT("AutoTestName");
dCSEG(TCHAR) szAUTOTESTDESC[]		=	TEXT("AutoTestDesc");
dCSEG(TCHAR) szAUTOTESTFUNC[]		=	TEXT("AutoTestFunc");
*/
dCSEG(char) szAUTOTESTNAME[]		=	"AutoTestName";
dCSEG(char) szAUTOTESTDESC[]		=	"AutoTestDesc";
dCSEG(char) szAUTOTESTFUNC[]		=	"AutoTestFunc";

dCSEG(TCHAR) szCOMBOBOX[]			=  TEXT("ComboBox");
dCSEG(TCHAR) szEllipse[]				= 	TEXT("...");
static TCHAR	szFRAMECLASS[] 		= TEXT("GATOR:Frame");




// For common dialog browsing directories
typedef struct tagBROWSEINFO {
	OPENFILENAME 	*lpofn;              // Pointer to OPENFILENAME
	UINT					uFileOkString;       // FILEOKSTRING message
	LPCSTR				szPrompt;				// Prompt text, NULL if not required
} BROWSEINFO, * lpBROWSEINFO;


// Bug #336
OPENFILENAME ofnGlobal;
 UINT guAttrib;


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
lpAUTOTESTINFO FindAlphaLocation(lpAUTOTESTINFO head, LPTSTR szName);
UINT EXTFUN SaveFileHook(HWND	hDlg, UINT	msg, WPARAM	wParam, LPARAM	lParam);



//*------------------------------------------------------------------------
//|  IsWindowsWOW:
//|		Returns TRUE if running under the WOW component of Windows NT
//|  Parms:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
#ifdef WIN32
_inline BOOL EXTFUN IsWindowsWOW(void) {return FALSE;}
#else
BOOL EXTFUN IsWindowsWOW(void)
{
	DWORD	dwFlags;

	dwFlags = GetWinFlags();

	return (dwFlags & WF_WINNT) ? TRUE : FALSE;
}
#endif


VOID INTFUN GtrGetLastError(HWND hwnd,LPTSTR lpszMsg)
{
	TCHAR lpMessageBuffer[2048]=TEXT("");
	DWORD	dwErr=GetLastError();

	GtrFormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dwErr,
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				(LPTSTR)lpMessageBuffer,
				sizeof(lpMessageBuffer)/sizeof(lpMessageBuffer[0]),
				NULL);

	GtrMessageBox(hwnd,lpMessageBuffer,lpszMsg,MB_ICONEXCLAMATION | MB_OK);

} //GtrGetLastError()


//Bug# 316,313,314,328,329,330
//*------------------------------------------------------------------------
//|  IsChicago:
//|		Returns TRUE if running Chicago
//|  Pars:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
BOOL EXTFUN IsChicago(void)
{
	BOOL	fChicago=FALSE;

#ifndef WIN32
	DWORD	dwVer=GetVersion();
	WORD	dwMajorVer=HIWORD(dwVer),
			dwMinorVer=LOWORD(dwVer);

	if (!IsWindowsWOW())
		{
		if (dwMajorVer > 3 || (dwMajorVer == 3 && dwMinorVer >= 95))
			fChicago=TRUE;
		}

#else

	OSVERSIONINFO  VerInfo; 	// points to version information structure
	BOOL fRC=FALSE;

	memset(&VerInfo,0,sizeof(OSVERSIONINFO));
	VerInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	fRC=GetVersionEx(&VerInfo);

	if (VerInfo.dwPlatformId == OS_CHICAGO)
		fChicago=TRUE;

#endif

	return(fChicago);
}

#ifdef WIN32
LPTSTR EXTFUN DLLGetEnv ( LPTSTR lpszVariableName ) {return (NULL);}
BOOL EXTFUN IsWindowsIntel(void) {return FALSE;}
BOOL EXTFUN IsWindowsMIPS(void) {return FALSE;}
BOOL EXTFUN IsWindowsDEC(void) {return FALSE;}
#else

//*********************************************************************
//*  DLLGetEnv ( lpszVariableName )                                    *
//*                                                                    *
//*  Takes a LPTSTR to the name of an environment variable and returns  *
//*  the contents of that variable. Returns NULL if the environment    *
//*  variable does not exist. The search for the environment variable  *
//*  is case sensitive.                                                *
//*                                                                    *
//*********************************************************************
LPTSTR EXTFUN DLLGetEnv ( LPTSTR lpszVariableName )
{
	LPTSTR lpEnvSearch;
	LPTSTR lpszVarSearch;

	//	Check for a NULL pointer
	if ( !*lpszVariableName )
		return NULL;

	//	Get a pointer to the DOS environment block
	lpEnvSearch = GetDOSEnvironment ();

	//	While there are strings to parse
	while ( *lpEnvSearch )
	{
		//Make a copy of the pointer to the name of the
		//environment variable to search for.

		lpszVarSearch = lpszVariableName;

		//	Check to see if the variable names match
		while ( *lpEnvSearch && * lpszVarSearch &&
				*lpEnvSearch == *lpszVarSearch )
		{
			lpEnvSearch++;
			lpszVarSearch++;
		}
		// If the names match, the lpEnvSearch pointer is on the "="
		// TCHARacter and lpszVarSearch is on a null terminator.
		// Increment and return lpszEnvSearch, which will point to the
		// environment variable's contents.
		// If the names do not match, increment lpEnvSearch until it
		// reaches the end of the current variable string.
		if ( *lpEnvSearch == TEXT('=') && *lpszVarSearch == TEXT('\0') )
			return ( lpEnvSearch + 1 );
		else
			while ( *lpEnvSearch )
				lpEnvSearch++;

		// At this point the end of the environment variable's string
		// has been reached. Increment lpEnvSearch to move to the
		// next variable in the environment block. If it is NULL,
		// the end of the environment block has been reached.
		lpEnvSearch++;
	}

	// If this section of code is reached, the variable
	// was not found.

 	return NULL;
}


//*------------------------------------------------------------------------
//|  IsWindowsIntel:
//|		Returns TRUE if running NT on an Intel processor
//|  Parms:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
BOOL EXTFUN IsWindowsIntel(void)
{
    LPTSTR	lpszValue;
    DWORD   dwFlags;

    lpszValue = DLLGetEnv(PROC_ARCH);
    if( !lpszValue )
        {
	    dwFlags = GetWinFlags();
	    return (dwFlags & WF_WINNT) && ((dwFlags & WF_CPU386)
			  	|| (dwFlags & WF_CPU486)) ? TRUE : FALSE;
        }
    else
        return (_tcscmp(lpszValue,"x86")) ? FALSE : TRUE;

}


//*------------------------------------------------------------------------
//|  IsWindowsDEC:
//|		Returns TRUE if running NT on a DEC Alpha processor
//|  Parms:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
BOOL EXTFUN IsWindowsDEC(void)
{
	LPTSTR	lpszValue;
	DWORD	dwFlags;

	lpszValue = DLLGetEnv(PROC_ARCH);
	if( !lpszValue )
		{
		dwFlags = GetWinFlags();
		return (dwFlags & WF_WINNT) && (dwFlags & WF_CPUALPHA) ? TRUE : FALSE;
		}
	else
		return (_tcscmp(lpszValue, "ALPHA")) ? FALSE : TRUE;
}


//*------------------------------------------------------------------------
//|  IsWindowsMIPS:
//|		Returns TRUE if running NT on a MIPS R4000 processor
//|  Parms:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
BOOL EXTFUN IsWindowsMIPS(void)
{
	LPTSTR	lpszValue;
	DWORD	dwFlags;

	lpszValue = DLLGetEnv(PROC_ARCH);
	if( !lpszValue )
		{
		dwFlags = GetWinFlags();
		return (dwFlags & WF_WINNT) && (dwFlags & WF_CPUR4000) ? TRUE : FALSE;
		}
	else
		return (_tcscmp(lpszValue, "MIPS")) ? FALSE : TRUE;
}

#endif

//*------------------------------------------------------------------------
//|  IsWin32s:
//|		Returns TRUE if running under Win32s
//|  Parms:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
BOOL EXTFUN IsWin32s(void)
{
	return (GetVersion() & WIN32S) ? TRUE : FALSE;
}


//*------------------------------------------------------------------------
//|  ErrorMsg:
//|		Just display a message.
//|  Parms:
//|		szMsg			The string to display
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
void ErrorMsg(LPTSTR szMsg)
{
	GtrMessageBox(GetActiveWindow(), szMsg, szODBCTESTAPI, MB_OK);
}


//*------------------------------------------------------------------------
//|  AddAutoTestNode:
//|		This function will add an auto test to the list of loaded
//|		auto tests by either looking for the existing loaded test,
//|		or adding it if not found.
//|  Parms:
//|		head			The head node of the auto test linked list,
//|							NULL if this is the first
//|		szName		Name of the Auto Test DLL as found in .INI.  If
//|							NULL is specified, then szPath must have a valid
//|							value which is used to find the name
//|		szPath		Full path of the DLL to load, NULL to use .INI
//|		fUnload		TRUE if DLL should be unloaded when we're done
//|		rtnstatus	Indicates how the node was added using a bitmask of
//|						  one or more of:
//|							ADD_AT_NEW_IN_LIST
//|							ADD_AT_ALREADY_IN_LIST
//|							ADD_AT_PATH_DIFFERS
//|
//|  Returns:
//|		Pointer to the new node, NULL on error
//*------------------------------------------------------------------------
lpAUTOTESTINFO AddAutoTest(lpAUTOTESTINFO * head, LPTSTR szName, LPTSTR szPath,
			BOOL fUnload, int * rtnstatus)
{
	lpAUTOTESTINFO		ati;
//	int					rtn;

	// If node exists, simply return it
	if(*head && *szName)
		if(ati = FindAutoTestNode(*head, szName)) {
			if(!fUnload && !ati->hInst)
				LoadAutoTestDLL(ati, szName);
			if(rtnstatus)
				*rtnstatus = ADD_AT_ALREADY_IN_LIST;
			return ati;
			}

	// No existing node so add a new one
	ati = (lpAUTOTESTINFO)AllocateMemory(sizeof(AUTOTESTINFO));
	if(!ati)
		return NULL;						// Out of memory

	// Load the DLL and fill out the structure
	if(!szPath) {
		if(!GetTestDLLPath(szName, ati->szPath, sizeof(ati->szPath))) {
			szMessageBox(GetActiveWindow(),
							MB_ICONEXCLAMATION | MB_OK,
							(LPTSTR)szErrTitle,
							GetString((LPTSTR)szErrBuff, MAXOUTBUFF, idsAutoTestNotInIni),
							(LPTSTR)szName);
			ReleaseMemory(ati);
			return NULL;
			}
		}
	else
		lstrcpy(ati->szPath, szPath);

	// Try to load the auto test DLL
	if(!LoadAutoTestDLL(ati, (*szName) ? szName : szPath)) {
		ReleaseMemory(ati);
		return NULL;
		}


	// If we don't have the name yet, we were only given the path so call the
	// AutoTestName function to get the full name, then make sure it isn't
	//	already loaded.
	if(!*szName) {
		(*ati->lpTestName)(szName, &ati->cbTestCases);
		if(*head && *szName) {
			lpAUTOTESTINFO tmpati;
			if(tmpati = FindAutoTestNode(*head, szName)) {  // Already in our list, so return that node
				FreeLibrary(ati->hInst);
				ReleaseMemory(ati);
				if(rtnstatus)
					*rtnstatus = ADD_AT_ALREADY_IN_LIST;
				if(szPath &&
					!_tcscmp(szPath, tmpati->szPath))
					if(rtnstatus)
						*rtnstatus |= ADD_AT_PATH_DIFFERS;
				return tmpati;
				}
			}
		if(!IsValidTestName(szName)) {
			szMessageBox(GetActiveWindow(),
						MB_ICONINFORMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString((LPTSTR)szErrBuff, MAXOUTBUFF, idsAutoTestNameInvalid),
						(*szName) ? szName : szPath);
			ReleaseMemory(ati);
			return NULL;
			}
		}
	else			// We had the name, but still need the number of tests
		(*ati->lpTestName)(szName, &ati->cbTestCases);
	lstrcpy(ati->szTestName, szName);

	// Now add the node to the list
	if(*head) {								// There is a list already, add after head
		lpAUTOTESTINFO loc;				// Node to put in after
		loc = FindAlphaLocation(*head, szName);
		ati->next = loc;
		ati->prev = loc->prev;
		loc->prev = ati;
		ati->prev->next = ati;
		if(loc == *head)					// New head node
			*head = ati;
		}
	else {									// New list with no existing nodes
		ati->next = ati;
		ati->prev = ati;
		*head = ati;
		}

	if(rtnstatus)
		*rtnstatus = ADD_AT_NEW_IN_LIST;


	if(fUnload)
		UnLoadAutoTestDLL(ati);

	return ati;
}


//*------------------------------------------------------------------------
//|  LoadAutoTestDLL:
//|		Given a pointer to an auto test, load the DLL and get the
//|		address of each entry point.
//|  Parms:
//|		ati			Pointer to the node to load
//|		szName		The DLL being loaded, (either name or path)
//|
//|  Returns:
//|		ati if successful, NULL on failure
//*------------------------------------------------------------------------
lpAUTOTESTINFO LoadAutoTestDLL(lpAUTOTESTINFO ati, LPTSTR szName)
{
	HANDLE		hf=NULL;
	DWORD			dwWritten=0;

	if(!ati)
		return NULL;

	// Verify the file exists first, to avoid a pop-up error from Windows
	//		which will get orphaned
	hf=GtrCreateFile(ati->szPath,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,					// if not exist flag error
						FILE_ATTRIBUTE_NORMAL,		// file attributes
						0);				// handle of file with attributes to copy

	if (hf == INVALID_HANDLE_VALUE)
	{
		szMessageBox(GetActiveWindow(),
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString((LPTSTR)szErrBuff, MAXOUTBUFF, idsLibraryFileNotFound),
						(LPTSTR)ati->szPath, (LPTSTR)szName);
		return NULL;
	}

	//_lclose((HFILE)hf);
	CloseHandle(hf);

	// The library was found, so try to load.  Give error on failure
	ati->hInst = GtrLoadLibrary(ati->szPath);
	if(ati->hInst < (HINSTANCE)HINSTANCE_ERROR)
	{
		GtrGetLastError(GetActiveWindow(),TEXT("LoadLibrary Failed"));
		ati->hInst = NULL;
		return NULL;
	}

	ati->lpTestName = (lpAUTOTESTNAME)GetProcAddress(ati->hInst, (LPCSTR)szAUTOTESTNAME);
	ati->lpTestDesc = (lpAUTOTESTDESC)GetProcAddress(ati->hInst, (LPCSTR)szAUTOTESTDESC);
	ati->lpTestFunc = (lpAUTOTESTFUNC)GetProcAddress(ati->hInst, (LPCSTR)szAUTOTESTFUNC);

	if(!ati->lpTestName || !ati->lpTestDesc || !ati->lpTestFunc)
	{
		szMessageBox(GetActiveWindow(),
						MB_ICONEXCLAMATION | MB_OK,
						(LPTSTR)szErrTitle,
						GetString((LPTSTR)szErrBuff, MAXOUTBUFF, idsAutoTestGetProcFailed),
						szName);
		FreeLibrary(ati->hInst);
		ati->hInst = NULL;
		return NULL;
	}

	return ati;
}


//*------------------------------------------------------------------------
//|  UnLoadAutoTestDLL:
//|		Free the library and return.
//|  Parms:
//|		ati			Pointer to the node to unload
//|
//|  Returns:
//|		ati if successful, NULL on failure
//*------------------------------------------------------------------------
VOID UnLoadAutoTestDLL(lpAUTOTESTINFO ati)
{
	if(!ati)
		return;
	FreeLibrary(ati->hInst);
	ati->hInst = NULL;
	return;
}


//*------------------------------------------------------------------------
//|  FindAutoTestNode:
//|		This function will search for an Auto Test on the open test list
//|		using the name of the test.
//|  Parms:
//|		head			Head node in the list.
//|		szName		Name of the Auto Test DLL as found in .INI
//|  Returns:
//|		Pointer to the new node, NULL on error or not found
//*------------------------------------------------------------------------
lpAUTOTESTINFO FindAutoTestNode(lpAUTOTESTINFO head, LPTSTR szName)
{
	lpAUTOTESTINFO		ati;
	BOOL					fDone=FALSE;

	if(!head)
		return NULL;
	ati = head;
	while(!fDone) {
		if(_tcscmp(ati->szTestName, szName) == 0)
			return ati;
		ati = ati->next;
		fDone = (ati == head);
		}
	return NULL;
}


//*------------------------------------------------------------------------
//|  FindAlphaLocation:
//|		This function will search the auto test list based on the head
//|		value and return the node which would immediately proceede the
//|		name passed in.
//|  Parms:
//|		head			Head node in the list.
//|		szName		Name of the Auto Test DLL as found in .INI
//|  Returns:
//|		Pointer to the new node, NULL on error or not found
//*------------------------------------------------------------------------
lpAUTOTESTINFO FindAlphaLocation(lpAUTOTESTINFO head, LPTSTR szName)
{
	lpAUTOTESTINFO		ati;
	BOOL					fDone=FALSE;

	if(!head)
		return NULL;
	ati = head;
	while(!fDone) {
		if(_tcscmp(ati->szTestName, szName) > 0)
			return ati;
		ati = ati->next;
		fDone = (ati == head);
		}
	return ati;
}



//*------------------------------------------------------------------------
//|  DeleteAutoTestNode:
//|		This function will delete an Auto Test from the open test list
//|		based on a pointer to the node.
//|  Parms:
//|		ati				The node to delete
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
void DeleteAutoTestNode(lpAUTOTESTINFO * head, lpAUTOTESTINFO ati)
{
	// There is more than one node in the list
	if(ati->next != ati) {
		if(*head == ati)
			*head = ati->next;
		ati->next->prev = ati->prev;
		ati->prev->next = ati->next;
		}
	else
		*head = NULL;					// No longer a list
	if(ati->hInst)
		FreeLibrary(ati->hInst);
	ReleaseMemory(ati);
}



//*------------------------------------------------------------------------
//|  DeleteAutoTestList:
//|		This function will walk an entire auto test list from head and
//|		remove all nodes.
//|  Parms:
//|		head				A node to start at (suggest using head)
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
void DeleteAutoTestList(lpAUTOTESTINFO head)
{
	lpAUTOTESTINFO  ati;
	if(!head)
		return;
	ati = head->next;
	while(head) {
		DeleteAutoTestNode(&head, ati);
		if(head)
			ati = head->next;
		}
}



//*------------------------------------------------------------------------
//|  VerifyTestCaseCount:
//|		This function will compare the installed test case count to
//|		the value the auto test itself returns. If a descrepency is
//|		found, the user has the change to update the value.
//|	Parms:
//|		szAutoTest			The auto test name to load
//|		ldcnt					The loaded count
//|		fRun					TRUE if detected while running the test
//|	Returns:
//|		FALSE if the values did not match, TRUE if they were correct
//*------------------------------------------------------------------------
BOOL INTFUN VerifyTestCaseCount(LPTSTR szAutoTest, UINT *pldcnt, BOOL fRun)
{
	UINT acnt = GetTestCaseCount(szAutoTest);

	if(acnt != *pldcnt)
	{
		TCHAR tmp[5];
		HWND	hwnd = GetActiveWindow();
		if(IDYES == szMessageBox(hwnd,
							MB_ICONQUESTION | MB_YESNO,
							(LPTSTR)szErrTitle,
							GetString(szErrBuff, MAXOUTBUFF, idsTestCaseCountMismatch),
							acnt, szAutoTest, *pldcnt, *pldcnt))
		{
			AddTestNameToIni(szAutoTest, _itot(*pldcnt, tmp, 10));
			szMessageBox(hwnd,
							MB_ICONINFORMATION | MB_OK,
							(LPTSTR)szErrTitle,
							GetString(szErrBuff, MAXOUTBUFF, (fRun) ? idsTestCaseUpdatedRun : idsTestCaseUpdated),
							szAutoTest);
		}
		else
			szMessageBox(hwnd,
							MB_ICONINFORMATION | MB_OK,
							(LPTSTR)szErrTitle,
							GetString(szErrBuff, MAXOUTBUFF, idsTestCaseNotCorrect),
							szAutoTest);

		//We don't need the new count until the next time
		//the autotest is loaded.
		*pldcnt = acnt;

		return FALSE;
	}
	else
		return TRUE;
}


//*------------------------------------------------------------------------
//|  RefreshInstalledTestList:
//|		This function will grab all of the installed tests from the
//|		.INI file, put them in a list box, and finally create a
//|		list of auto tests which are valid and loaded.
//|  Parms:
//|		hDlg				Dialog box which owns list box
//|		id					Control id of list box to fill
//|		lpHead			Pointer to the head node
//|  Returns:
//|		Number of items added to the list box.
//*------------------------------------------------------------------------
int INTFUN RefreshInstalledTestList(HWND hDlg, int id, lpAUTOTESTINFO * lpHead)
{
	VOID INTFUN RemoveTestsFromGroups(LPTSTR szTest);			// Found in DLLPICK.C
	lpAUTOTESTINFO	ati;
	int 	cbCnt=0;
	HWND 	hwnd;
	LPTSTR	str, astr;

	// First the list we have in the .INI file
	hwnd = GetDlgItem(hDlg, id);
	astr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF);
	if(str) {
		cbCnt = 0;
		SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
		if(GetInstalledTestList(str, MAXTESTBUFF))
			while(*astr) {
				if(!(ati = AddAutoTest(lpHead, astr, NULL, TRUE, NULL))) {
					// If we couldn't load the test, it may be invalid.  Ask user to remove it
					if(IDOK == szMessageBox(hDlg,
									MB_ICONQUESTION | MB_OKCANCEL,
									(LPTSTR)szErrTitle,
									GetString(szErrBuff, MAXOUTBUFF, idsAutoTestNotValid),
									astr)) {
						RemoveTestSectionFromIni(astr);
						RemoveTestNameFromIni(astr);
						RemoveTestsFromGroups(astr);
						}
					}
				else {
					++cbCnt;
					SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)astr);
					VerifyTestCaseCount(astr, &(ati->cbTestCases), FALSE);
					}
				astr += lstrlen(astr) + 1;
				}
		}
	SendMessage(hwnd, LB_SETSEL, TRUE, MAKELPARAM(0,0));
	ReleaseMemory(str);

	return cbCnt;
}


//*------------------------------------------------------------------------
//|  IsValidTestName:
//|		Parse a potential test name and look for bad TCHARacters
//|  Parms:
//|		szName			The auto test name
//|  Returns:
//|		TRUE if name is ok, FALSE on error
//*------------------------------------------------------------------------
BOOL IsValidTestName(LPTSTR szName)
{
	LPTSTR	str = szName;

	// No reserved section names, empty strings, or leading blanks
	if(!lstrcmpi(szName, szAUTOTESTS) 	||
		!lstrcmpi(szName, szDLL)       	||
		!lstrcmpi(szName, szGROUPS)    	||
		!lstrcmpi(szName, szSQLDRIVERS)	||
		!lstrcmpi(szName, szSTARTUP)		||
		!lstrcmpi(szName, szODBCTESTHANDLE) ||
		*szName == TEXT('\0')					 	||
		*szName == TEXT(' '))
		return FALSE;

	// And invalid TCHARacters in name
	while(*str) {
		switch(*str) {
			case TEXT('['):
			case TEXT(']'):
			case TEXT('='):
				return FALSE;
			}
		str = CharNext(str);
		}
	return TRUE;
}


//*------------------------------------------------------------------------
//|  GetIniNameUsage:
//|		This function takes and name and returns one of the following
//|		flags to indicate its usage:
//|				INI_STRING_NOT_FOUND
//|				INI_DATA_SOURCE
//|				INI_AUTO_TEST
//|				INI_GROUP_NAME
//|  Parms:
//|		szName			The auto test name
//|  Returns:
//|		One of the above values
//*------------------------------------------------------------------------
UINT INTFUN GetIniNameUsage(LPTSTR szName)
{
	TCHAR		szBuf[MAXOUTBUFF];

	if(GetSourceName(szName, szBuf, sizeof(szBuf)/sizeof(szBuf[0])))
		return INI_DATA_SOURCE;
	else if(GetTestNameFromIni(szName, szBuf, sizeof(szBuf)/sizeof(szBuf[0])))
		return INI_AUTO_TEST;
	else if(GetGroupNameFromIni(szName, szBuf, sizeof(szBuf)/sizeof(szBuf[0])))
		return INI_GROUP_NAME;
	else
		return INI_STRING_NOT_FOUND;
}


//*------------------------------------------------------------------------
//|  FlushBuffer:
//|		Will write out the buffered text.  Will try to do everything
//|		including the new string.
//|	Parms:
//|		lps			Server information
//|		str			String to write
//|		newlen		Length of new string
//|		fNew			Newline char as well?
//|	Returns:
//|		TRUE if everything, including str, was written, else
//|		FALSE if only buffer was written and str would not fit
//*------------------------------------------------------------------------
BOOL INTFUN FlushBuffer(lpSERVERINFO lps, LPTSTR str, DWORD newlen, BOOL fNew)
{
	BOOL		rtn=TRUE;
	if(newlen + lps->cBuff + 2 < MAXFLUSH) {
		lstrcpy(&lps->szBuff[lps->cBuff], str);
		lps->cBuff += (UINT)newlen;
		if(fNew) {
			lstrcpy(&lps->szBuff[lps->cBuff], (LPTSTR)szNewLine);
			lps->cBuff += 2;
			}
		}
	else
		rtn = FALSE;                    // Can only write buffered text, not str
	lps->szBuff[lps->cBuff] = TEXT('\0');
	SendMessage(lps->hwnd, EM_REPLACESEL, 0, (LPARAM)(LPCSTR)lps->szBuff);
	*lps->szBuff = TEXT('\0');
	lps->cBuff = 0;
	return rtn;
}


//*------------------------------------------------------------------------
//|  DoszWrite:
//|		Check user logging options and log to an output file and/or
//|		the edit window.  Checks are done for truncation of the edit
//|		window and for file errors with the log file.
//|	Parms:
//|		lps			Server information
//|		str			String to write
//|		fForce			TRUE if screen output should be done regardless
//|		fNew			Newline char as well?
//|	Returns:
//|		TRUE if successful, FALSE on a failure
//*------------------------------------------------------------------------
BOOL EXTFUN DoszWrite(lpSERVERINFO lps, LPTSTR str, BOOL fForce, BOOL fNew)
{
#define MAXOUTPUTBUFF 2000

	BOOL		rtn=TRUE;									// Assume best case
	CHAR		strA[MAXOUTPUTBUFF];

	// For logging, attempt to write to the file.  If an error occurs,
	//		ask user to verify that we should continue
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,str,-1,strA,MAXOUTPUTBUFF,NULL,NULL);
#else
	strcpy(strA,str);
#endif

	if(lps->fLog && *lps->szLogFile)
	{
		HANDLE		hf;
		LPTSTR		szLogFile = (LPTSTR)lps->szLogFile;
		DWORD			dwWritten=0;

		hf=GtrCreateFile(szLogFile,
							GENERIC_WRITE,
							FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,					// if not exist create and open it in Write mode
							FILE_ATTRIBUTE_NORMAL,		// file attributes
							0);				// handle of file with attributes to copy

		if (hf == INVALID_HANDLE_VALUE)
		{
			if(IDNO == szMessageBox(GetActiveWindow(),
							MB_ICONEXCLAMATION | MB_YESNO,
							(LPTSTR)szErrTitle,
							GetString(szErrBuff, MAXOUTBUFF, idsLogFileNotValid),
							szLogFile))
			{
				lps->fLog = FALSE;
			}

			rtn = FALSE;
		}

		//_llseek((HFILE)hf, 0L, 2);
		//_lwrite((HFILE)hf,(LPCSTR)strA,strlen(strA));
		SetFilePointer(hf,0L,0L,FILE_END);
		WriteFile(hf,strA,strlen(strA),&dwWritten,NULL);

		if(fNew)
		{
//			_llseek((HFILE)hf, 0L, 2);
//			_lwrite((HFILE)hf,(LPCSTR)"\r\n",strlen("\r\n"));
			SetFilePointer(hf,0L,0L,FILE_END);
			WriteFile(hf,"\r\n",strlen("\r\n"),&dwWritten,NULL);

		}

		CloseHandle(hf);
	}

	// For output to the screen, if there is no new-line, then try to
	//		bufferthe string until later to avoid excessive painting.
	if(lps->fScreen || fForce) {
		DWORD		newlen=_tcslen(str);			// Note: lstrlen gets confused on huge strings

		// If we are finished with this line or the line is getting too large
		//   for our buffer, then flush it to the screen
		if(fNew || _tcschr(str, TEXT('\n')) || (newlen + lps->cBuff + 2) >= MAXFLUSH) {
			// Check for truncation, then write the string
			DWORD		oldlen=(DWORD)SendMessage(lps->hwnd, WM_GETTEXTLENGTH, 0, 0L);

			if((DWORD)(newlen + oldlen + lps->cBuff + 2) >=
				(DWORD)(MAX_EDIT_WINDOW_TEXT)) {
				DWORD		len;
				SendMessage(lps->hwnd, EM_SETSEL, PORT_EM_SETSEL(0,2000));
				GetString(szErrBuff, MAXOUTBUFF, idsTruncated);
				SendMessage(lps->hwnd, EM_REPLACESEL, 0, (LPARAM)(LPTSTR)szErrBuff);
				len = (DWORD)SendMessage(lps->hwnd, WM_GETTEXTLENGTH, 0, 0L);
				SendMessage(lps->hwnd, EM_SETSEL, PORT_EM_SETSEL(len, len));
				}
			// Flush buffer if required.  If FlushBuffer can handle everything, we're done
			if(lps->cBuff)
				if(FlushBuffer(lps, str, newlen, fNew))
					return rtn;
			// Either there was no buffered text or we couldn't flush everything, so write string
			SendMessage(lps->hwnd, EM_REPLACESEL, 0, (LPARAM)(LPCSTR)str);
			if(fNew)
				SendMessage(lps->hwnd, EM_REPLACESEL, 0, (LPARAM)(LPTSTR)szNewLine);
			}
		// No need to flush the line yet, so simply add it to our buffer
		else {
			lstrcpy(&lps->szBuff[lps->cBuff], str);
			lps->cBuff += (UINT)newlen;
			}
		}

	return rtn;
}


//*------------------------------------------------------------------------
//|  szLogPrintf:
//|		Takes a variable list just like sprintf to format an output
//|		string.  The szWrite function is used for the actual write.
//|  Parms:
//|		lps				SERVERINFO struct containing output options
//|		fForce			TRUE to force screen output
//|		szFmt				Format string
//|		(varying)		Arguements for format string
//|  Returns:
//|		TRUE if successful, else FALSE
//*------------------------------------------------------------------------
BOOL EXTFUNCDECL CDECL szLogPrintf(lpSERVERINFO lps, BOOL fForce, LPTSTR szFmt, ...)
{
#define MAXOUTPUTBUFF 2000
	TCHAR			szBuff[MAXOUTPUTBUFF];
	va_list		marker;

	// Check for overflow
	if(lstrlen(szFmt) > MAXOUTPUTBUFF)
		lstrcpy(szFmt, TEXT("*** Buffer overflow ***\r\n"));

	// Use the argument list to format the output buffer
   va_start(marker, szFmt);
   _vsntprintf((LPTSTR)szBuff, MAXOUTPUTBUFF, szFmt, marker);
   va_end(marker);

	// Call DoszWrite to handle the actual write
	return(DoszWrite(lps, szBuff, fForce, FALSE));
}



//*------------------------------------------------------------------------
//|  szMessageBox:
//|		Works like sprintf only the output goes to a message box.
//|  Parms:
//|		hwnd				Owner window, NULL uses GetActiveWindow
//|		style 			Flags for GtrMessageBox
//|		szTitle			Title for message box, NULL for "Error"
//|		szFmt				Format string
//|		(varying)		Arguements for format string
//|  Returns:
//|		Id from GtrMessageBox
//*------------------------------------------------------------------------
int EXTFUNCDECL CDECL szMessageBox(HWND hwnd, UINT style, LPTSTR szTitle, LPTSTR szFmt, ...)
{
#define MAXOUTPUTBUFF 2000
	TCHAR 			szBuffer[MAXOUTPUTBUFF];
	LPTSTR			pszBuffer;
	va_list		marker;

	pszBuffer = &szBuffer[0];

	// Use format and arguements as input
	va_start(marker, szFmt);
	if (_vsntprintf(pszBuffer, MAXOUTPUTBUFF, szFmt, marker) < 0)
		wsprintf(pszBuffer,TEXT("Buffer overflow reporting '%*.*s'"), 50,50,szBuffer);
	va_end(marker);

   return(GtrMessageBox((hwnd) ? hwnd : GetActiveWindow(),
   				pszBuffer,
   				(szTitle != NULL) ? szTitle : szErrTitle,
   				style));
}


//*------------------------------------------------------------------------
//|  ControlSzWrite:
//|		Sends various messages to the control window
//|  Parms:
//|		hwnd				Window handle to use
//|		iFscreen			Screen control
//|		iFlog				Log control
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID EXTFUN ControlSzWrite(HWND hwnd, int iFscreen, int iFlog)
{
#define WM_CONTROLSZW WM_USER + 3
	SendMessage(hwnd,
				WM_CONTROLSZW,
				(WPARAM) 0,
				(LPARAM) MAKELONG(iFscreen, iFlog));

	return;
}

//*------------------------------------------------------------------------
//|  FAbortCheck:
//|		Looks for a press of the Escape key
//|  Parms:
//|		None
//|  Returns:
//|		Nothing
//*------------------------------------------------------------------------
BOOL EXTFUN FAbortCheck()
{
	static BYTE sz_Continuing[] = "Continuing...";
	static BYTE sz_Paused[]     =
		"Paused - hit any key to continue, or Esc to abort.";

	BOOL fAbort = FALSE;
	MSG msg;

	while(PeekMessage(&msg, (HWND) NULL, WM_KEYFIRST, WM_KEYLAST,PM_REMOVE))
		if(msg.message==WM_KEYDOWN)
			switch (msg.wParam) {
				case VK_ESCAPE: 	// break out of the current loop
					fAbort = TRUE;
					while(GetMessage(&msg, (HWND) NULL, WM_KEYFIRST, WM_KEYLAST) &&
						  msg.message != WM_KEYUP);
					break;

#ifdef ILLGETTOIT
				case VK_PAUSE:		// wait for another keystroke to continue
					ControlSzWrite(ENABLE | SAVE, SAME);
					szWrite(sz_Paused, TRUE);
					while(GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST) &&
						  msg.message != WM_KEYDOWN);
					fAbort = msg.wParam == VK_ESCAPE;
					if (!fAbort)
						szWrite(sz_Continuing, TRUE);

					while (PeekMessage(
						&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE));
					ControlSzWrite(RESTORE, SAME);
					break;
#endif
					}		// Switch on message

	return fAbort;
}



//*------------------------------------------------------------------------
//|  CElapsed:
//|		Finds the elapsed time between two time structs.
//|  Parms:
//|		start			The start time struct
//|		end			The ending time struct
//|		elapsed		The elapsed time struct
//|  Returns:
//|		A pointer to the elapsed time
//*------------------------------------------------------------------------
TIMEINFO * EXTFUNCDECL CElapsed(lpTIMEINFO start, lpTIMEINFO end, lpTIMEINFO elapsed)
{
	if(start->PORT_HOUR > end->PORT_HOUR) {
		end->PORT_HOUR += 24;
	}
	elapsed->PORT_HOUR = end->PORT_HOUR - start->PORT_HOUR;

	if(start->PORT_MINUTE > end->PORT_MINUTE) {
		elapsed->PORT_HOUR--;
		end->PORT_MINUTE += 60;
	}
	elapsed->PORT_MINUTE = end->PORT_MINUTE - start->PORT_MINUTE;

	if(start->PORT_SECOND > end->PORT_SECOND) {
		elapsed->PORT_MINUTE--;
		end->PORT_SECOND += 60;
	}
	elapsed->PORT_SECOND = end->PORT_SECOND - start->PORT_SECOND;

#ifdef WIN32
	if((start->wMilliseconds*10) > (end->wMilliseconds*10)) {
		elapsed->PORT_SECOND--;
		end->wMilliseconds += 1000;
	}
	elapsed->wMilliseconds = end->wMilliseconds - start->wMilliseconds;
#else
	if(start->PORT_HSECOND > end->PORT_HSECOND) {
		elapsed->PORT_SECOND--;
		end->PORT_HSECOND += 100;
	}
	elapsed->PORT_HSECOND = end->PORT_HSECOND - start->PORT_HSECOND;
#endif

	return elapsed;
}



//*------------------------------------------------------------------------
//| CenterDialog:
//|		Center the dialog over the parent window.  It is possible that
//|			their is not a parent window, since the Translate DLL was
//|			never meant to display data.  For this reason, we may just
//|			center over the entire screen.
//*------------------------------------------------------------------------
void EXTFUN CenterDialog(HWND hDlg)
{
	RECT  rcParent;                         // Parent window client rect
	RECT  rcDlg;                            // Dialog window rect
	int   nLeft, nTop;                      // Top-left coordinates
	int   cWidth, cHeight;                  // Width and height
	HWND	hwnd;

	// Get frame window client rect in screen coordinates
	if((hwnd = GetParent(hDlg)) == NULL) {
		rcParent.top = rcParent.left = 0;
		rcParent.right = GetSystemMetrics(SM_CXFULLSCREEN);
		rcParent.bottom = GetSystemMetrics(SM_CYFULLSCREEN);
		}
	else
		GetWindowRect(hwnd, &rcParent);

	// Determine the top-left point for the dialog to be centered
	GetWindowRect(hDlg, &rcDlg);
	cWidth  = rcDlg.right  - rcDlg.left;
	cHeight = rcDlg.bottom - rcDlg.top;
	nLeft   = rcParent.left +
            (((rcParent.right  - rcParent.left) - cWidth ) / 2);
	nTop    = rcParent.top  +
            (((rcParent.bottom - rcParent.top ) - cHeight) / 2);
	if (nLeft < 0) nLeft = 0;
	if (nTop  < 0) nTop  = 0;

	// Place the dialog
	MoveWindow(hDlg, nLeft, nTop, cWidth, cHeight, TRUE);
	return;
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
LPTSTR INTFUN FindFileDir(LPTSTR szFile, LPTSTR szPath)
{
	DWORD		fFound=FALSE;
	TCHAR		szBuffer[_MAX_PATH]=TEXT("");

	fFound=GtrSearchPath(NULL,						// address of search path
							szFile,					// address of filename
							NULL,						// address of extension
							_MAX_PATH,				// size, in characters, of buffer
							szBuffer,				// address of buffer for found filename
							NULL);					// address of pointer to file component

	if (!fFound)
		return NULL;

	lstrcpy(szPath, szBuffer);

	return szPath;
}



VOID INTFUN SetupFileInfo(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
		LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL * lpfAppend,
		OPENFILENAME *lpofn)
{

	TCHAR				szTmpFile[_MAX_PATH];
	DWORD				dwMode=CREATE_ALWAYS;
	FILE				*fhRecFile=NULL;
	DWORD				dwWritten=0;

	if(*szFile)
		lstrcpy((LPTSTR)szTmpFile, szFile);
	else
		memset((LPTSTR)szTmpFile, 0, sizeof(szTmpFile));

	memset(lpofn, 0, sizeof(OPENFILENAME));

	//Bug #336
	ofnGlobal.lStructSize	= lpofn->lStructSize		= sizeof(OPENFILENAME);
	ofnGlobal.hwndOwner		= lpofn->hwndOwner		= hwnd;
	ofnGlobal.hInstance		= lpofn->hInstance		= (!hInst) ? hThisInst : hInst;
	ofnGlobal.lpstrTitle		= lpofn->lpstrTitle		= szTitle;
	ofnGlobal.lpstrFilter	= lpofn->lpstrFilter		= szFilter;
	ofnGlobal.lpstrFile		= lpofn->lpstrFile		= szTmpFile;
	ofnGlobal.nMaxFile		= lpofn->nMaxFile			= sizeof(szTmpFile);
	ofnGlobal.lpstrDefExt	= lpofn->lpstrDefExt		= szDftExt;

	if (IsChicago()) //Bug #429
	{
		ofnGlobal.Flags			= lpofn->Flags			=OFN_HIDEREADONLY |
																OFN_OVERWRITEPROMPT | //Bug #340
																OFN_NOREADONLYRETURN |
																OFN_CREATEPROMPT |
																OFN_PATHMUSTEXIST |
																uAttrib;
	}
	else
	{
		ofnGlobal.Flags			= lpofn->Flags			=OFN_ENABLETEMPLATE |
																	OFN_ENABLEHOOK |
																	OFN_OVERWRITEPROMPT | //Bug #340
																	OFN_HIDEREADONLY |
																	OFN_NOREADONLYRETURN |
																	OFN_CREATEPROMPT |
																	OFN_PATHMUSTEXIST |
																	uAttrib;
		ofnGlobal.lCustData		= lpofn->lCustData		= *lpfAppend; //Bug #341
		ofnGlobal.lpfnHook		= lpofn->lpfnHook		= (LPOFNHOOKPROC)SaveFileHook;
		ofnGlobal.lpTemplateName= lpofn->lpTemplateName	= MAKEINTRESOURCE(IDD_GETLOG);
	}


} //SetupFileInfo()


//*---------------------------------------------------------------------------------
//| GetLogFile:
//|	Invoke common dialog function to open a file which must exist.
//| Parms:
//|	hwnd					Window handle for displays
//|	hInst					Instance handle of this DLL (CTL3D requirement)
//|	szTitle				Title for dialog
//|	szFilter				A filter for file names
//|									Ex:  "Log Files|*.LOG|SQL Files|*.SQL|"
//|	szDftExt				The default extension, eg: "log"
//|	szFile				Output for file name if successful
//|	uAttrib				Add'l style beyond default
//| Returns:
//|	TRUE if a new file was found, false otherwise
//*---------------------------------------------------------------------------------
BOOL EXTFUN GetLogFile(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
			LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL * lpfAppend)
{
	OPENFILENAME	ofn;
	TCHAR				szTmpFile[MAXFILELEN];

	if(*szFile)
		lstrcpy((LPTSTR)szTmpFile, szFile);
	else
		memset((LPTSTR)szTmpFile, 0, MAXFILELEN);
	memset(&ofn, 0, sizeof(ofn));

	//Bug #336
	ofnGlobal.lStructSize	= ofn.lStructSize		= sizeof(OPENFILENAME);
	ofnGlobal.hwndOwner		= ofn.hwndOwner		= hwnd;
	ofnGlobal.hInstance		= ofn.hInstance		= (!hInst) ? hThisInst : hInst;
	ofnGlobal.lpstrTitle		= ofn.lpstrTitle		= szTitle;
	ofnGlobal.lpstrFilter	= ofn.lpstrFilter		= szFilter;
	ofnGlobal.lpstrFile		= ofn.lpstrFile		= szTmpFile;
	ofnGlobal.nMaxFile		= ofn.nMaxFile			= sizeof(szTmpFile);
	ofnGlobal.lpstrDefExt	= ofn.lpstrDefExt		= szDftExt;
	ofnGlobal.Flags			= ofn.Flags				=//OFN_EXPLORER;
																	OFN_ENABLETEMPLATE |
																//	OFN_ENABLEHOOK |
																	OFN_HIDEREADONLY |
																	OFN_NOREADONLYRETURN |
																	OFN_CREATEPROMPT |
																	OFN_PATHMUSTEXIST |
																	uAttrib;
//	ofnGlobal.lpfnHook		= ofn.lpfnHook			= NULL;
	ofnGlobal.lpTemplateName= ofn.lpTemplateName	= MAKEINTRESOURCE(IDD_GETLOG);

	if(GetSaveFileName(&ofn)) {
//		OFSTRUCT ofs;
		HFILE		hFile=0;

		lstrcpy(szFile, ofn.lpstrFile);
		//Bug #339
		*lpfAppend=(BOOL)ofnGlobal.lCustData;

		//Bug #339
		//if user doesn't want to append then clear file
		/*
		if (!*lpfAppend)
		{
			hf=GtrCreateFile(szFile,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,					// if not exist flag error
						FILE_ATTRIBUTE_NORMAL,		// file attributes
						0);				// handle of file with attributes to copy
		}
		*/

		return TRUE;
		}
	return FALSE;				// Canceled or whatever
}


//*---------------------------------------------------------------------------------
//| SaveFileHook:
//|	Hook function for GetSaveFileName to give it 3d affect.  Note that caller
//|	will pass the address of a BOOL in the lCustData member of an OPENFILENAME
//|	structure.  We will save that value (passed to us by WM_INITDIALOG) and
//|	set the Append check box accordingly.
//| Parms:
//|	hDlg							Window handle
//|	msg							Window message
//|	wParam						Standard value
//|	lParam						Standard value
//| Returns:
//|	TRUE if we handled message, FALSE otherwise
//*---------------------------------------------------------------------------------
UINT EXTFUN SaveFileHook(HWND	hDlg, UINT	msg, WPARAM	wParam, LPARAM	lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{

			OPENFILENAME * lpofn = (OPENFILENAME *)lParam;
			BOOL fAppend=(BOOL)ofnGlobal.lCustData;

			SETWINDOWLONG(hDlg, lpofn);
			lpofn = (OPENFILENAME *)GETWINDOWLONG(hDlg);

			CheckDlgButton(hDlg, IDC_APPEND,fAppend);
			CenterDialog(hDlg);
		}
		return TRUE;

		case WM_COMMAND:
		{
			if(GET_WM_COMMAND_ID(wParam, lParam) == IDOK)
			{
				OPENFILENAME * lpofn;
            lpofn = (OPENFILENAME *)GETWINDOWLONG(hDlg);

				*lpofn=ofnGlobal;

				lpofn->lCustData = ofnGlobal.lCustData = (BOOL)IsDlgButtonChecked(hDlg, IDC_APPEND);

				//Toggle Append Check Box
				if(ofnGlobal.lCustData && (ofnGlobal.Flags & OFN_OVERWRITEPROMPT))
				{
					ofnGlobal.Flags &= ~OFN_OVERWRITEPROMPT;
					lpofn->Flags = ofnGlobal.Flags;
				}

			}
		}

		break;

		default:
			break;
	}

	return FALSE;
}


//*------------------------------------------------------------------------
//|  GetFilter:
//|		This function loads the ids string id from the string table and
//|		replaces all | chars with \0 so the returned string will
//|		be compatible with GetOpenFileName.
//|	Parms:
//|		szOut		Output buffer for loaded string
//|		cbOut		Size of buffer
//|		ids		String id of filter to load
//|	Returns:
//|		Pointer to the buffer if successful, NULL for an error.
//*------------------------------------------------------------------------
LPTSTR EXTFUN GetFilter(LPTSTR szOut, int cbOut, UINT ids)
{
	LPTSTR		str=szOut;

	GetString(szOut, cbOut, ids);
	while(str = _tcschr(str, TEXT('|')))
		*str++ = TEXT('\0');
	return szOut;
}




//*---------------------------------------------------------------------------------
//| BrowseDirs:
//|	Common dialog look-alike which allows you to select a directory.
//|	Stolen from CPanel stuff.
//| Parms:
//|	hDlg					Window handle for displays
//|	hInst					Instance handle of this DLL
//|	lpszPrompt			Prompt text for dialog, NULL for default
//|	lpszDrive			Drive name
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL EXTFUN BrowseDirs(HWND hDlg, HINSTANCE hInst,
					LPCSTR lpszPrompt, LPTSTR lpszDrive)
{
	OPENFILENAME	ofn;
	TCHAR				szFile[MAXFILELEN];
	BROWSEINFO		bi;

	lstrcpy(szFile, TEXT("DUMMY.TXT"));

	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.szPrompt = lpszPrompt;

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = hDlg;
	ofn.hInstance         = (!hInst) ? hThisInst : hInst;
	ofn.lpstrFile         = (LPTSTR)szFile;
	ofn.nMaxFile          = sizeof(szFile);
	ofn.lpstrInitialDir   = lpszDrive;
	ofn.Flags             = OFN_ENABLEHOOK       |
									OFN_ENABLETEMPLATE   |
									OFN_HIDEREADONLY     |
									OFN_NOTESTFILECREATE |
									OFN_PATHMUSTEXIST    |
									OFN_SHAREAWARE;
	ofn.lpfnHook          = (LPOFNHOOKPROC)CommDlgHook;
	ofn.lpTemplateName    = MAKEINTRESOURCE(IDD_BROWSE);
	ofn.lCustData			 = (LPARAM)(lpBROWSEINFO)&bi;
	if (GetOpenFileName(&ofn)) {
		_tcsncpy(lpszDrive, szFile, ofn.nFileOffset);
		*(lpszDrive+ofn.nFileOffset-1) = TEXT('\0');
		return TRUE;
	}
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| CommDlgHook:
//|	Hook function for browsing a directory.  Need for custom info.
//| Parms:
//|	hDlg							Window handle
//|	msg							Window message
//|	wParam						Standard value
//|	lParam						Standard value
//| Returns:
//|	TRUE if we handled message, FALSE otherwise
//*---------------------------------------------------------------------------------
UINT EXTFUN CommDlgHook(HWND	hDlg,
						UINT	msg,
						WPARAM	wParam,
						LPARAM	lParam)
{
	lpBROWSEINFO	lpbi;

	lpbi = (lpBROWSEINFO)GETWINDOWLONG(hDlg);

	switch (msg) {
		case WM_INITDIALOG:
			{
			OPENFILENAME * 	lpofn;

			lpofn = (OPENFILENAME *)lParam;
			lpbi = (lpBROWSEINFO)lpofn->lCustData;
			SETWINDOWLONG(hDlg, lpbi);

			CenterDialog(hDlg);

			lpbi->uFileOkString = RegisterWindowMessage(FILEOKSTRING);
			lpbi->lpofn         = (LPOPENFILENAME)lParam;
			if(lpbi->szPrompt)
				SetWindowText(GetDlgItem(hDlg, IDT_PROMPT), (LPTSTR)lpbi->szPrompt);
         }
			return TRUE;


		case WM_CLOSE:
			if (lpbi)
				SETWINDOWLONG(hDlg, 0L);
			break;

		default:
			break;
	}

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| Busy:
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
void Busy(int flag)
{
	static HCURSOR	hCursor;						// Save old cursor handle

	ShowCursor(FALSE);
	if(flag)
		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	else
		SetCursor(hCursor);
	ShowCursor(TRUE);
}


//*---------------------------------------------------------------------------------
//| EnumerateListBox:
//|	This function will go through each selected item in a multiple selection
//|	list box and send the user defined message to the parent window.  The
//|	message sent to the parent is:
//|			wParam:		Index of selected item
//|			lParam:		Pointer to the retrieved text value
//| Parms:
//|	hwndParent					Parent dialog handle
//|	hwndList						Handle of the list box to enumerate
//|	msg							User defined message to send to parent
//| Returns:
//|	Total items processed
//*---------------------------------------------------------------------------------
UINT EnumerateListBox(HWND hwndParent, HWND hwndList, unsigned msg)
{
	static TCHAR szItem[MAXLISTBOXSIZE];
	WPARAM		cbCnt;
	int			dex;
   int *	xIndex;
   UINT			cbTotal=0;

	if(!(cbCnt = (WPARAM)SendMessage(hwndList, LB_GETSELCOUNT, 0, 0L)))
   	return TRUE;
	if(!(xIndex = (int *)AllocateMemory((DWORD)(sizeof(int) * cbCnt))))
		return TRUE;
	SendMessage(hwndList, LB_GETSELITEMS, cbCnt, (LPARAM)(int *)xIndex);
	for(dex=(int)cbCnt-1;  dex>=0;  --dex) {
		SendMessage(hwndList, LB_GETTEXT, xIndex[dex], (LPARAM)(LPTSTR)szItem);
		SendMessage(hwndParent, msg, (WPARAM)xIndex[dex], (LPARAM)(LPTSTR)szItem);
		}
	ReleaseMemory(xIndex);
	return cbTotal;
}


//*---------------------------------------------------------------------------------
//| BuildSelList:
//|	This function will build a doubally null terminated list of selected
//|	items and return a pointer to it.
//| Parms:
//|	hwndList						List box handle
//| Returns:
//|	Total items processed
//*---------------------------------------------------------------------------------
LPTSTR BuildSelList(HWND hwndList)
{
	LPTSTR			szStr, str;
	WPARAM		cbCnt;
	int			dex;
   int *	xIndex;

	if(!(cbCnt = (WPARAM)SendMessage(hwndList, LB_GETSELCOUNT, 0, 0L)))
   	return NULL;
	if(!(xIndex = (int *)AllocateMemory((DWORD)(sizeof(int) * cbCnt))))
		return NULL;
	if(!(szStr = str = (LPTSTR)AllocateMemory(MAXTESTBUFF)))
		return NULL;
	SendMessage(hwndList, LB_GETSELITEMS, cbCnt, (LPARAM)(int *)xIndex);
	for(dex=0;  dex<(int)cbCnt;  dex++) {
		SendMessage(hwndList, LB_GETTEXT, xIndex[dex], (LPARAM)(LPTSTR)str);
		str += lstrlen(str) + 1;
		}
	ReleaseMemory(xIndex);
	return szStr;
}


//*---------------------------------------------------------------------------------
//| CountNullArray:
//|	Given a double-null terminated list, return the number of items
//|	in that list.
//| Parms:
//|	instr							Double-null list
//| Returns:
//|	Total items in list
//*---------------------------------------------------------------------------------
UINT	INTFUN CountNullArray(LPTSTR instr)
{
	LPTSTR str=instr;
	UINT		cnt=0;
	while(*str) {
		++cnt;
		str += lstrlen(str) + 1;
		}
	return cnt;
}


//*---------------------------------------------------------------------------------
//| DoSetFile:
//|	Given a file name and a window to display it in, find an abbreviated
//|	version of the file name to use.  For example, use the syntax:
//|		c:\...\d1\d2\d3
//|	when the file name:
//|		c:\a\b\c\d\e\f\g\d1\d2\d3
//|	will not fit.
//| Parms:
//|	hwnd							Window handle
//|	szInFile						File name, do not modify
//| Returns:
//|	TRUE if successful, FALSE on cancel
//*---------------------------------------------------------------------------------
BOOL INTFUN DoSetFile(HWND hwnd, LPCTSTR szInFile)
{
	TCHAR 	szFile[_MAX_PATH];
	TCHAR		szFileName[_MAX_FNAME];
	TCHAR		szExt[5];
	TCHAR		szDrive[3];
	TCHAR		szPath[_MAX_PATH];
	BOOL		fDone=FALSE, fFindDone;
	HDC		hdc;
	SIZE		sz;
	RECT		crect;
	int		len, elen = lstrlen(szEllipse);
	LPTSTR		str, lstr;

	lstrcpy(szFile, szInFile);					// Just in case
	hdc = GetDC(hwnd);
	if(hdc) {
		// How much room do we have?
		GetClientRect(hwnd, &crect);
		len = crect.right - crect.left;

		// Keep abbreviating the name until it fits
		_tsplitpath(szInFile, szDrive, szPath, szFileName, szExt);
		do {
			// Reconstruct the current path value
			lstrcpy(szFile, szDrive);

			//if(*szPath && *szPath != TEXT('\\'))
			//	lstrcat(szFile, TEXT("\\"));

			lstrcat(szFile, szPath);
			lstrcat(szFile, szFileName);
			lstrcat(szFile, szExt);

         // See if it fits
			GetTextExtentPoint(hdc, szFile,
						lstrlen(szFile), &sz);
			if(sz.cx <= len)
				fDone = TRUE;

			// It it does not fit, then make it smaller
			fFindDone = FALSE;
			lstr = str = szPath;
			while(!fFindDone && !fDone && str) {
				if(str = _tcschr(str, TEXT('\\'))) {
					if(_tcsnccmp(lstr, szEllipse, elen != 0))
						// At this point we have found a slash not after ...
						if((DWORD)(str - szPath) >= (DWORD)elen) {
							_tcsncpy(szPath, szEllipse, elen);
							lstrcpy((LPTSTR)(szPath + elen), str);
							fFindDone = TRUE;
							}
					}
				else
					fFindDone = fDone = TRUE;		// No more sub dirs
				lstr = str++;
				}
			} while(!fDone);

		// Clean-up
		ReleaseDC(hwnd, hdc);
		}
	else
		return FALSE;

	SetWindowText(hwnd, szFile);

	return TRUE;
}


//*------------------------------------------------------------------------
//|  GetString:
//|		This function will load a strings only from GATORTST.DLL and
//|		should be used for internal strings and messages.
//|	Parms:
//|		buf		Output buffer for the string being retrieved
//|		cbbuf		How large is the buffer to retrieve
//|		ids		Which string do you want?
//|	Returns:
//|		Pointer to the buffer if successful, an error message on failure
//*------------------------------------------------------------------------
LPTSTR EXTFUN GetString(LPTSTR buf, int cbbuf, UINT ids)
{
	if(!hThisInst)
		return NULL;
	if(!GtrLoadString(hThisInst, ids, buf, cbbuf))
		wsprintf(buf, TEXT("String %u not found."), ids);
	return buf;
}


//*------------------------------------------------------------------------
//|  GetRCString:
//|		This function works like GetString only it retrieves strings
//|		from a user's resource fork.  Inhouse user's should use
//|		GetidsString to accomplish the same thing which abstracts
//|		the HINSTANCE handle.
//|	Parms:
//|		hInst		Instance handle from the caller
//|		buf		Output buffer for the string being retrieved
//|		cbbuf		How large is the buffer to retrieve
//|		ids		Which string do you want?
//|	Returns:
//|		Pointer to the buffer if successful, an error message on failure
//*------------------------------------------------------------------------
LPTSTR EXTFUN GetRCString(HINSTANCE hInst, LPTSTR buf, int cbbuf, UINT ids)
{
	if(!GtrLoadString(hInst, ids, buf, cbbuf))
		wsprintf(buf, TEXT("String %u not found."), ids);
	return buf;
}



//*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*
//| What follows are some debug memory routines.  When compiled with the _DEBUG
//|	flag, they will track memory allocation to a text file which can be queried.
//*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*
#ifdef _DEBUG

//*---------------------------------------------------------------------------------
//| DumpDebugInfo:
//|	Take the given parameters and dump it in the form of a comma separated
//|	record which can later be queried for memory usage information.
//|
//| 	The table created is called MEM and has the following format:
//|				create table mem
//|			 		(address char(9),
//|			  		 logtime char(11),
//|			 		 status integer,
//|			 		 desired integer,
//|			 		 actual integer,
//|					 line integer,
//|			 		 file char(45))
//|	Run the following query to find memory usage information:
//|  		  select address, sum(status) from mem
//|				  group by address having sum(status) <> 0
//|	A record with status of 1 means the memory was allocated but never freed.
//|	A record with status of -1 means the handle was freed but never allocated.
//|	If this query returns Row not found, all was successful.
//|
//| Parms:
//|	in			size						How much memory
//| Returns:
//|	Long pointer to void
//*---------------------------------------------------------------------------------
void EXTFUNCDECL DumpDebugInfo(LPVOID tmp, LPTSTR szFile, int cbLine, int stat,
			DWORD rqSize, DWORD aSize)
{
	static	TCHAR			szDbgOut[80];
	static 	OFSTRUCT 	ofs;
	static 	HANDLE		hf;
	static	BOOL			fNew=0;
	static	TIMEINFO		thetime;
	static	TCHAR			szTime[12];
	DWORD						dwWritten=0;
	BOOL		fReturn=0;
	DWORD		cbBytes=0;

	GetTime(&thetime);
	szGetTime(szTime,thetime);

	wsprintf((LPTSTR)szDbgOut,
#ifdef WIN32
				TEXT("%08X,%s,%d,%lu,%lu,%u,%s\r\n"),
				tmp,
#else
				TEXT("%04X:%04X,%s,%d,%lu,%lu,%u,%s\r\n"),
				HIWORD(tmp), LOWORD(tmp),
#endif
				szTime,
				stat, rqSize, aSize, cbLine, (LPTSTR)szFile);


	hf=GtrCreateFile(szDEBUGFILE,
						GENERIC_WRITE,
						FILE_SHARE_WRITE,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						0);

	if (hf != INVALID_HANDLE_VALUE)
	{
		_llseek((HFILE)hf, 0L, 2);							// Try to go to end of file
		fReturn=WriteFile(hf,szDbgOut,sizeof(szDbgOut),&cbBytes,NULL);
		_lclose((HFILE)hf);

	}

}


void * EXTFUNCDECL DebugGetMemory(DWORD size, LPTSTR szFile, int cbLine)
{
	static LPVOID 	ptr;
#ifndef WIN32
	DWORD		aSize;
#endif

	ptr = DoGetMemory(size);
	if(!ptr)	{
#if defined(_DEBUG) && defined(_MEM_CHECK)
		// Give an assert under this condition to track failures
		TCHAR tmp[200];
		const TCHAR szAssertMsg[] = "Assertion failure: %s, %s, %d";
		const TCHAR szmsg[] = "Could not allocate memory";

		wsprintf((LPTSTR)tmp, szAssertMsg, (LPTSTR)szmsg, (LPTSTR)szFile, cbLine);
		GtrMessageBox(GetActiveWindow(), (LPTSTR)tmp, "error", MB_ICONEXCLAMATION | MB_OK);
#endif
		return ptr;
		}

// Temp hack to keep NT from screwing up pointer
#ifndef WIN32
	aSize = GlobalSize(GlobalPtrHandle(ptr));
	DumpDebugInfo(ptr, szFile, cbLine, 1, size, aSize);
#endif

	return ptr;
}

void EXTFUN DebugReleaseMemory(LPVOID ptr, LPTSTR szFile, int cbLine)
{
	GlobalFreePtr(ptr);
	DumpDebugInfo(ptr, szFile, cbLine, -1, 0, 0);
}
#endif		// Debug memory routines


//*---------------------------------------------------------------------------------
//| DoGetMemory:
//|	This function allocates the specified amount of memory.  Note that the error
//|	messages are hard coded since they are only used when memory is very low and
//|	a LoadString could fail under such circumstances.
//| Parms:
//|	in			size						How much memory
//| Returns:
//|	Long pointer to void
//*---------------------------------------------------------------------------------
void * EXTFUNCDECL DoGetMemory(DWORD size)
{
	LPVOID	tmp;
	const TCHAR	szOutOfMemory[] = TEXT("Error allocating memory.  Please close other applications and try your request again.");
	const TCHAR	szError[] = TEXT("Out of Memory");

   tmp = GlobalAllocPtr(GHND, size);
   if(!tmp) 						// Serious error allocating memory
   	GtrMessageBox(GetActiveWindow(),
   				(LPTSTR)szOutOfMemory,
   				(LPTSTR)szError,
   				MB_ICONHAND | MB_SYSTEMMODAL);

   return tmp;
}


//*---------------------------------------------------------------------------------
//| DoReleaseMemory:
//|	Free up the memory we have requested
//| Parms:
//|	ptr			The pointer to free
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
void EXTFUN DoReleaseMemory(LPVOID ptr)
{
	GlobalFreePtr(ptr);
}




//*---------------------------------------------------------------------------------
//| SetGatorIniPath:
//|	This function is used by ODBCTEST and Gatortst.dll to set the
//|	path for gator32.ini.
//| Parms:
//|	szGatorIni	path to gator32.ini
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------

VOID EXTFUN SetGatorIniPath(LPTSTR szGatorIni)
{

	if (!*g_szGatorIni)
	{
		if (*szGatorIni)
		{
			//Set the Global Var.
			lstrcpy(g_szGatorIni,szGatorIni);
		}
		else
		{
			//Global Var. hasn't been set look for INI file.
			FindFileDir(szGATORININAME,g_szGatorIni);
		}
	}

} //SetGatorIniPath()

//*--------------------------------------------------------------------------------------
//
//
//		ADDED HERE FOR WIN95's LACK OF UNICODE SUPPORT
//
//
//
//*-----------------------------------------------------------------------------------------
#undef MAXOUTPUTBUFF
#define MAXOUTPUTBUFF 1024

UINT GtrGetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName, INT nDefault,LPCTSTR lpFileName)
{

#ifdef _UNICODE

	CHAR	szAppNameA[MAXOUTPUTBUFF] = "";
	CHAR	szKeyNameA[MAXOUTPUTBUFF] = "";
	CHAR	szFileNameA[MAXOUTPUTBUFF] = "";
	LPSTR	lpAppNameA,lpKeyNameA,lpFileNameA;




	if (lpAppName)
	{
		lpAppNameA = szAppNameA;
		WideCharToMultiByte(CP_ACP,0,lpAppName,-1,lpAppNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpAppNameA = NULL;
	if (lpKeyName)
	{
		lpKeyNameA = szKeyNameA;
		WideCharToMultiByte(CP_ACP,0,lpKeyName,-1,lpKeyNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpKeyNameA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;


	return GetPrivateProfileIntA(lpAppNameA,lpKeyNameA,nDefault,lpFileNameA);
#else

	return GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,lpFileName);

#endif
}

DWORD GtrGetPrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault,
							 LPTSTR lpReturnedString, DWORD nSize,LPCTSTR lpFileName)
{
	DWORD	dwRetValue=0;
#ifdef _UNICODE

	CHAR	szAppNameA[MAXOUTPUTBUFF] = "";
	CHAR	szKeyNameA[MAXOUTPUTBUFF] = "";
	CHAR	szFileNameA[MAXOUTPUTBUFF] = "";
	CHAR	szDefaultA[MAXOUTPUTBUFF] = "";
	CHAR	szReturnedStringA[MAXOUTPUTBUFF];
	UINT size =0;

	LPSTR	lpAppNameA,lpKeyNameA,lpFileNameA,lpDefaultA,lpTemp;

	if(lpReturnedString)
		*lpReturnedString = TEXT('\0');

	memset((BYTE*)szReturnedStringA,0,MAXOUTPUTBUFF);

	if (lpAppName)
	{
		lpAppNameA = szAppNameA;
		WideCharToMultiByte(CP_ACP,0,lpAppName,-1,lpAppNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpAppNameA = NULL;
	if (lpKeyName)
	{
		lpKeyNameA = szKeyNameA;
		WideCharToMultiByte(CP_ACP,0,lpKeyName,-1,lpKeyNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpKeyNameA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;
	if(lpDefault)
	{
		lpDefaultA = szDefaultA;
		WideCharToMultiByte(CP_ACP,0,lpDefault,-1,lpDefaultA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpDefaultA = NULL;


	dwRetValue = GetPrivateProfileStringA(lpAppNameA,lpKeyNameA,lpDefaultA,
					szReturnedStringA,nSize,lpFileNameA);



	if (szReturnedStringA)
	{
		lpTemp = szReturnedStringA;
		while ( *lpTemp )
		{
			size += strlen(lpTemp) + 1;
			lpTemp += strlen(lpTemp) + 1;
		}
		MultiByteToWideChar(CP_ACP,0,szReturnedStringA,size,lpReturnedString,nSize);
	}
#else

	dwRetValue = GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,
					lpReturnedString,nSize,lpFileName);

#endif

	return dwRetValue;
}

BOOL GtrWritePrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpString,
							   LPCTSTR lpFileName)
{
#ifdef _UNICODE

	CHAR	szAppNameA[MAXOUTPUTBUFF] = "";
	CHAR	szKeyNameA[MAXOUTPUTBUFF] = "";
	CHAR	szFileNameA[MAXOUTPUTBUFF] = "";
	CHAR	szStringA[MAXOUTPUTBUFF] = "";
	BOOL	bRetValue=0;
	LPSTR	lpAppNameA,lpKeyNameA,lpFileNameA,lpStringA;


	if (lpAppName)
	{
		lpAppNameA = szAppNameA;
		WideCharToMultiByte(CP_ACP,0,lpAppName,-1,lpAppNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpAppNameA = NULL;
	if (lpKeyName)
	{
		lpKeyNameA = szKeyNameA;
		WideCharToMultiByte(CP_ACP,0,lpKeyName,-1,lpKeyNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpKeyNameA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;
	if(lpString)
	{
		lpStringA = szStringA;
		WideCharToMultiByte(CP_ACP,0,lpString,-1,lpStringA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpStringA = NULL;

	return  WritePrivateProfileStringA(lpAppNameA,lpKeyNameA,
					lpStringA,lpFileNameA);
#else
	return  WritePrivateProfileString(lpAppName,lpKeyName,
					lpString,lpFileName);
#endif

}

int GtrLoadString(HINSTANCE hInstance,UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
	int	 nRetValue;

#ifdef _UNICODE

	CHAR szBufferA[MAXOUTPUTBUFF];

	LPSTR lpBufferA;

	if (lpBuffer)
	{
		lpBufferA = szBufferA;
		WideCharToMultiByte(CP_ACP,0,lpBuffer,-1,lpBufferA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpBufferA = NULL;


	nRetValue = LoadStringA(hInstance , uID, lpBufferA,MAXOUTPUTBUFF);

	MultiByteToWideChar(CP_ACP,0,lpBufferA,-1,lpBuffer,nBufferMax);

#else
	nRetValue = LoadString(hInstance , uID, lpBuffer,nBufferMax);
#endif

	return nRetValue;
}

DWORD GtrFormatMessage(DWORD dwFlags,LPCVOID lpSource, DWORD dwMessageid, DWORD dwLanguageid,
					LPTSTR lpBuffer,DWORD nSize,va_list * Arguments)
{
	DWORD dwRetValue=0;
#ifdef _UNICODE

	//CHAR szSourceA[MAXOUTPUTBUFF];
	CHAR szBufferA[MAXOUTPUTBUFF];
	//LPSTR lpSourceA,lpBufferA;


	/*if (lpSource)
	{
		lpSourceA = szSourceA;
		WideCharToMultiByte(CP_ACP,0,lpSource,-1,lpSourceA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpSourceA = NULL;*/

	dwRetValue = FormatMessageA(dwFlags,lpSource,dwMessageid,dwLanguageid,szBufferA,
					MAXOUTPUTBUFF,Arguments);

	MultiByteToWideChar(CP_ACP,0,szBufferA,-1,lpBuffer,nSize);
#else
	dwRetValue = FormatMessage(dwFlags,lpSource,dwMessageid,dwLanguageid,lpBuffer,
					nSize,Arguments);
#endif

	return dwRetValue;
}

HANDLE GtrCreateFile(LPCTSTR lpFileName,DWORD dwDesiredAccess,DWORD dwSharedMode,LPSECURITY_ATTRIBUTES
				  lpSecurityAttributes,DWORD dwCreationDistribution,DWORD FlagsAndAttributes,
				  HANDLE hTemplateFile)
{

#ifdef _UNICODE

	CHAR szFileNameA[_MAX_PATH];
	LPSTR lpFileNameA;

	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;

	return CreateFileA( lpFileNameA,
						dwDesiredAccess,
						dwSharedMode,
						lpSecurityAttributes,
						dwCreationDistribution,
						FlagsAndAttributes,
						 hTemplateFile);
#else
	return CreateFile( lpFileName,
						dwDesiredAccess,
						dwSharedMode,
						lpSecurityAttributes,
						dwCreationDistribution,
						FlagsAndAttributes,
						 hTemplateFile);
#endif

}

HINSTANCE GtrLoadLibrary( LPCTSTR lpLibFileName)
{
#ifdef _UNICODE

	CHAR szLibFileNameA[_MAX_PATH];
	LPSTR	lpLibFileNameA;

	if(lpLibFileName)
	{
		lpLibFileNameA = szLibFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpLibFileName,-1,lpLibFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpLibFileNameA = NULL;

	return LoadLibraryA(lpLibFileNameA);
#else
	return LoadLibrary(lpLibFileName);
#endif
}

DWORD GtrSearchPath(LPCTSTR lpPath,LPCTSTR lpFileName,LPCTSTR lpExtensions, DWORD nBufferLength,
				 LPTSTR lpBuffer,LPTSTR *lpFilePart)
{
	DWORD	dwRetValue=0;
#ifdef _UNICODE

	CHAR	szPathA[_MAX_PATH];
	CHAR	szFileNameA[_MAX_PATH];
	CHAR	szExtensionsA[_MAX_PATH];
	CHAR	szBufferA[_MAX_PATH];
	CHAR	szFilePartA[_MAX_PATH];
	LPSTR	lpPathA,lpFileNameA,lpExtensionsA,lpFilePartA;
	LPSTR*	lppFilePartA;



	if (lpPath)
	{
		lpPathA = szPathA;
		WideCharToMultiByte(CP_ACP,0,lpPath,-1,lpPathA,_MAX_PATH,NULL,NULL);
	}
	else
		lpPathA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,_MAX_PATH,NULL,NULL);
	}
	else
		lpFileNameA = NULL;
	if (lpExtensions)
	{
		lpExtensionsA = szExtensionsA;
		WideCharToMultiByte(CP_ACP,0,lpExtensions,-1,lpExtensionsA,_MAX_PATH,NULL,NULL);
	}
	else
		lpExtensionsA = NULL;
	if (lpFilePart && *lpFilePart)
	{
		lpFilePartA = szFilePartA;
		WideCharToMultiByte(CP_ACP,0,*lpFilePart,-1,lpFilePartA,_MAX_PATH,NULL,NULL);
		lppFilePartA = &lpFilePartA;
	}
	else
		lppFilePartA = NULL;

	dwRetValue = SearchPathA(lpPathA,						// address of search path
							lpFileNameA,					// address of filename
							lpExtensionsA,						// address of extension
							nBufferLength,				// size, in characters, of buffer
							szBufferA,				// address of buffer for found filename
							lppFilePartA);

	MultiByteToWideChar(CP_ACP,0,szBufferA,-1,lpBuffer,_MAX_PATH);
#else
	dwRetValue = SearchPath(lpPath,						// address of search path
							lpFileName,					// address of filename
							lpExtensions,						// address of extension
							nBufferLength,				// size, in characters, of buffer
							lpBuffer,				// address of buffer for found filename
							lpFilePart);
#endif

	return dwRetValue;
}

int GtrMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
#ifdef _UNICODE
	CHAR	szTextA[_MAX_PATH];
	CHAR	szCaptionA[_MAX_PATH];
	LPSTR	lpTextA,lpCaptionA;

	if (lpText)
	{
		lpTextA = szTextA;
		WideCharToMultiByte(CP_ACP,0,lpText,-1,lpTextA,_MAX_PATH,NULL,NULL);
	}
	else
		lpTextA = NULL;
	if (lpCaption)
	{
		lpCaptionA = szCaptionA;
		WideCharToMultiByte(CP_ACP,0,lpCaption,-1,lpCaptionA,_MAX_PATH,NULL,NULL);
	}
	else
		lpCaptionA = NULL;

	return MessageBoxA(hWnd,lpTextA,lpCaptionA,uType);
#else
	return MessageBox(hWnd,lpText,lpCaption,uType);
#endif
}

BOOL GatorSetWindowText(HWND hWnd,LPCTSTR lpString)
{
#ifdef _UNICODE

	CHAR szStringA[MAXOUTPUTBUFF];
	LPSTR lpStringA;

	if (lpString)
	{
		lpStringA = szStringA;
		WideCharToMultiByte(CP_ACP,0,lpString,-1,lpStringA,_MAX_PATH,NULL,NULL);
	}
	else
		lpStringA = NULL;


	return SetWindowTextA(hWnd,lpStringA);
#else
	return SetWindowText(hWnd,lpString);
#endif
}

/*int _openA(const wchar_t *filename,int oflag,int pmode)
{

	char filenameA[_MAX_PATH];
#ifdef _UNICODE


	WideCharToMultiByte(CP_ACP,0,filename,-1,filenameA,_MAX_PATH,NULL,NULL);
#else
	strcpy(filenameA,(char *)filename);
#endif

	return _open(filenameA,oflag,pmode);
}*/

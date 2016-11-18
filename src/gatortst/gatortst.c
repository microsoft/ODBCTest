//*---------------------------------------------------------------------------------
//|	File:		GATORTST.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module conatins the main entry points which are required by
//|		all DLL's to be loaded.
//*---------------------------------------------------------------------------------
#include "gtrhdrs.h"


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
HINSTANCE 		hThisInst;

//#ifndef _RETAIL		// Changed to #ifdef _DEBUG
#ifdef _DEBUG
#ifdef WIN32
//LPGETSTATETYPE	lpGetStateType;		// Thunking DLL entry point (32s only)
//HINSTANCE		hLoadedThunk;			// Instance handle of thunking DLL
//dCSEG(TCHAR) szErr[]					=	"Auto Test Error";
//dCSEG(TCHAR) szErrLoadThunk[]		=	"The Inhouse testing functions cannot run under Win32s without ERRC16UT.DLL and ERRC32UT.DLL";
//dCSEG(TCHAR) szErrLoadThunkFunc[]	=	"Error getting address of GetStateType from ERRC32UT.DLL";
#endif
#endif


//-------------------------------------------------------------------------
//	For WIN32 programs, there is only one entry point which takes care of
//		everything.  Call the _CRT_INIT function to enable the C run-time
//		functions which are used heavily in the auto tests.
//-------------------------------------------------------------------------
#ifdef WIN32
#include <winnt.h>

int EXTFUN DllMain(HANDLE hInst, DWORD fdwReason, LPVOID lpReserved)
{

	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
			hThisInst = hInst;
//			InstallRemoveHelpHook(TRUE);
			break;

		// Free the thunking DLL for internal test functions
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_DETACH:
//			InstallRemoveHelpHook(FALSE);
			break;
		}

	return TRUE;
}

#else


//*---------------------------------------------------------------------------------
//| LibMain:
//|	This entry point will simply take care of our data segment.
//| Parms:
//|	in			hInst						Current hInstance (data segment)
//|	in			wDataSeg
//|	in			cbHeapSize
//|	in			szCmdLine				Command line arguments
//| Returns:
//|	TRUE if successful, FALSE on a failure
//*---------------------------------------------------------------------------------
int EXTFUN LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeapSize,
								LPTSTR szCmdLine)
{
	hThisInst = hinst;
	InstallRemoveHelpHook(TRUE);

	return TRUE;
}



//*---------------------------------------------------------------------------------
//| WEP:
//|	This is the windows exit procedure.  We will be passed a flag which
//|		tells us if we are being unloaded by a normal Unload call or if
//|		the system is shutting down.
//| Parms:
//|	int		fParm						What type of unload?
//| Returns:
//|	TRUE always
//*---------------------------------------------------------------------------------
int EXTFUN WEP(int fParm)
{
	InstallRemoveHelpHook(FALSE);

	switch (fParm) {
		case WEP_SYSTEM_EXIT:
		case WEP_FREE_DLL:
		default:
			return TRUE;
	}
}


#endif



//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	MEM.C
//|
//| Purpose:
//|		This module will abstract some of the memory management functions
//|		required.  This is necessary since there are several pitfalls with
//|		Windows memory management.  For example:
//|
//|			o Under NT, a GlobalReAllocPtr can move the physical pointer
//|				location which means you could not expand a bound column without
//|				doing a rebind
//|			o A 16-bit app is limited to 8192 selectors which could get eaten
//|				up quite quickly when doing bound columns and results sets.
//*---------------------------------------------------------------------------------
#include "fhheader.h"


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE			hInst;
extern lpUSROPTIONS		lpUsrOptions;
extern TCHAR					szOutBuff[MAXOUTBUFF];
DWORD	 cbPageSize=DFTPAGESIZE;

extern LPCTSTR	szErrTitle;



#ifdef WIN32
//*---------------------------------------------------------------------------------
//| AdjMemInit:
//|	Call this function once when the app starts.  It will find out the default
//|	page size for the machine and store that value.
//| Parms:
//|
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
VOID AdjMemInit(void)
{
	SYSTEM_INFO		si;

	GetSystemInfo(&si);
	cbPageSize = si.dwPageSize;
}
#endif


//*---------------------------------------------------------------------------------
//| AllocateAdjMem:
//|	Allocates adjustable memory.  This will map to either a VirtualAlloc
//|	for NT or a GlobalAlloc for 16-bit.
//| Parms:
//|
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
LPVOID AllocateAdjMem(DWORD cbBytes)
{
	LPVOID		ptr;

#ifndef WIN32

   ASSERTCHECK((!cbBytes), TEXT("Cannot allocate 0 bytes"));

	// Allocate memory.  Return value is selector:offset which abstracts physical
	//		location so we won't have problems with the value changing on realloc
	ptr = GlobalAllocPtr(GHND, cbBytes);
	if(!ptr)
		return NULL;
	else
		return ptr;

#else

	// Reserver enough virtual memory so that we can grow the memory if required.
	//		This memory is not yet in physical memory, but precludes other functions
	//		from using our range.  If we require more physical memory, we can commit
	//		pages from our range and the pointer won't change.
	ptr = VirtualAlloc(NULL, 					// Windows figures out good location
							 MAXVIRTMEM, 			// Ask for a range of pages
							 MEM_RESERVE,			// Don't commit yet, just reserve address space
							 PAGE_READWRITE);		// Need both access types
	if(!ptr)
		return NULL;

	// Must commit enough pages to give user cbBytes of memory
	if(ReAllocAdjMem(ptr, 0, cbBytes))
		return ptr;
	else {
		VirtualFree(ptr, 0, MEM_RELEASE);
        return NULL;
    }

#endif
}


//*---------------------------------------------------------------------------------
//| ReAllocAdjMem:
//|	This function will commit/decommit pages from a virtual range.  We still
//|	reserve our virtual range, we're just freeing/allocating physical memory.
//| Parms:
//|	inptr						The base pointer to grow/shrink
//|	cbFrom					What current size?
//|	cbTo						What is desired size?
//| Returns:
//|	TRUE if we're successful
//*---------------------------------------------------------------------------------
#ifndef WIN32
BOOL INTFUN ReAllocAdjMem(LPVOID inptr, DWORD cbFrom, DWORD cbTo)
{
   ASSERTCHECK((!inptr), TEXT("Attempt to realloc NULL pointer"));
	return (GlobalReAllocPtr(inptr, cbTo, GHND /*GMEM_ZEROINIT*/) != NULL);
}
#else
BOOL INTFUN ReAllocAdjMem(LPVOID inptr, DWORD cbFrom, DWORD cbTo)
{
	LPVOID		ptr;
	UINT_PTR			dwPtr=(UINT_PTR)inptr;
	int			iCurrentPages, iReqPages;
	DWORD			cbBytes;

   ASSERTCHECK((!inptr), TEXT("Attempt to realloc NULL pointer"));
   ASSERTCHECK((!cbTo), TEXT("Cannot adjust to 0 bytes"));

	// cbFrom is not necessarily on a page boundary.  Determine how many
	//		pages we have based on the users byte count
	iCurrentPages = (cbFrom / cbPageSize) +
							((cbFrom % cbPageSize) ? 1 : 0);
	iReqPages = (cbTo / cbPageSize) +
							((cbTo % cbPageSize) ? 1 : 0);

	// Need more memory so commit more pages for read/write
	if(iCurrentPages < iReqPages) {
		dwPtr = (UINT_PTR)inptr + (iCurrentPages * cbPageSize);
		ptr = (LPVOID)dwPtr;
		cbBytes = (iReqPages - iCurrentPages) * cbPageSize;
		if(!VirtualAlloc(ptr,
						 cbBytes,
						 MEM_COMMIT,
						 PAGE_READWRITE))
			return FALSE;
		}
	// Have more memory than required so uncommit it
	else if(iCurrentPages > iReqPages) {
		dwPtr = (UINT_PTR)inptr + (iReqPages * cbPageSize);
		ptr = (LPVOID)dwPtr;
		cbBytes = (iCurrentPages - iReqPages) * cbPageSize;
		if(!VirtualFree(ptr,
						cbBytes,
						MEM_DECOMMIT))
			return FALSE;
		}

	return TRUE;
}
#endif


//*---------------------------------------------------------------------------------
//| FreeAdjMem:
//|	Free any committed/uncommitted memory.
//| Parms:
//|	inptr						The base pointer to grow/shrink
//| Returns:
//|	TRUE if we're successful
//*---------------------------------------------------------------------------------
BOOL INTFUN FreeAdjMem(LPVOID inptr)
{
   ASSERTCHECK((!inptr), TEXT("Attempt to free NULL pointer"));

#ifndef WIN32

	return GlobalFreePtr(inptr);

#else

	return VirtualFree(inptr, 0, MEM_RELEASE);

#endif
}


#ifdef WIN32
//*---------------------------------------------------------------------------------
//| DebugReleaseMem:
//|	Free any committed/uncommitted memory.
//| Parms:
//|	lpMem		pointer to memory location
//| Returns:
//|	TRUE if we're successful
//*---------------------------------------------------------------------------------
BOOL INTFUN DebugReleaseMem(HWND hwnd,LPCVOID lpMem)
{
	HGLOBAL	hGlobal=NULL;
	BOOL		fSuccess=FALSE;
	LPVOID	lpMessageBuffer=NULL;
	LPTSTR	lpstr=NULL;

	if (lpMem)
		{
		if (hGlobal=(HGLOBAL)GlobalHandle(lpMem))
			if (GlobalUnlock(hGlobal))
				if (!GlobalFree(hGlobal))
					fSuccess=TRUE;

		}

	if (!fSuccess)
		{
		GtrFormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				(LPTSTR)lpMessageBuffer,
				0,
				NULL);

		lpstr=(LPTSTR)lpMessageBuffer;
		GtrMessageBox(hwnd,lpstr,TEXT("Release Memory"),MB_ICONEXCLAMATION | MB_OK);

		}

	return(fSuccess);


} //DebugReleaseMem()

#endif

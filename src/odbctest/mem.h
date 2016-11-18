//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	MEM.H
//|
//| Purpose:
//|		Header file for memory functions.
//*---------------------------------------------------------------------------------
#ifndef MEM_DEFS
#define MEM_DEFS

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include "portable.h"
#include "standard.h"
#include "gtrcomm.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------

// NOTE:  Although NT gives us 2GB of address space which we could reserve, this
//			would be stupid since you're really limiting the number of allocs you can
//			do.  The purpose of Gator is to allow ad-hoc testing of drivers, and the
//			most common test on NT will be do test 64KB boundary code, therefore, the
//			logical max alloc size is 2*64KB which will allow you to test these
//			boundary conditions without chewing up your address space.
#define MAXVIRTMEM			(2 * 65535)
#define DFTPAGESIZE			(254)
#define FREE_ADJPTR(ptr) \
{                     \
	if(ptr)            \
		FreeAdjMem(ptr); \
	ptr = NULL;            \
}


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
#ifdef WIN32
VOID AdjMemInit(void);
#endif
LPVOID AllocateAdjMem(DWORD cbBytes);
BOOL INTFUN ReAllocAdjMem(LPVOID inptr, DWORD cbFrom, DWORD cbTo);
BOOL INTFUN FreeAdjMem(LPVOID inptr);
BOOL INTFUN DebugReleaseMem(HWND hwnd,LPVOID lpMem);

#endif

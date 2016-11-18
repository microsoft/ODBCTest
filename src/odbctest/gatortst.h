//*---------------------------------------------------------------------------------
//|  Title:		GATORTST.H
//|		Copyright (c) Microsoft, All rights reserved
//|
//|	This file prototypes some of the functions we're going to borrow
//|	from GATORTST.DLL. This file is used when building the retail
//|	version of Gator.
//*---------------------------------------------------------------------------------
#ifndef GATORTST_DEFS
#define GATORTST_DEFS

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "portable.h"
#include "standard.h"
#include "tchar.h"
#include "wchar.h"



//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define VszFile static TCHAR vszFile[]=TEXT(__FILE__)
#define USER_FUNCTION_HELP				(WM_USER + 400)

#ifndef WIN32
#define LPTSTR LPSTR
#endif

// Debug constants
#ifdef _MEM_CHECK
void * EXTFUNCDECL DebugGetMemory(DWORD size, LPTSTR szFile, int cbLine);
void EXTFUN DebugReleaseMemory(LPVOID ptr, LPTSTR szFile, int cbLine);
#define AllocateMemory(size) DebugGetMemory(size, (LPTSTR)vszFile, __LINE__)
#define ReleaseMemory(ptr) DebugReleaseMemory(ptr, (LPTSTR)vszFile, __LINE__)
#else
void * EXTFUNCDECL DoGtrGetMemory(SIZE_T size);
#define AllocateMemory(size) DoGtrGetMemory(size)
#define ReleaseMemory(ptr) if (ptr) GlobalFreePtr(ptr)
#endif

// Further redefine based on debug
#ifdef _DEBUG
#undef ReleaseMemory
void INTFUN DbgReleaseMemory(LPVOID ptr, LPTSTR szFile, UINT iLine);
#define ReleaseMemory(ptr) DbgReleaseMemory(ptr, (LPTSTR)vszFile, __LINE__)
#endif


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//		Functions
//----------------------------------------------------------------------------------
void EXTFUN ManageSources(HWND hwnd, HINSTANCE hInst);
void EXTFUN ManageTests(HWND hwnd, HINSTANCE hInst);
void EXTFUN ManageGroups(HWND hwnd, HINSTANCE hInst);
void EXTFUN RunAutoTest(HWND hwnd, HINSTANCE hInst, void *, void *, void *,void *,HANDLE);
void EXTFUN SetGatorIniPath(LPTSTR);


int EXTFUNCDECL CDECL szMessageBox(HWND hwnd, UINT style, LPTSTR szTitle, LPTSTR szFmt, ...);
BOOL EXTFUN FAbortCheck();
TIMEINFO * EXTFUNCDECL CElapsed(lpTIMEINFO start, lpTIMEINFO end, lpTIMEINFO elapsed);
void EXTFUN CenterDialog(HWND hdlg);
LPTSTR INTFUN FindFileDir(LPTSTR szFile, LPTSTR szPath);
BOOL EXTFUN GetLogFile(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
			LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL * lpfAppend);
BOOL EXTFUN BrowseDirs(HWND hdlg, HINSTANCE hInst, LPTSTR lpszDrive);
UINT EnumerateListBox(HWND hwndParent, HWND hwndList, unsigned msg);
LPTSTR BuildSelList(HWND hwndList);
BOOL IsValidTestName(LPTSTR szName);
UINT	INTFUN CountNullArray(LPTSTR instr);
VOID EXTFUN DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart);



// Memory functions
void * EXTFUNCDECL DoGetMemory(DWORD size);
void EXTFUN DoReleaseMemory(LPVOID ptr);


#endif

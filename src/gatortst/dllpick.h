//*---------------------------------------------------------------------------------
//|  Title:		DLLPICK.H
//|		Copyright (c) Microsoft Corporation
//|
//|	This file contains defines and macros for the DLLPICK functions which allow
//|	the user to add and remove test DLL's from GATOR.INI.
//*---------------------------------------------------------------------------------
#ifndef DLLPICK_DEFS
#define DLLPICK_DEFS


//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define IDD_MANAGE_TESTS                101
#define IDB_FROM                        1000
#define IDL_AVAILABLE                   1001
#define IDL_INSTALLED                   1002
#define IDB_ADD                         1003
#define IDB_REMOVE                      1004
#define IDT_DLL                         1005
#define IDE_DESCRIPTION                 1006
#define IDT_FROM                        1007
#define IDT_NAME								 1008


//----------------------------------------------------------------------------------
//		Functions
//----------------------------------------------------------------------------------
void EXTFUN ManageTests(HWND hwnd, HINSTANCE hInst);
BOOL EXTFUN ManageTestsWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


#endif

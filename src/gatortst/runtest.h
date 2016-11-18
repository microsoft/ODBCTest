//*---------------------------------------------------------------------------------
//|  Title:		RUNTEST.H
//| Copyright (c) Microsoft Corporation
//|
//|	This file contains defines and macros for the DLLPICK functions which allow
//|	the user to add and remove test DLL's from GATOR.INI.
//*---------------------------------------------------------------------------------
#ifndef RUNTEST_DEFS
#define RUNTEST_DEFS


//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define IDD_RUN_TESTS						 110
#define IDL_AUTO_TESTS						1100
#define IDL_SOURCES							1101
#define IDX_LOG                        1102
#define IDX_SCREEN                     1103
#define IDX_DEBUG                      1104
#define IDX_ISOLATE                    1105
#define IDX_CURSOR_LIB						1106
#define IDB_LOG                        1107
#define IDT_LOG                        1108
#define IDB_RUNLISTS							1109
#define IDC_RUNLISTS							1110

#define IDD_RUN_LISTS                   111
#define IDT_CURRENT                    1130
#define IDL_RUNLISTS							1131
#define IDB_SAVE								1132
#define IDB_SAVEAS							1133
#define IDB_DELETERL							1134

#define IDD_NEW_RUNLIST						 112

#define USER_SET_BUTTON_STATE			WM_USER + 151
#define USER_RESET_INSTALLED			WM_USER + 152
#define USER_RESET_AVAILABLE			WM_USER + 153
#define USER_ADD_ITEM 					WM_USER + 154
#define USER_REMOVE_ITEM				WM_USER + 155
#define USER_RESET_RUNLISTS			WM_USER + 156

#define GRAYED_BUTTON		2

//----------------------------------------------------------------------------------
//		Function prototypes
//----------------------------------------------------------------------------------
void EXTFUN RunAutoTest(HWND hwnd, HINSTANCE hInst,
				HENV henv, HDBC hdbc, HSTMT hstmt,lpRUNTESTLIST lpRtl,
				HANDLE hConsole);


#endif

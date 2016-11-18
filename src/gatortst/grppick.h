//*---------------------------------------------------------------------------------
//|  Title:		GRPPICK.H
//|		Copyright (c) Microsoft Corporation
//|
//|	This file contains defines and macros for the GRPPICK functions which allow
//|	the user to add and remove groups from GATOR.INI.
//*---------------------------------------------------------------------------------
#ifndef GRPPICK_DEFS
#define GRPPICK_DEFS


//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define IDD_MANAGE_GROUPS               120
#define IDT_GROUP								1200
#define IDC_GROUPS							1201
#define IDB_NEW								1202
#define IDB_DELETE							1203
#define IDT_GROUP_INSTALLED				1204
#define IDT_GROUP_AVAIL						1205

#define IDD_NEW_GROUP						1230
#define IDE_GROUP								1231
//#define IDT_GROUP_SOURCE_TITLE			1232

#define USER_RESET_GROUPS				WM_USER + 156


//----------------------------------------------------------------------------------
//		Functions
//----------------------------------------------------------------------------------
void EXTFUN ManageGroups(HWND hwnd, HINSTANCE hInst);
BOOL EXTFUN ManageGroupsWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


#endif

//*---------------------------------------------------------------------------------
//|  Title:		FILE.H
//|		Copyright (c) Microsoft Corporation
//|
//|	This file contains defines and macros for the FileOpen Dialog
//*---------------------------------------------------------------------------------
#ifndef FILE_DEFS

#define FILE_DEFS
//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------


#define IDD_OPEN			200
#define IDD_CURDIR		201
#define IDD_NAME			202
#define IDD_FILELIST		203
#define IDD_DIRLIST		204
#define IDD_DRIVES		205
#define IDD_FILE_TYPES	206
#define IDC_APPEND_FILE	207
#define IDD_DRIVES_TXT	208
#define IDNETWORK			209

#define MAX_NUM_DRIVES	30
#define SMALLBUFF			256
#define MEDBUFF			512
#define LARGEBUFF			1048
#define XLARGEBUFF		2048


typedef struct tagDriveInfo
{
	int nNum;
	TCHAR	szLetter[_MAX_PATH];
	TCHAR	szVol[MEDBUFF];

} DRIVEINFO, *lpDRIVEINFO;


LRESULT EXTFUN OpenDlgProc(HWND hdlg,UINT wMsg,WPARAM wParam,LPARAM lParam);

#endif

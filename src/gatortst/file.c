//*---------------------------------------------------------------------------------
//|	File:		FILE.C
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	Purpose:
//|		This module contains functions that support gator's File Open
//|	Dialog.
//*---------------------------------------------------------------------------------
#include "gtrhdrs.h"
#include <windowsx.h>
#include <winnetwk.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <io.h>
#include	"file.h"
#pragma warning(disable:4996)

dCSEG(TCHAR) 	g_szWILD[]=TEXT("*.*");
dCSEG(TCHAR) 	g_szLOG[]=TEXT("*.log");
dCSEG(TCHAR)	g_szWILDTYPE[]=TEXT("All Files (*.*)");
dCSEG(TCHAR)	g_szLOGTYPE[]=TEXT("Log Files (*.log)");
dCSEG(TCHAR)	g_szInvalid[]=TEXT("File name or path is invalid.");

#define FILE_EXISTS 00

TCHAR			g_szCurDir[_MAX_PATH];
TCHAR			g_szCurLogFile[_MAX_PATH];
DRIVEINFO	g_rgDI[MAX_NUM_DRIVES];
DRIVEINFO	g_diCurDrive;
TCHAR			g_szWildCard[MEDBUFF];
int			g_nRC=0;

UINT uAllFilesAttrib = FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_COMPRESSED | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY;


//*------------------------------------------------------------------------
//|  CreatePath:
//|		This function creates a full path from szPath and szFileSpec
//|	Parms:
//|		szFileSpec		File specification
//|		szPath			file's path
//|		szFullPath		resulting full path
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

__inline VOID INTFUN CreatePath(const LPTSTR szFileSpec,const LPTSTR szPath,LPTSTR szFullPath)
{
	//check if absolute path has been entered
	TCHAR 	szFname[_MAX_PATH];
	TCHAR		szExt[5];
	TCHAR		szDrive[3];
	TCHAR		szPth[_MAX_PATH];
	LPTSTR	lpLastChar;

	_tsplitpath(szPath, szDrive, szPth, szFname, szExt);

	if (!*szDrive && !*szPth)
	{
		lpLastChar=CharPrev( (LPCTSTR)szFileSpec,(LPCTSTR)(szFileSpec + lstrlen(szFileSpec)));
		//add "*.*" to the current working directory
		//to handle DBCS if (szFileSpec[lstrlen(szFileSpec)-1] != TEXT('\\'))
		if( *lpLastChar != TEXT('\\') )
			wsprintf(szFullPath,TEXT("%s\\%s"),szFileSpec,szPath);
		else
			wsprintf(szFullPath,TEXT("%s%s"),szFileSpec,szPath);
	}
	else
		lstrcpy(szFullPath,szPath);

} //CreatePath()


//*------------------------------------------------------------------------
//|  LoadDrivesComboBox:
//|		This function loads the "Drives" combo-box
//|	Parms:
//|		hDlg	handle to Dialog
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

VOID INTFUN LoadDrivesComboBox(HWND hDlg)
{
	HWND		hCtl=NULL;
	TCHAR		szDrives[XLARGEBUFF]=TEXT(""),
				szBuff[XLARGEBUFF]=TEXT(""),
				szDrive[LARGEBUFF]=TEXT(""),
				szVol[XLARGEBUFF]=TEXT("");
	LPTSTR		pszDrives=NULL,
				pSlash=NULL;
	DWORD		wDex=0,
				dexCDrive=0,
				dwError=0;
	BOOL		fRC=0;
	NETRESOURCE rgNr[MAX_NUM_DRIVES];
	HANDLE		hEnum=NULL;
	DWORD		cCount=0xFFFFFFFF,
				iNetDrive;
	UINT		uType=0;




	if ((dwError=WNetOpenEnum(RESOURCE_CONNECTED,
				RESOURCETYPE_DISK,
				RESOURCEUSAGE_CONNECTABLE,
				NULL,
				&hEnum)) == NO_ERROR)
	{
		DWORD 	dwRC=0,
				dwBuffSize=sizeof(NETRESOURCE) *50;


		dwRC=WNetEnumResource(hEnum,&cCount,rgNr,&dwBuffSize);

		if (dwRC==ERROR_NO_MORE_ITEMS)
		{

			//Close enumeration
			WNetCloseEnum(hEnum);
			hEnum=NULL;
		}

	}
	else
	{
		TCHAR 	szErrorBuff[XLARGEBUFF]=TEXT(""),
				szNameBuf[XLARGEBUFF]=TEXT("");

		WNetGetLastError(&dwError,szErrorBuff,
								sizeof(szErrorBuff)/sizeof(TCHAR),
								szNameBuf,
								sizeof(szNameBuf)/sizeof(szNameBuf[0]));

	}


	//Get all available drives
	GetLogicalDriveStrings(sizeof(szDrives)/sizeof(szDrives[0]),szDrives);

	memset(g_rgDI,0,sizeof(DRIVEINFO));

	pszDrives=(LPTSTR)szDrives;

	lstrcpy((LPTSTR)szDrive,(LPCTSTR)szDrives);

	hCtl=GetDlgItem(hDlg,IDD_DRIVES);

	//Now load the "Drives" combo-box
	while(*pszDrives)
	{
		_tcslwr(pszDrives);

		uType=GetDriveType((LPCTSTR)pszDrives);

		*szVol=TEXT('\0');

		if (uType != DRIVE_REMOTE && uType != DRIVE_REMOVABLE)
		{
			//Get volumn information
			UINT		SEMFlags =0;
			DWORD		FSFlags =0,
						MaxCompLen=0,
						VolSerNum=0;
			TCHAR		szFSNBuffer[LARGEBUFF]=TEXT("");

			// bug #	2938:  turn of message box;
			SEMFlags = SetErrorMode(SEM_FAILCRITICALERRORS);


			fRC=GetVolumeInformation(pszDrives,
												szVol,
												sizeof(szVol)/sizeof(szVol[0]),
												&VolSerNum,
												&MaxCompLen,
												&FSFlags,
												szFSNBuffer,
												sizeof(szFSNBuffer));
			// Reset falgs
			SetErrorMode(SEMFlags);

		}


		//nuke the backslash
		if (pSlash=_tcschr(szDrive,TEXT('\\')))
			*pSlash=TEXT('\0');

		cCount=(cCount > MAX_NUM_DRIVES ? MAX_NUM_DRIVES : cCount);

		//Check if this drive is a network drive
		if (uType == DRIVE_REMOTE)
		{

			for (iNetDrive=0;iNetDrive < cCount ;iNetDrive++)
			{
				if (rgNr[iNetDrive].lpLocalName && !_tcsicmp(szDrive,rgNr[iNetDrive].lpLocalName))
				{
					lstrcpy(szVol,rgNr[iNetDrive].lpRemoteName);
					break;
				}

			}
		}

		wsprintf(szBuff,TEXT("%s %s"),szDrive,szVol);

		//add drive to combo-box
		wDex=(DWORD)SendMessage(hCtl,CB_ADDSTRING,0,(LPARAM)szBuff);

		//if (wDex >= 0 && wDex < MAX_NUM_DRIVES)
        if (wDex < MAX_NUM_DRIVES)
		{
			lstrcpy(g_rgDI[wDex].szLetter,szBuff);
			lstrcpy(g_rgDI[wDex].szVol,szVol);
		}

		//Find index of C drive
		if (!dexCDrive && _tcsstr(pszDrives,TEXT("c:")))
				dexCDrive=wDex;

		//go to next string
		pszDrives+=lstrlen(pszDrives)+1;
		lstrcpy(szDrive,pszDrives);
	}

	//Select C: drive
	wDex=(DWORD)SendMessage(hCtl,CB_SETCURSEL,dexCDrive,0L);
	g_diCurDrive=g_rgDI[dexCDrive];


} //LoadDrivesComboBox()



//*------------------------------------------------------------------------
//|  LoadAllFiles:
//|		This function loads,into the control, the file types specified by
//|	the uFileAttr arg.
//|	Parms:
//|		hDlg			handle to Dialog
//|		dwFileAttr	File attribute
//|	Returns:
//|		dwDexFirst	index of first item in the list
//*------------------------------------------------------------------------

DWORD INTFUN LoadAllFiles(HWND hCtl,DWORD dwFileAttr,LPTSTR szFileSpec)
{
	WIN32_FIND_DATA	fd;
	BOOL					fFound=TRUE;
	HANDLE				hHandle=NULL;
	TCHAR					szFileName[_MAX_PATH]=TEXT("");
	UDWORD				wDex=0;

	//clear list box
	SendMessage(hCtl,LB_RESETCONTENT,0L,0L);

	//reload the list box
	if ((hHandle=FindFirstFile(szFileSpec,&fd)) !=INVALID_HANDLE_VALUE)
	{
		while (fFound)
		{
			//add file only if it matches attribute we are looking
			//for
			if (fd.dwFileAttributes & dwFileAttr)
			{
				//add directory to combo-box
				wDex=(DWORD)SendMessage(hCtl,LB_ADDSTRING,0,(LPARAM)(LPTSTR)fd.cFileName);
			}

			fFound=FindNextFile(hHandle, &fd);
		}

	}

	return(wDex);

} //LoadAllFiles()


//*------------------------------------------------------------------------
//|  LoadDirectoriesComboBox:
//|		This function loads the "Directories" combo-box
//|	Parms:
//|		hDlg	handle to Dialog
//|		szCWD	current working directory (used in the INITDIALOG)
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

VOID INTFUN LoadDirectoriesComboBox(HWND hDlg,LPTSTR szCWD)
{
	HANDLE				hHandle=NULL;
	TCHAR					szFileName[_MAX_PATH]=TEXT("");
	DWORD					dwDexFirst;
	TCHAR					szFileSpec[_MAX_PATH]=TEXT(""),
							szTemp[_MAX_PATH]=TEXT("");

	//add "*.*" to the current working directory
	CreatePath(szCWD,(const LPTSTR)g_szWILD,(const LPTSTR)szFileSpec);

	dwDexFirst=LoadAllFiles(GetDlgItem(hDlg,IDD_DIRLIST),FILE_ATTRIBUTE_DIRECTORY,
						szFileSpec);

	lstrcpy(szTemp,szCWD);

	//add directory to combo-box
	DoSetFile(GetDlgItem(hDlg, IDD_CURDIR), (LPCTSTR)szTemp);

}	//LoadDirectoriesComboBox()


//*------------------------------------------------------------------------
//|  LoadFileListComboBox:
//|		This function loads the "Files" combo-box
//|	Parms:
//|		hDlg	handle to Dialog
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

VOID INTFUN LoadFileListComboBox(HWND hDlg,LPTSTR szFileSpec,LPTSTR szWild)
{
	DWORD dwDexFirst;
	HWND	hCtl=GetDlgItem(hDlg,IDD_NAME);
	TCHAR	szBuff[_MAX_PATH]=TEXT("");

	CreatePath(szFileSpec,szWild,szBuff);

	//Initialize file listing
	dwDexFirst=LoadAllFiles(GetDlgItem(hDlg,IDD_FILELIST),uAllFilesAttrib,szBuff);

	//Limit text length in edit control
	SendMessage(hCtl,EM_LIMITTEXT,_MAX_PATH,0);

	//Put "auto.log" into edit control
	SendMessage(hCtl,WM_SETTEXT,0,(LPARAM)g_szCurLogFile);

	//Select text in edit control
	SendMessage(hCtl,	EM_SETSEL,0,MAKELONG(0,_MAX_PATH));
}


//*------------------------------------------------------------------------
//|  OpenDlg_OnInitDialog:
//|		This function handles the WM_INITDIALOG msg
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

BOOL INTFUN OpenDlg_OnInitDialog(HWND hwnd,HWND hwndFocus,LPARAM lParam,
											lpRUNTESTLIST rtl)
{
	HWND		hCtl=NULL;
	TCHAR		szCWD[_MAX_PATH]=TEXT("");

	LoadDrivesComboBox(hwnd);

	lstrcpy(g_szWildCard,g_szLOG);
	lstrcpy(g_szCurLogFile,rtl->rs->szLogFile);

	CheckDlgButton(hwnd,IDC_APPEND_FILE,rtl->fAppend);

	if (!*g_szCurDir)
		lstrcpy(g_szCurDir,_tgetcwd(szCWD,sizeof(szCWD)/sizeof(szCWD[0])));


	LoadDirectoriesComboBox(hwnd,g_szCurDir);

	LoadFileListComboBox(hwnd,g_szCurDir,g_szWildCard);

	hCtl=GetDlgItem(hwnd,IDD_FILE_TYPES);

	//Load File Types combo box
	SendMessage(hCtl,CB_ADDSTRING,0,(LPARAM)(LPTSTR)g_szLOGTYPE);
	SendMessage(hCtl,CB_ADDSTRING,0,(LPARAM)(LPTSTR)g_szWILDTYPE);

	//set "*.log" as current
	SendMessage(hCtl,CB_SETCURSEL,0,0L);

	//Select text in edit control
	SendMessage(hCtl,	EM_SETSEL,0,MAKELONG(0,_MAX_PATH));

	return(TRUE);

} //OpenDlg_OnInitDialog()


//*------------------------------------------------------------------------
//|  SetNameField:
//|		This function sets the text of the IDD_NAME field
//|	Parms:
//|		hCtl			handle to control
//|		szFName		File name
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

VOID INTFUN SetNameField(HWND hCtl,LPTSTR szFName)
{
	WIN32_FIND_DATA	fd;
	HANDLE				hHandle=NULL;
	TCHAR					szBuff[_MAX_PATH]=TEXT("");


	CreatePath(g_szCurDir,szFName,szBuff);

	//Get attributes of file
	if ((hHandle=FindFirstFile(szBuff,&fd)) != INVALID_HANDLE_VALUE)
	{
		//If file is not a directory then display it
		if (fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{

			SendMessage(hCtl,
							WM_SETTEXT,
							0,
							(LPARAM)(LPTSTR)szFName);
		}
	}

} //SetNameField()


//*------------------------------------------------------------------------
//|  GetCurSel:
//|		This function retrieves the current selection from a list box
//|	Parms:
//|		hCtl			handle to control
//|		szCurSel		currently selected item
//|	Returns:
//|		szCurSel		currently selected item
//*------------------------------------------------------------------------

__inline LPTSTR INTFUN GetCurSel(HWND hCtl,LPTSTR szCurSel)
{
	DWORD	dwDex=0;

	//get current selected text from Directories list box
	dwDex=(DWORD)SendMessage(hCtl,LB_GETCURSEL,0,0);
	SendMessage(hCtl,LB_GETTEXT,dwDex,(LPARAM)szCurSel);

	return(szCurSel);
}


//*------------------------------------------------------------------------
//|  OpenDlg_OnCommand:
//|		This function handles the WM_COMMAND msg
//|	Parms:
//|		standard window parms
//|	Returns:
//|		Nothing.
//*------------------------------------------------------------------------

BOOL INTFUN OpenDlg_OnCommand(HWND hwnd,
										int  id,
										HWND hwndCtl,
										UINT codeNotify,
										lpRUNTESTLIST rtl)
{

	BOOL					bWild;
	TCHAR					szBuff[_MAX_PATH];
	HWND					hCtl;
	int					cc;
	LPTSTR				p;
	TCHAR					szTemp[_MAX_PATH],
							szMsg[XLARGEBUFF];
	DWORD					dwError;
	TCHAR 				szFname[_MAX_PATH];
	TCHAR					szExt[5];
	TCHAR					szDrive[3];
	TCHAR					szPth[_MAX_PATH];
	DWORD					dex=0;

	switch(id)
	{
		case IDOK:
		{
			//Read file name from edit control
			hCtl=GetDlgItem(hwnd,IDD_NAME);
			SendMessage(hCtl,
							WM_GETTEXT,
							_MAX_PATH,
							(LPARAM)szBuff);

			_tsplitpath(szBuff, szDrive, szPth, szFname, szExt);

			//Trim leading blanks from file name
			p=szBuff;
			while (*p == TEXT(' '))
				p++;

			if (p != szBuff)
				lstrcpy(szBuff, p);

			//Trim trailing blanks from file name
			cc = lstrlen(szBuff);
			for (p = szBuff + cc - 1;*p == TEXT(' '); p--)
				*p = 0;

			bWild = _tcschr(szBuff,TEXT('?')) || _tcschr(szBuff,TEXT('*'));


			CreatePath(g_szCurDir,szBuff,szTemp);

			_tcslwr(szTemp);

			//tack on ".log" if user didn't specify
			if (!*szExt)
				lstrcat(szTemp,TEXT(".log"));

			//If no wild cards, try to open it
			if (!bWild)
			{
				//Get the Append Flag
				if (!(rtl->fAppend=IsDlgButtonChecked(hwnd, IDC_APPEND_FILE)) &&
					!_taccess(szBuff,FILE_EXISTS))
				{
					wsprintf(szMsg,TEXT("%s already exists.\r\nDo you want to replace it?"),szTemp);
					g_nRC=GtrMessageBox(hwnd,szMsg,TEXT("Log File"),MB_ICONINFORMATION  | MB_YESNO);
					if (g_nRC == IDNO)
						break;

				}

				//store log file name
				lstrcpy(rtl->rs->szLogFile,szTemp);

				EndDialog(hwnd,TRUE);

				break;

			}

			//make local copy of wild card
			lstrcpy(g_szWildCard, szBuff);

			lstrcpy(g_szCurDir,szDrive);
			lstrcat(g_szCurDir,szPth);

			LoadDirectoriesComboBox(hwnd,g_szCurDir);

			LoadFileListComboBox(hwnd,g_szCurDir,szBuff);

			dex=(DWORD)SendMessage(GetDlgItem(hwnd,IDD_DRIVES), CB_FINDSTRING,0,(LPARAM)szDrive);

			if (dex != CB_ERR)
				SendMessage(GetDlgItem(hwnd,IDD_DRIVES), CB_SETCURSEL,dex,0L);

			//set the "List Files of Type" combo box if needed
			if (!_tcscmp(szBuff,g_szWILD))
				SendMessage(GetDlgItem(hwnd, IDD_FILE_TYPES),CB_SETCURSEL,1L,0L);
			else if (!_tcscmp(szBuff,g_szLOG))
				SendMessage(GetDlgItem(hwnd, IDD_FILE_TYPES),CB_SETCURSEL,0L,0L);

			//put the wild card into edit control
			hCtl=GetDlgItem(hwnd, IDD_NAME);
			SendMessage(hCtl,
							WM_SETTEXT,
							0,
							(LPARAM)g_szWildCard);


			//select text in edit control
			SendMessage(hCtl,
							EM_SETSEL,
							0,
							MAKELONG(0,_MAX_PATH));

			//give keyboard to edit control
			SetFocus(hCtl);
		}
		break;

		case IDCANCEL:
			EndDialog(hwnd,FALSE);
			break;
		case IDNETWORK:
			dwError=WNetConnectionDialog(hwnd,RESOURCETYPE_DISK);
			break;
		case IDD_NAME:
			if (codeNotify == EN_CHANGE)
			{
				//enable or disable push button
				//based on whether edit control is
				//empty or not
				cc=(int)SendMessage(hwndCtl,
											WM_GETTEXTLENGTH,
											0,0L);

				hCtl = GetDlgItem(hwnd, IDOK);
				EnableWindow(hCtl,cc);
			}
		break;

		case IDD_FILE_TYPES:
			//Get new File Type
			if (codeNotify == CBN_SELENDOK)
			{
				GetDlgItemText(hwnd,IDD_FILE_TYPES,szBuff,sizeof(szBuff)/sizeof(szBuff[0]));

				if (!_tcscmp(szBuff,g_szWILDTYPE))
					lstrcpy(g_szWildCard,g_szWILD);
				else
					lstrcpy(g_szWildCard,g_szLOG);

				//reload the file list list box with new wildcard
				LoadFileListComboBox(hwnd,g_szCurDir,g_szWildCard);

				//Set text in the IDD_NAME edit box
				SetNameField(GetDlgItem(hwnd,IDD_NAME),szBuff);
			}
		break;

		case IDD_DRIVES:
			//Get new File Type
			if (codeNotify == CBN_SELENDOK)
			{
				LPTSTR	pColon=NULL;
				int		nRC;
				TCHAR		szDrive[SMALLBUFF]=TEXT("");
				BOOL		fFound=FALSE;
				int		iDrive;

				GetDlgItemText(hwnd,IDD_DRIVES,szBuff,sizeof(szBuff)/sizeof(szBuff[0]));

				if (pColon=_tcschr(szBuff,TEXT(':')))
				{
					pColon++;
					*pColon=TEXT('\0');
					lstrcpy(szDrive,szBuff);
					lstrcat(szBuff,TEXT("\\"));

					do {
						//change to specified Drive
						if (_tchdir(szBuff) != -1)
						{
							lstrcpy(g_szCurDir,szBuff);

							//find index of current drive
							for (iDrive=0;iDrive < MAX_NUM_DRIVES;iDrive++)
							{
								if (_tcsstr(g_rgDI[iDrive].szLetter,szDrive))
								{
									fFound=TRUE;
									break;
								}
							}

							if (fFound)
								g_diCurDrive=g_rgDI[iDrive];

							//reload the file list list box with new wildcard
							LoadFileListComboBox(hwnd,g_szCurDir,g_szWildCard);
							LoadDirectoriesComboBox(hwnd,g_szCurDir);
						}
						else
						{
							TCHAR	szErr[LARGEBUFF]=TEXT("");

							//display error dialog
							wsprintf(szErr,TEXT("%s is not accessible.\r\n\r\nThe device is not ready\r\n\r\n"),szBuff);
							nRC=GtrMessageBox(hwnd,szErr,TEXT("Log File"),MB_ICONSTOP | MB_RETRYCANCEL);

						}

					} while (nRC==IDRETRY);

					if (nRC == IDCANCEL)
					{
						//reset the the Drives combo-box

					}
				}

			}
		break;

		case IDD_FILELIST:
			if (codeNotify == LBN_SELCHANGE)
			{
				GetCurSel(GetDlgItem(hwnd,IDD_FILELIST),szBuff);

				//Set text in the IDD_NAME edit box
				SendMessage(GetDlgItem(hwnd,IDD_NAME),
								WM_SETTEXT,
								0,
								(LPARAM)szBuff);

			}

			if (codeNotify == LBN_DBLCLK)
				SendMessage(hwnd,WM_COMMAND,IDOK,0L);

			break;

		case IDD_DIRLIST:
			if (codeNotify == LBN_SELCHANGE)
			{
				GetCurSel(GetDlgItem(hwnd,IDD_DIRLIST),szBuff);

				//Set text in the IDD_NAME edit box
				SetNameField(GetDlgItem(hwnd,IDD_NAME),szBuff);
			}

			if (codeNotify == LBN_DBLCLK)
			{

				GetCurSel(GetDlgItem(hwnd,IDD_DIRLIST),szBuff);

				//Set the current directory
				CreatePath(g_szCurDir,szBuff,szTemp);

				//get full path from relative path
				_tfullpath(g_szCurDir,szTemp,sizeof(g_szCurDir));

				LoadDirectoriesComboBox(hwnd,g_szCurDir);

				LoadFileListComboBox(hwnd,g_szCurDir,g_szWildCard);

				//select all text in the edit control
				SendMessage(GetDlgItem(hwnd, IDD_NAME),
								EM_SETSEL,
								0,
								MAKELONG(0,_MAX_PATH));

				SetFocus(GetDlgItem(hwnd,IDD_DIRLIST));
			}

			break;
	}

	return(TRUE);

} //OpenDlg_OnInitDialog()


//*------------------------------------------------------------------------
//|  OpenDlgProc:
//|		This window proc handles the Autotest Log file dialog
//|	Parms:
//|		standard window parms
//|	Returns:
//|		TRUE/FALSE
//*------------------------------------------------------------------------

LRESULT EXTFUN OpenDlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			lpRUNTESTLIST rtl = (lpRUNTESTLIST)lParam;

			SETWINDOWLONG(hDlg, lParam);
			CenterDialog(hDlg);

			if(!*szLogFilter)
				GetFilter((LPTSTR)szLogFilter, MAXOUTBUFF, idsLogFilter);

			if(!*szLogTitle)
				GetString((LPTSTR)szLogTitle, MAXTITLE, idsLogTitle);

			OpenDlg_OnInitDialog(hDlg,(HWND)wParam,lParam,rtl);

		}
		return TRUE;

		case WM_COMMAND:
		{
			lpRUNTESTLIST rtl = (lpRUNTESTLIST)lParam;

			rtl = (RUNTESTLIST *)GETWINDOWLONG(hDlg);

			OpenDlg_OnCommand(hDlg,
									LOWORD(wParam),
									(HWND)lParam,
									HIWORD(wParam),
									rtl);
		}
		return FALSE;

		// Must unregister 3D stuff and end dialog
		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			return TRUE;

		default:
			return FALSE;
		}

} //OpenDlgProc()

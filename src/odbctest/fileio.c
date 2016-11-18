//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FILEIO.C
//|
//| Purpose:
//|		This module contains the file I/O functions.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include <share.h>
#include "fileio.h"


BOOL EXTFUN IsWindowsWOW(void);
BOOL EXTFUN IsWin32s(void);
BOOL EXTFUN IsWindowsMIPS(void);
BOOL EXTFUN IsWindowsDEC(void);
BOOL EXTFUN IsWindowsIntel(void);
LPTSTR EXTFUN DLLGetEnv (LPTSTR lpszVariableName);
BOOL EXTFUN IsChicago(void);
VOID INTFUN SetupFileInfo(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
		LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL FAR * lpfAppend,
		OPENFILENAME *lpofn);


OPENFILENAME ofnGlobal;

//*---------------------------------------------------------------------------------
//| FileExist:
//|	This function will return TRUE if file exist, otherwise FALSE
//|	FALSE
//| Parms:
//|	szFile		File to search for
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

BOOL INTFUN FileExist(LPCTSTR szFile)
{
    HANDLE hf;

	hf = GtrCreateFile(szFile,
					GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,					// if not exist flag error
					FILE_ATTRIBUTE_NORMAL,		// file attributes
					NULL);				// handle of file with attributes to copy

    if (hf != INVALID_HANDLE_VALUE) {
        CloseHandle(hf);
    }

	return hf != INVALID_HANDLE_VALUE;

} //FileExist()


//*---------------------------------------------------------------------------------
//| GetFilePath:
//|	This function will return the file's full path + file name
//|	FALSE
//| Parms:
//|	szFile		File to search for
//|	szFileSpec	Full Path to file + file name
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

LPTSTR INTFUN GetFilePath(LPCTSTR szFile)
{
	LPTSTR					szFileSpec=NULL;
   SECURITY_ATTRIBUTES  sa;
   HANDLE					hTemplateFile=NULL,
								hf;

	hf=GtrCreateFile(szFile,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				&sa,
				OPEN_EXISTING,					// if not exist flag error
				FILE_ATTRIBUTE_NORMAL,		// file attributes
				hTemplateFile);				// handle of file with attributes to copy

	if (hf !=INVALID_HANDLE_VALUE)
		return(szFileSpec);
	else
		return(NULL);

} //GetFilePath()


//*---------------------------------------------------------------------------------
//| GtrOpenFile:
//|	This function will open the specified file
//| Parms:
//|	szFile		File to search for
//|	szFileSpec	Full Path to file + file name
//|	dwMode		Mode to open file in
//|	fRead			Read from file or Write to file
//|	ids			ids of message to display upon error
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

HANDLE INTFUN GtrOpenFile(HWND hwndIn,LPTSTR szFile,DWORD dwMode,
	BOOL fRead,UINT ids)
{
	TCHAR		szOutBuff[MAXOUTBUFF];
	LPTSTR	szFileSpec=NULL;
   HANDLE	hf=NULL;
	DWORD		dwDesiredAccess;
	DWORD		dwShareMode;

	if (fRead)
	{
		dwDesiredAccess=GENERIC_READ;
		dwShareMode=FILE_SHARE_READ;
	}
	else
	{
		dwDesiredAccess=GENERIC_WRITE;
		dwShareMode=FILE_SHARE_WRITE;
	}

	hf=GtrCreateFile(szFile,
				dwDesiredAccess,
				dwShareMode,
				NULL,
				dwMode,							// if not exist flag error
				FILE_ATTRIBUTE_NORMAL,		// file attributes
				0);							// handle of file with attributes to copy

	if (ids && (hf == INVALID_HANDLE_VALUE))
	{
		szMessageBox(hwndIn,
					MB_ICONEXCLAMATION | MB_OK,
					NULL,
					GetGtrString(szOutBuff, MAXOUTBUFF, ids),
					(LPTSTR)szFile);
	}

	return(hf);

} //GetFilePath()



//*---------------------------------------------------------------------------------
//| GtrWriteFile:
//|	This function will return the file's full path + file name
//|	FALSE
//| Parms:
//|	szFile		File to search for
//|	szFileSpec	Full Path to file + file name
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

UINT INTFUN GtrWriteFile(HWND hwndIn,HANDLE hFile,LPTSTR szFile,UWORD uLen)
{
	TCHAR 	szOutBuff[MAXOUTBUFF];
	DWORD		uCb=0;
	CHAR*	szBuf =AllocateMemory(uLen);


#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,szFile,uLen,szBuf,uLen,NULL,NULL);
#else
	lstrcpy(szBuf,szFile);
#endif

	CharToOemA(szBuf,szBuf);
    WriteFile(hFile, szBuf, uLen, &uCb, NULL);

	if(uCb < (UINT)uLen || uCb == HFILE_ERROR)
		szMessageBox(hwndIn,
		MB_ICONEXCLAMATION | MB_OK,
		NULL,
		GetGtrString(szOutBuff, MAXOUTBUFF, idsErrSaveFileNoBytes),
		szFile);

	free(szBuf);

	return(uCb);

} //GtrWriteFile()



//*---------------------------------------------------------------------------------
//| GtrCloseFile:
//|	This function will closes the specified file
//|	FALSE
//| Parms:
//|	hFile			handle of file to close
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

HFILE INTFUN GtrCloseFile(HANDLE hFile)
{
	CloseHandle(hFile);
	return 0;

} //GtrCloseFile()


//*---------------------------------------------------------------------------------
//| GtrSeekFile:
//|	This function will perform a seek on the specified file
//|	FALSE
//| Parms:
//|	hFile			handle of file to seek
//|	lOffset		Specifies the number of bytes the file pointer is to be moved.
//|	iOrigin		Specifies the starting position and direction of the file
//|					pointer. This parameter must be one of the following values:
//| Returns:
//|	the new offset of the pointer, in bytes, from the beginning of the file.
//*---------------------------------------------------------------------------------

LONG INTFUN GtrSeekFile(HANDLE hFile, LONG  lOffset, int  iOrigin)
{
//	return(_llseek((HFILE)hFile, 0L, 2));
    return SetFilePointer(hFile, 0, 0, FILE_END);

} //GtrSeekFile()


//*---------------------------------------------------------------------------------
//| GetRecordFile:
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
BOOL EXTFUN GetRecordFile(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
			LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL FAR * lpfAppend)
{
	OPENFILENAME	ofn;
	DWORD				dwMode=CREATE_ALWAYS;
	FILE				*fhRecFile=NULL;
	DWORD				dwWritten=0;
	TCHAR				szTmpFile[_MAX_PATH]=TEXT("");


	//SetupFileInfo(hwnd,hInst,szTitle,szFilter,szDftExt,szFile,uAttrib,lpfAppend,&ofn);

	lstrcpy((LPTSTR)szTmpFile, szFile);

	_fmemset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= hwnd;
	ofn.hInstance		= hInst;
	ofn.lpstrTitle		= szTitle;
	ofn.lpstrFilter		= szFilter;
	ofn.lpstrFile		= szTmpFile;
	ofn.nMaxFile			= _MAX_PATH;
	ofn.lpstrDefExt		= szDftExt;
	ofn.lpstrInitialDir	= NULL;
	ofn.Flags			  = OFN_HIDEREADONLY	 |
								OFN_PATHMUSTEXIST	 |
								OFN_OVERWRITEPROMPT	 |
								OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn))
	{
		HFILE		hFile=0;

		lstrcpy(szFile, ofn.lpstrFile);
		*lpfAppend=(BOOL)ofnGlobal.lCustData;

		fhRecFile = _tfsopen(szFile,TEXT("a"),_SH_DENYNO);

		if (fhRecFile)
			fclose(fhRecFile);

		return TRUE;
	}


	return FALSE;				// Canceled or whatever
}


//*---------------------------------------------------------------------------------
//| WriteRecordFile:
//|	This function will return the file's full path + file name
//|	FALSE
//| Parms:
//|	szFile		File to search for
//|	szFileSpec	Full Path to file + file name
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

int INTFUN WriteRecordFile(HWND hwndIn,FILE *fhRecordFile,LPTSTR szBuffer,UWORD cbBuff)
{
	TCHAR 	szOutBuff[MAXOUTBUFF];
	int		numwritten=0;

	numwritten=fwrite(szBuffer, lstrlen(szBuffer),1,fhRecordFile);

	if(numwritten < cbBuff)
		szMessageBox(hwndIn,
		MB_ICONEXCLAMATION | MB_OK,
		NULL,
		GetGtrString(szOutBuff, MAXOUTBUFF, idsErrSaveFileNoBytes),
		szBuffer);

	return(numwritten);

} //WriteRecordFile()



//*---------------------------------------------------------------------------------
//| CloseRecordFile:
//|	This function will closes the specified file
//|	FALSE
//| Parms:
//|	fhRecordFile		handle of file to close
//| Returns:
//|	HANDLE		handle to file
//*---------------------------------------------------------------------------------

int INTFUN CloseRecordFile(FILE *fhRecordFile)
{
	return(fclose(fhRecordFile));

} //CloseRecordFile()

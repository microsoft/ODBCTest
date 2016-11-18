//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	COMMON.C
//|
//| Purpose:
//|		Header file for utility functions.
//*---------------------------------------------------------------------------------
#ifndef GLOBALS_DEFS
#define GLOBALS_DEFS

#define tsizeof(szBuff)		sizeof(szBuff)/sizeof(TCHAR)

typedef struct tagODBCAPI
{
        int		iAPINum;			// the API number in SQLGetFunctions (i.e. SQL_API_????)
        int		iOrdinalNum;	// the ordinal number
		  LPTSTR szAPIName;		// SQL_API_??? as  a string
        LPTSTR szName;			// the function name
		  BOOL	fODBC30;			// Is this a ODBC 3.0 function
		  int		iDD;				// ID of it's associated dialog box
} ODBCAPI_LIST;


extern const ODBCAPI_LIST rgApiList[NUM_ODBC_APIS];
LPTSTR INTFUN GetApiConstant(UINT dex, LPTSTR str);


int GtrMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
BOOL GatorSetWindowText(HWND hWnd,LPCTSTR lpString);
DWORD GtrSearchPath(LPCTSTR lpPath,LPCTSTR lpFileName,LPCTSTR lpExtensions, DWORD nBufferLength,
				 LPTSTR lpBuffer,LPTSTR *lpFilePart);
HINSTANCE GtrLoadLibrary( LPCTSTR lpLibFileName);
HANDLE GtrCreateFile(LPCTSTR lpFileName,DWORD dwDesiredAccess,DWORD dwSharedMode,LPSECURITY_ATTRIBUTES
				  lpSecurityAttributes,DWORD dwCreationDistribution,DWORD FlagsAndAttributes,
				  HANDLE hTemplateFile);
DWORD GtrFormatMessage(DWORD dwFlags,LPCVOID lpSource, DWORD dwMessageid, DWORD dwLanguageid,
					LPTSTR lpBuffer,DWORD nSize,va_list * Arguments);
int GtrLoadString(HINSTANCE hInstance,UINT uID, LPTSTR lpBuffer, int nBufferMax);
BOOL GtrWritePrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpString,
							   LPCTSTR lpFileName);
DWORD GtrGetPrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault,
							 LPTSTR lpReturnedString, DWORD nSize,LPCTSTR lpFileName);
UINT GtrGetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName, INT nDefault,LPCTSTR lpFileName);
int _openA(const wchar_t *filename,int oflag,int pmode);

#endif

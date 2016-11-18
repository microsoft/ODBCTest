//*---------------------------------------------------------------------------------
//|  Title:		FILEIO.H
//|		Copyright (c) Microsoft, All rights reserved
//|
//|	This file prototypes the File I/O functions.
//*---------------------------------------------------------------------------------
#ifndef FILEIO_DEFS
#define FILEIO_DEFS

//Used by WriteLogInfo()
#define LOG_CONTINUE				1		//No error continue to log
#define LOG_ERROR_CONTINUE		0		//Error but user wants to continue to log
#define LOG_STOP					-1		//Stop logging

#define WRITETOFILE				0
#define READFROMFILE				1


BOOL INTFUN FileExist(LPCTSTR szFile);
LPTSTR INTFUN GetFilePath(LPCTSTR szFile);
HANDLE INTFUN GtrOpenFile(HWND hwndIn,LPTSTR szFile,DWORD dwMode,
	BOOL fRead,UINT ids);
UINT INTFUN GtrWriteFile(HWND hwndIn,HANDLE hFile,LPTSTR lpstr,UWORD uLen);
LONG INTFUN GtrSeekFile(HANDLE hFile, LONG  lOffset, int  iOrigin);
HFILE INTFUN GtrCloseFile(HANDLE hFile);
BOOL EXTFUN GetRecordFile(HWND hwnd, HINSTANCE hInst, LPTSTR szTitle,
			LPTSTR szFilter, LPTSTR szDftExt, LPTSTR szFile, UINT uAttrib, BOOL FAR * lpfAppend);
int INTFUN WriteRecordFile(HWND hwndIn,FILE *fhRecordFile,LPTSTR szBuffer,UWORD cbBuff);
int INTFUN CloseRecordFile(FILE *fhRecordFile);

_inline VOID INTFUN WriteNewLine(FILE *fhFile)
{
	LPSTR lpCRLF=");\r\n";
	fwrite(lpCRLF, strlen(lpCRLF),1,fhFile);

} //WriteNewLine()


#endif



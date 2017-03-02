/*--------------------------------------------------------------------------
  ODBCVer.h -- ODBC Version info
--------------------------------------------------------------------------*/


// Includes ----------------------------------------------------------------
// WIN16 is only defined in building odbc16gt.dll and ds16gt.dll
#ifdef WIN16
#include <ver.h>
#endif

#define VER_FILEFLAGSMASK (VS_FF_DEBUG | VS_FF_PRERELEASE)
#ifdef DEBUG
#define VER_FILEFLAGS (VS_FF_DEBUG)
#else
#define VER_FILEFLAGS (0)
#endif

#ifdef WIN16
#define	VER_FILEOS	VOS_DOS_WINDOWS16
#else
#define	VER_FILEOS	VOS__WINDOWS32
#endif

#define VER_COMPANYNAME_STR     "Microsoft Corporation\0"
#define VER_PRODUCTNAME_STR     "Microsoft Open Database Connectivity\0"
#define VER_LEGALCOPYRIGHT_STR	"Copyright (C) Microsoft Corporation\0"

// compare the version of this DLL (e.g. DM, Cursor lib etc) with the version 
// of resource DLL. They must match
#ifndef KAGERA
__inline BOOL DLLSameVer(HINSTANCE hDLL1, HINSTANCE hDLL2)
{
	CHAR szDLLPath1[MAX_PATH + 1];
	CHAR szDLLPath2[MAX_PATH + 1];
	DWORD dwVerInfoSize1;
	DWORD dwVerInfoSize2;
	BOOL fRet = FALSE;
	DWORD dwVerHnd1;
	DWORD dwVerHnd2;
	UINT uiLen;


	GetModuleFileNameA(hDLL1, szDLLPath1, sizeof(szDLLPath1));
	GetModuleFileNameA(hDLL2, szDLLPath2, sizeof(szDLLPath2));

	dwVerInfoSize1 = GetFileVersionInfoSizeA(szDLLPath1, &dwVerHnd1);
	dwVerInfoSize2 = GetFileVersionInfoSizeA(szDLLPath2, &dwVerHnd2);

	if (dwVerInfoSize1 && dwVerInfoSize2)
	{
		VOID * pvVerInfo1;
		VOID * pvVerInfo2;
		VS_FIXEDFILEINFO * pFileInfo1;
		VS_FIXEDFILEINFO * pFileInfo2;

		// Get a block big enough to hold version info
		pvVerInfo1 = malloc(dwVerInfoSize1);
		pvVerInfo2 = malloc(dwVerInfoSize2);

		// Get the info and format a version string.
		if(pvVerInfo1 && pvVerInfo2)
		{
			if(GetFileVersionInfoA(szDLLPath1, 0L, dwVerInfoSize1, pvVerInfo1) &&
				GetFileVersionInfoA(szDLLPath2, 0L, dwVerInfoSize2, pvVerInfo2))				
			{
				if(VerQueryValueA(pvVerInfo1, "\\", (VOID **) &pFileInfo1, &uiLen) &&
					VerQueryValueA(pvVerInfo2, "\\", (VOID **) &pFileInfo2, &uiLen))
				{
					if(pFileInfo1->dwFileVersionMS == pFileInfo2->dwFileVersionMS &&
						pFileInfo1->dwFileVersionLS == pFileInfo2->dwFileVersionLS)
					{
						fRet = TRUE;
					}
				}
			}
		}

		if(pvVerInfo1)
		{
			free(pvVerInfo1);		
		}
		if(pvVerInfo2)
		{
			free(pvVerInfo2);	
		}	
 	}
	return fRet;
}
#endif

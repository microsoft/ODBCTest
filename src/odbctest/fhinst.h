//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHINST.H
//|
//| Purpose:
//|		Contains constants and prototypes for functions in FHINST.C
//*---------------------------------------------------------------------------------
#ifndef FHINST_DEFS
#define FHINST_DEFS


#define DFT_TEXT_LEN			100




typedef struct tagINSTALLER {
	HWND		hwndParent;

	TCHAR		szArgs[MEDBUFF];
	TCHAR		szInfFile[MEDBUFF];
	TCHAR		szTranslator[MEDBUFF];
	TCHAR		szPathIn[MEDBUFF];
	TCHAR		szDriver[MEDBUFF];

	DWORD		dwUsageCount;

	TCHAR		szMsg[MEDBUFF];
	TCHAR		szPathOut[MEDBUFF];

	WORD		cbPathOutMax;
	WORD		fRequest;
	WORD		cbMsgMax;

	WORD		cbPathOut;
	WORD		cbMsgOut;

	RETCODE		fRemoveDSN;

} INSTALLER, *lpINSTALLER;


#define STD_INST_PARMS lpINSTALLER lpInst
#define STD_ACTUAL_INST_PARMS lpInst

//@INSTALLER
RETCODE INTFUN lpSQLRemoveDriver(STD_FH_PARMS);
RETCODE INTFUN lpSQLRemoveDriverManager(STD_FH_PARMS);
RETCODE INTFUN lpSQLInstallTranslator(STD_FH_PARMS);
RETCODE INTFUN lpSQLInstallTranslatorEx(STD_FH_PARMS);
RETCODE INTFUN lpSQLRemoveTranslator(STD_FH_PARMS);
RETCODE INTFUN lpConfigTranslator(STD_FH_PARMS);
RETCODE INTFUN lpConfigDSN(STD_FH_PARMS);
RETCODE INTFUN lpSQLConfigDriver(STD_FH_PARMS);
RETCODE INTFUN lpSQLConfigDataSource(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetInstalledDrivers(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetPrivateProfileString(STD_FH_PARMS);
RETCODE INTFUN lpSQLInstallDriver(STD_FH_PARMS);
RETCODE INTFUN lpSQLInstallDriverManager(STD_FH_PARMS);
RETCODE INTFUN lpSQLManageDataSources(STD_FH_PARMS);
RETCODE INTFUN lpSQLRemoveDefaultDataSource(STD_FH_PARMS);
RETCODE INTFUN lpSQLRemoveDSNFromIni(STD_FH_PARMS);
RETCODE INTFUN lpSQLValidDSN(STD_FH_PARMS);
RETCODE INTFUN lpSQLWriteDSNToIni(STD_FH_PARMS);
RETCODE INTFUN lpSQLWritePrivateProfileString(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetTranslator(STD_FH_PARMS);
BOOL EXTFUN GetInstallerInfoWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

RETCODE INTFUN lpSQLConfigTranslator(STD_FH_PARMS);
RETCODE INTFUN lpSQLRemoveDSNFromIni(STD_FH_PARMS);
RETCODE INTFUN lpSQLCreateDataSource(STD_FH_PARMS);
RETCODE INTFUN lpSQLInstallerError(STD_FH_PARMS);
RETCODE INTFUN lpSQLInstallDriverEx(STD_FH_PARMS);
RETCODE INTFUN lpSQLPostInstallerError(STD_FH_PARMS);
RETCODE INTFUN lpSQLWriteFileDSN(STD_FH_PARMS);
RETCODE INTFUN lpSQLReadFileDSN(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetConfigMode(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetConfigMode(STD_FH_PARMS);

#endif

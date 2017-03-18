//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGPARMS.C
//|
//| Purpose:
//|		This module provides us with parameter list descriptors for each
//|		function in the ODBC API.
//|
//|	Notes:
//|		In some cases dwDex is preset to a value to indicate how it is to
//|		be initialized.  These are the most common cases:
//|			DEX_DFT_ZERO			Indicates a default of 0 is to be used, mostly
//|											used on edit boxes where default buff len
//|											would otherwise be used.
//|			DEX_DFT_ONE				Indicates a default of 1, ususally good for
//|											column numbers, parm numbers, etc...
//|			1							Used for a cb value when tied to a sz value that
//|											contains <input window>.  Causes the calculated
//|											length from <input window> to be used as default.
//|
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "odbcinst.h"
#include "fhinst.h"
#include "dlgdfts.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;

extern lpUSROPTIONS lpUsrOptions;

lpDFTARRAY 	lpdftSqlCodes = (lpDFTARRAY)lpdSqlCodes;
lpDFTARRAY 	lpdftTrueFalse = (lpDFTARRAY)lpdTrueFalse;
lpDFTARRAY 	lpdftSQLTrueFalse = (lpDFTARRAY)lpdSQLTrueFalse;
lpDFTARRAY	lpdftParamType = (lpDFTARRAY)lpdfParamType;
lpDFTARRAY  lpdftCTypes   = (lpDFTARRAY)lpdfCTypes;
lpDFTARRAY	lpdftCTypesNoD = (lpDFTARRAY)lpdfCTypesNoD;
lpDFTARRAY	lpdftSqlTypes = (lpDFTARRAY)lpdfSqlTypesNoAll;
lpDFTARRAY	lpdftNullable = (lpDFTARRAY)lpdNullable;
lpDFTARRAY	lpdftFreeStmtOpt = (lpDFTARRAY)lpdFreeStmtOpt;
lpDFTARRAY	lpdftFetchType = (lpDFTARRAY)lpdfFetchType;
lpDFTARRAY	lpdftGetInfo = (lpDFTARRAY)lpdfInfoType;
lpDFTARRAY 	lpdftInstallerErrors = (lpDFTARRAY)lpdfInstallerErrors;
lpDFTARRAY	lpdftDateTimeSubType = (lpDFTARRAY)lpdDateTimeSubType;
lpDFTARRAY	lpdftIntervalTypeSubCodes = (lpDFTARRAY)lpdIntervalTypeSubCodes;
lpDFTARRAY	lpdftDescTypes = (lpDFTARRAY)lpdfDescTypes;
lpDFTARRAY	lpdftSetDescRecFields = (lpDFTARRAY)lpdSetDescRecFields;
lpDFTARRAY	lpdftSetDescHdrFields = (lpDFTARRAY)lpdSetDescHdrFields;
lpDFTARRAY	lpdftIntervalTypes = (lpDFTARRAY)lpdIntervalTypes;
lpDFTARRAY	lpdftSQLTrueFalse30 = (lpDFTARRAY)lpdSQLTrueFalse30;
lpDFTARRAY	lpdftTxnIsol = (lpDFTARRAY)lpdTxnIsol;
UINT			ciTxnIsol = NumItems(lpdTxnIsol);
lpDFTARRAY	lpdftConcurrency = (lpDFTARRAY)lpdConcurrency;
UINT			ciConcurrency = NumItems(lpdConcurrency);
lpDFTARRAY	lpdftCursorType = (lpDFTARRAY)lpdCursorType;
UINT			ciCursorType = NumItems(lpdCursorType);
lpDFTARRAY	lpdftUseBookMarks = (lpDFTARRAY)lpdUseBookMarks;
UINT			ciUseBookMarks = NumItems(lpdUseBookMarks);
lpDFTARRAY	lpdftBindType = (lpDFTARRAY)lpdBindType;
UINT			ciBindType = NumItems(lpdBindType);
lpDFTARRAY	lpdftfSetStmtAtr = (lpDFTARRAY)lpdfSetStmtAtr;
UINT			cifSetStmtAtr = NumItems(lpdfSetStmtAtr);
lpDFTARRAY	lpdftfSetConAtr = (lpDFTARRAY)lpdfSetConAtr;
UINT			cifSetConAtr = NumItems(lpdfSetConAtr);
lpDFTARRAY	lpdftdfParamcbValues = (lpDFTARRAY)lpdfParamcbValues;
UINT			cidfParamcbValues = NumItems(lpdfParamcbValues);
lpDFTARRAY	lpdftdfParamrgbValues = (lpDFTARRAY)lpdfParamrgbValues;
UINT			cidfParamrgbValues = NumItems(lpdfParamrgbValues);
lpDFTARRAY	lpdftTypeExceptionBehavior = (lpDFTARRAY)lpdTypeExceptionBehavior;
UINT			ciTypeExceptionBehavior = NumItems(lpdTypeExceptionBehavior);


UINT	ciSQLTrueFalse30 = NumItems(lpdSQLTrueFalse30);
UINT	ciIntervalTypes = NumItems(lpdIntervalTypes);
UINT  cDescRecFields = NumItems(lpdSetDescRecFields);
UINT  cDescHdrFields = NumItems(lpdSetDescHdrFields);
UINT  ciCTypes   = NumItems(lpdfCTypes);
UINT  ciCTypesNoD   = NumItems(lpdfCTypesNoD);
UINT	ciSqlTypes = NumItems(lpdfSqlTypesNoAll);
UINT	ciNullable = NumItems(lpdNullable);
UINT	ciTrueFalse = NumItems(lpdTrueFalse);
UINT	ciSqlCodes = NumItems(lpdSqlCodes);
UINT	ciFreeStmtOpt = NumItems(lpdFreeStmtOpt);
UINT	ciFetchType = NumItems(lpdfFetchType);
UINT	ciInfoTypes = NumItems(lpdfInfoType);
UINT	ciInstallerErrors = NumItems(lpdfInstallerErrors);
UINT	ciDescTypes = NumItems(lpdfDescTypes);

extern BOOL fUnicode;

//-------------------------------------------------------------------------------
//  Connection Menu
//-------------------------------------------------------------------------------

PARAMITEM lpmSQLAllocEnv[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts			  fHandle precision scale lpParmRgb lpParmPcb
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	---------------- ------- --------- ----- --------- ---------
	TEXT("&phenv:"),	PT_PTR_HENV | PT_LOG_OUT,	0,						0,						0,		0,			NULL,		0,									NULL,				  0,		 0,			0,			NULL,			NULL,
	};

PARAMITEM lpmSQLAllocConnect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts				fHandle precision scale lpParmRgb lpParmPcb
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------	------- --------- ----- --------- ---------
	TEXT("hen&v:"),	PT_HENV | PT_FREE_PTR |
							PT_LOG_IN,						0,						0,						0,		0,			NULL,		0,									NULL,				0,	0,0,			NULL,			NULL,
	TEXT("&phdbc:"),	PT_PTR_HDBC | PT_LOG_OUT,	0,						0,						0,		0,			NULL,		0,									NULL,				0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLConnect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ConnectionHandle:"),PT_HDBC | PT_LOG_IN,			0,				0,						0,		0,			NULL,		0,									NULL,				0, 0,0,			NULL,			NULL,
	TEXT("&ServerName:"),		PT_LOG_IN,						SQL_C_TCHAR,0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,	0,	0,0,			NULL,			NULL,
	TEXT("NameLength&1:"),		PT_LOG_IN,						SQL_C_SSHORT,DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),	(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&UserName:"),			PT_LOG_IN,						SQL_C_TCHAR,0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,	0,	0,0,			NULL,			NULL,
	TEXT("NameLength&2:"),		PT_LOG_IN,						SQL_C_SSHORT,DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),	(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&Authentication:"),	PT_LOG_IN,						SQL_C_TCHAR,0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,	0,	0,0,			NULL,			NULL,
	TEXT("NameLength&3:"),		PT_LOG_IN,						SQL_C_SSHORT,DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),	(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLBrowseConnect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ConnectionHandle:"),	PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&InConnectionString:"),	PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&StringLength1:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&OutConnectionString:"),PT_PTR_STR | PT_LOG_OUT,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("String&Length2Ptr:"),	PT_PTR_SWORD |
											PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLDriverConnect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ConnectionHandle:"),	PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("&WindowHandle:"),			PT_HWND | PT_LOG_IN, 		0,						0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("&InConnectionString:"),	PT_LOG_IN,						SQL_C_TCHAR,		1,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("StringLength&1:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&OutConnectionString:"),PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("StringLength&2Ptr:"),	PT_PTR_SWORD |
											PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("&DriverCompletion:"),	PT_LOG_IN,						SQL_C_USHORT,		1,						0,		0,			NULL,		NumItems(lpdfDrvrConn),		(lpDFTARRAY)lpdfDrvrConn, 	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLDisconnect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ConnectionHandle:"),			PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLFreeConnect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hd&bc:"),PT_HDBC | PT_LOG_IN |
						 PT_FREE_PTR,				0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLFreeEnv[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hen&v:"),PT_HENV | PT_FREE_PTR |
						PT_LOG_IN,					0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetConnectOption[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hd&bc:"),			PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("&fOption:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfSetConOpt),	(lpDFTARRAY)lpdfSetConOpt,		0,0,0,			NULL,			NULL,
	TEXT("&vParam:"),			PT_LOG_IN,						SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("Parameter &Type:"),PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,		0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetConnectOption[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hd&bc:"),			PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&fOption:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfGetConOpt),	(lpDFTARRAY)lpdfGetConOpt,	  0,0,0,			NULL,			NULL,
	TEXT("&pvParam:"),		PT_PTR_PTR |
									PT_LOG_OUT,						SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Parameter &Type:"),
									PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLDataSources[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&EnvironmentHandle:"),PT_HENV | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&Direction:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfDataSrcs),		(lpDFTARRAY)lpdfDataSrcs, 0,0,0,			NULL,			NULL,
	TEXT("&ServerName:"),		PT_PTR_STR | PT_LOG_OUT,		SQL_C_TCHAR,		0,						0,		0,			NULL,	0,										NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength1:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&NameLength1Ptr:"),	PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("D&escription:"),		PT_PTR_STR | PT_LOG_OUT,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("B&ufferLength2:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("N&ameLength2Ptr:"),	PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLDrivers[] = {
// szName								uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------					--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&EnvironmentHandle:"),	PT_HENV | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&Direction:"),				PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfDataSrcs),		(lpDFTARRAY)lpdfDataSrcs,	0,0,0,			NULL,			NULL,
	TEXT("D&riverDescription:"),	PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("BufferLength&1:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Description&LengthPtr:"),PT_PTR_SWORD |
											PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Driver&Attributes:"),	PT_LOG_OUT,						SQL_C_DOUBLESTR,	0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("BufferLength&2:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("AttributesL&engthPtr:"),PT_PTR_SWORD |
											PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};


PARAMITEM lpmSQLGetInfo[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ConnectionHandle:"),PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&InfoType:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfInfoType),		(lpDFTARRAY)lpdfInfoType,	0,0,0,			NULL,			NULL,
	TEXT("Info&ValuePtr:"),		PT_PTR_PTR |
										PT_LOG_OUT,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&StringLengthPtr:"),	PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Information Value &Type:"),
										PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	0,0,0,			NULL,			NULL,
	};



//-------------------------------------------------------------------------------
// Statement menu
//-------------------------------------------------------------------------------

PARAMITEM lpmSQLAllocStmt[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hd&bc:"),			PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&phstmt:"),			PT_PTR_HSTMT | PT_LOG_OUT,	0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetStmtOption[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&fOption:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfSetStmtOpt),	(lpDFTARRAY)lpdfSetStmtOpt, 0,0,0,			NULL,			NULL,
	TEXT("&vParam:"),			PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Parameter &Type:"),PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	 0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetStmtOption[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&fOption:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfGetStmtOpt),	(lpDFTARRAY)lpdfGetStmtOpt,	0,0,0,			NULL,			NULL,
	TEXT("&pvParam:"),		PT_PTR_PTR |
									PT_LOG_OUT,						SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Parameter &Type:"),
									PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	  0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLPrepare[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Statement&Text:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Text&Length:"),	   PT_LOG_IN,						SQL_C_SLONG,		1,						0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLBindParameter[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,									0,0,0,		NULL,			NULL,
	TEXT("Parameter&Number:"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,									0,0,0,		NULL,			NULL,
	TEXT("&InputOutputtype:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfParamType),	(lpDFTARRAY)lpdfParamType,		0,0,0,		NULL,			NULL,
	TEXT("&ValueType:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypes),		(lpDFTARRAY)lpdfCTypes,			0,0,0,		NULL,			NULL,
	TEXT("P&arameterType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfSqlTypesNoAll),(lpDFTARRAY)lpdfSqlTypesNoAll,0,0,0,		NULL,			NULL,
	TEXT("&ColumnSize:"),		PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,									0,0,0,		NULL,			NULL,
	TEXT("&DecimalDigits:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,									0,0,0,		NULL,			NULL,
	TEXT("&ParameterValuePtr:"),PT_PTR_PTR |
										PT_LOG_IN,						0,						0,						0,		0,			NULL,		NumItems(lpdfParamrgbValues),(lpDFTARRAY)lpdfParamrgbValues,0,0,0,	NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,										0,0,0,	NULL,			NULL,
	TEXT("Str&Len_or_IndPtr:"),PT_PTR_SDWORD |
										PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfParamcbValues),(lpDFTARRAY)lpdfParamcbValues,0,0,0,		NULL,			NULL,
	TEXT("S&QL_LEN_DATA_AT_EXEC"),
										PT_LOG_IN | PT_CHK,			0,						0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("Rows&et hdbc:"),		PT_LOG_IN | PT_ROWSET_CTL,	SQL_C_USHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("Rowset hs&tmt:"),		PT_LOG_IN | PT_ROWSET_CTL,	SQL_C_USHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("C&olumn:"),				PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("Ro&w:"),					PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("B&uffer Size"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLNumParams[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ParameterCountPtr:"),PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLDescribeParam[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ParameterNumber:"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&DataTypePtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfSqlTypesNoAll),(lpDFTARRAY)lpdfSqlTypesNoAll, 0,0,0,			NULL,			NULL,
	TEXT("P&arameterSizePtr:"),PT_PTR_UDWORD | PT_LOG_OUT,SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("D&ecimalDigits:"),	PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&NullablePtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdNullable),		(lpDFTARRAY)lpdNullable, 0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLExecute[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};


PARAMITEM lpmSQLExecDirect[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&Statementhandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,										NULL,				0,0,0,			NULL,			NULL,
	TEXT("Statement&Text:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,										NULL,				0,0,0,			NULL,			NULL,
	TEXT("Statement&length:"),	  PT_LOG_IN,					SQL_C_SLONG,		1,						0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetCursorName[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&CursorName:"),		PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&NameLength:"),	   PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetCursorName[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&CursorName:"),		PT_PTR_STR | PT_LOG_OUT,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&NameLengthPtr:"),	PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLParamData[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ValuePtr:"),		PT_PTR_PTR |
									PT_LOG_OUT,						0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLPutData[] = {
// szName									uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------						--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&DataPtr:"),					PT_PTR_PTR |
												PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Str&Len_or_IndPtr:"),	   PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfDftParmcbValues),(lpDFTARRAY)lpdfDftParmcbValues,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLParamOptions[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("c&row:"),			PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&pirow:"),			PT_PTR_UDWORD | PT_LOG_IN,	SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLFreeStmt[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_FREE_PTR |
										PT_LOG_IN,						0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&Option:"),				PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdFreeStmtOpt),	(lpDFTARRAY)lpdFreeStmtOpt,0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLNativeSql[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ConnectionHandle:"),PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("&InStatementText:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("&TextLength1:"),		PT_LOG_IN,						SQL_C_SLONG,		1,						0,		0,			NULL,		NumItems(lpdfNTScbValues),	(lpDFTARRAY)lpdfNTScbValues,	0,0,0,			NULL,			NULL,
	TEXT("&OutStatementText:"),PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	TEXT("Text&length2Ptr:"),	PT_PTR_SDWORD |
										PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmFillParam[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&ipar:"),			PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&Row:"),				PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&rgbValue:"),		PT_PTR_PTR | PT_LOG_OUT,	0,						CB_ERR,				0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&pcbValue:"),		PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfParamcbValues),	(lpDFTARRAY)lpdfParamcbValues,0,0,0,			NULL,			NULL,
	TEXT("S&QL_LEN_DATA_AT_EXEC"),
									PT_LOG_IN | PT_CHK,			0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};


//-------------------------------------------------------------------------------
//  Results Menu
//-------------------------------------------------------------------------------

PARAMITEM lpmSQLNumResultCols[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ColumnCountPtr:"),	PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLDescribeCol[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ColumnNumber:"),		PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Column&Name:"),		PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Name&LengthPtr:"),	PT_PTR_SWORD |
										PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
 	TEXT("&DataTypePtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfSqlTypesNoAll),
 																																													(lpDFTARRAY)lpdfSqlTypesNoAll, 0,0,0,			NULL,			NULL,
	TEXT("C&olumnSizePtr:"),	PT_PTR_UDWORD | PT_LOG_OUT,SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("D&ecimalDigits:"),	PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Nullable&Ptr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdNullable),		(lpDFTARRAY)lpdNullable,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLColAttributes[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&icol:"),			PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("fDesc&Type:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfDescType),		(lpDFTARRAY)lpdfDescType,	0,0,0,			NULL,			NULL,
	TEXT("&rgbDesc:"),		PT_PTR_PTR |
									PT_LOG_OUT,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("cbD&escMax:"),	   PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&pcbDesc:"),		PT_PTR_SWORD |
									PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("p&fDesc:"),			PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,				NULL,			NULL,
	TEXT("&Description Type:"),
									PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLColAttribute[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),		PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ColumnNumber:"),			PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Field&Identifier:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfDescType30),	(lpDFTARRAY)lpdfDescType30, 0,0,0,			NULL,			NULL,
	TEXT("Character&AttributePtr:"),	PT_PTR_PTR |
											PT_LOG_OUT,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("String&LengthPtr:"),		PT_PTR_SWORD |
											PT_LOG_OUT,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&NumericAttributePtr:"),PT_PTR_SDWORD |
											PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,				NULL,			NULL,
	TEXT("&fFieldIndentifier Type:"),PT_LOG_IN,					SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLRowCount[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&RowCountPtr:"),		PT_PTR_SDWORD |
										PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLBindCol[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ColumnNumber:"),		PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&TargetType:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypes),		(lpDFTARRAY)lpdfCTypes, 0,0,0,			NULL,			NULL,
	TEXT("Target&ValuePtr:"),	PT_PTR_PTR |
										PT_LOG_DEFFERED,						0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Str&Len_or_IndPtr:"),PT_PTR_SDWORD |
										PT_LOG_DEFFERED,				SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
    TEXT("Data_at_Fetch"),      PT_PTR_UWORD,                   SQL_C_USHORT,       0,                      0,      0,          NULL,       0,                                  NULL,               0,0,0,          NULL,           NULL,
	};

PARAMITEM lpmSQLFetch[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT(" "),						PT_NONE,							0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLExtendedFetch[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&FetchOrientation:"),PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfFetchType),	(lpDFTARRAY)lpdfFetchType,	0,0,0,			NULL,			NULL,
	TEXT("Fetch&Offset:"),		PT_LOG_IN,						SQL_C_SLONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&RowCountPtr:"),		PT_PTR_UDWORD | PT_LOG_OUT,SQL_C_ULONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Row&StatusArray:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT(" "),						PT_NONE,							0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLNextColumn[] = {
	// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
	//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"), PT_HSTMT | PT_LOG_IN, 0, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, NULL, NULL,
	TEXT("&ColumnParamNumberPtr:"), PT_PTR_SWORD |
	PT_LOG_OUT, SQL_C_SHORT, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, NULL, NULL,
};

PARAMITEM lpmSQLSetPos[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,							0,0,0,			NULL,			NULL,
	TEXT("&RowNumber:"),			PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,							0,0,0,			NULL,			NULL,
	TEXT("&Operation:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfSetPos),		(lpDFTARRAY)lpdfSetPos,	0,0,0,			NULL,			NULL,
	TEXT("&LockType:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfLock),			(lpDFTARRAY)lpdfLock,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLBulkOperations[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hstmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,							NULL,							0,0,0,			NULL,			NULL,
	TEXT("Operation:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdBOType),	(lpDFTARRAY)lpdBOType,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetData[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&Statementhandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&ColumnNumber:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&TargetType:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypes),		(lpDFTARRAY)lpdfCTypes,	0,0,0,			NULL,			NULL,
  	TEXT("Target&ValuePtr:"),	PT_PTR_PTR |
										PT_LOG_OUT,						0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("Str&Len_or_IndPtr:"),PT_PTR_SDWORD |
										PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetNestedHandle[] = {
	// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
	//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"), PT_HSTMT | PT_LOG_IN, 0, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, NULL, NULL,
	TEXT("&ColumnNumber:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&phstmt:"),			PT_PTR_HSTMT | PT_LOG_OUT,	0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
};

PARAMITEM lpmSQLMoreResults[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetScrollOptions[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("fConcurrenc&y:"),	PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSSOConcur),		(lpDFTARRAY)lpdSSOConcur,	 0,0,0,			NULL,			NULL,
	TEXT("crow&Keyset:"),	PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdcrowKeyset),	(lpDFTARRAY)lpdcrowKeyset,	 0,0,0,			NULL,			NULL,
	TEXT("crow&Rowset:"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};


//-------------------------------------------------------------------------------
//  Catalog Menu
//-------------------------------------------------------------------------------

PARAMITEM lpmSQLGetTypeInfo[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&DataType:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfSqlTypes),		(lpDFTARRAY)lpdfSqlTypes,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLTables[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Sche&maName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("T&ableType:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&4:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};


PARAMITEM lpmSQLColumns[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Schema&Name:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("C&olumnName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&4:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLStatistics[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Sche&maName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&Unique:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfUnique),		(lpDFTARRAY)lpdfUnique,	  0,0,0,			NULL,			NULL,
	TEXT("&Reserved:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfAccuracy),		(lpDFTARRAY)lpdfAccuracy, 0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLPrimaryKeys[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&SchemaName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLForeignKeys[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,   0,   	NULL,			NULL,
	TEXT("Pk&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&1:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("PkSche&maName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&2:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Pk&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&3:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("FkC&atalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&4:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("FkSche&maName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&5:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("FkTa&bleName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&6:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLTablePrivileges[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,   0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Sche&maName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLColumnPrivileges[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,	0,						0,						0,		0,			NULL,		0,									NULL,				0,0,   0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Schema&Name:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("C&olumnName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&4:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSpecialColumns[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
   TEXT("&IdentifierType:"),PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfColType),		(lpDFTARRAY)lpdfColType,	0,0,0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Sche&maName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&TableName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT("NameLength&3:"),  PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("Sc&ope:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfScope),			(lpDFTARRAY)lpdfScope,		0,0,0,			NULL,			NULL,
   TEXT("&Nullable:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfNullable),		(lpDFTARRAY)lpdfNullable,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLProcedures[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,   0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Schema&Name:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&ProcName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("NameLength&3:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLProcedureColumns[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,    0,			NULL,			NULL,
	TEXT("&CatalogName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("NameLength&1:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("Schema&Name:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("NameLength&2:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("&ProcName:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("NameLength&3:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("C&olumnName:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("NameLength&4:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

//-------------------------------------------------------------------------------
//  Catalog Menu
//-------------------------------------------------------------------------------

//Pre ODBC 3.0 Catalog functions
PARAMITEM lpmSQLTables25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,    0,			NULL,			NULL,
	TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Type:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0, 	0, 	0,			NULL,			NULL,
	TEXT("...T&ype:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLColumns25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hs&tmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,  0,			NULL,			NULL,
	TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...Na&me:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,	0,	0,			NULL,			NULL,
	TEXT("...Nam&e:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLStatistics25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,								0,	0,0,			NULL,			NULL,
	TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,	0,0,			NULL,			NULL,
	TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,	0,0,			NULL,			NULL,
	TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,	0,0,			NULL,			NULL,
	TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	TEXT("f&Unique:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfUnique),		(lpDFTARRAY)lpdfUnique,		0,	0,0,			NULL,			NULL,
	TEXT("&fAccuracy:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfAccuracy),		(lpDFTARRAY)lpdfAccuracy,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLPrimaryKeys25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLForeignKeys25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hs&tmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Qua&lifier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Own&er:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Na&me:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...Q&ualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Quali&fier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...O&wner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Owne&r:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
   TEXT("...N&ame:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...Name&:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLTablePrivileges25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				 0,0,0,			NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	 0,0,0, 			NULL,			NULL,
   TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,0,0,    		NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	 0,0,0,			NULL,			NULL,
   TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,0,0,    		NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				 0,0,0, 			NULL,			NULL,
   TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLColumnPrivileges25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hs&tmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				 0,  0,  0, 			NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,  0,  0,  			NULL,			NULL,
   TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,  0,  0,				NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,  0,  0,  			NULL,			NULL,
   TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,  0,  0,    		NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,  0,  0,  			NULL,			NULL,
   TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,  0,  0,    		NULL,			NULL,
   TEXT("...Na&me:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,  0,  0,  			NULL,			NULL,
   TEXT("...Nam&e:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,  0,  0,    		NULL,			NULL,
	};

PARAMITEM lpmSQLSpecialColumns25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,								0,0,0, 			NULL,			NULL,
   TEXT("&fColType:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfColType),		(lpDFTARRAY)lpdfColType, 	0,0,0,   		NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,     		NULL,			NULL,
   TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,							 	0,0,0,  			NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,     		NULL,			NULL,
   TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,								0,0,0,  			NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								0,0,0,  			NULL,			NULL,
   TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,							 	0,0,0,			NULL,			NULL,
   TEXT("f&Scope:"),			PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfScope),			(lpDFTARRAY)lpdfScope,		0,0,0,  			NULL,			NULL,
   TEXT("fN&ullable:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdfNullable),		(lpDFTARRAY)lpdfNullable,	0,0,0,  			NULL,			NULL,
	};

PARAMITEM lpmSQLProcedures25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				 0,0,0,			NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	 0,0,0, 			NULL,			NULL,
   TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,0,0,    		NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			    0,0,0, 			NULL,			NULL,
   TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,0,0,    		NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	 0,0,0, 			NULL,			NULL,
   TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,				 0,0,0,    		NULL,			NULL,
	};

PARAMITEM lpmSQLProcedureColumns25[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hs&tmt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("...&Qualifier:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,0,0, 			NULL,			NULL,
   TEXT("...Qualif&ier:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,    			NULL,			NULL,
   TEXT("...&Owner:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,0,0, 			NULL,			NULL,
   TEXT("...O&wner:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,    			NULL,			NULL,
   TEXT("...&Name:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			 	0,0,0, 			NULL,			NULL,
   TEXT("...N&ame:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,    			NULL,			NULL,
   TEXT("...Na&me:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,				0,0,0, 			NULL,			NULL,
   TEXT("...Nam&e:"),	   PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		NumItems(lpdfNTScbValues),		(lpDFTARRAY)lpdfNTScbValues,	0,	0,0,    			NULL,			NULL,
	};


//-------------------------------------------------------------------------------
//  Misc Menu
//-------------------------------------------------------------------------------

PARAMITEM lpmSQLTransact[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hen&v:"),			PT_HENV | PT_FREE_PTR |
									PT_LOG_IN,					0,						0,						0,		0,			NULL,		0,									NULL,										 0, 0,0,			NULL,			NULL,
   TEXT("hd&bc:"),			PT_HDBC | PT_FREE_PTR |
									PT_LOG_IN,					0,						0,						0,		0,			NULL,		0,									NULL,										 0, 0,0,   			NULL,			NULL,
   TEXT("&fType:"),			PT_LOG_IN,						SQL_C_USHORT,	0,						0,		0,			NULL,		NumItems(lpdTransactType),	(lpDFTARRAY)lpdTransactType,	 	 0, 0,0,			NULL,			NULL,
	};


PARAMITEM lpmSQLAllocHandle[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("Handle&Type:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLAllocHandle),	(lpDFTARRAY)lpdSQLAllocHandle, 0,0,0,			NULL,			NULL,
   TEXT("&InputHandle:"),	PT_HHANDLE |
									PT_FREE_PTR |
									PT_LOG_IN,					   0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
   TEXT("&OutputHandlePtr:"),	PT_HHANDLE |
									PT_HOUTPUT |
									PT_FREE_PTR |
									PT_LOG_OUT,					   0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};


PARAMITEM lpmSQLFreeHandle[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
	TEXT("&HandleType:"),PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLAllocHandle),	(lpDFTARRAY)lpdSQLAllocHandle,  0,0,0,			NULL,			NULL,
	TEXT("H&andle:"),	PT_HHANDLE |
								PT_FREE_PTR |
								PT_LOG_IN,					   0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLBindParam[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,

   TEXT("Parameter&Number:"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("&ValueType:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypes),		(lpDFTARRAY)lpdfCTypes,	0,0, 0, 			NULL,			NULL,
   TEXT("P&arameterType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfSqlTypesNoAll),(lpDFTARRAY)lpdfSqlTypesNoAll,0,0, 0, 	  			NULL,			NULL,
   TEXT("LenghtPrecision:"),	PT_LOG_IN,						SQL_C_SLONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("&ParameterScale:"),	PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
	TEXT("Pa&rameterValue:"),	PT_PTR_PTR |
										PT_LOG_OUT,						0,						0,						0,		0,			NULL,		NumItems(lpdfParamrgbValues),(lpDFTARRAY)lpdfParamrgbValues,0,0,0,	NULL,			NULL,
	TEXT("Str&Len_or_IndPtr:"),PT_PTR_SDWORD |
										PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfParamcbValues),(lpDFTARRAY)lpdfParamcbValues,0,0,0,		NULL,			NULL,
   TEXT("S&QL_LEN_DATA_AT_EXEC"),
										PT_LOG_IN | PT_CHK,			0,						0,						0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("Rows&et hdbc:"),		PT_LOG_IN | PT_ROWSET_CTL,	SQL_C_USHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("Rowset hs&tmt:"),		PT_LOG_IN | PT_ROWSET_CTL,	SQL_C_USHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("C&olumn:"),				PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("Ro&w:"),					PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
   TEXT("B&uffer Size"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,							0,0, 0, 			NULL,			NULL,
	};

PARAMITEM lpmSQLCloseCursor[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),	PT_HSTMT | PT_FREE_PTR |
										PT_LOG_IN,						0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLEndTran[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&HandleType:"),	PT_LOG_IN,					SQL_C_SSHORT,	0,						0,		0,			NULL,		NumItems(lpdSQLEndTran),	(lpDFTARRAY)lpdSQLEndTran,	 0,0,0,			NULL,			NULL,
   TEXT("H&andle:"),			PT_HHANDLE |
									PT_FREE_PTR |
									PT_LOG_IN,					0,					0,						0,		0,			NULL,		0,									NULL,									0,0,0,			NULL,			NULL,
   TEXT("Completion&Type:"),PT_LOG_IN,					SQL_C_SSHORT,	0,						0,		0,			NULL,		NumItems(lpdTransactType),	(lpDFTARRAY)lpdTransactType,	0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLFetchScroll[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
   TEXT("&FetchOrientation:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfFetchType),	(lpDFTARRAY)lpdfFetchType,	0,0,0,			NULL,			NULL,
   TEXT("Fetch&Offset:"),		PT_LOG_IN,						SQL_C_SLONG,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,								0,0,0,			NULL,			NULL,
	TEXT(" "),						PT_NONE,							0,						0,						0,		0,			NULL,		0,									NULL,				0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetEnvAttr[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&EnvironmentHandle:"),	PT_HENV | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,									  0,0,0,				NULL,			NULL,
   TEXT("&Attribute:"),				PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfSetEnvAttr),	(lpDFTARRAY)lpdfSetEnvAttr,	  0,0,0,				NULL,			NULL,
   TEXT("&ValuePtr:"),				PT_PTR_PTR |
											PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									  0,0,0,				NULL,			NULL,
   TEXT("&BufferLength:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,0,	0,0,			NULL,			NULL,
   TEXT("&StringLengthPtr:"),		PT_PTR_SDWORD |
											PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									  0,0,0,		 		NULL,			NULL,
   TEXT("Attribute &Type:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,		  0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLSetEnvAttr[] = {
// szName							uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------				--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&EnvironmentHandle:"),				PT_HENV | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&Attribute:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfSetEnvAttr),	(lpDFTARRAY)lpdfSetEnvAttr,	0,	0,0,			NULL,			NULL,
   TEXT("&ValuePtr:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&StringLengthPtr:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,0,	0,0,			NULL,			NULL,
   TEXT("fAttribute &Type:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,		0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetConnectAttr[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&ConnectionHandle:"),	PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,								0,	0,	0,				NULL,			NULL,
   TEXT("&Attribute:"),				PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfGetConAtr),	(lpDFTARRAY)lpdfGetConAtr,	0,	0,	0,				NULL,			NULL,
   TEXT("&ValuePtr:"),				PT_PTR_PTR |
											PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,								0,	0,	0,				NULL,			NULL,
   TEXT("&BufferLength:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),	(lpDFTARRAY)lpdfPtrNTScbValues,0,	0,0,			NULL,			NULL,
   TEXT("&StringLengthPtr:"),		PT_PTR_SDWORD |
											PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,								0,	0,	0,				NULL,			NULL,
   TEXT("fAttribute &Type:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	0,	0,	0,				NULL,			NULL,
	};

PARAMITEM lpmSQLSetConnectAttr[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&ConnectionHandle:"),	PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,								  	0,	0,0,			NULL,			NULL,
   TEXT("&Attribute:"),				PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfSetConAtr),	(lpDFTARRAY)lpdfSetConAtr,	  	0,	0,0,			NULL,			NULL,
	TEXT("&ValuePtr:"),				PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								  	0,	0,0,			NULL,			NULL,
   TEXT("&StringLength:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,0,0,0,		NULL,			NULL,
   TEXT("fAttribute &Type:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	  	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetStmtAttr[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),		PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,								  0,	0,0,			NULL,			NULL,
   TEXT("&Attribute:"),	PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfGetStmtAtr),	(lpDFTARRAY)lpdfGetStmtAtr,  0,	0,0,			NULL,			NULL,
   TEXT("&ValuePtr:"),		PT_PTR_PTR |
									PT_LOG_OUT,						0,						0,						0,		0,			NULL,		0,									NULL,								  0,	0,0,			NULL,			NULL,
   TEXT("&BufferLength:"),	   PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),	(lpDFTARRAY)lpdfPtrNTScbValues,0,0,0,			NULL,			NULL,
   TEXT("String&LengthPtr:"),		PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,								  0,	0,0,			NULL,			NULL,
   TEXT("fAttribute &Type:"),
									PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,	  0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetStmtAttr[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),	PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,										0,	0,0,			NULL,			NULL,
   TEXT("&Attribute:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfSetStmtAtr),	(lpDFTARRAY)lpdfSetStmtAtr,		0,	0,0,			NULL,			NULL,
   TEXT("&ValuePtr:"),			PT_LOG_IN,						0,						0,						0,		0,			NULL,		0,									NULL,										0,	0,0,			NULL,			NULL,
	TEXT("&StringLength:"),	   PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),	(lpDFTARRAY)lpdfPtrNTScbValues,0,0,0,			NULL,			NULL,
   TEXT("fAttribute &Type:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,			0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLCopyDesc[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&SourceDescHandle:"),PT_MULT_HDESC |
										PT_LOG_IN,						SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		 	0,	0,0,			NULL,			NULL,
   TEXT("&TargetDescHandle:"),PT_MULT_HDESC |
										PT_LOG_IN,						SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		 	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetDescField[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&Descriptorhandle:"),PT_HDESC |
										PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,											0,	0,0,			NULL,			NULL,
   TEXT("&RecNumber:"),			PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,											0,	0,0,			NULL,			NULL,
   TEXT("&FieldIdentifier:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdGetDescFields),	(lpDFTARRAY)lpdGetDescFields,		0,	0,0,			NULL,			NULL,
   TEXT("&ValuePtr:"),			PT_PTR_PTR |
										PT_LOG_OUT,						0,						0,						0,		0,			NULL,		NumItems(lpdNullValidPointer),(lpDFTARRAY)lpdNullValidPointer,	0,	0,0,			NULL,			NULL,
   TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,											0,	0,0,			NULL,			NULL,
   TEXT("&StringLengthPtr:"),	PT_PTR_SDWORD |
										PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,	0,0,0,			NULL,			NULL,
   TEXT("Parameter &Type:"),
										PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,				0,	0,0,			NULL,			NULL,
};

PARAMITEM lpmSQLSetDescField[] = {
// szName							uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------				--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&DescriptorHandle:"),PT_HDESC |
										PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,										0,	0,0,			NULL,			NULL,
	TEXT("&RecNumber:"),			PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,										0,	0,0,			NULL,			NULL,
   TEXT("&FieldIdentifier:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdSetDescFields),	(lpDFTARRAY)lpdSetDescFields,	0,	0,0,			NULL,			NULL,
   TEXT("&ValuePtr:"),			PT_LOG_IN,						0,						0,						0,		0,			NULL,		NumItems(lpdNullValidPointer),(lpDFTARRAY)lpdNullValidPointer,0,0,0,			NULL,			NULL,
   TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,0,0,0,			NULL,			NULL,
   TEXT("Parameter &Type:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,			0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetDescRec[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&DescriptorHandle:"),PT_HDESC |
										PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
	TEXT("&RecNumber:"),			PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&Name:"),				PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&BufferLength:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&StringLengthPtr:"),	PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&TyprPtr:"),			PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("S&ubTypePtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&LengthPtr:"),			PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("&PrecisionPtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("S&calePtr:"),			PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
   TEXT("Null&ablePtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetDescRec[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts										lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&DescriptorHandle:"),PT_HDESC |
									PT_LOG_IN,						SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,														0,0,0,			NULL,			NULL,
   TEXT("&RecNumber:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,														0,0,0,			NULL,			NULL,
   TEXT("&Type:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfDescTypes),	(lpDFTARRAY)lpdfDescTypes,							0,0,0,			NULL,			NULL,
   TEXT("&SubType:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdDateTimeIntervalCode),	(lpDFTARRAY)lpdDateTimeIntervalCode,0,0,0,			NULL,			NULL,
   TEXT("&Length:"),			PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,														0,0,0,			NULL,			NULL,
	TEXT("&Precision:"),		PT_LOG_IN,						SQL_C_SLONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,														0,0,0,			NULL,			NULL,
	TEXT("S&cale:"),			PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,														0,0,0,    		NULL,			NULL,
   TEXT("D&ataPtr:"),		PT_PTR_PTR |
									PT_LOG_DEFFERED,				0,						0,						0,		0,			NULL,		NumItems(lpdNullValidPointer),(lpDFTARRAY)lpdNullValidPointer,				0,	0,0,			NULL,			NULL,
	TEXT("StringL&engthPtr:"),PT_PTR_SDWORD |
									PT_LOG_DEFFERED,				SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfSetDescReccbValues),(lpDFTARRAY)lpdfSetDescReccbValues,		0,0,0,			NULL,			NULL,
   TEXT("&IndicatorPtr:"),	PT_PTR_SDWORD |
									PT_LOG_DEFFERED,				SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdfIndicatorPtr),(lpDFTARRAY)lpdfIndicatorPtr,						0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetDiagField[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("Handle&Type:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLAllocHandle),	(lpDFTARRAY)lpdSQLAllocHandle,  	0,	0,	0,				NULL,			NULL,
   TEXT("H&andle:"),					PT_HHANDLE |
											PT_FREE_PTR |
											PT_LOG_IN,					   0,						0,						0,		0,			NULL,		0,									NULL,										  	0,	0,	0,			NULL,			NULL,
   TEXT("&RecNumber:"),				PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,										  	0,	0,	0,			NULL,			NULL,
   TEXT("&DiagIdentifier:"),		PT_LOG_IN,						SQL_C_SLONG,		0,						0,		0,			NULL,		NumItems(lpdDiagId),			(lpDFTARRAY)lpdDiagId,				  	0,	0,	0,			NULL,			NULL,
   TEXT("D&iagInfoPtr:"),			PT_PTR_PTR |
											PT_LOG_OUT,						0,						0,						0,		0,			NULL,		0,									NULL,										  	0,	0,	0,			NULL,			NULL,
   TEXT("&BufferLength:"),			PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,	0,0,0, 			NULL,			NULL,
   TEXT("&StringLengthPtr:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,										  	0,	0,	0,			NULL,			NULL,
	TEXT("&fDiagIdentifier Type:"),PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfCTypesNoD),	(lpDFTARRAY)lpdfCTypesNoD,			  	0,	0,	0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetDiagRec[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("Handle&Type:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLAllocHandle),	(lpDFTARRAY)lpdSQLAllocHandle,  0,	0,0,			NULL,			NULL,
   TEXT("H&andle:"),			PT_HHANDLE |
									PT_FREE_PTR |
									PT_LOG_IN,					   0,						0,						0,		0,			NULL,		0,									NULL,										  0,	0,0,			NULL,			NULL,
   TEXT("&RecNumber:"),		PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,										  0,	0,0,			NULL,			NULL,
   TEXT("&SqlState:"),		PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,										  0,	0,0,			NULL,			NULL,
   TEXT("&NativeErrorPtr:"),PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,										  0,	0,0,			NULL,			NULL,
	TEXT("&MessageText:"),	PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,										  0,	0,0,			NULL,			NULL,
   TEXT("&BufferLength:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdfPtrNTScbValues),(lpDFTARRAY)lpdfPtrNTScbValues,	0,0,0,			NULL,			NULL,
   TEXT("Text&LengthPtr:"),PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,										  0,	0,0,			NULL,			NULL,
	};

#ifdef SQL_API_SQLLOCATOR

PARAMITEM lpmSQLLocator[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,									 	0,	0,0,			NULL,			NULL,
   TEXT("h&Locator:"),		PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,									 	0,	0,0,			NULL,			NULL,
   TEXT("&fOperation:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfOperation),(lpDFTARRAY)lpdLocfOperation,	 	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetLength[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,										 	0,	0,0,			NULL,			NULL,
   TEXT("&fLocatorType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfLocatorType),(lpDFTARRAY)lpdLocfLocatorType, 	0,	0,0,			NULL,			NULL,
   TEXT("h&Locator:"),		PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,										 	0,	0,0,			NULL,			NULL,
	TEXT("&pcbValue:"),		PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,										 	0,	0,0,			NULL,			NULL,
	TEXT("p&fIndicator:"),	PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,										 	0,	0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetPosition[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
   TEXT("&fLocatorType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfLocatorType),(lpDFTARRAY)lpdLocfLocatorType,	0,0,	0,				NULL,			NULL,
   TEXT("h&SourceLocator:"),PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
   TEXT("h&TargetLocator:"),PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
   TEXT("&szSearchString:"),PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
   TEXT("c&bSearchString:"),PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
   TEXT("&iFromPosition"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,											0,0,	0,
	TEXT("pib&LocatedAt:"),	PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
	TEXT("&pfIndicator:"),	PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,											0,0,	0,				NULL,			NULL,
	};

PARAMITEM lpmSQLGetSubString[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
   TEXT("&fLocatorType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfLocatorType),(lpDFTARRAY)lpdLocfLocatorType,	0,	0,0,				NULL,			NULL,
   TEXT("h&SourceLocator:"),PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
   TEXT("&iFromPosition"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
   TEXT("cbF&orLength:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
   TEXT("f&TargetType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfLocatorType),(lpDFTARRAY)lpdLocfLocatorType,	0,	0,0,				NULL,			NULL,
	TEXT("&rgbValue:"),		PT_PTR_PTR |
									PT_LOG_OUT,						0,						0,						0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
   TEXT("c&bValueMax:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
	TEXT("pcb&Value:"),		PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
	TEXT("&pfIndicator:"),	PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,											0,	0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLLocatedUpdate[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
   TEXT("f&LocatorType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfLocatorType),(lpDFTARRAY)lpdLocfLocatorType,  	0,	0,0,				NULL,			NULL,
   TEXT("h&TargetLocator:"),PT_LOG_IN,						SQL_C_ULONG,		DEX_DFT_ZERO,		0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
   TEXT("&iFromPosition"),	PT_LOG_IN,						SQL_C_USHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
   TEXT("c&bForLength:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
   TEXT("f&SourceType:"),	PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		NumItems(lpdLocfSrcTargType),(lpDFTARRAY)lpdLocfSrcTargType,  	0,	0,0,				NULL,			NULL,
	TEXT("&rgbValue:"),		PT_PTR_PTR |
									PT_LOG_OUT,						0,						0,						0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
   TEXT("c&bValue:"),		PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
	TEXT("&pfIndicator:"),	PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,										  	0,	0,0,				NULL,			NULL,
	};

#endif

//@INSTALLER

PARAMITEM lpmSQLRemoveDriver[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Driver:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,								 	 0,0,0,				NULL,			NULL,
   TEXT("&fRequest:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLTrueFalse),	(lpDFTARRAY)lpdSQLTrueFalse,	 0,0,0,				NULL,			NULL,
   TEXT("dw&UsageCount:"),	PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,									 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLRemoveDriverManager[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&dwUsageCount:"),	PT_PTR_SDWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,						0,0,0,				NULL,			NULL,
	};


PARAMITEM lpmSQLInstallTranslator[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&InfFile:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,							0,	0,0,				NULL,			NULL,
   TEXT("sz&Translator:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,							0,	0,0,				NULL,			NULL,
   TEXT("s&zPathIn:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,							0,	0,0,				NULL,			NULL,
   TEXT("szP&athOut:"),		PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,														 	 0,0,0,				NULL,			NULL,
   TEXT("cbPath&OutMax:"), PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,															 0,0,0,				NULL,			NULL,
   TEXT("&pcbPathOut:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,															 0,0,0,				NULL,			NULL,
   TEXT("&fRequest:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLInstallTranslator),	(lpDFTARRAY)lpdSQLInstallTranslator,	 0,0,0,				NULL,			NULL,
   TEXT("&dwUsageCount:"),	PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,															 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLInstallTranslatorEx[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Translator:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,							0,	0,0,				NULL,			NULL,
   TEXT("s&zPathIn:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		NumItems(lpdfStringValues),(lpDFTARRAY)lpdfStringValues,							0,	0,0,				NULL,			NULL,
   TEXT("szP&athOut:"),		PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,														 	 0,0,0,				NULL,			NULL,
   TEXT("cbPath&OutMax:"), PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,															 0,0,0,				NULL,			NULL,
   TEXT("&pcbPathOut:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,															 0,0,0,				NULL,			NULL,
   TEXT("&fRequest:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLInstallTranslator),	(lpDFTARRAY)lpdSQLInstallTranslator,	 0,0,0,				NULL,			NULL,
   TEXT("&dwUsageCount:"),	PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,															 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLInstallerError[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&iError:"),			PT_LOG_IN,						SQL_C_SSHORT,		DEX_DFT_ONE,		0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
	TEXT("&pfErrorCode:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
   TEXT("s&zErrorMsg:"),	PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
   TEXT("cbErrorMsgMax:"), PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
   TEXT("&pcbErrorMsg:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLRemoveTranslator[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Translator:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
   TEXT("&dwUsageCount:"),	PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,			0,0,0,				NULL,			NULL,
	};


PARAMITEM lpmSQLConfigDriver[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("h&wndParent:"),	PT_HWND |PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,											 	0,0,0,					NULL,			NULL,
   TEXT("&fRequest:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLConfigDriver),	(lpDFTARRAY)lpdSQLConfigDriver,	 	0,0,0,					NULL,			NULL,
   TEXT("sz&Driver:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,										 		0,0,0,					NULL,			NULL,
   TEXT("sz&Args:"),			PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,										 		0,0,0,					NULL,			NULL,
   TEXT("sz&Msg:"),			PT_PTR_STR | PT_LOG_OUT,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,										 		0,0,0,					NULL,			NULL,
   TEXT("c&bMsgMax:"), 		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,											 	0,0,0,					NULL,			NULL,
   TEXT("&pcbMsgMax:"),		PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,											 	0,0,0,					NULL,			NULL,
	};

PARAMITEM lpmSQLConfigDataSource[] = {
// szName							uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------				--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("h&wndParent:"),		PT_HWND |PT_LOG_IN,	0,						0,						0,		0,			NULL,		0,									NULL,														0,0,0,				NULL,			NULL,
   TEXT("&fRequest:"),			PT_LOG_IN,				SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLConfigDataSource),	(lpDFTARRAY)lpdSQLConfigDataSource,	0,0,0,				NULL,			NULL,
   TEXT("lpsz&Driver:"),		PT_LOG_IN,				SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,													 	0,0,0,				NULL,			NULL,
   TEXT("lpsz&Attributes:"),	PT_LOG_IN,				SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,													 	0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLCreateDataSource[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("h&wndParent:"),	PT_HWND |PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,			0,	  0,	0,					NULL,			NULL,
   TEXT("sz&DS:"),			PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,	 		0,	  0, 	0, 				NULL,			NULL,
	};

PARAMITEM lpmSQLGetInstalledDrivers[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Buf:"),			PT_LOG_OUT,						SQL_C_DOUBLESTR,	0,						0,		0,			NULL,		0,									NULL,			0,	  0,	0,					NULL,			NULL,
   TEXT("cbB&ufMax:"), 		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,			0,	  0,	0,					NULL,			NULL,
   TEXT("&pcbBufOut:"),		PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,			0,	  0,	0,					NULL,			NULL,
	};

PARAMITEM lpmSQLGetPrivateProfileString[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Section:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  		0,	0,0,				NULL,			NULL,
   TEXT("sz&Key:"),			PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  		0,	0,0,				NULL,			NULL,
   TEXT("sz&Default:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  		0,	0,0,				NULL,			NULL,
   TEXT("sz&ReturnBuffer:"),PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  		0,	0,0,				NULL,			NULL,
   TEXT("cchR&eturnBuffer:"),PT_LOG_IN,					SQL_C_SLONG,			0,						0,		0,			NULL,		0,									NULL,		  	0,	0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLInstallDriver[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&InfFile:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 		0,	0,0,				NULL,			NULL,
   TEXT("sz&Driver:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 		0,	0,0,				NULL,			NULL,
   TEXT("s&zPath:"),			PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 		0,	0,0,				NULL,			NULL,
   TEXT("c&bPathMax:"), 	PT_LOG_IN,						SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,		 	0,	0,0,				NULL,			NULL,
   TEXT("&pcbPathOut:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,			0,	0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLInstallDriverManager[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("s&zPath:"),			PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		 	0,	0,0,				NULL,			NULL,
   TEXT("c&bPathMax:"), 	PT_LOG_IN,						SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,			0,	0,0,				NULL,			NULL,
   TEXT("&pcbPathOut:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,			0,	0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLManageDataSources[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("h&wnd:"),	PT_HWND |PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,			 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLRemoveDefaultDataSource[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT(" "),			0,									0,						0,						0,		0,			NULL,		0,									NULL,		0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLRemoveDSNFromIni[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&DSN:"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLValidDSN[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&DSN:"),			PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLWriteDSNToIni[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&DSN:"),			PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 0,0,0,				NULL,			NULL,
   TEXT("szD&river:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLWritePrivateProfileString[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Section:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 0,0,0,				NULL,			NULL,
   TEXT("sz&Key:"),			PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 0,0,0,				NULL,			NULL,
   TEXT("szS&tring:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	 0,0,0,				NULL,			NULL,
	};

PARAMITEM lpmSQLGetTranslator[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("h&wndParent:"),	PT_HWND |PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
   TEXT("lpsz&Name:"),		PT_PTR_STR |PT_LOG_OUT,		SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
   TEXT("cbNa&meMax:"), 	PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
   TEXT("&pcbNameOut:"),	PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
   TEXT("lpszPa&th:"),		PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,	 	 0,0,0,						NULL,			NULL,
   TEXT("c&bPathMax:"), 	PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
   TEXT("pcbP&athOut:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
   TEXT("p&vOption:"),		PT_PTR_UDWORD | PT_LOG_OUT,SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,		 0,0,0,						NULL,			NULL,
	};

PARAMITEM lpmSQLCancel[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&StatementHandle:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,			0,		0,		0,				NULL,			NULL,
	};

PARAMITEM lpmSQLError[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("hen&v:"),			PT_HENV | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,			 0,	 0,	 0,				NULL,			NULL,
   TEXT("hd&bc:"),			PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,			 0,	 0,	 0,				NULL,			NULL,
   TEXT("hst&mt:"),			PT_HSTMT | PT_LOG_IN,		0,						0,						0,		0,			NULL,		0,									NULL,			 0,	 0,	 0,				NULL,			NULL,
   TEXT("&szSqlState:"),	PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,		 	0,		0,		0,					NULL,			NULL,
   TEXT("p&fNativeError:"),PT_PTR_SDWORD |
									PT_LOG_OUT,						SQL_C_SLONG,		0,						0,		0,			NULL,		0,									NULL,			 0,	 0,	 0,				NULL,			NULL,
   TEXT("s&zErrorMsg:"),	PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,		0,						0,		0,			NULL,		0,									NULL,		 	0,		0,		0,					NULL,			NULL,
   TEXT("cbErrorMsgMa&x:"), PT_LOG_IN,						SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,			 0,	 0,	 0,				NULL,			NULL,
   TEXT("&pcbErrorMsg:"),	PT_PTR_SWORD | PT_LOG_OUT,	SQL_C_SSHORT,		0,						0,		0,			NULL,		0,									NULL,			 0,	 0,	 0,				NULL,			NULL,
	};

PARAMITEM lpmSQLGetFunctions[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&ConnectionHandle:"),PT_HDBC | PT_LOG_IN,			0,						0,						0,		0,			NULL,		0,									NULL,									  0,0,0,					NULL,			NULL,
   TEXT("&FunctionId:"),		PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdGetFunct),		(lpDFTARRAY)lpdGetFunct,		  0,0,0,					NULL,			NULL,
   TEXT("&SupportedPtr:"),		PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdSQLTrueFalse30),(lpDFTARRAY)lpdSQLTrueFalse30, 0,0,0,					NULL,			NULL,
	};

PARAMITEM lpmSQLInstallDriverEx[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("sz&Driver:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,												 0,0,0,			NULL,			NULL,
   TEXT("sz&PathIn:"),		PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,												 0,0,0,					NULL,			NULL,
   TEXT("s&zPathOut:"),		PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,												 0,0,0,					NULL,			NULL,
   TEXT("c&bPathOutMax:"),	PT_LOG_IN,						SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,												 0,0,0,					NULL,			NULL,
   TEXT("&pcbPathOut:"),	PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,												 0,0,0,					NULL,			NULL,
   TEXT("&fRequest:"),		PT_LOG_IN,						SQL_C_USHORT,			0,						0,		0,			NULL,		NumItems(lpdSQLInstallDriverEx),	(lpDFTARRAY)lpdSQLInstallDriverEx,0,0,0,					NULL,			NULL,
   TEXT("dw&UsageCount:"),	PT_PTR_SDWORD | PT_LOG_OUT,SQL_C_SSHORT,			0,						0,		0,			NULL,		0,									NULL,												 0,0,0,					NULL,			NULL,
	};

PARAMITEM lpmSQLReadFileDSN[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("lpsz&FileName"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		0,		0,		0,			NULL,			NULL,
   TEXT("lpsz&AppName"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		0,		0,		0,			NULL,			NULL,
   TEXT("lpsz&KeyName"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		0,		0,		0,			NULL,			NULL,
   TEXT("lps&zString:"),	PT_PTR_STR | PT_LOG_OUT,	SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,		0,		0,		0,			NULL,			NULL,
   TEXT("c&bString:"),		PT_LOG_IN,						SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,		0,		0,		0,			NULL,			NULL,
   TEXT("pcbString:"),		PT_PTR_UWORD | PT_LOG_OUT,	SQL_C_USHORT,			0,						0,		0,			NULL,		0,									NULL,		0,		0,		0,			NULL,			NULL,
	};

PARAMITEM lpmSQLWriteFileDSN[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------		-     -
   TEXT("lpsz&FileName"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  0,	  0,    0,			NULL,			NULL,
   TEXT("lpsz&AppName"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  0,	  0,    0,  		NULL,			NULL,
   TEXT("lpsz&KeyName"),	PT_PTR_STR | PT_LOG_IN,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  0,	  0,    0,  		NULL,			NULL,
   TEXT("lpsz&String"),		PT_PTR_STR | PT_LOG_OUT,		SQL_C_TCHAR,			0,						0,		0,			NULL,		0,									NULL,	  0,	  0,    0,  		NULL,			NULL,
	};

PARAMITEM lpmSQLPostInstallerError[] = {
// szName				uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------	--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&fErrorCode:"),		PT_LOG_IN,						SQL_C_SLONG,	0,						0,		0,			NULL,		NumItems(lpdfInstallerErrors),	(lpDFTARRAY)lpdfInstallerErrors, 0,0,0,			NULL,			NULL,
   TEXT("sz&ErrorMsg:"),		PT_PTR_STR |PT_LOG_IN,		SQL_C_TCHAR,	0,						0,		0,			NULL,		0,									NULL,												0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLGetConfigMode[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&pwConfigMode:"),	PT_PTR_UWORD |
									PT_LOG_OUT,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdConfigModes),	(lpDFTARRAY)lpdConfigModes, 0,0,0,			NULL,			NULL,
	};

PARAMITEM lpmSQLSetConfigMode[] = {
// szName						uParmType						fCType         	dwDex					fNull	cBytes	lpData 	cDfts								lpDfts
//	-----------------			--------------------------	-----------------	-----------------	-----	--------	--------	--------------------------	----------------
   TEXT("&wConfigMode:"),	PT_LOG_IN,						SQL_C_USHORT,		0,						0,		0,			NULL,		NumItems(lpdConfigModes),		(lpDFTARRAY)lpdConfigModes,		  0,0,0,			NULL,			NULL,
	};

// This structure is used as a hashed array of pointers to parameter data.
//	The Initlpm macro simply pastes the function name to fill out the
// fields of the structure.
#define Initlpm(func)	TEXT(#func), lpm ## func, NumItems(lpm ## func), lp ## func
struct {
	LPTSTR				szName;				// Function name
	lpPARAMITEM			lpParms;				// Pointer to parameter structure
	UINT					cParms;				// Count of parameters
	lpFUNCHNDLR			lpFuncHndlr;		// Function handler
	} ApiParms[] = {
/*  0*/Initlpm(SQLAllocConnect),
/*  1*/Initlpm(SQLAllocEnv),
/*  2*/Initlpm(SQLAllocStmt),
/*  3*/Initlpm(SQLBindCol),
/*  4*/Initlpm(SQLCancel),
/*  5*/Initlpm(SQLColAttributes),
/*  6*/Initlpm(SQLConnect),
/*  7*/Initlpm(SQLDescribeCol),
/*  8*/Initlpm(SQLDisconnect),
/*  9*/Initlpm(SQLError),
/* 10*/Initlpm(SQLExecDirect),
/* 11*/Initlpm(SQLExecute),
/* 12*/Initlpm(SQLFetch),
/* 13*/Initlpm(SQLFreeConnect),
/* 14*/Initlpm(SQLFreeEnv),
/* 15*/Initlpm(SQLFreeStmt),
/* 16*/Initlpm(SQLGetCursorName),
/* 17*/Initlpm(SQLNumResultCols),
/* 18*/Initlpm(SQLPrepare),
/* 19*/Initlpm(SQLRowCount),
/* 20*/Initlpm(SQLSetCursorName),
/* 21*/NULL,NULL, 0, NULL,//Initlpm(SQLSetParam),
/* 22*/Initlpm(SQLTransact),
/* 23*/Initlpm(SQLColumns),
/* 24*/Initlpm(SQLDriverConnect),
/* 25*/Initlpm(SQLGetConnectOption),
/* 26*/Initlpm(SQLGetData),
/* 27*/Initlpm(SQLGetFunctions),
/* 28*/Initlpm(SQLGetInfo),
/* 29*/Initlpm(SQLGetStmtOption),
/* 30*/Initlpm(SQLGetTypeInfo),
/* 31*/Initlpm(SQLParamData),
/* 32*/Initlpm(SQLPutData),
/* 33*/Initlpm(SQLSetConnectOption),
/* 34*/Initlpm(SQLSetStmtOption),
/* 35*/Initlpm(SQLSpecialColumns),
/* 36*/Initlpm(SQLStatistics),
/* 37*/Initlpm(SQLTables),
/* 38*/Initlpm(SQLBrowseConnect),
/* 39*/Initlpm(SQLColumnPrivileges),
/* 40*/Initlpm(SQLDataSources),
/* 41*/Initlpm(SQLDescribeParam),
/* 42*/Initlpm(SQLExtendedFetch),
/* 43*/Initlpm(SQLForeignKeys),
/* 44*/Initlpm(SQLMoreResults),
/* 45*/Initlpm(SQLNativeSql),
/* 46*/Initlpm(SQLNumParams),
/* 47*/Initlpm(SQLParamOptions),
/* 48*/Initlpm(SQLPrimaryKeys),
/* 49*/Initlpm(SQLProcedureColumns),
/* 50*/Initlpm(SQLProcedures),
/* 51*/Initlpm(SQLSetPos),
/* 52*/Initlpm(SQLSetScrollOptions),
/* 53*/Initlpm(SQLTablePrivileges),
/* 54*/Initlpm(SQLDrivers),
/* 55*/Initlpm(SQLBindParameter),
/* 56*/Initlpm(SQLAllocHandle),
/* 57*/Initlpm(SQLBindParam),
/* 58*/Initlpm(SQLCloseCursor),
/* 59*/Initlpm(SQLColAttribute),
/* 60*/Initlpm(SQLCopyDesc),
/* 61*/Initlpm(SQLEndTran),
/* 62*/Initlpm(SQLFetchScroll),
/* 63*/Initlpm(SQLFreeHandle),
/* 64*/Initlpm(SQLGetConnectAttr),
/* 65*/Initlpm(SQLGetDescField),
/* 66*/Initlpm(SQLGetDescRec),
/* 67*/Initlpm(SQLGetDiagField),
/* 68*/Initlpm(SQLGetDiagRec),
/* 69*/Initlpm(SQLGetEnvAttr),
/* 70*/Initlpm(SQLGetStmtAttr),
/* 71*/Initlpm(SQLSetConnectAttr),
/* 72*/Initlpm(SQLSetDescField),
/* 73*/Initlpm(SQLSetDescRec),
/* 74*/Initlpm(SQLSetEnvAttr),
/* 75*/Initlpm(SQLSetStmtAttr),
/* 76*/Initlpm(SQLBulkOperations),
#ifdef SQL_API_SQLLOCATOR
/* 77*/Initlpm(SQLLocator),
/* 78*/Initlpm(SQLGetLength),
/* 79*/Initlpm(SQLGetPosition),
/* 80*/Initlpm(SQLGetSubString),
/* 81*/Initlpm(SQLLocatedUpdate),
#else
/* THE FOLLOWING ARE ONLY PLACE HOLDERS */
/* 77*/NULL,NULL, 0, NULL,
/* 78*/NULL,NULL, 0, NULL,
/* 79*/NULL,NULL, 0, NULL,
/* 80*/NULL,NULL, 0, NULL,
/* 81*/NULL,NULL, 0, NULL,
#endif
/* 82*/Initlpm(SQLTables25),
/* 83*/Initlpm(SQLColumns25),
/* 84*/Initlpm(SQLStatistics25),
/* 85*/Initlpm(SQLPrimaryKeys25),
/* 86*/Initlpm(SQLForeignKeys25),
/* 87*/Initlpm(SQLTablePrivileges25),
/* 88*/Initlpm(SQLColumnPrivileges25),
/* 89*/Initlpm(SQLSpecialColumns25),
/* 90*/Initlpm(SQLProcedures25),
/* 91*/Initlpm(SQLProcedureColumns25),
/* 92*/Initlpm(SQLConfigDataSource),
/* 93*/Initlpm(SQLConfigDriver),
/* 94*/Initlpm(SQLCreateDataSource),
/* 95*/Initlpm(SQLGetInstalledDrivers),
/* 96*/Initlpm(SQLGetPrivateProfileString),
/* 97*/Initlpm(SQLInstallDriver),
/* 98*/Initlpm(SQLInstallDriverManager),
/* 99*/Initlpm(SQLInstallTranslator),
/*100*/Initlpm(SQLInstallTranslatorEx),
/*101*/Initlpm(SQLManageDataSources),
/*102*/Initlpm(SQLRemoveDefaultDataSource),
/*103*/Initlpm(SQLRemoveDSNFromIni),
/*104*/Initlpm(SQLRemoveDriver),
/*105*/Initlpm(SQLRemoveDriverManager),
/*106*/Initlpm(SQLRemoveTranslator),
/*107*/Initlpm(SQLValidDSN),
/*108*/Initlpm(SQLWriteDSNToIni),
/*109*/Initlpm(SQLWritePrivateProfileString),
/*110*/Initlpm(SQLGetTranslator),
/*111*/Initlpm(FillParam),
/*112*/Initlpm(SQLInstallerError),
/*113*/Initlpm(SQLInstallDriverEx),
/*114*/Initlpm(SQLPostInstallerError),
/*115*/Initlpm(SQLReadFileDSN),
/*116*/Initlpm(SQLWriteFileDSN),
/*117*/Initlpm(SQLGetConfigMode),
/*118*/Initlpm(SQLSetConfigMode),
/*119*/Initlpm(SQLNextColumn),
/*120*/Initlpm(SQLGetNestedHandle),
	};





//*---------------------------------------------------------------------------------
//| GetApiCount:
//|	Returns the count of functions.
//| Parms:
//|	None.
//| Returns:
//|	Number of ODBC functions
//*---------------------------------------------------------------------------------
int INTFUN GetApiCount()
{
	return NumItems(ApiParms);
}


//*---------------------------------------------------------------------------------
//| GetApiName:
//|	Given the SQL_APIxxx number, return the character name of the function
//|	in the user's buffer.  User's buffer must be large enough.  This function
//|	provides an abstraction of the source of the name, rather than accessing
//|	the structure directly.
//| Parms:
//|	dex						Index of the function
//|	str						User's buffer, if NULL return pointer to our memory
//| Returns:
//|	Pointer to user's buffer
//*---------------------------------------------------------------------------------
LPTSTR INTFUN GetApiName(UINT dex, LPTSTR str)
{
	if(dex > NumItems(ApiParms))
		return NULL;
	else {
        if(str) {
            lstrcpy(str, ApiParms[dex].szName);
            return str;
        } else {
            return ApiParms[dex].szName;
        }
    }
}



//*---------------------------------------------------------------------------------
//| GetFunctionHandler:
//|	Given the SQL_APIxxx number, return a pointer to the function which will
//|	handle the request.
//| Parms:
//|	dex						Index of the function
//|	str						User's buffer, if NULL return pointer to our memory
//| Returns:
//|	Pointer to user's buffer
//*---------------------------------------------------------------------------------
lpFUNCHNDLR GetFunctionHandler(UINT dex)
{
	if(dex > NumItems(ApiParms))
		return NULL;
	else
		return ApiParms[dex].lpFuncHndlr;
}


//*---------------------------------------------------------------------------------
//| GetParmInfo:
//|	Given the SQL_APIxxx number, return a pointer to the parameter information
//|	for that function, as well as the number of parameters in the function.
//| Parms:
//|	dex						Index of the function
//|	pNumParms				Pointer for number of paramters
//| Returns:
//|	Pointer to paramter information, NULL if out of range
//*---------------------------------------------------------------------------------
lpPARAMITEM * GetParmInfo(UINT dex, UINT * pNumParms)
{
	UINT				adex=0;
	lpPARAMITEM	*	lpParmArray=NULL;
	lpDFTARRAY		lpDfts=NULL;
	UINT				cDfts=0;

	if(dex > NumItems(ApiParms))
		return NULL;
	*pNumParms = ApiParms[dex].cParms;

	if (!*pNumParms)
		return NULL;

	// Allocate an array of pointers, then fill each pointer with a valid address
	lpParmArray = (lpPARAMITEM *)AllocateMemory(sizeof(lpPARAMITEM *) * *pNumParms);
	if(!lpParmArray)
		return NULL;

	for (adex=0;  adex<*pNumParms;  adex++)
	{
		lpParmArray[adex] = (lpPARAMITEM)&ApiParms[dex].lpParms[adex];
	}

	return lpParmArray;
}

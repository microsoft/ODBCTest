//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGTMPLT.C
//|
//| Purpose:
//|		This module provides us with the dialog templates.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "resource.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;

//-------------------------------------------------------------------------------
//  Connection Menu
//-------------------------------------------------------------------------------

CONTROL lptSQLAllocEnv[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	------------  -------   ----------
	IDCK_AE_PHENV,	0,						DLG_CKBOX,	NULL,						idsPtrHenv,				0,									0,	0,							0,				0,			NULL,				0,			0,
	};

CONTROL lptSQLAllocConnect[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	------------  -------	----------
	IDCK_AC_HENV,	0,						DLG_CKBOX,	NULL,						idsPtrHdbc,				0,									0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,				0,			0,
	IDCK_AC_PHDBC,	0,						DLG_CKBOX,	NULL,						idsHenv,					0,									1,	0,							0,				0,			NULL,				0,			0,
	};

CONTROL lptSQLConnect[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID			fUpdated	lpUsrWndProc  wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------			--------	------------  -------	---------
	IDCK_C_HDBC,			0,						DLG_CKBOX,	NULL,						idsHdbc,					0,									0,	CBO_DFT_ODBC_HANDLE,	0,							0,			NULL,					0,			0 ,
	IDCB_C_SZDSN,			0,						DLG_COMBO,	NULL,						idsszDSN,				0,									1, CBO_CALCULATED_LEN,		IDCB_C_CBDSN,		0,			SetDftWndProc,		0,			0,
	IDCB_C_SZUID,			0,						DLG_COMBO,	NULL,						idsszUID,				0,									3, CBO_CALCULATED_LEN,		IDCB_C_CBUID,		0,			NULL,					0,			0,
	IDCB_C_SZAUTHSTR,		0,						DLG_COMBO,	NULL,						idsszAuthStr,			0,									5, CBO_CALCULATED_LEN,		IDCB_C_CBAUTHSTR,	0,			NULL,					0,			0,
	IDCB_C_CBDSN,			0,						DLG_COMBO,	NULL,						idscbDSN,				0,									2, CBO_DFT_SHOW_CALLEN |
																																										CBO_PATTERN_MATCHING,0,							0,			NULL,IDCB_C_SZDSN,			0,
	IDCB_C_CBUID,			0,						DLG_COMBO,	NULL,						idscbUID,				0,									4, CBO_DFT_SHOW_CALLEN |
																																										CBO_PATTERN_MATCHING,0,							0,			NULL,IDCB_C_SZUID,			0,
	IDCB_C_CBAUTHSTR,		0,						DLG_COMBO,	NULL,						idscbAuthStr,			0,									6, CBO_DFT_SHOW_CALLEN |
																																										CBO_PATTERN_MATCHING,0,							0,			NULL,IDCB_C_SZAUTHSTR,		0,
	};

CONTROL lptSQLBrowseConnect[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID				fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------				--------	--------------	 -----	---------
	IDCB_BC_SZCONNSTRIN	,	0,						DLG_COMBO,	NULL,						idsszConnStrIn,		0,							1,			CBO_STRING_VAL,		IDCB_BC_CBCONNSTRIN,	0,			NULL,			0,							0,
	IDCB_BC_CBCONNSTRIN	,	0,						DLG_COMBO,	NULL,						idscbConnStrIn,		0,							2,			CBO_DFT_SHOW_CALLEN |
																																											CBO_PATTERN_MATCHING,0,							0,			NULL,IDCB_BC_SZCONNSTRIN,			0,
	IDCK_BC_SZCONNSTROUT	,	0,						DLG_CKBOX,	NULL,						idsszConnStrOut,		0,							3,			0,							0,							0,			NULL,			0,							0,
	IDE_BC_CBCONNSTROUTMAX,	0,						DLG_EDIT,	NULL,						idscbConnStrOutMax,	0,							4,			0,							0,							0,			NULL,			0,							0,
	IDCK_BC_PCBCONNSTROUT,	0,						DLG_CKBOX,	NULL,						idspcbConnStrOut,		0,							5,			0,							0,							0,			NULL,			0,							0,
	IDCK_BC_HDBC			,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							0,			CBO_DFT_ODBC_HANDLE,	0,							0,			NULL,			0,							0,
	};

CONTROL lptSQLDriverConnect[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID				fUpdated	lpUsrWndProc  wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------				--------	--------------	------	---------
	IDCK_DC_HWND				,	0,						DLG_CKBOX,	NULL,						idsHwnd,					0,							1,			0,							0,							0,			NULL,			0,							0,
	IDCB_DC_SZCONNSTRIN		,	0,						DLG_COMBO,	NULL,						idsszConnStrIn,		0,							2,			CBO_STRING_VAL,		IDCB_DC_CBCONNSTRIN,	0,			NULL,IDCB_DC_SZCONNSTRIN,			0,
	IDCB_DC_CBCONNSTRIN		,	0,						DLG_COMBO,	NULL,						idscbConnStrIn,		0,							3,			CBO_DFT_SHOW_CALLEN |
																																												CBO_PATTERN_MATCHING,0,							0,			NULL,			0,							0,
	IDCK_DC_SZCONNSTROUT		,	0,						DLG_CKBOX,	NULL,						idsszConnStrOut,		0,							4,			0,							0,							0,			NULL,			0,							0,
	IDE_DC_CBCONNSTROUTMAX	,	0,						DLG_EDIT,	NULL,						idscbConnStrOutMax,	0,							5,			0,							0,							0,			NULL,			0,							0,
	IDCK_DC_PCBCONNSTROUT	,	0,						DLG_CKBOX,	NULL,						idspcbConnStrOut,		0,							6,			0,							0,							0,			NULL,			0,							0,
	IDCB_DC_FDRIVERCOMPLETION,	0,						DLG_COMBO,	NULL,						idsfDriverCompletion,0,							7,			CBO_PATTERN_MATCHING,0,							0,			NULL,			0,							0,
	IDCK_DC_HDBC				,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							0,			CBO_DFT_ODBC_HANDLE,	0,							0,			NULL,			0,							0,
	};

CONTROL lptSQLDisconnect[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------	------	---------
	IDCK_D_HDBC,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,				0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	};

CONTROL lptSQLFreeConnect[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------	------	---------
	IDCK_FC_HDBC,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,				0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	};

CONTROL lptSQLFreeEnv[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------	------	---------
	IDCK_FE_HENV,	0,						DLG_CKBOX,	NULL,						idsHenv,					0,							0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	};

CONTROL lptSQLSetConnectOption[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID			fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------			--------	-------------- ------	----------
	IDCK_SCO_HDBC		,		0,						DLG_CKBOX,	NULL,						idsHdbc,					0,						IPAR_SCO_HDBC		,						CBO_DFT_ODBC_HANDLE,	0,						0,			NULL,					0,			0,
	IDCB_SCO_FOPTION	,		0,						DLG_COMBO,	NULL,						0,							0,						IPAR_SCO_FOPTION	,						CBO_PATTERN_MATCHING |
									 																																										CBO_LINKED_FIELD,		IDCB_SCO_VPARAM,	0,			SetConAtrWndProc,	0,			0,
	IDCB_SCO_VPARAM	,		0,						DLG_COMBO,	NULL,						0,							0,						IPAR_SCO_VPARAM	,						0,							0,						0,			NULL,					0,			0,
	IDCB_SCO_PARAMETER_TYPE,0,						DLG_COMBO,	NULL,						0,							0,						IPAR_SCO_PARAMETER_TYPE,				CBO_PATTERN_MATCHING,0,						0,			SetConAtrWndProc,	0,			0,
	};

CONTROL lptSQLGetConnectOption[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum							lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------							--------------------	-----------	--------	--------------	------	---------
	IDCK_GCO_HDBC			,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							IPAR_GCO_HDBC			,		CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	IDCB_GCO_FOPTION		,	0,						DLG_COMBO,	NULL,						0,							0,							IPAR_GCO_FOPTION		,		CBO_PATTERN_MATCHING,0,				0,			SetConAtrWndProc,			0,			0,
	IDCK_GCO_PVPARAM		,	0,						DLG_CKBOX,	NULL,						0,							0,							IPAR_GCO_PVPARAM		,		0,							0,				0,			NULL,			0,			0,
	IDCB_GCO_PARAMETER_TYPE,0,						DLG_COMBO,	NULL,						0,							0,							IPAR_GCO_PARAMETER_TYPE,	CBO_PATTERN_MATCHING,0,				0,			SetConAtrWndProc,			0,			0,
	};


CONTROL lptSQLDataSources[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------	------	---------
	IDCK_DS_HENV			,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							0,	CBO_DFT_ODBC_HANDLE,			0,				0,			NULL,			0,			0,
	IDCB_DS_FDIRECTION	,	0,						DLG_COMBO,	NULL,						idsfDirection,			0,							1,	CBO_PATTERN_MATCHING,		0,				0,			NULL,			0,			0,
	IDCK_DS_SZDSN			,	0,						DLG_CKBOX,	NULL,						idsszDSN,				0,							2,	0,									0,				0,			NULL,			0,			0,
	IDE_DS_CBDSNMAX		,	0,						DLG_EDIT,	NULL,						idscbDSNMax,			0,							3,	0,									0,				0,			NULL,			0,			0,
	IDCK_DS_PCBDSN			,	0,						DLG_CKBOX,	NULL,						idspcbDSN,				0,							4,	0,									0,				0,			NULL,			0,			0,
	IDCK_DS_SZDESCRIPTION,	0,						DLG_CKBOX,	NULL,						idsszDescription,		0,							5,	0,									0,				0,			NULL,			0,			0,
	IDE_DS_CBDESCRIPTIONMAX,0,						DLG_EDIT,	NULL,						idscbDescriptionMax,	0,							6,	0,									0,				0,			NULL,			0,			0,
	IDCK_DS_PCBDESCRIPTION,	0,						DLG_CKBOX,	NULL,						idspcbDescription,	0,							7,	0,									0,				0,			NULL,			0,			0,
	};

CONTROL lptSQLDrivers[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle	iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------	--------	--------------------	-----------	--------	--------------	------	---------
	IDCB_D_FDIRECTION			,	0,						DLG_COMBO,	NULL,						idsfDirection,			0,					1,			CBO_PATTERN_MATCHING,0,				0,			NULL,			0,			0,
	IDCK_D_SZDRIVERDESC		,	0,						DLG_CKBOX,	NULL,						0,							0,					2,			0,							0,				0,			NULL,			0,			0,
	IDE_D_CBDRIVERDESCMAX	,	0,						DLG_EDIT,	NULL,						0,							0,					3,			0,							0,				0,			NULL,			0,			0,
	IDCK_D_PCBDRIVERDESC		,	0,						DLG_CKBOX,	NULL,						0,							0,					4,			0,							0,				0,			NULL,			0,			0,
	IDCK_D_SZDRIVERATTRIBUTES,	0,						DLG_CKBOX,	NULL,						0,							0,					5,			0,							0,				0,			NULL,			0,			0,
	IDE_D_CBDRVRATTRMAX		,	0,						DLG_EDIT,	NULL,						0,							0,					6,			0,							0,				0,			NULL,			0,			0,
	IDCK_D_PCBDRVRATTR		,	0,						DLG_CKBOX,	NULL,						0,							0,					7,			0,							0,				0,			NULL,			0,			0,
	IDCK_D_HENV					,	0,						DLG_CKBOX,	NULL,						idsHenv,					0,					0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	};

CONTROL lptSQLGetInfo[] = {
//	wID										lStyle				type			szText					uDescID					uFormatStyle	iParmNum								lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----										-----------------	-----------	--------------------	--------------------	-------------	----------------				--------------------	-----------	--------	--------------	------	---------
	IDCK_GI_HDBC						,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,					IPAR_GI_HSTMT,						CBO_DFT_ODBC_HANDLE,0,				0,			NULL,			0,			0,
	IDCB_GI_FINFOTYPE					,	0,						DLG_COMBO,	NULL,						idsfInfoType,			0,					IPAR_GI_FINFOTYPE,				CBO_PATTERN_MATCHING,0,				0,			SetCTypeWndProc,		0,			0,
	IDCK_GI_RGBINFOVALUE				,	0,						DLG_CKBOX,	NULL,						idsrgbInfoValue,		0,					IPAR_GI_RGBINFOVALUE,			0,							0,				0,			NULL,			0,			0,
	IDE_GI_CBINFOVALUEMAX			,	0,						DLG_EDIT,	NULL,						idscbInfoValueMax,	0,					IPAR_GI_CBINFOVALUEMAX,			0,							0,				0,			NULL,			0,			0,
	IDCK_GI_PCBINFOVALUE				,	0,						DLG_CKBOX,	NULL,						idspcbInfoValue,		0,					IPAR_GI_PCBINFOVALUE,			0,							0,				0,			NULL,			0,			0,
	IDCB_GI_INFORMATION_VALUE_TYPE,	0,						DLG_COMBO,	NULL,						0,							0,					IPAR_GI_INFORMATION_VALUE_TYPE,CBO_PATTERN_MATCHING,0,			0,			SetCTypeWndProc,		0,			0,
	};




//-------------------------------------------------------------------------------
//  Statement Menu
//-------------------------------------------------------------------------------

CONTROL lptSQLAllocStmt[] = {
//	wID					lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----					-----------------	-----------	--------------------	--------------------	------------------	--------	--------------------	-----------	--------	--------------	-------	---------
	IDCK_AS_PHSTMT,	0,						DLG_CKBOX,	NULL,						idsPtrHstmt,			0,							1,			0,							0,				0,			NULL,			0,			0,
	IDCK_AS_HDBC	,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	};

CONTROL lptSQLSetStmtOption[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum							lDataStyle				wLengthID			fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------							--------------------	-----------			--------	--------------
	IDCK_SSO_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,							IPAR_SSO_HSTMT,				CBO_DFT_ODBC_HANDLE,	0,						0,			NULL,			0,			0,
	IDCB_SSO_FOPTION		,	0,						DLG_COMBO,	NULL,						0,							0,							IPAR_SSO_FOPTION,				CBO_PATTERN_MATCHING |
	                      																																                  			CBO_LINKED_FIELD,		IDCB_SSO_VPARAM,	0,			SetConAtrWndProc,			0,			0,
	IDCB_SSO_VPARAM		,	0,						DLG_COMBO,	NULL,						0,							0,							IPAR_SSO_VPARAM,				CBO_PATTERN_MATCHING,0,						0,			NULL,			0,			0,
	IDCB_SSO_PARAMETER_TYPE,0,						DLG_COMBO,	NULL,						0,							0,							IPAR_SSO_PARAMETER_TYPE,	CBO_PATTERN_MATCHING,0,						0,			SetConAtrWndProc,			0,			0,
	};

CONTROL lptSQLGetStmtOption[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------	-------	---------
	IDCK_GSO_HSTMT				,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	IPAR_GSO_HSTMT				,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,			0,
	IDCB_GSO_FOPTION			,	0,						DLG_COMBO,	NULL,						0,							0,	IPAR_GSO_FOPTION			,	CBO_PATTERN_MATCHING,0,				0,			SetConAtrWndProc,			0,			0,
	IDCK_GSO_PVPARAM			,	0,						DLG_CKBOX,	NULL,						0,							0,	IPAR_GSO_PVPARAM			,	0,							0,				0,			NULL,			0,			0,
	IDCB_GSO_PARAMETER_TYPE	,	0,						DLG_COMBO,	NULL,						0,							0,	IPAR_GSO_PARAMETER_TYPE	,	CBO_PATTERN_MATCHING,0,				0,			SetConAtrWndProc,			0,			0,
	};

CONTROL lptSQLPrepare[] = {
//	wID					lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID			fUpdated	lpUsrWndProc    wDataID	fValidate
//	----					-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------			--------	--------------
	IDCB_P_SZSQLSTR,	0,						DLG_COMBO,	NULL,						idsszSqlStr,			0,									1,	CBO_DFT_INPUT_WIN |
																																								   CBO_CALCULATED_LEN |
																																									CBO_DFT_NULL |
																																									CBO_DFT_EMPTY,			IDCB_P_CBSQLSTR,	0,			NULL,			0,			0,
	IDCB_P_CBSQLSTR,	0,						DLG_COMBO,	NULL,						idscbSqlStr,			0,									2,	CBO_PATTERN_MATCHING |
																																									CBO_INWIN_LEN |
																																									CBO_DFT_SHOW_CALLEN,	0,						0,			NULL,			IDCB_P_SZSQLSTR,			0,
	IDCK_P_HSTMT,		0,						DLG_CKBOX,	NULL,						idsHstmt,				0,									0,	CBO_DFT_ODBC_HANDLE,	0,						0,			NULL,			0,			0,
	};

CONTROL lptSQLBindParameter[] = {
//	wID										lStyle				type			szText					uDescID			uFormatStyle	iParmNum					lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----										-----------------	-----------	--------------------	-------------	-------------- --------					--------------------	-----------	--------	--------------
	IDCK_BINDP_HSTMT					,	0,						DLG_CKBOX,	NULL,						idsHstmt,		0,					IPAR_BP_HSTMT,			CBO_DFT_ODBC_HANDLE,	0,					0,			NULL,						0,0,
	IDE_BINDP_IPAR			,				0,						DLG_EDIT,	NULL,						idsipar,			0,					IPAR_BP_IPAR,			0,							0,					0,			BindParameterWndProc,0,0,
	IDCB_BINDP_FPARAMTYPE,				0,						DLG_COMBO,	NULL,						0,					0,					IPAR_BP_FPARAMTYPE,	CBO_PATTERN_MATCHING,0,					0,			BindParameterWndProc,0,0,
	IDCB_BINDP_FCTYPE		,				0,						DLG_COMBO,	NULL,						idsfCType,		0,					IPAR_BP_FCTYPE,		CBO_PATTERN_MATCHING,0,					0,			BindParameterWndProc,0,0,
	IDCB_BINDP_FSQLTYPE	,				0,						DLG_COMBO,	NULL,						idsfSqlType,	0,					IPAR_BP_FSQLTYPE,		CBO_PATTERN_MATCHING,0,					0,			BindParameterWndProc,0,0,
	IDE_BINDP_CBCOLDEF	,				0,						DLG_EDIT,	NULL,						idscbColDef,	0,					IPAR_BP_CBCOLDEF,		0,							0,					0,			NULL,						0,0,
	IDE_BINDP_IBSCALE		,				0,						DLG_EDIT,	NULL,						idsibScale,		0,					IPAR_BP_IBSCALE,		0,							0,					0,			NULL,						0,0,
	IDCB_BINDP_RGBVALUE	,				0,						DLG_COMBO,	NULL,						idsrgbValue,	0,					IPAR_BP_RGBVALUE,		CBO_CALCULATED_LEN |
																																														CBO_VALIDATE_DATA   ,IDCB_BINDP_PCBVALUE,0,	BindParameterWndProc,0,0,
	IDE_BINDP_CBVALUEMAX	,				0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BP_CBVALUEMAX,	0,							0,					0,			NULL,						0,0,
	IDCB_BINDP_PCBVALUE	,				0,						DLG_COMBO,	NULL,						idspcbValue,	0,					IPAR_BP_PCBVALUE,		CBO_DFT_SHOW_CALLEN	|
																																														CBO_PATTERN_MATCHING,0,					0,			BindParameterWndProc,IDCB_BINDP_RGBVALUE,0,
	IDCK_BINDP_SQL_LEN_DATA_AT_EXEC,	0,						DLG_CKBOX,	NULL,						0,					0,					IPAR_BP_DAE,			0,							0,					0,			BindParameterWndProc,0,0,
	IDCB_BINDP_ROWSET_HDBC			,	0,						DLG_COMBO,	NULL,						0,					0,					IPAR_BP_RS_HDBC,		CBO_PATTERN_MATCHING,0,					0,			BindParameterWndProc,0,0,
	IDCB_BINDP_ROWSET_HSTMT			,	0,						DLG_COMBO,	NULL,						0,					0,					IPAR_BP_RS_HSTMT,		CBO_PATTERN_MATCHING,0,					0,			BindParameterWndProc,0,0,
	IDE_BINDP_COLUMN					,	0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BP_COLUMN,		0,							0,					0,			BindParameterWndProc,0,0,
	IDE_BINDP_ROW						,	0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BP_ROW,			0,							0,					0,			BindParameterWndProc,0,0,
	IDE_BINDP_BUFFER_SIZE			,	0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BP_BUFFERSIZE,	0,							0,					0,			NULL,						0,0,
	};

CONTROL lptSQLNumParams[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_NP_PCPAR,	0,						DLG_CKBOX,	NULL,						idspcpar,				0,				1,			0,							0,				0,			NULL,						0,0,
	IDCK_NP_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,				0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,							0,0,
	};

CONTROL lptSQLDescribeParam[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_DP_IPAR			,	0,						DLG_EDIT,	NULL,						idsipar,					0,			1,	0,							0,				0,			NULL,						   0,0,
	IDCK_DP_PFSQLTYPE	,	0,						DLG_CKBOX,	NULL,						idspfSqlType,			0,			2,	0,							0,				0,			NULL,							0,0,
	IDCK_DP_PCBCOLDEF	,	0,						DLG_CKBOX,	NULL,						idspcbColDef,			0,			3,	0,							0,				0,			NULL,							0,0,
	IDCK_DP_PIBSCALE	,	0,						DLG_CKBOX,	NULL,						idspibScale,			0,			4,	0,							0,				0,			NULL,						0,0,
	IDCK_DP_PFNULLABLE,	0,						DLG_CKBOX,	NULL,						idspfNullable,			0,			5,	0,							0,				0,			NULL,						0,0,
	IDCK_DP_HSTMT		,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLExecute[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_EX_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,				0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLExecDirect[] = {
//	wID					lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID			fUpdated	lpUsrWndProc    wDataID	fValidate
//	----					-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------			--------	--------------
	IDCB_ED_SZSQLSTR,	0,						DLG_COMBO,	NULL,						idsszSqlStr,			0,									1,	CBO_DFT_INPUT_WIN |
																																									CBO_CALCULATED_LEN |
																																									CBO_DFT_NULL |
																																									CBO_DFT_EMPTY,			IDCB_ED_CBSQLSTR,	0,			NULL,						0,0,
	IDCB_ED_CBSQLSTR,	0,						DLG_COMBO,	NULL,						idscbSqlStr,			0,									2,	CBO_PATTERN_MATCHING |
																																									CBO_INWIN_LEN |
																																									CBO_DFT_SHOW_CALLEN,	0,						0,			NULL,	IDCB_ED_SZSQLSTR,0,
	IDCK_ED_HSTMT,		0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								 	0,	CBO_DFT_ODBC_HANDLE,	0,						0,			NULL,						0,0,
	};

CONTROL lptSQLSetCursorName[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle					wLengthID			fUpdated	lpUsrWndProc   wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------		-----------			--------	--------------
	IDCB_SCN_SZCURSOR,	0,						DLG_COMBO,	NULL,						idsszCursor,			0,									1,	CBO_STRING_VAL,			IDCB_SCN_CBCURSOR,0,			NULL,						0,0,
	IDCB_SCN_CBCURSOR,	0,						DLG_COMBO,	NULL,						idscbCursor,			0,									2,	CBO_PATTERN_MATCHING |
	                																																				CBO_DFT_SHOW_CALLEN,		0,						0,			NULL,IDCB_SCN_SZCURSOR,0,
	IDCK_SCN_HSTMT	,		0,						DLG_CKBOX,	NULL,						idsHstmt,				0,									0,	CBO_DFT_ODBC_HANDLE,		0,						0,			NULL,						0,0,
	};

CONTROL lptSQLGetCursorName[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_GCN_SZCURSOR	,	0,						DLG_CKBOX,	NULL,						idsszCursor,			0,		1,			0,							0,				0,			NULL,						0,0,
	IDE_GCN_CBCURSORMAX,	0,						DLG_EDIT,	NULL,						idscbCursorMax,		0,		2,			0,							0,				0,			NULL,							0,0,
	IDCK_GCN_PCBCURSOR,	0,						DLG_CKBOX,	NULL,						idspcbCursor,			0,		3,			0,							0,				0,			NULL,						0,0,
	IDCK_GCN_HSTMT		,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLParamData[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_PD_PRGBVALUE,0,						DLG_CKBOX,	NULL,						idsprgbValue,			0,			1,			0,							0,				0,			NULL,						0,0,
	IDCK_PD_HSTMT	,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLPutData[] = {
//	wID					lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle					wLengthID			fUpdated	lpUsrWndProc    wDataID	fValidate
//	----					-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------		-----------			--------	--------------
	IDCB_PUT_RGBVALUE,0,						DLG_COMBO,	NULL,						idsrgbValue,			0,									1,	CBO_DFT_INPUT_WIN |
																																									CBO_CALCULATED_LEN |
																																									CBO_DFT_NULL |
																																									CBO_DFT_EMPTY |
																																									CBO_VALIDATE_DATA,				IDCB_PUT_CBVALUE,	0,			PutDataWndProc,					0,0,
	IDCB_PUT_CBVALUE,	0,						DLG_COMBO,	NULL,						0,							0,									2,	CBO_PATTERN_MATCHING |
																																									CBO_INWIN_LEN |
																																									CBO_DFT_SHOW_CALLEN,		0,						0,			NULL,				IDCB_PUT_RGBVALUE,0,
	IDCK_PUT_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								 	0,	CBO_DFT_ODBC_HANDLE,		0,						0,			NULL,									0,0,
	};

CONTROL lptSQLParamOptions[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_PO_CROW	,	0,						DLG_EDIT,	NULL,						0,							0,				1,	0,							0,				0,			NULL,						0,0,
	IDCK_PO_PIROW,	0,						DLG_CKBOX,	NULL,						0,							0,				2,	0,							0,				0,			NULL,						0,0,
	IDCK_PO_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,				0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLFreeStmt[] = {
//	wID					lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----					-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_FS_FOPTION,	0,						DLG_COMBO,	NULL,						idsfOption,				0,		1,	CBO_PATTERN_MATCHING,0,				0,			NULL,							0,0,
	IDCK_FS_HSTMT	,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLNativeSql[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_NS_SZSQLSTRIN,	0,						DLG_COMBO,	NULL,						idsszSqlStrIn,			0,									1,	CBO_DFT_INPUT_WIN |
																																										CBO_DFT_NULL |
																																										CBO_DFT_EMPTY |
																																										CBO_CALCULATED_LEN,	IDCB_NS_CBSQLSTRIN,	0,	NULL,					0,0,
	IDCB_NS_CBSQLSTRIN,	0,						DLG_COMBO,	NULL,						idscbSqlStrIn,			0,									2,	CBO_PATTERN_MATCHING |
																																										CBO_INWIN_LEN |
																																										CBO_DFT_SHOW_CALLEN, 0,				0,			NULL,IDCB_NS_SZSQLSTRIN,0,
	IDCK_NS_SZSQLSTR,		0,						DLG_CKBOX,	NULL,						idsszSqlStr,			0,									3,	0,							0,				0,			NULL,						0,0,
	IDE_NS_CBSQLSTRMAX,	0,						DLG_EDIT,	NULL,						idscbSqlStrMax,		0,									4,	0,							0,				0,			NULL,						0,0,
	IDCK_NS_PCBSQLSTR,	0,						DLG_CKBOX,	NULL,						idspcbSqlStr,			0,									5,	0,							0,				0,			NULL,						0,0,
	IDCK_NS_HDBC,			0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								 	0,	0,							0,				0,			NULL,						0,0,
	};


CONTROL lptFillParam[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle						wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------			-----------	--------	--------------
	IDC_FP_HSTMT,					0,						DLG_STATIC,	TEXT(""),						0,							0,					  -1,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	IDC_FP_IPAR					,	0,						DLG_EDIT,	NULL,								0,							0,					   0,	0,							0,				0,			FillParamWndProc,						0,0,
	IDC_FP_ROW					,	0,						DLG_EDIT,	NULL,								0,							0,					   1,	0,							0,				0,			FillParamWndProc,						0,0,
	IDC_FP_FPARAMTYPE_LABEL	,	0,						DLG_STATIC,	TEXT("fParamType:"),			0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_FPARAMTYPE			,	0,						DLG_STATIC,	TEXT(""),						0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_FCTYPE_LABEL		,	0,						DLG_STATIC,	TEXT("fCType:"),				0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_FCTYPE				,	0,						DLG_STATIC,	TEXT(""),						0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_FSQLTYPE_LABEL	,	0,						DLG_STATIC,	TEXT("fSqlType:"),			0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_FSQLTYPE			,  0,						DLG_STATIC,	TEXT(""),						0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_CBCOLDEF_LABEL	,	0,						DLG_STATIC,	TEXT("cbColDef:"),			0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_CBCOLDEF			,	0,						DLG_STATIC,	TEXT(""),						0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_IBSCALE_LABEL		,	0,						DLG_STATIC,	TEXT("ibScale:"),				0,							0,					  -1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_IBSCALE				,	0,						DLG_STATIC, TEXT(""),						0,							0,					  -1,	0,							0,				0,			NULL,							0,0,
	IDC_FP_RGBVALUE			,	0,						DLG_EDIT,	NULL,								0,							0,						2,	EDT_CALCULATED_LEN,	IDC_FP_CBVALUEMAX,0,	FillParamWndProc,						0,0,
	IDC_FP_CBVALUEMAX_LABEL	,	0,						DLG_STATIC,	TEXT("cbValueMax:"),			0,							0,					  -1,	0,							0,				0,			NULL,							0,0,
	IDC_FP_CBVALUEMAX			,	0,						DLG_STATIC, TEXT(""),						0,							0,					  -1,	0,							0,				0,			NULL,							0,0,
	IDC_FP_PCBVALUE			,	0,						DLG_COMBO,	NULL,								0,							0,						3,	CBO_DFT_SHOW_CALLEN |
																																											CBO_PATTERN_MATCHING,0,			0,			FillParamWndProc,						0,0,
	IDC_FP_MACRO			,	0,						DLG_CKBOX,	NULL,								0,							0,								4,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_HDBC_LABEL	,	0,						DLG_STATIC,	TEXT("Rowset hdbc:"),		0,							0,							-1,	0,							0,				0,			NULL,							0,0,
	IDC_FP_RS_HDBC			,	0,						DLG_STATIC,	TEXT(""),						0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_HSTMT_LABEL,	0,						DLG_STATIC,	TEXT("Rowset hstmt:"),		0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_HSTMT		,  0,						DLG_STATIC,	TEXT(""),						0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_COLUMN_LABEL,	0,						DLG_STATIC,	TEXT("Column:"),				0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_COLUMN		,	0,						DLG_STATIC, TEXT(""),						0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_ROW_LABEL	,	0,						DLG_STATIC,	TEXT("Row:"),					0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_RS_ROW			,	0,						DLG_STATIC,	TEXT(""),						0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	IDC_FP_HSTMT_LABEL,		0,						DLG_STATIC,	TEXT("hstmt:"),				0,							0,							-1,	0,							0,				0,			NULL,						0,0,
	};

//-------------------------------------------------------------------------------
//  Results Menu
//-------------------------------------------------------------------------------

CONTROL lptSQLNumResultCols[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_NRC_PCCOL,	0,						DLG_CKBOX,	NULL,						idspccol,				0,			1,			0,							0,				0,			NULL,							0,0,
	IDCK_NRC_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLDescribeCol[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_DC_ICOL			,	0,						DLG_EDIT,	NULL,						idsicol,					0,		1,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_SZCOLNAME	,	0,						DLG_CKBOX,	NULL,						idsszColName,			0,		2,			0,							0,				0,			NULL,						0,0,
	IDE_DC_CBCOLNAMEMAX,	0,						DLG_EDIT,	NULL,						idscbColNameMax,		0,		3,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_PCBCOLNAME,	0,						DLG_CKBOX,	NULL,						idspcbColName,			0,		4,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_PFSQLTYPE	,	0,						DLG_CKBOX,	NULL,						idspfSqlType,			0,		5,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_PCBCOLDEF	,	0,						DLG_CKBOX,	NULL,						idspcbColDef,			0,		6,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_PIBSCALE	,	0,						DLG_CKBOX,	NULL,						idspibScale,			0,		7,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_PFNULLABLE,	0,						DLG_CKBOX,	NULL,						idspfNullable,			0,		8,			0,							0,				0,			NULL,						0,0,
	IDCK_DC_HSTMT		,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLColAttributes[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_CA_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	IPAR_CAS_HSTMT,				CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	IDE_CA_ICOL				,	0,						DLG_EDIT,	NULL,						idsicol,					0,	IPAR_CAS_ICOL,					0,							0,				0,			NULL,						0,0,
	IDCB_CA_FDESCTYPE		,	0,						DLG_COMBO,	NULL,						idsfDescType,			0,	IPAR_CAS_FDESCTYPE,			CBO_PATTERN_MATCHING,0,				0,			SetCTypeWndProc,						0,0,
	IDCK_CA_RGBDESC		,	0,						DLG_CKBOX,	NULL,						idsrgbDesc,				0,	IPAR_CAS_RGBDESC,				0,							0,				0,			NULL,						0,0,
	IDE_CA_CBDESCMAX		,	0,						DLG_EDIT,	NULL,						idscbDescMax,			0,	IPAR_CAS_CBDESCMAX,			0,							0,				0,			NULL,						0,0,
	IDCK_CA_PCBDESC		,	0,						DLG_CKBOX,	NULL,						idspcbDesc,				0,	IPAR_CAS_PCBDESC,				0,							0,				0,			NULL,							0,0,
	IDCK_CA_PFDESC			,	0,						DLG_CKBOX,	NULL,						idspfDesc,				0,	IPAR_CAS_PFDESC,				0,							0,				0,			NULL,							0,0,
	IDCB_CA_DESCRIPTION_TYPE,0,					DLG_COMBO,	NULL,						0,							0, IPAR_CAS_DESCRIPTION_TYPE,	CBO_PATTERN_MATCHING,0,				0,			SetCTypeWndProc,						0,0,
	};

CONTROL lptSQLColAttribute[] = {
//	wID										lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----										----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_CA_HSTMT,							0,						DLG_COMBO,	NULL,						idsHstmt,				0,								IPAR_CA_HSTMT,						CBO_PATTERN_MATCHING |
																																																						CBO_DFT_NULL_HSTMT |
																																																						CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,							0,0,
	IDE_CA_ICOLUMNNUMBER,				0,						DLG_EDIT,	NULL,						idsicol,					0,								IPAR_CA_ICOLUMNNUMBER,			0,							0,				0,			NULL,	 						0,0,
	IDCB_CA_FFIELDINDENTIFIER,			0,						DLG_COMBO,	NULL,						idsfFieldIndentifier,0,								IPAR_CA_FFIELDINDENTIFIER,		CBO_PATTERN_MATCHING,0,				0,			SetCTypeWndProc,						0,0,
	IDCK_CA_RGBCHARACTERATTRIBUTE,	0,						DLG_CKBOX,	NULL,						idsrgbCharacterAttribute,0,						IPAR_CA_RGBCHARACTERATTRIBUTE,0,							0,				0,			NULL,						0,0,
	IDE_CA_CBBUFFERLENGTH,				0,						DLG_EDIT,	NULL,						idscbBuffLength,		0,								IPAR_CA_CBBUFFERLENGTH,			0,							0,				0,			NULL,						0,0,
	IDCK_CA_PCBSTRINGLENGTH,			0,						DLG_CKBOX,	NULL,						idspcbStrLength,		0,								IPAR_CA_PCBSTRINGLENGTH,		0,							0,				0,			NULL,						0,0,
	IDCK_CA_PVNUMERICATTRIBUTE,		0,						DLG_CKBOX,	NULL,						idspvNumericAttribute,0,							IPAR_CA_PVNUMERICATTRIBUTE,	0,							0,				0,			NULL,							0,0,
	IDCB_CA_FFIELDINDENTIFIER_TYPE,	0,						DLG_COMBO,	NULL,						0,							0,								IPAR_CA_FFIELDINDENTIFIER_TYPE,CBO_PATTERN_MATCHING,0,				0,		SetCTypeWndProc, 						0,0,
	};

CONTROL lptSQLRowCount[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_RC_PCROW,	0,						DLG_CKBOX,	NULL,						idspcrow,				0,				1,	0,							0,				0,			NULL,						0,0,
	IDCK_RC_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,				0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLBindCol[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_BC_ICOL		,	0,						DLG_EDIT,	NULL,						idsicol,					0,			1,	0,							0,				0,			NULL,						0,0,
	IDCB_BC_FCTYPE	,	0,						DLG_COMBO,	NULL,						idsfCType,				0,			2,	CBO_PATTERN_MATCHING,0,				0,			NULL,							0,0,
	IDCK_BC_RGBVALUE,	0,						DLG_CKBOX,	NULL,						idsrgbValue,			0,			3,	0,							0,				0,			NULL,						0,0,
	IDE_BC_CBVALUEMAX,0,						DLG_EDIT,	NULL,						idscbValueMax,			0,			4,	0,							0,				0,			NULL,							0,0,
	IDCK_BC_PCBVALUE,	0,						DLG_CKBOX,	NULL,						idspcbValue,			0,			5,	0,							0,				0,			NULL,							0,0,
	IDCK_BC_HSTMT	,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,							0,0,
    IDCK_BC_DATAATFETCH,0,                      DLG_CKBOX,  NULL,                       idsDataAtFetch,                   0,          6,  0,                          0,          0,          NULL,                           0,0
	};

CONTROL lptSQLFetch[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_F_HSTMT,				0,			DLG_CKBOX,	NULL,			idsHstmt,				0,							IPAR_F_HSTMT,						CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,		0,0,
	IDCK_F_DISPLAY_ROWSET,	0,			DLG_CKBOX,	NULL,			idsDisplayRowset,		0,							IPAR_F_DISPLAY_ROWSET,			0,							0,				0,			DisplayRowsetWndProc,  0,0	,
	};

CONTROL lptSQLExtendedFetch[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_EF_HSTMT		,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			IPAR_EF_HSTMT,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,	 		0,0,
	IDCB_EF_FFETCHTYPE,	0,						DLG_COMBO,	NULL,						idsfFetchType,			0,			IPAR_EF_FFETCHTYPE,	CBO_PATTERN_MATCHING,0,				0,			NULL,	 		0,0 ,
	IDE_EF_IROW			,	0,						DLG_EDIT,	NULL,						idsirow,					0,			IPAR_EF_IROW,			0,							0,				0,			NULL,	 		0,0,
	IDCK_EF_PCROW		,	0,						DLG_CKBOX,	NULL,						idspcrow,				0,			IPAR_EF_PCROW,			0,							0,				0,			NULL,	  		0,0,
	IDCK_EF_RGFROWSTATUS,0,						DLG_CKBOX,	NULL,						idsrgfRowStatus,		0,			IPAR_EF_RGFROWSTATUS,0,							0,				0,			NULL,	 		0,0 ,
	IDCK_EF_DISPLAY_ROWSET,0,					DLG_CKBOX,	NULL,						idsDisplayRowset,		0,			IPAR_EF_DISPLAY_ROW,	0,							0,				0,			DisplayRowsetWndProc,		0,0,
	};

CONTROL lptSQLFetchScroll[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc
//	----								-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_FS_HSTMT,					0,						DLG_COMBO,	NULL,						idsHstmt,				0,							IPAR_FS_HSTMT,						CBO_PATTERN_MATCHING |
																																																			CBO_DFT_NULL_HSTMT |
																																																			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,			0,0 ,
	IDCB_FS_FFETCHORIENTATION,	0,						DLG_COMBO,	NULL,						idsfFetchOrientation,0,							IPAR_FS_FFETCHORIENTATION,		CBO_PATTERN_MATCHING,0,				0,			NULL,			0,0,
	IDE_FS_FFETCHOFFSET,			0,						DLG_EDIT,	NULL,						idsfFetchOffset,		0,							IPAR_FS_FFETCHOFFSET,			0,							0,				0,			NULL,	  		0,0,
	IDCK_FS_DISPLAY_ROWSET,		0,						DLG_CKBOX,	NULL,						idsDisplayRowset,		0,							IPAR_FS_DISPLAY_ROWSET,			0,							0,				0,			DisplayRowsetWndProc, 		0,0,
	};


CONTROL lptSQLSetPos[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_SP_IROW		,	0,						DLG_EDIT,	NULL,						idsirow,					0,			1,			0,							0,				0,			NULL,						0,0,
	IDCB_SP_FOPTION,	0,						DLG_COMBO,	NULL,						idsfOption,				0,			2,			CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCB_SP_FLOCK	,	0,						DLG_COMBO,	NULL,						idsfLock,				0,			3,			CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCK_SP_HSTMT	,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLBulkOperations[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum					lDataStyle					wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------					--------------------		-----------	--------	--------------
	IDCB_BO_HSTMT,			0,						DLG_COMBO,	NULL,						idsHstmt,				0,							IPAR_BO_HSTMT,			CBO_PATTERN_MATCHING |
																																														CBO_DFT_NULL_HSTMT |
																																														CBO_DFT_ODBC_HANDLE,		0,				0,			NULL,							0,0,
	IDCB_BO_OPERATION,	0,						DLG_COMBO,	NULL,						idsfOption,				0,							IPAR_BO_OPERATION,	CBO_PATTERN_MATCHING,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetData[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_GD_ICOL		,	0,						DLG_EDIT,	NULL,						idsicol,					0,			1,	0,							0,		0,								NULL,						0,0,
	IDCB_GD_FCTYPE	,	0,						DLG_COMBO,	NULL,						idsfCType,				0,			2,	CBO_PATTERN_MATCHING,0,		0,								NULL,						0,0,
	IDCK_GD_RGBVALUE,	0,						DLG_CKBOX,	NULL,						idsrgbValue,			0,			3,	0,							0,		IDCK_GD_PCBVALUE,			NULL,						0,0,
	IDE_GD_CBVALUEMAX,0,						DLG_EDIT,	NULL,						idscbValueMax,			0,			4,	0,							0,		0,								NULL,						0,0,
	IDCK_GD_PCBVALUE,	0,						DLG_CKBOX,	NULL,						idspcbValue,			0,			5,	0,							0,		0,								NULL,						0,0,
	IDCK_GD_HSTMT	,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE,	0,		0,								NULL,						0,0,
	};

CONTROL lptSQLGetNestedHandle[] = {
	//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
	//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDE_GNH_ICOL		,	0,						DLG_EDIT,	NULL,						idsicol,				0,			1,	0,							0,		0,								NULL,						0,0,
	IDCK_GNH_OUTPUTHANDLE,0, 					DLG_CKBOX,	NULL,						idsHOutput, 		   0,							2,			0,							0,				0,			NULL,							0,0,
	IDCK_GNH_HSTMT	,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE,	0,		0,								NULL,						0,0,
};

CONTROL lptSQLMoreResults[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_MR_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,							0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,							0,0,
	};

CONTROL lptSQLNextColumn[] = {
	//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
	//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_NRC_PCCOL, 0, DLG_CKBOX, NULL, idspccol, 0, 1, 0, 0, 0, NULL, 0, 0,
	IDCK_NRC_HSTMT, 0, DLG_CKBOX, NULL, idsHstmt, 0, 0, CBO_DFT_ODBC_HANDLE, 0, 0, NULL, 0, 0,
};

CONTROL lptSQLSetScrollOptions[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_SSOP_FCONCURRENCY,	0,						DLG_COMBO,	NULL,						0,							0,	1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCB_SSOP_CROWKEYSET	,	0,						DLG_COMBO,	NULL,						0,							0,	2,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDE_SSOP_CROWROWSET	,	0,						DLG_EDIT,	NULL,						0,							0,	3,	0,							0,				0,			NULL,						0,0,
	IDCK_SSOP_HSTMT		,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};


//-------------------------------------------------------------------------------
//  Catalog Menu
//-------------------------------------------------------------------------------


CONTROL lptSQLGetTypeInfo[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_GTI_FSQLTYPE,	0,						DLG_COMBO,	NULL,						idsfSqlType,			0,				1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCK_GTI_HSTMT	,		0,						DLG_CKBOX,	NULL,						idsHstmt,				0,				0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};



CONTROL lptSQLTables[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle					wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------		-----------	--------	--------------
	IDCB_T_SZCATALOGNAME,	0,						DLG_COMBO,	NULL,						idsszCatalogName,		0,		1,		CBO_STRING_VAL,			IDCB_T_CBCATALOGNAME,0,			NULL,							0,0,
	IDCB_T_SZSCHEMANAME,		0,						DLG_COMBO,	NULL,						idsszSchemaName,		0,		3,		CBO_STRING_VAL,			IDCB_T_CBSCHEMANAME,	0,			NULL,							0,0,
	IDCB_T_SZTABLENAME	,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,		5,		CBO_STRING_VAL,			IDCB_T_CBTABLENAME,	0,			NULL,							0,0,
	IDCB_T_SZTABLETYPE	,	0,						DLG_COMBO,	NULL,						idsszTableType,		0,		7,		CBO_STRING_VAL,			IDCB_T_CBTABLETYPE,	0,			NULL,							0,0,
	IDCB_T_CBCATALOGNAME,	0,						DLG_COMBO,	NULL,						idscbCatalogName,		0,		2,		CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,	0,								0,			NULL,IDCB_T_SZCATALOGNAME,0,
	IDCB_T_CBSCHEMANAME,		0,						DLG_COMBO,	NULL,						idscbSchemaName,		0,		4,		CBO_PATTERN_MATCHING |
	                   																														CBO_DFT_SHOW_CALLEN,	0,								0,			NULL,	IDCB_T_SZSCHEMANAME,0,
	IDCB_T_CBTABLENAME	,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,		6,		CBO_PATTERN_MATCHING |
	                   																														CBO_DFT_SHOW_CALLEN,	0,								0,			NULL,	IDCB_T_SZTABLENAME,0,
	IDCB_T_CBTABLETYPE	,	0,						DLG_COMBO,	NULL,						idscbTableType,		0,		8,		CBO_PATTERN_MATCHING |
	                   																														CBO_DFT_SHOW_CALLEN,	0,								0,			NULL,	IDCB_T_SZTABLETYPE,0,
	IDCK_T_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,		CBO_DFT_ODBC_HANDLE,	0,								0,			NULL,							0,0,
	};


CONTROL lptSQLColumns[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_COL_SZCATALOGNAME,	0,						DLG_COMBO,	NULL,						idsszCatalogName,		0,		1,		CBO_STRING_VAL,		IDCB_COL_CBCATALOGNAME,			0,			NULL,							0,0,
	IDCB_COL_SZSCHEMANAME,	0,						DLG_COMBO,	NULL,						idsszSchemaName,		0,		3,		CBO_STRING_VAL,		IDCB_COL_CBSCHEMANAME ,			0,			NULL,							0,0,
	IDCB_COL_SZTABLENAME	,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,		5,		CBO_STRING_VAL,		IDCB_COL_CBTABLENAME  ,			0,			NULL,							0,0,
	IDCB_COL_SZCOLUMNNAME,	0,						DLG_COMBO,	NULL,						idsszColumnName,		0,		7,		CBO_STRING_VAL,		IDCB_COL_CBCOLUMNNAME ,			0,			NULL,							0,0,
	IDCB_COL_CBCATALOGNAME,	0,						DLG_COMBO,	NULL,						idscbCatalogName,		0,		2,		CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_COL_SZCATALOGNAME,0,
	IDCB_COL_CBSCHEMANAME,	0,						DLG_COMBO,	NULL,						idscbSchemaName,		0,		4,		CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_COL_SZSCHEMANAME,0,
	IDCB_COL_CBTABLENAME	,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,		6,		CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_COL_SZTABLENAME,0,
	IDCB_COL_CBCOLUMNNAME,	0,						DLG_COMBO,	NULL,						idscbColumnName,		0,		8,		CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_COL_SZCOLUMNNAME,0,
	IDCK_COL_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,		CBO_DFT_ODBC_HANDLE,	0,										0,			NULL,						0,0,
	};


CONTROL lptSQLStatistics[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_S_SZCATALOGNAME,	0,						DLG_COMBO,	NULL,						idsszCatalogName,		0,	1,			CBO_STRING_VAL,		IDCB_S_CBCATALOGNAME,			0,			NULL,						0,0,
	IDCB_S_SZSCHEMANAME,		0,						DLG_COMBO,	NULL,						idsszSchemaName,		0,	3,			CBO_STRING_VAL,		IDCB_S_CBSCHEMANAME ,			0,			NULL,						0,0,
	IDCB_S_SZTABLENAME	,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,	5,			CBO_STRING_VAL,		IDCB_S_CBTABLENAME  ,			0,			NULL,						0,0,
	IDCB_S_FUNIQUE		,		0,						DLG_COMBO,	NULL,						0,							0,	7,			CBO_PATTERN_MATCHING,0,										0,			NULL,						0,0,
	IDCB_S_CBCATALOGNAME,	0,						DLG_COMBO,	NULL,						idscbCatalogName,		0,	2,			CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,	IDCB_S_SZCATALOGNAME,0,
	IDCB_S_CBSCHEMANAME,		0,						DLG_COMBO,	NULL,						idscbSchemaName,		0,	4,			CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,	IDCB_S_SZSCHEMANAME,0,
	IDCB_S_CBTABLENAME	,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,	6,			CBO_PATTERN_MATCHING |
	                      																													CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,	IDCB_S_SZTABLENAME,0,
	IDCB_S_FACCURACY	,		0,						DLG_COMBO,	NULL,						0,							0,	8,			CBO_PATTERN_MATCHING,0,										0,			NULL,						0,0,
	IDCK_S_HSTMT      ,		0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	0,			CBO_DFT_ODBC_HANDLE,	0,										0,			NULL,						0,0,
	};


CONTROL lptSQLPrimaryKeys[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_PK_SZCATALOGNAME	,	0,						DLG_COMBO,	NULL,						idsszCatalogName,		0,							1,			CBO_STRING_VAL,		IDCB_PK_CBCATALOGNAME  ,	0,			NULL,						0,0,
	IDCB_PK_SZSCHEMANAME	,		0,						DLG_COMBO,	NULL,						idsszSchemaName,		0,							3,			CBO_STRING_VAL,		IDCB_PK_CBSCHEMANAME	,		0,			NULL,						0,0,
	IDCB_PK_SZTABLENAME	,		0,						DLG_COMBO,	NULL,						idsszTableName,		0,							5,			CBO_STRING_VAL,		IDCB_PK_CBTABLENAME	   ,	0,			NULL,						0,0,
	IDCB_PK_CBCATALOGNAME	,	0,						DLG_COMBO,	NULL,						idscbCatalogName,		0,							2,			CBO_PATTERN_MATCHING |
					                      																																CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_PK_SZCATALOGNAME,0,
	IDCB_PK_CBSCHEMANAME	,		0,						DLG_COMBO,	NULL,						idscbSchemaName,		0,							4,			CBO_PATTERN_MATCHING |
					                      																																CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_PK_SZSCHEMANAME,0,
	IDCB_PK_CBTABLENAME	,		0,						DLG_COMBO,	NULL,						idscbTableName,		0,							6,			CBO_PATTERN_MATCHING |
					                      																																CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_PK_SZTABLENAME,0,
	IDCK_PK_HSTMT			,		0,						DLG_CKBOX,	NULL,						idsHstmt,				0,							0,			CBO_DFT_ODBC_HANDLE,	0,									0,			NULL,						0,0,
	};



CONTROL lptSQLForeignKeys[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle				iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------		--------	--------------------	-----------	--------	--------------
	IDCB_FK_SZPKCATALOGNAME	,	0,						DLG_COMBO,	NULL,						0,							0,		1,		CBO_STRING_VAL,		IDCB_FK_CBPKCATALOGNAME,	0,			NULL,									0,0,
	IDCB_FK_SZPKSCHEMANAME	,	0,						DLG_COMBO,	NULL,						0,							0,		3,		CBO_STRING_VAL,		IDCB_FK_CBPKSCHEMANAME ,	0,			NULL,									0,0,
	IDCB_FK_SZPKTABLENAME	,	0,						DLG_COMBO,	NULL,						0,							0,		5,		CBO_STRING_VAL,		IDCB_FK_CBPKTABLENAME  ,	0,			NULL,									0,0,
	IDCB_FK_CBPKCATALOGNAME	,	0,						DLG_COMBO,	NULL,						0,							0,		2,		CBO_PATTERN_MATCHING |
					                      																										CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_FK_SZPKCATALOGNAME,		0,
	IDCB_FK_CBPKSCHEMANAME	,	0,						DLG_COMBO,	NULL,						0,							0,		4,		CBO_PATTERN_MATCHING |
					                      																										CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_FK_SZPKSCHEMANAME,		0,
	IDCB_FK_CBPKTABLENAME	,	0,						DLG_COMBO,	NULL,						0,							0,		6,		CBO_PATTERN_MATCHING |
					                      																										CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_FK_CBPKTABLENAME,		0,
	IDCB_FK_SZFKCATALOGNAME	,	0,						DLG_COMBO,	NULL,						0,							0,		7,		CBO_STRING_VAL,		IDCB_FK_CBFKCATALOGNAME ,	0,			NULL,									0,0,
	IDCB_FK_SZFKSCHEMANAME	,	0,						DLG_COMBO,	NULL,						0,							0,		9,		CBO_STRING_VAL,		IDCB_FK_CBFKSCHEMANAME  ,	0,			NULL,									0,0,
	IDCB_FK_SZFKTABLENAME	,	0,						DLG_COMBO,	NULL,						0,							0,		11,	CBO_STRING_VAL,		IDCB_FK_CBFKTABLENAME   ,	0,			NULL,									0,0,
	IDCB_FK_CBFKCATALOGNAME	,	0,						DLG_COMBO,	NULL,						0,							0,		8,		CBO_PATTERN_MATCHING |
					                      																										CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_FK_SZFKCATALOGNAME,		0,
	IDCB_FK_CBFKSCHEMANAME	,	0,						DLG_COMBO,	NULL,						0,							0,		10,	CBO_PATTERN_MATCHING |
					                      																										CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_FK_SZFKSCHEMANAME,		0,
	IDCB_FK_CBFKTABLENAME	,	0,						DLG_COMBO,	NULL,						0,							0,		12,	CBO_PATTERN_MATCHING |
					                      																										CBO_DFT_SHOW_CALLEN,	0,									0,			NULL,IDCB_FK_SZFKTABLENAME,			0,
	IDCK_FK_HSTMT				,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,		CBO_DFT_ODBC_HANDLE,	0,									0,			NULL,									 0,0,
	};

CONTROL lptSQLTablePrivileges[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_TP_SZCATALOGNAME,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,	1,			CBO_STRING_VAL,		IDCB_TP_CBCATALOGNAME ,			0,			NULL,						0,0,
	IDCB_TP_SZSCHEMANAME	,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,	3,			CBO_STRING_VAL,		IDCB_TP_CBSCHEMANAME	,			0,			NULL,						0,0,
	IDCB_TP_SZTABLENAME	,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,	5,			CBO_STRING_VAL,		IDCB_TP_CBTABLENAME	,			0,			NULL,						0,0,
	IDCB_TP_CBCATALOGNAME,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,	2,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_TP_SZCATALOGNAME,0,
	IDCB_TP_CBSCHEMANAME	,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,	4,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_TP_SZSCHEMANAME,0,
	IDCB_TP_CBTABLENAME	,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,	6,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_TP_SZTABLENAME,0,
	IDCK_TP_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	0,			CBO_DFT_ODBC_HANDLE,	0,										0,			NULL,						0,0,
	};

CONTROL lptSQLColumnPrivileges[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_CP_SZCATALOGNAME,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,	1,			CBO_STRING_VAL,		IDCB_CP_CBCATALOGNAME,			0,			NULL,						0,0,
	IDCB_CP_SZSCHEMANAME	,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,	3,			CBO_STRING_VAL,		IDCB_CP_CBSCHEMANAME	,			0,			NULL,						0,0,
	IDCB_CP_SZTABLENAME	,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,	5,			CBO_STRING_VAL,		IDCB_CP_CBTABLENAME	,			0,			NULL,						0,0,
	IDCB_CP_SZCOLUMNNAME	,	0,						DLG_COMBO,	NULL,						idsszColumnName,		0,	7,			CBO_STRING_VAL,		IDCB_CP_CBCOLUMNNAME	,			0,			NULL,						0,0,
	IDCB_CP_CBCATALOGNAME,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,	2,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_CP_SZCATALOGNAME,0,
	IDCB_CP_CBSCHEMANAME	,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,	4,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_CP_SZSCHEMANAME,0,
	IDCB_CP_CBTABLENAME	,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,	6,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_CP_SZTABLENAME,0,
	IDCB_CP_CBCOLUMNNAME	,	0,						DLG_COMBO,	NULL,						idscbColumnName,		0,	8,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_CP_SZCOLUMNNAME,0,
	IDCK_CP_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	0,			CBO_DFT_ODBC_HANDLE,	0,										0,			NULL,						0,0,
	};

CONTROL lptSQLSpecialColumns[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_SC_FCOLTYPE		,	0,						DLG_COMBO,	NULL,						0,							0,	1,			CBO_PATTERN_MATCHING,0,										0,			NULL,						0,0,
	IDCB_SC_SZCATALOGNAME,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,	2,			CBO_STRING_VAL,		IDCB_SC_CBCATALOGNAME ,			0,			NULL,						0,0,
	IDCB_SC_SZSCHEMANAME	,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,	4,			CBO_STRING_VAL,		IDCB_SC_CBSCHEMANAME	 ,			0,			NULL,						0,0,
	IDCB_SC_SZTABLENAME	,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,	6,			CBO_STRING_VAL,		IDCB_SC_CBTABLENAME   ,			0,			NULL,						0,0,
	IDCB_SC_CBCATALOGNAME,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,	3,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_SC_SZCATALOGNAME,0,
	IDCB_SC_CBSCHEMANAME	,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,	5,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_SC_SZSCHEMANAME,0,
	IDCB_SC_CBTABLENAME	,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,	7,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_SC_SZTABLENAME,0,
	IDCB_SC_FSCOPE			,	0,						DLG_COMBO,	NULL,						0,							0,	8,			CBO_PATTERN_MATCHING,0,										0,			NULL,						0,0,
	IDCB_SC_FNULLABLE		,	0,						DLG_COMBO,	NULL,						0,							0,	9,			CBO_PATTERN_MATCHING,0,										0,			NULL,						0,0,
	IDCK_SC_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	0,			CBO_DFT_ODBC_HANDLE,	0,										0,			NULL,						0,0,
	};

CONTROL lptSQLProcedures[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_PROC_SZPROCCATALOG	,	0,						DLG_COMBO,	NULL,						0,							0,							1,			CBO_STRING_VAL,		IDCB_PROC_CBPROCCATALOG ,			0,			NULL,						0,0,
	IDCB_PROC_SZPROCSCHEMA	,	0,						DLG_COMBO,	NULL,						0,							0,							3,			CBO_STRING_VAL,		IDCB_PROC_CBPROCSCHEMA  ,			0,			NULL,						0,0,
	IDCB_PROC_SZPROCNAME		,	0,						DLG_COMBO,	NULL,						0,							0,							5,			CBO_STRING_VAL,		IDCB_PROC_CBPROCNAME    ,			0,			NULL,						0,0,
	IDCB_PROC_CBPROCCATALOG	,	0,						DLG_COMBO,	NULL,						0,							0,							2,			CBO_PATTERN_MATCHING |
				                      																																	CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,IDCB_PROC_SZPROCCATALOG,0,
	IDCB_PROC_CBPROCSCHEMA	,	0,						DLG_COMBO,	NULL,						0,							0,							4,			CBO_PATTERN_MATCHING |
				                      																																	CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,IDCB_PROC_SZPROCSCHEMA,0,
	IDCB_PROC_CBPROCNAME		,	0,						DLG_COMBO,	NULL,						0,							0,							6,			CBO_PATTERN_MATCHING |
				                      																																	CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,IDCB_PROC_SZPROCNAME,0,
	IDCK_PROC_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,							0,			CBO_DFT_ODBC_HANDLE,	0,											0,			NULL,						0,0,
	};

CONTROL lptSQLProcedureColumns[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_PC_SZPROCCATALOG,	0,						DLG_COMBO,	NULL,						0,							0,	1,			CBO_STRING_VAL,		IDCB_PC_CBPROCCATALOG, 			0,			NULL,						0,0,
	IDCB_PC_SZPROCSCHEMA	,	0,						DLG_COMBO,	NULL,						0,							0,	3,			CBO_STRING_VAL,		IDCB_PC_CBPROCSCHEMA	, 			0,			NULL,						0,0,
	IDCB_PC_SZPROCNAME	,	0,						DLG_COMBO,	NULL,						0,							0,	5,			CBO_STRING_VAL,		IDCB_PC_CBPROCNAME	, 			0,			NULL,						0,0,
	IDCB_PC_SZCOLUMNNAME	,	0,						DLG_COMBO,	NULL,						0,							0,	7,			CBO_STRING_VAL,		IDCB_PC_CBCOLUMNNAME	, 			0,			NULL,						0,0,
	IDCB_PC_CBPROCCATALOG,	0,						DLG_COMBO,	NULL,						0,							0,	2,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_PC_SZPROCCATALOG,0,
	IDCB_PC_CBPROCSCHEMA	,	0,						DLG_COMBO,	NULL,						0,							0,	4,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_PC_SZPROCSCHEMA,0,
	IDCB_PC_CBPROCNAME	,	0,						DLG_COMBO,	NULL,						0,							0,	6,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_PC_SZPROCNAME,0,
	IDCB_PC_CBCOLUMNNAME	,	0,						DLG_COMBO,	NULL,						0,							0,	8,			CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,										0,			NULL,IDCB_PC_SZCOLUMNNAME,0,
	IDCK_PC_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,	0,			CBO_DFT_ODBC_HANDLE,	0,										0,			NULL,						0,0,
	};


//-------------------------------------------------------------------------------
//  Catalog Menu
//-------------------------------------------------------------------------------

CONTROL lptSQLTables25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	0,		0,						DLG_STATIC,	TEXT("szTable..."),	0,							0,							 -1,		0,							0,				0,			NULL,						0,0,
	501,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,								1,		CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,								3,		CBO_STRING_VAL,		506,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,								5,		CBO_STRING_VAL,		507,			0,			NULL,						0,0,
	504,	0,						DLG_COMBO,	NULL,						idsszTableType,		0,								7,		CBO_STRING_VAL,		508,			0,			NULL,						0,0,
	0,		0,						DLG_STATIC,	TEXT("cbTable..."),	0,							0,							 -1,		0,							0,				0,			NULL,						0,0,
	505,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,								2,		CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,								4,		CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,						0,0,
	507,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,								6,		CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,						0,0,
	508,	0,						DLG_COMBO,	NULL,						idscbTableType,		0,								8,		CBO_PATTERN_MATCHING |
				                      																										CBO_DFT_SHOW_CALLEN,	0,											0,			NULL,						0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								0,		0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLColumns25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szTable..."),	0,							0,							 -1,	0,							0,				0,			NULL,						0,0,
	501,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,								1,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,								3,	CBO_STRING_VAL,		506,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,								5,	CBO_STRING_VAL,		507,			0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("szColumn..."),	0,							0,							 -1,	0,							0,				0,			NULL,						0,0,
	504,	0,						DLG_COMBO,	NULL,						idsszColumnName,		0,								7,	CBO_STRING_VAL,		508,			0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbTable..."),	0,							0,							 -1,	0,							0,				0,			NULL,						0,0,
	505,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,								2,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,								4,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	507,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,								6,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbColumn..."),	0,							0,							 -1,	0,							0,				0,			NULL,						0,0,
	508,	0,						DLG_COMBO,	NULL,						idscbColumnName,		0,								8,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								0,	0,							0,				0,			NULL,							0,0,
	};

CONTROL lptSQLStatistics25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szTable..."),			0,							0,						  -1,	0,							0,				0,			NULL,							0,0,
	501,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,									1,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,									3,	CBO_STRING_VAL,		506,			0,			NULL,							0,0,
	503,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,									5,	CBO_STRING_VAL,		507,			0,			NULL,						0,0,
	504,	0,						DLG_COMBO,	NULL,						0,							0,									7,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbTable..."),			0,							0,						  -1,	0,							0,				0,			NULL,						0,0,
	505,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,									2,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,									4,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	507,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,									6,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	508,	0,						DLG_COMBO,	NULL,						0,							0,									8,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,									0,	0,							0,				0,			NULL,							0,0,
	};

CONTROL lptSQLPrimaryKeys25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_PK25_SZTABLELABEL,	0,						DLG_STATIC,	TEXT("szTable..."),	0,							0,	  -1,	0,							0,								0,			NULL,						0,0,
	IDCB_PK25_QUALIFIER,		0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,		1,	CBO_STRING_VAL,		IDCB_PK25_CBQUALIFIER,	0,			NULL,						0,0,
	IDCB_PK25_OWNER,			0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,		3,	CBO_STRING_VAL,		IDCB_PK25_CBOWNER,		0,			NULL,							0,0,
	IDCB_PK25_NAME,			0,						DLG_COMBO,	NULL,						idsszTableName,		0,		5,	CBO_STRING_VAL,		IDCB_PK25_CBNAME,			0,			NULL,						0,0,
	IDCB_PK25_CBTABLE,		0,						DLG_STATIC,	TEXT("cbTable..."),	0,							0,	  -1,	0,							0,								0,			NULL,						0,0,
	IDCB_PK25_CBQUALIFIER,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,		2,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,								0,			NULL,						0,0,
	IDCB_PK25_CBOWNER,		0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,		4,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,								0,			NULL,						0,0,
	IDCB_PK25_CBNAME,			0,						DLG_COMBO,	NULL,						idscbTableName,		0,		6,	CBO_PATTERN_MATCHING |
																																				CBO_DFT_SHOW_CALLEN,				0,								0,			NULL,						0,0,
	IDCK_PK25_HSTMT,			0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,	CBO_DFT_ODBC_HANDLE,	0,								0,			NULL,						0,0,
	};


CONTROL lptSQLForeignKeys25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szPkTable..."),0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	501,	0,						DLG_COMBO,	NULL,						0,							0,									1,	CBO_STRING_VAL,		504,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						0,							0,									3,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						0,							0,									5,	CBO_STRING_VAL,		506,			0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbPkTable..."),0,							0,								  -1,	0,							0,				0,			NULL,						0,0,
	504,	0,						DLG_COMBO,	NULL,						0,							0,									2,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	505,	0,						DLG_COMBO,	NULL,						0,							0,									4,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						0,							0,									6,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("szFkTable..."),0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	507,	0,						DLG_COMBO,	NULL,						0,							0,									7,	CBO_STRING_VAL,		510,			0,			NULL,							0,0,
	508,	0,						DLG_COMBO,	NULL,						0,							0,									9,	CBO_STRING_VAL,		511,			0,			NULL,							0,0,
	509,	0,						DLG_COMBO,	NULL,						0,							0,								  11,	CBO_STRING_VAL,		512,			0,			NULL,							0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbFkTable..."),0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	510,	0,						DLG_COMBO,	NULL,						0,							0,									8,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	511,	0,						DLG_COMBO,	NULL,						0,							0,								  10,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,					      0,0,
	512,	0,						DLG_COMBO,	NULL,						0,							0,								  12,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								 	0,	0,							0,				0,			NULL,							0,0,
	};

CONTROL lptSQLTablePrivileges25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szTable..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	501,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,									1,	CBO_STRING_VAL,		504,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,									3,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,									5,	CBO_STRING_VAL,		506,			0,			NULL,							0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbTable..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	504,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,									2,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	505,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,									4,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,									6,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,									0,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLColumnPrivileges25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szTable..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	501,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,									1,	CBO_STRING_VAL,		505,			0,			NULL,							0,0,
	502,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,									3,	CBO_STRING_VAL,		506,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,									5,	CBO_STRING_VAL,		507,			0,			NULL,							0,0,
	  0,	0,						DLG_STATIC,	TEXT("szColumn..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	504,	0,						DLG_COMBO,	NULL,						idsszColumnName,		0,									7,	CBO_STRING_VAL,		508,			0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbTable..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	505,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,									2,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,									4,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	507,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,									6,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbColumn..."),	0,							0,								  -1,	0,							0,				0,			NULL,						0,0,
	508,	0,						DLG_COMBO,	NULL,						idscbColumnName,		0,									8,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								 	0,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLSpecialColumns25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	501,	0,						DLG_COMBO,	NULL,						0,							0,									1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("szTable..."),	0,							0,								  -1,	0,							0,				0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						idsszTableQualifier,	0,									2,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						idsszTableOwner,		0,									4,	CBO_STRING_VAL,		506,			0,			NULL,						0,0,
	504,	0,						DLG_COMBO,	NULL,						idsszTableName,		0,									6,	CBO_STRING_VAL,		507,			0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbTable..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	505,	0,						DLG_COMBO,	NULL,						idscbTableQualifier,	0,									3,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	506,	0,						DLG_COMBO,	NULL,						idscbTableOwner,		0,									5,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	507,	0,						DLG_COMBO,	NULL,						idscbTableName,		0,									7,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	508,	0,						DLG_COMBO,	NULL,						0,							0,									8,	CBO_PATTERN_MATCHING,0,				0,			NULL,							0,0,
	509,	0,						DLG_COMBO,	NULL,						0,							0,									9,	CBO_PATTERN_MATCHING,0,				0,			NULL,							0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,									0,	0,							0,				0,			NULL,							0,0,
	};

CONTROL lptSQLProcedures25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szProc..."),	0,							0,								  -1,	0,							0,				0,			NULL,						0,0,
	501,	0,						DLG_COMBO,	NULL,						0,							0,									1,	CBO_STRING_VAL,		504,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						0,							0,									3,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	503,	0,						DLG_COMBO,	NULL,						0,							0,									5,	CBO_STRING_VAL,		506,			0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbProc..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	504,	0,						DLG_COMBO,	NULL,						0,							0,									2,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	505,	0,						DLG_COMBO,	NULL,						0,							0,									4,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						0,							0,									6,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,								 	0,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLProcedureColumns25[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	  0,	0,						DLG_STATIC,	TEXT("szProc..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	501,	0,						DLG_COMBO,	NULL,						0,							0,									1,	CBO_STRING_VAL,		505,			0,			NULL,						0,0,
	502,	0,						DLG_COMBO,	NULL,						0,							0,									3,	CBO_STRING_VAL,		506,			0,			NULL,							0,0,
	503,	0,						DLG_COMBO,	NULL,						0,							0,									5,	CBO_STRING_VAL,		507,			0,			NULL,							0,0,
	  0,	0,						DLG_STATIC,	TEXT("szColumn..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	504,	0,						DLG_COMBO,	NULL,						0,							0,									7,	CBO_STRING_VAL,		508,			0,			NULL,							0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbProc..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	505,	0,						DLG_COMBO,	NULL,						0,							0,									2,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	506,	0,						DLG_COMBO,	NULL,						0,							0,									4,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	507,	0,						DLG_COMBO,	NULL,						0,							0,									6,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,						0,0,
	  0,	0,						DLG_STATIC,	TEXT("cbColumn..."),	0,							0,								  -1,	0,							0,				0,			NULL,							0,0,
	508,	0,						DLG_COMBO,	NULL,						0,							0,									8,	CBO_PATTERN_MATCHING |
																																					CBO_DFT_SHOW_CALLEN,				0,				0,			NULL,							0,0,
	500,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,									0,	0,							0,				0,			NULL,						0,0,
	};



//-------------------------------------------------------------------------------
//  Misc. Menu
//-------------------------------------------------------------------------------

CONTROL lptSQLTransact[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_T_HDBC	,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							1,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,							0,0,
	IDCB_T_FTYPE,	0,						DLG_COMBO,	NULL,						0,							0,							2,			CBO_PATTERN_MATCHING,0,				0,			NULL,							0,0,
	IDCK_T_HENV	,	0,						DLG_CKBOX,	NULL,						idsHenv,					0,							0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,							0,0,
	};


CONTROL lptSQLAllocHandle[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_AH_FHANDLETYPE,	0,						DLG_COMBO,	NULL,						0,							0,							0,			CBO_PATTERN_MATCHING,0,				0,			GetSelectedHandleWndProc,							0,0,
	IDCB_AH_INPUTHANDLE,	0,						DLG_COMBO,	NULL,						idsHInput,				0,							1,			CBO_PATTERN_MATCHING |
																																										CBO_DFT_NULL_HANDLE |
																																										CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	IDCK_AH_OUTPUTHANDLE,0,						DLG_CKBOX,	NULL,						idsHOutput,			   0,							2,			0,							0,				0,			NULL,	 						0,0,
	};

CONTROL lptSQLFreeHandle[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum							lDataStyle				wLengthID	fUpdated	lpUsrWndProc	   wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------							--------------------	-----------	--------	--------------
	IDCB_FH_FHANDLETYPE,	0,						DLG_COMBO,	NULL,						0,							0,							IPAR_FH_FHANDLETYPE,			CBO_PATTERN_MATCHING,0,				0,			GetSelectedHandleWndProc,						0,0,
	IDCB_FH_INPUTHANDLE,	0,						DLG_COMBO,	NULL,						idsHInput,				0,							IPAR_FH_INPUTHANDLE,			CBO_PATTERN_MATCHING |
																																																CBO_DFT_NULL_HANDLE |
																																																CBO_DFT_ODBC_HANDLE,0,				0,			NULL,						0,0,
	};

CONTROL lptSQLBindParam[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_BP_HSTMT					,	0,						DLG_COMBO,	NULL,						idsHstmt,		0,					IPAR_BPARAM_HSTMT,		CBO_PATTERN_MATCHING |
																																														CBO_DFT_NULL_HSTMT |
																																														CBO_DFT_ODBC_HANDLE,	0,					0,			NULL,						0,0,
	IDE_BP_IPAR			,				0,						DLG_EDIT,	NULL,						idsipar,			0,					IPAR_BPARAM_IPAR,			0,							0,					0,			BindParamWndProc,0,0,
	IDCB_BP_FCTYPE		,				0,						DLG_COMBO,	NULL,						idsfCType,		0,					IPAR_BPARAM_FCTYPE,		CBO_PATTERN_MATCHING,0,					0,			BindParamWndProc,0,0,
	IDCB_BP_FSQLTYPE	,				0,						DLG_COMBO,	NULL,						idsfSqlType,	0,					IPAR_BPARAM_FSQLTYPE,	CBO_PATTERN_MATCHING,0,					0,			BindParamWndProc,0,0,
	IDE_BP_CBCOLDEF	,				0,						DLG_EDIT,	NULL,						idscbColDef,	0,					IPAR_BPARAM_CBCOLDEF,	0,							0,					0,			NULL,						0,0,
	IDE_BP_IBSCALE		,				0,						DLG_EDIT,	NULL,						idsibScale,		0,					IPAR_BPARAM_IBSCALE,		0,							0,					0,			NULL,						0,0,
	IDCB_BP_RGBVALUE	,				0,						DLG_COMBO,	NULL,						idsrgbValue,	0,					IPAR_BPARAM_RGBVALUE,	CBO_CALCULATED_LEN |
																																														CBO_VALIDATE_DATA   ,IDCB_BP_PCBVALUE,0,		BindParamWndProc,0,0,
	IDCB_BP_PCBVALUE	,				0,						DLG_COMBO,	NULL,						idspcbValue,	0,					IPAR_BPARAM_PCBVALUE,	CBO_DFT_SHOW_CALLEN	|
																																														CBO_PATTERN_MATCHING,0,					0,			BindParamWndProc,IDCB_BP_RGBVALUE,0,
	IDCK_BP_SQL_LEN_DATA_AT_EXEC,	0,						DLG_CKBOX,	NULL,						0,					0,					IPAR_BPARAM_DAE,			0,							0,					0,			BindParamWndProc,0,0,
	IDCB_BP_ROWSET_HDBC			,	0,						DLG_COMBO,	NULL,						0,					0,					IPAR_BPARAM_RS_HDBC,		CBO_PATTERN_MATCHING,0,					0,			BindParamWndProc,0,0,
	IDCB_BP_ROWSET_HSTMT			,	0,						DLG_COMBO,	NULL,						0,					0,					IPAR_BPARAM_RS_HSTMT,	CBO_PATTERN_MATCHING,0,					0,			BindParamWndProc,0,0,
	IDE_BP_COLUMN					,	0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BPARAM_COLUMN,		0,							0,					0,			BindParamWndProc,0,0,
	IDE_BP_ROW						,	0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BPARAM_ROW,			0,							0,					0,			BindParamWndProc,0,0,
	IDE_BP_BUFFER_SIZE			,	0,						DLG_EDIT,	NULL,						0,					0,					IPAR_BPARAM_BUFFERSIZE,	0,							0,					0,			NULL,						0,0,
	};

CONTROL lptSQLCloseCursor[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_CC_HSTMT,	0,						DLG_COMBO,	NULL,						idsHstmt,				0,				IPAR_CC_HSTMT,			CBO_PATTERN_MATCHING |
																																									CBO_DFT_NULL_HSTMT |
																																									CBO_DFT_ODBC_HANDLE,			0,				0,			NULL,						0,0,
	};

CONTROL lptSQLEndTran[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_ET_FHANDLETYPE,		0,						DLG_COMBO,	NULL,						idsfHandleType,		0,	IPAR_ET_FHANDLETYPE,				CBO_PATTERN_MATCHING,0,				0,			GetSelectedHandleWndProc,						0,0,
	IDCB_ET_HHANDLE	,		0,						DLG_COMBO,	NULL,						idshHandle,				0,	IPAR_ET_HHANDLE	,				CBO_PATTERN_MATCHING |
																																												CBO_DFT_NULL_HANDLE |
																																												CBO_DFT_ODBC_HANDLE,0,				0,			NULL,						0,0,
	IDCB_ET_FCOMPLETIONTYPE,0,						DLG_COMBO,	NULL,						idsfCompletionType, 	0,	IPAR_ET_FCOMPLETIONTYPE,		CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetEnvAttr[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle				iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------		--------	--------------------	-----------	--------	--------------
	IDCB_GEA_HENV		,			0,						DLG_COMBO,	NULL,						idsHenv,					0,				IPAR_GEA_HENV,					CBO_PATTERN_MATCHING |
																																															CBO_DFT_NULL_HENV |
																																															CBO_DFT_ODBC_HANDLE,0,				0,			NULL,	 										0,0,
	IDCB_GEA_FATTRIBUTE,			0,						DLG_COMBO,	NULL,						0,							0,				IPAR_GEA_FATTRIBUTE,			CBO_LINKED_FIELD,		IDCK_GEA_RGBVALUE,	0,			HandleIsPointerCbWndProc,				0,0,
	IDCK_GEA_RGBVALUE	,			0,						DLG_CKBOX,	NULL,						idsrgbValue,			0,				IPAR_GEA_RGBVALUE	,			0,							0,							0,			NULL,								0,0,
	IDE_GEA_CBVALUE	,			0,						DLG_EDIT,	NULL,						idscbValueMax,			0,				IPAR_GEA_CBVALUE	,			0,							0,							0,			NULL,								0,0,
	IDCK_GEA_PCBVALUE	,			0,						DLG_CKBOX,	NULL,						idspcbValue,			0,				IPAR_GEA_PCBVALUE	,			0,							0,							0,			HandleIsPointerCbWndProc,	IDCK_GEA_RGBVALUE,0,
	IDCB_GEA_FATTRIBUTE_TYPE,	0,						DLG_COMBO,	NULL,						0,							0,				IPAR_GEA_FATTRIBUTE_TYPE,	CBO_PATTERN_MATCHING,0,							0,			HandleIsPointerCbWndProc,	0,0,
	};

CONTROL lptSQLSetEnvAttr[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_SEA_HENV		,			0,			DLG_COMBO,	NULL,						idsHenv,					0,				IPAR_SEA_HENV,			CBO_PATTERN_MATCHING |
																																										CBO_DFT_NULL_HENV |
																																										CBO_DFT_ODBC_HANDLE,			0,							0,			NULL,							0,0,
	IDCB_SEA_FATTRIBUTE,			0,			DLG_COMBO,	NULL,						0,							0,		IPAR_SEA_FATTRIBUTE,			CBO_PATTERN_MATCHING |
	                   																										                   			CBO_LINKED_FIELD,				IDCB_SEA_RGBVALUE,	0,			HandleIsPointerCbWndProc,0,0,
	IDCB_SEA_RGBVALUE	,			0,			DLG_COMBO,	NULL,						idsrgbValue,			0,		IPAR_SEA_RGBVALUE	,			CBO_PATTERN_MATCHING |
																																										CBO_CALCULATED_LEN,			IDE_SEA_CBVALUE,		0,			NULL,							0,0,
	IDE_SEA_CBVALUE	,			0,			DLG_COMBO,	NULL,						idscbValueMax,			0,		IPAR_SEA_CBVALUE	,			CBO_DFT_SHOW_CALLEN |
																																										CBO_PATTERN_MATCHING,		0,							0,			HandleIsPointerCbWndProc,IDCB_SEA_RGBVALUE,0,
	IDCB_SEA_FATTRIBUTE_TYPE,	0,			DLG_COMBO,	NULL,						0,							0,		IPAR_SEA_FATTRIBUTE_TYPE,	CBO_PATTERN_MATCHING,		0,							0,			HandleIsPointerCbWndProc,0,0,
	};

CONTROL lptSQLGetConnectAttr[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle					iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------			--------	--------------------	-----------	--------	--------------
	IDCB_GCA_HDBC		,			0,						DLG_COMBO,	NULL,						idsHdbc,					0,		IPAR_GCA_HDBC,					CBO_PATTERN_MATCHING |
																																													CBO_DFT_NULL_HDBC |
																																													CBO_DFT_ODBC_HANDLE,0,				0,			NULL,						0,0,
	IDCB_GCA_FATTRIBUTE		,	0,						DLG_COMBO,	NULL,						0,							0,		IPAR_GCA_FATTRIBUTE		,	CBO_PATTERN_MATCHING,0,				0,			SetConAtrWndProc,		0,0,
	IDCK_GCA_RGBVALUE			,	0,						DLG_CKBOX,	NULL,						idsrgbValue,			0,		IPAR_GCA_RGBVALUE			,	0,							0,				0,			NULL,						0,0,
	IDE_GCA_CBVALUE			,	0,						DLG_EDIT,	NULL,						idscbValueMax,			0,		IPAR_GCA_CBVALUE			,	0,							0,				0,			NULL,						0,0,
	IDCK_GCA_PCBVALUE			,	0,						DLG_CKBOX,	NULL,						idspcbValue,			0,		IPAR_GCA_PCBVALUE			,	0,							0,				0,			NULL,						0,0,
	IDCB_GCA_PARAMETER_TYPE	,	0,						DLG_COMBO,	NULL,						0,							0,		IPAR_GCA_PARAMETER_TYPE	,	CBO_PATTERN_MATCHING,0,				0,			SetConAtrWndProc,		0,0,
	};


CONTROL lptSQLSetConnectAttr[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle					iParmNum	lDataStyle						wLengthID			fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------			--------	--------------------			-----------			--------	--------------
	IDCB_SCA_HDBC		,			0,						DLG_COMBO,	NULL,						idsHdbc,					0,									IPAR_SCA_HDBC,							CBO_PATTERN_MATCHING |
																																																						CBO_DFT_NULL_HDBC |
																																																						CBO_DFT_ODBC_HANDLE,		0,							0,			NULL,										0,0,
	IDCB_SCA_FATTRIBUTE		,	0,						DLG_COMBO,	NULL,						0,							0,									IPAR_SCA_FATTRIBUTE		,			CBO_PATTERN_MATCHING |
	                         																																		                         			CBO_LINKED_FIELD,			IDCB_SCA_RGBVALUE,	0,			SetConAtrWndProc,						0,0,
	IDCB_SCA_RGBVALUE			,	0,						DLG_COMBO,	NULL,						0,							0,									IPAR_SCA_RGBVALUE			,			CBO_PATTERN_MATCHING |
	                         																																		                         			CBO_CALCULATED_LEN,		IDCB_SCA_CBVALUE,		0,			NULL,										0,0,
	IDCB_SCA_CBVALUE			,	0,						DLG_COMBO,	NULL,						idscbValueMax,			0,									IPAR_SCA_CBVALUE			,			CBO_DFT_SHOW_CALLEN |
																																																						CBO_PATTERN_MATCHING,	0,							0,			SetConAtrWndProc,						IDCB_SCA_RGBVALUE,0,
	IDCB_SCA_PARAMETER_TYPE	,	0,						DLG_COMBO,	NULL,						0,							0,									IPAR_SCA_PARAMETER_TYPE	,			CBO_PATTERN_MATCHING,	0,							0,			SetConAtrWndProc,						0,0,
	};


CONTROL lptSQLSetStmtAttr[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle					iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------			--------	--------------------	-----------	--------	--------------
	IDCB_SSA_HSTMT		,			0,						DLG_COMBO,	NULL,						idsHstmt,					0,	IPAR_SSA_HSTMT,			CBO_PATTERN_MATCHING  |
																																												CBO_DFT_NULL_HSTMT |
																																												CBO_DFT_ODBC_HANDLE,			0,				0,						SetConAtrWndProc,						0,0,
	IDCB_SSA_FATTRIBUTE		,	0,						DLG_COMBO,	NULL,						0,							0,		IPAR_SSA_FATTRIBUTE,		CBO_PATTERN_MATCHING |
	                         																											                        CBO_LINKED_FIELD,				IDCB_SSA_RGBVALUE,	0,			SetConAtrWndProc,						0,0,
	IDCB_SSA_RGBVALUE			,	0,						DLG_COMBO,	NULL,						idsrgbValue,			0,		IPAR_SSA_RGBVALUE,		CBO_PATTERN_MATCHING |
	                         																											                     	CBO_CALCULATED_LEN,			IDE_SSA_CBVALUE,		0,			SetConAtrWndProc,						0,0,
	IDE_SSA_CBVALUE			,	0,						DLG_COMBO,	NULL,						idscbValue,				0,		IPAR_SSA_CBVALUE,			CBO_PATTERN_MATCHING	|
																																												CBO_PATTERN_MATCHING,		0,							0,			SetConAtrWndProc,						IDCB_SSA_RGBVALUE,0,
	IDCB_SSA_PARAMETER_TYPE	,	0,						DLG_COMBO,	NULL,						0,							0,		IPAR_SSA_PARAMETER_TYPE,CBO_PATTERN_MATCHING,		0,							0,			SetConAtrWndProc,						0,0,
	};


CONTROL lptSQLGetStmtAttr[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle					iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------			--------	--------------------	-----------	--------	--------------
	IDCB_GSA_HSTMT		,			0,						DLG_COMBO,	NULL,						idsHstmt,					0,	IPAR_GSA_HSTMT,			CBO_PATTERN_MATCHING |
																																												CBO_DFT_NULL_HSTMT |
																																												CBO_DFT_ODBC_HANDLE,			0,				0,			NULL,						0,0,
	IDCB_GSA_FATTRIBUTE		,	0,						DLG_COMBO,	NULL,						0,							0,		IPAR_GSA_FATTRIBUTE,		CBO_PATTERN_MATCHING,IDCK_GSA_RGBVALUE,0,			SetConAtrWndProc,		0,0,
	IDCK_GSA_RGBVALUE			,	0,						DLG_CKBOX,	NULL,						idsrgbValue,			0,		IPAR_GSA_RGBVALUE,		0,							0,						0,			NULL,						0,0,
	IDE_GSA_CBVALUE			,	0,						DLG_EDIT,	NULL,						idscbValueMax,			0,		IPAR_GSA_CBVALUE,			0,							0,						0,			NULL,						0,0,
	IDCK_GSA_PCBVALUE			,	0,						DLG_CKBOX,	NULL,						idspcbValue,			0,		IPAR_GSA_PCBVALUE,		0,							0,						0,			NULL,IDCK_GSA_RGBVALUE,0,
	IDCB_GSA_PARAMETER_TYPE	,	0,						DLG_COMBO,	NULL,						0,							0,		IPAR_GSA_PARAMETER_TYPE,CBO_PATTERN_MATCHING,0,						0,			SetConAtrWndProc,		0,0,
	};

CONTROL lptSQLCopyDesc[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum		lDataStyle						wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------		--------------------			-----------	--------	--------------
	IDCB_CD_HSRCDESC,		0,						DLG_COMBO,	NULL,						idshDesc,				0,			0,			CBO_PATTERN_MATCHING |
																																						CBO_DFT_NULL_HDESC |
																																						CBO_DFT_ODBC_HANDLE,			0,				0,			NULL,						0,0,
	IDCB_CD_HDESTDESC,	0,						DLG_COMBO,	NULL,						idshDesc,				0,			1,			CBO_PATTERN_MATCHING |
																																						CBO_DFT_NULL_HDESC |
																																						CBO_DFT_ODBC_HANDLE,			0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetDescField[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle					iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------			--------	--------------------	-----------	--------	--------------
	IDCB_GDF_HDESC,				0,						DLG_COMBO,	NULL,						idshDesc,				0,		IPAR_GDF_HDESC,					CBO_PATTERN_MATCHING |
																																														CBO_DFT_NULL_HDESC |
																																														CBO_DFT_ODBC_HANDLE,		0,				0,			NULL,								0,0,
	IDE_GDF_IRECNUM			,	0,						DLG_EDIT,	NULL,						idsRecNumber,			0,		IPAR_GDF_IRECNUM			,		0,								0,				0,			NULL,								0,0,
	IDCB_GDF_IFIELDIDENTIFIER,	0,						DLG_COMBO,	NULL,						idsFieldIdentifier,	0,		IPAR_GDF_IFIELDIDENTIFIER,		CBO_PATTERN_MATCHING |
																																														CBO_LINKED_FIELD,			IDCK_GDF_RGBVALUE,0,	HandleIsPointerCbWndProc,	0,0,
	IDCK_GDF_RGBVALUE			,	0,						DLG_CKBOX,	NULL,						idsValue,				0,		IPAR_GDF_RGBVALUE			,		0,								0,				0,			NULL,								0,0,
	IDE_GDF_CBVALUEMAX		,	0,						DLG_EDIT,	NULL,						idsBufferLength,		0,		IPAR_GDF_CBVALUEMAX		,		CBO_PATTERN_MATCHING,	0,				0,			NULL,								0,0,
	IDCK_GDF_PCBVALUE			,	0,						DLG_CKBOX,	NULL,						idsStringLength,		0,		IPAR_GDF_PCBVALUE			,		0,								0,				0,			HandleIsPointerCbWndProc,	IDCK_GDF_RGBVALUE,0,
	IDCB_GDF_IFIELDIDENTIFIER_TYPE,	0,				DLG_COMBO,	NULL,						0,							0,		IPAR_GDF_IFIELDIDENTIFIER_TYPE,CBO_PATTERN_MATCHING,	0,				0,			HandleIsPointerCbWndProc,	0,0,
	};


CONTROL lptSQLSetDescField[] = {
//	wID								lStyle				type			szText					uDescID					uFormatStyle					iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----								-----------------	-----------	--------------------	--------------------	--------------------			--------	--------------------	-----------	--------	--------------
	IDCB_SDF_HDESC,				0,						DLG_COMBO,	NULL,						idshDesc,				0,		IPAR_SDF_HDESC,					CBO_PATTERN_MATCHING|
																																														CBO_DFT_NULL_HDESC |
																																														CBO_DFT_ODBC_HANDLE,		0,									0,			NULL,								0,0,
	IDE_SDF_IRECNUM			,	0,						DLG_EDIT,	NULL,						idsRecNumber,			0,		IPAR_SDF_IRECNUM			,		0,								0,									0,			NULL,								0,0,
	IDCB_SDF_IFIELDIDENTIFIER,	0,						DLG_COMBO,	NULL,						idsFieldIdentifier,	0,		IPAR_SDF_IFIELDIDENTIFIER,		CBO_PATTERN_MATCHING |
																																														CBO_LINKED_FIELD,			IDCB_SDF_RGBVALUE,			0,			HandleIsPointerCbWndProc,	0,0,
	IDCB_SDF_RGBVALUE			,	0,						DLG_COMBO,	NULL,						idsValue,				0,		IPAR_SDF_RGBVALUE			,		CBO_PATTERN_MATCHING |
																																														CBO_CALCULATED_LEN,		IDE_SDF_CBBUFFERLENGTH,		0,			NULL,								0,0,
	IDE_SDF_CBBUFFERLENGTH	,	0,						DLG_COMBO,	NULL,						idsBufferLength,		0,		IPAR_SDF_CBBUFFERLENGTH	,		CBO_PATTERN_MATCHING,	0,									0,			HandleIsPointerCbWndProc,	IDCB_SDF_RGBVALUE,0,
	IDCB_SDF_IFIELDIDENTIFIER_TYPE,	0,				DLG_COMBO,	NULL,						0,							0,		IPAR_SDF_IFIELDIDENTIFIER_TYPE,CBO_PATTERN_MATCHING,	0,									0,			HandleIsPointerCbWndProc,	0,0,
	};

CONTROL lptSQLGetDescRec[] = {
//	wID						lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----						-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_GDR_HDESC,		0,						DLG_COMBO,	NULL,						idshDesc,				0,		IPAR_GDR_HDESC,			CBO_PATTERN_MATCHING|
																																										CBO_DFT_NULL_HDESC |
																																										CBO_DFT_ODBC_HANDLE,0,						0,		ListDescHandlesWndProc,	0,0,
	IDE_GDR_IRECNUM	,	0,						DLG_EDIT,	NULL,						idsRecNumber,			0,		IPAR_GDR_IRECNUM	,		0,							0,						0,			NULL,						0,0,
	IDCK_GDR_SZNAME	,	0,						DLG_CKBOX,	NULL,						idsName,					0,		IPAR_GDR_SZNAME	,		CBO_STRING_VAL,		IDCK_GDR_PCBNAME,	0,			NULL,						0,0,
	IDE_GDR_CBNAME		,	0,						DLG_EDIT,	NULL,						idsBufferLength,		0,		IPAR_GDR_CBNAME	,		0,							0,						0,			NULL,						0,0,
	IDCK_GDR_PCBNAME	,	0,						DLG_CKBOX,	NULL,						idsStringLength,		0,		IPAR_GDR_PCBNAME	,		0,							0,						0,			NULL,	IDCK_GDR_SZNAME,0,
	IDCK_GDR_FTYPE		,	0,						DLG_CKBOX,	NULL,						idsType,					0,		IPAR_GDR_FTYPE		,		CBO_PATTERN_MATCHING,0,						0,			NULL,						0,0,
	IDCK_GDR_FSUBTYPE	,	0,						DLG_CKBOX,	NULL,						idsSubType,				0,		IPAR_GDR_FSUBTYPE	,		0,							0,						0,			NULL,						0,0,
	IDCK_GDR_PCOLDEF	,	0,						DLG_CKBOX,	NULL,						idsLength,				0,		IPAR_GDR_PCOLDEF	,		0,							0,						0,			NULL,						0,0,
	IDCK_GDR_PPRECISION,	0,						DLG_CKBOX,	NULL,						idsPrecision,			0,		IPAR_GDR_PPRECISION,		0,							0,						0,			NULL,						0,0,
	IDCK_GDR_PSCALE	,	0,						DLG_CKBOX,	NULL,						idsScale,				0,		IPAR_GDR_PSCALE	,		0,							0,						0,			NULL,						0,0,
	IDCK_GDR_PFNULLABLE,	0,						DLG_CKBOX,	NULL,						idsNullable,			0,		IPAR_GDR_PFNULLABLE,		0,							0,						0,			NULL,						0,0,
	};


CONTROL lptSQLSetDescRec[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_SDR_HDESC,				0,			DLG_COMBO,	NULL,						idshDesc,				0,			IPAR_SDR_HDESC,					CBO_PATTERN_MATCHING|
																																												CBO_DFT_NULL_HDESC |
																																												CBO_DFT_ODBC_HANDLE,	0,								0,			NULL,						0,0,
	IDE_SDR_IRECNUM,				0,			DLG_EDIT,	NULL,						idsRecNumber,			0,			IPAR_SDR_IRECNUM,					0,							0,								0,			NULL,						0,0,
	IDCB_SDR_FTYPE	,				0,			DLG_COMBO,	NULL,						idsType,					0,			IPAR_SDR_FTYPE	,					CBO_PATTERN_MATCHING,0,								0,			SetCTypeWndProc,		0,0,
	IDCB_SDR_FSUBTYPE,			0,			DLG_COMBO,	NULL,						idsSubType,				0,			IPAR_SDR_FSUBTYPE,				CBO_PATTERN_MATCHING,0,								0,			NULL,						0,0,
	IDE_SDR_CBLENGTH,				0,			DLG_EDIT,	NULL,						idsLength,				0,			IPAR_SDR_CBLENGTH,				0,							0,								0,			NULL,						0,0,
	IDE_SDR_CBCOLDEF,				0,			DLG_EDIT,	NULL,						idsPrecision,			0,			IPAR_SDR_CBCOLDEF,				0,							0,								0,			NULL,						0,0,
	IDE_SDR_IBSCALE,				0,			DLG_EDIT,	NULL,						idsScale,				0,			IPAR_SDR_IBSCALE,					0,							0,								0,			NULL,						0,0,
	IDCB_SDR_RGBVALUE,			0,			DLG_COMBO,	NULL,						idsrgbValue,			0,			IPAR_SDR_RGBVALUE,				CBO_CALCULATED_LEN,	IDCB_SDR_PCBSTRINGLENGTH,0,		NULL,						0,0,
	IDCB_SDR_PCBSTRINGLENGTH,	0,			DLG_COMBO,	NULL,						idsStringLength,		0,			IPAR_SDR_PCBSTRINGLENGTH,		CBO_PATTERN_MATCHING |
																																												CBO_DFT_SHOW_CALLEN,	0,								0,			NULL,	IDCB_SDR_RGBVALUE,0,
	IDCK_SDR_PFINDICATOR,		0,			DLG_COMBO,	NULL,						idsIndicator,			0,			IPAR_SDR_PFINDICATOR,			CBO_PATTERN_MATCHING |
																																												CBO_DFT_SHOW_CALLEN,	0,								0,			NULL,						0,0,
	};

CONTROL lptSQLGetDiagField[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle						iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------				--------	--------------------	-----------	--------	--------------
	IDCB_GDF_FHANDLETYPE	,			0,						DLG_COMBO,	NULL,						idsfHandleType,		0,		IPAR_GDF_FHANDLETYPE	,				CBO_PATTERN_MATCHING,0,				0,			GetSelectedHandleWndProc,	0,0,
	IDCB_GDF_HHANDLE		,			0,						DLG_COMBO,	NULL,						idsHInput,				0,		IPAR_GDF_HHANDLE		,				CBO_PATTERN_MATCHING|
																																																CBO_DFT_NULL_HANDLE |
																																																CBO_DFT_ODBC_HANDLE,0,				0,			NULL,								0,0,
	IDE_GDF_IRECRECNUMBER		,	0,						DLG_EDIT,	NULL,						idsiRecord,				0,		IPAR_GDF_IRECRECNUMBER		,		0,							0,				0,			NULL,								0,0,
	IDCB_GDF_FDIAGIDENTIFIER	,	0,						DLG_COMBO,	NULL,						idsfDiagIdentifier,	0,		IPAR_GDF_FDIAGIDENTIFIER	,		CBO_PATTERN_MATCHING,0,				0,			GetDiagFieldWndProc,			0,0,
	IDCK_GDF_RGBDIAGINFO			,	0,						DLG_CKBOX,	NULL,						idsrgbDiagInfo,		0,		IPAR_GDF_RGBDIAGINFO			,		0,							0,				0,			GetDiagFieldWndProc,			0,0,
	IDE_GDF_CBBUFFERLENGTH		,	0,						DLG_EDIT,	NULL,						idscbBufferLength,	0,		IPAR_GDF_CBBUFFERLENGTH		,		0,							0,				0,			NULL,								0,0,
	IDCK_GDF_PCBSTRINGLENGTH	,	0,						DLG_CKBOX,	NULL,						idspcbStringLength,	0,		IPAR_GDF_PCBSTRINGLENGTH	,		0,							0,				0,			NULL,		IDCK_GDF_RGBDIAGINFO,0,
	IDCB_GDF_FDIAGIDENTIFIER_TYPE,0,						DLG_COMBO,	NULL,						0,							0,		IPAR_GDF_FDIAGIDENTIFIER_TYPE,	CBO_PATTERN_MATCHING,0,				0,			GetDiagFieldWndProc,			0,0,
	};

CONTROL lptSQLGetDiagRec[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_GDR_FHANDLETYPE	,	0,						DLG_COMBO,	NULL,						idsfHandleType,		0,	IPAR_GDR_FHANDLETYPE	,			CBO_PATTERN_MATCHING,0,				0,			GetSelectedHandleWndProc,	0,0,
	IDCB_GDR_HHANDLE		,	0,						DLG_COMBO,	NULL,						idsHInput,				0,	IPAR_GDR_HHANDLE		,			CBO_PATTERN_MATCHING|
																																												CBO_DFT_NULL_HANDLE |
																																												CBO_DFT_ODBC_HANDLE,0,				0,			NULL,								0,0,
	IDE_GDR_IRECRECNUMBER,	0,						DLG_EDIT,	NULL,						idsiRecord,				0,	IPAR_GDR_IRECRECNUMBER,			0,							0,				0,			NULL,								0,0,
	IDCK_GDR_SZSQLSTATE	,	0,						DLG_CKBOX,	NULL,						idsszSqlState,			0,	IPAR_GDR_SZSQLSTATE	,			0,							0,				0,			NULL,								0,0,
	IDCK_GDR_PFNATIVEERROR,	0,						DLG_CKBOX,	NULL,						idspfNativeError,		0,	IPAR_GDR_PFNATIVEERROR,			0,							0,				0,			NULL,								0,0,
	IDCK_GDR_SZMESSAGETEXT,	0,						DLG_CKBOX,	NULL,						idsszMessageText,		0,	IPAR_GDR_SZMESSAGETEXT,			0,							0,				0,			NULL,								0,0,
	IDE_GDR_CBBUFFERLENGTH,	0,						DLG_EDIT,	NULL,						idscbBufferLength,	0,	IPAR_GDR_CBBUFFERLENGTH,		0,							0,				0,			NULL,								0,0,
	IDCK_GDR_PCBTEXTLENGTH,	0,						DLG_CKBOX,	NULL,						idspcbTextLength,		0,	IPAR_GDR_PCBTEXTLENGTH,			0,							0,				0,			NULL,IDCK_GDR_SZMESSAGETEXT,0,
	};

#if ODBCVER >= 0x350

CONTROL lptSQLLocator[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_L_HLOCATOR	,	0,						DLG_COMBO,	NULL,						idshLocator,			0,				1,	0,							0,				0,			NULL,						0,0,
	IDCB_L_FOPERATION	,	0,						DLG_COMBO,	NULL,						idsfOperation,			0,				2,	0,							0,				0,			NULL,						0,0,
	IDCK_L_HSTMT		,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,				0,	CBO_DFT_ODBC_HANDLE |
																																					CBO_DFT_NULL_HSTMT,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetLength[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	-------------------	-----------	--------	--------------
	IDCK_GL_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE |
																																					CBO_DFT_NULL_HSTMT,	0,				0,			NULL,						0,0,
	IDCB_GL_FLOCATORTYPE	,	0,						DLG_COMBO,	NULL,						idsfLocatorType,		0,			1,	0,							0,				0,			NULL,						0,0,
	IDCB_GL_HLOCATOR		,	0,						DLG_COMBO,	NULL,						idshLocator,			0,			2,	0,							0,				0,			NULL,						0,0,
	IDCK_GL_PCBVALUE		,	0,						DLG_CKBOX,	NULL,						idspcbValue,			0,			3,	0,							0,				0,			NULL,						0,0,
	IDCK_GL_PFINDICATOR	,	0,						DLG_CKBOX,	NULL,						idspfIndicator,		0,			4,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetPosition[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc    wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_GP_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE |
																																					CBO_DFT_NULL_HSTMT,	0,				0,			NULL,						0,0,
	IDCB_GP_FLOCATORTYPE	,	0,						DLG_COMBO,	NULL,						idsfLocatorType,		0,			1,	0,							0,				0,			NULL,						0,0,
	IDCB_GP_HSOURCELOCATOR,	0,						DLG_COMBO,	NULL,						idshSourceLocator,	0,			2,	0,							0,				0,			NULL,						0,0,
	IDCB_GP_HTARGETLOCATOR,	0,						DLG_COMBO,	NULL,						idshTargetLocator,	0,			3,	0,							0,				0,			NULL,						0,0,
	IDCB_GP_SZSEARCHSTRING,	0,						DLG_COMBO,	NULL,						idsszSearchString,	0,			4,	0,							0,				0,			NULL,						0,0,
	IDE_GP_CBSEARCHSTRING,	0,						DLG_EDIT,	NULL,						idscbSearchString,	0,			5,	0,							0,				0,			NULL,						0,0,
	IDE_GP_IFROMPOSITION	,	0,						DLG_EDIT,	NULL,						idsiFromPosition,		0,			6,	0,							0,				0,			NULL,						0,0,
	IDCK_GP_PIBLOCATEDAT	,	0,						DLG_CKBOX,	NULL,						idspibLocatedAt,		0,			7,	0,							0,				0,			NULL,						0,0,
	IDCK_GP_PFINDICATOR	,	0,						DLG_CKBOX,	NULL,						idspfIndicator,		0,			8,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetSubString[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle				iParmNum	lDataStyle				wLengthID	fUpdated	 lpUsrWndProc wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------		--------	--------------------	-----------	--------	--------------
	IDCK_GSS_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,		0,	CBO_DFT_ODBC_HANDLE |
																																				CBO_DFT_NULL_HSTMT,	0,				0,			NULL,						0,0,
	IDCB_GSS_FLOCATORTYPE,	0,						DLG_COMBO,	NULL,						idsfLocatorType,		0,		1,	0,							0,				0,			NULL,						0,0,
	IDCB_GSS_HSOURCELOCATOR,0,						DLG_COMBO,	NULL,						idshSourceLocator,	0,		2,	0,							0,				0,			NULL,						0,0,
	IDE_GSS_IFROMPOSITION,	0,						DLG_EDIT,	NULL,						idsiFromPosition,		0,		3,	0,							0,				0,			NULL,						0,0,
	IDE_GSS_CBFORLENGTH	,	0,						DLG_EDIT,	NULL,						idscbForLength,		0,		4,	0,							0,				0,			NULL,						0,0,
	IDCB_GSS_FTARGETTYPE	,	0,						DLG_COMBO,	NULL,						idsfTargetType,		0,		5,	0,							0,				0,			NULL,						0,0,
	IDCB_GSS_RGBVALUE		,	0,						DLG_COMBO,	NULL,						idsrgbValue,			0,		6,	CBO_CALCULATED_LEN |
	                         																											CBO_DFT_NULL,			0,				0,			NULL,						0,0,
	IDE_GSS_CBVALUEMAX	,	0,						DLG_EDIT,	NULL,						idscbValueMax,			0,		7,	0,							0,				0,			NULL,						0,0,
	IDCK_GSS_PCBVALUE		,	0,						DLG_CKBOX,	NULL,						idspcbValue,			0,		8,	0,							0,				0,			NULL,						0,0,
	IDCK_GSS_PFINDICATOR	,	0,						DLG_CKBOX,	NULL,						idspfIndicator,		0,		9,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLLocatedUpdate[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle				iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	 wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------		--------	--------------------	-----------	--------	--------------
	IDCK_LU_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,			0,	CBO_DFT_ODBC_HANDLE |
																																					CBO_DFT_NULL_HSTMT,	0,				0,			NULL,						0,0,
	IDCB_LU_FLOCATORTYPE	,	0,						DLG_COMBO,	NULL,						idsfLocatorType,		0,			1,	0,							0,				0,			NULL,						0,0,
	IDCB_LU_HTARGETLOCATOR,	0,						DLG_COMBO,	NULL,						idshTargetLocator,	0,			2,	0,							0,				0,			NULL,						0,0,
	IDE_LU_IFROMPOSITION	,	0,						DLG_EDIT,	NULL,						idsiFromPosition,		0,			3,	0,							0,				0,			NULL,						0,0,
	IDE_LU_CBFORLENGTH	,	0,						DLG_EDIT,	NULL,						idscbForLength,		0,			4,	0,							0,				0,			NULL,						0,0,
	IDCB_LU_FSOURCETYPE	,	0,						DLG_COMBO,	NULL,						idsfSourceType,		0,			5,	0,							0,				0,			NULL,						0,0,
	IDCB_LU_RGBVALUE		,	0,						DLG_COMBO,	NULL,						idsrgbValue,			0,			6,	CBO_CALCULATED_LEN |
	                      																													CBO_DFT_NULL,			0,				0,			NULL,						0,0,
	IDE_LU_CBVALUE			,	0,						DLG_EDIT,	NULL,						idscbValue,				0,			7,	0,							0,				0,			NULL,						0,0,
	IDCK_LU_PFINDICATOR	,	0,						DLG_CKBOX,	NULL,						idspfIndicator,		0,			8,	0,							0,				0,			NULL,						0,0,
	};

#endif

//@INSTALLER
CONTROL lptSQLRemoveDriver[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	 wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_RD_SZDRIVER	,	0,						DLG_COMBO,	NULL,						idslpszDrivers,		0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_RD_FREQUEST	,	0,						DLG_COMBO,	NULL,						idsfRemoveDSN,			0,			1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCK_RD_DWUSAGECOUNT,0,						DLG_CKBOX,	NULL,						idsdwUsageCount,		0,			2,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLRemoveDriverManager[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_RDM_DWUSAGECOUNT,0,					DLG_CKBOX,	NULL,						idsdwUsageCount,		0,		0,			0,							0,				0,			NULL,	 						0,0,
	};

CONTROL lptSQLInstallTranslator[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	 wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_IT_SZINFFILE		,	0,						DLG_COMBO,	NULL,						idslpszInfFile,		0,		0,	CBO_CALCULATED_LEN,	0,				0,			NULL,								0,0,
	IDCB_IT_SZTRANSLATOR	,	0,						DLG_COMBO,	NULL,						idsszTranslator,		0,		1,	CBO_CALCULATED_LEN,	0,				0,			NULL,							  	0,0,
	IDCB_IT_SZPATHIN		,	0,						DLG_COMBO,	NULL,						idsszPathIn,			0,		2,	CBO_CALCULATED_LEN,	0,				0,			NULL,								0,0,
	IDCK_IT_SZPATHOUT		,	0,						DLG_CKBOX,	NULL,						idsszPathOut,			0,		3,	0,							0,				0,			NULL,								0,0,
	IDE_IT_CBPATHOUTMAX	,	0,						DLG_EDIT,	NULL,						idscbPathOutMax,		0,		4,	0,							0,				0,			NULL,								0,0,
	IDCK_IT_PCBPATHOUT	,	0,						DLG_CKBOX,	NULL,						idspcbPathOut,			0,		5,	0,							0,				0,			NULL,								0,0,
	IDCB_IT_FREQUEST		,	0,						DLG_COMBO,	NULL,						idsfRequest,			0,		6,	CBO_PATTERN_MATCHING,0,				0,			NULL,								0,0,
	IDCK_IT_DWUSAGECOUNT	,	0,						DLG_CKBOX,	NULL,						idsdwUsageCount,		0,		7,	0,							0,				0,			NULL,								0,0,
	};

CONTROL lptSQLInstallTranslatorEx[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_ITEX_SZTRANSLATOR	,	0,						DLG_COMBO,	NULL,						idsszTranslator,		0,		0,	CBO_CALCULATED_LEN,	0,				0,			NULL,								0,0,
	IDCB_ITEX_SZPATHIN		,	0,						DLG_COMBO,	NULL,						idsszPathIn,			0,		1,	CBO_CALCULATED_LEN,	0,				0,			NULL,								0,0,
	IDCK_ITEX_SZPATHOUT		,	0,						DLG_CKBOX,	NULL,						idsszPathOut,			0,		2,	0,							0,				0,			NULL,								0,0,
	IDE_ITEX_CBPATHOUTMAX	,	0,						DLG_EDIT,	NULL,						idscbPathOutMax,		0,		3,	0,							0,				0,			NULL,								0,0,
	IDCK_ITEX_PCBPATHOUT		,	0,						DLG_CKBOX,	NULL,						idspcbPathOut,			0,		4,	0,							0,				0,			NULL,								0,0,
	IDCB_ITEX_FREQUEST		,	0,						DLG_COMBO,	NULL,						idsfRequest,			0,		5,	CBO_PATTERN_MATCHING,0,				0,			NULL,								0,0,
	IDCK_ITEX_DWUSAGECOUNT	,	0,						DLG_CKBOX,	NULL,						idsdwUsageCount,		0,		6,	0,							0,				0,			NULL,								0,0,
	};


CONTROL lptSQLInstallerError[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle	iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	------------	--------	--------------------	-----------	--------	--------------
	IDE_IE_IERROR,				0,						DLG_EDIT,	NULL,						idsiError,				0,					0,				0,							0,				0,			NULL,						0,0,
	IDCK_IE_PFERRORCODE,		0,						DLG_CKBOX,	NULL,						idspfErrorCode,		0,					1,				0,							0,				0,			NULL,						0,0,
	IDCK_IE_SZERRORMSG,		0,						DLG_CKBOX,	NULL,						idsszErrorMsg,			0,					2,				0,							0,				0,			NULL,						0,0,
	IDE_IE_CBERRORMSGMAX,	0,						DLG_EDIT,	NULL,						idscbErrorMsgMax,		0,					3,				0,							0,				0,			NULL,						0,0,
	IDCK_IE_PCBERRORMSG,		0,						DLG_CKBOX,	NULL,						idspcbErrorMsg,		0,					4,				0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLRemoveTranslator[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_RT_SZTRANSLATOR,	0,						DLG_COMBO,	NULL,						idsszTranslator,		0,		0,	CBO_STRING_VAL,		0,				0,			NULL,							0,0,
	IDCK_RT_DWUSAGECOUNT,	0,						DLG_CKBOX,	NULL,						idsdwUsageCount,		0,		1,	0,							0,				0,			NULL,						0,0,
	};



CONTROL lptSQLConfigDriver[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	 wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_CD_HWNDPARENT,	0,						DLG_CKBOX,	NULL,						idshwndParent,			0,			0,	0,							0,				0,			NULL,						0,0,
	IDCB_CD_FREQUEST	,	0,						DLG_COMBO,	NULL,						idsfRequest,			0,			1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCB_CD_SZDRIVER	,	0,						DLG_COMBO,	NULL,						idslpszDrivers,		0,			2,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_CD_SZARGS		,	0,						DLG_COMBO,	NULL,						idslpszArgs,			0,			3,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_CD_SZMSG		,	0,						DLG_CKBOX,	NULL,						idslpszMsg,				0,			4,	0,							0,				0,			NULL,						0,0,
	IDE_CD_CBMSGMAX	,	0,						DLG_EDIT,	NULL,						idscbMsgMax,			0,			5,	0,							0,				0,			NULL,						0,0,
	IDCK_CD_PCBMSGMAX	,	0,						DLG_CKBOX,	NULL,						idspcbMsgMax,			0,			6,	0,							0,				0,			NULL,						0,0,

	};

CONTROL lptSQLConfigDataSource[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	 wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	-----------------		--------	--------------------	-----------	--------	--------------
	IDCB_CDS_FREQUEST		,	0,						DLG_COMBO,	NULL,						idsfRequest,			0,		1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCB_CDS_SZDRIVER		,	0,						DLG_COMBO,	NULL,						idslpszDrivers,		0,		2,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_CDS_SZATTRIBUTES,	0,						DLG_COMBO,	NULL,						idslpszAttributes,	0,		3,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_CDS_HWNDPARENT	,	0,						DLG_CKBOX,	NULL,						idshwndParent,			0,		0,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLCreateDataSource[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_CRDS_SZDS		,	0,						DLG_COMBO,	NULL,						idslpszDS,				0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_CRDS_HWNDPARENT,0,						DLG_CKBOX,	NULL,						idshwndParent,			0,			0,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetInstalledDrivers[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_GID_SZBUF	,	0,						DLG_CKBOX,	NULL,						idslpszMsg,				0,				0,	0,							0,				0,			NULL,						0,0,
	IDE_GID_CBBUFMAX,	0,						DLG_EDIT,	NULL,						idscbBufMax,			0,				1,	0,							0,				0,			NULL,						0,0,
	IDCK_GID_PCBBUFOUT,0,					DLG_CKBOX,	NULL,						idspcbBufOut,			0,				2,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetPrivateProfileString[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle				iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------		--------	--------------------	-----------	--------	--------------
	IDCB_GPPS_SZSECTION		,	0,						DLG_COMBO,	NULL,						idslpszSection,		0,		0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_GPPS_SZKEY			,	0,						DLG_COMBO,	NULL,						idslpszKey,				0,		1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_GPPS_SZDEFAULT		,	0,						DLG_COMBO,	NULL,						idslpszDefault,		0,		2,	0,							0,				0,			NULL,						0,0,
	IDCK_GPPS_SZRETURNBUFFER,	0,						DLG_CKBOX,	NULL,						idslpszReturnBuffer,	0,		3,	0,							0,				0,			NULL,						0,0,
	IDE_GPPS_CCHRETURNBUFFER,	0,						DLG_EDIT,	NULL,						idscchReturnBuffer,	0,		4,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLInstallDriver[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_ID_SZINFFILE,	0,						DLG_COMBO,	NULL,						idslpszInfFile,		0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_ID_SZDRIVER,		0,						DLG_COMBO,	NULL,						idsszDriver,			0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_ID_SZPATH	,		0,						DLG_CKBOX,	NULL,						idslpszPath,			0,			2,	0,							0,				0,			NULL,						0,0,
	IDE_ID_CBPATHMAX,		0,						DLG_EDIT,	NULL,						idscbPathMax,			0,			3,	0,							0,				0,			NULL,						0,0,
	IDCK_ID_PCBPATHOUT,	0,						DLG_CKBOX,	NULL,						idspcbPathOut,			0,			4,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLInstallDriverEx[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_IDEX_SZDRIVER,	0,						DLG_COMBO,	NULL,						idsszDriver,			0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_IDEX_SZPATHIN,	0,						DLG_COMBO,	NULL,						idslpszPathIn,			0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_IDEX_SZPATHOUT,	0,						DLG_CKBOX,	NULL,						idslpszPathOut,		0,			2,	0,							0,				0,			NULL,						0,0,
	IDE_IDEX_CBPATHOUTMAX,0,					DLG_EDIT,	NULL,						idscbPathOutMax,		0,			3,	0,							0,				0,			NULL,						0,0,
	IDCK_IDEX_PCBPATHOUT,0,						DLG_CKBOX,	NULL,						idspcbPathOut,			0,			4,	0,							0,				0,			NULL,						0,0,
	IDCB_IDEX_FREQUEST,	0,						DLG_COMBO,	NULL,						idsfRequest,			0,			5,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCK_IDEX_LPDWUSAGECOUNT,0,				DLG_CKBOX,	NULL,						idsdwUsageCount,		0,			6,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetConfigMode[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_GCM_PWCONFIGMODE,	0,						DLG_CKBOX,	NULL,						idspwConfigMode,		0,							0,			0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLSetConfigMode[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_SCM_WCONFIGMODE,0,						DLG_COMBO,	NULL,						idswConfigMode,		0,			0,	0,							0,				0,			NULL,								0,0,
 	};

CONTROL lptSQLInstallDriverManager[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_IDM_SZPATH	,	0,						DLG_CKBOX,	NULL,						idslpszPath,			0,			0,	0,							0,				0,			NULL,							0,0,
	IDE_IDM_CBPATHMAX	,	0,						DLG_EDIT,	NULL,						idscbPathMax,			0,			1,	0,							0,				0,			NULL,							0,0,
	IDCK_IDM_PCBPATHOUT,	0,						DLG_CKBOX,	NULL,						idspcbPathOut,			0,			2,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLManageDataSources[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle				iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------		--------	--------------------	-----------	--------	--------------
	IDCK_MDS_HWND,	0,						DLG_CKBOX,	NULL,						idshwndParent,			0,					0,			0,							0,				0,			NULL,							0,0,
	};

CONTROL lptSQLRemoveDefaultDataSource[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	0,		0,						DLG_STATIC,	NULL,						0,							0,					0,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLRemoveDSNFromIni[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_RDSNFI_SZDSN,	0,						DLG_COMBO,	NULL,						idsszDSN,				0,		0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	};

CONTROL lptSQLValidDSN[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_VDSN_SZDSN,	0,						DLG_COMBO,	NULL,						idsszDSN,				0,			0,	CBO_STRING_VAL,		0,				0,			NULL,							0,0,
	};

CONTROL lptSQLWriteDSNToIni[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_WDSNTI_SZDSN	,	0,						DLG_COMBO,	NULL,						idsszDSN,				0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_WDSNTI_SZDRIVER,0,						DLG_COMBO,	NULL,						idsszDriver,			0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	};

CONTROL lptSQLWritePrivateProfileString[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_WPPS_SZSECTION,	0,						DLG_COMBO,	NULL,						idslpszSection,		0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_WPPS_SZKEY	,	0,						DLG_COMBO,	NULL,						idslpszKey,				0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_WPPS_SZSTRING,	0,						DLG_COMBO,	NULL,						idslpszString,			0,			2,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetTranslator[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_GT_HWNDPARENT,	0,						DLG_CKBOX,	NULL,						idshwndParent,			0,			0,	0,							0,								0,			NULL,						0,0,
	IDCB_GT_SZNAME		,	0,						DLG_COMBO,	NULL,						idslpszName,			0,			1,	CBO_STRING_VAL,		IDE_GT_CBNAMEMAX,			0,			NULL,						0,0,
	IDE_GT_CBNAMEMAX	,	0,						DLG_EDIT,	NULL,						idscbNameMax,			0,			2,	CBO_CB_VAL,				IDCB_GT_SZNAME,			0,			NULL,						0,0,
	IDCK_GT_PCBNAMEOUT,	0,						DLG_CKBOX,	NULL,						idspcbNameOut,			0,			3,	0,							0,								0,			NULL,						0,0,
	IDCK_GT_SZPATH		,	0,						DLG_CKBOX,	NULL,						idslpszPath,			0,			4,	CBO_STRING_VAL,		IDE_GT_CBPATHMAX,			0,			NULL,						0,0,
	IDE_GT_CBPATHMAX	,	0,						DLG_EDIT,	NULL,						idscbPathMax,			0,			5,	CBO_CB_VAL,				IDCK_GT_SZPATH,			0,			NULL,						0,0,
	IDCK_GT_PCBPATHOUT,	0,						DLG_CKBOX,	NULL,						idspcbPathOut,			0,			6,	0,							0,								0,			NULL,						0,0,
	IDCK_GT_PVOUT		,	0,						DLG_CKBOX,	NULL,						idspvOption,			0,			7,	0,							0,								0,			NULL,						0,0,
	};


CONTROL lptSQLCancel[] = {
//	wID				lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----				-----------------	-----------	--------------------	--------------------	------------------	--------	--------------------	-----------	--------	--------------
	IDCK_C_HSTMT,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,							0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLError[] = {
//	wID							lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----							-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCK_E_HDBC				,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,							1,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	IDCK_E_HSTMT			,	0,						DLG_CKBOX,	NULL,						idsHstmt,				0,							2,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	IDCK_E_SZSQLSTATE		,	0,						DLG_CKBOX,	NULL,						0,							0,							3,			0,							0,				0,			NULL,						0,0,
	IDCK_E_PFNATIVEERROR	,	0,						DLG_CKBOX,	NULL,						0,							0,							4,			0,							0,				0,			NULL,						0,0,
	IDCK_E_SZERRORMSG		,	0,						DLG_CKBOX,	NULL,						0,							0,							5,			0,							0,				0,			NULL,						0,0,
	IDE_E_CBERRORMSGMAX	,	0,						DLG_EDIT,	NULL,						0,							0,							6,			0,							0,				0,			NULL,						0,0,
	IDCK_E_PCBERRORMSG	,	0,						DLG_CKBOX,	NULL,						0,							0,							7,			0,							0,				0,			NULL,						0,0,
	IDCK_E_HENV				,	0,						DLG_CKBOX,	NULL,						idsHenv,					0,		 					0,			CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLGetFunctions[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	 wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_GF_FFUNCTION	,	0,						DLG_COMBO,	NULL,						0,							0,			1,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCK_GF_PFEXISTS	,	0,						DLG_CKBOX,	NULL,						0,							0,			2,	0,							0,				0,			NULL,						0,0,
	IDCK_GF_HDBC		,	0,						DLG_CKBOX,	NULL,						idsHdbc,					0,			0,	CBO_DFT_ODBC_HANDLE,	0,				0,			NULL,						0,0,
	};

CONTROL lptSQLReadFileDSN[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_RFD_LPSZFILENAME,	0,						DLG_COMBO,	NULL,						idslpszFileName,		0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_RFD_LPSZAPPNAME,	0,						DLG_COMBO,	NULL,						idslpszAppName,		0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_RFD_LPSZKEYNAME,	0,						DLG_COMBO,	NULL,						idslpszKeyName,		0,			2,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCK_RFD_LPSZSTRING,		0,						DLG_CKBOX,	NULL,						idsszString,			0,			3,	0,							0,				0,			NULL,						0,0,
	IDE_RFD_CBSTRING,			0,						DLG_EDIT,	NULL,						idscbString,			0,			4,	0,							0,				0,			NULL,						0,0,
	IDCK_RFD_PCBSTRING,		0,						DLG_CKBOX,	NULL,						idspcbString,			0,			5,	0,							0,				0,			NULL,						0,0,
	};

CONTROL lptSQLWriteFileDSN[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc   wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_WFD_LPSZFILENAME,	0,						DLG_COMBO,	NULL,						idslpszFileName,		0,			0,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_WFD_LPSZAPPNAME,	0,						DLG_COMBO,	NULL,						idslpszAppName,		0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_WFD_LPSZKEYNAME,	0,						DLG_COMBO,	NULL,						idslpszKeyName,		0,			2,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	IDCB_WFD_LPSZSTRING,		0,						DLG_COMBO,	NULL,						idslpszString,			0,			3,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	};

CONTROL lptSQLPostInstallerError[] = {
//	wID	lStyle				type			szText					uDescID					uFormatStyle			iParmNum	lDataStyle				wLengthID	fUpdated	lpUsrWndProc	  wDataID	fValidate
//	----	-----------------	-----------	--------------------	--------------------	--------------------	--------	--------------------	-----------	--------	--------------
	IDCB_PIE_FERRORCODE,		0,						DLG_COMBO,	NULL,						idsfErrorCode,			0,			0,	CBO_PATTERN_MATCHING,0,				0,			NULL,						0,0,
	IDCB_PIE_SZERRORMSG,		0,						DLG_COMBO,	NULL,						idslpszErrorMsg,		0,			1,	CBO_STRING_VAL,		0,				0,			NULL,						0,0,
	};

// This structure is used as a hashed array of pointers to parameter data.
//	The Initlpt macro simply pastes the function name to fill out the
// fields of the structure.
#define Initlpt(func)	lpt ## func, NumItems(lpt ## func)
struct {
	lpCONTROL			lpCtl;				// Pointer to control structure
	UINT					cParms;				// Number of controls not including buttons
	} DialogTemplate[] = {
	// Core Functions
/*  0*/Initlpt(SQLAllocConnect),
/*  1*/Initlpt(SQLAllocEnv),
/*  2*/Initlpt(SQLAllocStmt),
/*  3*/Initlpt(SQLBindCol),
/*  4*/Initlpt(SQLCancel),
/*  5*/Initlpt(SQLColAttributes),
/*  6*/Initlpt(SQLConnect),
/*  7*/Initlpt(SQLDescribeCol),
/*  8*/Initlpt(SQLDisconnect),
/*  9*/Initlpt(SQLError),
/* 10*/Initlpt(SQLExecDirect),
/* 11*/Initlpt(SQLExecute),
/* 12*/Initlpt(SQLFetch),
/* 13*/Initlpt(SQLFreeConnect),
/* 14*/Initlpt(SQLFreeEnv),
/* 15*/Initlpt(SQLFreeStmt),
/* 16*/Initlpt(SQLGetCursorName),
/* 17*/Initlpt(SQLNumResultCols),
/* 18*/Initlpt(SQLPrepare),
/* 19*/Initlpt(SQLRowCount),
/* 20*/Initlpt(SQLSetCursorName),
/* 21*/NULL,0,//Initlpt(SQLSetParam),
/* 22*/Initlpt(SQLTransact),
/* 23*/Initlpt(SQLColumns),
/* 24*/Initlpt(SQLDriverConnect),
/* 25*/Initlpt(SQLGetConnectOption),
/* 26*/Initlpt(SQLGetData),
/* 27*/Initlpt(SQLGetFunctions),
/* 28*/Initlpt(SQLGetInfo),
/* 29*/Initlpt(SQLGetStmtOption),
/* 30*/Initlpt(SQLGetTypeInfo),
/* 31*/Initlpt(SQLParamData),
/* 32*/Initlpt(SQLPutData),
/* 33*/Initlpt(SQLSetConnectOption),
/* 34*/Initlpt(SQLSetStmtOption),
/* 35*/Initlpt(SQLSpecialColumns),
/* 36*/Initlpt(SQLStatistics),
/* 37*/Initlpt(SQLTables),
/* 38*/Initlpt(SQLBrowseConnect),
/* 39*/Initlpt(SQLColumnPrivileges),
/* 40*/Initlpt(SQLDataSources),
/* 41*/Initlpt(SQLDescribeParam),
/* 42*/Initlpt(SQLExtendedFetch),
/* 43*/Initlpt(SQLForeignKeys),
/* 44*/Initlpt(SQLMoreResults),
/* 45*/Initlpt(SQLNativeSql),
/* 46*/Initlpt(SQLNumParams),
/* 47*/Initlpt(SQLParamOptions),
/* 48*/Initlpt(SQLPrimaryKeys),
/* 49*/Initlpt(SQLProcedureColumns),
/* 50*/Initlpt(SQLProcedures),
/* 51*/Initlpt(SQLSetPos),
/* 52*/Initlpt(SQLSetScrollOptions),
/* 53*/Initlpt(SQLTablePrivileges),
/* 54*/Initlpt(SQLDrivers),
/* 55*/Initlpt(SQLBindParameter),
/* 56*/Initlpt(SQLAllocHandle),
/* 57*/Initlpt(SQLBindParam),
/* 58*/Initlpt(SQLCloseCursor),
/* 59*/Initlpt(SQLColAttribute),
/* 60*/Initlpt(SQLCopyDesc),
/* 61*/Initlpt(SQLEndTran),
/* 62*/Initlpt(SQLFetchScroll),
/* 63*/Initlpt(SQLFreeHandle),
/* 64*/Initlpt(SQLGetConnectAttr),
/* 65*/Initlpt(SQLGetDescField),
/* 66*/Initlpt(SQLGetDescRec),
/* 67*/Initlpt(SQLGetDiagField),
/* 68*/Initlpt(SQLGetDiagRec),
/* 69*/Initlpt(SQLGetEnvAttr),
/* 70*/Initlpt(SQLGetStmtAttr),
/* 71*/Initlpt(SQLSetConnectAttr),
/* 72*/Initlpt(SQLSetDescField),
/* 73*/Initlpt(SQLSetDescRec),
/* 74*/Initlpt(SQLSetEnvAttr),
/* 75*/Initlpt(SQLSetStmtAttr),
/* 76*/Initlpt(SQLBulkOperations),
#if ODBCVER >= 0x350
/* 77*/Initlpt(SQLLocator),
/* 78*/Initlpt(SQLGetLength),
/* 79*/Initlpt(SQLGetPosition),
/* 80*/Initlpt(SQLGetSubString),
/* 81*/Initlpt(SQLLocatedUpdate),
#else
/* THE FOLLOWING ARE ONLY PLACE HOLDERS */
/* 77*/NULL, 0,
/* 78*/NULL, 0,
/* 79*/NULL, 0,
/* 80*/NULL, 0,
/* 81*/NULL, 0,
#endif
/* 82*/Initlpt(SQLTables25),
/* 83*/Initlpt(SQLColumns25),
/* 84*/Initlpt(SQLStatistics25),
/* 85*/Initlpt(SQLPrimaryKeys25),
/* 86*/Initlpt(SQLForeignKeys25),
/* 87*/Initlpt(SQLTablePrivileges25),
/* 88*/Initlpt(SQLColumnPrivileges25),
/* 89*/Initlpt(SQLSpecialColumns25),
/* 90*/Initlpt(SQLProcedures25),
/* 91*/Initlpt(SQLProcedureColumns25),
/* 92*/Initlpt(SQLConfigDataSource),
/* 93*/Initlpt(SQLConfigDriver),
/* 94*/Initlpt(SQLCreateDataSource),
/* 95*/Initlpt(SQLGetInstalledDrivers),
/* 96*/Initlpt(SQLGetPrivateProfileString),
/* 97*/Initlpt(SQLInstallDriver),
/* 98*/Initlpt(SQLInstallDriverManager),
/* 99*/Initlpt(SQLInstallTranslator),
/*100*/Initlpt(SQLInstallTranslatorEx),
/*101*/Initlpt(SQLManageDataSources),
/*102*/Initlpt(SQLRemoveDefaultDataSource),
/*103*/Initlpt(SQLRemoveDSNFromIni),
/*104*/Initlpt(SQLRemoveDriver),
/*105*/Initlpt(SQLRemoveDriverManager),
/*106*/Initlpt(SQLRemoveTranslator),
/*107*/Initlpt(SQLValidDSN),
/*108*/Initlpt(SQLWriteDSNToIni),
/*109*/Initlpt(SQLWritePrivateProfileString),
/*110*/Initlpt(SQLGetTranslator),
/*111*/Initlpt(FillParam),
/*112*/Initlpt(SQLInstallerError),
/*113*/Initlpt(SQLInstallDriverEx),
/*114*/Initlpt(SQLPostInstallerError),
/*115*/Initlpt(SQLReadFileDSN),
/*116*/Initlpt(SQLWriteFileDSN),
/*117*/Initlpt(SQLGetConfigMode),
/*118*/Initlpt(SQLSetConfigMode),
/*119*/Initlpt(SQLNextColumn),
/*120*/Initlpt(SQLGetNestedHandle),
};

//*---------------------------------------------------------------------------------
//| GetControlInfo:
//|	Given the SQL_APIxxx number, return the count of controls, and build an
//|	array of pointers to each element in the array.
//| Parms:
//|	dex						The index of the function to return
//|	pNumCtls					Pointer for number of controls
//| Returns:
//|	Pointer to control information, NULL if out of range
//*---------------------------------------------------------------------------------
lpCONTROL * GetControlInfo(int dex, UINT * pNumCtls)
{
	UINT			adex;
	lpCONTROL * lpCtlArray;

	if(dex > NumItems(DialogTemplate))
		return NULL;

	*pNumCtls = DialogTemplate[dex].cParms;

	if (!*pNumCtls)
		return NULL;

	// Allocate an array of pointers, then fill each pointer with a valid address
	lpCtlArray = (lpCONTROL *)AllocateMemory(sizeof(lpCONTROL *) * *pNumCtls);
	if(!lpCtlArray)
		return NULL;
	for(adex=0;  adex<*pNumCtls;  adex++)
		lpCtlArray[adex] = (lpCONTROL)&DialogTemplate[dex].lpCtl[adex].wID;
	return lpCtlArray;
}


//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FUNCHNDL.H
//|
//| Purpose:
//|		This file contains the function prototypes for the
//|		Function Handlers.  The actual function themselves can be found
//|		in the following files (catagorized by menu name):
//|				Connect							FHCONN.C
//|				Statement						FHSTMT.C
//|				Results							FHRSLT.C
//|				Catalog							FHCATL.C
//|				Misc								FHMISC.C
//|
//| Notes:
//|	All parameter data is stored in an array of parameters.  The following
//|	notes hold true:
//|		1) For handles (HENV, HDBC, HSTMT), the lpData value is boolean TRUE
//|				if NULL should be passed, or FALSE if a valid handle (or pointer
//|				to a valid handle) should be passed
//|		2) For "normal", non-fetch related pointers, the lpData points to
//|				valid space for that parameter or NULL if desired.
//|		3) For non-pointers, lpData is large enough to represent that value
//|				alone (eg: 2 bytes for an SWORD).  You will see calls to the
//|				function which cast lpData as a pointer to that type, example:
//|					*(SWORD *)lpParm->lpData
//|				This line tells the copmiler that lpData is a pointer to an SWORD
//|				and the we want to pass what the pointer is pointing to.
//|
//*---------------------------------------------------------------------------------
#ifndef FUNCHNDL_DEFS
#define FUNCHNDL_DEFS

#define STD_FH_PARMS	LPTSTR szFuncName, UINT cParms, lpCONNECTIONINFO ci, lpPARAMITEM * lpParms, UDWORD uAttrib, LPVOID lpPtr

//----------------------------------------------------------------------------------
//		Utility functions located in function handler modules
//----------------------------------------------------------------------------------

VOID INTFUN LoadConnStmtHandles(HWND hwnd,lpCONNECTIONINFO lpCI,SDWORD *piHdbc,SDWORD *piHstmt,SWORD cConns);

// FHENV
VOID INTFUN SetCurrentHdbcFromList(lpENVIRONMENTINFO lpei);
SWORD INTFUN DropAllConns(lpENVIRONMENTINFO lpei);
VOID INTFUN DoDropConn(lpCONNECTIONINFO lpConn);
lpENVIRONMENTINFO INTFUN FindActiveEnvNode(lpENVIRONMENTINFO lpHeadEnv,
	lpCONNECTIONINFO lpActiveConnWin,int cNodes);
lpENVIRONMENTINFO INTFUN DoAllocEnv(HENV henv);
lpENVIRONMENTINFO INTFUN DoFreeEnvNode(lpENVIRONMENTINFO lpEi);
lpENVIRONMENTINFO INTFUN GetActiveEnv();
lpENVIRONMENTINFO AddHdbcToList(lpENVIRONMENTINFO lpActiveEnv,lpCONNECTIONINFO lpci);
BOOL INTFUN DoDropAllEnvs(lpENVIRONMENTINFO lpEi);
lpCONNECTIONINFO INTFUN FindHdbcNode(lpENVIRONMENTINFO lpEi,lpCONNECTIONINFO lpCi);
VOID INTFUN RemoveHdbcNode(lpCONNECTIONINFO lpCi);
lpENVIRONMENTINFO INTFUN RemoveHdbcFromList(lpCONNECTIONINFO lpCi);
lpENVIRONMENTINFO INTFUN GetActiveEnvNode();
HENV INTFUN GetActiveEnvHandle();
BOOL INTFUN DoDropEnv(lpENVIRONMENTINFO lpEi);


// FHDESC
lpDESCRIPTORHEADER INTFUN FindImplicitDescriptor(lpCONNECTIONINFO lpCI,UWORD	uwDescType);
lpDESCRIPTORHEADER INTFUN DoAllocDescriptor(lpCONNECTIONINFO lpCi,SQLHANDLE hHndl,
														  BOOL fName);
BOOL INTFUN DropAllDescs(lpCONNECTIONINFO lpci);
VOID INTFUN DoDropDesc(lpCONNECTIONINFO lpci,SQLHDESC hdesc);
lpDESCRIPTORHEADER INTFUN FindDescriptor(SQLHDESC hDesc,lpCONNECTIONINFO lpCI);
VOID INTFUN SetDescriptorInfo(lpCONNECTIONINFO lpci,
		SQLHDESC hdesc,
		UWORD iRec,
		SWORD fParamType,
		SWORD fCType,
		SWORD fSQLType,
		SWORD	ibScale,
		PTR rgbValue,
		UDWORD cbColDef,
		SDWORD cbValueMax,
		SQLLEN *pcbValue,
		UDWORD udAPI);
VOID INTFUN AssociateNameWithDesc(lpDESCRIPTORHEADER lpDesc);
VOID INTFUN LoadDescriptorDefaults(lpCONNECTIONINFO lpCI);
VOID INTFUN PickHdesc(lpCONNECTIONINFO lpCI);
lpDESCRIPTORRECORD INTFUN AddDescriptorRecord(lpDESCRIPTORHEADER lpDesc, SWORD iRec);
UDWORD INTFUN GetArrayStatusSize(lpCONNECTIONINFO lpci);
lpDESCRIPTORRECORD INTFUN FindDescriptorRecord(lpDESCRIPTORHEADER lpDesc,SWORD swRecNum);
VOID INTFUN SetDescriptorField(lpCONNECTIONINFO lpCI,lpSTATEMENTINFO lpStmt,
										 lpDESCRIPTORHEADER lpDesc,SDWORD sdFieldIndentifier,
										 PTR ValuePtr,SDWORD sdwStringLength,SWORD iRec);
VOID INTFUN FillDescriptorInfo(lpCONNECTIONINFO lpCI,lpDESCRIPTORHEADER lpDesc);
SQLHDESC INTFUN GetDescHandle(lpSTATEMENTINFO lpStmt,UDWORD udAttribute);
LPTSTR INTFUN FormatDescHandle(lpCONNECTIONINFO lpCI,lpDESCRIPTORHEADER lpDesc,LPTSTR szHndlStr);
BOOL INTFUN IsValidDescField(SWORD swFieldID,UWORD uwType);

// FHCONN
VOID INTFUN ConnectFullConnect(void);
BOOL INTFUN ConnectFullDisconnect(lpCONNECTIONINFO lpci,RETCODE rcDriverConnect);
VOID INTFUN ConnectDataSourcesAll(lpCONNECTIONINFO ci);
VOID INTFUN ConnectDriversAll(lpCONNECTIONINFO ci);
VOID INTFUN ConnectGetInfoAll(lpCONNECTIONINFO ci);
VOID INTFUN GetDescriptorAll(lpCONNECTIONINFO lpCI);



// FHSTMT
lpSTATEMENTINFO INTFUN DoAllocStmt(lpCONNECTIONINFO ci, HSTMT hstmt);
VOID INTFUN DoDropStmt(lpCONNECTIONINFO ci,lpSTATEMENTINFO lpState);
BOOL INTFUN DropAllStmts(lpCONNECTIONINFO lpci, BOOL fNeedToFree);
VOID INTFUN StatementFillParam(lpCONNECTIONINFO ci);
VOID INTFUN StatementShowParams(lpCONNECTIONINFO ci);
BOOL INTFUN FreeParameter(lpSTATEMENTINFO lpState, lpPARAMETERINFO lpParam);
BOOL INTFUN ChangeRowsetSize(lpCONNECTIONINFO lpci,
					lpSTATEMENTINFO lpStmt, UDWORD uNewRowsetSize,
					BOOL fResetStmtOpt,UDWORD uSQLApi);
BOOL INTFUN ChangeBindType(HWND hwnd, lpCONNECTIONINFO lpci,
						lpSTATEMENTINFO lpInStmt, UDWORD uNewVal);
VOID INTFUN GetCurrentCursorSettings(lpCONNECTIONINFO lpCI);
lpSTATEMENTINFO INTFUN FindSINode(lpCONNECTIONINFO lpCI,HSTMT hstmt);



// FHRSLT
BOOL INTFUN CommitBVChanges(lpSTATEMENTINFO lpStmt);
BOOL INTFUN ResetStmtBVMem(lpSTATEMENTINFO lpStmt, HWND hwndOut, UDWORD uOldRowSet);
VOID INTFUN ResultsGetDataAll(lpCONNECTIONINFO lpci);
VOID INTFUN ResultsGetDataRow(lpCONNECTIONINFO lpci);
VOID INTFUN ResultsGetRowSet(lpCONNECTIONINFO lpci);
VOID INTFUN ResultsDescribeColAll(lpCONNECTIONINFO ci);
lpBOUNDVARS INTFUN FreeBoundColumn(lpSTATEMENTINFO lpState, lpBOUNDVARS lpbv);
BOOL INTFUN ResultsBindColAll(lpCONNECTIONINFO lpci);
VOID INTFUN ResultsShowBoundCols(lpCONNECTIONINFO ci, lpBOUNDVARS lpbv, RETCODE rc,
						UWORD uCol, BOOL fShowColHdr);
VOID INTFUN ResultsFetchAll(lpCONNECTIONINFO lpci);
lpBOUNDVARS IsBoundColumn(UWORD uCol, lpSTATEMENTINFO lpStmt);
BOOL INTFUN IsBoundParamsOnRowset(lpSTATEMENTINFO lpStmt);
BOOL INTFUN UpdateBoundColumn(lpCONNECTIONINFO lpCI,UWORD icol,SWORD fCType,
										PTR rgbValue,UNALIGNED SQLLEN *pcbValue,UWORD uType,
										lpBOUNDVARS lpbv,SDWORD cbUsedValueMax,RETCODE rc,
										SDWORD *pcbTotValueMax);
lpBOUNDVARS INTFUN DoBindColumn(lpCONNECTIONINFO lpCI,UWORD icol,PTR *prgbValue,
										UNALIGNED SDWORD **ppcbValue,UWORD *puType,
										SDWORD cbValueMax,SDWORD *pcbValueMaxUsed,SWORD fCType,
										BOOL frgbValueNull,BOOL fpcbValueNull,
										SQLSMALLINT	fFieldID,SDWORD *pcbTotValueMax);

lpBOUNDVARS FindBoundVar(lpBOUNDVARS lpHead, UINT cnt, UWORD icol);

// FHATTR
VOID INTFUN SetCursorAttributes(lpSTATEMENTINFO lpStmt);

// FHMISC
RETCODE INTFUN MiscGetFunctionsAll(lpCONNECTIONINFO ci,UWORD uFunctionId);

//----------------------------------------------------------------------------------
//		Function Handlers
//----------------------------------------------------------------------------------
RETCODE INTFUN lpSQLAllocConnect(STD_FH_PARMS);
RETCODE INTFUN lpSQLAllocEnv(STD_FH_PARMS);
RETCODE INTFUN lpSQLAllocStmt(STD_FH_PARMS);
RETCODE INTFUN lpSQLBindCol(STD_FH_PARMS);
RETCODE INTFUN lpSQLCancel(STD_FH_PARMS);
RETCODE INTFUN lpSQLColAttributes(STD_FH_PARMS);
RETCODE INTFUN lpSQLConnect(STD_FH_PARMS);
RETCODE INTFUN lpSQLDescribeCol(STD_FH_PARMS);
RETCODE INTFUN lpSQLDisconnect(STD_FH_PARMS);
RETCODE INTFUN lpSQLError(STD_FH_PARMS);
RETCODE INTFUN lpSQLExecDirect(STD_FH_PARMS);
RETCODE INTFUN lpSQLExecute(STD_FH_PARMS);
RETCODE INTFUN lpSQLFetch(STD_FH_PARMS);
RETCODE INTFUN lpSQLFreeConnect(STD_FH_PARMS);
RETCODE INTFUN lpSQLFreeEnv(STD_FH_PARMS);
RETCODE INTFUN lpSQLFreeStmt(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetCursorName(STD_FH_PARMS);
RETCODE INTFUN lpSQLNumResultCols(STD_FH_PARMS);
RETCODE INTFUN lpSQLPrepare(STD_FH_PARMS);
RETCODE INTFUN lpSQLRowCount(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetCursorName(STD_FH_PARMS);
RETCODE INTFUN lpSQLTransact(STD_FH_PARMS);

RETCODE INTFUN lpSQLAllocHandle(STD_FH_PARMS);
RETCODE INTFUN lpSQLFreeHandle(STD_FH_PARMS);
RETCODE INTFUN lpSQLBindParam(STD_FH_PARMS);
RETCODE INTFUN lpSQLCloseCursor(STD_FH_PARMS);
RETCODE INTFUN lpSQLEndTran(STD_FH_PARMS);
RETCODE INTFUN lpSQLFetchScroll(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetEnvAttr(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetEnvAttr(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetConnectAttr(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetConnectAttr(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetStmtAttr(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetStmtAttr(STD_FH_PARMS);
RETCODE INTFUN lpSQLCopyDesc(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetDescField(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetDescField(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetDescRec(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetDescRec(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetDiagField(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetDiagRec(STD_FH_PARMS);
RETCODE INTFUN lpSQLColAttribute(STD_FH_PARMS);
RETCODE INTFUN lpSQLLocator(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetLength(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetPosition(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetSubString(STD_FH_PARMS);
RETCODE INTFUN lpSQLLocatedUpdate(STD_FH_PARMS);

	// Level 1
RETCODE INTFUN lpSQLColumns(STD_FH_PARMS);
RETCODE INTFUN lpSQLDriverConnect(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetConnectOption(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetData(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetFunctions(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetInfo(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetStmtOption(STD_FH_PARMS);
RETCODE INTFUN lpSQLGetTypeInfo(STD_FH_PARMS);
RETCODE INTFUN lpSQLParamData(STD_FH_PARMS);
RETCODE INTFUN lpSQLPutData(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetConnectOption(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetStmtOption(STD_FH_PARMS);
RETCODE INTFUN lpSQLSpecialColumns(STD_FH_PARMS);
RETCODE INTFUN lpSQLStatistics(STD_FH_PARMS);
RETCODE INTFUN lpSQLTables(STD_FH_PARMS);
RETCODE INTFUN lpSQLTables25(STD_FH_PARMS);
RETCODE INTFUN lpSQLColumns25(STD_FH_PARMS);
RETCODE INTFUN lpSQLStatistics25(STD_FH_PARMS);
RETCODE INTFUN lpSQLPrimaryKeys25(STD_FH_PARMS);
RETCODE INTFUN lpSQLForeignKeys25(STD_FH_PARMS);
RETCODE INTFUN lpSQLTablePrivileges25(STD_FH_PARMS);
RETCODE INTFUN lpSQLColumnPrivileges25(STD_FH_PARMS);
RETCODE INTFUN lpSQLSpecialColumns25(STD_FH_PARMS);
RETCODE INTFUN lpSQLProcedures25(STD_FH_PARMS);
RETCODE INTFUN lpSQLProcedureColumns25(STD_FH_PARMS);

	// Level 2
RETCODE INTFUN lpSQLBindParameter(STD_FH_PARMS);
RETCODE INTFUN lpSQLBrowseConnect(STD_FH_PARMS);
RETCODE INTFUN lpSQLColumnPrivileges(STD_FH_PARMS);
RETCODE INTFUN lpSQLDataSources(STD_FH_PARMS);
RETCODE INTFUN lpSQLDescribeParam(STD_FH_PARMS);
RETCODE INTFUN lpSQLDrivers(STD_FH_PARMS);
RETCODE INTFUN lpSQLExtendedFetch(STD_FH_PARMS);
RETCODE INTFUN lpSQLForeignKeys(STD_FH_PARMS);
RETCODE INTFUN lpSQLMoreResults(STD_FH_PARMS);
RETCODE INTFUN lpSQLNativeSql(STD_FH_PARMS);
RETCODE INTFUN lpSQLNumParams(STD_FH_PARMS);
RETCODE INTFUN lpSQLParamOptions(STD_FH_PARMS);
RETCODE INTFUN lpSQLPrimaryKeys(STD_FH_PARMS);
RETCODE INTFUN lpSQLProcedureColumns(STD_FH_PARMS);
RETCODE INTFUN lpSQLProcedures(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetPos(STD_FH_PARMS);
RETCODE INTFUN lpSQLSetScrollOptions(STD_FH_PARMS);
RETCODE INTFUN lpSQLTablePrivileges(STD_FH_PARMS);
RETCODE INTFUN lpSQLBulkOperations(STD_FH_PARMS);

// Tools
RETCODE INTFUN lpFillParam(STD_FH_PARMS);
RETCODE INTFUN lpUpdateCell(STD_FH_PARMS);
VOID INTFUN DisplayDiagnostics(RETCODE rcAct,SWORD fHandleType,SQLHANDLE hHandle,
										 LPTSTR szAPI);

BOOL INTFUN IsSupported(lpCONNECTIONINFO lpCI, SQLSMALLINT nFunctionId,
								BOOL fDontUseGetFunc);

RETCODE INTFUN GetSupportedFunctions(HDBC hdbc, lpCONNECTIONINFO ci);
VOID INTFUN ClearErrorQueue(SQLSMALLINT fHandleType,SQLHANDLE hHandle);


//----------------------------------------------------------------------------------
//		Shared inline functions
//----------------------------------------------------------------------------------
__inline VOID INTFUN ChangeBindTypeDetails(lpSTATEMENTINFO lpStmt, UDWORD uNewVal)
{
	lpStmt->fColBinding = (uNewVal==0) ? TRUE : FALSE;
	if(lpStmt->fColBinding)
		lpStmt->uRowWiseSize = 0;
	else
		lpStmt->uRowWiseSize = uNewVal;
}


#endif

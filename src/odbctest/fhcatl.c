//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHCATL.C
//|
//| Purpose:
//|		This module contains the function handlers for the Statement menu.
//|		Note that each function can be found in the order that it appears
//|		on the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR							szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options



//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN GetUpdatedStatusFlags(lpCONNECTIONINFO ci);          // From FHCONN.C



//*---------------------------------------------------------------------------------
//| lpSQLGetTypeInfo:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLGetTypeInfo(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLGetTypeInfo  (hstmt,
								*(SWORD *)lpParms[1]->lpData);		// fSqlType

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLTables:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLTables(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLTables  (hstmt,
						 (LPTSTR)lpParms[1]->lpData,											// szTableQualifier
						 *(SWORD *)lpParms[2]->lpData,            				// cbTableQualifier
						 (LPTSTR)lpParms[3]->lpData,                   				// szTableOwner
						 *(SWORD *)lpParms[4]->lpData,            				// cbTableOwner
						 (LPTSTR)lpParms[5]->lpData,                   				// szTableName
						 *(SWORD *)lpParms[6]->lpData,          					// cbTableName
						 (LPTSTR)lpParms[7]->lpData,                   				// szTableType
						 *(SWORD *)lpParms[8]->lpData);          					// cbTableType

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLTables25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLTables25(STD_FH_PARMS)
{
	return(lpSQLTables(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}



//*---------------------------------------------------------------------------------
//| lpSQLColumns:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLColumns(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLColumns (hstmt,
						 (LPTSTR)lpParms[1]->lpData,										// szTableQualifier
						 *(SWORD *)lpParms[2]->lpData,            						// cbTableQualifier
						 (LPTSTR)lpParms[3]->lpData,                   				// szTableOwner
						 *(SWORD *)lpParms[4]->lpData,            						// cbTableOwner
						 (LPTSTR)lpParms[5]->lpData,                   				// szTableName
						 *(SWORD *)lpParms[6]->lpData,          						// cbTableName
						 (LPTSTR)lpParms[7]->lpData,                   				// szColumnName
						 *(SWORD *)lpParms[8]->lpData);          						// cbColumnName

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLColumns25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLColumns25(STD_FH_PARMS)
{
	return(lpSQLColumns(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLStatistics:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLStatistics(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLStatistics   (hstmt,
						 		(LPTSTR)lpParms[1]->lpData,										// szTableQualifier
						 		*(SWORD *)lpParms[2]->lpData,            				// cbTableQualifier
						 		(LPTSTR)lpParms[3]->lpData,                   				// szTableOwner
						 		*(SWORD *)lpParms[4]->lpData,            				// cbTableOwner
						 		(LPTSTR)lpParms[5]->lpData,                   				// szTableName
						 		*(SWORD *)lpParms[6]->lpData,          				// cbTableName
						 		*(UWORD *)lpParms[7]->lpData,            				// fUnique
						 		*(UWORD *)lpParms[8]->lpData);          				// fAccuracy

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLStatistics25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLStatistics25(STD_FH_PARMS)
{
	return(lpSQLStatistics(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLPrimaryKeys:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLPrimaryKeys(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLPrimaryKeys  (hstmt,
						 		(LPTSTR)lpParms[1]->lpData,										// szTableQualifier
						 		*(SWORD *)lpParms[2]->lpData,            				// cbTableQualifier
						 		(LPTSTR)lpParms[3]->lpData,                   				// szTableOwner
						 		*(SWORD *)lpParms[4]->lpData,            				// cbTableOwner
						 		(LPTSTR)lpParms[5]->lpData,                   				// szTableName
						 		*(SWORD *)lpParms[6]->lpData);          				// cbTableName

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLPrimaryKeys25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLPrimaryKeys25(STD_FH_PARMS)
{
	return(lpSQLPrimaryKeys(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLForeignKeys:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLForeignKeys(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLForeignKeys  (hstmt,
						 		(LPTSTR)lpParms[1]->lpData,
								*(SWORD *)lpParms[2]->lpData,
						 	    (LPTSTR)lpParms[3]->lpData,
						 		*(SWORD *)lpParms[4]->lpData,
								(LPTSTR)lpParms[5]->lpData,
						 		*(SWORD *)lpParms[6]->lpData,
								(LPTSTR)lpParms[7]->lpData,
						 		*(SWORD *)lpParms[8]->lpData,
						 		(LPTSTR)lpParms[9]->lpData,
						 		*(SWORD *)lpParms[10]->lpData,
						 		(LPTSTR)lpParms[11]->lpData,
						 		*(SWORD *)lpParms[12]->lpData);

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}


//*---------------------------------------------------------------------------------
//| lpSQLForeignKeys25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLForeignKeys25(STD_FH_PARMS)
{
	return(lpSQLForeignKeys(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}

//*---------------------------------------------------------------------------------
//| lpSQLTablePrivileges:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLTablePrivileges(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLTablePrivileges (hstmt,
						 			(LPTSTR)lpParms[1]->lpData,										// szTableQualifier
						 			*(SWORD *)lpParms[2]->lpData,            				// cbTableQualifier
						 			(LPTSTR)lpParms[3]->lpData,                   				// szTableOwner
						 			*(SWORD *)lpParms[4]->lpData,            				// cbTableOwner
						 			(LPTSTR)lpParms[5]->lpData,                   				// szTableName
						 			*(SWORD *)lpParms[6]->lpData);          				// cbTableName

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLTablePrivileges25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLTablePrivileges25(STD_FH_PARMS)
{
	return(lpSQLTablePrivileges(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLColumnPrivileges:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLColumnPrivileges(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLColumnPrivileges   (hstmt,
						 				(LPTSTR)lpParms[1]->lpData,										// szTableQualifier
						 				*(SWORD *)lpParms[2]->lpData,            				// cbTableQualifier
						 				(LPTSTR)lpParms[3]->lpData,                   				// szTableOwner
						 				*(SWORD *)lpParms[4]->lpData,            				// cbTableOwner
						 				(LPTSTR)lpParms[5]->lpData,                   				// szTableName
						 				*(SWORD *)lpParms[6]->lpData,          				// cbTableName
						 				(LPTSTR)lpParms[7]->lpData,                   				// szColumnName
						 				*(SWORD *)lpParms[8]->lpData);          				// cbColumnName

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLColumnPrivileges25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLColumnPrivileges25(STD_FH_PARMS)
{
	return(lpSQLColumnPrivileges(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLSpecialColumns:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSpecialColumns(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLSpecialColumns  (hstmt,
						 			*(UWORD *)lpParms[1]->lpData,            				// fColType
						 			(LPTSTR)lpParms[2]->lpData,										// szTableQualifier
						 			*(SWORD *)lpParms[3]->lpData,            				// cbTableQualifier
						 			(LPTSTR)lpParms[4]->lpData,                   				// szTableOwner
						 			*(SWORD *)lpParms[5]->lpData,            				// cbTableOwner
						 			(LPTSTR)lpParms[6]->lpData,                   				// szTableName
						 			*(SWORD *)lpParms[7]->lpData,          				// cbTableName
						 			*(UWORD *)lpParms[8]->lpData,            				// fScope
						 			*(UWORD *)lpParms[9]->lpData);          				// fNullable

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLSpecialColumns25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLSpecialColumns25(STD_FH_PARMS)
{
	return(lpSQLSpecialColumns(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLProcedures:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLProcedures(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLProcedures 	(hstmt,
						 		(LPTSTR)lpParms[1]->lpData,										// szProcQualifier
						 		*(SWORD *)lpParms[2]->lpData,            				// cbProcQualifier
						 		(LPTSTR)lpParms[3]->lpData,                   				// szProcOwner
						 		*(SWORD *)lpParms[4]->lpData,            				// cbProcOwner
						 		(LPTSTR)lpParms[5]->lpData,                   				// szProcName
						 		*(SWORD *)lpParms[6]->lpData);          				// cbProcName

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLProcedures25:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLProcedures25(STD_FH_PARMS)
{
	return(lpSQLProcedures(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}


//*---------------------------------------------------------------------------------
//| lpSQLProcedureColumns:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLProcedureColumns(STD_FH_PARMS)
{
	RETCODE		rc;
	HSTMT			hstmt=NULL;

	if (lpParms[0]->lpData)
		hstmt=*(HSTMT *)lpParms[0]->lpData;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke function
	rc = SQLProcedureColumns 	(hstmt,
						 				(LPTSTR)lpParms[1]->lpData,										// szProcQualifier
						 				*(SWORD *)lpParms[2]->lpData,            				// cbProcQualifier
						 				(LPTSTR)lpParms[3]->lpData,                   				// szProcOwner
						 				*(SWORD *)lpParms[4]->lpData,            				// cbProcOwner
						 				(LPTSTR)lpParms[5]->lpData,                   				// szProcName
						 				*(SWORD *)lpParms[6]->lpData,          				// cbProcName
						 				(LPTSTR)lpParms[7]->lpData,                   				// szColumnName
						 				*(SWORD *)lpParms[8]->lpData);          				// cbColumnName

	if (RC_SUCCESSFUL(rc))
		ci->lpCurStmt->wCurState=STMT_EXECUTED;

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERRORSI(ci,rc,hstmt);

	return rc;
}

//*---------------------------------------------------------------------------------
//| lpSQLProcedureColumns25s:
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLProcedureColumns25(STD_FH_PARMS)
{
	return(lpSQLProcedureColumns(szFuncName,cParms,ci,lpParms,uAttrib,lpPtr));
}

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHLOCATR.C
//|
//| Purpose:
//|		This module contains the function handlers for the Misc menu.
//|		Note that each function can be found in the order that it appears
//|		on the menu.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#pragma warning(disable:4996)

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern lpDFTARRAY					lpdftGetFunct;
extern UINT							ciGetFunct;
extern lpDFTARRAY					lpdftTrueFalse;
extern UINT							ciTrueFalse;

#ifdef SQL_API_SQLLOCATOR



//*---------------------------------------------------------------------------------
//| lpSQLLocator:
//|	This function is used to hold or free locators
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

SQLRETURN INTFUN lpSQLLocator(STD_FH_PARMS)
{

	SQLRETURN			rc=SQL_SUCCESS;
	lpSTATEMENTINFO	lpStmt =		ci ? ci->lpCurStmt : NULL;
	SQLHSTMT          hstmt =		lpParms[0]->fNull ? NULL : lpStmt->hstmt;
	SQLHLOC				hLocator =	*(SQLHLOC *)lpParms[1]->lpData;
	UWORD					fOperation =*(UWORD *)lpParms[2]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLLocator(hstmt,hLocator,fOperation);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc);

	return rc;

} //lpSQLLocator()




//*---------------------------------------------------------------------------------
//| lpSQLGetLength:
//|	This function retrieves the length of a string value represented by a
//|	LOB locator
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

SQLRETURN INTFUN lpSQLGetLength(STD_FH_PARMS)
{

	SQLRETURN			rc=SQL_SUCCESS;
	lpSTATEMENTINFO	lpStmt =			ci ? ci->lpCurStmt : NULL;
	SQLHSTMT          hstmt =			lpParms[0]->fNull ? NULL : lpStmt->hstmt;
	SWORD					fLocatorType =	*(SWORD *)lpParms[1]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLGetLength(hstmt,
						fLocatorType,
						*(SQLHLOC *)lpParms[2]->lpData,
						(SDWORD *)lpParms[3]->lpData,
						(SDWORD *)lpParms[4]->lpData);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc);

	return rc;

} //lpSQLGetLength()


//*---------------------------------------------------------------------------------
//| lpSQLGetPosition:
//|	This function gets the starting position of a (search) string value
//|	within another (source) string value where the source string value
//|	is represented by a LOB locator
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

SQLRETURN INTFUN lpSQLGetPosition(STD_FH_PARMS)
{

	SQLRETURN			rc=SQL_SUCCESS;
	lpSTATEMENTINFO	lpStmt =			ci ? ci->lpCurStmt : NULL;
	SQLHSTMT          hstmt =			lpParms[0]->fNull ? NULL : lpStmt->hstmt;
	SWORD					fLocatorType =	 *(SWORD *)lpParms[1]->lpData;
	SQLHLOC				hSourceLocator= *(SQLHLOC *)lpParms[2]->lpData;
	SQLHLOC				hTargetLocator= *(SQLHLOC *)lpParms[3]->lpData;
	LPSTR					szSearchString= (LPSTR)lpParms[4]->lpData;
	SDWORD				cbSearchString =*(SDWORD *)lpParms[5]->lpData;
	SDWORD				iFromPosition = *(SDWORD *)lpParms[6]->lpData;
	SDWORD				*pibLocatedAt =  (SDWORD *)lpParms[7]->lpData;
	SDWORD				*pfIndicator =	  (SDWORD *)lpParms[8]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLGetPosition(hstmt,
								fLocatorType,
								hSourceLocator,
								hTargetLocator,
								szSearchString,
								cbSearchString,
								iFromPosition,
								pibLocatedAt,
								pfIndicator);


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc);

	return rc;

} //lpSQLGetPosition()


//*---------------------------------------------------------------------------------
//| lpSQLGetSubString:
//|	This function gets an individual descriptor field
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

SQLRETURN INTFUN lpSQLGetSubString(STD_FH_PARMS)
{

	SQLRETURN			rc=SQL_SUCCESS;
	lpSTATEMENTINFO	lpStmt =			ci ? ci->lpCurStmt : NULL;
	SQLHSTMT          hstmt =			lpParms[0]->fNull ? NULL : lpStmt->hstmt;
	SWORD					fLocatorType =	*(SWORD *)lpParms[1]->lpData;
	SQLHLOC				hSourceLocator=*(SQLHLOC *)lpParms[2]->lpData;
	SDWORD				iFromPosition =*(SDWORD *)lpParms[3]->lpData;
	SDWORD				cbForLength =	*(SDWORD *)lpParms[4]->lpData;
	SWORD					fTargetType =	*(SWORD *)lpParms[5]->lpData;
	SQLPOINTER			rgbValue =		 (SQLPOINTER *)lpParms[6]->lpData;
	SDWORD				cbValueMax =	*(SDWORD *)lpParms[7]->lpData;
	SDWORD				*pcbValue =		 (SDWORD *)lpParms[8]->lpData;
	SDWORD				*pfIndicator =	 (SDWORD *)lpParms[9]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLGetSubString(hstmt,
								fLocatorType,
								hSourceLocator,
								iFromPosition,
								cbForLength,
								fTargetType,
								rgbValue,
								cbValueMax,
								pcbValue,
								pfIndicator);


	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc);

	return rc;

} //lpSQLGetSubString()


//*---------------------------------------------------------------------------------
//| lpSQLLocatedUpdate:
//|	This function gets an individual descriptor field
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection information (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------

SQLRETURN INTFUN lpSQLLocatedUpdate(STD_FH_PARMS)
{

	SQLRETURN			rc=SQL_SUCCESS;
	lpSTATEMENTINFO	lpStmt =			ci ? ci->lpCurStmt : NULL;
	SQLHSTMT          hstmt =			lpParms[0]->fNull ? NULL : lpStmt->hstmt;
	SQLSMALLINT			fTargetType =	*(SQLSMALLINT *)lpParms[1]->lpData;
	SQLHLOC				hTargetLocator=*(SQLHLOC *)lpParms[2]->lpData;
	SQLINTEGER			iFromPosition =*(SQLINTEGER *)lpParms[3]->lpData;
	SQLINTEGER			cbForLength =	*(SQLINTEGER *)lpParms[4]->lpData;
	SQLSMALLINT			fSourceType =	*(SQLSMALLINT *)lpParms[5]->lpData;
	SQLPOINTER			rgbValue =		 (SQLPOINTER *)lpParms[6]->lpData;
	SQLINTEGER			cbValue =		*(SQLINTEGER *)lpParms[7]->lpData;
	SQLINTEGER			fIndicator =	*(SQLINTEGER *)lpParms[8]->lpData;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms, cParms, ci, TRUE);

	rc=SQLLocatedUpdate(hstmt,
								fTargetType,
								hTargetLocator,
								iFromPosition,
								cbForLength,
								fSourceType,
								rgbValue,
								cbValue,
								fIndicator);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors, wait until stmt's are changed to avoid use of bad HSTMT
   AUTOLOGERRORSI(ci,rc);

	return rc;

} //lpSQLLocatedUpdate()



#endif

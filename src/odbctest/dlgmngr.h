//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGMNGR.H
//|
//| Purpose:
//|		Contains prototypes and defines for the Dialog Manager.
//*---------------------------------------------------------------------------------
#ifndef DLGMNGR_DEFS
#define DLGMNGR_DEFS
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sql.h"
#include "sqlext.h"
#include "gtrcomm.h"
#include "toolbar.h"
#include "statbar.h"
#include "handles.h"
#include "strings.h"
#include "outwin.h"
#include "connwin.h"
#include "menus.h"



//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
//These constants were made large so that
//they won't conflict with any of dft constants
//(ie. SQL_FLOAT = 6)
#define INTERVALVAL		9994
#define ROWSETVAL			9995
#define SAMEAS_STRLEN	9996
#define VALIDPTR			9997
#define EMPTYSTR			9998
#define NULLPTR			9999



//Bug #318
//ODBC "Display Size". These are taken from App. D in the ODBC Ref. Man. except
//for DATE,TIME and TIMESTAMP, since GATOR represents each field as SWORD and UWORD.
#define DS_SHORT					3
#define DS_LONG					10
#define DS_TINYINT				3
#define DS_BIT						4 //i.e "0x00"
#define DS_FLOAT					13
#define DS_GUID						36
#define DS_DOUBLE					22
#define DS_DATE					18
#define DS_TIME					27
#define DS_TIMESTAMP				45
#define DS_BIGINT					20
#define DS_INTERVAL_TYPE		50

#define NO_RETCODE_CHECK			  -999
#define MAXEXPANDEDCONSTANT			MEDBUFF

#define USER_CBN_UPDATE_CONTENTS		(WM_USER + 300)
#define USER_EN_UPDATE_CONTENTS		(WM_USER + 301)
#define USER_CBN_KILL_FOCUS			(WM_USER + 302)
#define USER_EN_KILL_FOCUS				(WM_USER + 303)
#define USER_IDOK							(WM_USER + 304)
#define USER_RESET_CONTENTS			(WM_USER + 305)
#define USER_UPDATE_CONTROLS			(WM_USER + 306)
#define USER_CBN_SELCHANGE				(WM_USER + 307)

#define DLG_BASE_BEGIN					1
#define DLG_BASE_END						30000
#define DLG_TEXT_BASE					 25

#define DLG_DISPLAY_ONLY				0x00000001L
#define DLG_CANCEL_IS_SKIP				0x00000002L

#define SQL_C_DOUBLESTR					-99

#define UNDO_LEN_DATA_AT_EXEC(length) (-(length-SQL_LEN_DATA_AT_EXEC_OFFSET))
// Following used for initializing controls
#define DEX_DFT_ZERO						(-2)
#define DEX_DFT_ONE						(-3)
#define DEX_FORCED_NULL					(-4)
#define DEX_BIND_PARAM_ERROR			(-5)


//
// Edit-box styles
//
#define EDT_CALCULATED_LEN				0x00000001L


//
// Combo-box styles
//
#define CBO_DFT_NULL						0x00000001L //<null pointer>
#define CBO_DFT_EMPTY					0x00000002L //<empty string>
#define CBO_DFT_INPUT_WIN				0x00000004L //<input window>
#define CBO_DFT_SQL_NTS					0x00000008L //SQL_NTS
#define CBO_DFT_NULL_DATA				0x00000010L //SQL_NULL_DATA
#define CBO_DFT_DATA_AT_EXEC			0x00000020L //SQL_DATA_AT_EXEC
#define CBO_DFT_DEFAULT_PARAM			0x00000040L //<default param>
#define CBO_DFT_SHOW_CALLEN			0x00000080L //
#define CBO_DFT_ROWSET_VALUE			0x00000100L //<rowset value>
#define CBO_CALCULATED_LEN				0x00000200L
#define CBO_PATTERN_MATCHING			0x00000400L
#define CBO_LINKED_FIELD				0x00000800L
#define CBO_STORE_INDEX					0x00001000L
#define CBO_INWIN_LEN					0x00002000L
#define CBO_DFT_ODBC_HANDLE			0x00004000L	//odbc handle
#define CBO_DFT_NULL_HANDLE			0x00008000L	//SQL_NULL_HANDLE
#define CBO_DFT_INTERVAL_VALUE		0x00010000L //<interval value>
#define CBO_DFT_VALID_POINTER			0x00020000L //<valid pointer>
#define CBO_DFT_NULL_HENV				0x00040000L
#define CBO_DFT_NULL_HDBC				0x00080000L
#define CBO_DFT_NULL_HSTMT				0x00100000L
#define CBO_DFT_NULL_HDESC				0x00200000L
#define CBO_VALIDATE_DATA				0x00400000L


#define CBO_STRING_VAL					CBO_DFT_NULL | CBO_DFT_EMPTY | CBO_CALCULATED_LEN
#define CBO_CB_VAL						CBO_DFT_SQL_NTS | CBO_DFT_SHOW_CALLEN | CBO_PATTERN_MATCHING
#define CBO_PCB_VAL						CBO_DFT_NULL


//
// vParam Types, used for determining types of fOption information
//
#define PRM_16BIT              		0x00000001L
#define PRM_32BIT              		0x00000002L
#define PRM_32MSK              		0x00000004L
#define PRM_32HDL              		0x00000008L
#define PRM_STR              	 		0x00000010L
#define PRM_CONN_PREFIX					0x00000020L
#define PRM_STMT_PREFIX					0x00000040L
#define PRM_HWND							0x00000080L
#define PRM_HENV							0x00000100L
#define PRM_HDBC							0x00000200L
#define PRM_HSTMT							0x00000400L
#define PRM_HDESC							0x00000800L
#define PRM_PTR_BIN						0x00001000L //Pointer to a Binary Value (eg. Bookmarks)
#define PRM_PTR_32BIT					0x00002000L //Pointer to a 32 bit value
#define PRM_PTR_16BIT					0x00004000L //Pointer to a 16 bit value
#define PRM_PTR_STR						0x00008000L //Pointer to a string
#define PRM_PTR_PTR						0x00010000L //Pointer to a pointer



//
// fParmType, used for parameter list to indicate a pointer type
//
#define PT_HENV							0x00000001L
#define PT_HDBC							0x00000002L
#define PT_HSTMT							0x00000004L
#define PT_PTR_HENV						0x00000008L
#define PT_PTR_HDBC						0x00000010L
#define PT_PTR_HSTMT						0x00000020L
#define PT_PTR_STR						0x00000040L
#define PT_PTR_SWORD						0x00000080L
#define PT_PTR_UWORD						0x00000100L
#define PT_PTR_SDWORD					0x00000200L
#define PT_PTR_UDWORD					0x00000400L
#define PT_PTR_PTR						0x00000800L
#define PT_HWND							0x00001000L
#define PT_LOG_IN							0x00002000L
#define PT_LOG_OUT						0x00004000L
#define PT_LOG_IO							0x00008000L
#define PT_FREE_PTR						0x00010000L
#define PT_PTR_DOUBLESTR				0x00020000L						// Double Null-Terminated String
#define PT_CHK								0x00040000L						// True-False Checkbox
#define PT_ROWSET_CTL					0x00080000L						// Only used in BindParam Rowset Ctls
#define PT_HDESC							0x00100000L
#define PT_HOUTPUT						0x00200000L
#define PT_INPUT_OUTPUT					0x00400000L						//This is used by SQLGetTranslator where the szName
																					//Parameter is both and input and output paramter
#define PT_MULT_HDESC					0x00800000L						//This parameter takes multiple hdesc,(i.e.SQLCopyDesc)
#define PT_HHANDLE						0x01000000L
#define PT_PTR_BIN						0x02000000L						//Pointer to a binary value (eg. Bookmarks)
#define PT_CB_VALUE						0x04000000L						//Parameter is a CBVALUE
#define PT_LOG_DEFFERED					0x08000000L						//This is a deferred field
#define PT_NONE							0x10000000L						//this is a non-parameter, i.e. not logged


// Used for tracking if storage for output values is modified by the ODBC call
#define PT_OUT_NOTMODIFIED				0x20000000L
#define PT_OUT_NOMODIFY_OK				0x40000000L


//
// Function handler flags
//
#define FH_NO_LOG							0x00000001L


//
// Use to access the size and data of a parameter
//
#define CBPARAMDATA(lpa, dex) lpa[dex]->cBytes
#define LPPARAMDATA(lpa, dex) lpa[dex]->lpData
#define LPPARAMNULL(lpa, dex) lpa[dex]->fNull

// Check for truncation of returned data
#define TRUNCATIONCHECK(frgbNull, fpcbNull, rc, outwin, str, maxlen, retlen, szName) \
	if((!frgbNull) && (!fpcbNull)) TruncationCheck(rc, outwin, str, maxlen, retlen, szName);

// Allocates and output buffer with optional buffer checking, need to
//		handle negative buffer lengths and substitute a valid value
#define BUILDOUTPUTPARM(lpParm, cbMaxOut, fBuffCheck)													\
	if(!BuildOutputParm(lpParm,                                                            \
						((SDWORD)(cbMaxOut) <= 0) ? (UDWORD)lpUsrOptions->uDftBuffLen : (UDWORD)(cbMaxOut),  \
						fBuffCheck))                                                            \
		return SQL_ERROR;

// Log Parameters Macro
#define LOGPARAMETERS(szFuncName, lpParmArray, cParms, ci, fIn) \
			if(!(uAttrib & FH_NO_LOG)) \
				LogParametersSpecial(szFuncName, lpParmArray, cParms, ci, fIn, 0, -1, -1)

// Log Parameters Macro
#define LOGPARAMETERSSPEC(szFuncName, lpParmArray, cParms, ci, fIn) \
			if(!(uAttrib & FH_NO_LOG)) \
				LogParametersSpecial(szFuncName, lpParmArray, cParms, ci, fIn, 0, -1, -1)

// Log Return code Macro
#define LOGRETURNCODE(szFunc, ci, rc) \
	{                                  \
	if (ci && ci->lpCurStmt)				\
		ci->lpCurStmt->fStillExecuting=RC_STILLEXECUTING(rc);	\
	if (!(uAttrib & FH_NO_LOG))   \
		LogReturnCode(szFunc, ci, rc);  \
	}

// Log Return code Macro
#define LOGINSTRETURNCODE(szFunc, ci, rc) \
	{                                  \
	if(!(uAttrib & FH_NO_LOG))         \
		LogInstallerReturnCode(szFunc, ci, rc);  \
	}

// Log Return code Macro
#define LOGINSTRETURNCODE30(szFunc, ci, rc) \
	{                                  \
	if (ci && ci->lpCurStmt)				\
		ci->lpCurStmt->fStillExecuting=RC_STILLEXECUTING(rc);	\
	if(!(uAttrib & FH_NO_LOG))         \
		LogInstallerReturnCode30(szFunc, ci, rc);  \
	}

// Check user options and enumerates errors if required on the HENV
#define AUTOLOGERROREI(ci,ei,rc,henv) if (ei) ei->henvCurr=(henv); \
	if(lpUsrOptions->fAutoErrors) \
		ToolOptionsAllErrors(ci,(henv), NULL,NULL,NULL,(rc)); \
	if (ei) ei->henvCurr=NULL;

// Check user options and enumerates errors if required on the HDBC
#define AUTOLOGERRORCI(ci,rc,hdbc) if (ci) ci->hdbcCurr=(hdbc); \
	if(lpUsrOptions->fAutoErrors) \
		ToolOptionsAllErrors(ci, NULL, (hdbc), NULL,NULL,(rc)); \
	if (ci) ci->hdbcCurr=NULL

// Check user options and enumerates errors if required on the HSTMT
#define AUTOLOGERRORSI(ci,rc,hstmt) if (ci) ci->hstmtCurr=(hstmt); \
	if(lpUsrOptions->fAutoErrors) \
		ToolOptionsAllErrors(ci, NULL, NULL, (hstmt),NULL,(rc)); \
	if (ci) ci->hstmtCurr=NULL

// Check user options and enumerates DESCRIPTOR errors if required
#define AUTOLOGERRORDESC(ci,rc,hdesc) if (ci) ci->hdescCurr=(hdesc); \
	if(lpUsrOptions->fAutoErrors) \
		ToolOptionsAllErrors(ci, NULL, NULL, NULL,(hdesc),(rc)); \
	if (ci) ci->hdescCurr=NULL

// Check user options and enumerates errors if required on the HENV
#define AUTOLOGERRORALL(ci,rc) if(lpUsrOptions->fAutoErrors) \
		ToolOptionsAllErrors(ci, (ci) ? ci->lpActiveEnv->henv: NULL, (ci) ? ci->hdbc : NULL, 		\
		((ci) && (ci->lpCurStmt)) ? ci->lpCurStmt->hstmt : NULL,hdesc,(rc))

// Check user options and check output buffers as appropriate
#define OUTPUTPARAMCHECK(ci, rc, lpParms, cParms, fVocal) if(lpUsrOptions->fBufferChecking) \
		OutputParamCheck(ci, rc, lpParms, cParms, fVocal)

#define CHECKNULLTERM(ci, lpParmBuf, lpParmCb, cbValue) \
	if(lpUsrOptions->fNullChecking && lpParmBuf->uParmType && IsCharData(lpParmBuf->fCType)) \
		NullTerminationCheck(ci, lpParmBuf->szName, lpParmBuf->lpData, \
			lpParmBuf->cBytes, PtrValue(lpParmCb), (lpParmBuf->fCType == SQL_C_DOUBLESTR), cbValue)

// Check user options and check output buffers as appropriate
#define INSTOUTPUTPARAMCHECK(ci, fSuccess, lpParms, cParms, fVocal) if(lpUsrOptions->fBufferChecking) \
		InstallerOutputParamCheck(ci, fSuccess, lpParms, cParms, fVocal)

// Check user options and check output buffers as appropriate
#define UNUSEDBUFFERCHECK(ci, lpParmBuf, lpParmCb, fLastCheck) if(lpUsrOptions->fBufferChecking) \
		UnusedBufferCheck(ci, lpParmBuf->szName, lpParmCb->szName, lpParmBuf->lpData, \
		lpParmBuf->cBytes, &lpParmBuf->uParmType, PtrValue(lpParmCb), \
		fLastCheck, TRUE);

// Check user options and check buffer null termination
#define OUTPUTNULLTERM(buf, cb, maxcb) if(lpUsrOptions->fNullChecking) \
		OutputNullTerm(buf, cb > maxcb ? cb : maxcb))

// Will set or clear a flag for connection option
#define CHECKSTATBAROPTION(uVal, mask, set)	\
{                                            \
	if(set) uVal |= mask;                     \
	else    uVal &= ~mask;                    \
}

// Following typedef used to describe a user's WndProc
typedef BOOL (EXTFUN *lpWNDPROC)(HWND, UINT, WPARAM, LPARAM, lpVOID, UINT);

BOOL EXTFUN SetDftWndProc(HWND, UINT, WPARAM, LPARAM, lpVOID, UINT);
BOOL INTFUN BindParameterWndProc(HWND, UINT, WPARAM, LPARAM,lpVOID, UINT);
BOOL INTFUN PutDataWndProc(HWND, UINT, WPARAM, LPARAM,lpVOID, UINT);
BOOL INTFUN FillParamWndProc(HWND, UINT, WPARAM, LPARAM,lpVOID, UINT);
BOOL INTFUN SetConAtrWndProc(HWND, UINT, WPARAM, LPARAM,lpVOID, UINT);
BOOL INTFUN HandleIsPointerCbWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
BOOL INTFUN BindParamWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
BOOL INTFUN GetDiagFieldWndProc(HWND, UINT, WPARAM, LPARAM, lpVOID, UINT);
BOOL INTFUN ListDescHandlesWndProc(HWND, UINT, WPARAM, LPARAM, lpVOID, UINT);
BOOL INTFUN SubTypeWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
BOOL INTFUN GetSelectedHandleWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
BOOL INTFUN SetCTypeWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
BOOL INTFUN GetHandleFromFieldName(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
BOOL INTFUN DisplayRowsetWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex);
SDWORD INTFUN GetPcbIndex(SDWORD sdVal);
SDWORD INTFUN GetRgbIndex(SDWORD sdVal);


//*------------------------------------------------------------------------
//| DFTARRAY:
//|	This structure describes a set of default constants.  Note that an
//|	fOpt may have another array associated with it.  For example,
//|	the SQL_FETCH_DIRECTION fOpt can be any one of the SQL_FD_xxx
//|	flags.  For this case, lpLink points to the linked array of options.
//*------------------------------------------------------------------------
typedef struct tagDFTARRAY {
	SDWORD			fOpt;						// The option ID, note that format is based on following
	SWORD				fCType;					// The SQL_C_xxx type of fOpt
	LPCTSTR			szOpt;					// The option name
	UDWORD			uVersion;				// Major/minor version of this item
	UDWORD			uValInfo;				// User-defined (eg: could give type for GetInfo)
	UINT				cLinks;					// Number of items in the linked array
	struct tagDFTARRAY * lpLink;		// Array of linked defaults
	} DFTARRAY;
typedef DFTARRAY * lpDFTARRAY;


//*------------------------------------------------------------------------
//| CONTROL:
//|	This structure is used to describe each control in the dialog.
//*------------------------------------------------------------------------
typedef struct tagCONTROL {
	// Description information
	WORD				wID;						// Identifier
	DWORD				lStyle;					// Windows control style flag beyond default
	WORD				type;						// Numeric type of control (DLG_xxx)
	LPTSTR			szText;					// Prompt text, NULL if we should use PARAMINFO
   UINT				uDescID;					// Resource description ID for status bar
   UDWORD			uFormatStyle;			// Formatting style

	// Data information
	int				iParmNum;				// Index to corresponding parameter (-1 if N/A)
	UDWORD			lDataStyle;				// Indicates interpretation of data
	WORD				wLengthID;				// ID of combo-box to hold calculated length, 0 if N/A.
													//		Also used to indicate a linked control (eg: Set/GetConnectOption)

	// Status information, used at run-time by WndProc
	BOOL				fUpdated;				// TRUE if contents have been updated
	lpWNDPROC		lpUsrWndProc;			// User WndProc for extra behavior beyond default
	//sumit
	WORD				wDataID;					// ID of combo box for rgb value
	BOOL				fValidate;				// FALSE if combo box contents not to be validated (e.g for SQL_DATA_AT_EXEC)
	//sumit
	} CONTROL;
typedef CONTROL * lpCONTROL;


//*------------------------------------------------------------------------
//| PARAMITEM:
//|	This structure describes a parameter to a function.
//*------------------------------------------------------------------------
typedef struct tagPARAMITEM {
	LPTSTR			szName;					// Name of the parameter
	UDWORD			uParmType;				// Used to indicate if item is a pointer
	SWORD				fCType;					// ODBC C type number
	SWORD				dwDex;					// Index of selected item for a combo-box (note this should eventually be moved to lpControl)
	BOOL				fNull;					// TRUE if NULL is desired, ODBC handles and output only
	UDWORD			cBytes;					// How large is the item
	lpVOID			lpData;					// Pointer to storage for the item
	UINT				cDfts;					// Number of defaults in lpDfts
	lpDFTARRAY		lpDfts;					// Array of default options
	BOOL				fHandle;					// Is this parameter a Handle?
	UWORD				uwPrecision;			// Precision of this parameter
	UWORD				uwScale;					// Scale of this parameter
	struct tagPARAMITEM *lpParmrgb;		//Pointer to RgbValue
	struct tagPARAMITEM *lpParmpcb;		//pointer to pcb value
	} PARAMITEM, *lpPARAMITEM;


//*------------------------------------------------------------------------
//| DIALOGDESC:
//|	This structure pulls together a parameter list and control descs
//|	to create a template for a dialog box.
//*------------------------------------------------------------------------
typedef struct tagDIALOGDESC {
	// Run-time status
	UDWORD				uRunOptions;			// Indicates how we handle the dialog
	BOOL					fCancelled;				// TRUE if use pressed CANCEL, FALSE if OK

	// Template description and memory
	int					iFuncDex;				// Function index
	lpVOID				lpTemplate;				// Memory for the item
	LPTSTR				szFuncName;				// Name of the function
	lpCONNECTIONINFO	ci;						// Connection window

	// Control information
	UINT					cCtlItems;				// Number of controls
	lpCONTROL *			lpCtl;					// Array of control descriptions

	// Parameter list
	UINT					cParms;					// count of paramteres
	lpPARAMITEM *		lpParms;					// Array of parameters
	} DIALOGDESC;
typedef DIALOGDESC * lpDIALOGDESC;


// Following describes the function handler for each ODBC function
typedef RETCODE (PASCAL *lpFUNCHNDLR)(LPTSTR, UINT, lpCONNECTIONINFO, lpPARAMITEM *, UDWORD, LPVOID);


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN SetTabs(LPTSTR str, SWORD cTabs);
BOOL INTFUN DialogManager(lpCONNECTIONINFO ci, int dex, UDWORD uRunOptions,
			UINT uTimeOut);
BOOL INTFUN szBufferParmInfo(BOOL * pfFlushed, LPTSTR str, HWND hwnd);
BOOL INTFUN LogParametersSpecial(LPTSTR szFuncName, lpPARAMITEM * lpParmArray, UINT cParms,
			lpCONNECTIONINFO ci, BOOL fIn, UDWORD uValInfo, SDWORD iParmDft, SDWORD iParmNum);
BOOL INTFUN FormatSpecialParameters(lpDFTARRAY lpDfts, UINT cDfts, PTR *lpData, PTR *lpLinkData,
			HWND hwndOut, UDWORD uValInfo, LPTSTR *plpOutParms, LPTSTR *plpstr, BOOL *pfFlushed,
			LPTSTR szName);
BOOL INTFUN FormatValueBasedOnType(LPTSTR szOut, DWORD cbOut,
			LPVOID lpData, SWORD fCType, UDWORD cBytes,HWND hwndOut);
VOID INTFUN LogReturnCode(LPTSTR szFuncName, lpCONNECTIONINFO ci, RETCODE rc);
VOID INTFUN LogInstallerReturnCode(LPTSTR szFuncName, lpCONNECTIONINFO ci, BOOL rRc);
VOID INTFUN LogInstallerReturnCode30(LPTSTR szFuncName, lpCONNECTIONINFO ci, RETCODE rc);
BOOL INTFUN ToolOptionsAllErrors(lpCONNECTIONINFO ci, HENV henv, HDBC hdbc, HSTMT hstmt,
											SQLHDESC hdesc,RETCODE rc);
BOOL INTFUN LogErrorsToOutputWindow(lpCONNECTIONINFO ci, HENV henv, HDBC hdbc, HSTMT hstmt,
											SQLHDESC hdesc);
BOOL INTFUN EnumerateErrors30(LPTSTR szType, HWND hwndOut, HWND hwndOutWin,
					SQLSMALLINT HandleType,SQLHANDLE hHandle);
UINT INTFUN TruncationCheck(RETCODE rc, HWND hwndOut, LPTSTR szBuff,
					SDWORD cbMaxBuff, SDWORD cbBuff, LPTSTR szFieldName);
BOOL INTFUN BuildOutputParm(lpPARAMITEM lpParm, UDWORD cbMaxOut, BOOL fBuffCheck);
LPVOID AllocOutputParm(UDWORD cbMaxOut, LPCTSTR szName,
			UDWORD * pcBytes, BOOL fBuffCheck);
LPTSTR INTFUN GetExpandedConstant(LPTSTR szOut, lpDFTARRAY lpDft, BOOL fAddVersion);
BOOL INTFUN IsValidInt(SWORD fCType, LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName);
BOOL INTFUN IsValidInterval(SWORD fCType, LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName);
BOOL INTFUN IsValidBit(SWORD fCType, LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName);
LPTSTR INTFUN FormatFromDftArray(LPTSTR szOut, lpPARAMITEM lpParm);
lpPARAMETERINFO INTFUN SearchParamList(lpSTATEMENTINFO lpsi, UWORD ipar);
BOOL INTFUN IsValidDate(LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName);
BOOL INTFUN IsValidTime(LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName, BOOL fFrac);
BOOL INTFUN IsValidTimestamp(LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName);
BOOL INTFUN IsValidFloat(SWORD fCType, LPTSTR lpStr, BOOL fVocal, LPTSTR szFieldName);
BOOL INTFUN FormatHandle(LPTSTR szOutBuff, BOOL fNull, UDWORD uParmType,
				LPTSTR szFieldName,
				HENV * phenv, HDBC * phdbc,
				HSTMT * phstmt,SQLHDESC *phdes,
				UINT uNum,SQLHANDLE hActHandle);
UINT INTFUN GetDftStyleCount(UDWORD uStyle);
UDWORD INTFUN IsChoiceCBODefault(HWND hwnd, lpCONTROL lpCtl,lpPARAMITEM lpParm);
SWORD INTFUN GetCTypeFromSqlType(SWORD fSqlType,BOOL fSigned);
SIZE_T INTFUN GetSizeOfCType(SWORD fCType, SIZE_T cbMax);


VOID INTFUN InitUnModified(PTR lpBytes, UDWORD udchk);
BOOL INTFUN UnModified(PTR lpBytes, UDWORD udchk);
BOOL INTFUN UnusedBufferCheck(lpCONNECTIONINFO ci, LPCTSTR BufName, LPCTSTR CBName,
		void * buf, UDWORD bufMax, UDWORD *pBufuParm,  SDWORD cb, BOOL fLastChec, BOOL fVocal);
BOOL INTFUN OutputParamCheck(lpCONNECTIONINFO ci, RETCODE rc,
		lpPARAMITEM * lpParms, UINT cParms, BOOL fVocal);
BOOL INTFUN InstallerOutputParamCheck(lpCONNECTIONINFO ci, BOOL fSuccess,
		lpPARAMITEM * lpParms, UINT cParms, BOOL fVocal);
SDWORD INTFUN PtrValue(lpPARAMITEM lpParm);
BOOL INTFUN NullTerminationCheck(lpCONNECTIONINFO ci, LPCTSTR BufName,
		LPCTSTR buf, UDWORD bufMax, SDWORD cb, UDWORD fDouble, SDWORD cbValue);
BOOL INTFUN ConvertToTypeFromString(LPTSTR str, lpPARAMITEM lpParm, SWORD fCType, SDWORD cbLen);
PTR INTFUN ConvertToTypeFromChar(LPTSTR str, SWORD fCType, SDWORD cbLen,UDWORD *pcBytes,
											UWORD uwPrecision,UWORD uwScale);
PTR INTFUN ConvertToTypeFromHex(LPTSTR str, SWORD fCType, SDWORD cbLen,UDWORD *pcBytes,
						 UDWORD uParmType,UWORD uwPrecision,UWORD uwScale);
LPTSTR INTFUN GetInputWindowString(HWND hwnd, UDWORD * pcbLen);
BOOL INTFUN IsValidHex(SWORD fCType, LPTSTR lpStr, BOOL fVocal,
		LPTSTR szFieldNameIn, UDWORD *lpBytes);
LPTSTR INTFUN GetEditWindowString(HWND hwnd, DWORD * pdwLen,UINT uMsg);
SDWORD INTFUN AddHandle(HWND hwndCtl,SQLHANDLE hHandle,LPTSTR szStr,
										UINT uNum,LPTSTR szClientTitle,LPTSTR szDesc);
UDWORD INTFUN GetDisplaySizeBasedOnType(SWORD fCType, UDWORD cbMax);
VOID INTFUN RemoveDlgFormat(LPTSTR str);
BOOL INTFUN GetCharValueFromParm(LPTSTR szOut, DWORD cbOut, lpPARAMITEM lpParm,
			BOOL fIn,
			HENV * phenv, HDBC * phdbc,
			HSTMT * phstmt, SQLHDESC *phdesc,
			UINT uStmtNum,HWND hwndOut);
BOOL EatWhiteSpace(LPTSTR str);
BOOL EatSpace(LPTSTR str);

VOID INTFUN UpdateDescComboBox(HWND hDlg,WORD wID,UINT cDescs,lpDESCRIPTORHEADER lpDesc);
UINT INTFUN GetIndexFromControlID(lpDIALOGDESC lpDlg, UINT uID);

//Moved from hexview.h
// Do pointer arithmetic inline to increase speed
__inline VOID * FindAddress(PTR lpAddr, UDWORD itop,
					UDWORD irow, UDWORD cbOffset)
{
	LPBYTE		Addr;

	Addr = (LPBYTE)lpAddr;
	Addr += ((itop + irow) * cbOffset);
	return (PTR)(Addr);
}

#endif

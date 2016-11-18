//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	BLDDATA.H
//|
//| Purpose:
//|		Contains prototypes for functions in BLDDATA.C
//*---------------------------------------------------------------------------------
#ifndef BLDDATA_DEFS
#define BLDDATA_DEFS


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
BOOL INTFUN GetCharValueFromParm(LPTSTR szOut, DWORD cbOut, lpPARAMITEM lpParm,
			BOOL fIn,
			HENV * phenv, HDBC * phdbc,
			HSTMT * phstmt, SQLHDESC *phdesc,
			UINT uStmtNum,
			HWND hwndOut);
BOOL INTFUN ValidateEditWindowByType(HWND hDlg, int id, LPTSTR szFieldName, SWORD fCType);
VOID INTFUN RemoveDlgFormat(LPTSTR str);
VOID INTFUN InitUnModified(PTR lpBytes, UDWORD udchk);
BOOL INTFUN ValidateTypeData(LPTSTR szFieldName, SWORD fCType, BOOL fVocal, LPTSTR lpStr);
//sumit
BOOL INTFUN IsValidateFlagSet(HWND hDlg,lpPARAMITEM lpParm, lpCONTROL lpCtl);



//*---------------------------------------------------------------------------------
//| IsHexString:
//|	This function will return TRUE if lpString is in the form of
//|	"0x1234567"
//| Parms:
//|	lpString		string to examine
//| Returns:
//|	TRUE if the string is a hex string otherwise FALSE
//*---------------------------------------------------------------------------------
_inline BOOL INTFUN IsHexString(LPTSTR lpString)
{
	LPCTSTR	lpHex=TEXT("0x");

	if (!lpString)
		return (FALSE);

	return (!_tcsnicmp(lpString,lpHex,lstrlen(lpHex)));

} //IsHexString()



#endif

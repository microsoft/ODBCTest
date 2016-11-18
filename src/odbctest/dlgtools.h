//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGTOOLS.H
//|
//| Purpose:
//|		Contains prototypes for functions in DLGTOOLS.H
//*---------------------------------------------------------------------------------
#ifndef DLGTOOLS_DEFS
#define DLGTOOLS_DEFS

//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
BOOL INTFUN ValidateParameters(HWND hDlg, lpDIALOGDESC lpDlg);
BOOL INTFUN ParameterBuilder(HWND hDlg, lpDIALOGDESC lpDlg);
BOOL INTFUN FormatCheckBox(LPTSTR szOutBuff, BOOL fNull);
BOOL INTFUN DoubleToChar(LPTSTR szOut,SDOUBLE psdouble, int iprec);


#endif

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGPARMS.H
//|
//| Purpose:
//|		Defines some utility functions used to return information about
//|		parameters.
//*---------------------------------------------------------------------------------
#ifndef DLGPARMS_DEFS
#define DLGPARMS_DEFS

//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
int INTFUN GetApiCount();
lpFUNCHNDLR GetFunctionHandler(UINT dex);
LPTSTR INTFUN GetApiName(UINT dex, LPTSTR str);
lpPARAMITEM * GetParmInfo(UINT dex, UINT * pNumParms);
LPTSTR INTFUN GetApiConstant(UINT dex, LPTSTR str);

#endif

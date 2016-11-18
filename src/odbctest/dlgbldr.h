//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGBLDR.H
//|
//| Purpose:
//|		Prototypes and defines for Dialog Builder.
//*---------------------------------------------------------------------------------
#ifndef DLGBLDR_DEFS
#define DLGBLDR_DEFS

#include <Windows.h>
#include "gtrcomm.h"
#include "portable.h"
#include "standard.h"
#include "dlgmngr.h"


// Control styles, Note that 0x99 is translated to DLG_BUTTON with style BS_AUTOCHECKBOX
#define DLG_BUTTON		0x0080
#define DLG_EDIT			0x0081
#define DLG_STATIC		0x0082
#define DLG_LISTBOX		0x0083
#define DLG_SCROLLBAR	0x0084
#define DLG_COMBO			0x0085
#define DLG_CKBOX			0x0099


//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
lpDIALOGDESC DialogBuilder(int dex);
VOID INTFUN DestroyDialogs(UINT index);
VOID SetParamPtrs(lpDIALOGDESC lpDlg );


#endif

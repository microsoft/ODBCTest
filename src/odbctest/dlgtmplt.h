//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGTMPLT.H
//|
//| Purpose:
//|		Defines some utility functions used to return information about
//|		parameters.
//*---------------------------------------------------------------------------------
#ifndef DLGTMPLT_DEFS
#define DLGTMPLT_DEFS

#include <windows.h>
#include "gtrcomm.h"
#include "portable.h"
#include "standard.h"

#define NUM_CONTROL_BUTTONS 3

//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
lpCONTROL * GetControlInfo(int dex, UINT * pNumCtls);


#endif

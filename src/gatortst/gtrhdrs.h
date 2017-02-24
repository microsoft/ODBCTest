//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft Corporation
//|
//| Title:	Headers.h
//|
//| Purpose:
//|		ODBCTest precompiled headers
//|
//*---------------------------------------------------------------------------------


#ifndef WINVER
#define WINVER  0x0400
#endif

#ifndef RC_INVOKED
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <tchar.h>
#include <string.h>
#ifndef RC_INVOKED
#include <stdio.h>
#include <commdlg.h>
#endif
#include "portable.h"
#include "standard.h"
#include "sql.h"
#include "sqlext.h"
#include "bitarray.h"
#include "autotest.h"
#include "commtest.h"
#include "udl.h"
#include "dllpick.h"
#include "grppick.h"
#include "srcpick.h"
#include "runtest.h"

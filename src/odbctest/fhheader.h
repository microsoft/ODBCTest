//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
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
#include <windowsx.h>		// message crackers
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <tchar.h>
#include <ctype.h>
#include <stdarg.h>
#include <rpc.h>
#include <rpcdce.h>
#include "standard.h"
#include "portable.h"
#include "gtrcomm.h"
#include "dlgmngr.h"
#include "dlgtools.h"
#include "dlgtmplt.h"
#include "dlgparms.h"
#include "connwin.h"
#include "commdlg.h"
#endif

#include "options.h"

#ifndef RC_INVOKED
#include "bitarray.h"
#include "dlgbldr.h"
#include "fileio.h"
#endif

#include "funchndl.h"
#include "gatortst.h"
#include "globals.h"
#include "handles.h"
#include "mem.h"
#include "menus.h"
#include "outwin.h"
#include "rsltwin.h"
#include "statbar.h"
#include "strings.h"
#include "sql.h"
#include "sqlext.h"
#include "sqlucode.h"
#include "odbcss.h"
#include "toolbar.h"
#include "file.h"
#include "paramidx.h"


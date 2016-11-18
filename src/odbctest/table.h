//*---------------------------------------------------------------------------------
//|  Title:		MAKETBL.H
//|	Copyright (c) Microsoft, All rights reserved
//|
//|	MAKETBL is part of the common test functions which are not part of the
//|	retail build.
//*---------------------------------------------------------------------------------
#ifndef MAKETBL_DEFS
#define MAKETBL_DEFS


//----------------------------------------------------------------------------------
//              Defines and Macros
//----------------------------------------------------------------------------------
#ifndef VSZFile
#define VSZFile static TCHAR vszFile[]=__FILE__;
#endif


#define MAXTYPENAMES 30
#define QP_MAXSTRINGSIZE 40
#define QP_MAXSIZE 20
#define AUTO_MAKE_TABLE	TRUE

#define QUIET_MODE		1

//Used by MakeGenderTable()
#define GIRLS_TABLE		0
#define BOYS_TABLE		1


// Driver Name Bitmasks
#define DRVR_SQLSRVR                            0x00000001L
#define DRVR_SIMBA                              0x00000002L
#define DRVR_ORACLE                             0x00000004L
#undef  DRVR_QJET
#define DRVR_QJET                                       0x00000008L
#define DRVR_ODS                                        0x00000010L



// for GetHdbState & Disc
#define UNALLOCATED_HENV 99
#define ALLOCATED_HENV     1
#define ALLOCATED_HDBC     2
#define CONNECTED_HDBC     3
#define FREE_HENV                  1
#define DONT_FREE_HENV     0
#define NEW_HENV                   1
#define SAME_HENV                  0
// used in GetHstmtState
#define ALLOCATED_HSTMT   5
#define PREPARED_HSTMT    6
#define EXECUTED_HSTMT    7
#define POSITIONED_HSTMT  8
#define TABLE_EXISTS     13

#define DLL_UNKNOWN      -1
#define DLL_SQLSRVR       1
#define DLL_SQORA         2
#define DLL_SIMBA         4

// For GATORSTR.DLL
extern HINSTANCE hGatorStrInst;

//extern dCSEG(TCHAR) sz_Returned[];


#define IH_MAX_RC 10


#define MAX_TYPE                                                         129
#define         MAX_PREFIX                                          129
#define MAX_SUFFIX                                               129
#define MAX_PARAMS                                               129
#define MAX_LENGTH_BUF                                    11
#define MAX_SZ_SIZE                                              4048
#define         TABLENAMESIZE                                     31
#define MAX_TYPES                                                 29

#define MAX_VALUE 512

typedef struct FieldInfo {
		TCHAR		szType[MAX_TYPE];
		TCHAR		szFieldName[MAX_TYPE + 1];
		int		iFieldNum;
		SWORD		wSQLType;
		TCHAR		szPrefix[MAX_PREFIX];
		TCHAR		szSuffix[MAX_SUFFIX];
		TCHAR		szParams[MAX_PARAMS];
		TCHAR		szColDef[SMALLBUFF];	//Full column definition (i.e. "col1 TCHAR(255)")
		TCHAR		szLength[MAX_LENGTH_BUF];
		SQLULEN	precision;
		UDWORD		scale;
		UDWORD	precisionUsed;
		SWORD		scaleUsed;
		SWORD		nullable;
		SWORD		UseNulls;
		SQLLEN	length;
		int		fAutoUpdate;
		SQLLEN	fSearchable;
		SQLLEN	fUnsigned;
		UWORD		fAutoInc;
		BOOL		fUse;
		} FIELDINFO,  *lpFIELDINFO;

#define MAX_PARAM 30
#define MAXSTR 45

typedef struct tagQtStruct {
	TCHAR    sz[MAX_SZ_SIZE];
	TCHAR    szParam[MAX_PARAM];
	TCHAR    szDataItem[MAXSTR];
	TCHAR    szTableName[TABLENAMESIZE];
	TCHAR    szColNames[500];
	TCHAR    szValues[20 * MAX_TYPES];
} QTSTRUCT,  *lpQTSTRUCT;


#define MAX_IDS_STRING_LEN 80
#define LEVEL_AREA 1
#define LEVEL_SUBAREA 2
#define LEVEL_TEST 3
#define LEVEL_COMMENT 3
#define LEVEL_ERROR 3
#define LEVEL_FILE 3
#define LEVEL_PASSED 3
//int failed;
int DEBUG_MSG;

typedef TCHAR VALUESARRAY[MAX_TYPES][MAX_VALUE+1];

// Only declare this if we are compiling GATORTST.DLL
#ifdef DLL_INVOKED
jmp_buf  env;
#endif


#define DROPPED_TABLE                   0
#define DELETED_ROWS                            1
#define UNIQUE_INDEX                    1
#define CLUSTERED_INDEX                 2
#define STANDARD_INDEX                  3
#define FORCE                           1
#define NOFORCE                         0


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#ifdef WIN32
#define HUGE
#else
#define HUGE                _huge
#endif

typedef BYTE HUGE * HPBYTE;

#define MAX_CREATED_DATA_LEN 40
#define MAX_SCALE 5
typedef struct tagParamS {
	TCHAR pbs[MAX_TYPES][MAX_CREATED_DATA_LEN];
	SQLLEN cbValue[MAX_TYPES];
}	PSTRUCT,  *lpPSTRUCT;


#define TABLE_NORMAL	0
#define TABLE_EXT		1
#define TABLE_PATH		2
#define TABLE_PATH_EXT	3
#define TABLE_SPACES	4
#define TABLE_SPECIAL	5
#define MAX_TABLE_TYPES	6


int INTFUN MakeTable(HDBC  *pHdbc,LPTSTR  szCreatedTable, FIELDINFO  *rgFields,
	UINT uSeed,int *pcTypes);

UINT INTFUN GetTypeInfo(HDBC  *pHdbc,HSTMT  *pHstmt0,lpFIELDINFO rgFields,
	UINT uSeed);

#define FSSQLColAttributes(arg1,arg2,arg3,arg4,arg5,arg6,arg7)\
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
				SQLColAttributes(arg1,arg2,arg3,arg4,arg5,arg6,arg7))

#define FSSQLFreeStmt(arg1,arg2) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL,\
				SQLFreeStmt(arg1,arg2))

#define FSSQLGetInfo(arg1, arg2,arg3,arg4,arg5) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,(arg1),NULL,NULL,	\
				SQLGetInfo(arg1, arg2,arg3,arg4,arg5))

#define FSSQLGetConnectOption(arg1,arg2,arg3) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,(arg1),NULL,NULL, \
				SQLGetConnectOption(arg1,arg2,arg3))

#define FSSQLGetTypeInfo(arg1,arg2) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
				SQLGetTypeInfo(arg1,arg2))

#define FSSQLGetData(arg1,arg2,arg3,arg4,arg5,arg6) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
				SQLGetData(arg1,arg2,arg3,arg4,arg5,arg6))

#define FSSQLColumns(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)\
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
		SQLColumns(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9))

#define FSSQLColAttributes(arg1,arg2,arg3,arg4,arg5,arg6,arg7)\
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
		SQLColAttributes(arg1,arg2,arg3,arg4,arg5,arg6,arg7))

#define FSSQLBindParameter(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
		SQLBindParameter(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10))

#define FSSQLPrepare(arg1,arg2,arg3) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
		SQLPrepare(arg1,arg2,arg3))

#define FSSQLExecute(arg1) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,NULL,(arg1),NULL, \
		SQLExecute(arg1))

#define FSSQLAllocStmt(arg1,arg2) \
	ToolOptionsAllErrors(lpActiveConnWin,NULL,(arg1),NULL,NULL, \
		SQLAllocStmt(arg1,arg2))

#endif

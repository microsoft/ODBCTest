//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGDFTS.H
//|
//| Purpose:
//|		This module contains all of the default constant arrays that are used
//|		by many functions.  These values are included via pointers by the
//|		parameter arrays found in DLGPARMS.C.  Note that token pasting is
//|		used to store the text name of the constants, but this will not
//|		keep the values in the string.  These must be formatted for output.
//*---------------------------------------------------------------------------------
#ifndef DLGDFTS_DEFS
#define DLGDFTS_DEFS
#pragma warning(disable:4995)

//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------

#define DEX_HWND			1
#define DEX_HENV			2
#define DEX_HDBC			3
#define DEX_HSTMT			4
#define DEX_HDESC			5

#define INTERVALVAL		9994
#define ROWSETVAL			9995
#define SAMEAS_STRLEN	9996
#define VALIDPTR			9997
#define EMPTYSTR			9998
#define NULLPTR			9999


dCSEG(TCHAR) szNULLHANDLE[]	= TEXT("SQL_NULL_HANDLE");
dCSEG(TCHAR) szNULLPTR[]	= TEXT("<null pointer>");
dCSEG(TCHAR) szEMPTYSTRING[]	= TEXT("<empty string>");
dCSEG(TCHAR) szDFTHENV[]	= TEXT("<henv>");
dCSEG(TCHAR) szDFTHDBC[]	= TEXT("<hdbc>");
dCSEG(TCHAR) szDFTHSTMT[]	= TEXT("<hstmt>");
dCSEG(TCHAR) szDFTHDESC[]	= TEXT("<hdesc>");
dCSEG(TCHAR) szHWND[]		= TEXT("<hwnd>");
dCSEG(TCHAR) szVALIDPTR[]	= TEXT("<valid pointer>");
dCSEG(TCHAR) szSAME_AS_STRLEN_PTR[]	= TEXT("<same as StringLengthPtr>");
dCSEG(TCHAR) szROWSETVAL[] = TEXT("<rowset value>");
dCSEG(TCHAR) szINTERVALVAL[] = TEXT("<interval value>");

#define ODBC_INI				TEXT("ODBC.INI")
#define ODBCINST_INI			TEXT("ODBCINST.INI")



// Following macro pastes sz in front of the constant to declare a code seg
//		based string.
#define DeclareString(str) dCSEG(TCHAR) sz ## str [] = TEXT(#str)


// This macro will create a UDWORD which contains the major revision as the
//		HIWORD and the minor revision as the LOWORD
#define MAKEVERSION(major, minor) (UDWORD)(MAKELONG(major, minor))


// Following macros will initialize a default array as version 1.0 (and higher)
//		values.  NoS has no linked array, Sub does
#define InitDftNoSArray10(val,typ)		(SDWORD)val, typ, sz ## val, MAKEVERSION(1,0), 0, 0, NULL
#define InitDftSubArray10(val,typ,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(1,0), 0, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArrayVal10(val,typ,uInfo)		(SDWORD)val, typ, sz ## val, MAKEVERSION(1,0), (UDWORD)uInfo, 0, NULL
#define InitDftSubArrayVal10(val,typ,uInfo,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(1,0), (UDWORD)uInfo, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArray20(val,typ)		(SDWORD)val, typ, sz ## val, MAKEVERSION(2,0), 0, 0, NULL
#define InitDftSubArray20(val,typ,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(2,0), 0, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArrayVal20(val,typ,uInfo)		(SDWORD)val, typ, sz ## val, MAKEVERSION(2,0), (UDWORD)uInfo, 0, NULL
#define InitDftSubArrayVal20(val,typ,uInfo,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(2,0), (UDWORD)uInfo, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArray25(val,typ)		(SDWORD)val, typ, sz ## val, MAKEVERSION(2,5), 0, 0, NULL
#define InitDftSubArray25(val,typ,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(2,5), 0, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArrayVal25(val,typ,uInfo)		(SDWORD)val, typ, sz ## val, MAKEVERSION(2,5), (UDWORD)uInfo, 0, NULL
#define InitDftSubArrayVal25(val,typ,uInfo,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(2,5), (UDWORD)uInfo, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArray30(val,typ)		(SDWORD)val, typ, sz ## val, MAKEVERSION(3,0), 0, 0, NULL
#define InitDftSubArray30(val,typ,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(3,0), 0, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArrayVal30(val,typ,uInfo)		(SDWORD)val, typ, sz ## val, MAKEVERSION(3,0), (UDWORD)uInfo, 0, NULL
#define InitDftSubArrayVal30(val,typ,uInfo,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(3,0), (UDWORD)uInfo, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArray40(val,typ)		(SDWORD)val, typ, sz ## val, MAKEVERSION(4,0), 0, 0, NULL
#define InitDftSubArray40(val,typ,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(4,0), 0, NumItems(sub), (lpDFTARRAY)sub

#define InitDftNoSArrayVal40(val,typ,uInfo)		(SDWORD)val, typ, sz ## val, MAKEVERSION(4,0), (UDWORD)uInfo, 0, NULL
#define InitDftSubArrayVal40(val,typ,uInfo,sub)	(SDWORD)val, typ, sz ## val, MAKEVERSION(4,0), (UDWORD)uInfo, NumItems(sub), (lpDFTARRAY)sub

// This is a hack -- define these so that can be used with or without SQL_ALL_TYPES below
// Core SQL datatypes:
#define SqlTypesInfo	\
	InitDftNoSArray10(SQL_CHAR,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_BIGINT,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_BINARY,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_BIT,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_DATE,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_DECIMAL,						SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_DEFAULT,						SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_DOUBLE,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_FLOAT,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_GUID,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_INTEGER,						SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_DAY,					SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_DAY_TO_HOUR,		SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_DAY_TO_MINUTE,	SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_DAY_TO_SECOND,	SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_HOUR,				SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_HOUR_TO_MINUTE,	SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_HOUR_TO_SECOND,	SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_MINUTE,				SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_MINUTE_TO_SECOND,SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_INTERVAL_MONTH,				SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_SECOND,				SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_YEAR,				SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_INTERVAL_YEAR_TO_MONTH,	SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_LONGVARCHAR,					SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_LONGVARBINARY,				SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_NUMERIC,						SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_REAL,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_SMALLINT,						SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_TINYINT,						SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_TIME,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_TIMESTAMP,						SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_TYPE_DATE,						SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_TYPE_TIME,						SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_TYPE_TIMESTAMP,				SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_VARBINARY,						SQL_C_SSHORT),	\
	InitDftNoSArray10(SQL_VARCHAR,						SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_WCHAR,							SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_WLONGVARCHAR,					SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_WVARCHAR,						SQL_C_SSHORT),  \
	InitDftNoSArray30(SQL_VARIANT_TYPE,					SQL_C_SSHORT),  \
	InitDftNoSArray30(SQL_ARRAY,						SQL_C_SSHORT),  \
	InitDftNoSArray30(SQL_MULTISET,						SQL_C_SSHORT),  \
    InitDftNoSArray30(SQL_ROW,						    SQL_C_SSHORT),  \
	InitDftNoSArray30(SQL_UDT,						    SQL_C_SSHORT)




/*
#if ODBCVER >= 0x350
	InitDftNoSArray30(SQL_BLOB,							SQL_C_SSHORT),	\
	InitDftNoSArray30(SQL_CLOB,							SQL_C_SSHORT),	\
#endif
*/

//------------------------------------------------------------------------------------------------
// Declare the constants:
//		Note that it is not possible to declare a table with a string in the data segment, it
//		must therefore be declared separately and referred to by name.  Place all of these
//		values in the code segment reduces overall code size.
//------------------------------------------------------------------------------------------------

/* generally useful constants */
DeclareString(TRUE);
DeclareString(FALSE);
DeclareString(SQL_NTS);
DeclareString(SQL_IS_POINTER);
DeclareString(SQL_IS_UINTEGER);
DeclareString(SQL_IS_INTEGER);
DeclareString(SQL_IS_USMALLINT);
DeclareString(SQL_IS_SMALLINT);
DeclareString(SQL_SQLSTATE_SIZE);
DeclareString(SQL_MAX_MESSAGE_LENGTH);
DeclareString(SQL_MAX_DSN_LENGTH);

/* RETCODEs */
DeclareString(SQL_ERROR);
DeclareString(SQL_INVALID_HANDLE);
DeclareString(SQL_NEED_DATA);
DeclareString(SQL_NO_DATA_FOUND);
DeclareString(SQL_SUCCESS);
DeclareString(SQL_SUCCESS_WITH_INFO);
DeclareString(SQL_METADATA_CHANGED);
DeclareString(SQL_DATA_AVAILABLE);

//Installer RETCODE's
DeclareString(ODBC_ERROR_GENERAL_ERR);
DeclareString(ODBC_ERROR_INVALID_BUFF_LEN);
DeclareString(ODBC_ERROR_INVALID_HWND);
DeclareString(ODBC_ERROR_INVALID_STR);
DeclareString(ODBC_ERROR_INVALID_REQUEST_TYPE);
DeclareString(ODBC_ERROR_COMPONENT_NOT_FOUND);
DeclareString(ODBC_ERROR_INVALID_NAME);
DeclareString(ODBC_ERROR_INVALID_KEYWORD_VALUE);
DeclareString(ODBC_ERROR_INVALID_DSN);
DeclareString(ODBC_ERROR_INVALID_INF);
DeclareString(ODBC_ERROR_REQUEST_FAILED);
DeclareString(ODBC_ERROR_INVALID_PATH);
DeclareString(ODBC_ERROR_LOAD_LIB_FAILED);
DeclareString(ODBC_ERROR_INVALID_PARAM_SEQUENCE);
DeclareString(ODBC_ERROR_INVALID_LOG_FILE);
DeclareString(ODBC_ERROR_REMOVE_DSN_FAILED);
DeclareString(ODBC_ERROR_USAGE_UPDATE_FAILED);
DeclareString(ODBC_ERROR_WRITING_SYSINFO_FAILED);
DeclareString(ODBC_ERROR_USER_CANCELED);
DeclareString(ODBC_ERROR_OUT_OF_MEM);

DeclareString(ODBC_USER_DSN);
DeclareString(ODBC_SYSTEM_DSN);
DeclareString(ODBC_BOTH_DSN);


/* SQLFreeStmt defines */
DeclareString(SQL_CLOSE);
DeclareString(SQL_DROP);
DeclareString(SQL_UNBIND);
DeclareString(SQL_RESET_PARAMS);

/* SQLSetParam defines */
DeclareString(SQL_C_DEFAULT);

/* SQLTransact defines */
DeclareString(SQL_COMMIT);
DeclareString(SQL_ROLLBACK);

/* SQLAllocHandle defines */
DeclareString(SQL_HANDLE_SENV);
DeclareString(SQL_HANDLE_ENV);
DeclareString(SQL_HANDLE_DBC);
DeclareString(SQL_HANDLE_STMT);
DeclareString(SQL_HANDLE_DESC);
DeclareString(SQL_NULL_HANDLE);

//Locators
DeclareString(SQL_LOCATOR_HOLD);
DeclareString(SQL_LOCATOR_FREE);

DeclareString(SQL_SQL92_DATETIME_FUNCTIONS);
DeclareString(SQL_ISO_DATETIME_FUNCTIONS);
DeclareString(SQL_SDF_CURRENT_DATE);
DeclareString(SQL_SDF_CURRENT_TIME);
DeclareString(SQL_SDF_CURRENT_TIMESTAMP);

DeclareString(SQL_SQL92_FOREIGN_KEY_DELETE_RULE);
DeclareString(SQL_ISO_FOREIGN_KEY_DELETE_RULE);
DeclareString(SQL_SFKD_CASCADE);
DeclareString(SQL_SFKD_NO_ACTION);
DeclareString(SQL_SFKD_SET_DEFAULT);
DeclareString(SQL_SFKD_SET_NULL);

DeclareString(SQL_SQL92_FOREIGN_KEY_UPDATE_RULE);
DeclareString(SQL_ISO_FOREIGN_KEY_UPDATE_RULE);
DeclareString(SQL_SFKU_CASCADE);
DeclareString(SQL_SFKU_NO_ACTION);
DeclareString(SQL_SFKU_SET_DEFAULT);
DeclareString(SQL_SFKU_SET_NULL);

DeclareString(SQL_SQL92_GRANT);
DeclareString(SQL_ISO_GRANT);
DeclareString(SQL_SG_USAGE_ON_DOMAIN);
DeclareString(SQL_SG_USAGE_ON_CHARACTER_SET);
DeclareString(SQL_SG_USAGE_ON_COLLATION);
DeclareString(SQL_SG_USAGE_ON_TRANSLATION);
DeclareString(SQL_SG_WITH_GRANT_OPTION);
DeclareString(SQL_SG_DELETE_TABLE);
DeclareString(SQL_SG_INSERT_TABLE);
DeclareString(SQL_SG_INSERT_COLUMN);
DeclareString(SQL_SG_REFERENCES_TABLE);
DeclareString(SQL_SG_REFERENCES_COLUMN);
DeclareString(SQL_SG_SELECT_TABLE);
DeclareString(SQL_SG_UPDATE_TABLE);
DeclareString(SQL_SG_UPDATE_COLUMN);

DeclareString(SQL_SQL92_NUMERIC_VALUE_FUNCTIONS);
DeclareString(SQL_ISO_NUMERIC_VALUE_FUNCTIONS);
DeclareString(SQL_SNVF_BIT_LENGTH);
DeclareString(SQL_SNVF_CHAR_LENGTH);
DeclareString(SQL_SNVF_CHARACTER_LENGTH);
DeclareString(SQL_SNVF_EXTRACT);
DeclareString(SQL_SNVF_OCTET_LENGTH);
DeclareString(SQL_SNVF_POSITION);

DeclareString(SQL_SQL92_PREDICATES);
DeclareString(SQL_ISO_PREDICATES);
DeclareString(SQL_SP_EXISTS);
DeclareString(SQL_SP_ISNOTNULL);
DeclareString(SQL_SP_ISNULL);
DeclareString(SQL_SP_MATCH_FULL);
DeclareString(SQL_SP_MATCH_PARTIAL);
DeclareString(SQL_SP_MATCH_UNIQUE_FULL);
DeclareString(SQL_SP_MATCH_UNIQUE_PARTIAL);
DeclareString(SQL_SP_OVERLAPS);
DeclareString(SQL_SP_UNIQUE);
DeclareString(SQL_SP_LIKE);
DeclareString(SQL_SP_IN);
DeclareString(SQL_SP_BETWEEN);
DeclareString(SQL_SP_COMPARISON);
DeclareString(SQL_SP_QUANTIFIED_COMPARISON);


DeclareString(SQL_SQL92_RELATIONAL_JOIN_OPERATORS);
DeclareString(SQL_ISO_RELATIONAL_JOIN_OPERATORS);
DeclareString(SQL_SRJO_CORRESPONDING_CLAUSE);
DeclareString(SQL_SRJO_CROSS_JOIN);
DeclareString(SQL_SRJO_EXCEPT_JOIN);
DeclareString(SQL_SRJO_FULL_OUTER_JOIN);
DeclareString(SQL_SRJO_INNER_JOIN);
DeclareString(SQL_SRJO_INTERSECT_JOIN);
DeclareString(SQL_SRJO_LEFT_OUTER_JOIN);
DeclareString(SQL_SRJO_NATURAL_JOIN);
DeclareString(SQL_SRJO_RIGHT_OUTER_JOIN);
DeclareString(SQL_SRJO_UNION_JOIN);

DeclareString(SQL_SQL92_REVOKE);
DeclareString(SQL_ISO_REVOKE);
DeclareString(SQL_SR_USAGE_ON_DOMAIN);
DeclareString(SQL_SR_USAGE_ON_CHARACTER_SET);
DeclareString(SQL_SR_USAGE_ON_COLLATION);
DeclareString(SQL_SR_USAGE_ON_TRANSLATION);
DeclareString(SQL_SR_GRANT_OPTION_FOR);
DeclareString(SQL_SR_CASCADE);
DeclareString(SQL_SR_RESTRICT);
DeclareString(SQL_SR_DELETE_TABLE);
DeclareString(SQL_SR_INSERT_TABLE);
DeclareString(SQL_SR_INSERT_COLUMN);
DeclareString(SQL_SR_REFERENCES_TABLE);
DeclareString(SQL_SR_REFERENCES_COLUMN);
DeclareString(SQL_SR_SELECT_TABLE);
DeclareString(SQL_SR_UPDATE_TABLE);
DeclareString(SQL_SR_UPDATE_COLUMN);

DeclareString(SQL_SQL92_ROW_VALUE_CONSTRUCTOR);
DeclareString(SQL_ISO_ROW_VALUE_CONSTRUCTOR);
DeclareString(SQL_SRVC_VALUE_EXPRESSION);
DeclareString(SQL_SRVC_NULL);
DeclareString(SQL_SRVC_DEFAULT);
DeclareString(SQL_SRVC_ROW_SUBQUERY);

DeclareString(SQL_SQL92_STRING_FUNCTIONS);
DeclareString(SQL_ISO_STRING_FUNCTIONS);
DeclareString(SQL_SSF_CONVERT);
DeclareString(SQL_SSF_LOWER);
DeclareString(SQL_SSF_UPPER);
DeclareString(SQL_SSF_SUBSTRING);
DeclareString(SQL_SSF_TRANSLATE);
DeclareString(SQL_SSF_TRIM_BOTH);
DeclareString(SQL_SSF_TRIM_LEADING);
DeclareString(SQL_SSF_TRIM_TRAILING);
DeclareString(SQL_SSF_OVERLAY);
DeclareString(SQL_SSF_LENGTH);
DeclareString(SQL_SSF_POSITION);
DeclareString(SQL_SSF_CONCAT);


DeclareString(SQL_SQL92_VALUE_EXPRESSIONS);
DeclareString(SQL_ISO_VALUE_EXPRESSIONS);
DeclareString(SQL_SVE_CASE);
DeclareString(SQL_SVE_CAST);
DeclareString(SQL_SVE_COALESCE);
DeclareString(SQL_SVE_NULLIF);


DeclareString(SQL_DB_RETURN_TO_POOL);
DeclareString(SQL_DB_DISCONNECT);
DeclareString(SQL_DB_DEFAULT);

/* values for SQL_ATTR_ENLIST_IN_DTC */
DeclareString(SQL_DTC_DONE);

//SQLSetStmtAttr defines
DeclareString(SQL_ATTR_APP_ROW_DESC);
DeclareString(SQL_ATTR_APP_PARAM_DESC);
DeclareString(SQL_ATTR_ASYNC_ENABLE);
DeclareString(SQL_ATTR_AUTO_IPD);
DeclareString(SQL_ATTR_BIND_OFFSET);
DeclareString(SQL_ATTR_BIND_TYPE);
DeclareString(SQL_ATTR_CONCURRENCY);
DeclareString(SQL_ATTR_CONNECTION_TIMEOUT);
DeclareString(SQL_ATTR_DISCONNECT_BEHAVIOR);
DeclareString(SQL_ATTR_DYNAMIC_COLUMNS);
DeclareString(SQL_ATTR_ENLIST_IN_DTC);
DeclareString(SQL_ATTR_ENLIST_IN_XA);
DeclareString(SQL_ATTR_CURSOR_TYPE);
DeclareString(SQL_ATTR_ENABLE_AUTO_IPD);
DeclareString(SQL_ATTR_FETCH_BOOKMARK_PTR);
DeclareString(SQL_ATTR_IMP_ROW_DESC);
DeclareString(SQL_ATTR_IMP_PARAM_DESC);
DeclareString(SQL_ATTR_KEYSET_SIZE);
DeclareString(SQL_ATTR_MAX_LENGTH);
DeclareString(SQL_ATTR_MAX_ROWS);
DeclareString(SQL_ATTR_METADATA_ID);
DeclareString(SQL_ATTR_NOSCAN);
DeclareString(SQL_ATTR_PARAM_BIND_TYPE);
DeclareString(SQL_ATTR_PARAM_STATUS_PTR);
DeclareString(SQL_ATTR_PARAMSET_SIZE);
#ifdef SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR
DeclareString(SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR);
#endif
#ifdef SQL_ATTR_PREDICATE_PTR
DeclareString(SQL_ATTR_PREDICATE_PTR);
#endif
DeclareString(SQL_ATTR_QUERY_TIMEOUT);
DeclareString(SQL_ATTR_RETRIEVE_DATA);
DeclareString(SQL_ATTR_ROW_STATUS_PTR);
DeclareString(SQL_ATTR_ROWS_PROCESSED_PTR);
DeclareString(SQL_ATTR_ROW_ARRAY_SIZE);
DeclareString(SQL_ATTR_SIMULATE_CURSOR);
DeclareString(SQL_ATTR_USE_BOOKMARKS);
DeclareString(SQL_PARAM_BIND_BY_COLUMN);
DeclareString(SQL_ATTR_ROWS_FETCHED_PTR);
DeclareString(SQL_ATTR_CURSOR_SCROLLABLE);
DeclareString(SQL_ATTR_CURSOR_SENSITIVITY);
DeclareString(SQL_ATTR_PARAM_BIND_OFFSET_PTR);
DeclareString(SQL_ATTR_PARAM_OPERATION_PTR);
DeclareString(SQL_ATTR_PARAMS_PROCESSED_PTR);
DeclareString(SQL_ATTR_ROW_BIND_OFFSET_PTR);
DeclareString(SQL_ATTR_ROW_BIND_TYPE);
DeclareString(SQL_ATTR_ROW_OPERATION_PTR);
DeclareString(SQL_ATTR_ROW_FETCHED_PTR);
DeclareString(SQL_ATTR_ROW_NUMBER);
DeclareString(SQL_ATTR_TYPE_EXCEPTION_BEHAVIOR);
DeclareString(SQL_BINARY_FUNCTIONS);
DeclareString(SQL_ISO_BINARY_FUNCTIONS);


//Connection Attributes
DeclareString(SQL_ATTR_ACCESS_MODE);
DeclareString(SQL_ATTR_AUTOCOMMIT);
DeclareString(SQL_ATTR_CURRENT_CATALOG);
DeclareString(SQL_ATTR_LOGIN_TIMEOUT);
DeclareString(SQL_ATTR_ODBC_CURSORS);
DeclareString(SQL_ATTR_PACKET_SIZE);
DeclareString(SQL_ATTR_QUIET_MODE);
DeclareString(SQL_ATTR_TRACE);
DeclareString(SQL_ATTR_TRACEFILE);
DeclareString(SQL_ATTR_TRANSLATE_LIB);
DeclareString(SQL_ATTR_TRANSLATE_OPTION);
DeclareString(SQL_ATTR_TXN_ISOLATION);


DeclareString(SQL_DIAG_RETURNCODE);
DeclareString(SQL_DIAG_NUMBER);
DeclareString(SQL_DIAG_ROW_COUNT);
DeclareString(SQL_DIAG_DYNAMIC_FUNCTION);
DeclareString(SQL_DIAG_DYNAMIC_FUNCTION_CODE);
DeclareString(SQL_DIAG_CURSOR_ROW_COUNT);
DeclareString(SQL_DIAG_SQLSTATE);
DeclareString(SQL_DIAG_NATIVE);
DeclareString(SQL_DIAG_MESSAGE_TEXT);
DeclareString(SQL_DIAG_CLASS_ORIGIN);
DeclareString(SQL_DIAG_SUBCLASS_ORIGIN);
DeclareString(SQL_DIAG_CONNECTION_NAME);
DeclareString(SQL_DIAG_SERVER_NAME);
DeclareString(SQL_DIAG_ROW_NUMBER);
DeclareString(SQL_DIAG_COLUMN_NUMBER);


//@INSTALLER
//Installer defines
DeclareString(SQL_FALSE);
DeclareString(SQL_TRUE);
DeclareString(ODBC_INSTALL_INQUIRY);
DeclareString(ODBC_INSTALL_COMPLETE);
DeclareString(ODBC_INSTALL_DRIVER);
DeclareString(ODBC_REMOVE_DRIVER);
DeclareString(ODBC_ADD_DSN);
DeclareString(ODBC_CONFIG_DSN);
DeclareString(ODBC_REMOVE_DSN);
DeclareString(ODBC_ADD_SYS_DSN);
DeclareString(ODBC_CONFIG_SYS_DSN);
DeclareString(ODBC_REMOVE_SYS_DSN);
DeclareString(ODBC_REMOVE_DEFAULT_DSN);
DeclareString(ODBC_INI);
DeclareString(ODBCINST_INI);

/* Standard SQL datatypes, using ANSI type numbering */
DeclareString(SQL_CHAR);
DeclareString(SQL_NUMERIC);
DeclareString(SQL_DECIMAL);
DeclareString(SQL_INTEGER);
DeclareString(SQL_SMALLINT);
DeclareString(SQL_FLOAT);
DeclareString(SQL_GUID);
DeclareString(SQL_REAL);
DeclareString(SQL_DOUBLE);
DeclareString(SQL_VARCHAR);

//Wide Character support
DeclareString(SQL_WCHAR);
DeclareString(SQL_WLONGVARCHAR);
DeclareString(SQL_WVARCHAR);

DeclareString(SQL_DATETIME);
DeclareString(SQL_INTERVAL);

DeclareString(SQL_VARIANT_TYPE);
DeclareString(SQL_ARRAY);
DeclareString(SQL_MULTISET);
DeclareString(SQL_UDT);
DeclareString(SQL_ROW);

DeclareString(SQL_TYPE_MIN);
DeclareString(SQL_TYPE_NULL);
DeclareString(SQL_TYPE_MAX);

/* C datatype to SQL datatype mapping    SQL types
                                         ----------------------- */
DeclareString(SQL_C_CHAR);		/* CHAR, VARCHAR, DECIMAL, NUMERIC */
DeclareString(SQL_C_LONG);		/* INTEGER          */
DeclareString(SQL_C_SHORT);		/* SMALLINT         */
DeclareString(SQL_C_FLOAT);		/* REAL             */
DeclareString(SQL_C_GUID);
DeclareString(SQL_C_DOUBLE);		/* FLOAT, DOUBLE    */
DeclareString(SQL_C_WCHAR);
DeclareString(SQL_C_UBIGINT);
DeclareString(SQL_C_SBIGINT);

/* NULL status constants.  These are used in SQLColumns, SQLColAttributes,
SQLDescribeCol, and SQLSpecialColumns to describe the nullablity of a
column in a table.  SQL_NULLABLE_UNKNOWN can be returned only by
SQLDescribeCol or SQLColAttributes.  It is used when the DBMS's meta-data
does not contain this info.  */
DeclareString(SQL_NO_NULLS);
DeclareString(SQL_NULLABLE);
DeclareString(SQL_NULLABLE_UNKNOWN);

/* Special length values */
DeclareString(SQL_NULL_DATA);
DeclareString(SQL_DATA_AT_EXEC);
DeclareString(SQL_DEFAULT_PARAM);

/* SQLColAttributes defines */
DeclareString(SQL_COLUMN_COUNT);
DeclareString(SQL_COLUMN_NAME);
DeclareString(SQL_COLUMN_TYPE);
DeclareString(SQL_COLUMN_LENGTH);
DeclareString(SQL_COLUMN_PRECISION);
DeclareString(SQL_COLUMN_SCALE);
DeclareString(SQL_COLUMN_DISPLAY_SIZE);
DeclareString(SQL_COLUMN_NULLABLE);
DeclareString(SQL_COLUMN_UNSIGNED);
DeclareString(SQL_COLUMN_MONEY);
DeclareString(SQL_COLUMN_UPDATABLE);
DeclareString(SQL_COLUMN_AUTO_INCREMENT);
DeclareString(SQL_COLUMN_CASE_SENSITIVE);
DeclareString(SQL_COLUMN_SEARCHABLE);
DeclareString(SQL_COLUMN_TYPE_NAME);
DeclareString(SQL_COLUMN_TABLE_NAME);
DeclareString(SQL_COLUMN_OWNER_NAME);
DeclareString(SQL_COLUMN_QUALIFIER_NAME);
DeclareString(SQL_COLUMN_LABEL);


/* SQLColAttribute defines */
DeclareString(SQL_NAMED);
DeclareString(SQL_UNNAMED);
DeclareString(SQL_PRED_NONE);
DeclareString(SQL_PRED_CHAR);
DeclareString(SQL_PRED_BASIC);
DeclareString(SQL_PRED_SEARCHABLE);


/* SQLColAttributes subdefines for SQL_COLUMN_UPDATABLE */
DeclareString(SQL_ATTR_READONLY);
DeclareString(SQL_ATTR_WRITE);
DeclareString(SQL_ATTR_READWRITE_UNKNOWN);

/* SQLColAttributes subdefines for SQL_COLUMN_SEARCHABLE */
/* These are also used by SQLGetInfo                     */
DeclareString(SQL_UNSEARCHABLE);
DeclareString(SQL_LIKE_ONLY);
DeclareString(SQL_ALL_EXCEPT_LIKE);
DeclareString(SQL_SEARCHABLE);

/* SQLError defines */
DeclareString(SQL_NULL_HENV);
DeclareString(SQL_NULL_HDBC);
DeclareString(SQL_NULL_HSTMT);
DeclareString(SQL_NULL_HDESC);

/* environment specific definitions */
DeclareString(SQL_API);

/* Core Function Prototypes */
DeclareString(SQLAllocConnect);
DeclareString(SQLAllocEnv);
DeclareString(SQLAllocStmt);
DeclareString(SQLBindCol);
DeclareString(SQLCancel);
DeclareString(SQLColAttributes);
DeclareString(SQLConnect);
DeclareString(SQLDescribeCol);
DeclareString(SQLDisconnect);
DeclareString(SQLError);
DeclareString(SQLExecDirect);
DeclareString(SQLExecute);
DeclareString(SQLFetch);
DeclareString(SQLFreeConnect);
DeclareString(SQLFreeEnv);
DeclareString(SQLFreeStmt);
DeclareString(SQLGetCursorName);
DeclareString(SQLNumResultCols);
DeclareString(SQLPrepare);
DeclareString(SQLRowCount);
DeclareString(SQLSetCursorName);
DeclareString(SQLSetParam);
DeclareString(SQLTransact);

DeclareString(SQLAllocHandle);
DeclareString(SQLFreeHandle);
DeclareString(SQLBindParam);
DeclareString(SQLCloseCursor);
DeclareString(SQLEndTran);
DeclareString(SQLFetchScroll);
DeclareString(SQLGetEnvAttr);
DeclareString(SQLSetEnvAttr);
DeclareString(SQLGetConnectAttr);
DeclareString(SQLSetConnectAttr);
DeclareString(SQLGetStmtAttr);
DeclareString(SQLSetStmtAttr);
DeclareString(SQLCopyDesc);
DeclareString(SQLGetDescField);
DeclareString(SQLSetDescField);
DeclareString(SQLGetDescRec);
DeclareString(SQLSetDescRec);
DeclareString(SQLGetDiagField);
DeclareString(SQLGetDiagRec);

//@INSTALLER
DeclareString(SQLConfigDataSource);
DeclareString(ConfigDriver);
DeclareString(ConfigDSN);
DeclareString(SQLConfigDriver);
DeclareString(ConfigTranslator);
DeclareString(SQLDataSourceToDriver);
DeclareString(SQLDriverToDataSource);
DeclareString(SQLGetAvailableDrivers);
DeclareString(SQLGetInstalledDrivers);
DeclareString(SQLGetPrivateProfileString);
DeclareString(SQLGetTranslator);
DeclareString(SQLInstallDriver);
DeclareString(SQLInstallDriverManager);
DeclareString(SQLInstallODBC);
DeclareString(SQLInstallTranslator);
DeclareString(SQLManageDataSources);
DeclareString(SQLRemoveDefaultDataSource);
DeclareString(SQLRemoveDSNFromIni);
DeclareString(SQLRemoveDriver);
DeclareString(SQLRemovedDriverManager);
DeclareString(SQLRemoveTranslator);
DeclareString(SQLValidDSN);
DeclareString(SQLWriteDSNToIni);
DeclareString(SQLWritePrivateProfileString);



/*
** Named constants from sqlext.h:
*/

/* Defines used by both Level 1 and Level 2 functions */

/* Additional return codes */
DeclareString(SQL_STILL_EXECUTING);

/* SQL extended datatypes */
DeclareString(SQL_DATE);
DeclareString(SQL_TIME);
DeclareString(SQL_TIMESTAMP);
DeclareString(SQL_LONGVARCHAR);
DeclareString(SQL_BINARY);
DeclareString(SQL_VARBINARY);
DeclareString(SQL_LONGVARBINARY);
DeclareString(SQL_BIGINT);
DeclareString(SQL_TINYINT);
DeclareString(SQL_BIT);
//	DeclareString(SQL_SMALLDATETIME);

/* C datatype to SQL datatype mapping */
DeclareString(SQL_C_DATE);
DeclareString(SQL_C_TIME);
DeclareString(SQL_C_TIMESTAMP);
DeclareString(SQL_C_BINARY);
DeclareString(SQL_C_BIT);
DeclareString(SQL_C_TINYINT);
DeclareString(SQL_C_STINYINT);
DeclareString(SQL_C_UTINYINT);
DeclareString(SQL_C_SLONG);
DeclareString(SQL_C_ULONG);
DeclareString(SQL_C_SSHORT);
DeclareString(SQL_C_USHORT);
DeclareString(SQL_C_BOOKMARK);
DeclareString(SQL_C_VARBOOKMARK);
DeclareString(SQL_C_INTERVAL_YEAR);
DeclareString(SQL_C_NUMERIC);
DeclareString(SQL_C_TYPE_DATE);
DeclareString(SQL_C_TYPE_TIME);
DeclareString(SQL_C_TYPE_TIMESTAMP);
DeclareString(SQL_C_BLOB_LOCATOR);
DeclareString(SQL_C_CLOB_LOCATOR);
DeclareString(SQL_ARD_TYPE);
DeclareString(SQL_C_INTERVAL_MONTH);
DeclareString(SQL_C_INTERVAL_DAY);
DeclareString(SQL_C_INTERVAL_HOUR);
DeclareString(SQL_C_INTERVAL_MINUTE);
DeclareString(SQL_C_INTERVAL_SECOND);
DeclareString(SQL_C_INTERVAL_YEAR_TO_MONTH);
DeclareString(SQL_C_INTERVAL_DAY_TO_HOUR);
DeclareString(SQL_C_INTERVAL_DAY_TO_MINUTE);
DeclareString(SQL_C_INTERVAL_DAY_TO_SECOND);
DeclareString(SQL_C_INTERVAL_HOUR_TO_MINUTE);
DeclareString(SQL_C_INTERVAL_HOUR_TO_SECOND);
DeclareString(SQL_C_INTERVAL_MINUTE_TO_SECOND);


DeclareString(SQL_IS_YEAR);
DeclareString(SQL_IS_MONTH);
DeclareString(SQL_IS_DAY);
DeclareString(SQL_IS_HOUR);
DeclareString(SQL_IS_MINUTE);
DeclareString(SQL_IS_SECOND);
DeclareString(SQL_IS_YEAR_TO_MONTH);
DeclareString(SQL_IS_DAY_TO_HOUR);
DeclareString(SQL_IS_DAY_TO_MINUTE);
DeclareString(SQL_IS_DAY_TO_SECOND);
DeclareString(SQL_IS_HOUR_TO_MINUTE);
DeclareString(SQL_IS_HOUR_TO_SECOND);
DeclareString(SQL_IS_MINUTE_TO_SECOND);


/* from SQL.h -- extended data types change this */
/*        (sz_SQL_TYPE_MIN was defined above); */
DeclareString(SQL_ALL_TYPES);

DeclareString(SQL_CODE_DATE);
DeclareString(SQL_CODE_TIME);
DeclareString(SQL_CODE_TIMESTAMP);

DeclareString(SQL_TYPE_DATE);
DeclareString(SQL_TYPE_TIME);
DeclareString(SQL_TYPE_TIMESTAMP);
DeclareString(SQL_BLOB);
DeclareString(SQL_DEFAULT);
DeclareString(SQL_CLOB);
DeclareString(SQL_INTERVAL_MONTH);
DeclareString(SQL_INTERVAL_YEAR);
DeclareString(SQL_INTERVAL_YEAR_TO_MONTH);
DeclareString(SQL_INTERVAL_DAY);
DeclareString(SQL_INTERVAL_HOUR);
DeclareString(SQL_INTERVAL_MINUTE);
DeclareString(SQL_INTERVAL_SECOND);
DeclareString(SQL_INTERVAL_DAY_TO_HOUR);
DeclareString(SQL_INTERVAL_DAY_TO_MINUTE);
DeclareString(SQL_INTERVAL_DAY_TO_SECOND);
DeclareString(SQL_INTERVAL_HOUR_TO_MINUTE);
DeclareString(SQL_INTERVAL_HOUR_TO_SECOND);
DeclareString(SQL_INTERVAL_MINUTE_TO_SECOND);

DeclareString(SQL_CODE_MONTH);
DeclareString(SQL_CODE_YEAR);
DeclareString(SQL_CODE_YEAR_TO_MONTH);
DeclareString(SQL_CODE_DAY);
DeclareString(SQL_CODE_HOUR);
DeclareString(SQL_CODE_MINUTE);
DeclareString(SQL_CODE_SECOND);
DeclareString(SQL_CODE_DAY_TO_HOUR);
DeclareString(SQL_CODE_DAY_TO_MINUTE);
DeclareString(SQL_CODE_DAY_TO_SECOND);
DeclareString(SQL_CODE_HOUR_TO_MINUTE);
DeclareString(SQL_CODE_HOUR_TO_SECOND);
DeclareString(SQL_CODE_MINUTE_TO_SECOND);


/* Level 1 Functions                             */

/* Access modes                 */
DeclareString(SQL_MODE_READ_WRITE);
DeclareString(SQL_MODE_READ_ONLY);


//Env. Attributes
DeclareString(SQL_ATTR_OUTPUT_NTS);

DeclareString(SQL_ATTR_ODBC_VERSION);
DeclareString(SQL_OV_ODBC2);
DeclareString(SQL_OV_ODBC3);
DeclareString(SQL_OV_ODBC4);


/* Options for SQLDriverConnect */
DeclareString(SQL_DRIVER_NOPROMPT);
DeclareString(SQL_DRIVER_COMPLETE);
DeclareString(SQL_DRIVER_PROMPT);
DeclareString(SQL_DRIVER_COMPLETE_REQUIRED);

/* Special return values for SQLGetData */
DeclareString(SQL_NO_TOTAL);

DeclareString(SQL_IK_NONE);
DeclareString(SQL_IK_ASC);
DeclareString(SQL_IK_DESC);
DeclareString(SQL_IK_ALL);

/* Schema Info values */
DeclareString(SQL_ISV_SCHEMATA);
DeclareString(SQL_ISV_DOMAINS);
DeclareString(SQL_ISV_DOMAIN_CONSTRAINTS);
DeclareString(SQL_ISV_TABLES);
DeclareString(SQL_ISV_COLUMNS);
DeclareString(SQL_ISV_TABLE_PRIVILEGES);
DeclareString(SQL_ISV_COLUMN_PRIVILEGES);
DeclareString(SQL_ISV_USAGE_PRIVILEGES);
DeclareString(SQL_ISV_TABLE_CONSTRAINTS);
DeclareString(SQL_ISV_REFERENTIAL_CONSTRAINTS);
DeclareString(SQL_ISV_CHECK_CONSTRAINTS);
DeclareString(SQL_ISV_KEY_COLUMN_USAGE);
DeclareString(SQL_ISV_ASSERTIONS);
DeclareString(SQL_ISV_CHARACTER_SETS);
DeclareString(SQL_ISV_COLLATIONS);
DeclareString(SQL_ISV_TRANSLATIONS);
DeclareString(SQL_ISV_VIEW_TABLE_USAGE);
DeclareString(SQL_ISV_VIEW_COLUMN_USAGE);
DeclareString(SQL_ISV_CONSTRAINT_TABLE_USAGE);
DeclareString(SQL_ISV_CONSTRAINT_COLUMN_USAGE);
DeclareString(SQL_ISV_COLUMN_DOMAIN_USAGE);
DeclareString(SQL_ISV_SQL_LANGUAGES);
DeclareString(SQL_ISV_VIEWS);


/* SQL_ASYNC_MODE */
DeclareString(SQL_AM_CONNECTION);
DeclareString(SQL_AM_STATEMENT);
DeclareString(SQL_AM_NONE);

/* SQL_BATCH_ROW_COUNT */
DeclareString(SQL_BRC_PROCEDURES);
DeclareString(SQL_BRC_EXPLICIT);
DeclareString(SQL_BRC_ROLLED_UP);

/* SQL_SQL_CONFORMANCE */
DeclareString(SQL_SC_SQL92_ENTRY);
DeclareString(SQL_SC_FIPS127_2_TRANSITIONAL);
DeclareString(SQL_SC_SQL92_INTERMEDIATE);
DeclareString(SQL_SC_SQL92_FULL);

DeclareString(SQL_DATETIME_LITERALS);
DeclareString(SQL_DL_SQL92_DATE);
DeclareString(SQL_DL_SQL92_TIME);
DeclareString(SQL_DL_SQL92_TIMESTAMP);
DeclareString(SQL_DL_SQL92_INTERVAL_YEAR);
DeclareString(SQL_DL_SQL92_INTERVAL_MONTH);
DeclareString(SQL_DL_SQL92_INTERVAL_DAY);
DeclareString(SQL_DL_SQL92_INTERVAL_HOUR);
DeclareString(SQL_DL_SQL92_INTERVAL_MINUTE);
DeclareString(SQL_DL_SQL92_INTERVAL_SECOND);
DeclareString(SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH);
DeclareString(SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR);
DeclareString(SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE);
DeclareString(SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND);
DeclareString(SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE);
DeclareString(SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND);
DeclareString(SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND);


/* SQL_PARAM_ROW_COUNTS */
DeclareString(SQL_PARC_BATCH);
DeclareString(SQL_PARC_NO_BATCH);

/* SQL_PARAM_ARRAY_SELECTS */
DeclareString(SQL_PAS_BATCH);
DeclareString(SQL_PAS_NO_BATCH);
DeclareString(SQL_PAS_NO_SELECT);


/* SQL_BATCH_SUPPORT */
DeclareString(SQL_BS_SELECT_EXPLICIT);
DeclareString(SQL_BS_ROW_COUNT_EXPLICIT);
DeclareString(SQL_BS_SELECT_PROC);
DeclareString(SQL_BS_ROW_COUNT_PROC);

/* SQL_PREDICATE_FIND */
DeclareString(SQL_PF_COLUMN);
DeclareString(SQL_PF_ARBITRARY);

/* SQL_LOCATOR_UPDATABILITY */
DeclareString(SQL_LU_NOT_UPDATABLE);
DeclareString(SQL_LU_DIRECT_ONLY);
DeclareString(SQL_LU_ALL);

/* SQL_LOCATOR_SENSITIVITY */
DeclareString(SQL_LS_STATIC);
DeclareString(SQL_LS_DYNAMIC);


/* SQL_LOB_LOCATOR */
DeclareString(SQL_LL_DATA_TYPES);
DeclareString(SQL_LL_SQL);
DeclareString(SQL_LL_LONG_DATA_TYPES);
DeclareString(SQL_LL_EXPLICIT_FREE);

/* SQL_BATCH_ROW_COUNT */
DeclareString(SQL_LP_HOLDABLE);
DeclareString(SQL_LP_NON_HOLDABLE);


/* Defines for SQLGetInfo */
DeclareString(SQL_INFO_FIRST);
DeclareString(SQL_ACTIVE_CONNECTIONS);
DeclareString(SQL_ACTIVE_ENVIRONMENTS);
DeclareString(SQL_ACTIVE_STATEMENTS);
DeclareString(SQL_ALTER_DOMAIN);
DeclareString(SQL_ALTER_SCHEMA);
DeclareString(SQL_ALTER_TABLE);
DeclareString(SQL_SQL_CONFORMANCE);
DeclareString(SQL_DATA_SOURCE_NAME);
DeclareString(SQL_DRIVER_HDBC);
DeclareString(SQL_DRIVER_HDESC);
DeclareString(SQL_DRIVER_HENV);
DeclareString(SQL_DRIVER_HSTMT);
DeclareString(SQL_DRIVER_NAME);
DeclareString(SQL_DRIVER_VER);
DeclareString(SQL_FETCH_DIRECTION);
DeclareString(SQL_ODBC_API_CONFORMANCE);
DeclareString(SQL_ODBC_VER);
DeclareString(SQL_ROW_UPDATES);
DeclareString(SQL_ODBC_SAG_CLI_CONFORMANCE);
DeclareString(SQL_SERVER_NAME);
DeclareString(SQL_SEARCH_PATTERN_ESCAPE);
DeclareString(SQL_ODBC_SQL_CONFORMANCE);

DeclareString(SQL_DATABASE_NAME);
DeclareString(SQL_DBMS_NAME);
DeclareString(SQL_DBMS_VER);

DeclareString(SQL_ACCESSIBLE_TABLES);
DeclareString(SQL_ACCESSIBLE_PROCEDURES);
DeclareString(SQL_PROCEDURES);
DeclareString(SQL_CONCAT_NULL_BEHAVIOR);
DeclareString(SQL_CURSOR_COMMIT_BEHAVIOR);
DeclareString(SQL_CURSOR_ROLLBACK_BEHAVIOR);
DeclareString(SQL_DATA_SOURCE_READ_ONLY);
DeclareString(SQL_DEFAULT_TXN_ISOLATION);
DeclareString(SQL_EXPRESSIONS_IN_ORDERBY);
DeclareString(SQL_IDENTIFIER_CASE);
DeclareString(SQL_IDENTIFIER_QUOTE_CHAR);
DeclareString(SQL_INDEX_KEYWORDS);
DeclareString(SQL_MAX_COLUMN_NAME_LEN);
DeclareString(SQL_MAX_CURSOR_NAME_LEN);
DeclareString(SQL_MAX_OWNER_NAME_LEN);
DeclareString(SQL_MAX_PROCEDURE_NAME_LEN);
DeclareString(SQL_MAX_QUALIFIER_NAME_LEN);
DeclareString(SQL_MAX_TABLE_NAME_LEN);
DeclareString(SQL_MULT_RESULT_SETS);
DeclareString(SQL_MULTIPLE_ACTIVE_TXN);
DeclareString(SQL_OUTER_JOINS);
DeclareString(SQL_OJ_CAPABILITIES); //Bug #345
DeclareString(SQL_OWNER_TERM);
DeclareString(SQL_PROCEDURE_TERM);
DeclareString(SQL_QUALIFIER_NAME_SEPARATOR);
DeclareString(SQL_QUALIFIER_TERM);
DeclareString(SQL_SCROLL_CONCURRENCY);
DeclareString(SQL_SCROLL_OPTIONS);
DeclareString(SQL_TABLE_TERM);
DeclareString(SQL_TXN_CAPABLE);
DeclareString(SQL_USER_NAME);

/* ODBC 3.0 stuff */
DeclareString(SQL_MAX_DRIVER_CONNECTIONS);
DeclareString(SQL_MAX_CONCURRENT_ACTIVITIES);
DeclareString(SQL_MAX_IDENTIFIER_LEN);
DeclareString(SQL_MAX_COLUMNS_IN_FOREIGN_KEY);
DeclareString(SQL_MAX_COLUMNS_IN_INSERT);
DeclareString(SQL_MAX_COLUMNS_IN_JOIN);
DeclareString(SQL_MAX_COLUMNS_IN_UPDATE);
DeclareString(SQL_MAX_DDL_STATEMENT_SIZE);
DeclareString(SQL_MAX_FOREIGN_KEY_SIZE);
DeclareString(SQL_MAX_GROUP_BY_SIZE);
DeclareString(SQL_MAX_JOIN_COLUMNS_SIZE);
DeclareString(SQL_MAX_ORDER_BY_SIZE);
DeclareString(SQL_MAX_PREDICATES);
DeclareString(SQL_MAX_SCHEMA_DEFINITION_LEN);
DeclareString(SQL_MAX_SUBQUERIES);
DeclareString(SQL_MAX_SUBQUERY_NESTING);
DeclareString(SQL_MAX_TABLES_IN_CATALOG);
DeclareString(SQL_MAX_UNIQUE_KEY_SIZE);
DeclareString(SQL_ASYNC_MODE);
DeclareString(SQL_MAX_ASYNC_CONCURRENT_STATEMENTS);
DeclareString(SQL_LOB_LOCATOR);
DeclareString(SQL_LOCATOR_PERSISTENCE);

DeclareString(SQL_CORRELATION_NAME);
DeclareString(SQL_CREATE_ASSERTION);
DeclareString(SQL_CREATE_CHARACTER_SET);
DeclareString(SQL_CREATE_COLLATION);
DeclareString(SQL_CREATE_DOMAIN);
DeclareString(SQL_CREATE_SCHEMA);
DeclareString(SQL_CREATE_TABLE);
DeclareString(SQL_CREATE_VIEW);
DeclareString(SQL_CREATE_TRANSLATION);
DeclareString(SQL_DROP_ASSERTION);
DeclareString(SQL_DROP_CHARACTER_SET);
DeclareString(SQL_DROP_COLLATION);
DeclareString(SQL_DROP_DOMAIN);
DeclareString(SQL_DROP_SCHEMA);
DeclareString(SQL_DROP_TABLE);
DeclareString(SQL_DROP_VIEW);
DeclareString(SQL_DROP_TRANSLATION);
//DeclareString(SQL_GRANT);

DeclareString(SQL_NON_NULLABLE_COLUMNS);
DeclareString(SQL_DRIVER_HLIB);
DeclareString(SQL_DRIVER_ODBC_VER);
DeclareString(SQL_LOCK_TYPES);
DeclareString(SQL_POS_OPERATIONS);
DeclareString(SQL_BOOKMARK_PERSISTENCE);
DeclareString(SQL_STATIC_SENSITIVITY);
DeclareString(SQL_FILE_USAGE);
DeclareString(SQL_GETDATA_EXTENSIONS);
DeclareString(SQL_NULL_COLLATION);
DeclareString(SQL_COLUMN_ALIAS);
DeclareString(SQL_GROUP_BY);
DeclareString(SQL_KEYWORDS);
DeclareString(SQL_ORDER_BY_COLUMNS_IN_SELECT);
DeclareString(SQL_OWNER_USAGE);
DeclareString(SQL_POSITIONED_STATEMENTS);
DeclareString(SQL_QUALIFIER_USAGE);
DeclareString(SQL_QUOTED_IDENTIFIER_CASE);
DeclareString(SQL_SPECIAL_CHARACTERS);
DeclareString(SQL_SUBQUERIES);
DeclareString(SQL_UNION);
DeclareString(SQL_MAX_COLUMNS_IN_GROUP_BY);
DeclareString(SQL_MAX_COLUMNS_IN_INDEX);
DeclareString(SQL_MAX_COLUMNS_IN_ORDER_BY);
DeclareString(SQL_MAX_COLUMNS_IN_SELECT);
DeclareString(SQL_MAX_COLUMNS_IN_TABLE);
DeclareString(SQL_MAX_INDEX_SIZE);
#ifdef SQL_MAX_ROW_SIZE_INCLUDES_BLOB
DeclareString(SQL_MAX_ROW_SIZE_INCLUDES_BLOB);
#endif
DeclareString(SQL_MAX_ROW_SIZE_INCLUDES_LONG);
DeclareString(SQL_MAX_ROW_SIZE);
DeclareString(SQL_MAX_SCHEMA_NAME_LEN);
DeclareString(SQL_MAX_STATEMENT_LEN);
DeclareString(SQL_MAX_TABLES_IN_SELECT);
DeclareString(SQL_MAX_USER_NAME_LEN);
DeclareString(SQL_MAX_CHAR_LITERAL_LEN);
DeclareString(SQL_TIMEDATE_ADD_INTERVALS);
DeclareString(SQL_TIMEDATE_DIFF_INTERVALS);
DeclareString(SQL_MAX_CATALOG_NAME_LEN);

DeclareString(SQL_AGGREGATE_FUNCTIONS);
DeclareString(SQL_CONVERT_FUNCTIONS);
DeclareString(SQL_NUMERIC_FUNCTIONS);
DeclareString(SQL_STRING_FUNCTIONS);
DeclareString(SQL_SYSTEM_FUNCTIONS);
DeclareString(SQL_TIMEDATE_FUNCTIONS);
DeclareString(SQL_LIMIT_ESCAPE_CLAUSE);
DeclareString(SQL_NATIVE_ESCAPE_CLAUSE);
DeclareString(SQL_RETURN_ESCAPE_CLAUSE);

DeclareString(SQL_CONVERT_BIGINT);
DeclareString(SQL_CONVERT_GUID);
DeclareString(SQL_CONVERT_BINARY);
DeclareString(SQL_CONVERT_BIT);
DeclareString(SQL_CONVERT_CHAR);
DeclareString(SQL_CONVERT_DATE);
DeclareString(SQL_CONVERT_DECIMAL);
DeclareString(SQL_CONVERT_DOUBLE);
DeclareString(SQL_CONVERT_FLOAT);
DeclareString(SQL_CONVERT_INTEGER);
DeclareString(SQL_CONVERT_LONGVARCHAR);
DeclareString(SQL_CONVERT_NUMERIC);
DeclareString(SQL_CONVERT_REAL);
DeclareString(SQL_CONVERT_SMALLINT);
DeclareString(SQL_CONVERT_TIME);
DeclareString(SQL_CONVERT_TIMESTAMP);
DeclareString(SQL_CONVERT_TINYINT);
DeclareString(SQL_CONVERT_VARBINARY);
DeclareString(SQL_CONVERT_VARCHAR);
DeclareString(SQL_CONVERT_WCHAR);
DeclareString(SQL_CONVERT_WLONGVARCHAR);
DeclareString(SQL_CONVERT_WVARCHAR);
DeclareString(SQL_CONVERT_LONGVARBINARY);

DeclareString(SQL_NEED_LONG_DATA_LEN);
DeclareString(SQL_MAX_BINARY_LITERAL_LEN);
DeclareString(SQL_LIKE_ESCAPE_CLAUSE);
DeclareString(SQL_QUALIFIER_LOCATION);

DeclareString(SQL_TXN_ISOLATION_OPTION);
DeclareString(SQL_ODBC_SQL_OPT_IEF);
DeclareString(SQL_STANDARD_CLI_CONFORMANCE);

DeclareString(SQL_INFO_LAST);
DeclareString(SQL_INFO_DRIVER_START);

DeclareString(SQL_ODBC_INTERFACE_CONFORMANCE);
DeclareString(SQL_OIC_CORE);
DeclareString(SQL_OIC_LEVEL1);
DeclareString(SQL_OIC_LEVEL2);

DeclareString(SQL_OAC_NONE);
DeclareString(SQL_OAC_LEVEL1);
DeclareString(SQL_OAC_LEVEL2);

DeclareString(SQL_OSCC_NOT_COMPLIANT);
DeclareString(SQL_OSCC_COMPLIANT);

DeclareString(SQL_SCC_XOPEN_CLI_VERSION1);
DeclareString(SQL_SCC_ISO92_CLI);

DeclareString(SQL_OSC_MINIMUM);
DeclareString(SQL_OSC_CORE);
DeclareString(SQL_OSC_EXTENDED);

DeclareString(SQL_CB_NULL);
DeclareString(SQL_CB_NON_NULL);

DeclareString(SQL_CB_DELETE);
DeclareString(SQL_CB_CLOSE);
DeclareString(SQL_CB_PRESERVE);

DeclareString(SQL_INSENSITIVE);
DeclareString(SQL_UNSPECIFIED);
DeclareString(SQL_SENSITIVE);

DeclareString(SQL_IC_UPPER);
DeclareString(SQL_IC_LOWER);
DeclareString(SQL_IC_SENSITIVE);
DeclareString(SQL_IC_MIXED);

DeclareString(SQL_TC_NONE);
DeclareString(SQL_TC_DML);
DeclareString(SQL_TC_DDL_COMMIT);
DeclareString(SQL_TC_DDL_IGNORE);
DeclareString(SQL_TC_ALL);

DeclareString(SQL_AF_AVG);
DeclareString(SQL_AF_COUNT);
DeclareString(SQL_AF_MAX);
DeclareString(SQL_AF_MIN);
DeclareString(SQL_AF_SUM);
DeclareString(SQL_AF_DISTINCT);
DeclareString(SQL_AF_ALL);
DeclareString(SQL_AF_EVERY);
DeclareString(SQL_AF_ANY);
DeclareString(SQL_AF_STDEV_OP);
DeclareString(SQL_AF_STDEV_SAMP);
DeclareString(SQL_AF_VAR_SAMP);
DeclareString(SQL_AF_VAR_POP);
DeclareString(SQL_AF_ARRAY_AGG);
DeclareString(SQL_AF_COLLECT);
DeclareString(SQL_AF_FUSION);
DeclareString(SQL_AF_INTERSECTION);

DeclareString(SQL_FN_BIN_BIT_LENGTH);
DeclareString(SQL_FN_BIN_CONCAT);
DeclareString(SQL_FN_BIN_INSERT);
DeclareString(SQL_FN_BIN_LTRIM);
DeclareString(SQL_FN_BIN_OCTET_LENGTH);
DeclareString(SQL_FN_BIN_POSITION);
DeclareString(SQL_FN_BIN_RTRIM);
DeclareString(SQL_FN_BIN_SUBSTRING);

DeclareString(SQL_SBF_CONVERT);
DeclareString(SQL_SBF_SUBSTRING);
DeclareString(SQL_SBF_TRIM_BOTH);
DeclareString(SQL_SBF_TRIM_LEADING);
DeclareString(SQL_SBF_TRIM_TRAILING);
DeclareString(SQL_SBF_OVERLAY);
DeclareString(SQL_SBF_LENGTH);
DeclareString(SQL_SBF_POSITION);
DeclareString(SQL_SBF_CONCAT);

DeclareString(SQL_LC_NONE);
DeclareString(SQL_LC_TAKE);
DeclareString(SQL_LC_SKIP);

DeclareString(SQL_RC_NONE);
DeclareString(SQL_RC_INSERT_SINGLE_ROWID);
DeclareString(SQL_RC_INSERT_SINGLE_ANY);
DeclareString(SQL_RC_INSERT_MULTIPLE_ROWID);
DeclareString(SQL_RC_INSERT_MULTIPLE_ANY);
DeclareString(SQL_RC_INSERT_SELECT_ROWID);
DeclareString(SQL_RC_INSERT_SELECT_ANY);
DeclareString(SQL_RC_UPDATE_ROWID);
DeclareString(SQL_RC_UPDATE_ANY);
DeclareString(SQL_RC_DELETE_ROWID);
DeclareString(SQL_RC_DELETE_ANY);
DeclareString(SQL_RC_SELECT_INTO_ROWID);
DeclareString(SQL_RC_SELECT_INTO_ANY);

DeclareString(SQL_CL_START);
DeclareString(SQL_CL_END);

DeclareString(SQL_BP_CLOSE);
DeclareString(SQL_BP_DELETE);
DeclareString(SQL_BP_DROP);
DeclareString(SQL_BP_TRANSACTION);
DeclareString(SQL_BP_UPDATE);
DeclareString(SQL_BP_SCROLL);
DeclareString(SQL_BP_OTHER_HSTMT);

DeclareString(SQL_SS_ADDITIONS);
DeclareString(SQL_SS_DELETIONS);
DeclareString(SQL_SS_UPDATES);

DeclareString(SQL_FILE_NOT_SUPPORTED);
DeclareString(SQL_FILE_TABLE);
DeclareString(SQL_FILE_QUALIFIER);
DeclareString(SQL_FILE_CATALOG);

DeclareString(SQL_GD_ANY_COLUMN);
DeclareString(SQL_GD_ANY_ORDER);
DeclareString(SQL_GD_BLOCK);
DeclareString(SQL_GD_BOUND);

DeclareString(SQL_NC_HIGH);
DeclareString(SQL_NC_LOW);
DeclareString(SQL_NC_START);
DeclareString(SQL_NC_END);


DeclareString(SQL_CA_CREATE_ASSERTION);
DeclareString(SQL_CA_CONSTRAINT_INITIALLY_DEFERRED);
DeclareString(SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE);
DeclareString(SQL_CA_CONSTRAINT_DEFERRABLE);
DeclareString(SQL_CA_CONSTRAINT_NON_DEFERRABLE);

DeclareString(SQL_CCS_CREATE_CHARACTER_SET);
DeclareString(SQL_CCS_COLLATE_CLAUSE);
DeclareString(SQL_CCS_LIMITED_COLLATION);

DeclareString(SQL_CCOL_CREATE_COLLATION);

DeclareString(SQL_CDO_CREATE_DOMAIN);
DeclareString(SQL_CDO_DEFAULT);
DeclareString(SQL_CDO_CONSTRAINT);
DeclareString(SQL_CDO_COLLATION);
DeclareString(SQL_CDO_CONSTRAINT_NAME_DEFINITION);
DeclareString(SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED);
DeclareString(SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE);
DeclareString(SQL_CDO_CONSTRAINT_DEFERRABLE);
DeclareString(SQL_CDO_CONSTRAINT_NON_DEFERRABLE);

DeclareString(SQL_CS_CREATE_SCHEMA);
DeclareString(SQL_CS_AUTHORIZATION);
DeclareString(SQL_CS_DEFAULT_CHARACTER_SET);

DeclareString(SQL_CT_CREATE_TABLE);
DeclareString(SQL_CT_COMMIT_PRESERVE);
DeclareString(SQL_CT_COMMIT_DELETE);
DeclareString(SQL_CT_GLOBAL_TEMPORARY);
DeclareString(SQL_CT_LOCAL_TEMPORARY);
DeclareString(SQL_CT_CONSTRAINT_INITIALLY_DEFERRED);
DeclareString(SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE);
DeclareString(SQL_CT_CONSTRAINT_DEFERRABLE);
DeclareString(SQL_CT_CONSTRAINT_NON_DEFERRABLE);
DeclareString(SQL_CT_COLUMN_CONSTRAINT);
DeclareString(SQL_CT_COLUMN_DEFAULT);
DeclareString(SQL_CT_COLUMN_COLLATION);
DeclareString(SQL_CT_TABLE_CONSTRAINT);
DeclareString(SQL_CT_CONSTRAINT_NAME_DEFINITION);

DeclareString(SQL_CTR_CREATE_TRANSLATION);

DeclareString(SQL_CV_CREATE_VIEW);
DeclareString(SQL_CV_CHECK_OPTION);
DeclareString(SQL_CV_CASCADED);
DeclareString(SQL_CV_LOCAL);

DeclareString(SQL_DA_DROP_ASSERTION);

DeclareString(SQL_DCS_DROP_CHARACTER_SET);

DeclareString(SQL_DC_DROP_COLLATION);

DeclareString(SQL_DD_DROP_DOMAIN);
DeclareString(SQL_DD_RESTRICT);
DeclareString(SQL_DD_CASCADE);

DeclareString(SQL_DS_DROP_SCHEMA);
DeclareString(SQL_DS_RESTRICT);
DeclareString(SQL_DS_CASCADE);

DeclareString(SQL_DT_DROP_TABLE);
DeclareString(SQL_DT_RESTRICT);
DeclareString(SQL_DT_CASCADE);

DeclareString(SQL_DTR_DROP_TRANSLATION);

DeclareString(SQL_DV_DROP_VIEW);
DeclareString(SQL_DV_CASCADE);
DeclareString(SQL_DV_RESTRICT);


/* bitmasks for SQL_DYNAMIC_CURSOR_ATTRIBUTES1,
 * SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1,
 * SQL_KEYSET_CURSOR_ATTRIBUTES1, and SQL_STATIC_CURSOR_ATTRIBUTES1
 */
DeclareString(SQL_DYNAMIC_CURSOR_ATTRIBUTES1);
DeclareString(SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1);
DeclareString(SQL_KEYSET_CURSOR_ATTRIBUTES1);
DeclareString(SQL_STATIC_CURSOR_ATTRIBUTES1);
DeclareString(SQL_CA1_NEXT);
DeclareString(SQL_CA1_ABSOLUTE);
DeclareString(SQL_CA1_RELATIVE);
DeclareString(SQL_CA1_BOOKMARK);
DeclareString(SQL_CA1_LOCK_NO_CHANGE);
DeclareString(SQL_CA1_LOCK_EXCLUSIVE);
DeclareString(SQL_CA1_LOCK_UNLOCK);
DeclareString(SQL_CA1_POS_POSITION);
DeclareString(SQL_CA1_POS_UPDATE);
DeclareString(SQL_CA1_POS_DELETE);
DeclareString(SQL_CA1_POS_REFRESH);
DeclareString(SQL_CA1_POSITIONED_UPDATE);
DeclareString(SQL_CA1_POSITIONED_DELETE);
DeclareString(SQL_CA1_SELECT_FOR_UPDATE);
DeclareString(SQL_CA1_BULK_ADD);
DeclareString(SQL_CA1_BULK_UPDATE_BY_BOOKMARK);
DeclareString(SQL_CA1_BULK_DELETE_BY_BOOKMARK);
DeclareString(SQL_CA1_BULK_FETCH_BY_BOOKMARK);


/* bitmasks for SQL_DYNAMIC_CURSOR_ATTRIBUTES2,
 * SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2,
 * SQL_KEYSET_CURSOR_ATTRIBUTES2, and SQL_STATIC_CURSOR_ATTRIBUTES2
 */
DeclareString(SQL_DYNAMIC_CURSOR_ATTRIBUTES2);
DeclareString(SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2);
DeclareString(SQL_KEYSET_CURSOR_ATTRIBUTES2);
DeclareString(SQL_STATIC_CURSOR_ATTRIBUTES2);
DeclareString(SQL_CA2_READ_ONLY_CONCURRENCY);
DeclareString(SQL_CA2_LOCK_CONCURRENCY);
DeclareString(SQL_CA2_OPT_ROWVER_CONCURRENCY);
DeclareString(SQL_CA2_OPT_VALUES_CONCURRENCY);
DeclareString(SQL_CA2_SENSITIVITY_ADDITIONS);
DeclareString(SQL_CA2_SENSITIVITY_DELETIONS);
DeclareString(SQL_CA2_SENSITIVITY_UPDATES);
DeclareString(SQL_CA2_MAX_ROWS_SELECT);
DeclareString(SQL_CA2_MAX_ROWS_INSERT);
DeclareString(SQL_CA2_MAX_ROWS_DELETE);
DeclareString(SQL_CA2_MAX_ROWS_UPDATE);
DeclareString(SQL_CA2_MAX_ROWS_CATALOG);
DeclareString(SQL_CA2_MAX_ROWS_AFFECTS_ALL);
DeclareString(SQL_CA2_CRC_EXACT);
DeclareString(SQL_CA2_CRC_APPROXIMATE);
DeclareString(SQL_CA2_SIMULATE_NON_UNIQUE);
DeclareString(SQL_CA2_SIMULATE_TRY_UNIQUE);
DeclareString(SQL_CA2_SIMULATE_UNIQUE);

DeclareString(SQL_AD_ALTER_DOMAIN);

DeclareString(SQL_AT_ADD_COLUMN);
DeclareString(SQL_AT_DROP_COLUMN);
DeclareString(SQL_AT_ADD_CONSTRAINT);
DeclareString(SQL_AT_ADD_COLUMN_SINGLE);
DeclareString(SQL_AT_ADD_COLUMN_DEFAULT);
DeclareString(SQL_AT_ADD_COLUMN_COLLATION);
DeclareString(SQL_AT_SET_COLUMN_DEFAULT);
DeclareString(SQL_AT_DROP_COLUMN_DEFAULT);
DeclareString(SQL_AT_DROP_COLUMN_CASCADE);
DeclareString(SQL_AT_DROP_COLUMN_RESTRICT);
DeclareString(SQL_AT_ADD_TABLE_CONSTRAINT);
DeclareString(SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE);
DeclareString(SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT);
DeclareString(SQL_AT_CONSTRAINT_NAME_DEFINITION);
DeclareString(SQL_AT_CONSTRAINT_INITIALLY_DEFERRED);
DeclareString(SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE);
DeclareString(SQL_AT_CONSTRAINT_DEFERRABLE);


DeclareString(SQL_AD_CONSTRAINT_NAME_DEFINITION);
DeclareString(SQL_AD_ADD_DOMAIN_CONSTRAINT);
DeclareString(SQL_AD_DROP_DOMAIN_CONSTRAINT);
DeclareString(SQL_AD_ADD_DOMAIN_DEFAULT);
DeclareString(SQL_AD_DROP_DOMAIN_DEFAULT);
DeclareString(SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED);
DeclareString(SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE);
DeclareString(SQL_AD_ADD_CONSTRAINT_DEFERRABLE);
DeclareString(SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE);


DeclareString(SQL_CN_NONE);
DeclareString(SQL_CN_DIFFERENT);
DeclareString(SQL_CN_ANY);

DeclareString(SQL_GB_NOT_SUPPORTED);
DeclareString(SQL_GB_GROUP_BY_EQUALS_SELECT);
DeclareString(SQL_GB_GROUP_BY_CONTAINS_SELECT);
DeclareString(SQL_GB_NO_RELATION);
DeclareString(SQL_GB_COLLATE);

DeclareString(SQL_NNC_NULL);
DeclareString(SQL_NNC_NON_NULL);

DeclareString(SQL_OU_DML_STATEMENTS);
DeclareString(SQL_OU_PROCEDURE_INVOCATION);
DeclareString(SQL_OU_TABLE_DEFINITION);
DeclareString(SQL_OU_INDEX_DEFINITION);
DeclareString(SQL_OU_PRIVILEGE_DEFINITION);

DeclareString(SQL_PS_POSITIONED_DELETE);
DeclareString(SQL_PS_POSITIONED_UPDATE);
DeclareString(SQL_PS_SELECT_FOR_UPDATE);

DeclareString(SQL_QU_DML_STATEMENTS);
DeclareString(SQL_QU_PROCEDURE_INVOCATION);
DeclareString(SQL_QU_TABLE_DEFINITION);
DeclareString(SQL_QU_INDEX_DEFINITION);
DeclareString(SQL_QU_PRIVILEGE_DEFINITION);

DeclareString(SQL_SQ_COMPARISON);
DeclareString(SQL_SQ_EXISTS);
DeclareString(SQL_SQ_IN);
DeclareString(SQL_SQ_QUANTIFIED);
DeclareString(SQL_SQ_CORRELATED_SUBQUERIES);

DeclareString(SQL_FN_TSI_FRAC_SECOND);
DeclareString(SQL_FN_TSI_SECOND);
DeclareString(SQL_FN_TSI_MINUTE);
DeclareString(SQL_FN_TSI_HOUR);
DeclareString(SQL_FN_TSI_DAY);
DeclareString(SQL_FN_TSI_WEEK);
DeclareString(SQL_FN_TSI_MONTH);
DeclareString(SQL_FN_TSI_QUARTER);
DeclareString(SQL_FN_TSI_YEAR);
DeclareString(SQL_INFO_SCHEMA_VIEWS);
DeclareString(SQL_INTEGRITY);
DeclareString(SQL_BATCH_ROW_COUNT);
DeclareString(SQL_PARAM_ARRAY_ROW_COUNTS);
DeclareString(SQL_PARAM_ARRAY_SELECTS);
DeclareString(SQL_PARAM_ARRAY_SELECT_BATCH);
DeclareString(SQL_XOPEN_CLI_YEAR);
DeclareString(SQL_CURSOR_SENSITIVITY);
DeclareString(SQL_DESCRIBE_PARAMETER);
DeclareString(SQL_DM_VER);
DeclareString(SQL_CATALOG_NAME);
DeclareString(SQL_COLLATION_SEQ);

DeclareString(SQL_CONVERT_INTERVAL_YEAR_MONTH);
DeclareString(SQL_CONVERT_INTERVAL_DAY_TIME);
DeclareString(SQL_LOCATOR_UPDATABILITY);
DeclareString(SQL_LOCATOR_SENSITIVITY);
DeclareString(SQL_PREDICATE_FIND);
DeclareString(SQL_BATCH_SUPPORT);
DeclareString(SQL_SCHEMA_TERM);
DeclareString(SQL_CATALOG_NAME_SEPARATOR);
DeclareString(SQL_CATALOG_TERM);
DeclareString(SQL_SCHEMA_USAGE);
DeclareString(SQL_CATALOG_USAGE);
DeclareString(SQL_CATALOG_LOCATION);



//SQL_OJ_CAPABILITIES Bug #345
DeclareString(SQL_OJ_LEFT);
DeclareString(SQL_OJ_RIGHT);
DeclareString(SQL_OJ_FULL);
DeclareString(SQL_OJ_NESTED);
DeclareString(SQL_OJ_NOT_ORDERED);
DeclareString(SQL_OJ_INNER);
DeclareString(SQL_OJ_ALL_COMPARISON_OPS);

/* "SQL_CONVERT_" return value bitmasks */

DeclareString(SQL_CVT_CHAR);
DeclareString(SQL_CVT_NUMERIC);
DeclareString(SQL_CVT_DECIMAL);
DeclareString(SQL_CVT_INTEGER);
DeclareString(SQL_CVT_SMALLINT);
DeclareString(SQL_CVT_FLOAT);
DeclareString(SQL_CVT_REAL);
DeclareString(SQL_CVT_DOUBLE);
DeclareString(SQL_CVT_VARCHAR);
DeclareString(SQL_CVT_WCHAR);
DeclareString(SQL_CVT_WLONGVARCHAR);
DeclareString(SQL_CVT_WVARCHAR);
DeclareString(SQL_CVT_LONGVARCHAR);
DeclareString(SQL_CVT_BINARY);
DeclareString(SQL_CVT_VARBINARY);
DeclareString(SQL_CVT_BIT);
DeclareString(SQL_CVT_TINYINT);
DeclareString(SQL_CVT_BIGINT);
DeclareString(SQL_CVT_DATE);
DeclareString(SQL_CVT_TIME);
DeclareString(SQL_CVT_TIMESTAMP);
DeclareString(SQL_CVT_LONGVARBINARY);
DeclareString(SQL_CVT_INTERVAL_YEAR_MONTH);
DeclareString(SQL_CVT_INTERVAL_DAY_TIME);

DeclareString(SQL_CVT_C_BINARY);
DeclareString(SQL_CVT_C_BIT);
DeclareString(SQL_CVT_C_CHAR);
DeclareString(SQL_CVT_C_DATE);
DeclareString(SQL_CVT_C_DOUBLE);
DeclareString(SQL_CVT_C_FLOAT);
DeclareString(SQL_CVT_C_SLONG);
DeclareString(SQL_CVT_C_SSHORT);
DeclareString(SQL_CVT_C_STINYINT);
DeclareString(SQL_CVT_C_TIME);
DeclareString(SQL_CVT_C_TIMESTAMP);
DeclareString(SQL_CVT_C_ULONG);
DeclareString(SQL_CVT_C_USHORT);
DeclareString(SQL_CVT_C_UTINYINT);

/* Conversion functions */
DeclareString(SQL_FN_CVT_CONVERT);

/* Lock Types */
DeclareString(SQL_LCK_NO_CHANGE);
DeclareString(SQL_LCK_EXCLUSIVE);
DeclareString(SQL_LCK_UNLOCK);

/* Positioned Operations */
DeclareString(SQL_POS_POSITION);
DeclareString(SQL_POS_REFRESH);
DeclareString(SQL_POS_UPDATE);
DeclareString(SQL_POS_DELETE);
DeclareString(SQL_POS_UNDELETE);
DeclareString(SQL_POS_ADD);

DeclareString(SQL_CU_DML_STATEMENTS);
DeclareString(SQL_CU_PROCEDURE_INVOCATION);
DeclareString(SQL_CU_TABLE_DEFINITION);
DeclareString(SQL_CU_INDEX_DEFINITION);
DeclareString(SQL_CU_PRIVILEGE_DEFINITION);


/* String functions */

DeclareString(SQL_FN_STR_ASCII);
DeclareString(SQL_FN_STR_BIT_LENGTH);
DeclareString(SQL_FN_STR_CHAR);
DeclareString(SQL_FN_STR_CHAR_LENGTH);
DeclareString(SQL_FN_STR_CHARACTER_LENGTH);
DeclareString(SQL_FN_STR_CONCAT);
DeclareString(SQL_FN_STR_DIFFERENCE);
DeclareString(SQL_FN_STR_INSERT);
DeclareString(SQL_FN_STR_LCASE);
DeclareString(SQL_FN_STR_LEFT);
DeclareString(SQL_FN_STR_LENGTH);
DeclareString(SQL_FN_STR_LOCATE);
DeclareString(SQL_FN_STR_LOCATE_2);
DeclareString(SQL_FN_STR_LTRIM);
DeclareString(SQL_FN_STR_OCTET_LENGTH);
DeclareString(SQL_FN_STR_POSITION);
DeclareString(SQL_FN_STR_REPEAT);
DeclareString(SQL_FN_STR_REPLACE);
DeclareString(SQL_FN_STR_RIGHT);
DeclareString(SQL_FN_STR_RTRIM);
DeclareString(SQL_FN_STR_SOUNDEX);
DeclareString(SQL_FN_STR_SPACE);
DeclareString(SQL_FN_STR_SUBSTRING);
DeclareString(SQL_FN_STR_UCASE);

/* Numeric functions */

DeclareString(SQL_FN_NUM_ABS);
DeclareString(SQL_FN_NUM_ACOS);
DeclareString(SQL_FN_NUM_ASIN);
DeclareString(SQL_FN_NUM_ATAN);
DeclareString(SQL_FN_NUM_ATAN2);
DeclareString(SQL_FN_NUM_CEILING);
DeclareString(SQL_FN_NUM_COS);
DeclareString(SQL_FN_NUM_COT);
DeclareString(SQL_FN_NUM_DEGREES);
DeclareString(SQL_FN_NUM_EXP);
DeclareString(SQL_FN_NUM_FLOOR);
DeclareString(SQL_FN_NUM_LOG);
DeclareString(SQL_FN_NUM_LOG10);
DeclareString(SQL_FN_NUM_MOD);
DeclareString(SQL_FN_NUM_POWER);
DeclareString(SQL_FN_NUM_RADIANS);
DeclareString(SQL_FN_NUM_ROUND);
DeclareString(SQL_FN_NUM_SIGN);
DeclareString(SQL_FN_NUM_SIN);
DeclareString(SQL_FN_NUM_SQRT);
DeclareString(SQL_FN_NUM_TAN);
DeclareString(SQL_FN_NUM_TRUNCATE);
DeclareString(SQL_FN_NUM_PI);
DeclareString(SQL_FN_NUM_RAND);

/* Time/date functions */

DeclareString(SQL_FN_TD_CURRENT_DATE);
DeclareString(SQL_FN_TD_CURRENT_TIME);
DeclareString(SQL_FN_TD_CURRENT_TIMESTAMP);
DeclareString(SQL_FN_TD_CURDATE);
DeclareString(SQL_FN_TD_CURTIME);
DeclareString(SQL_FN_TD_DAYNAME);
DeclareString(SQL_FN_TD_DAYOFWEEK);
DeclareString(SQL_FN_TD_DAYOFMONTH);
DeclareString(SQL_FN_TD_DAYOFYEAR);
DeclareString(SQL_FN_TD_EXTRACT);
DeclareString(SQL_FN_TD_HOUR);
DeclareString(SQL_FN_TD_MINUTE);
DeclareString(SQL_FN_TD_MONTH);
DeclareString(SQL_FN_TD_MONTHNAME);
DeclareString(SQL_FN_TD_NOW);
DeclareString(SQL_FN_TD_QUARTER);
DeclareString(SQL_FN_TD_SECOND);
DeclareString(SQL_FN_TD_TIMESTAMPADD);
DeclareString(SQL_FN_TD_TIMESTAMPDIFF);
DeclareString(SQL_FN_TD_WEEK);
DeclareString(SQL_FN_TD_YEAR);

/* System functions */

DeclareString(SQL_FN_SYS_USERNAME);
DeclareString(SQL_FN_SYS_DBNAME);
DeclareString(SQL_FN_SYS_IFNULL);

/* Scroll option masks */

DeclareString(SQL_SO_FORWARD_ONLY);
DeclareString(SQL_SO_KEYSET_DRIVEN);
DeclareString(SQL_SO_DYNAMIC);
DeclareString(SQL_SO_MIXED);
DeclareString(SQL_SO_STATIC);

/* Scroll concurrency option masks */

DeclareString(SQL_SCCO_READ_ONLY);
DeclareString(SQL_SCCO_LOCK);
DeclareString(SQL_SCCO_OPT_VALUES);
DeclareString(SQL_SCCO_OPT_ROWVER);

/* Fetch direction option masks */

DeclareString(SQL_FD_FETCH_NEXT);
DeclareString(SQL_FD_FETCH_FIRST);
DeclareString(SQL_FD_FETCH_LAST);
DeclareString(SQL_FD_FETCH_PRIOR);
DeclareString(SQL_FD_FETCH_ABSOLUTE);
DeclareString(SQL_FD_FETCH_RELATIVE);
DeclareString(SQL_FD_FETCH_BOOKMARK);

#ifdef SQL_FD_FETCH_FIND
DeclareString(SQL_FD_FETCH_FIND);
#endif

/* Transaction isolation option masks */

DeclareString(SQL_TXN_READ_UNCOMMITTED);
DeclareString(SQL_TXN_READ_COMMITTED);
DeclareString(SQL_TXN_REPEATABLE_READ);
DeclareString(SQL_TXN_SERIALIZABLE);

/*  SQL_QUALIFIER_LOCATION constants */
DeclareString(SQL_QL_START);
DeclareString(SQL_QL_END);


DeclareString(SQL_SU_DML_STATEMENTS);
DeclareString(SQL_SU_PROCEDURE_INVOCATION);
DeclareString(SQL_SU_TABLE_DEFINITION);
DeclareString(SQL_SU_INDEX_DEFINITION);


/*  SQL_UNION constants */
DeclareString(SQL_U_UNION);
DeclareString(SQL_U_UNION_ALL);


/* Descriptor field constants */
//Header fields
DeclareString(SQL_DESC_ALLOC_TYPE);
DeclareString(SQL_DESC_ARRAY_SIZE);
DeclareString(SQL_DESC_ARRAY_STATUS_PTR);
DeclareString(SQL_DESC_BIND_OFFSET_PTR);
DeclareString(SQL_DESC_BIND_TYPE);
DeclareString(SQL_DESC_COUNT);
DeclareString(SQL_DESC_FETCH_BOOKMARK_PTR);
DeclareString(SQL_DESC_FETCH_LEVEL);
DeclareString(SQL_DESC_NESTED_DESC_ARRAY);
DeclareString(SQL_DESC_OUT_OF_LINE_HEAP);
DeclareString(SQL_DESC_BASE_TABLE_NAME);

#ifdef SQL_DESC_PREDICATE_OCTET_LENGTH_PTR
DeclareString(SQL_DESC_PREDICATE_OCTET_LENGTH_PTR);
#endif
#ifdef SQL_DESC_PREDICATE_PTR
DeclareString(SQL_DESC_PREDICATE_PTR);
#endif
DeclareString(SQL_DESC_ROWS_PROCESSED_PTR);
DeclareString(SQL_DESC_BIND_LAYOUT);
DeclareString(SQL_DESC_TOTAL_LEVELS);

//Record fields
DeclareString(SQL_DESC_AUTO_UNIQUE_VALUE);
#ifdef SQL_DESC_BIND_OUT_OF_LINE
DeclareString(SQL_DESC_BIND_OUT_OF_LINE);
#endif
DeclareString(SQL_DESC_BASE_COLUMN_NAME);
DeclareString(SQL_DESC_CASE_SENSITIVE);
DeclareString(SQL_DESC_CONCISE_TYPE);
DeclareString(SQL_DESC_CATALOG_NAME);
DeclareString(SQL_DESC_DATA_PTR);
DeclareString(SQL_DESC_DATETIME_INTERVAL_CODE);
DeclareString(SQL_DESC_DATETIME_INTERVAL_PRECISION);
DeclareString(SQL_DESC_DISPLAY_SIZE);
DeclareString(SQL_DESC_FIXED_PREC_SCALE);
DeclareString(SQL_DESC_INDICATOR_PTR);
DeclareString(SQL_DESC_LABEL);
DeclareString(SQL_DESC_LENGTH);
DeclareString(SQL_DESC_LITERAL_PREFIX);
DeclareString(SQL_DESC_LITERAL_SUFFIX);
DeclareString(SQL_DESC_LOCAL_TYPE_NAME);
DeclareString(SQL_DESC_MINIMUM_SCALE);
DeclareString(SQL_DESC_MAXIMUM_SCALE);
DeclareString(SQL_DESC_NAME);
DeclareString(SQL_DESC_NULLABLE);
DeclareString(SQL_DESC_OCTET_LENGTH);
DeclareString(SQL_DESC_OCTET_LENGTH_PTR);
DeclareString(SQL_DESC_PARAMETER_TYPE);
DeclareString(SQL_DESC_PRECISION);
DeclareString(SQL_DESC_SCALE);
DeclareString(SQL_DESC_SCHEMA_NAME);
DeclareString(SQL_DESC_SEARCHABLE);
DeclareString(SQL_DESC_TABLE_NAME);
DeclareString(SQL_DESC_TYPE);
DeclareString(SQL_DESC_TYPE_NAME);
DeclareString(SQL_DESC_UNNAMED);
DeclareString(SQL_DESC_UNSIGNED);
DeclareString(SQL_DESC_UPDATABLE);
DeclareString(SQL_DESC_NUM_PREC_RADIX);

/* options for SQLGetStmtOption/SQLSetStmtOption */
DeclareString(SQL_QUERY_TIMEOUT);
DeclareString(SQL_MAX_ROWS);
DeclareString(SQL_NOSCAN);
DeclareString(SQL_MAX_LENGTH);
DeclareString(SQL_ASYNC_ENABLE);
DeclareString(SQL_BIND_TYPE);
DeclareString(SQL_CONCURRENCY);
DeclareString(SQL_CURSOR_TYPE);
DeclareString(SQL_KEYSET_SIZE);
DeclareString(SQL_ROWSET_SIZE);
DeclareString(SQL_SIMULATE_CURSOR);
DeclareString(SQL_RETRIEVE_DATA);
DeclareString(SQL_USE_BOOKMARKS);
DeclareString(SQL_GET_BOOKMARK);
DeclareString(SQL_ROW_NUMBER);

/* options for SQLGetStmtAttr/SQLSetStmtAttr */
DeclareString(SQL_ASYNC_ENABLE_OFF);
DeclareString(SQL_ASYNC_ENABLE_ON);

DeclareString(SQL_BIND_BY_COLUMN);

DeclareString(SQL_DESC_ALLOC_USER);
DeclareString(SQL_DESC_ALLOC_AUTO);

DeclareString(SQL_CURSOR_FORWARD_ONLY);
DeclareString(SQL_CURSOR_STATIC);
DeclareString(SQL_CURSOR_KEYSET_DRIVEN);
DeclareString(SQL_CURSOR_DYNAMIC);

DeclareString(SQL_NONSCROLLABLE);
DeclareString(SQL_SCROLLABLE);

DeclareString(SQL_NOSCAN_OFF);
DeclareString(SQL_NOSCAN_ON);

DeclareString(SQL_SC_NON_UNIQUE);
DeclareString(SQL_SC_TRY_UNIQUE);
DeclareString(SQL_SC_UNIQUE);

DeclareString(SQL_RD_OFF);
DeclareString(SQL_RD_ON);

DeclareString(SQL_UB_OFF);
DeclareString(SQL_UB_ON);
DeclareString(SQL_UB_VARIABLE);

DeclareString(SQL_TE_ERROR);
DeclareString(SQL_TE_CONTINUE);
DeclareString(SQL_TE_REPORT);


/* options for SQLSetConnectOption/SQLGetConnectOption */
DeclareString(SQL_ACCESS_MODE);
DeclareString(SQL_AUTOCOMMIT);
DeclareString(SQL_CURRENT_QUALIFIER);
DeclareString(SQL_LOGIN_TIMEOUT);
DeclareString(SQL_ODBC_CURSORS);
DeclareString(SQL_OPT_TRACE);
DeclareString(SQL_OPT_TRACEFILE);
DeclareString(SQL_PACKET_SIZE);
DeclareString(SQL_QUIET_MODE);
DeclareString(SQL_TRANSLATE_DLL);
DeclareString(SQL_TRANSLATE_OPTION);
DeclareString(SQL_TXN_ISOLATION);
DeclareString(SQL_CONNECT_OPT_DRVR_START);

DeclareString(SQL_AUTOCOMMIT_OFF);
DeclareString(SQL_AUTOCOMMIT_ON);

DeclareString(SQL_CUR_USE_IF_NEEDED);
DeclareString(SQL_CUR_USE_ODBC);
DeclareString(SQL_CUR_USE_DRIVER);

DeclareString(SQL_OPT_TRACE_OFF);
DeclareString(SQL_OPT_TRACE_ON);

#ifdef _INTERNAL

#define SqlSvrTypes	\
	InitDftNoSArray10(SQLTEXT,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLVARBINARY,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLINTN,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLVARCHAR,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLBINARY,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLIMAGE,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLCHARACTER,							SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLINT1,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLBIT,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLINT2,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLINT4,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLMONEY,								SQL_C_SSHORT),	\
	InitDftNoSArray10(SQLDATETIME ,							SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLFLT8,								SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLFLTN ,							SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLMONEYN,							SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLDATETIMN ,						SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLFLT4 ,							SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLMONEY4,							SQL_C_SSHORT),	\
	InitDftNoSArray10( SQLDATETIM4 	,						SQL_C_SSHORT),	\
	InitDftNoSArray20( SQLDEC,							SQL_C_SSHORT),	\
	InitDftNoSArray20( SQLDECIMAL,							SQL_C_SSHORT),	\
	InitDftNoSArray20( SQLNUM,							SQL_C_SSHORT),	\
	InitDftNoSArray20( SQLNUMERIC,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLUNIQUEID,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLBIGCHAR,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLBIGVARCHAR,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLBIGBINARY	,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLBIGVARBINARY,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLBITN,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLNCHAR	,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLNVARCHAR ,							SQL_C_SSHORT),	\
	InitDftNoSArray30( SQLNTEXT	,							SQL_C_SSHORT)





//	User Data Type definitions.
//	Returned by SQLColAttributes/SQL_CA_SS_COLUMN_UTYPE.
//	Pre 7.0 server definitions

#define SQLSvrUsrTypes \
InitDftNoSArray10( SQLOldudtBINARY 	,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtBIT	,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtCHAR	,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtDATETIM4,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtDATETIME,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtDATETIMN,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtDECML		,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtDECMLN	,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtFLT4			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtFLT8			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtFLTN			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtIMAGE			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtINT1			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtINT2			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtINT4			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtINTN			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtMONEY			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtMONEY4			,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtMONEYN					,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtNUM					,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtNUMN					,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtSYSNAME				,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtTEXT					,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtTIMESTAMP				,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtVARBINARY				,							SQL_C_SSHORT),	\
InitDftNoSArray10( SQLOldudtVARCHAR				,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtBINARY					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtBIT						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtCHAR						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtDATETIM4					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtDATETIME					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtDATETIMN					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtDECML 					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtDECMLN					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtFLT4						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtFLT8						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtFLTN						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtIMAGE 					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtINT1						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtINT2					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtINT4						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtINTN						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtMONEY 					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtMONEY4					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtMONEYN					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtNUM						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtNUMN						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtSYSNAME					,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtTEXT						,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtTIMESTAMP 				,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtUNIQUEIDENTIFIER			,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtVARBINARY 				,							SQL_C_SSHORT),	\
InitDftNoSArray30( SQLudtVARCHAR					,							SQL_C_SSHORT)


//	SQL Server Data Type Tokens.
//	Returned by SQLColAttributes/SQL_CA_SS_COLUMN_SSTYPE.
//	Used for types in the bcp* apis.
DeclareString(SQLTEXT);
DeclareString(SQLVARBINARY);
DeclareString(SQLINTN);
DeclareString(SQLVARCHAR);
DeclareString(SQLBINARY);
DeclareString(SQLIMAGE);
DeclareString(SQLCHARACTER);
DeclareString(SQLINT1);
DeclareString(SQLBIT);
DeclareString(SQLINT2);
DeclareString(SQLINT4);
DeclareString(SQLMONEY);
DeclareString(SQLDATETIME);
DeclareString(SQLFLT8);
DeclareString(SQLFLTN);
DeclareString(SQLMONEYN);
DeclareString(SQLDATETIMN);
DeclareString(SQLFLT4);
DeclareString(SQLMONEY4);
DeclareString(SQLDATETIM4);
//	New types for 6.0 and later servers
DeclareString( SQLDEC);
DeclareString( SQLDECIMAL);
DeclareString( SQLNUM);
DeclareString( SQLNUMERIC);
//	New types for 7.0 and later servers
DeclareString( SQLUNIQUEID);
DeclareString( SQLBIGCHAR);
DeclareString( SQLBIGVARCHAR);
DeclareString( SQLBIGBINARY	);
DeclareString( SQLBIGVARBINARY);
DeclareString( SQLBITN);
DeclareString( SQLNCHAR	);
DeclareString( SQLNVARCHAR);
DeclareString( SQLNTEXT);

//	User Data Type definitions.
//	Returned by SQLColAttributes/SQL_CA_SS_COLUMN_UTYPE.
//	Pre 7.0 server definitions
DeclareString( SQLOldudtBINARY);
DeclareString( SQLOldudtBIT);
DeclareString( SQLOldudtCHAR);
DeclareString( SQLOldudtDATETIM4);
DeclareString( SQLOldudtDATETIME);
DeclareString( SQLOldudtDATETIMN);
DeclareString( SQLOldudtDECML);
DeclareString( SQLOldudtDECMLN);
DeclareString( SQLOldudtFLT4);
DeclareString( SQLOldudtFLT8);
DeclareString( SQLOldudtFLTN);
DeclareString( SQLOldudtIMAGE);
DeclareString( SQLOldudtINT1);
DeclareString( SQLOldudtINT2);
DeclareString( SQLOldudtINT4);
DeclareString( SQLOldudtINTN);
DeclareString( SQLOldudtMONEY);
DeclareString( SQLOldudtMONEY4);
DeclareString( SQLOldudtMONEYN);
DeclareString( SQLOldudtNUM	);
DeclareString( SQLOldudtNUMN);
DeclareString( SQLOldudtSYSNAME);
DeclareString( SQLOldudtTEXT);
DeclareString( SQLOldudtTIMESTAMP);
DeclareString( SQLOldudtVARBINARY);
DeclareString( SQLOldudtVARCHAR	);
//	7.0 server definitions
DeclareString( SQLudtBINARY	);
DeclareString( SQLudtBIT);
DeclareString( SQLudtCHAR);
DeclareString( SQLudtDATETIM4);
DeclareString( SQLudtDATETIME);
DeclareString( SQLudtDATETIMN);
DeclareString( SQLudtDECML 	);
DeclareString( SQLudtDECMLN	);
DeclareString( SQLudtFLT4	);
DeclareString( SQLudtFLT8);
DeclareString( SQLudtFLTN);
DeclareString( SQLudtIMAGE );
DeclareString( SQLudtINT1);
DeclareString( SQLudtINT2);
DeclareString( SQLudtINT4);
DeclareString( SQLudtINTN);
DeclareString( SQLudtMONEY );
DeclareString( SQLudtMONEY4);
DeclareString( SQLudtMONEYN);
DeclareString( SQLudtNUM);
DeclareString( SQLudtNUMN);
DeclareString( SQLudtSYSNAME);
DeclareString( SQLudtTEXT);
DeclareString( SQLudtTIMESTAMP);
DeclareString( SQLudtUNIQUEIDENTIFIER);
DeclareString( SQLudtVARBINARY);
DeclareString( SQLudtVARCHAR);


//	Aggregate operator types.
//	Returned by SQLColAttributes/SQL_CA_SS_COLUMN_OP.
DeclareString( SQLAOPCNT );
DeclareString( SQLAOPSUM );
DeclareString( SQLAOPAVG);
DeclareString( SQLAOPMIN);
DeclareString( SQLAOPMAX);
DeclareString( SQLAOPANY);
DeclareString( SQLAOPNOOP);


//	SQLGetInfo driver specific defines.
//	Microsoft has 1200 thru 1249 reserved for Microsoft SQL Server driver usage.


DeclareString( SQL_INFO_SS_NETLIB_NAME );



//	SQLGetDiagField driver specific defines.
//	Microsoft has -1150 thru -1199 reserved for Microsoft SQL Server driver usage.


DeclareString( SQL_DIAG_SS_MSGSTATE);
DeclareString( SQL_DIAG_SS_SEVERITY	);
DeclareString( SQL_DIAG_SS_SRVNAME );
DeclareString( SQL_DIAG_SS_PROCNAME);
DeclareString( SQL_DIAG_SS_LINE);


//	SQLGetDiagField/SQL_DIAG_DYNAMIC_FUNCTION_CODE driver specific defines.
//	Microsoft has -200 thru -299 reserved for Microsoft SQL Server driver usage.


DeclareString( SQL_DIAG_DFC_SS_ALTER_DATABASE);
DeclareString( SQL_DIAG_DFC_SS_CHECKPOINT);
DeclareString( SQL_DIAG_DFC_SS_CONDITION);
DeclareString( SQL_DIAG_DFC_SS_CREATE_DATABASE);
DeclareString( SQL_DIAG_DFC_SS_CREATE_DEFAULT);
DeclareString( SQL_DIAG_DFC_SS_CREATE_PROCEDURE);
DeclareString( SQL_DIAG_DFC_SS_CREATE_RULE );
DeclareString( SQL_DIAG_DFC_SS_CREATE_TRIGGER);
DeclareString( SQL_DIAG_DFC_SS_CURSOR_DECLARE);
DeclareString( SQL_DIAG_DFC_SS_CURSOR_OPEN );
DeclareString( SQL_DIAG_DFC_SS_CURSOR_FETCH	);
DeclareString( SQL_DIAG_DFC_SS_CURSOR_CLOSE	);
DeclareString( SQL_DIAG_DFC_SS_DEALLOCATE_CURSOR);
DeclareString( SQL_DIAG_DFC_SS_DBCC	);
DeclareString( SQL_DIAG_DFC_SS_DISK	);
DeclareString( SQL_DIAG_DFC_SS_DROP_DATABASE);
DeclareString( SQL_DIAG_DFC_SS_DROP_DEFAULT	);
DeclareString( SQL_DIAG_DFC_SS_DROP_PROCEDURE);
DeclareString( SQL_DIAG_DFC_SS_DROP_RULE);
DeclareString( SQL_DIAG_DFC_SS_DROP_TRIGGER);
DeclareString( SQL_DIAG_DFC_SS_DUMP_DATABASE);
DeclareString( SQL_DIAG_DFC_SS_DUMP_TABLE);
DeclareString( SQL_DIAG_DFC_SS_DUMP_TRANSACTION);
DeclareString( SQL_DIAG_DFC_SS_GOTO	);
DeclareString( SQL_DIAG_DFC_SS_INSERT_BULK);
DeclareString( SQL_DIAG_DFC_SS_KILL	);
DeclareString( SQL_DIAG_DFC_SS_LOAD_DATABASE);
DeclareString( SQL_DIAG_DFC_SS_LOAD_HEADERONLY);
DeclareString( SQL_DIAG_DFC_SS_LOAD_TABLE);
DeclareString( SQL_DIAG_DFC_SS_LOAD_TRANSACTION);
DeclareString( SQL_DIAG_DFC_SS_PRINT);
DeclareString( SQL_DIAG_DFC_SS_RAISERROR);
DeclareString( SQL_DIAG_DFC_SS_READTEXT	);
DeclareString( SQL_DIAG_DFC_SS_RECONFIGURE);
DeclareString( SQL_DIAG_DFC_SS_RETURN);
DeclareString( SQL_DIAG_DFC_SS_SELECT_INTO);
DeclareString( SQL_DIAG_DFC_SS_SET );
DeclareString( SQL_DIAG_DFC_SS_SET_IDENTITY_INSERT);
DeclareString( SQL_DIAG_DFC_SS_SET_ROW_COUNT);
DeclareString( SQL_DIAG_DFC_SS_SET_STATISTICS);
DeclareString( SQL_DIAG_DFC_SS_SET_TEXTSIZE	);
DeclareString( SQL_DIAG_DFC_SS_SETUSER );
DeclareString( SQL_DIAG_DFC_SS_SHUTDOWN	);
DeclareString( SQL_DIAG_DFC_SS_TRANS_BEGIN);
DeclareString( SQL_DIAG_DFC_SS_TRANS_COMMIT	);
DeclareString( SQL_DIAG_DFC_SS_TRANS_PREPARE);
DeclareString( SQL_DIAG_DFC_SS_TRANS_ROLLBACK);
DeclareString( SQL_DIAG_DFC_SS_TRANS_SAVE);
DeclareString( SQL_DIAG_DFC_SS_TRUNCATE_TABLE);
DeclareString( SQL_DIAG_DFC_SS_UPDATE_STATISTICS);
DeclareString( SQL_DIAG_DFC_SS_UPDATETEXT);
DeclareString( SQL_DIAG_DFC_SS_USE);
DeclareString( SQL_DIAG_DFC_SS_WAITFOR);
DeclareString( SQL_DIAG_DFC_SS_WRITETEXT);


//	SQLColAttributes driver specific defines.
//	SQLSet/GetDescField driver specific defines.
//	Microsoft has 1200 thru 1249 reserved for Microsoft SQL Server driver usage.


DeclareString(SQL_CA_SS_COLUMN_SSTYPE);
DeclareString(SQL_CA_SS_COLUMN_UTYPE);
DeclareString(SQL_CA_SS_NUM_ORDERS);
DeclareString(SQL_CA_SS_COLUMN_ORDER);
DeclareString(SQL_CA_SS_COLUMN_VARYLEN);
DeclareString(SQL_CA_SS_NUM_COMPUTES);
DeclareString(SQL_CA_SS_COMPUTE_ID);
DeclareString(SQL_CA_SS_COMPUTE_BYLIST);
DeclareString(SQL_CA_SS_COLUMN_ID);
DeclareString(SQL_CA_SS_COLUMN_OP);
DeclareString(SQL_CA_SS_COLUMN_SIZE);
DeclareString(SQL_CA_SS_COLUMN_HIDDEN);
DeclareString(SQL_CA_SS_COLUMN_KEY);



//	Defines for use with SQL_COPT_SS_USE_PROC_FOR_PREP

DeclareString(SQL_UP_OFF);
DeclareString( SQL_UP_ON);
DeclareString(SQL_UP_ON_DROP);
DeclareString(SQL_UP_DEFAULT);

//	Defines for use with SQL_COPT_SS_INTEGRATED_SECURITY - Pre-Connect Option only
DeclareString(SQL_IS_OFF);
DeclareString(SQL_IS_ON	);
DeclareString(SQL_IS_DEFAULT);

//	Defines for use with SQL_COPT_SS_PRESERVE_CURSORS
DeclareString(SQL_PC_OFF);
DeclareString(SQL_PC_ON);
DeclareString(SQL_PC_DEFAULT);

//	Defines for use with SQL_COPT_SS_ANSI_OEM
DeclareString(SQL_AO_OFF);
DeclareString(SQL_AO_ON);
DeclareString(SQL_AO_DEFAULT);

//	Defines for use with SQL_COPT_SS_CONNECTION_DEAD
DeclareString(SQL_CD_FALSE);
DeclareString(SQL_CD_TRUE);

//	Defines for use with SQL_COPT_SS_FALLBACK_CONNECT
DeclareString(SQL_FB_OFF);
DeclareString(SQL_FB_ON);
DeclareString(SQL_FB_DEFAULT);

//	Defines for use with SQL_COPT_SS_ENLIST_IN_DTC
//DeclareString(SQL_DTC_DONE);

//	Defines for use with SQL_COPT_SS_BCP
DeclareString(SQL_BCP_OFF);
DeclareString(SQL_BCP_ON);
DeclareString(SQL_BCP_DEFAULT);

//	Defines for use with SQL_COPT_SS_QUOTED_IDENT - Pre-Connect Option only
DeclareString(SQL_QI_OFF);
DeclareString(SQL_QI_ON);
DeclareString(SQL_QI_DEFAULT);

//	Defines for use with SQL_COPT_SS_ANSI_NPW - Pre-Connect Option only
DeclareString(SQL_AD_OFF);
DeclareString(SQL_AD_ON);
DeclareString(SQL_AD_DEFAULT);

// The following are options for SQL_COPT_SS_PERF_DATA and SQL_COPT_SS_PERF_QUERY
DeclareString(SQL_PERF_START);
DeclareString(SQL_PERF_STOP);

// The following are defines for SQL_COPT_SS_PERF_DATA_LOG
DeclareString(SQL_SS_DL_DEFAULT);

// The following are defines for SQL_COPT_SS_PERF_QUERY_LOG
DeclareString(SQL_SS_QL_DEFAULT);

// The following are defines for SQL_COPT_SS_PERF_QUERY_INTERVAL
DeclareString(SQL_SS_QI_DEFAULT);

//	Defines for use with SQL_SOPT_SS_TEXTPTR_LOGGING
DeclareString(SQL_TL_OFF);
DeclareString(SQL_TL_ON);
DeclareString(SQL_TL_DEFAULT);

//	Defines for use with SQL_SOPT_SS_HIDDEN_COLUMNS
DeclareString(SQL_HC_OFF);
DeclareString(SQL_HC_ON);
DeclareString(SQL_HC_DEFAULT);

//	Defines for use with SQL_SOPT_SS_NOBROWSETABLE
DeclareString(SQL_NB_OFF);
DeclareString(SQL_NB_ON);
DeclareString(SQL_NB_DEFAULT);






DeclareString(SQL_COPT_SS_REMOTE_PWD);
DeclareString(SQL_COPT_SS_USE_PROC_FOR_PREP);
DeclareString(SQL_COPT_SS_INTEGRATED_SECURITY);
DeclareString(SQL_COPT_SS_PRESERVE_CURSORS);
DeclareString(SQL_COPT_SS_USER_DATA);
DeclareString(SQL_COPT_SS_ANSI_OEM);
DeclareString(SQL_COPT_SS_ENLIST_IN_DTC);
DeclareString(SQL_COPT_SS_ENLIST_IN_XA);
DeclareString(SQL_COPT_SS_FALLBACK_CONNECT);
DeclareString(SQL_COPT_SS_CONNECTION_DEAD);
DeclareString(SQL_COPT_SS_PERF_DATA);
DeclareString(SQL_COPT_SS_PERF_DATA_LOG);
DeclareString(SQL_COPT_SS_PERF_QUERY_INTERVAL);
DeclareString(SQL_COPT_SS_PERF_QUERY_LOG);
DeclareString(SQL_COPT_SS_PERF_QUERY);
DeclareString(SQL_COPT_SS_PERF_DATA_LOG_NOW);
DeclareString(SQL_COPT_SS_QUOTED_IDENT);
DeclareString(SQL_COPT_SS_ANSI_NPW);
DeclareString(SQL_COPT_SS_BCP);

DeclareString(SQL_SOPT_SS_TEXTPTR_LOGGING);
DeclareString(SQL_SOPT_SS_CURRENT_COMMAND);
DeclareString(SQL_SOPT_SS_HIDDEN_COLUMNS);
DeclareString(SQL_SOPT_SS_NOBROWSETABLE);



#endif

//  SQL_ATTR_USE_LOCATORS
DeclareString(SQL_ATTR_USE_LOCATORS);
DeclareString(SQL_UL_ON);
DeclareString(SQL_UL_OFF);

/* SQLSetPos options */
DeclareString(SQL_POSITION);
DeclareString(SQL_REFRESH);
DeclareString(SQL_UPDATE);
DeclareString(SQL_DELETE);
DeclareString(SQL_ADD);
#if (ODBCVER >= 0x0300)
DeclareString(SQL_UPDATE_BY_BOOKMARK);
DeclareString(SQL_DELETE_BY_BOOKMARK);
DeclareString(SQL_FETCH_BY_BOOKMARK);
#endif


DeclareString(SQL_LOCK_NO_CHANGE);
DeclareString(SQL_LOCK_EXCLUSIVE);
DeclareString(SQL_LOCK_UNLOCK);

/*  Column types and scopes in SQLSpecialColumns.  */
DeclareString(SQL_BEST_ROWID);
DeclareString(SQL_ROWVER);

DeclareString(SQL_SCOPE_CURROW);
DeclareString(SQL_SCOPE_TRANSACTION);
DeclareString(SQL_SCOPE_SESSION);


/* Level 1 Prototypes */
DeclareString(SQLColumns);
DeclareString(SQLDriverConnect);
DeclareString(SQLGetConnectOption);
DeclareString(SQLGetData);
DeclareString(SQLGetNestedHandle);
DeclareString(SQLGetFunctions);
DeclareString(SQLGetInfo);
DeclareString(SQLGetStmtOption);
DeclareString(SQLGetTypeInfo);
DeclareString(SQLParamData);
DeclareString(SQLPutData);
DeclareString(SQLSetConnectOption);
DeclareString(SQLSetStmtOption);
DeclareString(SQLSpecialColumns);
DeclareString(SQLStatistics);
DeclareString(SQLStructuredTypeColumns);
DeclareString(SQLStructuredTypes);
DeclareString(SQLTables);
DeclareString(SQLNextColumn);


/* Level 2 Functions                             */

/* SQLExtendedFetch "fFetchType" values */
DeclareString(SQL_FETCH_NEXT);
DeclareString(SQL_FETCH_FIRST);
DeclareString(SQL_FETCH_LAST);
DeclareString(SQL_FETCH_PRIOR);
DeclareString(SQL_FETCH_ABSOLUTE);
DeclareString(SQL_FETCH_RELATIVE);
DeclareString(SQL_FETCH_BOOKMARK);

/* SQLExtendedFetch "rgfRowStatus" element values */
DeclareString(SQL_ROW_SUCCESS);
DeclareString(SQL_ROW_SUCCESS_WITH_INFO);
DeclareString(SQL_ROW_ERROR);
DeclareString(SQL_ROW_DELETED);
DeclareString(SQL_ROW_UPDATED);
DeclareString(SQL_ROW_NOROW);
DeclareString(SQL_ROW_ADDED);

/* Defines for SQLForeignKeys (returned in result set); */
DeclareString(SQL_CASCADE);
DeclareString(SQL_RESTRICT);
DeclareString(SQL_SET_NULL);

/* Defines for SQLProcedureColumns (returned in the result set) and by SQLBindParameter; */
DeclareString(SQL_PARAM_TYPE_UNKNOWN);
DeclareString(SQL_PARAM_INPUT);
DeclareString(SQL_PARAM_INPUT_OUTPUT);
DeclareString(SQL_PARAM_OUTPUT);
DeclareString(SQL_RESULT_COL);

/* Defines for SQLSetScrollOptions */
DeclareString(SQL_CONCUR_READ_ONLY);
DeclareString(SQL_CONCUR_LOCK);
DeclareString(SQL_CONCUR_VALUES);
DeclareString(SQL_CONCUR_ROWVER);

DeclareString(SQL_SCROLL_FORWARD_ONLY);
DeclareString(SQL_SCROLL_KEYSET_DRIVEN);
DeclareString(SQL_SCROLL_DYNAMIC);
DeclareString(SQL_SCROLL_STATIC);

/* Defines for SQLStatistics */
DeclareString(SQL_INDEX_UNIQUE);
DeclareString(SQL_INDEX_ALL);
DeclareString(SQL_ENSURE);
DeclareString(SQL_QUICK);

/* Defines for SQLStatistics (returned in the result set); */
DeclareString(SQL_TABLE_STAT);
DeclareString(SQL_INDEX_CLUSTERED);
DeclareString(SQL_INDEX_HASHED);
DeclareString(SQL_INDEX_OTHER);

/* Defines for SQLSetPos */
DeclareString(SQL_ENTIRE_ROWSET);

/* Level 2 Prototypes */
DeclareString(SQLBrowseConnect);
DeclareString(SQLColumnPrivileges);
DeclareString(SQLDataSources);
DeclareString(SQLDescribeParam);
DeclareString(SQLDrivers);
DeclareString(SQLExtendedFetch);
DeclareString(SQLForeignKeys);
DeclareString(SQLMoreResults);
DeclareString(SQLNativeSql);
DeclareString(SQLNumParams);
DeclareString(SQLParamOptions);
DeclareString(SQLPrimaryKeys);
DeclareString(SQLProcedureColumns);
DeclareString(SQLProcedures);
DeclareString(SQLSetPos);
DeclareString(SQLSetScrollOptions);
DeclareString(SQLTablePrivileges);
DeclareString(SQLBulkOperations);

/* Defines for SQLGetFunctions */
DeclareString(SQL_API_ALL_FUNCTIONS);
DeclareString(SQL_API_ODBC3_ALL_FUNCTIONS);

/* Core Function Prototypes */
DeclareString(SQL_API_SQLALLOCCONNECT);
DeclareString(SQL_API_SQLALLOCENV);
DeclareString(SQL_API_SQLALLOCSTMT);
DeclareString(SQL_API_SQLBINDCOL);
DeclareString(SQL_API_SQLCANCEL);
DeclareString(SQL_API_SQLCOLATTRIBUTES);
DeclareString(SQL_API_SQLCONNECT);
DeclareString(SQL_API_SQLDESCRIBECOL);
DeclareString(SQL_API_SQLDISCONNECT);
DeclareString(SQL_API_SQLERROR);
DeclareString(SQL_API_SQLEXECDIRECT);
DeclareString(SQL_API_SQLEXECUTE);
DeclareString(SQL_API_SQLFETCH);
DeclareString(SQL_API_SQLFREECONNECT);
DeclareString(SQL_API_SQLFREEENV);
DeclareString(SQL_API_SQLFREESTMT);
DeclareString(SQL_API_SQLGETCURSORNAME);
DeclareString(SQL_API_SQLNUMRESULTCOLS);
DeclareString(SQL_API_SQLPREPARE);
DeclareString(SQL_API_SQLROWCOUNT);
DeclareString(SQL_API_SQLSETCURSORNAME);
DeclareString(SQL_API_SQLSETPARAM);
DeclareString(SQL_API_SQLTRANSACT);

DeclareString(SQL_API_SQLALLOCHANDLE);
DeclareString(SQL_API_SQLFREEHANDLE);
DeclareString(SQL_API_SQLBINDPARAM);
DeclareString(SQL_API_SQLCLOSECURSOR);
DeclareString(SQL_API_SQLENDTRAN);
DeclareString(SQL_API_SQLFETCHSCROLL);
DeclareString(SQL_API_SQLGETENVATTR);
DeclareString(SQL_API_SQLSETENVATTR);
DeclareString(SQL_API_SQLGETCONNECTATTR);
DeclareString(SQL_API_SQLSETCONNECTATTR);
DeclareString(SQL_API_SQLGETSTMTATTR);
DeclareString(SQL_API_SQLSETSTMTATTR);
DeclareString(SQL_API_SQLCOPYDESC);
DeclareString(SQL_API_SQLGETDESCFIELD);
DeclareString(SQL_API_SQLSETDESCFIELD);
DeclareString(SQL_API_SQLGETDESCREC);
DeclareString(SQL_API_SQLSETDESCREC);
DeclareString(SQL_API_SQLGETDIAGFIELD);
DeclareString(SQL_API_SQLGETDIAGREC);

/* Level 1 Prototypes */
DeclareString(SQL_API_SQLCOLUMNS);
DeclareString(SQL_API_SQLDRIVERCONNECT);
DeclareString(SQL_API_SQLGETCONNECTOPTION);
DeclareString(SQL_API_SQLGETDATA);
DeclareString(SQL_API_SQLGETNESTEDHANDLE);
DeclareString(SQL_API_SQLGETFUNCTIONS);
DeclareString(SQL_API_SQLGETINFO);
DeclareString(SQL_API_SQLGETSTMTOPTION);
DeclareString(SQL_API_SQLGETTYPEINFO);
DeclareString(SQL_API_SQLPARAMDATA);
DeclareString(SQL_API_SQLPUTDATA);
DeclareString(SQL_API_SQLSETCONNECTOPTION);
DeclareString(SQL_API_SQLSETSTMTOPTION);
DeclareString(SQL_API_SQLSPECIALCOLUMNS);
DeclareString(SQL_API_SQLSTATISTICS);
DeclareString(SQL_API_SQLSTRUCTUREDTYPECOLUMNS);
DeclareString(SQL_API_SQLSTRUCTUREDTYPES);
DeclareString(SQL_API_SQLTABLES);
DeclareString(SQL_API_SQLNEXTCOLUMN);
/* Level 2 Prototypes */
DeclareString(SQL_API_SQLBROWSECONNECT);
DeclareString(SQL_API_SQLCOLUMNPRIVILEGES);
DeclareString(SQL_API_SQLDATASOURCES);
DeclareString(SQL_API_SQLDESCRIBEPARAM);
DeclareString(SQL_API_SQLDRIVERS);
DeclareString(SQL_API_SQLEXTENDEDFETCH);
DeclareString(SQL_API_SQLFOREIGNKEYS);
DeclareString(SQL_API_SQLMORERESULTS);
DeclareString(SQL_API_SQLNATIVESQL);
DeclareString(SQL_API_SQLNUMPARAMS);
DeclareString(SQL_API_SQLPARAMOPTIONS);
DeclareString(SQL_API_SQLPRIMARYKEYS);
DeclareString(SQL_API_SQLPROCEDURECOLUMNS);
DeclareString(SQL_API_SQLPROCEDURES);
DeclareString(SQL_API_SQLSETPOS);
DeclareString(SQL_API_SQLSETSCROLLOPTIONS);
DeclareString(SQL_API_SQLTABLEPRIVILEGES);
DeclareString(SQL_API_SQLBINDPARAMETER);
DeclareString(SQL_API_SQLCOLATTRIBUTE);
DeclareString(SQL_API_SQLLOCATOR);
DeclareString(SQL_API_SQLBULKOPERATIONS);

//@INSTALLER
DeclareString(IDM_SQLCONFIGDATASOURCE);
DeclareString(IDM_CONFIGDRIVER);
DeclareString(IDM_CONFIGDSN);
DeclareString(IDM_SQLCONFIGDRIVER);
DeclareString(IDM_CONFIGTRANSLATOR);
DeclareString(IDM_SQLDATASOURCETODRIVER);
DeclareString(IDM_SQLDRIVERTODATASOURCE);
DeclareString(IDM_SQLGETAVAILABLEDRIVERS);
DeclareString(IDM_SQLGETINSTALLEDDRIVERS);
DeclareString(IDM_SQLGETPRIVATEPROFILESTRING);
DeclareString(IDM_SQLGETTRANSLATOR);
DeclareString(IDM_SQLINSTALLDRIVER);
DeclareString(IDM_SQLINSTALLDRIVERMANAGER);
DeclareString(IDM_SQLINSTALLODBC);
DeclareString(IDM_SQLINSTALLTRANSLATOR);
DeclareString(IDM_SQLMANAGEDATASOURCES);
DeclareString(IDM_SQLREMOVEDEFAULTDATASOURCE);
DeclareString(IDM_SQLREMOVEDSNFROMINI);
DeclareString(IDM_SQLREMOVEDRIVER);
DeclareString(IDM_SQLREMOVEDRIVERMANAGER);
DeclareString(IDM_SQLREMOVETRANSLATOR);
DeclareString(IDM_SQLVALIDDSN);
DeclareString(IDM_SQLWRITEDSNTOINI);
DeclareString(IDM_SQLWRITEPRIVATEPROFILESTRING);

//------------------------------------------------------------------------------------------------
// Declare the Default Arrays:
//		These default arrays are placed in memory and later used in DLGPARMS.C.  They will be
//		used to log paramters, give user defaults, and other interfaces.
//------------------------------------------------------------------------------------------------

// TRUE/FALSE array
dCSEG(DFTARRAY) lpdSqlCodes[] = {
	InitDftNoSArray10(SQL_SUCCESS,				SQL_C_SLONG),
	InitDftNoSArray10(SQL_SUCCESS_WITH_INFO,	SQL_C_SLONG),
	InitDftNoSArray10(SQL_ERROR,					SQL_C_SLONG),
	InitDftNoSArray10(SQL_NO_DATA_FOUND,		SQL_C_SLONG),
	InitDftNoSArray10(SQL_INVALID_HANDLE,		SQL_C_SLONG),
	InitDftNoSArray10(SQL_STILL_EXECUTING,		SQL_C_SLONG),
	InitDftNoSArray10(SQL_NEED_DATA,				SQL_C_SLONG),
	InitDftNoSArray10(SQL_METADATA_CHANGED,		SQL_C_SLONG),
	InitDftNoSArray10(SQL_DATA_AVAILABLE, 		SQL_C_SLONG),
	};

// TRUE/FALSE array
dCSEG(DFTARRAY) lpdTrueFalse[] = {
	InitDftNoSArray10(TRUE,				SQL_C_USHORT),
	InitDftNoSArray10(FALSE,			SQL_C_USHORT),
	};


//SQL_DESC_UNNAMED
dCSEG(DFTARRAY) lpdUnnamed[] = {
	InitDftNoSArray30(SQL_NAMED, SQL_C_USHORT),
	InitDftNoSArray30(SQL_UNNAMED, SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLDriverConnect constants:
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfDrvrConn[] = {
	InitDftNoSArray10(SQL_DRIVER_NOPROMPT, SQL_C_USHORT),
	InitDftNoSArray10(SQL_DRIVER_COMPLETE, SQL_C_USHORT),
	InitDftNoSArray10(SQL_DRIVER_PROMPT, SQL_C_USHORT),
	InitDftNoSArray10(SQL_DRIVER_COMPLETE_REQUIRED, SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdIntervalTypes[] = {
	InitDftNoSArray30(SQL_IS_YEAR,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_MONTH,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_DAY,					SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_HOUR,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_MINUTE,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_SECOND,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_YEAR_TO_MONTH,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_DAY_TO_HOUR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_DAY_TO_MINUTE,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_DAY_TO_SECOND,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_HOUR_TO_MINUTE,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_HOUR_TO_SECOND,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_IS_MINUTE_TO_SECOND,SQL_C_USHORT),
	};


#define CTYPES \
	InitDftNoSArray10(SQL_C_CHAR,			SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_BINARY,		SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_BIT,			SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_BOOKMARK,	SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_DATE,			SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_DOUBLE,		SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_FLOAT,		SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_GUID,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_DAY,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_DAY_TO_HOUR,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_DAY_TO_MINUTE,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_DAY_TO_SECOND,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_HOUR,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_HOUR_TO_MINUTE,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_HOUR_TO_SECOND,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_MINUTE_TO_SECOND,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_MONTH,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_MINUTE,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_SECOND,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_YEAR,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_INTERVAL_YEAR_TO_MONTH,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_LONG,			SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_NUMERIC,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_SBIGINT,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_SLONG,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_SHORT,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_SSHORT,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_STINYINT,	SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_TIME,			SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_TIMESTAMP,	SQL_C_SSHORT), \
	InitDftNoSArray10(SQL_C_TINYINT,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_TYPE_DATE,	SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_TYPE_TIME,	SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_TYPE_TIMESTAMP,	SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_UBIGINT,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_ULONG,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_USHORT,		SQL_C_SSHORT), \
	InitDftNoSArray20(SQL_C_UTINYINT,	SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_C_VARBOOKMARK,SQL_C_SSHORT), \
 	InitDftNoSArray30(SQL_C_WCHAR,		SQL_C_SSHORT), \
	InitDftNoSArray30(SQL_ARD_TYPE,	SQL_C_SSHORT),

//------------------------------------------------------------------------------------------------
// fCType constants:
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfCTypes[] = {
	CTYPES
	InitDftNoSArray10(SQL_C_DEFAULT,		SQL_C_SSHORT),
	};


//------------------------------------------------------------------------------------------------
// fCType constants without SQL_C_DEFAULT:
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfCTypesNoD[] = {
	CTYPES
	};



//------------------------------------------------------------------------------------------------
// fSqlType constants:
//------------------------------------------------------------------------------------------------

// This one contains SQL_ALL_TYPES
dCSEG(DFTARRAY) lpdfSqlTypes[] = {
// Extension SQL datatypes:
	InitDftNoSArray10(SQL_ALL_TYPES,		SQL_C_SSHORT),
	SqlTypesInfo
	};


// Descriptor Subtypes
dCSEG(DFTARRAY) lpdfSqlSubTypes[] = {
	InitDftNoSArray30(SQL_CODE_DATE,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_TIME,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_TIMESTAMP,		SQL_C_SSHORT),
	};


// This one doesn't
dCSEG(DFTARRAY) lpdfSqlTypesNoAll[] = {
	SqlTypesInfo
	};


//------------------------------------------------------------------------------------------------
// SQLDataSources constants:
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfDataSrcs[] = {
	InitDftNoSArray10(SQL_FETCH_FIRST, SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_NEXT,  SQL_C_USHORT),
	};


//------------------------------------------------------------------------------------------------
// SQLGetInfo values:
//------------------------------------------------------------------------------------------------

//  SQL_FETCH_DIRECTION
dCSEG(DFTARRAY) lpdFetchDir[] = {
	InitDftNoSArrayVal10(SQL_FD_FETCH_NEXT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FD_FETCH_FIRST,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FD_FETCH_LAST,	 	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FD_FETCH_PRIOR,    	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FD_FETCH_ABSOLUTE, 	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FD_FETCH_RELATIVE, 	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FD_FETCH_BOOKMARK, 	SQL_C_ULONG, 	PRM_32MSK),
	//InitDftNoSArrayVal30(SQL_FD_FETCH_FIND,	 	SQL_C_ULONG, 	PRM_32MSK),
	};


dCSEG(DFTARRAY) lpdInterfaceConformance[] = {
	InitDftNoSArrayVal30(SQL_OIC_CORE,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_OIC_LEVEL1,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_OIC_LEVEL2,	SQL_C_USHORT,	PRM_32MSK),
	};

//  SQL_API_CONFORMANCE
dCSEG(DFTARRAY) lpdAPIConf[] = {
	InitDftNoSArrayVal10(SQL_OAC_NONE,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_OAC_LEVEL1,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_OAC_LEVEL2, 	SQL_C_USHORT, 	PRM_16BIT),
	};

dCSEG(DFTARRAY) lpdStandardCLIConformance[] = {
	InitDftNoSArrayVal30(SQL_SCC_XOPEN_CLI_VERSION1,SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_SCC_ISO92_CLI,SQL_C_USHORT,	PRM_32BIT),
};


//  SQL_ODBC_SAG_CLI_CONFORMANCE
dCSEG(DFTARRAY) lpdSAGConf[] = {
	InitDftNoSArrayVal10(SQL_OSCC_NOT_COMPLIANT,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_OSCC_COMPLIANT,		SQL_C_USHORT,	PRM_16BIT),
	};

//  SQL_ODBC_SQL_CONFORMANCE
dCSEG(DFTARRAY) lpdSQLConf[] = {
	InitDftNoSArrayVal10(SQL_OSC_MINIMUM,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_OSC_CORE,			SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_OSC_EXTENDED, 	SQL_C_USHORT, 	PRM_16BIT),
	};


//  SQL_CONCAT_NULL_BEHAVIOR
dCSEG(DFTARRAY) lpdConcatBehav[] = {
	InitDftNoSArrayVal10(SQL_CB_NULL,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_CB_NON_NULL,	SQL_C_USHORT,	PRM_16BIT),
	};

dCSEG(DFTARRAY) lpdCursorSensitivity[] = {
	InitDftNoSArrayVal30(SQL_INSENSITIVE,	SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_UNSPECIFIED,	SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_SENSITIVE,		SQL_C_USHORT,	PRM_32BIT),
	};

//  SQL_CURSOR_COMMIT_BEHAVIOR
dCSEG(DFTARRAY) lpdCommitBehav[] = {
	InitDftNoSArrayVal10(SQL_CB_DELETE,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_CB_CLOSE,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_CB_PRESERVE, 	SQL_C_USHORT, 	PRM_16BIT),
	};

//  SQL_DEFAULT_TXN_ISOLATION and SQL_TXN_ISOLATION_OPTION  // Also used by SQLSetConnectOption and SQLGetConnectOption
dCSEG(DFTARRAY) lpdTxnIsol[] = {
	InitDftNoSArrayVal10(SQL_TXN_READ_UNCOMMITTED,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_TXN_READ_COMMITTED,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_TXN_REPEATABLE_READ, 	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_TXN_SERIALIZABLE,    	SQL_C_ULONG, 	PRM_32MSK),
	};

// SQL_QUALIFIER_LOCATION
dCSEG(DFTARRAY) lpdQualLocate[] = {
	InitDftNoSArrayVal20(SQL_QL_START,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_QL_END,			SQL_C_USHORT,	PRM_16BIT),
	};

dCSEG(DFTARRAY) lpdSchemaUsage[] = {
	InitDftNoSArrayVal30(SQL_SU_DML_STATEMENTS,			SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SU_PROCEDURE_INVOCATION,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SU_TABLE_DEFINITION,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SU_INDEX_DEFINITION,		SQL_C_USHORT,	PRM_32MSK),
	};

//  SQL_IDENTIFIER_CASE
dCSEG(DFTARRAY) lpdIdentCase[] = {
	InitDftNoSArrayVal10(SQL_IC_UPPER,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_IC_LOWER,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_IC_SENSITIVE,	SQL_C_USHORT, 	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_IC_MIXED,	 	SQL_C_USHORT, 	PRM_16BIT),
	};

//  SQL_SCROLL_CONCURRENCY
dCSEG(DFTARRAY) lpdScrollConcur[] = {
	InitDftNoSArrayVal10(SQL_SCCO_READ_ONLY,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_SCCO_LOCK,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_SCCO_OPT_ROWVER,	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_SCCO_OPT_VALUES,	SQL_C_ULONG, 	PRM_32MSK),
	};

//  SQL_SCROLL_OPTIONS
dCSEG(DFTARRAY) lpdScrollOpts[] = {
	InitDftNoSArrayVal10(SQL_SO_FORWARD_ONLY,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SO_STATIC,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_SO_KEYSET_DRIVEN,	SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_SO_DYNAMIC,			SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_SO_MIXED,				SQL_C_ULONG, 	PRM_32MSK),
	};

//  SQL_TXN_CAPABLE
dCSEG(DFTARRAY) lpdTxnCapable[] = {
	InitDftNoSArrayVal10(SQL_TC_NONE,			SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_TC_DML,				SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_TC_DDL_COMMIT,	SQL_C_USHORT, 	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_TC_DDL_IGNORE,	SQL_C_USHORT, 	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_TC_ALL,	 			SQL_C_USHORT, 	PRM_16BIT),
	};

//  SQL_AGGREGATE_FUNCTIONS
dCSEG(DFTARRAY) lpdAggFunct[] = {
	InitDftNoSArrayVal30(SQL_AF_AVG,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AF_COUNT,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AF_MAX,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AF_MIN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AF_SUM,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AF_DISTINCT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AF_ALL,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_EVERY,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_ANY,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_STDEV_OP,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_STDEV_SAMP,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_VAR_SAMP,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_VAR_POP,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_ARRAY_AGG,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_COLLECT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_AF_INTERSECTION,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_CONVERT_FUNCTIONS
dCSEG(DFTARRAY) lpdConvFunct[] = {
	InitDftNoSArrayVal10(SQL_FN_CVT_CONVERT,		SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_NUMERIC_FUNCTIONS
dCSEG(DFTARRAY) lpdNumFunct[] = {
	InitDftNoSArrayVal10(SQL_FN_NUM_ABS,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_ACOS,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_ASIN,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_ATAN,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_ATAN2,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_CEILING,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_COS,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_COT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_NUM_DEGREES,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_EXP,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_FLOOR,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_LOG,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_NUM_LOG10,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_MOD,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_PI,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_NUM_POWER,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_NUM_RADIANS,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_RAND,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_NUM_ROUND,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_SIGN,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_SIN,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_SQRT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_NUM_TAN,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_NUM_TRUNCATE,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_STRING_FUNCTIONS
dCSEG(DFTARRAY) lpdStrFunct[] = {
	InitDftNoSArrayVal10(SQL_FN_STR_ASCII,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_STR_BIT_LENGTH,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_CHAR,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_STR_CHAR_LENGTH,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_STR_CHARACTER_LENGTH,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_CONCAT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_STR_DIFFERENCE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_INSERT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_LCASE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_LEFT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_LENGTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_LOCATE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_STR_LOCATE_2,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_LTRIM,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_STR_OCTET_LENGTH,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_STR_POSITION,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_REPEAT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_REPLACE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_RIGHT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_RTRIM,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_STR_SOUNDEX,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_STR_SPACE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_SUBSTRING,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_STR_UCASE,					SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_BINARY_FUNCTIONS
dCSEG(DFTARRAY) lpdBinFunct[] = {
	InitDftNoSArrayVal40(SQL_FN_BIN_BIT_LENGTH,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_CONCAT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_INSERT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_LTRIM,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_OCTET_LENGTH,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_POSITION,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_RTRIM,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_FN_BIN_SUBSTRING,			SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_ISO_BINARY_FUNCTIONS
dCSEG(DFTARRAY) lpdISOBinFunct[] = {
	InitDftNoSArrayVal40(SQL_SBF_CONVERT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_SUBSTRING,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_TRIM_BOTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_TRIM_LEADING,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_TRIM_TRAILING,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_OVERLAY,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_LENGTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_POSITION,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SBF_CONCAT,				SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_SYSTEM_FUNCTIONS
dCSEG(DFTARRAY) lpdSysFunct[] = {
	InitDftNoSArrayVal10(SQL_FN_SYS_DBNAME,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_SYS_IFNULL,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_SYS_USERNAME,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_TIMEDATE_FUNCTIONS
dCSEG(DFTARRAY) lpdTimeDateFunct[] = {
	InitDftNoSArrayVal30(SQL_FN_TD_CURRENT_DATE,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_TD_CURRENT_TIME,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_TD_CURRENT_TIMESTAMP,SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_CURDATE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_CURTIME,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TD_DAYNAME,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_DAYOFWEEK,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_DAYOFMONTH,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_DAYOFYEAR,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_FN_TD_EXTRACT,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_HOUR,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_MINUTE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_MONTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TD_MONTHNAME,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_NOW,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_QUARTER,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_SECOND,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TD_TIMESTAMPADD,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TD_TIMESTAMPDIFF,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_WEEK,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_FN_TD_YEAR,				SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_CONVERT_????????? -- Used by all 1.0 SQL_CONVERT types
dCSEG(DFTARRAY) lpdConvert10[] = {
	InitDftNoSArrayVal10(SQL_CVT_BIGINT,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_BINARY,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_BIT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_CHAR,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_DATE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_DECIMAL,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_DOUBLE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_FLOAT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_INTEGER,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_LONGVARBINARY,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_LONGVARCHAR,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_NUMERIC,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_REAL,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_SMALLINT,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_TIME,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_TIMESTAMP,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_TINYINT,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_VARBINARY,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal10(SQL_CVT_VARCHAR,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CVT_WCHAR,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CVT_WLONGVARCHAR,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CVT_WVARCHAR,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CVT_INTERVAL_YEAR_MONTH,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CVT_INTERVAL_DAY_TIME,		SQL_C_ULONG,	PRM_32MSK),
	};


//  SQL_LIMIT_ESCAPE_CLAUSE
dCSEG(DFTARRAY) lpdLimitEscapeClause[] = {
	InitDftNoSArrayVal40(SQL_LC_NONE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_LC_TAKE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_LC_SKIP,					SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_RETURN_ESCAPE_CLAUSE
dCSEG(DFTARRAY) lpdReturnEscapeClause[] = {
	InitDftNoSArrayVal40(SQL_RC_NONE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_INSERT_SINGLE_ROWID,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_INSERT_SINGLE_ANY,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_INSERT_MULTIPLE_ROWID,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_INSERT_MULTIPLE_ANY,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_INSERT_SELECT_ROWID,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_INSERT_SELECT_ANY,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_UPDATE_ROWID,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_UPDATE_ANY,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_DELETE_ROWID,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_DELETE_ANY,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_SELECT_INTO_ROWID,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_RC_SELECT_INTO_ANY,		SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_LOCK_TYPES
dCSEG(DFTARRAY) lpdLockTypes[] = {
	InitDftNoSArrayVal20(SQL_LCK_NO_CHANGE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_LCK_EXCLUSIVE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_LCK_UNLOCK,		SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_POS_OPERATIONS
dCSEG(DFTARRAY) lpdPosOps[] = {
	InitDftNoSArrayVal20(SQL_POS_POSITION,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_POS_REFRESH,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_POS_UPDATE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_POS_DELETE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_POS_ADD,		SQL_C_ULONG,	PRM_32MSK),
	};


//  SQL_CATALOG_USAGE
dCSEG(DFTARRAY) lpdCatUsage[] = {
	InitDftNoSArrayVal30(SQL_CU_DML_STATEMENTS,			SQL_C_ULONG,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_CU_PROCEDURE_INVOCATION,	SQL_C_ULONG,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_CU_TABLE_DEFINITION,		SQL_C_ULONG,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_CU_INDEX_DEFINITION,		SQL_C_ULONG,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_CU_PRIVILEGE_DEFINITION,	SQL_C_ULONG,	PRM_16BIT),
	};


//  SQL_CATALOG_LOCATION
dCSEG(DFTARRAY) lpdCLLocation[] = {
	InitDftNoSArrayVal30(SQL_CL_START,	SQL_C_ULONG,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_CL_END	,	SQL_C_ULONG,	PRM_16BIT),
	};


//  SQL_BOOKMARK_PERSISTENCE
dCSEG(DFTARRAY) lpdBMPersist[] = {
	InitDftNoSArrayVal20(SQL_BP_CLOSE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_BP_DELETE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_BP_DROP,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_BP_SCROLL,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_BP_TRANSACTION,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_BP_UPDATE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_BP_OTHER_HSTMT,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_STATIC_SENSITIVITY
dCSEG(DFTARRAY) lpdStaticSensitivity[] = {
	InitDftNoSArrayVal20(SQL_SS_ADDITIONS,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SS_DELETIONS,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SS_UPDATES,		SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_FILE_USAGE
dCSEG(DFTARRAY) lpdFileUsage[] = {
	InitDftNoSArrayVal20(SQL_FILE_NOT_SUPPORTED,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_FILE_TABLE,			SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_FILE_QUALIFIER,		SQL_C_USHORT, 	PRM_16BIT),
	};

//  SQL_FILE_USAGE
dCSEG(DFTARRAY) lpdFileUsage30[] = {
	InitDftNoSArrayVal20(SQL_FILE_NOT_SUPPORTED,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_FILE_TABLE,			SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_FILE_CATALOG,			SQL_C_USHORT, 	PRM_16BIT),
	};

//  SQL_GETDATA_EXTENSIONS
dCSEG(DFTARRAY) lpdGetDataExt[] = {
	InitDftNoSArrayVal20(SQL_GD_ANY_COLUMN,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_GD_ANY_ORDER,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_GD_BLOCK,			SQL_C_ULONG, 	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_GD_BOUND,			SQL_C_ULONG, 	PRM_32MSK),
	};

//  SQL_NULL_COLLATION
dCSEG(DFTARRAY) lpdNullColl[] = {
	InitDftNoSArrayVal20(SQL_NC_HIGH,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_NC_LOW,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_NC_START,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_NC_END,		SQL_C_USHORT,	PRM_16BIT),

	};

//  SQL_ALTER_TABLE
dCSEG(DFTARRAY) lpdAlterTable[] = {
	InitDftNoSArrayVal30(SQL_AT_ADD_COLUMN,							SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_DROP_COLUMN,							SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_ADD_CONSTRAINT,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_ADD_COLUMN_SINGLE,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_ADD_COLUMN_DEFAULT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_ADD_COLUMN_COLLATION,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_SET_COLUMN_DEFAULT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_DROP_COLUMN_DEFAULT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_DROP_COLUMN_CASCADE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_DROP_COLUMN_RESTRICT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_ADD_TABLE_CONSTRAINT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_CONSTRAINT_NAME_DEFINITION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_CONSTRAINT_INITIALLY_DEFERRED,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AT_CONSTRAINT_DEFERRABLE,				SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_ALTER_DOMAIN
dCSEG(DFTARRAY) lpdAlterDomain[] = {
	InitDftNoSArrayVal30(SQL_AD_CONSTRAINT_NAME_DEFINITION,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_ADD_DOMAIN_CONSTRAINT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_DROP_DOMAIN_CONSTRAINT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_ADD_DOMAIN_DEFAULT,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_DROP_DOMAIN_DEFAULT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE,SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_ADD_CONSTRAINT_DEFERRABLE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE,		SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_CORRELATION_NAME
dCSEG(DFTARRAY) lpdCorrelationName[] = {
	InitDftNoSArrayVal10(SQL_CN_NONE,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_CN_DIFFERENT,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_CN_ANY,			SQL_C_USHORT,	PRM_16BIT),
	};

/* SQL_CREATE_ASSERTION */
dCSEG(DFTARRAY) lpdCreateAssertion[] = {
	InitDftNoSArrayVal30(SQL_CA_CREATE_ASSERTION,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA_CONSTRAINT_INITIALLY_DEFERRED,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA_CONSTRAINT_INITIALLY_IMMEDIATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA_CONSTRAINT_DEFERRABLE,				SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA_CONSTRAINT_NON_DEFERRABLE,			SQL_C_USHORT,	PRM_32MSK),
	};

/* SQL_CREATE_CHARACTER_SET */
dCSEG(DFTARRAY) lpdCreateCharSet[] = {
	InitDftNoSArrayVal30(SQL_CCS_CREATE_CHARACTER_SET,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CCS_COLLATE_CLAUSE,			SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CCS_LIMITED_COLLATION,		SQL_C_USHORT,	PRM_32MSK),
};


/* SQL_CREATE_COLLATION */
dCSEG(DFTARRAY) lpdCreateCollation[] = {
	InitDftNoSArrayVal30(SQL_CCOL_CREATE_COLLATION,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdCreateDomain[] = {
	InitDftNoSArrayVal30(SQL_CDO_CREATE_DOMAIN,						SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_DEFAULT,								SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_CONSTRAINT,							SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_COLLATION,							SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_CONSTRAINT_NAME_DEFINITION,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE,SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_CONSTRAINT_DEFERRABLE,			SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CDO_CONSTRAINT_NON_DEFERRABLE,		SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdCreateSchema[] = {
	InitDftNoSArrayVal30(SQL_CS_CREATE_SCHEMA,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CS_AUTHORIZATION,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CS_DEFAULT_CHARACTER_SET,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdCreateTable[] = {
	InitDftNoSArrayVal30(SQL_CT_CREATE_TABLE,							SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_COMMIT_PRESERVE,						SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_COMMIT_DELETE,						SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_GLOBAL_TEMPORARY,					SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_LOCAL_TEMPORARY,						SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_CONSTRAINT_INITIALLY_DEFERRED,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_CONSTRAINT_DEFERRABLE,				SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_CONSTRAINT_NON_DEFERRABLE,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_COLUMN_CONSTRAINT,					SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_COLUMN_DEFAULT,						SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_COLUMN_COLLATION,					SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_TABLE_CONSTRAINT,					SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CT_CONSTRAINT_NAME_DEFINITION,		SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdCreateTranslation[] = {
	InitDftNoSArrayVal30(SQL_CTR_CREATE_TRANSLATION,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdCreateView[] = {
	InitDftNoSArrayVal30(SQL_CV_CREATE_VIEW,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CV_CHECK_OPTION,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CV_CASCADED,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CV_LOCAL,			SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropAssertion[] = {
	InitDftNoSArrayVal30(SQL_DA_DROP_ASSERTION,		SQL_C_USHORT,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdDropCharSet[] = {
	InitDftNoSArrayVal30(SQL_DCS_DROP_CHARACTER_SET,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropCollation[] = {
	InitDftNoSArrayVal30(SQL_DC_DROP_COLLATION,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropDomain[] = {
	InitDftNoSArrayVal30(SQL_DD_DROP_DOMAIN,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DD_RESTRICT,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DD_CASCADE,		SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropSchema[] = {
	InitDftNoSArrayVal30(SQL_DS_DROP_SCHEMA,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DS_RESTRICT,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DS_CASCADE,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropTable[] = {
	InitDftNoSArrayVal30(SQL_DT_DROP_TABLE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DT_RESTRICT,		SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DT_CASCADE,		SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropTranslation[] = {
	InitDftNoSArrayVal30(SQL_DTR_DROP_TRANSLATION,	SQL_C_USHORT,	PRM_32MSK),
};


dCSEG(DFTARRAY) lpdCA1[] = {
	InitDftNoSArrayVal30(SQL_CA1_NEXT,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_ABSOLUTE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_RELATIVE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_BOOKMARK,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_LOCK_NO_CHANGE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_LOCK_EXCLUSIVE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_LOCK_UNLOCK,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_POS_POSITION,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_POS_UPDATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_POS_DELETE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_POS_REFRESH,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_POSITIONED_UPDATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_POSITIONED_DELETE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_SELECT_FOR_UPDATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_BULK_ADD,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_BULK_UPDATE_BY_BOOKMARK,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_BULK_DELETE_BY_BOOKMARK,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA1_BULK_FETCH_BY_BOOKMARK,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdCA2[] = {
	InitDftNoSArrayVal30(SQL_CA2_READ_ONLY_CONCURRENCY,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_LOCK_CONCURRENCY,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_OPT_ROWVER_CONCURRENCY,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_OPT_VALUES_CONCURRENCY,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_SENSITIVITY_ADDITIONS,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_SENSITIVITY_DELETIONS,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_SENSITIVITY_UPDATES,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_MAX_ROWS_SELECT,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_MAX_ROWS_INSERT,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_MAX_ROWS_DELETE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_MAX_ROWS_UPDATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_MAX_ROWS_CATALOG,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_MAX_ROWS_AFFECTS_ALL,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_CRC_EXACT,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_CRC_APPROXIMATE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_SIMULATE_NON_UNIQUE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_SIMULATE_TRY_UNIQUE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_CA2_SIMULATE_UNIQUE,	SQL_C_USHORT,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdDropView[] = {
	InitDftNoSArrayVal30(SQL_DV_DROP_VIEW,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DV_CASCADE,	SQL_C_USHORT,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DV_RESTRICT,	SQL_C_USHORT,	PRM_32MSK),
};

//  SQL_GROUP_BY
dCSEG(DFTARRAY) lpdGroupBy[] = {
	InitDftNoSArrayVal20(SQL_GB_NOT_SUPPORTED,				SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_GB_GROUP_BY_EQUALS_SELECT,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_GB_GROUP_BY_CONTAINS_SELECT,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_GB_NO_RELATION,					SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_GB_COLLATE,						SQL_C_USHORT,	PRM_16BIT),
	};

//  SQL_NON_NULLABLE_COLUMNS
dCSEG(DFTARRAY) lpdNonNullCols[] = {
	InitDftNoSArrayVal10(SQL_NNC_NULL,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal10(SQL_NNC_NON_NULL,	SQL_C_USHORT,	PRM_16BIT),
	};

//  SQL_OWNER_USAGE
dCSEG(DFTARRAY) lpdOwnerUsage[] = {
	InitDftNoSArrayVal20(SQL_OU_DML_STATEMENTS,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OU_PROCEDURE_INVOCATION,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OU_TABLE_DEFINITION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OU_INDEX_DEFINITION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OU_PRIVILEGE_DEFINITION,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_POSITIONED_STATEMENTS
dCSEG(DFTARRAY) lpdPosStmts[] = {
	InitDftNoSArrayVal20(SQL_PS_POSITIONED_DELETE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_PS_POSITIONED_UPDATE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_PS_SELECT_FOR_UPDATE,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_QUALIFIER_USAGE
dCSEG(DFTARRAY) lpdQualUsage[] = {
	InitDftNoSArrayVal20(SQL_QU_DML_STATEMENTS,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_QU_PROCEDURE_INVOCATION,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_QU_TABLE_DEFINITION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_QU_INDEX_DEFINITION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_QU_PRIVILEGE_DEFINITION,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_QUOTED_IDENTIFIER_CASE
dCSEG(DFTARRAY) lpdQuoteIdentCase[] = {
	InitDftNoSArrayVal20(SQL_IC_UPPER,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_IC_LOWER,		SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_IC_SENSITIVE,	SQL_C_USHORT,	PRM_16BIT),
	InitDftNoSArrayVal20(SQL_IC_MIXED,		SQL_C_USHORT,	PRM_16BIT),
	};

//  SQL_SUBQUERIES
dCSEG(DFTARRAY) lpdSubqueries[] = {
	InitDftNoSArrayVal20(SQL_SQ_COMPARISON,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SQ_EXISTS,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SQ_IN,							SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SQ_QUANTIFIED,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_SQ_CORRELATED_SUBQUERIES,	SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_UNION
dCSEG(DFTARRAY) lpdUnion[] = {
	InitDftNoSArrayVal20(SQL_U_UNION,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_U_UNION_ALL,					SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_TIMEDATE_ADD_INTERVALS and SQL_TIMEDATE_DIFF_INTERVALS
dCSEG(DFTARRAY) lpdTimeDateInts[] = {
	InitDftNoSArrayVal20(SQL_FN_TSI_FRAC_SECOND,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_SECOND,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_MINUTE,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_HOUR,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_DAY,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_WEEK,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_MONTH,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_QUARTER,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_FN_TSI_YEAR,			SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_OJ_CAPABILITIES
// Bug #345
dCSEG(DFTARRAY) lpdOJCap[] = {
	InitDftNoSArrayVal20(SQL_OJ_LEFT,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OJ_RIGHT,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OJ_FULL,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OJ_NESTED,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OJ_NOT_ORDERED,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OJ_INNER,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal20(SQL_OJ_ALL_COMPARISON_OPS,		SQL_C_ULONG,	PRM_32MSK)
	};

//	SQL_INDEX_KEYWORDS
dCSEG(DFTARRAY) lpdIndexKeyWords[] = {
	InitDftNoSArrayVal30(SQL_IK_NONE,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_IK_ASC,							SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_IK_DESC,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_IK_ALL,							SQL_C_ULONG,	PRM_32MSK),
	};

//  SQL_INFO_SCHEMA_VIEWS
dCSEG(DFTARRAY) lpdInfoSchemaViews[] = {
	InitDftNoSArrayVal30(SQL_ISV_ASSERTIONS,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_CHARACTER_SETS,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_CHECK_CONSTRAINTS,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_COLLATIONS,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_COLUMN_DOMAIN_USAGE,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_COLUMN_PRIVILEGES,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_COLUMNS,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_CONSTRAINT_COLUMN_USAGE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_CONSTRAINT_TABLE_USAGE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_DOMAIN_CONSTRAINTS,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_DOMAINS,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_KEY_COLUMN_USAGE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_REFERENTIAL_CONSTRAINTS,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_SCHEMATA,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_SQL_LANGUAGES,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_TABLE_CONSTRAINTS,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_TABLE_PRIVILEGES,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_TABLES,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_TRANSLATIONS,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_USAGE_PRIVILEGES,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_VIEW_COLUMN_USAGE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_VIEW_TABLE_USAGE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_ISV_VIEWS,							SQL_C_ULONG,	PRM_32MSK),
	};


dCSEG(DFTARRAY) lpdAsyncMode[] = {
	InitDftNoSArrayVal30(SQL_AM_CONNECTION,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_AM_STATEMENT,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_AM_NONE,				SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdANSISQLConformance[] = {
	InitDftNoSArrayVal30(SQL_SC_SQL92_ENTRY,				SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_SC_FIPS127_2_TRANSITIONAL,SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_SC_SQL92_INTERMEDIATE,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_SC_SQL92_FULL,				SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdANSIDateTimeLiterals[] = {
	InitDftNoSArrayVal30(SQL_DL_SQL92_DATE,							SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_TIME,							SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_TIMESTAMP,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_YEAR,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_MONTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_DAY,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_HOUR,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_MINUTE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_SECOND,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND,SQL_C_ULONG,	PRM_32MSK),
};


#ifdef SQL_LL_DATA_TYPES

dCSEG(DFTARRAY) lpdLobLocator[] = {
	InitDftNoSArrayVal30(SQL_LL_DATA_TYPES,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_LL_SQL,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_LL_LONG_DATA_TYPES,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_LL_EXPLICIT_FREE,		SQL_C_ULONG,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdLocatorUpdatability[] = {
	InitDftNoSArrayVal30(SQL_LU_NOT_UPDATABLE,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_LU_DIRECT_ONLY,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_LU_ALL,					SQL_C_ULONG,	PRM_32BIT),
};


dCSEG(DFTARRAY) lpdLocatorSensitivity[] = {
	InitDftNoSArrayVal30(SQL_LS_STATIC,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_LS_DYNAMIC,		SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdPredicateFind[] = {
	InitDftNoSArrayVal30(SQL_PF_COLUMN,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_PF_ARBITRARY,		SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdLocfOperationPersistence[] = {
	InitDftNoSArrayVal30(SQL_LP_HOLDABLE,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_LP_NON_HOLDABLE,		SQL_C_ULONG,	PRM_32BIT),
};

//  SQL_ATTR_USE_LOCATORS
dCSEG(DFTARRAY) lpdUseLocators[] = {
	InitDftNoSArrayVal30(SQL_UL_ON,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_UL_OFF,	SQL_C_ULONG,	PRM_32BIT),
	};

#endif

dCSEG(DFTARRAY) lpdParamArrayRowCounts[] = {
	InitDftNoSArrayVal30(SQL_PARC_BATCH,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_PARC_NO_BATCH,		SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdParamArraySelects[] = {
	InitDftNoSArrayVal30(SQL_PAS_BATCH,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_PAS_NO_BATCH,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_PAS_NO_SELECT,	SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdBatchSupport[] = {
	InitDftNoSArrayVal30(SQL_BS_SELECT_EXPLICIT,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_BS_ROW_COUNT_EXPLICIT,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_BS_SELECT_PROC,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_BS_ROW_COUNT_PROC,		SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdBatchRowCount[] = {
	InitDftNoSArrayVal30(SQL_BRC_PROCEDURES,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_BRC_EXPLICIT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_BRC_ROLLED_UP,	SQL_C_ULONG,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdSQL92DatetimeFunc[] = {
		InitDftNoSArrayVal30(SQL_SDF_CURRENT_DATE,			SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SDF_CURRENT_TIME,			SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SDF_CURRENT_TIMESTAMP,	SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92ForeignKeyDeleteRule[] = {
		InitDftNoSArrayVal30(SQL_SFKD_CASCADE,		SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SFKD_NO_ACTION,	SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SFKD_SET_DEFAULT,	SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SFKD_SET_NULL,	SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92ForeignKeyUpdateRule[] = {
		InitDftNoSArrayVal30(SQL_SFKU_CASCADE,			SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SFKU_NO_ACTION,		SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SFKU_SET_DEFAULT,	SQL_C_ULONG,	PRM_32MSK),
		InitDftNoSArrayVal30(SQL_SFKU_SET_NULL,		SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92Grant[] = {
	InitDftNoSArrayVal30(SQL_SG_USAGE_ON_DOMAIN,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_USAGE_ON_CHARACTER_SET,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_USAGE_ON_COLLATION,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_USAGE_ON_TRANSLATION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_WITH_GRANT_OPTION,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_DELETE_TABLE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_INSERT_TABLE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_INSERT_COLUMN,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_REFERENCES_TABLE,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_REFERENCES_COLUMN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_SELECT_TABLE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_UPDATE_TABLE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SG_UPDATE_COLUMN,				SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92NumericValueFunctions[] = {
	InitDftNoSArrayVal30(SQL_SNVF_BIT_LENGTH,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SNVF_CHAR_LENGTH,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SNVF_CHARACTER_LENGTH,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SNVF_EXTRACT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SNVF_OCTET_LENGTH,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SNVF_POSITION,			SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92Predicates[] = {
	InitDftNoSArrayVal30(SQL_SP_EXISTS,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_ISNOTNULL,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_ISNULL,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_MATCH_FULL,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_MATCH_PARTIAL,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_MATCH_UNIQUE_FULL,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_MATCH_UNIQUE_PARTIAL,SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_OVERLAPS,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_UNIQUE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_LIKE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_IN,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_BETWEEN,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_COMPARISON,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SP_QUANTIFIED_COMPARISON,SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92RelationalJoinOperators[] = {
	InitDftNoSArrayVal30(SQL_SRJO_CORRESPONDING_CLAUSE,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_CROSS_JOIN,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_EXCEPT_JOIN,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_FULL_OUTER_JOIN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_INNER_JOIN,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_INTERSECT_JOIN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_LEFT_OUTER_JOIN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_NATURAL_JOIN,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_RIGHT_OUTER_JOIN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRJO_UNION_JOIN,					SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92Revoke[] = {
	InitDftNoSArrayVal30(SQL_SR_USAGE_ON_DOMAIN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_USAGE_ON_CHARACTER_SET,SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_USAGE_ON_COLLATION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_USAGE_ON_TRANSLATION,	SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_GRANT_OPTION_FOR,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_CASCADE,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_RESTRICT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_DELETE_TABLE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_INSERT_TABLE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_INSERT_COLUMN,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_REFERENCES_TABLE,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_REFERENCES_COLUMN,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_SELECT_TABLE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_UPDATE_TABLE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SR_UPDATE_COLUMN,			SQL_C_ULONG,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdSQL92StringFunctions[] = {
	InitDftNoSArrayVal30(SQL_SSF_CONVERT,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_LOWER,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_UPPER,						SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_SUBSTRING,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRANSLATE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRIM_BOTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRIM_LEADING,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRIM_TRAILING,			SQL_C_ULONG,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdISOStringFunctions[] = {
	InitDftNoSArrayVal30(SQL_SSF_CONVERT,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_LOWER,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_UPPER,					SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_SUBSTRING,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRANSLATE,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRIM_BOTH,				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRIM_LEADING,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SSF_TRIM_TRAILING,			SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SSF_OVERLAY, 				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SSF_LENGTH, 				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SSF_POSITION, 				SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal40(SQL_SSF_CONCAT, 				SQL_C_ULONG,	PRM_32MSK),
};

dCSEG(DFTARRAY) lpdSQL92RowValueConstructor[] = {
	InitDftNoSArrayVal30(SQL_SRVC_VALUE_EXPRESSION,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRVC_NULL,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRVC_DEFAULT,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SRVC_ROW_SUBQUERY,		SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdSQL92ValueExpressions[] = {
	InitDftNoSArrayVal30(SQL_SVE_CASE,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SVE_CAST,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SVE_COALESCE,		SQL_C_ULONG,	PRM_32MSK),
	InitDftNoSArrayVal30(SQL_SVE_NULLIF,		SQL_C_ULONG,	PRM_32MSK),
	};

dCSEG(DFTARRAY) lpdfInstallerErrors[] = {
	InitDftNoSArray30(ODBC_ERROR_GENERAL_ERR,					SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_BUFF_LEN,			SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_HWND,				SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_STR,					SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_REQUEST_TYPE,		SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_COMPONENT_NOT_FOUND,		SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_NAME,				SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_KEYWORD_VALUE,	SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_DSN,					SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_INF,					SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_REQUEST_FAILED,				SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_PATH,				SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_LOAD_LIB_FAILED,			SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_PARAM_SEQUENCE,	SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_INVALID_LOG_FILE,			SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_REMOVE_DSN_FAILED,			SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_USAGE_UPDATE_FAILED,		SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_USER_CANCELED,				SQL_C_SLONG),
	InitDftNoSArray30(ODBC_ERROR_OUT_OF_MEM,					SQL_C_SLONG),
};


dCSEG(DFTARRAY) lpdConfigModes[] = {
	InitDftNoSArray30(ODBC_BOTH_DSN,								SQL_C_SLONG),
	InitDftNoSArray30(ODBC_SYSTEM_DSN,							SQL_C_SLONG),
	InitDftNoSArray30(ODBC_USER_DSN,								SQL_C_SLONG),
};

//
dCSEG(DFTARRAY) lpdfInfoType[] = {
	InitDftNoSArrayVal10(SQL_ACCESSIBLE_PROCEDURES,			SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_ACCESSIBLE_TABLES,				SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_ACTIVE_CONNECTIONS,				SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal30(SQL_ACTIVE_ENVIRONMENTS,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal10(SQL_ACTIVE_STATEMENTS,				SQL_C_USHORT, PRM_16BIT),
	InitDftSubArrayVal40(SQL_AGGREGATE_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK, lpdAggFunct),
	InitDftSubArrayVal30(SQL_ALTER_DOMAIN,						SQL_C_USHORT, PRM_32MSK,	lpdAlterDomain),
	InitDftSubArrayVal20(SQL_ALTER_TABLE,						SQL_C_USHORT, PRM_32MSK,	lpdAlterTable),
	InitDftSubArrayVal30(SQL_SQL_CONFORMANCE,					SQL_C_USHORT, PRM_32BIT,	lpdANSISQLConformance),
	InitDftSubArrayVal30(SQL_DATETIME_LITERALS,				SQL_C_USHORT, PRM_32MSK,	lpdANSIDateTimeLiterals),
	InitDftSubArrayVal30(SQL_ASYNC_MODE,						SQL_C_USHORT, PRM_32BIT,	lpdAsyncMode),
	InitDftSubArrayVal30(SQL_BATCH_ROW_COUNT,					SQL_C_USHORT, PRM_32MSK,	lpdBatchRowCount),
	InitDftSubArrayVal30(SQL_BATCH_SUPPORT,					SQL_C_USHORT, PRM_32MSK,	lpdBatchSupport),
	InitDftSubArrayVal40(SQL_BINARY_FUNCTIONS, 				SQL_C_USHORT, PRM_32MSK,	lpdBinFunct),
	InitDftSubArrayVal20(SQL_BOOKMARK_PERSISTENCE,			SQL_C_USHORT, PRM_32MSK,	lpdBMPersist),
	InitDftSubArrayVal30(SQL_CATALOG_LOCATION,				SQL_C_USHORT, PRM_16BIT,	lpdCLLocation),
	InitDftNoSArrayVal30(SQL_CATALOG_NAME,						SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_CATALOG_NAME_SEPARATOR,		SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_CATALOG_TERM,						SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_CATALOG_USAGE,					SQL_C_USHORT, PRM_32MSK, lpdCatUsage),
	InitDftNoSArrayVal30(SQL_COLLATION_SEQ,					SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_COLUMN_ALIAS,						SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal10(SQL_CONCAT_NULL_BEHAVIOR,			SQL_C_USHORT, PRM_16BIT, lpdConcatBehav),
	InitDftSubArrayVal10(SQL_CONVERT_BIGINT,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
    InitDftSubArrayVal10(SQL_CONVERT_GUID,                    SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_BINARY,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_BIT,						SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_CHAR,						SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_DATE,						SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_DECIMAL,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_DOUBLE,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_FLOAT,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK, lpdConvFunct),
	InitDftSubArrayVal10(SQL_CONVERT_INTEGER,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal30(SQL_CONVERT_INTERVAL_YEAR_MONTH, SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal30(SQL_CONVERT_INTERVAL_DAY_TIME,	SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_LONGVARBINARY,			SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_LONGVARCHAR,			SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_NUMERIC,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_REAL,						SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_SMALLINT,				SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_TIME,						SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_TIMESTAMP,				SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_TINYINT,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_VARBINARY,				SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CONVERT_VARCHAR,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal30(SQL_CONVERT_WCHAR,					SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal30(SQL_CONVERT_WLONGVARCHAR,			SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal30(SQL_CONVERT_WVARCHAR,				SQL_C_USHORT, PRM_32MSK, lpdConvert10),
	InitDftSubArrayVal10(SQL_CORRELATION_NAME,				SQL_C_USHORT, PRM_16BIT, lpdCorrelationName),
	InitDftSubArrayVal30(SQL_CREATE_ASSERTION,				SQL_C_USHORT, PRM_32MSK, lpdCreateAssertion),
	InitDftSubArrayVal30(SQL_CREATE_CHARACTER_SET,			SQL_C_USHORT, PRM_32MSK, lpdCreateCharSet),
	InitDftSubArrayVal30(SQL_CREATE_COLLATION,				SQL_C_USHORT, PRM_32MSK, lpdCreateCollation),
	InitDftSubArrayVal30(SQL_CREATE_DOMAIN,					SQL_C_USHORT, PRM_32MSK, lpdCreateDomain),
	InitDftSubArrayVal30(SQL_CREATE_SCHEMA,					SQL_C_USHORT, PRM_32MSK, lpdCreateSchema),
	InitDftSubArrayVal30(SQL_CREATE_TABLE,						SQL_C_USHORT, PRM_32MSK, lpdCreateTable),
	InitDftSubArrayVal30(SQL_CREATE_TRANSLATION,				SQL_C_USHORT, PRM_32MSK, lpdCreateTranslation),
	InitDftSubArrayVal30(SQL_CREATE_VIEW,						SQL_C_USHORT, PRM_32MSK, lpdCreateView),
	InitDftSubArrayVal10(SQL_CURSOR_COMMIT_BEHAVIOR,		SQL_C_USHORT, PRM_16BIT, lpdCommitBehav),
	InitDftSubArrayVal10(SQL_CURSOR_ROLLBACK_BEHAVIOR,		SQL_C_USHORT, PRM_16BIT, lpdCommitBehav),
	InitDftSubArrayVal30(SQL_CURSOR_SENSITIVITY,				SQL_C_USHORT, PRM_32BIT, lpdCursorSensitivity),
	InitDftNoSArrayVal10(SQL_DATA_SOURCE_NAME,				SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_DATA_SOURCE_READ_ONLY,			SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_DATABASE_NAME,					SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_DBMS_NAME,							SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_DBMS_VER,							SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal10(SQL_DEFAULT_TXN_ISOLATION,			SQL_C_USHORT, PRM_32BIT, lpdTxnIsol),
	InitDftNoSArrayVal30(SQL_DESCRIBE_PARAMETER,				SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_DM_VER,								SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_DRIVER_HDBC,						SQL_C_USHORT, PRM_32HDL),
	InitDftNoSArrayVal30(SQL_DRIVER_HDESC,						SQL_C_USHORT, PRM_32HDL),
	InitDftNoSArrayVal10(SQL_DRIVER_HENV,						SQL_C_USHORT, PRM_32HDL),
	InitDftNoSArrayVal20(SQL_DRIVER_HLIB,						SQL_C_USHORT, PRM_32HDL),
	InitDftNoSArrayVal10(SQL_DRIVER_HSTMT,						SQL_C_USHORT, PRM_32HDL),
	InitDftNoSArrayVal10(SQL_DRIVER_NAME,						SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_DRIVER_ODBC_VER,					SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_DRIVER_VER,						SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DROP_ASSERTION,					SQL_C_USHORT, PRM_32MSK, lpdDropAssertion),
	InitDftSubArrayVal30(SQL_DROP_CHARACTER_SET,				SQL_C_USHORT, PRM_32MSK, lpdDropCharSet),
	InitDftSubArrayVal30(SQL_DROP_COLLATION,					SQL_C_USHORT, PRM_32MSK, lpdDropCollation),
	InitDftSubArrayVal30(SQL_DROP_DOMAIN,						SQL_C_USHORT, PRM_32MSK, lpdDropDomain),
	InitDftSubArrayVal30(SQL_DROP_SCHEMA,						SQL_C_USHORT, PRM_32MSK, lpdDropSchema),
	InitDftSubArrayVal30(SQL_DROP_TABLE,						SQL_C_USHORT, PRM_32MSK, lpdDropTable),
	InitDftSubArrayVal30(SQL_DROP_TRANSLATION,				SQL_C_USHORT, PRM_32MSK, lpdDropTranslation),
	InitDftSubArrayVal30(SQL_DROP_VIEW,							SQL_C_USHORT, PRM_32MSK, lpdDropView),
	InitDftSubArrayVal30(SQL_DYNAMIC_CURSOR_ATTRIBUTES1,	SQL_C_USHORT, PRM_32MSK, lpdCA1),
	InitDftSubArrayVal30(SQL_DYNAMIC_CURSOR_ATTRIBUTES2,	SQL_C_USHORT, PRM_32MSK, lpdCA2),
	InitDftNoSArrayVal10(SQL_EXPRESSIONS_IN_ORDERBY,		SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal10(SQL_FETCH_DIRECTION,					SQL_C_USHORT, PRM_32MSK, lpdFetchDir),
	InitDftSubArrayVal20(SQL_FILE_USAGE,						SQL_C_USHORT, PRM_16BIT, lpdFileUsage30),
	InitDftSubArrayVal30(SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1,SQL_C_USHORT, PRM_32MSK, lpdCA1),
	InitDftSubArrayVal30(SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2,SQL_C_USHORT, PRM_32MSK, lpdCA2),
	InitDftSubArrayVal20(SQL_GETDATA_EXTENSIONS,				SQL_C_USHORT, PRM_32MSK, lpdGetDataExt),
	InitDftSubArrayVal20(SQL_GROUP_BY,							SQL_C_USHORT, PRM_16BIT, lpdGroupBy),
	InitDftSubArrayVal10(SQL_IDENTIFIER_CASE,					SQL_C_USHORT, PRM_16BIT, lpdIdentCase),
	InitDftNoSArrayVal10(SQL_IDENTIFIER_QUOTE_CHAR,			SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_INDEX_KEYWORDS,					SQL_C_USHORT, PRM_32MSK, lpdIndexKeyWords),
	InitDftSubArrayVal30(SQL_INFO_SCHEMA_VIEWS,				SQL_C_USHORT, PRM_32MSK, lpdInfoSchemaViews),
	InitDftNoSArrayVal30(SQL_INTEGRITY,							SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal40(SQL_ISO_BINARY_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK,	lpdISOBinFunct),
	InitDftSubArrayVal40(SQL_ISO_DATETIME_FUNCTIONS,		SQL_C_USHORT, PRM_32MSK, lpdSQL92DatetimeFunc),
	InitDftSubArrayVal40(SQL_ISO_FOREIGN_KEY_DELETE_RULE,	SQL_C_USHORT, PRM_32MSK, lpdSQL92ForeignKeyDeleteRule),
	InitDftSubArrayVal40(SQL_ISO_FOREIGN_KEY_UPDATE_RULE,	SQL_C_USHORT, PRM_32MSK, lpdSQL92ForeignKeyUpdateRule),
	InitDftSubArrayVal40(SQL_ISO_GRANT, 						SQL_C_USHORT, PRM_32MSK, lpdSQL92Grant),
	InitDftSubArrayVal40(SQL_ISO_NUMERIC_VALUE_FUNCTIONS,	SQL_C_USHORT, PRM_32MSK, lpdSQL92NumericValueFunctions),
	InitDftSubArrayVal40(SQL_ISO_PREDICATES,					SQL_C_USHORT, PRM_32MSK, lpdSQL92Predicates),
	InitDftSubArrayVal40(SQL_ISO_RELATIONAL_JOIN_OPERATORS,SQL_C_USHORT, PRM_32MSK, lpdSQL92RelationalJoinOperators),
	InitDftSubArrayVal40(SQL_ISO_REVOKE,						SQL_C_USHORT, PRM_32MSK, lpdSQL92Revoke),
	InitDftSubArrayVal40(SQL_ISO_ROW_VALUE_CONSTRUCTOR, SQL_C_USHORT, PRM_32MSK, lpdSQL92RowValueConstructor),
	InitDftSubArrayVal40(SQL_ISO_STRING_FUNCTIONS,		SQL_C_USHORT, PRM_32MSK, lpdISOStringFunctions),
	InitDftSubArrayVal40(SQL_ISO_VALUE_EXPRESSIONS, 	SQL_C_USHORT, PRM_32MSK, lpdSQL92ValueExpressions),
	InitDftSubArrayVal30(SQL_KEYSET_CURSOR_ATTRIBUTES1,	SQL_C_USHORT, PRM_32MSK, lpdCA1),
	InitDftSubArrayVal30(SQL_KEYSET_CURSOR_ATTRIBUTES2,	SQL_C_USHORT, PRM_32MSK, lpdCA2),
	InitDftNoSArrayVal20(SQL_KEYWORDS,							SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_LIKE_ESCAPE_CLAUSE,				SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal40(SQL_LIMIT_ESCAPE_CLAUSE,		SQL_C_USHORT, PRM_32MSK, lpdLimitEscapeClause),
#ifdef SQL_LOB_LOCATOR
	InitDftSubArrayVal30(SQL_LOB_LOCATOR,						SQL_C_USHORT, PRM_32MSK, lpdLobLocator),
	InitDftSubArrayVal30(SQL_LOCATOR_PERSISTENCE,			SQL_C_USHORT, PRM_32BIT, lpdLocfOperationPersistence),
	InitDftSubArrayVal30(SQL_LOCATOR_SENSITIVITY,			SQL_C_USHORT, PRM_32BIT, lpdLocatorSensitivity),
	InitDftSubArrayVal30(SQL_LOCATOR_UPDATABILITY,			SQL_C_USHORT, PRM_32BIT, lpdLocatorUpdatability),
#endif
	InitDftSubArrayVal20(SQL_LOCK_TYPES,						SQL_C_USHORT, PRM_32MSK, lpdLockTypes),
	InitDftNoSArrayVal30(SQL_MAX_ASYNC_CONCURRENT_STATEMENTS,SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal20(SQL_MAX_BINARY_LITERAL_LEN,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal20(SQL_MAX_CATALOG_NAME_LEN,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_CHAR_LITERAL_LEN,			SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MAX_COLUMN_NAME_LEN,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_COLUMNS_IN_GROUP_BY,		SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_COLUMNS_IN_INDEX,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_COLUMNS_IN_ORDER_BY,		SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_COLUMNS_IN_SELECT,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_COLUMNS_IN_TABLE,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal30(SQL_MAX_CONCURRENT_ACTIVITIES,	SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal10(SQL_MAX_CURSOR_NAME_LEN,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal30(SQL_MAX_DRIVER_CONNECTIONS,		SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal30(SQL_MAX_IDENTIFIER_LEN,				SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_INDEX_SIZE,					SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MAX_PROCEDURE_NAME_LEN,		SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_ROW_SIZE,						SQL_C_USHORT, PRM_32BIT),
#ifdef SQL_MAX_ROW_SIZE_INCLUDES_BLOB
	InitDftNoSArrayVal30(SQL_MAX_ROW_SIZE_INCLUDES_BLOB,	SQL_C_USHORT, PRM_STR),
#endif
	InitDftNoSArrayVal20(SQL_MAX_ROW_SIZE_INCLUDES_LONG,	SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_MAX_SCHEMA_NAME_LEN,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_STATEMENT_LEN,				SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MAX_TABLE_NAME_LEN,				SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_TABLES_IN_SELECT,			SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal20(SQL_MAX_USER_NAME_LEN,				SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal10(SQL_MAX_OWNER_NAME_LEN,				SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal10(SQL_MAX_QUALIFIER_NAME_LEN,		SQL_C_USHORT, PRM_16BIT),
	InitDftNoSArrayVal10(SQL_MULT_RESULT_SETS,				SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_MULTIPLE_ACTIVE_TXN,			SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal40(SQL_NATIVE_ESCAPE_CLAUSE,				SQL_C_USHORT, PRM_STR, lpdReturnEscapeClause),
	InitDftNoSArrayVal20(SQL_NEED_LONG_DATA_LEN,				SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal10(SQL_NON_NULLABLE_COLUMNS,			SQL_C_USHORT, PRM_16BIT, lpdNonNullCols),
	InitDftSubArrayVal20(SQL_NULL_COLLATION,					SQL_C_USHORT, PRM_16BIT, lpdNullColl),
	InitDftSubArrayVal10(SQL_NUMERIC_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK, lpdNumFunct),
	InitDftSubArrayVal10(SQL_ODBC_API_CONFORMANCE,			SQL_C_USHORT, PRM_16BIT, lpdAPIConf),
	InitDftSubArrayVal30(SQL_ODBC_INTERFACE_CONFORMANCE,	SQL_C_USHORT, PRM_32BIT, lpdInterfaceConformance),
	InitDftSubArrayVal10(SQL_ODBC_SAG_CLI_CONFORMANCE,		SQL_C_USHORT, PRM_16BIT, lpdSAGConf),
	InitDftSubArrayVal10(SQL_ODBC_SQL_CONFORMANCE,			SQL_C_USHORT, PRM_16BIT, lpdSQLConf),
	InitDftNoSArrayVal10(SQL_ODBC_SQL_OPT_IEF,				SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_ODBC_VER,							SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal20(SQL_OJ_CAPABILITIES,					SQL_C_USHORT, PRM_32MSK, lpdOJCap), // Bug #345
	InitDftNoSArrayVal10(SQL_OUTER_JOINS,						SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_OWNER_TERM,						SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_ORDER_BY_COLUMNS_IN_SELECT,	SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal20(SQL_OWNER_USAGE,						SQL_C_USHORT, PRM_32MSK, lpdOwnerUsage),
	InitDftSubArrayVal30(SQL_PARAM_ARRAY_ROW_COUNTS,		SQL_C_USHORT, PRM_32BIT, lpdParamArrayRowCounts),
	InitDftSubArrayVal30(SQL_PARAM_ARRAY_SELECTS,			SQL_C_USHORT, PRM_32BIT, lpdParamArraySelects),
	InitDftSubArrayVal20(SQL_POS_OPERATIONS,					SQL_C_USHORT, PRM_32MSK, lpdPosOps),
	InitDftSubArrayVal20(SQL_POSITIONED_STATEMENTS,			SQL_C_USHORT, PRM_32MSK, lpdPosStmts),
#ifdef SQL_PARAM_ARRAY_SELECT_BATCH
	InitDftSubArrayVal30(SQL_PARAM_ARRAY_SELECT_BATCH,		SQL_C_USHORT, PRM_32BIT, lpdParamArraySelects),
	InitDftSubArrayVal30(SQL_PREDICATE_FIND,					SQL_C_USHORT, PRM_32MSK, lpdPredicateFind),
#endif
	InitDftNoSArrayVal10(SQL_PROCEDURE_TERM,					SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_PROCEDURES,						SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal20(SQL_QUALIFIER_USAGE,					SQL_C_USHORT, PRM_32MSK, lpdQualUsage),
	InitDftSubArrayVal20(SQL_QUOTED_IDENTIFIER_CASE,		SQL_C_USHORT, PRM_16BIT, lpdQuoteIdentCase),
	InitDftNoSArrayVal10(SQL_QUALIFIER_NAME_SEPARATOR,		SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_QUALIFIER_TERM,					SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal20(SQL_QUALIFIER_LOCATION,				SQL_C_USHORT, PRM_16BIT, lpdQualLocate), // Bug #356
	InitDftSubArrayVal40(SQL_RETURN_ESCAPE_CLAUSE,		SQL_C_USHORT, PRM_32MSK, lpdReturnEscapeClause),
	InitDftNoSArrayVal10(SQL_ROW_UPDATES,						SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_SCHEMA_TERM,						SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_SCHEMA_USAGE,						SQL_C_USHORT, PRM_32MSK, lpdSchemaUsage),
	InitDftSubArrayVal10(SQL_SCROLL_OPTIONS,					SQL_C_USHORT, PRM_32MSK, lpdScrollOpts),
	InitDftSubArrayVal10(SQL_SCROLL_CONCURRENCY,				SQL_C_USHORT, PRM_32MSK, lpdScrollConcur),
	InitDftNoSArrayVal10(SQL_SEARCH_PATTERN_ESCAPE,			SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal10(SQL_SERVER_NAME,						SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_SPECIAL_CHARACTERS,				SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_SQL92_DATETIME_FUNCTIONS,		SQL_C_USHORT, PRM_32MSK, lpdSQL92DatetimeFunc),
	InitDftSubArrayVal30(SQL_SQL92_FOREIGN_KEY_DELETE_RULE,	SQL_C_USHORT, PRM_32MSK, lpdSQL92ForeignKeyDeleteRule),
	InitDftSubArrayVal30(SQL_SQL92_FOREIGN_KEY_UPDATE_RULE,	SQL_C_USHORT, PRM_32MSK, lpdSQL92ForeignKeyUpdateRule),
	InitDftSubArrayVal30(SQL_SQL92_GRANT,							SQL_C_USHORT, PRM_32MSK, lpdSQL92Grant),
	InitDftSubArrayVal30(SQL_SQL92_NUMERIC_VALUE_FUNCTIONS,	SQL_C_USHORT, PRM_32MSK, lpdSQL92NumericValueFunctions),
	InitDftSubArrayVal30(SQL_SQL92_PREDICATES,					SQL_C_USHORT, PRM_32MSK, lpdSQL92Predicates),
	InitDftSubArrayVal30(SQL_SQL92_RELATIONAL_JOIN_OPERATORS,SQL_C_USHORT, PRM_32MSK, lpdSQL92RelationalJoinOperators),
	InitDftSubArrayVal30(SQL_SQL92_REVOKE,						SQL_C_USHORT, PRM_32MSK, lpdSQL92Revoke),
	InitDftSubArrayVal30(SQL_SQL92_ROW_VALUE_CONSTRUCTOR,	SQL_C_USHORT, PRM_32MSK, lpdSQL92RowValueConstructor),
	InitDftSubArrayVal30(SQL_SQL92_STRING_FUNCTIONS,		SQL_C_USHORT, PRM_32MSK, lpdSQL92StringFunctions),
	InitDftSubArrayVal30(SQL_SQL92_VALUE_EXPRESSIONS,		SQL_C_USHORT, PRM_32MSK, lpdSQL92ValueExpressions),
	InitDftSubArrayVal30(SQL_STANDARD_CLI_CONFORMANCE,		SQL_C_USHORT, PRM_32MSK, lpdStandardCLIConformance),
	InitDftSubArrayVal30(SQL_STATIC_CURSOR_ATTRIBUTES1,	SQL_C_USHORT, PRM_32MSK, lpdCA1),
	InitDftSubArrayVal30(SQL_STATIC_CURSOR_ATTRIBUTES2,	SQL_C_USHORT, PRM_32MSK, lpdCA2),
	InitDftSubArrayVal20(SQL_STATIC_SENSITIVITY,				SQL_C_USHORT, PRM_32MSK, lpdStaticSensitivity),
	InitDftSubArrayVal10(SQL_STRING_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK, lpdStrFunct),
	InitDftSubArrayVal20(SQL_SUBQUERIES,						SQL_C_USHORT, PRM_32MSK, lpdSubqueries),
	InitDftSubArrayVal10(SQL_SYSTEM_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK, lpdSysFunct),
	InitDftNoSArrayVal10(SQL_TABLE_TERM,						SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal20(SQL_TIMEDATE_ADD_INTERVALS,		SQL_C_USHORT, PRM_32MSK, lpdTimeDateInts),
	InitDftSubArrayVal20(SQL_TIMEDATE_DIFF_INTERVALS,		SQL_C_USHORT, PRM_32MSK, lpdTimeDateInts),
	InitDftSubArrayVal10(SQL_TIMEDATE_FUNCTIONS,				SQL_C_USHORT, PRM_32MSK, lpdTimeDateFunct),
	InitDftSubArrayVal10(SQL_TXN_CAPABLE,						SQL_C_USHORT, PRM_16BIT, lpdTxnCapable),
	InitDftSubArrayVal10(SQL_TXN_ISOLATION_OPTION,			SQL_C_USHORT, PRM_32MSK, lpdTxnIsol),
	InitDftSubArrayVal20(SQL_UNION,								SQL_C_USHORT, PRM_32MSK, lpdUnion),
	InitDftNoSArrayVal10(SQL_USER_NAME,							SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_XOPEN_CLI_YEAR,					SQL_C_USHORT, PRM_STR),

#ifdef _INTERNAL
	InitDftNoSArrayVal10(SQL_INFO_SS_NETLIB_NAME,							SQL_C_USHORT, PRM_STR),
#endif
};


//------------------------------------------------------------------------------------------------
// SQLBindParameter, fParamType
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfParamType[] = {
	InitDftNoSArray20(SQL_PARAM_INPUT,				SQL_C_USHORT),
	InitDftNoSArray20(SQL_PARAM_INPUT_OUTPUT,		SQL_C_USHORT),
	InitDftNoSArray20(SQL_PARAM_OUTPUT,				SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLFreeStmt, fOption
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdFreeStmtOpt[] = {
	InitDftNoSArray10(SQL_CLOSE,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_DROP,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_UNBIND,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_RESET_PARAMS,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdNullPointer[] = {
	NULLPTR,	SQL_C_ULONG,	szNULLPTR,	MAKEVERSION(2,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	};

dCSEG(DFTARRAY) lpdNullValidPointer[] = {
	NULLPTR,		SQL_C_ULONG,	szNULLPTR,	MAKEVERSION(2,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	VALIDPTR,	SQL_C_ULONG,	szVALIDPTR,	MAKEVERSION(2,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	};

dCSEG(DFTARRAY) lpdNullEmptyPointer[] = {
	InitDftNoSArrayVal30(SQL_NULL_HDESC,	SQL_C_ULONG,	PRM_32BIT),
};

dCSEG(DFTARRAY) lpdNullHandle[] = {
	InitDftNoSArray30(SQL_NULL_HANDLE,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdDateTimeSubType[] = {
	InitDftNoSArray30(SQL_CODE_DATE,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_TIME,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_TIMESTAMP,					SQL_C_SSHORT),
	};

dCSEG(DFTARRAY) lpdfStringValues[] = {
	NULLPTR,	SQL_C_ULONG,	szNULLPTR,		MAKEVERSION(2,0),	PRM_STR,	0, NULL,
	EMPTYSTR,SQL_C_ULONG,	szEMPTYSTRING,	MAKEVERSION(2,0),	PRM_STR,	0, NULL,
	};

// SQL_COLUMN_NULLABLE
dCSEG(DFTARRAY) lpdNullable[] = {
	InitDftNoSArray10(SQL_NO_NULLS,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_NULLABLE,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_NULLABLE_UNKNOWN,	SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdDateTimeIntervalCode[] = {
	//InitDftNoSArray30(0,											SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_DATE,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_TIME,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_TIMESTAMP,					SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_DAY,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_DAY_TO_HOUR,				SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_DAY_TO_MINUTE,				SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_DAY_TO_SECOND,				SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_HOUR,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_HOUR_TO_MINUTE,			SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_HOUR_TO_SECOND,			SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_MINUTE,						SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_MINUTE_TO_SECOND,			SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_MONTH,						SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_SECOND,						SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_YEAR,							SQL_C_SSHORT),
	InitDftNoSArray30(SQL_CODE_YEAR_TO_MONTH,				SQL_C_SSHORT),
	};

dCSEG(DFTARRAY) lpdConciseType[] = {
	CTYPES
	SqlTypesInfo
	};

dCSEG(DFTARRAY) lpdIntervalTypeSubCodes[] = {
	InitDftNoSArray30(SQL_INTERVAL_DAY,						SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_DAY_TO_HOUR,			SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_DAY_TO_MINUTE,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_DAY_TO_SECOND,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_HOUR,					SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_HOUR_TO_MINUTE,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_HOUR_TO_SECOND,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_MINUTE,					SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_MINUTE_TO_SECOND,	SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_MONTH,					SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_SECOND,					SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_YEAR,					SQL_C_SSHORT),
	InitDftNoSArray30(SQL_INTERVAL_YEAR_TO_MONTH,		SQL_C_SSHORT),
	};

// SQL_DESC_SEARCHABLE
dCSEG(DFTARRAY) lpdSearchable30[] = {
	InitDftNoSArray30(SQL_PRED_NONE,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_PRED_CHAR,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_PRED_BASIC,			SQL_C_USHORT),
	InitDftNoSArray30(SQL_PRED_SEARCHABLE,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdSQLTrueFalse30[] = {
	InitDftNoSArray30(SQL_FALSE,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_TRUE,		SQL_C_USHORT),
	};

// SQL_COLUMN_UPDATABLE
dCSEG(DFTARRAY) lpdUpdatable[] = {
	InitDftNoSArray10(SQL_ATTR_READONLY,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_ATTR_WRITE,					SQL_C_USHORT),
	InitDftNoSArray10(SQL_ATTR_READWRITE_UNKNOWN,	SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdfDescTypes[] = {
	InitDftNoSArray10(SQL_CHAR,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_VARCHAR,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_LONGVARCHAR,	SQL_C_SSHORT),
	InitDftNoSArray10(SQL_DECIMAL,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_NUMERIC,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_SMALLINT,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_INTEGER,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_REAL,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_FLOAT,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_GUID,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_DOUBLE,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_BIT,				SQL_C_SSHORT),
	InitDftNoSArray10(SQL_TINYINT,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_BIGINT,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_BINARY,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_VARBINARY,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_LONGVARBINARY,SQL_C_SSHORT),
	InitDftNoSArray30(SQL_WCHAR,			SQL_C_SSHORT),
	InitDftNoSArray30(SQL_WVARCHAR,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_WLONGVARCHAR,	SQL_C_SSHORT),
	InitDftSubArrayVal30(SQL_DATETIME,	SQL_C_SSHORT, PRM_16BIT,lpdDateTimeSubType),
	InitDftSubArrayVal30(SQL_INTERVAL,	SQL_C_SSHORT, PRM_16BIT,lpdDateTimeIntervalCode),
	InitDftNoSArray10(SQL_C_CHAR,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_BINARY,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_BIT,			SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_UBIGINT,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_SBIGINT,		SQL_C_SSHORT),
#ifdef SQL_C_BLOB_LOCATOR
	InitDftNoSArray30(SQL_C_BLOB_LOCATOR,SQL_C_SSHORT),
 	InitDftNoSArray30(SQL_C_CLOB_LOCATOR,SQL_C_SSHORT),
#endif
	InitDftNoSArray20(SQL_C_BOOKMARK,	SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_DATE,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_DEFAULT,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_DOUBLE,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_FLOAT,		SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_GUID,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_YEAR,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_MONTH,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_DAY,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_HOUR,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_MINUTE,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_SECOND,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_YEAR_TO_MONTH,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_DAY_TO_HOUR,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_DAY_TO_MINUTE,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_DAY_TO_SECOND,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_HOUR_TO_MINUTE,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_HOUR_TO_SECOND,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_INTERVAL_MINUTE_TO_SECOND,		SQL_C_SSHORT),
	InitDftNoSArray20(SQL_C_SLONG,		SQL_C_SSHORT),
	InitDftNoSArray20(SQL_C_SSHORT,		SQL_C_SSHORT),
	InitDftNoSArray20(SQL_C_STINYINT,	SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_TIME,			SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_TIMESTAMP,	SQL_C_SSHORT),
	InitDftNoSArray10(SQL_C_TINYINT,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_TYPE_DATE,	SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_TYPE_TIME,	SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_TYPE_TIMESTAMP,	SQL_C_SSHORT),
	InitDftNoSArray20(SQL_C_ULONG,		SQL_C_SSHORT),
	InitDftNoSArray20(SQL_C_USHORT,		SQL_C_SSHORT),
	InitDftNoSArray20(SQL_C_UTINYINT,	SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_VARBOOKMARK,SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_WCHAR,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_C_NUMERIC,		SQL_C_SSHORT),
	InitDftNoSArray30(SQL_ARD_TYPE,	SQL_C_SSHORT),
};

//  SQL_ATTR_PARAM_BIND_TYPE
dCSEG(DFTARRAY) lpdParamBindType[] = {
	InitDftNoSArrayVal10(SQL_PARAM_BIND_BY_COLUMN,	SQL_C_ULONG,	PRM_32BIT),
	};

//  SQL_BIND_TYPE
dCSEG(DFTARRAY) lpdBindType[] = {
	InitDftNoSArrayVal30(SQL_BIND_BY_COLUMN,	SQL_C_ULONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdAllocTypes[] = {
	InitDftNoSArrayVal30(SQL_DESC_ALLOC_USER,	SQL_C_ULONG,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_DESC_ALLOC_AUTO,	SQL_C_ULONG,	PRM_16BIT),
	};

//Header Fields
#define GetDescHdrFields \
	InitDftSubArrayVal30(SQL_DESC_ALLOC_TYPE,								SQL_C_USHORT, PRM_16BIT,lpdAllocTypes), \
	InitDftNoSArrayVal30(SQL_DESC_ARRAY_SIZE,								SQL_C_USHORT, PRM_32BIT), \
	InitDftSubArrayVal30(SQL_DESC_ARRAY_STATUS_PTR,						SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer), \
	InitDftSubArrayVal30(SQL_DESC_BIND_OFFSET_PTR,						SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer), \
	InitDftSubArrayVal30(SQL_DESC_BIND_TYPE,								SQL_C_USHORT, PRM_32BIT,lpdBindType), \
	InitDftNoSArrayVal30(SQL_DESC_COUNT,									SQL_C_USHORT, PRM_16BIT), \
	InitDftSubArrayVal30(SQL_DESC_ROWS_PROCESSED_PTR,					SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),


//Record fields
#define GetDescRecFields \
	InitDftNoSArrayVal30(SQL_DESC_AUTO_UNIQUE_VALUE,					SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_BASE_COLUMN_NAME,						SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_BASE_TABLE_NAME,						SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftNoSArrayVal30(SQL_DESC_CASE_SENSITIVE,						SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_CATALOG_NAME,							SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_CONCISE_TYPE,							SQL_C_USHORT, PRM_16BIT,lpdConciseType),    \
	InitDftSubArrayVal30(SQL_DESC_DATA_PTR,								SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),    \
	InitDftSubArrayVal30(SQL_DESC_DATETIME_INTERVAL_CODE,				SQL_C_USHORT, PRM_16BIT,lpdDateTimeIntervalCode),    \
	InitDftNoSArrayVal30(SQL_DESC_DATETIME_INTERVAL_PRECISION,		SQL_C_USHORT, PRM_32BIT),    \
	InitDftNoSArrayVal30(SQL_DESC_DISPLAY_SIZE,							SQL_C_USHORT, PRM_32BIT),    \
	InitDftNoSArrayVal30(SQL_DESC_FIXED_PREC_SCALE,						SQL_C_USHORT, PRM_16BIT),    \
	InitDftSubArrayVal30(SQL_DESC_INDICATOR_PTR,							SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),    \
	InitDftSubArrayVal30(SQL_DESC_LABEL,									SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftNoSArrayVal30(SQL_DESC_LENGTH,									SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_LITERAL_PREFIX,						SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_LITERAL_SUFFIX,						SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_LOCAL_TYPE_NAME,						SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_NAME,										SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_NULLABLE,								SQL_C_USHORT, PRM_16BIT,lpdNullable),    \
	InitDftNoSArrayVal30(SQL_DESC_OCTET_LENGTH,							SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_OCTET_LENGTH_PTR,						SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),    \
	InitDftSubArrayVal30(SQL_DESC_PARAMETER_TYPE,						SQL_C_USHORT, PRM_16BIT,lpdfParamType),    \
	InitDftNoSArrayVal30(SQL_DESC_PRECISION,								SQL_C_USHORT, PRM_16BIT),    \
	InitDftNoSArrayVal30(SQL_DESC_SCALE,									SQL_C_USHORT, PRM_16BIT),    \
	InitDftSubArrayVal30(SQL_DESC_SCHEMA_NAME,							SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_SEARCHABLE,								SQL_C_USHORT, PRM_16BIT,lpdSearchable30),    \
	InitDftSubArrayVal30(SQL_DESC_TABLE_NAME,								SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_TYPE,										SQL_C_USHORT, PRM_16BIT,lpdfDescTypes),    \
	InitDftSubArrayVal30(SQL_DESC_TYPE_NAME,								SQL_C_USHORT, PRM_STR,lpdNullPointer),    \
	InitDftSubArrayVal30(SQL_DESC_UNNAMED,									SQL_C_USHORT, PRM_16BIT,lpdUnnamed),    \
	InitDftSubArrayVal30(SQL_DESC_UNSIGNED,								SQL_C_USHORT, PRM_16BIT,lpdSQLTrueFalse30),    \
	InitDftSubArrayVal30(SQL_DESC_UPDATABLE,								SQL_C_USHORT, PRM_16BIT,lpdUpdatable),


//Header Fields
#define SetDescHdrFields \
	InitDftSubArrayVal30(SQL_DESC_ALLOC_TYPE,								SQL_C_USHORT, PRM_32BIT,lpdAllocTypes), \
	InitDftNoSArrayVal30(SQL_DESC_ARRAY_SIZE,								SQL_C_USHORT, PRM_32BIT), \
	InitDftSubArrayVal30(SQL_DESC_ARRAY_STATUS_PTR,						SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer), \
	InitDftSubArrayVal30(SQL_DESC_BIND_OFFSET_PTR,						SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer), \
	InitDftSubArrayVal30(SQL_DESC_BIND_TYPE,								SQL_C_USHORT, PRM_32BIT,lpdBindType), \
	InitDftNoSArrayVal30(SQL_DESC_COUNT,									SQL_C_USHORT, PRM_32BIT), \
	InitDftSubArrayVal30(SQL_DESC_ROWS_PROCESSED_PTR,					SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),


//Record fields
#define SetDescRecFields \
	InitDftNoSArrayVal30(SQL_DESC_AUTO_UNIQUE_VALUE,					SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_BASE_COLUMN_NAME,						SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_BASE_TABLE_NAME,						SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftNoSArrayVal30(SQL_DESC_CASE_SENSITIVE,						SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_CATALOG_NAME,							SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_CONCISE_TYPE,							SQL_C_USHORT, PRM_32BIT,lpdConciseType),    \
	InitDftSubArrayVal30(SQL_DESC_DATA_PTR,								SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),    \
	InitDftSubArrayVal30(SQL_DESC_DATETIME_INTERVAL_CODE,				SQL_C_USHORT, PRM_32BIT,lpdDateTimeIntervalCode),    \
	InitDftNoSArrayVal30(SQL_DESC_DATETIME_INTERVAL_PRECISION,		SQL_C_USHORT, PRM_32BIT),    \
	InitDftNoSArrayVal30(SQL_DESC_DISPLAY_SIZE,							SQL_C_USHORT, PRM_32BIT),    \
	InitDftNoSArrayVal30(SQL_DESC_FIXED_PREC_SCALE,						SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_INDICATOR_PTR,							SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),    \
	InitDftSubArrayVal30(SQL_DESC_LABEL,									SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftNoSArrayVal30(SQL_DESC_LENGTH,									SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_LITERAL_PREFIX,						SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_LITERAL_SUFFIX,						SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_LOCAL_TYPE_NAME,						SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_NAME,										SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_NULLABLE,								SQL_C_USHORT, PRM_32BIT,lpdNullable),    \
	InitDftNoSArrayVal30(SQL_DESC_OCTET_LENGTH,							SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_OCTET_LENGTH_PTR,						SQL_C_USHORT, PRM_PTR_32BIT,lpdNullValidPointer),    \
	InitDftSubArrayVal30(SQL_DESC_PARAMETER_TYPE,						SQL_C_USHORT, PRM_32BIT,lpdfParamType),    \
	InitDftNoSArrayVal30(SQL_DESC_PRECISION,								SQL_C_USHORT, PRM_32BIT),    \
	InitDftNoSArrayVal30(SQL_DESC_SCALE,									SQL_C_USHORT, PRM_32BIT),    \
	InitDftSubArrayVal30(SQL_DESC_SCHEMA_NAME,							SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_SEARCHABLE,								SQL_C_USHORT, PRM_32BIT,lpdSearchable30),    \
	InitDftSubArrayVal30(SQL_DESC_TABLE_NAME,								SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_TYPE,										SQL_C_USHORT, PRM_32BIT,lpdfDescTypes),    \
	InitDftSubArrayVal30(SQL_DESC_TYPE_NAME,								SQL_C_USHORT, PRM_STR,lpdfStringValues),    \
	InitDftSubArrayVal30(SQL_DESC_UNNAMED,									SQL_C_USHORT, PRM_32BIT,lpdUnnamed),    \
	InitDftSubArrayVal30(SQL_DESC_UNSIGNED,								SQL_C_USHORT, PRM_32BIT,lpdSQLTrueFalse30),    \
	InitDftSubArrayVal30(SQL_DESC_UPDATABLE,								SQL_C_USHORT, PRM_32BIT,lpdUpdatable),

//------------------------------------------------------------------------------------------------
// Options used by Descriptor API's
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdSetDescHdrFields[] = {
	SetDescHdrFields
	};

dCSEG(DFTARRAY) lpdGetDescHdrFields[] = {
	GetDescHdrFields
	};

//------------------------------------------------------------------------------------------------
// Options used by Descriptor API's
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdSetDescFields[] = {
	SetDescHdrFields

	SetDescRecFields
	};


//------------------------------------------------------------------------------------------------
// Options used by Descriptor API's
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdSetDescRecFields[] = {
	SetDescRecFields
	};

//------------------------------------------------------------------------------------------------
// Options used by Descriptor API's
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdGetDescRecFields[] = {
	GetDescRecFields
	};


//------------------------------------------------------------------------------------------------
 // Options used by Descriptor API's
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdGetDescFields[] = {
	GetDescHdrFields

	GetDescRecFields
	};

#ifdef _INTERNAL

// This one contains Sqlsrvrtypes
dCSEG(DFTARRAY) lpdfSqlSvrTypes[] = {
// Extension SQL datatypes:
	SqlSvrTypes
	};

dCSEG(DFTARRAY) lpdfSqlSvrUsrTypes[] = {
// Extension SQL datatypes:
	SQLSvrUsrTypes
	};

dCSEG(DFTARRAY) lpdfColumnOp[] = {
	InitDftNoSArrayVal10(SQLAOPCNT,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQLAOPSUM,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQLAOPAVG,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQLAOPMIN,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQLAOPMAX,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQLAOPANY,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQLAOPNOOP,		SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdTptrLog[] = {
	InitDftNoSArrayVal10(SQL_TL_OFF,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_TL_ON,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_TL_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdBCP[] = {
	InitDftNoSArrayVal10(SQL_BCP_OFF,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_BCP_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_BCP_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),

	};


dCSEG(DFTARRAY) lpdHiddenCols[] = {
	InitDftNoSArrayVal10(SQL_HC_OFF,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_HC_ON,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_HC_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdBrowseTable[] = {
	InitDftNoSArrayVal10(SQL_NB_OFF,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_NB_ON,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_NB_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),

	};






#endif
//------------------------------------------------------------------------------------------------
// SQLColAttributes/SQLColAttribute values:
//			if the array name ends in 30 then it is used by SQLColAttribute
//------------------------------------------------------------------------------------------------


// SQL_COLUMN_SEARCHABLE
dCSEG(DFTARRAY) lpdSearchable[] = {
	InitDftNoSArray10(SQL_UNSEARCHABLE,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_LIKE_ONLY,					SQL_C_USHORT),
	InitDftNoSArray10(SQL_ALL_EXCEPT_LIKE,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_SEARCHABLE,				SQL_C_USHORT),
	};

//
dCSEG(DFTARRAY) lpdfDescType[] = {
	InitDftNoSArrayVal10(SQL_COLUMN_COUNT,				SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_COLUMN_NAME,				SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal10(SQL_COLUMN_TYPE,				SQL_C_USHORT, PRM_32BIT,	lpdfSqlTypes),
	InitDftNoSArrayVal10(SQL_COLUMN_LENGTH,			SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_COLUMN_PRECISION,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_COLUMN_SCALE,				SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_COLUMN_DISPLAY_SIZE,	SQL_C_USHORT, PRM_32BIT),
	InitDftSubArrayVal10(SQL_COLUMN_NULLABLE,			SQL_C_USHORT, PRM_32BIT,	lpdNullable),
	InitDftSubArrayVal10(SQL_COLUMN_UNSIGNED,			SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftSubArrayVal10(SQL_COLUMN_MONEY,				SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftSubArrayVal10(SQL_COLUMN_UPDATABLE,		SQL_C_USHORT, PRM_32BIT,	lpdUpdatable),
	InitDftSubArrayVal10(SQL_COLUMN_AUTO_INCREMENT,	SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftSubArrayVal10(SQL_COLUMN_CASE_SENSITIVE,	SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftSubArrayVal10(SQL_COLUMN_SEARCHABLE,		SQL_C_USHORT, PRM_32BIT,	lpdSearchable),
	InitDftNoSArrayVal10(SQL_COLUMN_TYPE_NAME,		SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_COLUMN_TABLE_NAME,		SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_COLUMN_OWNER_NAME,		SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_COLUMN_QUALIFIER_NAME,	SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal20(SQL_COLUMN_LABEL,				SQL_C_USHORT, PRM_STR),
#ifdef _INTERNAL
	InitDftSubArrayVal10(SQL_CA_SS_COLUMN_SSTYPE,		SQL_C_USHORT, PRM_32BIT,	lpdfSqlSvrTypes),
	InitDftSubArrayVal10(SQL_CA_SS_COLUMN_UTYPE,		SQL_C_USHORT, PRM_32BIT,	lpdfSqlSvrUsrTypes),
	InitDftNoSArrayVal10(SQL_CA_SS_NUM_ORDERS,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COLUMN_ORDER,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COLUMN_VARYLEN,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_NUM_COMPUTES,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COMPUTE_ID,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COMPUTE_BYLIST,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COLUMN_ID,		SQL_C_USHORT, PRM_32BIT),
	InitDftSubArrayVal10(SQL_CA_SS_COLUMN_OP,		SQL_C_USHORT, PRM_32BIT,	lpdfColumnOp),
	InitDftNoSArrayVal10(SQL_CA_SS_COLUMN_SIZE,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COLUMN_HIDDEN,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CA_SS_COLUMN_KEY,		SQL_C_USHORT, PRM_32BIT),
#endif
	};

//
dCSEG(DFTARRAY) lpdfDescType30[] = {
	InitDftSubArrayVal30(SQL_DESC_AUTO_UNIQUE_VALUE,SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftNoSArrayVal30(SQL_DESC_BASE_COLUMN_NAME,	SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_DESC_BASE_TABLE_NAME,	SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DESC_CASE_SENSITIVE,	SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftNoSArrayVal30(SQL_DESC_CATALOG_NAME,		SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DESC_CONCISE_TYPE,		SQL_C_USHORT, PRM_32BIT,	lpdfSqlTypes),
	InitDftNoSArrayVal30(SQL_DESC_COUNT,				SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DESC_DISPLAY_SIZE,		SQL_C_USHORT, PRM_32BIT),
	InitDftSubArrayVal30(SQL_DESC_FIXED_PREC_SCALE,	SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftNoSArrayVal30(SQL_DESC_LABEL,				SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_DESC_LENGTH,				SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DESC_LITERAL_PREFIX,	SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_DESC_LITERAL_SUFFIX,	SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_DESC_LOCAL_TYPE_NAME,	SQL_C_USHORT, PRM_STR),
	InitDftNoSArrayVal30(SQL_DESC_NAME,					SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DESC_NULLABLE,			SQL_C_USHORT, PRM_32BIT,	lpdNullable),
	InitDftNoSArrayVal30(SQL_DESC_OCTET_LENGTH,		SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DESC_PRECISION,			SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DESC_SCALE,				SQL_C_USHORT, PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DESC_SCHEMA_NAME,		SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DESC_SEARCHABLE,			SQL_C_USHORT, PRM_32BIT,	lpdSearchable30),
	InitDftNoSArrayVal30(SQL_DESC_TABLE_NAME,			SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DESC_TYPE,					SQL_C_USHORT, PRM_32BIT,	lpdfSqlTypes),
	InitDftNoSArrayVal30(SQL_DESC_TYPE_NAME,			SQL_C_USHORT, PRM_STR),
	InitDftSubArrayVal30(SQL_DESC_UNNAMED,				SQL_C_USHORT, PRM_32BIT,	lpdUnnamed),
	InitDftSubArrayVal30(SQL_DESC_UNSIGNED,			SQL_C_USHORT, PRM_32BIT,	lpdTrueFalse),
	InitDftSubArrayVal30(SQL_DESC_UPDATABLE,			SQL_C_USHORT, PRM_32BIT,	lpdUpdatable),
	};


//------------------------------------------------------------------------------------------------
// SQLExtendedFetch, fFetchType
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfFetchType[] = {
	InitDftNoSArray10(SQL_FETCH_NEXT,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_FIRST,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_LAST,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_PRIOR,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_ABSOLUTE,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_RELATIVE,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_FETCH_BOOKMARK,	SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLSetPos, fOption
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfSetPos[] = {
	InitDftNoSArray10(SQL_POSITION,					SQL_C_USHORT),
	InitDftNoSArray10(SQL_REFRESH,					SQL_C_USHORT),
	InitDftNoSArray20(SQL_UPDATE,						SQL_C_USHORT),
	InitDftNoSArray20(SQL_DELETE,						SQL_C_USHORT),
	InitDftNoSArray20(SQL_ADD,							SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdfLock[] = {
	InitDftNoSArray10(SQL_LOCK_NO_CHANGE,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_LOCK_EXCLUSIVE,	SQL_C_USHORT),
	InitDftNoSArray20(SQL_LOCK_UNLOCK,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdBOType[] = {
	InitDftNoSArray30(SQL_ADD,						SQL_C_USHORT),
	InitDftNoSArray30(SQL_UPDATE_BY_BOOKMARK,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_DELETE_BY_BOOKMARK,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_FETCH_BY_BOOKMARK,	SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLStatistics
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfUnique[] = {
	InitDftNoSArray10(SQL_INDEX_UNIQUE,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_INDEX_ALL,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdfAccuracy[] = {
	InitDftNoSArray10(SQL_QUICK,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_ENSURE,		SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLSpecialColumns
//------------------------------------------------------------------------------------------------
dCSEG(DFTARRAY) lpdfColType[] = {
	InitDftNoSArray10(SQL_BEST_ROWID,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_ROWVER,			SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdfScope[] = {
	InitDftNoSArray10(SQL_SCOPE_CURROW,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_SCOPE_TRANSACTION,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_SCOPE_SESSION,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdfNullable[] = {
	InitDftNoSArray10(SQL_NO_NULLS,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_NULLABLE,	SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLSetStmtOption
//------------------------------------------------------------------------------------------------

//  SQL_ASYNC_ENABLE
dCSEG(DFTARRAY) lpdAsyncEnable[] = {
	InitDftNoSArrayVal10(SQL_ASYNC_ENABLE_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_ASYNC_ENABLE_ON,		SQL_C_ULONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdDynamicColumnsEnable[] = {
    InitDftNoSArrayVal10(SQL_FALSE,	SQL_C_SSHORT,	PRM_32BIT),
    InitDftNoSArrayVal10(SQL_TRUE,  SQL_C_SSHORT,	PRM_32BIT),
};

//  SQL_ATTR_TYPE_EXCEPTION_BEHAVIOR 
dCSEG(DFTARRAY) lpdTypeExceptionBehavior[] = {
	InitDftNoSArrayVal10(SQL_TE_ERROR,  SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_TE_REPORT, SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_TE_CONTINUE,SQL_C_ULONG,	PRM_32BIT),
	};


//  SQL_CONCURRENCY
dCSEG(DFTARRAY) lpdConcurrency[] = {
	InitDftNoSArrayVal20(SQL_CONCUR_READ_ONLY,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_CONCUR_LOCK,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_CONCUR_ROWVER,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_CONCUR_VALUES,		SQL_C_ULONG,	PRM_32BIT),
	};

//  SQL_CURSOR_TYPE
dCSEG(DFTARRAY) lpdCursorType[] = {
	InitDftNoSArrayVal10(SQL_CURSOR_FORWARD_ONLY,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CURSOR_KEYSET_DRIVEN,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CURSOR_DYNAMIC,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_CURSOR_STATIC,			SQL_C_ULONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdCursorScrollable[] = {
	InitDftNoSArrayVal30(SQL_NONSCROLLABLE,			SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_SCROLLABLE,				SQL_C_SLONG,	PRM_32BIT),
	};

//  SQL_NOSCAN
dCSEG(DFTARRAY) lpdNoScan[] = {
	InitDftNoSArrayVal10(SQL_NOSCAN_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_NOSCAN_ON,		SQL_C_ULONG,	PRM_32BIT),
	};

//  SQL_SIMULATE_CURSOR
dCSEG(DFTARRAY) lpdSimCursor[] = {
	InitDftNoSArrayVal20(SQL_SC_NON_UNIQUE,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_SC_TRY_UNIQUE,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_SC_UNIQUE,			SQL_C_ULONG,	PRM_32BIT),
	};


dCSEG(DFTARRAY) lpdOdbcVer[] = {
	InitDftNoSArray30(SQL_OV_ODBC4,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_OV_ODBC3,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_OV_ODBC2,		SQL_C_USHORT),
	};

//  SQL_RETRIEVE_DATA
dCSEG(DFTARRAY) lpdRetrieveData[] = {
	InitDftNoSArrayVal20(SQL_RD_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_RD_ON,	SQL_C_ULONG,	PRM_32BIT),
	};

//  SQL_USE_BOOKMARKS
dCSEG(DFTARRAY) lpdUseBookMarks[] = {
	InitDftNoSArrayVal20(SQL_UB_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_UB_ON,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_UB_VARIABLE,	SQL_C_ULONG,	PRM_32BIT),
	};


dCSEG(DFTARRAY) lpdfPtrcbValues[] = {
	InitDftNoSArrayVal30(SQL_IS_UINTEGER,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_INTEGER,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_USMALLINT,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_SMALLINT,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_POINTER,		SQL_C_SLONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdfPtrNTScbValues[] = {
	InitDftNoSArrayVal30(SQL_NTS,					SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_POINTER,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_UINTEGER,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_INTEGER,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_USMALLINT,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_IS_SMALLINT,		SQL_C_SLONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdfDftParmcbValues[] = {
	InitDftNoSArrayVal30(SQL_NTS,					SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_NULL_DATA,			SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DEFAULT_PARAM,	SQL_C_SLONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdfParamcbValues[] = {
	InitDftNoSArrayVal30(SQL_NTS,					SQL_C_SLONG,	PRM_32BIT),
	NULLPTR, SQL_C_ULONG, szNULLPTR, MAKEVERSION(2,0), PRM_32HDL | PRM_HWND, 0, NULL,
	ROWSETVAL, SQL_C_ULONG, szROWSETVAL, MAKEVERSION(2,0), PRM_32HDL | PRM_HWND, 0, NULL,
	InitDftNoSArrayVal30(SQL_NULL_DATA,			SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DATA_AT_EXEC,		SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DEFAULT_PARAM,	SQL_C_SLONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdfParamrgbValues[] = {
	NULLPTR,			SQL_C_ULONG, szNULLPTR,		MAKEVERSION(2,0), PRM_32HDL | PRM_HWND, 0, NULL,
	ROWSETVAL,		SQL_C_ULONG, szROWSETVAL,	MAKEVERSION(2,0), PRM_32HDL | PRM_HWND, 0, NULL,
	INTERVALVAL,	SQL_C_ULONG, szINTERVALVAL,MAKEVERSION(2,0), PRM_32HDL | PRM_HWND, 0, NULL,
	};

dCSEG(DFTARRAY) lpdfNTScbValues[] = {
	InitDftNoSArrayVal30(SQL_NTS,					SQL_C_SLONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdfSetDescReccbValues[] = {
	InitDftNoSArrayVal30(SQL_NTS,					SQL_C_SLONG,	PRM_32BIT),
	NULLPTR,	SQL_C_ULONG,	szNULLPTR,	MAKEVERSION(2,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	};

dCSEG(DFTARRAY) lpdfIndicatorPtr[] = {
	NULLPTR,	SQL_C_ULONG,	szNULLPTR,	MAKEVERSION(2,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	SAMEAS_STRLEN,	SQL_C_ULONG,	szSAME_AS_STRLEN_PTR,MAKEVERSION(3,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	};

//
dCSEG(DFTARRAY) lpdfSetStmtAtr[] = {
	InitDftSubArrayVal30(SQL_ATTR_APP_ROW_DESC,					SQL_C_SSHORT,	PRM_HDESC,	lpdNullHandle),
	InitDftSubArrayVal30(SQL_ATTR_APP_PARAM_DESC,				SQL_C_SSHORT,	PRM_HDESC,	lpdNullHandle),
	InitDftSubArrayVal30(SQL_ATTR_ASYNC_ENABLE,					SQL_C_SSHORT,	PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal20(SQL_ATTR_CONCURRENCY,					SQL_C_SSHORT,	PRM_32BIT,	lpdConcurrency),
	InitDftSubArrayVal20(SQL_ATTR_CURSOR_TYPE,					SQL_C_SSHORT,	PRM_32BIT,	lpdCursorType),
	InitDftSubArrayVal30(SQL_ATTR_CURSOR_SCROLLABLE,			SQL_C_SSHORT,	PRM_32BIT,	lpdCursorScrollable),
	InitDftSubArrayVal30(SQL_ATTR_CURSOR_SENSITIVITY,			SQL_C_SSHORT,	PRM_32BIT,	lpdCursorSensitivity),
    InitDftSubArrayVal30(SQL_ATTR_DYNAMIC_COLUMNS,              SQL_C_SSHORT,   PRM_32BIT,  lpdDynamicColumnsEnable),
	InitDftSubArrayVal30(SQL_ATTR_ENABLE_AUTO_IPD,				SQL_C_SSHORT,	PRM_32BIT,	lpdSQLTrueFalse30),
	InitDftSubArrayVal30(SQL_ATTR_FETCH_BOOKMARK_PTR,			SQL_C_SSHORT,	PRM_PTR_BIN,lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_IMP_PARAM_DESC,				SQL_C_SSHORT,	PRM_HDESC,	lpdNullHandle),
	InitDftSubArrayVal30(SQL_ATTR_IMP_ROW_DESC,					SQL_C_SSHORT,	PRM_HDESC,	lpdNullHandle),
	InitDftNoSArrayVal20(SQL_ATTR_KEYSET_SIZE,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_ATTR_MAX_LENGTH,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_ATTR_MAX_ROWS,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_METADATA_ID,					SQL_C_SSHORT,	PRM_32BIT,	lpdSQLTrueFalse30),
	InitDftSubArrayVal10(SQL_ATTR_NOSCAN,							SQL_C_SSHORT,	PRM_32BIT,	lpdNoScan),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_BIND_OFFSET_PTR,		SQL_C_SSHORT,	PRM_PTR_32BIT,lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_BIND_TYPE,				SQL_C_SSHORT,	PRM_32BIT,	lpdParamBindType),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_OPERATION_PTR,			SQL_C_SSHORT,	PRM_PTR_BIN,lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_STATUS_PTR,				SQL_C_SSHORT,	PRM_PTR_BIN,lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAMS_PROCESSED_PTR,		SQL_C_SSHORT,	PRM_PTR_32BIT,lpdNullValidPointer),
	InitDftNoSArrayVal30(SQL_ATTR_PARAMSET_SIZE,					SQL_C_SSHORT,	PRM_32BIT),
#ifdef SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR
	InitDftNoSArrayVal30(SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR,SQL_C_SSHORT,	PRM_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PREDICATE_PTR,					SQL_C_SSHORT,	PRM_STR,	lpdNullValidPointer),
#endif
	InitDftNoSArrayVal10(SQL_ATTR_QUERY_TIMEOUT,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_RETRIEVE_DATA,					SQL_C_SSHORT,	PRM_32BIT,	lpdRetrieveData),
	InitDftSubArrayVal30(SQL_ATTR_ROW_BIND_OFFSET_PTR,			SQL_C_SSHORT,	PRM_PTR_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal10(SQL_ATTR_ROW_BIND_TYPE,					SQL_C_SSHORT,	PRM_32BIT,	lpdBindType),
	InitDftSubArrayVal30(SQL_ATTR_ROW_OPERATION_PTR,			SQL_C_SSHORT,	PRM_PTR_BIN,lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_ROW_STATUS_PTR,				SQL_C_SSHORT,	PRM_PTR_BIN,lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_ROWS_FETCHED_PTR,				SQL_C_SSHORT,	PRM_PTR_32BIT,lpdNullValidPointer),
	InitDftNoSArrayVal30(SQL_ATTR_ROW_ARRAY_SIZE,				SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_SIMULATE_CURSOR,				SQL_C_SSHORT,	PRM_32BIT,	lpdSimCursor),
	InitDftSubArrayVal30(SQL_ATTR_TYPE_EXCEPTION_BEHAVIOR,		SQL_C_SSHORT,	PRM_32BIT,	lpdTypeExceptionBehavior),
	InitDftSubArrayVal20(SQL_ATTR_USE_BOOKMARKS,					SQL_C_SSHORT,	PRM_32BIT,	lpdUseBookMarks),
	InitDftNoSArrayVal20(SQL_ROWSET_SIZE,							SQL_C_SSHORT,	PRM_32BIT),
};

//
dCSEG(DFTARRAY) lpdfGetStmtAtr[] = {
	InitDftNoSArrayVal30(SQL_ATTR_APP_ROW_DESC,					SQL_C_SSHORT,	PRM_HDESC),
	InitDftNoSArrayVal30(SQL_ATTR_APP_PARAM_DESC,				SQL_C_SSHORT,	PRM_HDESC),
	InitDftSubArrayVal10(SQL_ATTR_ASYNC_ENABLE,					SQL_C_SSHORT,	PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal20(SQL_ATTR_CONCURRENCY,					SQL_C_SSHORT,	PRM_32BIT,	lpdConcurrency),
	InitDftSubArrayVal20(SQL_ATTR_CURSOR_TYPE,					SQL_C_SSHORT,	PRM_32BIT,	lpdCursorType),
	InitDftSubArrayVal30(SQL_ATTR_CURSOR_SCROLLABLE,			SQL_C_SSHORT,	PRM_32BIT,	lpdCursorScrollable),
	InitDftSubArrayVal30(SQL_ATTR_CURSOR_SENSITIVITY,			SQL_C_SSHORT,	PRM_32BIT,	lpdCursorSensitivity),
    InitDftSubArrayVal20(SQL_ATTR_DYNAMIC_COLUMNS,              SQL_C_SSHORT,   PRM_32BIT,  lpdDynamicColumnsEnable),
	InitDftSubArrayVal30(SQL_ATTR_ENABLE_AUTO_IPD,				SQL_C_SSHORT,	PRM_32BIT,	lpdSQLTrueFalse30),
	InitDftSubArrayVal30(SQL_ATTR_FETCH_BOOKMARK_PTR,			SQL_C_SSHORT,	PRM_PTR_BIN,lpdNullValidPointer),
	InitDftNoSArrayVal30(SQL_ATTR_IMP_PARAM_DESC,				SQL_C_SSHORT,	PRM_HDESC),
	InitDftNoSArrayVal30(SQL_ATTR_IMP_ROW_DESC,					SQL_C_SSHORT,	PRM_HDESC),
	InitDftNoSArrayVal20(SQL_ATTR_KEYSET_SIZE,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_ATTR_MAX_LENGTH,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_ATTR_MAX_ROWS,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_METADATA_ID,					SQL_C_SSHORT,	PRM_32BIT,	lpdSQLTrueFalse30),
	InitDftSubArrayVal10(SQL_ATTR_NOSCAN,							SQL_C_SSHORT,	PRM_32BIT,	lpdNoScan),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_BIND_OFFSET_PTR,		SQL_C_SSHORT,	PRM_PTR_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_BIND_TYPE,				SQL_C_SSHORT,	PRM_32BIT,	lpdParamBindType),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_OPERATION_PTR,			SQL_C_SSHORT,	PRM_PTR_BIN,		lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_STATUS_PTR,				SQL_C_SSHORT,	PRM_PTR_BIN,		lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAMS_PROCESSED_PTR,		SQL_C_SSHORT,	PRM_PTR_32BIT,		lpdNullValidPointer),
	InitDftNoSArrayVal30(SQL_ATTR_PARAMSET_SIZE,					SQL_C_SSHORT,	PRM_32BIT),
#ifdef SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR
	InitDftNoSArrayVal30(SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR,SQL_C_SSHORT,	PRM_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PREDICATE_PTR,					SQL_C_SSHORT,	PRM_STR,		lpdNullValidPointer),
#endif
	InitDftNoSArrayVal10(SQL_ATTR_QUERY_TIMEOUT,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_RETRIEVE_DATA,					SQL_C_SSHORT,	PRM_32BIT,	lpdRetrieveData),
	InitDftSubArrayVal30(SQL_ATTR_ROW_BIND_OFFSET_PTR,			SQL_C_SSHORT,	PRM_PTR_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal10(SQL_ATTR_ROW_BIND_TYPE,					SQL_C_SSHORT,	PRM_32BIT,	lpdBindType),
	InitDftNoSArrayVal30(SQL_ATTR_ROW_NUMBER,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_ROW_OPERATION_PTR,			SQL_C_SSHORT,	PRM_PTR_BIN,		lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_ROW_STATUS_PTR,				SQL_C_SSHORT,	PRM_PTR_BIN,		lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_ROWS_FETCHED_PTR,				SQL_C_SSHORT,	PRM_PTR_32BIT,		lpdNullValidPointer),
	InitDftNoSArrayVal20(SQL_ATTR_ROW_ARRAY_SIZE,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_SIMULATE_CURSOR,				SQL_C_SSHORT,	PRM_32BIT,	lpdSimCursor),
	InitDftSubArrayVal30(SQL_ATTR_TYPE_EXCEPTION_BEHAVIOR,			SQL_C_SSHORT,	PRM_32BIT,	lpdTypeExceptionBehavior),
	InitDftSubArrayVal20(SQL_ATTR_USE_BOOKMARKS,					SQL_C_SSHORT,	PRM_32BIT,	lpdUseBookMarks),
	InitDftNoSArrayVal20(SQL_ATTR_ROW_NUMBER,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROWSET_SIZE,							SQL_C_SSHORT,	PRM_32BIT),
	};


//
dCSEG(DFTARRAY) lpdfSetStmtOpt[] = {
 	InitDftNoSArrayVal10(SQL_QUERY_TIMEOUT,	SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MAX_ROWS,			SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal10(SQL_NOSCAN,				SQL_C_USHORT,	PRM_32BIT,	lpdNoScan),
	InitDftNoSArrayVal10(SQL_MAX_LENGTH,		SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal10(SQL_ASYNC_ENABLE,		SQL_C_USHORT,	PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal10(SQL_BIND_TYPE,			SQL_C_USHORT,	PRM_32BIT,	lpdBindType),
	InitDftSubArrayVal20(SQL_CURSOR_TYPE,		SQL_C_USHORT,	PRM_32BIT,	lpdCursorType),
	InitDftSubArrayVal20(SQL_CONCURRENCY,		SQL_C_USHORT,	PRM_32BIT,	lpdConcurrency),
	InitDftNoSArrayVal20(SQL_KEYSET_SIZE,		SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROWSET_SIZE,		SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_SIMULATE_CURSOR,	SQL_C_USHORT,	PRM_32BIT,	lpdSimCursor),
	InitDftSubArrayVal20(SQL_RETRIEVE_DATA,	SQL_C_USHORT,	PRM_32BIT,	lpdRetrieveData),
	InitDftSubArrayVal20(SQL_USE_BOOKMARKS,	SQL_C_USHORT,	PRM_32BIT,	lpdUseBookMarks),
	InitDftNoSArrayVal20(SQL_GET_BOOKMARK,		SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROW_NUMBER,		SQL_C_USHORT,	PRM_32BIT),
#ifdef _INTERNAL
	InitDftSubArrayVal20(SQL_SOPT_SS_TEXTPTR_LOGGING,	SQL_C_USHORT,	PRM_32BIT,	lpdTptrLog),
	InitDftNoSArrayVal20(SQL_SOPT_SS_CURRENT_COMMAND,	SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_SOPT_SS_HIDDEN_COLUMNS,	SQL_C_USHORT,	PRM_32BIT,	lpdHiddenCols),
	InitDftSubArrayVal20(SQL_SOPT_SS_NOBROWSETABLE,		SQL_C_USHORT,	PRM_32BIT,	lpdBrowseTable),

#endif
	};

//
dCSEG(DFTARRAY) lpdfGetStmtOpt[] = {
 	InitDftNoSArrayVal10(SQL_QUERY_TIMEOUT,	SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MAX_ROWS,			SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal10(SQL_NOSCAN,				SQL_C_USHORT,	PRM_32BIT,	lpdNoScan),
	InitDftNoSArrayVal10(SQL_MAX_LENGTH,		SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal10(SQL_ASYNC_ENABLE,		SQL_C_USHORT,	PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal10(SQL_BIND_TYPE,			SQL_C_USHORT,	PRM_32BIT,	lpdBindType),
	InitDftSubArrayVal20(SQL_CURSOR_TYPE,		SQL_C_USHORT,	PRM_32BIT,	lpdCursorType),
	InitDftSubArrayVal20(SQL_CONCURRENCY,		SQL_C_USHORT,	PRM_32BIT,	lpdConcurrency),
	InitDftNoSArrayVal20(SQL_KEYSET_SIZE,		SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROWSET_SIZE,		SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_SIMULATE_CURSOR,	SQL_C_USHORT,	PRM_32BIT,	lpdSimCursor),
	InitDftSubArrayVal20(SQL_RETRIEVE_DATA,	SQL_C_USHORT,	PRM_32BIT,	lpdRetrieveData),
	InitDftSubArrayVal20(SQL_USE_BOOKMARKS,	SQL_C_USHORT,	PRM_32BIT,	lpdUseBookMarks),
	InitDftNoSArrayVal20(SQL_GET_BOOKMARK,		SQL_C_USHORT,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROW_NUMBER,		SQL_C_USHORT,	PRM_32BIT),
	#ifdef _INTERNAL
	InitDftSubArrayVal20(SQL_SOPT_SS_TEXTPTR_LOGGING,	SQL_C_USHORT,	PRM_32BIT,	lpdTptrLog),
	InitDftNoSArrayVal20(SQL_SOPT_SS_CURRENT_COMMAND,	SQL_C_USHORT,	PRM_32BIT),
	InitDftSubArrayVal20(SQL_SOPT_SS_HIDDEN_COLUMNS,	SQL_C_USHORT,	PRM_32BIT,	lpdHiddenCols),
	InitDftSubArrayVal20(SQL_SOPT_SS_NOBROWSETABLE,		SQL_C_USHORT,	PRM_32BIT,	lpdBrowseTable),

	#endif
	};

//------------------------------------------------------------------------------------------------
// SQLSetConnectOptions
//------------------------------------------------------------------------------------------------

//  SQL_ACCESS_MODE
dCSEG(DFTARRAY) lpdAccessMode[] = {
	InitDftNoSArrayVal10(SQL_MODE_READ_WRITE,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MODE_READ_ONLY,	SQL_C_ULONG,	PRM_32BIT),
	};


//  SQL_ACCESS_MODE
dCSEG(DFTARRAY) lpdNullTermValues[] = {
	InitDftNoSArrayVal30(SQL_TRUE,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_FALSE,	SQL_C_ULONG,	PRM_32BIT),
	};

//  SQL_AUTOCOMMIT
dCSEG(DFTARRAY) lpdAutocommit[] = {
	InitDftNoSArrayVal10(SQL_AUTOCOMMIT_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_AUTOCOMMIT_ON,	SQL_C_ULONG,	PRM_32BIT),
	};


/* values for SQL_ATTR_DISCONNECT_BEHAVIOR */
dCSEG(DFTARRAY) lpdDisconnectBehavior[] = {
	InitDftNoSArrayVal30(SQL_DB_RETURN_TO_POOL,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DB_DISCONNECT,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DB_DEFAULT,			SQL_C_ULONG,	PRM_32BIT),
	};

/* values for SQL_ATTR_ENLIST_IN_DTC */
dCSEG(DFTARRAY) lpdEnlistInDtc[] = {
	InitDftNoSArrayVal30(SQL_DTC_DONE,	SQL_C_SLONG,	PRM_32BIT),
	};

//  SQL_ODBC_CURSORS
dCSEG(DFTARRAY) lpdCursors[] = {
	InitDftNoSArrayVal20(SQL_CUR_USE_IF_NEEDED,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_CUR_USE_ODBC,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal20(SQL_CUR_USE_DRIVER,		SQL_C_ULONG,	PRM_32BIT),
	};


//  SQL_TRACE
dCSEG(DFTARRAY) lpdTrace[] = {
	InitDftNoSArrayVal10(SQL_OPT_TRACE_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_OPT_TRACE_ON,		SQL_C_ULONG,	PRM_32BIT),
	};

// SQL_QUIET_MODE
dCSEG(DFTARRAY) lpdQuiet[] = {
	NULLPTR,	SQL_C_ULONG,	szNULLPTR,	MAKEVERSION(2,0),	PRM_32HDL | PRM_HWND,	0, NULL,
	DEX_HWND,		SQL_C_ULONG,	szHWND,		MAKEVERSION(2,0), PRM_32HDL | PRM_HWND,	0,	NULL,
	};



#ifdef _INTERNAL


//
dCSEG(DFTARRAY) lpdProcForPrep[] = {
	InitDftNoSArrayVal10(SQL_UP_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_UP_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_UP_ON_DROP,SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_UP_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};

//SQL_COPT_SS_INTEGRATED_SECURITY
dCSEG(DFTARRAY) lpdIntegSecurity[] = {
	InitDftNoSArrayVal10(SQL_IS_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_IS_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_IS_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};

//SQL_COPT_SS_PRESERVE_CURSORS
dCSEG(DFTARRAY) lpdPrsrvCursors[] = {
	InitDftNoSArrayVal10(SQL_PC_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_PC_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_PC_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdAnsiOem[] = {
	InitDftNoSArrayVal10(SQL_AO_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_AO_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_AO_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdConnDead[] = {
	InitDftNoSArrayVal10(SQL_CD_FALSE,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CD_TRUE,	SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdFallbackConn[] = {
	InitDftNoSArrayVal10(SQL_FB_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_FB_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_FB_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdPerfData[] = {
	InitDftNoSArrayVal10(SQL_PERF_START,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_PERF_STOP,		SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdPerfDataLog[] = {
	InitDftNoSArrayVal10(SQL_SS_DL_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdPerfQI[] = {
	InitDftNoSArrayVal10(SQL_SS_QI_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdPerfQLog[] = {
	InitDftNoSArrayVal10(SQL_SS_QL_DEFAULT,	SQL_C_ULONG,	PRM_32BIT),
	};

dCSEG(DFTARRAY) lpdQuotedID[] = {
	InitDftNoSArrayVal10(SQL_QI_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_QI_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_QI_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};

dCSEG(DFTARRAY) lpdAnsiNpw[] = {
	InitDftNoSArrayVal10(SQL_AD_OFF,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_AD_ON,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_AD_DEFAULT,SQL_C_ULONG,	PRM_32BIT),

	};





#endif

// SetConnectOptions
dCSEG(DFTARRAY) lpdfSetConOpt[] = {
	InitDftSubArrayVal10(SQL_ACCESS_MODE,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAccessMode),
	InitDftSubArrayVal10(SQL_AUTOCOMMIT,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAutocommit),
	InitDftNoSArrayVal10(SQL_LOGIN_TIMEOUT,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_OPT_TRACE,				SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTrace),
	InitDftSubArrayVal10(SQL_OPT_TRACEFILE,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,		lpdfStringValues),
	InitDftSubArrayVal10(SQL_TRANSLATE_DLL,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,		lpdfStringValues),
	InitDftNoSArrayVal10(SQL_TRANSLATE_OPTION,	SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_TXN_ISOLATION,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTxnIsol),
	InitDftSubArrayVal20(SQL_ODBC_CURSORS,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdCursors),
	InitDftSubArrayVal20(SQL_QUIET_MODE,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32HDL,	lpdQuiet),
	InitDftNoSArrayVal20(SQL_PACKET_SIZE,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
 	InitDftNoSArrayVal10(SQL_QUERY_TIMEOUT,		SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal10(SQL_MAX_ROWS,				SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_NOSCAN,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdNoScan),
	InitDftNoSArrayVal10(SQL_MAX_LENGTH,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_ASYNC_ENABLE,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal10(SQL_BIND_TYPE,				SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdBindType),
	InitDftSubArrayVal20(SQL_CURSOR_TYPE,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdCursorType),
	InitDftSubArrayVal20(SQL_CONCURRENCY,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdConcurrency),
	InitDftNoSArrayVal20(SQL_KEYSET_SIZE,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROWSET_SIZE,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal20(SQL_SIMULATE_CURSOR,		SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdSimCursor),
	InitDftSubArrayVal20(SQL_RETRIEVE_DATA,		SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdRetrieveData),
	InitDftSubArrayVal20(SQL_USE_BOOKMARKS,		SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdUseBookMarks),
	InitDftNoSArrayVal20(SQL_GET_BOOKMARK,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ROW_NUMBER,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_CURRENT_CATALOG,	SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,	lpdfStringValues),
	#ifdef _INTERNAL
	InitDftSubArrayVal20(SQL_COPT_SS_REMOTE_PWD,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_USE_PROC_FOR_PREP,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdProcForPrep),
	InitDftSubArrayVal20(SQL_COPT_SS_INTEGRATED_SECURITY,SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdIntegSecurity),
	InitDftSubArrayVal20(SQL_COPT_SS_PRESERVE_CURSORS,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPrsrvCursors),
	InitDftSubArrayVal20(SQL_COPT_SS_USER_DATA,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_ANSI_OEM,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdAnsiOem),
	InitDftSubArrayVal20(SQL_COPT_SS_ENLIST_IN_DTC,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdEnlistInDtc),
	InitDftNoSArrayVal20(SQL_COPT_SS_ENLIST_IN_XA,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT),
	InitDftSubArrayVal20(SQL_COPT_SS_FALLBACK_CONNECT,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdFallbackConn),
	InitDftSubArrayVal20(SQL_COPT_SS_CONNECTION_DEAD,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdConnDead),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_DATA,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPerfData),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_DATA_LOG,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_QUERY_INTERVAL,SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPerfQI),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_QUERY_LOG,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_QUERY,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPerfData),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_DATA_LOG_NOW,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,   lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_QUOTED_IDENT,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT, lpdQuotedID	),
	InitDftSubArrayVal20(SQL_COPT_SS_ANSI_NPW,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT, lpdAnsiNpw),
	InitDftSubArrayVal20(SQL_COPT_SS_BCP,				SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT, lpdBCP),


#endif


	};

// SetConnectOptions
dCSEG(DFTARRAY) lpdfSetEnvAttr[] = {
	InitDftSubArrayVal30(SQL_ATTR_ODBC_VERSION,	SQL_C_USHORT,		PRM_32BIT,		lpdOdbcVer),
	InitDftSubArrayVal30(SQL_ATTR_OUTPUT_NTS,		SQL_C_USHORT,		PRM_32BIT,		lpdSQLTrueFalse30),
	};

// SetConnectOptions
dCSEG(DFTARRAY) lpdfGetEnvAttr[] = {
	InitDftSubArrayVal30(SQL_ATTR_ODBC_VERSION,	SQL_C_USHORT,	PRM_32BIT,	lpdOdbcVer),
	};

// GetConnectOptions (cannot do Statement options here)
dCSEG(DFTARRAY) lpdfGetConOpt[] = {
	InitDftSubArrayVal10(SQL_ACCESS_MODE,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAccessMode),
	InitDftSubArrayVal10(SQL_AUTOCOMMIT,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAutocommit),
	InitDftNoSArrayVal10(SQL_LOGIN_TIMEOUT,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_OPT_TRACE,				SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTrace),
	InitDftSubArrayVal10(SQL_OPT_TRACEFILE,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftSubArrayVal10(SQL_TRANSLATE_DLL,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftNoSArrayVal10(SQL_TRANSLATE_OPTION,	SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_TXN_ISOLATION,		SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTxnIsol),
	InitDftSubArrayVal20(SQL_CURRENT_QUALIFIER,	SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftSubArrayVal20(SQL_ODBC_CURSORS,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdCursors),
	InitDftNoSArrayVal20(SQL_QUIET_MODE,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32HDL),
	InitDftNoSArrayVal20(SQL_PACKET_SIZE,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),

	#ifdef _INTERNAL
	InitDftSubArrayVal20(SQL_COPT_SS_REMOTE_PWD,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_USE_PROC_FOR_PREP,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdProcForPrep),
	InitDftSubArrayVal20(SQL_COPT_SS_INTEGRATED_SECURITY,SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdIntegSecurity),
	InitDftSubArrayVal20(SQL_COPT_SS_PRESERVE_CURSORS,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPrsrvCursors),
	InitDftSubArrayVal20(SQL_COPT_SS_USER_DATA,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_ANSI_OEM,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdAnsiOem),
	InitDftSubArrayVal20(SQL_COPT_SS_ENLIST_IN_DTC,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdEnlistInDtc),
	InitDftNoSArrayVal20(SQL_COPT_SS_ENLIST_IN_XA,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT),
	InitDftSubArrayVal20(SQL_COPT_SS_FALLBACK_CONNECT,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdFallbackConn),
	InitDftSubArrayVal20(SQL_COPT_SS_CONNECTION_DEAD,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdConnDead),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_DATA,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPerfData),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_DATA_LOG,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_QUERY_INTERVAL,SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPerfQI),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_QUERY_LOG,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,	lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_QUERY,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT,	lpdPerfData),
	InitDftSubArrayVal20(SQL_COPT_SS_PERF_DATA_LOG_NOW,	SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_STR,   lpdfStringValues),
	InitDftSubArrayVal20(SQL_COPT_SS_QUOTED_IDENT,		SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT, lpdQuotedID	),
	InitDftSubArrayVal20(SQL_COPT_SS_ANSI_NPW,			SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT, lpdAnsiNpw),
	InitDftSubArrayVal20(SQL_COPT_SS_BCP,				SQL_C_USHORT,	PRM_CONN_PREFIX |PRM_32BIT, lpdBCP),


#endif

	};

// GetConnectAttr (cannot do Statement options here)
dCSEG(DFTARRAY) lpdfGetConAtr[] = {
	InitDftSubArrayVal10(SQL_ATTR_ACCESS_MODE,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAccessMode),
	InitDftSubArrayVal30(SQL_ATTR_ASYNC_ENABLE,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal10(SQL_ATTR_AUTOCOMMIT,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAutocommit),
	InitDftNoSArrayVal30(SQL_ATTR_AUTO_IPD,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_CURRENT_CATALOG,				SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftNoSArrayVal30(SQL_ATTR_CONNECTION_TIMEOUT,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_DISCONNECT_BEHAVIOR,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT, lpdDisconnectBehavior),
	InitDftSubArrayVal30(SQL_ATTR_ENLIST_IN_DTC,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT, lpdEnlistInDtc),
	InitDftNoSArrayVal30(SQL_ATTR_ENLIST_IN_XA,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal10(SQL_ATTR_LOGIN_TIMEOUT,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_ODBC_CURSORS,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdCursors),
	InitDftNoSArrayVal20(SQL_ATTR_PACKET_SIZE,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_QUIET_MODE,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32HDL,	lpdQuiet),
	InitDftSubArrayVal10(SQL_ATTR_TRACE,							SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTrace),
	InitDftSubArrayVal10(SQL_ATTR_TRACEFILE,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftSubArrayVal10(SQL_ATTR_TRANSLATE_LIB,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftNoSArrayVal10(SQL_ATTR_TRANSLATE_OPTION,				SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_ATTR_TXN_ISOLATION,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTxnIsol),
#ifdef SQL_ATTR_USE_LOCATORS
	InitDftSubArrayVal30(SQL_ATTR_USE_LOCATORS,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT, lpdUseLocators),
#endif
	};

// SetConnectAttr
dCSEG(DFTARRAY) lpdfSetConAtr[] = {
	InitDftSubArrayVal10(SQL_ATTR_ACCESS_MODE,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAccessMode),
	InitDftSubArrayVal30(SQL_ATTR_ASYNC_ENABLE,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAsyncEnable),
	InitDftSubArrayVal10(SQL_ATTR_AUTOCOMMIT,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdAutocommit),
	InitDftSubArrayVal10(SQL_ATTR_ROW_BIND_TYPE,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdBindType),
	InitDftSubArrayVal20(SQL_ATTR_CONCURRENCY,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdConcurrency),
	InitDftNoSArrayVal30(SQL_ATTR_CONNECTION_TIMEOUT,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_ATTR_CURRENT_CATALOG,				SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,lpdfStringValues),
	InitDftSubArrayVal20(SQL_ATTR_CURSOR_TYPE,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdCursorType),
	InitDftSubArrayVal30(SQL_ATTR_DISCONNECT_BEHAVIOR,			SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT, lpdDisconnectBehavior),
	InitDftSubArrayVal30(SQL_ATTR_ENLIST_IN_DTC,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT, lpdEnlistInDtc),
	InitDftNoSArrayVal30(SQL_ATTR_ENLIST_IN_XA,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_ENABLE_AUTO_IPD,				SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdSQLTrueFalse30),
	InitDftSubArrayVal30(SQL_ATTR_FETCH_BOOKMARK_PTR,			SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_PTR_BIN,lpdNullValidPointer),
	InitDftNoSArrayVal20(SQL_ATTR_KEYSET_SIZE,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ATTR_LOGIN_TIMEOUT,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ATTR_MAX_ROWS,						SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftNoSArrayVal20(SQL_ATTR_MAX_LENGTH,						SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_METADATA_ID,					SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_32BIT,	lpdSQLTrueFalse30),
	InitDftSubArrayVal10(SQL_ATTR_NOSCAN,							SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdNoScan),
	InitDftSubArrayVal20(SQL_ATTR_ODBC_CURSORS,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdCursors),
	InitDftNoSArrayVal20(SQL_ATTR_PACKET_SIZE,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_BIND_OFFSET_PTR,		SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_BIND_TYPE,				SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_32BIT,	lpdParamBindType),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_OPERATION_PTR,			SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_BIN,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAM_STATUS_PTR,				SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_BIN,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_PARAMS_PROCESSED_PTR,		SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_32BIT,	lpdNullValidPointer),
	InitDftNoSArrayVal30(SQL_ATTR_PARAMSET_SIZE,					SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_32BIT),
#ifdef SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR
	InitDftNoSArrayVal30(SQL_ATTR_PREDICATE_OCTET_LENGTH_PTR,SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_BIN32,	lpdNullValidPointer),
	InitDftNoSArrayVal30(SQL_ATTR_PREDICATE_PTR,					SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_STR,	lpdNullValidPointer),
#endif
	InitDftSubArrayVal20(SQL_ATTR_QUIET_MODE,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32HDL,	lpdQuiet),
 	InitDftNoSArrayVal10(SQL_ATTR_QUERY_TIMEOUT,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_RETRIEVE_DATA,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdRetrieveData),
	InitDftNoSArrayVal20(SQL_ATTR_ROW_ARRAY_SIZE,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT),
	InitDftSubArrayVal30(SQL_ATTR_ROW_BIND_OFFSET_PTR,			SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_32BIT,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_ROW_OPERATION_PTR,			SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_BIN,	lpdNullValidPointer),
	InitDftSubArrayVal30(SQL_ATTR_ROW_STATUS_PTR,				SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_PTR_BIN,	lpdNullValidPointer),
//	InitDftNoSArrayVal30(SQL_ATTR_ROW_FETCHED_PTR,				SQL_C_USHORT,	PRM_STMT_PREFIX |  PRM_32BIT),
	InitDftSubArrayVal20(SQL_ATTR_SIMULATE_CURSOR,				SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdSimCursor),
	InitDftSubArrayVal10(SQL_ATTR_TRACE,							SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTrace),
	InitDftSubArrayVal10(SQL_ATTR_TRACEFILE,						SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,		lpdfStringValues),
	InitDftSubArrayVal10(SQL_ATTR_TRANSLATE_LIB,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_STR,		lpdfStringValues),
	InitDftNoSArrayVal10(SQL_ATTR_TRANSLATE_OPTION,				SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT),
	InitDftSubArrayVal10(SQL_ATTR_TXN_ISOLATION,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT,	lpdTxnIsol),
	InitDftSubArrayVal20(SQL_ATTR_USE_BOOKMARKS,					SQL_C_USHORT,	PRM_STMT_PREFIX | PRM_32BIT,	lpdUseBookMarks),
#ifdef SQL_ATTR_USE_LOCATORS
	InitDftSubArrayVal30(SQL_ATTR_USE_LOCATORS,					SQL_C_USHORT,	PRM_CONN_PREFIX | PRM_32BIT, lpdUseLocators),
#endif
	};


//------------------------------------------------------------------------------------------------
// SQLSetScrollOptions
//------------------------------------------------------------------------------------------------

//  fConcurrency
dCSEG(DFTARRAY) lpdSSOConcur[] = {
	InitDftNoSArrayVal10(SQL_CONCUR_READ_ONLY,	SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CONCUR_LOCK,			SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CONCUR_ROWVER,		SQL_C_ULONG,	PRM_32BIT),
	InitDftNoSArrayVal10(SQL_CONCUR_VALUES,		SQL_C_ULONG,	PRM_32BIT),
	};

//  crowKeyset
dCSEG(DFTARRAY) lpdcrowKeyset[] = {
	InitDftNoSArray10(SQL_SCROLL_FORWARD_ONLY,	SQL_C_SLONG),
	InitDftNoSArray10(SQL_SCROLL_KEYSET_DRIVEN,	SQL_C_SLONG),
	InitDftNoSArray10(SQL_SCROLL_DYNAMIC,			SQL_C_SLONG),
	InitDftNoSArray10(SQL_SCROLL_STATIC,			SQL_C_SLONG),
	};

//------------------------------------------------------------------------------------------------
// SQLTransact
//------------------------------------------------------------------------------------------------

//  fType
dCSEG(DFTARRAY) lpdTransactType[] = {
	InitDftNoSArray10(SQL_COMMIT,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_ROLLBACK,	SQL_C_USHORT),
	};

//------------------------------------------------------------------------------------------------
// SQLAllocHandle
//------------------------------------------------------------------------------------------------

//  fType
dCSEG(DFTARRAY) lpdSQLAllocHandle[] = {
	InitDftNoSArray30(SQL_HANDLE_ENV,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_HANDLE_DBC,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_HANDLE_STMT,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_HANDLE_SENV,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_HANDLE_DESC,		SQL_C_USHORT),
	};


#ifdef SQL_LOCATOR_HOLD

//Locators
dCSEG(DFTARRAY) lpdLocfOperation[] = {
	InitDftNoSArray30(SQL_LOCATOR_HOLD,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_LOCATOR_FREE,		SQL_C_USHORT),
	};

//Locators
dCSEG(DFTARRAY) lpdLocfLocatorType[] = {
	InitDftNoSArray30(SQL_C_CLOB_LOCATOR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_C_BLOB_LOCATOR,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdLocfSrcTargType[] = {
	InitDftNoSArray30(SQL_C_CHAR,					SQL_C_USHORT),
	InitDftNoSArray30(SQL_C_BINARY,				SQL_C_USHORT),
	InitDftNoSArray30(SQL_C_CLOB_LOCATOR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_C_BLOB_LOCATOR,		SQL_C_USHORT),
	};

#endif

dCSEG(DFTARRAY) lpdDiagId[] = {
	InitDftNoSArrayVal30(SQL_DIAG_CURSOR_ROW_COUNT,			SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_DYNAMIC_FUNCTION,			SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_DYNAMIC_FUNCTION_CODE,	SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_NUMBER,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_RETURNCODE,					SQL_C_SSHORT,	PRM_16BIT),
	InitDftNoSArrayVal30(SQL_DIAG_ROW_COUNT,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_CLASS_ORIGIN,				SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_COLUMN_NUMBER,				SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_CONNECTION_NAME,			SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_MESSAGE_TEXT,				SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_NATIVE,						SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_ROW_NUMBER,					SQL_C_SSHORT,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_SERVER_NAME,				SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_SQLSTATE,					SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_SUBCLASS_ORIGIN,			SQL_C_SSHORT,	PRM_STR),
#ifdef _INTERNAL
	InitDftNoSArrayVal30(SQL_DIAG_SS_MSGSTATE,				SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_SS_SEVERITY,				SQL_C_SLONG,	PRM_32BIT),
	InitDftNoSArrayVal30(SQL_DIAG_SS_SRVNAME,				SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_SS_PROCNAME,				SQL_C_SSHORT,	PRM_STR),
	InitDftNoSArrayVal30(SQL_DIAG_SS_LINE,					SQL_C_SSHORT,	PRM_16BIT),
#endif
	};


dCSEG(DFTARRAY) lpdSQLEndTran[] = {
	InitDftNoSArray30(SQL_HANDLE_ENV,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_HANDLE_DBC,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_HANDLE_SENV,		SQL_C_USHORT),
	};


dCSEG(DFTARRAY) lpdOutputHandle[] = {
	InitDftNoSArray30(SQL_NULL_HENV,		SQL_C_SLONG),
	InitDftNoSArray30(SQL_NULL_HDBC,		SQL_C_SLONG),
	InitDftNoSArray30(SQL_NULL_HSTMT,	SQL_C_SLONG),
	InitDftNoSArray30(SQL_NULL_HDESC,	SQL_C_SLONG),
	};

//@INSTALLER

dCSEG(DFTARRAY) lpdSQLTrueFalse[] = {
	InitDftNoSArray25(SQL_FALSE,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_TRUE,		SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdSQLInstallTranslator[] = {
	InitDftNoSArray25(ODBC_INSTALL_INQUIRY,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_INSTALL_COMPLETE,	SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdConfigDSN[] = {
	InitDftNoSArray25(ODBC_INSTALL_DRIVER,		SQL_C_USHORT),
	InitDftNoSArray25(ODBC_REMOVE_DRIVER,	SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdSQLConfigDriver[] = {
	InitDftNoSArray25(ODBC_INSTALL_DRIVER,		SQL_C_USHORT),
	InitDftNoSArray25(ODBC_REMOVE_DRIVER,	SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdSQLInstallDriverEx[] = {
	InitDftNoSArray30(ODBC_INSTALL_INQUIRY,		SQL_C_USHORT),
	InitDftNoSArray30(ODBC_INSTALL_COMPLETE,		SQL_C_USHORT),
	};

//dCSEG(DFTARRAY) lpdSQLGetPrivateProfileString[] = {
//	InitDftNoSArray25(ODBC_INI,		SQL_C_USHORT),
//	InitDftNoSArray25(ODBCINST_INI,	SQL_C_USHORT),
//};


dCSEG(DFTARRAY) lpdSQLConfigDataSource[] = {
	InitDftNoSArray25(ODBC_ADD_DSN,		SQL_C_USHORT),
	InitDftNoSArray25(ODBC_CONFIG_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_REMOVE_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_ADD_SYS_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_CONFIG_SYS_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_REMOVE_SYS_DSN,	SQL_C_USHORT),
	InitDftNoSArray30(ODBC_REMOVE_DEFAULT_DSN,	SQL_C_USHORT),
	};


dCSEG(DFTARRAY) lpdSQLManageDataSources[] = {
	InitDftNoSArray25(ODBC_ADD_DSN,		SQL_C_USHORT),
	InitDftNoSArray25(ODBC_CONFIG_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_REMOVE_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_ADD_SYS_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_CONFIG_SYS_DSN,	SQL_C_USHORT),
	InitDftNoSArray25(ODBC_REMOVE_SYS_DSN,	SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdSQLDataSourceToDriver[] = {
	InitDftNoSArray25(SQL_CHAR,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_BIGINT,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_NUMERIC,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_DECIMAL,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_INTEGER,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_SMALLINT,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_FLOAT,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_GUID,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_REAL,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_DOUBLE,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_VARCHAR,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_DATE,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_TIME,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_TIMESTAMP,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_LONGVARCHAR,	SQL_C_USHORT),
	InitDftNoSArray25(SQL_BINARY,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_VARBINARY,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_LONGVARBINARY,SQL_C_USHORT),
	InitDftNoSArray25(SQL_BIGINT,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_TINYINT,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_BIT,				SQL_C_USHORT),
	};

dCSEG(DFTARRAY) lpdSQLDriverToDataSource[] = {
	InitDftNoSArray25(SQL_CHAR,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_BIGINT,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_NUMERIC,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_DECIMAL,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_INTEGER,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_SMALLINT,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_FLOAT,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_GUID,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_REAL,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_DOUBLE,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_VARCHAR,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_DATE,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_TIME,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_TIMESTAMP,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_LONGVARCHAR,	SQL_C_USHORT),
	InitDftNoSArray25(SQL_BINARY,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_VARBINARY,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_LONGVARBINARY,SQL_C_USHORT),
	InitDftNoSArray25(SQL_BIGINT,			SQL_C_USHORT),
	InitDftNoSArray25(SQL_TINYINT,		SQL_C_USHORT),
	InitDftNoSArray25(SQL_BIT,				SQL_C_USHORT),
	};


//------------------------------------------------------------------------------------------------
// SQLGetFunctions
//------------------------------------------------------------------------------------------------

dCSEG(DFTARRAY) lpdGetFunct[] = {
	InitDftNoSArray10(SQL_API_ALL_FUNCTIONS,			SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_ODBC3_ALL_FUNCTIONS,	SQL_C_USHORT),
// Core Functions
	InitDftNoSArray10(SQL_API_SQLALLOCCONNECT,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLALLOCENV,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLALLOCSTMT,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLBINDCOL,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLCANCEL,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLCOLATTRIBUTES,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLCONNECT,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLDESCRIBECOL,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLDISCONNECT,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLERROR,					SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLEXECDIRECT,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLEXECUTE,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFETCH,					SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFREECONNECT,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFREEENV,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFREESTMT,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETCURSORNAME,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLNUMRESULTCOLS,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPREPARE,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLROWCOUNT,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETCURSORNAME,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETPARAM,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLTRANSACT,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLALLOCHANDLE,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFREEHANDLE,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLBINDPARAM,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLCLOSECURSOR,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLENDTRAN,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFETCHSCROLL,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETENVATTR,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETENVATTR,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETCONNECTATTR,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETCONNECTATTR,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETSTMTATTR,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETSTMTATTR,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLCOPYDESC,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETDESCFIELD,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETDESCFIELD,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETDESCREC,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETDESCREC,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETDIAGFIELD,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETDIAGREC,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLNEXTCOLUMN,			SQL_C_USHORT),

// Extension Level 1 Functions
	InitDftNoSArray10(SQL_API_SQLCOLUMNS,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLDRIVERCONNECT,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETCONNECTOPTION,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETDATA,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETNESTEDHANDLE,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETFUNCTIONS,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETINFO,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETSTMTOPTION,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLGETTYPEINFO,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPARAMDATA,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPUTDATA,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETCONNECTOPTION,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETSTMTOPTION,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSPECIALCOLUMNS,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSTATISTICS,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSTRUCTUREDTYPECOLUMNS,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSTRUCTUREDTYPES,	    SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLTABLES,				SQL_C_USHORT),
// Extension Level 2 Functions
	InitDftNoSArray10(SQL_API_SQLBROWSECONNECT,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLCOLUMNPRIVILEGES,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLDATASOURCES,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLDESCRIBEPARAM,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLEXTENDEDFETCH,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLFOREIGNKEYS,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLMORERESULTS,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLNATIVESQL,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLNUMPARAMS,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPARAMOPTIONS,		SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPRIMARYKEYS,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPROCEDURECOLUMNS,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLPROCEDURES,			SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETPOS,				SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLSETSCROLLOPTIONS,	SQL_C_USHORT),
	InitDftNoSArray10(SQL_API_SQLTABLEPRIVILEGES,	SQL_C_USHORT),
	InitDftNoSArray20(SQL_API_SQLDRIVERS,				SQL_C_USHORT),
	InitDftNoSArray20(SQL_API_SQLBINDPARAMETER,		SQL_C_USHORT),

	//ODBC 3.0 stuff
	InitDftNoSArray30(SQL_API_SQLALLOCHANDLE,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLBINDPARAM,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLBULKOPERATIONS,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLCLOSECURSOR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLCOLATTRIBUTE,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLCOPYDESC,			SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLENDTRAN,			SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLFETCHSCROLL,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLFREEHANDLE,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETCONNECTATTR,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETDESCFIELD,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETDESCREC,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETDIAGFIELD,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETDIAGREC,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETENVATTR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLGETSTMTATTR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLSETCONNECTATTR,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLSETDESCFIELD,	SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLSETDESCREC,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLSETENVATTR,		SQL_C_USHORT),
	InitDftNoSArray30(SQL_API_SQLSETSTMTATTR,		SQL_C_USHORT),
#ifdef SQL_API_SQLLOCATOR
	InitDftNoSArray30(SQL_API_SQLLOCATOR,			SQL_C_USHORT),
#endif

	};

//lpDFTARRAY lpTestSample = lpdNullable;


#endif

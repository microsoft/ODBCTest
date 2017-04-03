//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	GLOBALS.C
//|
//| Purpose:
//|		Contains all global structures (initializations)
//|
//*---------------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <commdlg.h>
#include <io.h>
#include "portable.h"
#include "standard.h"
#include "fhheader.h"
#include "resource.h"

/*
int _imp___vsnprintf(
	char *buffer,
	size_t count,
	const char *format,
	va_list argptr
)
{
	return vsnprintf(buffer, count, format, argptr);
}
*/


extern const ODBCAPI_LIST rgApiList[NUM_ODBC_APIS] = {
			//iAPINum,								iOrdinalNum,							szAPIName,											szName										fODBC30			iDD
/*0*/		{SQL_API_SQLALLOCCONNECT, 			API_SQLALLOCCONNECT,					TEXT("SQL_API_SQLALLOCCONNECT"),				TEXT("SQLAllocConnect"),				FALSE,			IDD_SQLALLOCCONNECT},
/*1*/		{SQL_API_SQLALLOCENV,				API_SQLALLOCENV,						TEXT("SQL_API_SQLALLOCENV"),					TEXT("SQLAllocEnv"),						FALSE,			IDD_SQLALLOCENV},
/*2*/		{SQL_API_SQLALLOCSTMT,			   API_SQLALLOCSTMT,						TEXT("SQL_API_SQLALLOCSTMT"),					TEXT("SQLAllocStmt"),					FALSE,			IDD_SQLALLOCSTMT},
/*3*/		{SQL_API_SQLBINDCOL,					API_SQLBINDCOL,						TEXT("SQL_API_SQLBINDCOL"),					TEXT("SQLBindCol"),						FALSE,			IDD_SQLBINDCOL},
/*4*/		{SQL_API_SQLCANCEL,					API_SQLCANCEL,							TEXT("SQL_API_SQLCANCEL"),						TEXT("SQLCancel"),						FALSE,			IDD_SQLCANCEL},
/*5*/		{SQL_API_SQLCOLATTRIBUTES,			API_SQLCOLATTRIBUTES,				TEXT("SQL_API_SQLCOLATTRIBUTES"),			TEXT("SQLColAttributes"),				FALSE,			IDD_SQLCOLATTRIBUTES},
/*6*/		{SQL_API_SQLCONNECT,					API_SQLCONNECT,						TEXT("SQL_API_SQLCONNECT"),					TEXT("SQLConnect"),						FALSE,			IDD_SQLCONNECT},
/*7*/		{SQL_API_SQLDESCRIBECOL,			API_SQLDESCRIBECOL,					TEXT("SQL_API_SQLDESCRIBECOL"),				TEXT("SQLDescribeCol"),					FALSE,			IDD_SQLDESCRIBECOL},
/*8*/		{SQL_API_SQLDISCONNECT,				API_SQLDISCONNECT,					TEXT("SQL_API_SQLDISCONNECT"),				TEXT("SQLDisconnect"),					FALSE,			IDD_SQLDISCONNECT},
/*9*/		{SQL_API_SQLERROR,				   API_SQLERROR,							TEXT("SQL_API_SQLERROR"),						TEXT("SQLError"),							FALSE,			IDD_SQLERROR},
/*10*/	{SQL_API_SQLEXECDIRECT,			   API_SQLEXECDIRECT,					TEXT("SQL_API_SQLEXECDIRECT"),				TEXT("SQLExecDirect"),					FALSE,			IDD_SQLEXECDIRECT},
/*11*/	{SQL_API_SQLEXECUTE,		  	 		API_SQLEXECUTE,						TEXT("SQL_API_SQLEXECUTE"),					TEXT("SQLExecute"),						FALSE,			IDD_SQLEXECUTE},
/*12*/	{SQL_API_SQLFETCH,				   API_SQLFETCH,							TEXT("SQL_API_SQLFETCH"),						TEXT("SQLFetch"),							FALSE,			IDD_SQLFETCH},
/*13*/	{SQL_API_SQLFREECONNECT,		   API_SQLFREECONNECT,					TEXT("SQL_API_SQLFREECONNECT"),				TEXT("SQLFreeConnect"),					FALSE,			IDD_SQLFREECONNECT},
/*14*/	{SQL_API_SQLFREEENV,		  	 		API_SQLFREEENV,						TEXT("SQL_API_SQLFREEENV"),		  			TEXT("SQLFreeEnv"),						FALSE,			IDD_SQLFREEENV},
/*15*/	{SQL_API_SQLFREESTMT,			   API_SQLFREESTMT,						TEXT("SQL_API_SQLFREESTMT"),					TEXT("SQLFreeStmt"),						FALSE,			IDD_SQLFREESTMT},
/*16*/	{SQL_API_SQLGETCURSORNAME,		   API_SQLGETCURSORNAME,				TEXT("SQL_API_SQLGETCURSORNAME"),			TEXT("SQLGetCursorName"),				FALSE,			IDD_SQLGETCURSORNAME},
/*17*/	{SQL_API_SQLNUMRESULTCOLS,		   API_SQLNUMRESULTCOLS,				TEXT("SQL_API_SQLNUMRESULTCOLS"),			TEXT("SQLNumResultCols"),				FALSE,			IDD_SQLNUMRESULTCOLS},
/*18*/	{SQL_API_SQLPREPARE,		  	 		API_SQLPREPARE,						TEXT("SQL_API_SQLPREPARE"),		  			TEXT("SQLPrepare"),						FALSE,			IDD_SQLPREPARE},
/*19*/	{SQL_API_SQLROWCOUNT,			   API_SQLROWCOUNT,						TEXT("SQL_API_SQLROWCOUNT"),					TEXT("SQLRowCount"),						FALSE,			IDD_SQLROWCOUNT},
/*20*/	{SQL_API_SQLSETCURSORNAME,		   API_SQLSETCURSORNAME,				TEXT("SQL_API_SQLSETCURSORNAME"),			TEXT("SQLSetCursorName"),				FALSE,			IDD_SQLSETCURSORNAME},
/*21*/	{SQL_API_SQLSETPARAM,			   API_SQLSETPARAM,						TEXT("SQL_API_SQLSETPARAM"),					TEXT("SQLSetParam"),						FALSE,			0,/*IDD_SQLSETPARAM*/},
/*22*/	{SQL_API_SQLTRANSACT,			   API_SQLTRANSACT,						TEXT("SQL_API_SQLTRANSACT"),					TEXT("SQLTransact"),						FALSE,			IDD_SQLTRANSACT},
/*23*/	{SQL_API_SQLCOLUMNS,		  			API_SQLCOLUMNS,						TEXT("SQL_API_SQLCOLUMNS"),					TEXT("SQLColumns"),						FALSE,			IDD_SQLCOLUMNS},
/*24*/	{SQL_API_SQLDRIVERCONNECT,	   	API_SQLDRIVERCONNECT,				TEXT("SQL_API_SQLDRIVERCONNECT"),			TEXT("SQLDriverConnect"),				FALSE,			IDD_SQLDRIVERCONNECT},
/*25*/	{SQL_API_SQLGETCONNECTOPTION,  	API_SQLGETCONNECTOPTION,			TEXT("SQL_API_SQLGETCONNECTOPTION"),		TEXT("SQLGetConnectOption"),			FALSE,			IDD_SQLGETCONNECTOPTION},
/*26*/	{SQL_API_SQLGETDATA,		   		API_SQLGETDATA,						TEXT("SQL_API_SQLGETDATA"),					TEXT("SQLGetData"),						FALSE,			IDD_SQLGETDATA},
/*27*/	{SQL_API_SQLGETFUNCTIONS,	   	API_SQLGETFUNCTIONS,					TEXT("SQL_API_SQLGETFUNCTIONS"),				TEXT("SQLGetFunctions"),				FALSE,			IDD_SQLGETFUNCTIONS},
/*28*/	{SQL_API_SQLGETINFO,		   		API_SQLGETINFO,						TEXT("SQL_API_SQLGETINFO"),					TEXT("SQLGetInfo"),						FALSE,			IDD_SQLGETINFO},
/*29*/	{SQL_API_SQLGETSTMTOPTION,	   	API_SQLGETSTMTOPTION,				TEXT("SQL_API_SQLGETSTMTOPTION"),			TEXT("SQLGetStmtOption"),				FALSE,			IDD_SQLGETSTMTOPTION},
/*30*/	{SQL_API_SQLGETTYPEINFO,	  		API_SQLGETTYPEINFO,					TEXT("SQL_API_SQLGETTYPEINFO"),				TEXT("SQLGetTypeInfo"),					FALSE,			IDD_SQLGETTYPEINFO},
/*31*/	{SQL_API_SQLPARAMDATA,		   	API_SQLPARAMDATA,						TEXT("SQL_API_SQLPARAMDATA"),					TEXT("SQLParamData"),					FALSE,			IDD_SQLPARAMDATA},
/*32*/	{SQL_API_SQLPUTDATA,		   		API_SQLPUTDATA,						TEXT("SQL_API_SQLPUTDATA"),					TEXT("SQLPutData"),						FALSE,			IDD_SQLPUTDATA},
/*33*/	{SQL_API_SQLSETCONNECTOPTION,		API_SQLSETCONNECTOPTION,			TEXT("SQL_API_SQLSETCONNECTOPTION"),		TEXT("SQLSetConnectOption"),			FALSE,			IDD_SQLSETCONNECTOPTION},
/*34*/	{SQL_API_SQLSETSTMTOPTION,	   	API_SQLSETSTMTOPTION,				TEXT("SQL_API_SQLSETSTMTOPTION"),			TEXT("SQLSetStmtOption"),				FALSE,			IDD_SQLSETSTMTOPTION},
/*35*/	{SQL_API_SQLSPECIALCOLUMNS,	   API_SQLSPECIALCOLUMNS,				TEXT("SQL_API_SQLSPECIALCOLUMNS"),			TEXT("SQLSpecialColumns"),				FALSE,			IDD_SQLSPECIALCOLUMNS},
/*36*/	{SQL_API_SQLSTATISTICS,		   	API_SQLSTATISTICS,					TEXT("SQL_API_SQLSTATISTICS"),				TEXT("SQLStatistics"),					FALSE,			IDD_SQLSTATISTICS},
/*37*/	{SQL_API_SQLTABLES,			   	API_SQLTABLES,							TEXT("SQL_API_SQLTABLES"),						TEXT("SQLTables"),						FALSE,			IDD_SQLTABLES},
/*38*/	{SQL_API_SQLBROWSECONNECT,	   	API_SQLBROWSECONNECT,				TEXT("SQL_API_SQLBROWSECONNECT"),			TEXT("SQLBrowseConnect"),				FALSE,			IDD_SQLBROWSECONNECT},
/*39*/	{SQL_API_SQLCOLUMNPRIVILEGES,  	API_SQLCOLUMNPRIVILEGES,			TEXT("SQL_API_SQLCOLUMNPRIVILEGES"),		TEXT("SQLColumnPrivileges"),			FALSE,			IDD_SQLCOLUMNPRIVILEGES},
/*40*/	{SQL_API_SQLDATASOURCES,	   	API_SQLDATASOURCES,					TEXT("SQL_API_SQLDATASOURCES"),				TEXT("SQLDataSources"),					FALSE,			IDD_SQLDATASOURCES},
/*41*/	{SQL_API_SQLDESCRIBEPARAM,	   	API_SQLDESCRIBEPARAM,				TEXT("SQL_API_SQLDESCRIBEPARAM"),			TEXT("SQLDescribeParam"),				FALSE,			IDD_SQLDESCRIBEPARAM},
/*42*/	{SQL_API_SQLEXTENDEDFETCH,	   	API_SQLEXTENDEDFETCH,				TEXT("SQL_API_SQLEXTENDEDFETCH"),			TEXT("SQLExtendedFetch"),				FALSE,			IDD_SQLEXTENDEDFETCH},
/*43*/	{SQL_API_SQLFOREIGNKEYS,	   	API_SQLFOREIGNKEYS,					TEXT("SQL_API_SQLFOREIGNKEYS"),				TEXT("SQLForeignKeys"),					FALSE,			IDD_SQLFOREIGNKEYS},
/*44*/	{SQL_API_SQLMORERESULTS,	   	API_SQLMORERESULTS,					TEXT("SQL_API_SQLMORERESULTS"),				TEXT("SQLMoreResults"),					FALSE,			IDD_SQLMORERESULTS},
/*45*/	{SQL_API_SQLNATIVESQL,		   	API_SQLNATIVESQL,						TEXT("SQL_API_SQLNATIVESQL"),					TEXT("SQLNativeSql"),					FALSE,			IDD_SQLNATIVESQL},
/*46*/	{SQL_API_SQLNUMPARAMS,		   	API_SQLNUMPARAMS,						TEXT("SQL_API_SQLNUMPARAMS"),					TEXT("SQLNumParams"),					FALSE,			IDD_SQLNUMPARAMS},
/*47*/	{SQL_API_SQLPARAMOPTIONS,	   	API_SQLPARAMOPTIONS,					TEXT("SQL_API_SQLPARAMOPTIONS"),				TEXT("SQLParamOptions"),				FALSE,			IDD_SQLPARAMOPTIONS},
/*48*/	{SQL_API_SQLPRIMARYKEYS,	   	API_SQLPRIMARYKEYS,					TEXT("SQL_API_SQLPRIMARYKEYS"),				TEXT("SQLPrimaryKeys"),					FALSE,			IDD_SQLPRIMARYKEYS},
/*49*/	{SQL_API_SQLPROCEDURECOLUMNS,  	API_SQLPROCEDURECOLUMNS,			TEXT("SQL_API_SQLPROCEDURECOLUMNS"),		TEXT("SQLProcedureColumns"),			FALSE,			IDD_SQLPROCEDURECOLUMNS},
/*50*/	{SQL_API_SQLPROCEDURES,		   	API_SQLPROCEDURES,					TEXT("SQL_API_SQLPROCEDURES"),				TEXT("SQLProcedures"),					FALSE,			IDD_SQLPROCEDURES},
/*51*/	{SQL_API_SQLSETPOS,			   	API_SQLSETPOS,							TEXT("SQL_API_SQLSETPOS"),						TEXT("SQLSetPos"),						FALSE,			IDD_SQLSETPOS},
/*52*/	{SQL_API_SQLSETSCROLLOPTIONS,  	API_SQLSETSCROLLOPTIONS,			TEXT("SQL_API_SQLSETSCROLLOPTIONS"),		TEXT("SQLSetScrollOptions"),			FALSE,			IDD_SQLSETSCROLLOPTIONS},
/*53*/	{SQL_API_SQLTABLEPRIVILEGES,   	API_SQLTABLEPRIVILEGES,				TEXT("SQL_API_SQLTABLEPRIVILEGES"),			TEXT("SQLTablePrivileges"),			FALSE,			IDD_SQLTABLEPRIVILEGES},
/*54*/	{SQL_API_SQLDRIVERS,		   		API_SQLDRIVERS,						TEXT("SQL_API_SQLDRIVERS"),					TEXT("SQLDrivers"),						FALSE,			IDD_SQLDRIVERS},
/*55*/	{SQL_API_SQLBINDPARAMETER,	   	API_SQLBINDPARAMETER,				TEXT("SQL_API_SQLBINDPARAMETER"),			TEXT("SQLBindParameter"),				FALSE,			IDD_SQLBINDPARAMETER},
/*56*/	{SQL_API_SQLALLOCHANDLE,	   	API_SQLALLOCHANDLE				,TEXT("SQL_API_SQLALLOCHANDLE"),					TEXT("SQLAllocHandle"),					TRUE,				IDD_SQLALLOCHANDLE},
/*57*/	{SQL_API_SQLBINDPARAM,		   	API_SQLBINDPARAM					,TEXT("SQL_API_SQLBINDPARAM"),					TEXT("SQLBindParam"),					TRUE,				IDD_SQLBINDPARAM},
/*58*/	{SQL_API_SQLCLOSECURSOR,	   	API_SQLCLOSECURSOR				,TEXT("SQL_API_SQLCLOSECURSOR"),					TEXT("SQLCloseCursor"),					TRUE,				IDD_SQLCLOSECURSOR},
/*59*/	{SQL_API_SQLCOLATTRIBUTE,	   	API_SQLCOLATTRIBUTE				,TEXT("SQL_API_SQLCOLATTRIBUTE"),				TEXT("SQLColAttribute"),				TRUE,				IDD_SQLCOLATTRIBUTE},
/*60*/	{SQL_API_SQLCOPYDESC,		   	API_SQLCOPYDESC					,TEXT("SQL_API_SQLCOPYDESC"),						TEXT("SQLCopyDesc"),						TRUE,				IDD_SQLCOPYDESC},
/*61*/	{SQL_API_SQLENDTRAN,		   		API_SQLENDTRAN						,TEXT("SQL_API_SQLENDTRAN"),						TEXT("SQLEndTran"),						TRUE,				IDD_SQLENDTRAN},
/*62*/	{SQL_API_SQLFETCHSCROLL,	   	API_SQLFETCHSCROLL				,TEXT("SQL_API_SQLFETCHSCROLL"),					TEXT("SQLFetchScroll"),					TRUE,				IDD_SQLFETCHSCROLL},
/*63*/	{SQL_API_SQLFREEHANDLE,		   	API_SQLFREEHANDLE					,TEXT("SQL_API_SQLFREEHANDLE"),					TEXT("SQLFreeHandle"),					TRUE,				IDD_SQLFREEHANDLE},
/*64*/	{SQL_API_SQLGETCONNECTATTR,	   API_SQLGETCONNECTATTR			,TEXT("SQL_API_SQLGETCONNECTATTR"),				TEXT("SQLGetConnectAttr"),				TRUE,				IDD_SQLGETCONNECTATTR},
/*65*/	{SQL_API_SQLGETDESCFIELD,	   	API_SQLGETDESCFIELD				,TEXT("SQL_API_SQLGETDESCFIELD"),				TEXT("SQLGetDescField"),				TRUE,				IDD_SQLGETDESCFIELD},
/*66*/	{SQL_API_SQLGETDESCREC,		   	API_SQLGETDESCREC					,TEXT("SQL_API_SQLGETDESCREC"),					TEXT("SQLGetDescRec"),					TRUE,				IDD_SQLGETDESCREC},
/*67*/	{SQL_API_SQLGETDIAGFIELD,	   	API_SQLGETDIAGFIELD				,TEXT("SQL_API_SQLGETDIAGFIELD"),				TEXT("SQLGetDiagField"),				TRUE,				IDD_SQLGETDIAGFIELD},
/*68*/	{SQL_API_SQLGETDIAGREC,		   	API_SQLGETDIAGREC					,TEXT("SQL_API_SQLGETDIAGREC"),					TEXT("SQLGetDiagRec"),					TRUE,				IDD_SQLGETDIAGREC},
/*69*/	{SQL_API_SQLGETENVATTR,		   	API_SQLGETENVATTR					,TEXT("SQL_API_SQLGETENVATTR"),					TEXT("SQLGetEnvAttr"),					TRUE,				IDD_SQLGETENVATTR},
/*70*/	{SQL_API_SQLGETSTMTATTR,	   	API_SQLGETSTMTATTR				,TEXT("SQL_API_SQLGETSTMTATTR"),					TEXT("SQLGetStmtAttr"),					TRUE,				IDD_SQLGETSTMTATTR},
/*71*/	{SQL_API_SQLSETCONNECTATTR,	   API_SQLSETCONNECTATTR			,TEXT("SQL_API_SQLSETCONNECTATTR"),				TEXT("SQLSetConnectAttr"),				TRUE,				IDD_SQLSETCONNECTATTR},
/*72*/	{SQL_API_SQLSETDESCFIELD,	   	API_SQLSETDESCFIELD				,TEXT("SQL_API_SQLSETDESCFIELD"),				TEXT("SQLSetDescField"),				TRUE,				IDD_SQLSETDESCFIELD},
/*73*/	{SQL_API_SQLSETDESCREC,		   	API_SQLSETDESCREC					,TEXT("SQL_API_SQLSETDESCREC"),					TEXT("SQLSetDescRec"),					TRUE,				IDD_SQLSETDESCREC},
/*74*/	{SQL_API_SQLSETENVATTR,		   	API_SQLSETENVATTR					,TEXT("SQL_API_SQLSETENVATTR"),					TEXT("SQLSetEnvAttr"),					TRUE,				IDD_SQLSETENVATTR},
/*75*/	{SQL_API_SQLSETSTMTATTR,	   	API_SQLSETSTMTATTR				,TEXT("SQL_API_SQLSETSTMTATTR"),					TEXT("SQLSetStmtAttr"),					TRUE,				IDD_SQLSETSTMTATTR},
/*76*/	{SQL_API_SQLBULKOPERATIONS,	  	API_SQLBULKOPERATIONS			,TEXT("SQL_API_SQLBULKOPERATIONS"),				TEXT("SQLBulkOperations"),				TRUE,          IDD_SQLBULKOPERATIONS},
#ifdef SQL_API_SQLLOCATOR
/*77*/	{SQL_API_SQLLOCATOR,					API_SQLLOCATOR						,TEXT("SQL_API_SQLLOCATOR"),						TEXT("SQLLocator"),						TRUE,				IDD_SQLLOCATOR},
/*78*/	{SQL_API_SQLGETLENGTH,				API_SQLGETLENGTH					,TEXT("SQL_API_GETLENGTH"),						TEXT("SQLGetLength"),					TRUE,				IDD_SQLGETLENGTH},
/*79*/	{SQL_API_SQLGETPOSITION,			API_SQLGETPOSITION				,TEXT("SQL_API_GETPOSITION"),						TEXT("SQLGetPosition"),					TRUE,          IDD_SQLGETPOSITION},
/*80*/	{SQL_API_SQLGETSUBSTRING,			API_SQLGETSUBSTRING				,TEXT("SQL_API_SUBSTRING"),						TEXT("SQLGetSubString"),				TRUE,				IDD_SQLGETSUBSTRING},
/*81*/	{SQL_API_SQLLOCATEDUPDATE,			API_SQLLOCATEDUPDATE				,TEXT("SQL_API_LOCATEDUPDATE"),					TEXT("SQLLocatedUpdate"),				TRUE,          IDD_SQLLOCATEDUPDATE},
#else
/*77*/	{-1,										-1										,NULL									,					NULL,										FALSE,			-1},
/*78*/	{-1,										-1										,NULL									,					NULL,										FALSE,			-1},
/*79*/	{-1,										-1										,NULL									,					NULL,										FALSE,			-1},
/*80*/	{-1,										-1										,NULL									,					NULL,										FALSE,			-1},
/*81*/	{-1,										-1										,NULL									,					NULL,										FALSE,			-1},
#endif
/*82*/	{SQL_API_SQLTABLES,			   	API_SQLTABLES25					,TEXT("SQL_API_SQLTABLES"),						TEXT("SQLTables"),					FALSE,			IDD_SQLTABLES25},
/*83*/	{SQL_API_SQLCOLUMNS,		  			API_SQLCOLUMNS25					,TEXT("SQL_API_SQLCOLUMNS"),						TEXT("SQLColumns"),					FALSE,			IDD_SQLCOLUMNS25},
/*84*/	{SQL_API_SQLSTATISTICS,				API_SQLSTATISTICS25				,TEXT("SQL_API_SQLSTATISTICS"),					TEXT("SQLStatistics"),				FALSE,			IDD_SQLSTATISTICS25},
/*85*/	{SQL_API_SQLPRIMARYKEYS,	   	API_SQLPRIMARYKEYS25				,TEXT("SQL_API_SQLPRIMARYKEYS"),					TEXT("SQLPrimaryKeys"),				FALSE,			IDD_SQLPRIMARYKEYS25},
/*86*/	{SQL_API_SQLFOREIGNKEYS,	   	API_SQLFOREIGNKEYS25				,TEXT("SQL_API_SQLFOREIGNKEYS"),					TEXT("SQLForeignKeys"),				FALSE,			IDD_SQLFOREIGNKEYS25},
/*87*/	{SQL_API_SQLTABLEPRIVILEGES,   	API_SQLTABLEPRIVILEGES25		,TEXT("SQL_API_SQLTABLEPRIVILEGES"),			TEXT("SQLTablePrivileges"),		FALSE,			IDD_SQLTABLEPRIVILEGES25},
/*88*/	{SQL_API_SQLCOLUMNPRIVILEGES,  	API_SQLCOLUMNPRIVILEGES25		,TEXT("SQL_API_SQLCOLUMNPRIVILEGES"),			TEXT("SQLColumnPrivileges"),		FALSE,			IDD_SQLCOLUMNPRIVILEGES25},
/*89*/	{SQL_API_SQLSPECIALCOLUMNS,		API_SQLSPECIALCOLUMNS25			,TEXT("SQL_API_SQLSPECIALCOLUMNS"),				TEXT("SQLSpecialColumns"),			FALSE,			IDD_SQLSPECIALCOLUMNS25},
/*90*/	{SQL_API_SQLPROCEDURES,				API_SQLPROCEDURES25				,TEXT("SQL_API_SQLPROCEDURES"),					TEXT("SQLProcedures"),				FALSE,			IDD_SQLPROCEDURES25},
/*91*/	{SQL_API_SQLPROCEDURECOLUMNS,  	API_SQLPROCEDURECOLUMNS25		,TEXT("SQL_API_SQLPROCEDURECOLUMNS"),			TEXT("SQLProcedureColumns"),		FALSE,			IDD_SQLPROCEDURECOLUMNS25},
/*92*/	{IDM_SQLCONFIGDATASOURCE,			API_SQLCONFIGDATASOURCE			,TEXT("IDM_SQLCONFIGDATASOURCE"),			   TEXT("SQLConfigDataSource"),		FALSE,			IDD_SQLCONFIGDATASOURCE},
/*93*/	{IDM_SQLCONFIGDRIVER,				API_SQLCONFIGDRIVER				,TEXT("IDM_SQLCONFIGDRIVER"),						TEXT("SQLConfigDriver"),			FALSE,			IDD_SQLCONFIGDRIVER},
/*94*/	{IDM_SQLCREATEDATASOURCE,			API_SQLCREATEDATASOURCE			,TEXT("IDM_SQLCREATEDATASOURCE"),			   TEXT("SQLCreateDataSource"),		FALSE,			IDD_SQLCREATEDATASOURCE},
/*95*/	{IDM_SQLGETINSTALLEDDRIVERS,		API_SQLGETINSTALLEDDRIVERS		,TEXT("IDM_SQLGETINSTALLEDDRIVERS"),		   TEXT("SQLGetInstalledDrivers"),	FALSE,			IDD_SQLGETINSTALLEDDRIVERS},
/*96*/	{IDM_SQLGETPRIVATEPROFILESTRING,	API_SQLGETPRIVATEPROFILESTRING,TEXT("IDM_SQLGETPRIVATEPROFILESTRING"),		TEXT("SQLGetPrivateProfileString"),FALSE,			IDD_SQLGETPRIVATEPROFILESTRING},
/*97*/	{IDM_SQLINSTALLDRIVER,				API_SQLINSTALLDRIVER				,TEXT("IDM_SQLINSTALLDRIVER"),				   TEXT("SQLInstallDriver"),			FALSE,			IDD_SQLINSTALLDRIVER},
/*98*/	{IDM_SQLINSTALLDRIVERMANAGER,		API_SQLINSTALLDRIVERMANAGER	,TEXT("IDM_SQLINSTALLDRIVERMANAGER"),			TEXT("SQLInstallDriverManager"),	FALSE,			IDD_SQLINSTALLDRIVERMANAGER},
/*99*/	{IDM_SQLINSTALLTRANSLATOR,			API_SQLINSTALLTRANSLATOR		,TEXT("IDM_SQLINSTALLTRANSLATOR"),				TEXT("SQLInstallTranslator"),		FALSE,			IDD_SQLINSTALLTRANSLATOR},
/*100*/	{IDM_SQLINSTALLTRANSLATOREX,		API_SQLINSTALLTRANSLATOREX		,TEXT("IDM_SQLINSTALLTRANSLATOREX"),			TEXT("SQLInstallTranslatorEx"),	FALSE,			IDD_SQLINSTALLTRANSLATOREX},
/*101*/	{IDM_SQLMANAGEDATASOURCES,			API_SQLMANAGEDATASOURCES		,TEXT("IDM_SQLMANAGEDATASOURCES"),				TEXT("SQLManageDataSources"),		FALSE,			IDD_SQLMANAGEDATASOURCES},
/*102*/	{IDM_SQLREMOVEDEFAULTDATASOURCE,	API_SQLREMOVEDEFAULTDATASOURCE,TEXT("IDM_SQLREMOVEDEFAULTDATASOURCE"),		TEXT("SQLRemoveDefaultDataSource"),FALSE,		   IDD_SQLREMOVEDEFAULTDATASOURCE},
/*103*/	{IDM_SQLREMOVEDSNFROMINI,			API_SQLREMOVEDSNFROMINI			,TEXT("IDM_SQLREMOVEDSNFROMINI"),			   TEXT("SQLRemoveDSNFromIni"),		FALSE,			IDD_SQLREMOVEDSNFROMINI},
/*104*/	{IDM_SQLREMOVEDRIVER,				API_SQLREMOVEDRIVER				,TEXT("IDM_SQLREMOVEDRIVER"),						TEXT("SQLRemoveDriver"),			FALSE,			IDD_SQLREMOVEDRIVER},
/*105*/	{IDM_SQLREMOVEDRIVERMANAGER,		API_SQLREMOVEDRIVERMANAGER		,TEXT("IDM_SQLREMOVEDRIVERMANAGER"),		   TEXT("SQLRemovedDriverManager"),	FALSE,			IDD_SQLREMOVEDRIVERMANAGER},
/*106*/	{IDM_SQLREMOVETRANSLATOR,			API_SQLREMOVETRANSLATOR			,TEXT("IDM_SQLREMOVETRANSLATOR"),			   TEXT("SQLRemoveTranslator"),		FALSE,			IDD_SQLREMOVETRANSLATOR},
/*107*/	{IDM_SQLVALIDDSN,						API_SQLVALIDDSN					,TEXT("IDM_SQLVALIDDSN"),						   TEXT("SQLValidDSN"),					FALSE,			IDD_SQLVALIDDSN},
/*108*/	{IDM_SQLWRITEDSNTOINI,				API_SQLWRITEDSNTOINI				,TEXT("IDM_SQLWRITEDSNTOINI"),				   TEXT("SQLWriteDSNToIni"),			FALSE,			IDD_SQLWRITEDSNTOINI},
/*109*/	{IDM_SQLWRITEPRIVATEPROFILESTRING,API_SQLWRITEPRIVATEPROFILESTRING,TEXT("IDM_SQLWRITEPRIVATEPROFILESTRING"),TEXT("SQLWritePrivateProfileString"),FALSE,		IDD_SQLWRITEPRIVATEPROFILESTRING},
/*110*/	{IDM_SQLGETTRANSLATOR,				API_SQLGETTRANSLATOR				,TEXT("IDM_SQLGETTRANSLATOR"),				   TEXT("SQLGetTranslator"),			FALSE,			IDD_SQLGETTRANSLATOR},
/*111*/	{IDM_FILL_PARAM,						API_FILL_PARAM						,TEXT("IDM_FILL_PARAM"),							TEXT("FillParam Tool"),				FALSE,			IDD_FILLPARAM},
/*112*/	{IDM_SQLINSTALLERERROR,				API_SQLINSTALLERERROR			,TEXT("IDM_SQLINSTALLERERROR"),					TEXT("SQLInstallerError"),			TRUE,				IDD_SQLINSTALLERERROR},
/*113*/	{IDM_SQLINSTALLDRIVEREX,			API_SQLINSTALLDRIVEREX			,TEXT("IDM_SQLINSTALLDRIVEREX"),					TEXT("SQLInstallDriverEx"),		TRUE,				IDD_SQLINSTALLDRIVEREX},
/*114*/	{IDM_SQLPOSTINSTALLERERROR,		API_SQLPOSTINSTALLERERROR		,TEXT("IDM_SQLPOSTINSTALLERERROR"),				TEXT("SQLPostInstallerError"),	TRUE,				IDD_SQLPOSTINSTALLERERROR},
/*115*/	{IDM_SQLREADFILEDSN,					API_SQLREADFILEDSN				,TEXT("IDM_SQLREADFILEDSN"),						TEXT("SQLReadFileDSN"),				TRUE,				IDD_SQLREADFILEDSN},
/*116*/	{IDM_SQLWRITEFILEDSN,				API_SQLWRITEFILEDSN				,TEXT("IDM_SQLWRITEFILEDSN"),						TEXT("SQLWriteFileDSN"),			TRUE,				IDD_SQLWRITEFILEDSN},
/*117*/	{IDM_SQLGETCONFIGMODE,				API_SQLGETCONFIGMODE				,TEXT("IDM_SQLGETCONFIGMODE"),					TEXT("SQLGetConfigMode"),			TRUE,				IDD_SQLGETCONFIGMODE},
/*118*/	{IDM_SQLSETCONFIGMODE,				API_SQLSETCONFIGMODE				,TEXT("IDM_SQLSETCONFIGMODE"),					TEXT("SQLSetConfigMode"),			TRUE,				IDD_SQLSETCONFIGMODE},
/*119*/ {SQL_API_SQLNEXTCOLUMN,             API_SQLNEXTCOLUMN                   ,TEXT("SQL_API_SQLNEXTCOLUMN"),                 TEXT("SQLNextColumn"),              FALSE,              IDD_SQLNEXTCOLUMN },
/*120*/ {SQL_API_SQLGETNESTEDHANDLE,		API_SQLGETNESTEDHANDLE				,TEXT("SQL_API_SQLGETNESTEDHANDLE"),			TEXT("SQLGetNestedHandle"),			FALSE,				IDD_SQLGETNESTEDHANDLE },
/*121*/ {SQL_API_SQLSTRUCTUREDTYPES,        API_SQLSTRUCTUREDTYPES              ,TEXT("SQL_API_SQLSTRUCTUREDTYPES"),            TEXT("SQLStructuredTypes"),      FALSE,              IDD_SQLSTRUCTUREDTYPES },
/*122*/ {SQL_API_SQLSTRUCTUREDTYPECOLUMNS,	API_SQLSTRUCTUREDTYPECOLUMNS		,TEXT("SQL_API_SQLSTRUCTUREDTYPECOLUMNS"),		TEXT("SQLStructuredTypeColumns"),FALSE,				IDD_SQLSTRUCTUREDTYPECOLUMNS },
};



//*---------------------------------------------------------------------------------
//| GetApiConstant:
//|	Given the SQL_APIxxx number, return the character name of the API
//|	(i.e. "SQL_APIxxx")
//|	in the user's buffer.  User's buffer must be large enough.  This function
//|	provides an abstraction of the source of the name, rather than accessing
//|	the structure directly.
//| Parms:
//|	dex						Index of the function
//|	str						User's buffer, if NULL return pointer to our memory
//| Returns:
//|	Pointer to user's buffer
//*---------------------------------------------------------------------------------
LPTSTR INTFUN GetApiConstant(UINT dex, LPTSTR str)
{
	UINT uAPIs=NumItems(rgApiList);

	if(dex > uAPIs)
		return NULL;
	else {
        if(str) {
		    lstrcpy(str, rgApiList[dex].szAPIName);
            return str;
        } else {
		    return rgApiList[dex].szName;
        }
    }
}

//*--------------------------------------------------------------------------------
//
//		ADDED HERE FOR WIN95's LACK OF UNICODE SUPPORT
//
//
///
//*-----------------------------------------------------------------------------

#undef MAXOUTPUTBUFF
#define MAXOUTPUTBUFF 1024
/*
UINT GtrGetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName, INT nDefault,LPCTSTR lpFileName)
{

#ifdef _UNICODE

	CHAR	szAppNameA[MAXOUTPUTBUFF] = "";
	CHAR	szKeyNameA[MAXOUTPUTBUFF] = "";
	CHAR	szFileNameA[MAXOUTPUTBUFF] = "";
	LPSTR	lpAppNameA,lpKeyNameA,lpFileNameA;




	if (lpAppName)
	{
		lpAppNameA = szAppNameA;
		WideCharToMultiByte(CP_ACP,0,lpAppName,-1,lpAppNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpAppNameA = NULL;
	if (lpKeyName)
	{
		lpKeyNameA = szKeyNameA;
		WideCharToMultiByte(CP_ACP,0,lpKeyName,-1,lpKeyNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpKeyNameA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;


	return GetPrivateProfileIntA(lpAppNameA,lpKeyNameA,nDefault,lpFileNameA);
#else

	return GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,lpFileName);

#endif
}

/*DWORD GtrGetPrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault,
							 LPTSTR lpReturnedString, DWORD nSize,LPCTSTR lpFileName)
{
	DWORD	dwRetValue=0;
#ifdef _UNICODE

	CHAR	szAppNameA[MAXOUTPUTBUFF] = "";
	CHAR	szKeyNameA[MAXOUTPUTBUFF] = "";
	CHAR	szFileNameA[MAXOUTPUTBUFF] = "";
	CHAR	szDefaultA[MAXOUTPUTBUFF] = "";
	CHAR	szReturnedStringA[MAXOUTPUTBUFF];
	UINT size =0;

	LPSTR	lpAppNameA,lpKeyNameA,lpFileNameA,lpDefaultA,lpTemp;


	memset(szReturnedStringA,0,nSize);
	if (lpAppName)
	{
		lpAppNameA = szAppNameA;
		WideCharToMultiByte(CP_ACP,0,lpAppName,-1,lpAppNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpAppNameA = NULL;
	if (lpKeyName)
	{
		lpKeyNameA = szKeyNameA;
		WideCharToMultiByte(CP_ACP,0,lpKeyName,-1,lpKeyNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpKeyNameA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;
	if(lpDefault)
	{
		lpDefaultA = szDefaultA;
		WideCharToMultiByte(CP_ACP,0,lpDefault,-1,lpDefaultA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpDefaultA = NULL;


	dwRetValue = GetPrivateProfileStringA(lpAppNameA,lpKeyNameA,lpDefaultA,
					szReturnedStringA,nSize,lpFileNameA);



	if (szReturnedStringA)
	{
		lpTemp = szReturnedStringA;
		while ( *lpTemp)
		{
			size += strlen(lpTemp) + 1;
			lpTemp += strlen(lpTemp) + 1;
		}
		MultiByteToWideChar(CP_ACP,0,szReturnedStringA,size,lpReturnedString,nSize);
	}
#else

	dwRetValue = GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,
					lpReturnedString,nSize,lpFileName);

#endif

	return dwRetValue;
}
/*
BOOL GtrWritePrivateProfileString(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpString,
							   LPCTSTR lpFileName)
{
#ifdef _UNICODE

	CHAR	szAppNameA[MAXOUTPUTBUFF] = "";
	CHAR	szKeyNameA[MAXOUTPUTBUFF] = "";
	CHAR	szFileNameA[MAXOUTPUTBUFF] = "";
	CHAR	szStringA[MAXOUTPUTBUFF] = "";
	BOOL	bRetValue=0;
	LPSTR	lpAppNameA,lpKeyNameA,lpFileNameA,lpStringA;


	if (lpAppName)
	{
		lpAppNameA = szAppNameA;
		WideCharToMultiByte(CP_ACP,0,lpAppName,-1,lpAppNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpAppNameA = NULL;
	if (lpKeyName)
	{
		lpKeyNameA = szKeyNameA;
		WideCharToMultiByte(CP_ACP,0,lpKeyName,-1,lpKeyNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpKeyNameA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpFileNameA = NULL;
	if(lpString)
	{
		lpStringA = szStringA;
		WideCharToMultiByte(CP_ACP,0,lpString,-1,lpStringA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpStringA = NULL;

	return  WritePrivateProfileStringA(lpAppNameA,lpKeyNameA,
					lpStringA,lpFileNameA);
#else
	return  WritePrivateProfileString(lpAppName,lpKeyName,
					lpString,lpFileName);
#endif

}

/*int GtrLoadString(HINSTANCE hInstance,UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
	int	 nRetValue;

#ifdef _UNICODE

	CHAR szBufferA[MAXOUTPUTBUFF];

	LPSTR lpBufferA;

	if (lpBuffer)
	{
		lpBufferA = szBufferA;
		WideCharToMultiByte(CP_ACP,0,lpBuffer,-1,lpBufferA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpBufferA = NULL;


	nRetValue = LoadStringA(hInstance , uID, lpBufferA,MAXOUTPUTBUFF);

	MultiByteToWideChar(CP_ACP,0,lpBufferA,-1,lpBuffer,nBufferMax);

#else
	nRetValue = LoadString(hInstance , uID, lpBuffer,nBufferMax);
#endif

	return nRetValue;
}

/*DWORD GtrFormatMessage(DWORD dwFlags,LPCVOID lpSource, DWORD dwMessageid, DWORD dwLanguageid,
					LPTSTR lpBuffer,DWORD nSize,va_list * Arguments)
{
	DWORD dwRetValue=0;
#ifdef _UNICODE

	//CHAR szSourceA[MAXOUTPUTBUFF];
	CHAR szBufferA[MAXOUTPUTBUFF];
	//LPSTR lpSourceA,lpBufferA;


	/*if (lpSource)
	{
		lpSourceA = szSourceA;
		WideCharToMultiByte(CP_ACP,0,lpSource,-1,lpSourceA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpSourceA = NULL;

	dwRetValue = FormatMessageA(dwFlags,lpSource,dwMessageid,dwLanguageid,szBufferA,
					MAXOUTPUTBUFF,Arguments);

	MultiByteToWideChar(CP_ACP,0,szBufferA,-1,lpBuffer,nSize);
#else
	dwRetValue = FormatMessage(dwFlags,lpSource,dwMessageid,dwLanguageid,lpBuffer,
					nSize,Arguments);
#endif

	return dwRetValue;
}

/*HANDLE GtrCreateFile(LPCTSTR lpFileName,DWORD dwDesiredAccess,DWORD dwSharedMode,LPSECURITY_ATTRIBUTES
				  lpSecurityAttributes,DWORD dwCreationDistribution,DWORD FlagsAndAttributes,
				  HANDLE hTemplateFile)
{

#ifdef _UNICODE

	CHAR szFileNameA[_MAX_PATH];
	LPSTR lpFileNameA= NULL;

	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,_MAX_PATH,NULL,NULL);
	}

	return CreateFileA( lpFileNameA,
						dwDesiredAccess,
						dwSharedMode,
						lpSecurityAttributes,
						dwCreationDistribution,
						FlagsAndAttributes,
						 hTemplateFile);
#else
	return CreateFile( lpFileName,
						dwDesiredAccess,
						dwSharedMode,
						lpSecurityAttributes,
						dwCreationDistribution,
						FlagsAndAttributes,
						 hTemplateFile);
#endif

}

/*HINSTANCE GtrLoadLibrary( LPCTSTR lpLibFileName)
{
#ifdef _UNICODE

	CHAR szLibFileNameA[_MAX_PATH];
	LPSTR	lpLibFileNameA;

	if(lpLibFileName)
	{
		lpLibFileNameA = szLibFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpLibFileName,-1,lpLibFileNameA,MAXOUTPUTBUFF,NULL,NULL);
	}
	else
		lpLibFileNameA = NULL;

	return LoadLibraryA(lpLibFileNameA);
#else
	return LoadLibrary(lpLibFileName);
#endif
}

/*DWORD GtrSearchPath(LPCTSTR lpPath,LPCTSTR lpFileName,LPCTSTR lpExtensions, DWORD nBufferLength,
				 LPTSTR lpBuffer,LPTSTR *lpFilePart)
{
	DWORD	dwRetValue=0;
#ifdef _UNICODE

	CHAR	szPathA[_MAX_PATH];
	CHAR	szFileNameA[_MAX_PATH];
	CHAR	szExtensionsA[_MAX_PATH];
	CHAR	szBufferA[_MAX_PATH];
	CHAR	szFilePartA[_MAX_PATH];
	LPSTR	lpPathA,lpFileNameA,lpExtensionsA,lpFilePartA;
	LPSTR*	lppFilePartA;



	if (lpPath)
	{
		lpPathA = szPathA;
		WideCharToMultiByte(CP_ACP,0,lpPath,-1,lpPathA,_MAX_PATH,NULL,NULL);
	}
	else
		lpPathA = NULL;
	if (lpFileName)
	{
		lpFileNameA = szFileNameA;
		WideCharToMultiByte(CP_ACP,0,lpFileName,-1,lpFileNameA,_MAX_PATH,NULL,NULL);
	}
	else
		lpFileNameA = NULL;
	if (lpExtensions)
	{
		lpExtensionsA = szExtensionsA;
		WideCharToMultiByte(CP_ACP,0,lpExtensions,-1,lpExtensionsA,_MAX_PATH,NULL,NULL);
	}
	else
		lpExtensionsA = NULL;
	if (lpFilePart && *lpFilePart)
	{
		lpFilePartA = szFilePartA;
		WideCharToMultiByte(CP_ACP,0,*lpFilePart,-1,lpFilePartA,_MAX_PATH,NULL,NULL);
		lppFilePartA = &lpFilePartA;
	}
	else
		lppFilePartA = NULL;

	dwRetValue = SearchPathA(lpPathA,						// address of search path
							lpFileNameA,					// address of filename
							lpExtensionsA,						// address of extension
							nBufferLength,				// size, in characters, of buffer
							szBufferA,				// address of buffer for found filename
							lppFilePartA);

	MultiByteToWideChar(CP_ACP,0,szBufferA,-1,lpBuffer,_MAX_PATH);
#else
	dwRetValue = SearchPath(lpPath,						// address of search path
							lpFileName,					// address of filename
							lpExtensions,						// address of extension
							nBufferLength,				// size, in characters, of buffer
							lpBuffer,				// address of buffer for found filename
							lpFilePart);
#endif

	return dwRetValue;
}

/*int GtrMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
#ifdef _UNICODE
	CHAR	szTextA[_MAX_PATH];
	CHAR	szCaptionA[_MAX_PATH];
	LPSTR	lpTextA,lpCaptionA;

	if (lpText)
	{
		lpTextA = szTextA;
		WideCharToMultiByte(CP_ACP,0,lpText,-1,lpTextA,_MAX_PATH,NULL,NULL);
	}
	else
		lpTextA = NULL;
	if (lpCaption)
	{
		lpCaptionA = szCaptionA;
		WideCharToMultiByte(CP_ACP,0,lpCaption,-1,lpCaptionA,_MAX_PATH,NULL,NULL);
	}
	else
		lpCaptionA = NULL;

	return MessageBoxA(hWnd,lpTextA,lpCaptionA,uType);
#else
	return MessageBox(hWnd,lpText,lpCaption,uType);
#endif
}

BOOL GatorSetWindowText(HWND hWnd,LPCTSTR lpString)
{
#ifdef _UNICODE

	CHAR szStringA[MAXOUTPUTBUFF];
	LPSTR lpStringA;

	if (lpString)
	{
		lpStringA = szStringA;
		WideCharToMultiByte(CP_ACP,0,lpString,-1,lpStringA,_MAX_PATH,NULL,NULL);
	}
	else
		lpStringA = NULL;


	return SetWindowTextA(hWnd,lpStringA);
#else
	return SetWindowText(hWnd,lpString);
#endif
}*/

/*int _openA(const wchar_t *filename,int oflag,int pmode)
{

	char filenameA[_MAX_PATH];
#ifdef _UNICODE


	WideCharToMultiByte(CP_ACP,0,filename,-1,filenameA,_MAX_PATH,NULL,NULL);
#else
	strcpy(filenameA,(char *)filename);
#endif

	return _open(filenameA,oflag,pmode);
}*/

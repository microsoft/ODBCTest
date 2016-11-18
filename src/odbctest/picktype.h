//*---------------------------------------------------------------------------------
//|  Title:		PICKTYPES.H
//|		Copyright (c) Microsoft Corporation
//|
//|	This file contains defines and macros which allow
//|	the user to add and remove groups SQL Types from the "AutoMakeTable"
//*---------------------------------------------------------------------------------
#ifndef PICKTYPES_DEFS
#define PICKTYPES_DEFS


//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define USER_RESET_TYPES				WM_USER + 150
#define USER_SET_BUTTON_STATE			WM_USER + 151
#define USER_RESET_INSTALLED			WM_USER + 152
#define USER_RESET_AVAILABLE			WM_USER + 153
#define USER_ADD_ITEM 					WM_USER + 154
#define USER_REMOVE_ITEM				WM_USER + 155
#define USER_RESET_RUNLISTS			WM_USER + 156

#define AUTO_MAX_TEST_NAME          35
#define AUTO_MAX_TESTCASE_NAME      35
#define AUTO_MAX_TESTDESC_NAME      75
#define MAXTESTDESC	   				  70
#define MAXTESTSOURCE					  50
#define MAXLISTBOXSIZE 					 100

//------------------------------------------------------------------------------------------
// .INI macros
//------------------------------------------------------------------------------------------
#define INI_STRING_NOT_FOUND		0
#define INI_DATA_SOURCE				1
#define INI_AUTO_TEST				2
#define INI_GROUP_NAME				4


// For Types
#define GetTypeList(outbuff, cboutbuff)															\
	(GtrGetPrivateProfileString(szTYPES, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define GetTypeAutoTests(group, outbuff, cboutbuff)											\
	(GtrGetPrivateProfileString(group, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define GetTypeNameFromIni(group, outbuff, cboutbuff)										\
	(GtrGetPrivateProfileString(szTYPES, group, NULL, outbuff, cboutbuff, szGATORINI))
#define AddTypeToIni(group)																			\
	(GtrWritePrivateProfileString(szTYPES, group, szINSTALLED, szGATORINI))
#define AddTypeAutoTest(group, autotest)															\
	(GtrWritePrivateProfileString(group, autotest, szINSTALLED, szGATORINI))
#define RemoveTypeAutoTest(group, autotest)														\
	(GtrWritePrivateProfileString(group, autotest, NULL, szGATORINI))
#define RemoveTypeFromIni(group)																	\
	(GtrWritePrivateProfileString(szTYPES, group, NULL, szGATORINI))
#define RemoveType(group)																				\
	(GtrWritePrivateProfileString(group, NULL, NULL, szGATORINI))
#define GetInstalledTestList(outbuff, cboutbuff)												\
	(GtrGetPrivateProfileString(szAUTOTESTS, NULL, NULL, outbuff, cboutbuff, szGATORINI))
#define GetSourceName(name, outbuff, cboutbuff)                                     \
	(GtrGetPrivateProfileString(szSQLDRIVERS, name, NULL, outbuff, cboutbuff, szGATORINI))
#define GetTestNameFromIni(test, outbuff, cboutbuff)											\
	(GtrGetPrivateProfileString(szAUTOTESTS, test, NULL, outbuff, cboutbuff, szGATORINI))

//----------------------------------------------------------------------------------
//		Functions
//----------------------------------------------------------------------------------
int EXTFUN PickTypes(HWND hwnd, HINSTANCE hInst,lpFIELDINFO rgFields);

#endif

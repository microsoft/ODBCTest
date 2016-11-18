//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	MENUS.H
//|
//| Purpose:
//|		This header file contains the menu ids.  Note that the
//|		SQL function values are based so that we may hash to their
//|		control information quickly.
//|
//|	Menu ranges:
//|			101 to 299		Base menu items, not include ODBC API functions
//|			300 to 399		ODBC API functions (hashed based on SQL_API_xxx
//|			400 to 450		MDI Child windows, created and handled by Windows
//*---------------------------------------------------------------------------------
#ifndef MENUS_DEFS
#define MENUS_DEFS


//
// For lack of a better place, here are the constants used for the icons
//	Note that Win32s and Chicago use the first icon found as the default as
//	opposed to Win 16 and NT which used the first registered.  For this
// reason, always leave GATORICON as the first icon
//
#define GATORICON					700
#define CONNECTICON				701
#define RESULTICON				702
#define OUTWINICON				703


// Normally menu numbers start at 0 and go up, which is how each offset is shown
//		in this file.  If a child MDI window is zoomed, however, then the child
//		system menu becomes menu number 0 and others are offset.  The following
//		macro will look at the current active window and return the correct
//		0 based menu number.
#define GETMENUNUM(iMenu) (iMenu - (int)IsZoomed(hwndCurMDIChild))



#define IDM_SYSTEM_CLOSE_POS		  6
#define USER_MOD_MDI_CHILD_MENU  (WM_USER + 301)

#define IDM_LOWEST_MENU				API_SQLALLOCCONNECT
#define IDM_HIGHEST_MENU			NUM_ODBC_APIS

#define MENU25									1001
#define MENU30									1002
#define MENUALL								1003


// Following is accelerator for the F6=Move window focus key
#define IDM_MOVE_WINDOW						250

// File menu IDs
#define IDM_FILE_MENU						  0
#define IDM_FILE_OPEN						251
#define IDM_FILE_SAVEAS						252
#define IDM_FILE_USEROPTIONS				253
#define IDM_FILE_EXIT						254


// Edit menu IDs
#define IDM_EDIT_MENU						  1
#define IDM_EDIT_UNDO						255
#define IDM_EDIT_CUT							256
#define IDM_EDIT_COPY						257
#define IDM_EDIT_PASTE						258
#define IDM_EDIT_DELETE 					259
#define IDM_EDIT_CLEAR						260
#define IDM_EDIT_SELECT_ALL				261
#define IDM_EDIT_WRAP						262

#define IDM_DIAG_MENU						2

// Connect menu IDs
//
#define IDM_ENV_MENU							3


// Connect menu IDs
//
#define IDM_CONNECT_MENU					4
#define IDM_FULL_CONNECT					3000
#define IDM_FULL_DISCONNECT 				3001
#define IDM_DATA_SOURCES_ALL				3002
#define IDM_GET_INFO_ALL					3003
#define IDM_DRIVERS_ALL						3004
#define IDM_CONNECT_SET_DRIVER_DEP_MENUS	3005
#define IDM_ALL_ERRORS						3006
#define IDM_GET_FUNCTIONS_ALL				3007
#define IDM_GET_DESCRIPTOR_ALL			3008
#define IDM_LOAD_DESCRIPTOR_DEFAULTS 	3009
#define IDM_CLEAR_ERROR_QUEUE				3010

#define IDM_DESC_MENU						5

// Statement menu
//
#define IDM_STATEMENT_MENU					6
#define IDM_FILL_PARAM						3100
#define IDM_SHOW_PARAMS 					3101
#define IDM_SHOW_CURSOR_SETTINGS			3102
//-- Following not really on menu, but in results win child system menu
#define IDM_FREESTMT_CLOSE					3103
#define IDM_MAKE_TABLE						3104

#define IDM_ATTR_MENU						7
#define IDM_SET_CURSOR_ATTR				3150

// Results Menu
//
#define IDM_RESULTS_MENU					8
#define IDM_DESCRIBE_COL_ALL				3200
#define IDM_BIND_COL_ALL					3201
#define IDM_SHOW_BOUND_COLS 				3202
#define IDM_FETCH_ALL						3203
#define IDM_GET_DATA_ROW					3204
#define IDM_GET_DATA_ALL					3205
#define IDM_UPDATE_CELL						3206
#define IDM_DISPLAY_ROWSET             3207

// Catalog Menu
//
#define IDM_CATALOG_MENU					9


// Misc Menu
//
#define IDM_INSTALLER_MENU					10

// Tools Menu
//
#define IDM_TOOLS_MENU						11
// Other
#define IDM_TOOLS_USER_OPTIONS			3402
#define IDM_USER_ROWSET_OPTIONS			3403
#define IDM_TRACE								3404
//#define IDM_ASYNC							3405
#define IDM_TOOLS_MANAGE_SOURCES			3406
#define IDM_TOOLS_MANAGE_TESTS			3407
#define IDM_TOOLS_MANAGE_GROUPS			3408
#define IDM_TOOLS_RUN_AUTO_TESTS			3409
#define IDM_TOOLS_RECORD					3410
#define IDM_TOOLS_PLAYBACK					3411

// Window menu
#define IDM_WINDOW_MENU						12
#define IDM_CASCADE							3500
#define IDM_TILE								3501
#define IDM_TILE_HORZ						3502
#define IDM_ARRANGE							3503
#define IDM_CLOSEALL							3504

// Help menu
#define IDM_HELP_MENU						13
#define IDM_TEST_HELP						3600
#define IDM_API_HELP							3601
#define IDM_ABOUT								3602




// Toolbar menu-type item
#define IDM_BIND_COL_EXT_FETCH			499
#define IDM_FETCH_FIRST						498
#define IDM_FETCH_PRIOR						497
#define IDM_FETCH_NEXT						496
#define IDM_FETCH_LAST						495
#define IDM_FREE_STMT_CLOSE				494

#define NUM_MENUS								(IDM_HELP_MENU + 1)
#define MENU_MDI_BEGIN						500
#define MENU_MDI_END							550
#define MENU_POPUP_HELP_BEGIN				(IDM_HELP_MENU + 10)


//
// The SQL_API number found in sqlext.h is used to define a menu number
//		for each of the menu items which are in the API.  This way the
//		menu number serves as a hash to the function to be executed
//
#define IDM_API_BASE							300
#define IDM_CORE_FUNCS						300
#define IDM_EXT_FUNCS						300
#define INSTALLER_ADDITIONS				 24
#define TOOL_ADDITIONS						  2 //+INSTALLER_ADDITIONS
#define NUM_API_ITEMS						(SQL_NUM_FUNCTIONS + SQL_NUM_EXTENSIONS + TOOL_ADDITIONS)



//----------------------------------------------------------------------------------
//		Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN DoMenuSelect(UINT uMenu, UINT uFlags, HMENU hMenu);
VOID INTFUN ResetMenuStatus(HMENU hMenu, int iMenu, int iCalledFrom);



#endif


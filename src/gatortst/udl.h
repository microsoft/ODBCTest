//*-----------------------------------------------------------------------
//| Copyright (c) Microsoft Corporation
//|	This include file contains the constants required for the control
//|		dialog of the ODBC Translate tool.
//*-----------------------------------------------------------------------
#ifndef UDL_DEFS
#define UDL_DEFS

//------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------
#define GROUP_LEVEL		0
#define AUTOT_LEVEL		1
#define TESTC_LEVEL		2

#define IDBT_PLUS					900
#define IDBT_MINUS				901

#define IDBT_GROUP				905
#define IDBT_AUTOT				906
#define IDBT_TESTC				907

#define UDL_SELCHANGE			(WM_USER + 1000)
#define UDL_EXPAND				(WM_USER + 1001)
#define UDL_SETTOP				(WM_USER + 1002)
#define UDL_LINEUP				(WM_USER + 1003)
#define UDL_LINEDOWN				(WM_USER + 1004)
#define UDL_PAGEUP				(WM_USER + 1005)
#define UDL_PAGEDOWN				(WM_USER + 1006)

#define UDL_SELECTION_CHANGED (WM_USER + 1010)

#define GETUDLINFOBOX(hwnd) (lpUDLINFOBOX)GetWindowLongPtr(hwnd, 0)

#define DFTBITMAPWIDTH		14
#define DFTBITMAPHEIGHT		12
typedef unsigned long int UDWORD;


// Following structure used to track top level items, changes
// in selection status, and other items
#define UDL_GROUP						0
#define UDL_AUTOT						1
#define UDL_TESTC						2

#define UDL_UP							1
#define UDL_DOWN						2

#define UDL_NO_RELATIVE_SCROLL	((UINT)-1)

#define UDL_GROUP_ON_TOP			1
#define UDL_AUTOT_ON_TOP		   2
#define UDL_TESTC_ON_TOP			4
typedef struct tagLVLINDEX {
	UINT		fOpt;								// Bitmask for operation
	UINT		group;							// Group index
	UINT		autot;							// Auto test within group
	UINT		testc;							// Test within auto test
	} LVLINDEX;
typedef LVLINDEX * lpLVLINDEX;


// Typedef for the strings
typedef struct tagARRAYLPTSTR {
	LPTSTR			str;
	} ARRAYLPTSTR;
typedef ARRAYLPTSTR * lpARRAYLPTSTR;


// This structure is used to the attributes which all levels
// at a level have in common
typedef struct tagUDLLEVELDESC {
	UINT										fx;				// Free space width
	UINT										bx;				// Bitmap width
	UINT										by;				// Bitmap height
	UINT										ux;				// User text width (pixels)
	HBITMAP									hMap;				// Handle to bitmap for this level
	} UDLLEVELDESC;
typedef UDLLEVELDESC * lpUDLLEVELDESC;


// Information on an actual physical level including all user test and
// selection/expansion information. Next is an array of items below this level
typedef struct tagUDLINFOLEVEL {
	UINT										level;			// Which level is this (0 based)
	UINT										cItems;			// How many items at this level
	BOOL										fLoaded;			// TRUE if szItems have been set
	lpARRAYLPTSTR							szItems;			// Array of pointers to item data
	UINT *								aSelected;		// Bitmask of selected item
	UINT *								aPartial;		// Upper level partial select
	UINT *								aExpanded;		// Bitmask of expanded items
	UDWORD									cNextItems;		// How many items at next level
	UINT										pdex;				// Parent index
	struct tagUDLINFOLEVEL *		prev;				// Owner level, NULL for parent
	struct tagUDLINFOLEVEL * * 	next;				// Array of levels below this
	} UDLINFOLEVEL;
typedef UDLINFOLEVEL * lpUDLINFOLEVEL;

// Information about the box including everything required for drawing the
// elements and points to the correct structures
typedef struct tagUDLINFOBOX {
	HINSTANCE				hInst;			// Instance handle of this owner
	HWND						hwnd;				// Window handle of control
	HWND						hwndOwner;		// Owner window on create
	HWND						hwndScroll;		// Handle of scrollbar
	BOOL						fhwndScroll;	// TRUE if enabled
	HWND						hwndBox;			// Handle of client draw area
	UINT						cLevels;			// How many levels are there?
	UINT						dx;				// Width of hwndBox
	UINT						dy;				// Height of hwndBox
	UINT						ly;				// Line height max
	LVLINDEX					top;				// Top level information
	UDWORD					cVisible;		// Total rows that will fit in box
	UINT						cCurRelative;	// Current realtive row number
	UINT						cScrollPos;		// Relative to over-all scroll
	UINT						cRange;			// Range for scroll bar
	HFONT						hFont;			// The font to use for drawing
	UINT						cx;				// Average width of character
	UINT						cy;				// Height of average character
	UINT						cbExtra;			// White space to use for drawing
	HBITMAP					hPlus;			// More levels to be seen
	HBITMAP					hMinus;			// No more levels to be seen
	UINT						ex;				// Expansion indicator width
	UDWORD					cTotItems;		// Total items for all levels
	UDWORD					cTotVisible;	// Total visible items for all levels
	lpUDLLEVELDESC			lpdesc;			// Array of level descriptors
	lpUDLINFOLEVEL			lplvl;			// First level descriptor
	} UDLINFOBOX;
typedef UDLINFOBOX * lpUDLINFOBOX;

// Following structure is used to track the selected auto tests
typedef struct tagRUNTESTLIST {
	BOOL						fRun;								// TRUE if we should run
	BOOL						fAppend;							// TRUE if we should append to existing log files
	HINSTANCE				hInst;							// Instance handle of owner
	HWND						hRunDlg;							// Window handle of Run Auto Tests
	LPTSTR					szSource;						// Dbl-null list of sources
	TCHAR						szRunList[MAXFILELEN];		// Name of Run List we're using
	lpSERVERINFO			rs;								// Pointer to struct to file out
	lpAUTOTESTINFO			head;								// Auto test list for run-info
	lpUDLINFOBOX			lpbox;							// The user drawn list of groups
	} RUNTESTLIST;
typedef RUNTESTLIST *  lpRUNTESTLIST;




//------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------
BOOL UDLInit(HANDLE hInst);
UDLINFOBOX * EXTFUNCDECL UDLCreateBox(UINT cLevels, HWND hwnd, HINSTANCE hInst,
		int nLeft, int nTop, int dx, int dy);
VOID EXTFUN UDLFreeBox(lpUDLINFOBOX lpbox);
BOOL EXTFUN  UDLCreateLevelDesc(
				lpUDLINFOBOX 	lpbox,
				UINT				level,
				HINSTANCE		hInst,
				LPTSTR				szBit,
				UINT				bx,
				UINT				by);
void EXTFUN UDLFreeLevelDesc(lpUDLINFOBOX lpbox, UINT level);
UDLINFOLEVEL * EXTFUNCDECL UDLCreateLevel(lpUDLINFOLEVEL parent, UINT pdex,
		UINT cLevel, UINT cItems, BOOL fLastLevel);
VOID EXTFUN UDLFreeLevel(lpUDLINFOLEVEL lplvl);
BOOL EXTFUN UDLSetLevelText(lpUDLINFOLEVEL lplvl,
			UINT index,	LPTSTR szStr);
VOID EXTFUN UDLFreeLevelText(lpUDLINFOLEVEL lplvl);
UINT INTFUN UDLHowManySelections(lpUDLINFOLEVEL lplvl);
UINT INTFUN UDLHowManyPartials(lpUDLINFOLEVEL lplvl);

//long EXTFUN UDLWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
UDWORD INTFUN UDLVisibleItems(lpUDLINFOBOX lpbox);
VOID UDLAdjustScrollBar(lpUDLINFOBOX lpbox);
void INTFUN UDLAdjustPartial(lpUDLINFOLEVEL lplvl, UINT dex);


#endif

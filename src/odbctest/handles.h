//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	HANDLES.H
//|
//| Purpose:
//|		This header contains structure definitions for the main data
//|		structures used to track the program elements.  This includes
//|		tracking connections, statements, and results.
//*---------------------------------------------------------------------------------
#ifndef HANDLES_DEFS
#define HANDLES_DEFS

#include <windows.h>
#include "portable.h"
#include "standard.h"
#include "sql.h"
#include "sqlext.h"


//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------
#define DFTBUFFSIZE		40
#define MAXTITLE		  100
#define MAX_COL_NAME		40

#define STATE_ALLOCATED_HDBC		0x00000001L
#define STATE_BROWSING_HDBC		0x00000002L
#define STATE_CONNECTED_HDBC		0x00000004L
#define STATE_ALLOCATED_HSTMT		0x00000008L


//*------------------------------------------------------------------------
//| DESCRIPTORRECORD
//|	This structure contains information for one descriptor record.
//*------------------------------------------------------------------------
typedef struct tagDESCRIPTORRECORD {

	SWORD		swRecNum;				// Record number of this descriptor record

	BOOL		fAutoUniqueValue;		// IRD: TRUE if column is an auto-incrementing
											//		  other wise FALSE
	UWORD		uwBindOutOfLine;		// AD's:SQL_TRUE if fields are bound out of line,
											//			otherwise SQL_FALSE
	TCHAR    *pszBaseColName;		// IRD: base column name for the result set column
	BOOL		fCaseSensitive;		// IRD: TRUE if the column is treated as case-
											//		  sensitive for collations and comparison
	SWORD		swConciseType;			//
	TCHAR    *pszCatName;			// IRD: contains catalog or qualifier name for the
											//		  base table that contains the column
	PTR		pDataPtr;				// ARD: Column Value
											// APD: Parameter Value
	SWORD		swDTInterval;			// ALL: Date-Time interval
	SDWORD	sdDTIntPrec;			// ALL: Precision if the TYPE field is SQL_INTERVAL
	SDWORD	sdDisplaySize;			// IRD: Max. number of chars. required to display the
											//		  data from the column.
	BOOL		fFixedPrecScale;		// IRD: TRUE if column is an exact numeric column and
											//		  has a fixed precision and non-zero scale (such
											//		  as MONEY
	SWORD		*pswIndPtr;				// ARD: points to the indicator variable
	TCHAR    *pszLabel;				// IRD: contains column label and title
	UDWORD	udLength;				// ALL: max. or actual char. length of a char string
											//		  or a binary data type
	TCHAR    *pszLitPrefix;			// IRD: prefix for a literal of the datatype
	TCHAR    *pszLitSuffix;			// IRD: suffix for a literal of the datatype
	TCHAR    *pszLocTypeName;		// IRD: localizeed (native language) name for the
											//		  data type that may be different from the
											//		  regular name of the data type
	TCHAR    *pszName;				//	ALL: column alias
											// IPD: set to SQL_UNNAMED
	SWORD		swNullable;				// ARD: columns nullability
											// IPD: always SQL_NULLLABLE
	SDWORD	sdOctetLen;				// ALL: length in bytes of a character string or
											//		  binary data type.
	SDWORD	*psdOctLenPtr;			// PD:  total length in bytes of a dynamic argument
											// RD:  total length in bytes of a bound column
	SWORD		swParmType;				// PD:  SQL_PARAM_INPUT,SQL_PARAM_OUTPUT,SQL_PARAM_INPUT_OUTPUT
	SWORD		swPrecision;			// ALL: precision for numeric data type
	SDWORD	*psdPredOctLenPtr;	// ARD: points to the number of bytes in the buffer pointed
											//		  to by the PREDICATE_PTR record field
	TCHAR    *pszPredPtr;			// ARD: Predicate for column
	SWORD		swScale;					// ALL: Scale for DECIMAL and NUMERIC data types
	TCHAR    *pszSchemaName;		// IRD: schema name fo the bawe table that contains the column
	SWORD		swSearchable;			// IRD: SQL_PRED_NONE,SQL_PRED_CHAR,SQL_PRED_BASIC,
											//		  SQL_SEARCHABLE.
	TCHAR    *pszTableName;			// IRD: name of the base table
	SWORD		swType;					// ALL: Desc Type
	TCHAR    *pszTypeName;			// IRD: Data type name
	SWORD		fUnNamed;				// ALL: TRUE if SQL_NAMED otherwise FALSE
	SWORD		fUnsigned;				// IRD: TRUE if column type is unsigned or non-numeric
	SWORD		swUpdateable;			// IRD: SQL_ATTR_READ_ONLY, SQL_ATTR_WRITE,
											//		  SQL_ATTR_READWRITE_UNKNOWN

	struct tagDESCRIPTORRECORD *next;		// Next node in the list
	struct tagDESCRIPTORRECORD *prev;		// Previous node in the list

	} DESCRIPTORRECORD, * lpDESCRIPTORRECORD;

//Forward reference for STATEMENTINFO structure
struct tagSTATEMENTINFO;


//*------------------------------------------------------------------------
//| DESC_HEADER
//|	This structure contains Descriptor Header information from the
//|	Descriptor Table Header
//*------------------------------------------------------------------------
typedef struct tagDESCRIPTORHEADER {
	UWORD		uwDescType;				//Is this desc. an ARD,APD,IRD,IPD?

	UWORD		uwAllocType;			// Desc. allocated automatically or explicitly
	TCHAR		szName[MEDBUFF];		//User defined name for Descriptor

	UDWORD	udArraySize;			//ARD: # of rows in the rowset
											//APD: # of values for each parameter
	UWORD		*rgArrayStatusPtr;	//ARD: rgfRowStatus
											//APD: parameter status
	SDWORD	*psdBindOffsetPtr;	//ALL: bind offset to be added on to SQL_DESC_DATA_PTR
											//		 SQL_DESC_INDICATOR, and SQL_DESC_OCTET_LENGTH_PTR
											//		 (used for Quick rebinding)
	BOOL		fBindOutOfLine;		//ALL: True if app. must set the SQL_DESC_OUT_OF_LINE_HEAP
											//		 otherwise it will be false
	SDWORD	sdBindType;				//ARD: SQL_BIND_BY_COLUMN or sizeof of
											//		 rowwise structure for columns
											//APD: SQL_BIND_BY_COLUMN or sizeof of
											//		 rowwise structure for parameters
	SWORD		swCount;					//ALL: # of records that contain data
	PTR		pFetchBookMarkPtr;	//ARD: points to a bookmark values
	SWORD		swFetchLevel;			//ARD: Indicates the level of the result set that this
											//		 descriptor cooresponds to.
											//IRD: Indicates the level of rows that will be
											//		 available in  the next call to SQLFetchScroll()
	SQLHDESC	*rghDesc;				//IRD: pointer to an array of descriptor handles
	PTR		pOutOfLineHeapPtr;	//ARD: points to the starting location of out-of-line
											//		 heap that is to contain data for out-of-line
											//		 bindings.
	SDWORD	sdOOLHeapOctetLength;//ARD: number of bytes in the out-of-line heap
	SDWORD	*psdPredOctLenPtr;	//ARD: points to number of bytes in the buffer pointed
											//		 to by PREDICATE_PTR header field.
	TCHAR    *pszszPredPtr;			//ARD: points to predicate
	UDWORD	*pudRowsProcPtr;		//ARD: contains the number of rows fetched after a
											//		 call to SQLFetchScroll()
											//APD: contains the number of the row as each row
											//		 of parameters is processed
	SWORD		swTotalLevels;			//IRD: number of levels of result sets that is available.


	SQLHDESC		hdesc;				// Descriptor handle
	TCHAR			szHdesc[MEDBUFF];	// Descriptor handle in text form
	SWORD			uDescNum;			// Descriptor Number
	SWORD			cRecs;				// Number of row records associated with this descriptor
	lpDESCRIPTORRECORD	lpRec;		// Linked list of Desc. Records

	//Non-descriptor fields, Used for Gator's book-keeping
	SDWORD		cbTotMem;			// Total allocated, cbElement * (cRowSet + 1 fBufferOffset)
	UINT			uRgbLockCnt;		// Count of usage for the rgbValue Buffer
	UINT			uPcbLockCnt;		// Count of usage for the pcbValue Buffer

	struct tagCONNECTIONINFO	*lpCI;			// Pointer to associated Connection records

	//linked list of stmts that this descriptor is associated with
	struct tagSTATEMENTINFO	*lpSI;			// Pointer to linked list of HSTMT nodes

	struct tagDESCRIPTORHEADER *next;		// Next node in the list
	struct tagDESCRIPTORHEADER *prev;		// Previous node in the list

	}	DESCRIPTORHEADER, * lpDESCRIPTORHEADER;


//*------------------------------------------------------------------------
//| BOUNDVARS
//|	This structure is used to bind and store memory used for
//|	fetching values from a cursor.  An array of cHighestBoundCol
//|	is allocated for binding.  Memory is allocated according to:
//|		Column-wise:
//|			When fColBinding==TRUE, then rgbValue points to the first byte
//|			of the array for that column.
//|		Row-wise:
//|			For row-wise binding, a block of records is allocated and
//|			assigned to lpRowData.  In this case rgbValue points to
//|			an offset for the particular column.
//*------------------------------------------------------------------------


typedef struct tagBOUNDVARS {
	// Description of bound information
	UWORD				icol;									// Which column is this
	BOOL				fBound;								// True if this descriptor is bound
	UWORD				uType;								// Binding State

	//Existing Bindings
	SWORD				fCType;								// C data type, describes format of memory
	SDWORD			cbElement;							// Size of an array element, cbMaxRowSet has count
	lpVOID			rgbValue;							// New address of Actual data. (i.e. for rebind)
	SDWORD			cbTotMem;							// Total allocated, cbElement * (cRowSet + 1 fBufferOffset)
	UNALIGNED SDWORD *pcbValue;						// Array of column lengths

	//New/Rebind Bindings
	SWORD				fCTypeNew;							// C data type, describes format of memory
	SDWORD			cbElementNew;						// Size of an array element, cbMaxRowSet has count
	lpVOID			rgbValueNew;						// Actual data
	SDWORD			cbTotMemNew;						// Total allocated, cbElement * (cRowSet + 1 fBufferOffset)
	UNALIGNED SDWORD *pcbValueNew;					// New address for Array of column lengths (i.e. for rebind)

	TCHAR				szCol[MAX_COL_NAME];				// For column name if chosen for Rowset window
	UINT				uRgbLockCnt;						// Count of usage for the rgbValue Buffer
	UINT				uPcbLockCnt;						// Count of usage for the pcbValue Buffer
	LPVOID			lpStmt;								// Need parent for orphaned mem

	// Links
 	struct tagBOUNDVARS * next;					// Next node in the list
	struct tagBOUNDVARS * prev;					// Previous node in the list
	} BOUNDVARS, * lpBOUNDVARS;


//*------------------------------------------------------------------------
//| ROLLBACKBV
//|	This structure is used to record changes to bound memory which may
//|	have to be rolled back due to errors.  By keeping this information
//|	in a list of nodes, we can easily undo any changes.
//*------------------------------------------------------------------------
typedef struct tagROLLBACKBV {
	LPVOID			lpData;								// The base memory pointer
	SDWORD			cbOldSize;							// The old element size
	SDWORD			cbNewSize;							// The new size
	} ROLLBACKBV, * lpROLLBACKBV;


//*------------------------------------------------------------------------
//| ROWWISEINFO
//|	This structure contains information about the memory allocated for
//|	rowset created using rowwise binding
//*------------------------------------------------------------------------
typedef struct tagROWWISEINFO {
	lpVOID			rgbValue;							// Buffer for row-wise binding
	UINT				uRgbLockCnt;						// Count of usage for the rgbValue Buffer
	SDWORD			cbTotMem;							// Total allocate memory
	} ROWWISEINFO, * lpROWWISEINFO;


//*------------------------------------------------------------------------
//| PARAMETERINFO
//|	This structure is stored with the creation of a parameter using
//|	SQLBindParameter.  It keeps track of all parameters for a given hstmt.
//*------------------------------------------------------------------------
typedef struct PARAMETERINFO {
   UWORD				ipar;									// The parameter number
	SWORD				fParamType;							// The parameter type
	SWORD				dwParamDex;							// The index into the array of parameter types
   SWORD				fCType;								// The C Type of the parameter
   SWORD				dwCDex;								// The index into the array of C Types
   SWORD				fSqlType;							// The Sql Type of the parameter
   SWORD				dwSqlDex;							// The index into the array of Sql Types
   UDWORD			cbColDef;							// The precision of the parameter
   SWORD				ibScale;								// The scale of the parameter
   PTR				rgbValue;							// Pointer to the parameter data (pointer to a UDWORD if pcbValue = SQL_DATA_AT_EXEC)
	UDWORD			cBytes;								// Number of actual bytes of data in one element(row) in rgbValue
   SDWORD			cbValueMax;							// The maximum length of the rgbValue buffer
   UNALIGNED SDWORD *	pcbValue;							// Pointer to storage for the length of the parameter
	BOOL *		lpfMacro;							// Flag for SQL_LEN_DATA_AT_EXEC macro

	// Bound Variable Information (for parameters that point into a rowset)
	BOOL				frgbRowset;							// TRUE if rgbValue points to a rowset, FALSE if not
	BOOL				fpcbRowset;							// TRUE if pcbValue points to a rowset, FALSE if not
	BOOL				fColBinding;						// TRUE for column-wise bound, FALSE for row-wise bound
	UINT				iHdbc;								// The number of the hdbc
	UINT				iHstmt;								// The number of the hstmt
	lpVOID			lpRowset;							// Pointer to either lpBOUNDVARS or lpROWWISE structure
	UDWORD			iCol;									// The column number
	UDWORD			iRow;									// The row number

   // Linked list relations
   struct PARAMETERINFO * next;					// Next parameter in line
   struct PARAMETERINFO * prev;					// Previous parameter in line
	} PARAMETERINFO, * lpPARAMETERINFO;


//*------------------------------------------------------------------------
//| LOCATORINFO
//|	This structure tracks information for a particular Locator.
//*------------------------------------------------------------------------
#ifdef SQLHLOC

typedef struct tagLOCATORINFO {

	SQLHLOC		hLoc;									//Locator
	BOOL			fHoldable;							//Is this Locator Holdable

   // Linked list relations
   struct tagLOCATORINFO * next;					// Next Locator in line
   struct tagLOCATORINFO * prev;					// Previous Locator in line
} LOCATORINFO, *lpLOCATORINFO;

#endif

struct tagCONNECTIONINFO;

//*------------------------------------------------------------------------
//| STATEMENTINFO
//|	This structure tracks information for a particular HSTMT, including
//|	statement options we need to be aware of, memory for bound variables,
//|	and other general information.
//*------------------------------------------------------------------------
typedef struct tagSTATEMENTINFO {
	// Our handles
	HSTMT				hstmt;								// Our statement handle
   UINT				uStmtNum;							// This statement number

	// Flag information for function handling and verification
	BOOL				fAsync;								// TRUE if async enabled on this hstmt
	BOOL				fColBinding;						// TRUE if we're using column wise binding
	UDWORD			uBookMarkAttr;						// Contains the BOOKMARK Connection/Statement
																// Attr/Option
	UDWORD			uCursorType;						// SQL_CURSOR_TYPE required for fetching
	UDWORD			uConcurType;						// SQL_CONCURRENCY_TYPE
	UDWORD			uStatBarFlag;						// Bitmask for status bar

	// Bound storage information
	UDWORD			uBufferOffset;						// SQL_BUFFER_OFFSET, determines starting element for fetch
	UDWORD			uRowSetSize;						// current rowset size
	UDWORD			uEFRowSetSize;						// SQL_ROWSET_SIZE
	UDWORD			uFSFRowSetSize;					// SQL_ATTR_ROW_ARRAY_SIZE
	UDWORD			uRowWiseSize;						// SQL_BIND_TYPE for row-wise binding (!fColBinding)
	UWORD				uReservedRows;						// Reserved for testing SQLSetPos w/SQL_ADD
	UDWORD			uCurrentRow;						// Current row number
	UWORD				*rgfRowStatus;						// For extended fetch
	UWORD				*rgfRowCache;						// Cache status values to detect changes
	UINT				uHighestBoundCol;					// The number of row descriptors we're tracking
	UINT				uBoundNodes;						// Number of bound columns that have been fetched
																//			(not necessarily same as uBoundCols)
	UINT				uBoundCols;							// Number of bound columns before fetching data
	lpBOUNDVARS		lpVars;								// A bound storage structure used for cursors
	lpROWWISEINFO	lpRowWise;							// A bound storage structure used for row wise bound buffers
	UINT				uRollBack;							// Count of items in array
	lpROLLBACKBV	lpRollBack;							// Rollback array for binding
	LPVOID			lpResults;							// Pointer to the results set information
	BOOL				fAutoDisplay;						// Should we automatically display rowset

	//Automatically allocated Descriptors
	lpDESCRIPTORHEADER	lpARD;							// Application Row Descriptor
	lpDESCRIPTORHEADER	lpAPD;							// Application Parameter Descriptor
	lpDESCRIPTORHEADER	lpIRD;							// Implimentation Row Descriptor
	lpDESCRIPTORHEADER	lpIPD;							// Implimentation Parameter Descriptor

	//linked list of Locators
#ifdef SQLHLOC
	lpLOCATORINFO		lpLoc;
#endif

	struct tagCONNECTIONINFO *lpCI;					// Connection info record associated with this Statement info record

	// Parameter information associated with this hstmt
	UWORD				cParams;								// The number of parameters for this hstmt
	UDWORD			cParamRows;							// The number of rows in each parameter (set by SQLParamOptions), default to 1
	BOOL				fParamOpt;							// TRUE if ParamOptions has been called, FALSE if not
	BOOL				fpirowNull;							// TRUE if pirow was NULL on the call to ParamOptions
	UDWORD			pirow;								// Buffer for pirow used by ParamOptions
	BOOL				fDisabled;							// TRUE if the FillParam controls are disabled, FALSE if enabled
	lpPARAMETERINFO	lpParams;						// Pointer to the linked list of parameters
	lpPARAMETERINFO	lpCurParam;						// Pointer to param currently being processed

	// Information needed for calls to ParamData & PutData
	BOOL				fSetPos;								// TRUE if the last call that returned SQL_NEED_DATA was from SQLSetPos, FALSE for SQLExecute or SQLExecDirect
	SWORD				ParamDataCType;					// The C Type of the parameter needing data (last time SQLParamData was called)

	BOOL		fStillExecuting;							// Did the previous call return SQL_STILL_EXECUTING?
	WORD		wFetchAPI;									// Did use call SQLExtendedFetch,SQLFetchScroll,
																//		or SQLFetch for this rowset.
	WORD		wCurState;									// State of this hstmt

	// Linked list information
	struct tagSTATEMENTINFO * next;					//	Next HSTMT on this HDBC
	struct tagSTATEMENTINFO * prev;					// Previous HSTMT on this HDBC

	// nested handle tracking
	struct tagSTATEMENTINFO * lpStmtChildren;				// Pointer to linked list of child HSTMT nodes
	struct tagSTATEMENTINFO * nextChild;					// Next child HSTMT
	struct tagSTATEMENTINFO * prevChild;					// Previous child HSTMT
	struct tagSTATEMENTINFO * lpStmtParent;					// Pointer to parent HSTMT, NULL if none

	} STATEMENTINFO, * lpSTATEMENTINFO;


//*------------------------------------------------------------------------
//| CONNECTIONINFO
//|	This structure is stored with each connection window handle and
//|	is will tie a window into the overall picture.  This structure
//|	is therefore used in many function calls.
//*------------------------------------------------------------------------
typedef struct tagCONNECTIONINFO {
	// Required windows information
	HWND				hwnd;									// Handle to the child window
	HWND				hwndIn;								// Input window
	HWND				hwndOut;								//	Output window
	HWND				hwndStmts;							// Combo-box of statement handles
	RECT				rToolRect;							// Bounding rectangle for mini-tool bar
	BOOL				fActive;								// Is this connection window active?
	// ODBC information for this window
	HDBC				hdbc;									// Connection handle for this window

	//Descriptor stuff
	SWORD				cDescs;								// # of descriptor handles allocated
	lpDESCRIPTORHEADER lpDesc;							// Descriptor table(s)
	lpDESCRIPTORHEADER lpSrcDesc,						// Current Source Descriptor
							 lpDestDesc;					// Current Destination Descriptor
	lpDESCRIPTORHEADER lpCurDesc;						// Current Descriptor
	UINT				uState;								// Our current ODBC state
	TCHAR				szUser[LARGEBUFF];				// User name stored for titles and errors
	TCHAR				szServer[SQL_MAX_DSN_LENGTH+1];// Server name stored for titles and errors
	TCHAR				szClientTitle[MAXTITLE];		// Title for the window
	UWORD				*puSupportedFunctions;			// Bitmask of supported SQL functions

	//The following 4 handles will store the handle that has new errors, (i.e. errors
	//that haven't been displayed yet. These will be used by the "Errors All" tool, to
	//determine which errors to display.
	SQLHDBC			hdbcCurr;
	SQLHSTMT			hstmtCurr;
	SQLHDESC			hdescCurr;

	// Statement information
	SWORD				cStmts;								// Current number of statements
	lpSTATEMENTINFO	lpStmt;							// Pointer to linked list of HSTMT nodes
	lpSTATEMENTINFO	lpCurStmt;						// Pointer to current HSTMT in lpStmt, NULL if none
	UDWORD			uRowSetSize;						// Default SQL_ROWSET_SIZE for new HSTMTs
	UDWORD			uRowWiseSize;						// SQL_BIND_TYPE for row-wise binding (!fColBinding)

	// User may have one file per connection window open
	OFSTRUCT		 	lpofn;								// For opening user file
	HFILE				hFile;								// Pointer to file
	TCHAR  			szFile[_MAX_PATH];				// Name of the file
	int   			cbFileSize;							// How big is the file
	LPTSTR  			szFileBuff;							// Memory for the file

	// Linked list relations
	UINT				uConNum;								// Which connection number is this?
	struct tagCONNECTIONINFO * next;					// Next connection in line
	struct tagCONNECTIONINFO * prev;					// Previous connection
	struct tagENVIRONMENTINFO *lpActiveEnv;		// Pointer to active Env node.

	} CONNECTIONINFO, * lpCONNECTIONINFO;


//*------------------------------------------------------------------------
//| ENVIRONMENTINFO
//|	This structure is stored with each environment handle and
//|	is will tie a window into the overall picture.  This structure
//|	is therefore used in many function calls.
//*------------------------------------------------------------------------
typedef struct tagENVIRONMENTINFO {
	// Required windows information
	HINSTANCE 		hInst;								//	Handle to our data segment
	HWND				hwndClient;							// Handle to client window
	HWND				hwnd;									// Handle to the child window
	RECT				rToolRect;							// Bounding rectangle for mini-tool bar
	HENV				henvCurr;

	// ODBC information for this window
	HENV				henv;									// Environment handle for application
	BOOL				fNTS;									//Added for 3.0, if TRUE if driver returns
																//string data null term. otherwise not null term
	BOOL				fODBC3XEnv;							//is this an odbc 3.0 env.

	//Connection information
	SWORD					cConns;							// Current number of connections
	lpCONNECTIONINFO	lpConn;							// Pointer to linked list of HDBC nodes
	lpCONNECTIONINFO	lpCurConn;						// Pointer to current HDBC in lpConn, NULL if none

	//SQLCopyDesc args.
	lpDESCRIPTORHEADER lpSrcDesc;						//Source Descriptor
	lpDESCRIPTORHEADER lpDestDesc;					//Destination Descriptor


	// Linked list relations
	UINT				uEnvNum;							// Which environment number is this?
	UINT				cCurStmtNum;					// Used to start stmt numbers at 0 for each Env
	UINT				cCurDescNum;					// Used to start desc numbers at 0 for each Env
	struct tagENVIRONMENTINFO * next;			// Next environment in line
	struct tagENVIRONMENTINFO * prev;			// Previous environment

	} ENVIRONMENTINFO, * lpENVIRONMENTINFO;


//*------------------------------------------------------------------------
//| HEADENV
//|	This structure contains global hEnv info.
//*------------------------------------------------------------------------
typedef struct tagHEADENV {
	RECT	rect;
	int	iCBEnvHeight;
	UINT	cyExtra;
	UINT	yToolBar;
	int x;													//x position of drop down
	int y;													//y position of drop down
	int nWidth;												//width of drop down
	int nHeight;											//heigth of drop down
	HWND	hwndEnvs;										//handle to drop down list of hEnv's
	SWORD	cEnvs;											// total number of hEnv's open.
	lpENVIRONMENTINFO lpHead;							//head node of list of hEnv nodes

	} HEADENV, * lpHEADENV;

//*------------------------------------------------------------------------
//| COLCELLINFO
//|	This structure holds the data for each Rowset cell in the rowset
//*------------------------------------------------------------------------
typedef struct tagCOLCELLINFO {
	SDWORD	wRow;						//Cell's row number
	SDWORD	wCol;						//Cell's column number
	HWND		hwndrgb;					//Handle to Cell's rgbValue edit window
	HWND		hwndpcb;					//Handle to Cell's pcbValue edit window
	SDWORD	dexRgb;					//Index of rgbValue Data
	SDWORD	dexPcb;					//Index of pcbValue Data
	RECT		rgbRect;					//Coord's for rgbValue Cell
	RECT		pcbRect;					//Coord's for pcbValue Cell
	SDWORD	wrgbHeight;				//Cell height
	SDWORD	wpcbHeight;				//Cell height
	SDWORD	wrgbWidth;				//Cell width
	SDWORD	wpcbWidth;				//Cell width
	PTR		rgbValue;				//Cell's rgbValue
	UDWORD	*pcbValue;				//Cell's pcbValue
	HBRUSH	hBrush;					//Brush associated with this Cell
	COLORREF crBkColor;				//Background color for this Cell
	COLORREF crTextColor;			//Text color for this Cell
	HFONT	hFont;						//Font Associated with this Cell

	} COLCELLINFO, *lpCOLCELLINFO;

//*------------------------------------------------------------------------
//| CELLINFO
//|	This structure holds the data for each Cell for the "Row",
//|	"rgfRowStatus", and "ARD Status" columns
//*------------------------------------------------------------------------
typedef struct tagCELLINFO {
	SDWORD	wRow;						//Cell's row number
	HWND		hwnd;						//Handle to Cell's window
	RECT		colRect;					//Coord's for Cell
	SDWORD	wHeight;					//Cell height
	SDWORD	wWidth;					//Cell width
	TCHAR		szData[MEDBUFF];		//Cell's Data
	HBRUSH	hBrush;					//Brush associated with this Cell
	COLORREF crBkColor;				//Background color for this Cell
	COLORREF crTextColor;			//Text color for this Cell
	HFONT	hFont;						//Font Associated with this Cell

	} CELLINFO, *lpCELLINFO;

//*------------------------------------------------------------------------
//| RESULTWININFO
//|	This structure tracks a results window which is owned by a
//|	connection.  Individual column meta data is kept with the
//|	bound var for that column.
//*------------------------------------------------------------------------
typedef struct tagRESULTWININFO {
	// Required windows information
	HINSTANCE 		hInst;								//	Handle to our data segment
	HWND				hwndClient;							// Handle to client window
	HWND				hwnd;									// Handle to the child window
	HWND				hwndVertScroll;					// Vertical scroll bar
	HWND				hwndHorzScroll;					// Horizontal scroll bar
	RECT				rToolRect;							// Bounding rectangle for mini-tool bar
	RECT				rMiniBar;							// Small rectangle between scroll bars
	HWND				hwndrgb;								// Handle to (rgbValue) edit control
	RECT				rRgbCell;							// Bounding rectangle (pcbValue) for edit cell
	HWND				hwndpcb;								// Handle to combo-box control
	RECT				rPcbCell;							// Bounding rectangle for combo-box cell
	int				iCurrRow;							// current row being processed
	int				iCurrCol;							// current column being processed
	HWND				hwndARDCell;						// Handle to ARD ArrayStatusPtr combo-box cell
	RECT				rARDCell;							// Bounding rectangle for combo-box cell
	// Window particulars
	TCHAR				szClientTitle[MAXTITLE];		// Title for the window
	int				left;									// where the editable start
	int				dx;									// Client area width
	int				dy;									// Client area height
	int				ly;									// Line pixel height (total)
	int				coly;									// Column header height
	int				rowcx;								// Total width of all columns (?)
	int				statcx;								// Width of status column (if present)
	int				rownumcolcx;						// Width of row number column
	int				cx;									// Default character width
	int				iTitleHeight;						// x drop for title text
	int				sumdx;								// Total width of results
	int				*colx;									// Array of colx, one for each bound col
	int				*colpcbx;								// Width of pcb portion of colx
	int				arraystatcx;
	UINT				*uPcbVals;							// Bitmask to columns to show pcbs
	TCHAR				szStatText[MAXOUTBUFF];			// Text for mini status bar

	// Scroll bar information
	BOOL				fHorzPresent;						// Horizontal scroll-bar required
	BOOL				fVertPresent;						// Vertical scroll-bar required
	int				iHorzRange;							// Scroll range horz
	int				iVertRange;							// Scroll range vert
	BOOL				fNoDataFound;						//TRUE if Fetch on FAT cursor returned SQL_NO_DATA_FOUND
	BOOL				frgfRowStatusExists,
						fArrayStatusExists;


	// ODBC information for this window
	lpCONNECTIONINFO	lpci;								// Parent connection handle
	lpSTATEMENTINFO	lpStmt;							// Parent statement handle

	} RESULTWININFO, * lpRESULTWININFO;


#define ACT_ROWSET_SIZE(lp)	(lp->uRowSetSize)
#define TOT_ROWSET_SIZE(lp)	(lp->uRowSetSize + lp->uReservedRows)

#endif

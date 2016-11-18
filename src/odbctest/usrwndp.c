//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	USRWNDP.C
//|
//| NOTES:  If any piece of this tool should be rewritten, this is it.  The user
//|			WndProcs were originally designed for very simply operations (like
//|			filling the DSNs for SQLConnect).  As time went on, however, more
//|			complex uses were found (Parameters, Bind col, etc...).  There is
//|			some common functionality in these which could have been generalized
//|			a lot better.
//|
//| Purpose:
//|		Contains user defined WndProc's for dialogs that require special handling
//|	(eg. SQLBindParameter, SQLGet/SetConnectOption, SQLGetInfo)
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "blddata.h"
#include "resource.h"

#pragma warning(disable:4996)
//----------------------------------------------------------------------------------
//		Defines and macros
//----------------------------------------------------------------------------------

#define INVALID_VALUE					9999
#define MAX_TIMESTAMP_CHAR_LEN		47				// Size of the character buffer required for the largest timestamp value

#define DEX_SQL_TIMESTAMP				10

#define SOURCE								TRUE
#define DESTINATION						FALSE
#define INVALID_TYPE						-999

#define ARD     0
#define APD     1
#define IRD     2
#define IPD     3

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE 			hInst;
extern HWND					hwndClient;
extern HWND					hwndStatBar;
extern TCHAR 				szOutBuff[MAXOUTBUFF];
extern lpUSROPTIONS		lpUsrOptions;
extern LPCTSTR 			szErrTitle;
extern lpCONNECTIONINFO	lpConnWin;					// Head node for connection windows
extern UINT					cNextConnect;				// Number assigned to next connection window
extern TCHAR			szSQL_C_SLONG[];
extern TCHAR			szSQL_C_ULONG[];
extern TCHAR			szSQL_C_SSHORT[];
extern TCHAR			szSQL_C_USHORT[];
extern TCHAR			szSQL_C_DEFAULT[];
extern TCHAR			szSQL_C_CHAR[];
extern TCHAR			szSQL_TIMESTAMP[];
extern TCHAR			szSQL_C_TIMESTAMP[];
extern TCHAR			szSQL_ATTR_APP_ROW_DESC[];
extern TCHAR			szSQL_ATTR_APP_PARAM_DESC[];
extern TCHAR			szSQL_C_WCHAR[];
extern BOOL				fUnicode;
extern TCHAR			szSQL_HANDLE_ENV[];
extern TCHAR			szSQL_HANDLE_DBC[];
extern TCHAR			szSQL_HANDLE_STMT[];
extern TCHAR			szSQL_HANDLE_DESC[];
extern TCHAR			szNULLPTR[];
extern TCHAR			szSQL_NULL_HANDLE[];
extern TCHAR			szSQL_C_BINARY[];
extern TCHAR			szSQL_DATETIME[];
extern TCHAR			szSQL_INTERVAL[];

dCSEG(TCHAR) szvParamType[]						= TEXT("vParamType");
dCSEG(TCHAR) szRGBVALUE[]							= TEXT("rgbValue");
dCSEG(TCHAR) szPCBVALUE[]							= TEXT("pcbValue");
dCSEG(TCHAR) szRgbAndPcb[]							= TEXT("rgbValue or pcbValue");
dCSEG(TCHAR) szRowsetHDBC[]						= TEXT("Rowset hdbc");
dCSEG(TCHAR) szRowsetHSTMT[]						= TEXT("Rowset hstmt");
dCSEG(TCHAR) szRowsetRow[]							= TEXT("Rowset row");
dCSEG(TCHAR) szRowsetColumn[]						= TEXT("Rowset column");
dCSEG(TCHAR) szrgbInfoValueType[]				= TEXT("rgbInfoValueType");
dCSEG(TCHAR) szfCType[]								= TEXT("fCType");
dCSEG(TCHAR) szNullTime[]							= TEXT(" 00:00:00");
dCSEG(TCHAR) szNullDate[]							= TEXT("0000-00-00 ");
dCSEG(TCHAR) szOneSpace[]							= TEXT(" ");
dCSEG(TCHAR) szNA[]									= TEXT("N/A");
dCSEG(TCHAR) szZeroOnly[]							= TEXT("0");
dCSEG(TCHAR) sz1[]									= TEXT("1");
dCSEG(TCHAR) szNullTerm[]							= TEXT("\0");
dCSEG(TCHAR) szBOUNDCOL[]							= TEXT("Bound Column");
dCSEG(TCHAR) szBOUNDROW[]							= TEXT("Bound Row");
dCSEG(TCHAR) szCLOSE[]								= TEXT("C&lose");
dCSEG(TCHAR) szUPDATE[]								= TEXT("&Update");
dCSEG(TCHAR) szUPDCELL[]							= TEXT("Update Cell");
dCSEG(TCHAR) szUpdatedParam[]						= TEXT("Parameter was updated successfully");
dCSEG(TCHAR) szFILLPARAM[]							= TEXT("Fill Param");
dCSEG(TCHAR) szipar[]								= TEXT("ipar");
dCSEG(TCHAR) szfCTypeTxt[]							= TEXT("fCType");
dCSEG(TCHAR) szfSqlType[]							= TEXT("fSqlType");
dCSEG(TCHAR) szRowText[]							= TEXT("Row");
dCSEG(TCHAR) szROWSETVALTXT[]						= TEXT("<rowset value>");
dCSEG(TCHAR) szERRORTXT[]							= TEXT("Error");

dCSEG(TCHAR) szHSTMTSELECT[]						= TEXT("hstmt ");
dCSEG(TCHAR) szHDBCSELECT[]						= TEXT("hdbc ");
dCSEG(TCHAR) szHENVSELECT[]						= TEXT("henv ");
dCSEG(TCHAR) szHDESCSELECT[]						= TEXT("hdesc ");

// Following refer to default arrays which must be used out of context of
//		the parameter arrays
extern lpDFTARRAY					lpdftParamType;
extern lpDFTARRAY					lpdftSqlTypes;
extern lpDFTARRAY					lpdftCTypes;
extern lpDFTARRAY					lpdftDateTimeSubType;
extern lpDFTARRAY					lpdftIntervalTypeSubCodes;
extern lpDFTARRAY					lpdftDescTypes;
extern lpDFTARRAY					lpdftIntervalTypes;
extern lpDFTARRAY					lpdftSQLTrueFalse30;
extern UINT							ciCTypes;
extern UINT							ciIntervalTypes;
extern UINT							ciDescTypes;
extern UINT							ciSQLTrueFalse30;
extern lpDFTARRAY					lpdftdfParamcbValues;
extern UINT							cidfParamcbValues;
extern lpDFTARRAY					lpdftdfParamrgbValues;
extern UINT							cidfParamrgbValues;


//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
VOID INTFUN UpdateLengthItem(HWND hwnd, WORD wID,LPCTSTR szBuff,SWORD fCType,BOOL fNTS);		// From DLGMNGR.C

VOID INTFUN DisableFillParamControls(HWND hDlg, lpDIALOGDESC lpDlg);
VOID INTFUN UpdateFillParamControls(HWND hDlg, lpDIALOGDESC lpDlg, lpPARAMETERINFO lpParam, UDWORD udRow);
VOID INTFUN	ValidateRgbValueTimestamp(HWND hDlg, int irgbValue);
BOOL INTFUN UpdateParameterNode(HWND hDlg, lpDIALOGDESC lpDlg);
BOOL INTFUN CheckHdbcForRowsets(lpCONNECTIONINFO lpci);
VOID INTFUN UpdateBPRowsetControls(HWND hDlg,  lpPARAMETERINFO lpParam,lpCONTROL lpCtl,SWORD idxRSHdbc,
											  SWORD idxRSHstmt,SWORD idxRSCol,SWORD idxRSRow);
VOID INTFUN EnableFPRowsetControls(HWND hDlg, lpPARAMETERINFO lpParam);
VOID INTFUN DisableFPRowsetControls(HWND hDlg);



//*------------------------------------------------------------------------
//|  GetPcbIndex:
//|		This function returns the index into the pcbValue Default array
//|	of the 'szVal' arg.
//|
//*------------------------------------------------------------------------

SDWORD INTFUN GetPcbIndex(SDWORD sdVal)
{
	SDWORD idx;
	//find the index of rgbValue <rowset value> in the dft array
	for (idx=0;idx < (SDWORD)cidfParamcbValues;idx++)
	{
		if (lpdftdfParamcbValues[idx].fOpt == sdVal)
			break;
	}

	return(idx);

} //GetPcbIndex()



//*------------------------------------------------------------------------
//|  GetRgbIndex:
//|		This function returns the index into the rgbValue Default array
//|	of the 'szVal' arg.
//|
//*------------------------------------------------------------------------

SDWORD INTFUN GetRgbIndex(SDWORD sdVal)
{
	SDWORD idx;

	for (idx=0;idx < (SDWORD)cidfParamrgbValues;idx++)
	{
		if (lpdftdfParamrgbValues[idx].fOpt == sdVal)
			break;
	}

	return(idx);

} //GetRgbIndex()

//*------------------------------------------------------------------------
//|  UserSelectedRowset:
//|		This function returns TRUE if the user selected <rowset value>
//|	for either the pcbValue or rgbValue
//|
//|	Returns:
//|		TRUE if the user selected <rowset value> for either the
//|	pcbValue or rgbValue
//*------------------------------------------------------------------------

BOOL INTFUN UserSelectedRowset(SDWORD dwRgbDex,SDWORD dwPcbDex)
{
	SDWORD	idxrgbRowset = 0,
				idxpcbRowset = 0;

	if (dwRgbDex != CB_ERR)
		idxrgbRowset=GetRgbIndex(ROWSETVAL);

	if (dwPcbDex != CB_ERR)
		idxpcbRowset=GetPcbIndex(ROWSETVAL);

	return(dwRgbDex == idxrgbRowset || dwPcbDex == idxpcbRowset);

} //UserSelectedRowset()


//*------------------------------------------------------------------------
//|  SetCType:
//|		For the selected fDescType, set the rgbInfoValue data type
//|	to the correct data type depending on the type required by the
//|	fDescType, as well as the DescriptionType combo box
//|	Returns:
//|		the associated fCType
//*------------------------------------------------------------------------


SWORD INTFUN SetCType(HWND hDlg,lpDFTARRAY lpDfts,lpDFTARRAY lpUserDfts,SDWORD nID,SDWORD wDex,
							 SDWORD wInfoTypeID,BOOL *pfHandle)
{

	SWORD fCType=0,
			ndx=0,
			wDex2;
	TCHAR	szfCType[MEDBUFF];


	//Assume this parameter is not a handle
	*pfHandle=FALSE;

	if (wDex==CB_ERR)
	{
		// The case of a user-defined option
	   // For fDescType of string type, set the rgbDesc data type to SQL_C_CHAR
		if (fUnicode)
		{
			fCType = SQL_C_WCHAR;
			lstrcpy(szfCType,szSQL_C_WCHAR);
		}
		else
		{
			fCType = SQL_C_CHAR;
			lstrcpy(szfCType,szSQL_C_CHAR);
		}
	}
	else
	{
		if (lpDfts[wDex].uValInfo & PRM_16BIT)
		{
	   	// For fDescType of 16-bit types, set the rgbDesc data type to SQL_C_USHORT
			fCType = SQL_C_SSHORT;
			lstrcpy(szfCType,szSQL_C_SSHORT);
		}
		else if ((lpDfts[wDex].uValInfo & PRM_32HDL) ||
					(lpDfts[wDex].uValInfo & PRM_HENV) ||
					(lpDfts[wDex].uValInfo & PRM_HDBC) ||
					(lpDfts[wDex].uValInfo & PRM_HSTMT) ||
					(lpDfts[wDex].uValInfo & PRM_HDESC))
		{
	   	// For fDescType of 32-bit types, set the rgbDesc data type to SQL_C_BINARY
	   	fCType = SQL_C_BINARY;
			lstrcpy(szfCType,szSQL_C_BINARY);
			*pfHandle=TRUE;
		}
		else if ((lpDfts[wDex].uValInfo & PRM_32MSK) ||
					(lpDfts[wDex].uValInfo & PRM_32BIT))
		{
	   	// For fDescType of 32-bit types, set the rgbDesc data type to SQL_C_ULONG
	   	fCType = SQL_C_SLONG;
			lstrcpy(szfCType,szSQL_C_SLONG);
		}
		else if (lpDfts[wDex].uValInfo & PRM_PTR_BIN)
		{
	   	// For fDescType of 32-bit types, set the rgbDesc data type to SQL_C_ULONG
	   	fCType = SQL_C_BINARY;
			lstrcpy(szfCType,szSQL_C_BINARY);
		}
		else
		{
			// The case of a user-defined option or PRM_STR
   		// For fDescType of string type, set the rgbDesc data type to SQL_C_CHAR
			if (fUnicode)
			{
				fCType = SQL_C_WCHAR;
				lstrcpy(szfCType,szSQL_C_WCHAR);
			}
			else
			{
				fCType = SQL_C_CHAR;
				lstrcpy(szfCType,szSQL_C_CHAR);
			}
		}

	} //if (wDex==CB_ERR)

	ndx=GetCTypeIndex(!USE_C_DFT_ARRAY,szfCType);
	wDex2=(SWORD)SendDlgItemMessage(hDlg,wInfoTypeID,CB_SETCURSEL,ndx,0L);

	if (!fCType && (wDex >= 0))
		fCType = (SWORD)lpUserDfts[wDex].fOpt;

	return(fCType);

} //SetCType()


//*---------------------------------------------------------------------------------
//| DoInitDialog:
//|	This function updates info type combobox (Containing the fCType of the info type
//|	for a SQL API).
//| Parms:
//|	hDlg		Handle to the BindParam dialog
//|	lpDlg		Pointer to the dialog structure
//|	lpCtl		Pointer to the control structure
//|	lpParm	Pointer to the PARAMETERINFO structure to update the controls to
//|   wTargetId ID of the Info field
//|	wInfoTYpeID	ID of the Info type field
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DoInitDialog(HWND hDlg, lpDIALOGDESC lpDlg,lpCONTROL lpCtl,lpPARAMITEM lpParm,
				SDWORD wTargetId,SDWORD wInfoTypeID,SDWORD idxInfo, SDWORD idxInfoType,SDWORD idxRgbValue)
{
	LONG	wDex,
			wDex2;

	if (lpCtl->wID == wInfoTypeID)
	{
		// Get current selection from target list
		wDex = (DWORD)SendDlgItemMessage(hDlg,wTargetId,CB_GETCURSEL,0,0L);
		if (wDex == CB_ERR)
			return;

		// For the selected fInfoType, set the rgbInfoValue data type, as well
		// as the InfoValueType combo box to the correct data type depending
		// on the type required by the fInfoType
		lpDlg->lpParms[idxRgbValue]->fCType=SetCType(hDlg,lpDlg->lpParms[idxInfo]->lpDfts,
				lpDlg->lpParms[idxRgbValue]->lpDfts,wInfoTypeID,wDex,wInfoTypeID,
				&(lpDlg->lpParms[idxRgbValue]->fHandle));

		if (IsCharData(lpDlg->lpParms[idxRgbValue]->fCType))
		{
			if (fUnicode)
			{
				lpDlg->lpParms[idxRgbValue]->fCType=SQL_C_WCHAR;
				wDex=GetCTypeIndex(USE_C_DFT_ARRAY,szSQL_C_WCHAR);
			}
			else
				wDex=GetCTypeIndex(USE_C_DFT_ARRAY,szSQL_C_CHAR);

			wDex2 = (WORD)SendDlgItemMessage(hDlg,wInfoTypeID,CB_SETCURSEL,wDex,0L);

		}
	}

} //DoInitDialog()



//*---------------------------------------------------------------------------------
//| DoUpdateContents:
//|	This function updates info type combobox (Containing the fCType of the info type
//|	for a SQL API).
//| Parms:
//|	hDlg		Handle to the BindParam dialog
//|	lpDlg		Pointer to the dialog structure
//|	lpCtl		Pointer to the control structure
//|	lpParm	Pointer to the PARAMETERINFO structure to update the controls to
//|   wTargetId ID of the Info field
//|	wInfoTYpeID	ID of the Info type field
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN DoUpdateContents(HWND hDlg, lpDIALOGDESC lpDlg,lpCONTROL lpCtl,lpPARAMITEM lpParm,
				WORD wTargetId,WORD wInfoTypeID,SWORD idxInfo, SWORD idxInfoType,SWORD idxRgbValue)
{
	SWORD		wDex;
	LPTSTR	str;
	UWORD		fOption;
	BOOL		fEnable;
	TCHAR		szAPI[LARGEBUFF];


	// Combo Box UpdateContents message for fInfoType combo box
	if (lpCtl->wID == wTargetId)
	{

		// Get the new fInfoType value
		wDex = (SWORD)SendDlgItemMessage(hDlg,lpCtl->wID,CB_GETCURSEL,0,0L);

		// If it is not in the list, it is probably a driver-specific value
		if(wDex == CB_ERR)
		{
			// Find out the size (in digits)
			DWORD dwDigits = GetTrueWindowTextLength(hDlg,lpCtl->wID);
			str = (LPTSTR)AllocateMemory(dwDigits + sizeof(TCHAR));

			if(!str)
				return FALSE;

			GetDlgItemText(hDlg,lpCtl->wID,str,(int)(dwDigits/sizeof(TCHAR)));				// Get the fInfoType as a string

			if(!IsValidInt(SQL_C_ULONG, str, FALSE, lpParm->szName))   // Validate it as a SQL_C_USHORT
				goto exit01;

			fOption = (UWORD)_ttoi(str);		// Convert value to an integer
			fEnable = TRUE;         // Set flag to enable the InfoValueType combo box
		}
		else		// fInfoType was in the list, so get the index to the chosen fOption
		{
			fOption = (UWORD)lpParm->lpDfts[wDex].fOpt;
			fEnable = FALSE;		// Clear flag so the InfoValueType combo box is not enabled
		}

		// For the selected fInfoType, set the rgbInfoValue data type, as well
		// as the InfoValueType combo box to the correct data type depending
		// on the type required by the fInfoType
		lpDlg->lpParms[idxRgbValue]->fCType=SetCType(hDlg,lpDlg->lpParms[idxInfo]->lpDfts,
				lpDlg->lpParms[idxRgbValue]->lpDfts,wInfoTypeID,wDex,wInfoTypeID,
				&(lpDlg->lpParms[idxRgbValue]->fHandle));


		// If fInfoType is a driver-specific type, enable the InfoValueType combo box
		EnableWindow(GetDlgItem(hDlg,wInfoTypeID),fEnable);
		EnableWindow(GetDlgItem(hDlg,wInfoTypeID+DLG_TEXT_BASE),fEnable);
	}
	// Combo Box UpdateContents message for InfoValueType combo box
	else if (lpCtl->wID == wInfoTypeID)
	{

		wDex = (WORD)SendDlgItemMessage(hDlg,wInfoTypeID,CB_GETCURSEL,0,0L);		// Get the new index
		// Validate the SQL_C data type
		if (wDex == (WORD)CB_ERR)
			// The value is not a valid data type, so display an error, reset the ParameterType combo box to SQL_C_CHAR,
			// set the vParam data type to SQL_C_CHAR, and set the focus to the ParameterType combo box
		{
			szMessageBox(hDlg,MB_ICONINFORMATION | MB_OK, (LPTSTR)szrgbInfoValueType,
							 GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidDataType),(LPTSTR)szrgbInfoValueType, (LPTSTR)szrgbInfoValueType);
			if (fUnicode)
			{
   			lpDlg->lpParms[idxRgbValue]->fCType = SQL_C_WCHAR;
				lstrcpy(szAPI,szSQL_C_WCHAR);
			}
			else
			{
   			lpDlg->lpParms[idxRgbValue]->fCType = SQL_C_CHAR;
				lstrcpy(szAPI,szSQL_C_CHAR);
			}

   		SendDlgItemMessage(hDlg,wInfoTypeID,CB_SETCURSEL,GetCTypeIndex(USE_C_DFT_ARRAY,szAPI),0L);
   		SetFocus(GetDlgItem(hDlg,lpCtl->wID));
		}
		else		// The value was a valid data type, so update the vParam combo box data type to the new type
			lpDlg->lpParms[idxRgbValue]->fCType = (SWORD)lpDlg->lpParms[idxInfoType]->lpDfts[wDex].fOpt;
	}

	return(TRUE);

exit01:
	FREE_PTR(str);
    return FALSE;

} //DoUpdateContents()

//*---------------------------------------------------------------------------------
//| DisableBPRowsetControls
//|	This function disables the controls in the BindParameter dialog that are only
//|		only used by parameters bound to a rowset element.  It also sets the text
//|		in the controls to N/A.
//| Parms:
//|	hDlg		Handle to the BindParam dialog
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DisableBPRowsetControls(HWND hDlg,SWORD idxRSHdbc,SWORD idxRSHstmt,
												SWORD idxRSCol,SWORD idxRSRow,
												SWORD idxRSHdbcTxt,SWORD idxRSHstmtTxt,
												SWORD idxRSColTxt,SWORD idxRSRowTxt)
{
	// Now disable all of those controls and their labels (hot-keys)
   EnableWindow(GetDlgItem(hDlg,	IDGB_BINDP_ROWSET),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSHdbc),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSHdbcTxt),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSHstmt),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSHstmtTxt),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSCol),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSColTxt),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSRow),FALSE);
   EnableWindow(GetDlgItem(hDlg,idxRSRowTxt),FALSE);

	return;
}

//*---------------------------------------------------------------------------------
//| EnableBPRowsetControls
//|	This function enables the controls in the BindParameter dialog that are only
//|		only used by parameters bound to a rowset element.
//| Parms:
//|	hDlg		Handle to the BindParam dialog
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN EnableBPRowsetControls(HWND hDlg,SWORD idxRSHdbc,SWORD idxRSHstmt,
												SWORD idxRSCol,SWORD idxRSRow,
												SWORD idxRSHdbcTxt,SWORD idxRSHstmtTxt,
												SWORD idxRSColTxt,SWORD idxRSRowTxt)
{
	// Now disable all of those controls and their labels (hot-keys)
   EnableWindow(GetDlgItem(hDlg,	IDGB_BINDP_ROWSET),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSHdbc),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSHdbcTxt),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSHstmt),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSHstmtTxt),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSCol),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSColTxt),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSRow),TRUE);
   EnableWindow(GetDlgItem(hDlg,idxRSRowTxt),TRUE);

	return;
}

//*---------------------------------------------------------------------------------
//| UpdateBindParamControls:
//|	This function updates the controls in the BindParam dialog
//| Parms:
//|	hDlg		Handle to the BindParam dialog
//|	lpDlg		Pointer to the dialog structure
//|	lpParam	Pointer to the PARAMETERINFO structure to update the controls to
//|   ipar		The parameter number (ipar from BindParam)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN UpdateBindParamControls(HWND hDlg, lpCONTROL lpCtl,lpDIALOGDESC lpDlg, UWORD ipar,
							SWORD idxfParamType,
							SWORD idxfCType,
							SWORD idxfSQLType,
							SWORD idxcbColDef,
							SWORD idxibScale,
							SWORD idxcbValueMax,
							SWORD idxrgbValue,
							SWORD idxpcbValue,
							SWORD idxDAE,
							SWORD idxRSRow,
							SWORD idxRSHdbc,
							SWORD idxRSHstmt,
							SWORD idxRSCol,
							SWORD idxRSRowTxt,
							SWORD idxRSHdbcTxt,
							SWORD idxRSHstmtTxt,
							SWORD idxRSColTxt)
{
	lpCONNECTIONINFO	lpci = lpDlg->ci;
	lpPARAMETERINFO	lpParam=NULL;
	BOOL					fFormat=FALSE;
	BOOL					fMacro=FALSE;
	LPTSTR				lprgbValue=NULL;
	TCHAR					szTemp[50];
	UDWORD				dLength;
   SDWORD				pcbValueTmp;
	DWORD					idxRgb = 0, // BUGBUG: never initialized
							idxRgbRowset,
							idxRgbNullPtr,
							idxPcb,
							idxPcbRowset,
							idxPcbNTS,
							idxPcbNullPtr,
							idxPcbNullData,
							idxPcbDAE,
							idxPcbDftParm;



	if (!lpci || !lpci->lpCurStmt || !lpci->lpCurStmt->lpParams)
		return; //Nothing to do

	//find the index of pcbValue <rowset value> in the dft array
	for (idxPcb=0;idxPcb < cidfParamcbValues;idxPcb++)
	{
		switch(lpdftdfParamcbValues[idxPcb].fOpt)
		{
			case ROWSETVAL:
				idxPcbRowset=idxRgb;
				break;
			case NULLPTR:
				idxPcbNullPtr=idxRgb;
				break;
			case SQL_NTS:
				idxPcbNTS=idxPcb;
				break;
			case SQL_NULL_DATA:
				idxPcbNullData=idxPcb;
				break;
			case SQL_DATA_AT_EXEC:
				idxPcbDAE=idxPcb;
				break;
			case SQL_DEFAULT_PARAM:
				idxPcbDftParm=idxPcb;
				break;
		}
	}

	//find the indices of rgbValue dft values
	for (idxRgb=0;idxRgb < cidfParamrgbValues;idxRgb++)
	{
		switch(lpdftdfParamrgbValues[idxRgb].fOpt)
		{
			case ROWSETVAL:
				idxRgbRowset=idxRgb;
				break;
			case NULLPTR:
				idxRgbNullPtr=idxRgb;
				break;
		}
	}

	//Return if there Parameter 'ipar' doesn't exist
	if (!(lpParam=SearchParamList(lpci->lpCurStmt,ipar)))
		return;

	// If it is user-defined, the regular dialog handler will take care of it
	if ((lpParam->dwParamDex != CB_ERR) && (idxfParamType != -1))
		SendDlgItemMessage(hDlg,idxfParamType,CB_SETCURSEL,lpParam->dwParamDex,0L);

   SendDlgItemMessage(hDlg,idxfCType,CB_SETCURSEL,lpParam->dwCDex,0L);

	//Update the rgbValue's fCType
	lpDlg->lpParms[idxfParamType == -1 ? IPAR_BPARAM_RGBVALUE : IPAR_BP_RGBVALUE]->fCType = lpParam->fCType;
   SendDlgItemMessage(hDlg,idxfSQLType,CB_SETCURSEL,lpParam->dwSqlDex,0L);

	wsprintf(szTemp, TEXT("%lu"), lpParam->cbColDef);
	SetWindowText(GetDlgItem(hDlg,idxcbColDef),szTemp);

	wsprintf(szTemp, TEXT("%d"), lpParam->ibScale);
	SetWindowText(GetDlgItem(hDlg,idxibScale),szTemp);

	if (idxcbValueMax != -1)
	{
		wsprintf(szTemp, TEXT("%ld"), lpParam->cbValueMax);
		SetWindowText(GetDlgItem(hDlg,idxcbValueMax),szTemp);
	}

	// Format rgbValue
	if(lpParam->frgbRowset)
	   SendDlgItemMessage(hDlg,idxrgbValue,CB_SETCURSEL,idxRgbRowset,0L);
	else if(!(lpParam->rgbValue))
	   SendDlgItemMessage(hDlg,idxrgbValue,CB_SETCURSEL,idxRgbNullPtr,0L);
	else
	{

		//Bug #318
		dLength=GetDisplaySizeBasedOnType(lpParam->fCType,lpParam->cBytes);

		//
		lprgbValue = (PTR)AllocateMemory((dLength + 10) * sizeof(TCHAR));

		if(!lprgbValue)
		{
			szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)TEXT("Error"),
					GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
					dLength, (LPTSTR)szRGBVALUE);
		}

		if(lprgbValue)
			fFormat = FormatValueBasedOnType(lprgbValue, dLength, lpParam->rgbValue, lpParam->fCType,
										lpParam->cBytes,GETOUTPUTWINDOW(lpci));
		else
			lprgbValue = szTemp;

		if(!fFormat)
			lstrcpy(lprgbValue, (LPTSTR)TEXT(""));

		// Strip off the extra "" (double quotes) that come from FormatValueBasedOnType function
		if (IsCharData(lpParam->fCType))
		{
			LPTSTR		str, lstr;

			str = lstr = lprgbValue;
			while(lstr = _tcschr(str,TEXT('"')))
				lstrcpy(lstr,lstr+1);
		}

		SendDlgItemMessage(hDlg,idxrgbValue,CB_SETCURSEL,(WPARAM)-1,0L);
		SetWindowText(GetDlgItem(hDlg,idxrgbValue),lprgbValue);
	}

	// Format pcbValue
	if(lpParam->fpcbRowset)
	   SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,idxPcbRowset,0L);
	else if(!(lpParam->pcbValue))
	   SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,idxPcbNullPtr,0L);
	else
	{
		switch(*(SDWORD *)(lpParam->pcbValue))
		{
			case SQL_NTS:
			   SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,idxPcbNTS,0L);
				break;
			case SQL_NULL_DATA:
			   SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,idxPcbNullData,0L);
				break;
			case SQL_DATA_AT_EXEC:
			   SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,idxPcbDAE,0L);
				break;
			case SQL_DEFAULT_PARAM:
			   SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,idxPcbDftParm,0L);
				break;
			default:
				fMacro=TRUE;
				// The pcbValue is a number -- see if the macro was invoked
				// If it was, then undo it to display the real number
				if(*(lpParam->lpfMacro))
					pcbValueTmp = UNDO_LEN_DATA_AT_EXEC(*(SDWORD *)(lpParam->pcbValue));
				else
					pcbValueTmp = *(SDWORD *)(lpParam->pcbValue);

				wsprintf(szTemp, TEXT("%ld"), pcbValueTmp);
				SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,(WPARAM)-1,0L);
				SetWindowText(GetDlgItem(hDlg,idxpcbValue),(LPTSTR)szTemp);

				if(*(lpParam->lpfMacro))
					CheckDlgButton(hDlg, idxDAE, TRUE);
				else
					CheckDlgButton(hDlg, idxDAE, FALSE);

				break;
		}

		EnableWindow(GetDlgItem(hDlg,idxDAE),fMacro);
	}

	if((lpParam->frgbRowset) || (lpParam->fpcbRowset))
	{
		EnableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);

		UpdateBPRowsetControls(hDlg, lpParam,lpCtl,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow);
	}
	else
		DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);

	FREE_PTR(lprgbValue);

 	return;
}


//*------------------------------------------------------------------------
//|  AddHstmtToDropDown:
//|		This function adds a hstmt to Drop down list.
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN AddHstmtToDropDown(HWND hwndHSTMT,lpPARAMETERINFO lpParam,lpCONNECTIONINFO lpCiList,
										 lpSTATEMENTINFO lpSi,DWORD *pdwSel)
{
	UINT	idx;
	TCHAR	szTemp[MEDBUFF];
	DWORD	dwDex;

	for(idx=0;idx<(UINT)(lpCiList->cStmts);idx++)
	{
		// Check the hstmt for bound memory.  If found, then add this hstmt to the drop down list
		if((lpSi->fColBinding && lpSi->lpVars) || ((!lpSi->fColBinding) && lpSi->lpRowWise))
		{
#ifdef WIN32
			wsprintf(szTemp, TEXT("%u: 0x%08p"), lpSi->uStmtNum, lpSi->hstmt);
#else
			wsprintf(szTemp, TEXT("%u: 0x%04X:0x%04X"), lpSi->uStmtNum,
						HIWORD(lpSi->hstmt), LOWORD(lpSi->hstmt));
#endif
			dwDex = (DWORD)SendMessage(hwndHSTMT, CB_ADDSTRING, 0, (LPARAM)szTemp);

			if(dwDex != CB_ERR)
				SendMessage(hwndHSTMT, CB_SETITEMDATA, (WPARAM)dwDex, (LPARAM)lpSi->uStmtNum);

			if(pdwSel && (lpSi->uStmtNum == lpParam->iHstmt))
				*pdwSel = dwDex;
		}

		lpSi = lpSi->next;
	}

} //AddHstmtToDropDown()



//*------------------------------------------------------------------------
//|  HandleUserOpt:
//|		This function sets the fCType in the case where the user has
//|	entered a user defined type.
//|	Returns:
//|		the associated fCType
//*------------------------------------------------------------------------

BOOL INTFUN HandleUserOpt(HWND hDlg,lpPARAMITEM lpParm,UWORD wCtlID,SWORD *pwDex,
									int nID,UWORD *pfOption)
{
	LPTSTR	str;
	BOOL		fEnable=FALSE;

	// For the selected fDescType, set the rgbInfoValue data type, as well
	//	as the DescriptionType combo box to the correct data type depending
	// on the type required by the fDescType
	*pwDex = (SWORD)SendDlgItemMessage(hDlg,wCtlID,CB_GETCURSEL,0,0L);	// Get the new fDescType value

	if(*pwDex == CB_ERR)	// If it is not in the list, it is probably a driver-specific value
	{
		// Find out the size (in digits)
		DWORD dwDigits = GetTrueWindowTextLength(hDlg,wCtlID);

		str = (LPTSTR)AllocateMemory(dwDigits + sizeof(TCHAR));
		if(!str)
			return(FALSE);

		// Get the fInfoType as a string
		GetDlgItemText(hDlg,wCtlID,str,(int)(dwDigits/sizeof(TCHAR)));

		// Validate it as a SQL_C_USHORT
		if(!IsValidInt(SQL_C_USHORT, str, FALSE, lpParm->szName))
		{
			FREE_PTR(str);

			return(FALSE);
		}

		*pfOption = (UWORD)_ttoi(str);		// Convert value to an integer
		fEnable = TRUE;         // Set flag to enable the InfoValueType combo box
	}
	else		// fInfoType was in the list, so get the index to the chosen fOption
	{
		*pfOption = (UWORD)lpParm->lpDfts[*pwDex].fOpt;
		fEnable = FALSE;		// Clear flag so the InfoValueType combo box is not enabled
	}

	return (fEnable);

} //HandleUserOpt()





//*------------------------------------------------------------------------
//|  SetDftWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN SetDftWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	switch(msg) {
		// We're interested in setting defaults when doing SQLConnect
		case WM_INITDIALOG:
			{
			lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
			lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
			GetDSNs(GetDlgItem(hDlg, lpCtl->wID));
			}
			return FALSE;

		default:
			break;
		}

	return TRUE;
}


//*------------------------------------------------------------------------
//|  ProcessRowSetHDBC:
//|		This function handles the ROWSET HDBC Dialog Box
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessRowSetHDBC(HWND hwndCtl,lpCONNECTIONINFO lpCiList)
{
	DWORD		dwDex;
	BOOL		fReset=TRUE;
	TCHAR		szBuff[LARGEBUFF]=TEXT("");
	LPTSTR	pszConnStr=NULL;

	//Find the hdbc #
	if (pszConnStr=_tcschr(lpCiList->szClientTitle,TEXT(':')))
	{
		//back up to the char before the ":"
		pszConnStr--;
		lstrcpy(szBuff,pszConnStr);
	}

	// We need to go through the linked list of hdbc's checking for ones that
	// have a valid rowset pending
	if(CheckHdbcForRowsets(lpCiList))
	{
		SendMessage(hwndCtl,CB_RESETCONTENT,0,0L);
		fReset = FALSE;
		dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)szBuff);
		if(dwDex != CB_ERR)
			SendMessage(hwndCtl, CB_SETITEMDATA, (WPARAM)dwDex, (LPARAM)lpCiList->uConNum);
	}

	lpCiList = lpCiList->next;

	while(lpCiList != lpConnWin)
	{
		if(CheckHdbcForRowsets(lpCiList))
		{
			if(fReset)
			{
				SendMessage(hwndCtl,CB_RESETCONTENT,0,0L);
				fReset = FALSE;
			}

			dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)lpCiList->szClientTitle);

			if(dwDex != CB_ERR)
				SendMessage(hwndCtl, CB_SETITEMDATA, (WPARAM)dwDex, (LPARAM)lpCiList->uConNum);
		}

		lpCiList = lpCiList->next;
	}

	// If we had any valid hdbc's, set the current selection to the first one
	if(SendMessage(hwndCtl,CB_GETCOUNT,0,0L))
		SendMessage(hwndCtl,CB_SETCURSEL,0,0L);

} //ProcessRowSetHDBC()




//*------------------------------------------------------------------------
//|  ProcessRowSetHSTMT:
//|		This function handles the ROWSET HSTMT Control
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessRowSetHSTMT(HWND hDlg,lpCONTROL lpCtl,UINT idxRSHdbc,
										 lpCONNECTIONINFO lpCiList)
{

	// If a valid hdbc was available, fill the hstmt drop down and set the current selection to the first hstmt
	lpSTATEMENTINFO	lpSi=NULL;
	HWND					hwndHSTMT = GetDlgItem(hDlg,lpCtl->wID);
	HWND					hwndHDBC	 = GetDlgItem(hDlg,idxRSHdbc);
	DWORD					dwDex;
	UINT					iConnNum;
	BOOL					fFound=FALSE;

	// We need to go through the linked list of hdbc's checking for ones that have a valid rowset pending
	dwDex = (DWORD)SendMessage(hwndHDBC,CB_GETCURSEL,0,0L);
	iConnNum = (UINT)SendMessage(hwndHDBC,CB_GETITEMDATA,(WPARAM)dwDex,0L);
	if(iConnNum == lpCiList->uConNum)
		fFound = TRUE;
	if(!fFound){
		lpCiList = lpCiList->next;
		while(lpCiList != lpConnWin){
			if(iConnNum == lpCiList->uConNum){
				fFound = TRUE;
				break;
				}
			lpCiList = lpCiList->next;
			}
		}

	// Now we have a pointer to the connection, now traverse the linked list of hstmts
	// and put them in the HSTMT combo list box
	SendMessage(hwndHSTMT,CB_RESETCONTENT,0,0L);
	if(fFound)
	{
		if(lpCiList->lpStmt)
			AddHstmtToDropDown(hwndHSTMT,NULL,lpCiList,lpCiList->lpStmt,NULL);

		// If we had any valid hstmt's, set the current selection to the first one
		if(SendMessage(hwndHSTMT,CB_GETCOUNT,0,0L))
			SendMessage(hwndHSTMT,CB_SETCURSEL,0,0L);
		else
			SetDlgItemText(hDlg,lpCtl->wID,szZeroOnly);

	}
	else
		SetDlgItemText(hDlg,lpCtl->wID,szZeroOnly);

} //ProcessRowSetHSTMT()


//*------------------------------------------------------------------------
//|  ProcessRgbValue:
//|		This function handles the rgbValue DropDown Control
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessRgbValue(HWND hDlg,lpDIALOGDESC lpDlg,SWORD iParfCType,SWORD iParrgbValue,
			SWORD idxRSHdbc,SWORD idxRSHstmt,SWORD idxRSCol,SWORD idxRSRow,
			SWORD idxRSHdbcTxt,SWORD idxRSHstmtTxt,SWORD idxRSColTxt,SWORD idxRSRowTxt)
{

	SDWORD idxRgb;

	//find the index of rgbValue <rowset value> in the dft array
	for (idxRgb=0;idxRgb < (SDWORD)cidfParamrgbValues;idxRgb++)
	{
		if (lpdftdfParamrgbValues[idxRgb].fOpt == ROWSETVAL)
			break;
	}

	// If no fCType is set yet,
	if(!lpDlg->lpParms[iParfCType]->lpData)
	{
		// set the rgbValue type to SQL_C_CHAR
		if (fUnicode)
			lpDlg->lpParms[iParrgbValue]->fCType = SQL_C_WCHAR;
		else
			lpDlg->lpParms[iParrgbValue]->fCType = SQL_C_CHAR;

		DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
	}
	else if (UserSelectedRowset(lpDlg->lpParms[iParrgbValue]->dwDex,CB_ERR))
	{
		EnableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
	}
	else
	{
		DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
	}

} //ProcessRgbValue()


//*------------------------------------------------------------------------
//|  ProcessRowSetRow:
//|		This function handles the ROWSET Row Drop Down Control
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessRowSetRow(HWND hDlg,lpCONTROL lpCtl,lpDIALOGDESC lpDlg,
	SWORD iParrgbValue,SWORD iParpcbValue,SWORD idxipar,SWORD iParfDAE,
	SWORD idxDAE,SWORD idxRSCol,SWORD idxRSRow,SWORD idxRSHdbc,SWORD idxRSHstmt,
	SWORD idxfParamType,	SWORD idxfCType,SWORD idxfSQLType,SWORD idxcbColDef,SWORD idxibScale,
	SWORD idxcbValueMax,	SWORD idxrgbValue, SWORD idxpcbValue,
	SWORD idxRSColTxt,SWORD idxRSRowTxt,SWORD idxRSHdbcTxt,SWORD idxRSHstmtTxt)
{
	TCHAR	szTemp[SMALLBUFF];

	// First check to see if the last call to set param was an error
	// If it was, don't use values out of the linked list as defaults, use
	// the ones stored in the parameter array (the invalid values from the last call)
	if(lpDlg->lpParms[iParrgbValue]->dwDex != DEX_BIND_PARAM_ERROR)
	{
		// First validate the data in ipar
		GetDlgItemText(hDlg,idxipar,szTemp,15);

		if(!ValidateEditWindowByType(hDlg,idxipar,(LPTSTR)szipar,SQL_C_USHORT))
		{
			// It is not a valid SQL_C_USHORT, reset the edit box to "" and give it the focus
			SetWindowText(GetDlgItem(hDlg,idxipar),TEXT(""));
			SetFocus(GetDlgItem(hDlg,idxipar));
			SendDlgItemMessage(hDlg,idxipar,EM_SETSEL,(WPARAM)0,MAKELPARAM(0,-1));
		}
		else
		{
			if(lpDlg->lpParms[iParrgbValue]->lpData)
				UpdateBindParamControls(hDlg, lpCtl, lpDlg, (UWORD)_ttoi(szTemp),
							idxfParamType,
							idxfCType,
							idxfSQLType,
							idxcbColDef,
							idxibScale,
							idxcbValueMax,
							idxrgbValue,
							idxpcbValue,
							idxDAE,
							idxRSHdbc,
							idxRSHstmt,
							idxRSCol,
							idxRSRow,
							idxRSHdbcTxt,
							idxRSHstmtTxt,
							idxRSColTxt,
							idxRSRowTxt);
		}
	}
	else
	{

		// Make sure the Macro checkbox gets reset correctly on an error
		if(lpDlg->lpParms[iParfDAE]->fNull)
			CheckDlgButton(hDlg,idxDAE,TRUE);
		else
			CheckDlgButton(hDlg,idxDAE,FALSE);

		// Also make sure the Rowset controls get enabled/disabled correctly
		if (UserSelectedRowset(lpDlg->lpParms[iParrgbValue]->dwDex,
									  lpDlg->lpParms[iParpcbValue]->dwDex))
		{
			EnableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
		}
		else
		{
			DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
		}
	}

} //ProcessRowSetRow()

//*---------------------------------------------------------------------------------
//| IsSQLCDEFAULT:
//|		This function returns TRUE if idx is foun
//| Parms:
//|	idx	index of C Type
//| Returns:
//|	TRUE if
//*---------------------------------------------------------------------------------

BOOL INTFUN IsSQLCDEFAULT(SWORD idx)
{
//	SWORD index;

//	for (index=0;index < ciCTypes;index++)
//	{
		if (lpdftCTypes[idx].fOpt == SQL_C_DEFAULT)
			return(TRUE);
//	}

	return(FALSE);

} // IsSQLCDEFAULT()



//*------------------------------------------------------------------------
//|  ProcessfCtype:
//|		This function handles the fCType Drop Down
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessfCtype(HWND hDlg,SWORD *pfCType,
								  lpDFTARRAY lpCTypeDfts,
								  lpDFTARRAY lpSQLTypeDfts,
								  SWORD idxfCType,
								  SWORD idxfSQLType)
{
	TCHAR	szTemp[SMALLBUFF];
	SWORD	wDex=0,
			wDex2=0,
			idx=0;

	// Get the current fCType
	wDex = (SWORD)SendDlgItemMessage(hDlg,idxfCType,CB_GETCURSEL,0,0L);

	// Validate the SQL_C data type
	// Only print out error messages and reset focus if we are not in an IDOK state
	// otherwise the regular parameter checking stuff will catch it and we don't want
	// to print out errors twice
	if(GetFocus() != GetDlgItem(hDlg,IDOK))
	{
		if(wDex == CB_ERR)
		{
			GetDlgItemText(hDlg,idxfCType,szTemp,tsizeof(szTemp));
			if(ValidateTypeData((LPTSTR)szfCTypeTxt,SQL_C_SSHORT,FALSE,szTemp))
				*pfCType = (SWORD)_ttoi(szTemp);
		}
		else
		{
			// The current fCType is valid
			if(IsSQLCDEFAULT(wDex))
			{
				wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxfSQLType,CB_GETCURSEL,0,0L);

				if(wDex2 == CB_ERR)
				{
					GetDlgItemText(hDlg,idxfSQLType,szTemp,tsizeof(szTemp));
					*pfCType = GetCTypeFromSqlType((SWORD)_ttoi(szTemp),TRUE);
				}
				else
					*pfCType =GetCTypeFromSqlType((SWORD)lpSQLTypeDfts[wDex2].fOpt,TRUE);
			}
			else
				*pfCType = (SWORD)lpCTypeDfts[wDex].fOpt;
		}
	}

} //ProcessfCtype()



//*------------------------------------------------------------------------
//|  ProcessfSQLType:
//|		This function handles the fSQLType Drop Down
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessfSQLType(HWND hDlg,SWORD *pfCType,lpDFTARRAY lpDfts,
									 SWORD idxfCType,SWORD idxfSQLType)
{
	TCHAR	szTemp[SMALLBUFF];
	SWORD	wDex=0,
			wDex2=0;

	// If fCType is SQL_C_DEFAULT, use the fSqlType to determine the C Type of the data in rgbValue
	wDex = (SWORD)SendDlgItemMessage(hDlg,idxfCType,CB_GETCURSEL,0,0L);

	if(IsSQLCDEFAULT(wDex))
	{
		wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxfSQLType,CB_GETCURSEL,0,0L);
		// Only print out error messages and reset focus if we are not in an IDOK state
		// otherwise the regular parameter checking stuff will catch it and we don't want
		// to print out errors twice
		if(GetFocus() != GetDlgItem(hDlg,IDOK))
		{
			if(wDex2 == CB_ERR)
			{
				GetDlgItemText(hDlg,idxfSQLType,szTemp,tsizeof(szTemp));
				if(ValidateTypeData((LPTSTR)szfSqlType,SQL_C_SSHORT,FALSE,szTemp))
					*pfCType = GetCTypeFromSqlType((SWORD)_ttoi(szTemp),TRUE);
			}
			else
				*pfCType = GetCTypeFromSqlType((SWORD)lpDfts[wDex2].fOpt,TRUE);
		}
	}

} //ProcessfSQLType()

//*------------------------------------------------------------------------
//|  EnableDaySecondStruct:
//|		This function enables or disables the controls in the
//|	"Day-Second Structure" group box
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN EnableDaySecondStruct(HWND hDlg,BOOL fEnable)
{
	EnableWindow(GetDlgItem(hDlg,IDGB_DAY_SECOND_STRUCT), fEnable);

	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_DAY_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_HOUR_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_SECOND_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_FRACTION_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_MINUTE_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_DAY), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_HOUR), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_SECOND), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_FRACTION), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_MINUTE), fEnable);

} //EnableDaySecondStruct()

//*------------------------------------------------------------------------
//|  EnableYearMonthStruct:
//|		This function enables or disables the controls in the
//|	"Year-Month Structure" group box
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN EnableYearMonthStruct(HWND hDlg,BOOL fEnable)
{
	EnableWindow(GetDlgItem(hDlg,IDGB_YEAR_MONTH_STRUCT), fEnable);

	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_MONTH_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_YEAR_TXT), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_MONTH), fEnable);
	EnableWindow(GetDlgItem(hDlg,IDE_INTERVAL_YEAR), fEnable);

} //EnableYearMonthStruct()


//*------------------------------------------------------------------------
//|  GetIntervalInfoWndProc:
//|		This function handles the IDD_INTERVAL_STRUCT dialog box when
//|	<interval value> is selected for rgbValue
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

LRESULT EXTFUN GetIntervalInfoWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
		// Default values come from the sturcture itself
		case WM_INITDIALOG:
		{
			SQL_INTERVAL_STRUCT	*lpIS = (SQL_INTERVAL_STRUCT *)lParam;
			TCHAR						szHandle[MEDBUFF]=TEXT(""),
										szAddress[MEDBUFF]=TEXT("");
			HWND						hwndFocus=NULL;
			UINT						iIntTypes=0;
			HWND						hwndType = GetDlgItem(hDlg,IDCB_INTERVAL_TYPE),
										hwndSign = GetDlgItem(hDlg,IDCB_INTERVAL_SIGN);
			DWORD						dwDex=0;
			BOOL						fFound=FALSE;


			CenterDialog(hDlg);
			SETWINDOWLONG(hDlg, lpIS);

			//Load the Interval Type Combo box
			for (iIntTypes=0;iIntTypes < ciIntervalTypes;iIntTypes++)
				dwDex=(DWORD)SendMessage(hwndType, CB_INSERTSTRING, iIntTypes, (LPARAM)lpdftIntervalTypes[iIntTypes].szOpt);

			//get current year
			for (iIntTypes=0;iIntTypes < ciIntervalTypes;iIntTypes++)
			{
				if (lpIS->interval_type == lpdftIntervalTypes[iIntTypes].fOpt)
				{
					fFound=TRUE;
					break;
				}
			}

			//set current selection
			iIntTypes = fFound ? iIntTypes :0;
			SendMessage(hwndType,CB_SETCURSEL,iIntTypes,0L);

			//Disable "Day-Second Structure" group box and all of its fields
			//because the initial value of "Interval Type" is SQL_IS_YEAR
			EnableYearMonthStruct(hDlg,TRUE);

			EnableDaySecondStruct(hDlg,FALSE);

			//Load the Interval Sign Combo box
			for (iIntTypes=0;iIntTypes < ciSQLTrueFalse30;iIntTypes++)
				dwDex=(DWORD)SendMessage(hwndSign, CB_INSERTSTRING, iIntTypes, (LPARAM)lpdftSQLTrueFalse30[iIntTypes].szOpt);

			fFound=FALSE;

			for (iIntTypes=0;iIntTypes < ciSQLTrueFalse30;iIntTypes++)
			{
				if (lpIS->interval_sign == lpdftIntervalTypes[iIntTypes].fOpt)
				{
					fFound=TRUE;
					break;
				}
			}

			//set current selection
			iIntTypes = fFound ? iIntTypes :0;
			SendMessage(hwndSign,CB_SETCURSEL,iIntTypes,0L);

			//set the union field intval based upon the interval_type field
			switch(lpIS->interval_type)
			{
				case SQL_IS_YEAR_TO_MONTH:
				case SQL_IS_YEAR:
				case SQL_IS_MONTH:
					//initialize year-month structure
					SetDlgItemInt(hDlg,IDE_INTERVAL_YEAR,lpIS->intval.year_month.year,FALSE);
					SetDlgItemInt(hDlg,IDE_INTERVAL_MONTH,lpIS->intval.year_month.month,FALSE);
					break;
				case SQL_IS_DAY:
				case SQL_IS_HOUR:
				case SQL_IS_MINUTE:
				case SQL_IS_SECOND:
				case SQL_IS_DAY_TO_HOUR:
				case SQL_IS_DAY_TO_MINUTE:
				case SQL_IS_DAY_TO_SECOND:
				case SQL_IS_HOUR_TO_MINUTE:
				case SQL_IS_HOUR_TO_SECOND:
				case SQL_IS_MINUTE_TO_SECOND:
					//initialize day-second structure
					SetDlgItemInt(hDlg,IDE_INTERVAL_HOUR,lpIS->intval.day_second.hour,FALSE);
					SetDlgItemInt(hDlg,IDE_INTERVAL_DAY,lpIS->intval.day_second.day,FALSE);
					SetDlgItemInt(hDlg,IDE_INTERVAL_MINUTE,lpIS->intval.day_second.minute,FALSE);
					SetDlgItemInt(hDlg,IDE_INTERVAL_SECOND,lpIS->intval.day_second.second,FALSE);
					SetDlgItemInt(hDlg,IDE_INTERVAL_FRACTION,lpIS->intval.day_second.fraction,FALSE);
					break;
			}

		}
		return TRUE;
		case WM_COMMAND:
		{

			if (GET_WM_COMMAND_CODE(wParam, lParam)==CBN_SELCHANGE)
			{
				//get currently selected Interval Type and set appropriate field in the IS
				DWORD			wDex=(DWORD)SendMessage(GetDlgItem(hDlg,IDCB_INTERVAL_TYPE),CB_GETCURSEL,0,0L);
				SQLINTERVAL	interval_type=lpdftIntervalTypes[wDex].fOpt;

				//set the union field intval based upon the interval_type field
				switch(interval_type)
				{
					case SQL_IS_YEAR_TO_MONTH:
					case SQL_IS_YEAR:
					case SQL_IS_MONTH:
						//Disable "Day-Second Structure" group box all associated fields
						EnableDaySecondStruct(hDlg,FALSE);
						//Enable "Year-Month Structure" group box all associated fields
						EnableYearMonthStruct(hDlg,TRUE);
						break;
					case SQL_IS_DAY:
					case SQL_IS_HOUR:
					case SQL_IS_MINUTE:
					case SQL_IS_SECOND:
					case SQL_IS_DAY_TO_HOUR:
					case SQL_IS_DAY_TO_MINUTE:
					case SQL_IS_DAY_TO_SECOND:
					case SQL_IS_HOUR_TO_MINUTE:
					case SQL_IS_HOUR_TO_SECOND:
					case SQL_IS_MINUTE_TO_SECOND:
						//Enable "Day-Second Structure" group box all associated fields
						EnableDaySecondStruct(hDlg,TRUE);
						//Disable "Year-Month Structure" group box all associated fields
						EnableYearMonthStruct(hDlg,FALSE);
						break;
				}

			}

			switch(GET_WM_COMMAND_ID(wParam,lParam))
			{
				case IDOK:
				{
					UINT 						cbBytes=0;
					SQL_INTERVAL_STRUCT	*lpIS=NULL;
					DWORD						wDex=0;
					BOOL						fTranslated=FALSE;

					//Now load the Interval struct to pass to the driver
					lpIS = (SQL_INTERVAL_STRUCT *)GETWINDOWLONG(hDlg);

					//get currently selected Interval Type and set appropriate field in the IS
					wDex=(DWORD)SendMessage(GetDlgItem(hDlg,IDCB_INTERVAL_TYPE),CB_GETCURSEL,0,0L);
					lpIS->interval_type=lpdftIntervalTypes[wDex].fOpt;

					//get currently selected Interval Type and set appropriate field in the IS
					wDex=(DWORD)SendMessage(GetDlgItem(hDlg,IDCB_INTERVAL_SIGN),CB_GETCURSEL,0,0L);
					lpIS->interval_sign=(UWORD)lpdftSQLTrueFalse30[wDex].fOpt;

					//set the union field intval based upon the interval_type field
					switch(lpIS->interval_type)
					{
						case SQL_IS_YEAR_TO_MONTH:
						case SQL_IS_YEAR:
						case SQL_IS_MONTH:
							lpIS->intval.year_month.year=GetDlgItemInt(hDlg,IDE_INTERVAL_YEAR,&fTranslated, FALSE);
							lpIS->intval.year_month.month=GetDlgItemInt(hDlg,IDE_INTERVAL_MONTH,&fTranslated, FALSE);
							break;
						case SQL_IS_DAY:
						case SQL_IS_HOUR:
						case SQL_IS_MINUTE:
						case SQL_IS_SECOND:
						case SQL_IS_DAY_TO_HOUR:
						case SQL_IS_DAY_TO_MINUTE:
						case SQL_IS_DAY_TO_SECOND:
						case SQL_IS_HOUR_TO_MINUTE:
						case SQL_IS_HOUR_TO_SECOND:
						case SQL_IS_MINUTE_TO_SECOND:
							lpIS->intval.day_second.day=GetDlgItemInt(hDlg,IDE_INTERVAL_DAY,&fTranslated, FALSE);
							lpIS->intval.day_second.hour=GetDlgItemInt(hDlg,IDE_INTERVAL_HOUR,&fTranslated, FALSE);
							lpIS->intval.day_second.minute=GetDlgItemInt(hDlg,IDE_INTERVAL_MINUTE,&fTranslated, FALSE);
							lpIS->intval.day_second.second=GetDlgItemInt(hDlg,IDE_INTERVAL_SECOND,&fTranslated, FALSE);
							lpIS->intval.day_second.fraction=GetDlgItemInt(hDlg,IDE_INTERVAL_FRACTION,&fTranslated, FALSE);
							break;
					}

					//if user didn't enter a name use address as name
					EndDialog(hDlg, IDOK);
				}
				return TRUE;

            case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
			}

			if(GET_WM_COMMAND_CODE(wParam, lParam) ==	CBN_DBLCLK)
				SendMessage(hDlg, WM_COMMAND, IDOK, 0L);

		}
		return FALSE;

		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
	}

	return FALSE;

} //GetDescNameWndProc()





//*------------------------------------------------------------------------
//|  ProcessKillFocusRgbValue:
//|		This function handles the USER_CBN_KILL_FOCUS message for the
//|	rgbValue drop down
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessKillFocusRgbValue(HWND hDlg,lpDIALOGDESC lpDlg,
	lpCONNECTIONINFO lpCiList,SWORD idxpcbValue,SWORD idxrgbValue,
	SWORD idxRSHdbc,SWORD idxRSHstmt,SWORD idxRSCol,SWORD idxRSRow,
	SWORD idxDAE,SWORD idxRSHdbcTxt,SWORD idxRSHstmtTxt,
	SWORD idxRSColTxt,SWORD idxRSRowTxt,SWORD iparrgbValue,
	SWORD iparpcbValue)
{
	BOOL	fFound = FALSE;
	SWORD wDex=0,
			wDex2=0,
			iCurPcb,
			iCurRgb;
	lpPARAMITEM		lpParm = lpDlg->lpParms[iparrgbValue],
						lpPcbValueParm=lpDlg->lpParms[iparpcbValue];

	wDex = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETCURSEL,0,0L);
	iCurPcb = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETITEMDATA,wDex,0L);
	wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETCURSEL,0,0L);
	iCurRgb = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETITEMDATA,wDex2,0L);

	if (iCurRgb == INTERVALVAL)
	{
		//<interval value> was selected, display the Interval Struct Dialog Box
		SWORD						cbIS=sizeof(SQL_INTERVAL_STRUCT);
		SQL_INTERVAL_STRUCT	*pIS=(SQL_INTERVAL_STRUCT *)AllocateMemory(cbIS);
		TCHAR						szPcbValue[MEDBUFF]=TEXT("");
		DWORD						dwDex=0;

		memset(pIS,0,cbIS);

		// First get the connection info, if they cancel, nothing to do
		DialogBoxParam(hInst,
						MAKEINTRESOURCE(IDD_INTERVAL_STRUCT),
						hDlg,
						GetIntervalInfoWndProc,
						(LPARAM)pIS);

		FREE_PTR(lpParm->lpData);

		//set rgbValue
		lpParm->lpData=pIS;
		//set pcbValue
		lpParm->cBytes=cbIS;

		//set pcbValue
		dwDex=(SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_ADDSTRING,0,(LPARAM)_itot(cbIS,szPcbValue,10));
		//select current pcbValue
		wDex2=(SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_SETCURSEL,dwDex,0L);
		SetFocus(GetDlgItem(hDlg,idxpcbValue));

	}
	else if ((iCurPcb == ROWSETVAL) || (iCurRgb == ROWSETVAL))
	{
		// <rowset value> was selected, update the rowset controls
		// based on whether or not a valid rowset is present somewhere

		if(lpConnWin)
		{
			// Check to make sure we have an hdbc with bound memory
			if(CheckHdbcForRowsets(lpCiList))
				fFound = TRUE;

			lpCiList = lpCiList->next;

			while(lpCiList != lpConnWin)
			{
				if(CheckHdbcForRowsets(lpCiList))
					fFound = TRUE;
				lpCiList = lpCiList->next;
			}
		}

		// If we found one, fFound will be TRUE by
		// the time it gets here
		if(fFound)
		{
			EnableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
		}
		else
		{
			DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
		}
	}
	else
	{
		DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);

		if (wDex2 == CB_ERR && (wDex != iCurPcb))
		{
			// Any user-entered value will cause a calculated length to appear in the pcbValue
			// combo box, so we should enable the macro checkbox
			EnableWindow(GetDlgItem(hDlg,idxDAE),TRUE);
		}
	}

} //ProcessKillFocusRgbValue()



//*------------------------------------------------------------------------
//|  ProcessKillFocusPcbValue:
//|		This function handles the USER_CBN_KILL_FOCUS message for the
//|	pcbValue drop down
//|
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN ProcessKillFocusPcbValue(HWND hDlg,lpDIALOGDESC lpDlg,
	lpCONNECTIONINFO lpCiList,SWORD idxpcbValue,SWORD idxrgbValue,
	SWORD idxRSHdbc,SWORD idxRSHstmt,SWORD idxRSCol,SWORD idxRSRow,
	SWORD idxDAE,UINT cDex,SWORD idxHstmt,SWORD idxcbValueMax,
	SWORD idxRSHdbcTxt,SWORD idxRSHstmtTxt,SWORD idxRSColTxt,SWORD idxRSRowTxt)
{
	lpCONTROL	lpCtl = lpDlg->lpCtl[cDex];
	BOOL			fFound = FALSE;
	BOOL			fError = FALSE;
	HWND			hwndFocus;
	HWND			hwndOK;
	HWND			hwndCancel;
	SWORD			wDex=0,
					wDex2=0;
	SDWORD		iCurPcb,
					iCurRgb;

	wDex = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETCURSEL,0,0L);
	iCurPcb = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETITEMDATA,wDex,0L);
	wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETCURSEL,0,0L);
	iCurRgb = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETITEMDATA,wDex2,0L);

	// Check to make sure the focus hasn't gone back to the cbValueMax combo box
	// If it has, we don't want to do any of the set focus calls in the section because
	// all of the SetFocus calls are assuming the user hit Tab from the pcbValue combo box
	hwndFocus = GetFocus();
	hwndOK = GetDlgItem(hDlg,IDOK);
	hwndCancel = GetDlgItem(hDlg,IDCANCEL);

	// If pcbValue is a numeric value, set the focus to the macro checkbox
	if((wDex == CB_ERR) || (wDex == (SWORD)(SendDlgItemMessage(hDlg,lpCtl->wID,CB_GETCOUNT,0,0L)-1)))
	{
		;
	}
	else
	{
		if(hwndFocus == 0)		// The focus was moved to the macro checkbox which was disabled
			SetFocus(GetDlgItem(hDlg, idxHstmt));

		// If rgbValue or pcbValue contain <rowset value> (and pcbValue is not a numeric value)
		// set the focus to the rowset hdbc combo box, otherwise set it to the hstmt checkbox
		// Also check to see if <rowset value> was selected.  If it was, update the rowset controls
		// based on whether or not a valid rowset is present somewhere
		if((iCurPcb == ROWSETVAL) || (iCurRgb == ROWSETVAL))
		{
			if(lpConnWin)
			{
				// Check to make sure we have an hdbc with bound memory
				if(CheckHdbcForRowsets(lpCiList))
					fFound = TRUE;

				lpCiList = lpCiList->next;

				while(lpCiList != lpConnWin)
				{
					if(CheckHdbcForRowsets(lpCiList))
						fFound = TRUE;
					lpCiList = lpCiList->next;
				}
			}

			// If we found one, fFound will be TRUE by the time it gets here
			if(fFound)
			{
				EnableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,
										idxRSCol,idxRSRow,idxRSHdbcTxt,idxRSHstmtTxt,
										idxRSColTxt,idxRSRowTxt);

				if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))
					if(!IsWindowEnabled(hwndFocus))
						SetFocus(GetDlgItem(hDlg,idxRSHdbc));
			}
			else
			{
				fError = TRUE;

				if((wDex == CB_ERR) || (wDex == (SWORD)(SendDlgItemMessage(hDlg,lpCtl->wID,CB_GETCOUNT,0,0L)-1)))
				{
					if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))
						if(!IsWindowEnabled(hwndFocus))
							SetFocus(GetDlgItem(hDlg,idxDAE));
				}
				else
				{
					if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))
						if(!IsWindowEnabled(hwndFocus))
							SetFocus(GetDlgItem(hDlg,idxHstmt));
				}

				DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
			}
		}
		else
		{
			DisableBPRowsetControls(hDlg,idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,
										idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);

			// if we have a valid hstmt
			if((lpDlg->ci) && (lpDlg->ci->lpCurStmt))
			{
				if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))
				{
					if(!IsWindowEnabled(hwndFocus))
						SetFocus(GetDlgItem(hDlg,idxHstmt));			// go to the hstmt checkbox
				}
			}
			else
			{
				if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))
				{
					if(!IsWindowEnabled(hwndFocus))
						SetFocus(GetDlgItem(hDlg,IDOK));						// otherwise go to the OK button

				} //if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))

			} //if((lpDlg->ci) && (lpDlg->ci->lpCurStmt))

		} //if((wDex == DEX_BP_ROWSET_VALUE) || (wDex2 == DEX_BP_ROWSET_VALUE))
	}

	// Now check to see if the user had pressed Shift-Tab from the pcbValue combo box
	// If they did, hwndFocus should have it's handle and we need to reset the focus to it
	// since we have changed the focus in our blind assumptions above
	if (idxcbValueMax != -1)
	{
		if((!fError) && (hwndFocus == GetDlgItem(hDlg,idxcbValueMax)))
			if((hwndFocus != hwndOK) && (hwndFocus != hwndCancel))
				SetFocus(hwndFocus);
	}

} //ProcessKillFocusPcbValue()



//*------------------------------------------------------------------------
//|  ProcessKillFocusPcbValue:
//|		This function handles the USER_CBN_KILL_FOCUS message for the
//|	pcbValue drop down
//|
//|	Returns:
//|		TRUE or FALSE
//*------------------------------------------------------------------------

BOOL INTFUN ProcessKillFocusRowSetHDBC(HWND hDlg,lpCONTROL lpCtl,
					UINT idxRSHdbc, lpCONNECTIONINFO lpCiList,SWORD idxRSHstmt)
{
	lpSTATEMENTINFO	lpSi=NULL;
	HWND					hwndCtl = GetDlgItem(hDlg,lpCtl->wID);
	SDWORD				dwDex;
	UINT					iConnNum;
	BOOL					fFound=FALSE;


	// We need to go through the linked list of hdbc's and find the one that was selected
	dwDex = (DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0,0L);

	// Make sure the user selected one out of the list. If not, put up a message box and make
	// the user change it.  Don't let him out of the control until he selects a valid one
	if(dwDex == CB_ERR)
	{
		SendMessage(hwndCtl,CB_SETCURSEL,0,0L);
		return TRUE;
	}

	// Everything beyond here assumes we have a valid hdbc with a rowset
	iConnNum = (UINT)SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)dwDex,0L);

	if(iConnNum == lpCiList->uConNum)
		fFound = TRUE;

	if(!fFound)
	{
		lpCiList = lpCiList->next;

		while(lpCiList != lpConnWin)
		{
			if(iConnNum == lpCiList->uConNum)
			{
				fFound = TRUE;
				break;
			}

			lpCiList = lpCiList->next;
		}
	}

	// We need to go through the linked list of hstmt's checking for ones that have a valid rowset pending
	// Now we have a pointer to the connection, now traverse the linked list of hstmts
	// and put them in the HSTMT combo list box
	hwndCtl = GetDlgItem(hDlg,idxRSHstmt);
	SendMessage(hwndCtl,CB_RESETCONTENT,0,0L);

	if (fFound)
	{

		if (lpCiList->lpStmt)
			AddHstmtToDropDown(hwndCtl,NULL,lpCiList,lpCiList->lpStmt,NULL);

		// If we had any valid hstmt's, set the current selection to the first one
		if(SendMessage(hwndCtl,CB_GETCOUNT,0,0L))
			SendMessage(hwndCtl,CB_SETCURSEL,0,0L);

	} //if (fFound)

    return TRUE;
} //ProcessKillFocusRowSetHDBC()




//*------------------------------------------------------------------------
//|  ProcessIDOK:
//|		This function handles the IDOK message
//|
//|	Returns:
//|		TRUE or FALSE
//*------------------------------------------------------------------------

BOOL INTFUN ProcessIDOK(HWND hDlg,lpDIALOGDESC lpDlg,lpCONNECTIONINFO lpCiList,
			SWORD idxRSRow,SWORD idxpcbValue,SWORD idxrgbValue,SWORD idxRSHdbc,
			SWORD idxRSHstmt,SWORD idxRSCol,SWORD idxcbValueMax,SWORD iParcbColDef,
			SWORD iParibScale,SWORD idxcbColDef,
			SWORD idxibScale,SWORD iParrgbValue)
{
	UINT					ihdbc;
	UINT					ihstmt;
	UDWORD				icol;
	UDWORD				irow;
	SDWORD				cbValueMax;
	lpSTATEMENTINFO	lpSiList;
	BOOL					fFound = FALSE;
	lpBOUNDVARS			lpBV;
	BOOL					fFoundCol=FALSE;
	UINT					idx;
	SDWORD				dwDex,
							wDex,
							wDex2,
							idxCurPcb,
							idxCurRgb;
	LPTSTR				str = NULL;
	TCHAR					szTemp[MEDBUFF];
	lpCONNECTIONINFO	lpci = lpDlg->ci;
	lpSTATEMENTINFO	lpCurStmt=lpci ? lpci->lpCurStmt : NULL;
	SQL_NUMERIC_STRUCT *lpNS=NULL;
	lpPARAMITEM			*lpParms = lpDlg->lpParms;


// First check and see if <rowset value> is selected in rgbValue or pcbValue.
	wDex = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETCURSEL,0,0L);
	idxCurPcb = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETITEMDATA,wDex,0L);
	wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETCURSEL,0,0L);
	idxCurRgb = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETITEMDATA,wDex2,0L);

	//Set up for SQL_C_NUMERIC parameter
	if (lpParms[iParrgbValue] && lpParms[iParrgbValue]->fCType == SQL_C_NUMERIC)
	{
		//get precision of SQL_C_NUMERIC
		GetWindowText(GetDlgItem(hDlg,idxcbColDef),szTemp,tsizeof(szTemp));
		lpParms[iParrgbValue]->uwPrecision=(UWORD)_ttoi(szTemp);

		//get scale of SQL_C_NUMERIC
		GetWindowText(GetDlgItem(hDlg,idxibScale),szTemp,tsizeof(szTemp));
		lpParms[iParrgbValue]->uwScale=(UWORD)_ttoi(szTemp);
	}

	if((idxCurPcb != ROWSETVAL) && (idxCurRgb != ROWSETVAL))
		return TRUE;

	if(lpConnWin)
	{
		// Check to make sure we have an hdbc with bound memory
		if(CheckHdbcForRowsets(lpCiList))
			fFound = TRUE;

		lpCiList = lpCiList->next;

		while(lpCiList != lpConnWin)
		{
			if(CheckHdbcForRowsets(lpCiList))
				fFound = TRUE;
			lpCiList = lpCiList->next;
		}
	}

	// If we found one, fFound will be TRUE by
	// the time it gets here
	if(!fFound)
	{
		szMessageBox(hDlg,MB_ICONINFORMATION | MB_OK, (LPTSTR)szRgbAndPcb,
						 GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsNoRowsetsAvailable));
		return FALSE;
	}

	// Get hdbc, hstmt, row, and col from the controls
	dwDex = (DWORD)SendMessage(GetDlgItem(hDlg,idxRSHdbc),CB_GETCURSEL,0,0L);
	if(dwDex == CB_ERR)
		return TRUE;

	ihdbc = (UINT)SendMessage(GetDlgItem(hDlg,idxRSHdbc),CB_GETITEMDATA,(WPARAM)dwDex,0L);
	dwDex = (DWORD)SendMessage(GetDlgItem(hDlg,idxRSHstmt),CB_GETCURSEL,0,0L);
	ihstmt = (UINT)SendMessage(GetDlgItem(hDlg,idxRSHstmt),CB_GETITEMDATA,(WPARAM)dwDex,0L);

	GetDlgItemText(hDlg,idxRSCol,szTemp,15);
	icol = (UDWORD)_ttol(szTemp);
	GetDlgItemText(hDlg,idxRSRow,szTemp,15);
	irow = (UDWORD)_ttol(szTemp);

	// We need to go through the linked list of hdbc's and find the one that was selected
	while(lpCiList->uConNum != ihdbc)
	   lpCiList = lpCiList->next;

	lpSiList = lpCiList->lpStmt;
	while(lpSiList->uStmtNum != ihstmt)
		lpSiList = lpSiList->next;

	// Now that we have the correct hstmt, determine whether it uses column or row-wise binding
	// Check to make sure we are still within our rowset size
	if((irow == 0) || (irow > lpSiList->uRowSetSize) ||
		(lpCurStmt->fParamOpt && ((irow + lpci->lpCurStmt->cParamRows - 1) > lpSiList->uRowSetSize)))
	{
			szMessageBox(hDlg,MB_ICONINFORMATION | MB_OK, (LPTSTR)szRowsetRow,
							 GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidRowsetRow));
			SetFocus(GetDlgItem(hDlg,idxRSRow));
			SendDlgItemMessage(hDlg,idxRSRow,EM_SETSEL,(WPARAM)0,MAKELPARAM(0,-1));

			return FALSE;
	}

	// We need to find the correct column
	lpBV = lpSiList->lpVars;

	for(idx=0;idx<lpSiList->uBoundCols;idx++)
	{
		if(lpBV->icol == (UWORD)icol)
		{

			if (idxcbValueMax == -1)
				cbValueMax=300;
			else
			{
				// Now we need to make sure that the cbValueMax is <= the buffer size
				GetDlgItemText(hDlg,idxcbValueMax,szTemp,15);
				cbValueMax = (SDWORD)_ttol(szTemp);
			}

			if(cbValueMax > lpBV->cbElement)
			{
				szMessageBox(hDlg,MB_ICONINFORMATION | MB_OK, (LPTSTR)szRowsetColumn,
								 GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidcbValueMax));
				SetFocus(GetDlgItem(hDlg,idxcbValueMax));
				SendDlgItemMessage(hDlg,idxcbValueMax,EM_SETSEL,(WPARAM)0,MAKELPARAM(0,-1));

				return FALSE;
			}

			fFoundCol = TRUE;
			break;
		}
		lpBV = lpBV->next;
	}

	if(!fFoundCol)
	{
		szMessageBox(hDlg,MB_ICONINFORMATION | MB_OK, (LPTSTR)szRowsetColumn,
						 GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidRowsetColumn));
		SetFocus(GetDlgItem(hDlg,idxRSCol));
		SendDlgItemMessage(hDlg,idxRSCol,EM_SETSEL,(WPARAM)0,MAKELPARAM(0,-1));
		return FALSE;
	}

	return TRUE;

} //ProcessIDOK()


//*------------------------------------------------------------------------
//|  BindParam:
//|		This function is shared between SQLBindParameter and SQLBindParam.
//|
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------

BOOL INTFUN BindParam(UINT msg,
							HWND hDlg,
							lpDIALOGDESC lpDlg,
							UINT cDex,
							SWORD idxHstmt,
							SWORD idxipar,
							SWORD idxfParamType,
							SWORD idxfCType,
							SWORD idxfSQLType,
							SWORD idxcbColDef,
							SWORD idxibScale,
							SWORD idxrgbValue,
							SWORD idxcbValueMax,
							SWORD idxpcbValue,
							SWORD idxDAE,
							SWORD idxRSHdbc,
							SWORD idxRSHstmt,
							SWORD idxRSCol,
							SWORD idxRSRow,
							SWORD	idxcbBuffSize,
							SWORD idxRSHdbcTxt,
							SWORD idxRSHstmtTxt,
							SWORD idxRSColTxt,
							SWORD idxRSRowTxt)
{

	lpCONTROL			lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM			lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	SWORD					wDex,wDex2,wDex3;
	TCHAR					szTemp[SMALLBUFF];
	lpCONNECTIONINFO	lpCiList=lpConnWin;
	lpCONNECTIONINFO	lpci = lpDlg->ci;
	SWORD	iParHstmt,
			iParipar,
			iParfParamType,
			iParfCType,
			iParfSqlType,
			iParcbColDef,
			iParibScale,
			iParrgbValue,
			iParcbValueMax,
			iParpcbValue,
			iParfDAE,
			iParRSHdbc,
			iParRSHstmt,
			iParRSCol,
			iParRSRow,
			iParcbBuffSize;

	if (idxfParamType == -1)
	{
		//set for SQLBindParam
		iParHstmt		= 	IPAR_BPARAM_HSTMT;
		iParipar			= 	IPAR_BPARAM_IPAR;
		iParfCType		= 	IPAR_BPARAM_FCTYPE;
		iParfSqlType	= 	IPAR_BPARAM_FSQLTYPE;
		iParcbColDef	= 	IPAR_BPARAM_CBCOLDEF;
		iParibScale		= 	IPAR_BPARAM_IBSCALE;
		iParrgbValue	= 	IPAR_BPARAM_RGBVALUE;
		iParpcbValue	= 	IPAR_BPARAM_PCBVALUE;
		iParfDAE			= 	IPAR_BPARAM_DAE;
		iParRSHdbc		= 	IPAR_BPARAM_RS_HDBC;
		iParRSHstmt		= 	IPAR_BPARAM_RS_HSTMT;
		iParRSCol		= 	IPAR_BPARAM_COLUMN;
		iParRSRow		= 	IPAR_BPARAM_ROW;
		iParcbBuffSize = 	IPAR_BPARAM_BUFFERSIZE;
	}
	else
	{
		//set for SQLBindParameter
		iParHstmt		= 	 IPAR_BP_HSTMT;
		iParipar			= 	 IPAR_BP_IPAR;
		iParfParamType	= 	 IPAR_BP_FPARAMTYPE;
		iParfCType		= 	 IPAR_BP_FCTYPE;
		iParfSqlType	= 	 IPAR_BP_FSQLTYPE;
		iParcbColDef	= 	 IPAR_BP_CBCOLDEF;
		iParibScale		= 	 IPAR_BP_IBSCALE;
		iParrgbValue	= 	 IPAR_BP_RGBVALUE;
		iParcbValueMax	= 	 IPAR_BP_CBVALUEMAX;
		iParpcbValue	= 	 IPAR_BP_PCBVALUE;
		iParfDAE			= 	 IPAR_BP_DAE;
		iParRSHdbc		= 	 IPAR_BP_RS_HDBC;
		iParRSHstmt		= 	 IPAR_BP_RS_HSTMT;
		iParRSCol		= 	 IPAR_BP_COLUMN;
		iParRSRow		= 	 IPAR_BP_ROW;
		iParcbBuffSize = 	 IPAR_BP_BUFFERSIZE;
	}

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			switch(lpCtl->wID)
			{
				case IDE_BP_IPAR:
				case IDE_BINDP_IPAR:
				{
					lpSTATEMENTINFO lpStmt=NULL;

					if (lpci && lpci->lpCurStmt)
					{
						lpStmt=lpci->lpCurStmt;

						if(lpStmt->lpParams)
						{
							wsprintf(szTemp, TEXT("%u"), lpStmt->lpParams->ipar);
							SetDlgItemText(hDlg,lpCtl->wID,szTemp);
						}
					}
				}
				break;

				case IDCB_BP_RGBVALUE:
				case IDCB_BINDP_RGBVALUE:
					ProcessRgbValue(hDlg,lpDlg,iParfCType,iParrgbValue,idxRSHdbc,idxRSHstmt,
										idxRSCol,idxRSRow,idxRSHdbcTxt,idxRSHstmtTxt,
										idxRSColTxt,idxRSRowTxt);

				break;

				case IDCB_BINDP_PCBVALUE:
				case IDCB_BP_PCBVALUE:
					if (lpParm->dwDex == GetPcbIndex(NULLPTR))
					{
						CheckDlgButton(hDlg,idxDAE,FALSE);
						EnableWindow(GetDlgItem(hDlg,idxDAE),FALSE);
					}
					else
					{
						EnableWindow(GetDlgItem(hDlg,idxDAE),TRUE);
					}

					break;

				case IDCB_BP_ROWSET_HDBC:
				case IDCB_BINDP_ROWSET_HDBC:

					if(lpConnWin)
						ProcessRowSetHDBC(GetDlgItem(hDlg,lpCtl->wID),lpCiList);

					break;

				case IDCB_BP_ROWSET_HSTMT:
				case IDCB_BINDP_ROWSET_HSTMT:

					if(!lpCiList)
						return FALSE;

					ProcessRowSetHSTMT(hDlg,lpCtl,idxRSHdbc,lpCiList);

					break;

				case IDE_BP_ROW:
				case IDE_BINDP_ROW:
					ProcessRowSetRow(hDlg,lpCtl,lpDlg,iParrgbValue,iParpcbValue,
						idxipar,iParfDAE,idxDAE,idxRSCol,idxRSRow,idxRSHdbc,idxRSHstmt,
						idxfParamType,	idxfCType,idxfSQLType,idxcbColDef,idxibScale,
						idxcbValueMax,	idxrgbValue, idxpcbValue,
						idxRSColTxt,idxRSRowTxt,idxRSHdbcTxt,idxRSHstmtTxt);
					break;

				default:
					break;
				}
			}
			break;

		case USER_CBN_UPDATE_CONTENTS:
	   	{
	   	switch(lpCtl->wID){
	   		case IDCB_BP_FCTYPE:
				case IDCB_BINDP_FCTYPE:
					ProcessfCtype(hDlg,&(lpDlg->lpParms[iParrgbValue]->fCType),
						lpDlg->lpParms[iParfCType]->lpDfts,
						lpDlg->lpParms[iParfSqlType]->lpDfts,
						idxfCType,idxfSQLType);
					break;

				case IDCB_BP_FSQLTYPE:
				case IDCB_BINDP_FSQLTYPE:
					ProcessfSQLType(hDlg, &(lpDlg->lpParms[iParrgbValue]->fCType),
							lpDlg->lpParms[iParfSqlType]->lpDfts,
							idxfCType,idxfSQLType);

					break;

				case IDCB_BP_RGBVALUE:
				case IDCB_BINDP_RGBVALUE:
					{
					SWORD uTSIndex=GetCTypeIndex(USE_C_DFT_ARRAY,szSQL_C_TIMESTAMP),
							uDefIndex=GetCTypeIndex(USE_C_DFT_ARRAY,szSQL_C_DEFAULT),
							uSQLTSIndex=GetSQLTypeIndex(szSQL_TIMESTAMP,USE_C_DFT_ARRAY);

					// If fCType is SQL_C_TIMESTAMP, check for date or time only and pad the other portion with zero's
					wDex = (SWORD)SendDlgItemMessage(hDlg,idxfCType,CB_GETCURSEL,0,0L);
					wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxfSQLType,CB_GETCURSEL,0,0L);
					if((wDex == uTSIndex) || ((wDex == uDefIndex) && (wDex2 == uSQLTSIndex)))
						ValidateRgbValueTimestamp(hDlg,idxrgbValue);
					}
					break;

				case IDCB_BP_PCBVALUE:
				case IDCB_BINDP_PCBVALUE:
				{
					wDex = (SWORD)SendDlgItemMessage(hDlg,idxpcbValue,CB_GETCURSEL,0,0L);
					wDex2 = (SWORD)SendDlgItemMessage(hDlg,idxrgbValue,CB_GETCURSEL,0,0L);
					wDex3 = (SWORD)(SendDlgItemMessage(hDlg,lpCtl->wID,CB_GETCOUNT,0,0L)-1);

					// If pcbValue is not an item in the drop down, or is the calculated length item,
					// enable the SQL_LEN_DATA_AT_EXEC macro checkbox
					// If we are going to grey out the checkbox, make sure it is unchecked
					if (!((wDex == CB_ERR) || (wDex == wDex3)))
					{
						CheckDlgButton(hDlg,idxDAE,FALSE);
						EnableWindow(GetDlgItem(hDlg,idxDAE),FALSE);
					}
					else
					{
						EnableWindow(GetDlgItem(hDlg,idxDAE),TRUE);
					}
				}
				break;

				default:
					break;
				}
			}
			break;

		case USER_EN_KILL_FOCUS:
		{
			switch(lpCtl->wID)
			{
				case IDE_BP_IPAR:
				case IDE_BINDP_IPAR:
				{
					HWND	hwndFocus = GetFocus();


					// First check to see if the last call to set param was an error
					// If it was, don't use values out of the linked list as defaults, use
					// the ones stored in the parameter array (the invalid values from the last call)
					if(lpDlg->lpParms[iParrgbValue]->dwDex != DEX_BIND_PARAM_ERROR)
					{
						// First validate the data in ipar
						GetDlgItemText(hDlg,idxipar,szTemp,15);
						if(ValidateTypeData((LPTSTR)szipar,SQL_C_USHORT,FALSE,szTemp))
							// It is a valid SQL_C_USHORT
							if(lpDlg->lpParms[iParrgbValue]->lpData)
								UpdateBindParamControls(hDlg, lpCtl,lpDlg, (UWORD)_ttoi(szTemp),
																idxfParamType,
																idxfCType,
																idxfSQLType,
																idxcbColDef,
																idxibScale,
																idxcbValueMax,
																idxrgbValue,
																idxpcbValue,
																idxDAE,
																idxRSHdbc,
																idxRSHstmt,
																idxRSCol,
																idxRSRow,
																idxRSHdbcTxt,
																idxRSHstmtTxt,
																idxRSColTxt,
																idxRSRowTxt);
					}

					// Reset ipar's dwDex so that we update controls on the next update message
					if(lpDlg->lpParms[iParrgbValue]->dwDex == DEX_BIND_PARAM_ERROR)
						lpDlg->lpParms[iParrgbValue]->dwDex = -1;
				}
				break;

				default:
					break;
			}
		}
		break;

		case USER_CBN_KILL_FOCUS:
	   	{
	   	switch(lpCtl->wID)
			{
				case IDCB_BP_RGBVALUE:
				case IDCB_BINDP_RGBVALUE:
					ProcessKillFocusRgbValue(hDlg,lpDlg,lpCiList,idxpcbValue,idxrgbValue,
						idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,idxDAE,
						idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt,iParrgbValue,
						iParpcbValue);
				break;

				case IDCB_BP_PCBVALUE:
				case IDCB_BINDP_PCBVALUE:
					ProcessKillFocusPcbValue(hDlg,lpDlg,lpCiList,idxpcbValue,idxrgbValue,
						idxRSHdbc,idxRSHstmt,idxRSCol,idxRSRow,idxDAE,cDex,
						idxHstmt,idxcbValueMax,
						idxRSHdbcTxt,idxRSHstmtTxt,idxRSColTxt,idxRSRowTxt);
					break;

				case IDCB_BP_ROWSET_HDBC:
				case IDCB_BINDP_ROWSET_HDBC:
					if(!lpCiList)
						return FALSE;

					ProcessKillFocusRowSetHDBC(hDlg,lpCtl,idxRSHdbc,lpCiList,idxRSHstmt);

					break;

				case IDCB_BP_ROWSET_HSTMT:
				case IDCB_BINDP_ROWSET_HSTMT:
					{
					SDWORD	dwDex;
					HWND	hwndCtl = GetDlgItem(hDlg,lpCtl->wID);

					dwDex = (DWORD)SendMessage(hwndCtl,CB_GETCURSEL,0,0L);
					// Make sure the user selected one out of the list. If not, put up a message box and make
					// the user change it.  Don't let him out of the control until he selects a valid one
					if(dwDex == CB_ERR)
						SendMessage(hwndCtl,CB_SETCURSEL,0,0L);
					}
					break;

				default:
					break;
				}
			}
			break;

		case USER_IDOK:
			return(ProcessIDOK(hDlg,
							lpDlg,
							lpCiList,
							idxRSRow,
							idxpcbValue,
							idxrgbValue,
							idxRSHdbc,
							idxRSHstmt,
							idxRSCol,
							idxcbValueMax,
							iParcbColDef,
							iParibScale,
							idxcbColDef,
							idxibScale,
							iParrgbValue));

		default:
      	break;

	} //switch(msg)

	return TRUE;

} //BindParam()


//*------------------------------------------------------------------------
//|  BindParameterWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN BindParameterWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{

	return(BindParam(msg,
				hDlg,
				(lpDIALOGDESC)lpUsr,
				cDex,
				IDCK_BINDP_HSTMT,
				IDE_BINDP_IPAR,
				IDCB_BINDP_FPARAMTYPE,
				IDCB_BINDP_FCTYPE,
				IDCB_BINDP_FSQLTYPE,
				IDE_BINDP_CBCOLDEF,
				IDE_BINDP_IBSCALE,
				IDCB_BINDP_RGBVALUE,
				IDE_BINDP_CBVALUEMAX,
				IDCB_BINDP_PCBVALUE,
				IDCK_BINDP_SQL_LEN_DATA_AT_EXEC,
				IDCB_BINDP_ROWSET_HDBC,
				IDCB_BINDP_ROWSET_HSTMT,
				IDE_BINDP_COLUMN,
				IDE_BINDP_ROW,
				IDE_BINDP_BUFFER_SIZE,
				IDCB_BINDP_ROWSET_HDBC_TXT,
				IDCB_BINDP_ROWSET_HSTMT_TXT,
				IDE_BINDP_COLUMN_TXT,
				IDE_BINDP_ROW_TXT));

	}

//*------------------------------------------------------------------------
//|  BindParamWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN BindParamWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	return(BindParam(msg,
				hDlg,
				(lpDIALOGDESC)lpUsr,
				cDex,
				IDCB_BP_HSTMT,
				IDE_BP_IPAR,
				-1,
				IDCB_BP_FCTYPE,
				IDCB_BP_FSQLTYPE,
				IDE_BP_CBCOLDEF,
				IDE_BP_IBSCALE,
				IDCB_BP_RGBVALUE,
				-1,
				IDCB_BP_PCBVALUE,
				IDCK_BP_SQL_LEN_DATA_AT_EXEC,
				IDCB_BP_ROWSET_HDBC,
				IDCB_BP_ROWSET_HSTMT,
				IDE_BP_COLUMN,
				IDE_BP_ROW,
				IDE_BP_BUFFER_SIZE,
				IDCB_BP_ROWSET_HDBC_TXT,
				IDCB_BP_ROWSET_HSTMT_TXT,
				IDE_BP_COLUMN_TXT,
				IDE_BP_ROW_TXT));

} //BindParamWndProc()



//*------------------------------------------------------------------------
//|  PutDataWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN PutDataWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM		lpParm = lpDlg->lpParms[lpCtl->iParmNum];

	if (msg == WM_INITDIALOG && lpCtl->wID == IDCB_PUT_RGBVALUE)		// InitDialog message for the prgbValue combo box
		// Set the default C Type for prgbValue combo box according to the type set by SQLBindParameter
		if((lpDlg->ci) && (lpDlg->ci->lpCurStmt))
			lpDlg->lpParms[1]->fCType = lpDlg->ci->lpCurStmt->ParamDataCType;
		else
		{
			if (fUnicode)
				lpDlg->lpParms[1]->fCType = SQL_C_WCHAR;
			else
				lpDlg->lpParms[1]->fCType = SQL_C_CHAR;
		}

	return TRUE;
}


//*------------------------------------------------------------------------
//|  FillParamWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN FillParamWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONNECTIONINFO	lpci = lpDlg->ci;
	lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM		lpParm;
	lpPARAMETERINFO	lpParam=NULL;
	TCHAR				szTemp[100];
	BOOL				fErr;
	UWORD				ipartmp;
	SWORD				wDex;
	UDWORD			udRowTmp;

	if((lpCtl->iParmNum) != -1)	// Make sure the control is not a static text control or we will crash
		lpParm = lpDlg->lpParms[lpCtl->iParmNum];		// Then select the correct parameter structure for the control

	switch(msg){
		case WM_INITDIALOG:
			{
			switch(lpCtl->wID){
				case IDC_FP_PCBVALUE:
					FormatHandle(szTemp, FALSE, PT_HSTMT, TEXT(""), NULL, NULL,
									&lpci->lpCurStmt->hstmt, &lpci->lpDesc->hdesc,
									lpci->lpCurStmt->uStmtNum,NULL);
					SetWindowText(GetDlgItem(hDlg,IDC_FP_HSTMT),szTemp);
					wsprintf(szTemp,TEXT("%u"),lpci->lpCurStmt->lpParams->ipar);
					SetWindowText(GetDlgItem(hDlg,IDC_FP_IPAR),szTemp);
					SetWindowText(GetDlgItem(hDlg,IDC_FP_ROW),TEXT("1"));
					UpdateFillParamControls(hDlg, lpDlg, lpci->lpCurStmt->lpParams, 1);
					// Reset buttons
					SetDlgItemText(hDlg, IDOK, (LPTSTR)szUPDATE);
					SetDlgItemText(hDlg, IDCANCEL, (LPTSTR)szCLOSE);
				   break;
				}
			}
			break;

		case USER_EN_UPDATE_CONTENTS:
			{
			switch(lpCtl->wID){
				case IDC_FP_IPAR:
				case IDC_FP_ROW:
					SetStatBarText(hwndStatBar, (LPTSTR)szNullTerm, BLACK_TEXT);	// Clear the Status Bar
					GetDlgItemText(hDlg,IDC_FP_IPAR,szTemp,tsizeof(szTemp));	// See what is in the ipar edit box
					fErr=ValidateTypeData((LPTSTR)szipar,SQL_C_USHORT,FALSE,szTemp);	// Validate the edit box value
					if(!fErr)
						// ipar was invalid, so clear static text fields & disable controls
						DisableFillParamControls(hDlg, lpDlg);
					else{	// ipar value was a valid parameter number now see if it is in the linked list
						ipartmp = (UWORD)_ttoi(szTemp);
						lpParam = SearchParamList(lpci->lpCurStmt, ipartmp);
						if(lpParam){	// The parameter was found in the linked list, so update the controls to this parameter
							// Now check the current Row
							if(lpci->lpCurStmt->cParamRows > 1){
								GetDlgItemText(hDlg,IDC_FP_ROW,szTemp,tsizeof(szTemp));	// See what is in the Row edit box
								fErr=ValidateTypeData((LPTSTR)szRowText,SQL_C_ULONG,FALSE,szTemp);	// Validate the edit box value
								if(fErr){
									udRowTmp = (UDWORD)_ttol(szTemp);
									if((udRowTmp != 0) && (udRowTmp <= lpci->lpCurStmt->cParamRows))
										UpdateFillParamControls(hDlg, lpDlg, lpParam, udRowTmp);
									else
										DisableFillParamControls(hDlg, lpDlg);
									}
								else
									DisableFillParamControls(hDlg, lpDlg);
								}
							else
								UpdateFillParamControls(hDlg, lpDlg, lpParam, 1);
							}
						else
							// parameter wasn't found in the list, so clear static text fields & disable controls
							DisableFillParamControls(hDlg, lpDlg);
						}
					break;
				}
			}
			break;

		case USER_CBN_UPDATE_CONTENTS:
			{
			switch(lpCtl->wID){
				case IDC_FP_PCBVALUE:
					{
					HWND	hwndFocus = GetFocus();
					HWND	hwndMacro = GetDlgItem(hDlg,IDC_FP_MACRO);

					wDex = (SWORD)SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_GETCURSEL,0,0L);

					// If pcbValue is not an item in the drop down, or is the calculated length item,
					// enable the SQL_LEN_DATA_AT_EXEC macro checkbox
					if(!((wDex == CB_ERR) || (wDex == (SWORD)(SendDlgItemMessage(hDlg,lpCtl->wID,CB_GETCOUNT,0,0L)-1)))){						// If we are going to grey out the checkbox, make sure it is unchecked
						CheckDlgButton(hDlg,IDC_FP_MACRO,FALSE);
				      EnableWindow(GetDlgItem(hDlg,IDC_FP_MACRO),FALSE);
				      if(hwndFocus == hwndMacro)
				      	SetFocus(GetDlgItem(hDlg,IDOK));
				      }
					else{
				      EnableWindow(hwndMacro,TRUE);
						}
					}
				}
			}
			break;

		case USER_EN_KILL_FOCUS:
			{
			switch(lpCtl->wID){
				case IDC_FP_RGBVALUE:
					if(lpParm->fCType == SQL_C_TIMESTAMP)
						ValidateRgbValueTimestamp(hDlg,IDC_FP_RGBVALUE);
					// Enable the SQL_LEN_DATA_AT_EXEC macro because calculated length from rgbValue
					// will always put a valid number in pcbValue
			      EnableWindow(GetDlgItem(hDlg,IDC_FP_MACRO),TRUE);
				   break;
				}
			}
			break;

		case USER_CBN_KILL_FOCUS:
			{
			switch(lpCtl->wID){
				case IDC_FP_PCBVALUE:
					{
					HWND	hwndFocus = GetFocus();
					HWND	hwndOK = GetDlgItem(hDlg,IDOK);
					HWND	hwndMacro = GetDlgItem(hDlg,IDC_FP_MACRO);
					HWND	hwndrgb = GetDlgItem(hDlg,IDC_FP_RGBVALUE);
					HWND	hwndpcb = GetDlgItem(hDlg,IDC_FP_PCBVALUE);

					if(hwndFocus == GetDlgItem(hDlg,IDC_FP_MACRO)){
						if(!IsWindowEnabled(GetDlgItem(hDlg,IDC_FP_MACRO)))
							SetFocus(GetDlgItem(hDlg,IDOK));
						}
					else
						SetFocus(hwndFocus);
					}
				   break;
				}
			}
			break;

      case USER_IDOK:
			// Update the linked list node before we change the controls
			if(!(lpci->lpCurStmt->fDisabled))
				if(UpdateParameterNode(hDlg, lpDlg))
					SetStatBarText(hwndStatBar, (LPTSTR)szUpdatedParam, BLACK_TEXT);
		   return FALSE;
		}

	return TRUE;
}

//*------------------------------------------------------------------------
//|  SetCurrentDesc:
//|		This function sets the source and destination and current Descriptor
//|	records in the connection info record depending on the calling function
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
lpDESCRIPTORHEADER INTFUN SetCurrentDesc(HWND hDlg, WORD wID, lpDESCRIPTORHEADER lpDesc,
									SWORD cDescs)
{
	SQLHDESC	hdesc=NULL;
	SDWORD	dwDex=0;
	SWORD		iDesc;

	//get index to currently selected hdesc
	dwDex = (SWORD)SendDlgItemMessage(hDlg,wID, CB_GETCURSEL, 0, 0L);

	if(dwDex != CB_ERR)
	{
		//retrieve the descriptor handle from the currently selected
		//descriptor
		hdesc=(SQLHDESC)SendMessage(GetDlgItem(hDlg,wID),
								CB_GETITEMDATA,(WPARAM)dwDex, 0L);
	}

	if (lpDesc)
	{
		for (iDesc=0;iDesc < cDescs;iDesc++)
		{
			if (hdesc==lpDesc->hdesc)
				return(lpDesc);

			lpDesc=lpDesc->next;
		}

	}

	return(NULL);

} //SetCurrentDesc()



//*------------------------------------------------------------------------
//|  UpdateDescComboBox:
//|		This function Adds descriptor handles/descriptions to the CopyDesc
//|	dialog box.
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN UpdateDescComboBox(HWND hDlg,WORD wID,UINT cDescs,lpDESCRIPTORHEADER lpDesc)
{
	HWND		hwndCtl=GetDlgItem(hDlg,wID);
	UINT		i,
				iHdbc;
	TCHAR		szAddress[MEDBUFF]=TEXT("");
	DWORD		dwDex=INVALID_VALUE,
				dwFirst=INVALID_VALUE;
	TCHAR		szBuff[MEDBUFF]=TEXT(""),
				szHDBCString[MEDBUFF]=TEXT("");

	if (lpDesc->lpCI && lpDesc->lpSI)
	{
		wsprintf(szHDBCString,TEXT("hdbc %u, hstmt %u"),lpDesc->lpCI->uConNum,
			lpDesc->lpSI->uStmtNum);
	}
	else
	{
		wsprintf(szHDBCString,TEXT("hdbc %u"),lpDesc->lpCI->uConNum);
	}

	for (i=0,iHdbc=0;i < cDescs;i++)
	{
		if (lpDesc->hdesc)
		{
			// Concatenate description to HDESC String
			wsprintf(szBuff,TEXT("hdesc: %s(%s) \"%s\""),lpDesc->szHdesc,szHDBCString,lpDesc->szName);

			// Check to see if we have already added it to the combobox
			if (SendMessage(hwndCtl, CB_FINDSTRING, (WPARAM)0, (LPARAM)szBuff)==CB_ERR)
			{
				dwDex = (DWORD)SendMessage(hwndCtl, CB_ADDSTRING, 0, (LPARAM)szBuff);

				//Set data item
				SendMessage(hwndCtl,CB_SETITEMDATA,dwDex,(LPARAM)lpDesc->hdesc);
			}

			if (dwFirst == INVALID_VALUE)
				dwFirst=dwDex;

			iHdbc++;

		}

		lpDesc=lpDesc->next;
	}

	if(dwDex != CB_ERR)
			//Make first Descriptor the current selection
			SendMessage(hwndCtl, CB_SETCURSEL, (WPARAM)dwFirst, (LPARAM)0);

} //UpdateDescComboBox()


//*------------------------------------------------------------------------
//|  IsDescAttr:
//|		This function returns TRUE if we are processing a APD,ARD,IPD or
//|	an IRD attribute otherwise FALSE is returned
//*------------------------------------------------------------------------
BOOL INTFUN IsDescAttr(HWND hDlg,WORD idxvParam)
{
	DWORD	dex=(DWORD)SendDlgItemMessage(hDlg,idxvParam,CB_GETCURSEL,0L,0L);

	switch(SendDlgItemMessage(hDlg,idxvParam,CB_GETITEMDATA,(WPARAM)dex,0L))
	{
		case SQL_ATTR_APP_ROW_DESC:
		case SQL_ATTR_APP_PARAM_DESC:
		case SQL_ATTR_IMP_ROW_DESC:
		case SQL_ATTR_IMP_PARAM_DESC:
			return(TRUE);
		default:
			return(FALSE);
	}

} //IsDescAttr()


//*------------------------------------------------------------------------
//|  HandleIsPointerCbWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN HandleIsPointerCbWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	WORD				idx,
						idfAttr,
						idrgb,
						idcb;
	lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM		lpParm = lpDlg->lpParms[lpCtl->iParmNum],
						lpAttrParm=NULL,
						lprgbParm=NULL,
						lpcbParm=NULL;
	lpDFTARRAY		lpcbDfts,
						lpfAttrDfts;
	UINT				ccbDfts,
						idxcb,
						cfAttrDfts;
	int				iTmpDex=lpDlg->iFuncDex+1,
						iType;
	SDWORD			dwSel,
						dwDex;
	BOOL				fDoit;

	if (!(lpDlg && lpDlg->lpParms))
		return(FALSE);

	SetCTypeWndProc(hDlg,msg,wParam,lParam,lpUsr,cDex);

	// Find out if user has called SQLGetStmt/ConnectAttr or SQLSetStmt/ConnectAttr
	// because the "Parameter Type" combo box is in a different position between
	// the Get and Set functions
	switch(iTmpDex)
	{
		case API_SQLSETSTMTATTR:
				lpAttrParm=lpDlg->lpParms[IPAR_SSA_FATTRIBUTE],
				lprgbParm=lpDlg->lpParms[IPAR_SSA_RGBVALUE],
				lpcbParm=lpDlg->lpParms[IPAR_SSA_CBVALUE],
				idfAttr=IDCB_SSA_FATTRIBUTE;
				idrgb=IDCB_SSA_RGBVALUE;
				idcb=IDE_SSA_CBVALUE;
				break;
		case API_SQLSETENVATTR:
				lpAttrParm=lpDlg->lpParms[IPAR_SEA_FATTRIBUTE],
				lprgbParm=lpDlg->lpParms[IPAR_SEA_RGBVALUE],
				lpcbParm=lpDlg->lpParms[IPAR_SEA_CBVALUE],
				idfAttr=IDCB_SEA_FATTRIBUTE;
				idrgb=IDCB_SEA_RGBVALUE;
				idcb=IDE_SEA_CBVALUE;
				break;
		case API_SQLSETCONNECTATTR:
				lpAttrParm=lpDlg->lpParms[IPAR_SCA_FATTRIBUTE],
				lprgbParm=lpDlg->lpParms[IPAR_SCA_RGBVALUE],
				lpcbParm=lpDlg->lpParms[IPAR_SCA_CBVALUE],
				idfAttr=IDCB_SCA_FATTRIBUTE;
				idrgb=IDCB_SCA_RGBVALUE;
				idcb=IDCB_SCA_CBVALUE;
			break;
		case API_SQLSETDESCREC:
				lpAttrParm=lpDlg->lpParms[IPAR_SDR_FTYPE],
				lprgbParm=lpDlg->lpParms[IPAR_SDR_RGBVALUE],
				lpcbParm=lpDlg->lpParms[IPAR_SDR_PCBSTRINGLENGTH],
				idfAttr=IDCB_SDR_FTYPE;
				idrgb=IDCB_SDR_RGBVALUE;
				idcb=IDCB_SDR_PCBSTRINGLENGTH;
				break;
		case API_SQLSETDESCFIELD:
				lpAttrParm=lpDlg->lpParms[IPAR_SDF_IFIELDIDENTIFIER],
				lprgbParm=lpDlg->lpParms[IPAR_SDF_RGBVALUE],
				lpcbParm=lpDlg->lpParms[IPAR_SDF_CBBUFFERLENGTH],
				idfAttr=IDCB_SDF_IFIELDIDENTIFIER;
				idrgb=IDCB_SDF_RGBVALUE;
				idcb=IDE_SDF_CBBUFFERLENGTH;
				break;
	}

	switch(msg)
	{
		case WM_INITDIALOG:
			fDoit=lpCtl->wID==idcb;
			break;
		case USER_CBN_UPDATE_CONTENTS:
		case USER_CBN_KILL_FOCUS:
			fDoit=lpCtl->wID==idfAttr;
			break;
	}

	//Is this the cbValue/StringLength field
	if (fDoit && lpcbParm && lpcbParm->lpDfts)
	{
		lpcbDfts=lpcbParm->lpDfts;
		ccbDfts=lpcbParm->cDfts;
		lpfAttrDfts=lpAttrParm->lpDfts;
		cfAttrDfts=lpAttrParm->cDfts;


		//Get the fAttribute
		// Get the new index
		dwDex = (DWORD)SendDlgItemMessage(hDlg,idfAttr,CB_GETCURSEL,0,0L);

		//Get Index of cbValue and verify if the the user has entered his own value
		idxcb = (SWORD)SendDlgItemMessage(hDlg,idcb,CB_GETCURSEL,0,0L);
		if (idxcb != CB_ERR)
		{
			//set the uParamType in the Parameter array so that we can log
			//it correctly
			lprgbParm->uParmType &=lpfAttrDfts[dwDex].uValInfo;

			//if (idxcb >= ccbDfts)// && (lpfAttrDfts[dwDex].uValInfo & PRM_STR))
			//{
				//User entered a value don't change the cbValue
				return(TRUE);
			//}
		}


		if (lpfAttrDfts[dwDex].uValInfo & PRM_16BIT)
		{
			iType=SQL_IS_SMALLINT;
		}
		else if (lpfAttrDfts[dwDex].uValInfo & PRM_32BIT ||
					lpfAttrDfts[dwDex].uValInfo & PRM_32MSK)
		{
			iType=SQL_IS_INTEGER;
		}
		else if (lpfAttrDfts[dwDex].uValInfo & PRM_STR)
		{
			iType=SQL_NTS;
		}
		else
		{
			iType=SQL_IS_POINTER;
		}

		//Get index of SQL_IS_SMALLINT in the default array
		for (idx=0;idx < ccbDfts;idx++)
		{
			if (lpcbDfts[idx].fOpt == iType)
				break;
		}

		if (idx < ccbDfts)
		{
			//found it, now set the StringLength field
			dwSel=(DWORD)SendMessage(GetDlgItem(hDlg,idcb),CB_SETCURSEL,idx,0L);
		}
	}

	return TRUE;

} //HandleIsPointerCbWndProc()

//*------------------------------------------------------------------------
//|  SetConAtrWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN SetConAtrWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	WORD				iVparamType,
						iParmDex;
	SWORD				wDex = 0; // BUGBUG: never initialized
	UDWORD			uValInfoTmp = 0; // BUGBUG: never initialized
	lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM		lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	int				iTmpDex=lpDlg->iFuncDex+1;
	DWORD				dwDex=0;
	TCHAR				szCType[MEDBUFF];
	BOOL				fClearBits=FALSE;
	lpDESCRIPTORHEADER lpDesc=NULL;

	if (!(lpDlg && lpDlg->lpParms))
		return(FALSE);

	// Find out if user has called SQLGetStmt/ConnectAttr or SQLSetStmt/ConnectAttr
	// because the "Parameter Type" combo box is in a different position between
	// the Get and Set functiuons
	switch(iTmpDex)
	{
		case API_SQLSETSTMTOPTION:
				iParmDex=3;
				iVparamType=IDCB_SSO_PARAMETER_TYPE;
				break;
		case API_SQLSETCONNECTOPTION:
				iParmDex=3;
				fClearBits=TRUE;
				iVparamType=IDCB_SCO_PARAMETER_TYPE;
				break;
		case API_SQLGETSTMTOPTION:
				iParmDex=3;
				iVparamType=IDCB_GSO_PARAMETER_TYPE;
				break;
		case API_SQLGETCONNECTOPTION:
				iParmDex=3;
				fClearBits=TRUE;
				iVparamType=IDCB_GCO_PARAMETER_TYPE;
				break;
		case API_SQLSETSTMTATTR:
				iVparamType=IDCB_SSA_PARAMETER_TYPE;
				iParmDex=4;
				break;
		case API_SQLSETCONNECTATTR:
				iParmDex=4;
				fClearBits=TRUE;
				iVparamType=IDCB_SCA_PARAMETER_TYPE;
			break;
		case API_SQLGETSTMTATTR:
				iParmDex=5;
				iVparamType=IDCB_GSA_PARAMETER_TYPE;
				break;
		case API_SQLGETCONNECTATTR:
				iParmDex=5;
				fClearBits=TRUE;
				iVparamType=IDCB_GCA_PARAMETER_TYPE;
				break;

	}

	SetCTypeWndProc(hDlg,msg,wParam,lParam,lpUsr,cDex);

	HandleIsPointerCbWndProc(hDlg,msg,wParam,lParam,lpUsr,cDex);

	switch(msg)
	{
		case WM_INITDIALOG:
			{

			if (lpCtl->wID==IDCB_SSA_RGBVALUE)
			{

				// Only update combobox with Descriptor handles
				// if it's an ARD,APD,IRD or an IPD
				if (!IsDescAttr(hDlg,IDCB_SSA_FATTRIBUTE))
					return(TRUE);

				if (!lpDlg || !lpDlg->ci)
					return FALSE;

				lpDesc=lpDlg->ci->lpDesc;

				if (lpDesc)
					UpdateDescComboBox(hDlg,lpCtl->wID,lpDlg->ci->cDescs,lpDesc);

				if (fClearBits)
					lpDlg->lpParms[1]->lpDfts[wDex].uValInfo=uValInfoTmp;

			}
		}
		break;

		case USER_CBN_UPDATE_CONTENTS:
			{
				if (lpCtl->wID == iVparamType) // Combo Box UpdateContents message for ParameterType combo box
				{
					SWORD	wDex;

					wDex = (SWORD)SendDlgItemMessage(hDlg,iVparamType,CB_GETCURSEL,0,0L);	// Get the new index
					// Validate the SQL_C data type
					if (wDex == CB_ERR)
						// The value is not a valid data type, so display an error, reset the ParameterType combo box to SQL_C_CHAR,
						// set the vParam data type to SQL_C_CHAR, and set the focus to the ParameterType combo box
					{

						szMessageBox(hDlg,MB_ICONINFORMATION | MB_OK, (LPTSTR)szvParamType,
										 GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidDataType),
										 (LPTSTR)szvParamType, (LPTSTR)szvParamType);

						if (fUnicode)
						{
		   				lpDlg->lpParms[2]->fCType = SQL_C_WCHAR;
							lstrcpy(szCType,szSQL_C_WCHAR);
						}
						else
						{
		   				lpDlg->lpParms[2]->fCType = SQL_C_CHAR;
							lstrcpy(szCType,szSQL_C_CHAR);
						}

   					SendDlgItemMessage(hDlg,iVparamType,CB_SETCURSEL,GetCTypeIndex(!USE_C_DFT_ARRAY,szCType),0L);
   					SetFocus(GetDlgItem(hDlg,lpCtl->wID));
   				}
					else
					{	// The value was a valid data type, so update the vParam combo box data type to the new type
						lpDlg->lpParms[2]->fCType = (SWORD)lpDlg->lpParms[iParmDex]->lpDfts[wDex].fOpt;
					}
				}
			}
			break;
		case USER_CBN_KILL_FOCUS:
		{
			if (lpCtl->wID==IDCB_SSA_FATTRIBUTE)
			{
				// Only update combobox with Descriptor handles
				// if it's an ARD,APD,IRD or an IPD
				if (!IsDescAttr(hDlg,lpCtl->wID))
					return(TRUE);

				if (!lpDlg || !lpDlg->ci || !lpDlg->ci->lpDesc)
					return FALSE;

				lpDesc=lpDlg->ci->lpDesc;

				if (lpDesc)
					UpdateDescComboBox(hDlg,IDCB_SSA_RGBVALUE,lpDlg->ci->cDescs,lpDesc);
			}
			else
				return(FALSE); //didin't handle this msg
		}
		break;

	} //switch(msg)

	return TRUE;

}

//*---------------------------------------------------------------------------------
//| DisableFillParamControls:
//|	This function clears and disables the controls in the Fill Param dialog
//| Parms:
//|	hDlg		Handle to the FillParam dialog
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DisableFillParamControls(HWND hDlg, lpDIALOGDESC lpDlg)
{
	// First clear all of the controls except hstmt & ipar
	SetWindowText(GetDlgItem(hDlg,IDC_FP_FPARAMTYPE),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_FCTYPE),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_FSQLTYPE),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_CBCOLDEF),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_IBSCALE),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RGBVALUE),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_CBVALUEMAX),TEXT(""));
	SetWindowText(GetDlgItem(hDlg,IDC_FP_PCBVALUE),TEXT(""));
	CheckDlgButton(hDlg,IDC_FP_MACRO,FALSE);

	// Now disable all of those controls and their labels (hot-keys)
   EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FPARAMTYPE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FPARAMTYPE_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FCTYPE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FCTYPE_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FSQLTYPE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FSQLTYPE_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBCOLDEF),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBCOLDEF_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_IBSCALE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_IBSCALE_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RGBVALUE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RGBVALUE+DLG_TEXT_BASE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBVALUEMAX),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBVALUEMAX_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_PCBVALUE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_PCBVALUE+DLG_TEXT_BASE),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_MACRO),FALSE);

	DisableFPRowsetControls(hDlg);

   // Set the flag for disabled controls in the StatementInfo structure
   lpDlg->ci->lpCurStmt->fDisabled = TRUE;

	return;
}


//*---------------------------------------------------------------------------------
//| UpdateFillParamControls:
//|	This function updates and enables the controls in the Fill Param dialog
//| Parms:
//|	hDlg		Handle to the FillParam dialog
//|	lpDlg		Pointer to the dialog structure
//|	lpParam	Pointer to the PARAMETERINFO structure to update the controls to
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN UpdateFillParamControls(HWND hDlg, lpDIALOGDESC lpDlg, lpPARAMETERINFO lpParam, UDWORD udRow)
{
	lpDFTARRAY	lpDftParam=lpdftParamType;
	lpDFTARRAY	lpDftSql=lpdftSqlTypes;
	lpDFTARRAY	lpDftC=lpdftCTypes;
	TCHAR			szParamType[40];
	TCHAR			szSqlType[40];
	TCHAR			szCType[40];
	TCHAR			szTemp[20];
	LPTSTR			lprgbValue=NULL;
	TCHAR			szpcbValue[sizeof(SDWORD)];
	SDWORD		pcbValueTmp;
	BOOL			fFormat=FALSE;
	UDWORD		dLength;
	BOOL			frgbValue=TRUE;
	BOOL			fpcbValue=TRUE;
	BOOL			fMacro=FALSE;
	SDWORD		pcbTmp;
	lpCONNECTIONINFO	lpci = lpDlg->ci;
	HWND			hwndOut=GETOUTPUTWINDOW(lpci);


	// Find fParamType in default array
	if(lpParam->dwParamDex != CB_ERR)
		wsprintf(szParamType, TEXT("%s=%d"), lpDftParam[lpParam->dwParamDex].szOpt, lpDftParam[lpParam->dwParamDex].fOpt);
	else
		FormatValueBasedOnType(szParamType,
                               sizeof(szParamType)/sizeof(szParamType[0]), &(lpParam->fParamType), SQL_C_USHORT, sizeof(USHORT),
										hwndOut);

	// Find fCType in default array
	wsprintf(szCType, TEXT("%s=%d"), lpDftC[lpParam->dwCDex].szOpt, lpDftC[lpParam->dwCDex].fOpt);
	if(lpParam->fCType == SQL_C_DEFAULT)
		lpDlg->lpParms[2]->fCType = GetCTypeFromSqlType(lpParam->fSqlType,TRUE);
	else
		lpDlg->lpParms[2]->fCType = lpParam->fCType;

	// Find fSqlType in default array
	wsprintf(szSqlType, TEXT("%s=%d"), lpDftSql[lpParam->dwSqlDex].szOpt, lpDftSql[lpParam->dwSqlDex].fOpt);

	// First update all of the controls except hstmt & ipar
	SetWindowText(GetDlgItem(hDlg,IDC_FP_FPARAMTYPE),szParamType);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_FCTYPE),szCType);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_FSQLTYPE),szSqlType);
	wsprintf(szTemp, TEXT("%lu"), lpParam->cbColDef);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_CBCOLDEF),szTemp);
	wsprintf(szTemp, TEXT("%d"), lpParam->ibScale);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_IBSCALE),szTemp);
	wsprintf(szTemp, TEXT("%ld"), lpParam->cbValueMax);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_CBVALUEMAX),szTemp);

	// Check the SQL_LEN_DATA_AT_EXEC macro checkbox if necessary
	if(*(lpParam->lpfMacro + (udRow - 1)))
		CheckDlgButton(hDlg,IDC_FP_MACRO,TRUE);
	else
		CheckDlgButton(hDlg,IDC_FP_MACRO,FALSE);

	// Format rgbValue
	if((lpParam->frgbRowset))
	{
		SetWindowText(GetDlgItem(hDlg,IDC_FP_RGBVALUE),szROWSETVALTXT);
		lpDlg->lpParms[2]->fNull = TRUE;
		frgbValue=FALSE;
	}
	else if(!(lpParam->rgbValue))
	{
		SetWindowText(GetDlgItem(hDlg,IDC_FP_RGBVALUE),szNULLPTR);
		lpDlg->lpParms[2]->fNull = TRUE;
		frgbValue=FALSE;
	}
	else
	{
		if(!lpParam->rgbValue)
			dLength = 5;			// size of "NULL"
		else
			//Bug #323
			dLength=GetDisplaySizeBasedOnType(lpParam->fCType,lpParam->cBytes);

		lprgbValue = (PTR)AllocateMemory(dLength);
		if(!lprgbValue)
		{
			szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szERRORTXT,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
					dLength, (LPTSTR)TEXT("rgbValue"));
		}

		if(lprgbValue)
			fFormat = FormatValueBasedOnType(lprgbValue, dLength,
														(LPTSTR)lpParam->rgbValue + (lpParam->cBytes * (udRow-1)),
														lpParam->fCType, lpParam->cBytes,hwndOut);
		else
		   lprgbValue = szTemp;

		if(!fFormat)
			lstrcpy(lprgbValue, (LPTSTR)TEXT(""));

		// Strip off the extra "" (double quotes) that come from FormatValueBasedOnType function
		if (IsCharData(lpParam->fCType))
		{
			LPTSTR		str, lstr;

			str = lstr = lprgbValue;
			while(lstr = _tcschr(str,TEXT('"')))
				lstrcpy(lstr,lstr+1);
		}

		SetWindowText(GetDlgItem(hDlg,IDC_FP_RGBVALUE),lprgbValue);
	}

	// Format pcbValue
	if(lpParam->fpcbRowset)
	{
		SetWindowText(GetDlgItem(hDlg,IDC_FP_PCBVALUE),szROWSETVALTXT);
		fpcbValue=FALSE;
	}
	else if(!(lpParam->pcbValue))
	{
		SetWindowText(GetDlgItem(hDlg,IDC_FP_PCBVALUE),szNULLPTR);
		fpcbValue=FALSE;
	}
	else
	{
		if(*(SDWORD *)(lpParam->pcbValue + (udRow - 1)) < 0)
		{
			pcbTmp = (SDWORD)GetSizeOfCType(lpParam->fCType,
									(lpParam->rgbValue ? (UDWORD)lstrlen(lpParam->rgbValue) : 0));

			if(lpParam->fCType == SQL_C_BINARY || lpParam->fCType == SQL_C_DEFAULT)
				pcbTmp = pcbTmp / 2;

			wsprintf(szpcbValue, TEXT("%ld"), pcbTmp);
			UpdateLengthItem(hDlg,IDC_FP_PCBVALUE,szpcbValue,lpParam->fCType,FALSE);
		}

		switch(*(SDWORD *)(lpParam->pcbValue + (udRow - 1)))
		{
			case SQL_NTS:
	   		SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_SETCURSEL,GetPcbIndex(SQL_NTS),0L);
				break;
			case SQL_NULL_DATA:
	   		SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_SETCURSEL,GetPcbIndex(SQL_NULL_DATA),0L);
				break;
			case SQL_DATA_AT_EXEC:
	   		SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_SETCURSEL,GetPcbIndex(SQL_DATA_AT_EXEC),0L);
				break;
			case SQL_DEFAULT_PARAM:
	   		SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_SETCURSEL,GetPcbIndex(SQL_DEFAULT_PARAM),0L);
				break;
			default:
				fMacro=TRUE;		// The pcbValue is not from the drop down, so enable the macro checkbox
				// The pcbValue is a number -- see if the macro was invoked
				// If it was, then undo it to display the real number
				if(*(lpParam->lpfMacro + (udRow -1)))
					pcbValueTmp = UNDO_LEN_DATA_AT_EXEC(*(SDWORD *)(lpParam->pcbValue + (udRow - 1)));
				else
					pcbValueTmp = *(SDWORD *)(lpParam->pcbValue + (udRow - 1));

				wsprintf(szpcbValue, TEXT("%ld"), pcbValueTmp);
				UpdateLengthItem(hDlg,IDC_FP_PCBVALUE,szpcbValue,lpParam->fCType,FALSE);
				break;
		}
	}

	// Now enable all of those controls and their labels (hot-keys)
   EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_ROW),lpDlg->ci->lpCurStmt->fParamOpt);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_ROW+DLG_TEXT_BASE),lpDlg->ci->lpCurStmt->fParamOpt);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FPARAMTYPE_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FPARAMTYPE),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FCTYPE_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FCTYPE),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FSQLTYPE_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_FSQLTYPE),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBCOLDEF_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBCOLDEF),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_IBSCALE_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_IBSCALE),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RGBVALUE+DLG_TEXT_BASE),frgbValue);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RGBVALUE),frgbValue);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBVALUEMAX_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_CBVALUEMAX),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_PCBVALUE+DLG_TEXT_BASE),fpcbValue);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_PCBVALUE),fpcbValue);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_MACRO),fMacro);

	// Now update the rowset controls if necessary
	if(lpParam->frgbRowset || lpParam->fpcbRowset)
		EnableFPRowsetControls(hDlg, lpParam);
	else
		DisableFPRowsetControls(hDlg);

	// Now set the disabled flag in the StatementInfo structure and update the last-used ipar and row numbers
	lpDlg->ci->lpCurStmt->fDisabled = FALSE;

   FREE_PTR(lprgbValue);
	return;
}

//*---------------------------------------------------------------------------------
//| SearchParamList:
//|	This function searches the parameter linked list in search of a given
//|	parameter number.
//| Parms:
//|	lpsi		Pointer to the STATEMENTINFO structure for the current hstmt
//|	ipar		The parameter number we are looking for in the linked list
//| Returns:
//|	lpParam if the parameter was found, NULL if it was not
//*---------------------------------------------------------------------------------
lpPARAMETERINFO INTFUN SearchParamList(lpSTATEMENTINFO lpsi, UWORD ipar)
{
	UWORD	idx;
	lpPARAMETERINFO	lpParam;

	if(!lpsi->lpParams)
		return NULL;
	lpParam = lpsi->lpParams;
	for(idx=0;idx<(lpsi->cParams);idx++)
		if(ipar == lpParam->ipar){
			return lpParam;
			}
		else
			lpParam = lpParam->next;
	return NULL;
}

//*---------------------------------------------------------------------------------
//| ValidateRgbValueTimestamp:
//|	This function is used by SQLBindParameter and FillParam to check the value entered
//|	in rgbValue when the fCType is SQL_C_TIMESTAMP.  It looks for entries that
//|	contain only a valid date or a valid time.  If only a valid date is found,
//|	the time is added to the end of the string as 00:00:00.  If only a valid time
//|	is found, the current date is added to the beginning of the string.  The string
//|	is then reset in the combo box.  This function is called from the UsrWndProc's
//|	of the two functions, upon receiving an update contentes message on the rgbValue
//|	combo box.
//| Parms:
//|	hDlg				Handle to the dialog
//|	irgbValue		The ID of the rgbValue control in the dialog
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN	ValidateRgbValueTimestamp(HWND hDlg, int irgbValue)
{
	TCHAR		szTemp[50];
	TCHAR		szTemp2[50];
	DATEINFO	dt;

	// Get what is in the rgbValue combo box
	GetDlgItemText(hDlg,irgbValue,szTemp,tsizeof(szTemp));

	// If it is a hex value, let it go
	if(!IsHexString(szTemp))
	{
		lstrcpy(szTemp2,szTemp);

		// If it is a valid timestamp, let it go
		if(!(IsValidTimestamp(szTemp2,FALSE,NULL)))
		{
			lstrcpy(szTemp2,szTemp);

			// If it is only a valid date,
			if(IsValidDate(szTemp2,FALSE,NULL))
			{
				// add on the time as zero's (00:00:00)
				lstrcat(szTemp,szNullTime);
				SetWindowText(GetDlgItem(hDlg,irgbValue),szTemp);
			}
			else
			{
				lstrcpy(szTemp2,szTemp);

				// If it is only a valid time,
				if(IsValidTime(szTemp2,FALSE,NULL,TRUE))
				{
					GetDate(&dt);
					szGetDate(szTemp2, ((lpDATEINFO)&dt));
					lstrcat(szTemp2, szOneSpace);
				   lstrcat(szTemp2, szTemp);
					SetWindowText(GetDlgItem(hDlg,irgbValue),szTemp2);
				}
			}
		}
	}
}



//*---------------------------------------------------------------------------------
//| UpdateParameterNode:
//|	This function updates a parameter node from the values in the FillParam dialog
//| Parms:
//|	hDlg		Handle to the BindParam dialog
//| Returns:
//|	TRUE if the update was successful (or if the parameter wasn't in the list), FALSE if it wasn't
//*---------------------------------------------------------------------------------
BOOL INTFUN UpdateParameterNode(HWND hDlg, lpDIALOGDESC lpDlg)
{
	lpPARAMETERINFO	lpParam=NULL;
	HWND					hwndOut;
	lpCONNECTIONINFO	ci=lpDlg->ci;
	LPTSTR					lprgbValue=NULL;
	TCHAR					pcbValueTmp[15];
	SDWORD				pcbTmp;
	UWORD					iparTmp;
	UDWORD				udRowTmp;
	DWORD					dwIdx,
							dwData=0;
	BOOL					fValid=TRUE;
	SDWORD				OldpcbValue;
	BOOL					OldfMacro;
	BOOL					fpcbNTS=FALSE;
	SWORD					fCTypeTmp;
	UDWORD				cBytes;
	TCHAR					rgbValueTmp[64];
	PARAMITEM			ParmTmp;
	lpPARAMITEM			lpParmTmp=&ParmTmp;
	TCHAR					szTemp[15];
	int					cExtra=sizeof(TCHAR);

	if(!((ci) && (ci->lpCurStmt) && (ci->lpCurStmt->lpParams)))
		return FALSE;

	fBusy(TRUE);
	hwndOut = GETOUTPUTWINDOW(ci);

	// Store local copies of the ipar and row we need to update
	GetDlgItemText(hDlg,IDC_FP_IPAR,szTemp,tsizeof(szTemp));	// See what is in the ipar edit box
	iparTmp = (UWORD)_ttoi(szTemp);
	GetDlgItemText(hDlg,IDC_FP_ROW,szTemp,tsizeof(szTemp));	// See what is in the row edit box
	udRowTmp = (UDWORD)_ttol(szTemp);

	// Search the linked list for the correct PARAMETERINFO structure
	lpParam = SearchParamList(ci->lpCurStmt, iparTmp);
	if(!lpParam)
	{
		ASSERTCHECK(TRUE, (LPTSTR)TEXT("Parameter was not found in the linked list."));
		return TRUE;
	}

	// Store the old pcbValue & fMacro values in case we need to put them back (if rgbValue is invalid)
	if(lpParam->pcbValue)
		OldpcbValue = *(lpParam->pcbValue + (udRowTmp - 1));

	OldfMacro = *(lpParam->lpfMacro + (udRowTmp - 1));

	// Update pcbValue
	if(!(lpParam->fpcbRowset))
	{
		// if pcbValue is from a rowset, we don't want to touch it
		if(lpParam->pcbValue)
		{
			// pcbValue is not a null pointer
			dwIdx = (DWORD)SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_GETCURSEL,0,0L);
			dwData = (DWORD)SendDlgItemMessage(hDlg,IDC_FP_PCBVALUE,CB_GETITEMDATA,dwIdx,0L);

			switch(dwData)
			{
				case SQL_NTS:
				case SQL_NULL_DATA:
				case SQL_DATA_AT_EXEC:
				case SQL_DEFAULT_PARAM:
					*(lpParam->pcbValue + (udRowTmp - 1)) = dwData;
					break;
				default:	// The pcbValue is a SDWORD
					fValid = ValidateEditWindowByType(hDlg,IDC_FP_PCBVALUE,(LPTSTR)szPCBVALUE,SQL_C_SLONG);

					if(fValid)
					{
						GetDlgItemText(hDlg,IDC_FP_PCBVALUE,pcbValueTmp,15);
						pcbTmp = (SDWORD)_ttol(pcbValueTmp);

						//
						if( (lpParam->fParamType != SQL_PARAM_INPUT) &&
							 (pcbTmp > lpParam->cbValueMax))
						{
							szMessageBox(hDlg, MB_ICONINFORMATION | MB_OK, (LPTSTR)szFILLPARAM,
										 GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idscbValueTruncated),
										 pcbTmp, lpParam->cbValueMax);

							pcbTmp = lpParam->cbValueMax;
							wsprintf(pcbValueTmp, TEXT("%ld"), pcbTmp);
							UpdateLengthItem(hDlg,IDC_FP_PCBVALUE,pcbValueTmp,lpParam->fCType,FALSE);
						}

						if(IsDlgButtonChecked(hDlg, IDC_FP_MACRO))
							*(lpParam->pcbValue + (udRowTmp - 1)) = (SDWORD)SQL_LEN_DATA_AT_EXEC(pcbTmp);
						else
							*(lpParam->pcbValue + (udRowTmp - 1)) = pcbTmp;
					}
					else
					{
						wsprintf(szTemp, TEXT("%u"), iparTmp);
						SetDlgItemText(hDlg, IDC_FP_IPAR, szTemp);
						wsprintf(szTemp, TEXT("%lu"), udRowTmp);
						SetDlgItemText(hDlg, IDC_FP_ROW, szTemp);
						SetFocus(GetDlgItem(hDlg, IDC_FP_PCBVALUE));
						return FALSE;
					}
					break;
				}
			}
		}

	// Update the SQL_LEN_DATA_AT_EXEC macro flag
	if(IsDlgButtonChecked(hDlg, IDC_FP_MACRO))
		*(lpParam->lpfMacro + (udRowTmp - 1)) = TRUE;
	else
		*(lpParam->lpfMacro + (udRowTmp - 1)) = FALSE;


	// Update rgbValue
	// Types SQL_C_CHAR and SQL_C_BINARY require we copy the correct number of bytes (truncate if necessary)
	// If we fail, we need to back out the changes to pcbValue & fMacro
	// First we need to allocate a buffer for rgbValue
	if(!(lpParam->frgbRowset))
	{
		// if rgbValue is from a rowset, we don't want to touch it
		if(lpParam->rgbValue)
		{
			fCTypeTmp = lpParam->fCType;

			// Now get the data and it's size
			if (IsVarLenData(fCTypeTmp))
			{
				cBytes = (UDWORD)GetTrueWindowTextLength(hDlg,IDC_FP_RGBVALUE);

				// Allocate memory for a temporary rgbValue buffer
				lprgbValue = AllocateMemory(cBytes+cExtra) ;

				if(!lprgbValue)
				{
					szMessageBox(hwndOut,MB_ICONINFORMATION | MB_OK, (LPTSTR)szERRORTXT,
							GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
							(cBytes+cExtra), (LPTSTR)TEXT("rgbValue's temporary buffer"));
					// Now back out the changes to pcbValue & fMacro
					if(lpParam->pcbValue)
						*(lpParam->pcbValue + (udRowTmp - 1)) = OldpcbValue;
					*(lpParam->lpfMacro + (udRowTmp - 1)) = OldfMacro;
					return FALSE;
				}
				*lprgbValue = TEXT('\0');

				if(!ValidateEditWindowByType(hDlg,IDC_FP_RGBVALUE,(LPTSTR)szRGBVALUE,lpParam->fCType))
				{
					wsprintf(szTemp, TEXT("%u"), iparTmp);
					SetDlgItemText(hDlg, IDC_FP_IPAR, szTemp);
					wsprintf(szTemp, TEXT("%lu"), udRowTmp);
					SetDlgItemText(hDlg, IDC_FP_ROW, szTemp);
					SetFocus(GetDlgItem(hDlg, IDC_FP_RGBVALUE));
					return FALSE;
				}

				GetDlgItemText(hDlg,IDC_FP_RGBVALUE,lprgbValue,(int)( (cBytes+cExtra)/sizeof(TCHAR)));
				ConvertToTypeFromString(lprgbValue, lpParmTmp, lpParam->fCType, 0);
				if (IsVarLenData(fCTypeTmp))
					cBytes = lpParmTmp->cBytes;

			}
			else
			{
				if(!ValidateEditWindowByType(hDlg,IDC_FP_RGBVALUE,(LPTSTR)szRGBVALUE,lpParam->fCType))
				{
					wsprintf(szTemp, TEXT("%u"), iparTmp);
					SetDlgItemText(hDlg, IDC_FP_IPAR, szTemp);
					wsprintf(szTemp, TEXT("%lu"), udRowTmp);
					SetDlgItemText(hDlg, IDC_FP_ROW, szTemp);
					SetFocus(GetDlgItem(hDlg, IDC_FP_RGBVALUE));
					return FALSE;
				}
				GetDlgItemText(hDlg,IDC_FP_RGBVALUE,rgbValueTmp,tsizeof(rgbValueTmp));
				ConvertToTypeFromString(rgbValueTmp, lpParmTmp, lpParam->fCType, 0);
			}

			if (IsVarLenData(lpParam->fCType))
			{
				// First check if pcbValue is SQL_NTS
				if(lpParam->pcbValue)
				{
					if(*(lpParam->pcbValue + (udRowTmp - 1)) == SQL_NTS)
						fpcbNTS=TRUE;
				}

				// if data is larger than the buffer and pcbValue = SQL_NTS
				// then truncate the data and add a null terminator
				// Also put up a message box to alert the user to the truncation
				if((cBytes >= (UDWORD)lpParam->cBytes) && fpcbNTS){
					_fmemcpy(((LPTSTR)lpParam->rgbValue + (lpParam->cBytes * (udRowTmp -1))),
									lpParmTmp->lpData, (size_t)(lpParam->cBytes - 1));
					*((UCHAR *)lpParam->rgbValue + lpParam->cbValueMax - 1) = TEXT('\0');

					szMessageBox(hDlg, MB_ICONINFORMATION | MB_OK, (LPTSTR)szFILLPARAM,
								 GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsrgbValueTruncated),
								 (lpParam->cBytes));

					if(IsHexString(lprgbValue))
						*(lprgbValue + (lpParam->cBytes * 2)) = TEXT('\0');
					else
						*(lprgbValue + lpParam->cBytes - 1) = TEXT('\0');
					SetDlgItemText(hDlg,IDC_FP_RGBVALUE,lprgbValue);
				}
				// if data is larger than buffer (no SQL_NTS), just truncate the data
				// Also put up a message box to alert the user to the truncation
				else if(cBytes > (UDWORD)lpParam->cBytes)
				{
					_fmemcpy(((LPTSTR)lpParam->rgbValue + (lpParam->cBytes * (udRowTmp -1))),
									lpParmTmp->lpData, (size_t)lpParam->cBytes);

					szMessageBox(hDlg, MB_ICONINFORMATION | MB_OK, (LPTSTR)szFILLPARAM,
								 GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsrgbValueTruncated),
								 lpParam->cBytes);

					if(IsHexString(lprgbValue))
						*(lprgbValue + (lpParam->cBytes * 2) + 2) = TEXT('\0');
					else
						*(lprgbValue + lpParam->cBytes) = TEXT('\0');
					SetDlgItemText(hDlg,IDC_FP_RGBVALUE,lprgbValue);
				}
				// if data fits in the buffer and pcbValue = SQL_NTS, just copy and add the null terminator
				else if(fpcbNTS)
				{
					_fmemcpy(((LPTSTR)lpParam->rgbValue + (lpParam->cBytes * (udRowTmp -1))),
									lpParmTmp->lpData, (size_t)cBytes);
					*((UCHAR *)lpParam->rgbValue + cBytes) = TEXT('\0');
				}
				// otherwise, just copy the data
				else
					_fmemcpy(((LPTSTR)lpParam->rgbValue + (lpParam->cBytes * (udRowTmp -1))), lpParmTmp->lpData, (size_t)cBytes);
			}
			else
			{
				_fmemcpy(((LPTSTR)lpParam->rgbValue + (lpParam->cBytes * (udRowTmp - 1))), lpParmTmp->lpData, (size_t)lpParmTmp->cBytes);
			}

			// Clean up our temporary buffers
			FREE_PTR(lpParmTmp->lpData);
			FREE_PTR(lprgbValue);
		}
	}

	fBusy(FALSE);
	return TRUE;
}


//*---------------------------------------------------------------------------------
//| CheckHdbcListForRowsets
//|	This function checks the entire linked list of HDBC's for connections with
//| 	statements that have a rowset (bound cols, or row-wise bound memory).
//| Parms:
//|	Nothing -- it uses the global pointer to the HDBC linked list
//| Returns:
//|	TRUE if valid hstmts with rowsets are found, FALSE if none are found
//*---------------------------------------------------------------------------------
BOOL INTFUN CheckHdbcForRowsets(lpCONNECTIONINFO lpci)
{
	UINT					idx;
	BOOL					fFound=FALSE;
	lpSTATEMENTINFO	lpsi;

	if(lpci->lpStmt){
		lpsi = lpci->lpStmt;
		for(idx=0;idx<(UINT)(lpci->cStmts);idx++){
			// Determine whether to check for column-wise or row-wise binding
			if(lpsi->fColBinding){				// Column wise binding
				if(lpsi->lpVars){					// Bound columns are present
					fFound = TRUE;
					break;
					}
				}
			else{										// It's row-wise binding
				if(lpsi->lpRowWise){
					fFound = TRUE;
					break;
					}
				}
			lpsi = lpsi->next;
			}
		}
	return fFound;
}

//*---------------------------------------------------------------------------------
//| UpdateBPRowsetControls
//|	This function updates the 4 rowset controls based on a parameterinfo pointer.
//| Parms:
//|	hDlg		Handle to the dialog
//|	lpParam	Pointer to the parameterinfo structure for the given parameter
//| Returns:
//|	TRUE if valid hstmts with rowsets are found, FALSE if none are found
//*---------------------------------------------------------------------------------
VOID INTFUN UpdateBPRowsetControls(HWND hDlg,  lpPARAMETERINFO lpParam,lpCONTROL lpCtl,SWORD idxRSHdbc,
											  SWORD idxRSHstmt,SWORD idxRSCol,SWORD idxRSRow)
{
	DWORD					dwDex;
	UINT					idx;
	DWORD					dwSel=0;
	BOOL					fFound=FALSE;
	lpCONNECTIONINFO	lpCiList=lpConnWin;
	lpSTATEMENTINFO	lpSi;
	TCHAR					szTemp[50];
	HWND					hwndHSTMT = GetDlgItem(hDlg,lpCtl->wID);

	// First we need to determine if the parameter's hdbc is still valid
	dwDex = (DWORD)SendDlgItemMessage(hDlg,idxRSHdbc,CB_GETCOUNT,0,0L);

   for(idx=0;idx<(UINT)dwDex;idx++)
	{
   	if((UWORD)SendDlgItemMessage(hDlg,idxRSHdbc,CB_GETITEMDATA,(WPARAM)idx,0L) == lpParam->iHdbc)
		{
   		SendDlgItemMessage(hDlg,idxRSHdbc,CB_SETCURSEL,(WPARAM)idx,0L);

			while(lpCiList->uConNum != lpParam->iHdbc)
				lpCiList = lpCiList->next;

			fFound = TRUE;

			break;
   	}
   }

   if(!fFound)
	{
   	if(dwDex > 0)
   		SendDlgItemMessage(hDlg,idxRSHdbc,CB_SETCURSEL,0,0L);
   	return;
   }
	// Now we should have a valid hdbc

	// We need to go through the linked list of hstmt's checking for ones that have a valid rowset pending
	// Now we have a pointer to the connection, so traverse the linked list of hstmts
	// and put them in the HSTMT combo list box, looking for
	SendDlgItemMessage(hDlg,idxRSHstmt,CB_RESETCONTENT,0,0L);
	lpSi = lpCiList->lpStmt;

	if(lpSi)
		AddHstmtToDropDown(hwndHSTMT,lpParam,lpCiList,lpCiList->lpStmt,&dwSel);

	if(lpSi)
		SendDlgItemMessage(hDlg,idxRSHstmt,CB_SETCURSEL,(WPARAM)dwSel,0L);

	// Update the Column and Row controls
	wsprintf(szTemp, TEXT("%lu"), lpParam->iCol);
	SetDlgItemText(hDlg,idxRSCol,szTemp);
	wsprintf(szTemp, TEXT("%lu"), lpParam->iRow);
	SetDlgItemText(hDlg,idxRSRow,szTemp);

	return;
}


//*---------------------------------------------------------------------------------
//| EnableFPRowsetControls
//|	This function updates the 4 rowset controls based on a parameterinfo pointer.
//| Parms:
//|	hDlg		Handle to the dialog
//|	lpParam	Pointer to the parameterinfo structure for the given parameter
//| Returns:
//|	TRUE if valid hstmts with rowsets are found, FALSE if none are found
//*---------------------------------------------------------------------------------
VOID INTFUN EnableFPRowsetControls(HWND hDlg, lpPARAMETERINFO lpParam)
{
	TCHAR		szTemp[15];
	UDWORD	udRowTmp;
	UDWORD	udRow;
	BOOL		fTranslated;

	// Now enable all of those controls and their labels
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HDBC),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HDBC_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HSTMT),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HSTMT_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_COLUMN),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_COLUMN_LABEL),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_ROW),TRUE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_ROW_LABEL),TRUE);

	// First set all of the controls to the current parameter's rowset information
	wsprintf(szTemp, TEXT("%u"), lpParam->iHdbc);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_HDBC),szTemp);
	wsprintf(szTemp, TEXT("%u"), lpParam->iHstmt);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_HSTMT),szTemp);
	wsprintf(szTemp, TEXT("%lu"), lpParam->iCol);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_COLUMN),szTemp);
	if(lpActiveConnWin->lpCurStmt->fParamOpt){
		udRow = (UDWORD)GetDlgItemInt(hDlg, IDC_FP_ROW, &fTranslated, FALSE);
		udRowTmp = udRow + lpParam->iRow - 1;
		}
	else
		udRowTmp = lpParam->iRow;
	wsprintf(szTemp, TEXT("%lu"), udRowTmp);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_ROW),szTemp);

	return;
}


//*---------------------------------------------------------------------------------
//| DisableFPRowsetControls
//|	This function updates the 4 rowset controls based on a parameterinfo pointer.
//| Parms:
//|	hDlg		Handle to the dialog
//|	lpParam	Pointer to the parameterinfo structure for the given parameter
//| Returns:
//|	TRUE if valid hstmts with rowsets are found, FALSE if none are found
//*---------------------------------------------------------------------------------
VOID INTFUN DisableFPRowsetControls(HWND hDlg)
{
	// Now disable all of those controls and their labels (hot-keys)
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HDBC),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HDBC_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HSTMT),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_HSTMT_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_COLUMN),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_COLUMN_LABEL),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_ROW),FALSE);
   EnableWindow(GetDlgItem(hDlg,IDC_FP_RS_ROW_LABEL),FALSE);

	// First set all of the controls to zero
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_HDBC),szNA);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_HSTMT),szNA);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_COLUMN),szNA);
	SetWindowText(GetDlgItem(hDlg,IDC_FP_RS_ROW),szNA);

	return;
}




//*------------------------------------------------------------------------
//|  SetDescComboBoxValue:
//|		This function sets the correct current selection in each of the
//|	Comboboxes for SQLCopyDesc()
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------

VOID INTFUN SetDescComboBoxValue(HWND hDlg,WORD wIDSrc,WORD wIDDest)
{
	HWND	hwndSrc = GetDlgItem(hDlg,wIDSrc),
			hwndDest = GetDlgItem(hDlg,wIDDest);
	DWORD	dwDex=0;

	//Get current selection
	dwDex=(DWORD)SendMessage(hwndSrc, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	SendMessage(hwndDest, CB_SETCURSEL, (WPARAM)dwDex, (LPARAM)0);
}


//*------------------------------------------------------------------------
//|  GetDiagFieldWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN GetDiagFieldWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	HWND					hwnd=NULL;
	lpDIALOGDESC		lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL			lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM			lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	SWORD					wDex=0;
	UWORD					fOption=FALSE;
	BOOL					fEnable=FALSE;
	LPTSTR				str=NULL;
	DWORD					dwDex=0;

	// InitDialog message for the DescriptionType combo box
	if (msg == WM_INITDIALOG && lpCtl->wID == IDCB_GDF_FDIAGIDENTIFIER_TYPE)
	{
		// If no fDescType is set yet, set the DescriptionType combo box to SQL_C_ULONG by default
		if (!lpDlg->lpParms[IPAR_GDF_FDIAGIDENTIFIER]->lpData)
  		{
	  		SendDlgItemMessage(hDlg,IDCB_GDF_FDIAGIDENTIFIER_TYPE,CB_SETCURSEL,GetCTypeIndex(!USE_C_DFT_ARRAY,szSQL_C_SLONG),0L);
	      EnableWindow(GetDlgItem(hDlg,IDCB_GDF_FDIAGIDENTIFIER_TYPE),FALSE);
	      EnableWindow(GetDlgItem(hDlg,IDCB_GDF_FDIAGIDENTIFIER_TYPE+DLG_TEXT_BASE),FALSE);
			lpDlg->lpParms[IPAR_GDF_RGBDIAGINFO]->fCType=SetCType(hDlg,lpDlg->lpParms[IPAR_GDF_FDIAGIDENTIFIER]->lpDfts,
						lpDlg->lpParms[IPAR_GDF_FDIAGIDENTIFIER_TYPE]->lpDfts,IDCB_GDF_FDIAGIDENTIFIER_TYPE,
						wDex,IDCB_GDF_FDIAGIDENTIFIER_TYPE,&(lpDlg->lpParms[IPAR_GDF_RGBDIAGINFO]->fHandle));

		}
	}
	else if ((msg == USER_CBN_UPDATE_CONTENTS) &&
		(lpCtl->wID == IDCB_GDF_FDIAGIDENTIFIER))
	{

		wDex = (SWORD)SendDlgItemMessage(hDlg,lpCtl->wID,CB_GETCURSEL,0,0L);		// Get the new fOption value
		if(wDex == CB_ERR)		// If it is not in the list, it is probably a driver-specific option
		{
			// Find out the size (in digits)
			DWORD dwDigits = GetTrueWindowTextLength(hDlg,lpCtl->wID);

			str = (LPTSTR)AllocateMemory(dwDigits + sizeof(TCHAR));
			if(!str)
				return FALSE;

			GetDlgItemText(hDlg,lpCtl->wID,str,(int)(dwDigits/sizeof(TCHAR)));				// Get the fOption as a string

			if(!IsValidInt(SQL_C_USHORT, str, FALSE, lpParm->szName))   // Validate it as a SQL_C_USHORT
				goto exitGetDiagField;

			fOption = (UWORD)_ttoi(str);	// Convert value to an integer

			fEnable = TRUE;      // Set flag to enable the ParameterType combo box
		}
		else
		{
			// fOption was in the list, so get the index to the chosen fOption

			HandleUserOpt(hDlg,lpParm,lpCtl->wID,&wDex,IDCB_GDF_FDIAGIDENTIFIER_TYPE,&fOption);

			lpDlg->lpParms[IPAR_GDF_RGBDIAGINFO]->fCType=SetCType(hDlg,lpDlg->lpParms[IPAR_GDF_FDIAGIDENTIFIER]->lpDfts,
						lpDlg->lpParms[IPAR_GDF_FDIAGIDENTIFIER_TYPE]->lpDfts,
						IDCB_GDF_FDIAGIDENTIFIER_TYPE,wDex,IDCB_GDF_FDIAGIDENTIFIER_TYPE,
						&(lpDlg->lpParms[IPAR_GDF_RGBDIAGINFO]->fHandle));

		}
	}
	else if (msg == USER_IDOK)
	{
		lpDESCRIPTORHEADER lpDesc=NULL;

		if (lpDlg && lpDlg->ci && lpDlg->ci->lpDesc)
			lpDesc=lpDlg->ci->lpDesc;
		else
			return TRUE;

		// Set the Current selection so the correct Descriptor
		// handle can be set
		lpDlg->ci->lpCurDesc=SetCurrentDesc(hDlg,IDCB_GDF_HHANDLE,lpDesc,lpDlg->ci->cDescs);


	}


	// If the fOption is a driver-specific option, enable the ParameterType combo box
	EnableWindow(GetDlgItem(hDlg,IDCB_GDF_FDIAGIDENTIFIER_TYPE),fEnable);

	return TRUE;

exitGetDiagField:

	FREE_PTR(str);
    return FALSE;

} //GetDiagFieldWndProc()




//*------------------------------------------------------------------------
//|  ListDescHandlesWndProc:
//|		This looks like a window procedure but is not. It takes the
//|		add'l parameters of a dialog descriptor and the index of
//|		the control we're interested in.  The other parms are from
//|		the main dialog box.  This purpose of this routine is to handle
//|		extra steps required to intialize certain controls.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN ListDescHandlesWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	HWND						hwnd=NULL;
	lpDIALOGDESC			lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL				lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM				lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	lpDESCRIPTORHEADER	lpDesc=NULL;
	SWORD						cDescs=0;
	lpCONNECTIONINFO		lpCI=NULL;
	lpENVIRONMENTINFO		lpEI=GetActiveEnv();
	UINT						cConns=0;
	UINT						i;

	if (!lpDlg || !lpDlg->ci || !lpDlg->ci->lpDesc)
		return(TRUE);

	lpCI=lpDlg->ci;
	//Get Descriptor linked list
	lpDesc=lpCI->lpDesc;
	//Get number of Descriptor records
	cDescs=lpCI->cDescs;

	if (msg != WM_INITDIALOG && ((lpDlg->iFuncDex+1) != API_SQLCOPYDESC))
		return(TRUE);

	cConns=lpEI->cConns;
	if (lpEI->lpConn)
		lpCI=lpEI->lpConn;

	if (lpDlg && lpDlg->ci && lpDlg->ci->lpDesc)
		lpDesc=lpDlg->ci->lpDesc;

	if (!lpDesc)
		return(FALSE);

	//Loop through all connections and list out all
	//of the descriptors
	for (i=0;i < cConns;i++)
	{
		if (lpCI->lpDesc)
			UpdateDescComboBox(hDlg,lpCtl->wID,lpCI->cDescs,lpCI->lpDesc);

		//Next connection
		lpCI=lpCI->next;
	}

	return TRUE;

} //ListDescHandlesWndProc()


//*------------------------------------------------------------------------
//|  SubTypeWndProc:
//|		This function is responsible for displaying the DATETIME_INTERVAL_CODE
//|	when the SQLSetDescRec fType == SQL_DATETIME
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN SubTypeWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	HWND					hwnd=NULL;
	lpDIALOGDESC		lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL			lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM			lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	BOOL					fEnable=FALSE;
	DWORD					dwDex=0;
	static DWORD		dexDateTime=0;
	static DWORD		dexInterval=0;
	UINT					i;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			//find index of SQL_DATETIME and SQL_INTERVAL
			for (i=0;i <= ciDescTypes;i++)
 			{
				if (!dexDateTime && !lstrcmp(lpdftDescTypes[i].szOpt,szSQL_DATETIME))
					dexDateTime=i;

				if (!dexInterval && !lstrcmp(lpdftDescTypes[i].szOpt,szSQL_INTERVAL))
					dexInterval=i;
			}
		}
		// WE NEED TO FALL THROUGH HERE
		case USER_CBN_UPDATE_CONTENTS:
		case USER_CBN_KILL_FOCUS:
		{
			if (lpCtl->wID == IDCB_SDR_FTYPE)
			{
				// Get what is in the fType combo box
				dwDex=(SWORD)SendDlgItemMessage(hDlg,IDCB_SDR_FTYPE,CB_GETCURSEL,0L,0L);

				if ((dwDex == dexDateTime) || (dwDex == dexInterval))
					fEnable=TRUE;

				//grey drop down if there are no subtypes, otherwise
				//enable the drop down.
				EnableWindow(GetDlgItem(hDlg, IDCB_SDR_FSUBTYPE), fEnable);
				EnableWindow(GetDlgItem(hDlg, IDCB_SDR_FSUBTYPETXT), fEnable);

			}
		}
		break;
	}
	return TRUE;

} //SubTypeWndProc()

//*------------------------------------------------------------------------
//|  SetParamType:
//|		This function sets the uParmType in the lpPARAMITEM array for
//|	those parameters that can change from the time the parameter arrays
//|	are loaded. These include sqlgetinfo, SQLSet/Get/CONNECT/ENV/STMT ATTR'S.
//|	This is done so that the parameter will get logged correctly. (i.e.
//|	PRM_STR gets logged as a string, PRM_PTR_UWORD gets logged as a
//|	pointer to a UWORD,etc.
//|	Returns:
//|		NOTHING
//*------------------------------------------------------------------------
VOID INTFUN SetParamType(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	WORD				idfAttr,
						idrgb;
	lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM		lpParm = lpDlg->lpParms[lpCtl->iParmNum],
						lpAttrParm=NULL,
						lprgbParm=NULL;
	lpDFTARRAY		lpfAttrDfts;
	UINT				cfAttrDfts;
	int				iTmpDex=lpDlg->iFuncDex+1;
	SDWORD			dwDex;
	UINT				uType=0;

	switch(iTmpDex)
	{
		case API_SQLSETSTMTATTR:
				lpAttrParm=lpDlg->lpParms[IPAR_SSA_FATTRIBUTE];
				lprgbParm=lpDlg->lpParms[IPAR_SSA_RGBVALUE];
				idfAttr=IDCB_SSA_FATTRIBUTE;
				idrgb=IDCB_SSA_RGBVALUE;
				break;
		case API_SQLSETENVATTR:
				lpAttrParm=lpDlg->lpParms[IPAR_SEA_FATTRIBUTE];
				lprgbParm=lpDlg->lpParms[IPAR_SEA_RGBVALUE];
				idfAttr=IDCB_SEA_FATTRIBUTE;
				idrgb=IDCB_SEA_RGBVALUE;
				break;
		case API_SQLSETCONNECTATTR:
				lpAttrParm=lpDlg->lpParms[IPAR_SCA_FATTRIBUTE];
				lprgbParm=lpDlg->lpParms[IPAR_SCA_RGBVALUE];
				idfAttr=IDCB_SCA_FATTRIBUTE;
				idrgb=IDCB_SCA_RGBVALUE;
			break;
		case API_SQLSETDESCREC:
				lpAttrParm=lpDlg->lpParms[IPAR_SDR_FTYPE];
				lprgbParm=lpDlg->lpParms[IPAR_SDR_RGBVALUE];
				idfAttr=IDCB_SDR_FTYPE;
				idrgb=IDCB_SDR_RGBVALUE;
				break;
		case API_SQLSETDESCFIELD:
				lpAttrParm=lpDlg->lpParms[IPAR_SDF_IFIELDIDENTIFIER];
				lprgbParm=lpDlg->lpParms[IPAR_SDF_RGBVALUE];
				idfAttr=IDCB_SDF_IFIELDIDENTIFIER;
				idrgb=IDCB_SDF_RGBVALUE;
				break;
		case API_SQLGETINFO:
				lpAttrParm=lpDlg->lpParms[IPAR_GI_FINFOTYPE];
				lprgbParm=lpDlg->lpParms[IPAR_GI_RGBINFOVALUE];
				idfAttr=IDCB_GI_FINFOTYPE;
				idrgb=IDCK_GI_RGBINFOVALUE;
				break;
	}

	if (!lpAttrParm || !lprgbParm)
		return;

	lpfAttrDfts=lpAttrParm->lpDfts;
	cfAttrDfts=lpAttrParm->cDfts;

	//Get the fAttribute
	// Get the new index
	dwDex = (DWORD)SendDlgItemMessage(hDlg,idfAttr,CB_GETCURSEL,0,0L);

	if (dwDex != CB_ERR)
	{
		//set the uParamType in the Parameter array so that we can log
		//it correctly
		switch (lpfAttrDfts[dwDex].uValInfo)
		{
			case PRM_PTR_BIN:
			case PRM_PTR_32BIT:
			case PRM_PTR_16BIT:
			case PRM_PTR_STR:
			case PRM_PTR_PTR:
				lprgbParm->fHandle = PT_PTR_PTR;
				break;
			default:
				lprgbParm->fHandle=FALSE;
		}
	}

} //SetParamType()


//*------------------------------------------------------------------------
//|  SetCTypeWndProc:
//|		This psuedo-WndProc sets the C Type for various API's drop downs
//|	field.
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN SetCTypeWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	lpDIALOGDESC	lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL		lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM		lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	WORD				iFieldID,
						iFieldIDType,
						iParFieldID,
						iParFieldIDType,
						iParRgbValue;
	BOOL				fProcessed=FALSE;


	if (lpDlg && lpDlg->lpParms && (msg == USER_IDOK))
		SetParamType(hDlg,msg,wParam,lParam,lpUsr,cDex);

	switch (lpDlg->iFuncDex+1)
	{

		case API_SQLSETDESCREC:
			ProcessfCtype(hDlg,&(lpDlg->lpParms[IPAR_SDR_RGBVALUE]->fCType),
				lpDlg->lpParms[IPAR_SDR_FTYPE]->lpDfts,
				lpDlg->lpParms[IPAR_SDR_FTYPE]->lpDfts,
				IDCB_SDR_FTYPE,IDCB_SDR_FTYPE);
			fProcessed=TRUE;
			break;
		case API_SQLSETDESCFIELD:
			iFieldID=IDCB_SDF_IFIELDIDENTIFIER;
			iFieldIDType=IDCB_SDF_IFIELDIDENTIFIER_TYPE;
			iParFieldID=IPAR_SDF_IFIELDIDENTIFIER;
			iParFieldIDType=IPAR_SDF_IFIELDIDENTIFIER_TYPE;
			iParRgbValue=IPAR_SDF_RGBVALUE;
			break;
		case API_SQLGETDESCFIELD:
			iFieldID=IDCB_GDF_IFIELDIDENTIFIER;
			iFieldIDType=IDCB_GDF_IFIELDIDENTIFIER_TYPE;
			iParFieldID=IPAR_GDF_IFIELDIDENTIFIER;
			iParFieldIDType=IPAR_GDF_IFIELDIDENTIFIER_TYPE;
			iParRgbValue=IPAR_GDF_RGBVALUE;
			break;
		case API_SQLGETINFO:
			iFieldID=IDCB_GI_FINFOTYPE;
			iFieldIDType=IDCB_GI_INFORMATION_VALUE_TYPE;
			iParFieldID=IPAR_GI_FINFOTYPE;
			iParFieldIDType=IPAR_GI_INFORMATION_VALUE_TYPE;
			iParRgbValue=IPAR_GI_RGBINFOVALUE;
			break;
		case API_SQLCOLATTRIBUTE:
			iFieldID=IDCB_CA_FFIELDINDENTIFIER;
			iFieldIDType=IDCB_CA_FFIELDINDENTIFIER_TYPE;
			iParFieldID=IPAR_CA_FFIELDINDENTIFIER;
			iParFieldIDType=IPAR_CA_FFIELDINDENTIFIER_TYPE;
			iParRgbValue=IPAR_CA_RGBCHARACTERATTRIBUTE;
			break;
		case API_SQLCOLATTRIBUTES:
			iFieldID=IDCB_CA_FDESCTYPE;
			iFieldIDType=IDCB_CA_DESCRIPTION_TYPE;
			iParFieldID=IPAR_CAS_FDESCTYPE;
			iParFieldIDType=IPAR_CAS_DESCRIPTION_TYPE;
			iParRgbValue=IPAR_CAS_RGBDESC;
			break;
		case API_SQLSETENVATTR:
			iFieldID=IDCB_SEA_FATTRIBUTE;
			iFieldIDType=IDCB_SEA_FATTRIBUTE_TYPE;
			iParFieldID=IPAR_SEA_FATTRIBUTE;
			iParFieldIDType=IPAR_SEA_FATTRIBUTE_TYPE;
			iParRgbValue=IPAR_SEA_RGBVALUE;
			break;
		case API_SQLGETENVATTR:
			iFieldID=IDCB_GEA_FATTRIBUTE;
			iFieldIDType=IDCB_GEA_FATTRIBUTE_TYPE;
			iParFieldID=IPAR_GEA_FATTRIBUTE;
			iParFieldIDType=IPAR_GEA_FATTRIBUTE_TYPE;
			iParRgbValue=IPAR_GEA_RGBVALUE;
			break;
		case API_SQLGETSTMTATTR:
			iFieldID=IDCB_GSA_FATTRIBUTE;
			iFieldIDType=IDCB_GSA_PARAMETER_TYPE;
			iParFieldID=IPAR_GSA_FATTRIBUTE;
			iParFieldIDType=IPAR_GSA_PARAMETER_TYPE;
			iParRgbValue=IPAR_GSA_RGBVALUE;
			break;
		case API_SQLSETSTMTATTR:
			iFieldID=IDCB_SSA_FATTRIBUTE;
			iFieldIDType=IDCB_SSA_PARAMETER_TYPE;
			iParFieldID=IPAR_SSA_FATTRIBUTE;
			iParFieldIDType=IPAR_SSA_PARAMETER_TYPE;
			iParRgbValue=IPAR_SSA_RGBVALUE;
			break;
		case API_SQLSETCONNECTATTR:
			iFieldID=IDCB_SCA_FATTRIBUTE;
			iFieldIDType=IDCB_SCA_PARAMETER_TYPE;
			iParFieldID=IPAR_SCA_FATTRIBUTE;
			iParFieldIDType=IPAR_SCA_PARAMETER_TYPE;
			iParRgbValue=IPAR_SCA_RGBVALUE;
			break;
		case API_SQLGETCONNECTATTR:
			iFieldID=IDCB_GCA_FATTRIBUTE;
			iFieldIDType=IDCB_GCA_PARAMETER_TYPE;
			iParFieldID=IPAR_GCA_FATTRIBUTE;
			iParFieldIDType=IPAR_GCA_PARAMETER_TYPE;
			iParRgbValue=IPAR_GCA_RGBVALUE;
			break;
		case API_SQLGETSTMTOPTION:
			iFieldID=IDCB_GSO_FOPTION;
			iFieldIDType=IDCB_GSO_PARAMETER_TYPE;
			iParFieldID=IPAR_GSO_FOPTION;
			iParFieldIDType=IPAR_GSO_PARAMETER_TYPE;
			iParRgbValue=IPAR_GSO_PVPARAM;
			break;
		case API_SQLSETSTMTOPTION:
			iFieldID=IDCB_SSO_FOPTION;
			iFieldIDType=IDCB_SSO_PARAMETER_TYPE;
			iParFieldID=IPAR_SSO_FOPTION;
			iParFieldIDType=IPAR_SSO_PARAMETER_TYPE;
			iParRgbValue=IPAR_SSO_VPARAM;
			break;
		case API_SQLSETCONNECTOPTION:
			iFieldID=IDCB_SCO_FOPTION;
			iFieldIDType=IDCB_SCO_PARAMETER_TYPE;
			iParFieldID=IPAR_SCO_FOPTION;
			iParFieldIDType=IPAR_SCO_PARAMETER_TYPE;
			iParRgbValue=IPAR_SCO_VPARAM;
			break;
		case API_SQLGETCONNECTOPTION:
			iFieldID=IDCB_GCO_FOPTION;
			iFieldIDType=IDCB_GCO_PARAMETER_TYPE;
			iParFieldID=IPAR_GCO_FOPTION;
			iParFieldIDType=IPAR_GCO_PARAMETER_TYPE;
			iParRgbValue=IPAR_GCO_PVPARAM;
			break;
		default:
			;
	}

	if (!fProcessed)
	{
		switch(msg)
		{
			case WM_INITDIALOG:
				if (lpCtl->wID==iFieldIDType)
				{
					DoInitDialog(hDlg,lpDlg,lpCtl,lpParm,iFieldID,iFieldIDType,
						iParFieldID, iParFieldIDType,iParRgbValue);
				}
			break;

			case USER_CBN_UPDATE_CONTENTS:
			case USER_CBN_KILL_FOCUS:
			case USER_CBN_SELCHANGE:
				return(DoUpdateContents(hDlg,lpDlg,lpCtl,lpParm,iFieldID,iFieldIDType,
					iParFieldID, iParFieldIDType,iParRgbValue));
				break;

		} // switch (msg)
	}

	return TRUE;

} //SetCTypeWndProc()


//*------------------------------------------------------------------------
//|  SelectHandleFromTypeField:
//|		This function sets the appropriate handle based on what fHandleType
//|	is set
//|	Returns:
//|		Nothing
//*------------------------------------------------------------------------
VOID INTFUN SelectHandleFromTypeField(HWND hDlg,lpCONNECTIONINFO lpCI,WORD wID,WORD *pidxSelText,
								 WORD iHandle)
{
	//if this is called by lpSQLFreeHandle then we need to match up
	// the handles different.
	BOOL		fAllocHandle=(wID == IDCB_AH_FHANDLETYPE),
				fDescHandle=FALSE;
	TCHAR		szString[SMALLBUFF];
	DWORD		dwDex=0,
				dwHandleType=0;
	HWND		hwndCtl=GetDlgItem(hDlg,wID);

	//get currently selected Handle Type from Combo-box
	dwDex = (SWORD)SendDlgItemMessage(hDlg, wID, CB_GETCURSEL, 0, 0L);

	if(dwDex != CB_ERR)
		//Get actual handle type
		dwHandleType=(DWORD)SendMessage(hwndCtl,CB_GETITEMDATA,(WPARAM)dwDex, 0L);

	switch(dwHandleType)
	{
		case SQL_HANDLE_ENV:
		case SQL_HANDLE_SENV:
			if (fAllocHandle)
				lstrcpy(szString,szSQL_NULL_HANDLE);
			else
				lstrcpy(szString,szHENVSELECT);
			break;

		case SQL_HANDLE_DBC:
			if (fAllocHandle)
				lstrcpy(szString,szHENVSELECT);
			else
				lstrcpy(szString,szHDBCSELECT);
			break;
		case SQL_HANDLE_STMT:
			if (fAllocHandle)
				lstrcpy(szString,szHDBCSELECT);
			else
				lstrcpy(szString,szHSTMTSELECT);
			break;

		case SQL_HANDLE_DESC:
			//If there are no descriptor records (i.e a 2.5 datasource)
			//then select SQL_NULL_HANDLE
			if (lpCI && lpCI->lpDesc)
			{
				if (fAllocHandle)
					lstrcpy(szString,szHDBCSELECT);
				else
				{
					lstrcpy(szString,szHDESCSELECT);
					fDescHandle=TRUE;
				}
			}
			else
				lstrcpy(szString,szNULLPTR);
			break;
		default:
			lstrcpy(szString,szNULLPTR);
	}

	//If we are freeing an hdesc the combobox has already been updated.
	if (!fDescHandle)
	{
		//update ComboBox
		SendDlgItemMessage(hDlg,iHandle,CB_SELECTSTRING,(WPARAM)-1,(LPARAM)szString);
	}

} //SelectHandleFromTypeField()


//*------------------------------------------------------------------------
//|  GetSelectedHandleWndProc:
//|		This function sets the current handle selection to the correct
//|	handle for the given fHandleType and also sets the hstmt drop down
//|	to <hstmt> on the INITDIALOG for those 3.0 API's that require hstmt's
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN GetSelectedHandleWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	HWND					hwnd=NULL;
	lpDIALOGDESC		lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL			lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM			lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	WORD					idxGetText,
							idxSelText,
							idxValid=INVALID_VALUE,
							iHandleType=0,
							iHandle=0;
	lpCONNECTIONINFO	lpCI=NULL;
	lpDESCRIPTORHEADER lpDesc=NULL;

	if (!lpDlg || !lpDlg->ci)
		return TRUE;

	lpCI=lpDlg->ci;
	lpDesc=lpCI->lpDesc;

	switch (lpDlg->iFuncDex+1)
	{
		case API_SQLGETDIAGFIELD:
			iHandleType=IDCB_GDF_FHANDLETYPE;
			iHandle=IDCB_GDF_HHANDLE;
			break;
		case API_SQLGETDIAGREC:
			iHandleType=IDCB_GDR_FHANDLETYPE;
			iHandle=IDCB_GDR_HHANDLE;
			break;
		case API_SQLENDTRAN:
			iHandleType=IDCB_ET_FHANDLETYPE;
			iHandle=IDCB_ET_HHANDLE;
			break;
		case API_SQLFREEHANDLE:
			iHandleType=IDCB_FH_FHANDLETYPE;
			iHandle=IDCB_FH_INPUTHANDLE;
			break;
		case API_SQLALLOCHANDLE:
			iHandleType=IDCB_AH_FHANDLETYPE;
			iHandle=IDCB_AH_INPUTHANDLE;
			break;
	}

	switch (msg)
	{
		case WM_INITDIALOG:
			{
			idxGetText=iHandleType;
			idxSelText=idxValid=iHandle;
			}
			break;

		case USER_CBN_KILL_FOCUS:
		case USER_CBN_SELCHANGE:
		case USER_IDOK:
			{
			idxGetText=idxValid=iHandleType;
			idxSelText=iHandle;
			}
			break;

	}


	if (lpCtl->wID == idxValid && iHandle !=IDCB_FH_INPUTHANDLE )
		SelectHandleFromTypeField(hDlg,lpDlg->ci,idxGetText,&idxSelText,iHandle);

	if (msg == USER_IDOK)
	{
		TCHAR		szHandle[MEDBUFF];

		//See if user selected HDESC
		GetText(GetDlgItem(hDlg, idxGetText), szHandle);

		// Set the Current selection so the correct Descriptor
		// handle can be set
		if (_tcsstr(szHandle,szSQL_HANDLE_DESC))
			lpCI->lpCurDesc=SetCurrentDesc(hDlg,iHandle,lpDesc,lpCI->cDescs);
	}

	return TRUE;

} //GetSelectedHandleWndProc()


//*------------------------------------------------------------------------
//|  DisplayRowsetWndProc:
//|		This function handles the "Display Rowset" check box on SQLFetch,
//|	SQLExtendedFetch,SQLFetchScroll
//|	Returns:
//|		TRUE if we've handled the message, FALSE if not
//*------------------------------------------------------------------------
BOOL INTFUN DisplayRowsetWndProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
				lpVOID lpUsr, UINT cDex)
{
	HWND					hwnd=NULL;
	lpDIALOGDESC		lpDlg = (lpDIALOGDESC)lpUsr;
	lpCONTROL			lpCtl = lpDlg->lpCtl[cDex];
	lpPARAMITEM			lpParm = lpDlg->lpParms[lpCtl->iParmNum];
	lpSTATEMENTINFO 	lpStmt=NULL;
	UINT					idDisplayRowset;
	BOOL					fStmtAvail = lpDlg && lpDlg->ci && lpDlg->ci->lpCurStmt;


	switch (lpDlg->iFuncDex+1)
	{
		case API_SQLFETCHSCROLL:
			idDisplayRowset=IDCK_FS_DISPLAY_ROWSET;
			break;
		case API_SQLFETCH:
			idDisplayRowset=IDCK_F_DISPLAY_ROWSET;
			break;
		case API_SQLEXTENDEDFETCH:
			idDisplayRowset=IDCK_EF_DISPLAY_ROWSET;
			break;
	}

	switch (msg)
	{
		case WM_INITDIALOG:

			EnableWindow(GetDlgItem(hDlg,idDisplayRowset),fStmtAvail);

			CheckDlgButton(hDlg, idDisplayRowset,lpUsrOptions->fAutoDisplay);
			break;

		case USER_IDOK:
			{
				if (fStmtAvail)
				{
					lpStmt=lpDlg->ci->lpCurStmt;
					lpStmt->fAutoDisplay=IsDlgButtonChecked(hDlg, idDisplayRowset);
				}
			}
			break;
	}

	return TRUE;

} //DisplayRowsetWndProc()

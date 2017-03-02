//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGTOOLS.C
//|
//| Purpose:
//|		Contains tools used by the dialog routines to format data in
//|	various controls, etc.
//*---------------------------------------------------------------------------------
#include "fhheader.h"
#include "blddata.h"
#include "bigint.h"
#pragma warning(disable:4996)
#define MAX_TIMESTAMP_FRAC_NUM							999999999
#define MAX_HOUR_NUM											99999
#define MIN_HOUR_NUM											-99999
#define INTERVAL_LEADING_PRECISION_DEFAULT			2
#define INTERVAL_LEADING_SECONDS_PRECISION_DEFAULT	6
#define DONT_ADD_QUOTES										0
#define ADD_QUOTES											1

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE 			hInst;
extern HWND					hwndClient;
extern HWND					hwndStatBar;
extern HWND					hwndStatBar;
extern TCHAR 				szOutBuff[MAXOUTBUFF];
extern lpUSROPTIONS		lpUsrOptions;

dCSEG(TCHAR) szQUOTE[]							= TEXT("\"");
dCSEG(TCHAR) szELIPSE[]							= TEXT("...");
dCSEG(TCHAR) szNULLTERMSTR[]					= TEXT("\\0");
dCSEG(TCHAR) szFMTDATE[] 						= TEXT("%04d-%02u-%02u");
dCSEG(TCHAR) szFMTTIME[] 						= TEXT("%02u:%02u:%02u");
dCSEG(TCHAR) szFMTTIMESTAMP[] 				= TEXT("%04d-%02u-%02u %02u:%02u:%02u");
dCSEG(TCHAR) szFMTNUMERIC[]					= TEXT("-%02u%lu");
dCSEG(TCHAR) szFMTYEARMONTH[] 				= TEXT("%04d-%02u");
dCSEG(TCHAR) szFMTTIMESTAMPFRAC[]			= TEXT(".%09lu");
dCSEG(TCHAR) szHEXPREFIX[]						= TEXT("0x");
dCSEG(TCHAR) szTenTabs[]						= TEXT("\t\t\t\t\t\t\t\t\t\t");
dCSEG(TCHAR) szLogIn[]							= TEXT("\t\t\t\tIn:\t\t\t\t");
dCSEG(TCHAR) szLogOut[]							= TEXT("\t\t\t\tOut:\t\t\t");
dCSEG(TCHAR) szUnmodified[]					= TEXT("<unmodified>");
dCSEG(TCHAR) szNineTabs[]						= TEXT("\t\t\t\t\t\t\t\t\t");
dCSEG(TCHAR) szCTypeInvalid[]					= TEXT("Invalid fCType: %d");
dCSEG(TCHAR) szBinaryConv[]					= TEXT("binary conversion");
dCSEG(TCHAR) szStringCRLF[]					= TEXT("%s:\r\n");
dCSEG(TCHAR) szFMTINTERVAL[]	 				= TEXT("%04d-%02u-%02u %02u:%02u:%02u");
dCSEG(TCHAR) sz2DIG_INTVAL_FIELD[]	 		= TEXT("INTERVAL %s '%02d' %s");
dCSEG(TCHAR) sz4DIG_INTVAL_FIELD[]	 		= TEXT("INTERVAL %s '%04d' %s");





//*---------------------------------------------------------------------------------
//| RemoveDlgFormat:
//|	The text kept with each paramters is in Dialog format.  This function will
//|	remove the & and : chars leaving only a valid name for output
//| Parms:
//|	str						The string to strip
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN RemoveDlgFormat(LPTSTR str)
{
	LPTSTR		lstr=str;
	if(lstr = _tcschr(str, TEXT('&')))
		lstrcpy(lstr, lstr+1);
	if(lstr = _tcschr(str, TEXT(':')))
		*lstr = TEXT('\0');
}


//*------------------------------------------------------------------------
//|  szBufferParmInfo:
//|		Will do buffering of parameters very much like szPrintf.  This
//|		function will handle the logic of fitting all parameters in one
//|		screen-full of data.
//|  Parms:
//|		pfFlushed		Flag for flush, TRUE if the buffer is empty
//|		str				The latest string to output
//|		hwnd				Edit window
//|  Returns:
//|		Nothing.  *pfFlushed is set on output
//|		TRUE if everything was successful, FALSE if there was an error
//*------------------------------------------------------------------------
BOOL INTFUN szBufferParmInfo(BOOL * pfFlushed, LPTSTR str, HWND hwnd)
{
	DWORD				newlen=lstrlen(str);
	static TCHAR		szBuff[(DFT_PARM_LEN_MAX + 1)];
	static int		cBuff;


	// If we are finished with this line or the line is getting too large
	//   for our buffer, then flush it to the screen
	if(_tcschr(str, TEXT('\n')) || (newlen + cBuff + 2) >= lpUsrOptions->uDftParmLen)
	{
		// Flush buffer if required
		if(cBuff)
		{
			szBuff[cBuff] = TEXT('\0');
			szPrintf(lpUsrOptions, hwnd, MSG_SEV2, TRUE, FALSE, szBuff);
			if(szBuff[cBuff-1] != TEXT('\n') &&	!_tcschr(str, TEXT('\n')))	// No new line in buffer
			{
				// and none in current
				szPrintf(lpUsrOptions, hwnd, MSG_SEV2,TRUE, TRUE, TEXT("\r\n"));	// So force it out

				if(*str != TEXT('\r'))										// New line will require indent before current parm
					szPrintf(lpUsrOptions, hwnd, MSG_SEV2, TRUE, FALSE, (LPTSTR)szTenTabs);
				else
					*pfFlushed = TRUE;
			}
			*szBuff = TEXT('\0');
			cBuff = 0;
		}
		// Write the string
		szPrintf(lpUsrOptions, hwnd, MSG_SEV2, FALSE, FALSE, str);
	}
	// No need to flush the line yet, so simply add it to our buffer
	else
	{
		lstrcpy(&szBuff[cBuff], str);
		cBuff += (UINT)newlen;
		*pfFlushed = FALSE;
	}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| SetTabs:
//|	This function simply copies cTabs tabs into the string and adds a null.
//| Parms:
//|	str						String to set
//|	cTabs						Number of tabs to set
//| Returns:
//|	Pointer to user buffer
//*---------------------------------------------------------------------------------
VOID INTFUN SetTabs(LPTSTR str, SWORD cTabs)
{
	LPTSTR lstr = str;

	if(!lstr)
		return;
	while(cTabs--)
		*lstr++ = TEXT('\t');
	*lstr = TEXT('\0');
}


//*---------------------------------------------------------------------------------
//| ParamNeedsDeref:
//|		This function determines if we are processing a parameter that needs to
//|	be dereferenced to get the data value.
//| Parms:
//|	uParmType		parameter type
//| Returns:
//|	TRUE or FALSE
//*---------------------------------------------------------------------------------

__inline BOOL INTFUN ParamNeedsDeref(UDWORD uParmType)
{
	return(uParmType & PT_PTR_PTR ||
			 uParmType & PT_PTR_SWORD ||
			 uParmType & PT_PTR_SDWORD ||
			 uParmType & PT_PTR_UWORD ||
			 uParmType & PT_PTR_BIN ||
			 uParmType & PT_LOG_OUT ||
			 uParmType & PT_INPUT_OUTPUT ||
			 uParmType & PT_PTR_UDWORD);

} //ParamNeedsDeref()

//*---------------------------------------------------------------------------------
//| IsOutputParam:
//|		This function determines if we are processing an output parameter
//| Parms:
//|	uParmType		parameter type
//| Returns:
//|	TRUE or FALSE
//*---------------------------------------------------------------------------------

__inline BOOL INTFUN IsOutputParam(UDWORD uParmType)
{
	return(uParmType & PT_LOG_OUT ||
			 uParmType & PT_LOG_IO ||
			 uParmType & PT_INPUT_OUTPUT ||
			 uParmType & PT_LOG_DEFFERED);

} //IsOutputParam()

//*---------------------------------------------------------------------------------
//| IsParamLoggable:
//|		This function determines if we are processing a loggable parameter
//| Parms:
//|	uParmType		parameter type
//| Returns:
//|	TRUE or FALSE
//*---------------------------------------------------------------------------------

__inline BOOL INTFUN IsParamLoggable(UDWORD uParmType)
{
	return(!(uParmType & PT_ROWSET_CTL ||
				uParmType & PT_NONE));

} //IsOutputParam()


//*---------------------------------------------------------------------------------
//| GetParamCount:
//|	This function returns the count of either input or output parms
//| Parms:
//|	lpParmArray				Pointer to the array of parameters
//|	uParmType				Input or Output Parameter
//|	cParms					Count of parameters
//| Returns:
//|	parameter count
//*---------------------------------------------------------------------------------

UINT INTFUN GetParamCount(BOOL fIn,lpPARAMITEM *lpParmArray,UINT cParms)
{
	UINT	uCnt=0,
			uDex;
	UDWORD	fFound=FALSE,
				fLogIO=FALSE,
				fLogInOut=FALSE,
				fLogDeferred=FALSE;

	for(uDex=0, uCnt=0;  uDex<cParms;  uDex++)
	{
		if (fIn)
			uCnt+=IsParamLoggable(lpParmArray[uDex]->uParmType);
		else
			uCnt+=IsOutputParam(lpParmArray[uDex]->uParmType);
	}

	return(uCnt);


} //GetParamCount()



//*---------------------------------------------------------------------------------
//| FormatDefaultValue:
//|	This function retrieves the default value from the parameter, locates
//|	it in the default array and formats the constant name.
//| Parms:
//|	lpParm			parameter to log
//|	str				output string
//| Returns:
//|	TRUE if the parameter's data was found in the default array, otherwise FALSE
//*---------------------------------------------------------------------------------

BOOL INTFUN FormatDefaultValue(lpPARAMITEM lpParm,LPTSTR str)
{
	SDWORD	sdValBuf;
	BOOL		fFound=FALSE;
	UINT		dex2;

	switch(lpParm->fCType)
	{
		case SQL_C_SHORT:
		case SQL_C_SSHORT:
			sdValBuf = (SDWORD)(*(SWORD *)lpParm->lpData);
			break;
		case SQL_C_USHORT:
			sdValBuf = (SDWORD)(*(UWORD *)lpParm->lpData);
			break;
		case SQL_C_LONG:
		case SQL_C_SLONG:
			sdValBuf = *(SDWORD *)lpParm->lpData;
			break;
		case SQL_C_ULONG:
			sdValBuf = (SDWORD)(*(UDWORD *)lpParm->lpData);
			break;
		default:
			goto nodata;
	}

	// First check to see if the index into the default array matches the actual value
	// If it does, use it.  Otherwise search through the list for the value
	if(lpParm->dwDex < (SDWORD)lpParm->cDfts)
	{
		if(sdValBuf == lpParm->lpDfts[lpParm->dwDex].fOpt)
			fFound = TRUE;
	}

	if(!fFound)
	{
		// Now step throught the default array looking for the value
		for(dex2=0; dex2<lpParm->cDfts; dex2++)
		{
			if(sdValBuf == lpParm->lpDfts[dex2].fOpt)
			{
				lpParm->dwDex = (SWORD)dex2;
				fFound = TRUE;
				break;
			}
		}
	}

nodata:
	// Now print it out if we found it, otherwise just format it based on its type
	if(fFound)
		GetExpandedConstant(str, &(lpParm->lpDfts[lpParm->dwDex]), FALSE);

	return(fFound);

} //FormatDefaultValue()

//*---------------------------------------------------------------------------------
//| LogParametersSpecial:
//|	This function will log the input or output parameters depending on
//|	the options specified for each, with the addition of handles, bitmasks,
//|	and linked arrays.
//|	There are basically 4 types of paramters
//|		1) Input Paramters
//|		2) Output Paramters
//|		3) Deferred Input/Output Parameters
//| Parms:
//|	lpParmArray				Pointer to the array of parameters
//|	cParms					Count of parameters
//|	ci							Connection information (used for logging)
//|	fIn						TRUE to log input parms, FALSE for output
//|	uValInfo					Format of the special parameter (ie. PRM_32HDL, PRM_32BIT, PRM_32MSK, PRM_16BIT)
//|	iParmDft					The parameter number from which the linked array is found
//|	iParmNum             The parameter number which needs special formatting
//| Returns:
//|	Pointer to user buffer
//*---------------------------------------------------------------------------------
BOOL INTFUN LogParametersSpecial(LPTSTR szFuncName, lpPARAMITEM * lpParmArray, UINT cParms,
			lpCONNECTIONINFO ci, BOOL fIn, UDWORD uValInfo, SDWORD iParmDft, SDWORD iParmNum)
{
	UINT				dex, uCnt;
	lpPARAMITEM		lpParm;
	BOOL				fFlushed=FALSE;				// Used to flush buffer
	LPTSTR			szOutParms;						// For output which can get long
	LPTSTR			str;
	HWND				hwndOut;                   // For conn win out or generic output
	HENV				*phenv=NULL;
	HDBC				*phdbc=NULL;
	HSTMT				*phstmt=NULL;
	SQLHDESC			*phdesc=NULL;
	UINT				uStmtNum=0;
	BOOL				fNoErrors=TRUE;
	BOOL				fFound=FALSE;
	HENV				henv=GetActiveEnvHandle();
	int				nExtra=lstrlen(TEXT(", ")) + sizeof(TCHAR);
	int				nBufLen=0;
	BOOL				fDftFound=FALSE;
	TCHAR				szName[LARGEBUFF];
	UDWORD			size;



	hwndOut = GETOUTPUTWINDOW(ci);

	// Look for handles for logging
	if(henv)
		phenv = &henv;

	if(ci)
	{
		if(ci->hdbc)
			phdbc = &ci->hdbc;

		if(ci->lpCurStmt)
		{
			if(ci->lpCurStmt->hstmt)
				phstmt = &ci->lpCurStmt->hstmt;
			uStmtNum = ci->lpCurStmt->uStmtNum;
		}

		if(ci->lpCurDesc)
			phdesc = &ci->lpCurDesc->hdesc;
	}

	// Return right away if user doesn't want parameters logged
	if(lpUsrOptions->uLogLevel < MSG_SEV2)
		return TRUE;

	uCnt=GetParamCount(fIn,lpParmArray,cParms);

	// Print function header
	if(fIn)
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE,
					(LPTSTR)szStringCRLF, szFuncName);

	if(!uCnt)
		return TRUE;					// Nothing to log

 	// User specified how large a parameter line is
	// allocate the larger of uDftBuffLen and uDftParmLen
	// this fixes both bugs.
	if ( lpUsrOptions->uDftBuffLen > lpUsrOptions->uDftParmLen )
		size = lpUsrOptions->uDftBuffLen;
	else
		size = lpUsrOptions->uDftParmLen;

	str = szOutParms = AllocateMemory(size * sizeof(TCHAR));

	InitUnModified(str, size);

	if(!szOutParms)
		return FALSE;

	*str = TEXT('\0');


	// Print either the In or out
	szBufferParmInfo(&fFlushed,
				(fIn) ? (LPTSTR)szLogIn :
						  (LPTSTR)szLogOut, hwndOut);

	// Loop through each parameter and log according to option
	for(dex=0;  dex<cParms;  dex++)
	{
		//Reset for each parameter
		fDftFound=FALSE;

		lpParm = lpParmArray[dex];

		//if parameter is not loggable then skip to next parameter
		if (!IsParamLoggable(lpParm->uParmType))
			continue;

		if (!fIn) //We are processing output parameters now.
		{
			if (!IsOutputParam(lpParm->uParmType))
			{
				//The current parameter is not an output parameter,
				//skip to next parameter
				continue;
			}
			else if (ParamNeedsDeref(lpParm->uParmType))
			{
				//This parameter needs to be dereferenced so
				//add the asterisk
				lstrcpy(str, TEXT("*"));
			}
		}

		// Format the parameter name
		lstrcat(str, lpParm->szName);									// Parameter name, formatted for Dialog
		RemoveDlgFormat(str);											// Remove the '&' and ':' chars
		lstrcpy(szName,str);
		lstrcat(str, TEXT(" = "));

		// Check for a default array.  If there is one, attempt to locate the value
		//		in the array to add the text to it.
		str += lstrlen(szOutParms);

		if(!fIn && UnModified(lpParm->lpData, lpParm->cBytes))
		{
			lpParm->uParmType |= PT_OUT_NOTMODIFIED;
			lstrcpy(str, szUnmodified);
		}
      // Check to see if this is the parameter that needs special formatting
      else if ((SDWORD)dex == iParmNum)
		{
			if (!lpParm->lpData || lpParm->fNull)
				lstrcpy(str, szNULL);
			else
				fNoErrors=FormatSpecialParameters(lpParmArray[iParmDft]->lpDfts, lpParmArray[iParmDft]->cDfts,
										 lpParmArray[iParmDft]->lpData, lpParmArray[dex]->lpData,
										 hwndOut, uValInfo, &szOutParms, &str, &fFlushed,
										 szName);
		}
		else
		{
			if (!lpParm->fNull && lpParm->lpDfts && (lpParm->dwDex != -1))
			{
				//Parameter has default array and a user selected a value
				//in the default array.
				fDftFound=FormatDefaultValue(lpParm,str);

			}

			if (!fDftFound || (lpParm->lpDfts && lpParm->dwDex == -1))
			{
				// There is no default array, or the value the user entered was not in the
				//		default array, so just print its value by type

				GetCharValueFromParm(str,
							lpUsrOptions->uDftParmLen - lstrlen(szOutParms)*sizeof(TCHAR) - nExtra,
							lpParm, fIn,
							phenv,
							phdbc,
							phstmt,
							phdesc,
							uStmtNum,
							hwndOut);
			}
		}

		// We've just formatted a parameter, if there are more left, add a comma
		if(--uCnt && fNoErrors)
			lstrcat(str, TEXT(", "));

		// Now print the newest parameter and reset the strings
		szBufferParmInfo(&fFlushed, szOutParms, hwndOut);
		str = szOutParms;
		*szOutParms = TEXT('\0');

		// If we've written the line and there are more parms, start a new one
		if(fFlushed && uCnt)
			szBufferParmInfo(&fFlushed, (LPTSTR)szTenTabs, hwndOut);

	} //for(dex=0;  dex<cParms;  dex++)

	// All parameters have been logged, so make sure all has been written
	if(!fFlushed)
		szBufferParmInfo(&fFlushed, TEXT("\r\n"), hwndOut);

	FREE_PTR(szOutParms);

	szOutParms=NULL;

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| FormatSpecialParameters:
//|	This function formats parameters that are handles, bitmasks, or are values
//|	from a linked array and prints them to the screen.
//| Parms:
//|	lpDft						Pointer to the default array
//|	cDfts						Count of items in the default array
//|	lpData					Pointer to the data in the option parameter
//|	lpLinkData				Pointer to the data in the linked parameter
//|	hwndOut					Handle to the output window currently active
//|	uValInfo					Format of the special parameter (PRM_32HDL, PRM_32BIT, PRM_32MSK, PRM_16BIT)
//|	plpOutParms				Pointer to a pointer to a character buffer for output
//|	pLPTSTR					Pointer to a pointer into the plpOutParms character buffer
//|	pfFlushed				Flag as to whether or not the output buffer has been cleared
//|	szName					Name of parameter
//| Returns:
//|	TRUE if special formatting (and printing) is done and no errors are encountered
//|	FALSE if no formatting is done, or if errors are encountered
//*---------------------------------------------------------------------------------
BOOL INTFUN FormatSpecialParameters(lpDFTARRAY lpDfts, UINT cDfts, PTR *lpData, PTR *lpLinkData, HWND hwndOut,
												UDWORD uValInfo, LPTSTR *plpOutParms, LPTSTR *pLPTSTR, BOOL *pfFlushed,
												LPTSTR szName)
{
   TCHAR		szTemp[SMALLBUFF];
   UINT		idx,idx2;
   BOOL		fErrors=FALSE;


	// Now switch on the special format type
	switch(uValInfo)
		{
		case PRM_PTR_BIN:
		//This parameter points to a binary value (e.g. BookMarks)
	   {
	   	HANDLE	uHdlTemp = *(HANDLE *)lpLinkData,
						hValue=NULL;
		   TCHAR		szHandle[SMALLBUFF];

			HandleToText(szTemp,uHdlTemp);

			lstrcat(*pLPTSTR,szTemp);
			//this parameter is pointing to a binary value
			//we need to display its contents as well
			if (uHdlTemp)
			{
				hValue=*(HANDLE *)uHdlTemp;
				HandleToText(szHandle,hValue);
				wsprintf(*pLPTSTR,TEXT("%s,**%s = %s"),szTemp,szName,szHandle);

			}
		}
	   break;

		case PRM_PTR_32BIT:
		//This parameter points to a 32 bit value
	   {
	   	HANDLE	uHdlTemp = *(HANDLE *)lpLinkData;
			SDWORD	sdValue=0;

			HandleToText(szTemp,uHdlTemp);

			lstrcat(*pLPTSTR,szTemp);
			//this parameter is pointing to a 32 bit value
			//we need to display its contents as well
			if (uHdlTemp)
			{
				sdValue=*(SDWORD *)uHdlTemp;
				wsprintf(*pLPTSTR,TEXT("%s,*%s = %d"),szTemp,szName,sdValue);
			}

		}
		break;

		case PRM_PTR_16BIT:
		//This parameter points to a 16 bit value
	   {
	   	HANDLE	uHdlTemp = *(HANDLE *)lpLinkData;
			SWORD		sValue=0;

			HandleToText(szTemp,uHdlTemp);

			lstrcat(*pLPTSTR,szTemp);
			//this parameter is pointing to a 32 bit value
			//we need to display its contents as well
			if (uHdlTemp)
			{
				sValue=*(SWORD *)uHdlTemp;
				wsprintf(*pLPTSTR,TEXT("%s,*%s = %d"),szTemp,szName,sValue);

			}
		}
		break;

		case PRM_PTR_STR:
		//This parameter points to a string
	   {
	   	HANDLE	uHdlTemp = *(HANDLE *)lpLinkData;
			LPTSTR	pString=NULL;

			HandleToText(szTemp,uHdlTemp);

			lstrcat(*pLPTSTR,szTemp);
			//this parameter is pointing to a 32 bit value
			//we need to display its contents as well
			if (uHdlTemp)
			{
				pString=*(LPTSTR *)uHdlTemp;
				wsprintf(*pLPTSTR,TEXT("%s,*%s = %s"),szTemp,szName,pString);
			}
		}
		break;


		case PRM_32HDL:		// 32-bit handle
	   	{
	   	HANDLE	uHdlTemp = *(HANDLE *)lpLinkData;

			HandleToText(szTemp,uHdlTemp);

			lstrcat(*pLPTSTR,szTemp);
	   	break;
	   	}
	   case PRM_32MSK:		// 32-bit bitmask
	   	{
			SWORD	dOption = *(SWORD *)lpData;
	   	UDWORD	uFlag=0,uTemp=1,uBuff=0;
	   	BOOL		fFound=FALSE,fFirstTime=TRUE;
			UWORD		dTmp;

	   	// First output the hexidecimal equivalent of the bitmask data
	   	wsprintf(szTemp,TEXT("0x%08lX"), *(UDWORD *)lpLinkData);
	   	if(*(UDWORD *)lpLinkData != 0)
				lstrcat(szTemp,TEXT(" = "));
	   	lstrcat(*plpOutParms,szTemp);

	   	// Step through each bit in the mask and see if it matches an option in the linked array
	   	for(idx=0;idx<(8*sizeof(UDWORD));idx++){
	   		uBuff = *(UDWORD *)lpLinkData;
	   		uBuff &= uTemp;
	   		if(uBuff){
	   			// Find the correct item in the default array
	   			for(dTmp=0;dTmp<cDfts;dTmp++){

						//Bug #464 Since SQL_OJ_CAPABILITIES = 65003 util 3.0
						//this kludge is needed.
						if (dTmp==115)
							{
							if (lpDfts[dTmp].fOpt == SQL_OJ_CAPABILITIES)
								break;
							}
	   				else
							{
	   					if(dOption == (SWORD)lpDfts[dTmp].fOpt)
	   						break;
							}

	   				}

	   			// Search for that default bitmask
	   			for(idx2=0;idx2<(lpDfts[dTmp].cLinks);idx2++)
	   				{
	   				if((UDWORD)lpDfts[dTmp].lpLink[idx2].fOpt == uTemp){
	   					if(!fFirstTime)
	   						lstrcat(*plpOutParms,TEXT(" | "));
	   					lstrcat(*plpOutParms,(LPTSTR)lpDfts[dTmp].lpLink[idx2].szOpt);
	   					fFound=TRUE;
	   					fFirstTime=FALSE;
	   					}
	   				}
	   			if(!fFound)
	   				uFlag |= uTemp;
	   			else {
						// Now print the newest option (bitmask) and reset the strings
						szBufferParmInfo(pfFlushed, *plpOutParms, hwndOut);
						*pLPTSTR = *plpOutParms;
						**plpOutParms = TEXT('\0');

						// If we've written the line, start a new one
						if((*pfFlushed))
							szBufferParmInfo(pfFlushed, (LPTSTR)szTenTabs, hwndOut);
	   				fFound=FALSE;

						}
	   			}
	   		uTemp<<=1;		// Shift mask to the right one
	   		}
	   		// If any of the bitmask was unnamed, then print an error message listing the bits
	   		if(uFlag){
	            if(!(*pfFlushed))
						szBufferParmInfo(pfFlushed, TEXT("\r\n"), hwndOut);
					szPrintf(lpUsrOptions, hwndOut,
								MSG_SEV0,
								TRUE, TRUE,
								GetGtrString(szTemp, MAXOUTBUFF, TST0007),
								uFlag);
					fErrors=TRUE;
					// We just printed the error message, so start a new line
					szBufferParmInfo(pfFlushed, (LPTSTR)szNineTabs, hwndOut);
	            }
	   	break;
	   	}
	   case PRM_32BIT:		// 32-bit value from a linked array
	   case PRM_16BIT:		// 16-bit value from a linked array
	   	{
			SWORD	dOption = *(SWORD *)lpData;
			SDWORD	dData;
			BOOL		fFound=FALSE;

			if(uValInfo == PRM_16BIT)	// 16-bit integer
				dData = (SWORD)*(SWORD *)lpLinkData;
			else								// 32-bit integer
				dData = *(SDWORD *)lpLinkData;

			// Find the correct item in the default array
			for(idx=0;idx<cDfts;idx++){
				if(dOption == (SWORD)lpDfts[idx].fOpt)
					break;
				}

			// Search for that default integer in the linked array
			for(idx2=0;idx2<(lpDfts[idx].cLinks);idx2++){
				if(lpDfts[idx].lpLink[idx2].fOpt == dData){
					wsprintf(szTemp,TEXT("%s = %d"),
								lpDfts[idx].lpLink[idx2].szOpt,
								lpDfts[idx].lpLink[idx2].fOpt);
					lstrcat(*pLPTSTR,szTemp);
					fFound=TRUE;
					break;
					}
				}
			// Also trap the case where zero is returned (since it is valid as per 2.0 spec)
			if(!fFound)
				if(dData == 0){
					wsprintf(szTemp,TEXT("%d"),dData);
					lstrcat(*pLPTSTR,szTemp);
					szBufferParmInfo(pfFlushed, *plpOutParms, hwndOut);
					**plpOutParms = TEXT('\0');
	            if(!(*pfFlushed))
						szBufferParmInfo(pfFlushed, TEXT("\r\n"), hwndOut);
					szPrintf(lpUsrOptions, hwndOut,
								MSG_SEV0,
								TRUE, TRUE,
								GetGtrString(szTemp, MAXOUTBUFF, TST1006),
								lpDfts[idx].szOpt);
					fErrors=TRUE;
					// We just printed the error message, so start a new line
					szBufferParmInfo(pfFlushed, (LPTSTR)szNineTabs, hwndOut);
					fFound=TRUE;
					}
			// If no item was found in the linked array, print the value and an error message
			if(!fFound){
				wsprintf(szTemp,TEXT("%ld"),dData);
				lstrcat(*plpOutParms,szTemp);
				szBufferParmInfo(pfFlushed, *plpOutParms, hwndOut);
				**plpOutParms = TEXT('\0');
            if(!(*pfFlushed))
					szBufferParmInfo(pfFlushed, TEXT("\r\n"), hwndOut);
				szPrintf(lpUsrOptions, hwndOut,
							MSG_SEV0,
							TRUE, TRUE,
							GetGtrString(szTemp, MAXOUTBUFF, TST0006),
							dData, lpDfts[idx].szOpt);
				fErrors=TRUE;
				// We just printed the error message, so start a new line
				szBufferParmInfo(pfFlushed, (LPTSTR)szNineTabs, hwndOut);
				}
	   	break;
	   	}
	   default:
	   	return FALSE;
	   }
	if(fErrors)
		return FALSE;
	else
		return TRUE;
}

int wcslen_unaligned(const WCHAR UNALIGNED *str)
{
    const WCHAR UNALIGNED *pch;

    if ((PtrToUlong(str) & 1) == 0) {
        return (int)wcslen((LPCWSTR)str);
    }

    pch = str;
    while (*pch != UNICODE_NULL) {
        pch++;
    }

    return (int)((ULONG_PTR)(pch - str) >> 1);
}

//*---------------------------------------------------------------------------------
//| FormatCharData:
//|	This function formats string data
//|
//| Parms:
//|	szOut						Where to write string
//|	cbOut						Maximum length we can write
//|	lpData					Pointer to the data
//|	fCType					The C type of the data
//|	cBytes					Count of bytes in lpData
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------
BOOL INTFUN FormatCharData(LPTSTR szOut, DWORD cbOut,
			LPTSTR szData, SWORD fCType, UDWORD cBytes,
			BOOL fAddQuotes)
{
	//length of string will be calculated based on
	//2 - double quotes and the elipse chars.
	UINT			cchElipse=lstrlen(szELIPSE);
	BOOL			fOverflow=FALSE;
	DWORD			cbChar=sizeof(TCHAR);
	DWORD			cchData;
	DWORD			cchOut=cbOut/cbChar;
	int				cch=cchOut-cchElipse;
	LPTSTR			lpData=NULL;

	cchData=(fCType == SQL_C_CHAR) ?
		(DWORD)strlen((char*)szData) : wcslen_unaligned((WCHAR *)szData);


	if ( szData)
	{
#ifndef _UNICODE
		if ( fCType == SQL_C_WCHAR)
		{
			lpData = AllocateMemory(cBytes);
			WideCharToMultiByte(CP_ACP,0,(WCHAR *)szData,cBytes,lpData,cBytes,NULL,NULL);
		}
		else
			lpData=szData;
#else
		if( fCType == SQL_C_CHAR)
		{
			lpData = AllocateMemory((cBytes+1) *sizeof(TCHAR));
			MultiByteToWideChar(CP_ACP,0,(char*)szData,cBytes,lpData,cBytes * sizeof(TCHAR));
		}
		else
			lpData = szData;
#endif
	}

	if(fAddQuotes &&
		(( cch -= (int)(lstrlen(szQUOTE)*2 + lstrlen(szNULLTERMSTR)) ) < 0)  )
		return FALSE;


	if(lpData)
	{
		if ((cchData+(cbChar * 3)+cchElipse) >= cchOut)
		{
			fOverflow = TRUE;
			if(cchData < cchElipse)		// Not enough room for trunc warning, quotes and ...
				return FALSE;
		}

		if (fAddQuotes)
			lstrcpy(szOut, szQUOTE);

		if(!fOverflow)
			lstrcat(szOut, lpData);
		else
		{
			// String will be truncated, so copy only what will fit along with ...
			_tcsncpy((LPTSTR)(szOut + 1), lpData, (size_t)cch);
			lstrcpy(&szOut[cch+1], szELIPSE);
		}

		if (fAddQuotes)
			lstrcat(szOut, szQUOTE);
	}
	else
		lstrcpy(szOut, szNULL);

#ifndef _UNICODE
	if ( fCType == SQL_C_WCHAR)
		ReleaseMemory(lpData);
#else
	if ( fCType == SQL_C_CHAR)
		ReleaseMemory(lpData);
#endif

	return(TRUE);

} //FormatCharData()


//*---------------------------------------------------------------------------------
//| FormatCharData:
//|	This function formats INTERVAL data
//|
//| Parms:
//|	fCType					The C type of the data
//|	pIS						pointer to data
//|	cBytes					Count of bytes in lpData
//|	szOut						Where to write string
//|	cbOut						Maximum length we can write
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------

VOID INTFUN FormatIntervalData(SWORD fCType,SQL_INTERVAL_STRUCT *pIS,
										 UDWORD cBytes,LPTSTR szOut, DWORD cbOut,
										 HWND hwndOut)
{
	TCHAR		szSign[TINYBUFF]=TEXT(""),
				szMsg[LARGEBUFF]=TEXT("");

	switch(pIS->interval_sign)
	{
		case SQL_FALSE:
			//leading field is unsigned
			break;
		case SQL_TRUE:
			lstrcpy(szSign,TEXT("-"));
			break;
		default:
			//unknown sign
			szPrintf(lpUsrOptions, hwndOut,
						MSG_SEV1,
						TRUE, TRUE,
						GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST0021),
						pIS->interval_sign);
	}

	switch(pIS->interval_type)
	{
		case SQL_CODE_YEAR:
			wsprintf(szOut,sz4DIG_INTVAL_FIELD,szSign,pIS->intval.year_month.year,TEXT("YEAR"));
			break;
		case SQL_CODE_MONTH:
			wsprintf(szOut,sz2DIG_INTVAL_FIELD,szSign,pIS->intval.year_month.month,TEXT("MONTH"));
			break;
		case SQL_CODE_YEAR_TO_MONTH:
			wsprintf(szOut,TEXT("INTERVAL %s '%04d-%02d' YEAR TO MONTH"),szSign,pIS->intval.year_month.year,
															pIS->intval.year_month.month);
			break;
		case SQL_CODE_DAY:
			wsprintf(szOut,sz2DIG_INTVAL_FIELD,szSign,pIS->intval.day_second.day,TEXT("DAY"));
			break;
		case SQL_CODE_HOUR:
			wsprintf(szOut,sz2DIG_INTVAL_FIELD,szSign,pIS->intval.day_second.hour,TEXT("HOUR"));
			break;
		case SQL_CODE_MINUTE:
			wsprintf(szOut,sz2DIG_INTVAL_FIELD,szSign,pIS->intval.day_second.minute,TEXT("MINUTE"));
			break;
		case SQL_CODE_SECOND:
			if (pIS->intval.day_second.fraction)
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%02d.%04d' %s"),
							szSign,
							pIS->intval.day_second.second,
							pIS->intval.day_second.fraction,
							TEXT("SECOND"));
			}
			else
				wsprintf(szOut,sz2DIG_INTVAL_FIELD,szSign,pIS->intval.day_second.second,TEXT("SECOND"));

			break;
		case SQL_CODE_DAY_TO_HOUR:
			wsprintf(szOut,TEXT("INTERVAL %s '%02d %02d' DAY TO HOUR"),
								szSign,
								pIS->intval.day_second.day,
								pIS->intval.day_second.hour);
			break;
		case SQL_CODE_DAY_TO_MINUTE:
			wsprintf(szOut,TEXT("INTERVAL %s '%d %02d:%02d' DAY TO MINUTE"),
								szSign,
								pIS->intval.day_second.day,
								pIS->intval.day_second.hour,
								pIS->intval.day_second.minute);
			break;
		case SQL_CODE_DAY_TO_SECOND:
			if (pIS->intval.day_second.fraction)
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%d %02d:%02d:%02d.%04d' DAY TO SECOND"),
																		szSign,
																		pIS->intval.day_second.day,
																		pIS->intval.day_second.hour,
																		pIS->intval.day_second.minute,
																		pIS->intval.day_second.second,
																		pIS->intval.day_second.fraction);
			}
			else
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%d %02d:%02d:%02d.%04d' DAY TO SECOND"),
																		szSign,
																		pIS->intval.day_second.day,
																		pIS->intval.day_second.hour,
																		pIS->intval.day_second.minute,
																		pIS->intval.day_second.second,
																		pIS->intval.day_second.fraction);

			}
			break;
		case SQL_CODE_HOUR_TO_MINUTE:
			wsprintf(szOut,TEXT("INTERVAL %s '%02d:%02d' HOUR TO MINUTE"),
									szSign,
									pIS->intval.day_second.hour,
									pIS->intval.day_second.minute);
			break;
		case SQL_CODE_HOUR_TO_SECOND:
			if (pIS->intval.day_second.fraction)
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%02d:%02d:%02d.%04d' HOUR TO SECOND"),
																szSign,
																pIS->intval.day_second.hour,
																pIS->intval.day_second.minute,
																pIS->intval.day_second.second,
																pIS->intval.day_second.fraction);
			}
			else
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%02d:%02d:%02d' HOUR TO SECOND"),
																szSign,
																pIS->intval.day_second.hour,
																pIS->intval.day_second.minute,
																pIS->intval.day_second.second);
			}

			break;
		case SQL_CODE_MINUTE_TO_SECOND:
			if (pIS->intval.day_second.fraction)
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%02d:%02d.%d' MINUTE TO SECOND"),
										szSign,
										pIS->intval.day_second.minute,
										pIS->intval.day_second.second,
										pIS->intval.day_second.fraction);
			}
			else
			{
				wsprintf(szOut,TEXT("INTERVAL %s '%02d:%02d' MINUTE TO SECOND"),
										szSign,
										pIS->intval.day_second.minute,
										pIS->intval.day_second.second);
			}
			break;
		default:
			//unknown interval_type
			szPrintf(lpUsrOptions, hwndOut,
						MSG_SEV1,
						TRUE, TRUE,
						GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST0022),
						pIS->interval_type);
	}

}

//*---------------------------------------------------------------------------------
//| FormatValueBasedOnType:
//|	This function takes a pointer to data and formats it into a character
//|	value based on its type.  Output length is enforced.
//|
//|		Note that if the user uses SQL_C_DEFAULT, we cannot know what type
//|		to use.  For example, if the user called SQLBindCol with SQL_C_DEFAULT,
//|		how do we know what type the column is (remember we are not allowed to
//|		call other functions out of turn).  Gator 1.0 tried to use the size of
//|		the data type to figure this out, but this has a termendous number of
//|		problems (like displaying any 4 byte value as an SDWORD, etc...).  For
//|		2.0, user's are not supposed to use SQL_C_DEFAULT, so we'll spit it out
//|		in binary.
//|
//| Parms:
//|	szOut						Where to write string
//|	cbOut						Maximum length we can write
//|	lpData					Pointer to the data
//|	fCType					The C type of the data
//|	cBytes					Count of bytes in lpData
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------
BOOL INTFUN FormatValueBasedOnType(LPTSTR szOut, DWORD cbOut,
			LPVOID lpData, SWORD fCType, UDWORD cBytes,HWND hwndOut)
{
	BOOL		fOverflow=FALSE;
	TCHAR		szStrW[MAXOUTBUFF]=TEXT("");


	// No pointer involved, so simply record generic info
	switch(fCType)
	{

		case SQL_C_WCHAR:

			if (!FormatCharData(szOut,cbOut,(LPTSTR) lpData,fCType,cBytes,ADD_QUOTES))
				goto overflow;
			break;

		case SQL_C_CHAR:
			{

			if (!FormatCharData(szOut,cbOut,lpData,fCType,cBytes,ADD_QUOTES))
				goto overflow;

			}
			break;

		case SQL_C_DOUBLESTR:
		{
			//Bug #330 - I rewrote the code for this case
			LPTSTR		str = (LPTSTR)lpData,
						szEndStr=TEXT("...\\0");
			DWORD		cbChar=sizeof(TCHAR),
						cNullTerm=lstrlen(szNULLTERMSTR),
						cbBytes=0,
						ccStr=0,
						cbEndStr=(lstrlen(szEndStr)+1)*cbChar;
			BOOL		fAppend=FALSE;
			UINT		cbOutStr=0;
			TCHAR		szOutBuff[XLARGEBUFF]=TEXT("");
			DWORD		cbActual=0;
			LPTSTR		pOut=szOut;


			lstrcpy(pOut, szQUOTE);
			pOut++;
			cbOut--;

			if(!*str)
         		lstrcat(pOut, szNULLTERMSTR);

			while(*str)
			{
				ccStr=lstrlen(str);

				//See if str will fit in szOut
				if (ccStr >= cbOut)
				{
					if (cbEndStr < cbOut)
						cbOut-=cbEndStr;

					//if (cbOut >= 0)

						//copy enough to fill the rest of the buffer minus
						//"...\0"
						_tcsnccpy(pOut,str,cbOut);
						pOut+=cbOut;


					//Tack on "....\0"
					_tcsnccpy(pOut,szEndStr,cbEndStr);
					pOut+=cbEndStr-1;

					break;
				}

				if (!FormatCharData(pOut,cbOut,str,fCType,cBytes,DONT_ADD_QUOTES))
					goto overflow;

				pOut+=ccStr;
				cbOut-=ccStr;

				if ((cbOut-cNullTerm) <= 0)
					pOut-=abs(cbOut-cNullTerm);

				//Replace null term with its char. representation
				lstrcat(pOut,szNULLTERMSTR);
				pOut+=cNullTerm;
				cbOut-=cNullTerm;

				//Go to next string
				str+=lstrlen(str)+1;
			}

			lstrcpy(pOut,TEXT("\""));

		}
		break;

		case SQL_C_BIT:
			_stprintf(szOut, TEXT("0x0%x"), *(BYTE *)lpData);
			break;

		case SQL_C_TINYINT:
		case SQL_C_STINYINT:
			{
			STINYINT *  	pstiny;
			pstiny = (STINYINT *)lpData;
			wsprintf(szOut, TEXT("%d"), *pstiny);
			}
			break;

		case SQL_C_UBIGINT:
			{
			DWLONG *pubigint;
			pubigint = (DWLONG *)lpData;

			BigintToChar(pubigint,(LPBYTE)szOut, 0, TRUE);

			}
			break;

		case SQL_C_SBIGINT:
			{
			DWLONG *pubigint;
			pubigint = (DWLONG *)lpData;

			BigintToChar(pubigint,(LPBYTE)szOut, 0, FALSE);

			}
			break;

		case SQL_C_UTINYINT:
			{
			UTINYINT *  	putiny;
			putiny = (UTINYINT *)lpData;
			wsprintf(szOut, TEXT("%u"), *putiny);
			}
			break;

		case SQL_C_SHORT:
		case SQL_C_SSHORT:
			{
			SWORD * 	psword;
			psword = (SWORD *)lpData;
			wsprintf(szOut, TEXT("%d"), *psword);
			}
			break;

		case SQL_C_USHORT:
			{
			UWORD * 	psword;
			psword = (UWORD *)lpData;
			wsprintf(szOut, TEXT("%u"), *psword);
			}
			break;

		case SQL_C_LONG:
		case SQL_C_SLONG:
			{
			SDWORD * 	psdword;

			psdword = (SDWORD *)lpData;
			wsprintf(szOut, TEXT("%ld"), *psdword);
			}
			break;

		case SQL_C_ULONG:
			{
			UDWORD * 	psdword;
			psdword = (UDWORD *)lpData;
			wsprintf(szOut, TEXT("%lu"), *psdword);
			}
			break;

		case SQL_C_FLOAT:
			{
			SFLOAT *		psfloat;
			psfloat = (SFLOAT *)lpData;
			//_stprintf(szOut,TEXT("%f"),*psfloat);
			DoubleToChar(szOut,*psfloat,20);

			//Bug #282 replaced sprintf() with _ecvt()
			//wsprintf(szOut, TEXT("%s"), _ecvt(*psfloat,20, &dec,&sign));
			}
			break;

		case SQL_C_GUID:
			{
			UUID *		Uuid;
			TCHAR * StringUid = NULL;


			Uuid = (UUID *)lpData;
			UuidToString(Uuid,&StringUid);
			wsprintf(szOut, TEXT("%s"), StringUid);


			}
		break;

		case SQL_C_DOUBLE:
			{
			SDOUBLE *		psdouble;
			psdouble = (SDOUBLE *)lpData;
			//_stprintf(szOut,TEXT("%f"),*psdouble);
			DoubleToChar(szOut,*psdouble,20);

			//Bug #282 replaced sprintf() with _ecvt()
			//wsprintf(szOut, TEXT("%s"), _ecvt(*psdouble,20, &dec,&sign));
			psdouble=0;

			}
			break;

		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
			{
			DATE_STRUCT * ds;
			ds = (DATE_STRUCT *)lpData;
			if(ds)
				wsprintf(szOut, szFMTDATE, ds->year, ds->month, ds->day);
			else
				lstrcpy(szOut, szNULL);
			}
			break;

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
			{
			TIME_STRUCT * ts;
			ts = (TIME_STRUCT *)lpData;
			if(ts)
				wsprintf(szOut, szFMTTIME, ts->hour, ts->minute, ts->second);
			else
				lstrcpy(szOut, szNULL);
			}
			break;

		case SQL_C_TIMESTAMP:
		case SQL_C_TYPE_TIMESTAMP:
			{
			TIMESTAMP_STRUCT * tsp;
			TCHAR	sztmp[15]=TEXT("");
			tsp = (TIMESTAMP_STRUCT *)lpData;
			if(tsp)
			{
				wsprintf(szOut, szFMTTIMESTAMP,
						tsp->year, tsp->month, tsp->day, tsp->hour, tsp->minute, tsp->second);

				if(tsp->fraction <= MAX_TIMESTAMP_FRAC_NUM)
				{
					// If fraction is displayable (ie. < 999,999,999
					// then format it correctly and concat to the ts string
					wsprintf(sztmp, szFMTTIMESTAMPFRAC, tsp->fraction);
					lstrcat(szOut, sztmp);
				}
			}
			else
				lstrcpy(szOut, szNULL);
			}
			break;

		case SQL_C_NUMERIC:
		{
			SQL_NUMERIC_STRUCT *lpns=NULL;

			lpns = (SQL_NUMERIC_STRUCT *)lpData;
			numerictostring(lpns, sizeof(SQL_NUMERIC_STRUCT), szOut,cbOut);
		}
		break;

		case SQL_C_INTERVAL_YEAR:
		case SQL_C_INTERVAL_YEAR_TO_MONTH:
		case SQL_C_INTERVAL_MONTH:
		case SQL_C_INTERVAL_DAY:
		case SQL_C_INTERVAL_DAY_TO_HOUR:
		case SQL_C_INTERVAL_DAY_TO_MINUTE:
		case SQL_C_INTERVAL_DAY_TO_SECOND:
		case SQL_C_INTERVAL_HOUR:
		case SQL_C_INTERVAL_HOUR_TO_MINUTE:
		case SQL_C_INTERVAL_HOUR_TO_SECOND:
		case SQL_C_INTERVAL_MINUTE:
		case SQL_C_INTERVAL_MINUTE_TO_SECOND:
		case SQL_C_INTERVAL_SECOND:
		{
			if (lpData)
				FormatIntervalData(fCType,(SQL_INTERVAL_STRUCT *)lpData,
										cBytes,szOut,cbOut,hwndOut);
			else
				lstrcpy(szOut, szNULL);
		}
		break;

		case SQL_C_DEFAULT:
		case SQL_C_BINARY:
		default:
			{
			LPSTR			str;
			UINT			iElipse;
			BYTE			*outstr;
			BYTE			*curin = (BYTE *)lpData;
		   BYTE	 		uletter;
		   UDWORD		cCount;

			//Bug #326, Bug #324
			UINT			iOut=0,
							iIn=0;
			DWORD			cBinBytes=0; //# of bytes in binary string
			//Bug #342
			DWORD			cbMaxBinary=0;
			DWORD			cbChar=sizeof(TCHAR);
			LPTSTR		szStrW=NULL;

			// Now format the data as if it were SQL_C_BINARY (since we need to format data
			// in some manner for user-defined C Types -- SQLBindParameter for example)
			if(cBytes == 0)
				return TRUE;			// The buffer is of size zero, so we don't have anything to write to
			else
				{
				//Bug #276 - Removed "- 1"
				//Bug #478 in the case of a binary field of say 5000 bytes,
				//cbMaxBinary would be 10,000, clearly we don't want
				//to display more than 512 bytes.
				cCount = (cBytes > cbOut ? cbOut : cBytes);

				//Bug #342
				//cbMaxBinary contains the count of bytes for
				//a BINARY strings (i.e. 0x424445)
				cbMaxBinary=((cCount * (2 * cbChar)) + (3 * cbChar));
				}

			// Attempt to allocate the memory
			outstr = (BYTE *)AllocateMemory(cbMaxBinary);
			szStrW = (LPTSTR)AllocateMemory(cbMaxBinary);

			if(!outstr) {
				szMessageBox(GetActiveWindow(),
						MB_ICONINFORMATION | MB_OK,
						NULL,
						GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
						((cBytes / 2) + 1), szBinaryConv);
				return FALSE;
				}
			strcpy(outstr,"0x");
			str = outstr;
			iOut += (UINT)strlen(outstr);

			// Convert binary string into a character string for display
			//Bug #326, Bug #324, Added check for valid pointers
			// (i.e. 'if (curin)' and 'if (outstr)' and added iIn and iOut
			//to index into outstr and curin because incrementing the pointer
			//changed the original pointer, therefore when FREE_PTR()
			//was called on outstr, outstr was not pointing to the original
			//allocated address.


			while (cCount-- && curin)
				{
				if (curin)
					{

					uletter = (curin[iIn] & 0xF0) >>4; //high nibble
					if (uletter <= 9)
						{
						if (outstr)
							outstr[iOut] = uletter + TEXT('0');
						}
					else
						{
						if (outstr)
							outstr[iOut] = TEXT('A') + (uletter - 10);
						}

					iOut++;
					}

				if (curin)
					{
					uletter = curin[iIn++] & 0x0F;  //low nibble

					if (uletter <= 9)
						{
						if (outstr)
							outstr[iOut] = uletter + TEXT('0');
						}
					else
						{
						if (outstr)
							outstr[iOut] = TEXT('A') + (uletter - 10);
						}

					iOut++;
					}
				}

			outstr[iOut] = '\0'; //null terminate

			if(str)
			{
				//Bug #342
				if (cbMaxBinary > cbOut)
				{
					fOverflow = TRUE;
					iElipse = lstrlen(szELIPSE) + 1;				// Count NULL term and "..."
					if(cbOut < iElipse)
					{								// Not enough room for trunc warning, quotes and ...
						FREE_PTR(outstr);
						goto overflow;
					}
				}

#ifdef UNICODE
				ConvertToUnicode(str, szStrW,(WORD)cbMaxBinary);
#else
				lstrcpy(szStrW,str);
#endif

				if(!fOverflow)
					lstrcpy(szOut, szStrW);
				else
				{
					// String will be truncated, so copy only what will fit along with ...
					_tcsncpy(szOut, szStrW, (size_t)(cbOut - iElipse));
					*(LPTSTR)(szOut + (cbOut - iElipse)) = TEXT('\0');
					lstrcat(szOut, szELIPSE);
				}
			}

			FREE_PTR(outstr);
			FREE_PTR(szStrW);

		}
			break;
	}

	return TRUE;

overflow:
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| IsHandleParam:
//|	This function determines if the parameter specfied by uParmType is a "Handle"
//|	that needs to be formatted as a handle. For example: an HSTMT, or the
//|	Str&Len_or_IndPtr for SQLGetData which is only an OUTPUT paramter.
//|
//| Parms:
//|	uParmType	Parameter type
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------
BOOL INTFUN IsHandleParam(UDWORD uParmType)
{
	if (uParmType & PT_HENV			||
		 uParmType & PT_HDBC			||
		 uParmType & PT_HSTMT		||
		 uParmType & PT_HWND			||
		 uParmType & PT_HDESC		||
		 uParmType & PT_HHANDLE		||
		 uParmType & PT_LOG_DEFFERED ||
		 uParmType & PT_MULT_HDESC)
	{
		return(TRUE);
	}

	return(FALSE);

} //IsHandleParam()

//*---------------------------------------------------------------------------------
//| IsPtrPtrParam:
//|	This function determines if the parameter specfied by uParmType is a pointer
//|	to an address that needs to be dereferenced and formatted as a handle. For
//|	example: SQL_DESC_ARRAY_STATUS_PTR
//|
//| Parms:
//|	uParmType	Parameter type
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------
BOOL INTFUN IsPtrPtrParam(UDWORD uParmType)
{
	if (uParmType & PT_PTR_HENV	||
		 uParmType & PT_PTR_HDBC	||
		 uParmType & PT_PTR_HSTMT	||
		 uParmType & PT_MULT_HDESC)
	{
		return(TRUE);
	}

	return(FALSE);

} //IsPtrPtrParam()


//*---------------------------------------------------------------------------------
//| IsPtrToValue:
//|	This function determines if the parameter specfied by uParmType is a pointer
//|	to an address that needs to be dereferenced and formatted as a handle. For
//|	example: SQL_DESC_ARRAY_STATUS_PTR
//|
//| Parms:
//|	uParmType	Parameter type
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------
BOOL INTFUN IsPtrToValue(UDWORD *puParmType)
{
	BOOL fPtrToValue=FALSE;

	// If this parameter is a pointer to a value then
	//we need to reset the parameter type since the
	//next time through it could be a different
	//parameter type
	if (*puParmType & PT_PTR_HENV)
	{
		*puParmType &= ~PT_PTR_HENV;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_HDBC)
	{
		*puParmType &= ~PT_PTR_HDBC;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_HSTMT)
	{
		*puParmType &= ~PT_PTR_HSTMT;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_STR)
	{
		*puParmType &= ~PT_PTR_STR;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_SWORD)
	{
		*puParmType &= ~PT_PTR_SWORD;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_UWORD)
	{
		*puParmType &= ~PT_PTR_UWORD;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_SDWORD)
	{
		*puParmType &= ~PT_PTR_SDWORD;
		fPtrToValue=TRUE;
	}
	else if (*puParmType & PT_PTR_UDWORD)
	{
		*puParmType &= ~PT_PTR_UDWORD;
		fPtrToValue=TRUE;
	}

	return(fPtrToValue);

} //IsPtrToValue()

//*---------------------------------------------------------------------------------
//| GetCharValueFromParm:
//|	This function will take a parmater pointer and format the value into
//|	a character string.
//| Parms:
//|	szOut						Where to write string
//|	cbOut						Maximum length we can write
//|	lpParm					Parameter pointer
//|	fIn						TRUE if values should treated as input (eg:
//|									NULL, VALID vs. actual value for pointer)
//|	phenv						Pointer to environment handle
//|	phdbc						Pointer to connection handle
//|	phstmt					Pointer to statement handle
//|	uStmtNum					Hstmt number
//| Returns:
//|	TRUE if successful, FALSE on overflow
//*---------------------------------------------------------------------------------
BOOL INTFUN GetCharValueFromParm(LPTSTR szOut, DWORD cbOut, lpPARAMITEM lpParm,
			BOOL fIn,
			HENV * phenv, HDBC * phdbc,
			HSTMT * phstmt, SQLHDESC *phdesc,
			UINT uStmtNum,
			HWND hwndOut)
{

	// For input parameters, format handles.  Note that if we're looking for output
	//		parms, we use the value (example:  pointer to a string is logged as VALID
	//		or NULL for fIn==TRUE, but it's value is used when fIn==FALSE)
	if(fIn || lpParm->fNull)
	{
		if(lpParm->uParmType & PT_CHK)
		{
			// It's a True-False Checkbox
			if(lpParm->fNull)
				lstrcpy(szOut, TEXT("TRUE"));
			else
				lstrcpy(szOut, TEXT("FALSE"));
			return TRUE;
		}
		else if (lpParm->fNull ||
					(lpParm->uParmType & PT_LOG_OUT) ||
					IsHandleParam(lpParm->uParmType) ||
					lpParm->fHandle == PRM_32HDL)
		{
			//this is either a pointer or it's strictly an output parm
			//in which case we need to display it as a handle because it's
			//contents aren't defined until after the ODBC call
			return FormatHandle(szOut, lpParm->fNull, lpParm->uParmType,
					NULL, phenv, phdbc, phstmt, phdesc,uStmtNum,
					lpParm->lpData);
		}
	}

	if (lpParm->lpData && (lpParm->fHandle ||
		(lpParm->fHandle == PT_PTR_PTR)))
	{
		SQLHANDLE hHandle=*(SQLHANDLE *)lpParm->lpData;

		return HandleToText(szOut,hHandle) != NULL;
	}
	else
	{
		//Just format parameter based on the parameter's fCType
		return FormatValueBasedOnType(szOut, cbOut, lpParm->lpData, lpParm->fCType, lpParm->cBytes,hwndOut);
	}

}

//*---------------------------------------------------------------------------------
//| TruncationCheck:
//|	This function checks for truncation of a field.  All errors and/or
//|	warnings are issued in the current output window.
//| Parms:
//|	rc							Return code
//|	hwndOut					Output window target
//|	szBuff					The TCHARacter buffer to look at
//|	cbMaxBuff				The maximum size which can be written
//|	cbBuff					The returned length from driver
//|	szFieldName				The name of the field which indicates truncation (pcb...)
//| Returns:
//|	The error message number if issued, 0 on no error
//*---------------------------------------------------------------------------------
UINT INTFUN TruncationCheck(RETCODE rc, HWND hwndOut, LPTSTR szBuff,
					SDWORD cbMaxBuff, SDWORD cbBuff, LPTSTR szFieldName)
{
	UINT		uErrNum=0;
	TCHAR		szField[40];

	// Don't check truncation unless it was successful
	if(rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO && rc != SQL_NEED_DATA)
		return 0;

	if(cbBuff >= cbMaxBuff) {						// Truncation occured
		TCHAR		szMsg[100];
		lstrcpy(szField, szFieldName);
		RemoveDlgFormat(szField);
		// If trunction occurred and SQL_SUCCESS_WITH_INFO was issued, then this is only
		//		a warning.  If another return code was issued, it is an error.
		if(rc == SQL_SUCCESS_WITH_INFO)
			uErrNum = TST1002;
		else if(rc == SQL_SUCCESS)
			uErrNum = TST0005;
		else	// SQL_BROWSE_CONNECT can cause truncation on some functions
			uErrNum = TST1016;

		szPrintf(lpUsrOptions, hwndOut,
					(uErrNum==TST0005) ? MSG_SEV0 : MSG_SEV1,
					TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), uErrNum),
					szField,
					cbBuff);
		}
	return uErrNum;
}

//*---------------------------------------------------------------------------------
//| EnumerateErrors30:
//|	This function will enumerate all errors on the given handles displaying
//|	each to the output window with the given header.
//| Parms:
//|	szType					Header name, eg:  "dbc:  "
//|	hwndOut					Output window for errors
//|	HandleType				SQL_HANDLE_ENV,SQL_HANDLE_DBC,SQL_HANDLE_STMT, or
//|								SQL_HANDLE_DESC
//|	hHandle					Handle
//| Returns:
//|	TRUE	if Errors were found otherwise FALSE
//*---------------------------------------------------------------------------------
BOOL INTFUN EnumerateErrors30(LPTSTR szType, HWND hwndOut, HWND hwndOutWin,
					SQLSMALLINT HandleType,SQLHANDLE hHandle)
{
	RETCODE				rc=SQL_SUCCESS;
	SDWORD				fNativeError;
	SWORD					cbErrorMsg;
	TCHAR					szSqlState[12];
	TCHAR					szMsg[LARGEBUFF];
	TCHAR					szErrMsg[SQL_MAX_MESSAGE_LENGTH];
	SQLSMALLINT			RecNumber=1;
	BOOL					fErrorsFound=FALSE;
	SDWORD					ColNum= SQL_NO_COLUMN_NUMBER,
							RowNum=	SQL_NO_ROW_NUMBER;

	// Look for all errors on each ODBC handle
	rc = SQLGetDiagRec(HandleType,hHandle,RecNumber,szSqlState,&fNativeError,
							szErrMsg,tsizeof(szErrMsg),&cbErrorMsg);

	while(RC_SUCCESSFUL(rc))
	{
		fErrorsFound=TRUE;

		rc = SQLGetDiagField(HandleType, hHandle, RecNumber,SQL_DIAG_COLUMN_NUMBER,
							(PTR)&ColNum,sizeof(ColNum),NULL);
		rc = SQLGetDiagField(HandleType, hHandle, RecNumber,SQL_DIAG_ROW_NUMBER,
							(PTR)&RowNum,sizeof(RowNum),NULL);

		// Print error message
		if (hwndOutWin)
		{
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, TRUE,
						GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), idsSQLErrorMessage30),
						szType, szSqlState, fNativeError, cbErrorMsg, ColNum, RowNum);
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, FALSE, TEXT("\t\t\t\t\t\tMessageText = \""));
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, FALSE, szErrMsg);
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, FALSE, TEXT("\"\r\n"));
		}

		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), idsSQLErrorMessage30),
					szType, szSqlState, fNativeError, cbErrorMsg,ColNum, RowNum);
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, TEXT("\t\t\t\t\t\tMessageText = \""));
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, szErrMsg);
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, TEXT("\"\r\n"));

		// If truncation occurs, tell user of the error
		if(hwndOutWin &&
			TruncationCheck(rc,hwndOutWin, szErrMsg, tsizeof(szErrMsg), cbErrorMsg, TEXT("*TextLengthPtr")))
		{
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1003),
					szSqlState);
		}

		if (TruncationCheck(rc,hwndOut, szErrMsg, tsizeof(szErrMsg), cbErrorMsg, TEXT("*TextLengthPtr")))
		{
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1003),
					szSqlState);
		}

		//Get Next Record
		RecNumber++;

		rc = SQLGetDiagRec(HandleType,hHandle,RecNumber,szSqlState,&fNativeError,
								szErrMsg,tsizeof(szErrMsg),&cbErrorMsg);
	}

	return(fErrorsFound);

} //EnumerateErrors30()



//*---------------------------------------------------------------------------------
//| EnumerateErrors:
//|	This function will enermate all errors on the given handles displaying
//|	each to the output window with the given header.
//| Parms:
//|	szType					Header name, eg:  "dbc:  "
//|	hwndOut					Output window for errors
//|	henv						ODBC Handles
//|	hdbc
//|	hstmt
//| Returns:
//|	TRUE	if Errors were found otherwise FALSE
//*---------------------------------------------------------------------------------
BOOL INTFUN EnumerateErrors(LPTSTR szType, HWND hwndOut,  HWND hwndOutWin,
					HSTMT henv, HDBC hdbc, HSTMT hstmt)
{
	RETCODE				rc=SQL_SUCCESS;
	SDWORD				fNativeError;
	SWORD					cbErrorMsg;
	TCHAR					szSqlState[6];
	TCHAR					szMsg[100];
	TCHAR					szErrMsg[SQL_MAX_MESSAGE_LENGTH];
	BOOL					fErrorsFound=FALSE;

	// Look for all errors on each ODBC handle
	rc = SQLError(henv, hdbc, hstmt, szSqlState,
					&fNativeError, szErrMsg,
                    sizeof(szErrMsg)/sizeof(szErrMsg[0]), &cbErrorMsg);

	while(RC_SUCCESSFUL(rc))
	{
		fErrorsFound=TRUE;

		// Print error message
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), idsSQLErrorMessage),
					szType, szSqlState, fNativeError, cbErrorMsg);

		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, TEXT("\t\t\t\t\t\tszErrorMsg = \""));
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, szErrMsg);
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, FALSE, TEXT("\"\r\n"));

		if (hwndOutWin)
		{
			// Print error message
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, TRUE,
						GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), idsSQLErrorMessage),
						szType, szSqlState, fNativeError, cbErrorMsg);
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, FALSE, TEXT("\t\t\t\t\t\tszErrorMsg = \""));
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, FALSE, szErrMsg);
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, FALSE, TEXT("\"\r\n"));
		}

		// If truncation occurs, tell user of the error
		if(TruncationCheck(rc, hwndOut, szErrMsg,
                           sizeof(szErrMsg)/sizeof(szErrMsg[0]), cbErrorMsg, TEXT("*pcbErrorMsg")))
		{
			szPrintf(lpUsrOptions, hwndOut, MSG_SEV0, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1003),
					szSqlState);
		}

		// If truncation occurs, tell user of the error
		if(hwndOutWin &&
			TruncationCheck(rc, hwndOutWin, szErrMsg,
                            sizeof(szErrMsg)/sizeof(szErrMsg[0]), cbErrorMsg, TEXT("*pcbErrorMsg")))
		{
			szPrintf(lpUsrOptions, hwndOutWin, MSG_SEV0, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1003),
					szSqlState);
		}

		rc = SQLError(henv, hdbc, hstmt, szSqlState,
						&fNativeError, szErrMsg,
                        sizeof(szErrMsg)/sizeof(szErrMsg[0]), &cbErrorMsg);
	}

	return(fErrorsFound);

} //EnumerateErrors()


//*---------------------------------------------------------------------------------
//| ToolOptionsAllErrors:
//|	This function will check the user options to see if all errors should be
//|	looked for on each handle.
//| Parms:
//|	ei							Used to direct output
//|	ci							Used to direct output
//|	henv						Environment handle
//|	hdbc						Connection handle
//|	hstmt						Statement handle
//|	hdesc						Descriptor handle
//|	rc							return code of calling API
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN ToolOptionsAllErrors(lpCONNECTIONINFO ci, HENV henv, HDBC hdbc, HSTMT hstmt,
											SQLHDESC hdesc,RETCODE rc)
{
	HWND						hwndOut =hwndOutWin;
	BOOL						fODBC3XEnv=lpUsrOptions->fODBC30;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	SWORD						cDescs=0;
	BOOL						fErrorsFound=0;
	SQLHANDLE				*phHandle=NULL;
	TCHAR						szStr[SMALLBUFF]=TEXT("");
	UWORD						fHandleType=0;
	lpENVIRONMENTINFO		lpEnv=GetActiveEnv();



	switch(rc)
	{
		case SQL_INVALID_HANDLE:
		case SQL_NO_DATA_FOUND:
		case SQL_SUCCESS:
			return(fErrorsFound);
	}

	if (ci)
	{
		hwndOut=ci->hwndOut;
		lpDesc=ci->lpDesc;
		cDescs=ci->cDescs;

	}

	// Do all environment handle errors
	if (lpEnv && lpEnv->henvCurr)
	{
		fHandleType=SQL_HANDLE_ENV;
		phHandle=&lpEnv->henvCurr;
		lstrcpy(szStr,TEXT("env"));
	}
	else if (ci && ci->hdbcCurr)
	{
		fHandleType=SQL_HANDLE_DBC;
		phHandle=&ci->hdbcCurr;
		lstrcpy(szStr,TEXT("dbc"));
	}
	else if (ci && ci->hstmtCurr)
	{
		fHandleType=SQL_HANDLE_STMT;
		phHandle=&ci->hstmtCurr;
		lstrcpy(szStr,TEXT("stmt"));
	}
	else if (ci && ci->hdescCurr)
	{
		fHandleType=SQL_HANDLE_DESC;
		phHandle=&ci->hdescCurr;
		lstrcpy(szStr,TEXT("hdesc"));
	}

	if ((fODBC3XEnv && phHandle) || fHandleType==SQL_HANDLE_DESC)
		fErrorsFound+=EnumerateErrors30(szStr, hwndOut, NULL,fHandleType,*phHandle);
	else
		fErrorsFound+=EnumerateErrors(szStr, hwndOut, NULL,henv,hdbc,hstmt);

	if (phHandle)
		*phHandle=SQL_NULL_HANDLE;

	return(fErrorsFound);

}

//*---------------------------------------------------------------------------------
//| LogErrorsToOutputWindow:
//|	This function will check the user options to see if all errors should be
//|	looked for on each handle and will output errors messages to the Output
//|	window as well as the connection window
//| Parms:
//|	ei							Used to direct output
//|	ci							Used to direct output
//|	henv						Environment handle
//|	hdbc						Connection handle
//|	hstmt						Statement handle
//|	hdesc						Descriptor handle
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN LogErrorsToOutputWindow(lpCONNECTIONINFO ci, HENV henv, HDBC hdbc, HSTMT hstmt,
											SQLHDESC hdesc)
{
	HWND						hwndOut =hwndOutWin;
	BOOL						fODBC3XEnv=lpUsrOptions->fODBC30;
	lpDESCRIPTORHEADER	lpDesc=NULL;
	SWORD						cDescs=0;
	BOOL						fErrorsFound=0;

	if (ci)
	{
		hwndOut=ci->hwndOut;
		lpDesc=ci->lpDesc;
		cDescs=ci->cDescs;
	}

	// Do all environment handle errors
	if(henv)
	{
		if (fODBC3XEnv)
			fErrorsFound+=EnumerateErrors30(TEXT("env"), hwndOut,
						hwndOutWin,SQL_HANDLE_ENV, henv);
		else
			fErrorsFound+=EnumerateErrors(TEXT("env"), hwndOut,
						hwndOutWin,henv, NULL, NULL);
	}

	// Do all connection handle erros
	if(hdbc)
	{
		if (fODBC3XEnv)
			fErrorsFound+=EnumerateErrors30(TEXT("dbc"), hwndOut,
						hwndOutWin,SQL_HANDLE_DBC, hdbc);
		else
			fErrorsFound+=EnumerateErrors(TEXT("dbc"), hwndOut,
						hwndOutWin,NULL, hdbc, NULL);
	}

	// Do all statement handle errors
	if(hstmt)
	{
		if (fODBC3XEnv)
			fErrorsFound+=EnumerateErrors30(TEXT("stmt"), hwndOut,
						hwndOutWin,SQL_HANDLE_STMT, hstmt);
		else
			fErrorsFound+=EnumerateErrors(TEXT("stmt"), hwndOut,
						hwndOutWin,NULL, NULL, hstmt);
	}

	if (hdesc)
	{
		fErrorsFound+=EnumerateErrors30(TEXT("desc"),
									hwndOut, hwndOutWin, SQL_HANDLE_DESC,hdesc);
	}

	return(fErrorsFound);
}


//*---------------------------------------------------------------------------------
//| LogReturnCode:
//|	Log the return code to either the output window or a message box.
//| Parms:
//|	szFuncName				The function name
//|	ci							Connection information
//|	rc							The return code to log
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN LogReturnCode(LPTSTR szFuncName, lpCONNECTIONINFO ci, RETCODE rc)
{
	static TCHAR			szTmp[100];
	static TCHAR			szRtn[35];
	HWND					hwndOut;                   // For conn win out or generic output

	hwndOut = GETOUTPUTWINDOW(ci);

	if(!szFuncName)
		szPrintf(lpUsrOptions, hwndOut,
				MSG_SEV0, TRUE, TRUE,
				GetGtrString(szTmp, sizeof(szTmp), idsReturnCodeWas),
				GetRetCodeName(rc, szRtn));
	else
		szPrintf(lpUsrOptions,hwndOut,
				MSG_SEV0, TRUE, TRUE,
				GetGtrString(szTmp, sizeof(szTmp), idsFuncReturnCodeWas),
				szFuncName,
				GetRetCodeName(rc, szRtn));

} //LogReturnCode()

//*---------------------------------------------------------------------------------
//| LogInstallerReturnCode:
//|	Log the return code of Installer API's to either the output window or a message box. This is
//| used for those Installer API's that return BOOL's
//| Parms:
//|	szFuncName				The function name
//|	ci							Connection information
//|	rc							The return code to log
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN LogInstallerReturnCode(LPTSTR szFuncName, lpCONNECTIONINFO ci, BOOL fRc)
{
	static TCHAR			szTmp[100];
	static TCHAR			szRtn[35];
	HWND					hwndOut;                   // For conn win out or generic output

	hwndOut = GETOUTPUTWINDOW(ci);

	if(!szFuncName)
		szPrintf(lpUsrOptions, hwndOut,
				MSG_SEV0, TRUE, TRUE,
				GetGtrString(szTmp, sizeof(szTmp), idsReturnCodeWas),
				GetInstallerRetCodeName(fRc, szRtn));
	else
		szPrintf(lpUsrOptions,hwndOut,
				MSG_SEV0, TRUE, TRUE,
				GetGtrString(szTmp, sizeof(szTmp), idsFuncReturnCodeWas),
				szFuncName,
				GetInstallerRetCodeName(fRc, szRtn));
}


//*---------------------------------------------------------------------------------
//| LogInstallerReturnCode30:
//|	Log the return code of Installer API's to either the output window or a message box. This is
//| used for those Installer API's that return RETCODE's
//| Parms:
//|	szFuncName				The function name
//|	ci							Connection information
//|	rc							The return code to log
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN LogInstallerReturnCode30(LPTSTR szFuncName, lpCONNECTIONINFO ci, RETCODE rc)
{
	static TCHAR			szTmp[100];
	static TCHAR			szRtn[35];
	HWND					hwndOut;                   // For conn win out or generic output

	hwndOut = GETOUTPUTWINDOW(ci);

	if(!szFuncName)
		szPrintf(lpUsrOptions, hwndOut,
				MSG_SEV0, TRUE, TRUE,
				GetGtrString(szTmp, sizeof(szTmp), idsReturnCodeWas),
				GetInstallerRetCodeName30(rc, szRtn));
	else
		szPrintf(lpUsrOptions,hwndOut,
				MSG_SEV0, TRUE, TRUE,
				GetGtrString(szTmp, sizeof(szTmp), idsFuncReturnCodeWas),
				szFuncName,
				GetInstallerRetCodeName30(rc, szRtn));
}



//*---------------------------------------------------------------------------------
//| BuildOutputParm:
//|	This function will allocate memory for a function, including, if required,
//|	extra memory for buffer checking. This function should always be called
//|	via the BUILDOUTPUTPARM macro which will substitute a valid buffer length
//|	for any negative length values.
//| Parms:
//|	lpParm					The parameter to build
//|	cbMaxOut					The amount required
//|	fBuffCheck				TRUE if we should allocate an extra byte and
//|										set the buffer to UNTOUCHED
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN BuildOutputParm(lpPARAMITEM lpParm, UDWORD cbMaxOut, BOOL fBuffCheck)
{
	// First free old memory
	if(lpParm->lpData)
	{
		FREE_PTR(lpParm->lpData);
		lpParm->lpData = NULL;
		lpParm->cBytes = 0;
	}

	//	If a NULL pointer is desired, then simply return since memory has been freed
	if(lpParm->fNull)
		return TRUE;

	// Look for stupid parms which can happen when user dereferences a 2byte
	//	integer as a 4 byte value
#ifdef DOASSERTWARNINGS
	ASSERTCHECK((cbMaxOut + 1 > 65535), TEXT("High memory alloc, did you dereference a pointer incorrectly?"));
#endif
   lpParm->lpData = AllocOutputParm(cbMaxOut, lpParm->szName,
   					&lpParm->cBytes, fBuffCheck);

	return (lpParm->lpData != NULL);
}


//*---------------------------------------------------------------------------------
//| AllocOutputParm:
//|	This functionality was broken out to allow other apps which needed
//|	buffers to work without a parm array to do so.
//| Parms:
//|	cbMaxOut					The amount required
//|	szName					Name of param we're allocating
//|	pcBtyes					For returning the actual mem allocated
//|	fBuffCheck				TRUE if we should allocate an extra byte and
//|										set the buffer to UNTOUCHED
//| Returns:
//|	Pointer to memory, NULL on error
//*---------------------------------------------------------------------------------
LPVOID AllocOutputParm(UDWORD cbMaxOut, LPCTSTR szName,
			UDWORD * pcBytes, BOOL fBuffCheck)
{
	LPVOID			ptr=NULL;
	TCHAR				szFieldName[MAXEXPANDEDCONSTANT];


// Under 16-bit, no drivers handle _huge pointers and therefore we don't even
// waste our time trying to use them.  Disallow here.
#ifndef WIN32
	if((cbMaxOut + 1) > (UINT)(-1)) {
		szMessageBox(GETOUTPUTWINDOW(lpActiveConnWin),
				MB_ICONINFORMATION | MB_OK,
				NULL,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsHugePointersNotAllowed));
		return NULL;
		}
#endif

	// Attempt to allocate the memory
	ptr = AllocateMemory(cbMaxOut + 1);
	if(!ptr) {
		lstrcpy(szFieldName, szName);
		RemoveDlgFormat(szFieldName);
		szMessageBox(GETOUTPUTWINDOW(lpActiveConnWin),
				MB_ICONINFORMATION | MB_OK,
				NULL,
				GetGtrString(szOutBuff, MAXOUTBUFF, idsFunctionCannotProceed),
				cbMaxOut, szFieldName);
		return NULL;
		}
	else
		*pcBytes = cbMaxOut;

	// Now check the buffer stuff
	if(fBuffCheck) {
		InitUnModified(ptr, cbMaxOut + 1);
		}
	else
		memset(ptr, 0, (size_t)cbMaxOut);

	return ptr;
}
////////////////////////////////////////////////////////////////////////////////////////
// DoubleToChar :
// Routine to handle conversion from double to char.
////////////////////////////////////////////////////////////////////////////////////////

BOOL INTFUN  DoubleToChar(LPTSTR szOut,SDOUBLE psdouble, int iprec)
{
	char * p;
	int dec,sign;
	BOOL fExpNeeded=TRUE;

	p=_ecvt(psdouble,iprec,&dec,&sign);

	if(sign)
		*szOut++ = '-';
	if(abs(dec) < iprec)
	{
		if(dec < 0)
		{
			SQLCHAR *pchT;
			//check for number between 0.000000001 to 0.9999999999
			//check for significant zeros
			for(sign=0,pchT=p+iprec-1;*pchT == '0';sign++,pchT--);

			if(sign >= (-dec) )
			{
				//we do not need E
				 for(sign=dec,*szOut++='0',*szOut++='.';sign;sign++)
					 *szOut++='0';
				 for(;*p;)
					 *szOut++=*p++;
				 fExpNeeded=FALSE;
			}

		}
		else
		{
			if(dec==0)
				*szOut++='0';
			for(sign=dec; sign ;sign--)
			{
				if(*p)
					*szOut++=*p++;
				else
					*szOut++='0';
			}
			*szOut++='.';
			for(;*p;)
				*szOut++=*p++;
			fExpNeeded = FALSE;
		}
	}

	if(fExpNeeded)
	{
		*szOut++=*p++;
		*szOut++='.';
		for(;*p;)
			*szOut++=*p++;
	}
	//remove trailing zeros
	for(;*(szOut-1)=='0';*szOut--);

	if(*(szOut-1)=='.')
		*szOut++='0';
	*szOut='\0';

	if(fExpNeeded)
		wsprintf(szOut,TEXT("E%c%d"),(sign<0?TEXT('-'):TEXT('+')),abs(dec-1));


	return FALSE;
}

//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	DLGBLDR.C
//|
//| Purpose:
//|		This module loads the dialog Parameters (dlgparms.c) and dialog templates
//| (dlgtmplt.c) in the their appropriate structures.
//*---------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include "fhheader.h"

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;

// Following array tracks dialogs for every function
lpDIALOGDESC	Dialogs[NUM_ODBC_APIS];


//*---------------------------------------------------------------------------------
//| DestroyDialogs:
//|	This function will free all allocated memory for all dialogs, as well as
//|	any memory allocated for parameter values.
//| Parms:
//|	index							-1 for all to be freed, otherwise specific index
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN DestroyDialogs(UINT index)
{
	UINT 				dex, pdex;
	UINT				begdex=0;
	UINT				enddex = GetNumItemsInArray(rgApiList);
	LPVOID			lpData;

	if(index != -1)
	{
		begdex = index;
		enddex = index + 1;
	}

	for(dex=begdex;  dex<enddex;  dex++)
	{
		if(Dialogs[dex])
		{
			FREE_PTR(Dialogs[dex]->lpCtl);

			// Release each allocated parameter, followed by the arrays
			for(pdex=0;  pdex<Dialogs[dex]->cParms;  pdex++)
				if(lpData = Dialogs[dex]->lpParms[pdex]->lpData)
					FREE_PTR(lpData);
			FREE_PTR(Dialogs[dex]->lpParms);

			// Now for the dialog node
			FREE_PTR(Dialogs[dex]);
		}
	}
}


//*---------------------------------------------------------------------------------
//| DialogBuilder:
//|	This component will create a dialog template in memory based on the
//|	description of an ODBC function.
//| Parms:
//|	dex						Index to the function to call
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
lpDIALOGDESC DialogBuilder(int dex)
{
	lpDIALOGDESC	lpDlg;
	static BOOL		fGetVer;

	// No need to build it if we already have it
	if (!(lpDlg = Dialogs[dex]))
	{
		// Need memory for the dialog descriptor
		if (!(lpDlg = Dialogs[dex] = AllocateMemory(sizeof(DIALOGDESC))))
			return NULL;
	}

	// Get function name and tell user what we're doing
	if(!lpDlg->szFuncName)
		lpDlg->szFuncName = GetApiName(dex, lpDlg->szFuncName);

	if ((lpDlg->lpCtl = GetControlInfo(dex, &lpDlg->cCtlItems)) 	&&	// Control array
		(lpDlg->lpParms = GetParmInfo(dex, &lpDlg->cParms))) 			// Parameter array
	{
		SetParamPtrs(lpDlg);
		return(lpDlg);
	}

	FREE_PTR(lpDlg);

	return NULL;
}



//*---------------------------------------------------------------------------------
//| SetParamPtrs:
//|	This function  will set the rgb and pcb ptrs of the parameters.
//|
//| Parms:
//|		lpDIALOGDESC
//|
//|
//*---------------------------------------------------------------------------------
VOID  SetParamPtrs(lpDIALOGDESC lpDlg )
{

	UINT dex, dex2;
	UDWORD lDataStyle;



	//loop through all the controls and set the rgb and pcb ptrs in the param array.
	for ( dex = 0; dex < lpDlg->cCtlItems  ; dex++)
	{

		if( lpDlg->lpCtl[dex]->wLengthID )
		{

			lDataStyle = lpDlg->lpCtl[dex]->lDataStyle;
			dex2 = GetIndexFromControlID(lpDlg, (UINT)lpDlg->lpCtl[dex]->wLengthID);

			if(lDataStyle & CBO_LINKED_FIELD) //it's an rgb value
				lpDlg->lpParms[lpDlg->lpCtl[dex]->iParmNum]->lpParmrgb = lpDlg->lpParms[lpDlg->lpCtl[dex2]->iParmNum];
			else
				lpDlg->lpParms[lpDlg->lpCtl[dex]->iParmNum]->lpParmpcb = lpDlg->lpParms[lpDlg->lpCtl[dex2]->iParmNum];
		}
		if ( lpDlg->lpCtl[dex]->wDataID)
		{

			dex2 = GetIndexFromControlID(lpDlg, (UINT)lpDlg->lpCtl[dex]->wDataID);
			lpDlg->lpParms[lpDlg->lpCtl[dex]->iParmNum]->lpParmrgb = lpDlg->lpParms[lpDlg->lpCtl[dex2]->iParmNum];
		}

	}


}

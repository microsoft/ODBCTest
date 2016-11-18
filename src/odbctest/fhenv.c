//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHENV.C
//|
//| Purpose:
//|		This module contains the function handlers for the environments
//*---------------------------------------------------------------------------------
#include "fhheader.h"

//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
UINT									cEnvs=0;						// Count of Environments

extern TCHAR							szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndFrame;							// Main window (for menu access)
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern lpDFTARRAY					lpdftGetInfo;
extern UINT							ciInfoTypes;
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern HMENU						hMenu;								// Menu handle for Trace item

lpHEADENV							lpHeadEnv;				//Pointer to Global Env structure
UINT									cNextEnv=1;				// Number assigned to next environment


UINT INTFUN GetEnvNum(	lpHEADENV lpHeadEnv);





//*---------------------------------------------------------------------------------
//| SetCurrentHdbcFromList:
//|	This function will find and set the current hdbc node based on the current
//|	selection in the hstmt list of the given connection window.
//| Parms:
//|	lpci						The connection window to set
//| Returns:
//|	Nothing.
//*---------------------------------------------------------------------------------
VOID INTFUN SetCurrentHdbcFromList(lpENVIRONMENTINFO lpei)
{
	SWORD wIndex=(SWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCURSEL, 0, 0L);

	// Assum head node to start
	lpei = lpHeadEnv->lpHead;

	// An error or index==0 means NULL
	if(!wIndex || (wIndex == CB_ERR))
		lpei->lpCurConn = NULL;
	// Else index is node number in order from head
	else while(--wIndex)
		lpei->lpCurConn = lpei->lpCurConn->next;

} //SetCurrentHdbcFromList()


//*---------------------------------------------------------------------------------
//| GetActiveEnvNode:
//|	This function will return the current active ENV node
//| Parms:
//|	None
//| Returns:
//|	lpei		active ENV node
//*---------------------------------------------------------------------------------
lpENVIRONMENTINFO INTFUN GetActiveEnvNode()
{
	lpENVIRONMENTINFO lpei=NULL;
	SWORD wIndex=(SWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCURSEL, 0, 0L);

	// Assum head node to start
	lpei = lpHeadEnv->lpHead;

	// An error or index==0 means NULL
	if(!wIndex || (wIndex == CB_ERR))
		lpei=NULL;
	// Else index is node number in order from head
	else while(--wIndex)
		lpei=lpei->next;

	return(lpei);

} //GetActiveEnvNode()

//*---------------------------------------------------------------------------------
//| SetCurrentHdbcFromList:
//|	This function will return the current active henv
//| Parms:
//|	None
//| Returns:
//|	lpei		active ENV node
//*---------------------------------------------------------------------------------
HENV INTFUN GetActiveEnvHandle()
{
	lpENVIRONMENTINFO lpei=NULL;
	SWORD wIndex=(SWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCURSEL, 0, 0L);

	// Assum head node to start
	lpei = lpHeadEnv->lpHead;

	// An error or index==0 means NULL
	if(!wIndex || (wIndex == CB_ERR))
		lpei=NULL;
	// Else index is node number in order from head
	else while(--wIndex)
		lpei=lpei->next;

	return(lpei ? lpei->henv : NULL);

} //SetCurrentHdbcFromList()


//*---------------------------------------------------------------------------------
//| GetActiveEnv:
//|	This function will find and set the current ENVIRONMENTINFO node based on the current
//|	selection in the hstmt list of the given connection window.
//| Parms:
//|	None
//| Returns:
//|	lpei			Active ENVIRONMENTINFO node.
//*---------------------------------------------------------------------------------
lpENVIRONMENTINFO INTFUN GetActiveEnv()
{
	SWORD wIndex=0;
	lpENVIRONMENTINFO lpEi=NULL;

	if (!lpHeadEnv)
		return(NULL);

	wIndex=(SWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCURSEL, 0, 0L);
	lpEi=lpHeadEnv->lpHead; //assume head node

	// An error or index==0 means NULL
	if(!wIndex || (wIndex == CB_ERR))
		lpEi = NULL;
	// Else index is node number in order from head
	else
	{
		while(--wIndex)
			lpEi = lpEi->next;
	}

	return(lpEi);

} //GetActiveEnv()


//*---------------------------------------------------------------------------------
//| DropAllConns:
//|	This function will free all of the hstmts for a connection window.
//|	The caller determines if each hstmt must be dropped (won't be the
//|	case when SQLDisconnect is successful).
//| Parms:
//|	lpci						Connection window to free
//|	fNeedToFree				TRUE if we must try to free each hstmt
//| Returns:
//|	cConns					Number of remaining connections
//*---------------------------------------------------------------------------------
SWORD INTFUN DropAllConns(lpENVIRONMENTINFO lpei)
{
	UINT cConns=(lpei ? lpei->cConns : 0);

	while(cConns > 0)
	{
		if (!ConnectFullDisconnect((lpei->lpCurConn) ? lpei->lpCurConn : lpei->lpConn->prev,SQL_SUCCESS))
			return (-1);

		if (cConns > 0)
			cConns--;

	}

	return((SWORD)cConns);

} //DropAllConns()


//*---------------------------------------------------------------------------------
//| DoDropConn:
//|	This function set states and free statement nodes after an hstmt has been
//|	successfully drop using SQLFreeStmt.
//| Parms:
//|	ei							Current environment information
//| Returns:
//|	A pointer to the statement info structure if successful, else NULL
//*---------------------------------------------------------------------------------
VOID INTFUN DoDropConn(lpCONNECTIONINFO lpConn)
{
	lpCONNECTIONINFO	lpHeadConn=lpHeadEnv->lpHead->lpConn;

	DropAllStmts(lpConn,TRUE);

	if(lpConn->next != lpConn) {		// More than one node
		lpConn->prev->next = lpConn->next;
		lpConn->next->prev = lpConn->prev;
		if(lpHeadConn == lpConn)			// Removing head node?
			lpHeadConn = lpConn->next;
		}
	else											// Removing only node
		lpHeadConn = NULL;


	// Now set the current selection
	if (lpConn)
		SetCurrentHdbcFromList(lpConn->lpActiveEnv);

	// Refresh the toolbar so that some buttons get disabled correctly
	RefreshToolbar();

} //DoDropConn()


//*---------------------------------------------------------------------------------
//| DoFreeEnvNode:
//|	This function will free our henv.  Output will be logged if there is an
//|	error and we have a window to log it in.  If successful, this function
//|	has no output.
//| Parms:
//|	lpEi	pointer to ENVIRONMENTINFO linked list
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
lpENVIRONMENTINFO INTFUN DoFreeEnvNode(lpENVIRONMENTINFO lpEi)
{
	SWORD		cConns=0;
	DWORD		dwIndex=0,
				dwCnt=0;

	dwIndex=FindIndexInDropDown(lpHeadEnv->hwndEnvs,lpEi->henv,lpEi->uEnvNum,szHENV);
	DropNodeFromToolBar(lpHeadEnv->hwndEnvs,dwIndex);

	// Remove the node from the linked list
	if(lpEi->next != lpEi)
	{
		// More than one node
		lpEi->prev->next = lpEi->next;
		lpEi->next->prev = lpEi->prev;
		// Removing head node?
		if(lpHeadEnv->lpHead == lpEi)
			lpHeadEnv->lpHead = lpEi->next;
	}
	else
	{
		lpEi->lpCurConn = NULL;
		lpEi->lpConn = NULL;
	}

	FREE_PTR(lpEi);

	lpEi=NULL;

	//one less EI node
	if (lpHeadEnv->cEnvs > 0)
		lpHeadEnv->cEnvs--;

	if (!lpHeadEnv->cEnvs)
	{
		dwIndex=0;
		lpHeadEnv->lpHead=NULL;
	}
	else
	{
		dwCnt=(DWORD)SendMessage(lpHeadEnv->hwndEnvs, CB_GETCOUNT, 0L, 0L);
	}

	SendMessage(lpHeadEnv->hwndEnvs, CB_SETCURSEL, (WPARAM)dwIndex, 0L);

	// Refresh the toolbar (for all buttons relating to an allocated henv)
	RefreshToolbar();

	return(lpEi);


} //DoFreeEnvNode()


//*---------------------------------------------------------------------------------
//| DoDropEnv:
//|	This function will free our henv.  Output will be logged if there is an
//|	error and we have a window to log it in.  If successful, this function
//|	has no output.
//| Parms:
//|	lpEi	pointer to ENVIRONMENTINFO linked list
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN DoDropEnv(lpENVIRONMENTINFO lpEi)
{
	SWORD		cConns=0;

	//if all connections have been dropped then nuke the EI  node
	if (lpEi)
	{
		if (lpEi->cConns > 0)
		{
			if (DropAllConns(lpEi) == -1)
				return(FALSE);
		}
		else
		{
			// Invoke the function
			if (RC_SUCCESSFUL(SQLFreeEnv(lpEi->henv)))
				DoFreeEnvNode(lpEi);

		}
	}

	return(TRUE);

} //DoDropEnv()




//*---------------------------------------------------------------------------------
//| DoDropAllEnvs:
//|	This function will free our henv.  Output will be logged if there is an
//|	error and we have a window to log it in.  If successful, this function
//|	has no output.
//| Parms:
//|	lpEi	pointer to ENVIRONMENTINFO linked list
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN DoDropAllEnvs(lpENVIRONMENTINFO lpEi)
{
	SWORD	cEnvs=lpHeadEnv->cEnvs;

	if (cEnvs > 0)
	{
		while (cEnvs--)
		{
			if (!DoDropEnv(lpEi ? lpEi->prev: lpEi))
				return(FALSE);
		}
	}

	// Refresh the toolbar (for all buttons relating to an allocated henv)
	RefreshToolbar();

	return(TRUE);

} //DoFreeEnvNode()



//*---------------------------------------------------------------------------------
//| lpSQLFreeEnv:
//|	This function will free our henv.  Output will be logged if there is an
//|	error and we have a window to log it in.  If successful, this function
//|	has no output.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLFreeEnv(STD_FH_PARMS)
{
	lpENVIRONMENTINFO	lpEnv=NULL;
	RETCODE				rc=SQL_SUCCESS;
	HENV					henv=NULL;

	if (lpEnv && lpEnv->henv)
		henv=lpEnv->henv;

	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	//Get current active hEnv Node
	lpEnv=GetActiveEnv();

	// Invoke the function
	rc=SQLFreeEnv((lpParms[0]->fNull) ? NULL : lpEnv->henv);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

   //  Check for errors
   AUTOLOGERROREI(ci,lpEnv,rc,henv);

	if (RC_SUCCESSFUL(rc))
		DoFreeEnvNode(lpEnv);

   return(rc);

} //lpSQLFreeEnv()



//*---------------------------------------------------------------------------------
//| FindActiveEnvNode:
//|	This function locates
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
lpENVIRONMENTINFO INTFUN FindActiveEnvNode(lpENVIRONMENTINFO lpHeadEnv,
	lpCONNECTIONINFO lpActiveConnWin,int cNodes)
{
	lpENVIRONMENTINFO	lpenv=lpHeadEnv;
	BOOL					fFound=FALSE;

	while (lpenv && !fFound)
		{
		fFound=lpenv->lpCurConn == lpActiveConnWin;
		lpenv=lpenv->next;
		}

	if (!fFound)
		lpenv=NULL;

	return(lpenv);

} //FindActiveEnvNode()




//*---------------------------------------------------------------------------------
//| lpSQLAllocEnv:
//|	Allocate an environment handle.  This will have no affect on anyting
//|	but the global value henv variable, since this application only allocates
//|	one henv.  Any errors are logged and displayed via szMessageBox since there
//|	is no output window and this function will not be called if there is already
//|	an allocated henv.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
RETCODE INTFUN lpSQLAllocEnv(STD_FH_PARMS)
{
	RETCODE	rc=SQL_SUCCESS;
	HENV		henv=NULL;
	lpENVIRONMENTINFO	lpei=NULL;
	SQLHDBC					hdbc=NULL;
	SQLHSTMT					hstmt=NULL;


	// Log input parameters
	LOGPARAMETERS(szFuncName, lpParms,cParms, ci, TRUE);

	// Invoke the function
	rc=SQLAllocEnv(lpParms[0]->fNull ? NULL : &henv);

	// Log return code information
	LOGRETURNCODE(NULL, ci, rc);

	if (RC_SUCCESSFUL(rc))
		lpei=DoAllocEnv(henv);

	if(lpUsrOptions->fAutoErrors && (rc != SQL_INVALID_HANDLE))
	{
		if (ci)
		{
			hdbc=ci->hdbc;

			if (ci->lpCurStmt)
				hstmt=ci->lpCurStmt->hstmt;

		}

		ToolOptionsAllErrors(ci, henv, hdbc,hstmt,FALSE,rc);
	}

	return(rc);

} //lpSQLAllocEnv()


//*---------------------------------------------------------------------------------
//| DoAllocEnv:
//|	Allocate an environment handle.  This will have no affect on anyting
//|	but the global value henv variable, since this application only allocates
//|	one henv.  Any errors are logged and displayed via szMessageBox since there
//|	is no output window and this function will not be called if there is already
//|	an allocated henv.
//| Parms:
//|	lpDlg						The dialog descriptor
//|	ci							Current connection informaiton (always NULL)
//|	lpParms					Pointer to the paramter array to use for the request
//| Returns:
//|	The return code from the function
//*---------------------------------------------------------------------------------
lpENVIRONMENTINFO INTFUN DoAllocEnv(HENV henv)
{
	RETCODE				rc=SQL_SUCCESS;
	lpENVIRONMENTINFO	lpei=NULL,
							lpHead=(lpHeadEnv?lpHeadEnv->lpHead: NULL);

	// Allocate memory for a new node, then create the window
	lpei = (lpENVIRONMENTINFO)AllocateMemory(sizeof(ENVIRONMENTINFO));
	if (!lpei)
		return(NULL);


	lpei->hInst = hInst;
	lpei->hwndClient = hwndClient;
	lpei->uEnvNum = (UINT)GetEnvNum(lpHeadEnv);
	lpei->henv=henv;

	if(lpHead)
		{
		//insert node at end of list
		lpHeadEnv->lpHead->prev->next=lpei;
		lpei->prev=lpHeadEnv->lpHead->prev;
		lpei->next=lpHeadEnv->lpHead;
		lpHeadEnv->lpHead->prev=lpei;
		}
	else
		{							// Only connection at this time
		lpHeadEnv->lpHead=lpei;
		lpHeadEnv->lpHead->next = lpei;
		lpHeadEnv->lpHead->prev = lpei;
		}


	// Finally add the new node to the mini-tool bar of henvs
	 if (henv)
		AddNodeToToolBar(lpei->uEnvNum,(HANDLE)henv,lpHeadEnv->hwndEnvs,szHENV);

	//Default NTS Env. Attr to TRUE.
	lpei->fNTS=TRUE;

	return(lpei);

} //DoAllocEnv()


//*---------------------------------------------------------------------------------
//| FindHdbcNode:
//|		This function finds the CONNECTIONINFO node in the linked list hanging off
//|	off of the ENVIRONMENTINFO node.
//| Parms:
//|	lpEi				pointer to ENVIRONMENTINFO node associated with the current
//|						connection node
//|	lpCi				pointer to CONNECTIONINFO node to be removed from
//|						  list of CONNECTIONINFO nodes.
//| Returns:
//|	lpCi				revised connectioninfo list (i.e. with the disconnected CI node
//|						removed).
//*---------------------------------------------------------------------------------

lpCONNECTIONINFO INTFUN FindHdbcNode(lpENVIRONMENTINFO lpEi,lpCONNECTIONINFO lpCi)
{
	lpCONNECTIONINFO	lpHeadCi=NULL;
	int					i,
							cHdbcs;

	//Get current Environment information node
	if (!lpCi)
		return(NULL);

	lpEi=lpCi->lpActiveEnv;

	//Set # of connections hanging off this EI nodef
	cHdbcs=lpEi->cConns;

	//Set head node of HDBC list
	lpHeadCi=lpEi->lpConn;

	//Find specified CI node
	for (i=0;(i < cHdbcs) && lpHeadCi;i++)
		{
		if (lpHeadCi == lpCi)
			return(lpHeadCi);
		else
			lpHeadCi=lpHeadCi->next;

		} //end for


	return(lpHeadCi);

} //FindHdbcNode()


//*---------------------------------------------------------------------------------
//| RemoveHdbcNode:
//|		This function removes the CONNECTIONINFO node in the linked list hanging off
//|	off of the ENVIRONMENTINFO node.
//| Parms:
//|	lpCi				pointer to CONNECTIONINFO node to be removed from
//|						  list of CONNECTIONINFO nodes.
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN RemoveHdbcNode(lpCONNECTIONINFO lpCi)
{
	lpCONNECTIONINFO	lpNext=NULL,
							lpPrev=NULL;

	//Get current Environment information node
	if (!lpCi)
		return;

	lpNext=lpCi->next;
	lpPrev=lpCi->prev;

	lpNext->prev=lpPrev;
	lpPrev->next=lpNext;

	FREE_PTR(lpCi);

	lpCi=NULL;

} //FindHdbcNode()



//*---------------------------------------------------------------------------------
//| RemoveHdbcFromList:
//|		This function removes the CONNECTIONINFO node to the list of existing
//|	CONNECTIONINFO linked list.
//| Parms:
//|	lpci				pointer to CONNECTIONINFO node to be added to
//|						  list of CONNECTIONINFO nodes.
//| Returns:
//|	lpActiveEnv		ENVIRONMENTINFO node
//*---------------------------------------------------------------------------------

lpENVIRONMENTINFO INTFUN RemoveHdbcFromList(lpCONNECTIONINFO lpCi)
{
	lpENVIRONMENTINFO lpEi=NULL;
	lpCONNECTIONINFO	lpCurrCi=NULL;

	//Get current Environment information node
	if (!lpCi)
		return(NULL);

	lpEi=lpCi->lpActiveEnv;

	if (lpCurrCi=FindHdbcNode(lpEi,lpCi))
		RemoveHdbcNode(lpCurrCi);

	if (lpEi->cConns > 0)
		lpEi->cConns--;

	return(lpEi);

} //RemoveConnectionFromList()

UINT INTFUN GetEnvNum(	lpHEADENV lpHeadEnv)
{

	lpENVIRONMENTINFO  lpEnv;
	UINT					max = 0,count =lpHeadEnv->cEnvs;



	if( ! lpHeadEnv->cEnvs  )
		return ++lpHeadEnv->cEnvs;
	else
		for (lpEnv = lpHeadEnv->lpHead ; count-- && lpEnv!=NULL; lpEnv = lpEnv->next)
			if ( lpEnv->uEnvNum > max )
				max = lpEnv->uEnvNum;
	++lpHeadEnv->cEnvs;
	return ++max;
}

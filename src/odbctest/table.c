//************************************************************************
//|
//|   Title: table.c
//|
//| 	(c) Copyright 1991, 1992, 1993
//| 	    by Microsoft Corp.
//|
//************************************************************************
#include "fhheader.h"
#include "table.h"
#pragma warning(disable:4996)
dCSEG(TCHAR) szStringFormat[]	= TEXT("'%s'");
extern TCHAR szEOS[];
dCSEG(TCHAR) chEOS			= TEXT('\0');
LPTSTR		szNEWLINE		= TEXT("\r\n");

VszFile;
extern TCHAR						szOutBuff[MAXOUTBUFF];
extern HINSTANCE 					hInst;								// For this app
extern HWND							hwndFrame;							// Main window (for menu access)
extern HWND							hwndClient;							// For MDI children
extern HWND							hwndStatBar;	               // For status bar updates
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern lpDFTARRAY					lpdftGetInfo;
extern UINT							ciInfoTypes;
extern lpUSROPTIONS				lpUsrOptions;						// Global user options
extern HMENU						hMenu;								// Menu handle for Trace item

//*-----------------------------------------------------------------------
//| CreateColumnName()
//|     This function creates the column name based on the column type
//|	Calling Function:
//|		All over
//| Parameters:
//|		szColName	string containing the created Column Name
//|		rgFields	Array of column info.
//|		nColNum		column number (i.e. index into rgFields)
//|		szPrefix	User defined prefix, used to create Column Name
//| Return Value:
//|		fError		Error Occured?
//*-----------------------------------------------------------------------

LPTSTR INTFUN CreateColumnName(FIELDINFO  *rgFields,UDWORD nColNum,
	LPTSTR szPrefix,LPTSTR szColName)
{

	int		nColLen,nCurChar;
	TCHAR		szBuff[TINYBUFF],
				szBuff2[SMALLBUFF];

	GetGtrString(szBuff2, sizeof(szBuff2)/sizeof(szBuff2[0]), idsColumnFormat);

	/* Use only the first 4 chars of the Type */
	for(nColLen=0,nCurChar=0;nColLen<4 && rgFields[nColNum].szType[nCurChar] != TEXT('\0');nCurChar++)
	{
		szBuff[nColLen]=rgFields[nColNum].szType[nCurChar];
		if(szBuff[nColLen]!=TEXT(' '))
			nColLen++;
	}

	/* Add null terminator */
	szBuff[nColLen]=TEXT('\0');
/*
	if (lstrlen(rgFields[nColNum].szType) >= nColLen)
		lstrcpyn(szBuff,rgFields[nColNum].szType,nColLen);
	else
		lstrcpy(szBuff,rgFields[nColNum].szType);
*/
	if (szColName)
		wsprintf(szColName,szBuff2,szPrefix,nColNum,szBuff);
	else
		wsprintf(rgFields[nColNum].szFieldName,szBuff2,szPrefix,nColNum,szBuff);

	return(rgFields[nColNum].szFieldName);

} /* CreateColumnName() */




LPTSTR  INTFUN MakeData(int nRow, FIELDINFO  *FieldInfo,LPTSTR  sz)
{
	SQLULEN precision;
	UDWORD scale;
	UDWORD i;
	TCHAR   szCol[10];
	TCHAR   szDecimal[SMALLBUFF];
	LPTSTR pch;
	TCHAR buf[SMALLBUFF];

	//Clear out Data buffer
	*sz = chEOS;

	if ((nRow == FieldInfo->iFieldNum && FieldInfo->UseNulls == SQL_NULLABLE) ||
	 	 FieldInfo->fAutoUpdate)
		return sz;

	_itot(nRow * 103, szCol, 10);


	if(FieldInfo->wSQLType == SQL_GUID){
		_tcscpy(buf,szCol);
		for( i=0; i< 12; i++)
			_tcscat(buf,szCol);
		_stprintf(sz,TEXT("%.8s-%.4s-%.4s-%.4s-%.12s"),buf,buf,buf,buf,buf);
		return sz;
	}

	switch (FieldInfo->wSQLType)
		{
		case SQL_FLOAT:
		case SQL_REAL:
		case SQL_DOUBLE:
			scale = nRow > MAX_SCALE ? MAX_SCALE : nRow;
			break;
		default:
			scale = FieldInfo->scale;
		}

	*sz = chEOS;

	if(FieldInfo->precision > QP_MAXSIZE || FieldInfo->precision <= 0)
		{
		if(scale > QP_MAXSIZE)
			scale = QP_MAXSIZE;
		precision = QP_MAXSIZE;
		}
	else
		precision = FieldInfo->precision;

	if(precision > 5)
		{
	    switch (FieldInfo->wSQLType)
			{
			case SQL_NUMERIC:
			case SQL_DECIMAL:
			case SQL_FLOAT:
			case SQL_DOUBLE:
			case SQL_INTEGER:
				precision--;
				break;
			case SQL_REAL:
				precision = 5;
				break;
			}
        }

	*szDecimal = chEOS;
	switch(FieldInfo->wSQLType)
		{
		case SQL_NUMERIC  :
		case SQL_DECIMAL  :
		case SQL_FLOAT    :
		case SQL_REAL     :
		case SQL_DOUBLE   :
			GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsPeriod);
			lstrcat(szDecimal, buf);
			for(i = 0; i < scale; i += lstrlen(szCol))
				lstrcat(szDecimal, szCol);
			break;

		default:
			scale = 0;
		}

	szDecimal[scale + 1] = chEOS;

	if(FieldInfo->wSQLType == SQL_FLOAT || FieldInfo->wSQLType == SQL_REAL ||
		FieldInfo->wSQLType == SQL_DOUBLE)
		{
		/* get rid of any 0's hanging on the end. */
		for(i = lstrlen(szDecimal) - 1;szDecimal[i] == TEXT('0'); i--);
		szDecimal[i + 1] = chEOS;
		}

	GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsHyphen);

	switch(FieldInfo->wSQLType)
		{
		case SQL_SMALLINT:
		case SQL_INTEGER:
		case SQL_TINYINT:
		case SQL_NUMERIC:
		case SQL_DECIMAL:
		case SQL_BIGINT:
		case SQL_DOUBLE:
		case SQL_FLOAT:
		case SQL_REAL:

		if(nRow % 3 == 1 && !FieldInfo->fUnsigned)
			lstrcat(sz, buf);
		}

	for(i = 1
		; i < (precision >= scale ? precision - scale : precision)
		; i += lstrlen(szCol))
		lstrcat(sz, (LPTSTR )szCol);

	sz[precision >= scale ? precision - scale : precision] = chEOS;
	if(!*sz || lstrcmp(sz, buf) == 0) {
		GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsZeroString);
		lstrcat(sz, buf);
	}
	lstrcat(sz, (LPTSTR )szDecimal);

	//if we had an odd 2 digit numbers, we may have ended up with an even
	//number of characters, one too many. */

	switch(FieldInfo->wSQLType)
		{
			/* types with a decimal point shouldn't have this problem */
		case SQL_DECIMAL:
			break;
		case SQL_NUMERIC:
			/* Oracles NUMBER(38,4) stores 0 as 0.0000 */
			if(nRow == 0)
			{
				GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsZeroString);
				lstrcpy(sz, buf);
				lstrcat(sz, szDecimal);
			}
			break;
		case SQL_DOUBLE:
		case SQL_FLOAT:
		case SQL_REAL:
			if(nRow == 0) {
				GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsZeroString);
				GetGtrString(szDecimal,sizeof(szDecimal)/sizeof(szDecimal[0]), idsPeriod);
				lstrcpy(sz, buf);
				lstrcat(sz, szDecimal);
				lstrcat(sz, buf);
			}
			break;
		default:
			sz[precision] = chEOS;

		}

	if(FieldInfo->wSQLType == SQL_BINARY ||
		FieldInfo->wSQLType == SQL_VARBINARY) {
		for(i = lstrlen(sz) - 1; i > 1 && sz[i] == TEXT('0'); i--)
			sz[i] = chEOS;
		/* binary numbers right justify everything */
		if(lstrlen(sz) > 2 && (lstrlen(sz) / 2 == (lstrlen(sz) - 1) / 2))
		{
			lstrcpy(szDecimal, sz);
			sz[0] = TEXT('0');
			lstrcpy(&sz[1], szDecimal);
		}
		if(nRow > 1)
			sz[0] = TEXT('F');
	}


	switch(FieldInfo->wSQLType) {
		TCHAR szNum[30];
		int  value;
		long  lvalue;

		case SQL_CHAR:
		case SQL_VARCHAR:
		case SQL_LONGVARCHAR:
			if(nRow > 0)
				sz[0] = TEXT('H');
			break;
		case SQL_TINYINT:
			lstrcpy(szNum, sz);
			value = _ttoi(szNum);
			if(FieldInfo->fUnsigned) {
				lstrcpy(sz, _itot(value % 0xFF, szNum, 10));
			} else
				lstrcpy(sz, _itot(value % 0x7F, szNum, 10));
			break;

		case SQL_SMALLINT:
			lstrcpy(szNum, sz);
			lvalue = _ttol(szNum);
			if(lvalue > 0x7FFF)
				lstrcpy(sz, _ltot(lvalue % 0x7FFF, szNum, 10));
			break;

		case SQL_BIT:
			lstrcpy(sz, _itot(!!nRow, szNum, 10)); /*bits should be 0 or 1 */
			break;

		case SQL_DATE:
		case SQL_TYPE_DATE:
			{
			TCHAR buf[30];
			TCHAR format[15];

			GetGtrString(format,sizeof(format)/sizeof(format[0]), idsDateStringFormat);
			wsprintf(buf, format, 1991, 10, nRow % 30 + 1);
			lstrcpy(sz, buf);
			break;
		}

		case SQL_TIMESTAMP:
		case SQL_TYPE_TIMESTAMP:
			{
			UWORD day;
			UWORD hour = 0;
			UWORD minute = 0;
			UWORD second = 0;
			UDWORD fraction = 0;
			TCHAR buf[30];

			switch(FieldInfo->precision) {
				case 21:
				case 22:
				case 23:
				case 24:
				case 25:
				case 26:
				case 27:
				case 28:
				case 29:
					fraction = nRow + 1;
				case 19:
					second = nRow % 60;
				case 16:
					minute = nRow % 60;
				case 13:
					hour = nRow % 24;
				case 10:
					day = nRow % 30 + 1;
					break;
				default:
					day = 0;
				}

			if(fraction != 0) {
				GetGtrString(szNum,sizeof(szNum)/sizeof(szNum[0]), idsDateFraction);
				_stprintf(buf, szNum, 1992, 10, day, hour,
					minute, second, FieldInfo->scale - 1, fraction);
			} else {
				GetGtrString(szNum,sizeof(szNum)/sizeof(szNum[0]), idsDateNoFraction);
				wsprintf(buf, szNum, 1991, 10, day, hour,
					minute, second);
			}
			lstrcpy(sz, buf);
			break;
		}

		case SQL_TIME:
		case SQL_TYPE_TIME:
			{
			TCHAR buf[30];
			TCHAR format[15];

			GetGtrString(format,sizeof(format)/sizeof(format[0]), idsTimeStringFormat);
			wsprintf(buf, format, nRow % 24, nRow % 60, nRow % 60);
			lstrcpy(sz, buf);
			break;

		}
	}

	/* make strings one smaller if they are less than 10 so we don't
		end up with dupes on multiples of 11 */
	if(nRow < 10 && (FieldInfo->wSQLType == SQL_CHAR ||
		FieldInfo->wSQLType == SQL_VARCHAR))
		sz[lstrlen(sz) - 1] = chEOS;

	pch = sz;
	if(!nRow)
		switch(FieldInfo->wSQLType) {
			case SQL_SMALLINT:
			case SQL_TINYINT:
			case SQL_INTEGER:
				GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsZeroString);
				lstrcpy(sz, buf);
				break;
			case SQL_DECIMAL:
				pch = _tcschr(sz, TEXT('.')) - 1;
				break;
			case SQL_VARBINARY:
			case SQL_LONGVARBINARY:
				GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsZeroBinaryString);
				lstrcpy(sz, buf);
				break;
		}

	if(!FieldInfo->iFieldNum)
	{
		TCHAR szzero[] = {TEXT('1'),chEOS};
		TCHAR szperiod[] = {TEXT('.'),chEOS};
		UDWORD ccount;
		_itot(nRow, buf, 10);

		switch(FieldInfo->wSQLType)
		{
			case SQL_DECIMAL:
			case SQL_NUMERIC:
				if(FieldInfo->scale)
				{
					lstrcpy(sz, buf);
					lstrcat(sz, szperiod);
					for(ccount = 0; ccount < FieldInfo->scale; ccount++)
						lstrcat(sz, szzero);
				}
				break;
			case SQL_REAL:
			case SQL_FLOAT:
			case SQL_DOUBLE:
				lstrcpy(sz, buf);
				lstrcat(sz, szperiod);
				lstrcat(sz, szzero);
				break;
		}
	}

	return pch;
}


int INTFUN DoBasicSelect(HSTMT hstmt, FIELDINFO  *rgFields,
	LPTSTR szTableName, int cTypes)
{

	TCHAR sz[1200];
	TCHAR buf[SMALLBUFF];
	int i;
	int	fError=FALSE;

	GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsComma);
	GetGtrString(sz,sizeof(sz)/sizeof(sz[0]), idsSELECT);

	for(i = 0; i < cTypes; i++)
		{
		lstrcat(sz, rgFields[i].szFieldName);
		lstrcat(sz, buf);
		}

	sz[lstrlen(sz) - lstrlen(buf)] = chEOS;

	GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsFromString);
	wsprintf(&sz[lstrlen(sz)], buf, (LPTSTR )szTableName);

	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
				sz);

	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
				szNEWLINE);

	if(RC_NOTSUCCESSFUL(SQLExecDirect(hstmt, sz, SQL_NTS)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,NULL,hstmt,NULL,SQL_ERROR);
	}

	return (fError);

}

BOOL INTFUN Is_Num_Type(SWORD fSqlType)
{
	switch(fSqlType)
		{
		case SQL_NUMERIC:
		case SQL_DECIMAL:
		case SQL_INTEGER:
		case SQL_SMALLINT:
		case SQL_FLOAT:
		case SQL_REAL:
		case SQL_DOUBLE:
		case SQL_BIGINT:
		case SQL_TINYINT:
			return TRUE;

		}

	return FALSE;

}


int INTFUN GetNumericColumn(FIELDINFO  *rgFields,int numCols)
{
	int i;

	//Find Char value in rgCols array
	for(i=0;i < numCols;i++)
		{
		if ((rgFields[i].fAutoInc) ||
			!Is_Num_Type(rgFields[i].wSQLType) ||
			(rgFields[i].wSQLType==SQL_TINYINT))
			continue;

		break;
		}

	if(i==numCols)
		return(-1);

	return i;
}



int INTFUN CreateIndex(HSTMT hstmt, LPTSTR  szIndexName,
	LPTSTR szTableName,LPTSTR szFieldName,int fIndexType, int fForce)
{
	RETCODE rc;
 	TCHAR szSqlStr[MEDBUFF],szIndexStr[MEDBUFF];
	int idsIndex;
	extern int failed;
	switch(fIndexType) {
		case UNIQUE_INDEX:
			idsIndex = idsUniqueIndex;
			break;
		case STANDARD_INDEX:
			idsIndex = idsCreateIndex;
			break;
		case CLUSTERED_INDEX:
			idsIndex = idsClusteredIndex;
			break;
		default: {
			GetGtrString(szIndexStr,sizeof(szIndexStr)/sizeof(szIndexStr[0]), idstype);
			GetGtrString(szSqlStr,sizeof(szSqlStr)/sizeof(szSqlStr[0]), ids_unknown);
			lstrcat(szSqlStr, szIndexStr);
			return FALSE;
		}
	}
	wsprintf(szSqlStr,GetGtrString(szIndexStr,sizeof(szIndexStr)/sizeof(szIndexStr[0]),idsIndex),
		szIndexName,szTableName, szFieldName);

	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
				szSqlStr);
	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
				szNEWLINE);

	rc=SQLExecDirect(hstmt, szSqlStr, SQL_NTS);

	return rc == SQL_SUCCESS;
}


int INTFUN MakeUniqueIndex(HDBC  *pHdbc,HSTMT hstmt,LPTSTR szTableName,
	LPTSTR szFieldName, int fForce)
{
TCHAR szIndName[50],szReadOnly[2];
SWORD	cbReadOnly;

	// we need some way here to check if indexes are supported
	// the best we can do for now is assume that if a driver is read-only,
	// you can't create indexes
	if (RC_NOTSUCCESSFUL(SQLGetInfo(*pHdbc,SQL_DATA_SOURCE_READ_ONLY,szReadOnly,2,&cbReadOnly)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,*pHdbc,NULL,NULL,SQL_ERROR);
	}

	if(lstrcmp(szReadOnly,TEXT("N")))
		{
		// the driver is read-only, so we can't create indexes
		//QPTabidsWrite(idsCreateIndexFailed,3,TRUE);
		return 1;
		}

	// first, try using the tablename as the index name
	if(!CreateIndex(hstmt,szTableName,szTableName,szFieldName,UNIQUE_INDEX,NOFORCE))
		{
		// try a different index name
		lstrcpy(szIndName,szTableName);
		szIndName[0]=TEXT('x');
		CreateIndex(hstmt,szIndName,szTableName,szFieldName,UNIQUE_INDEX,fForce);
		}
	return 1;
}




LPTSTR  INTFUN GetTableName(HDBC  *pHdbc,LPTSTR szNameBuf)
{
	TIMEINFO	time;
	HSTMT		hstmt0=NULL;
	SWORD		cTableName;
	TCHAR 	sz_GetTableFormat[MEDBUFF],
				szSqlStr[100],
				idsBuf[MEDBUFF],
				sep[5],
				szUserName[3];
	RETCODE	rc=SQL_SUCCESS;
	BOOL		fError=FALSE;

	// we need another hstmt for this function to complete, so abort
	// if we can't get one

	if (FSSQLAllocStmt(*pHdbc,&hstmt0))
		goto GetTableNameCleanup;

	//While loop continues to generate new table names
	//until a SELECT statement with the table name returns
	//SQL_ERROR (table d/n exist)
	while(rc==SQL_SUCCESS)
	{
		lstrcpy(szUserName,TEXT("gtr"));

		GetGtrString(sz_GetTableFormat,sizeof(sz_GetTableFormat)/sizeof(sz_GetTableFormat[0]),idsGetTableFormat);

		rc=SQLGetInfo(*pHdbc, SQL_MAX_TABLE_NAME_LEN, &cTableName,
			sizeof (int), NULL);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,*pHdbc,NULL,NULL,SQL_ERROR);
			goto GetTableNameCleanup;
		}

		if(cTableName > TABLENAMESIZE)
			cTableName = TABLENAMESIZE;

      // Pull back the separator.  Note not all drivers will have one,
      //		this is not an error
		SQLGetInfo(*pHdbc, SQL_QUALIFIER_NAME_SEPARATOR,sep,
			sizeof(sep),NULL);

		if(!lstrcmp(sep,TEXT("\\")))	  /* this means the table is a file-name */
			cTableName = 8;

		GetTime(&time);
		wsprintf(szNameBuf, sz_GetTableFormat, (LPTSTR)szUserName,
				(int)GetTimeSeconds(time),	*pHdbc);

		if(cTableName<lstrlen(szNameBuf))
			szNameBuf[cTableName] = chEOS;

		wsprintf(szSqlStr,GetGtrString(idsBuf,sizeof(idsBuf)/sizeof(idsBuf[0]),idsSelectStarPercent),
			(LPTSTR )szNameBuf);

 	 	rc=SQLExecDirect(hstmt0,szSqlStr,SQL_NTS);

		if (RC_NOTSUCCESSFUL(SQLFreeStmt(hstmt0,SQL_CLOSE)))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,hstmt0,NULL,SQL_ERROR);
		}


	}

GetTableNameCleanup:

	if (fError)
		return(NULL);

	if (RC_NOTSUCCESSFUL(SQLFreeStmt(hstmt0,SQL_DROP)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,*pHdbc,NULL,NULL,SQL_ERROR);
	}

 	return szNameBuf;

} //GetTableName()

int INTFUN QPNumCreateParams(LPTSTR szCreateParams)
{
int iNumCP=1,iCharCounter;
 	//If szCreateParams has ANYTHING, then there is at least 1 CP,
	//so iNumCP defaults to 1
	if(szCreateParams==NULL) return 0;
	if(lstrlen(szCreateParams)==0) return 0;
	for(iCharCounter=0;iCharCounter<lstrlen(szCreateParams);iCharCounter++)
		if(szCreateParams[iCharCounter]==TEXT(',')) iNumCP++;
	return iNumCP;
}


//This function returns TRUE if the current type needs to be eliminated
//from the table.
BOOL INTFUN EliminateTypes(LPTSTR szType,SWORD wSQLType)
{

	switch(wSQLType)
	{
		case SQL_BINARY:
			return(TRUE);
		case SQL_TYPE_TIMESTAMP:
			return(TRUE);
		default:
			;
	}

	if(!lstrcmp(szType, TEXT("LONG VARCHAR")))
		return(TRUE);

	if(!lstrcmp(szType, TEXT("LONG VARCHAR FOR BIT DATA")))
		return(TRUE);

	return(FALSE);

}

LPTSTR INTFUN CreateParams(FIELDINFO  *rgFields,int iCol,LPTSTR szParam)
{

	int 	iNumCParams;
	LPTSTR	pch=NULL;
	TCHAR	szNum[30],
			buf[30];

	szParam[0] = chEOS;

	iNumCParams=QPNumCreateParams(rgFields[iCol].szParams);
	lstrcpy(szParam, rgFields[iCol].szType);

	if(iNumCParams==1)
		{
		if(pch = _tcschr(szParam, TEXT(')')))
			{
			*pch = chEOS;
			lstrcat(szParam, rgFields[iCol].szLength);
			lstrcat(szParam, _tcschr(rgFields[iCol].szType, TEXT(')')));
			}
		else
			{
			GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsOpenParen);
			lstrcat(szParam, buf);
			lstrcat(szParam, rgFields[iCol].szLength);
			GetGtrString(buf,MEDBUFF, idsCloseParen);
			lstrcat(szParam, buf);
			}
		}

	if(iNumCParams==2)
		{
		lstrcpy(szNum,rgFields[iCol].szLength);
		rgFields[iCol].precisionUsed = _ttoi(szNum);
		rgFields[iCol].scaleUsed=4;
		GetGtrString(buf,MEDBUFF, idsOpenParen);
		lstrcat(szParam, buf);
		lstrcat(szParam, rgFields[iCol].szLength);
		GetGtrString(buf,MEDBUFF, idsComma);
		lstrcat(szParam, buf);
		lstrcat(szParam, _itot(rgFields[iCol].scaleUsed, szNum, 10));
		GetGtrString(buf,MEDBUFF, idsCloseParen);
		lstrcat(szParam, buf);
		}

/*	GetGtrString(buf,MEDBUFF, idsFieldNameFormat);
	wsprintf(rgFields[i].szFieldName, buf, i,(LPTSTR )rgFields[i].szType);

*/

	lstrcpy(rgFields[iCol].szColDef,szParam);

	return(szParam);

} /* CreateParams() */



UINT INTFUN GetTypeInfo(HDBC  *pHdbc,HSTMT  *pHstmt0,lpFIELDINFO rgFields,
	UINT uSeed)
{
	BOOL 	fError=FALSE;
	UINT	uIdentity=0;
	UINT	i=0;
	UWORD	cFieldName;
	SQLLEN	sdw;
	TCHAR	*pch=NULL,
			szNum[30];
	BOOL	fFound=FALSE;
	RETCODE	rc=SQL_SUCCESS;

	// Get the type information so we can use it in a create statement
	// for a table

	rc=SQLGetInfo(*pHdbc, SQL_MAX_COLUMN_NAME_LEN, &cFieldName,sizeof(cFieldName), NULL);
	if (RC_NOTSUCCESSFUL(rc))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,*pHdbc,NULL,NULL,SQL_ERROR);
		goto TypeInfoCleanup;
	}

	cFieldName=(cFieldName > MAX_TYPE) ? MAX_TYPE : cFieldName;

	if (RC_NOTSUCCESSFUL(SQLGetTypeInfo(*pHstmt0, SQL_ALL_TYPES)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
		goto TypeInfoCleanup;
	}

	while (i < MAX_TYPES)
		{

		if (RC_NOTSUCCESSFUL(SQLFetch(*pHstmt0)))
			break;

		*rgFields[i].szType	  = *rgFields[i].szLength =
		*rgFields[i].szPrefix = *rgFields[i].szSuffix =
		*rgFields[i].szParams = chEOS;

		rgFields[i].iFieldNum = i;

		/* get type name */
		rc=SQLGetData(*pHstmt0,1,SQL_C_TCHAR,rgFields[i].szType,
			sizeof(rgFields[i].szType),&sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		// null terminate at max length
		rgFields[i].szFieldName[cFieldName] = chEOS;

		// change spaces in type name to '_'
		while((pch = _tcschr(rgFields[i].szFieldName, TEXT(' '))) )
			*pch = TEXT('_');

		CreateColumnName(rgFields,i,TEXT("C"),NULL);

		/* get the SQLType */
		rc=SQLGetData(*pHstmt0, 2, SQL_C_DEFAULT,(SWORD  *)&rgFields[i].wSQLType,
			sizeof(rgFields[i].wSQLType), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		/* Max length */
		rc=SQLGetData(*pHstmt0, 3, SQL_C_TCHAR,rgFields[i].szLength,
			sizeof(rgFields[i].szLength), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		/* if the length is over 10,000 -> divide it by 10 */
		if(sdw>=4)
			(rgFields[i].szLength[3]=chEOS);

		// don't actually use literal prefix or suffix,
		//	using Prepare/ Execute with params

		/* literal prefix */
		rc=SQLGetData(*pHstmt0, 4, SQL_C_TCHAR, rgFields[i].szPrefix,
			sizeof(rgFields[i].szPrefix),&sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		/* literal suffix */
		rc=SQLGetData(*pHstmt0, 5, SQL_C_TCHAR,rgFields[i].szSuffix,
			sizeof(rgFields[i].szSuffix), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		/* create params */
		rc=SQLGetData(*pHstmt0, 6, SQL_C_TCHAR,rgFields[i].szParams,
			sizeof(rgFields[i].szParams), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		// nullable
		rc=SQLGetData(*pHstmt0, 7, SQL_C_USHORT, (SWORD  *)&(rgFields[i].nullable),
			sizeof(rgFields[i].nullable), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		/* searchable */
		rc=SQLGetData(*pHstmt0, 9, SQL_C_TCHAR, szNum,
			sizeof(szNum), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		/* auto increment */
		rgFields[i].fAutoInc=0;
		rc=SQLGetData(*pHstmt0, 12, SQL_C_SHORT, (UWORD  *)&(rgFields[i].fAutoInc),
			sizeof(rgFields[i].fAutoInc), &sdw);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto TypeInfoCleanup;
		}

		rgFields[i].fSearchable = _ttoi(szNum);

		//Check for identity column.only allow 1 per table
		if ((sdw != SQL_NULL_DATA) && rgFields[i].fAutoInc)
		{
			uIdentity++;

			//See if it matches passed in seed
			if (!fFound && (uIdentity==uSeed))
				fFound=TRUE;
			else
				continue;
		}

		if (EliminateTypes(rgFields[i].szType,rgFields[i].wSQLType))
			continue;

		//next data type
		i++;
	}

	if (RC_NOTSUCCESSFUL(SQLFreeStmt(*pHstmt0,SQL_CLOSE)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
	}

TypeInfoCleanup:

	//return total number of SQL Types
	return(i);

} //GetTypeInfo()

//*-----------------------------------------------------------------------
//| DoCononicalFormatInput()
//|     This function wraps the data to be inserted with a cononical
//|		format if needed.
//|	Calling Function:
//|		MakeTable()
//| Parameters:
//| Return Value:
//|		fError		Error Occured?
//*-----------------------------------------------------------------------

LPTSTR  INTFUN DoCononicalFormatInput(LPTSTR  sz, int wSQLType,BOOL fAutoMakeTable)
{
	TCHAR buf[QP_MAXSTRINGSIZE];
	TCHAR FormatString[QP_MAXSTRINGSIZE];

	if(!sz || !*sz)
		return sz;

	switch(wSQLType)
		{
		case SQL_CHAR:
		case SQL_VARCHAR:
		case SQL_LONGVARCHAR:
		case SQL_BINARY:
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:
			if (!fAutoMakeTable) //if called from AutoMakeTable don't quote char strings
				{
				wsprintf(buf, szStringFormat,sz);
				lstrcpy(sz, buf);
				}
			break;
		case SQL_TIME:
		case SQL_DATE:
		case SQL_TIMESTAMP:
		case SQL_TYPE_TIME:
		case SQL_TYPE_DATE:
		case SQL_TYPE_TIMESTAMP:
			GetGtrString(FormatString,sizeof(FormatString)/sizeof(FormatString[0]), idsESCStringFormat);
			switch(wSQLType)
				{
				case SQL_TIME:
				case SQL_TYPE_TIME:
					wsprintf(buf, FormatString, TEXT('t'), TEXT(' '), sz);
					break;
				case SQL_DATE:
				case SQL_TYPE_DATE:
					wsprintf(buf, FormatString, TEXT('d'), TEXT(' '), sz);
					break;
				case SQL_TIMESTAMP:
				case SQL_TYPE_TIMESTAMP:
					wsprintf(buf, FormatString, TEXT('t'),TEXT('s'), sz);
					break;
				}
			lstrcpy(sz, buf);
			break;
		default:;
		}

	return sz;
}


BOOL INTFUN CreateInsert(HSTMT *pHstmt0,lpFIELDINFO rgFields,lpQTSTRUCT lpqt,int cTypes)
{
	LPTSTR	pch=NULL;
	SQLLEN	sdw=0;
	UWORD	w;
	int		i,
			ind;
	BOOL	fError=FALSE;
	lpPSTRUCT lpParams=NULL;
	TCHAR	buf[MEDBUFF];
	BOOL	fColNames=TRUE;
	RETCODE	rc=SQL_SUCCESS;

	lpParams = (lpPSTRUCT)AllocateMemory(sizeof (PSTRUCT));

	// put together the insert statement, and set the parameters
	// parameters are only set the first time through, after which
	// the contents of the pointers is changed, using the prepare
	// setparam method

	*lpqt->szColNames = chEOS;
	*lpqt->szValues = chEOS;

	for(i = 0; i <= cTypes; i++)
		{
		for(ind = 0, w = 0; ind < cTypes; ind++)
			{
			//we don't want to insert if this is an
			//identity column
			if (rgFields[ind].fAutoInc)
				continue;

			if (rgFields[ind].wSQLType == 93)
			{
				pch = DoCononicalFormatInput(MakeData(i+1,&rgFields[ind],
					lpqt->szDataItem),rgFields[ind].wSQLType,AUTO_MAKE_TABLE);
			}
			else if( rgFields[ind].wSQLType == -11)
				{
					pch = DoCononicalFormatInput(MakeData(i+1,&rgFields[ind],
					lpqt->szDataItem),rgFields[ind].wSQLType,AUTO_MAKE_TABLE);
					rgFields[ind].precision = 36;
				}
				else
					{
						pch = DoCononicalFormatInput(MakeData(i,&rgFields[ind],
						lpqt->szDataItem),rgFields[ind].wSQLType,AUTO_MAKE_TABLE);
					}

			if(!*pch) /* current type is REAL_ONLY) */
				continue;

			w++;
			// for every nullable type, that field will be set to
			// null when the row number corresponds with the field
			// number

			if(*pch)
			{
				lstrcpy(&lpParams->pbs[w][0], pch);
				lpParams->cbValue[w] = SQL_NTS;
			}
			else
			{
				lpParams->pbs[w][0] = chEOS;
				lpParams->cbValue[w] = SQL_NULL_DATA;
			}

			if(fColNames)
			{
				// the first time through, the insert
				//statement itself is created
				lstrcat(lpqt->szColNames, rgFields[ind].szFieldName);
				GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsComma);
				lstrcat(lpqt->szColNames, buf);
				GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsParamComma);
				lstrcat(lpqt->szValues, buf);

				// and parameters are set set all the
				// parameters using pointers to buffers
				// in the param struct.

				rc=SQLBindParameter(*pHstmt0, w,
								SQL_PARAM_INPUT,SQL_C_TCHAR, rgFields[ind].wSQLType,
								rgFields[ind].precision,
                                (SQLSMALLINT)rgFields[ind].scale,
								&lpParams->pbs[w][0],
								MAX_CREATED_DATA_LEN,
								&lpParams->cbValue[w]);
				if (RC_NOTSUCCESSFUL(rc))
				{
					ToolOptionsAllErrors(lpActiveConnWin,
												NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
					goto CreateInsertCleanup;
				}

			}
		}
		if(fColNames)
			{
			// the first time through, the insert
			//statement needs to be SQLPrepare'd
			// remove the ", " at the end of the string
			lpqt->szColNames[lstrlen(lpqt->szColNames) - 2] = chEOS;
			lpqt->szValues[lstrlen(lpqt->szValues) - 1] = chEOS;

			GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsInsertWCols);
			wsprintf(lpqt->sz, buf,lpqt->szTableName,lpqt->szColNames,
				lpqt->szValues);


			szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
					lpqt->sz);
			szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
						szNEWLINE);

			if (RC_NOTSUCCESSFUL(SQLPrepare(*pHstmt0, lpqt->sz, SQL_NTS)))
			{
				ToolOptionsAllErrors(lpActiveConnWin,
											NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
				goto CreateInsertCleanup;
			}

			fColNames = FALSE; /* no more first times through */
		}

		/* this inserts a row */
		if (RC_NOTSUCCESSFUL(SQLExecute(*pHstmt0)))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto CreateInsertCleanup;
		}

		SQLRowCount(*pHstmt0, &sdw);

		if (RC_NOTSUCCESSFUL(SQLFreeStmt(*pHstmt0, SQL_CLOSE)))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,NULL,*pHstmt0,NULL,SQL_ERROR);
			goto CreateInsertCleanup;
		}
	}

CreateInsertCleanup:

	FREE_PTR(lpParams);

	return(fError);
}

LPTSTR INTFUN BuildCreateStatement(lpQTSTRUCT lpqt,lpFIELDINFO rgFields,int cTypes)
{
	TCHAR 	buf[MEDBUFF];
	int	i;
	TCHAR	szParm[LARGEBUFF],
			*pComma=NULL;

	lstrcpy(lpqt->sz,szEOS);
	GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsCreateTable);

	/* "create table %s (%s)" - remove "(%s)" */
	buf[lstrlen(buf) - 4] = chEOS;
	wsprintf(lpqt->sz, buf, lpqt->szTableName);
	GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsOpenParen);
	lstrcat(lpqt->sz, buf);
	for(i = 0; i < cTypes; i++)
		{
		CreateParams(rgFields,i,lpqt->szParam);
		CreateColumnName(rgFields,i,TEXT("C"),NULL);

		//create parameter
		lstrcpy(szParm,rgFields[i].szFieldName);
		lstrcat(szParm, TEXT(" "));
		lstrcat(szParm, lpqt->szParam);
		lstrcat(szParm, TEXT(","));
		lstrcat(lpqt->sz, szParm);

		} //for(i = 0; i < cTypes; i++)

	/* remove the last comma and space */
	if (pComma=_tcsrchr(lpqt->sz,TEXT(',')))
		*pComma=chEOS;

	GetGtrString(buf,sizeof(buf)/sizeof(buf[0]), idsCloseParen);
	lstrcat(lpqt->sz, buf);

	return(lpqt->sz);

}

int INTFUN MakeTable(HDBC *pHdbc,LPTSTR  szCreatedTable, FIELDINFO  *rgFields,
	UINT uSeed,int *pcTypes)
{
	HSTMT			hstmt0=NULL;
	int			indexcol;
	SQLLEN		sdw=0;
	BOOL			fTableExists = FALSE,
					fError=FALSE;
	UWORD			cTableName;
	lpQTSTRUCT 	lpqt=NULL;
	TCHAR			szWidth[6];
	TCHAR			szPrecisionScale[16];
	TCHAR			szColName[SMALLBUFF]=TEXT("");
	UINT			uIdentity=0;
	UWORD			i;
	RETCODE		rc=SQL_SUCCESS;

	GetGtrString(szWidth,sizeof(szWidth)/sizeof(szWidth[0]), idsWidth);
	GetGtrString(szPrecisionScale,sizeof(szPrecisionScale)/sizeof(szPrecisionScale[0]), idsPrecisionScale);

	lpqt=(QTSTRUCT  *)AllocateMemory(sizeof (QTSTRUCT));

	if (RC_NOTSUCCESSFUL(SQLAllocStmt(*pHdbc, &hstmt0)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,*pHdbc,NULL,NULL,SQL_ERROR);
		goto MakeTableCleanup;
	}

	//if rgFields has data then we don't need to get Field info
	if (!*pcTypes)
	{

		if (!(*pcTypes=GetTypeInfo(pHdbc,&hstmt0,rgFields,uSeed)))
			goto MakeTableCleanup;

		rc=SQLGetInfo(*pHdbc, SQL_MAX_TABLE_NAME_LEN, &cTableName,
			sizeof(cTableName), NULL);
		if (RC_NOTSUCCESSFUL(rc))
		{
			ToolOptionsAllErrors(lpActiveConnWin,
										NULL,*pHdbc,NULL,NULL,SQL_ERROR);
			goto MakeTableCleanup;
		}

		/* the first field in the table should be a numeric field */
		indexcol = GetNumericColumn(rgFields,*pcTypes);

		if(indexcol > -1)
		{
			FIELDINFO tmp;
			int iFieldTmp;

			memcpy(&tmp, &rgFields[indexcol], sizeof(FIELDINFO));
			memcpy(&rgFields[indexcol], &rgFields[0], sizeof(FIELDINFO));
			memcpy(&rgFields[0], &tmp, sizeof(FIELDINFO));
			iFieldTmp = rgFields[indexcol].iFieldNum;
			rgFields[indexcol].iFieldNum = rgFields[0].iFieldNum;
			rgFields[0].iFieldNum = iFieldTmp;
		}

	} //if (lstrcmp(rgFields[0].szType,szEOS))

	GetTableName(pHdbc, lpqt->szTableName);

   //build create statement
	// the column names will be ctypname (where typename is the
	//	type name returned by the source in SQLGetTypeInfo)

	if (!BuildCreateStatement(lpqt, rgFields,*pcTypes))
		goto MakeTableCleanup;

	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
				lpqt->sz);
	szPrintf(lpUsrOptions, GETOUTPUTWINDOW(lpActiveConnWin), MSG_SEV1, TRUE, TRUE,
				szNEWLINE);

	if (RC_NOTSUCCESSFUL(SQLExecDirect(hstmt0, lpqt->sz, SQL_NTS)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,NULL,hstmt0,NULL,SQL_ERROR);
		goto MakeTableCleanup;
	}

	fTableExists=TRUE;

	// create a unique index, because some drivers require it, create it on the first
	// field which was made a NUMERIC field earlier
 	// call with NOFORCE because the driver may or may not support indexes.  Unless
 	// the appropriate GetInfo option is verified it can't be assumed to work
	MakeUniqueIndex(pHdbc, hstmt0, lpqt->szTableName,
		rgFields[0].szFieldName,NOFORCE);

	// THIS IS A HACK BECAUSE NULL=0 ON DBASE, WHICH WOULD LOOK LIKE A DUPLICATE
   	//VALUE, SO TO AVOID THIS PROBLEM WE DISALLOW NULLS ON THE INDEXED COLUMN
	rgFields[0].UseNulls = SQL_NO_NULLS;

	if (RC_NOTSUCCESSFUL(SQLFreeStmt(hstmt0, SQL_CLOSE)))
	{
		ToolOptionsAllErrors(lpActiveConnWin,
									NULL,NULL,hstmt0,NULL,SQL_ERROR);
		goto MakeTableCleanup;
	}

	// look for columns the test should not try to update by running
	//	a select * query and calling ColAttributes on the result fields

	if(DoBasicSelect(hstmt0, rgFields, lpqt->szTableName, *pcTypes))
		goto MakeTableCleanup;

	for(i = 1; i <= (UWORD)*pcTypes; i++)
		{
		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_UPDATABLE, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].fAutoUpdate = sdw == SQL_ATTR_READONLY;

		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_SEARCHABLE, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].fSearchable = sdw;

		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_UNSIGNED, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].fUnsigned = sdw;

		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_PRECISION, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].precision = sdw;


		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_SCALE, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].scale = (SWORD)sdw;

		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_LENGTH, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].length = sdw;

		FSSQLColAttributes(hstmt0, i, SQL_COLUMN_NULLABLE, NULL,
			0, NULL, &sdw);

		rgFields[i - 1].length = sdw;



	}

	FSSQLFreeStmt(hstmt0, SQL_CLOSE);

	CreateInsert(&hstmt0,rgFields,lpqt,*pcTypes);

	// reset nullable for the index column

	FSSQLFreeStmt(hstmt0, SQL_DROP);

	lstrcpy(szCreatedTable, lpqt->szTableName);

MakeTableCleanup:

	FREE_PTR(lpqt);

	return *pcTypes;
}



BOOL INTFUN GetSQLTypeInfo(HSTMT  *pHstmt0,FIELDINFO  *pField,
	SWORD wSQLType)
{
	SQLLEN	sdw;
	TCHAR	*pch=NULL,
			szNum[30];

	if(FSSQLGetTypeInfo(*pHstmt0, wSQLType))
		return(FALSE);

	if (RC_NOTSUCCESSFUL(SQLFetch(*pHstmt0)))
		return(FALSE);

	*pField->szType	 =
	*pField->szLength =
	*pField->szPrefix =
	*pField->szSuffix =
	*pField->szParams = chEOS;

	/* get type name */
	if (FSSQLGetData(*pHstmt0,1,SQL_C_TCHAR,pField->szType,
		sizeof(pField->szType),&sdw))
		return(FALSE);

	/* get the SQLType */
	if(FSSQLGetData(*pHstmt0, 2, SQL_C_DEFAULT,(SWORD  *)&pField->wSQLType,
		sizeof(pField->wSQLType), &sdw))
		return(FALSE);

	/* Max length */
	if(FSSQLGetData(*pHstmt0, 3, SQL_C_TCHAR,pField->szLength,
		sizeof(pField->szLength), &sdw))
		return(FALSE);

	/* if the length is over 10,000 -> divide it by 10 */
	if(sdw>=5)
		(pField->szLength[4]=chEOS);

	// don't actually use literal prefix or suffix,
	//	using Prepare/ Execute with params

	/* literal prefix */
	if(FSSQLGetData(*pHstmt0, 4, SQL_C_TCHAR, pField->szPrefix,
		sizeof(pField->szPrefix),&sdw))
		return(FALSE);

	/* literal suffix */
	if(FSSQLGetData(*pHstmt0, 5, SQL_C_TCHAR,pField->szSuffix,
		sizeof(pField->szSuffix), &sdw))
		return(FALSE);

	/* create params */
	if(FSSQLGetData(*pHstmt0, 6, SQL_C_TCHAR,pField->szParams,
		sizeof(pField->szParams), &sdw))
		return(FALSE);

	// nullable
	if(FSSQLGetData(*pHstmt0, 7, SQL_C_USHORT, (SWORD  *)&(pField->nullable),
		sizeof(pField->nullable), &sdw))
		return(FALSE);

	/* searchable */
	if(FSSQLGetData(*pHstmt0, 9, SQL_C_TCHAR, szNum,
		sizeof(szNum), &sdw))
		return(FALSE);

	/* auto increment */
	if(FSSQLGetData(*pHstmt0, 12, SQL_C_SHORT, (UWORD  *)&(pField->fAutoInc),
		sizeof(pField->fAutoInc), &sdw))
		return(FALSE);

	pField->fSearchable = _ttoi(szNum);

	FSSQLFreeStmt(*pHstmt0,SQL_CLOSE);

	return(TRUE);

} //GetSQLTypeInfo()



LPTSTR INTFUN MakeTypeData(HSTMT  *pHstmt,int nRow,SWORD wSQLType,
	LPTSTR szData,lpFIELDINFO rgFields,int cTypes)
{
	int i;

	//clear out szData
	lstrcpy(szData,szEOS);

	//Find info on wSQLType
	for (i=0;i < cTypes;i++)
		{
		if (rgFields[i].wSQLType==wSQLType)
			break;
		}

	//Make Data
	if (i < cTypes)
		DoCononicalFormatInput(MakeData(nRow,&rgFields[i],szData),
			wSQLType,FALSE);

	if (!lstrcmp(szData,szEOS))
		lstrcpy(szData,TEXT(""));

	return(szData);

} //MakeTypeData()

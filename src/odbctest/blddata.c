//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	BLDDATA.C
//|
//| Purpose:
//|		Contains tools used for building and validating parameters
//|
//*---------------------------------------------------------------------------------
#include <ctype.h>
#include "fhheader.h"
#include "blddata.h"
#include "bigint.h"
#pragma warning(disable:4996)
#define MAXPOSEXPFLOAT					3.40001E+38
#define MAXNEGEXPFLOAT					3.39999E-38
#define MAXPOSEXPDOUBLE					1.7000000000001E+308
#define MAXNEGEXPDOUBLE					1.6999999999999E-308
#define MAX_TIMESTAMP_FRAC_NUM		999999999
#define UCHAR_BYTE_COUNT				2
#define MAX_NUM_PRECISION				38

// The following macros abstract the limits so they'll work on any system
//		Note that a unary minus on NT causes a cast to 4 bytes and truncation occurs,
//		therefore you must do a * -1 to work for the MINSWORD case.
#define MAXUCHAR							((UCHAR)(-1))
#define MAXUTINYINT						((UTINYINT)(-1))
#define MAXSTINYINT						((UTINYINT)(MAXUTINYINT / 2))
#define MINSTINYINT						((-1 * (STINYINT)(MAXUTINYINT / 2)) - 1)
#define MAXUWORD							((UWORD)(-1))
#define MAXUDWORD 						((UDWORD)(-1))
#define MINSDWORD							(-(SDWORD)((MAXUDWORD / 2) + 1))
#define MAXSDWORD							((UDWORD)(MAXUDWORD / 2))
#define MAXSWORD 							((UWORD)(MAXUWORD / 2))
#ifdef WIN32
#define MINSWORD 							((SWORD)(((MAXUWORD / 2) + 1) * -1))
#define __huge
#else
#define MINSWORD							(-(SWORD)((MAXUWORD / 2) + 1))
#endif

// CHKBYTE is the value used in initialization for buffer checking
#define CHKBYTE '\xCA'


//----------------------------------------------------------------------------------
//		Globals
//----------------------------------------------------------------------------------
VszFile;
extern HINSTANCE 			hInst;
extern HWND					hwndClient;
extern HWND					hwndStatBar;
extern TCHAR 				szOutBuff[MAXOUTBUFF];
extern lpUSROPTIONS		lpUsrOptions;
extern LPCTSTR 				szErrTitle;

dCSEG(TCHAR) szBIT[]									= TEXT("BIT");
dCSEG(TCHAR) szUCHAR[]								= TEXT("UCHAR");
dCSEG(TCHAR) szSCHAR[]								= TEXT("SCHAR");
dCSEG(TCHAR) szUTINYINT[]							= TEXT("UTINYINT");
dCSEG(TCHAR) szUWORD[]								= TEXT("UWORD");
dCSEG(TCHAR) szUDWORD[]								= TEXT("UDWORD");
dCSEG(TCHAR) szSTINYINT[]							= TEXT("STINYINT");
dCSEG(TCHAR) szSWORD[]								= TEXT("SWORD");
dCSEG(TCHAR) szSDWORD[]								= TEXT("SDWORD");
dCSEG(TCHAR) szSFLOAT[]								= TEXT("SFLOAT");
dCSEG(TCHAR) szSDOUBLE[]							= TEXT("SDOUBLE");
dCSEG(TCHAR) szUWORDRange[]						= TEXT("%u to %u");
dCSEG(TCHAR) szSWORDRange[]						= TEXT("%d to %d");
dCSEG(TCHAR) szUDWORDRange[]						= TEXT("%lu to %lu");
dCSEG(TCHAR) szSDWORDRange[]						= TEXT("%ld to %ld");
dCSEG(TCHAR) szSFLOATRange[]						= TEXT("-3.4E+38 to -3.4E-38, 0, and 3.4E-38 to 3.4E+38");
dCSEG(TCHAR) szSDOUBLERange[]						= TEXT("-1.7E+308 to -1.7E-308, 0, and 1.7E-308 to 1.7E+308");
dCSEG(TCHAR) szDATETIMEDELIM[] 					= TEXT("-:. ");
dCSEG(TCHAR) szDATEDELIM[]							= TEXT("-");
dCSEG(TCHAR) szTIMEDELIM[]							= TEXT(":");
dCSEG(TCHAR) szTIMEFRACDELIM[]					= TEXT(".");
dCSEG(TCHAR) szSPACE[]								= TEXT(" ");
LPTSTR		 szInvalidParm							= TEXT("Invalid Parameter");
dCSEG(TCHAR) szInvalidCType[]						= TEXT("Invalid fCType %d");
dCSEG(TCHAR) szUnsupportedCType[]				= TEXT("Unsupported (possibly non-integer) fCType %d");
dCSEG(TCHAR) szUnsupportedFloat[]				= TEXT("Unsupported (possibly non-floating point) fCType %d");
dCSEG(TCHAR) szInvalidDlgType[]					= TEXT("Invalid DLG_xxx type %d");

//----------------------------------------------------------------------------------
//		Local Function Prototypes
//----------------------------------------------------------------------------------
BOOL INTFUN StoreFromCBDftArray(UDWORD udex, lpPARAMITEM lpParm, SWORD fCType);
BOOL INTFUN InitToCBODft(lpPARAMITEM lpParm, SWORD sVal);
BOOL INTFUN StoreCBDataFromType(HWND hwnd, lpPARAMITEM lpParm, SWORD fCType, SDWORD dwLen);

_inline BYTE ConvertHexCharToBinary (LPTSTR str);
WORD ConvertHexCharToWord (LPTSTR str);
DWORD ConvertHexCharToDWord (LPTSTR str);

// Convert the data from hex to char and place in lpData
VOID INTFUN ConvertFromHex(LPBYTE lpChr,LPBYTE lpData,LPTSTR lpTemp)
{
	lpChr = (LPBYTE)lpData;

   while(*lpTemp)
	{
		*lpChr=ConvertHexCharToBinary(lpTemp);
      lpChr++;
      lpTemp +=2;
	}
}


//*---------------------------------------------------------------------------------
//| IsValidNumeric:
//|	This function will examine a string and return TRUE if it is a valid
//|	numeric value, FALSE if not.  Valid interger values may contain a leading
//|	sign, but most be comprised only of digits after that.  This function
//|	also verifies that the *count* of digits is in range.
//| Parms:
//|	fCType					The c type which indicates the format
//|	lptstr					The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidNumeric(SWORD fCType, LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	BOOL		fPos=TRUE;
	LPTSTR	str=lptstr;
	UWORD 	len = (UWORD)lstrlen(lptstr);
	UWORD		numDigits;
	TCHAR		szMax[20]=TEXT("");
	LPTSTR	szTypeName;									// Use for error message, give type, eg: UWORD
	TCHAR		szTypeRange[DFTMSGBUFF];				// Use for range error, give range

	// Make local copy for formatting purposes
	if(szFieldNameIn)									// 32-bit nonsense
		lstrcpy((LPTSTR)szFieldName, szFieldNameIn);

	// Must have a length and not contain too many digits
	if(!len)
		goto InvalidNum;

	// Check for optional leading sign and ignore
	if (IsSigned(str,&fPos))
	{
		++str;	// Start parsing after sign
		--len;	// Don't count sign
	}

	// Nothing but sign was specified
	if(!len || (len > MAX_NUM_PRECISION))
		goto InvalidNum;

	// Figure out the type information so that any errors out of range
	//		will have correct info
	numDigits = (UWORD)log10(MAXUTINYINT);
	szTypeName = (LPTSTR)szUCHAR;
	wsprintf((LPTSTR)szTypeRange, (LPTSTR)szUWORDRange, 0, MAXUTINYINT);

	// Negative number can't be in range
	if(!fPos)
		goto ValueOutOfRange;


	// Passed all checks
	return TRUE;

InvalidNum:
	if(fVocal)
	{
		TCHAR szBuff[MAXOUTBUFF];

		RemoveDlgFormat(szFieldName);

		GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidNumeric);
		wsprintf(szBuff,szOutBuff,szFieldName,MAX_NUM_PRECISION);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					szInvalidParm, szBuff);

	}

	return FALSE;

ValueOutOfRange:
	if(fVocal)
	{
		TCHAR szBuff[MAXOUTBUFF];

		RemoveDlgFormat(szFieldName);

		wsprintf(szBuff,GetGtrString(szOutBuff, MAXOUTBUFF, idsNumOutOfRange),
					szFieldName,szTypeName,	szTypeRange);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					szInvalidParm,szBuff);

		}
	return FALSE;
}



//*---------------------------------------------------------------------------------
//| ValidateTypeData:
//|	This function will look at the format of the data and make sure
//|	it is valid for the type given.
//| Parms:
//|	szFieldName				The name of the field we are checking
//|	fCType					The c type which indicates the format
//|	fVocal					TRUE if errors should be displayed
//|	LPTSTR						The data to examine in TCHARacter format
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN ValidateTypeData(LPTSTR szFieldName, SWORD fCType, BOOL fVocal, LPTSTR lptstr)
{
	if (!lptstr)
		return(FALSE);

	// Check for a hex string
	// Bug #470 Check for valid LPTSTR before using
	if(lptstr)
	{
		if (IsHexString(lptstr))
			return IsValidHex(fCType, lptstr, fVocal, szFieldName, NULL);
	}

	// Value is not a hex string, check the value based on the type
	switch(fCType)
	{
		// Always a valid string
		case SQL_C_CHAR:
		case SQL_C_WCHAR:
		case SQL_C_BINARY:
			return TRUE;

		// Must be a valid UCHAR
		case SQL_C_BIT:
			return IsValidBit(fCType, lptstr, fVocal, szFieldName);

		// Must be a valid number and in correct range
		case SQL_C_TINYINT:
		case SQL_C_UTINYINT:
		case SQL_C_STINYINT:
		case SQL_C_SHORT:
		case SQL_C_USHORT:
		case SQL_C_SSHORT:
		case SQL_C_LONG:
		case SQL_C_ULONG:
		case SQL_C_SLONG:
			return IsValidInt(fCType, lptstr, fVocal, szFieldName);
		case SQL_C_NUMERIC:
			return IsValidNumeric(fCType, lptstr, fVocal, szFieldName);

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
			return IsValidInterval(fCType, lptstr, fVocal, szFieldName);

		case SQL_C_UBIGINT:
			return IsValidBigInt(lptstr,lstrlen(lptstr),4,TRUE,szFieldName);
		case SQL_C_SBIGINT:
			return IsValidBigInt(lptstr,lstrlen(lptstr),4,FALSE,szFieldName);
		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
			return IsValidDate(lptstr, fVocal, szFieldName);

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
			return IsValidTime(lptstr, fVocal, szFieldName, FALSE);

		case SQL_C_TIMESTAMP:
		case SQL_C_TYPE_TIMESTAMP:
			return IsValidTimestamp(lptstr, fVocal, szFieldName);

		case SQL_C_FLOAT:
		case SQL_C_DOUBLE:
			return IsValidFloat(fCType, lptstr, fVocal, szFieldName);

		default:
			ONDEBUG(wsprintf(szOutBuff, szInvalidCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| IsValidHex:
//|	This function will examine a string and return TRUE if it is a valid
//|	hexidecimal value for the data type given, FALSE if not.
//| Parms:
//|	fCType					The c type which indicates the format
//|	lptstr						The data to examine in character format with the '0x'
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//|	lpBytes					Pointer to a buffer for the size of CHAR or BINARY data
//| Returns:
//|	TRUE if the string given will work for the type.  If the type is SQL_C_CHAR or
//|		SQL_C_BINARY, the size (in bytes) of the string (without spaces) is
//|		returned in lpBytes.
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidHex(SWORD fCType, LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn, UDWORD *lpBytes)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	LPTSTR	str=lptstr+2;
	UDWORD	cChars=0;
	UDWORD	cbChar=sizeof(TCHAR),
				cbCnt=UCHAR_BYTE_COUNT*cbChar;

	// Set the return size to 0 in case we error before we set it
	if(lpBytes)
		*lpBytes = 0;

	// Make local copy for formatting purposes
	if(szFieldNameIn)
		lstrcpy(szFieldName, szFieldNameIn);

    if (lptstr[0] == TEXT('\0') ||
        lptstr[1] == TEXT('\0') ||
        lptstr[2] == TEXT('\0')) {
        goto InvalidCharacter;
        return FALSE;
    }

	// First convert all characters in the string to upper case
	// and check that they are in the valid range (0-9 and A-F) or a blank space

	while(*str)
	{
		*str=(TCHAR)_totupper(*str);
		if (_istspace(*str))
			++str;                				// Increment to the next letter in string
		else if	(_istxdigit(*str))
		{
			++str;			                     // Increment in the next letter in the string
			++cChars;											// Increment the count of valid characters
		}
		else if (*str == TEXT('"'))			//Descriptor handles have a description tacked on
														//but still a valid Hex String.
			break;
		else
			goto InvalidCharacter;
		}

	// Now validate the byte count for the given data type
	str=lptstr+2;
	if(str)
	{
		switch(fCType)
		{
			case SQL_C_CHAR:
			case SQL_C_WCHAR:
			case SQL_C_BINARY:                  // Number of characters must be divisible by two
				if(lpBytes)
					*lpBytes = cChars / cbCnt;			// Store the number of valid characters

				if(cChars % cbCnt)
				{
					if(lpBytes)
						(lpBytes)++;					// Increment by 1 for the extra half-byte
					goto InvalidValueForType;
				}
				break;

			case SQL_C_BIT:
			case SQL_C_TINYINT:
			case SQL_C_UTINYINT:
			case SQL_C_STINYINT:              	// Number of characters must be less than the size of a char
				if(cChars > (2 * sizeof(UCHAR)))
	   				goto InvalidValueForType;
				break;

			case SQL_C_SHORT:
			case SQL_C_USHORT:
			case SQL_C_SSHORT:
				if(cChars > 2 * sizeof(SWORD))	// Number of characters must be less than the size of an int
	   				goto InvalidValueForType;
				break;

			case SQL_C_LONG:
			case SQL_C_ULONG:
			case SQL_C_SLONG:                // Number of characters must be less that the size of a long int
				if(cChars > 2 * sizeof(SDWORD))
	   				goto InvalidValueForType;
				break;

			case SQL_C_FLOAT:                // Number of characters must be exactly the size of a float
				if(cChars != 2 * sizeof(SFLOAT))
	   				goto InvalidValueForType;
				break;

			case SQL_C_DOUBLE:               // Number of characters must be exactly the size of a double
				if(cChars != 2 * sizeof(SDOUBLE))
	   				goto InvalidValueForType;
				break;

			case SQL_C_DATE:						// Number of characters must be exactly the size of the DATE struct
			case SQL_C_TYPE_DATE:
				if(cChars != 2 * (sizeof(SWORD) + (2 * sizeof(UWORD))))
	   				goto InvalidValueForType;
				break;

			case SQL_C_TIME:						// Number of characters must be exactly the size of the TIME struct
			case SQL_C_TYPE_TIME:
				if(cChars != 2 * (3 * sizeof(UWORD)))
	   				goto InvalidValueForType;
				break;

			case SQL_C_TIMESTAMP:				// Number of characters must be exactly the size of a char
			case SQL_C_TYPE_TIMESTAMP:
				if(!((cChars == 2 * (sizeof(SWORD) + (5 * sizeof(UWORD)) + sizeof(UDWORD)))
	   				|| (cChars == 2 * (sizeof(SWORD) + (5 * sizeof(UWORD))))))
	   				goto InvalidValueForType;
	  				break;

				default:
					ONDEBUG(wsprintf(szOutBuff, szInvalidCType, fCType));
					ASSERTCHECK(TRUE, szOutBuff);
					break;

	   	} //switch(fCType)
		}

		return TRUE;

InvalidCharacter:
	if(fVocal)
	{
		RemoveDlgFormat(szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidHexChar),
					szFieldName);
	}

	return FALSE;

InvalidValueForType:
	if(fVocal)
	{
		RemoveDlgFormat(szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidHexValue),
					szFieldName);
	}

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| IsValidInt:
//|	This function will examine a string and return TRUE if it is a valid
//|	integer value, FALSE if not.  Valid interger values may contain a leading
//|	sign, but most be comprised only of digits after that.  This function
//|	also verifies that the *count* of digits is in range.
//| Parms:
//|	fCType					The c type which indicates the format
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidInt(SWORD fCType, LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	BOOL		fPos=TRUE;
	LPTSTR	str=lptstr;
	UWORD 	len = (UWORD)lstrlen(lptstr);
	UWORD		numDigits;
	TCHAR		szMax[20];
	LPTSTR	szTypeName;									// Use for error message, give type, eg: UWORD
	TCHAR		szTypeRange[DFTMSGBUFF];				// Use for range error, give range

	// Make local copy for formatting purposes
	if(szFieldNameIn)									// 32-bit nonsense
		lstrcpy((LPTSTR)szFieldName, szFieldNameIn);

	// Must have a length and not contain too many digits
	if(!len)
		goto InvalidInt;

	// Check for optional leading sign and ignore
	if (IsSigned(str,&fPos))
	{
		++str;								// Start parsing after sign
		--len;										// Don't count sign
	}

	if(!len)											// Nothing but sign was specified
		goto InvalidInt;

	// Figure out the type information so that any errors out of range
	//		will have correct info
	switch(fCType) {
		case SQL_C_TINYINT:
		case SQL_C_UTINYINT:
			numDigits = (UWORD)log10(MAXUTINYINT);
			szTypeName = (LPTSTR)szUCHAR;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szUWORDRange, 0, MAXUTINYINT);
			if(!fPos)						// Negative number can't be in range
				goto ValueOutOfRange;
			break;

		case SQL_C_STINYINT:
			numDigits = (UWORD)log10(MAXUTINYINT);
			szTypeName = (LPTSTR)szSCHAR;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szSWORDRange, MINSTINYINT, MAXSTINYINT);
			break;

		case SQL_C_USHORT:
			numDigits = (UWORD)log10(MAXUWORD);
			szTypeName = (LPTSTR)szUWORD;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szUWORDRange, 0, MAXUWORD);
			if(!fPos)						// Negative number can't be in range
				goto ValueOutOfRange;
			break;

		case SQL_C_SHORT:
		case SQL_C_SSHORT:
			numDigits = (UWORD)log10(MAXUWORD);
			szTypeName = (LPTSTR)szSWORD;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szSWORDRange, MINSWORD, MAXSWORD);
			break;

		case SQL_C_ULONG:
			numDigits = (UWORD)log10(MAXUDWORD);
			szTypeName = (LPTSTR)szUDWORD;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szUDWORDRange, 0L, MAXUDWORD);
			if(!fPos)						// Negative number can't be in range
				goto ValueOutOfRange;
			break;

		case SQL_C_LONG:
		case SQL_C_SLONG:
			numDigits = (UWORD)log10(MAXUDWORD);
			szTypeName = (LPTSTR)szSDWORD;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szSDWORDRange, MINSDWORD, MAXSDWORD);
			break;

		default:
			ONDEBUG(wsprintf(szOutBuff, szUnsupportedCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

	// Each remaining character must be a valid digit
	while(*str)
	{
		if (!_istdigit(*str))
			goto InvalidInt;

		++str;		// Increment to the next letter in string

	}

	// It's all valid digits (optional sign).  Two obvious checks,
	//		if there are too many digits, reject, if there are fewer
	//		than the max, accept it
	++numDigits;
	if(len > numDigits)
		goto ValueOutOfRange;
	else if(len < numDigits)
		return TRUE;

	// Check for negative numbers when unsigned expected, too many digits,
	//		as well as actual range
	switch(fCType) {
		case SQL_C_TINYINT:
		case SQL_C_UTINYINT:
			wsprintf((LPTSTR)szMax, TEXT("%u"), MAXUTINYINT);
			break;

		case SQL_C_STINYINT:
			if(fPos)
				wsprintf((LPTSTR)szMax, TEXT("%u"), MAXSTINYINT);
			else
				wsprintf((LPTSTR)szMax, TEXT("%d"), MINSTINYINT);
			break;

		case SQL_C_USHORT:
			wsprintf((LPTSTR)szMax, TEXT("%u"), MAXUWORD);
			break;

		case SQL_C_SHORT:
		case SQL_C_SSHORT:
			if(fPos)
				wsprintf((LPTSTR)szMax, TEXT("%u"), MAXSWORD);
			else
				wsprintf((LPTSTR)szMax, TEXT("%d"), MINSWORD);
			break;

		case SQL_C_ULONG:
			wsprintf((LPTSTR)szMax, TEXT("%lu"), MAXUDWORD);
			break;

		case SQL_C_LONG:
		case SQL_C_SLONG:
			if(fPos)
				wsprintf((LPTSTR)szMax, TEXT("%lu"), MAXSDWORD);
			else
				wsprintf((LPTSTR)szMax, TEXT("%ld"), MINSDWORD);
			break;

		default:
			ONDEBUG(wsprintf(szOutBuff, szUnsupportedCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

	if(lstrcmp((LPTSTR)szMax, lptstr) < 0)
		goto ValueOutOfRange;

	// Passed all checks
	return TRUE;

InvalidInt:
	if(fVocal)
	{
		TCHAR szBuff[MAXOUTBUFF];

		RemoveDlgFormat(szFieldName);

		GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidInt);
		wsprintf(szBuff,szOutBuff,szFieldName);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					szInvalidParm, szBuff);

	}

	return FALSE;

ValueOutOfRange:
	if(fVocal)
	{
		TCHAR szBuff[MAXOUTBUFF];

		RemoveDlgFormat(szFieldName);

		wsprintf(szBuff,GetGtrString(szOutBuff, MAXOUTBUFF, idsIntOutOfRange),
					szFieldName,szTypeName,	szTypeRange);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					szInvalidParm,szBuff);

		}
	return FALSE;
}



//*---------------------------------------------------------------------------------
//| IsValidFraction:
//|	This function will examine a string and return TRUE if it is a valid
//|	fraction value, FALSE if not.
//| Parms:
//|	szFrac					The data to examine in character format
//|	szFieldName				Name of the field for errors
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------

BOOL INTFUN IsValidFraction(LPTSTR szFrac,LPTSTR szFieldNameIn)
{
	BOOL		fValid=FALSE;
	ULONG		ulTmp=0;

	// If fractional part is > than 10 characters
	// Then truncated it to 9 digits
	if(lstrlen(szFrac) >= 10)
		lstrcpy((LPTSTR)(szFrac + 9),TEXT(""));

	if(IsValidInt(SQL_C_ULONG, szFrac, FALSE, szFieldNameIn))
	{
		// Validate fractional portion of time
		ulTmp = _tcstoul(szFrac, NULL, 10);
		fValid = ulTmp <= MAX_TIMESTAMP_FRAC_NUM;
	}

	return(fValid);

} //IsValidFraction()


//*---------------------------------------------------------------------------------
//| IsValidSeconds:
//|	This function will examine a string and return TRUE if it is a valid
//|	seconds value, FALSE if not.
//| Parms:
//|	lptstr					The data to examine in character format
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//|	fFrac						TRUE if fractional part is acceptable, FALSE if it is not
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------

BOOL INTFUN IsValidSeconds(LPTSTR szSecs, LPTSTR szFieldNameIn, BOOL fFrac)
{
	BOOL		fValid=FALSE;
	LPTSTR	token=NULL;

	if (token = _tcstok(szSecs, szTIMEFRACDELIM))
	{
		// Validate seconds portion of time
		if(IsValidInt(SQL_C_USHORT, token, FALSE, szFieldNameIn))
		{
			if(token = _tcstok(NULL, szTIMEFRACDELIM))
			{
				if(!fFrac)// Fractional part not acceptable
					return(FALSE);

				fValid=IsValidFraction(token,szFieldNameIn);
			}
			else
				fValid=TRUE;
		}
	}

	return(fValid);

} //IsValidSeconds()

//*---------------------------------------------------------------------------------
//| ValidateIntervalFields:
//|	This function will validate a single integer value
//| Parms:
//|	szValue				the Interval's 'value' field
//|	szFieldName			Name of the field for errors
//|	szSeps				value separator (i.e '-', ':',etc.)
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------

BOOL INTFUN ValidateIntervalFields(LPTSTR szValue,LPTSTR szFieldName,LPTSTR szSeps,int nCount)
{
	LPTSTR	pTok;
	BOOL		fValid=TRUE;

    if (szValue == NULL && nCount > 0) {
        return FALSE;
    }

    pTok=_tcstok(szValue,szSeps);

	//loop through all inteval values
	while (pTok && fValid)
	{
        nCount--;

		fValid=IsValidInt(SQL_C_SSHORT,pTok,FALSE,szFieldName);

		pTok=_tcstok(NULL,szSeps);

	} //end while

	return(fValid && nCount <= 0);

} //ValidateIntervalFields()




//*---------------------------------------------------------------------------------
//| IsValidInterval:
//|	This function will examine a string and return TRUE if it is a valid
//|	Interval value, FALSE if not.  Valid interger values may contain a leading
//|	sign, but most be comprised only of digits after that.  This function
//|	also verifies that the *count* of digits is in range.
//| Parms:
//|	fCType					The c type which indicates the format
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------

BOOL INTFUN IsValidInterval(SWORD fCType, LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	LPTSTR 	str = lptstr;
	LPTSTR 	token;
	UWORD 	len = (UWORD)lstrlen(lptstr);
	TCHAR		szSeps[SMALLBUFF]=TEXT("\'"),
				szQualifier[LARGEBUFF]=TEXT("");
	UWORD		nLen=(UWORD)lstrlen(TEXT("INTERVAL"));
	LPTSTR	pszQualifier=NULL,
				pszFrac=NULL;
	TCHAR		szFrac[SMALLBUFF];
    int nCount = 1;

	// Make local copy for formatting purposes
	if(szFieldNameIn)
		lstrcpy(szFieldName, szFieldNameIn);

	// Must have a length
	if(!len)
		goto InvalidInterval;

	// Parse interval
	if(token = _tcstok(str,szSeps))
	{
		//"INTERVAL" with optional sign must be first
		if (_tcsncicmp(token,TEXT("INTERVAL"),nLen))
			goto InvalidInterval;
		else
		{
			token+=nLen;
			EatWhiteSpace(token);

			//Next char is either '-','+',nothing
			switch(*token)
			{
				case TEXT('+'):
				case TEXT('-'):
				case TEXT('\0'):
					break;
				default:
					goto InvalidInterval;
			}

		}

		//next token should either be a sign or a single quote
		if (token = _tcstok(NULL,szSeps))
		{
			//Get <interval Qualifier>
			pszQualifier = _tcstok(NULL,szSeps);
			EatSpace(pszQualifier);

			switch(fCType)
			{
				case SQL_C_INTERVAL_YEAR:
					lstrcpy(szQualifier,TEXT("YEAR"));
					break;

				case SQL_C_INTERVAL_MONTH:
					lstrcpy(szQualifier,TEXT("MONTH"));
					break;

				case SQL_C_INTERVAL_DAY:
					lstrcpy(szQualifier,TEXT("DAY"));
					break;

				case SQL_C_INTERVAL_HOUR:
					lstrcpy(szQualifier,TEXT("HOUR"));
					break;

				case SQL_C_INTERVAL_MINUTE:
					lstrcpy(szQualifier,TEXT("MINUTE"));
					break;
				case SQL_C_INTERVAL_SECOND:
					lstrcpy(szQualifier,TEXT("SECOND"));
					//get fraction field
					if (pszFrac=_tcschr(token,TEXT('.')))
					{
						lstrcpy(szFrac,(LPTSTR)(pszFrac+1));
						*pszFrac=TEXT('\0');
					}
					break;
				case SQL_C_INTERVAL_YEAR_TO_MONTH:
					lstrcpy(szQualifier,TEXT("YEAR TO MONTH"));
					lstrcpy(szSeps,TEXT("-"));
                    nCount = 2;
					break;
				case SQL_C_INTERVAL_DAY_TO_HOUR:
					lstrcpy(szQualifier,TEXT("DAY TO HOUR"));
					lstrcpy(szSeps,TEXT(" "));
                    nCount = 2;
					break;
				case SQL_C_INTERVAL_DAY_TO_MINUTE:
					lstrcpy(szQualifier,TEXT("DAY TO MINUTE"));
					lstrcpy(szSeps,TEXT(":"));
					//Since there are 2 different separators for
					//this interval we need to validate the "DAY"
					//portion
					token = _tcstok(token,TEXT(" "));
					if (!IsValidInt(SQL_C_SSHORT,token,TRUE,szFieldNameIn))
						goto InvalidInterval;
					token = _tcstok(NULL,TEXT(" "));
                    nCount = 2;
					break;
				case SQL_C_INTERVAL_DAY_TO_SECOND:
					lstrcpy(szQualifier,TEXT("DAY TO SECOND"));
					lstrcpy(szSeps,TEXT(":"));
					//Since there are 2 different separators for
					//this interval we need to validate the "DAY"
					//portion
					token = _tcstok(token,TEXT(" "));
					if (!IsValidInt(SQL_C_SSHORT,token,TRUE,szFieldNameIn))
						goto InvalidInterval;
					token = _tcstok(NULL,TEXT(" "));
					//get fraction field
					if (token != NULL && (pszFrac=_tcschr(token,TEXT('.'))))
					{
						lstrcpy(szFrac,(LPTSTR)(pszFrac+1));
						*pszFrac=TEXT('\0');
					}
                    nCount = 3;
					break;
				case SQL_C_INTERVAL_HOUR_TO_MINUTE:
					lstrcpy(szQualifier,TEXT("HOUR TO MINUTE"));
					lstrcpy(szSeps,TEXT(":"));
                    nCount = 2;
					break;
				case SQL_C_INTERVAL_HOUR_TO_SECOND:
					lstrcpy(szQualifier,TEXT("HOUR TO SECOND"));
					lstrcpy(szSeps,TEXT(":"));
					//get fraction field
					if (pszFrac=_tcschr(token,TEXT('.')))
					{
						lstrcpy(szFrac,(LPTSTR)(pszFrac+1));
						*pszFrac=TEXT('\0');
					}
                    nCount = 3;
					break;
				case SQL_C_INTERVAL_MINUTE_TO_SECOND:
					lstrcpy(szQualifier,TEXT("MINUTE TO SECOND"));
					lstrcpy(szSeps,TEXT(":"));
					//get fraction field
					if (pszFrac=_tcschr(token,TEXT('.')))
					{
						lstrcpy(szFrac,(LPTSTR)(pszFrac+1));
						*pszFrac=TEXT('\0');
					}
                    nCount = 2;
					break;
				default:
					goto InvalidInterval;

			} //switch(fCType)

			if (!ValidateIntervalFields(token,szFieldNameIn,szSeps,nCount))
				goto InvalidInterval;

			if (pszFrac)
			{
				//Validate Second's Fraction
				if (!IsValidFraction(szFrac,szFieldNameIn))
					goto InvalidInterval;
			}

			//Validate <interval qualifier>
			if (!pszQualifier || lstrcmpi(pszQualifier,szQualifier))
				goto InvalidInterval;

		} //if (token = _tcstok(NULL,szSeps))

	} //if(token = _tcstok(str,szSeps))

	return(TRUE);

InvalidInterval:

	if(fVocal)
	{
		RemoveDlgFormat(szFieldName);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidInterval),
					(LPTSTR)szFieldName);
	}

	return FALSE;

} //IsValidInterval()



//*---------------------------------------------------------------------------------
//| IsValidBit:
//|	This function will verify that the string sent in is a valid SQL_C_BIT
//|	value.  This means it must be within the range of a UCHAR.
//| Parms:
//|	fCType					The c type which indicates the format
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidBit(SWORD fCType, LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	BOOL		fPos=TRUE;
	LPTSTR	str=lptstr;
	UWORD 	len = (UWORD)lstrlen(lptstr);
	UWORD		numDigits;
	TCHAR		szMax[20];
	LPTSTR	szTypeName;								// Use for error message, give type, eg: UWORD
	TCHAR		szTypeRange[DFTMSGBUFF];				// Use for range error, give range

	// Make local copy for formatting purposes
	if(szFieldNameIn)									// 32-bit nonsense
		lstrcpy((LPTSTR)szFieldName, (LPTSTR)szFieldNameIn);

	// Must have a length and not contain too many digits
	if(!len)
		goto InvalidBit;

	//do we have a '+' or '-' at the start of str
	if (IsSigned(str,&fPos))
	{
		++str;								// Start parsing after sign
		--len;										// Don't count sign
	}

	if(!len)											// Nothing but sign was specified
		goto InvalidBit;

	// Figure out the type information so that any errors out of range
	//		will have correct info
	switch(fCType)
	{
		case SQL_C_BIT:
			numDigits = (UWORD)log10(MAXUCHAR);
			szTypeName = (LPTSTR)szUCHAR;
			wsprintf((LPTSTR)szTypeRange, (LPTSTR)szUWORDRange, 0, MAXUCHAR);
			if(!fPos)						// Negative number can't be in range
				goto ValueOutOfRange;
			break;

		default:
			ONDEBUG(wsprintf(szOutBuff, szUnsupportedCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

	// Each remaining character must be a valid digit
	while(*str)
	{
		if (!_istdigit(*str))
			goto InvalidBit;

		++str;
	}

	// It's all valid digits (optional sign).  Two obvious checks,
	//		if there are too many digits, reject, if there are fewer
	//		than the max, accept it
	++numDigits;
	if(len > numDigits)
		goto ValueOutOfRange;
	else if(len < numDigits)
		return TRUE;

	// Check for negative numbers when unsigned expected, too many digits,
	//		as well as actual range
	switch(fCType) {
		case SQL_C_BIT:
			wsprintf((LPTSTR)szMax, TEXT("%u"), MAXUCHAR);
			if(lstrcmp((LPTSTR)szMax, lptstr) < 0)
				goto ValueOutOfRange;
			break;

		default:
			ONDEBUG(wsprintf(szOutBuff, szUnsupportedCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

	// Passed all checks
	return TRUE;

InvalidBit:
	if(fVocal) {
		RemoveDlgFormat((LPTSTR)szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsInvalidBit),
					(LPTSTR)szFieldName);
		}
	return FALSE;

ValueOutOfRange:
	if(fVocal) {
		RemoveDlgFormat((LPTSTR)szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsIntOutOfRange),
					(LPTSTR)szFieldName,
					(LPTSTR)szTypeName,
					(LPTSTR)szTypeRange);
		}
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| IsValidDate:
//|	This function will examine a string and return TRUE if it is a valid
//|	date value, FALSE if not.
//| Parms:
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidDate(LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	TCHAR		*str=(TCHAR *)lptstr;
	TCHAR		*token=NULL;
	UWORD 	len = (UWORD)lstrlen(lptstr);
	TCHAR		szBuf1[50],
				szBuf2[50];

	lstrcpy(szBuf1,szBuf2);

	// Make local copy for formatting purposes
	if(szFieldNameIn)										// 32-bit nonsense
		lstrcpy((LPTSTR)szFieldName, szFieldNameIn);

	// Must have a length
	if(!len)
		goto InvalidDate;

   // Parse date string into separate integers and validate each one
	if(token = _tcstok(str,szDATEDELIM))
	{
		// Validate year portion of date
	   if(IsValidInt(SQL_C_SSHORT, (LPTSTR)token, FALSE, (LPTSTR)szFieldNameIn))
		{
			if(token = _tcstok(NULL,(LPTSTR)szDATEDELIM))
			{
				if(IsValidInt(SQL_C_USHORT, token, FALSE, szFieldNameIn))
				{
					// Validate month portion of date
					str = token + (lstrlen(token) + 1);

					// Validate day portion of date
					if(IsValidInt(SQL_C_USHORT, str, FALSE, szFieldNameIn))
						return TRUE;
				}
			}
		}

	}

InvalidDate:
	if(fVocal){
		RemoveDlgFormat((LPTSTR)szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString((LPTSTR)szOutBuff, MAXOUTBUFF, idsInvalidDate),
					(LPTSTR)szFieldName);
		}
	return FALSE;
}

//*---------------------------------------------------------------------------------
//| IsValidTime:
//|	This function will examine a string and return TRUE if it is a valid
//|	time value, FALSE if not.
//| Parms:
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//|	fFrac						TRUE if fractional part is acceptable, FALSE if it is not
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidTime(LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn, BOOL fFrac)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	LPTSTR		str=lptstr;
	LPTSTR		token;
	UWORD 	len = (UWORD)lstrlen(lptstr);

	// Make local copy for formatting purposes
	if(szFieldNameIn)										// 32-bit nonsense
		lstrcpy((LPTSTR)szFieldName, szFieldNameIn);

	// Must have a length
	if(!len)
		goto InvalidTime;

   // Parse date string into separate integers and validate each one
	if(token = _tcstok(str,szTIMEDELIM))
	{
		// Validate hours portion of time
	   if(IsValidInt(SQL_C_USHORT, token, FALSE, szFieldNameIn))
		{
			if(token = _tcstok(NULL,szTIMEDELIM))
			{
				// Validate minutes portion of time
				if(IsValidInt(SQL_C_USHORT, token, FALSE, szFieldNameIn))
				{
					if(token = _tcstok(NULL, szTIMEFRACDELIM))
					{
						if (IsValidSeconds(token,szFieldNameIn,fFrac))
							return TRUE;
						else
							goto InvalidTime;

					}
				}
			}
		}
	}

InvalidTime:
	if(fVocal)
	{
		RemoveDlgFormat(szFieldName);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidTime),
					(LPTSTR)szFieldName);
	}

	return FALSE;
}


//*---------------------------------------------------------------------------------
//| IsValidTimestamp:
//|	This function will examine a string and return TRUE if it is a valid
//|	timestamp value, FALSE if not.
//| Parms:
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidTimestamp(LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	LPTSTR 	str = lptstr;
	LPTSTR 	token;
	UWORD 	len = (UWORD)lstrlen(lptstr);

	// Make local copy for formatting purposes
	if(szFieldNameIn)
		lstrcpy(szFieldName, szFieldNameIn);

	// Must have a length
	if(!len)
		goto InvalidTimestamp;

	// Parse timestamp into date and time and validate each portion separately
	if(token = _tcstok(str,szSPACE))
	{
		str = token + lstrlen(token) + 1;

		if(IsValidDate(token, FALSE, szFieldName))
		{
			if(IsValidTime(str, FALSE, szFieldName, TRUE))
				return TRUE;
		}
	}

InvalidTimestamp:
	if(fVocal)
	{
		RemoveDlgFormat(szFieldName);

		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidTimestamp),
					(LPTSTR)szFieldName);
	}
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| IsValidFloat:
//|	This function will examine a string and return TRUE if it is a valid
//|	floating-point value, FALSE if not.  Valid floating point values may contain a leading
//|	sign, but most be comprised only of digits after that.  This function
//|	also verifies that the *count* of digits is in range.
//| Parms:
//|	fCType					The c type which indicates the format
//|	lptstr						The data to examine in character format
//|	fVocal					TRUE if errors should be reported
//|	szFieldName				Name of the field for errors when fVocal==TRUE
//| Returns:
//|	TRUE if the string given will work for the type
//*---------------------------------------------------------------------------------
BOOL INTFUN IsValidFloat(SWORD fCType, LPTSTR lptstr, BOOL fVocal, LPTSTR szFieldNameIn)
{
	TCHAR		szFieldName[MAXEXPANDEDCONSTANT];
	LPTSTR		str=lptstr;
	UWORD 	len = (UWORD)lstrlen(lptstr);
	LPTSTR		lpEnd;
   double	dTemp;
   LPTSTR		szTypeName;
   LPTSTR		szTypeRange;

	// Make local copy for formatting purposes
	if(szFieldNameIn)										// 32-bit nonsense
		lstrcpy(szFieldName, szFieldNameIn);

	// Must have a length and not contain too many digits
	if(!len)
		goto InvalidFloat;

	// Convert the string to a long double and check for valid conversion
	dTemp = _tcstod(str, &lpEnd);

	if(!(lpEnd == str + len))				// Did not correctly convert entire string to floating point value
		goto InvalidFloat;

	// Now cast the value to the correct type and do range checking
	switch(fCType) {
		case SQL_C_FLOAT:
			if(((dTemp >= MAXNEGEXPFLOAT) && (dTemp <= MAXPOSEXPFLOAT)) ||		// In the valid range for a float
				((dTemp >= -MAXPOSEXPFLOAT) && (dTemp <= -MAXNEGEXPFLOAT)) ||
				(dTemp == 0))
				return TRUE;
			else{
				szTypeName = (LPTSTR)szSFLOAT;
				szTypeRange = (LPTSTR)szSFLOATRange;
				goto FloatOutOfRange;
				}

		case SQL_C_DOUBLE:
			if(((dTemp >= MAXNEGEXPDOUBLE) && (dTemp <= MAXPOSEXPDOUBLE)) ||		// In the valid range for a double
				((dTemp >= -MAXPOSEXPDOUBLE) && (dTemp <= -MAXNEGEXPDOUBLE)) ||
				(dTemp == 0))
				return TRUE;
			else{
				szTypeName = (LPTSTR)szSFLOAT;
				szTypeRange = (LPTSTR)szSDOUBLERange;
				goto FloatOutOfRange;
				}

		default:
			ONDEBUG(wsprintf(szOutBuff, szUnsupportedFloat, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

InvalidFloat:
	if(fVocal) {
		RemoveDlgFormat(szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsInvalidFloat),
					(LPTSTR)szFieldName);
		}
	return FALSE;

FloatOutOfRange:
	if(fVocal) {
		RemoveDlgFormat(szFieldName);
		szMessageBox(GetActiveWindow(),
					MB_ICONINFORMATION | MB_OK,
					(LPTSTR)szInvalidParm,
					GetGtrString(szOutBuff, MAXOUTBUFF, idsIntOutOfRange),
					(LPTSTR)szFieldName,
					szTypeName,
					szTypeRange);
		}
	return FALSE;
}

//*---------------------------------------------------------------------------------
//| EatWhiteSpace:
//|	This function will take a pointer to a string and remove all spaces from
//|	the string.
//|
//| Parms:
//|	LPTSTR				A pointer to the string
//| Returns:
//|	TRUE if successful, FALSE if not
//*---------------------------------------------------------------------------------
BOOL EatWhiteSpace(LPTSTR str)
{
	LPTSTR	lpTmp = str;

	while(*str)
	{
		if (_istspace(*str))
			str++;		      			// Increment past the white space
		else
			*lpTmp++ = *str++;			// Copy character to temporary string
	}

	//Null Terminate string
	*lpTmp=TEXT('\0');

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| EatSpace:
//|	This function will take a pointer to a string and remove all spaces from
//|	the string the beginning and end of the string
//|
//| Parms:
//|	LPTSTR				A pointer to the string
//| Returns:
//|	TRUE if successful, FALSE if not
//*---------------------------------------------------------------------------------
BOOL EatSpace(LPTSTR str)
{

	LPTSTR	lpTmp = str;
	UINT		cb=0;

	if (!str || *str == TEXT('\0'))
		return(TRUE);

	//go to end of string
	cb=lstrlen(str)-1;

	//eat trailing white space
	while(str[cb] && _istspace(str[cb]))
		cb--;

	//Null Terminate string
	str[cb+1]=TEXT('\0');

	//eat white space before string
	while(*str && _istspace(*str))
		str++;

	// Copy character to temporary string
	while(*str)
		*lpTmp++ = *str++;

	//Null Terminate string
	*lpTmp=TEXT('\0');

	return TRUE;
}

//*---------------------------------------------------------------------------------
//| ValidateEditWindowByType:
//|	This function will validate the contents of an edit window (which is either
//|	a true edit window or the edit window of a combo box) given the type of the
//|	control.
//| Parms:
//|	hDlg						Dialog window handle
//|	id							The id of the control
//|	szFieldName				Name of field for error messages
//|	fCType					The expected type of the parm
//| Returns:
//|	TRUE if contents are valid for type, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ValidateEditWindowByType(HWND hDlg, int id, LPTSTR szFieldName, SWORD fCType)
{
	LPTSTR		lpstr;
	BOOL			rtn;
	int			len=0;

	// Allocate memory for the user buffer and retrieve the text
	len=GetTrueWindowTextLength(hDlg, id)+sizeof(TCHAR);

	lpstr = AllocateMemory(len);
	if(!lpstr)
	{
		return FALSE;
	}

	if (id == -1)
	{
		GetText(hDlg, lpstr);
	}
	else
	{
		GetText(GetDlgItem(hDlg, id), lpstr);
	}

	//Remove all white space
	EatSpace(lpstr);

	rtn = ValidateTypeData(szFieldName, fCType, TRUE, lpstr);

	FREE_PTR(lpstr);

	return rtn;
}


//*---------------------------------------------------------------------------------
//| ValidateParameters:
//|	This function will go through each control and verify the data is
//|	formatted correctly for the data style.  If errors are detected,
//|	a message is raised which the user must dismiss.
//| Parms:
//|	hDlg						Dialog window handle
//|	lpDlg						The dialog description
//| Returns:
//|	TRUE if all parameters contain valid values, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ValidateParameters(HWND hDlg, lpDIALOGDESC lpDlg)
{
	UINT				dex;
	lpCONTROL		lpCtl;
	lpPARAMITEM		lpParm;
	BOOL				fNoValNeeded=FALSE;

	// Loop through the parameters and verify each
	for(dex=0;  dex<lpDlg->cCtlItems;  dex++) {
		lpCtl = lpDlg->lpCtl[dex];
		if((lpCtl->iParmNum != -1) &&				// There is a parm to validate
			(IsWindowEnabled(GetDlgItem(hDlg,lpCtl->wID))) )
		{


			lpParm = lpDlg->lpParms[lpCtl->iParmNum];

			if(IsValidateFlagSet(hDlg, lpParm, lpCtl))
				return TRUE;

			switch(lpCtl->type) {
				// Nothing do validate for this, it can only be one of two things
				case DLG_CKBOX:
					break;

				// Retrieve the user text and validate it by type
				case DLG_EDIT:
					{
					if(!(lpParm->fNull))			// This should only fail on the FillParam rgbValue edit box when it is a null pointer or a rowset value
						if(!ValidateEditWindowByType(hDlg, lpCtl->wID,
									(lpCtl->szText) ? lpCtl->szText : lpParm->szName, lpParm->fCType))
							goto exiterr;
					}
					break;

				// Verify only non CBO_DFT values, do so by type
				case DLG_COMBO:
					{
					WORD				udex;
					WORD				dftcnt = (WORD)GetDftStyleCount(lpCtl->lDataStyle);
					LPTSTR				szInput=NULL;
					UDWORD			cbInput;

					udex = (WORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_GETCURSEL, 0, 0L);
					// If there are CBO_DFT values and the user selected one, there is no
					//	need to validate them unless it is CBO_DFT_INPUT_WIN, in which case
					// we need to validate the data from the input window
					if(udex < dftcnt)
					{
						if(CBO_DFT_INPUT_WIN == IsChoiceCBODefault(hDlg, lpCtl, NULL)){
							szInput = GetInputWindowString((lpDlg->ci) ? lpDlg->ci->hwndIn : hwndOutWin, &cbInput);
							return ValidateTypeData(lpParm->szName, lpParm->fCType, TRUE, szInput);
							}
						break;
					}
					// Else see if the user selected from a default array, no need to check
					else if(lpParm->lpDfts && udex < lpParm->cDfts)
					{
						// The following code was added specifically to handle
						// the SQL_ATTR_APP_ROW_DESC and SQL_ATTR_APP_PARAM_DESC
						// Statement Attributes. If fOption is one of the above
						// then the rgbValue contains a descriptor handle +
						// the description, these take special validations, so
						// postpone validation until calling lpSQLSetStmtAttr
						if (lpParm->lpDfts->uValInfo & PRM_HDESC)
							fNoValNeeded=TRUE;
						else
							fNoValNeeded=FALSE;

						break;
					}
					else if (udex != (WORD)CB_ERR)
					{
						if (lpParm->uParmType & PT_ROWSET_CTL)
						{
						// Special case here for the Rowset HDBC and HSTMT controls in the BindParameter dialog
						// This was needed since we don't have default array, but we do have valid values
						// in the list box (the list is dynamic -- based on current HDBC's & HSTMT's)
							break;
						}
						else if ((lpParm->uParmType & PT_HDESC) || fNoValNeeded)
						{
							// No processing at this point needs to be done for Descriptor handles
							break;
						}
					}

					if(!ValidateEditWindowByType(hDlg, lpCtl->wID,
								(lpCtl->szText) ? lpCtl->szText : lpParm->szName, lpParm->fCType))
						goto exiterr;
					}
					break;

				default:
					ONDEBUG(wsprintf(szOutBuff, szInvalidDlgType, lpCtl->type));
					ASSERTCHECK(TRUE, szOutBuff);
					break;
				}
			}
		}
	return TRUE;

	// If we are here an error was detected.  Errors should have already been reported,
	//		so set focus to the offending control and return FALSE
exiterr:
	SetFocus(GetDlgItem(hDlg, lpCtl->wID));
	SendDlgItemMessage(hDlg, lpCtl->wID, EM_SETSEL, (WPARAM)0, MAKELPARAM(0,-1));
	return FALSE;
}


//*---------------------------------------------------------------------------------
//| ParameterBuilder:
//|	This function will go through each control and extract the data value
//|	for it into the parameter structure.  Special notes:
//|		Check-boxes (pointer and handles)
//|			o The fNull parm is set, lpData is ignored
//|		Edit boxes
//|			o Memory is allocated and assigned to lpData, entire value is
//|				 copied to the allocated buffer
//|		Combo boxes
//|			o For CBO_DFT_NULL, lpData is released and set to NULL
//|			o For CBO_DFT_EMPTY, 2 bytes are allocated and set to '\0'
//|			o For CBO_DFT_INPUT_WIN, whatever selection there is taken
//|				 and placed in an allocated buffer large enough to hold it
//|			o For CBO_DFT_SQL_NTS, a value large enough for the fCType
//|				 is allocated and set to SQL_NTS
//|			o For CBO_DFT_NULL_DATA, same scenario as CBO_DFT_SQL_NTS
//|			o For other types, the CBO_TYPE_xxx value is used to determine
//|				 what type the parameter is.  Enough room is allocated for
//|				 that type.  For character, the WM_TEXTLENGTH is used for
//|				 the string.  Anything length modifiers (eg: cbxxx) are
//|				 left up to the function handler.
//| Parms:
//|	hDlg						Dialog window handle
//|	lpDlg						The dialog description
//| Returns:
//|	TRUE if all parameters contain valid values, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ParameterBuilder(HWND hDlg, lpDIALOGDESC lpDlg)
{
	UINT				dex;
	lpCONTROL		lpCtl;
	lpPARAMITEM		lpParm;
	TCHAR				szHandle[MEDBUFF]=TEXT(""),
						*pStart=NULL;
	SQLHANDLE		hHandle=NULL;
	UDWORD			dwDex=0;
	INT				uErr=0;


	// Loop through the parameters and verify each
	for(dex=0;  dex<lpDlg->cCtlItems; dex++)
	{
		lpCtl = lpDlg->lpCtl[dex];

		if(lpCtl->iParmNum != -1)
		{					// There is a parameter
			lpParm = lpDlg->lpParms[lpCtl->iParmNum];

			//	fvalidate is set to 1 if parameter is not be validated.
			if(lpCtl->fValidate)
			{
				// treat like null data
				FREE_PTR(lpParm->lpData);

				lpParm->fNull = TRUE;
				lpParm->lpData = NULL;
				lpParm->cBytes = 0;


				continue;
			}


			switch(lpCtl->type)
			{
				// Check boxes (for pointer) will set fNull to TRUE/FALSE. The
				//		function handler must figure out the meaning.
				//		is required for a numeric pointer, it is allocated
				case DLG_CKBOX:
					{
						// If disabled, then there was no ODBC handle to use and therefore
						//	NULL must be used.  Else check status
						// **Unless it is a ordinary checkbox (SQL_LEN_DATA_AT_EXEC Macro)
						// In which case disabled == FALSE
						if(!IsWindowEnabled(GetDlgItem(hDlg, lpCtl->wID)))
						{
							if(lpParm->uParmType & PT_CHK)
								lpParm->fNull = FALSE;
							else
								lpParm->fNull = TRUE;
						}
						else
							lpParm->fNull = IsDlgButtonChecked(hDlg, lpCtl->wID);

						FREE_PTR(lpParm->lpData);

						// Check for numeric pointer and allocate, all others handled by
						//		function handler
						if(!lpParm->fNull)
						{
							if(lpParm->uParmType & PT_PTR_SWORD ||
								lpParm->uParmType & PT_PTR_UWORD)
							{
								lpParm->cBytes = sizeof(SWORD);
							}
							else if(lpParm->uParmType & PT_PTR_SDWORD ||
								lpParm->uParmType & PT_PTR_UDWORD)
							{
								lpParm->cBytes = sizeof(SDWORD);
							}
							else if (lpParm->uParmType & PT_HENV ||
										lpParm->uParmType & PT_HDBC ||
										lpParm->uParmType & PT_HSTMT ||
										lpParm->uParmType & PT_HDESC ||
										lpParm->uParmType & PT_MULT_HDESC ||
										lpParm->uParmType & PT_HWND)
							{
								lpParm->fHandle=PT_HHANDLE;
								lpParm->cBytes = sizeof(SQLHANDLE);
							}

							// Size has been determined, so allocate the actual memory
							if(lpParm->cBytes)
							{
								lpParm->lpData = AllocateMemory(lpParm->cBytes + 1);
								InitUnModified(lpParm->lpData, lpParm->cBytes + 1);
								if (lpParm->fHandle)
								{
									//this param is a handle, retrieve handle from control.
									GetWindowText(GetDlgItem(hDlg,
                                                             lpCtl->wID),szHandle,sizeof(szHandle)/sizeof(szHandle[0]));
									if (pStart=_tcsstr(_tcslwr(szHandle),TEXT("0x")))
									{
										pStart+=2;
										TextToHandle(pStart,(SQLHANDLE)lpParm->lpData);
									}

								}

							}
						}
					}
				break;

				// Data has already been validated for type, so simply store in the correct
				//	format
				case DLG_EDIT:
					{
					StoreCBDataFromType(GetDlgItem(hDlg, lpCtl->wID),
									lpParm, lpParm->fCType, 0);
					lpParm->dwDex = 0;
					}
					break;

				// See function header for specific notes
				case DLG_COMBO:
					{
					UDWORD			lType;
					LPTSTR			pBegin=NULL,
										pEnd=NULL,
										p=NULL;

					lpParm->fNull = FALSE;

					switch(lType=IsChoiceCBODefault(GetDlgItem(hDlg, lpCtl->wID),
										lpCtl,lpParm))
					{
						case CBO_DFT_VALID_POINTER:
							{
								//Release lpData
								FREE_PTR(lpParm->lpData);

								lpParm->cBytes = sizeof(SQLPOINTER);

								lpParm->lpData = AllocateMemory(lpParm->cBytes + 1);

								lpParm->fHandle=PT_PTR_PTR;

								//Failed to allocate data pointer
								if (!lpParm->lpData)
									return(FALSE);
							}
							break;
						case CBO_DFT_ODBC_HANDLE:
						{
								//This parameter is an ODBC Handle (i.e. HENV,HDBC,etc)
								lpParm->fHandle=PT_HHANDLE;

								FREE_PTR(lpParm->lpData);

								lpParm->cBytes = sizeof(SQLHANDLE);

								lpParm->lpData = AllocateMemory(lpParm->cBytes + 1);

								//Failed to allocate data pointer
								if (!lpParm->lpData)
									return(FALSE);

								dwDex = (SWORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_GETCURSEL, 0, 0L);

								if(dwDex != CB_ERR)
								{
									*(SQLHANDLE *)lpParm->lpData=
												(SQLHANDLE)SendMessage(GetDlgItem(hDlg, lpCtl->wID),
												CB_GETITEMDATA,(WPARAM)dwDex, 0L);

									if (*(SQLHANDLE *)lpParm->lpData == SQL_NULL_HANDLE)
										lpParm->fNull=TRUE;
								}

								if (!(*(SQLHANDLE *)lpParm->lpData))
								{
									//This handle needs to be formatted correctly.This
									//is most likely from Descriptor handle string from
									//SQLSetStmtAttr or SQLSetConnAttr. In this case
									//we need to extract the handle from the string
									if (pBegin=_tcsstr(szHandle,TEXT(":")))
									{
										pBegin+=4;
										if (pEnd=_tcsstr(szHandle,TEXT("(")))
										{
											*pEnd=TEXT('\0');
											TextToHandle(pBegin,lpParm->lpData);
										}
									}
								}

							}
							break;

						// Release any memory and leave it NULL
						case CBO_DFT_NULL:
						case CBO_DFT_NULL_HANDLE:
						case CBO_DFT_NULL_HENV:
						case CBO_DFT_NULL_HDBC:
						case CBO_DFT_NULL_HSTMT:
						case CBO_DFT_NULL_HDESC:
							{
							FREE_PTR(lpParm->lpData);
							lpParm->fNull = TRUE;
							lpParm->cBytes = 0;
							}
							break;

						// Use any allocated memory and set it to empty
						//		Note that since it is not possible to dynamically allocate less than
						//		32-bytes with the set alingment, any value is big enough
						case CBO_DFT_EMPTY:
						{
							if(!lpParm->lpData)
								lpParm->lpData = AllocateMemory(sizeof(TCHAR) + 1);
							if(lpParm->lpData)
							{
								*(LPTSTR)lpParm->lpData = TEXT('\0');
								lpParm->cBytes = sizeof(TCHAR);
							}
							else
								return FALSE;
						}
						break;

						// Gather input from window, note this could turn out to
						//		be an empty string
						case CBO_DFT_INPUT_WIN:
							{
								UINT			dex2;
								LPTSTR			szInStr=NULL;
								TCHAR			szTemp[TINYBUFF];
								SDWORD		dwLen=0;

								// Free any existing memory
								FREE_PTR(lpParm->lpData);

								szInStr = GetInputWindowString((lpDlg->ci) ? lpDlg->ci->hwndIn : hwndOutWin, &lpParm->cBytes);

								// Get the associated length from the control (given by lpCtl->wLengthID)
								// We will need this for the call to
								if(lpCtl->wLengthID)
								{
									for(dex2=0;  dex2<lpDlg->cCtlItems;  dex2++)
									{
										if(lpCtl->wLengthID == lpDlg->lpCtl[dex2]->wID)
											break;
									}

									if(!IsChoiceCBODefault(GetDlgItem(hDlg, lpCtl->wLengthID), lpDlg->lpCtl[dex2], NULL))
									{
										GetDlgItemText(hDlg, lpCtl->wLengthID,
                                                       szTemp,sizeof(szTemp)/sizeof(szTemp[0]));
										dwLen = _ttol(szTemp);
									}
								}

								if(szInStr)
								{
									if(ConvertToTypeFromString(szInStr, lpParm, lpParm->fCType, dwLen))
									{
										FREE_PTR(szInStr);
									}
									else
										return FALSE;						// There was an error
								}
								else
									return FALSE;							// No string to retrieve, or out of memory
							}
							break;

						// Release any memory and leave it NULL, we will use the dwDex value to
						// determine if <rowset value> was selected.
						case CBO_DFT_ROWSET_VALUE:
						{
							FREE_PTR(lpParm->lpData);
							lpParm->lpData = NULL;
							lpParm->cBytes = 0;
						}
						break;

						// Memory has already been allocated and filled by
						//the usrwndp.c, so do nothing here
						case CBO_DFT_INTERVAL_VALUE:
						break;

						// The following is easy, just allocate the memory and set to value.  Note
						//	that SQLPrepare and SQLExecDirect allow an SDWORD, whereas others use SWORD
						case CBO_DFT_SQL_NTS:
							if(!InitToCBODft(lpParm, SQL_NTS))
								return FALSE;
							break;

						// The following is easy, just allocate the memory and set to value
						case CBO_DFT_NULL_DATA:
							if(!InitToCBODft(lpParm, SQL_NULL_DATA))
								return FALSE;
							break;

						// The following is easy, just allocate the memory and set to value
						case CBO_DFT_DATA_AT_EXEC:
							if(!InitToCBODft(lpParm, SQL_DATA_AT_EXEC))
								return FALSE;
							break;

						// The following is easy, just allocate the memory and set to value
						case CBO_DFT_DEFAULT_PARAM:
							if(!InitToCBODft(lpParm, SQL_DEFAULT_PARAM))
								return FALSE;
							break;

						// Default, use the parameter type to allocate memory
						default:
							{
								SWORD			udex;
								UINT			dftcnt = GetDftStyleCount(lpCtl->lDataStyle);
								UINT			dex2;
								TCHAR			szTemp[25];
								SDWORD		dwLen=0;

								FREE_PTR(lpParm->lpData);

								// Check for CBO_STORE_INDEX in which case we store current selection
								udex = (SWORD)SendDlgItemMessage(hDlg, lpCtl->wID, CB_GETCURSEL, 0, 0L);
								if(lpCtl->lDataStyle & CBO_STORE_INDEX)
								{
									if(lpParm->lpData = AllocateMemory(sizeof(SWORD)))
										*(SWORD *)lpParm->lpData = udex;
								}

								// See if the chosen selection is a known user-constant
								if(lpParm->lpDfts && (UINT)udex < lpParm->cDfts)
									StoreFromCBDftArray(udex, lpParm, lpParm->fCType);
								// We're down to just storing the data
								else
								{
									// Get the associated length from the control (given by lpCtl->wLengthID)
									// We will need this for the call to
									if(lpCtl->wLengthID)
									{
										for(dex2=0;  dex2<lpDlg->cCtlItems;  dex2++)
										{
											if(lpCtl->wLengthID == lpDlg->lpCtl[dex2]->wID)
												break;
										}

										if(!IsChoiceCBODefault(GetDlgItem(hDlg, lpCtl->wLengthID), lpDlg->lpCtl[dex2], NULL) ||
											((lpParm->uParmType & PT_INPUT_OUTPUT)== PT_INPUT_OUTPUT))
										{
											GetDlgItemText(hDlg,lpCtl->wLengthID,szTemp,25);
											dwLen = _ttol(szTemp);
										}
									}

									StoreCBDataFromType(GetDlgItem(hDlg, lpCtl->wID), lpParm, lpParm->fCType, dwLen);

								}
							}
							break;
						}
					}
					break;		// End of DLG_COMBO

				default:
					ONDEBUG(wsprintf(szOutBuff, szInvalidDlgType, lpCtl->type));
					ASSERTCHECK(TRUE, szOutBuff);
					break;
			}
		}
	}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| StoreCBDataFromType:
//|	This function will take the given type and store the data in the
//|	given window into the correct format for that type.  Note that
//|	all data is assumed to be valid at this point, since the validation
//|	routines have already been done.
//| Parms:
//|	hwnd						The handle of the control
//|	lpParm					Where to store the data
//|	fCType					In anticipation of future requirements where
//|										we won't want lpParm->fCType
//|	dwLen						Length of data from calculated control (if needed)
//| Returns:
//|	CBO_DFT_xxx if the user has selected a default value, else 0
//*---------------------------------------------------------------------------------

BOOL INTFUN StoreCBDataFromType(HWND hwnd, lpPARAMITEM lpParm, SWORD fCType, SDWORD dwLen)
{
	LPTSTR		szBuff;
	BOOL			rtn;
	LRESULT		lLen=GetTrueWindowTextLength(hwnd,-1)+sizeof(TCHAR);

	if (dwLen > lLen)
		lLen=dwLen;

	if(!(szBuff = (LPTSTR)AllocateMemory(lLen)))
		return FALSE;
	GetText(hwnd, szBuff);

	rtn = ConvertToTypeFromString(szBuff, lpParm, fCType, dwLen);

	FREE_PTR(szBuff);

	return rtn;
}

//*---------------------------------------------------------------------------------
//| EatDescDescription:
//|	This function removes the Description of an HDESC
//|
//| Parms:
//|	str			The string to convert
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN EatDescDescription(LPTSTR str)
{
	TCHAR *pQuote=NULL;

	//Search for a quote
	if (pQuote=_tcschr(str,TEXT('(')))
	{
		*pQuote=TEXT('\0');
	}



} //EatDescDescription



//*---------------------------------------------------------------------------------
//| GetTSFraction:
//|	This function retrieves the Time stamp fraction from 'frstr' parameter
//|
//| Parms:
//|	str			The string to convert
//| Returns:
//|	fraction
//*---------------------------------------------------------------------------------

UDWORD INTFUN GetTSFraction(LPTSTR frstr)
{
	UDWORD udFrac=0;
	TCHAR		szFrac[11];
	int		iFracLen;

	if (frstr)
	{
		memset(szFrac, TEXT('0'), 9);
		szFrac[9] = TEXT('\0');

		if(lstrlen(frstr) > 9)							// Check if fractional part is > 9 characters
			iFracLen = 9;									// If it is, truncate it to 9 characters
		else
			iFracLen = lstrlen(frstr);

		_tcsncpy(szFrac, frstr, iFracLen);
		szFrac[iFracLen]=TEXT('\0');
		udFrac = _tcstoul(szFrac, NULL, 10);
	}

	return(udFrac);

} //GetTSFunction()


//*---------------------------------------------------------------------------------
//| LoadIntervalStruct:
//|	This function loads the fields of SQL_INTERVAL_STRUCT structure.
//|
//| Parms:
//|	lpis			pointer to allocated interval struct
//|	str			The string to convert
//|	lpParm		Where to store the data
//|	fCType		In anticipation of future requirements where
//|						we won't want lpParm->fCType
//|	cbLen			The size of the string given in the cbValue control (the
//|						length control associated with the string)
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN LoadIntervalStruct(SQL_INTERVAL_STRUCT *lpis, LPTSTR str,
										 PTR lpData, SWORD fCType, SDWORD cbLen,
										 UDWORD *pcBytes)
{
	LPTSTR		tmpstr=str,
					pSign=NULL,
					pTok=NULL;

	lpis = (SQL_INTERVAL_STRUCT *)lpData;

	*pcBytes = sizeof(SQL_INTERVAL_STRUCT);

	//Strip of prefix(i.e. 'INTERVAL') and suffix (i.e. 'YEAR TO MONTH')
	pTok=_tcstok(tmpstr,TEXT("'"));

	//Set sign field
	lpis->interval_sign=(_tcschr(pTok,TEXT('-')) == NULL ? SQL_FALSE:SQL_TRUE);

	//get next token (should be the interval data
	pTok=_tcstok(NULL,TEXT("'"));

	switch(fCType)
	{
		case SQL_C_INTERVAL_YEAR:
			//Format: INTERVAL [sign] '1990' YEAR
			lpis->interval_type=SQL_IS_YEAR;
			lpis->intval.year_month.year = _ttoi(pTok);
			break;
		case SQL_C_INTERVAL_YEAR_TO_MONTH:
			//Format: INTERVAL [sign] '1990-12' YEAR TO MONTH
			lpis->interval_type=SQL_IS_YEAR_TO_MONTH;
			lpis->intval.year_month.year = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.year_month.month = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_MONTH:
			//Format: INTERVAL [sign] '11' MONTH
			lpis->interval_type=SQL_IS_MONTH;
			lpis->intval.year_month.month = _ttoi(pTok);
			break;
		case SQL_C_INTERVAL_DAY:
			//Format: INTERVAL [sign] '30' DAY
			lpis->interval_type=SQL_IS_DAY;
			lpis->intval.day_second.day = _ttoi(pTok);
			break;
		case SQL_C_INTERVAL_DAY_TO_HOUR:
			//Format: INTERVAL [sign] '30 22' DAY TO HOUR
			lpis->interval_type=SQL_IS_DAY_TO_HOUR;
			lpis->intval.day_second.day = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.hour = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_DAY_TO_MINUTE:
			//Format: INTERVAL [sign] '30 22:01' DAY TO MINUTE
			lpis->interval_type=SQL_IS_DAY_TO_MINUTE;
			lpis->intval.day_second.day = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.hour = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.minute = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_DAY_TO_SECOND:
			//Format: INTERVAL [sign] '30 22:01:02' DAY TO SECOND
			lpis->interval_type=SQL_IS_DAY_TO_SECOND;
			lpis->intval.day_second.day = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.hour = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.minute = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.second = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.fraction = GetTSFraction(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_HOUR:
			//Format: INTERVAL [sign] '36' HOUR
			lpis->interval_type=SQL_IS_HOUR;
			lpis->intval.day_second.hour = _ttoi(pTok);
			break;
		case SQL_C_INTERVAL_HOUR_TO_MINUTE:
			//Format: INTERVAL [sign] '22:01' HOUR TO MINUTE
			lpis->interval_type=SQL_IS_HOUR_TO_MINUTE;
			lpis->intval.day_second.hour = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.minute = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_HOUR_TO_SECOND:
			//Format: INTERVAL [sign] '22:01:02.1234' HOUR TO SECOND
			lpis->interval_type=SQL_IS_HOUR_TO_SECOND;
			lpis->intval.day_second.hour = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.minute = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.second = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.fraction = GetTSFraction(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_MINUTE:
			//Format: INTERVAL [sign] '12' MINUTE
			lpis->interval_type=SQL_IS_MINUTE;
			lpis->intval.day_second.minute = _ttoi(pTok);
			break;
		case SQL_C_INTERVAL_MINUTE_TO_SECOND:
			//Format: INTERVAL [sign] '01:02.1234' MINUTE TO SECOND
			lpis->interval_type=SQL_IS_MINUTE_TO_SECOND;
			lpis->intval.day_second.minute = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.second = _ttoi(_tcstok(NULL, szDATETIMEDELIM));
			lpis->intval.day_second.fraction = GetTSFraction(_tcstok(NULL, szDATETIMEDELIM));
			break;
		case SQL_C_INTERVAL_SECOND:
			//Format: INTERVAL [sign] '34' SECOND
			lpis->interval_type=SQL_IS_SECOND;
			lpis->intval.day_second.second = _ttoi(_tcstok(pTok, szDATETIMEDELIM));
			lpis->intval.day_second.fraction = GetTSFraction(_tcstok(NULL, szDATETIMEDELIM));
			break;

		default:
			ASSERTCHECK(fCType,TEXT("Invalid fCType"));
	}


} //LoadIntervalStruct()


//*---------------------------------------------------------------------------------
//| ConvertToTypeFromHex:
//|	This function converts the hex string str to it's type and stuffs it in lpData
//|
//| Parms:
//|	str			The string to convert
//|	lpData		Where to store the data
//|	fCType		In anticipation of future requirements where
//|						we won't want lpParm->fCType
//|	cbLen			The size of the string given in the cbValue control (the
//|						length control associated with the string)
//|	pcBytes		sizeof of data type
//|	uParmType	Parameter type.
//|	uwPrecision	Precision of data
//|	uwScale		Scale of data
//| Returns:
//|	lpData		converted data
//*---------------------------------------------------------------------------------

PTR INTFUN ConvertToTypeFromHex(LPTSTR str, SWORD fCType, SDWORD cbLen,UDWORD *pcBytes,
						 UDWORD uParmType,UWORD uwPrecision,UWORD uwScale)
{
	PTR lpData=NULL;

	switch(fCType)
	{
		case SQL_C_CHAR:
		case SQL_C_WCHAR:
		case SQL_C_BINARY:
			{
				SDWORD		dwlen=0;
				LPTSTR		lpTemp=str+2;
				LPBYTE		lpChr=NULL,
								lpByte=NULL;
				LPTSTR		pDesc=NULL;

				// Figure out how many bytes we need to allocate for the string.
				// First count the number of bytes needed then add 1 for the NULL terminator (or count the '0x')
				// If this string comes from a control with an associated calculated length control, we need
				// 	to use the greater of these two:  1) The length of the string, or 2) The value specified
				//		in the associated length control

				//Bug #276 dwLen should not include the '0x' prefix
				dwlen = lstrlen(str);
				dwlen = dwlen > 2 ? (dwlen - 2) / 2 : 0;

				//At this point cbLen is lstrlen(str) which is not the correct
				//number of bytes in a hex string. So we need to recalc cbLen
				// I realize that this assignment statement obsoletes the
				//"dwlen > cbLen" code but the code was left in just in case
				//the fix had problems
				cbLen=dwlen;

				if(lpData = AllocateMemory((dwlen > cbLen) ? dwlen : cbLen))
				{

					//Find the beginning of the Descriptor handle Description
					if ((pDesc=_tcschr(lpTemp,TEXT('('))) ||
						 (pDesc=_tcschr(lpTemp,TEXT('"'))))
					{
						*pDesc=TEXT('\0');
					}

					//Bug #691
					// The following code was added so that gator correctly stores
					// an address (i.e. a HDESC).
					if (uParmType & PT_HDESC)
						_stscanf(lpTemp,TEXT("%x"),lpData);
					else
						ConvertFromHex(lpChr, lpData, lpTemp);

					*pcBytes = (dwlen > cbLen) ? dwlen : cbLen;
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_BIT:
		case SQL_C_TINYINT:
		case SQL_C_STINYINT:
		case SQL_C_UTINYINT:
			{
				UINT			cPads;
				TCHAR			Buff[3];
				UTINYINT		uTiny=0;

				// First count the number of bytes and zero pad the beginning of the string
				// Also remove the '0x' for simplicity (just don't copy it to temporary buffer (Buff)
				cPads = (sizeof(UTINYINT) * 2) + 2 - lstrlen(str);
				memset(Buff, '0', cPads);
				lstrcpy((Buff+cPads), (str+2));

				if(lpData = AllocateMemory(sizeof(UTINYINT)))
				{
					// Convert the data from hex to char and place in lpData
					_stscanf(Buff, TEXT("%x"), &uTiny);
					*(UTINYINT *)lpData=uTiny;
  					*pcBytes = sizeof(UTINYINT);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_SHORT:
		case SQL_C_SSHORT:
		case SQL_C_USHORT:
			{
				UINT		cPads;
				TCHAR		Buff[30];

				// First count the number of bytes and zero pad the beginning of the string
				// Also remove the '0x' for simplicity (just don't copy it to temporary buffer (Buff)
				cPads = (sizeof(UWORD) * 2) + 2 - lstrlen(str);
				memset(Buff, '0', cPads);
				lstrcpy((Buff+cPads), (str+2));

				if(lpData = AllocateMemory(sizeof(UWORD)))
				{
					// Convert the data from hex to char and place in lpData
					*(UWORD *)lpData=ConvertHexCharToWord(Buff);
					*pcBytes = sizeof(UWORD);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_LONG:
		case SQL_C_SLONG:
		case SQL_C_ULONG:
			{
				UINT		cPads;
				TCHAR		Buff[30];

				// First count the number of bytes and zero pad the beginning of the string
				// Also remove the '0x' for simplicity (just don't copy it to temporary buffer (Buff)
				cPads = (sizeof(UDWORD) * 2) + 2 - lstrlen(str);
				memset(Buff, '0', cPads);
				lstrcpy((Buff+cPads), (str+2));

				if(lpData = AllocateMemory(sizeof(UDWORD)))
				{
					// Convert the data from hex to char and place in lpData
					*(UDWORD *)lpData=ConvertHexCharToDWord(Buff);
					*pcBytes = sizeof(UDWORD);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_FLOAT:
			{
				LPTSTR	lpTemp=str+2;
				LPBYTE	lpChr=NULL;

				if(lpData = AllocateMemory(sizeof(SFLOAT)))
				{
					ConvertFromHex(lpChr, lpData, lpTemp);
					*pcBytes = sizeof(SFLOAT);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_DOUBLE:
			{
				LPTSTR	lpTemp=str+2;
				LPBYTE	lpChr=NULL;

				if(lpData = AllocateMemory(sizeof(SDOUBLE)))
				{
					ConvertFromHex(lpChr, lpData, lpTemp);
					*pcBytes = sizeof(SDOUBLE);
				}
				else
					return FALSE;
			}
			break;
		case SQL_C_SBIGINT:
		case SQL_C_UBIGINT:
			{
				LPTSTR	lpTemp=str+2;
				LPBYTE	lpChr=NULL;
				BOOL	fSigned=fCType == SQL_C_SBIGINT;

				if (lpData = AllocateMemory(sizeof(DWORDLONG)))
				{
					CharToBigint(lpTemp,lstrlen(lpTemp),lpData,0,fSigned);
					*pcBytes = sizeof(DWORDLONG);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_NUMERIC:
			{
				LPTSTR	lpTemp=str+2;
				LPBYTE	lpChr=NULL;

				if(lpData = AllocateMemory(sizeof(SQL_NUMERIC_STRUCT)))
				{
					ConvertFromHex(lpChr, lpData, lpTemp);
					*pcBytes = sizeof(SQL_NUMERIC_STRUCT);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_INTERVAL_DAY:
		case SQL_C_INTERVAL_DAY_TO_HOUR:
		case SQL_C_INTERVAL_DAY_TO_MINUTE:
		case SQL_C_INTERVAL_DAY_TO_SECOND:
		case SQL_C_INTERVAL_HOUR:
		case SQL_C_INTERVAL_HOUR_TO_MINUTE:
		case SQL_C_INTERVAL_HOUR_TO_SECOND:
		case SQL_C_INTERVAL_MINUTE:
		case SQL_C_INTERVAL_MINUTE_TO_SECOND:
		case SQL_C_INTERVAL_MONTH:
		case SQL_C_INTERVAL_SECOND:
		case SQL_C_INTERVAL_YEAR:
		case SQL_C_INTERVAL_YEAR_TO_MONTH:
			{
				WORD		*lpChr=NULL;
				LPTSTR	lpTemp=str+2;

				if(lpData = AllocateMemory(sizeof(SQL_INTERVAL_STRUCT)))
				{
					lpChr = (WORD *)lpData;

					// Convert the "interval_sign" and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp +=2;

					// Convert the "interval_type" and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp +=2;

					if (!lstrcmp(lpTemp,TEXT("SQL_YEAR")) ||
						 !lstrcmp(lpTemp,TEXT("SQL_MONTH")) ||
						 !lstrcmp(lpTemp,TEXT("SQL_YEAR_TO_MONTH")))
					{

						// Convert the "year" and place in lpData
						*(lpChr++)=ConvertHexCharToWord(lpTemp);
						lpTemp +=2;
						// Convert the "month" and place in lpData
						*(lpChr++)=ConvertHexCharToWord(lpTemp);
						lpTemp +=2;
					}
					else
					{
						// Convert the "day" and place in lpData
						*(lpChr++)=ConvertHexCharToWord(lpTemp);
						lpTemp +=2;
						// Convert the "hour" and place in lpData
						*(lpChr++)=ConvertHexCharToWord(lpTemp);
						lpTemp +=2;
						// Convert the "minute" and place in lpData
						*(lpChr++)=ConvertHexCharToWord(lpTemp);
						lpTemp +=2;
						// Convert the "second" and place in lpData
						*(lpChr++)=ConvertHexCharToWord(lpTemp);
						lpTemp +=2;
						// Convert the "fraction and place in lpData
						*lpChr=ConvertHexCharToWord(lpTemp);
					}

					*pcBytes = sizeof(SQL_INTERVAL_STRUCT);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
			{
				WORD		*lpChr=NULL;
				LPTSTR	lpTemp=str+2;

				if(lpData = AllocateMemory(sizeof(DATE_STRUCT)))
				{
					lpChr = (WORD *)lpData;
					// Convert the year and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp +=2;
					// Convert the month and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp +=2;
					// Convert the day and place in lpData
					*lpChr=ConvertHexCharToWord(lpTemp);

					*pcBytes = sizeof(DATE_STRUCT);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
			{
				WORD		*lpChr=NULL;
				LPTSTR	lpTemp=str+2;

				if(lpData = AllocateMemory(sizeof(TIME_STRUCT)))
				{
					lpChr = (WORD *)lpData;
					// Convert the hours and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the minutes and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the seconds and place in lpData
					*lpChr=ConvertHexCharToWord(lpTemp);

					*pcBytes = sizeof(TIME_STRUCT);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_TIMESTAMP:
		case SQL_C_TYPE_TIMESTAMP:
			{
				WORD		*lpChr;
				LPTSTR	lpTemp=str+2;

				if(lpData = AllocateMemory(sizeof(TIMESTAMP_STRUCT)))
				{
					lpChr = (WORD *)lpData;
					// Convert the year and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the month and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the day and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the hours and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the minutes and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the seconds and place in lpData
					*(lpChr++)=ConvertHexCharToWord(lpTemp);
					lpTemp += (sizeof(UWORD) * 2);
					// Convert the fractional part and place in lpData
					if(*lpTemp)  // Check for NULL in case the fractional part is missing
						*((UDWORD *)lpChr)=ConvertHexCharToDWord(lpTemp);

					*pcBytes = sizeof(TIMESTAMP_STRUCT);
				}
				else
					return FALSE;
			}
			break;

 		default:
		{
			SDWORD		dwlen=0;
			LPTSTR		lpTemp=str+2;
			LPBYTE 		lpChr = NULL;
			UINT			cbChar=sizeof(TCHAR);

			ONDEBUG(wsprintf(szOutBuff, szInvalidCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);

			// Figure out how many bytes we need to allocate for the string.
			// First count the number of bytes needed then add 1 for the NULL terminator (or count the '0x')
			// If this string comes from a control with an associated calculated length control, we need
			// 	to use the greater of these two:  1) The length of the string, or 2) The value specified
			//		in the associated length control
			dwlen = (lstrlen(lpTemp)/2) * cbChar;

			if (lpData = AllocateMemory(((dwlen > cbLen) ? dwlen : cbLen)+cbChar))
			{

				lpChr=(LPBYTE)lpData;

				// Convert the data from hex to char and place in lpData
				while(*lpTemp)
            {
					*lpChr=ConvertHexCharToBinary(lpTemp);
					lpChr++;
					lpTemp+=2;
				}

				*pcBytes = (dwlen > cbLen) ? dwlen : cbLen;
			}
			else
				return FALSE;
		}
		break;
	}

	return(lpData);

} //ConvertToTypeFromHex()


//*---------------------------------------------------------------------------------
//| ParseDate:
//|	This function converts a date string to a DATE_STRUCT
//|
//| Parms:
//|	lpds			pointer to DATE_STRUCT
//|	szDate		string containing the DATE_STRUCT data
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN ParseDate(DATE_STRUCT *lpds,LPTSTR szDate)
{
	lpds->year = (WORD)_ttoi(_tcstok(szDate, szDATETIMEDELIM));
	lpds->month = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));
	lpds->day = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));

} //ParseDate()


//*---------------------------------------------------------------------------------
//| ParseTime:
//|	This function converts a time string to a TIME_STRUCT
//|
//| Parms:
//|	lpts			pointer to TIME_STRUCT
//|	szTime		string containing the TIME_STRUCT data
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN ParseTime(TIME_STRUCT *lpts,LPTSTR szTime)
{
	lpts->hour = (UWORD)_ttoi(_tcstok(szTime, szDATETIMEDELIM));
	lpts->minute = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));
	lpts->second = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));

} //ParseTime()


//*---------------------------------------------------------------------------------
//| ParseTimeStamp:
//|	This function converts a time-stamp string to a TIMESTAMP_STRUCT
//|
//| Parms:
//|	lptss			pointer to TIMESTAMP_STRUCT
//|	szTS			string containing the TIMESTAMP_STRUCT data
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------

VOID INTFUN ParseTimeStamp(TIMESTAMP_STRUCT *lptss,LPTSTR szTS)
{

	// First the date
	lptss->year = (WORD)_ttoi(_tcstok(szTS, szDATETIMEDELIM));
	lptss->month = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));
	lptss->day = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));

	// Then the complete time
	lptss->hour = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));
	lptss->minute = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));
	lptss->second = (UWORD)_ttoi(_tcstok(NULL, szDATETIMEDELIM));

	lptss->fraction=GetTSFraction(_tcstok(NULL, szDATETIMEDELIM));

} //ParseTimeStamp()


//*---------------------------------------------------------------------------------
//| ConvertToTypeFromChar:
//|	This function converts the char string str to it's type and stuffs it in lpData
//|
//| Parms:
//|	str			The string to convert
//|	lpData		Where to store the data
//|	fCType		In anticipation of future requirements where
//|						we won't want lpParm->fCType
//|	cbLen			The size of the string given in the cbValue control (the
//|						length control associated with the string)
//|	pcBytes		sizeof of data type
//|	uwPrecision	Precision of data
//|	uwScale		Scale of data
//| Returns:
//|	lpData		converted data
//*---------------------------------------------------------------------------------

PTR INTFUN ConvertToTypeFromChar(LPTSTR szInStr, SWORD fCType, SDWORD cbLen,UDWORD *pcBytes,
											UWORD uwPrecision,UWORD uwScale)
{
	PTR lpData=NULL;
	LPTSTR str=_tcsdup(szInStr);

	switch(fCType)
	{
		case SQL_C_NUMERIC:
			{
			SQL_NUMERIC_STRUCT *lpNS=NULL;

			*pcBytes = sizeof(SQL_NUMERIC_STRUCT);

			if(lpData = AllocateMemory(*pcBytes))
			{
				memset(lpData,0,*pcBytes);

				lpNS=(SQL_NUMERIC_STRUCT *)lpData;
				//precision and scale needs to come from the APD
				//
				lpNS->precision=(BYTE)uwPrecision;
				lpNS->scale=(BYTE)uwScale;

				stringtonumeric(str,(UWORD)(lstrlen(str)*sizeof(TCHAR)),
										(SQL_NUMERIC_STRUCT *)lpData,pcBytes);
			}
			else
				return FALSE;
			}
			break;
		case SQL_C_WCHAR:
		{
			SDWORD	cbChar=sizeof(TCHAR);
			SDWORD		dwlen = lstrlen(str)*cbChar;
			SDWORD		dwBytes= (dwlen+cbChar > cbLen) ? dwlen+cbChar : cbLen;

			// Figure out how many bytes we need to allocate for the string.
			// First count the number of bytes needed then add 1 for the NULL terminator
			// If this string comes from a control with an associated calculated length control, we need
			// 	to use the greater of these two:  1) The length of the string, or 2) The value specified
			//		in the associated length control
			if(lpData = AllocateMemory(dwBytes * 2))
			{
#ifndef _UNICODE
				*pcBytes = MultiByteToWideChar(CP_ACP,0,str,-1,(WCHAR * )lpData,dwBytes * 2);

#else
				lstrcpy((LPTSTR)lpData, str);
				*pcBytes = (dwlen > cbLen) ? dwlen : cbLen;
#endif
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_CHAR:
		{
			SDWORD	cbChar=sizeof(TCHAR);
			SDWORD		dwlen = lstrlen(str)*cbChar;
			SDWORD		dwBytes= (dwlen+cbChar > cbLen) ? dwlen+cbChar : cbLen;

			// Figure out how many bytes we need to allocate for the string.
			// First count the number of bytes needed then add 1 for the NULL terminator
			// If this string comes from a control with an associated calculated length control, we need
			// 	to use the greater of these two:  1) The length of the string, or 2) The value specified
			//		in the associated length control
			if(lpData = AllocateMemory(dwBytes))
			{
#ifdef _UNICODE
				*pcBytes = WideCharToMultiByte(CP_ACP,0,str,-1,(LPSTR )lpData,dwBytes,NULL,NULL);
				*pcBytes -= 1; // remove the null terminator

#else
				lstrcpy((LPTSTR)lpData, str);
				*pcBytes = (dwlen > cbLen) ? dwlen : cbLen;
#endif
			}
			else
				return FALSE;
		}
		break;
		case SQL_C_BINARY:
			{
			SDWORD	cbChar=sizeof(TCHAR);
			SDWORD		dwlen = lstrlen(str)*cbChar;
			SDWORD		dwBytes= (dwlen+cbChar > cbLen) ? dwlen+cbChar : cbLen;

			// Figure out how many bytes we need to allocate for the string.
			// First count the number of bytes needed then add 1 for the NULL terminator
			// If this string comes from a control with an associated calculated length control, we need
			// 	to use the greater of these two:  1) The length of the string, or 2) The value specified
			//		in the associated length control
			if(lpData = AllocateMemory(dwBytes))
			{
				lstrcpy((LPTSTR)lpData, str);
				*pcBytes = (dwlen > cbLen) ? dwlen : cbLen;

			}
			else
				return FALSE;
		}
		break;
		case SQL_C_BIT:
		{
			if(lpData = AllocateMemory(sizeof(UCHAR)))
			{
				*(UCHAR *)lpData = (UCHAR)_ttoi(str);
				*pcBytes = sizeof(UCHAR);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_TINYINT:
		case SQL_C_STINYINT:
		{
			if(lpData = AllocateMemory(sizeof(STINYINT)))
			{
				*(STINYINT *)lpData = (STINYINT)_ttoi(str);
				*pcBytes = sizeof(STINYINT);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_UTINYINT:
		{
			if(lpData = AllocateMemory(sizeof(UTINYINT)))
			{
				*(UTINYINT *)lpData = (UTINYINT)_ttoi(str);
				*pcBytes = sizeof(UTINYINT);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_SHORT:
		case SQL_C_SSHORT:
		{
			if(lpData = AllocateMemory(sizeof(SWORD)))
			{
				*(SWORD *)lpData = (SWORD)_ttoi(str);
				*pcBytes = sizeof(SWORD);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_USHORT:
		{
            int iValue = _ttoi(str);

			if(iValue >= 0 && (lpData = AllocateMemory(sizeof(UWORD))))
			{
				*(UWORD *)lpData = (UWORD)iValue;
				*pcBytes = sizeof(UWORD);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_LONG:
		case SQL_C_SLONG:
		{
			if(lpData = AllocateMemory(sizeof(SDWORD)))
			{
				*(SDWORD *)lpData = _ttol(str);
				*pcBytes = sizeof(SDWORD);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_ULONG:
		{
            long iValue = _ttol(str);
			if(iValue >= 0 && (lpData = AllocateMemory(sizeof(UDWORD))))
			{
				*(UDWORD *)lpData = (UDWORD)iValue;
				*pcBytes = sizeof(UDWORD);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_FLOAT:
		{
			TCHAR *endptr=NULL;
			SFLOAT f=(SFLOAT)0;

			if(lpData = AllocateMemory(sizeof(SFLOAT)))
			{
				_stscanf(str, TEXT("%f"),&f);

				*(SFLOAT *)lpData = f;
				*pcBytes = sizeof(SFLOAT);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_DOUBLE:
		{
			TCHAR *endptr=NULL;
			SDOUBLE d=0;

			if(lpData = AllocateMemory(sizeof(SDOUBLE)))
			{
				d=_tcstod(str,&endptr);
				*(SDOUBLE *)lpData = d;
				*pcBytes = sizeof(SDOUBLE);
			}
			else
				return FALSE;
		}
		break;
		case SQL_C_SBIGINT:
		case SQL_C_UBIGINT:
			{
				BOOL	fSigned=fCType == SQL_C_SBIGINT;

				if (lpData = AllocateMemory(sizeof(DWORDLONG)))
				{
					CharToBigint(str,lstrlen(str),lpData,0,fSigned);
					*pcBytes = sizeof(DWORDLONG);
				}
				else
					return FALSE;
			}
			break;

		case SQL_C_DATE:
		case SQL_C_TYPE_DATE:
		{
			LPTSTR		tmpstr=str;

			if(lpData = AllocateMemory(sizeof(DATE_STRUCT)))
			{
				ParseDate((DATE_STRUCT *)lpData,tmpstr);
				*pcBytes = sizeof(DATE_STRUCT);
			}
			else
				return FALSE;
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
			SQL_INTERVAL_STRUCT *lpis=NULL;

			if(lpData = AllocateMemory(sizeof(SQL_INTERVAL_STRUCT)))
				LoadIntervalStruct(lpis,str,lpData,fCType,cbLen,pcBytes);
			else
				return FALSE;
		}
		break;

		case SQL_C_TIME:
		case SQL_C_TYPE_TIME:
		{
			LPTSTR		tmpstr=str;

			if(lpData = AllocateMemory(sizeof(TIME_STRUCT)))
			{
				ParseTime((TIME_STRUCT *)lpData,tmpstr);
				*pcBytes = sizeof(TIME_STRUCT);
			}
			else
				return FALSE;
		}
		break;

		case SQL_C_TIMESTAMP:
		case	SQL_C_TYPE_TIMESTAMP:
			{
			LPTSTR							tmpstr=str;

			if(lpData = AllocateMemory(sizeof(TIMESTAMP_STRUCT)))
			{
				ParseTimeStamp((TIMESTAMP_STRUCT *)lpData,tmpstr);
				*pcBytes = sizeof(TIMESTAMP_STRUCT);
			}
			else
				return FALSE;
			}
			break;
		case SQL_C_GUID:
			{
				LPTSTR	tmpstr=str;
				if(lpData = AllocateMemory(sizeof(GUID)) )
				{
					UuidFromString(tmpstr,(UUID *)lpData);
                    *pcBytes = sizeof(UUID);
				}
				else
					return FALSE;
			}
			break;
 		default:
			{
			SDWORD		dwlen = lstrlen(str)+1;
			SDWORD		dwChr = sizeof(TCHAR);				// Add room for NULL

			dwlen*=dwChr;

			ONDEBUG(wsprintf(szOutBuff, szInvalidCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);

			// Figure out how many bytes we need to allocate for the string.
			// First count the number of bytes needed then add 1 for the NULL terminator
			// If this string comes from a control with an associated calculated length control, we need
			// 	to use the greater of these two:  1) The length of the string, or 2) The value specified
			//		in the associated length control
			if(lpData = AllocateMemory((dwlen > cbLen) ? dwlen : cbLen)) {
				lstrcpy((LPTSTR)lpData, str);
				*pcBytes = (dwlen > cbLen) ? dwlen : cbLen;
				}
			else
				return FALSE;
			}
			break;
	}

	if (str)
		free(str);

	return(lpData);

} //ConvertToTypeFromChar()


//*---------------------------------------------------------------------------------
//| ConvertToTypeFromString:
//|	This function will take a NULL terminated string and convert its value
//|	to the fCType specified.  The value is stored in the lpParm structure.
//|
//|	NOTE:  This is a ParameterBuilder component.  It is assumed that the value
//|				passed in is a valid value to be converted as is.  No extra error
//|				checking will be done.
//| Parms:
//|	str						The string to convert
//|	lpParm					Where to store the data
//|	fCType					In anticipation of future requirements where
//|										we won't want lpParm->fCType
//|	cbLen						The size of the string given in the cbValue control (the
//|										length control associated with the string)
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN ConvertToTypeFromString(LPTSTR str, lpPARAMITEM lpParm, SWORD fCType, SDWORD cbLen)
{
	LPTSTR	lpUp=str;
	DWORD		dwLO=0,
				dwHI=0;
	HANDLE	hHandle=0;
	UINT		cbChar=sizeof(TCHAR);


	// Check for a hex string.  Parse & build from the hex string to given data type
	if (IsHexString(lpUp))
	{
		//toupper the string
		_tcsupr(lpUp);

		// Remove all spaces from the hex string
	   if(!EatWhiteSpace(lpUp))
	   	return FALSE;

		//This may be an HDESC containing a Description. We need to eat the
		//Description.
		EatDescDescription(lpUp);

		//Is this a handle
		if (lpParm->fHandle)
		{
			//eat the "0x"
			lpUp+=cbChar*2;

			TextToHandle(lpUp,&hHandle);

			if(lpParm->lpData = AllocateMemory(sizeof(HANDLE)))
			{
				*(HANDLE *)lpParm->lpData=hHandle;
				return(TRUE);
			}
			else
				return(FALSE);


		}

		//Do the conversion
		lpParm->lpData=ConvertToTypeFromHex(lpUp,fCType,cbLen,&lpParm->cBytes,
										lpParm->uParmType,lpParm->uwPrecision,lpParm->uwScale);

	}
   else
	{
		// If the string is not a hex string
		//Do the conversion
		lpParm->lpData=ConvertToTypeFromChar(lpUp,fCType,cbLen,&lpParm->cBytes,
			lpParm->uwPrecision,lpParm->uwScale);
	}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| ConvertHexCharToBinary:
//|	This function will take a pointer to a string and convert it from its
//|	character representation of a hexidecimal(binary) value into the binary
//|	value itself.
//|
//| Parms:
//|	LPTSTR						A pointer to a pointer to the string
//|
//| Returns:
//|		A UCHAR * which points to the binary value (after the conversion).
//|	This function also sets the LPTSTR pointer to the character in the string
//|	which follows the last character that was converted (the next character
//|	to be converted) in the string.
//*---------------------------------------------------------------------------------

_inline BYTE GetDigit(BYTE tChr)
{
	BYTE Chr;

	switch(tChr)
	{
		case '0':
				Chr=0;
				break;
		case '1':
				Chr=1;
				break;
		case '2':
				Chr=2;
				break;
		case '3':
				Chr=3;
				break;
		case '4':
				Chr=4;
				break;
		case '5':
				Chr=5;
				break;
		case '6':
				Chr=6;
				break;
		case '7':
				Chr=7;
				break;
		case '8':
				Chr=8;
				break;
		case '9':
				Chr=9;
				break;
		case 'A':
				Chr=10;
				break;
		case 'B':
				Chr=11;
				break;
		case 'C':
				Chr=12;
				break;
		case 'D':
				Chr=13;
				break;
		case 'E':
				Chr=14;
				break;
		case 'F':
				Chr=15;
				break;

	} //end switch

	return(Chr);

} //MyConvertHexCharToBinary()


//*---------------------------------------------------------------------------------
//| ConvertHexCharToBinary:
//|	This function will take a pointer to a string and convert it from its
//|	character representation of a hexidecimal(binary) value into the binary
//|	value itself.
//|
//| Parms:
//|	LPTSTR						A pointer to a pointer to the string
//|
//| Returns:
//|		A UCHAR * which points to the binary value (after the conversion).
//|	This function also sets the LPTSTR pointer to the character in the string
//|	which follows the last character that was converted (the next character
//|	to be converted) in the string.
//*---------------------------------------------------------------------------------

_inline BYTE ConvertHexCharToBinary (LPTSTR str)
{
	BYTE	Chr=0;
	TCHAR	tChr[3]=TEXT("");

	_tcsnccpy(tChr,str,2);
	_stscanf(tChr,TEXT("%x"),&Chr);

	return Chr;									// Return a pointer to the converted character
}


//*---------------------------------------------------------------------------------
//| ConvertHexCharToWord:
//|	This function will take a pointer to a string and convert it from its
//|	character representation of a hexidecimal(binary) WORD into the binary
//|	value (WORD) itself.
//|
//| Parms:
//|	LPTSTR						A pointer to a pointer to the string
//|
//| Returns:
//|		A WORD which contains the binary value (after the conversion) if successful.
//|	This function also sets the LPTSTR pointer to the character in the string
//|	which follows the last character that was converted (the next character to be
//|	converted) in the string.
//*---------------------------------------------------------------------------------

WORD ConvertHexCharToWord (LPTSTR str)
{
	BYTE		Chr;
	UWORD		Tmp=0;
	Chr = ConvertHexCharToBinary(str);		// Convert the high byte (first byte) of the string
   Tmp = (WORD)Chr << 8;						// Set the high byte of Tmp to the converted character
   str += (sizeof(BYTE) * 2);

   Chr = ConvertHexCharToBinary(str);		// Convert the low byte (second byte) of the string
   Tmp += (BYTE)Chr;								// Set the low byte of Tmp to the converted character

  return Tmp;										// Return a pointer to the converted WORD
}


//*---------------------------------------------------------------------------------
//| ConvertHexCharToDWord:
//|	This function will take a pointer to a string and convert it from its
//|	character representation of a hexidecimal(binary) DWORD into the binary
//|	value (DWORD) itself.
//|
//| Parms:
//|	LPTSTR						A pointer to a pointer to the string
//|
//| Returns:
//|		A DWORD which contains the binary value (after the conversion) if successful
//|		This function also sets the LPTSTR pointer to the character in the string
//|	which follows the last character that was converted (the next character to be
//|	converted) in the string.
//*---------------------------------------------------------------------------------
DWORD ConvertHexCharToDWord (LPTSTR str)
{
	BYTE		Chr;
	UDWORD	Tmp=0;

	Chr = ConvertHexCharToBinary(str);		// Convert the highest byte (first byte) of the string
   Tmp = (UDWORD)Chr << 24;					// Set the highest byte of Tmp to the converted character
   str += (sizeof(BYTE) * 2);

   Chr = ConvertHexCharToBinary(str);		// Convert the next highest byte (second byte) of the string
   Tmp += (UDWORD)Chr << 16;					// Set the second byte of Tmp to the converted character
   str += (sizeof(BYTE) * 2);

   Chr = ConvertHexCharToBinary(str);		// Convert the next highest byte (third byte) of the string
   Tmp += (UDWORD)Chr << 8;					// Set the third byte of Tmp to the converted character
   str += (sizeof(BYTE) * 2);

   Chr = ConvertHexCharToBinary(str);		// Convert the lowest byte (fourth byte) of the string
   Tmp += (UDWORD)Chr;							// Set the lowest byte of Tmp to the converted character

  return Tmp;										// Return a pointer to the converted DWORD
}




//*---------------------------------------------------------------------------------
//| GetInputWindowString:
//|	Given an input window, retrive the text from that window and store it
//|	in the parameter desricptor passed in.  If portions of the text are
//|	selected, then take only the selection.  Note that if the window is
//|	empty, it is possible for this function to pass an empty string.
//| Parms:
//|	hwnd						The handle of the input window
//|	pcbLen					The length of the string returned
//| Returns:
//|	Pointer to the string if successful, else NULL
//*---------------------------------------------------------------------------------
LPTSTR INTFUN GetInputWindowString(HWND hwnd, UDWORD * pcbLen)
{
	DWORD			len;
	DWORD			dsel;
	LPTSTR		szBuff=NULL;
	WORD			cbChar=sizeof(TCHAR);

	// Now retrieve length of string and allocate memory for it

	len=GetTrueWindowTextLength(hwnd,-1)+sizeof(TCHAR);


	if(szBuff = (LPTSTR)AllocateMemory(len))
	{
		GetText(hwnd, szBuff);											// Get entire text

		// Check for selection and copy only that much
		dsel = (DWORD)SendMessage(hwnd, EM_GETSEL, 0, 0L);
		if(HIWORD(dsel) - LOWORD(dsel) > 0)
		{					// Only want selected text
			len = (HIWORD(dsel) - LOWORD(dsel)) * cbChar;

			memmove(szBuff, (LPTSTR)&szBuff[LOWORD(dsel)], (size_t)len);
			lstrcpy(&szBuff[len/cbChar],TEXT("\0"));

			if(!GlobalReAllocPtr((LPVOID)szBuff, len+cbChar, GPTR))
				return NULL;
		}

		if(pcbLen)
			*pcbLen = (UDWORD)len;

	}

	return szBuff;

}

//*---------------------------------------------------------------------------------
//| InitToCBODft:
//|	This function will allocate memory and set the value for a parameter
//|	based on which value and size it is.
//| Parms:
//|	lpParm					The parameter to set
//|	sVal						What value to give it?
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
BOOL INTFUN InitToCBODft(lpPARAMITEM lpParm, SWORD sVal)
{
	DWORD		dwReqd=sizeof(SWORD);						// Assume SQL_C_USHORT || SQL_C_SSHORT

	// Determine if a larger value is required
	if(lpParm->fCType == SQL_C_ULONG ||
		lpParm->fCType == SQL_C_SLONG)
		dwReqd = sizeof(SDWORD);

	// Allocate memory if required, if already there, should be enough
	if(!lpParm->lpData)
		lpParm->lpData = AllocateMemory(dwReqd);

	// Based on its size, set the value
	if(lpParm->lpData)
	{
		if(dwReqd == sizeof(SWORD))
			*(SWORD *)lpParm->lpData = sVal;
		else
			*(SDWORD *)lpParm->lpData = (SDWORD)sVal;
	}
	else
		return FALSE;

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| StoreFromCBDftArray:
//|	This function is called when the user selected a value from a combo-box which
//|	is not a CBO_DFT, but is in the user defined default array.  This means
//|	we will need to use the default array to find the value.
//| Parms:
//|	udex						Index to default values
//|	lpParm					Where to store the data
//|	fCType					In anticipation of future requirements where
//|										we won't want lpParm->fCType
//| Returns:
//|	TRUE if successful, FALSE on error
//*---------------------------------------------------------------------------------
BOOL INTFUN StoreFromCBDftArray(UDWORD udex, lpPARAMITEM lpParm, SWORD fCType)
{
	// Switch on the type of the parameter, allocate enough memory for
	//		the value, then store it using udex as an index to the
	//		default arrays.
	switch(fCType)
	{
		case SQL_C_USHORT:
			if(lpParm->lpData = AllocateMemory(sizeof(UWORD)))
			{
				*(UWORD *)lpParm->lpData = (UWORD)lpParm->lpDfts[udex].fOpt;
				lpParm->cBytes = sizeof(UWORD);
			}
			break;

		case SQL_C_SSHORT:
			if(lpParm->lpData = AllocateMemory(sizeof(SWORD)))
			{
				*(SWORD *)lpParm->lpData = (SWORD)lpParm->lpDfts[udex].fOpt;
				lpParm->cBytes = sizeof(SWORD);
			}
			break;

		case SQL_C_ULONG:
			if(lpParm->lpData = AllocateMemory(sizeof(UDWORD)))
			{
				*(UDWORD *)lpParm->lpData = (UDWORD)lpParm->lpDfts[udex].fOpt;
				lpParm->cBytes = sizeof(UDWORD);
			}
			break;

		case SQL_C_SLONG:
			if(lpParm->lpData = AllocateMemory(sizeof(SDWORD)))
			{
				*(SDWORD *)lpParm->lpData = (SDWORD)lpParm->lpDfts[udex].fOpt;
				lpParm->cBytes = sizeof(SDWORD);
			}
			break;

		default:
			ONDEBUG(wsprintf(szOutBuff, szInvalidCType, fCType));
			ASSERTCHECK(TRUE, szOutBuff);
			break;
		}

	return TRUE;
}




//*---------------------------------------------------------------------------------
//| UnModified:
//| 	This function determines if the buffer segment passed in was modified
//| 	since it's initialization to the CHKBYTE value.
//| Parms:
//|	lpBytes					pointer to begining of array segment to check
//|	udchk					number of bytes to verify (size of buffer)
//|
//| Returns:
//|	TRUE if unmodified from initialization, FALSE otherwise
//*---------------------------------------------------------------------------------
BOOL INTFUN UnModified(PTR lpBytes, UDWORD udchk)
{
	UDWORD	i;
	LPBYTE	lpB=lpBytes;
	BYTE		ChkByte=(BYTE)(CHKBYTE);

	if(!lpBytes)
		return FALSE;

	for(i = 0; i < udchk; i++)
	{
		if (lpB[i] != ChkByte)
			return FALSE;
	}

	return TRUE;
}

//*---------------------------------------------------------------------------------
//| InitUnModified:
//| 	This function initializes a buffer to the CHKBYTE value.
//| Parms:
//|	lpBytes					pointer to begining of array segment to initialize
//|	udchk					number of bytes to verify (size of buffer)
//|
//| Returns:
//|	Nothing
//*---------------------------------------------------------------------------------
VOID INTFUN InitUnModified(PTR lpBytes, UDWORD udchk)
{
	if(!lpBytes)
		return;

	memset(lpBytes,CHKBYTE,udchk);

}


//*---------------------------------------------------------------------------------
//| OutputBufferCheck:
//| 	This function displays an error if modifications were made to an
//| 	output buffer when they shouldn't have been, or if no modifications
//| 	were made when they should have been.  Whether or not modifications
//| 	should have been made is determined by the returncode for the
//| 	function and the PT_OUT_MODIFY_OK value which indicates if the
//| 	corresponding pcb value did not determine how many (if any) bytes
//| 	should have been written to the output buffer
//| Parms:
//| 	ci: connection info for output
//| 	rc: returncode
//| 	szName: name of parameter being checked
//| 	buf: buffer being checked
//| 	uFlags: bitmask with data type info
//| 	fVocal: specifies whether or not to display error to output window
//|
//| Returns:
//|	TRUE if buffer was not incorrectly modified
//|   FALSE otherwise
//*---------------------------------------------------------------------------------
BOOL INTFUN OutputBufferCheck(lpCONNECTIONINFO ci, RETCODE rc, LPCTSTR szName, void * buf,
										UDWORD cBytes, UDWORD uFlags, BOOL fVocal)
{
	TCHAR				szMsg[100],
						str[100];
	HWND				hwndOut;                   // For conn win out or generic output
	BOOL				f1=FALSE,
						f2=FALSE,
						f3=FALSE;

	hwndOut = GETOUTPUTWINDOW(ci);

	// We need to check if the buffer was NOT modified and set the flag accordingly
	if(UnModified(buf, cBytes))
		uFlags |= PT_OUT_NOTMODIFIED;

	switch(rc) {
	// cases where the output buffers should be modififed
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
	case SQL_NEED_DATA:
		if ((uFlags & PT_OUT_NOTMODIFIED)
			&& (uFlags & (PT_LOG_OUT | PT_LOG_IO))
			&& !(uFlags & PT_OUT_NOMODIFY_OK))
			{

			if(fVocal) {
				lstrcpy(str, szName);
				RemoveDlgFormat(str);											// Remove the '&' and ':' chars
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1001),
					str);
				}
			return FALSE;
			}
		break;

	// cases where output buffers should not be modified
	default:
		if(buf
			&& !(uFlags & PT_OUT_NOTMODIFIED)
			&& (uFlags & (PT_LOG_OUT | PT_LOG_IO))) {

			if(fVocal) {
				lstrcpy(str, szName);
				RemoveDlgFormat(str);											// Remove the '&' and ':' chars
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1009),
					str);
				}
			return FALSE;
			}

		}
	return TRUE;
}

//*---------------------------------------------------------------------------------
//| InstallerOutputBufferCheck:
//| 	This function displays an error if modifications were made to an
//| 	output buffer when they shouldn't have been, or if no modifications
//| 	were made when they should have been.  Whether or not modifications
//| 	should have been made is determined by the returncode for the
//| 	function and the PT_OUT_MODIFY_OK value which indicates if the
//| 	corresponding pcb value did not determine how many (if any) bytes
//| 	should have been written to the output buffer
//| Parms:
//| 	ci: connection info for output
//| 	rc: returncode
//| 	szName: name of parameter being checked
//| 	buf: buffer being checked
//| 	uFlags: bitmask with data type info
//| 	fVocal: specifies whether or not to display error to output window
//|
//| Returns:
//|	TRUE if buffer was not incorrectly modified
//|   FALSE otherwise
//*---------------------------------------------------------------------------------
BOOL INTFUN InstallerOutputBufferCheck(lpCONNECTIONINFO ci, BOOL fSuccess, LPCTSTR szName, void * buf,
										UDWORD cBytes, UDWORD uFlags, BOOL fVocal)
{
	TCHAR szMsg[100], str[100];
	HWND				hwndOut;                   // For conn win out or generic output

	hwndOut = GETOUTPUTWINDOW(ci);

	// We need to check if the buffer was NOT modified and set the flag accordingly
	if(UnModified(buf, cBytes))
		uFlags |= PT_OUT_NOTMODIFIED;

	if (fSuccess)
	{
		if((uFlags & PT_OUT_NOTMODIFIED)
			&& (uFlags & (PT_LOG_OUT | PT_LOG_IO))
			&& !(uFlags & PT_OUT_NOMODIFY_OK)) {

			if(fVocal) {
				lstrcpy(str, szName);
				RemoveDlgFormat(str);											// Remove the '&' and ':' chars
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1001),
					str);
				}
			return FALSE;
			}
	}
	else
	{
		// cases where output buffers should not be modified
		if(buf
			&& !(uFlags & PT_OUT_NOTMODIFIED)
			&& (uFlags & (PT_LOG_OUT | PT_LOG_IO)))
		{

			if(fVocal)
			{
				lstrcpy(str, szName);
				RemoveDlgFormat(str);											// Remove the '&' and ':' chars
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
					GetGtrString(szMsg, sizeof(szMsg)/sizeof(szMsg[0]), TST1009),	str);
			}

			return FALSE;
		}

	}

	return TRUE;
}


//*---------------------------------------------------------------------------------
//| OutputParamCheck
//| 	This function loops through the params and calls OutputBufferCheck to
//| 	determine if any of the output buffers were incorrectly written to.
//| Parms:
//| 	ci: connection information for printing error messages
//|	rc: functions return code
//|	lpParms: parameter list
//|	cParams: number of parameters
//|
//| Returns:
//|	TRUE
//*---------------------------------------------------------------------------------
BOOL INTFUN OutputParamCheck(lpCONNECTIONINFO ci, RETCODE rc, lpPARAMITEM * lpParms, UINT cParms, BOOL fVocal)
{
	UINT i;
	for(i = 0; i < cParms; i++) {
      OutputBufferCheck(ci, rc, lpParms[i]->szName, lpParms[i]->lpData,
      						lpParms[i]->cBytes, lpParms[i]->uParmType, fVocal);
		// clear modified flags
		lpParms[i]->uParmType  &= ~(PT_OUT_NOTMODIFIED | PT_OUT_NOMODIFY_OK);
	}
	return TRUE;
}


//*---------------------------------------------------------------------------------
//| InstallerOutputParamCheck
//| 	This function loops through the params and calls OutputBufferCheck to
//| 	determine if any of the output buffers were incorrectly written to.
//| Parms:
//| 	ci: connection information for printing error messages
//|	rc: functions return code
//|	lpParms: parameter list
//|	cParams: number of parameters
//|
//| Returns:
//|	TRUE
//*---------------------------------------------------------------------------------
BOOL INTFUN InstallerOutputParamCheck(lpCONNECTIONINFO ci, BOOL fSuccess, lpPARAMITEM * lpParms, UINT cParms, BOOL fVocal)
{
	UINT i;
	for(i = 0; i < cParms; i++) {
      InstallerOutputBufferCheck(ci, fSuccess, lpParms[i]->szName, lpParms[i]->lpData,
      						lpParms[i]->cBytes, lpParms[i]->uParmType, fVocal);
		// clear modified flags
		lpParms[i]->uParmType  &= ~(PT_OUT_NOTMODIFIED | PT_OUT_NOMODIFY_OK);
	}
	return TRUE;
}


#define NULL_CB_VALUE 0xfa
//*---------------------------------------------------------------------------------
//| PtrValue
//| 	This function returns the value pointed to by the lpData field as
//| 	an SDWORD.  It is used for evaluating pcbValue's associated with
//| 	output buffers.
//| Parms:
//| 	lpParm	Pointer to the Paramitem structure containing the data
//|
//| Returns:
//|	numeric value interpreted at end of data ptr.
//*---------------------------------------------------------------------------------
SDWORD INTFUN PtrValue(lpPARAMITEM lpParm)
{

	if(!lpParm || !lpParm->lpData)
		return NULL_CB_VALUE;

	if(lpParm->uParmType & PT_PTR_SWORD)
		return (SDWORD)*(SWORD *)lpParm->lpData;

	if(lpParm->uParmType & PT_PTR_SDWORD)
		return *(SDWORD *)lpParm->lpData;

	if(lpParm->uParmType & PT_PTR_BIN)
		return (SDWORD)*(SDWORD *)lpParm->lpData;

	return 0;
}


//*---------------------------------------------------------------------------------
//| UnusedBufferCheck
//| 	This function determines if the buffer segment passed in should have been
//| 	and was modified since it's initialization to the UNTOUCHED value.
//| Parms:
//| 	ci		Pointer to the ConnectionInfo structure
//|   BufName	Pointer to a string containing the name of the buffer
//|	CBName	Pointer to a string containing the name of the cb buffer
//|	buf		Pointer to the buffer we are interested in checking
//|	bufMax	Size of the buf buffer
//|	pBufuParm	Pointer to a UDWORD containing flags for the parameter
//|	fLastCheck	TRUE if the "extra" byte is to be checked
//|	fVocal		TRUE if messages are to be displayed, FALSE if not
//| Returns:
//|	TRUE if it is unmodified and it is not supposed to be, or vice-versa
//*---------------------------------------------------------------------------------
BOOL INTFUN UnusedBufferCheck(lpCONNECTIONINFO ci, LPCTSTR BufName, LPCTSTR CBName, void * buf,
										UDWORD bufMax, UDWORD *pBufuParm,  SDWORD cb, BOOL fLastCheck,
										BOOL fVocal)
{
	HWND				hwndOut;                   // For conn win out or generic output
	LPTSTR 			szMsg = NULL;
	LPTSTR				str = NULL;

	if(!buf)
		return TRUE;

	szMsg = (LPTSTR)AllocateMemory(lpUsrOptions->uDftBuffLen);
	str = (LPTSTR)AllocateMemory(lpUsrOptions->uDftBuffLen);
	if(!szMsg || !str)
		return TRUE;

	hwndOut = GETOUTPUTWINDOW(ci);

	// We need to check if the buffer was NOT modified and set the flag accordingly
	if(UnModified(buf, bufMax))
		*pBufuParm |= PT_OUT_NOTMODIFIED;

		if(!(*pBufuParm & PT_OUT_NOTMODIFIED)) {
			// check to see that the *extra* byte was not written to
			if(fLastCheck && !UnModified(&((BYTE *)buf)[bufMax], 1))
				if(fVocal) {
					lstrcpy(str, BufName);
					RemoveDlgFormat(str);						// Remove the '&' and ':' chars
					szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE, GetGtrString(szMsg, sizeof(szMsg), TST0009), str);
					}
			// check if buffer was modified beyond returned cb value
			if(cb != NULL_CB_VALUE && cb >= 0 && (UDWORD)cb < bufMax)
			{
				if(!UnModified(&((UCHAR *)buf)[cb + 1],bufMax - cb - 1))
				{
					if(fVocal)
					{
						lstrcpy(str, BufName);
						RemoveDlgFormat(str);						// Remove the '&' and ':' chars
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE, GetGtrString(szMsg, sizeof(szMsg), TST1008), str);
					}

					// Release the memory we allocated before we exit
					FREE_PTR(szMsg);
					FREE_PTR(str);

					return FALSE;
				}
			}
			else
			{
				if(cb < 0 && cb != SQL_NULL_DATA)
				{
					if(fVocal)
					{
						lstrcpy(str, CBName);
						RemoveDlgFormat(str);						// Remove the '&' and ':' chars
						szPrintf(lpUsrOptions, hwndOut, MSG_SEV2, TRUE, TRUE, GetGtrString(szMsg, sizeof(szMsg), TST1010), str);
					}
				}
			}
		}
		else
		{
			if(cb == NULL_CB_VALUE || cb == SQL_NULL_DATA || bufMax == 0)
				// if the cb does not give enough information, the expected modification of the
				// buffer can not be determined.
				*pBufuParm |= PT_OUT_NOMODIFY_OK;
		}

	// Release the memory we allocated
	FREE_PTR(szMsg);
	FREE_PTR(str);
	return TRUE;
}



//*---------------------------------------------------------------------------------
//| NullTerminationCheck:
//| 	This function determines if the buffer is properly NULL terminated
//|
//| Parms:
//|   ci				Pointer to the ConnectionInfo structure
//|	BufName		Pointer to a string containing the name of the buffer
//|   buf			Pointer to the buffer we are interested in checking
//|	bufMax		Size of the buf buffer
//|	cb				The associated cb length value for the buffer
//|	fDouble		TRUE if it is a double-null terminated string
//|	cbValue		this is specifically for those API's that have output buffers
//|					but have no lengths associated with them (i.e. szSQLState in
//|					SQLError, has no cb value.
//| Returns:
//|	TRUE if the buffer is correctly null-terminated
//*---------------------------------------------------------------------------------
BOOL INTFUN NullTerminationCheck(lpCONNECTIONINFO ci, LPCTSTR BufName, LPCTSTR buf,
											UDWORD bufMax, SDWORD cb, UDWORD fDouble, SDWORD cbValue)
{
	HWND						hwndOut;                   // For conn win out or generic output
	LPTSTR					szMsg = NULL;
	LPTSTR					str = NULL;
	LPBYTE					lpBuffer;
	BOOL						fNoError=TRUE;
	lpENVIRONMENTINFO		lpActiveEnv=GetActiveEnv();
	UINT						idsMsg=0;
	UINT						cbChar=sizeof(TCHAR);

	if(!buf || bufMax == 0 || !lpActiveEnv || (*buf == TEXT(CHKBYTE)))
		return TRUE;

	szMsg = (LPTSTR)AllocateMemory(lpUsrOptions->uDftBuffLen);
	str = (LPTSTR)AllocateMemory(lpUsrOptions->uDftBuffLen);
	if(!szMsg || !str)
		return TRUE;

	hwndOut = GETOUTPUTWINDOW(ci);

	if ((UDWORD)cb > bufMax)
		cb=bufMax - cbChar;

	lpBuffer = (LPBYTE)&buf[cb/cbChar];

	// Perform null termination checking
	//if SQL_ATTR_OUTPUT_NTS has been set then we need to check
	//for null terminator otherwise, we need to make sure that
	//the buffer has not been null terminated.
	if (lpActiveEnv->fNTS)
	{
		if (*lpBuffer != '\0' || fDouble && lpBuffer[cb > 0 ? - 1 : 0] != '\0')
			idsMsg=TST1011;
	}
	else
	{
		//buffer shouldn't be null terminated
		if (*lpBuffer == '\0' || fDouble && lpBuffer[cb > 0 ? - 1 : 0] == '\0')
			idsMsg=TST1020;

	}

	if(idsMsg)
	{
		lstrcpy(str, BufName);
		RemoveDlgFormat(str);						// Remove the '&' and ':' chars
		szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE,
			GetGtrString(szMsg, lpUsrOptions->uDftBuffLen, idsMsg), str);
		fNoError = FALSE;
	}

	// If the string was a single-null terminated string AND was terminated correctly,
	// then check for embedded nulls.
	if(fNoError && !fDouble)
	{
		if((cb > 0) && (bufMax > 1))
			lpBuffer--;

		while(lpBuffer != (LPBYTE)buf)
		{
			if(*lpBuffer == '\0')
			{
				// We have an embedded null
				lstrcpy(str, BufName);
				RemoveDlgFormat(str);						// Remove the '&' and ':' chars
				szPrintf(lpUsrOptions, hwndOut, MSG_SEV1, TRUE, TRUE, GetGtrString(szMsg, sizeof(szMsg), TST1012), str);
				fNoError = FALSE;
				break;
			}

			lpBuffer--;
		}
	}

	// Release the memory we allocated
	FREE_PTR(szMsg);
	FREE_PTR(str);

	if(fNoError)
		return TRUE;
	else
		return FALSE;
}


BOOL INTFUN IsValidateFlagSet(HWND hDlg, lpPARAMITEM lpParm, lpCONTROL lpCtl)
{

	// if the control is an rgb value check the pcb control
	// if it is DATA_AT_EXEC do not validate this control i.e set fvalidate to TRUE.
	if(lpCtl->lDataStyle & CBO_VALIDATE_DATA && lpCtl->wLengthID )
	{

		DWORD		idex;
		TCHAR		szText[MAXEXPANDEDCONSTANT];
		//TCHAR		szTmp[MAXEXPANDEDCONSTANT];
		int      id = lpCtl->wLengthID;


	 //if cursel is SQL_DATA_AT_EXEC then set fvalidate flag for wDataID to false.
		idex = (DWORD)SendDlgItemMessage(hDlg,id, CB_GETCURSEL,0,0L);
		if (CB_ERR != idex &&
			CB_ERR !=SendDlgItemMessage(hDlg,id, CB_GETLBTEXT, (WPARAM)idex,(LPARAM)(LPCSTR)szText) )
		{
				if ( !lstrcmp( szText,TEXT("SQL_DATA_AT_EXEC=-2  (3.0)")) ||
					!lstrcmp( szText,TEXT("SQL_NULL_DATA=-1  (3.0)")))
				{
					//actaully flase but have set default to false so this should be set to true
					lpCtl->fValidate = TRUE; //do not validate this control
					return TRUE;
				} else {
                    lpCtl->fValidate = FALSE;
                }
		 }

	}

	return FALSE;
}

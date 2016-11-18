//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	BIGINT.H
//|
//| Purpose:
//|		Contains constants and prototypes for functions in BIGINT.C
//*---------------------------------------------------------------------------------
#ifndef BIGINT_DEFS
#define BIGINT_DEFS


#define CVT_NO_ERROR		0				// if no conversion errors.
#define CVT_TRUNC			1
#define CVT_PREC			2
#define CVT_ERROR			3
#define CVT_DT_ERROR		4
#define CVT_TM_ERROR		5
#define CVT_TS_ERROR		6
#define CVT_ILLEGAL		7

#ifdef WIN32
#define MIN_LIMIT_ADJUST 0
#else
#define MIN_LIMIT_ADJUST 1
#endif

#if defined(WIN32)
#define MATH64

#define MAXPREC 	DWORDLONG	/* Use 64 bit integer arithmetic */
#define MAXCHUNK	ULONG
#define DWLONG	DWORDLONG
#define LLONG	LONGLONG
#else
#define MAXPREC 	ULONG		/* Use 32 bit integer arithmetic */
#define MAXCHUNK	UWORD

typedef struct {
	UDWORD Lower;
	UDWORD Upper;
} DWLONG;

typedef struct {
	UDWORD Lower;
	SDWORD Upper;
} LLONG;

typedef struct dbmoney
{						// Internal representation of MONEY data type
	LONG  mnyhigh;		// Money value *10,000 (High 32 bits/signed)
	ULONG mnylow;		// Money value *10,000 (Low 32 bits/unsigned)
} DBMONEY;

#endif

typedef struct numvalue	{
  INT   len;
  BYTE  sign;
  BYTE *data;
} NUMVALUE;

#ifndef _I64_MIN
/* minimum signed 64 bit value */
#define _I64_MIN    (-9223372036854775807i64 - 1)
#endif

#ifndef _I64_MAX
/* maximum signed 64 bit value */
#define _I64_MAX      9223372036854775807i64
#endif

#ifndef _UI64_MAX
/* maximum unsigned 64 bit value */
#define _UI64_MAX     0xffffffffffffffffui64
#endif



#define BASE_10   10
#define BASE_BITS (8)
#define BASE_MOD (1 << BASE_BITS)


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Prototypes
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

BOOL INTFUN NegateBigint (LLONG  *pValue);
UWORD INTFUN CharToBigint (TCHAR  *charstr, SDWORD srclen,
			DWLONG  *pValue, UWORD Scale, BOOL fUnsigned);
SWORD INTFUN bigint_shift10add (DWLONG  *pValue, SWORD scale,
		BYTE value);
SHORT INTFUN BigintToChar (DWLONG  *, BYTE  *, UWORD, BOOL);
VOID INTFUN numeric_shift10add(NUMVALUE *, BYTE);
VOID INTFUN numeric_add(NUMVALUE *, LONG);
BOOL INTFUN numeric_div10(NUMVALUE  * result, NUMVALUE  * number, ULONG  * rem);
UWORD INTFUN stringtonumeric(TCHAR  *str, SWORD cbstr, SQL_NUMERIC_STRUCT  * retvalue, SDWORD  *);
BOOL INTFUN numerictostring(SQL_NUMERIC_STRUCT  * value, BYTE valuelen, TCHAR  * str, int cbstr);
SDWORD INTFUN FindSigNumber(TCHAR  * *, SDWORD);
BOOL INTFUN IsValidBigInt(TCHAR *charstr,SDWORD srclen,UWORD Scale, BOOL fUnsigned,LPTSTR szFieldName);

#endif


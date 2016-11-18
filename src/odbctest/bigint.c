//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	BIGINT.C
//|
//| Purpose:
//|		Contains tools used to support BIGINT data type
//|
//*---------------------------------------------------------------------------------
#include <float.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "fhheader.h"
#include "bigint.h"
#pragma warning(disable:4996)
/* Max and min values of datatypes */
#define  MAXMONEY	(922337203685477.58075)
#define  MONEYHIGH	(DOUBLE)429496.7296
#define  MONEYMULT	10000
#define	MAX_UBIGINT_PREC 20
#define	MAX_SBIGINT_PREC 19
/* offset of high word in long address, machine dependent */
#define LONGHIWORD      1
#define BIGINT_FAIL				-1
#define BIGINT_SUCCEED			0

extern LPTSTR		szInvalidParm;

/* positions within the bigint structure for msw/lsw values for the mnyhigh and */
/* mnylow elements of the structure, machine dependent, below is the INTEL */
/* model (lsw/msw). */
static BYTE position[4] = { 0,1,2,3 };

/* Digits of 2**16 (65536) */
SHORT	ArC[19] = {6,3,5,5,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* Digits of 2**32 (4,294,967,296) */
SHORT	ArB[19] = {6,9,2,7,6,9,4,9,2,4,0,0,0,0,0,0,0,0,0};

/* Digits of 2**48 (281,474,976,710,656) */
SHORT	ArA[19] = {6,5,6,0,1,7,6,7,9,4,7,4,1,8,2,0,0,0,0};

static unsigned char NumeLenToPrec[SQL_MAX_NUMERIC_LEN] =
{
	2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38
};



//	Bigint conversion functions

/*
** BigintToChar - Converts a bigint to char
*/
SHORT INTFUN BigintToChar
(
DWLONG *bigint,	// the bigint value to convert
BYTE *buf,			// buffer to put it in - must be at least 22 bytes long
UWORD	 scale,			// scale of value (0 for bigint, 4 for money)
BOOL	 fUnsigned		// TRUE if value is unsigned
)
{
	TCHAR   *ptr;
	BOOL	   fPlus;

#ifdef MATH64
	SHORT	   cch;
	DWLONG	value;

	fPlus = TRUE;
	value = *bigint;
	if (!fUnsigned && (LLONG)value < 0)	// if the input value is negative
	{
		fPlus = FALSE;
		value = -(LLONG)value;
	}

	ptr = (LPTSTR)buf;
	cch = 0;
	do {	//	Convert each base 10 digit
		if (scale && cch == scale)
			*ptr++ = TEXT('.');	//	Insert decimal point
		else
		{
			*ptr++ = abs((TCHAR)(value % 10)) + TEXT('0');
			value /= 10;
		}
		cch++;
	} while (value != 0 || (scale && cch <= scale));

#else // MATH64
	LONG	   upper;
	ULONG	   lower;
	LONG	   hold;			/* temporary work area */
	USHORT	   r;				/* temporary variable */
	USHORT	   i;				/* temporary variable */
	USHORT	   j;				/* temporary variable */
	SHORT	   a[5];			/* arrays holding digits */
	SHORT	   b[5];			/* arrays holding digits */
	SHORT	   c[5];			/* arrays holding digits */
	SHORT	   d[5];			/* arrays holding digits */

	fPlus = TRUE;			 /* assume positive result */
	lower = bigint->Lower;
	upper = bigint->Upper;

	if (!fUnsigned && upper < 0)		/* if the input value is negative */
	{
		fPlus = FALSE;
		upper = ~upper; 		/* complement the value */
		if (lower != 0)
			lower = ~lower + 1L;
		else
			upper++;
	}

	/* First divide the bigint value into its digits */

	r = (USHORT)(lower & 0xFFFFL);		/* Lowest 16 bits */

	for (i = 0; i < 5; i++)
	{
		d[i] = r % 10;
		r /= 10;
	}

	r = (USHORT)(((ULONG)lower >> 16) & 0xFFFFL);	/* Next 16 bits */

	for (i = 0; i < 5; i++)
	{
		c[i] = r % 10;
		r /= 10;
	}

	r = (USHORT)(upper & 0xFFFF);	  /* Lowest 16 bits of the higher long*/

	for (i = 0; i < 5; i++)
	{
		b[i] = r % 10;
		r /= 10;
	}

	r = (USHORT)(((ULONG)upper >> 16) & 0xFFFFL);	/* Highest 16 bits */
	for (i = 0; i < 5; i++)
	{
		a[i] = r % 10;
		r /= 10;
	}

	/* Now start calculating from the smallest digit */
	ptr = buf;
	hold = 0L;
	j = 0;

	do {	//	Convert each base 10 digit
		hold += (j < 5) ? d[j] : 0;

		for (i = 0; i <= (USHORT)min(j, scale); i++)
		{
			hold += a[i] * ArA[j-i] + b[i]*ArB[j-i] + c[i] * ArC[j-i];
		}
		if (scale && j == scale)
			*ptr++ = TEXT('.');		/* Insert a decimal point */

		*ptr++ = (TCHAR)(hold % 10 + TEXT('0'));
		hold /= 10;
		j++;
	} while (hold != 0 || (scale && j <= scale));

	ASSERTCHECK (hold == 0,TEXT("BIGINT.C")); 	// Didn't finish conversion

#endif	//	MATH64
	if (!fPlus)			// add sign if input value is negative
		*(ptr++) = TEXT('-');
	*ptr = TEXT('\0');		// Add terminating character
	_strrev (buf);

	return (SHORT)strlen (buf);
}

#ifndef MATH64
/*
** bigint_shift10add (DWLONG *pValue, INT scale, BYTE value)
** This function converts a single digit into a money value, used to convert
** an ascii string into money you must walk down the string passing each
** valid digit to this function, scale ten for single digits, and this
** function will properly fold it into the money value, upscaling the values
** in the money struct by the scale factor, so each digit with a scale of 10
** folds the current digit in, and promotes all the rest by 10.  Higher
** scales are used for the last few decimals if they are not part of the passed
** string.
*/
static SWORD INTFUN bigint_shift10add (
DWLONG *pValue,		/* target value structure pointer */
SWORD	    scale,	    /* scaling factor, usually 10 */
BYTE	    value)    /* value to fold into structure */
{
    UDWORD	temp;	    /* tempory long for catching overflow in calcs */
    UWORD 	*hiword;    /* pointer to high byte of temp for overflow */
    UWORD  *wordptr;   /* pointer into money structure for curr word */
    SWORD x;      /* loop counter */

    hiword = ((UWORD *)&temp)+LONGHIWORD;	/* point to high word of temp */
	wordptr = (UWORD *)pValue;	/* point to start of value struct */

	/* do the calc scaling value up and adding in new value, use long to catch */
    /* any overflow from the operation, only on first word upscale does the */
    /* new value get added in */
    temp = ((SDWORD)*(wordptr+position[0]) * (SDWORD)scale + (SDWORD)value);

    /* point word back into structure */
    *(wordptr+position[0]) = (UWORD)(temp);

    /* now must upscale rest of money value */
	for (x=1; x<4; x++)
    {
		/* upscale it and add in overflow from last upscale, use long to */
		/* catch any overflow from the operation */
		temp = (((SDWORD)*(wordptr+position[x]) * (SDWORD)scale)+(SDWORD)*(hiword));

		/* put it back into value structure */
		*(wordptr+position[x]) = (UWORD)(temp);
    }

    /* if overflow still present, had overflow of money value */
	if (*hiword > 0 || (SWORD)temp < 0)
       return BIGINT_FAIL;

    return BIGINT_SUCCEED;        /* otherwise all done just right */
}

/*
** NegateBigint (LLONG *pValue) -- This function negates a bigint value,
** effectively multiplying it by -1.  If an overflow occurs it returns
** fail otherwise returns succeed
*/
static BOOL INTFUN NegateBigint(
LLONG *pValue)			/* target bigint structure */
{
	BOOL    wasneg = FALSE;
	UNALIGNED DBMONEY *money1 = (DBMONEY *)pValue;

	if (money1->mnyhigh < 0)	/* if the input value is negative */
	{
		wasneg = TRUE;
	}

	money1->mnyhigh = ~(money1->mnyhigh);	 /* and complement the value */

	if (money1->mnylow != 0)
		money1->mnylow = ~(money1->mnylow) + 1L;
	else
		(money1->mnyhigh)++;

	if (wasneg)
	{	// Overflow could only occur if we are converting highest negative number
		if ((money1->mnyhigh == 0) && (money1->mnylow == 0))
			return FALSE;	/* Overflow occured */
	}

	return TRUE;     /* otherwise we completed without error */
}

#endif	//	!MATH64


/*
** CharToBigint -- This function takes a ascii
** null terminated string and converts it to a bigint value.  It walks down
** the structure and passes each char to a conversion routine.  It looks for
** too many -'s, etc. and returns fail if that happens.
*/

static SWORD scale[] =	  /* scale factors depending on how many digits */
{						/* completed */
	10000,
	1000,
	100,
	10
};

UWORD INTFUN CharToBigint (
TCHAR *charstr,		// string to convert
SDWORD		srclen,			// length of source string
DWLONG *pValue,		// target to convert into
UWORD	Scale,				// Scale of target (0 for bigint, 4 for money)
BOOL	fUnsigned)			// TRUE if target is unsigned
{
	TCHAR	c;				/* char for current digit */
	TCHAR *ptr;			/* pointer to source string */
	SWORD	sign = 0;		/* sign byte */
	BOOL	dp = FALSE; 	/* flag for decimal point */
	BOOL	started = FALSE;/* flag for starting, if start then ., fail */
	SWORD	digcount = 0;	/* counter for decimal place digits */
	SWORD	len = 0;	    /* counter for length */
	UWORD	retcode = CVT_NO_ERROR;

#ifdef MATH64
	*pValue = 0;
#else
	pValue->Upper = 0;
	pValue->Lower = 0;
#endif

    ptr = charstr;              /* get pointer to string */

	while (len != srclen && *ptr != TEXT('\0'))	 /* until null */
	{
	    c = *ptr++;             /* get a char */
	    len++;                  /* next character */
	    if (c == TEXT('.'))	/* Handle decimal point separator */
	    {
			if (dp)		/* if already saw one fail */
				return CVT_ERROR;
			dp = TRUE;	 /* decimal encountered */
	    }
	    else			    /* Handle all other cases */
	    {
	        switch(c)
	        {
			  case TEXT('-') :		/* is a negative sign */
				if (sign || started) // already converting or saw one already
					return CVT_ERROR;
				sign = -1;	   /* otherwise is a negative */
				break;

			  case TEXT('+') :		/* is plus */
				if (sign || started) /* already saw one, or started already */
					return CVT_ERROR;
				sign = 1;		 /* else is positive */
				break;

			  default :		/* must be a digit??? */
				if (c >= TEXT('0') && c <= TEXT('9'))
	    	    {
	    	        started = TRUE;	    /* mark as starting conversion */
					if (dp)			/* if decimal digit, count it */
	    	        	digcount++;
					if (digcount > Scale)	 // If to many fractional digits
					{
						if (c != TEXT('0'))
							retcode = CVT_TRUNC;
	    	        	continue;
					}
#ifdef MATH64
					if ((fUnsigned && (DWLONG)(*pValue*10) > (DWLONG)(_UI64_MAX-(c-TEXT('0')))) ||
						(!fUnsigned && (LONGLONG)(*pValue*10) > (LONGLONG)(_I64_MAX-(c-TEXT('0')))))
						return CVT_PREC;
					*pValue = *pValue*10 + (c-TEXT('0'));
#else
					if (bigint_shift10add (pValue, 10, (BYTE)(c-TEXT('0'))) == BIGINT_FAIL)
						return CVT_PREC;
#endif
	    	    }
	    	    else		    /* not a valid number */
					return CVT_ERROR;
	    	    break;
	        }
	    }
    }

	if (Scale && digcount < Scale)
	{	// if less than Scale digits encountered, do scale number
#ifdef MATH64
		if ((fUnsigned && *pValue > _UI64_MAX/(DWLONG)scale[digcount]) ||
			(!fUnsigned && (LLONG)*pValue > _I64_MAX/(LLONG)scale[digcount]))
			return CVT_PREC;
		*pValue *= scale[digcount];
	}
#else
		if (bigint_shift10add (pValue, scale[digcount], 0) == BIGINT_FAIL)
			return CVT_PREC;		   /* complete rest of digits */
    }
#endif

	if (sign < 0)				 /* if negative, negate the money value */
	{
		if (fUnsigned)
			return CVT_PREC;	// Output was to be unsigned
#ifdef MATH64
		if (*(LLONG *)pValue < 0 && *(LLONG *)pValue != _I64_MIN)
#else
		if (pValue->Lower < 0 && pValue->Lower != _I64_MIN)
#endif
			return CVT_PREC;

#ifdef MATH64
		*pValue = -(*(LLONG *)pValue);
#else
		*pValue = -pValue.Lower;
		if (NegateBigint ((LLONG *)pValue) == BIGINT_FAIL)
			return CVT_PREC;
#endif
	}

	return (retcode);				  /* if we got here we are done */
}

//	Numeric conversion routines

/*****************************************************************************
* stringtonumeric                                                            *
*****************************************************************************/

UWORD INTFUN stringtonumeric(TCHAR *str, SWORD cbstr, SQL_NUMERIC_STRUCT * retvalue, SDWORD *pcbOut)
{
	NUMVALUE value;
	INT num;
	INT decpnt = -1;
	INT i, prec;
	SHORT exp;
	UWORD retcode = CVT_NO_ERROR;
	TCHAR chBuff[SQL_MAX_NUMERIC_LEN];
	TCHAR *lpchBuff;

	ASSERTCHECK (retvalue->precision <= SQL_PREC_NUMERIC,NULL);
	ASSERTCHECK (retvalue->scale <= retvalue->precision,NULL);
	// Init fields.
	value.data = retvalue->val;
	value.len = 0;
	memset (retvalue->val, 0, SQL_MAX_NUMERIC_LEN);

	for (i = 0; i < cbstr; i++)
	{
		//	Check for scientific notation
		if ((*(str + i) | 0x20) == TEXT('e'))
		{
			//	If possible scientific notation
			if (!(exp=(SHORT)_ttoi(str+i+1)))
				return (retcode == CVT_TRUNC) ? CVT_ERROR : retcode;

			cbstr = (SWORD)i;	//	Set length to length of value string excluding exponent

			if (exp == 0)
				break;	//	Nothing needs to be done since exp = 0

			if (abs(exp)+i+1 > sizeof(chBuff))
				return CVT_PREC;

			lpchBuff = chBuff;

			if (*str == TEXT('-') || *str == TEXT('+'))
			{	//	If leading sign character
				cbstr--;
				if (*str == TEXT('-'))
				{	//	If negative sign
					*lpchBuff++ = TEXT('-');
				}
				str++;
			}

			if (exp > 0)
			{	//	If positive exponent, shift decimal point to right
				for (i = 0; i < cbstr; i++)
				{	//	Copy leading digits
					if (*(str+i) == TEXT('.'))
						break;
					*lpchBuff++ = *(str+i);
				}
				if (i < cbstr-1)
				{	//	If more after decimal point
					for (i++; i < cbstr && exp; i++, exp--)
						*lpchBuff++ = *(str+i);
				}
				if (exp)
				{	//	Have to add trailing zeros
					memset (lpchBuff, TEXT('0'), exp);
					lpchBuff += exp;
				}
				else
				{	//	Insert decimal point and copy rest of string
					*lpchBuff++ = TEXT('.');	//	insert decimal point
					memmove (lpchBuff, str+i, cbstr-i);
					lpchBuff += cbstr-i;
				}
			}
			else
			{	//	Negative exponent, shift decimal point to left
				exp = -exp;
				for (i = 0; i < cbstr; i++)
				{	//	Find decimal point
					if (*(str+i) == TEXT('.'))
						break;
				}
				if (i < exp)
				{	//	if need leading zeros
					*lpchBuff++ = TEXT('.');	//	insert decimal point
					memset (lpchBuff, TEXT('0'), exp-i);
					lpchBuff += exp-i;
					memmove (lpchBuff, str, i); //	Move digits before decimal
					lpchBuff += i;
				}
				else
				{	//	If just moving decimal point
					memmove (lpchBuff, str, i-exp); //	Move up to new decimal
					lpchBuff += i-exp;
					*lpchBuff++ = TEXT('.');
					memmove (lpchBuff, str+i-exp, exp);
					lpchBuff += exp;
				}
				if (i < cbstr)
					i++;	//	Adjust for decimal point
				cbstr -= (SWORD)i;
				if (cbstr)
				{	//	If more digits after decimal
					memmove (lpchBuff, str+i, cbstr);
				}
				lpchBuff += cbstr;
			}
			str = chBuff;	//	Set new buffer address
			cbstr = (SWORD)(lpchBuff - str);
			break;
		}
	}

	cbstr = (SWORD)FindSigNumber (&str, cbstr);

	// Get sign for numeric value.
	if (*str == TEXT('-'))
	{
		value.sign = 0;
		str++;
		cbstr--;
	}
	else
	{
		value.sign = 1;
		if (*str == TEXT('+'))
		{
			str++;
			cbstr--;
		}
	}

	// Convert string to numeric value by
	// looping through input string.
	for (i = 0, prec = 0; i < cbstr; i++)
	{
		num = *str++;
		if (num >= TEXT('0') && num <= TEXT('9'))
			num -= TEXT('0');
		else if (decpnt < 0 && num == TEXT('.'))
		{	//	If no decimal point marker and one found
			decpnt = prec;
			continue;
		}
		else	// If more than 1 decimal point in string
			return (CVT_ERROR);

		if (prec >= retvalue->precision ||
			(decpnt < 0 && prec >= retvalue->precision - retvalue->scale))
		{	//	If number is too large
			if (decpnt < 0)
				return CVT_PREC;	// Whole number is too large
			retcode = CVT_TRUNC;
			break;
		}
		if (decpnt >= 0 && prec - decpnt >= retvalue->scale)
		{	//	If fraction is too large
			retcode = CVT_TRUNC;
			break;
		}
		numeric_shift10add (&value, (BYTE)num);

		prec++;
	}
	if (prec == 0)
		return CVT_ERROR;	// No digits were found

	if (decpnt < 0) //	If no decimal point found
		decpnt = prec;
	for (; prec-decpnt < retvalue->scale; prec++)
	{	//	Adjust value to required scale
		numeric_shift10add (&value, 0);
	}

	memset (&retvalue->val[value.len], 0, SQL_MAX_NUMERIC_LEN-value.len);
	if (value.len)
	{	//	If non-zero value
		retvalue->sign = value.sign;
		*pcbOut = value.len + 3;
	}
    else
	{	//	Value is zero
		retvalue->sign = 1; //	Force sign to + if 0 value
		*pcbOut = value.len + 4;
	}

	return (retcode);
}

VOID INTFUN numeric_shift10add (
NUMVALUE * value,
BYTE num)
{

	if (value->len <= sizeof (MAXPREC)-1)
	{	//	If operation won't overflow
		*(UNALIGNED MAXPREC *)value->data *= BASE_10;
		*(UNALIGNED MAXPREC *)value->data += num;
	}
	else
	{
		MAXPREC accum = num;
		INT cntr;

		for (cntr = 0; cntr < SQL_MAX_NUMERIC_LEN/sizeof(MAXCHUNK); cntr++)
		{
			accum = accum +
				(MAXPREC)(*(((UNALIGNED MAXCHUNK *)value->data) + cntr)) * BASE_10;
			*(((UNALIGNED MAXCHUNK *)value->data) + cntr) = (MAXCHUNK)accum;
			accum = accum >> sizeof(MAXCHUNK)*8;
		}
	}
	if (value->len < SQL_MAX_NUMERIC_LEN && *(value->data + value->len) != 0)
		value->len++;	//	Adjust length if overflow into next byte

}

/*****************************************************************************
* numeric to string                                                          *
* This set of routines was originally developed by MikeHab.                  *
*****************************************************************************/

BOOL INTFUN numerictostring(SQL_NUMERIC_STRUCT * value, BYTE valuelen, TCHAR * str, int cbstr)
{
	NUMVALUE number;
	int 	 num_digits = 0;
	TCHAR	 tempstr[128];
	TCHAR   * ptr = tempstr;
	BYTE	 tempval[128];
	long	 rem=0;


    /*
    ** Initialize numeric value,
    ** sign and length.
    */
    number.data = tempval;
	memcpy (tempval, value->val, valuelen);

	number.len = valuelen;
	number.sign = value->sign;

    /*
    ** Check for numeric zero case.
    */
	if (valuelen == 1 && value->val[0] == 0)
    {
		number.sign = 1;
		*ptr++ = TEXT('0');
    }
    else
    {
		int i;


		if( !(number.len <=1 && number.data[0] == 0) )
			for (i=number.len;i>=0 && numeric_div10(&number,&number,&rem) ;i--)
			{
				*ptr++ = (BYTE)(rem+TEXT('0'));
				num_digits++;
			}
	 }

    /*
    ** Check if input string is large
    ** enough to hold numeric value.
    */
	if (cbstr < num_digits)
		return(FALSE);

    /*
    ** Build the final numeric string.
    */
	if (!number.sign)
		*str++ = TEXT('-');

    while (num_digits)
    {
		if (num_digits-- == value->scale)
			*str++ = TEXT('.');

		*str++ = *--ptr;
    }
	*str = 0;

    return(TRUE);
}


#ifdef WIN32
#pragma optimize ("g", off) //	Global optimize causes bad results in for loop w/C9.0
#endif
BOOL INTFUN numeric_div10 (
NUMVALUE * result,
NUMVALUE * number,
ULONG * rem)
{
    int	  cntr;
	ULONG accum;


#ifdef MATH64
	if (!*(((UNALIGNED MAXPREC *)number->data) + 1)) // if upper 64 bits are zero
#else
	if (number->len <= sizeof (MAXPREC))	// if size <= 4 bytes
#endif
	{	// If can perform op in one pass

		if( ((ULONG)(*(UNALIGNED MAXPREC *)number->data)) == 0 )
			return FALSE;

		accum = (ULONG)(*(UNALIGNED MAXPREC *)number->data % 10);

		*(UNALIGNED MAXPREC *)result->data = *(UNALIGNED MAXPREC *)number->data / BASE_10;

	}



	else
	{	//	Must loop thru calculating MAXCHUNK bits at a time (ULONG/UWORD)
		//	Code assumes INTEL byte ordering
		MAXPREC value = 0;

		for (cntr = SQL_MAX_NUMERIC_LEN/sizeof(MAXCHUNK)-1; cntr >= 0; cntr--)
		{
			value = (value << (sizeof(MAXCHUNK)*8)) +
				*(((UNALIGNED MAXCHUNK *)number->data) + cntr);

			*(((UNALIGNED MAXCHUNK *)result->data) + cntr) = (MAXCHUNK)(value / BASE_10);
			value = value % BASE_10;
		}
		accum = (ULONG)value;
	}

    /*
    ** Adjust result length.
    */
	for (cntr = result->len-1; cntr > 0 && result->data[cntr] == 0;	cntr--)
		;

    result->len = cntr+1;
    *rem = accum;

    return TRUE;
}

#ifdef WIN32
#pragma optimize("", on)
#endif

SDWORD INTFUN FindSigNumber (
TCHAR **lplpIn,
SDWORD cbIn)
{
	BOOL fNotZero = TRUE;
	SDWORD cbInTmp, cbInTmp1;
	TCHAR *lpIn = *lplpIn;

	for (; cbIn; cbIn--, lpIn++)
	{	//	Skip leading zeros or blanks
		if (fNotZero && *lpIn == TEXT(' '))
			continue;
		if (*lpIn != TEXT('0'))
			break;
		fNotZero = FALSE;
	}
	if (!fNotZero && (!cbIn || *lpIn < TEXT('1') || *lpIn > TEXT('9')))
	{	//	Keep one zero is case '0' is input
		lpIn--;
		cbIn++;
	}
	for (; cbIn; cbIn--)
	{	//	Skip trailing blanks
		if (*(lpIn+cbIn-1) != TEXT(' '))
			break;
	}
	for (cbInTmp = cbIn; cbInTmp; cbInTmp--)
	{	//	Skip insignificant trailing zeros
		if (*(lpIn+cbInTmp-1) != TEXT('0'))
		{
			for (cbInTmp1 = cbInTmp; cbInTmp1; cbInTmp1--)
			{	//	Check to make sure it's a fraction
				if (!isdigit(*(lpIn+cbInTmp1-1)))
				{	//	If non-digit character
					if (cbInTmp != cbIn && *(lpIn+cbInTmp1-1) == TEXT('.'))
					{
						cbIn = cbInTmp;
						if (cbIn == 1)
							lpIn++; 		// Point to '0'
					}
					break;
				}
			}
			break;
		}
	}
	*lplpIn = lpIn;
	return cbIn;
}






// charstr    string to convert
// srclen     length of source string
// pValue     target to convert into
// Scale      Scale of target (0 for bigint, 4 for money)
// fUnsigned  TRUE if target is unsigned

BOOL INTFUN IsValidBigInt(TCHAR *charstr,SDWORD srclen,UWORD Scale, BOOL fUnsigned,LPTSTR szFieldName)
{
	TCHAR	c;						/* char for current digit */
	TCHAR *ptr;					/* pointer to source string */
	SWORD	sign = 0;			/* sign byte */
	BOOL	dp = FALSE; 		/* flag for decimal point */
	BOOL	started = FALSE;	/* flag for starting, if start then ., fail */
	SWORD	digcount = 0;		/* counter for decimal place digits */
   SWORD	len = 0;				/* counter for length */
	DWLONG Value;				/* temp value for big int */
	DWLONG *pValue=&Value;
	TCHAR szBuff[MAXOUTBUFF],
			szOut[MAXOUTBUFF];
	DWORD	dwPrecision;


#ifdef MATH64
	Value = 0;
#else
	pValue->Upper = 0;
	pValue->Lower = 0;
#endif

	ptr = charstr;              /* get pointer to string */

	dwPrecision = fUnsigned ? MAX_UBIGINT_PREC : MAX_SBIGINT_PREC;

	if (_tcschr(ptr,TEXT('-')) && fUnsigned)
		goto InvalidBigInt;

	while (len != srclen && *ptr != TEXT('\0'))	 /* until null */
	{
		c = *ptr++;             /* get a char */
		len++;                  /* next character */
	    if (c == TEXT('.'))	/* Handle decimal point separator */
	    {
			if (dp)		/* if already saw one fail */
				goto InvalidBigInt;
			dp = TRUE;	 /* decimal encountered */
	    }
	    else			    /* Handle all other cases */
	    {
			switch(c)
			{
				case TEXT('-') :		/* is a negative sign */
				if (sign || started) // already converting or saw one already
					goto InvalidBigInt;

				sign = -1;	   /* otherwise is a negative */
				break;

				case TEXT('+') :		/* is plus */
					if (sign || started) /* already saw one, or started already */
						goto InvalidBigInt;
				sign = 1;		 /* else is positive */
				break;

			  default :		/* must be a digit??? */
				if (c >= TEXT('0') && c <= TEXT('9'))
	    	    {
					started = TRUE;	    /* mark as starting conversion */

					if (dp)			/* if decimal digit, count it */
	    	        	digcount++;

					if (digcount > Scale)	 // If to many fractional digits
					{
						if (c != TEXT('0'))
							goto InvalidBigInt;
	    	        	continue;
					}
#ifdef MATH64
					if ((fUnsigned && Value > _UI64_MAX/10) ||
						(!fUnsigned && Value > _I64_MAX/10))
						goto InvalidBigInt;

					Value = Value*10 + (c-TEXT('0'));
#else
					if (bigint_shift10add (pValue, 10, (BYTE)(c-TEXT('0'))) == BIGINT_FAIL)
						goto InvalidBigInt;
#endif
	    	    }
	    	    else		    /* not a valid number */
					 goto InvalidBigInt;
	    	    break;
	        }
	    }

    } //end while

	return(TRUE);

InvalidBigInt:

    {
    LPTSTR pszNewFieldName = _tcsdup(szFieldName);

	RemoveDlgFormat(pszNewFieldName);

	GetGtrString(szOut, MAXOUTBUFF, idsInvalidBigInt);
	wsprintf(szBuff,szOut,pszNewFieldName,dwPrecision);
    free(pszNewFieldName);

	szMessageBox(GetActiveWindow(),
				MB_ICONINFORMATION | MB_OK,
				szInvalidParm, szBuff);
    }

	return(FALSE);

} //IsValidBigInt()

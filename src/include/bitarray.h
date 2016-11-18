/*
**	Simple bitmap functions.  Assume that a long is four words
**
**
**	BITARRAYSIZE:	How big of a BITCELL array to declare to handle n bits
**	BITTEST:		Return TRUE if a bit is on
**	BITSET:			Set a bit on
**	BITCLEAR:		Set a bit off
*/

#ifndef BITARRAY_DEFS
#define BITARRAY_DEFS


// Following macros are used to decide how many UINTs are required to have enough
// bits to represent (items) of data.  
#define CBITS (sizeof(UINT) * 8)
#define BitSet(lpa, pos)	\
	(lpa[((pos) / CBITS)] |= 1 << ((pos) - (CBITS * ((pos) / CBITS))))
#define BitClear(lpa, pos)	\
	(lpa[((pos) / CBITS)] &= ~(1 << ((pos) - (CBITS * ((pos) / CBITS)))))
#define BitGet(lpa, pos)	\
	(lpa[((pos) / CBITS)] & (1 << ((pos) - (CBITS * ((pos) / CBITS)))))
#define BitFlip(lpa,pos)	\
	(BitGet(lpa,pos)) ? BitClear(lpa,pos) : BitSet(lpa,pos)
#define MINREQUIREDINTS(items) (UINT)((items / CBITS) + ((items % CBITS) ? 1 : 0))


#define getbit(p1,p2) BitGet(p1,(p2)-1)
#define setbit(p1,p2) BitSet(p1,p2)


#endif


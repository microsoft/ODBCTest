//*---------------------------------------------------------------------------------
//|  Title:		GATORTST.H
//|		Copyright (c) Microsoft Corporation, All rights reserved
//|
//|	This file contains all of the headers which each module needs to include.  It
//|	also contains constants for conditional compilation.
//*---------------------------------------------------------------------------------
#ifndef GATORTST_DEFS
#define GATORTST_DEFS

//----------------------------------------------------------------------------------
// Name resolution for compilcation objects is different from C to C++.  Because
//	of this, if we create a DLL using C and need to find it in a C++ program,
//	we must override the default C++ naming method to C so that it can be found.
//	The following will do so for a C++ program.  All prototypes between the {
//	and } are handled this way.
//----------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {                         /* Assume C declarations for C++ */
#endif	/* __cplusplus */

#include "gtrhdrs.h"


//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------

#ifdef WIN32
#define SEGMENT LPVOID
#else
#define SEGMENT _segment
#define SELF    _self
#endif




//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------


#ifdef __cplusplus
}                                    /* End of extern "C" { */
#endif	/* __cplusplus */

#endif


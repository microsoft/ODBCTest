//*---------------------------------------------------------------------------------
//|  Title:		SRCPICK.H
//| Copyright (c) Microsoft Corporation
//|
//|	This file contains defines and macros for the SRCPICK functions which allow
//|	the user to add and remove sources from GATOR.INI.
//*---------------------------------------------------------------------------------
#ifndef SRCPICK_DEFS
#define SRCPICK_DEFS

//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
#define IDD_MANAGE_SOURCES             130
#define IDD_NEW_SOURCE						131

#define IDE_SOURCE							215

#define IDC_SOURCES							200
#define IDC_DSN0								201
#define IDE_UID0								202
#define IDE_PWD0								203
#define IDE_KEYWORDS							204
#define IDT_DSN0								205
#define IDT_UID0								206
#define IDT_PWD0								207
#define IDT_KEYWORDS							208
#define IDT_PARMS								209
#define IDT_TEST_SOURCES					210
#define FIRST_SRC_CONTROL				   201
#define LAST_SRC_CONTROL					209


//----------------------------------------------------------------------------------
//		Functions
//----------------------------------------------------------------------------------
void EXTFUN ManageSources(HWND hwnd, HINSTANCE hInst);


#endif

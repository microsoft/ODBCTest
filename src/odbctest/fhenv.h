//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	FHENV.H
//|
//| Purpose:
//|		This header file contains the function prototypes in fhenv.c
//*---------------------------------------------------------------------------------

#ifndef FHENV_DEFS
#define FHENV_DEFS

VOID INTFUN SetCurrentHdbcFromList(lpENVIRONMENTINFO lpei);
BOOL INTFUN DropAllConns(lpENVIRONMENTINFO lpei, BOOL fNeedToFree);
VOID INTFUN DoDropConn(lpENVIRONMENT ei);
lpENVIRONMENTINFO INTFUN FindActiveEnvNode(lpENVIRONMENTINFO lpEnv,lpCONNECTIONINFO lpActiveConnWin);

#endif

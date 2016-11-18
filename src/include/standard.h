//*---------------------------------------------------------------------------------
//|  Title:		STANDARD.H
//|	Copyright (c) Microsoft, All rights reserved
//|
//|	This file contains defines and macros which are generic in nature, that is
//|	they do not relate to our functions, but are for any file to use.
//*---------------------------------------------------------------------------------
#ifndef STANDARD_DEFS
#define STANDARD_DEFS


//----------------------------------------------------------------------------------
//		Defines and Macros
//----------------------------------------------------------------------------------
typedef VOID  * lpVOID;
                  		
#define NumItems(s) (sizeof(s) / sizeof(s[0]))


// Dialog macros
#define RemoveListBoxString(hdlg, id, string)													\
	{                                                                                \
	HWND hwnd=GetDlgItem(hdlg, id);                                                  \
	SendMessage(hwnd, LB_DELETESTRING,                                               \
		(WPARAM)SendMessage(hwnd, LB_FINDSTRINGEXACT,                                 \
					(WPARAM)-1, (LPARAM)(LPTSTR)string),     										\
		0L);                                                                          \
	}
#define GetText(h,s) 																					\
	{																											\
	SendMessage(h, WM_GETTEXT, 																		\
					(WPARAM)SendMessage(h, WM_GETTEXTLENGTH, 0, 0L) + 1,						\
					(LPARAM)(LPTSTR)s);																	\
	}
#define GetLBText(h, s)																					\
	{                                                                    				\
	WPARAM		dex;                                                                 \
	if(LB_ERR == (dex = (WPARAM)SendMessage(h, LB_GETCURSEL, 0, 0L)))                \
		*s = '\0';                                                                    \
	else                                                                             \
		SendMessage(h, LB_GETTEXT, dex, (LPARAM)(LPTSTR)s);                            \
	}
#define GetCBText(h,s)																					\
	{                                                                                \
	WPARAM		dex;                                                                 \
	if(CB_ERR == (dex = (WPARAM)SendMessage(h, CB_GETCURSEL, 0, 0L)))                \
		*s = '\0';                                                                    \
	else                                                                             \
		SendMessage(h, CB_GETLBTEXT, dex, (LPARAM)(LPTSTR)s);                          \
	}
#define IsRadioButtonOn(hwnd) SendMessage(hwnd, BM_GETCHECK, 0, 0L)


#endif

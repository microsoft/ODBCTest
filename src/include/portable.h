//*-----------------------------------------------------------------------
//| Copyright (c) Microsoft Corporation
//|	This include contains extra macros for dealing with portability
//|		issues.  Whenever possible, use the version defined by Windows.
//|
//|	Notes:
//|	 o	Time functions:
//|		 -	Always allocate a TIMEINFO structure and use the GetTime
//|				macro to find the actual time
//|		 -	The szGetTime macro will always return a string version of
//|				the time
//*-----------------------------------------------------------------------
#ifndef PORTABLE_DEFS
#define PORTABLE_DEFS

#ifndef RC_INVOKED
#include <windows.h>

#ifdef	VG_UNIX
#include <time.h>
#define	wsprintf	sprintf
#endif

//*-----------------------------------------------------------------------------------
//|	Win32 specific macros
//*-----------------------------------------------------------------------------------
#ifdef WIN32
#include <windowsx.h>

//
// These macros will take an address and put it on a correct alignment by
// padding bytes if required.  These work by adding the max-1 and
// truncating bits from the number to align it correctly.
//
#define ALIGNON4BYTE(ptr)																	\
  { 																								\
  ULONG l;																						\
  l = (ULONG) ptr;																			\
  l +=3;																							\
  l >>=2;																						\
  l <<=2;																						\
  ptr = (LPVOID) l;																			\
  }
#define ALIGNON8BYTE(ptr)																	\
  { 																								\
  ULONG l;																						\
  l = (ULONG) ptr;																			\
  l +=7;																							\
  l >>=3;																						\
  l <<=3;																						\
  ptr = (LPVOID) l;																			\
  }

#define GETADDRINFO(ptr)										(ptr)


// UNICODE
//
// Added support for Win95's lack of unicode support. Instead of using the
// Windows versions of routines, use the C-runtime library - more likely to work!
#ifdef _UNICODE
#undef	lstrcpy
#undef	lstrcat
#undef	lstrcmp
#undef	lstrcmpi
#undef	strdup
#undef	strtok


#define	strtok		_tcstok
#define	strdup		_tcsdup
#define	lstrcpy		_tcscpy
#define	lstrcat		_tcscat
#define	lstrcmpi	_tcsicmp
#define	lstrcmp		_tcscmp

#undef	wsprintf
#define	wsprintf	_stprintf
#undef  wvsprintf
#define wvsprintf	_vstprintf
#undef	AnsiNext
#define	AnsiNext(x)	(x + 1)
#undef	AnsiPrev
#define	AnsiPrev(y,x)	(x - 1)
#endif



// Memory overrides
#ifdef VG_MAC
// Change EXTFUNCDECL & EXTFUN for MAC
#define EXTFUNCDECL                     CDECL
#define EXPFUN                          __declspec(dllexport)

#ifdef VG_PPC
#define EXTFUN                          CDECL
#define INTFUN                          CDECL
#else
#define EXTFUN                          PASCAL
#define INTFUN                          PASCAL
#endif

#else	/* ! VG_MAC */

#define EXPFUN          __declspec(dllexport)
#define EXTFUNCDECL             _stdcall
#define EXTFUN                          _stdcall
#define INTFUN                          PASCAL
#endif	/* ! VG_MAC */

#define dCSEG(type) const type

//#define dCSEG(type) static type
#define WRITE	OF_WRITE

// Window attributes
#define GETWINDOWLONG(x) GetWindowLongPtr(x, GWLP_USERDATA)
#define SETWINDOWLONG(x, y) SetWindowLongPtr(x, GWLP_USERDATA, (LONG_PTR)(y))
#define PORT_GETCOLOR_BTN_HIGHLIGHT							GetSysColor(COLOR_BTNHIGHLIGHT)

// Graphics
#define MoveTo(hdc,x,y)											MoveToEx(hdc, x, y, NULL)
#define GETTEXTEXTENT(hdc, str, len, outval)									\
		{                                                              \
		SIZE		tmp;                                               	\
		tmp.cx = tmp.cy = 0;                                           \
		GetTextExtentPoint(hdc, str, len, &tmp);                       \
		outval = tmp.cx;                                               \
		}

// Edit window messages
#ifdef VG_MAC
#define MAX_EDIT_WINDOW_TEXT										(30000)
#else
#define MAX_EDIT_WINDOW_TEXT                                        (60000)
#endif
#define PORT_EM_SETSEL(start,end) 							(WPARAM)(start), (LPARAM)(end)

// WM_COMMAND macros for sending
#define PORT_WM_COMMAND_SEND(id,parm) 						(MAKELPARAM(id,0)), parm
#define GET_WM_COMMAND_ID(wp, lp)               		LOWORD(wp)
#define GET_WM_COMMAND_CODE(wp, lp)							HIWORD(wp)
//#define GET_WM_COMMAND_HWND(wp, lp)							((HWND)lp)

// Time macros
typedef SYSTEMTIME TIMEINFO;
typedef TIMEINFO * lpTIMEINFO;
#define GetTime(t)												GetLocalTime(t)
#define szGetTime(buf,t) 														\
			((wsprintf(buf, TEXT("%u:%u:%u.%u"), (WORD)t.wHour, (WORD)t.wMinute, (WORD)t.wSecond, (WORD)(t.wMilliseconds*10))) ? buf : TEXT("#error#"))
#define GetTimeSeconds(t)										(t.wSecond)
#define PORT_HOUR													wHour
#define PORT_MINUTE												wMinute
#define PORT_SECOND												wSecond
#define PORT_HSECOND												wMilliseconds

// Date macros
typedef TIMEINFO DATEINFO, * lpDATEINFO;
#define GetDate(t)												GetTime(t)
#define szGetDate(buf, t) \
	((wsprintf(buf, TEXT("%04u-%02u-%02u"), (UWORD)t->wYear, (UWORD)t->wMonth, (UWORD)t->wDay)) ? (LPSTR)buf : (LPSTR)(TEXT("#error#")))
#define szGetMonthDay(buf, t) \
	((wsprintf(buf, TEXT("%02u-%02u"), (UWORD)t->wMonth, (UWORD)t->wDay)) ? (LPSTR)buf : (LPSTR)(TEXT("#error#")))

// Scrolling macros
#define PORT_SEND_WM_VSCROLL(code,pos,hwnd)				(MAKELONG(code,pos)),((LPARAM)(hwnd))

// Menu macros
//#define GET_WM_MENUSELECT_FLAGS(wp,lp)						((UINT)HIWORD(wp))
//#define GET_WM_MENUSELECT_HMENU(wp,lp)						((HMENU)(lp))
#define GET_WM_MENUSELECT_ID(wp,lp)							((UINT)LOWORD(wp))

// stdlib functions
#ifndef _MAC
#define _atold atof
#endif



//*-----------------------------------------------------------------------------------
//|	Win 3.1 macros
//*-----------------------------------------------------------------------------------
#else

#ifndef RC_INVOKED
#ifndef VG_UNIX
#include <dos.h>
#include <direct.h>
#endif
#include <stdio.h>
#endif

// Macro is included for compiler purposes only
#define ALIGNONDWORD(ptr)
#define GETADDRINFO(ptr)										HIWORD(ptr), LOWORD(ptr)
// #define RUNNING_ON_WIN32S										((BOOL)(FALSE))

// Memory overrides
#ifdef  VG_UNIX
#define dCSEG(type) const type
#else
#define dCSEG(type) const type _based(_segname("_CODE"))
#endif
#define EXPFUN				__export
#define EXTFUNCDECL		__export
#define EXTFUN				__export FAR PASCAL
#define INTFUN				PASCAL
typedef int					INT;

// Window attributes
//		Note that COLOR_BTN_HIGHTLIGHT is not valid under Win 3.0
#define GETWINDOWLONG(x)										GetWindowLong(x, DWL_USER)
#define SETWINDOWLONG(x, y)     								SetWindowLong(x, DWL_USER, (LONG)y)
#define PORT_GETCOLOR_BTN_HIGHLIGHT							(COLORREF)(16777215)

// Graphics
#define GETTEXTEXTENT(hdc, str, len, outval)				outval = GetTextExtent(hdc, str, len)

// Edit window messages
#define MAX_EDIT_WINDOW_TEXT									(60000)
#define PORT_EM_SETSEL(start,end) 							(TRUE), (MAKELONG(start,end))

// WM_COMMAND macros for sending
#define PORT_WM_COMMAND_SEND(id,parm) 						(id), (parm)
#define GET_WM_COMMAND_ID(wp,lp)								(wp)
#define GET_WM_COMMAND_CODE(wp, lp)							HIWORD(lp)
#define GET_WM_COMMAND_HWND(wp, lp)							((HWND)LOWORD(lParam))

// Time macros
#ifdef  VG_UNIX
extern  time_t  time_t_v;   /*  from windows.c */
#define SYSTEMTIME  struct tm

#if     defined(VG_SUNOS)
#define GetLocalTime(t) memcpy(t,localtime((time(&time_t_v),&time_t_v)), sizeof(struct tm))
#elif   defined(VG_SOLARIS)
#define GetLocalTime(t) localtime_r((time(&time_t_v),&time_t_v),t)
#elif   defined(VG_HPUX) | defined(VG_AIX)
#define GetLocalTime(t) *((struct tm *)t) = *((struct tm *)localtime((time(&time_t_v),&time_t_v)))
#else	/* generic Unix time */
void nonsunLocalTime(struct tm *ttm)
{
    time_t      time_t_v;
    struct tm * tmptr;

    time(&time_t_v);
    tmptr = localtime(&time_t_v);
    memcpy(ttm, tmptr, sizeof(struct tm));
}
#define GetLocalTime    nonsunLocalTime
#endif	/* generic Unix time */

typedef SYSTEMTIME TIMEINFO;
typedef TIMEINFO * lpTIMEINFO;
#define GetTime(t)          GetLocalTime(t)
#define szGetTime(buf,t)    ((wsprintf(buf, TEXT("%02d:%02d:%02d.%02d"), \
                                     (int)t.tm_hour, (int)t.tm_min, \
                                     (int)t.tm_sec, 0)) \
                                     ? buf : TEXT("#error#"))
#define GetTimeSeconds(t)   (t.tm_sec)

#else	/* ! VG_UNIX */

typedef struct dostime_t TIMEINFO;
typedef TIMEINFO FAR * lpTIMEINFO;
#define GetTime(t) 												_dos_gettime(t)
#define szGetTime(buf,t) \
	((wsprintf(buf, TEXT("%02d:%02d:%02d.%02d"), (int)t.hour, (int)t.minute, (int)t.second, (int)t.hsecond)) ? (LPSTR)buf : (LPSTR)(TEXT("#error#")))
#define PORT_HOUR													hour
#define PORT_MINUTE												minute
#define PORT_SECOND												second
#define PORT_HSECOND												hsecond
#define GetTimeSeconds(t)										(t.hsecond)

#endif	/* ! VG_UNIX */

// Date macros
#ifndef VG_UNIX
typedef struct _dosdate_t DATEINFO, FAR * lpDATEINFO;
#define GetDate(t)												_dos_getdate(t)
#define szGetDate(buf, t) \
	((wsprintf(buf, TEXT("%04u-%02u-%02u"), (UWORD)t->year, (UWORD)t->month, (UWORD)t->day)) ? (LPSTR)buf : (LPSTR)(TEXT("#error#")))
#define szGetMonthDay(buf, t) \
	((wsprintf(buf, "%02u-%02u", (UWORD)t->month, (UWORD)t->day)) ? (LPSTR)buf : (LPSTR)(TEXT("#error#")))
#else	/* VG_UNIX */
typedef TIMEINFO DATEINFO, * lpDATEINFO;
#define GetDate(t)  GetTime(t)
#define szGetDate(buf, t) \
    sprintf(buf, TEXT("%04u-%02u-%02u"), (UWORD)t->tm_year, (UWORD)t->tm_mon, (UWORD)t->tm_mday)
#define szGetMonthDay(buf, t) \
    ((WSPRINTF((buf, TEXT("%02u-%02u"), (UWORD)t->tm_mon, (UWORD)t->tm_mday))) ? (LPSTR)buf : (LPSTR)(TEXT("#error#")))
#endif	/* VG_UNIX */

// Scrolling macros
#define GET_WM_VSCROLL_CODE(wp,lp)							(wp)
#define GET_WM_VSCROLL_POS(wp,lp)							(LOWORD(lp))
#define GET_WM_VSCROLL_HWND(wp,lp)							((HWND)HIWORD(lp))
#define PORT_SEND_WM_VSCROLL(code,pos,hwnd)				(code),(MAKELONG(pos,hwnd))
#define GET_WM_HSCROLL_CODE(wp,lp)							(wp)
#define GET_WM_HSCROLL_POS(wp,lp)							(LOWORD(lp))
#define GET_WM_HSCROLL_HWND(wp,lp)							((HWND)HIWORD(lp))

// Menu macros
#define GET_WM_MENUSELECT_FLAGS(wp,lp)						((UINT)LOWORD(lp))
#define GET_WM_MENUSELECT_HMENU(wp,lp)						((HMENU)HIWORD(lp))
#define GET_WM_MENUSELECT_ID(wp,lp)							((UINT)wp)

// File functions not present in Win 3.1, but present in Win32
#define GetCurrentDirectory(len, buf)						_getcwd(buf, len)
#define SetCurrentDirectory(buf)								_chdir(buf)
#define DeleteFile(szfile)									remove(szfile)
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))

// The __unalign declaration is a no op on 16-bit
#define UNALIGNED

// End of Win 3.1 macros
#endif

// Use the following to determine if we're on Win32s
#define RUNNING_ON_WIN32S ((BOOL)((GetVersion() & 0x80000000L) ? TRUE : FALSE))

// Use the following to determine if we are running true 32bit 
// This was added to determine support for long filenames.
// NT, Win95, Cairo will return TRUE, WIN32S will return FALSE!
#ifndef WIN32
#define ISTRUE32WIN() FALSE
#else
#define ISTRUE32WIN() (((LOBYTE(LOWORD(GetVersion())) <= 3) && RUNNING_ON_WIN32S) ? FALSE : TRUE )
#endif

// End of RC_INVOKED
#endif

// End of include rule
#endif


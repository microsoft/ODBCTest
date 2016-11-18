TARGETTYPE=PROGRAM
!INCLUDE $(PROJROOT)\user.mk
EXEDEF=..\gator32.def

UMTYPE=windows
USE_MSVCRT=1
USE_ATL=1
ATL_VER=30
LOCALIZE_NO_MUI=0

C_DEFINES=$(C_DEFINES)

TARGETLIBS=\
        $(SDK_LIB_PATH)\kernel32.lib \
        $(SDK_LIB_PATH)\gdi32.lib \
        $(SDK_LIB_PATH)\comdlg32.lib \
        $(SDK_LIB_PATH)\comctl32.lib \
        $(SDK_LIB_PATH)\rpcrt4.lib \
        $(SDK_LIB_PATH)\htmlhelp.lib \
        $(SDK_LIB_PATH)\odbc32.lib \
        $(SDK_LIB_PATH)\oleaut32.lib \
        $(SDK_LIB_PATH)\odbccp32.lib

INCLUDES=..;..\..\include;$(INCLUDE)

SOURCES=\
        ..\gator.rc \
        ..\bigint.c \
        ..\blddata.c \
        ..\connwin.c \
        ..\dlgbldr.c \
        ..\dlgmngr.c \
        ..\dlgparms.c \
        ..\dlgtmplt.c \
        ..\dlgtools.c \
        ..\fhattr.c \
        ..\fhbind.c \
        ..\fhcatl.c \
        ..\fhconn.c \
        ..\fhdesc.c \
        ..\fhdiag.c \
        ..\fhenv.c \
        ..\fhhndl.c \
        ..\fhinst.c \
        ..\fhlocatr.c \
        ..\fhmisc.c \
        ..\fhrslt.c \
        ..\fhstmt.c \
        ..\fileio.c \
        ..\globals.c \
        ..\gtrcomm.c \
        ..\main.c \
        ..\mem.c \
        ..\optwin.c \
        ..\outwin.c \
        ..\picktype.c \
        ..\rsltwin.c \
        ..\runtest.c \
        ..\statbar.c \
        ..\table.c \
        ..\toolbar.c \
        ..\usrwndp.c

UMENTRYABS=WinMainCRTStartup

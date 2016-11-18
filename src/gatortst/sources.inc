TARGETTYPE=DYNLINK
!INCLUDE $(PROJROOT)\user.mk


PRECOMPILED_INCLUDE=..\gtrhdrs.h

UMTYPE=windows

USE_LIBCMT=1
USE_ATL=1

C_DEFINES=$(C_DEFINES)

TARGETLIBS=\
        $(SDK_LIB_PATH)\kernel32.lib \
        $(SDK_LIB_PATH)\gdi32.lib \
        $(SDK_LIB_PATH)\user32.lib \
        $(SDK_LIB_PATH)\comdlg32.lib \
        $(SDK_LIB_PATH)\mpr.lib \
        $(SDK_LIB_PATH)\odbc32.lib

INCLUDES=..;..\..\include

SOURCES=\
        ..\gatortst.rc \
        ..\commtest.c \
        ..\dllpick.c \
        ..\file.c \
        ..\gatortst.c \
        ..\grppick.c \
        ..\runtest.c \
        ..\srcpick.c \
        ..\udl.c

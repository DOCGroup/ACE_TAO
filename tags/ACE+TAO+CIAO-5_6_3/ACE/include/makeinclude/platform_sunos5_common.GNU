# -*- Makefile -*-

# $Id$

# SunOS 5.x common settings

ifeq ($(shell /bin/uname -r),5.9)
  # Solaris 9 puts sendfile() in a separate library.
  LIBS          += -lsendfile
endif
ifeq ($(shell /bin/uname -r),5.10)
  # Solaris 10 puts sendfile() in a separate library.
  LIBS          += -lsendfile
endif
ifeq ($(shell /bin/uname -r),5.11)
  # Solaris 11 puts sendfile() in a separate library.
  LIBS          += -lsendfile
endif

PLATFORM_X11_CPPFLAGS= -I/usr/openwin/include -I/usr/dt/include
PLATFORM_X11_LIBS =-lX11
PLATFORM_X11_LDFLAGS=

PLATFORM_TK_CPPFLAGS=
PLATFORM_TK_LIBS=-ltk -ltcl
PLATFORM_TK_LDFLAGS=

# TODO: Fix the parameters
#PLATFORM_TCL_CPPFLAGS=-I/usr/include/tcl8.4
#PLATFORM_TCL_LIBS=-ltcl8.4
#PLATFORM_TCL_LDFLAGS=



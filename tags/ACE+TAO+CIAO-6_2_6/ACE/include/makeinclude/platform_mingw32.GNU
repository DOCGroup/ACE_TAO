# -*- Makefile -*-
# $Id$
#
# This file should allow to build ACE for mingw32 with mingw tools.

ACE_PLATFORM_CONFIG ?= config-win32.h

mingw32 = 1

# MinGW has wfmo and registry
wfmo ?= 1
winregistry ?= 1

# We can use the QoS support
qos ?= 1

PWD=$(subst \,/,$(shell pwd))

ifeq (cmd,$(findstring cmd,$(SHELL)))
  CMDSEP = &
  vxworks_ntbuild = 1
  MKDIR_NTSYTLE = 1
  ACE_MKDIR = mkdir
  ECHO_BLANK_LINE = cmd /c echo.
  PRELIB:=
endif

include $(ACE_ROOT)/include/makeinclude/platform_gnuwin32_common.GNU

PLATFORM_SSL_LIBS ?= -lssl32 -leay32

ifeq ($(winsock2),1)
  LIBS += -lws2_32 -lmswsock -liphlpapi
endif

LIBS += -lwsock32 -lnetapi32

ifeq ($(GXX_4_OR_BETTER),1)
  ifeq ($(CXX_MAJOR_VERSION),4)
    ifeq ($(findstring $(CXX_MINOR_VERSION),7 8 9),$(CXX_MINOR_VERSION))
      CPPFLAGS += -Wno-unknown-pragmas
    endif
  else
    CPPFLAGS += -Wno-unknown-pragmas
  endif
endif

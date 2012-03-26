# -*- Makefile -*-
# $Id$
#
# This file should allow to build ACE for mingw32 with mingw tools.
#

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

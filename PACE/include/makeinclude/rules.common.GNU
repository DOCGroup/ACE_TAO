#----------------------------------------------------------------------------
#	$Id$
#
#	Common rules for building PACE targets
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/PACE
endif # ! PACE_ROOT

VPATH:=common:arpa:config:netinet:sys

ifeq ($(PACE_SYSNAME),)
  PACE_SYSNAME = $(shell uname -s)
endif # ! PACE_SYSNAME

ifeq ($(PACE_SYSVER),)
  PACE_SYSVER = $(shell uname -r)
endif # ! PACE_SYSVER

PACE_SYSVER := $(shell echo $(PACE_SYSVER) | sed 's/\.//g')

# List of platforms and uname -s values
# =====================================
# Solaris           SunOS
# NT                "NT" (uname not supported)
# LynxOS            LynxOS
# VxWorks           "VxWorks" (strictly cross-compiled)
# Linux             Linux
# pSOS              "pSOS" (strictly cross-compiled)
# Chorus            "Chorus" (strictly cross-compiled)
# QNX Neutrino      "Neutrino" (strictly cross-compiled)
# OSF/1             OSF1
# IRIX (64 bit)     IRIX64
# IRIX (32 bit)     IRIX (unconfirmed)
# HPUX              HP-UX

# Solaris is POSIX - change this when the platform directories get
# squared away
#ifeq ($(PACE_SYSNAME),SunOS || $(PACE_SYSNAME),Linux)

ifeq ($(PACE_SYSNAME),SunOS)
# Hard code special cases for SunOS 5.6 and 5.7 since we
# need a 3 digit version number for comparison purposes.
  ifeq ($(PACE_SYSVER),57)
    PACE_SYSVER := $(PACE_SYSVER)0
  endif # PACE_SYSVER
  ifeq ($(PACE_SYSVER),56)
    PACE_SYSVER := $(PACE_SYSVER)0
  endif # PACE_SYSVER
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_SUNOS=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),NT)
VPATH :=.:win32:$(VPATH)
CFLAGS += -DPACE_WIN32=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),LynxOS)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_LYNXOS=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),VxWorks)
VPATH :=.:vxworks:$(VPATH)
CFLAGS += -DPACE_VXWORKS=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),Linux)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_LINUX=$(PACE_SYSVER)
endif

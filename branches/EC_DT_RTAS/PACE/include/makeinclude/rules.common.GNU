#----------------------------------------------------------------------------
#       $Id$
#
#       Common rules for building PACE targets
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/PACE
endif # ! PACE_ROOT

VPATH+=config:sys

ifeq ($(PACE_SYSNAME),)
  PACE_SYSNAME = $(shell uname -s)
endif # ! PACE_SYSNAME

ifeq ($(PACE_SYSVER),)
  PACE_SYSVER = $(shell uname -r)
endif # ! PACE_SYSVER

ifeq ($(PACE_SYSNAME),Linux)
  PACE_SYSVER := $(shell echo $(PACE_SYSVER) | \
    perl -ne 'printf "%lu\n", hex sprintf "%02X%02X%02x", split /\./')
else
  PACE_SYSVER := $(shell echo $(PACE_SYSVER) | sed 's/\.//g')
endif # PACE_SYSNAME == Linux

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
# AIX               AIX

ifeq ($(PACE_SYSNAME),SunOS)
  # Hard code special cases for SunOS 5.6, 5.7, and 5.8 since
  # we need a 3 digit version number for comparison purposes.
  ifeq ($(PACE_SYSVER),58)
    PACE_SYSVER := $(PACE_SYSVER)0
  endif # PACE_SYSVER == 58
  ifeq ($(PACE_SYSVER),57)
    PACE_SYSVER := $(PACE_SYSVER)0
  endif # PACE_SYSVER == 57
  ifeq ($(PACE_SYSVER),56)
    PACE_SYSVER := $(PACE_SYSVER)0
  endif # PACE_SYSVER == 56
  VPATH :=.:posix:$(VPATH)
  CFLAGS += -DPACE_SUNOS=$(PACE_SYSVER) -D_REENTRANT
  ifeq ($(threads),1)
    CFLAGS += -D_POSIX_PTHREAD_SEMANTICS
  endif # ! ($(threads),0)
endif

ifeq ($(PACE_SYSNAME),NT)
  VPATH :=.:win32:$(VPATH)
  CFLAGS += -DPACE_WIN32=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),LynxOS)
  VPATH :=.:posix:$(VPATH)
  CFLAGS += -DPACE_LYNXOS=$(PACE_SYSVER)
  ifeq ($(PACE_SYSVER),0x300)
    CFLAGS += -D_POSIX_THREADS_CALLS
  endif # ($(PACE_SYSVER),0x300)
endif

ifeq ($(PACE_SYSNAME),OSF1)
  VPATH :=.:posix:$(VPATH)
  CFLAGS += -DPACE_OSF1=$(PACE_SYSVER) -D_DEC_XPG
endif

ifeq ($(PACE_SYSNAME),VxWorks)
  VPATH :=.:vxworks:$(VPATH)
  CFLAGS += -DPACE_VXWORKS=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),Linux)
  VPATH :=.:posix:$(VPATH)
  emulation=1
  CFLAGS += -DPACE_LINUX=$(PACE_SYSVER)
endif

ifeq ($(PACE_SYSNAME),AIX)
  VPATH := .:posix:$(VPATH)
  CFLAGS += -DPACE_HAS_POSIX -D_REENTRANT
endif

ifeq ($(PACE_SYSNAME),HP-UX)
  VPATH := .:posix:$(VPATH)
  CFLAGS += -DPACE_HAS_POSIX -D_REENTRANT
endif

ifeq ($(PACE_SYSNAME),QNX)
  VPATH := .:posix:$(VPATH)
  CFLAGS += -DPACE_QNX=$(PACE_SYSVER) -D_REENTRANT
endif

ifeq ($(debug),)
  ifeq (,$(findstring $(CFLAGS),$(CCFLAGS)))
    CCFLAGS += -DPACE_NDEBUG
  else
    CFLAGS  += -DPACE_NDEBUG
  endif
endif # debug

ifeq ($(pace_tracing),1)
  ifeq (,$(findstring $(CFLAGS),$(CCFLAGS)))
    CCFLAGS += -DPACE_TRACING
  else
    CFLAGS  += -DPACE_TRACING
  endif
endif # debug

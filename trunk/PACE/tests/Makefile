#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the PACE tests
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/pace
endif # ! PACE_ROOT

ifndef static_libs_only
  ifndef shared_libs_only
    static_libs_only = 1
  endif # shared_libs_only
endif # static_libs_only

ifeq ($(PACE_SYSNAME),)
  PACE_SYSNAME = $(shell uname -s)
endif # ! PACE_SYSNAME

# If we are inlining the PACE functions then we don't want to include
# the PACE library (and we need to define PACE_HAS_INLINE). If we're not
# inlining then we need to include the PACE library. Inlining is the default.
ifndef inline
  CFLAGS += -DPACE_HAS_INLINE
else
  ifeq (0,$(inline))
    LIBS += -L$(PACE_ROOT)/pace -lPACE
  else
    CFLAGS += -DPACE_HAS_INLINE
  endif # inline
endif # ! inline

VPATH:=common:arpa:config:netinet:sys

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

# Solaris is POSIX - change this when the platform directories get
# squared away

ifeq ($(PACE_SYSNAME),SunOS)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_SUNOS
endif

ifeq ($(PACE_SYSNAME),NT)
VPATH :=.:win32:$(VPATH)
CFLAGS += -DPACE_WIN32
endif

ifeq ($(PACE_SYSNAME),LynxOS)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_LYNXOS
endif

ifeq ($(PACE_SYSNAME),VxWorks)
VPATH :=.:vxworks:$(VPATH)
CFLAGS += -DPACE_VXWORKS
endif

ifeq ($(PACE_SYSNAME),Linux)
VPATH :=.:posix:$(VPATH)
CFLAGS += -DPACE_LINUX
endif

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN =   Stdio_Test \
        Posix_SP_Test

#### If the PACE library wasn't built with all components, don't
#### try to build certain tests.
PACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/pace_components --pace)

PSRC=$(addsuffix .c,$(BIN))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
ACELIB =
INCLDIRS += -I$(PACE_ROOT)
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifndef SOEXT
  SOEXT=so
endif

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# $Id$
#
# LynxOS with g++.  Defaults to LynxOS Version 3.0.0.  For Version 3.1.0,
# for example, add "VERSION=3.1.0" to your make invocation.

#### NOTE:  It's best to create a file that includes this one, instead
#### of symlinking it.  That way, you can easily override the default
#### configuration.  For example, to configure for a PowerPC target, I
#### use a include/makeinclude/platform_macros.GNU that contains the
#### following lines (without comment markers):
####
####     LYNXTARGET = ppc
####     include $(ACE_ROOT)/include/makeinclude/platform_lynxos.GNU
####
#### Similarly, the default VERSION and HOST_OS settings (see below)
#### can be overridden for your particular platform.


ifeq ($(debug),)
  debug = 1
endif
shared_libs =
static_libs = 1
#### optimize is defined below because it is version-dependent.

ifneq ($(shell uname -s),LynxOS)
  CROSS-COMPILE = 1

  PACE_SYSNAME = LynxOS

  #### The following may need to be customized for your host or target type.
  #### Or, you can source the appropriate (for your shell) Lynx SETUP file
  #### before running "make".

  ifeq (,$(VERSION))
    export VERSION=3.0.0
  endif # VERSION

  PACE_SYSVER = 0x$(VERSION)

  ifeq (,$(LYNXTARGET))
    export LYNXTARGET = x86
  endif # LYNXTARGET

  ifeq (,$(OBJSFORMAT))
    ifeq (x86,$(LYNXTARGET))
      OBJSFORMAT = coff
    else  # ! x86
      OBJSFORMAT = xcoff
    endif # ! x86
    export OBJSFORMAT
  endif # OBJSFORMAT

  ifeq (,$(HOST_OS))
    export HOST_OS = sunos
  endif # HOST_OS

  ifeq (,$(HOST_OS_REVISION))
    #### Added for 3.1.0
    export HOST_OS_REVISION = `uname -r`
  endif # HOST_OS_REVISION

  ifeq (,$(HOST_OS_TYPE))
    #### Added for 3.1.0
    export HOST_OS_TYPE = solaris
  endif # HOST_OS_TYPE

  ifeq (,$(ENV_PREFIX))
    export ENV_PREFIX = /usr/lynx/$(VERSION)/$(LYNXTARGET)
  endif # ENV_PREFIX

  ifeq (,$(findstring $(ENV_PREFIX),$(PATH)))
    export PATH:=\
$(ENV_PREFIX)/cdk/$(HOST_OS)-$(OBJSFORMAT)-$(LYNXTARGET)/bin:$(ENV_PREFIX)/cdk/$(HOST_OS)-$(OBJSFORMAT)-$(LYNXTARGET)/usr/bin:$(PATH)
  endif # PATH
endif # ! LynxOS

ifeq (2.5.0,$(VERSION))
  # NOTE: On LynxOS Version 2.5.0, optimization causes these warnings:
  #       warning: internal compiler error: debugging info corrupted
  optimize = 0
  ACE_HAS_GNUG_PRE_2_8 = 1
else
  optimize = 1

  #### Even though the g++ version is 2.7-97r1, it supports nested
  #### classes, so it can build the TAO tests.
  ACE_HAS_GNUG_PRE_2_8 = 0
endif # VERSION

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe -ansi -Wpointer-arith
#### -Winline complains a lot with -O2.
#### CFLAGS          += -Winline
#### LynxOS 3.0.0's g++ has a lot of warnings about non-virtual base class
#### destructors with -Wall.
ifeq (2.5.0,$(VERSION))
  CFLAGS += -Wall
endif # VERSION

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lnetinet -lnsl
OCFLAGS         += -O2
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
PRELIB          = @true

# To save much disk space, strip all executables.  Comment the
# following line out if you want to debug.  Or, add "POSTLINK="
# to your make invocation.
POSTLINK        = ; strip $@

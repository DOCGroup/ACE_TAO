# $Id$
#
# LynxOS with g++.  Defaults to LynxOS Version 2.5.0.  For Version 3.0.0,
# add "VERSION=3.0.0" to your make invocation.

debug = 1
shared_libs =
static_libs = 1
#### optimize is defined below because it is version-dependent.

ifneq ($(shell uname -s),LynxOS)
  CROSS-COMPILE = 1

  #### The following may need to be customized for your host or target type.
  #### Or, you can source the appropriate (for your shell) Lynx SETUP file
  #### before running "make".

  ifeq (,$(VERSION))
    VERSION=2.5.0
  endif # VERSION

  ifeq (,$(HOST_OS))
    ifeq (2.5.0,$(VERSION))
      HOST_OS = sunos
    else
      HOST_OS = sunos-coff
    endif # VERSION
  endif # HOST_OS

  ifeq (,$(LYNXTARGET))
    LYNXTARGET = x86
  endif # LYNXTARGET

  ifeq (,$(findstring /usr/lynx/$(VERSION)/$(LYNXTARGET),$(PATH)))
    PATH:=\
/usr/lynx/$(VERSION)/$(LYNXTARGET)/cdk/$(HOST_OS)-$(LYNXTARGET)/bin:/usr/lynx/$(VERSION)/$(LYNXTARGET)/cdk/$(HOST_OS)-$(LYNXTARGET)/usr/bin:$(PATH)
  endif # PATH
endif # ! LynxOS

ifeq (2.5.0,$(VERSION))
  # NOTE: On LynxOS Version 2.5.0, optimization causes these warnings:
  #       warning: internal compiler error: debugging info corrupted
  optimize = 0
else
  optimize = 1
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
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
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
# following line out if you want to debug.  Or, added "POSTLINK="
# to your make invocation.
POSTLINK        = ; strip $@

# $Id$
# LynxOS with g++

debug = 1
optimize = 1
shared_libs =
static_libs = 1

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

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe -ansi -Wpointer-arith -Winline
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
# following line out if you want to debug.
POSTLINK        = ; strip $@

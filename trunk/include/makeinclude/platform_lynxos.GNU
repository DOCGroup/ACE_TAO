# $Id$
# LynxOS with g++

debug = 1
shared_libs =
static_libs = 1

ifneq ($(shell uname -s),LynxOS)
  CROSS-COMPILE = 1

  #### The following may need to be customized for your host or target type.
  #### Or, you can source the appropriate (for your shell) Lynx SETUP file
  #### before running "make".

  ifeq (,$(HOST_OS))
    HOST_OS = sunos
  endif # HOST_OS

  ifeq (,$(LYNXTARGET))
    LYNXTARGET = x86
  endif # LYNXTARGET

  ifeq (,$(VERSION))
    VERSION=2.5.0
  endif # VERSION

  ifeq (,$(findstring /usr/lynx/$(VERSION)/$(LYNXTARGET),$(PATH)))
    PATH:=\
/usr/lynx/$(VERSION)/$(LYNXTARGET)/cdk/$(HOST_OS)-$(LYNXTARGET)/bin:/usr/lynx/$(VERSION)/$(LYNXTARGET)/cdk/$(HOST_OS)-$(LYNXTARGET)/usr/bin:$(PATH)
  endif # PATH
endif # ! LynxOS

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe -ansi -Wall -Wpointer-arith -Winline
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

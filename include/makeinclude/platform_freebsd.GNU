# $Id$

# platform_freebsd.GNU

VERSION := $(shell uname -r | sed -e "s/`uname -r | sed -e 's/.//'`//")
ifeq ($(VERSION),2)
versioned_so = 1
endif

optimize = 1

debug = 1

CC              = gcc
CXX             = g++

CXX_VERSION := $(shell $(CXX) -dumpversion)
 
# If no option has been specified, try to enable implicit_templates based
# on the version of the compiler.
#
ifeq ($(implicit_templates),)
  ifeq (2.95,$(findstring 2.95,$(CXX_VERSION)))
    implicit_templates = 1
  else
    ifeq (3.,$(findstring 3.,$(CXX_VERSION)))
      implicit_templates = 1
    endif
    ifeq (2.96,$(findstring 2.96,$(CXX_VERSION)))
      implicit_templates = 1
    endif
  endif
endif
 
# User didn't specify and we couldn't guess if this version of the compiler
# works fine with implicit template instantiation. Turn if off. This could
# as well go into the previous check but GNU Make conditionals are lame.
#
ifeq ($(implicit_templates),)
  implicit_templates = 0
endif
 
ifeq ($(implicit_templates),0)
  ifeq ($(IMPLICIT_TEMPLATES_FLAG),) # Turn on flags if none is speficied.
    IMPLICIT_TEMPLATES_FLAG=-fno-implicit-templates
  endif
endif

CFLAGS          += -W -Wall -Wpointer-arith -pipe
CCFLAGS         += $(CFLAGS) $(IMPLICIT_TEMPLATES_FLAG)
DCFLAGS         += -g
ifneq ($(VERSION),3)
  LDFLAGS       += -Wl,-rpath $(ACE_ROOT)/ace
endif
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lstdc++
OCFLAGS         += -O2
PIC             = -fpic
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB = @echo

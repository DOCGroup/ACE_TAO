# $Id$

# platform_netbsd.GNU

# ***** Notice: This file is still under testing. *****
# To be used on netbsd platform without pthread support.

debug = 1
optimize = 1
versioned_so = 1

CC              = gcc
CXX		= g++

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

CFLAGS          += -w -fno-strict-prototypes
CCFLAGS         += $(CFLAGS) -fno-implicit-templates
DCFLAGS         += -g
DLD             = ld
LD		= $(CXX)
LIBS            += -lstdc++  #-lcompat -lresolv
##OCFLAGS         += -O2
PIC             = -fpic -DPIC
AR              = ar
ARFLAGS         = ruv
RANLIB          = ranlib

SOFLAGS = -Bshareable -x /usr/lib/c++rt0.o
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so  $<
PRELIB = @echo

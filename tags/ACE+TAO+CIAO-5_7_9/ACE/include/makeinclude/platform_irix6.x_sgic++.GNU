# -*- Makefile -*-

# $Id$

# Irix 6.[234] with SGI C++
# In order to get the -n32 flag enabled please set the SGI_ABI
# environment variable to -n32.  This causes a new compiler to be
# invoked.

threads ?= 1
templates ?= implicit

include $(ACE_ROOT)/include/makeinclude/platform_irix6.x_common.GNU

# Instead of setting the ABI here we rely on the the macro SGI_ABI.
CC		= cc
CXX		= CC
DLD		= $(CXX)
LD		= $(CXX)

# Basic flags

# In Irix 6.2 w/o the thread patches this macro should have no effect.
ifeq ($(threads),1)
	CPPFLAGS	+= -D_SGI_MP_SOURCE
endif

OCCFLAGS	+= -O -OPT:Olimit=0
DCCFLAGS	+= -g

# Enable 64-bit builds
ifeq (64,$(buildbits))
  CFLAGS        += -64
  CPPFLAGS      += -64
endif

# Enable exceptions even in the o32 bit ABI.
CCFLAGS		+= -exceptions -DACE_HAS_EXCEPTIONS

# Use the standard library and standard template instantiation mechanisms
CCFLAGS		+= -LANG:std

ifneq ($(templates),implicit)
  # Instantiate no templates automatically, do not run prelinker. There
  # are other choices available, but those have not been used in a while.
  CCFLAGS		+= -no_prelink
endif # templates != implicit

# Instantiate everything; without this, apps/Gateway/Gateway/gatewayd might
# not build due to missing template instantiations. NOTE: it seems
# this setting no longer works for ACE.
#CCFLAGS		+= -ptall
# Instantiate used templates, plus prelinking instantiation
#CCFLAGS	+= -ptused -prelink
# Instantiate used templates, but do not run prelinker
#CCFLAGS		+= -ptused

# Suppress warnings about "pack" pragmas not being defined
# Suppress warnings about "member with the same name as its class"
# Suppress warnings about "class has no copy assignment operator"
# Suppress warnings about "Access control" on inherited classes
# Suppress warnings about "last argument" of varargs function is unnamed
ifeq (7.3,$(findstring 7.3,$(CXX_VERSION)))
    # 7.3.x doesn't have the 3506 error code
    CCFLAGS         += -diag_suppress 3284,1253,3439,1234
else
    CCFLAGS         += -diag_suppress 3284,1253,3439,1234,3506
endif

LDFLAGS		+= -Wl,-woff,15
LDFLAGS		+= -Wl,-woff,84
LDFLAGS		+= -Wl,-woff,85
LDFLAGS		+= -Wl,-woff,133

# You need to remove this if you are using 6.2 without the Pthread
# patches or if you want to compile without threads support.
ifndef DONT_LINK_PTHREADS
ifeq ($(threads),1)
	LIBS	+= -lpthread
endif
endif

PIC		= -KPIC

ifneq ($(templates),implicit)
  AR            = ar
  ARFLAGS       = r
else
# We need to create the archive via CC so that the prelinker can
# instantiate the required templates.
  AR            = CC -ar
  ARFLAGS       = -o
endif # templates != implicit

RANLIB		= echo
SOFLAGS		+= -shared $(CCFLAGS) $(CPPFLAGS) -all -multigot
SOBUILD		= $(RM) $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
		  /bin/ln $(VSHDIR)$*.o $@

# The math library
MATHLIB=-lm

# $Id$
#
# RTEMS 4.5.0 and later with GNU g++.

#### Notes:
#### 1) This file requires that the WIND_BASE and WIND_HOST_TYPE environment
####    variables be set.  If the target CPU is not a PPC604, then your CPU
####    environment variable must be set.  If perl is not on your path,
####    you'll also need to set your PERL_PATH environment variable to
####    the full path to perl.

CROSS-COMPILE = 1

ifndef debug
  debug = 1
endif # ! debug
ifndef optimize
  optimize = 1
endif # ! optimize
ifndef rtti
  rtti =
endif # ! rtti

shared_libs =
static_libs = 1

ifeq (,$(RTEMS_MAKEFILE_PATH))
  default:
	@ERROR: you must set your RTEMS_MAKEFILE_PATH environment variable
endif # RTEMS_MAKEFILE_PATH

ifeq (,$(PERL_PATH))
  PERL_PATH = perl
endif # ! PERL_PATH


# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

#### BEGIN target CPU-specific settings

# look them up
CFLAGS = $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cflags)
#CCFLAGS = $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cflags) -fno-implicit-templates
CCFLAGS = $(TEMPLATES_FLAG)

#### END target CPU-specific settings

CXX := $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cxx)

ifndef ACE_CC
ACE_CC := $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cc)
endif # ! ACE_CC
ifndef ACE_CC_VERSION
  ACE_CC_VERSION := $(shell $(ACE_CC) --version)
endif # ! ACE_CC_VERSION

ifeq (egcs,$(findstring egcs,$(ACE_CC_VERSION)))
  ACE_HAS_GNUG_PRE_2_8 := 0
  CFLAGS +=
  ifneq (1,$(exceptions))
    #### With exceptions, this is applied in wrapper_macros.GNU.
    #### Without, we do it here.
    CFLAGS += -Wno-uninitialized
  endif # ! exceptions
endif # egcs

#CFLAGS          += -D_REENTRANT -ansi -fno-builtin -fno-defer-pop \
#                   -fvolatile $(PIPE_OPT) -W -Wall
#CCFLAGS         += $(CFLAGS)
ifeq (,$(rtti))
  #### Don't use RTTI even with egcs, because Tornado 1.0.x libraries
  #### don't support it.
  CCFLAGS += -fno-rtti -DACE_LACKS_RTTI
endif # rtti
DCFLAGS         += -g
DLD             = $(LD)
INCLDIRS        +=
# Hand build tests/rtems_init.o  .. something like this
#  sparc-rtems-gcc -B/opt/rtems/sparc-rtems/erc32/lib/ -specs bsp_specs \
#       -qrtems -mcpu=cypress  -O -g \
#       -I$(ACE_ROOT) -c -o rtems_init.o rtems_init.c

LD              = $(CXX) $(CCFLAGS) $(ACE_ROOT)/tests/rtems_init.o
LDFLAGS         += #-Wl,-X -Wl,-r
OCFLAGS         += -O
PIC             =


#### ld can't handle INCLDIRS, so override LINK definitions.
LINK.c           = override
LINK.c.override  = $(LD) $(LDFLAGS) $(LDLIBS) $(LIBS)

LINK.cc          = override
LINK.cc.override = $(LD)

# $Id$
#
# RTEMS 4.6.6 and later with GNU g++.

CROSS-COMPILE = 1

debug ?= 1
optimize ?= 1
rtti ?= 1

shared_libs =
static_libs = 1

ifeq (,$(RTEMS_MAKEFILE_PATH))
  default:
	@ERROR: you must set your RTEMS_MAKEFILE_PATH environment variable
endif # RTEMS_MAKEFILE_PATH

ifeq (,$(PERL_PATH))
  PERL_PATH = perl
endif # ! PERL_PATH

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
#include $(RTEMS_CUSTOM)
#include $(PROJECT_ROOT)/make/leaf.cfg

CXX = $(CC_FOR_TARGET)
AR  = $(AR_FOR_TARGET)
LD  = $(LD_FOR_TARGET)
NM  = $(NM_FOR_TARGET)
AS  = $(AS_FOR_TARGET)

# Test for template instantiation.
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

#### BEGIN target CPU-specific settings
# look them up
#CFLAGS = $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cflags)
#CCFLAGS = $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cflags) -fno-implicit-templates
#CCFLAGS = $(TEMPLATES_FLAG)
#### END target CPU-specific settings

#CXX := $(shell make --no-print-directory -f $(ACE_ROOT)/include/makeinclude/Makefile.rtems print_cxx)

CFLAGS          += -D_REENTRANT -ansi -fno-builtin -fno-defer-pop -fvolatile $(PIPE_OPT) -W -Wall
CCFLAGS         += $(CFLAGS)
ifeq (,$(rtti))
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

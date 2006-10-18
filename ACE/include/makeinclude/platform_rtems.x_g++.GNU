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
include $(RTEMS_MAKEFILE_PATH)/make/target.cfg

# Test for template instantiation.
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CFLAGS          += -D_REENTRANT -ansi -fno-builtin -fno-defer-pop -fvolatile $(PIPE_OPT) -W -Wall -I$(RTEMS_MAKEFILE_PATH)/lib/include -DACE_HAS_RTEMS
CCFLAGS         += $(CFLAGS) $(CPPFLAGS)
ifeq (,$(rtti))
  CCFLAGS += -fno-rtti -DACE_LACKS_RTTI
endif # rtti
DCFLAGS         += -g
DLD             = $(LD)
INCLDIRS        +=

LD              = $(CXX) $(CCFLAGS)
LDFLAGS         += -specs bsp_specs -qrtems -Wl,--allow-multiple-definition -B$(RTEMS_MAKEFILE_PATH)/lib
OCFLAGS         += -O
PIC             =

#### ld can't handle INCLDIRS, so override LINK definitions.
LINK.c           = override
LINK.c.override  = $(LD) $(LDFLAGS) $(LDLIBS) $(LIBS)

LINK.cc          = override
LINK.cc.override = $(LD)

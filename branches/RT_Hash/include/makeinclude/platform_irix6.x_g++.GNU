# $Id$

# Irix 6.X with GNU C++

include $(ACE_ROOT)/include/makeinclude/platform_irix6.x_common.GNU

ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 1
endif

CC              = gcc
CXX             = g++
CFLAGS          += $(DCFLAGS)

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         += -L$(ACE_ROOT)/ace -Wl,-dont_warn_unused
LIBS            += -lpthread
OCFLAGS         += -O2
PIC             = -fpic
# AR            = ar
ARFLAGS         = rv
RANLIB          = @true
SOFLAGS         += -shared $(CPPFLAGS)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
                  $(SOLINK.cc) $(LDFLAGS) -o $@ $(VSHDIR)$*.o

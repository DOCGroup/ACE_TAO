# platform_macosx.GNU,v 4.6 2001/08/14 18:57:28 bala Exp
# platform_macosx.GNU
# support for Mac OS X 10.1

CC              = cc
CXX             = c++
CFLAGS          += -Wall -Wpointer-arith -pipe

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)
DCFLAGS         += -g
DLD             = libtool
LD              = $(CXX)
LDFLAGS		+= -flat_namespace
LIBS            += -lcc_dynamic -lstdc++ -lSystem $(ACELIB)
OCFLAGS         += -O3
RANLIB          = ranlib
SOEXT		= dylib
SOFLAGS += -dynamic
SOBUILD = -o $(VSHDIR)$*.dylib $<

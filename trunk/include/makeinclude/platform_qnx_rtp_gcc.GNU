# platform_qnx_rtp_gcc.GNU,v 1.11 2003/08/09 04:43:51 dhinton Exp

# QNX/RTP hosted, using gcc-2.95.2

nto_version := $(shell uname -a | awk '{ gsub("\\.","", $$3 ); print $$3;}')

inline = 1
debug ?= 1
optimize ?= 1
exceptions ?= 1
threads ?= 1
ifeq ($(exceptions),1)
  CCFLAGS += -fexceptions
  LDFLAGS += -fexceptions
else
  CCFLAGS += -fno-exceptions
  LDFLAGS += -fno-exceptions
endif # ! exceptions

ifeq ($(inline),1)
  CCFLAGS += -finline-functions
else
  CCFLAGS += -fno-inline
endif # inline

ifeq ($(debug),1)
	DCFLAGS += -gdwarf-2
endif #debug

ifeq ($(optimize),1)
	OCFLAGS += -O3
endif #optimize

CC      = gcc
CXX     = g++

CFLAGS  += -pipe -W -Wall -Wpointer-arith -DACE_NTO_VERS=$(nto_version)
CPPFLAGS += -DACE_NTO_VERS=$(nto_version)

DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -lsocket -lstdc++ -lm

PIC     = -fPIC
AR      = ar
ARFLAGS = ruv
#RANLIB  = ranlib
SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true

# Test for template instantiation, add to SOFLAGS if SONAME set, 
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU
CCFLAGS += $(CFLAGS) $(TEMPLATES_FLAG)

# $Id$

# According to Ben Eng <ben@jetpen.com>, this file
# should allow ACE to be built on Linux using KAI C++.

debug = 1
optimize = 0

CC      = KCC
CXX     = KCC --no_auto_instantiation -x
CFLAGS  += -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT
CCFLAGS += $(CFLAGS) -DNULL=0
DCFLAGS += +K0 -g
DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -ldl -lpthread
OCFLAGS += +K3 -O
PIC     = -fPIC
AR      = $(CXX)
ARFLAGS = -o
RANLIB  = @true
SOFLAGS = $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<
PRELIB  = @true
PRELIB_USES_OBJ_ONLY = 1

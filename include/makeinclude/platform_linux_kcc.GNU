# $Id$

# According to Ben Eng <ben@jetpen.com>, this file
# should allow ACE to be built on Linux using KAI C++ 3.3a.

debug = 1
optimize = 0

CC      = KCC
CXX     = $(CC)
CFLAGS  += -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT
CCFLAGS += $(CFLAGS) -DNULL=0
DCFLAGS += +K0 -g
DLD     = $(CC)
LD      = $(CC)
LIBS    += -ldl -lpthread-0.7
OCFLAGS += +K3 -O
PIC     = -fPIC
AR      = $(CC)
ARFLAGS = -o
RANLIB  = @true
SOFLAGS = $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<
PRELIB  = @true
PRELIB_USES_OBJ_ONLY = 1

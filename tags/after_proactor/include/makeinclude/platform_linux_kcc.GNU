# $Id$

# According to Ben Eng <ben@jetpen.com>, this file
# should allow ACE to be built on Linux using KAI C++ 3.3a.

debug = 0
optimize = 1

CC      = /opt/KCC/KCC_BASE/bin/KCC
CXX     = $(CC)
CFLAGS  += -D_POSIX_THREADS -D_POSIX_THREAD_SAFE_FUNCTIONS -D_REENTRANT
CCFLAGS += $(CFLAGS) -DNULL=0
DCFLAGS += +K0 -g
DLD     = $(CC)
LD      = $(CC)
LIBS    += -ldl -lpthread -lm
EXCEPTION = --no_exceptions
OCFLAGS += +K3 -O --one_instantiation_per_object $(EXCEPTION)
PIC     = -fPIC
AR      = $(CC)
ARFLAGS = $(EXCEPTION) -o
RANLIB  = @true
SOFLAGS = $(CPPFLAGS) $(EXCEPTION)
SOBUILD = $(COMPILE.cc) $(PIC) $(EXCEPTION) -o $(VSHDIR)$*.o $<
PRELIB  = @true
PRELIB_USES_OBJ_ONLY = 1

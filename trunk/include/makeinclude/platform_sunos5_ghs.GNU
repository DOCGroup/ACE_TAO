# $Id$

# SunOS 5.x (Solaris 2.x) with GreenHills compiler.

debug = 1
optimize = 1
threads = 1

ifeq ($(threads),0)
  CFLAGS += -DACE_MT_SAFE=0
endif # threads

#### CPU type
ifndef CPU
  CPU = spc
endif # CPU

CC       = cc$(CPU)
CXX      = $(CC)
CCFLAGS  += $(CFLAGS) --no_rtti -tnone
DCFLAGS  += -g
DLD      = $(CXX)
LD       = $(CXX)
LDFLAGS  = --no_auto_instantiation
LIBS     += -lsocket -ldl -lgen -lnsl -lposix4 -lthread -lw
OCFLAGS  += -OL -OM
PIC      = -PIC
RANLIB   = @true
AR       = ax
ARFLAGS  = ruv
RANLIB   = @true
SOFLAGS  = -shared

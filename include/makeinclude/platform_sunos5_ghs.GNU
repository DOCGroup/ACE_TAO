# $Id$

# SunOS 5.x (Solaris 2.x) with GreenHills compiler.

debug = 1
optimize = 1
exceptions =

#### Green Hills 1.8.9 isn't compatible with the SunOS threads libraries.
threads =

CC       = gcx
CXX      = $(CC)
CCFLAGS  += $(CFLAGS) --no_rtti -tnone
DCFLAGS  += -g
DLD      = $(CXX)
LD       = $(CXX)
LDFLAGS  = --no_auto_instantiation
LIBS     += -lsocket -ldl -lgen -lnsl -lposix4
OCFLAGS  += -OL -OM
PIC      = -PIC
RANLIB   = @true
AR       = $(CC) $(LDFLAGS)
ARFLAGS  = -archive -o
RANLIB   = @true
SOFLAGS  = -shared

ifeq ($(threads),1)
  LIBS += -lthread
else  # ! threads
  CFLAGS += -DACE_MT_SAFE=0
endif # ! threads

ifdef exceptions
  CCFLAGS += --exceptions
else  # ! exceptions
  CCFLAGS += --no_exceptions
endif # ! exceptions

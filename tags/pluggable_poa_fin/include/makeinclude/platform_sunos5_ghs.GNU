# $Id$

# SunOS 5.x (Solaris 2.x) with GreenHills compiler.

debug = 1
exceptions =
optimize = 1
threads = 1

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
  #### Don't use g7 register with threading.
  CFLAGS += -Xleaveg7
  LIBS += -lthread
else  # ! threads
  CFLAGS += -DACE_MT_SAFE=0
endif # ! threads

ifdef exceptions
  CCFLAGS += --exceptions
else  # ! exceptions
  CCFLAGS += --no_exceptions
endif # ! exceptions

# $Id$

# SunOS 5.x (Solaris 2.x) with GreenHills compiler.

debug = 1
exceptions = 0
optimize = 1
threads = 1

# I(kitty) don't know anything about this compiler. Making it explicit.
#
templates = explicit

# Turn on the proper flags for explicit template instantiation.
#
ifeq ($(templates),explicit)
  ifeq ($(TEMPLATES_FLAG),) # Turn on flags if none is speficied.
    TEMPLATES_FLAG=-tnone
  endif
  CPPFLAGS += -DACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
  LDFLAGS  += --no_auto_instantiation
endif

CC       = gcx
CXX      = $(CC)
CCFLAGS  += $(CFLAGS) --no_rtti $(TEMPLATES_FLAG)
DCFLAGS  += -g
DLD      = $(CXX)
LD       = $(CXX)
LIBS     += -lsocket -ldl -lgen -lnsl -lposix4
OCFLAGS  += -OL -OM
PIC      = -PIC
RANLIB   = @true
AR       = $(CC) $(LDFLAGS)
ARFLAGS  = -archive -o
RANLIB   = @true
SOFLAGS  += -shared

ifeq ($(threads),1)
  #### Don't use g7 register with threading.
  CFLAGS += -Xleaveg7
  LIBS += -lthread
else  # ! threads
  CFLAGS += -DACE_MT_SAFE=0
endif # ! threads

ifeq (1,$(exceptions))
  CCFLAGS += --exceptions
else  # ! exceptions
  CCFLAGS += --no_exceptions
endif # ! exceptions

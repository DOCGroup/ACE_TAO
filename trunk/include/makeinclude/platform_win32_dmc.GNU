# $Id$

# debug = 1
# optimize = 1
# exceptions = 0

ifndef winsock2
  winsock2 = 1
endif

ifndef exceptions
  exceptions = 0
endif
ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 1
endif
ifeq (,$(threads))
  threads = 1
endif
ifeq (,$(rtti))
  rtti = 1
endif
pipes = 0
ifeq ($(debug),0)
CFLAGS += -DNDEBUG -v1
endif
ifeq ($(rtti),1)
  CFLAGS += -Ar
endif

CC      = dmc
CXX     = dmc
RC      =

CFLAGS += $(MODEL_FLAGS) -ND

EXEEXT = .exe

DCFLAGS += -g
DLD     = link
LD      = link

PIC        =
AR         = ar
ARFLAGS    = rsuv
RANLIB     = ranlib
SOEXT      = dll
SOFLAGS    += $(MODEL_FLAGS) $(CPPFLAGS)
SOBUILD    = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<

PRELIB  = @true

CCFLAGS += $(CFLAGS) $(TEMPLATES_FLAG)

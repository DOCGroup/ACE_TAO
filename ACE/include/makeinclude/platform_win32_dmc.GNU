# $Id$

# debug = 1
# optimize = 1
# exceptions = 0

winsock2 ?= 1
exceptions ?= 0
debug ?= 1
optimize ?= 1
threads ?= 1

pipes = 0
ifeq ($(debug),0)
CFLAGS += -DNDEBUG -v1
CFLAGS += -Ar

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

# Dmc doesn't have rwho
rwho = 0

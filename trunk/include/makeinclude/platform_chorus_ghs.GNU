# $Id$

# Chorus with Green Hills

CHORUS = 1

debug = 1
optimize = 1
shared_libs =
static_libs = 1

CC              = /project/doc/green68k/GreenHills/releases/chorus-m68k/build -driver ch68
CXX             = $(CC)
CCFLAGS         += $(CFLAGS) --no_rtti -tnone
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         = --no_auto_instantiation
OFLAGS          += -O -OM -OL
AR              = /project/doc/green68k/GreenHills/releases/chorus-m68k/ax
ARFLAGS         = rv
RANLIB          = /bin/true

# $Id$

# VxWorks 5.2-5.3.1 with Green Hills 1.8.8

VXWORKS = 1
CROSS-COMPILE = 1

debug = 1
shared_libs =
static_libs = 1

CC              = cxvxppc
CXX             = $(CC)
CCFLAGS         += $(CFLAGS) --no_rtti -tnone
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         = --no_auto_instantiation
LIBS            += /appl/newgreen/vxppc603/ghsbltin.o /appl/newgreen/vxppc603/ghsmath.o
OCFLAGS         += -O -OM -OL
PIC             =
AR              = ax
ARFLAGS         = rv
RANLIB          = /bin/true

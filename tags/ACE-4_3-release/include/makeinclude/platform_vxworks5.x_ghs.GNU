# $Id$

# VxWorks 5.2-5.3.1 with Green Hills 1.8.8

CC              = cxvxppc
CXX             = $(CC)
CFLAGS          += $(DCFLAGS)
CCFLAGS         += $(CFLAGS) --no_rtti -tnone
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         = --no_auto_instantiation
LIBS            += /appl/newgreen/vxppc603/ghsbltin.o /appl/newgreen/vxppc603/ghsmath.o
PIC             =
AR              = ax
ARFLAGS         = rv
RANLIB          = /bin/true

#### don't build shared objects:
BUILD           = $(VOBJS) $(VLIB) $(VBIN)
OBJDIRS         = .obj
SHLIB           =
SHOBJ           = $(addsuffix .o,$(FILES))
SOEXT           = o
VSHDIR          = .obj/

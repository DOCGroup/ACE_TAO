# $Id$

# Irix 6.x with KCC 3.3f

# WARNING_FLAGS +=
# 111: statement is unreachable.    There's a whole bunch of these
#                                   in the ace code.
WARNING_FLAGS += --diag_suppress 111

debug = 1
optimize = 1

CC              = KCC
CXX             = KCC
CFLAGS          += -32
CCFLAGS         += $(CFLAGS) -DNULL=0 $(WARNING_FLAGS)
ifeq ($(optimize),1)
DCFLAGS         += -g --backend -g3
else
DCFLAGS         += +K0 -g
endif
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         += -32 -L$(ACE_ROOT)/ace -L.
LIBS            += -lpthread
OCFLAGS         += +K3 -O2
PIC             = 
AR              = KCC
ARFLAGS         = -o
RANLIB          = @true
SOFLAGS         = $(CPPFLAGS) -L$(ACE_ROOT)/ace $(ACELIB)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true

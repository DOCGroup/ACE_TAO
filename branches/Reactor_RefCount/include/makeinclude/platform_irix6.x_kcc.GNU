# $Id$

# Irix 6.x with KCC 3.4d

include $(ACE_ROOT)/include/makeinclude/platform_irix6.x_common.GNU

# WARNING_FLAGS +=
# 111: statement is unreachable.    There's a whole bunch of these
#                                   in the ace code.
# 280: declaration of a member with the same name as its class
#                                   Get this from <netinet/in.h>.
# 608: this pragma must immediately precede a declaration or statement
#                                   Get this from <ucontext.h>
WARNING_FLAGS += --diag_suppress 111,280,608

debug = 1
optimize = 1

CC              = KCC
CXX             = $(CC)
CFLAGS          += -n32
KCCOPTS         = --thread_safe --one_per
CCFLAGS         += $(CFLAGS) -DNULL=0 $(WARNING_FLAGS) $(KCCOPTS)
ifeq ($(optimize),1)
DCFLAGS         += -g
else
DCFLAGS         += +K0 -g
endif
DLD             = $(CC)
LD              = $(CC)
# warning 84: "" is not used for resolving any symbol.
LDFLAGS         += -n32 -L$(ACE_ROOT)/ace -L. --backend -Wl,-woff,84
#LIBS            += -lpthread
OCFLAGS         += +K3 -O2
PIC             =
AR              = $(CC)
ARFLAGS         = $(KCCOPTS) -o
RANLIB          = @true
SOFLAGS         += $(CPPFLAGS) $(KCCOPTS) -L$(ACE_ROOT)/ace $(ACELIB)
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true

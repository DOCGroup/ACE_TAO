# $Id$

# This platform macros file is intended to work with Digital UNIX 4.0
# (OSF/1 4.0) and KAI C++ 3.3f.

debug = 1
optimize = 1

# WARNING_FLAGS +=
#   9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
#                                   and /usr/include/net/if_types.h
# 111: statement is unreachable.    There's a whole bunch of these
#                                   in the ace code.
WARNING_FLAGS += --diag_suppress 9,111

CC              = KCC
CXX             = $(CC)
CFLAGS          += -D_REENTRANT
CCFLAGS         += $(CFLAGS) -DNULL=0 $(WARNING_FLAGS)
ifeq ($(optimize),1)
DCFLAGS         += -g3
else
DCFLAGS         += +K0 -g
endif
LD              = $(CC)
DLD             = $(CC)
LIBS            += -lpthread -lmach -lexc -ltli -lrt -lm
OCFLAGS         += +K3 -O
PIC             = 
AR              = $(CC)
ARFLAGS         = -o
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB          = /usr/bin/true
SOFLAGS         = $(CPPFLAGS) -L$(ACE_ROOT)/ace $(ACELIB) -lc
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true

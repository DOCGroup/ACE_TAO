# $Id$

# This platform macros file is intended to work with Digital UNIX 4.x
# (OSF/1 4.x) and KAI C++ 3.3f.

ifeq (,$(debug))
  debug = 1
endif

ifeq (,$(exceptions))
  exceptions = 1
endif

ifeq (,$(rtti))
   rtti = 1
endif

ifeq (,$(threads))
  threads = 1
endif

ifeq (,$(optimize))
  optimize = 1
endif

# WARNING_FLAGS +=
#   9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
#                                   and /usr/include/net/if_types.h
# 111: statement is unreachable.    There's a whole bunch of these
#                                   in the ace code.
WARNING_FLAGS += --display_error_number --diag_suppress 9,111

CC              = KCC
CXX             = $(CC)
CFLAGS          += -D_REENTRANT
KCCOPTS         = --thread_safe --one_per
CCFLAGS         += $(CFLAGS) -DNULL=0 $(WARNING_FLAGS) $(KCCOPTS)
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
ARFLAGS         = $(KCCOPTS) -o
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB          = /usr/bin/true
SOFLAGS         += $(CPPFLAGS) $(KCCOPTS) -L$(ACE_ROOT)/ace $(ACELIB) -lc

# With kcc 4, if it does template splitting and ends up with more than
# a certain number of templates, it can screw up the link command so that
# the entry point for the program gets set to one of the template
# instantiations!  This forces the correct entry point.
LDFLAGS         += --backend -Wl,-e,__start

# Work around kcc overflowing the arg list...
SHLIBBUILD      = $(AR) $(ARFLAGS) $@-tmp.a $(VSHOBJS1) $(LDFLAGS) $(LIBS); $(filter-out -c,$(COMPILE.cc) $(PIC)) -o $@ -all $@-tmp.a -none $(LDFLAGS) $(LIBS) -lc --COMPO_ln_dy -expect_unresolved --COMPO_ln_dy '*'; rm $@-tmp.a

SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB          = @true

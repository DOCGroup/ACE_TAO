# $Id$
#
# This is for the HP-UX 10.x and 11.x using the aC++ compiler.
#
# For 10.x, most of the difference for threaded vs. non-threaded is
# contained in ace/config-hpux-10.x.h.  However, there is a line to verify
# in this file - if you are using DCE threads, "-lcma" needs to be on the
# 10.x version of the LIBS line, below.  If you are not using DCE threads,
# then use the LIBS without -lcma.
#
# HP-UX 11 supports both 32-bit and 64-bit builds, regardless of which
# architecture the build is done on.  This file is set up to build native
# to the machine it's running on.  To cross-build for another platform, add
# the appropriate +DA switch to CCFLAGS.

debug = 1

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS	:= $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#
CC		= /bin/cc
CXX		= /opt/aCC/bin/aCC
# Suppres warning 829 (string literal to char * is deprecated)
CCFLAGS		+= $(CFLAGS) -D_THREAD_SAFE -D_REENTRANT -DHPUX_VERS=$(HPUX_VERS) +W829,302
DCFLAGS		+= -g
DLD		= $(CXX) $(CCFLAGS)
LD		= $(CXX) $(CCFLAGS)
PIC		= +Z

AR		= $(COMPILE.cc) +inst_close $^; /bin/ar
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -Wl,+s
SOFLAGS		= -b
SOEXT		= sl
#
# Grab the appropriate libraries based on the version of HP-UX we're building
# on.  If on HP-UX 10.x and not using DCE threads, remove -lcma.
#
ifeq ($(word 2,$(HPVERS_WORDS)), 11)
LIBS		= -lxti -lpthread -lrt -ldld
else
LIBS		= -lxti -ldld -lcma
endif

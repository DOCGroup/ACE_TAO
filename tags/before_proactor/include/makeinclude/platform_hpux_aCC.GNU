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
# the appropriate +DA switch to CCFLAGS.  Note that if you do this, and want to
# shut up the "At least one PA 2.0 object file detected..." messages, you
# should also add -Wl,+vnocompatwarnings.

debug = 1
threads = 1

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS	:= $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#
CC		= /bin/cc
CXX		= /opt/aCC/bin/aCC
#
# Set the appropriate preprocessor defs for threading based on OS version.
# If the user has requested building without threads, then don't set any.
#
ifeq ($(threads),1)
ifeq ($(word 2,$(HPVERS_WORDS)), 11)
THR_DEFS	= -DACE_HAS_THREADS -D_POSIX_C_SOURCE=199506L
else
THR_DEFS	= -DACE_HAS_THREADS -D_REENTRANT
endif
endif
#
# Suppress warning 829 (string literal to char * is deprecated)
# Suppress warning 302 ((...) parameter list is a non-portable feature)
CCFLAGS		+= $(CFLAGS) $(THR_DEFS) -DHPUX_VERS=$(HPUX_VERS) -DACE_LACKS_PRAGMA_ONCE +W829,302
# Additionally, on HP-UX 10.20, suppress 495 to shut up the warnings from
# the system header files.  667 is also suppressed, but the compiler still
# tells you there was a future error, but at least you can pick out any real
# errors by quickly scanning the output.
ifeq ($(word 2,$(HPVERS_WORDS)), 10)
CCFLAGS         += +W495,667
endif
DCFLAGS		+= -g
DLD		= $(CXX) $(CCFLAGS)
LD		= $(CXX)
PIC		= +Z

AR		= $(COMPILE.cc) +inst_close $^; /bin/ar
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -Wl,+s
SOFLAGS		= -b
SOEXT		= sl
#
# Grab the appropriate libraries based on the version of HP-UX we're building
# on and whether or not thread support is being used.
#
ifeq ($(word 2,$(HPVERS_WORDS)), 11)

#11.x:
ifeq ($(threads),1)
LIBS		+= -lxti -lpthread -lrt -ldld
else
LIBS		+= -lxti -lrt -ldld
endif

else

# 10.x:
LIBS		+= -lxti -ldld
ifeq ($(threads),1)
LIBS		+= -lcma
endif

endif

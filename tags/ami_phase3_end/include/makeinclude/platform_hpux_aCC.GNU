# $Id$
#
# This is for the HP-UX 10.x and 11.x using the aC++ compiler.
#
# For 10.x, most of the difference for threaded vs. non-threaded is
# contained in ace/config-hpux-10.x.h.  However, to build without threads,
# you should run the make with "threads=0" since the default is to build
# with threads.
#
# HP-UX 11 supports both 32-bit and 64-bit builds, regardless of which
# architecture the build is done on.  This file is set up to build native
# to the machine it's running on.  To cross-build for another platform, add
# "buildbits=32" to make a 32-bit build, and "buildbits=64" to do a 64-bit
# build.  Note that if you explicitly specify a 64-bit build, the option
#  -Wl,+vnocompatwarnings is added to shut up the "At least one PA 2.0 object
# file detected..." messages.

buildbits = 0
debug = 1
distrib = 0
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
OCCFLAGS	= -O
PIC		= +Z

AR		= $(COMPILE.cc) +inst_close $^; /bin/ar
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -Wl,+s
SOFLAGS		= -b

ifeq ($(distrib),1)

ifeq ($(word 2,$(HPVERS_WORDS)), 10)
SOFLAGS		+= -Wl,-x -Wl,+h/usr/lib/$(@F)
else
ifeq ($(buildbits),32)
SOFLAGS		+= -Wl,-x -Wl,+h/usr/lib/$(@F)
endif
ifeq ($(buildbits),64)
SOFLAGS		+= -Wl,-x -Wl,+h/usr/lib/pa20_64/$(@F)
endif
endif

endif

SOEXT		= sl

ifeq ($(buildbits),32)
CCFLAGS		+= +DA1.1 +DS1.1
endif
ifeq ($(buildbits),64)
CCFLAGS		+= +DA2.0 +DS2.0
LDFLAGS		+= -Wl,+vnocompatwarnings
endif

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

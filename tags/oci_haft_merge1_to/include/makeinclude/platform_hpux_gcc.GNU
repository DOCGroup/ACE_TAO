# $Id$
#

ifeq (,$(buildbits))
  buildbits = 0
endif
ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(distrib))
  distrib = 0
endif
ifeq (,$(exceptions))
  exceptions = 1
endif
ifeq (,$(optimize))
  optimize = 1
endif
ifeq (,$(stdcpplib))
  stdcpplib = 1
endif
ifeq (,$(threads))
  threads = 1
endif
ifeq (,$(with_ld))
  with_ld = hpux
endif

# First, extract the OS version number.
HPVERS_WORDS := $(subst ., ,$(shell uname -r))
HPUX_VERS	:= $(join $(word 2,$(HPVERS_WORDS)),$(word 3,$(HPVERS_WORDS)))
#
CC		= gcc
CXX		= g++
CFLAGS		+= -w

# Test for template instantiation.
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

#
# Set the appropriate preprocessor defs for threading based on OS version.
# If the user has requested building without threads, then don't set any.
#
ifeq ($(threads),1)
  ifeq (3.,$(findstring 3.,$(CXX_VERSION)))
    THR_DEFS = -threads
  endif
  ifeq ($(word 2,$(HPVERS_WORDS)), 11)
    THR_DEFS += -DACE_HAS_THREADS -D_POSIX_C_SOURCE=199506L
  else
    THR_DEFS += -DACE_HAS_THREADS -D_REENTRANT
endif
endif

CCFLAGS		+= $(CFLAGS) $(TEMPLATES_FLAG) -DHPUX_VERS=$(HPUX_VERS) $(THR_DEFS)
# On HP-UX 11, there's a conflict with pthread_atfork in sys/unistd.h and
# sys/pthread.h - both define the function, but slightly differently.
# -fstrict-prototype resolves the issue.
#ifeq ($(word 2,$(HPVERS_WORDS)), 11)
#CCFLAGS		+= -fstrict-prototype
#endif
DCFLAGS		+= -g
DLD		= $(CXX)
LD		= $(CXX)
OCFLAGS		+= -O2
PIC		= -fPIC
# GNU ar doesn't work (missing libfl.sl?) so use HP ar.
AR              = /usr/bin/ar
ARFLAGS		= ruv
RANLIB		= echo
LDFLAGS		= -L. -L$(ACE_ROOT)/ace -Wl,+s
SOFLAGS		+= $(CPPFLAGS) -shared -nostdlib $(PIC)
SOEXT		= sl
SOBUILD 	= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.$(SOEXT) $<
PRELIB = @true
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

# $Id$

# This platform macros file is intended to work with
# Digital UNIX using the cxx compiler.

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_pre.GNU

# Note that TAO has not been tested with debug=0 on this platform. So
# you should be careful when turning off debug.
debug ?= 1

# exceptions=1 is the default on this platform. exceptions=0 has not
# been tested. See the bottom of this file for more. Turning off exceptions
# on this platform can cause bad things to happen.
exceptions ?= 1

threads ?= 1

# Use of Standard C++ Library requires some flags to be added to the
# compile line
#
stdcpplib ?= 1

CC              = cxx
CXX             = $(CC)

# Note: Just get the version if it hasnt been defined so far.
#
ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX) -V)
endif # ! CXX_VERSION

#### Note: -thread uses POSIX threads.  If you want to use DCE threads
####       instead, change "-pthread" to "-threads".
ifeq ($(threads),1)
  CFLAGS          += -pthread
endif

#### These flags can only be used with cxx Version 6.0 and later.
#### They are enabled by default; they can be disabled by settting
#### CXX_VER to CXX_5 either on the make command line, or by
#### or by setting the CXX_VER environment variable to CXX_5.
ifneq ($(CXX_VER),CXX_5)
  WARNING_FLAGS += -w0
  ifneq (,$(VERBOSE_WARNING))
    WARNING_FLAGS += -msg_display_number -msg_display_tag
  endif # VERBOSE_WARNING

  ifneq (,$(optimize))
    #### See ace/config-cxx-common.h for the meaning of this warning.
    #### It's not necessary on DU 5.0.
    WARNING_FLAGS += -msg_disable 1016
  endif # optimize
  ifeq (4.,$(findstring 4.,$(shell uname -r)))
    # 9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
    WARNING_FLAGS += -msg_disable 9
  endif
endif # 6.0 or later

templates ?= automatic

# Turn on the appropriate flags
#
ifeq ($(templates), automatic)
  ifeq ($(TEMPLATES_FLAG),) # Turn on flags if none is speficied.
    TEMPLATES_FLAG = -pt
    SOFLAGS        += -tall
  endif
else
  ifeq ($(templates), used)
    ifeq ($(TEMPLATES_FLAG),) # Turn on flags if none is speficied.
      TEMPLATES_FLAG = -pt
      SOFLAGS        += -tused
    endif
  endif # templates == used
endif # templates == automatic

# Add the extra definitions needed to use the Standard Template Library
#
ifeq ($(stdcpplib), 1)
  CPPFLAGS += -D__USE_STD_IOSTREAM
endif

# The correct flags to pass to the linker for ELF dynamic shared library
# versioning
#
ifneq ($(SONAME),)
  SOFLAGS += -soname $(SONAME)
endif

CCFLAGS         += $(CFLAGS) $(WARNING_FLAGS) $(TEMPLATES_FLAG)
DCFLAGS         += -g1 -O0
DLD             = $(CXX)
LD              = $(CXX)
#### The -hidden [...] -non_hidden business avoids multiply defined
#### symbols between the shared library and libcxxstd.a.  It's the
#### workaround recommended in PTR 43-4-204.  It must appear before any
#### linker commands.
LDFLAGS         += -hidden $(wildcard /usr/lib/libcxxstd.a) -non_hidden
LIBS            += $(CFLAGS) -lxti -lrt
MATHLIB         = -lm
OCFLAGS         += -O3
PIC             =
ARFLAGS         = cruvZ
# This is needed to pull in all the template instantiations from the
# repository into the library.
#
TMPINCDIR       = cxx_repository

# When libraries are archived, a hash index is automatically created
# so there is no need for ranlib
#
RANLIB          = @true
SOFLAGS         += -shared -use_ld_input $(ACELIB)

LIBS            :=  $(LIBS)
SOBUILD         = $(RM) $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
                  ln $(VSHDIR)$*.o $@

ifeq (0,$(exceptions))
  #### Disable the default exception handling of cxx >= 6.0.
  #### This is untested.  exceptions=1 is the default, so it's not used
  #### by default.
  ifeq (6.,$(findstring 6.,$(CXX_VERSION)))
    CCFLAGS += -nocleanup
  endif # 6.x
endif # ! exceptions

### There is a bug with pthreads header file shipped with v1885 of
### V5.1. We need to turn this flag to get asm () inline for
### compilation.
ifeq (1885,$(findstring 1885,$(ACE_DU_SUBVERSION)))
  ifeq (6.,$(findstring 6.,$(CXX_VERSION)))
    CPPFLAGS += -D_PTHREAD_ENV_DECCXX
  endif # 6.x
endif #v1885

# Include the common macros for all compilers
#
include $(ACE_ROOT)/include/makeinclude/platform_tru64_post.GNU

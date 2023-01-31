# -*- Makefile -*-

ifeq ($(insure),1)
  CC  = insure
  CXX = insure
else
  ifneq ($(CROSS_COMPILE),)
    CROSS-COMPILE = 1
    # Build using the cross-tools
    CC  = ${CROSS_COMPILE}gcc${CROSS_COMPILE_SUFFIX}
    CXX = ${CROSS_COMPILE}g++${CROSS_COMPILE_SUFFIX}
    AR  = ${CROSS_COMPILE}ar${CROSS_COMPILE_SUFFIX}
    # Cross-linker requires this for linked in shared libs that depend
    # themselves on other shared libs (not directly linked in)
    LDFLAGS += -Wl,-rpath-link,$(ACE_ROOT)/lib
    ifneq (,$(HOST_ROOT))
      TAO_IDLFLAGS += -g $(HOST_ROOT)/bin/ace_gperf
      TAO_IDL = $(HOST_ROOT)/bin/tao_idl
      TAO_IDL_DEP = $(TAO_IDL)
      # make sure to use the host compiler, not the cross-compiler
      # as preprocessor for the cross-compiled idl tools
      TAO_IDL_PREPROCESSOR = gcc
    endif
  endif
endif

ifneq ($(TCPU),)
  ifeq ($(tunemodelflag),1)
    FLAGS_C_CC += -mtune=$(TCPU)
  endif

  ifeq ($(cpumodelflag),1)
    FLAGS_C_CC += -mcpu=$(TCPU)
  endif

  ifeq ($(archmodelflag),1)
    FLAGS_C_CC += -march=$(TCPU)
  endif
endif

ifeq ($(gprof),1)
  FLAGS_C_CC += -pg
  LDFLAGS += -pg
endif

ifeq ($(no_deprecated),1)
  CCFLAGS += -Wno-deprecated
endif

ifeq ($(gcov),1)
  CCFLAGS += --coverage
  LDFLAGS += --coverage
endif

ifeq ($(CXX),insure)
  # insure does not pass through the -dumpversion option.
  CXX_FOR_VERSION_TEST ?= g++
else
  CXX_FOR_VERSION_TEST ?= $(CXX)
endif

CXX_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion)
CXX_MACHINE := $(shell $(CXX_FOR_VERSION_TEST) -dumpmachine)
ifeq (cmd,$(findstring cmd,$(SHELL)))
CXX_MAJOR_VERSION := $(firstword $(subst ., ,$(CXX_VERSION)))
else
CXX_MAJOR_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion | sed -e 's/[^0-9\.]//g' | sed -e 's/\..*$$//')
endif
ifeq (cmd,$(findstring cmd,$(SHELL)))
CXX_MINOR_VERSION := $(word 2,$(subst ., ,$(CXX_VERSION)))
else
CXX_MINOR_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion | sed -e 's/[^0-9\.]//g' | sed -e 's/^[0-9]*\.//' | sed -e 's/\..*$$//')
endif

# gcc 4 has C++03 as default C++ version, enable this to be C++11
ifeq ($(findstring $(CXX_MAJOR_VERSION),4),$(CXX_MAJOR_VERSION))
  c++11 ?= 1
  # gcc 4 has a problem that it warnings about missing initializers
  # for an std::array containing std::string even when {{}} is used
  # as it should
  FLAGS_C_CC += -Wno-missing-field-initializers
endif

CXX_FULL_VERSION := $(shell $(CXX_FOR_VERSION_TEST) --version)

# Only modify LDFLAGS if DLD has been set.
ifneq ($(DLD),)
  ifeq ($(DLD),$(CXX_FOR_VERSION_TEST)) # only try this is we are using ld through gcc
    LD_FOR_VERSION_TEST = $(shell $(CXX_FOR_VERSION_TEST) -print-prog-name=ld)
  else
    LD_FOR_VERSION_TEST = $(DLD)
  endif # DLD = CXX_FOR_VERSION_TEST
  # The -E option is GNU ld specific
  ifneq ($(mingw32),1)
    GNU_LD := $(shell sh -c '$(LD_FOR_VERSION_TEST) -v 2>&1 | grep -c "GNU ld"')
  else
    GNU_LD := $(if $(findstring GNU ld,$(shell $(LD_FOR_VERSION_TEST) -v)), 1, 0)
  endif # mingw32
endif # DLD

ifeq ($(no-optimize-sibling-calls), 1)
  FLAGS_C_CC += -fno-optimize-sibling-calls
endif

ifeq ($(no_strict_aliasing), 1)
  FLAGS_C_CC += -fno-strict-aliasing
else
  ifneq ($(no_strict_aliasing), 0)
    # if not explicitly disabled enable suppression of strict-aliasing checks by default
    # for GCC >= 4.2; these checks and the resulting warnings are very controversial and
    # popular opinion on the web seems to be that it brings little practical value and a
    # lot of pain to attempt to solve code issues and the best way to deal is to suppress
    ifneq ($(findstring $(CXX_MAJOR_VERSION).$(CXX_MINOR_VERSION),4.1),$(CXX_MAJOR_VERSION).$(CXX_MINOR_VERSION))
      FLAGS_C_CC += -fno-strict-aliasing
    endif
  endif
endif

minimaltoc ?= 0
ifeq ($(minimaltoc),1)
  FLAGS_C_CC += -mminimal-toc
endif

address-sanitizer ?= 0
ifeq ($(address-sanitizer),1)
  CPPFLAGS += -fsanitize=address -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address
endif

thread-sanitizer ?= 0
ifeq ($(thread-sanitizer),1)
  CPPFLAGS += -fsanitize=thread
  LDFLAGS += -fsanitize=thread
endif

pipes ?= 1

# Things GCC has in common with Clang
include $(ACE_ROOT)/include/makeinclude/platform_gcc_clang_common.GNU

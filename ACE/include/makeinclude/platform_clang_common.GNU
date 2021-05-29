# -*- Makefile -*-

# Compiling TAO requires relaxing strict 2-phase name lookup rules:
CCFLAGS += -fdelayed-template-parsing

ifneq ($(CROSS_COMPILE),)
  CROSS-COMPILE = 1
  # Build using the cross-tools
  override_cross_compile ?= 0
  ifeq ($(override_cross_compile),0)
    CC  = ${CROSS_COMPILE}clang${CROSS_COMPILE_SUFFIX}
    CXX = ${CROSS_COMPILE}clang++${CROSS_COMPILE_SUFFIX}
    AR  = ${CROSS_COMPILE}ar${CROSS_COMPILE_SUFFIX}
  endif
  # Cross-linker requires this for linked in shared libs that depend
  # themselves on other shared libs (not directly linked in)
  LDFLAGS += -Wl,-rpath-link,$(ACE_ROOT)/lib
  ifneq (,$(HOST_ROOT))
    TAO_IDLFLAGS += -g $(HOST_ROOT)/bin/ace_gperf
    TAO_IDL = $(HOST_ROOT)/bin/tao_idl
    TAO_IDL_DEP = $(TAO_IDL)
    # make sure to use the host compiler, not the cross-compiler
    # as preprocessor for the cross-compiled idl tools
    TAO_IDL_PREPROCESSOR = clang
  endif
endif

ifeq ($(CXX),insure)
  # insure does not pass through the -dumpversion option.
  CXX_FOR_VERSION_TEST ?= g++
else
  CXX_FOR_VERSION_TEST ?= $(CXX)
endif

CXX_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion)

CXX_FULL_VERSION := $(shell $(CXX_FOR_VERSION_TEST) --version)

# Only modify LDFLAGS if DLD has been set.
ifneq ($(DLD),)
  ifeq ($(DLD),$(CXX_FOR_VERSION_TEST)) # only try this is we are using ld through gcc
    LD_FOR_VERSION_TEST = $(subst \,/,$(shell $(CXX_FOR_VERSION_TEST) -print-prog-name=ld))
  else
    LD_FOR_VERSION_TEST = $(DLD)
  endif # DLD = CXX_FOR_VERSION_TEST
  # The -E option is GNU ld specific
  GNU_LD := $(if $(findstring GNU ld,$(shell $(LD_FOR_VERSION_TEST) -v 2>&1)), 1, 0)
endif # DLD

ifeq ($(GNU_LD),1)
  STATIC_LINK_FLAG ?= -static
endif # GNU_LD

ifeq ($(no_strict_aliasing), 1)
  CCFLAGS += -fno-strict-aliasing
endif

# Things Clang has in common with GCC
include $(ACE_ROOT)/include/makeinclude/platform_gcc_clang_common.GNU

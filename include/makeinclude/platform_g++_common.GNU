# -*- Makefile -*-
# $Id$
#
# Common file help turn on/off explicit template instantiation

ifeq ($(CXX),insure)
  # insure does not pass through the -dumpversion option.
  CXX_FOR_VERSION_TEST ?= g++
else
  CXX_FOR_VERSION_TEST ?= $(CXX)
endif

CXX_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion)
ifeq (cmd,$(findstring cmd,$(SHELL)))
CXX_MAJOR_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion | sed -e "s/[^0-9\.]//g" | sed -e "s/\..*$$//")
else
CXX_MAJOR_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion | sed -e 's/[^0-9\.]//g' | sed -e 's/\..*$$//')
endif
ifeq ($(findstring $(CXX_MAJOR_VERSION),1 2 3),$(CXX_MAJOR_VERSION))
GXX_4_OR_BETTER := 0
else
GXX_4_OR_BETTER := 1
endif

# If no option has been specified, set templates to automatic
# version of the compiler.
#
templates ?= automatic

# The correct flags to pass to the linker for ELF dynamic shared library
# versioning. None of this works on OpenBSD's ld.
# If the platform file didn't already set versioned_so, default to 1.
versioned_so ?= 1
with_ld ?=
ifeq ($(versioned_so),1)
  ifeq ($(with_ld),hpux)
    SOFLAGS += -Wl,+h -Wl,$(SONAME)
  else
    ifeq ($(with_ld),openbsd)
    else
      ifeq ($(with_ld),aix)
      # What to do here???
      else
        SOFLAGS += -Wl,-h -Wl,$(SONAME)
      endif
    endif
  endif
endif

static_libs_only ?=

CXX_FULL_VERSION := $(shell $(CXX_FOR_VERSION_TEST) --version)
gcc_template_instantiation_visibility ?= 0

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
    GNU_LD := $(shell $(LD_FOR_VERSION_TEST) -v | grep -c 'GNU ld')
  endif # mingw32
endif # DLD

ifeq ($(GNU_LD),1)
  STATIC_LINK_FLAG ?= -static
endif # GNU_LD

ifeq ($(shared_libs), 1)
  ifneq ($(static_libs_only), 1)
    # Add all symbols to the dynamic symbol table.  Needed to enable
    # dynamic_cast<> for shared libraries. (see
    # http://gcc.gnu.org/faq.html#dso)

    ifeq ($(GNU_LD),1)
      # Make sure this version of ld supports the -E option.
      ifneq ($(mingw32),1)
        LD_EXPORT_DEFINED := $(shell sh -c '$(LD_FOR_VERSION_TEST) -E 2>&1 | egrep -i "(option|flag)" /dev/null; echo $$?')
      else
        LD_EXPORT_DEFINED := $(shell $(LD_FOR_VERSION_TEST) -E 2>&1 | grep -c -i -e '(option|flag)')
	    ifeq ($(LD_EXPORT_DEFINED),0)
	      LD_EXPORT_DEFINED:=1
	    else
	      LD_EXPORT_DEFINED:=0
	    endif
      endif # mingw32
      ifeq ($(LD_EXPORT_DEFINED),1)
        LDFLAGS += -Wl,-E
      endif # LD_EXPORT_DEFINED = 1
    endif # GNU ld

    # Take advantage of G++ (>= 4.x) visibility attributes to generate
    # improved shared library binaries.
    ifeq ($(GXX_4_OR_BETTER), 1)
      ifneq ($(no_hidden_visibility),1)
        CCFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
        ifeq ($(gcc_template_instantiation_visibility),1)
          CCFLAGS += -DACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS=1
        endif # gcc_template_instantiation_visibility
      else
        CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
      endif # no_hidden_visibility
    endif # GXX_4_OR_BETTER == 1
  else
    # Explicitly disable G++ (>= 4.x) visibility attributes since
    # they are only meant for use when building shared libraries.
    ifeq ($(GXX_4_OR_BETTER), 1)
      no_hidden_visibility = 1
      CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
    endif # GXX_4_OR_BETTER == 1
  endif # static_libs_only
else
  # Explicitly disable G++ (>= 4.x) visibility attributes since
  # they are only meant for use when building shared libraries.
  ifeq ($(GXX_4_OR_BETTER), 1)
    no_hidden_visibility = 1
    CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
  endif # GXX_4_OR_BETTER == 1
endif # shared_libs

# GCC 2.x needs this to compile TAO correctly 
ifeq ($(CXX_MAJOR_VERSION), 2)
  CCFLAGS += -ftemplate-depth-23
endif

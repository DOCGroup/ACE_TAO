# Shared Logic for GCC and Clang

FLAGS_C_CC += -Wall -Wextra -Wpointer-arith
CCFLAGS += -Wnon-virtual-dtor

# Suppress "Creating *.a" Message
ARFLAGS += -c

gcc_template_instantiation_visibility ?= 0
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

    # Take advantage of g++ visibility attributes to generate
    # improved shared library binaries.
    ifneq ($(no_hidden_visibility),1)
      CCFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
      ifeq ($(gcc_template_instantiation_visibility),1)
        CCFLAGS += -DACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS=1
      endif # gcc_template_instantiation_visibility
    else
      CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
    endif # no_hidden_visibility
  else
    # Explicitly disable g++ visibility attributes since
    # they are only meant for use when building shared libraries.
    no_hidden_visibility = 1
    CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
  endif # static_libs_only
else
  # Explicitly disable g++ visibility attributes since
  # they are only meant for use when building shared libraries.
  no_hidden_visibility = 1
  CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
endif # shared_libs

ifeq ($(c++20),1)
  CCFLAGS += -std=c++20
else
  ifeq ($(c++17),1)
    CCFLAGS += -std=c++17
  else
    ifeq ($(c++14),1)
      CCFLAGS += -std=c++14
    else
      ifeq ($(c++11),1)
        CCFLAGS += -std=c++11
      endif # c++11
    endif # c++14
  endif #c++17
endif #c++20

# If no option has been specified, set templates to automatic
# version of the compiler.
#
templates ?= automatic

# The correct flags to pass to the linker for ELF dynamic shared library
# versioning.
# If the platform file didn't already set versioned_so, default to 1.
versioned_so ?= 1
with_ld ?=
ifneq ($(versioned_so),0)
  ifeq ($(with_ld),hpux)
    SOFLAGS += -Wl,+h -Wl,$(SONAME)
  else
    ifeq ($(with_ld),aix)
    # What to do here???
    else
      SOFLAGS += -Wl,-h -Wl,$(SONAME)
    endif
  endif
endif

ifeq ($(versioned_so),2)
  FLAGS_C_CC += -DACE_VERSIONED_SO=2
endif

static_libs_only ?=

ifeq ($(GNU_LD),1)
  STATIC_LINK_FLAG ?= -static
endif # GNU_LD

ifeq ($(openmp),1)
  FLAGS_C_CC += -fopenmp
  LDFLAGS += -fopenmp
endif

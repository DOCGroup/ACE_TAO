# $Id$
#
# Common file help turn on/off explicit template instantiation

ifeq ($(CXX),insure)
  # insure does not pass through the -dumpversion option.
  CXX_FOR_VERSION_TEST = g++
else
  CXX_FOR_VERSION_TEST = $(CXX)
endif

CXX_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion)

# If no option has been specified, try to enable templates based on the
# version of the compiler.
#
ifeq ($(templates),)
  ifeq (2.95,$(findstring 2.95,$(CXX_VERSION)))
    templates = automatic
  else
    ifeq (3.,$(findstring 3.,$(CXX_VERSION)))
      templates = automatic
    else
      ifeq (2.96,$(findstring 2.96,$(CXX_VERSION)))
        templates = automatic
      else
        ifeq (egcs, $(findstring egcs, $(CXX_VERSION)))
          templates = explicit
        else
          templates = explicit
        endif
      endif
    endif
  endif
endif

# Turn on the proper flags for explicit template instantiation.
#
ifeq ($(templates),explicit)
  ifeq ($(TEMPLATES_FLAG),) # Turn on flags if none is speficied.
    TEMPLATES_FLAG=-fno-implicit-templates
  endif
  CPPFLAGS += -DACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
endif

# The correct flags to pass to the linker for ELF dynamic shared library
# versioning
#
ifneq ($(SONAME),)
  ifeq ($(with_ld),hpux)
    SOFLAGS += -Wl,+h -Wl,$(SONAME)
  else
    ifneq ($(with_ld),vxworks)
      ifneq ($(with_ld),aix)
        SOFLAGS += -Wl,-h -Wl,$(SONAME)
      endif
    endif
  endif
endif

# Add all symbols to the dynamic symbol table.  Needed to enable dynamic_cast 
# for shared libraries. (see http://gcc.gnu.org/faq.html#dso)
ifeq ($(shared_libs), 1)
  SOFLAGS += -Wl,-E
endif # shared_libs


# $Id$
#
# Common file help turn on/off explicit template instantiation

CXX_VERSION := $(shell $(CXX) -dumpversion)

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
  SOFLAGS += -Wl,-h -Wl,$(SONAME)
endif

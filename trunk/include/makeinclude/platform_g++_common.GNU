# $Id$
#
# Common file help turn on/off explicit template instantiation

CXX_VERSION := $(shell $(CXX) -dumpversion)

# If no option has been specified, try to enable implicit_templates based
# on the version of the compiler.
#
ifeq ($(templates),)
  ifeq (2.95,$(findstring 2.95,$(CXX_VERSION)))
    templates = "automatic"
  else
    ifeq (3.,$(findstring 3.,$(CXX_VERSION)))
      templates = "automatic"
    else
      ifeq (2.96,$(findstring 2.96,$(CXX_VERSION)))
        templates = "automatic"
      else
        templates = "explicit"
      endif
    endif
  endif
endif

# User didn't specify and we couldn't guess if this version of the compiler
# works fine with implicit template instantiation. Turn if off.
#
ifeq ("$(templates)","explicit")
  ifeq ($(TEMPLATES_FLAG),) # Turn on flags if none is speficied.
    TEMPLATES_FLAG=-fno-implicit-templates
  endif
  CPPFLAGS += -DACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
endif

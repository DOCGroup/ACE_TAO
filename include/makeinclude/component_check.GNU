# -*- Makefile -*-

#----------------------------------------------------------------------------
#  $Id$
#----------------------------------------------------------------------------

# Only include once.
ifndef COMPONENT_CHECK_GNU
COMPONENT_CHECK_GNU = 1

# Compile by default
COMPILE_COMPONENT = 1

# Load all components.
ALL_COMPONENTS := $(ACE_MAKE_OPTIONS)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --tao)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --pace)

# Filter out all the components that are currently available from the
# list of requirements.
MISSING_COMPONENTS := $(filter-out $(ALL_COMPONENTS), $(REQUIRED_COMPONENTS))

# If anything is left, we're missing requirements, so don't compile it.
ifneq ($(MISSING_COMPONENTS),)
  COMPILE_COMPONENT = 0
endif

# Now set the targets, if not already set.
ifeq ($(COMPILE_COMPONENT),1)
  ifneq ($(CLEANUP_BIN),)
    ifeq ($(BIN),)
      BIN = $(CLEANUP_BIN)
    endif
  endif
  ifneq ($(CLEANUP_LIB),)
    ifeq ($(LIB),)
      LIB = $(CLEANUP_LIB)
    endif
  endif
  ifneq ($(CLEANUP_SHLIB),)
    ifeq ($(SHLIB),)
      SHLIB = $(CLEANUP_SHLIB)
    endif
  endif
endif 

endif # COMPONENT_CHECK_GNU

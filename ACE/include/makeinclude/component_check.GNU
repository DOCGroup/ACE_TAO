# -*- Makefile -*-

#----------------------------------------------------------------------------
#  $Id$
#----------------------------------------------------------------------------

# Load all components.
ALL_COMPONENTS := $(ACE_MAKE_OPTIONS)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --tao)
ALL_COMPONENTS += $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)

# Filter out all the components that are currently available from the
# list of requirements.
MISSING_COMPONENTS := $(filter-out $(ALL_COMPONENTS), $(REQUIRED_COMPONENTS))

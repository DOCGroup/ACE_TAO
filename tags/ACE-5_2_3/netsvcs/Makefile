#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the ACE network services
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

INFO  = README

# lib must come first!
ACE_BUILD_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --ace)
ifeq (Other,$(findstring Other,$(ACE_BUILD_COMPONENTS)))
DIRS  = lib \
        clients \
	servers
endif # ! Other

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

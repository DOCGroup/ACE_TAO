#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the client programs that test the ACE network services
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

#### If the ACE library wasn't built with the Other component, don't
#### try to build the clients.
ACE_BUILD_COMPONENTS := $(shell $(ACE_ROOT)/bin/ace_components --ace)
ifeq (Other,$(findstring Other,$(ACE_BUILD_COMPONENTS)))
  DIRS  = Logger \
          Naming \
          Tokens
else
  DIRS =
endif # ! Other

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the client programs that test the ACE network services
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Logger \
        Naming \
        Tokens

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

#### If ACE_COMPONENTS was set and doesn't contain Other, don't
#### try to build any of the clients.
ifdef ACE_COMPONENTS
  ifeq (,$(findstring Other,$(ACE_COMPONENTS)))
    DIRS =
  endif # ! Other
endif # ACE_COMPONENTS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU


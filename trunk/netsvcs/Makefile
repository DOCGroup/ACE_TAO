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
DIRS  = lib \
        clients

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

#### If ACE_COMPONENTS was set and doesn't contain Other and Token, don't
#### try to build the servers.
ifdef ACE_COMPONENTS
  ifeq (Other,$(findstring Other,$(ACE_COMPONENTS)))
    ifeq (Token,$(findstring Token,$(ACE_COMPONENTS)))
      DIRS  += servers
    endif # Token
  endif # Other
else  # ! ACE_COMPONENTS
  DIRS  += servers
endif # ! ACE_COMPONENTS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

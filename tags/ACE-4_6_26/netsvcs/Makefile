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

ifeq (Other,$(findstring Other,$(ACE_COMPONENTS)))
  ifeq (Token,$(findstring Token,$(ACE_COMPONENTS)))
    DIRS  += servers
  endif # Token
endif # Other

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

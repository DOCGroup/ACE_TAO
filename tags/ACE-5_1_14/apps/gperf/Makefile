#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(CROSS-COMPILE),)
  DIRS = src tests
else
  DIRS=
endif # ! CROSS-COMPILE

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

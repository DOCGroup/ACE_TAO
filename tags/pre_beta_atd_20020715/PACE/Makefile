#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the PACE directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/PACE
endif # ! PACE_ROOT

INFO  = README

DIRS  =	pace \
	tests

# Add these DIRS later - keep it simple for now
#	pace++ \
#	examples

# Makefiles TBD:
#	ttcp
#       CORBA


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
ifdef CHORUS
DIRS  =
  all clean realclean:
	@echo PACE won\'t run or build for Chorus.
else
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
endif # VXWORKS

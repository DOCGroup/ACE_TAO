#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the TAO Cubit tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

# Currently, IDL_Cubit *must* come before the IDL_Cubit tests.
DIRS  = IDL_Cubit \
	MT_Cubit

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifndef minimum_corba

# No DII in minimum CORBA
  DIRS += DII_Cubit

endif # minimum_corba

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU


#----------------------------------------------------------------------------
#
#	$Id$
#
#	Makefile for the TAO examples
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

DIRS  = Callback_Quoter \
        POA \
	Logging \
	Simple \
	Event_Comm \
	OBV

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifndef minimum_corba

# No POA forwarding in minimum CORBA
  DIRS += Quoter

endif # minimum_corba

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

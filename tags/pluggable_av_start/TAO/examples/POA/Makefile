#----------------------------------------------------------------------------
#
# $Id$
#
# Makefile for the POA tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

DIRS  = Generic_Servant \
	Adapter_Activator \
	DSI \
	Default_Servant \
	Explicit_Activation \
	FindPOA \
	Forwarding \
	Identity \
	Loader \
	NewPOA \
	On_Demand_Activation \
	On_Demand_Loading \
	RootPOA \
	Reference_Counted_Servant \
	TIE

# The following test hasn't been updated yet
# Demux_Test

#### Does not compile on UNIX:	Forwarding \

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

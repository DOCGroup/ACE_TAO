#----------------------------------------------------------------------------
#
# $Id$
#
# Makefile for the POA tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Generic_Servant \
        RootPOA \
        Reference_Counted_Servant \
	Adapter_Activator \
	DSI \
	Default_Servant \
	Explicit_Activation \
	FindPOA \
	Forwarding \
	Loader \
	On_Demand_Activation \
	On_Demand_Loading \
	NewPOA \
	TIE

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO examples
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Callback_Quoter \
        POA \
        Logging \
        Simple \
        Event_Comm \
        OBV \
        Load_Balancing \
	Persistent_Grid \
	AMI \
	Quoter \
	Buffered_Oneways \
	Buffered_AMI \
	Load_Balancing_persistent \
	Simulator

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

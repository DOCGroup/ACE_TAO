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
	OBV \
	Load_Balancing

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifndef minimum_corba

# No POA forwarding in minimum CORBA
  DIRS += Quoter

endif # minimum_corba

ifdef corba_messaging

# No Buffered Oneways without CORBA Messaging
  DIRS += Buffered_Oneways

endif # corba_messaging

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

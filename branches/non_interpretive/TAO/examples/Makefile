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
	Persistent_Grid

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)

# No POA forwarding in minimum CORBA
  DIRS += Quoter

endif # minimum_corba

ifeq ($(corba_messaging),1)

# No Buffered Oneways without CORBA Messaging
  DIRS += Buffered_Oneways

  ifeq ($(ami_callback),1)

    # No Buffered Requests without CORBA Messaging and requires AMI callbacks
    DIRS += Buffered_AMI

  endif # ami_callback

endif # corba_messaging

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

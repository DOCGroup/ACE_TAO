#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO tests
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = CDR \
        IDL_Test \
        OctetSeq \
        InterOp-Naming \
        NestedUpcall \
        Multiple_Inheritance \
        MT_Client \
        MT_Server \
        FL_Cube \
        POA \
        IORManipulation \
        Connection_Purging \
        ORB_init \
        Smart_Proxies \
        Nested_Event_Loop \
        Leader_Followers \
	Object_Loader

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)

# No DynAny in minimum CORBA
  DIRS += DynAny_Test

# No NVList and Context in minimum CORBA
  DIRS += Param_Test \
          DSI_Gateway

# No Implicit activation in minimum CORBA
  DIRS += Faults

endif # minimum_corba

ifeq ($(ami_callback),1)

# Requires AMI callbacks
  DIRS += AMI

endif # ami_callback

ifeq ($(corba_messaging),1)

# Requires CORBA Messaging
  DIRS += Timeout \
          Timed_Buffered_Oneways

endif # corba_messaging

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

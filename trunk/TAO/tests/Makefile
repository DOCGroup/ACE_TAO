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
        Nested_Event_Loop \
        Leader_Followers \
	Object_Loader \
	MProfile \
	MProfile_Forwarding \
	Native_Exceptions \
	Faults \
	Param_Test \
	DSI_Gateway \
	AMI \
	Timeout \
	Timed_Buffered_Oneways \
	Smart_Proxies \
	Interceptors \
	Endpoint_Per_Priority \
	Explicit_Event_Loop

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

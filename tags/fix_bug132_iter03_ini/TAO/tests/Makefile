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
	Codec \
        BiDirectional \
	BiDirectional_NestedUpcall \
	Collocation \
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
        ORB_destroy \
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
	AMI_Timeouts \
	Timeout \
	Timed_Buffered_Oneways \
	Smart_Proxies \
	Portable_Interceptors \
	Endpoint_Per_Priority \
	Explicit_Event_Loop \
	DynAny_Test \
	Policies \
	LongUpcalls \
	RTCORBA \
	Forwarding \
	Exposed_Policies \
	Smart_Proxies \
	Single_Read \
	QtTests \
	Multiple \
	LongWrites \
	Crashed_Callback \
	Client_Leaks \
	Server_Leaks \
	Muxing \
	Hello

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

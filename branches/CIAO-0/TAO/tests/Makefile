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
        OBV \
        IORManipulation \
        Connection_Purging \
        ORB_init \
        ORB_destroy \
	Objref_Sequence_Test \
        File_IO \
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
	Crashed_Callback \
	Client_Leaks \
	Server_Leaks \
	Muxing \
	Muxed_GIOP_Versions \
	Hello \
	Cache_Growth_Test \
	MT_Timeout \
	Crash_On_Write \
	Reliable_Oneways \
	Blocking_Sync_None \
	Oneway_Buffering \
	Queued_Message_Test \
	Big_Oneways \
	Big_Twoways \
	Big_Reply \
	Big_Request_Muxing \
	Oneways_Invoking_Twoways \
	AMI_Buffering \
	DLL_ORB \
	Two_Objects

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifdef VXWORKS
  DIRS := TestUtils $(DIRS)
endif

include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

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
	AMH_Exceptions \
	Codec \
        BiDirectional \
	BiDirectional_NestedUpcall \
	Bug_1269_Regression \
	Bug_1270_Regression \
	Bug_1020_Regression \
	Bug_1361_Regression \
	Bug_1020_Basic_Regression \
	Bug_1482_Regression \
	Collocation_Tests \
	CodeSets \
	Collocation \
	Connection_Timeout \
        IDL_Test \
        OctetSeq \
        InterOp-Naming \
        NestedUpcall \
        Multiple_Inheritance \
        MT_Client \
        MT_Server \
        MT_BiDir \
        FL_Cube \
        POA \
        OBV \
        IORManipulation \
        Connection_Purging \
        Server_Connection_Purging \
        ORB_init \
        ORB_destroy \
	Objref_Sequence_Test \
	No_Server_MT_Connect_Test \
	Connect_Strategy_Test \
        File_IO \
        Nested_Event_Loop \
        Leader_Followers \
	Object_Loader \
	MProfile \
	MProfile_Forwarding \
	MProfile_Connection_Timeout \
	Native_Exceptions \
	Faults \
	Param_Test \
	DSI_Gateway \
	AMI \
	AMI_Timeouts \
	Timeout \
	Timed_Buffered_Oneways \
	Smart_Proxies \
	Stack_Recursion \
	Portable_Interceptors \
	ORT \
	Explicit_Event_Loop \
	DynAny_Test \
	Policies \
	LongUpcalls \
	RTCORBA \
	Forwarding \
	Exposed_Policies \
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
        ORB_shutdown \
	Two_Objects \
	Nested_Upcall_Crash

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

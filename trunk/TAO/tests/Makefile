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
	Smart_Proxies \
	Portable_Interceptors \
	Explicit_Event_Loop \
	DynAny_Test \
	RTCORBA \
	Forwarding \
	Exposed_Policies \
	Smart_Proxies \
	Single_Read \
	QtTests \
	Multiple \
	Client_Leaks \
	Server_Leaks \
	Muxing \
	Cache_Growth_Test \
	Crash_On_Write \
	Queued_Message_Test \
	Big_Oneways \
	Big_Twoways \
	Big_Reply \
	Oneways_Invoking_Twoways \
	DLL_ORB

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(corba_messaging),1)

DIRS += \
	AMI \
	AMI_Buffering \
	AMI_Timeouts \
	Big_Request_Muxing \
	Blocking_Sync_None \
	Crashed_Callback \
	DSI_Gateway \
	Hello \
	LongUpcalls \
	MT_Timeout \
	Oneway_Buffering \
	Param_Test \
	Policies \
	Reliable_Oneways \
	Strategies \
	Timed_Buffered_Oneways \
	Timeout \
	Two_Objects

endif # corba_messaging


ifdef VXWORKS
  DIRS := TestUtils $(DIRS)
endif

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

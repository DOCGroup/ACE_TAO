# Microsoft Developer Studio Project File - Name="TAO LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Static.mak" CFG="TAO LIB - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO LIB - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TAO_Stat"
# PROP BASE Intermediate_Dir "TAO_Stat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "../../" /I "../" /D "_WINDOWS" /D "_CONSOLE" /D "NDEBUG" /D "WIN32" /D "TAO_AS_STATIC_LIBS" /D "ACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAOs.lib"

!ELSEIF  "$(CFG)" == "TAO LIB - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TAO_Sta0"
# PROP BASE Intermediate_Dir "TAO_Sta0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /Od /I "../../" /I "../" /D "_WINDOWS" /D "_CONSOLE" /D "_DEBUG" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAOsd.lib"

!ENDIF 

# Begin Target

# Name "TAO LIB - Win32 Static Release"
# Name "TAO LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Abstract_Servant_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Any.cpp
# End Source File
# Begin Source File

SOURCE=.\append.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Queued_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Base_Transport_Property.cpp
# End Source File
# Begin Source File

SOURCE=.\BiDir_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.cpp
# End Source File
# Begin Source File

SOURCE=.\Block_Flushing_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Blocked_Connect_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\BoundsC.cpp
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\Cache_Entries.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR_Encaps_Codec.cpp
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Func_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientInterceptorAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CodecFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\CodecFactory_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\Codeset_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Codeset_Translator_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Collocation_Resolver.cpp
# End Source File
# Begin Source File

SOURCE=.\Connect_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection_Purging_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Connector_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBA_String.cpp
# End Source File
# Begin Source File

SOURCE=.\corbafwd.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBALOC_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBANAME_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrentC.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\default_client.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Collocation_Resolver.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Endpoint_Selector_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Protocols_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\default_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\default_server.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Stub_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Thread_Lane_Resources_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\DLL_ORB.cpp
# End Source File
# Begin Source File

SOURCE=.\DLL_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\DomainC.cpp
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicC.cpp
# End Source File
# Begin Source File

SOURCE=.\Encodable.cpp
# End Source File
# Begin Source File

SOURCE=.\Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Endpoint_Selector_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Environment.cpp
# End Source File
# Begin Source File

SOURCE=.\Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\Exclusive_TMS.cpp
# End Source File
# Begin Source File

SOURCE=.\Fault_Tolerance_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\FILE_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Flushing_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_10.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_11.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_12.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Locate_Header.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Version.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOPC.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_Client_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Lite_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Incoming_Message_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Interceptor_List.cpp
# End Source File
# Begin Source File

SOURCE=.\Invocation.cpp
# End Source File
# Begin Source File

SOURCE=.\Invocation_Endpoint_Selectors.cpp
# End Source File
# Begin Source File

SOURCE=.\IOP_CodecC.cpp
# End Source File
# Begin Source File

SOURCE=.\IOP_IORC.cpp
# End Source File
# Begin Source File

SOURCE=.\IOPC.cpp
# End Source File
# Begin Source File

SOURCE=.\IOR_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\IORInterceptor_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.cpp
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower_Flushing_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_CH_Event.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Connect_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Event.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Event_Binder.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Event_Loop_Thread_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Follower.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Follower_Auto_Adder.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Follower_Auto_Ptr.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Invocation_Event.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy_Complete.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LRU_Connection_Purging_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Marshal.cpp
# End Source File
# Begin Source File

SOURCE=.\MCAST_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_PolicyValueC.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_SyncScopeC.cpp
# End Source File
# Begin Source File

SOURCE=.\MProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Muxed_TMS.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\NVList.cpp
# End Source File
# Begin Source File

SOURCE=.\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Broker.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Ref_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectIdListC.cpp
# End Source File
# Begin Source File

SOURCE=.\OctetSeqC.cpp
# End Source File
# Begin Source File

SOURCE=.\operation_details.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Core.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Core_Auto_Ptr.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\ORBInitializer_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\ORBInitInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\params.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\PI_ForwardC.cpp
# End Source File
# Begin Source File

SOURCE=.\PICurrent.cpp
# End Source File
# Begin Source File

SOURCE=.\PICurrent_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.cpp
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_ForwardC.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyC.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyFactory_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\PollableC.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.cpp
# End Source File
# Begin Source File

SOURCE=.\PredefinedType_Seq_Tmplinst.cpp
# End Source File
# Begin Source File

SOURCE=.\Principal.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocol_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\Queued_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactive_Connect_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactive_Flushing_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Broker.cpp
# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Request_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestInfo_Util.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Resume_Handle.cpp
# End Source File
# Begin Source File

SOURCE=.\Sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Context.cpp
# End Source File
# Begin Source File

SOURCE=.\Services_Activate.cpp
# End Source File
# Begin Source File

SOURCE=.\ServicesC.cpp
# End Source File
# Begin Source File

SOURCE=.\skip.cpp
# End Source File
# Begin Source File

SOURCE=.\StringSeqC.cpp
# End Source File
# Begin Source File

SOURCE=.\Stub.cpp
# End Source File
# Begin Source File

SOURCE=.\Stub_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch_Queued_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch_Refcountable.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch_Reply_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Tagged_Components.cpp
# End Source File
# Begin Source File

SOURCE=.\Tagged_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO_Server_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\TAOC.cpp
# End Source File
# Begin Source File

SOURCE=.\target_specification.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Lane_Resources.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Lane_Resources_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Cache_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Descriptor_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Mux_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Typecode.cpp
# End Source File
# Begin Source File

SOURCE=.\Typecode_Constants.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeCodeFactory_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Valuetype_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Leader_Follower.cpp
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Read.cpp
# End Source File
# Begin Source File

SOURCE=.\Wait_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\WrongTransactionC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;fi;fd"
# Begin Source File

SOURCE=.\Abstract_Servant_Base.h
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Adapter.h
# End Source File
# Begin Source File

SOURCE=.\any.h
# End Source File
# Begin Source File

SOURCE=.\Any_Array_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\Any_Basic_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\Any_Dual_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\Any_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\Any_Special_Basic_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\Any_Special_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\Any_Specializations.h
# End Source File
# Begin Source File

SOURCE=.\arg_shifter.h
# End Source File
# Begin Source File

SOURCE=.\Array_VarOut_T.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Queued_Message.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher_Base.h
# End Source File
# Begin Source File

SOURCE=.\Base_Transport_Property.h
# End Source File
# Begin Source File

SOURCE=.\BiDir_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.h
# End Source File
# Begin Source File

SOURCE=.\Block_Flushing_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Blocked_Connect_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\BoundsC.h
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.h
# End Source File
# Begin Source File

SOURCE=.\Cache_Entries.h
# End Source File
# Begin Source File

SOURCE=.\cdr.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Encaps_Codec.h
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Func_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Func_Registry.inl
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\ClientInterceptorAdapter.h
# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo.h
# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo_i.h
# End Source File
# Begin Source File

SOURCE=.\CodecFactory.h
# End Source File
# Begin Source File

SOURCE=.\CodecFactory_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\Codeset_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Codeset_Translator_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Collocation_Resolver.h
# End Source File
# Begin Source File

SOURCE=.\Connect_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Purging_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Connector_Registry.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES_T.h
# End Source File
# Begin Source File

SOURCE=.\corba.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_String.h
# End Source File
# Begin Source File

SOURCE=.\corbafwd.h
# End Source File
# Begin Source File

SOURCE=.\CORBALOC_Parser.h
# End Source File
# Begin Source File

SOURCE=.\CORBANAME_Parser.h
# End Source File
# Begin Source File

SOURCE=.\CurrentC.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\default_client.h
# End Source File
# Begin Source File

SOURCE=.\Default_Collocation_Resolver.h
# End Source File
# Begin Source File

SOURCE=.\Default_Endpoint_Selector_Factory.h
# End Source File
# Begin Source File

SOURCE=.\default_ports.h
# End Source File
# Begin Source File

SOURCE=.\Default_Protocols_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\default_resource.h
# End Source File
# Begin Source File

SOURCE=.\default_server.h
# End Source File
# Begin Source File

SOURCE=.\Default_Stub_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Default_Thread_Lane_Resources_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\DLL_ORB.h
# End Source File
# Begin Source File

SOURCE=.\DLL_Parser.h
# End Source File
# Begin Source File

SOURCE=.\DomainC.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\DynamicC.h
# End Source File
# Begin Source File

SOURCE=.\Encodable.h
# End Source File
# Begin Source File

SOURCE=.\Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\Endpoint_Selector_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Environment.h
# End Source File
# Begin Source File

SOURCE=.\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Exclusive_TMS.h
# End Source File
# Begin Source File

SOURCE=.\Fault_Tolerance_Service.h
# End Source File
# Begin Source File

SOURCE=.\FILE_Parser.h
# End Source File
# Begin Source File

SOURCE=.\Flushing_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\giop.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_10.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_11.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_12.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_Impl.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Locate_Header.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Version.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.h
# End Source File
# Begin Source File

SOURCE=.\GIOPC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_Client_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Lite_Factory.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\IIOPC.h
# End Source File
# Begin Source File

SOURCE=.\Incoming_Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Incoming_Message_Queue.inl
# End Source File
# Begin Source File

SOURCE=.\Interceptor_List.h
# End Source File
# Begin Source File

SOURCE=.\Invocation.h
# End Source File
# Begin Source File

SOURCE=.\Invocation_Endpoint_Selectors.h
# End Source File
# Begin Source File

SOURCE=.\IOP_CodecC.h
# End Source File
# Begin Source File

SOURCE=.\IOP_IORC.h
# End Source File
# Begin Source File

SOURCE=.\IOPC.h
# End Source File
# Begin Source File

SOURCE=.\ior_corbaloc_export.h
# End Source File
# Begin Source File

SOURCE=.\ior_corbaname_export.h
# End Source File
# Begin Source File

SOURCE=.\IOR_Parser.h
# End Source File
# Begin Source File

SOURCE=.\IORInfo.h
# End Source File
# Begin Source File

SOURCE=.\IORInterceptor_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.h
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower_Flushing_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\LF_CH_Event.h
# End Source File
# Begin Source File

SOURCE=.\LF_Connect_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\LF_Event.h
# End Source File
# Begin Source File

SOURCE=.\LF_Event.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Event_Binder.h
# End Source File
# Begin Source File

SOURCE=.\LF_Event_Binder.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Event_Loop_Thread_Helper.h
# End Source File
# Begin Source File

SOURCE=.\LF_Event_Loop_Thread_Helper.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Follower.h
# End Source File
# Begin Source File

SOURCE=.\LF_Follower.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Follower_Auto_Adder.h
# End Source File
# Begin Source File

SOURCE=.\LF_Follower_Auto_Adder.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Follower_Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\LF_Follower_Auto_Ptr.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Invocation_Event.h
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy.inl
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy_Complete.h
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy_Complete.inl
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\LocalObject.h
# End Source File
# Begin Source File

SOURCE=.\LRU_Connection_Purging_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.h
# End Source File
# Begin Source File

SOURCE=.\Marshal.h
# End Source File
# Begin Source File

SOURCE=.\MCAST_Parser.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_PolicyValueC.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_SyncScopeC.h
# End Source File
# Begin Source File

SOURCE=.\MProfile.h
# End Source File
# Begin Source File

SOURCE=.\Muxed_TMS.h
# End Source File
# Begin Source File

SOURCE=.\Notify_Handler.h
# End Source File
# Begin Source File

SOURCE=.\nvlist.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.h
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.h
# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Broker.h
# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Object_Ref_Table.h
# End Source File
# Begin Source File

SOURCE=.\objectid.h
# End Source File
# Begin Source File

SOURCE=.\ObjectIdListC.h
# End Source File
# Begin Source File

SOURCE=.\Objref_VarOut_T.h
# End Source File
# Begin Source File

SOURCE=.\OctetSeqC.h
# End Source File
# Begin Source File

SOURCE=.\operation_details.h
# End Source File
# Begin Source File

SOURCE=.\orb.h
# End Source File
# Begin Source File

SOURCE=.\orb_core.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Core_Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Table.h
# End Source File
# Begin Source File

SOURCE=.\orbconf.h
# End Source File
# Begin Source File

SOURCE=.\ORBInitializer_Registry.h
# End Source File
# Begin Source File

SOURCE=.\ORBInitInfo.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.h
# End Source File
# Begin Source File

SOURCE=.\PI_ForwardC.h
# End Source File
# Begin Source File

SOURCE=.\PICurrent.h
# End Source File
# Begin Source File

SOURCE=.\PICurrent_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Policy_ForwardC.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Set.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Validator.h
# End Source File
# Begin Source File

SOURCE=.\PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\PolicyFactory_Registry.h
# End Source File
# Begin Source File

SOURCE=.\PollableC.h
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.h
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.h
# End Source File
# Begin Source File

SOURCE=.\Principal.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\Protocol_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Protocols_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\Pseudo_VarOut_T.h
# End Source File
# Begin Source File

SOURCE=.\Queued_Message.h
# End Source File
# Begin Source File

SOURCE=.\Queued_Message.inl
# End Source File
# Begin Source File

SOURCE=.\Reactive_Connect_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Reactive_Flushing_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Broker.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Request_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\RequestInfo_Util.h
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Resume_Handle.h
# End Source File
# Begin Source File

SOURCE=.\Resume_Handle.inl
# End Source File
# Begin Source File

SOURCE=.\Seq_Out_T.h
# End Source File
# Begin Source File

SOURCE=.\Seq_Var_T.h
# End Source File
# Begin Source File

SOURCE=.\sequence.h
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.h
# End Source File
# Begin Source File

SOURCE=.\Service_Context.h
# End Source File
# Begin Source File

SOURCE=.\Services_Activate.h
# End Source File
# Begin Source File

SOURCE=.\ServicesC.h
# End Source File
# Begin Source File

SOURCE=.\singletons.h
# End Source File
# Begin Source File

SOURCE=.\StringSeqC.h
# End Source File
# Begin Source File

SOURCE=.\stub.h
# End Source File
# Begin Source File

SOURCE=.\Stub_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.h
# End Source File
# Begin Source File

SOURCE=.\Synch_Queued_Message.h
# End Source File
# Begin Source File

SOURCE=.\Synch_Refcountable.h
# End Source File
# Begin Source File

SOURCE=.\Synch_Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Tagged_Components.h
# End Source File
# Begin Source File

SOURCE=.\Tagged_Profile.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Export.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.h
# End Source File
# Begin Source File

SOURCE=.\TAOC.h
# End Source File
# Begin Source File

SOURCE=.\target_specification.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Lane_Resources.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Lane_Resources_Manager.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.h
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.h
# End Source File
# Begin Source File

SOURCE=.\Transport.h
# End Source File
# Begin Source File

SOURCE=.\Transport.inl
# End Source File
# Begin Source File

SOURCE=.\Transport_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Cache_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Connector.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Descriptor_Interface.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Mux_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Timer.h
# End Source File
# Begin Source File

SOURCE=.\try_macros.h
# End Source File
# Begin Source File

SOURCE=.\typecode.h
# End Source File
# Begin Source File

SOURCE=.\TypeCodeFactory_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\varbase.h
# End Source File
# Begin Source File

SOURCE=.\VarOut_T.h
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Leader_Follower.h
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Read.h
# End Source File
# Begin Source File

SOURCE=.\Wait_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\WrongTransactionC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Acceptor_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.i
# End Source File
# Begin Source File

SOURCE=.\Adapter.i
# End Source File
# Begin Source File

SOURCE=.\any.i
# End Source File
# Begin Source File

SOURCE=.\Any_Array_Impl_T.inl
# End Source File
# Begin Source File

SOURCE=.\Any_Basic_Impl_T.inl
# End Source File
# Begin Source File

SOURCE=.\Any_Dual_Impl_T.inl
# End Source File
# Begin Source File

SOURCE=.\Any_Impl_T.inl
# End Source File
# Begin Source File

SOURCE=.\Any_Special_Basic_Impl_T.inl
# End Source File
# Begin Source File

SOURCE=.\Any_Special_Impl_T.inl
# End Source File
# Begin Source File

SOURCE=.\Array_VarOut_T.inl
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher_Base.i
# End Source File
# Begin Source File

SOURCE=.\Base_Transport_Property.inl
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.i
# End Source File
# Begin Source File

SOURCE=.\BoundsC.i
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.i
# End Source File
# Begin Source File

SOURCE=.\Cache_Entries.i
# End Source File
# Begin Source File

SOURCE=.\cdr.i
# End Source File
# Begin Source File

SOURCE=.\ClientInterceptorAdapter.inl
# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo_i.inl
# End Source File
# Begin Source File

SOURCE=.\Collocation_Resolver.i
# End Source File
# Begin Source File

SOURCE=.\Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\Connection_Purging_Strategy.inl
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_String.inl
# End Source File
# Begin Source File

SOURCE=.\corbafwd.i
# End Source File
# Begin Source File

SOURCE=.\CORBALOC_Parser.i
# End Source File
# Begin Source File

SOURCE=.\CORBANAME_Parser.i
# End Source File
# Begin Source File

SOURCE=.\CurrentC.i
# End Source File
# Begin Source File

SOURCE=.\default_client.i
# End Source File
# Begin Source File

SOURCE=.\Default_Collocation_Resolver.i
# End Source File
# Begin Source File

SOURCE=.\Default_Protocols_Hooks.i
# End Source File
# Begin Source File

SOURCE=.\default_resource.i
# End Source File
# Begin Source File

SOURCE=.\default_server.i
# End Source File
# Begin Source File

SOURCE=.\Default_Thread_Lane_Resources_Manager.i
# End Source File
# Begin Source File

SOURCE=.\DLL_ORB.inl
# End Source File
# Begin Source File

SOURCE=.\DLL_Parser.i
# End Source File
# Begin Source File

SOURCE=.\DomainC.i
# End Source File
# Begin Source File

SOURCE=.\DynamicC.i
# End Source File
# Begin Source File

SOURCE=.\Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\Environment.i
# End Source File
# Begin Source File

SOURCE=.\Exception.i
# End Source File
# Begin Source File

SOURCE=.\Fault_Tolerance_Service.i
# End Source File
# Begin Source File

SOURCE=.\FILE_Parser.i
# End Source File
# Begin Source File

SOURCE=.\giop.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_10.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_11.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_12.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_Impl.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Locate_Header.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Version.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.i
# End Source File
# Begin Source File

SOURCE=.\GIOPC.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.i
# End Source File
# Begin Source File

SOURCE=.\IIOPC.i
# End Source File
# Begin Source File

SOURCE=.\Interceptor_List.inl
# End Source File
# Begin Source File

SOURCE=.\Invocation.i
# End Source File
# Begin Source File

SOURCE=.\Invocation_Endpoint_Selectors.i
# End Source File
# Begin Source File

SOURCE=.\IOP_CodecC.i
# End Source File
# Begin Source File

SOURCE=.\IOP_IORC.i
# End Source File
# Begin Source File

SOURCE=.\IOPC.i
# End Source File
# Begin Source File

SOURCE=.\IOR_Parser.i
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.i
# End Source File
# Begin Source File

SOURCE=.\LF_Invocation_Event.inl
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\LocalObject.i
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.i
# End Source File
# Begin Source File

SOURCE=.\Marshal.i
# End Source File
# Begin Source File

SOURCE=.\MCAST_Parser.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_PolicyValueC.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_SyncScopeC.i
# End Source File
# Begin Source File

SOURCE=.\MProfile.i
# End Source File
# Begin Source File

SOURCE=.\NVList.i
# End Source File
# Begin Source File

SOURCE=.\object.i
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.i
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.i
# End Source File
# Begin Source File

SOURCE=.\ObjectIdListC.i
# End Source File
# Begin Source File

SOURCE=.\OctetSeqC.i
# End Source File
# Begin Source File

SOURCE=.\operation_details.i
# End Source File
# Begin Source File

SOURCE=.\ORB.i
# End Source File
# Begin Source File

SOURCE=.\orb_core.i
# End Source File
# Begin Source File

SOURCE=.\ORB_Core_Auto_Ptr.inl
# End Source File
# Begin Source File

SOURCE=.\ORB_Table.inl
# End Source File
# Begin Source File

SOURCE=.\params.i
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.i
# End Source File
# Begin Source File

SOURCE=.\PI_ForwardC.i
# End Source File
# Begin Source File

SOURCE=.\PICurrent.inl
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.i
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.i
# End Source File
# Begin Source File

SOURCE=.\Policy_ForwardC.i
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Policy_Set.i
# End Source File
# Begin Source File

SOURCE=.\PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\PollableC.i
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.i
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.i
# End Source File
# Begin Source File

SOURCE=.\Principal.i
# End Source File
# Begin Source File

SOURCE=.\Profile.i
# End Source File
# Begin Source File

SOURCE=.\Pseudo_VarOut_T.inl
# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.i
# End Source File
# Begin Source File

SOURCE=.\Seq_Out_T.inl
# End Source File
# Begin Source File

SOURCE=.\Seq_Var_T.inl
# End Source File
# Begin Source File

SOURCE=.\sequence.i
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.i
# End Source File
# Begin Source File

SOURCE=.\Service_Context.inl
# End Source File
# Begin Source File

SOURCE=.\ServicesC.i
# End Source File
# Begin Source File

SOURCE=.\StringSeqC.i
# End Source File
# Begin Source File

SOURCE=.\stub.i
# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.i
# End Source File
# Begin Source File

SOURCE=.\Synch_Refcountable.inl
# End Source File
# Begin Source File

SOURCE=.\Tagged_Components.i
# End Source File
# Begin Source File

SOURCE=.\Tagged_Profile.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.inl
# End Source File
# Begin Source File

SOURCE=.\TAOC.i
# End Source File
# Begin Source File

SOURCE=.\target_specification.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Lane_Resources.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Lane_Resources_Manager.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.i
# End Source File
# Begin Source File

SOURCE=.\Transport_Acceptor.inl
# End Source File
# Begin Source File

SOURCE=.\Transport_Cache_Manager.inl
# End Source File
# Begin Source File

SOURCE=.\Transport_Connector.inl
# End Source File
# Begin Source File

SOURCE=.\Transport_Descriptor_Interface.inl
# End Source File
# Begin Source File

SOURCE=.\typecode.i
# End Source File
# Begin Source File

SOURCE=.\VarOut_T.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Any_Array_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Any_Basic_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Any_Dual_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Any_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Any_Special_Basic_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Any_Special_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Array_VarOut_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Codeset_Translator_Factory_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Objref_VarOut_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Pseudo_VarOut_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Seq_Out_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Seq_Var_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\VarOut_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project

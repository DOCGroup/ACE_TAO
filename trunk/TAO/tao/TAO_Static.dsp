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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D TAO_HAS_DLL=0 /D ACE_OS_HAS_DLL=0 /FD /c
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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "../../" /I "../" /D TAO_HAS_DLL=0 /D "_WINDOWS" /D "_DEBUG" /D "WIN32" /D ACE_HAS_DLL=0 /D ACE_OS_HAS_DLL=0 /FD /c
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
# Begin Group "Header Files"

# PROP Default_Filter "h"
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

SOURCE=.\Active_Object_Map.h
# End Source File
# Begin Source File

SOURCE=.\Adapter.h
# End Source File
# Begin Source File

SOURCE=.\any.h
# End Source File
# Begin Source File

SOURCE=.\arg_shifter.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.h
# End Source File
# Begin Source File

SOURCE=.\BoundsC.h
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.h
# End Source File
# Begin Source File

SOURCE=.\cdr.h
# End Source File
# Begin Source File

SOURCE=.\Client_Priority_Policy.h
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Connector_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Context.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES.h
# End Source File
# Begin Source File

SOURCE=.\corba.h
# End Source File
# Begin Source File

SOURCE=.\corbafwd.h
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

SOURCE=.\default_resource.h
# End Source File
# Begin Source File

SOURCE=.\default_server.h
# End Source File
# Begin Source File

SOURCE=.\DII_Invocation.h
# End Source File
# Begin Source File

SOURCE=.\DII_Reply_Dispatcher.h
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

SOURCE=.\DomainS.h
# End Source File
# Begin Source File

SOURCE=.\DynamicC.h
# End Source File
# Begin Source File

SOURCE=.\Endpoint.h
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

SOURCE=.\FILE_Parser.h
# End Source File
# Begin Source File

SOURCE=.\Forwarding_Servant.h
# End Source File
# Begin Source File

SOURCE=.\giop.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Headers.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\GIOPC.h
# End Source File
# Begin Source File

SOURCE=.\ifrfwd.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connect.h
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

SOURCE=.\IIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceC.h
# End Source File
# Begin Source File

SOURCE=.\Invocation.h
# End Source File
# Begin Source File

SOURCE=.\IOPC.h
# End Source File
# Begin Source File

SOURCE=.\IOPS.h
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.h
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\LocalObject.h
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.h
# End Source File
# Begin Source File

SOURCE=.\marshal.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\MessagingC.h
# End Source File
# Begin Source File

SOURCE=.\MessagingS.h
# End Source File
# Begin Source File

SOURCE=.\MProfile.h
# End Source File
# Begin Source File

SOURCE=.\Muxed_TMS.h
# End Source File
# Begin Source File

SOURCE=.\nvlist.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.h
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

SOURCE=.\ObjectIDList.h
# End Source File
# Begin Source File

SOURCE=.\Operation_Table.h
# End Source File
# Begin Source File

SOURCE=.\orb.h
# End Source File
# Begin Source File

SOURCE=.\orb_core.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\orbconf.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable.h
# End Source File
# Begin Source File

SOURCE=.\poa.h
# End Source File
# Begin Source File

SOURCE=.\POA_CORBA.h
# End Source File
# Begin Source File

SOURCE=.\poa_macros.h
# End Source File
# Begin Source File

SOURCE=.\POAManager.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.h
# End Source File
# Begin Source File

SOURCE=.\PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\PollableC.h
# End Source File
# Begin Source File

SOURCE=.\PollableS.h
# End Source File
# Begin Source File

SOURCE=.\Pool_Per_Endpoint.h
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

SOURCE=.\Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\Protocol_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Reactor_Per_Priority.h
# End Source File
# Begin Source File

SOURCE=.\Reactor_Registry.h
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

SOURCE=.\request.h
# End Source File
# Begin Source File

SOURCE=.\Request_Info.h
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Current.h
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.h
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS_T.h
# End Source File
# Begin Source File

SOURCE=.\sequence.h
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.h
# End Source File
# Begin Source File

SOURCE=.\servant_base.h
# End Source File
# Begin Source File

SOURCE=.\Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.h
# End Source File
# Begin Source File

SOURCE=.\Services.h
# End Source File
# Begin Source File

SOURCE=.\Services_Activate.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connect.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\Single_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\singletons.h
# End Source File
# Begin Source File

SOURCE=.\Smart_Proxies.h
# End Source File
# Begin Source File

SOURCE=.\stub.h
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

SOURCE=.\TAO.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Export.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.h
# End Source File
# Begin Source File

SOURCE=.\TAOC.h
# End Source File
# Begin Source File

SOURCE=.\TAOS.h
# End Source File
# Begin Source File

SOURCE=.\target_specification.h
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Mux_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\typecode.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connect.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\ValueBase.h
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.h
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.h
# End Source File
# Begin Source File

SOURCE=.\varbase.h
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

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\Acceptor_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.i
# End Source File
# Begin Source File

SOURCE=.\Active_Object_Map.i
# End Source File
# Begin Source File

SOURCE=.\Adapter.i
# End Source File
# Begin Source File

SOURCE=.\any.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.i
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

SOURCE=.\cdr.i
# End Source File
# Begin Source File

SOURCE=.\Client_Priority_Policy.i
# End Source File
# Begin Source File

SOURCE=.\Context.i
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.i
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES.i
# End Source File
# Begin Source File

SOURCE=.\corbafwd.i
# End Source File
# Begin Source File

SOURCE=.\default_client.i
# End Source File
# Begin Source File

SOURCE=.\default_resource.i
# End Source File
# Begin Source File

SOURCE=.\default_server.i
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.i
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

SOURCE=.\DomainS_T.i
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

SOURCE=.\FILE_Parser.i
# End Source File
# Begin Source File

SOURCE=.\giop.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Headers.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\GIOPC.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connect.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_ORB.i
# End Source File
# Begin Source File

SOURCE=.\InterfaceC.i
# End Source File
# Begin Source File

SOURCE=.\Invocation.i
# End Source File
# Begin Source File

SOURCE=.\IOPC.i
# End Source File
# Begin Source File

SOURCE=.\IOPS.i
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.i
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

SOURCE=.\marshal.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\MessagingC.i
# End Source File
# Begin Source File

SOURCE=.\MessagingS.i
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

SOURCE=.\ObjectIDList.i
# End Source File
# Begin Source File

SOURCE=.\ORB.i
# End Source File
# Begin Source File

SOURCE=.\orb_core.i
# End Source File
# Begin Source File

SOURCE=.\ORB_Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\params.i
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.i
# End Source File
# Begin Source File

SOURCE=.\POAManager.i
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.i
# End Source File
# Begin Source File

SOURCE=.\PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\Pool_Per_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.i
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Profile.i
# End Source File
# Begin Source File

SOURCE=.\Reactor_Per_Priority.i
# End Source File
# Begin Source File

SOURCE=.\Reactor_Registry.i
# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.i
# End Source File
# Begin Source File

SOURCE=.\Request.i
# End Source File
# Begin Source File

SOURCE=.\RT_Current.i
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.i
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.i
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS_T.i
# End Source File
# Begin Source File

SOURCE=.\sequence.i
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.i
# End Source File
# Begin Source File

SOURCE=.\Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.i
# End Source File
# Begin Source File

SOURCE=.\Services.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connect.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\Single_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Smart_Proxies.i
# End Source File
# Begin Source File

SOURCE=.\stub.i
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

SOURCE=.\TAO_Singleton.inl
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.inl
# End Source File
# Begin Source File

SOURCE=.\TAOC.i
# End Source File
# Begin Source File

SOURCE=.\TAOS.i
# End Source File
# Begin Source File

SOURCE=.\target_specification.i
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.i
# End Source File
# Begin Source File

SOURCE=.\typecode.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connect.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\ValueBase.i
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.i
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.i
# End Source File
# End Group
# Begin Group "Resource files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\tao.rc
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TAO_Singleton.cpp
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
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

SOURCE=.\Asynch_Reply_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.cpp
# End Source File
# Begin Source File

SOURCE=.\BoundsC.cpp
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR.cpp
# End Source File
# Begin Source File

SOURCE=.\Client_Priority_Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Connector_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Context.cpp
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.cpp
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES.cpp
# End Source File
# Begin Source File

SOURCE=.\corbafwd.cpp
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

SOURCE=.\default_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\default_server.cpp
# End Source File
# Begin Source File

SOURCE=.\DII_Invocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DII_Reply_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.cpp
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

SOURCE=.\DynamicC.cpp
# End Source File
# Begin Source File

SOURCE=.\Encodable.cpp
# End Source File
# Begin Source File

SOURCE=.\Endpoint.cpp
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

SOURCE=.\FILE_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Accept_State.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Acceptors.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Connectors.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Headers.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOPC.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connect.cpp
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

SOURCE=.\InterfaceC.cpp
# End Source File
# Begin Source File

SOURCE=.\Invocation.cpp
# End Source File
# Begin Source File

SOURCE=.\IOPC.cpp
# End Source File
# Begin Source File

SOURCE=.\IOPS.cpp
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.cpp
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Marshal.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagingC.cpp
# End Source File
# Begin Source File

SOURCE=.\MProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Muxed_TMS.cpp
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

SOURCE=.\ObjectIDList.cpp
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

SOURCE=.\params.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Pluggable.cpp
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.cpp
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyC.cpp
# End Source File
# Begin Source File

SOURCE=.\PollableC.cpp
# End Source File
# Begin Source File

SOURCE=.\PollableS.cpp
# End Source File
# Begin Source File

SOURCE=.\Pool_Per_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Principal.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocol_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactor_Per_Priority.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactor_Registry.cpp
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

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Request_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Current.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS_T.cpp
# End Source File
# Begin Source File

SOURCE=.\Sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.cpp
# End Source File
# Begin Source File

SOURCE=.\Services.cpp
# End Source File
# Begin Source File

SOURCE=.\Services_Activate.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connect.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\Single_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\skip.cpp
# End Source File
# Begin Source File

SOURCE=.\Smart_Proxies.cpp
# End Source File
# Begin Source File

SOURCE=.\Stub.cpp
# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.cpp
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

SOURCE=.\TAO_Singleton_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\TAOC.cpp
# End Source File
# Begin Source File

SOURCE=.\TAOS.cpp
# End Source File
# Begin Source File

SOURCE=.\target_specification.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport_Mux_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Typecode.cpp
# End Source File
# Begin Source File

SOURCE=.\Typecode_Constants.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connect.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Lite_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.cpp
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
# End Target
# End Project

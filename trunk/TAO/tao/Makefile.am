##----------------------------------------------------------------------------
##       $Id$
##
##       Makefile.am for TAO
##----------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

POA_FILES = \
  Object_Adapter.cpp \
  Key_Adapters.cpp \
  POA.cpp \
  PortableServerC.cpp \
  POAManager.cpp \
  Active_Object_Map.cpp \
  Servant_Base.cpp \
  Forwarding_Servant.cpp

PLUGGABLE_PROTOCOLS_FILES = \
  Pluggable.cpp \
  Transport.cpp \
  Profile.cpp \
  Connector_Registry.cpp \
  Acceptor_Registry.cpp \
  Protocol_Factory.cpp \
  IIOP_Factory.cpp \
  IIOP_Profile.cpp \
  IIOP_Transport.cpp \
  IIOP_Connector.cpp \
  IIOP_Acceptor.cpp \
  IIOP_Connect.cpp \
  UIOP_Factory.cpp \
  UIOP_Profile.cpp \
  UIOP_Transport.cpp \
  UIOP_Connector.cpp \
  UIOP_Acceptor.cpp \
  UIOP_Connect.cpp \
  IORC.cpp \
  IORManipulation.cpp

PLUGGABLE_MESSAGING_FILES = \
  Pluggable_Messaging \
  Pluggable_Messaging_Utils \
  GIOP_Message_Base \
  GIOP_Message_Connectors \
  GIOP_Message_Acceptors \
  GIOP_Message_Accept_State \
  GIOP_Utils \
  GIOP_Message_Headers \
  target_specification \
  TAO_Server_Request \
  GIOP_Message_Lite \
  GIOP_Message_State \
  Tagged_Profile

DEFAULT_RESOURCES_FILES = \
  default_client.cpp \
  default_server.cpp \
  default_resource.cpp \
  xt_resource.cpp \
  qt_resource.cpp

INTERPRETIVE_MARSHALING_FILES = \
  append.cpp \
  decode.cpp \
  deep_free.cpp \
  encode.cpp \
  Marshal.cpp \
  skip.cpp

IDL_COMPILER_FILES = \
  Managed_Types.cpp \
  Operation_Table.cpp

ORB_CORE_FILES = \
  Any.cpp \
  Bind_Dispatcher_Guard.cpp \
  CORBA_String.cpp \
  CORBALOC_Parser.cpp \
  CORBANAME_Parser.cpp \
  CurrentC.cpp \
  Exception.cpp \
  WrongTransactionC.cpp \
  Environment.cpp \
  DLL_Parser.cpp \
  FILE_Parser.cpp \
  Object.cpp \
  ObjectIDList.cpp \
  ORB.cpp \
  corbafwd.cpp \
  PolicyC.cpp \
  DomainC.cpp \
  TimeBaseC.cpp \
  Principal.cpp \
  Sequence.cpp \
  MProfile.cpp \
  Stub.cpp \
  Typecode.cpp \
  ValueBase.cpp \
  ValueFactory.cpp \
  ValueFactory_Map.cpp \
  CDR.cpp \
  Client_Strategy_Factory.cpp \
  debug.cpp \
  DynamicInterface_Adapter.cpp \
  Object_KeyC.cpp \
  IFR_Client_Adapter.cpp \
  IFR_TypeCodes.cpp \
  Invocation.cpp \
  Interceptor.cpp \
  InterceptorC.cpp \
  Asynch_Invocation.cpp \
  operation_details.cpp \
  PortableInterceptor.cpp \
  Request_Info.cpp \
  PortableInterceptorC.cpp \
  DynamicC.cpp \
  OctetSeqC.cpp \
  StringSeqC.cpp \
  ORB_Core.cpp \
  ORB_Table.cpp \
  LF_Event.cpp \
  LF_Event_Binder.cpp \
  LF_Event_Loop_Thread_Helper.cpp \
  LF_Strategy.cpp \
  LF_Strategy_Complete.cpp \
  Leader_Follower.cpp \
  Leader_Follower_Flushing_Strategy.cpp \
  LF_Follower.cpp \
  LF_Follower_Auto_Ptr.cpp \
  LF_Follower_Auto_Adder.cpp \
  params.cpp \
  Resource_Factory.cpp \
  Server_Strategy_Factory.cpp \
  Sync_Strategies.cpp \
  TAO_Internal.cpp \
  Typecode_Constants.cpp \
  TypeCodeFactory_Adapter.cpp \
  IOR_LookupTable.cpp \
  TAO.cpp \
  Wait_Strategy.cpp \
  Wait_On_Read.cpp \
  Wait_On_Reactor.cpp \
  Wait_On_Leader_Follower.cpp \
  Transport_Mux_Strategy.cpp \
  Muxed_TMS.cpp \
  Exclusive_TMS.cpp \
  Reply_Dispatcher.cpp \
  Synch_Reply_Dispatcher.cpp \
  Asynch_Reply_Dispatcher.cpp \
  Asynch_Timeout_Handler.cpp \
  Dynamic_Adapter.cpp \
  IOPC.cpp \
  PollableC.cpp \
  CONV_FRAMEC.cpp \
  Tagged_Components.cpp \
  Services.cpp \
  NVList.cpp \
  MessagingC.cpp \
  Policy_Manager.cpp \
  Messaging_Policy_i.cpp \
  TAOC.cpp \
  Buffering_Constraint_Policy.cpp \
  GIOPC.cpp \
  TAO_Singleton_Manager.cpp \
  DLL_ORB.cpp \
  Endpoint_Selector_Factory.cpp \
  Thread_Lane_Resources.cpp \
  Thread_Lane_Resources_Manager.cpp \
  Default_Thread_Lane_Resources_Manager.cpp


# Build a libtool library, libTAO.la for installation in libdir.
lib_LTLIBRARIES = libTAO.la
libTAO_la_SOURCES = \
  $(POA_FILES) \
  $(PLUGGABLE_PROTOCOLS_FILES) \
  $(DEFAULT_RESOURCES_FILES) \
  $(INTERPRETIVE_MARSHALING_FILES) \
  $(IDL_COMPILER_FILES) \
  $(ORB_CORE_FILES)

## The following is an excerpt from the "libtool" manual, written by
## Gordon Matzigkeit:
##
## This flag accepts an argument of the form `current[:revision[:age]]'. So,
## passing `-version-info 3:12:1' sets current to 3, revision to 12, and age
## to 1.
## If either revision or age are omitted, they default to 0. Also note that
## age must be less than or equal to the current interface number.
##
## Here are a set of rules to help you update your library version
## information:
##
##     1.Start with version information of `0:0:0' for each libtool library.
##     2.Update the version information only immediately before a public
##       release of your software. More frequent updates are unnecessary, and
##       only guarantee that the current interface number gets larger faster.
##     3.If the library source code has changed at all since the last update,
##       then increment revision (`c:r:a' becomes `c:r+1:a').
##     4.If any interfaces have been added, removed, or changed since the last
##       update, increment current, and set revision to 0.
##     5.If any interfaces have been added since the last public release, then
##       increment age.
##     6.If any interfaces have been removed since the last public release,
##       then set age to 0.
##
## Never try to set the interface numbers so that they correspond to the
## release number of your package. This is an abuse that only fosters
## misunderstanding of the purpose of library versions. Instead, use the
## `-release' flag (see section 6.4 Managing release information), but be
## warned that every release of your package will not be binary compatibility
## with any other release.

## The below "-version-info" isn't being used the way libtool intends
## it to be used but we do it that way to make the version numbers
## that TAO uses match those created by libtool from "-version-info."
##libTAO_la_LDFLAGS = -version-info 2:41:2   ## 2-2=0 --> libTAO.so.0.2.41
libTAO_la_LDFLAGS = -version-info @TAO_CURRENT@:@TAO_REVISION@:@TAO_AGE@

## These are source files that are needed by the header files if the
## ACE_TEMPLATES_REQUIRE_SOURCE or ACE_TEMPLATES_REQUIRE_PRAGMA
## macro is defined.
if TEMPLATES_REQUIRE_SOURCE
TEMPLATE_FILES = \
  Acceptor_Impl.cpp \
  CONV_FRAMES_T.cpp \
  DomainS_T.cpp \
  DynUnion_i_T.cpp \
  IOPS_T.cpp \
  MessagingS_T.cpp \
  PollableS_T.cpp \
  Sequence_T.cpp \
  TAOS_T.cpp \
  TimeBaseS_T.cpp \
  TAO_Singleton.cpp
else
TEMPLATE_FILES =
endif

IDL_FILES = \
  orb.idl \
  Policy.pidl

HEADER_FILES = \
  Acceptor_Impl.h \
  Acceptor_Registry.h \
  Active_Object_Map.h \
  Any.h \
  Asynch_Invocation.h \
  BoundsC.h \
  Buffering_Constraint_Policy.h \
  CDR.h \
  CONV_FRAMEC.h \
  CONV_FRAMES.h \
  CONV_FRAMES_T.h \
  CORBA_String.h \
  Client_Strategy_Factory.h \
  Connector_Registry.h \
  CurrentC.h \
  DLL_ORB.h
  DomainC.h \
  DomainS_T.h \
  DynamicInterface_Adapter.h \
  Environment.h \
  Exception.h \
  Forwarding_Servant.h \
  GIOP.h \
  GIOPC.h \
  GIOP_Server_Request.h \
  IFR_Client_Adapter.h \
  IIOP_Acceptor.h \
  IIOP_Connect.h \
  IIOP_Connector.h \
  IIOP_Factory.h \
  IIOP_Profile.h \
  IIOP_Transport.h \
  IOPC.h \
  IOPS.h \
  IOPS_T.h \
  IORC.h \
  IORManipulation.h \
  IORS.h \
  IOR_LookupTable.h \
  InconsistentTypeCodeC.h \
  Interceptor.h \
  InterceptorC.h \
  InterceptorS.h \
  Invocation.h \
  Key_Adapters.h \
  Follower.h \
  Follower_Auto_Ptr.h \
  LF_Follower_Auto_Adder.h \
  Leader_Follower.h \
  Leader_Follower_Flushing_Strategy.h \
  LF_Event.h \
  LF_Event_Binder.h \
  LF_Event_Loop_Thread_Helper.h \
  LF_Strategy.h \
  LF_Strategy_Complete.h \
  MProfile.h \
  Managed_Types.h \
  Marshal.h \
  MessagingC.h \
  MessagingS.h \
  MessagingS_T.h \
  Messaging_Policy_i.h \
  NVList.h \
  ORB.h \
  operation_details.h \
  PortableInterceptor.h \
  Request_Info.h \
  PortableInterceptorC.h \
  DynamicC.h \
  OctetSeqC.h \
  StringSeqC.h \
  ORB_Core.h \
  ORB_Table.h \
  Object.h \
  ObjectIDList.h \
  Object_Adapter.h \
  Object_KeyC.h \
  Operation_Table.h \
  POA.h \
  PortableServerC.h \
  POAManager.h \
  POA_CORBA.h \
  Pluggable.h \
  Transport.h \
  PolicyC.h \
  Policy_Manager.h \
  PollableC.h \
  PollableS.h \
  PollableS_T.h \
  Principal.h \
  Profile.h \
  Protocol_Factory.h \
  Resource_Factory.h \
  Sequence.h \
  Sequence_T.h \
  Servant_Base.h \
  Server_Strategy_Factory.h \
  Services.h \
  Stub.h \
  Sync_Strategies.h \
  TAO.h \
  TAOC.h \
  TAOS.h \
  TAOS_T.h \
  TAO_Internal.h \
  TAO_Singleton.h \
  Tagged_Components.h \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  Timeprobe.h \
  Typecode.h \
  TypeCodeFactory_Adapter.h \
  UIOP_Acceptor.h \
  UIOP_Connect.h \
  UIOP_Connector.h \
  UIOP_Factory.h \
  UIOP_Profile.h \
  UIOP_Transport.h \
  ValueBase.h \
  ValueFactory.h \
  ValueFactory_Map.h \
  Version.h \
  Wait_Strategy.h \
  Wait_On_Read.h \
  Wait_On_Reactor.h \
  Wait_On_Leader_Follower.h \
  Transport_Mux_Strategy.h \
  Muxed_TMS.h \
  Exclusive_TMS.h \
  Reply_Dispatcher.h \
  Synch_Reply_Dispatcher.h \
  Asynch_Reply_Dispatcher.h \
  Asynch_Timeout_Handler.h \
  WrongTransactionC.h \
  Dynamic_Adapter.h \
  corba.h \
  corbafwd.h \
  debug.h \
  default_client.h \
  default_resource.h \
  default_server.h \
  orbconf.h \
  params.h \
  poa_macros.h \
  qt_resource.h \
  try_macros.h \
  xt_resource.h \
  Endpoint_Selector_Factory.h

INLINE_FILES = \
  Acceptor_Impl.i \
  Acceptor_Registry.i \
  Active_Object_Map.i \
  Any.i \
  Asynch_Invocation.i \
  BoundsC.i \
  Buffering_Constraint_Policy.i \
  CDR.i \
  CONV_FRAMEC.i \
  CONV_FRAMES.i \
  CONV_FRAMES_T.i \
  CORBA_String.inl \
  Client_Priority_Policy.i \
  Connector_Registry.i \
  CurrentC.i \
  DLL_ORB.inl \
  DomainC.i \
  DomainS_T.i \
  Environment.i \
  Exception.i \
  GIOP.i \
  GIOPC.i \
  GIOP_Server_Request.i \
  IIOP_Acceptor.i \
  IIOP_Connect.i \
  IIOP_Profile.i \
  IOPC.i \
  IOPS.i \
  IOPS_T.i \
  IORC.i \
  IORS.i \
  Interceptor.i \
  InterceptorC.i \
  Invocation.i \
  Key_Adapters.i \
  Follower.inl \
  Follower_Auto_Ptr.inl \
  LF_Follower_Auto_Adder.inl \
  Leader_Follower.i \
  Leader_Follower_Flushing_Strategy.inl \
  LF_Event.inl \
  LF_Event_Binder.inl \
  LF_Event_Loop_Thread_Helper.inl \
  LF_Strategy.inl \
  LF_Strategy_Complete.inl \
  MProfile.i \
  Managed_Types.i \
  Marshal.i \
  MessagingC.i \
  MessagingS.i \
  MessagingS_T.i \
  Messaging_Policy_i.i \
  NVList.i \
  ORB.i \
  operation_details.i \
  PortableInterceptor.i \
  PortableInterceptorC.i \
  DynamicC.i \
  OctetSeqC.i \
  StringSeqC.i \
  Smart_Proxies.i \
  ORB_Core.i \
  ORB_Table.inl \
  Object.i \
  ObjectIDList.i \
  Object_Adapter.i \
  Object_KeyC.i \
  POA.i \
  PortableServerC.i \
  POAManager.i \
  Pluggable.i \
  Transport.inl \
  PolicyC.i \
  Policy_Manager.i \
  PollableC.i \
  PollableS.i \
  PollableS_T.i \
  Principal.i \
  Priority_Mapping.i \
  Profile.i \
  RT_Current.i \
  Reply_Dispatcher.i \
  Synch_Reply_Dispatcher.i \
  Asynch_Reply_Dispatcher.i \
  Sequence.i \
  Sequence_T.i \
  Servant_Base.i \
  Services.i \
  Stub.i \
  Sync_Strategies.i \
  TAO_Singleton.inl \
  TAOC.i \
  TAOS.i \
  TAOS_T.i \
  Tagged_Components.i \
  TimeBaseC.i \
  TimeBaseS.i \
  TimeBaseS_T.i \
  Typecode.i \
  UIOP_Connect.i \
  UIOP_Profile.i \
  ValueBase.i \
  ValueFactory.i \
  ValueFactory_Map.i \
  corbafwd.i \
  default_client.i \
  default_resource.i \
  default_server.i \
  params.i \
  qt_resource.i \
  xt_resource.i


pkgincludedir = $(prefix)/include/tao
pkginclude_HEADERS = \
  $(HEADER_FILES) \
  $(INLINE_FILES) \
  $(TEMPLATE_FILES) \
  $(IDL_FILES)


## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

##  Process this file with automake to create Makefile.in

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

## Makefile.TAO.am 

ORB_Core = \
  ServicesC.cpp \
  NVList.cpp \
  Dynamic_Adapter.cpp \
  Policy_Manager.cpp \
  Buffering_Constraint_Policy.cpp \
  Messaging_PolicyValueC.cpp \
  Any.cpp \
  Any_Basic_Impl.cpp \
  Any_Impl.cpp \
  Any_Unknown_IDL_Type.cpp \
  Any_SystemException.cpp \
  CORBA_String.cpp \
  CurrentC.cpp \
  CurrentA.cpp \
  Exception.cpp \
  WrongTransactionC.cpp \
  WrongTransactionA.cpp \
  Environment.cpp \
  Object.cpp \
  LocalObject.cpp \
  ObjectIdListC.cpp \
  ORB.cpp \
  Policy_ForwardC.cpp \
  Policy_ForwardA.cpp \
  PolicyC.cpp \
  PolicyA.cpp \
  Policy_Validator.cpp \
  Encodable.cpp \
  Abstract_Servant_Base.cpp \
  Object_Proxy_Broker.cpp \
  Remote_Object_Proxy_Broker.cpp \
  Object_Proxy_Impl.cpp \
  DomainC.cpp \
  DomainA.cpp \
  TimeBaseC.cpp \
  TimeBaseA.cpp \
  Principal.cpp \
  Sequence.cpp \
  MProfile.cpp \
  Stub.cpp \
  Typecode.cpp \
  CDR.cpp \
  Client_Strategy_Factory.cpp \
  debug.cpp \
  Object_KeyC.cpp \
  Protocols_Hooks.cpp \
  Default_Protocols_Hooks.cpp \
  Messaging_SyncScopeC.cpp \
  Invocation_Base.cpp \
  Invocation_Adapter.cpp \
  LocateRequest_Invocation_Adapter.cpp \
  Profile_Transport_Resolver.cpp \
  Remote_Invocation.cpp \
  Collocated_Invocation.cpp \
  Synch_Invocation.cpp \
  LocateRequest_Invocation.cpp \
  Invocation_Endpoint_Selectors.cpp \
  operation_details.cpp \
  ClientRequestInfo.cpp \
  ClientRequestInfo_i.cpp \
  RequestInfo_Util.cpp \
  ClientRequestInterceptor_Adapter.cpp \
  PI_ForwardC.cpp \
  PortableInterceptorC.cpp \
  PortableInterceptorA.cpp \
  PICurrent.cpp \
  PICurrent_Copy_Callback.cpp \
  PICurrent_ORBInitializer.cpp \
  DynamicC.cpp \
  DynamicA.cpp \
  Dynamic_ParameterC.cpp \
  Dynamic_ParameterA.cpp \
  ParameterMode.cpp \
  AnySeqA.cpp \
  AnySeqC.cpp \
  BooleanSeqA.cpp \
  BooleanSeqC.cpp \
  CharSeqA.cpp \
  CharSeqC.cpp \
  DoubleSeqA.cpp \
  DoubleSeqC.cpp \
  FloatSeqA.cpp \
  FloatSeqC.cpp \
  LongDoubleSeqA.cpp \
  LongDoubleSeqC.cpp \
  LongLongSeqA.cpp \
  LongLongSeqC.cpp \
  LongSeqA.cpp \
  LongSeqC.cpp \
  OctetSeqA.cpp \
  OctetSeqC.cpp \
  ShortSeqA.cpp \
  ShortSeqC.cpp \
  StringSeqA.cpp \
  StringSeqC.cpp \
  ULongLongSeqA.cpp \
  ULongLongSeqC.cpp \
  ULongSeqA.cpp \
  ULongSeqC.cpp \
  UShortSeqA.cpp \
  UShortSeqC.cpp \
  WCharSeqA.cpp \
  WCharSeqC.cpp \
  WStringSeqA.cpp \
  WStringSeqC.cpp \
  Codeset_Manager.cpp \
  Codeset_Translator_Factory.cpp \
  UTF16_BOM_Translator.cpp \
  UTF16_BOM_Factory.cpp \
  ORB_Core.cpp \
  ORB_Core_Auto_Ptr.cpp \
  Collocation_Resolver.cpp \
  Default_Collocation_Resolver.cpp \
  Stub_Factory.cpp \
  ORB_Table.cpp \
  LF_Follower.cpp \
  LF_Follower_Auto_Ptr.cpp \
  LF_Follower_Auto_Adder.cpp \
  Leader_Follower.cpp \
  Leader_Follower_Flushing_Strategy.cpp \
  LF_Event.cpp \
  LF_Invocation_Event.cpp \
  LF_CH_Event.cpp \
  LF_Event_Binder.cpp \
  LF_Event_Loop_Thread_Helper.cpp \
  LF_Strategy.cpp \
  LF_Strategy_Complete.cpp \
  params.cpp \
  Resource_Factory.cpp \
  Server_Strategy_Factory.cpp \
  Sync_Strategies.cpp \
  TAO_Internal.cpp \
  TAO_Server_Request.cpp \
  Typecode_Constants.cpp \
  PredefinedType_Seq_Tmplinst.cpp \
  TypeCodeFactory_Adapter.cpp \
  Wait_Strategy.cpp \
  Wait_On_Read.cpp \
  Wait_On_Reactor.cpp \
  Wait_On_Leader_Follower.cpp \
  Transport_Mux_Strategy.cpp \
  Exclusive_TMS.cpp \
  Muxed_TMS.cpp \
  Reply_Dispatcher.cpp \
  Synch_Reply_Dispatcher.cpp \
  Synch_Refcountable.cpp \
  Asynch_Reply_Dispatcher_Base.cpp \
  IOP_IORC.cpp \
  IOP_IORA.cpp \
  IOP_CodecC.cpp \
  IOP_CodecA.cpp \
  IOPC.cpp \
  PollableC.cpp \
  CONV_FRAMEC.cpp \
  CONV_FRAMEA.cpp \
  Tagged_Components.cpp \
  Service_Context.cpp \
  GIOPC.cpp \
  GIOPA.cpp \
  BoundsC.cpp \
  BoundsA.cpp \
  TAOC.cpp \
  TAOA.cpp \
  Object_Loader.cpp \
  TAO_Singleton_Manager.cpp \
  DLL_ORB.cpp \
  Adapter.cpp \
  Services_Activate.cpp \
  Service_Callbacks.cpp \
  Parser_Registry.cpp \
  IOR_Parser.cpp \
  DLL_Parser.cpp \
  FILE_Parser.cpp \
  CORBALOC_Parser.cpp \
  CORBANAME_Parser.cpp \
  MCAST_Parser.cpp \
  Bind_Dispatcher_Guard.cpp \
  Fault_Tolerance_Service.cpp \
  Interceptor_List.cpp \
  IORInterceptor_Adapter.cpp \
  IORInterceptor_Adapter_Factory.cpp \
  IFR_Client_Adapter.cpp \
  ORBInitInfo.cpp \
  ORBInitializer_Registry.cpp \
  PolicyFactory_Registry.cpp \
  Cache_Entries.cpp \
  Base_Transport_Property.cpp \
  Transport_Cache_Manager.cpp \
  Cleanup_Func_Registry.cpp \
  Transport_Descriptor_Interface.cpp \
  Object_Ref_Table.cpp \
  BiDir_Adapter.cpp \
  CodecFactory.cpp \
  CodecFactory_ORBInitializer.cpp \
  CDR_Encaps_Codec.cpp \
  Endpoint_Selector_Factory.cpp \
  Flushing_Strategy.cpp \
  Block_Flushing_Strategy.cpp \
  Reactive_Flushing_Strategy.cpp \
  Connect_Strategy.cpp \
  Blocked_Connect_Strategy.cpp \
  Reactive_Connect_Strategy.cpp \
  LF_Connect_Strategy.cpp \
  Queued_Message.cpp \
  Synch_Queued_Message.cpp \
  Asynch_Queued_Message.cpp \
  Transport_Timer.cpp \
  Connection_Purging_Strategy.cpp \
  LRU_Connection_Purging_Strategy.cpp \
  Policy_Set.cpp \
  Default_Endpoint_Selector_Factory.cpp \
  Thread_Lane_Resources.cpp \
  Thread_Lane_Resources_Manager.cpp \
  Default_Thread_Lane_Resources_Manager.cpp \
  Default_Stub_Factory.cpp \
  Request_Dispatcher.cpp \
  Valuetype_Adapter.cpp \
  ObjectKey_Table.cpp \
  Refcounted_ObjectKey.cpp \
  TSS_Resources.cpp \
  Argument.cpp \
  Basic_Arguments.cpp \
  Special_Basic_Arguments.cpp

Pluggable_Protocols = \
  Transport_Acceptor.cpp \
  Transport_Connector.cpp \
  Transport.cpp \
  Incoming_Message_Queue.cpp \
  Resume_Handle.cpp \
  Profile.cpp \
  Endpoint.cpp \
  Connector_Registry.cpp \
  Connection_Handler.cpp \
  Thread_Per_Connection_Handler.cpp \
  Acceptor_Registry.cpp \
  Protocol_Factory.cpp \
  Acceptor_Filter.cpp \
  IIOP_Factory.cpp \
  IIOP_Lite_Factory.cpp \
  IIOP_Profile.cpp \
  IIOP_Transport.cpp \
  IIOP_Connector.cpp \
  IIOP_Acceptor.cpp \
  IIOP_Connection_Handler.cpp \
  IIOP_Endpoint.cpp \
  IIOP_EndpointsC.cpp \
  IIOP_EndpointsA.cpp \
  IIOPC.cpp \
  IIOPA.cpp

Pluggable_Messaging = \
  Pluggable_Messaging.cpp \
  Pluggable_Messaging_Utils.cpp \
  GIOP_Message_Base.cpp \
  GIOP_Message_Lite.cpp \
  GIOP_Message_Generator_Parser.cpp \
  GIOP_Message_Generator_Parser_10.cpp \
  GIOP_Message_Generator_Parser_11.cpp \
  GIOP_Message_Generator_Parser_12.cpp \
  GIOP_Message_Generator_Parser_Impl.cpp \
  GIOP_Utils.cpp \
  GIOP_Message_Locate_Header.cpp \
  target_specification.cpp \
  GIOP_Message_State.cpp \
  GIOP_Message_Version.cpp \
  Tagged_Profile.cpp

Default_Resources = \
  default_client.cpp \
  default_server.cpp \
  default_resource.cpp \
  xt_resource.cpp \
  qt_resource.cpp

Interpretive_Marshaling = \
  append.cpp \
  Marshal.cpp \
  skip.cpp

IDL_Compiler = \
  Managed_Types.cpp

lib_LTLIBRARIES = libTAO.la

libTAO_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_BUILD_DLL

libTAO_la_SOURCES = \
  $(ORB_Core) \
  $(Pluggable_Protocols) \
  $(Pluggable_Messaging) \
  $(Default_Resources) \
  $(Interpretive_Marshaling) \
  $(IDL_Compiler)

libTAO_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_la_LIBADD = \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS = \
  Abstract_Servant_Base.h \
  Acceptor_Filter.h \
  Acceptor_Impl.h \
  Acceptor_Registry.h \
  Adapter.h \
  Allocator.h \
  Any.h \
  Any_Array_Impl_T.h \
  Any_Basic_Impl.h \
  Any_Basic_Impl_T.h \
  Any_Dual_Impl_T.h \
  Any_Impl.h \
  Any_Impl_T.h \
  Any_Special_Impl_T.h \
  Any_SystemException.h \
  Any_Unknown_IDL_Type.h \
  AnySeqC.h \
  AnySeqS.h \
  Arg_Traits_T.h \
  Argument.h \
  Array_VarOut_T.h \
  Asynch_Queued_Message.h \
  Asynch_Reply_Dispatcher_Base.h \
  Auto_Functor.h \
  Base_Transport_Property.h \
  Basic_Argument_T.h \
  Basic_Arguments.h \
  Basic_SArgument_T.h \
  Basic_SArguments.h \
  Basic_Types.h \
  BD_String_Argument_T.h \
  BD_String_SArgument_T.h \
  BiDir_Adapter.h \
  Bind_Dispatcher_Guard.h \
  Block_Flushing_Strategy.h \
  Blocked_Connect_Strategy.h \
  BooleanSeqC.h \
  BooleanSeqS.h \
  BoundsC.h \
  Buffer_Allocator_T.h \
  Buffering_Constraint_Policy.h \
  Cache_Entries.h \
  CDR.h \
  CDR_Encaps_Codec.h \
  CharSeqC.h \
  CharSeqS.h \
  Cleanup_Func_Registry.h \
  Client_Strategy_Factory.h \
  ClientRequestInfo.h \
  ClientRequestInfo_i.h \
  ClientRequestInterceptor_Adapter.h \
  CodecFactory.h \
  CodecFactory_ORBInitializer.h \
  Codeset_Manager.h \
  Codeset_Translator_Factory.h \
  Codeset_Translator_Factory_T.h \
  Collocated_Invocation.h \
  Collocation_Proxy_Broker.h \
  Collocation_Resolver.h \
  Collocation_Strategy.h \
  Condition.h \
  Connect_Strategy.h \
  Connection_Handler.h \
  Connection_Purging_Strategy.h \
  Connector_Impl.h \
  Connector_Registry.h \
  CONV_FRAMEC.h \
  CONV_FRAMES.h \
  CONV_FRAMES_T.h \
  corba.h \
  CORBA_methods.h \
  CORBA_String.h \
  corbafwd.h \
  CORBALOC_Parser.h \
  CORBANAME_Parser.h \
  CurrentC.h \
  debug.h \
  default_client.h \
  Default_Collocation_Resolver.h \
  Default_Endpoint_Selector_Factory.h \
  default_environment.h \
  default_ports.h \
  Default_Protocols_Hooks.h \
  default_resource.h \
  default_server.h \
  Default_Stub_Factory.h \
  Default_Thread_Lane_Resources_Manager.h \
  DLL_ORB.h \
  DLL_Parser.h \
  DomainC.h \
  DoubleSeqC.h \
  DoubleSeqS.h \
  Dynamic_Adapter.h \
  Dynamic_ParameterC.h \
  DynamicC.h \
  Encodable.h \
  Endpoint.h \
  Endpoint_Selector_Factory.h \
  Environment.h \
  Exception.h \
  Exception_Data.h \
  Exclusive_TMS.h \
  Fault_Tolerance_Service.h \
  FILE_Parser.h \
  Fixed_Array_Argument_T.h \
  Fixed_Array_SArgument_T.h \
  Fixed_Size_Argument_T.h \
  Fixed_Size_SArgument_T.h \
  FloatSeqC.h \
  FloatSeqS.h \
  Flushing_Strategy.h \
  GIOP_Message_Base.h \
  GIOP_Message_Generator_Parser.h \
  GIOP_Message_Generator_Parser_10.h \
  GIOP_Message_Generator_Parser_11.h \
  GIOP_Message_Generator_Parser_12.h \
  GIOP_Message_Generator_Parser_Impl.h \
  GIOP_Message_Lite.h \
  GIOP_Message_Locate_Header.h \
  GIOP_Message_State.h \
  GIOP_Message_Version.h \
  GIOP_Utils.h \
  GIOPC.h \
  GIOPS.h \
  GIOPS_T.h \
  IFR_Client_Adapter.h \
  IIOP_Acceptor.h \
  IIOP_Connection_Handler.h \
  IIOP_Connector.h \
  IIOP_Endpoint.h \
  IIOP_EndpointsC.h \
  IIOP_Factory.h \
  IIOP_Lite_Factory.h \
  IIOP_Profile.h \
  IIOP_Transport.h \
  IIOPC.h \
  Incoming_Message_Queue.h \
  Interceptor_List.h \
  Invocation_Adapter.h \
  Invocation_Base.h \
  Invocation_Endpoint_Selectors.h \
  Invocation_Utils.h \
  IOP_CodecC.h \
  IOP_IORC.h \
  IOPC.h \
  IOPS.h \
  IOPS_T.h \
  IOR_Parser.h \
  IORInterceptor_Adapter.h \
  IORInterceptor_Adapter_Factory.h \
  Leader_Follower.h \
  Leader_Follower_Flushing_Strategy.h \
  LF_CH_Event.h \
  LF_Connect_Strategy.h \
  LF_Event.h \
  LF_Event_Binder.h \
  LF_Event_Loop_Thread_Helper.h \
  LF_Follower.h \
  LF_Follower_Auto_Adder.h \
  LF_Follower_Auto_Ptr.h \
  LF_Invocation_Event.h \
  LF_Strategy.h \
  LF_Strategy_Complete.h \
  LocalObject.h \
  LocateRequest_Invocation.h \
  LocateRequest_Invocation_Adapter.h \
  LongDoubleSeqC.h \
  LongDoubleSeqS.h \
  LongLongSeqC.h \
  LongLongSeqS.h \
  LongSeqC.h \
  LongSeqS.h \
  LRU_Connection_Purging_Strategy.h \
  Managed_Types.h \
  Marshal.h \
  MCAST_Parser.h \
  Messaging_PolicyValueC.h \
  Messaging_SyncScopeC.h \
  MProfile.h \
  Muxed_TMS.h \
  New_Leader_Generator.h \
  NVList.h \
  Object.h \
  Object_Argument_T.h \
  Object_KeyC.h \
  Object_Loader.h \
  Object_Proxy_Broker.h \
  Object_Proxy_Impl.h \
  Object_Ref_Table.h \
  Object_SArgument_T.h \
  Object_T.h \
  objectid.h \
  ObjectIdListC.h \
  ObjectKey_Table.h \
  Objref_VarOut_T.h \
  OBV_Constants.h \
  OctetSeqC.h \
  OctetSeqS.h \
  operation_details.h \
  ORB.h \
  ORB_Constants.h \
  ORB_Core.h \
  ORB_Core_Auto_Ptr.h \
  ORB_Table.h \
  orbconf.h \
  ORBInitializer_Registry.h \
  ORBInitInfo.h \
  ParameterMode.h \
  params.h \
  Parser_Registry.h \
  PI_ForwardC.h \
  PICurrent.h \
  PICurrent_Copy_Callback.h \
  PICurrent_ORBInitializer.h \
  Pluggable_Messaging.h \
  Pluggable_Messaging_Utils.h \
  Policy_ForwardC.h \
  Policy_Manager.h \
  Policy_Set.h \
  Policy_Validator.h \
  PolicyC.h \
  PolicyFactory_Registry.h \
  PolicyS.h \
  PollableC.h \
  PollableS.h \
  PollableS_T.h \
  PortableInterceptor.h \
  PortableInterceptorC.h \
  PortableInterceptorS.h \
  Principal.h \
  Profile.h \
  Profile_Transport_Resolver.h \
  Protocol_Factory.h \
  Protocols_Hooks.h \
  Pseudo_VarOut_T.h \
  qt_resource.h \
  Queued_Message.h \
  Reactive_Connect_Strategy.h \
  Reactive_Flushing_Strategy.h \
  Refcounted_ObjectKey.h \
  Remote_Invocation.h \
  Remote_Object_Proxy_Broker.h \
  Reply_Dispatcher.h \
  Request_Dispatcher.h \
  RequestInfo_Util.h \
  Resource_Factory.h \
  Resume_Handle.h \
  Seq_Out_T.h \
  Seq_Var_T.h \
  Sequence.h \
  Sequence_T.h \
  Server_Strategy_Factory.h \
  Service_Callbacks.h \
  Service_Context.h \
  Services_Activate.h \
  ServicesC.h \
  ServicesS.h \
  ServicesS_T.h \
  ShortSeqC.h \
  ShortSeqS.h \
  Special_Basic_Argument_T.h \
  Special_Basic_Arguments.h \
  Special_Basic_SArgument_T.h \
  Special_Basic_SArguments.h \
  StringSeqC.h \
  StringSeqS.h \
  Stub.h \
  Stub_Factory.h \
  Sync_Strategies.h \
  Synch_Invocation.h \
  Synch_Queued_Message.h \
  Synch_Refcountable.h \
  Synch_Reply_Dispatcher.h \
  Tagged_Components.h \
  Tagged_Profile.h \
  TAO_Export.h \
  TAO_Internal.h \
  TAO_Server_Request.h \
  TAO_Singleton.h \
  TAO_Singleton_Manager.h \
  TAOC.h \
  TAOS.h \
  TAOS_T.h \
  target_specification.h \
  TC_Constants_Forward.h \
  Thread_Lane_Resources.h \
  Thread_Lane_Resources_Manager.h \
  Thread_Per_Connection_Handler.h \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  Timeprobe.h \
  Transport.h \
  Transport_Acceptor.h \
  Transport_Cache_Manager.h \
  Transport_Connector.h \
  Transport_Descriptor_Interface.h \
  Transport_Mux_Strategy.h \
  Transport_Timer.h \
  TSS_Resources.h \
  Typecode.h \
  Typecode_Constants.h \
  TypeCodeFactory_Adapter.h \
  UB_String_Argument_T.h \
  UB_String_Arguments.h \
  UB_String_SArgument_T.h \
  UB_String_SArguments.h \
  ULongLongSeqC.h \
  ULongLongSeqS.h \
  ULongSeqC.h \
  ULongSeqS.h \
  UShortSeqC.h \
  UShortSeqS.h \
  UTF16_BOM_Factory.h \
  UTF16_BOM_Translator.h \
  Valuetype_Adapter.h \
  Var_Array_Argument_T.h \
  Var_Array_SArgument_T.h \
  Var_Size_Argument_T.h \
  Var_Size_SArgument_T.h \
  varbase.h \
  VarOut_T.h \
  Version.h \
  Wait_On_Leader_Follower.h \
  Wait_On_Reactor.h \
  Wait_On_Read.h \
  Wait_Strategy.h \
  WCharSeqC.h \
  WCharSeqS.h \
  WrongTransactionC.h \
  WStringSeqC.h \
  WStringSeqS.h \
  xt_resource.h \
  Acceptor_Filter.i \
  Acceptor_Impl.i \
  Acceptor_Registry.i \
  Adapter.i \
  Any.i \
  Asynch_Reply_Dispatcher_Base.i \
  Bind_Dispatcher_Guard.i \
  BooleanSeqC.i \
  BoundsC.i \
  Buffering_Constraint_Policy.i \
  CDR.i \
  Collocation_Resolver.i \
  Connector_Registry.i \
  CONV_FRAMEC.i \
  CONV_FRAMES.i \
  CONV_FRAMES_T.i \
  corbafwd.i \
  CORBALOC_Parser.i \
  CORBANAME_Parser.i \
  CurrentC.i \
  default_client.i \
  Default_Collocation_Resolver.i \
  Default_Protocols_Hooks.i \
  default_resource.i \
  default_server.i \
  Default_Thread_Lane_Resources_Manager.i \
  DLL_Parser.i \
  DomainC.i \
  Dynamic_ParameterC.i \
  DynamicC.i \
  Endpoint.i \
  Environment.i \
  Exception.i \
  Fault_Tolerance_Service.i \
  FILE_Parser.i \
  FloatSeqC.i \
  GIOP_Message_Base.i \
  GIOP_Message_Lite.i \
  GIOP_Message_Locate_Header.i \
  GIOP_Utils.i \
  GIOPC.i \
  GIOPS.i \
  GIOPS_T.i \
  IIOP_Acceptor.i \
  IIOP_Connection_Handler.i \
  IIOP_Endpoint.i \
  IIOP_EndpointsC.i \
  IIOP_Profile.i \
  IIOP_Transport.i \
  IIOPC.i \
  Invocation_Endpoint_Selectors.i \
  IOP_CodecC.i \
  IOP_IORC.i \
  IOPC.i \
  IOPS.i \
  IOPS_T.i \
  IOR_Parser.i \
  Leader_Follower.i \
  LocalObject.i \
  Managed_Types.i \
  Marshal.i \
  MCAST_Parser.i \
  Messaging_PolicyValueC.i \
  Messaging_SyncScopeC.i \
  MProfile.i \
  NVList.i \
  Object.i \
  Object_KeyC.i \
  Object_Loader.i \
  ObjectIdListC.i \
  operation_details.i \
  ORB.i \
  ORB_Core.i \
  params.i \
  Parser_Registry.i \
  PI_ForwardC.i \
  Pluggable_Messaging.i \
  Pluggable_Messaging_Utils.i \
  Policy_ForwardC.i \
  Policy_Manager.i \
  Policy_Set.i \
  PolicyC.i \
  PollableC.i \
  PollableS.i \
  PollableS_T.i \
  PortableInterceptorC.i \
  Principal.i \
  Profile.i \
  qt_resource.i \
  Reply_Dispatcher.i \
  Sequence.i \
  Sequence_T.i \
  Service_Callbacks.i \
  ServicesC.i \
  ServicesS.i \
  ServicesS_T.i \
  ShortSeqC.i \
  Stub.i \
  Sync_Strategies.i \
  Tagged_Components.i \
  Tagged_Profile.i \
  TAO_Server_Request.i \
  TAOC.i \
  TAOS.i \
  TAOS_T.i \
  target_specification.i \
  Thread_Lane_Resources_Manager.i \
  TimeBaseC.i \
  TimeBaseS.i \
  TimeBaseS_T.i \
  Typecode.i \
  ULongSeqC.i \
  UShortSeqC.i \
  WrongTransactionC.i \
  xt_resource.i \
  Any_Array_Impl_T.inl \
  Any_Basic_Impl_T.inl \
  Any_Dual_Impl_T.inl \
  Any_Impl_T.inl \
  Any_Special_Impl_T.inl \
  AnySeqC.inl \
  Array_VarOut_T.inl \
  Base_Transport_Property.inl \
  Basic_Argument_T.inl \
  Basic_SArgument_T.inl \
  BD_String_Argument_T.inl \
  BD_String_SArgument_T.inl \
  Cache_Entries.inl \
  CharSeqC.inl \
  Cleanup_Func_Registry.inl \
  ClientRequestInfo.inl \
  ClientRequestInfo_i.inl \
  ClientRequestInterceptor_Adapter.inl \
  Condition.inl \
  Connection_Handler.inl \
  Connection_Purging_Strategy.inl \
  Connector_Impl.inl \
  CORBA_String.inl \
  DLL_ORB.inl \
  DoubleSeqC.inl \
  Fixed_Array_Argument_T.inl \
  Fixed_Array_SArgument_T.inl \
  Fixed_Size_Argument_T.inl \
  Fixed_Size_SArgument_T.inl \
  GIOP_Message_Generator_Parser.inl \
  GIOP_Message_Generator_Parser_10.inl \
  GIOP_Message_Generator_Parser_11.inl \
  GIOP_Message_Generator_Parser_12.inl \
  GIOP_Message_Generator_Parser_Impl.inl \
  GIOP_Message_State.inl \
  GIOP_Message_Version.inl \
  Incoming_Message_Queue.inl \
  Interceptor_List.inl \
  Invocation_Adapter.inl \
  Invocation_Base.inl \
  LF_Event.inl \
  LF_Event_Binder.inl \
  LF_Event_Loop_Thread_Helper.inl \
  LF_Follower.inl \
  LF_Follower_Auto_Adder.inl \
  LF_Follower_Auto_Ptr.inl \
  LF_Invocation_Event.inl \
  LF_Strategy.inl \
  LF_Strategy_Complete.inl \
  LongDoubleSeqC.inl \
  LongLongSeqC.inl \
  LongSeqC.inl \
  Object_Argument_T.inl \
  Object_SArgument_T.inl \
  Objref_VarOut_T.inl \
  OctetSeqC.inl \
  ORB_Core_Auto_Ptr.inl \
  ORB_Table.inl \
  ORBInitInfo.inl \
  PICurrent.inl \
  Profile_Transport_Resolver.inl \
  Pseudo_VarOut_T.inl \
  Queued_Message.inl \
  Refcounted_ObjectKey.inl \
  Resume_Handle.inl \
  Seq_Out_T.inl \
  Seq_Var_T.inl \
  Service_Context.inl \
  Special_Basic_Argument_T.inl \
  Special_Basic_SArgument_T.inl \
  StringSeqC.inl \
  Synch_Invocation.inl \
  Synch_Refcountable.inl \
  TAO_Singleton.inl \
  TAO_Singleton_Manager.inl \
  Transport.inl \
  Transport_Acceptor.inl \
  Transport_Cache_Manager.inl \
  Transport_Connector.inl \
  Transport_Descriptor_Interface.inl \
  UB_String_Argument_T.inl \
  UB_String_SArgument_T.inl \
  ULongLongSeqC.inl \
  Var_Array_Argument_T.inl \
  Var_Array_SArgument_T.inl \
  Var_Size_Argument_T.inl \
  Var_Size_SArgument_T.inl \
  VarOut_T.inl \
  Wait_Strategy.inl \
  WCharSeqC.inl \
  WStringSeqC.inl \
  Any_Array_Impl_T.cpp \
  Any_Basic_Impl_T.cpp \
  Any_Dual_Impl_T.cpp \
  Any_Impl_T.cpp \
  Any_Special_Impl_T.cpp \
  Array_VarOut_T.cpp \
  Basic_Argument_T.cpp \
  Basic_SArgument_T.cpp \
  BD_String_Argument_T.cpp \
  BD_String_SArgument_T.cpp \
  Buffer_Allocator_T.cpp \
  Codeset_Translator_Factory_T.cpp \
  CONV_FRAMES_T.cpp \
  Fixed_Array_Argument_T.cpp \
  Fixed_Array_SArgument_T.cpp \
  Fixed_Size_Argument_T.cpp \
  Fixed_Size_SArgument_T.cpp \
  GIOPS_T.cpp \
  IOPS_T.cpp \
  Object_Argument_T.cpp \
  Object_SArgument_T.cpp \
  Object_T.cpp \
  Objref_VarOut_T.cpp \
  PollableS_T.cpp \
  Pseudo_VarOut_T.cpp \
  Seq_Out_T.cpp \
  Seq_Var_T.cpp \
  Sequence_T.cpp \
  ServicesS_T.cpp \
  Special_Basic_Argument_T.cpp \
  Special_Basic_SArgument_T.cpp \
  TAOS_T.cpp \
  TimeBaseS_T.cpp \
  UB_String_Argument_T.cpp \
  UB_String_SArgument_T.cpp \
  Var_Array_Argument_T.cpp \
  Var_Array_SArgument_T.cpp \
  Var_Size_Argument_T.cpp \
  Var_Size_SArgument_T.cpp \
  VarOut_T.cpp \
  Acceptor_Impl.cpp \
  Condition.cpp \
  Connector_Impl.cpp \
  TAO_Singleton.cpp \
  orb.idl


## Makefile.BiDir_GIOP.am 

lib_LTLIBRARIES += libTAO_BiDirGIOP.la

libTAO_BiDirGIOP_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_BIDIRGIOP_BUILD_DLL

libTAO_BiDirGIOP_la_SOURCES = \
  BiDir_GIOP/BiDirPolicyC.cpp \
  BiDir_GIOP/BiDirPolicy_Validator.cpp \
  BiDir_GIOP/BiDir_ORBInitializer.cpp \
  BiDir_GIOP/BiDir_PolicyFactory.cpp \
  BiDir_GIOP/BiDir_Policy_i.cpp \
  BiDir_GIOP/BiDirGIOP.cpp

libTAO_BiDirGIOP_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_BiDirGIOP_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  BiDir_GIOP/BiDirGIOP.h \
  BiDir_GIOP/BiDirPolicyC.h \
  BiDir_GIOP/BiDir_ORBInitializer.h \
  BiDir_GIOP/BiDir_PolicyFactory.h \
  BiDir_GIOP/BiDir_Policy_i.h \
  BiDir_GIOP/bidirgiop_export.h \
  BiDir_GIOP/BiDirPolicy_Validator.h \
  BiDir_GIOP/BiDirPolicyC.i \
  BiDir_GIOP/BiDir_Policy_i.inl


## Makefile.Valuetype.am 

lib_LTLIBRARIES += libTAO_Valuetype.la

libTAO_Valuetype_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_VALUETYPE_BUILD_DLL

libTAO_Valuetype_la_SOURCES = \
  Valuetype/AbstractBase.cpp \
  Valuetype/AbstractBase_Invocation_Adapter.cpp \
  Valuetype/Seq_Tmplinst.cpp \
  Valuetype/ValueFactory_Map.cpp \
  Valuetype/ValueBase.cpp \
  Valuetype/ValueFactory.cpp \
  Valuetype/Valuetype_Adapter_Impl.cpp

libTAO_Valuetype_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Valuetype_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  Valuetype/AbstractBase.h \
  Valuetype/Sequence_T.h \
  Valuetype/AbstractBase_Invocation_Adapter.h \
  Valuetype/AbstractBase_T.h \
  Valuetype/ValueBase.h \
  Valuetype/ValueFactory.h \
  Valuetype/Value_CORBA_methods.h \
  Valuetype/ValueFactory_Map.h \
  Valuetype/Value_VarOut_T.h \
  Valuetype/Valuetype_Adapter_Impl.h \
  Valuetype/valuetype_export.h \
  Valuetype/AbstractBase.inl \
  Valuetype/Sequence_T.inl \
  Valuetype/ValueBase.inl \
  Valuetype/ValueFactory.inl \
  Valuetype/ValueFactory_Map.inl \
  Valuetype/Value_VarOut_T.inl \
  Valuetype/AbstractBase_T.cpp \
  Valuetype/Sequence_T.cpp \
  Valuetype/Value_VarOut_T.cpp


## Makefile.ObjRefTemplate.am 

lib_LTLIBRARIES += libTAO_ObjRefTemplate.la

libTAO_ObjRefTemplate_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_ORT_BUILD_DLL

libTAO_ObjRefTemplate_la_SOURCES = \
  ObjRefTemplate/ObjectReferenceTemplateC.cpp

libTAO_ObjRefTemplate_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_ObjRefTemplate_la_LIBADD = \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  ObjRefTemplate/ObjectReferenceTemplateC.h \
  ObjRefTemplate/ort_export.h \
  ObjRefTemplate/ObjectReferenceTemplateC.i


## Makefile.IORInterceptor.am 

lib_LTLIBRARIES += libTAO_IORInterceptor.la

libTAO_IORInterceptor_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_IORINTERCEPTOR_BUILD_DLL

libTAO_IORInterceptor_la_SOURCES = \
  IORInterceptor/IORInterceptor_List.cpp \
  IORInterceptor/IORInterceptorC.cpp \
  IORInterceptor/IORInterceptor_Adapter_Factory_Impl.cpp \
  IORInterceptor/IORInterceptor_Adapter_Impl.cpp

libTAO_IORInterceptor_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IORInterceptor_la_LIBADD = \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  IORInterceptor/IORInterceptorC.h \
  IORInterceptor/IORInterceptor_Adapter_Factory_Impl.h \
  IORInterceptor/IORInterceptor_Adapter_Impl.h \
  IORInterceptor/IORInterceptor_List.h \
  IORInterceptor/iorinterceptor_export.h \
  IORInterceptor/IORInterceptorC.i \
  IORInterceptor/IORInterceptor_List.inl


## Makefile.PortableServer.am 

lib_LTLIBRARIES += libTAO_PortableServer.la

libTAO_PortableServer_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_PORTABLESERVER_BUILD_DLL

libTAO_PortableServer_la_SOURCES = \
  PortableServer/Default_Acceptor_Filter.cpp \
  PortableServer/AMH_Response_Handler.cpp \
  PortableServer/Active_Object_Map.cpp \
  PortableServer/Object_Adapter.cpp \
  PortableServer/Collocated_Object_Proxy_Broker.cpp \
  PortableServer/Default_ORTC.cpp \
  PortableServer/IORInfo.cpp \
  PortableServer/Default_Servant_Dispatcher.cpp \
  PortableServer/Default_Policy_Validator.cpp \
  PortableServer/ImR_LocatorC.cpp \
  PortableServer/ImR_LocatorS.cpp \
  PortableServer/ImplRepoC.cpp \
  PortableServer/ImplRepoS.cpp \
  PortableServer/ImplRepo_i.cpp \
  PortableServer/Key_Adapters.cpp \
  PortableServer/ORB_Manager.cpp \
  PortableServer/POA.cpp \
  PortableServer/ObjectReferenceTemplate.cpp \
  PortableServer/Operation_Table.cpp \
  PortableServer/PICurrent_Guard.cpp \
  PortableServer/POA_Cached_Policies.cpp \
  PortableServer/POAManager.cpp \
  PortableServer/PortableServer.cpp \
  PortableServer/POA_Policies.cpp \
  PortableServer/POA_Policy_Set.cpp \
  PortableServer/PortableGroup_Hooks.cpp \
  PortableServer/PortableServerA.cpp \
  PortableServer/PortableServerC.cpp \
  PortableServer/PortableServer_ORBInitializer.cpp \
  PortableServer/PortableServer_PolicyFactory.cpp \
  PortableServer/Servant_Base.cpp \
  PortableServer/Servant_Dispatcher.cpp \
  PortableServer/ServerInterceptorAdapter.cpp \
  PortableServer/ServerRequestInfo.cpp \
  PortableServer/PolicyS.cpp

libTAO_PortableServer_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_PortableServer_la_LIBADD = \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  PortableServer/AMH_Response_Handler.h \
  PortableServer/Collocated_Object_Proxy_Broker.h \
  PortableServer/Default_Acceptor_Filter.h \
  PortableServer/Default_ORTC.h \
  PortableServer/POA.h \
  PortableServer/Default_Policy_Validator.h \
  PortableServer/IORInfo.h \
  PortableServer/Default_Servant_Dispatcher.h \
  PortableServer/ImR_LocatorC.h \
  PortableServer/ImR_LocatorS.h \
  PortableServer/ImplRepoC.h \
  PortableServer/ImplRepoS.h \
  PortableServer/ImplRepo_i.h \
  PortableServer/Key_Adapters.h \
  PortableServer/ORB_Manager.h \
  PortableServer/ObjectReferenceTemplate.h \
  PortableServer/Object_Adapter.h \
  PortableServer/Operation_Table.h \
  PortableServer/PolicyS_T.h \
  PortableServer/PolicyS.h \
  PortableServer/PICurrent_Guard.h \
  PortableServer/POAManager.h \
  PortableServer/POA_Cached_Policies.h \
  PortableServer/POA_Policies.h \
  PortableServer/POA_Policy_Set.h \
  PortableServer/PortableServer_ORBInitializer.h \
  PortableServer/PortableGroup_Hooks.h \
  PortableServer/PortableServer.h \
  PortableServer/PortableServerC.h \
  PortableServer/PortableServerS.h \
  PortableServer/poa_macros.h \
  PortableServer/PortableServer_PolicyFactory.h \
  PortableServer/Servant_Base.h \
  PortableServer/Servant_Dispatcher.h \
  PortableServer/ServerInterceptorAdapter.h \
  PortableServer/ServerRequestInfo.h \
  PortableServer/Upcall_Wrapper.h \
  PortableServer/portableserver_export.h \
  PortableServer/Active_Object_Map.h \
  PortableServer/Active_Object_Map.i \
  PortableServer/Default_ORTC.i \
  PortableServer/IORInfo.inl \
  PortableServer/ImR_LocatorC.i \
  PortableServer/ImR_LocatorS.i \
  PortableServer/ImplRepoC.i \
  PortableServer/ImplRepoS.i \
  PortableServer/Key_Adapters.i \
  PortableServer/ObjectReferenceTemplate.inl \
  PortableServer/Object_Adapter.i \
  PortableServer/POA.i \
  PortableServer/POAManager.i \
  PortableServer/POA_Cached_Policies.i \
  PortableServer/POA_Policies.i \
  PortableServer/POA_Policy_Set.i \
  PortableServer/PolicyS.i \
  PortableServer/PolicyS_T.i \
  PortableServer/PortableServerC.i \
  PortableServer/Servant_Base.i \
  PortableServer/ServerInterceptorAdapter.inl \
  PortableServer/ServerRequestInfo.inl \
  PortableServer/PolicyS_T.cpp


## Makefile.IFR_Client.am 

lib_LTLIBRARIES += libTAO_IFR_Client.la

libTAO_IFR_Client_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_IFR_CLIENT_BUILD_DLL

libTAO_IFR_Client_la_SOURCES = \
  IFR_Client/IFR_BaseA.cpp \
  IFR_Client/IFR_BaseC.cpp \
  IFR_Client/IFR_BasicA.cpp \
  IFR_Client/IFR_BasicC.cpp \
  IFR_Client/IFR_Client_Adapter_Impl.cpp \
  IFR_Client/IFR_ComponentsA.cpp \
  IFR_Client/IFR_ComponentsC.cpp \
  IFR_Client/IFR_ExtendedA.cpp \
  IFR_Client/IFR_ExtendedC.cpp

libTAO_IFR_Client_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IFR_Client_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  IFR_Client/IFR_BaseC.h \
  IFR_Client/IFR_BaseS.h \
  IFR_Client/IFR_BasicC.h \
  IFR_Client/IFR_Client_Adapter_Impl.h \
  IFR_Client/IFR_ComponentsC.h \
  IFR_Client/IFR_ExtendedC.h \
  IFR_Client/ifr_client_export.h \
  IFR_Client/IFR_BaseC.i \
  IFR_Client/IFR_BasicC.i \
  IFR_Client/IFR_ComponentsC.i \
  IFR_Client/IFR_ExtendedC.i


## Makefile.Domain.am 

lib_LTLIBRARIES += libTAO_Domain.la

libTAO_Domain_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_DOMAIN_BUILD_DLL

libTAO_Domain_la_SOURCES = \
  Domain/DomainS.cpp

libTAO_Domain_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Domain_la_LIBADD = \
 libTAO_PortableServer.la \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO_IFR_Client.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  Domain/DomainS.h \
  Domain/DomainS_T.h \
  Domain/domain_export.h \
  Domain/DomainS.i \
  Domain/DomainS_T.i \
  Domain/DomainS_T.cpp


## Makefile.DynamicAny.am 

lib_LTLIBRARIES += libTAO_DynamicAny.la

libTAO_DynamicAny_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_DYNAMICANY_BUILD_DLL

libTAO_DynamicAny_la_SOURCES = \
  DynamicAny/DynAny_i.cpp \
  DynamicAny/DynArray_i.cpp \
  DynamicAny/DynCommon.cpp \
  DynamicAny/DynEnum_i.cpp \
  DynamicAny/DynSequence_i.cpp \
  DynamicAny/DynStruct_i.cpp \
  DynamicAny/DynUnion_i.cpp \
  DynamicAny/DynamicAny.cpp \
  DynamicAny/DynamicAnyC.cpp \
  DynamicAny/DynAnyFactory.cpp

libTAO_DynamicAny_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DynamicAny_la_LIBADD = \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  DynamicAny/DynAny_i.h \
  DynamicAny/DynArray_i.h \
  DynamicAny/DynCommon.h \
  DynamicAny/DynEnum_i.h \
  DynamicAny/DynSequence_i.h \
  DynamicAny/DynStruct_i.h \
  DynamicAny/DynUnion_i.h \
  DynamicAny/DynamicAny.h \
  DynamicAny/DynamicAnyC.h \
  DynamicAny/dynamicany_export.h \
  DynamicAny/DynAnyFactory.h \
  DynamicAny/DynamicAnyC.i


## Makefile.Messaging.am 

lib_LTLIBRARIES += libTAO_Messaging.la

libTAO_Messaging_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_MESSAGING_BUILD_DLL

libTAO_Messaging_la_SOURCES = \
  Messaging/Connection_Timeout_Policy_i.cpp \
  Messaging/Asynch_Reply_Dispatcher.cpp \
  Messaging/Asynch_Timeout_Handler.cpp \
  Messaging/Messaging.cpp \
  Messaging/MessagingC.cpp \
  Messaging/MessagingS.cpp \
  Messaging/Messaging_ORBInitializer.cpp \
  Messaging/Messaging_No_ImplC.cpp \
  Messaging/Messaging_Policy_i.cpp \
  Messaging/Messaging_PolicyFactory.cpp \
  Messaging/Messaging_RT_PolicyC.cpp \
  Messaging/Messaging_SyncScope_PolicyC.cpp \
  Messaging/TAO_ExtC.cpp \
  Messaging/Asynch_Invocation_Adapter.cpp \
  Messaging/Asynch_Invocation.cpp

libTAO_Messaging_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Messaging_la_LIBADD = \
 libTAO_PortableServer.la \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  Messaging/Asynch_Invocation.h \
  Messaging/Asynch_Invocation_Adapter.h \
  Messaging/Asynch_Reply_Dispatcher.h \
  Messaging/Asynch_Timeout_Handler.h \
  Messaging/Connection_Timeout_Policy_i.h \
  Messaging/Messaging.h \
  Messaging/TAO_ExtC.h \
  Messaging/MessagingC.h \
  Messaging/MessagingS.h \
  Messaging/MessagingS_T.h \
  Messaging/Messaging_No_ImplC.h \
  Messaging/Messaging_ORBInitializer.h \
  Messaging/Messaging_PolicyFactory.h \
  Messaging/Messaging_Policy_i.h \
  Messaging/Messaging_RT_PolicyC.h \
  Messaging/Messaging_SyncScope_PolicyC.h \
  Messaging/messaging_export.h \
  Messaging/Asynch_Invocation_Adapter.inl \
  Messaging/Asynch_Reply_Dispatcher.i \
  Messaging/MessagingC.i \
  Messaging/MessagingS.i \
  Messaging/MessagingS_T.i \
  Messaging/Messaging_No_ImplC.i \
  Messaging/Messaging_Policy_i.i \
  Messaging/Messaging_RT_PolicyC.i \
  Messaging/Messaging_SyncScope_PolicyC.i \
  Messaging/TAO_ExtC.i \
  Messaging/MessagingS_T.cpp


## Makefile.DynamicInterface.am 

lib_LTLIBRARIES += libTAO_DynamicInterface.la

libTAO_DynamicInterface_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_DYNAMICINTERFACE_BUILD_DLL

libTAO_DynamicInterface_la_SOURCES = \
  DynamicInterface/DII_Arguments.cpp \
  DynamicInterface/Context.cpp \
  DynamicInterface/DII_Invocation_Adapter.cpp \
  DynamicInterface/DII_Invocation.cpp \
  DynamicInterface/Dynamic_Implementation.cpp \
  DynamicInterface/DII_Reply_Dispatcher.cpp \
  DynamicInterface/Request.cpp \
  DynamicInterface/ExceptionList.cpp \
  DynamicInterface/Dynamic_Adapter_Impl.cpp \
  DynamicInterface/Unknown_User_Exception.cpp \
  DynamicInterface/Server_Request.cpp

libTAO_DynamicInterface_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DynamicInterface_la_LIBADD = \
 libTAO_Messaging.la \
 libTAO_PortableServer.la \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  DynamicInterface/Context.h \
  DynamicInterface/DII_Arguments.h \
  DynamicInterface/DII_CORBA_methods.h \
  DynamicInterface/DII_Invocation.h \
  DynamicInterface/DII_Invocation_Adapter.h \
  DynamicInterface/DII_Reply_Dispatcher.h \
  DynamicInterface/Dynamic_Adapter_Impl.h \
  DynamicInterface/Dynamic_Implementation.h \
  DynamicInterface/ExceptionList.h \
  DynamicInterface/Request.h \
  DynamicInterface/Server_Request.h \
  DynamicInterface/Unknown_User_Exception.h \
  DynamicInterface/dynamicinterface_export.h \
  DynamicInterface/Context.inl \
  DynamicInterface/DII_Arguments.inl \
  DynamicInterface/DII_Invocation.inl \
  DynamicInterface/ExceptionList.inl \
  DynamicInterface/Request.inl \
  DynamicInterface/Server_Request.inl


## Makefile.IORManipulation.am 

lib_LTLIBRARIES += libTAO_IORManip.la

libTAO_IORManip_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_IORMANIP_BUILD_DLL

libTAO_IORManip_la_SOURCES = \
  IORManipulation/IORA.cpp \
  IORManipulation/IORC.cpp \
  IORManipulation/IORManip_Loader.cpp \
  IORManipulation/IORManipulation.cpp

libTAO_IORManip_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IORManip_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  IORManipulation/IORC.h \
  IORManipulation/IORManip_Loader.h \
  IORManipulation/IORManipulation.h \
  IORManipulation/ior_manip_export.h \
  IORManipulation/IORC.i


## Makefile.IORTable.am 

lib_LTLIBRARIES += libTAO_IORTable.la

libTAO_IORTable_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_IORTABLE_BUILD_DLL

libTAO_IORTable_la_SOURCES = \
  IORTable/IOR_Table_Impl.cpp \
  IORTable/IORTableC.cpp \
  IORTable/Table_Adapter.cpp \
  IORTable/IORTable.cpp

libTAO_IORTable_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IORTable_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  IORTable/IORTable.h \
  IORTable/IORTableC.h \
  IORTable/IOR_Table_Impl.h \
  IORTable/iortable_export.h \
  IORTable/Table_Adapter.h \
  IORTable/IORTableC.i


## Makefile.RTCORBA.am 

lib_LTLIBRARIES += libTAO_RTCORBA.la

libTAO_RTCORBA_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTCORBA_BUILD_DLL

libTAO_RTCORBA_la_SOURCES = \
  RTCORBA/Direct_Priority_Mapping.cpp \
  RTCORBA/RTCORBA.cpp \
  RTCORBA/Linear_Priority_Mapping.cpp \
  RTCORBA/Multi_Priority_Mapping.cpp \
  RTCORBA/Network_Priority_Mapping.cpp \
  RTCORBA/Network_Priority_Mapping_Manager.cpp \
  RTCORBA/Priority_Mapping.cpp \
  RTCORBA/RTCORBAC.cpp \
  RTCORBA/Priority_Mapping_Manager.cpp \
  RTCORBA/RT_Current.cpp \
  RTCORBA/RT_Invocation_Endpoint_Selectors.cpp \
  RTCORBA/RT_Endpoint_Selector_Factory.cpp \
  RTCORBA/RT_Endpoint_Utils.cpp \
  RTCORBA/RT_Mutex.cpp \
  RTCORBA/RT_ORB.cpp \
  RTCORBA/RT_Transport_Descriptor.cpp \
  RTCORBA/RT_ORBInitializer.cpp \
  RTCORBA/RT_ORB_Loader.cpp \
  RTCORBA/RT_PolicyFactory.cpp \
  RTCORBA/RT_Policy_i.cpp \
  RTCORBA/RT_Protocols_Hooks.cpp \
  RTCORBA/RT_Stub.cpp \
  RTCORBA/RT_Stub_Factory.cpp \
  RTCORBA/RT_Thread_Lane_Resources_Manager.cpp \
  RTCORBA/RT_Transport_Descriptor_Property.cpp \
  RTCORBA/Thread_Pool.cpp \
  RTCORBA/Linear_Network_Priority_Mapping.cpp \
  RTCORBA/Continuous_Priority_Mapping.cpp

libTAO_RTCORBA_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTCORBA_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  RTCORBA/Continuous_Priority_Mapping.h \
  RTCORBA/Direct_Priority_Mapping.h \
  RTCORBA/Priority_Mapping.h \
  RTCORBA/Linear_Network_Priority_Mapping.h \
  RTCORBA/Linear_Priority_Mapping.h \
  RTCORBA/RTCORBA.h \
  RTCORBA/Network_Priority_Mapping_Manager.h \
  RTCORBA/Multi_Priority_Mapping.h \
  RTCORBA/Network_Priority_Mapping.h \
  RTCORBA/RTCORBA_includeC.h \
  RTCORBA/Priority_Mapping_Manager.h \
  RTCORBA/RTCORBAC.h \
  RTCORBA/RTCORBA_includeS.h \
  RTCORBA/RT_Current.h \
  RTCORBA/RT_Endpoint_Selector_Factory.h \
  RTCORBA/RT_Endpoint_Utils.h \
  RTCORBA/RT_ORBInitializer.h \
  RTCORBA/RT_Invocation_Endpoint_Selectors.h \
  RTCORBA/RT_Mutex.h \
  RTCORBA/RT_ORB.h \
  RTCORBA/RT_ORB_Loader.h \
  RTCORBA/RT_PolicyFactory.h \
  RTCORBA/RT_Policy_i.h \
  RTCORBA/RT_Protocols_Hooks.h \
  RTCORBA/RT_Stub.h \
  RTCORBA/RT_Stub_Factory.h \
  RTCORBA/RT_Thread_Lane_Resources_Manager.h \
  RTCORBA/RT_Transport_Descriptor.h \
  RTCORBA/RT_Transport_Descriptor_Property.h \
  RTCORBA/Thread_Pool.h \
  RTCORBA/rtcorba_export.h \
  RTCORBA/rtcorba_typedefs.h \
  RTCORBA/Continuous_Priority_Mapping.i \
  RTCORBA/Direct_Priority_Mapping.i \
  RTCORBA/Linear_Network_Priority_Mapping.i \
  RTCORBA/Linear_Priority_Mapping.i \
  RTCORBA/Multi_Priority_Mapping.i \
  RTCORBA/Network_Priority_Mapping.i \
  RTCORBA/Network_Priority_Mapping_Manager.i \
  RTCORBA/Priority_Mapping.i \
  RTCORBA/Priority_Mapping_Manager.i \
  RTCORBA/RTCORBAC.i \
  RTCORBA/RT_Current.i \
  RTCORBA/RT_Invocation_Endpoint_Selectors.i \
  RTCORBA/RT_Mutex.i \
  RTCORBA/RT_ORB.i \
  RTCORBA/RT_Policy_i.i \
  RTCORBA/RT_Protocols_Hooks.i \
  RTCORBA/RT_Thread_Lane_Resources_Manager.i \
  RTCORBA/RT_Transport_Descriptor.inl \
  RTCORBA/RT_Transport_Descriptor_Property.inl \
  RTCORBA/Thread_Pool.i


## Makefile.RTPortableServer.am 

lib_LTLIBRARIES += libTAO_RTPortableServer.la

libTAO_RTPortableServer_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTPORTABLESERVER_BUILD_DLL

libTAO_RTPortableServer_la_SOURCES = \
  RTPortableServer/RT_Collocation_Resolver.cpp \
  RTPortableServer/RTPortableServerA.cpp \
  RTPortableServer/RTPortableServerC.cpp \
  RTPortableServer/RT_Acceptor_Filters.cpp \
  RTPortableServer/RT_Object_Adapter_Factory.cpp \
  RTPortableServer/RT_POA.cpp \
  RTPortableServer/RT_Servant_Dispatcher.cpp \
  RTPortableServer/RT_Policy_Validator.cpp \
  RTPortableServer/RTPortableServer.cpp

libTAO_RTPortableServer_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTPortableServer_la_LIBADD = \
 libTAO_PortableServer.la \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO_RTCORBA.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  RTPortableServer/RTPortableServer.h \
  RTPortableServer/RTPortableServerC.h \
  RTPortableServer/RT_Acceptor_Filters.h \
  RTPortableServer/RT_Collocation_Resolver.h \
  RTPortableServer/RT_POA.h \
  RTPortableServer/RT_Object_Adapter_Factory.h \
  RTPortableServer/RT_Policy_Validator.h \
  RTPortableServer/rtportableserver_export.h \
  RTPortableServer/RT_Servant_Dispatcher.h \
  RTPortableServer/RTPortableServerC.i \
  RTPortableServer/RT_Acceptor_Filters.i \
  RTPortableServer/RT_Collocation_Resolver.i \
  RTPortableServer/RT_POA.i


## Makefile.RTScheduler.am 

lib_LTLIBRARIES += libTAO_RTScheduler.la

libTAO_RTScheduler_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTSCHEDULER_BUILD_DLL

libTAO_RTScheduler_la_SOURCES = \
  RTScheduling/RTScheduler_Initializer.cpp \
  RTScheduling/Distributable_Thread.cpp \
  RTScheduling/RTSchedulerC.cpp \
  RTScheduling/RTScheduler_Loader.cpp \
  RTScheduling/RTScheduler_Manager.cpp \
  RTScheduling/Request_Interceptor.cpp \
  RTScheduling/RTScheduler.cpp \
  RTScheduling/Current.cpp

libTAO_RTScheduler_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTScheduler_la_LIBADD = \
 libTAO_PortableServer.la \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO_RTCORBA.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  RTScheduling/Current.h \
  RTScheduling/Distributable_Thread.h \
  RTScheduling/RTScheduler.h \
  RTScheduling/RTSchedulerC.h \
  RTScheduling/RTScheduler_Initializer.h \
  RTScheduling/RTScheduler_Loader.h \
  RTScheduling/RTScheduler_Manager.h \
  RTScheduling/RTScheduler_includeC.h \
  RTScheduling/Request_Interceptor.h \
  RTScheduling/ThreadAction.h \
  RTScheduling/rtscheduler_export.h \
  RTScheduling/RTSchedulerC.i \
  RTScheduling/RTScheduler_Manager.i


## Makefile.SmartProxies.am 

lib_LTLIBRARIES += libTAO_SmartProxies.la

libTAO_SmartProxies_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_SMARTPROXIES_BUILD_DLL

libTAO_SmartProxies_la_SOURCES = \
  SmartProxies/Smart_Proxies.cpp

libTAO_SmartProxies_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_SmartProxies_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  SmartProxies/Smart_Proxies.h \
  SmartProxies/smartproxies_export.h \
  SmartProxies/Smart_Proxies.inl


## Makefile.Strategies.am 

lib_LTLIBRARIES += libTAO_Strategies.la

libTAO_Strategies_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_STRATEGIES_BUILD_DLL

libTAO_Strategies_la_SOURCES = \
  Strategies/DIOP_Acceptor.cpp \
  Strategies/DIOP_Connector.cpp \
  Strategies/DIOP_Transport.cpp \
  Strategies/DIOP_Connection_Handler.cpp \
  Strategies/DIOP_Endpoint.cpp \
  Strategies/DIOP_Factory.cpp \
  Strategies/DIOP_Profile.cpp \
  Strategies/LF_Strategy_Null.cpp \
  Strategies/FIFO_Connection_Purging_Strategy.cpp \
  Strategies/LFU_Connection_Purging_Strategy.cpp \
  Strategies/NULL_Connection_Purging_Strategy.cpp \
  Strategies/SCIOP_Acceptor.cpp \
  Strategies/SHMIOP_Connection_Handler.cpp \
  Strategies/SCIOP_Connection_Handler.cpp \
  Strategies/SCIOP_Connector.cpp \
  Strategies/SCIOP_Endpoint.cpp \
  Strategies/SCIOP_Factory.cpp \
  Strategies/SCIOP_Lite_Factory.cpp \
  Strategies/SCIOP_Profile.cpp \
  Strategies/SCIOP_Transport.cpp \
  Strategies/SHMIOP_Acceptor.cpp \
  Strategies/UIOP_Connection_Handler.cpp \
  Strategies/SHMIOP_Connector.cpp \
  Strategies/SHMIOP_Endpoint.cpp \
  Strategies/SHMIOP_Factory.cpp \
  Strategies/SHMIOP_Profile.cpp \
  Strategies/SHMIOP_Transport.cpp \
  Strategies/UIOP_Acceptor.cpp \
  Strategies/UIOP_Lite_Factory.cpp \
  Strategies/UIOP_Connector.cpp \
  Strategies/UIOP_Endpoint.cpp \
  Strategies/UIOP_Factory.cpp \
  Strategies/advanced_resource.cpp \
  Strategies/UIOP_Profile.cpp \
  Strategies/UIOP_Transport.cpp \
  Strategies/sciop_endpoints.cpp \
  Strategies/uiop_endpoints.cpp

libTAO_Strategies_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Strategies_la_LIBADD = \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  Strategies/DIOP_Acceptor.h \
  Strategies/DIOP_Connection_Handler.h \
  Strategies/DIOP_Connector.h \
  Strategies/DIOP_Endpoint.h \
  Strategies/DIOP_Factory.h \
  Strategies/DIOP_Profile.h \
  Strategies/DIOP_Transport.h \
  Strategies/SCIOP_Connection_Handler.h \
  Strategies/FIFO_Connection_Purging_Strategy.h \
  Strategies/LFU_Connection_Purging_Strategy.h \
  Strategies/LF_Strategy_Null.h \
  Strategies/NULL_Connection_Purging_Strategy.h \
  Strategies/SCIOP_Acceptor.h \
  Strategies/SCIOP_Connector.h \
  Strategies/SCIOP_Endpoint.h \
  Strategies/SCIOP_Factory.h \
  Strategies/SCIOP_Lite_Factory.h \
  Strategies/SCIOP_Profile.h \
  Strategies/SCIOP_Transport.h \
  Strategies/SHMIOP_Acceptor.h \
  Strategies/SHMIOP_Connection_Handler.h \
  Strategies/SHMIOP_Connector.h \
  Strategies/SHMIOP_Endpoint.h \
  Strategies/SHMIOP_Factory.h \
  Strategies/SHMIOP_Profile.h \
  Strategies/SHMIOP_Transport.h \
  Strategies/UIOP_Acceptor.h \
  Strategies/UIOP_Connection_Handler.h \
  Strategies/UIOP_Connector.h \
  Strategies/UIOP_Endpoint.h \
  Strategies/UIOP_Factory.h \
  Strategies/UIOP_Lite_Factory.h \
  Strategies/UIOP_Profile.h \
  Strategies/UIOP_Transport.h \
  Strategies/advanced_resource.h \
  Strategies/sciop_endpoints.h \
  Strategies/strategies_export.h \
  Strategies/uiop_endpoints.h \
  Strategies/DIOP_Acceptor.i \
  Strategies/DIOP_Connection_Handler.i \
  Strategies/DIOP_Endpoint.i \
  Strategies/DIOP_Profile.i \
  Strategies/DIOP_Transport.i \
  Strategies/LF_Strategy_Null.inl \
  Strategies/SCIOP_Acceptor.i \
  Strategies/SCIOP_Connection_Handler.i \
  Strategies/SCIOP_Endpoint.i \
  Strategies/SCIOP_Profile.i \
  Strategies/SCIOP_Transport.i \
  Strategies/SHMIOP_Acceptor.i \
  Strategies/SHMIOP_Connection_Handler.inl \
  Strategies/SHMIOP_Endpoint.i \
  Strategies/SHMIOP_Profile.i \
  Strategies/SHMIOP_Transport.i \
  Strategies/UIOP_Connection_Handler.inl \
  Strategies/UIOP_Endpoint.i \
  Strategies/UIOP_Profile.i \
  Strategies/UIOP_Transport.i \
  Strategies/advanced_resource.i \
  Strategies/sciop_endpoints.i \
  Strategies/uiop_endpoints.i


## Makefile.TypeCodeFactory.am 

lib_LTLIBRARIES += libTAO_TypeCodeFactory.la

libTAO_TypeCodeFactory_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_TYPECODEFACTORY_BUILD_DLL

libTAO_TypeCodeFactory_la_SOURCES = \
  TypeCodeFactory/TypeCodeFactoryC.cpp \
  TypeCodeFactory/TypeCodeFactory_Adapter_Impl.cpp \
  TypeCodeFactory/TypeCodeFactory_i.cpp \
  TypeCodeFactory/TypeCodeFactory_Loader.cpp

libTAO_TypeCodeFactory_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_TypeCodeFactory_la_LIBADD = \
 libTAO_IFR_Client.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  TypeCodeFactory/TypeCodeFactoryC.h \
  TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h \
  TypeCodeFactory/TypeCodeFactory_Loader.h \
  TypeCodeFactory/TypeCodeFactory_i.h \
  TypeCodeFactory/typecodefactory_export.h \
  TypeCodeFactory/TypeCodeFactoryC.inl


## Makefile.Utils.am 

lib_LTLIBRARIES += libTAO_Utils.la

libTAO_Utils_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_UTILS_BUILD_DLL

libTAO_Utils_la_SOURCES = \
  Utils/Implicit_Deactivator.cpp \
  Utils/ORB_Destroyer.cpp \
  Utils/PolicyList_Destroyer.cpp \
  Utils/RIR_Narrow.cpp \
  Utils/Servant_Var.cpp \
  Utils/Server_Main.cpp

libTAO_Utils_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Utils_la_LIBADD = \
 libTAO_PortableServer.la \
 libTAO_IORInterceptor.la \
 libTAO_ObjRefTemplate.la \
 libTAO_Valuetype.la \
 libTAO.la \
 $(ACE_BUILDDIR)/ace/libACE.la

nobase_pkginclude_HEADERS += \
  Utils/Auto_Functor.h \
  Utils/Implicit_Deactivator.h \
  Utils/ORB_Destroyer.h \
  Utils/PolicyList_Destroyer.h \
  Utils/RIR_Narrow.h \
  Utils/Servant_Var.h \
  Utils/Server_Main.h \
  Utils/utils_export.h \
  Utils/PolicyList_Destroyer.inl \
  Utils/Servant_Var.inl


## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB

##  Process this file with automake to create Makefile.in

includedir = @includedir@/tao
pkgconfigdatadir = ${prefix}/lib/pkgconfig

ACE_ROOT = $(top_srcdir)/..
ACE_BUILDDIR = $(top_builddir)/..
TAO_ROOT = $(top_srcdir)
TAO_BUILDDIR = $(top_builddir)

nobase_include_HEADERS =
lib_LTLIBRARIES =

## Makefile.TAO.am

ORB_Core = \
  Abstract_Servant_Base.cpp \
  Adapter.cpp \
  Adapter_Factory.cpp \
  Adapter_Registry.cpp \
  Any.cpp \
  AnySeqA.cpp \
  AnySeqC.cpp \
  Any_Basic_Impl.cpp \
  Any_Impl.cpp \
  Any_SystemException.cpp \
  Any_Unknown_IDL_Type.cpp \
  Argument.cpp \
  Asynch_Queued_Message.cpp \
  Asynch_Reply_Dispatcher_Base.cpp \
  Base_Transport_Property.cpp \
  Basic_Arguments.cpp \
  BiDir_Adapter.cpp \
  Bind_Dispatcher_Guard.cpp \
  Block_Flushing_Strategy.cpp \
  Blocked_Connect_Strategy.cpp \
  BooleanSeqA.cpp \
  BooleanSeqC.cpp \
  BoundsA.cpp \
  BoundsC.cpp \
  Buffering_Constraint_Policy.cpp \
  CDR.cpp \
  CDR_Encaps_Codec.cpp \
  CONV_FRAMEA.cpp \
  CONV_FRAMEC.cpp \
  CORBALOC_Parser.cpp \
  CORBANAME_Parser.cpp \
  CORBA_String.cpp \
  Cache_Entries.cpp \
  CharSeqA.cpp \
  CharSeqC.cpp \
  Cleanup_Func_Registry.cpp \
  ClientRequestInfo.cpp \
  ClientRequestInfo_i.cpp \
  ClientRequestInterceptor_Adapter.cpp \
  Client_Strategy_Factory.cpp \
  CodecFactory.cpp \
  CodecFactory_ORBInitializer.cpp \
  Codeset_Manager.cpp \
  Codeset_Translator_Factory.cpp \
  Collocated_Invocation.cpp \
  Collocation_Resolver.cpp \
  Connect_Strategy.cpp \
  Connection_Purging_Strategy.cpp \
  CurrentA.cpp \
  CurrentC.cpp \
  DLL_Parser.cpp \
  Default_Collocation_Resolver.cpp \
  Default_Endpoint_Selector_Factory.cpp \
  Default_Protocols_Hooks.cpp \
  Default_Stub_Factory.cpp \
  Default_Thread_Lane_Resources_Manager.cpp \
  DomainA.cpp \
  DomainC.cpp \
  DoubleSeqA.cpp \
  DoubleSeqC.cpp \
  DynamicA.cpp \
  DynamicC.cpp \
  Dynamic_Adapter.cpp \
  Dynamic_ParameterA.cpp \
  Dynamic_ParameterC.cpp \
  Encodable.cpp \
  Endpoint_Selector_Factory.cpp \
  Environment.cpp \
  Exception.cpp \
  Exclusive_TMS.cpp \
  FILE_Parser.cpp \
  Fault_Tolerance_Service.cpp \
  FloatSeqA.cpp \
  FloatSeqC.cpp \
  Flushing_Strategy.cpp \
  GIOPA.cpp \
  GIOPC.cpp \
  IFR_Client_Adapter.cpp \
  IOPC.cpp \
  IOP_CodecA.cpp \
  IOP_CodecC.cpp \
  IOP_IORA.cpp \
  IOP_IORC.cpp \
  IORInterceptor_Adapter.cpp \
  IORInterceptor_Adapter_Factory.cpp \
  IOR_Parser.cpp \
  Interceptor_List.cpp \
  Invocation_Adapter.cpp \
  Invocation_Base.cpp \
  Invocation_Endpoint_Selectors.cpp \
  LF_CH_Event.cpp \
  LF_Connect_Strategy.cpp \
  LF_Event.cpp \
  LF_Event_Binder.cpp \
  LF_Event_Loop_Thread_Helper.cpp \
  LF_Follower.cpp \
  LF_Follower_Auto_Adder.cpp \
  LF_Follower_Auto_Ptr.cpp \
  LF_Invocation_Event.cpp \
  LF_Strategy.cpp \
  LF_Strategy_Complete.cpp \
  LRU_Connection_Purging_Strategy.cpp \
  Leader_Follower.cpp \
  Leader_Follower_Flushing_Strategy.cpp \
  LocalObject.cpp \
  LocateRequest_Invocation.cpp \
  LocateRequest_Invocation_Adapter.cpp \
  LongDoubleSeqA.cpp \
  LongDoubleSeqC.cpp \
  LongLongSeqA.cpp \
  LongLongSeqC.cpp \
  LongSeqA.cpp \
  LongSeqC.cpp \
  MCAST_Parser.cpp \
  MProfile.cpp \
  Messaging_PolicyValueC.cpp \
  Messaging_SyncScopeC.cpp \
  Muxed_TMS.cpp \
  NVList.cpp \
  ORB.cpp \
  orb_typesC.cpp \
  orb_typesA.cpp \
  ORBInitInfo.cpp \
  ORBInitializer_Registry.cpp \
  ORB_Core.cpp \
  ORB_Core_Auto_Ptr.cpp \
  ORB_Table.cpp \
  Object.cpp \
  ObjectIdListC.cpp \
  ObjectIdListA.cpp \
  ObjectKey_Table.cpp \
  Object_KeyC.cpp \
  Object_Loader.cpp \
  Object_Proxy_Broker.cpp \
  Object_Proxy_Impl.cpp \
  Object_Ref_Table.cpp \
  OctetSeqA.cpp \
  OctetSeqC.cpp \
  PICurrent.cpp \
  PICurrent_Copy_Callback.cpp \
  PICurrent_ORBInitializer.cpp \
  PI_ForwardC.cpp \
  ParameterModeC.cpp \
  Parser_Registry.cpp \
  PolicyA.cpp \
  PolicyC.cpp \
  PolicyFactory_Registry.cpp \
  Policy_ForwardA.cpp \
  Policy_ForwardC.cpp \
  Policy_Manager.cpp \
  Policy_Set.cpp \
  Policy_Validator.cpp \
  PollableC.cpp \
  PortableInterceptorA.cpp \
  PortableInterceptorC.cpp \
  PredefinedType_Seq_Tmplinst.cpp \
  Principal.cpp \
  Profile_Transport_Resolver.cpp \
  Protocols_Hooks.cpp \
  Queued_Message.cpp \
  Reactive_Connect_Strategy.cpp \
  Reactive_Flushing_Strategy.cpp \
  Refcounted_ObjectKey.cpp \
  Remote_Invocation.cpp \
  Remote_Object_Proxy_Broker.cpp \
  Reply_Dispatcher.cpp \
  RequestInfo_Util.cpp \
  Request_Dispatcher.cpp \
  Resource_Factory.cpp \
  Sequence.cpp \
  Server_Strategy_Factory.cpp \
  Service_Callbacks.cpp \
  Service_Context.cpp \
  ServicesC.cpp \
  Services_Activate.cpp \
  ShortSeqA.cpp \
  ShortSeqC.cpp \
  Special_Basic_Arguments.cpp \
  StringSeqA.cpp \
  StringSeqC.cpp \
  Stub.cpp \
  Stub_Factory.cpp \
  Sync_Strategies.cpp \
  Synch_Invocation.cpp \
  Synch_Queued_Message.cpp \
  Synch_Refcountable.cpp \
  Synch_Reply_Dispatcher.cpp \
  SystemException.cpp \
  TAOA.cpp \
  TAOC.cpp \
  TAO_Internal.cpp \
  TAO_Server_Request.cpp \
  TAO_Singleton_Manager.cpp \
  TSS_Resources.cpp \
  Tagged_Components.cpp \
  Thread_Lane_Resources.cpp \
  Thread_Lane_Resources_Manager.cpp \
  TimeBaseA.cpp \
  TimeBaseC.cpp \
  Transport_Cache_Manager.cpp \
  Transport_Descriptor_Interface.cpp \
  Transport_Mux_Strategy.cpp \
  Transport_Timer.cpp \
  TypeCodeFactory_Adapter.cpp \
  Typecode.cpp \
  Typecode_typesC.cpp \
  Typecode_Constants.cpp \
  ULongLongSeqA.cpp \
  ULongLongSeqC.cpp \
  ULongSeqA.cpp \
  ULongSeqC.cpp \
  UShortSeqA.cpp \
  UShortSeqC.cpp \
  UTF16_BOM_Factory.cpp \
  UTF16_BOM_Translator.cpp \
  UserException.cpp \
  Valuetype_Adapter.cpp \
  WCharSeqA.cpp \
  WCharSeqC.cpp \
  WStringSeqA.cpp \
  WStringSeqC.cpp \
  Wait_On_LF_No_Upcall.cpp \
  Wait_On_Leader_Follower.cpp \
  Wait_On_Reactor.cpp \
  Wait_On_Read.cpp \
  Wait_Strategy.cpp \
  WrongTransactionA.cpp \
  WrongTransactionC.cpp \
  debug.cpp \
  operation_details.cpp \
  params.cpp

Pluggable_Protocols = \
  Acceptor_Filter.cpp \
  Acceptor_Registry.cpp \
  Connection_Handler.cpp \
  Connector_Registry.cpp \
  Endpoint.cpp \
  IIOPA.cpp \
  IIOPC.cpp \
  IIOP_Acceptor.cpp \
  IIOP_Connection_Handler.cpp \
  IIOP_Connector.cpp \
  IIOP_Endpoint.cpp \
  IIOP_EndpointsA.cpp \
  IIOP_EndpointsC.cpp \
  IIOP_Factory.cpp \
  IIOP_Lite_Factory.cpp \
  IIOP_Profile.cpp \
  IIOP_Transport.cpp \
  Incoming_Message_Queue.cpp \
  Profile.cpp \
  Protocol_Factory.cpp \
  Resume_Handle.cpp \
  Thread_Per_Connection_Handler.cpp \
  Transport.cpp \
  Transport_Acceptor.cpp \
  Transport_Connector.cpp

Pluggable_Messaging = \
  GIOP_Message_Base.cpp \
  GIOP_Message_Generator_Parser.cpp \
  GIOP_Message_Generator_Parser_10.cpp \
  GIOP_Message_Generator_Parser_11.cpp \
  GIOP_Message_Generator_Parser_12.cpp \
  GIOP_Message_Generator_Parser_Impl.cpp \
  GIOP_Message_Lite.cpp \
  GIOP_Message_Locate_Header.cpp \
  GIOP_Message_State.cpp \
  GIOP_Message_Version.cpp \
  GIOP_Utils.cpp \
  Pluggable_Messaging.cpp \
  Pluggable_Messaging_Utils.cpp \
  Tagged_Profile.cpp \
  target_specification.cpp

Default_Resources = \
  default_client.cpp \
  default_resource.cpp \
  default_server.cpp \
  qt_resource.cpp \
  xt_resource.cpp

Interpretive_Marshaling = \
  Marshal.cpp \
  append.cpp \
  skip.cpp

IDL_Compiler = \
  Managed_Types.cpp

lib_LTLIBRARIES += libTAO.la

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

nobase_include_HEADERS += \
  Abstract_Servant_Base.h \
  Acceptor_Filter.h \
  Acceptor_Impl.h \
  Acceptor_Registry.h \
  Adapter.h \
  Adapter_Factory.h \
  Adapter_Registry.h \
  Allocator.h \
  Any.h \
  AnySeqC.h \
  AnySeqS.h \
  Any_Array_Impl_T.h \
  Any_Basic_Impl.h \
  Any_Basic_Impl_T.h \
  Any_Dual_Impl_T.h \
  Any_Impl.h \
  Any_Impl_T.h \
  Any_Special_Impl_T.h \
  Any_SystemException.h \
  Any_Unknown_IDL_Type.h \
  Arg_Traits_T.h \
  Argument.h \
  Array_VarOut_T.h \
  Asynch_Queued_Message.h \
  Asynch_Reply_Dispatcher_Base.h \
  Auto_Functor.h \
  BD_String_Argument_T.h \
  BD_String_SArgument_T.h \
  Base_Transport_Property.h \
  Basic_Argument_T.h \
  Basic_Arguments.h \
  Basic_SArgument_T.h \
  Basic_SArguments.h \
  Basic_Types.h \
  BiDir_Adapter.h \
  Bind_Dispatcher_Guard.h \
  Block_Flushing_Strategy.h \
  Blocked_Connect_Strategy.h \
  BooleanSeqC.h \
  BooleanSeqS.h \
  BoundsC.h \
  Buffer_Allocator_T.h \
  Buffering_Constraint_Policy.h \
  CDR.h \
  CDR_Encaps_Codec.h \
  CONV_FRAMEC.h \
  CONV_FRAMES.h \
  CONV_FRAMES_T.h \
  CORBALOC_Parser.h \
  CORBANAME_Parser.h \
  CORBA_String.h \
  CORBA_methods.h \
  Cache_Entries.h \
  CharSeqC.h \
  CharSeqS.h \
  Cleanup_Func_Registry.h \
  ClientRequestInfo.h \
  ClientRequestInfo_i.h \
  ClientRequestInterceptor_Adapter.h \
  Client_Strategy_Factory.h \
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
  CurrentC.h \
  DLL_Parser.h \
  Default_Collocation_Resolver.h \
  Default_Endpoint_Selector_Factory.h \
  Default_Protocols_Hooks.h \
  Default_Stub_Factory.h \
  Default_Thread_Lane_Resources_Manager.h \
  DomainC.h \
  DoubleSeqC.h \
  DoubleSeqS.h \
  DynamicC.h \
  Dynamic_Adapter.h \
  Dynamic_ParameterC.h \
  Encodable.h \
  Endpoint.h \
  Endpoint_Selector_Factory.h \
  Environment.h \
  Exception.h \
  Exception_Data.h \
  Exclusive_TMS.h \
  FILE_Parser.h \
  Fault_Tolerance_Service.h \
  Fixed_Array_Argument_T.h \
  Fixed_Array_SArgument_T.h \
  Fixed_Size_Argument_T.h \
  Fixed_Size_SArgument_T.h \
  FloatSeqC.h \
  FloatSeqS.h \
  Flushing_Strategy.h \
  GIOPC.h \
  GIOPS.h \
  GIOPS_T.h \
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
  IFR_Client_Adapter.h \
  IIOPC.h \
  IIOP_Acceptor.h \
  IIOP_Connection_Handler.h \
  IIOP_Connector.h \
  IIOP_Endpoint.h \
  IIOP_EndpointsC.h \
  IIOP_Factory.h \
  IIOP_Lite_Factory.h \
  IIOP_Profile.h \
  IIOP_Transport.h \
  IOPC.h \
  IOPS.h \
  IOPS_T.h \
  IOP_CodecC.h \
  IOP_IORC.h \
  IORInterceptor_Adapter.h \
  IORInterceptor_Adapter_Factory.h \
  IOR_Parser.h \
  Incoming_Message_Queue.h \
  Interceptor_List.h \
  Invocation_Adapter.h \
  Invocation_Base.h \
  Invocation_Endpoint_Selectors.h \
  Invocation_Utils.h \
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
  LRU_Connection_Purging_Strategy.h \
  Leader_Follower.h \
  Leader_Follower_Flushing_Strategy.h \
  LocalObject.h \
  LocateRequest_Invocation.h \
  LocateRequest_Invocation_Adapter.h \
  LongDoubleSeqC.h \
  LongDoubleSeqS.h \
  LongLongSeqC.h \
  LongLongSeqS.h \
  LongSeqC.h \
  LongSeqS.h \
  MCAST_Parser.h \
  MProfile.h \
  Managed_Types.h \
  Marshal.h \
  Messaging_PolicyValueC.h \
  Messaging_SyncScopeC.h \
  Muxed_TMS.h \
  NVList.h \
  New_Leader_Generator.h \
  OBV_Constants.h \
  ORB.h \
  ORBInitInfo.h \
  ORBInitializer_Registry.h \
  ORB_Constants.h \
  ORB_Core.h \
  ORB_Core_Auto_Ptr.h \
  ORB_Table.h \
  orb_typesC.h \
  orb_typesS.h \
  Object.h \
  ObjectIdListC.h \
  ObjectKey_Table.h \
  Object_Argument_T.h \
  Object_KeyC.h \
  Object_Loader.h \
  Object_Proxy_Broker.h \
  Object_Proxy_Impl.h \
  Object_Ref_Table.h \
  Object_SArgument_T.h \
  Object_T.h \
  Objref_VarOut_T.h \
  OctetSeqC.h \
  OctetSeqS.h \
  PICurrent.h \
  PICurrent_Copy_Callback.h \
  PICurrent_ORBInitializer.h \
  PI_ForwardC.h \
  ParameterModeC.h \
  ParameterModeS.h \
  Parser_Registry.h \
  Pluggable_Messaging.h \
  Pluggable_Messaging_Utils.h \
  PolicyC.h \
  PolicyFactory_Registry.h \
  PolicyS.h \
  Policy_ForwardC.h \
  Policy_Manager.h \
  Policy_Set.h \
  Policy_Validator.h \
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
  Queued_Message.h \
  Reactive_Connect_Strategy.h \
  Reactive_Flushing_Strategy.h \
  Refcounted_ObjectKey.h \
  Remote_Invocation.h \
  Remote_Object_Proxy_Broker.h \
  Reply_Dispatcher.h \
  RequestInfo_Util.h \
  Request_Dispatcher.h \
  Resource_Factory.h \
  Resume_Handle.h \
  Seq_Out_T.h \
  Seq_Var_T.h \
  Sequence.h \
  Sequence_T.h \
  Server_Strategy_Factory.h \
  Service_Callbacks.h \
  Service_Context.h \
  ServicesC.h \
  ServicesS.h \
  ServicesS_T.h \
  Services_Activate.h \
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
  SystemException.h \
  TAOC.h \
  TAOS.h \
  TAOS_T.h \
  TAO_Export.h \
  TAO_Internal.h \
  TAO_Server_Request.h \
  TAO_Singleton.h \
  TAO_Singleton_Manager.h \
  TC_Constants_Forward.h \
  TSS_Resources.h \
  Tagged_Components.h \
  Tagged_Profile.h \
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
  TypeCodeFactory_Adapter.h \
  Typecode.h \
  Typecode_typesS.h \
  Typecode_typesC.h \
  Typecode_Constants.h \
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
  UserException.h \
  Valuetype_Adapter.h \
  VarOut_T.h \
  Var_Array_Argument_T.h \
  Var_Array_SArgument_T.h \
  Var_Size_Argument_T.h \
  Var_Size_SArgument_T.h \
  Version.h \
  WCharSeqC.h \
  WCharSeqS.h \
  WStringSeqC.h \
  WStringSeqS.h \
  Wait_On_LF_No_Upcall.h \
  Wait_On_Leader_Follower.h \
  Wait_On_Reactor.h \
  Wait_On_Read.h \
  Wait_Strategy.h \
  WrongTransactionC.h \
  corba.h \
  corbafwd.h \
  debug.h \
  default_client.h \
  default_environment.h \
  default_ports.h \
  default_resource.h \
  default_server.h \
  objectid.h \
  operation_details.h \
  orbconf.h \
  params.h \
  qt_resource.h \
  target_specification.h \
  varbase.h \
  xt_resource.h \
  Acceptor_Registry.i \
  Any.i \
  AnySeqC.inl \
  Any_Array_Impl_T.inl \
  Any_Basic_Impl_T.inl \
  Any_Dual_Impl_T.inl \
  Any_Impl_T.inl \
  Any_Special_Impl_T.inl \
  Array_VarOut_T.inl \
  Asynch_Reply_Dispatcher_Base.i \
  BD_String_Argument_T.inl \
  BD_String_SArgument_T.inl \
  Base_Transport_Property.inl \
  Basic_Argument_T.inl \
  Basic_SArgument_T.inl \
  Bind_Dispatcher_Guard.i \
  BooleanSeqC.inl \
  BoundsC.inl \
  Buffering_Constraint_Policy.i \
  CDR.i \
  CONV_FRAMEC.inl \
  CONV_FRAMES.inl \
  CONV_FRAMES_T.inl \
  CORBALOC_Parser.i \
  CORBANAME_Parser.i \
  CORBA_String.inl \
  Cache_Entries.inl \
  CharSeqC.inl \
  Cleanup_Func_Registry.inl \
  ClientRequestInfo.inl \
  ClientRequestInfo_i.inl \
  ClientRequestInterceptor_Adapter.inl \
  Condition.inl \
  Connection_Handler.inl \
  Connection_Purging_Strategy.inl \
  Connector_Registry.i \
  CurrentC.inl \
  DLL_Parser.i \
  DomainC.inl \
  DoubleSeqC.inl \
  DynamicC.inl \
  Dynamic_ParameterC.inl \
  Endpoint.i \
  Environment.i \
  Exception.i \
  FILE_Parser.i \
  Fault_Tolerance_Service.i \
  Fixed_Array_Argument_T.inl \
  Fixed_Array_SArgument_T.inl \
  Fixed_Size_Argument_T.inl \
  Fixed_Size_SArgument_T.inl \
  FloatSeqC.inl \
  GIOPC.inl \
  GIOPS.inl \
  GIOPS_T.inl \
  GIOP_Message_Generator_Parser_Impl.inl \
  GIOP_Message_Locate_Header.i \
  GIOP_Message_State.inl \
  GIOP_Message_Version.inl \
  IIOPC.inl \
  IIOP_Acceptor.i \
  IIOP_Endpoint.i \
  IIOP_EndpointsC.inl \
  IOPC.i \
  IOPS.i \
  IOPS_T.i \
  IOP_CodecC.i \
  IOP_IORC.i \
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
  LF_Strategy_Complete.inl \
  Leader_Follower.i \
  LocalObject.i \
  LongDoubleSeqC.inl \
  LongLongSeqC.inl \
  LongSeqC.inl \
  MCAST_Parser.i \
  MProfile.i \
  Managed_Types.i \
  Marshal.i \
  Messaging_PolicyValueC.inl \
  Messaging_SyncScopeC.inl \
  NVList.i \
  ORB.i \
  orb_typesC.inl \
  ORBInitInfo.inl \
  ORB_Core.i \
  ORB_Core_Auto_Ptr.inl \
  ORB_Table.inl \
  Object.i \
  ObjectIdListC.inl \
  Object_Argument_T.inl \
  Object_KeyC.inl \
  Object_SArgument_T.inl \
  OctetSeqC.inl \
  PICurrent.inl \
  PI_ForwardC.inl \
  ParameterModeC.inl \
  Parser_Registry.i \
  Pluggable_Messaging_Utils.i \
  PolicyC.inl \
  Policy_ForwardC.i \
  Policy_Manager.i \
  Policy_Set.i \
  PollableC.inl \
  PollableS.inl \
  PollableS_T.inl \
  PortableInterceptorC.inl \
  Principal.i \
  Profile.i \
  Profile_Transport_Resolver.inl \
  Pseudo_VarOut_T.inl \
  Queued_Message.inl \
  Refcounted_ObjectKey.inl \
  Reply_Dispatcher.i \
  Resume_Handle.inl \
  Seq_Out_T.inl \
  Seq_Var_T.inl \
  Sequence.i \
  Sequence_T.i \
  Service_Context.inl \
  ServicesC.inl \
  ServicesS.inl \
  ServicesS_T.inl \
  ShortSeqC.inl \
  Special_Basic_Argument_T.inl \
  Special_Basic_SArgument_T.inl \
  StringSeqC.inl \
  Stub.i \
  Synch_Invocation.inl \
  Synch_Refcountable.inl \
  SystemException.inl \
  TAOC.inl \
  TAOS.inl \
  TAOS_T.inl \
  TAO_Server_Request.i \
  TAO_Singleton.inl \
  TAO_Singleton_Manager.inl \
  Tagged_Components.i \
  Tagged_Profile.i \
  TimeBaseC.inl \
  TimeBaseS.inl \
  TimeBaseS_T.inl \
  Transport.inl \
  Transport_Acceptor.inl \
  Transport_Cache_Manager.inl \
  Transport_Connector.inl \
  Transport_Descriptor_Interface.inl \
  Typecode.i \
  Typecode_typesC.inl \
  UB_String_Argument_T.inl \
  UB_String_SArgument_T.inl \
  ULongLongSeqC.inl \
  ULongSeqC.inl \
  UShortSeqC.inl \
  UserException.inl \
  VarOut_T.inl \
  Var_Array_Argument_T.inl \
  Var_Array_SArgument_T.inl \
  Var_Size_Argument_T.inl \
  Var_Size_SArgument_T.inl \
  WCharSeqC.inl \
  WStringSeqC.inl \
  Wait_Strategy.inl \
  WrongTransactionC.inl \
  operation_details.i \
  params.i \
  qt_resource.i \
  target_specification.i \
  xt_resource.i \
  Acceptor_Impl.cpp \
  Any_Array_Impl_T.cpp \
  Any_Basic_Impl_T.cpp \
  Any_Dual_Impl_T.cpp \
  Any_Impl_T.cpp \
  Any_Special_Impl_T.cpp \
  Array_VarOut_T.cpp \
  BD_String_Argument_T.cpp \
  BD_String_SArgument_T.cpp \
  Basic_Argument_T.cpp \
  Basic_SArgument_T.cpp \
  Buffer_Allocator_T.cpp \
  CONV_FRAMES_T.cpp \
  Codeset_Translator_Factory_T.cpp \
  Condition.cpp \
  Connector_Impl.cpp \
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
  TAO_Singleton.cpp \
  TimeBaseS_T.cpp \
  UB_String_Argument_T.cpp \
  UB_String_SArgument_T.cpp \
  VarOut_T.cpp \
  Var_Array_Argument_T.cpp \
  Var_Array_SArgument_T.cpp \
  Var_Size_Argument_T.cpp \
  Var_Size_SArgument_T.cpp \
  orb.idl \
  orb_types.pidl \
  Typecode_types.pidl \
  AnySeq.pidl \
  BooleanSeq.pidl \
  Bounds.pidl \
  CONV_FRAME.pidl \
  CharSeq.pidl \
  Current.pidl \
  Domain.pidl \
  DoubleSeq.pidl \
  Dynamic.pidl \
  Dynamic_Parameter.pidl \
  FloatSeq.pidl \
  GIOP.pidl \
  IIOP.pidl \
  IIOP_Endpoints.pidl \
  IOP.pidl \
  IOP_Codec.pidl \
  IOP_IOR.pidl \
  InterfaceDef.pidl \
  InvalidName.pidl \
  LongDoubleSeq.pidl \
  LongLongSeq.pidl \
  LongSeq.pidl \
  Messaging_PolicyValue.pidl \
  Messaging_SyncScope.pidl \
  ObjectIdList.pidl \
  Object_Key.pidl \
  OctetSeq.pidl \
  ParameterMode.pidl \
  PI_Forward.pidl \
  Policy.pidl \
  Policy_Forward.pidl \
  Pollable.pidl \
  PortableInterceptor.pidl \
  Services.pidl \
  ShortSeq.pidl \
  StringSeq.pidl \
  TAO.pidl \
  TimeBase.pidl \
  ULongLongSeq.pidl \
  ULongSeq.pidl \
  UShortSeq.pidl \
  WCharSeq.pidl \
  WStringSeq.pidl \
  WrongTransaction.pidl

pkgconfigdata_DATA = TAO.pc

TAO.pc: ${top_builddir}/config.status ${srcdir}/TAO.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/TAO.pc.in

EXTRA_DIST = \
  tao.rc

## Makefile.BiDir_GIOP.am

lib_LTLIBRARIES += libTAO_BiDirGIOP.la

libTAO_BiDirGIOP_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_BIDIRGIOP_BUILD_DLL

libTAO_BiDirGIOP_la_SOURCES = \
  BiDir_GIOP/BiDirGIOP.cpp \
  BiDir_GIOP/BiDirPolicyC.cpp \
  BiDir_GIOP/BiDirPolicy_Validator.cpp \
  BiDir_GIOP/BiDir_ORBInitializer.cpp \
  BiDir_GIOP/BiDir_PolicyFactory.cpp \
  BiDir_GIOP/BiDir_Policy_i.cpp

libTAO_BiDirGIOP_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_BiDirGIOP_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  BiDir_GIOP/BiDirGIOP.h \
  BiDir_GIOP/BiDirPolicyC.h \
  BiDir_GIOP/BiDirPolicy_Validator.h \
  BiDir_GIOP/BiDir_ORBInitializer.h \
  BiDir_GIOP/BiDir_PolicyFactory.h \
  BiDir_GIOP/BiDir_Policy_i.h \
  BiDir_GIOP/bidirgiop_export.h \
  BiDir_GIOP/BiDirPolicyC.inl \
  BiDir_GIOP/BiDirPolicy.pidl

pkgconfigdata_DATA += TAO_BiDirGIOP.pc

TAO_BiDirGIOP.pc: ${top_builddir}/config.status ${srcdir}/BiDir_GIOP/TAO_BiDirGIOP.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/BiDir_GIOP/TAO_BiDirGIOP.pc.in

EXTRA_DIST += \
  BiDir_GIOP/TAO_BiDir_GIOP.rc

## Makefile.PortableServer.am

lib_LTLIBRARIES += libTAO_PortableServer.la

libTAO_PortableServer_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_PORTABLESERVER_BUILD_DLL

libTAO_PortableServer_la_SOURCES = \
  PortableServer/Active_Object_Map.cpp \
  PortableServer/Collocated_Object_Proxy_Broker.cpp \
  PortableServer/Default_Acceptor_Filter.cpp \
  PortableServer/Default_Policy_Validator.cpp \
  PortableServer/Default_Servant_Dispatcher.cpp \
  PortableServer/ImplRepoA.cpp \
  PortableServer/ImplRepoC.cpp \
  PortableServer/ImplRepoS.cpp \
  PortableServer/ServerObjectA.cpp \
  PortableServer/ServerObjectC.cpp \
  PortableServer/ServerObjectS.cpp \
  PortableServer/ImplRepo_i.cpp \
  PortableServer/Key_Adapters.cpp \
  PortableServer/ORB_Manager.cpp \
  PortableServer/ORT_Adapter.cpp \
  PortableServer/ORT_Adapter_Factory.cpp \
  PortableServer/Object_Adapter.cpp \
  PortableServer/Operation_Table.cpp \
  PortableServer/PICurrent_Guard.cpp \
  PortableServer/POA.cpp \
  PortableServer/POAManager.cpp \
  PortableServer/POA_Cached_Policies.cpp \
  PortableServer/POA_Policies.cpp \
  PortableServer/POA_Policy_Set.cpp \
  PortableServer/PolicyS.cpp \
  PortableServer/PortableGroup_Hooks.cpp \
  PortableServer/PortableServer.cpp \
  PortableServer/PortableServerA.cpp \
  PortableServer/PortableServerC.cpp \
  PortableServer/PortableServer_ORBInitializer.cpp \
  PortableServer/PortableServer_PolicyFactory.cpp \
  PortableServer/Servant_Base.cpp \
  PortableServer/Servant_Dispatcher.cpp \
  PortableServer/ServerInterceptorAdapter.cpp \
  PortableServer/ServerRequestInfo.cpp

libTAO_PortableServer_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_PortableServer_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  PortableServer/Active_Object_Map.h \
  PortableServer/Collocated_Object_Proxy_Broker.h \
  PortableServer/Default_Acceptor_Filter.h \
  PortableServer/Default_Policy_Validator.h \
  PortableServer/Default_Servant_Dispatcher.h \
  PortableServer/ImplRepoC.h \
  PortableServer/ImplRepoS.h \
  PortableServer/ServerObjectC.h \
  PortableServer/ServerObjectS.h \
  PortableServer/ImplRepo_i.h \
  PortableServer/Key_Adapters.h \
  PortableServer/ORB_Manager.h \
  PortableServer/ORT_Adapter.h \
  PortableServer/ORT_Adapter_Factory.h \
  PortableServer/Object_Adapter.h \
  PortableServer/Operation_Table.h \
  PortableServer/PICurrent_Guard.h \
  PortableServer/POA.h \
  PortableServer/POAManager.h \
  PortableServer/POA_Cached_Policies.h \
  PortableServer/POA_Policies.h \
  PortableServer/POA_Policy_Set.h \
  PortableServer/PolicyS.h \
  PortableServer/PolicyS_T.h \
  PortableServer/PortableGroup_Hooks.h \
  PortableServer/PortableServer.h \
  PortableServer/PortableServerC.h \
  PortableServer/PortableServerS.h \
  PortableServer/PortableServer_includeC.h \
  PortableServer/PortableServer_includeS.h \
  PortableServer/PortableServer_ORBInitializer.h \
  PortableServer/PortableServer_PolicyFactory.h \
  PortableServer/Servant_Base.h \
  PortableServer/Servant_Dispatcher.h \
  PortableServer/ServerInterceptorAdapter.h \
  PortableServer/ServerRequestInfo.h \
  PortableServer/Upcall_Wrapper.h \
  PortableServer/poa_macros.h \
  PortableServer/portableserver_export.h \
  PortableServer/Active_Object_Map.i \
  PortableServer/ImplRepoC.inl \
  PortableServer/ImplRepoS.inl \
  PortableServer/ServerObjectC.inl \
  PortableServer/ServerObjectS.inl \
  PortableServer/Object_Adapter.i \
  PortableServer/POA.i \
  PortableServer/POAManager.i \
  PortableServer/POA_Cached_Policies.i \
  PortableServer/POA_Policy_Set.i \
  PortableServer/PolicyS.inl \
  PortableServer/PolicyS_T.inl \
  PortableServer/PortableServerC.inl \
  PortableServer/Servant_Base.i \
  PortableServer/ServerInterceptorAdapter.inl \
  PortableServer/ServerRequestInfo.inl \
  PortableServer/PolicyS_T.cpp \
  PortableServer/ImplRepo.pidl \
  PortableServer/ServerObject.pidl \
  PortableServer/PortableServer.pidl \
  PortableServer/PortableServer_include.pidl

pkgconfigdata_DATA += TAO_PortableServer.pc

TAO_PortableServer.pc: ${top_builddir}/config.status ${srcdir}/PortableServer/TAO_PortableServer.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/PortableServer/TAO_PortableServer.pc.in

EXTRA_DIST += \
  PortableServer/TAO_PortableServer.rc

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

nobase_include_HEADERS += \
  IFR_Client/IFR_BaseC.h \
  IFR_Client/IFR_BaseS.h \
  IFR_Client/IFR_BasicC.h \
  IFR_Client/IFR_Client_Adapter_Impl.h \
  IFR_Client/IFR_ComponentsC.h \
  IFR_Client/IFR_ExtendedC.h \
  IFR_Client/ifr_client_export.h \
  IFR_Client/IFR_BaseC.inl \
  IFR_Client/IFR_BasicC.inl \
  IFR_Client/IFR_ComponentsC.inl \
  IFR_Client/IFR_ExtendedC.inl \
  IFR_Client/IFR_Base.pidl \
  IFR_Client/IFR_Basic.pidl \
  IFR_Client/IFR_Components.pidl \
  IFR_Client/IFR_Extended.pidl

pkgconfigdata_DATA += TAO_IFR_Client.pc

TAO_IFR_Client.pc: ${top_builddir}/config.status ${srcdir}/IFR_Client/TAO_IFR_Client.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/IFR_Client/TAO_IFR_Client.pc.in

EXTRA_DIST += \
  IFR_Client/TAO_IFR_Client.rc

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
  libTAO_IFR_Client.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Domain/DomainS.h \
  Domain/DomainS_T.h \
  Domain/domain_export.h \
  Domain/DomainS.i \
  Domain/DomainS_T.i \
  Domain/DomainS_T.cpp

pkgconfigdata_DATA += TAO_Domain.pc

TAO_Domain.pc: ${top_builddir}/config.status ${srcdir}/Domain/TAO_Domain.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/Domain/TAO_Domain.pc.in

EXTRA_DIST += \
  Domain/TAO_Domain.rc

## Makefile.DynamicAny.am

lib_LTLIBRARIES += libTAO_DynamicAny.la

libTAO_DynamicAny_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_DYNAMICANY_BUILD_DLL

libTAO_DynamicAny_la_SOURCES = \
  DynamicAny/DynAnyFactory.cpp \
  DynamicAny/DynAny_i.cpp \
  DynamicAny/DynArray_i.cpp \
  DynamicAny/DynCommon.cpp \
  DynamicAny/DynEnum_i.cpp \
  DynamicAny/DynSequence_i.cpp \
  DynamicAny/DynStruct_i.cpp \
  DynamicAny/DynUnion_i.cpp \
  DynamicAny/DynamicAny.cpp \
  DynamicAny/DynamicAnyC.cpp

libTAO_DynamicAny_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DynamicAny_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  DynamicAny/DynAnyFactory.h \
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
  DynamicAny/DynamicAnyC.i \
  DynamicAny/DynamicAny.pidl

pkgconfigdata_DATA += TAO_DynamicAny.pc

TAO_DynamicAny.pc: ${top_builddir}/config.status ${srcdir}/DynamicAny/TAO_DynamicAny.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/DynamicAny/TAO_DynamicAny.pc.in

EXTRA_DIST += \
  DynamicAny/TAO_DynamicAny.rc

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
  Valuetype/ValueBase.cpp \
  Valuetype/ValueFactory.cpp \
  Valuetype/ValueFactory_Map.cpp \
  Valuetype/Valuetype_Adapter_Impl.cpp

libTAO_Valuetype_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Valuetype_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Valuetype/AbstractBase.h \
  Valuetype/AbstractBase_Invocation_Adapter.h \
  Valuetype/AbstractBase_T.h \
  Valuetype/Sequence_T.h \
  Valuetype/ValueBase.h \
  Valuetype/ValueFactory.h \
  Valuetype/ValueFactory_Map.h \
  Valuetype/Value_CORBA_methods.h \
  Valuetype/Value_VarOut_T.h \
  Valuetype/Valuetype_Adapter_Impl.h \
  Valuetype/valuetype_export.h \
  Valuetype/AbstractBase.inl \
  Valuetype/Sequence_T.inl \
  Valuetype/ValueBase.inl \
  Valuetype/AbstractBase_T.cpp \
  Valuetype/Sequence_T.cpp \
  Valuetype/Value_VarOut_T.cpp

pkgconfigdata_DATA += TAO_Valuetype.pc

TAO_Valuetype.pc: ${top_builddir}/config.status ${srcdir}/Valuetype/TAO_Valuetype.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/Valuetype/TAO_Valuetype.pc.in

EXTRA_DIST += \
  Valuetype/TAO_Valuetype.rc

## Makefile.Messaging.am

lib_LTLIBRARIES += libTAO_Messaging.la

libTAO_Messaging_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_MESSAGING_BUILD_DLL

libTAO_Messaging_la_SOURCES = \
  Messaging/AMH_Response_Handler.cpp \
  Messaging/Asynch_Invocation.cpp \
  Messaging/Asynch_Invocation_Adapter.cpp \
  Messaging/Asynch_Reply_Dispatcher.cpp \
  Messaging/Asynch_Timeout_Handler.cpp \
  Messaging/Connection_Timeout_Policy_i.cpp \
  Messaging/Messaging.cpp \
  Messaging/MessagingC.cpp \
  Messaging/MessagingS.cpp \
  Messaging/Messaging_No_ImplC.cpp \
  Messaging/Messaging_ORBInitializer.cpp \
  Messaging/Messaging_PolicyFactory.cpp \
  Messaging/Messaging_Policy_i.cpp \
  Messaging/Messaging_RT_PolicyC.cpp \
  Messaging/Messaging_SyncScope_PolicyC.cpp \
  Messaging/TAO_ExtC.cpp

libTAO_Messaging_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Messaging_la_LIBADD = \
  libTAO_Valuetype.la \
  libTAO_PortableServer.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Messaging/AMH_Response_Handler.h \
  Messaging/Asynch_Invocation.h \
  Messaging/Asynch_Invocation_Adapter.h \
  Messaging/Asynch_Reply_Dispatcher.h \
  Messaging/Asynch_Timeout_Handler.h \
  Messaging/Connection_Timeout_Policy_i.h \
  Messaging/Messaging.h \
  Messaging/MessagingC.h \
  Messaging/MessagingS.h \
  Messaging/MessagingS_T.h \
  Messaging/Messaging_No_ImplC.h \
  Messaging/Messaging_ORBInitializer.h \
  Messaging/Messaging_PolicyFactory.h \
  Messaging/Messaging_Policy_i.h \
  Messaging/Messaging_RT_PolicyC.h \
  Messaging/Messaging_SyncScope_PolicyC.h \
  Messaging/TAO_ExtC.h \
  Messaging/messaging_export.h \
  Messaging/Asynch_Invocation_Adapter.inl \
  Messaging/MessagingC.i \
  Messaging/MessagingS.i \
  Messaging/MessagingS_T.i \
  Messaging/Messaging_No_ImplC.i \
  Messaging/Messaging_Policy_i.i \
  Messaging/Messaging_RT_PolicyC.i \
  Messaging/Messaging_SyncScope_PolicyC.i \
  Messaging/TAO_ExtC.i \
  Messaging/MessagingS_T.cpp \
  Messaging/Messaging.pidl \
  Messaging/Messaging_No_Impl.pidl \
  Messaging/Messaging_RT_Policy.pidl \
  Messaging/Messaging_SyncScope_Policy.pidl \
  Messaging/TAO_Ext.pidl

pkgconfigdata_DATA += TAO_Messaging.pc

TAO_Messaging.pc: ${top_builddir}/config.status ${srcdir}/Messaging/TAO_Messaging.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/Messaging/TAO_Messaging.pc.in

EXTRA_DIST += \
  Messaging/TAO_Messaging.rc

## Makefile.DynamicInterface.am

if !BUILD_MINIMUM_CORBA

lib_LTLIBRARIES += libTAO_DynamicInterface.la

libTAO_DynamicInterface_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_DYNAMICINTERFACE_BUILD_DLL

libTAO_DynamicInterface_la_SOURCES = \
  DynamicInterface/Context.cpp \
  DynamicInterface/DII_Arguments.cpp \
  DynamicInterface/DII_Invocation.cpp \
  DynamicInterface/DII_Invocation_Adapter.cpp \
  DynamicInterface/DII_Reply_Dispatcher.cpp \
  DynamicInterface/Dynamic_Adapter_Impl.cpp \
  DynamicInterface/Dynamic_Implementation.cpp \
  DynamicInterface/ExceptionList.cpp \
  DynamicInterface/Request.cpp \
  DynamicInterface/Server_Request.cpp \
  DynamicInterface/Unknown_User_Exception.cpp

libTAO_DynamicInterface_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_DynamicInterface_la_LIBADD = \
  libTAO_Messaging.la \
  libTAO_PortableServer.la \
  libTAO_Valuetype.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
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

pkgconfigdata_DATA += TAO_DynamicInterface.pc

TAO_DynamicInterface.pc: ${top_builddir}/config.status ${srcdir}/DynamicInterface/TAO_DynamicInterface.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/DynamicInterface/TAO_DynamicInterface.pc.in

endif !BUILD_MINIMUM_CORBA

EXTRA_DIST += \
  DynamicInterface/TAO_DynamicInterface.rc

## Makefile.ObjRefTemplate.am

lib_LTLIBRARIES += libTAO_ObjRefTemplate.la

libTAO_ObjRefTemplate_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_ORT_BUILD_DLL

libTAO_ObjRefTemplate_la_SOURCES = \
  ObjRefTemplate/Default_ORTC.cpp \
  ObjRefTemplate/ORT_Adapter_Factory_Impl.cpp \
  ObjRefTemplate/ORT_Adapter_Impl.cpp \
  ObjRefTemplate/ObjectReferenceTemplateC.cpp \
  ObjRefTemplate/ObjectReferenceTemplate_i.cpp

libTAO_ObjRefTemplate_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_ObjRefTemplate_la_LIBADD = \
  libTAO_PortableServer.la \
  libTAO_Valuetype.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  ObjRefTemplate/Default_ORTC.h \
  ObjRefTemplate/ORT_Adapter_Factory_Impl.h \
  ObjRefTemplate/ORT_Adapter_Impl.h \
  ObjRefTemplate/ObjectReferenceTemplateC.h \
  ObjRefTemplate/ObjectReferenceTemplate_i.h \
  ObjRefTemplate/ort_export.h \
  ObjRefTemplate/Default_ORTC.inl \
  ObjRefTemplate/ObjectReferenceTemplateC.inl \
  ObjRefTemplate/Default_ORT.pidl \
  ObjRefTemplate/ObjectReferenceTemplate.pidl

pkgconfigdata_DATA += TAO_ObjRefTemplate.pc

TAO_ObjRefTemplate.pc: ${top_builddir}/config.status ${srcdir}/ObjRefTemplate/TAO_ObjRefTemplate.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/ObjRefTemplate/TAO_ObjRefTemplate.pc.in

EXTRA_DIST += \
  ObjRefTemplate/TAO_ObjRefTemplate.rc

## Makefile.IORInterceptor.am

lib_LTLIBRARIES += libTAO_IORInterceptor.la

libTAO_IORInterceptor_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_IORINTERCEPTOR_BUILD_DLL

libTAO_IORInterceptor_la_SOURCES = \
  IORInterceptor/IORInfo.cpp \
  IORInterceptor/IORInfoC.cpp \
  IORInterceptor/IORInterceptorC.cpp \
  IORInterceptor/IORInterceptor_Adapter_Factory_Impl.cpp \
  IORInterceptor/IORInterceptor_Adapter_Impl.cpp \
  IORInterceptor/IORInterceptor_List.cpp

libTAO_IORInterceptor_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IORInterceptor_la_LIBADD = \
  libTAO_ObjRefTemplate.la \
  libTAO_Valuetype.la \
  libTAO_PortableServer.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  IORInterceptor/IORInfo.h \
  IORInterceptor/IORInfoC.h \
  IORInterceptor/IORInterceptorC.h \
  IORInterceptor/IORInterceptor_Adapter_Factory_Impl.h \
  IORInterceptor/IORInterceptor_Adapter_Impl.h \
  IORInterceptor/IORInterceptor_List.h \
  IORInterceptor/iorinterceptor_export.h \
  IORInterceptor/IORInfo.inl \
  IORInterceptor/IORInfoC.inl \
  IORInterceptor/IORInterceptorC.inl \
  IORInterceptor/IORInterceptor_List.inl \
  IORInterceptor/IORInfo.pidl \
  IORInterceptor/IORInterceptor.pidl

pkgconfigdata_DATA += TAO_IORInterceptor.pc

TAO_IORInterceptor.pc: ${top_builddir}/config.status ${srcdir}/IORInterceptor/TAO_IORInterceptor.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/IORInterceptor/TAO_IORInterceptor.pc.in

EXTRA_DIST += \
  IORInterceptor/TAO_IORInterceptor.rc

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

nobase_include_HEADERS += \
  IORManipulation/IORC.h \
  IORManipulation/IORManip_Loader.h \
  IORManipulation/IORManipulation.h \
  IORManipulation/ior_manip_export.h \
  IORManipulation/IORC.i \
  IORManipulation/IOR.pidl

pkgconfigdata_DATA += TAO_IORManip.pc

TAO_IORManip.pc: ${top_builddir}/config.status ${srcdir}/IORManipulation/TAO_IORManip.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/IORManipulation/TAO_IORManip.pc.in

EXTRA_DIST += \
  IORManipulation/TAO_IORManip.rc

## Makefile.IORTable.am

lib_LTLIBRARIES += libTAO_IORTable.la

libTAO_IORTable_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_IORTABLE_BUILD_DLL

libTAO_IORTable_la_SOURCES = \
  IORTable/IORTable.cpp \
  IORTable/IORTableC.cpp \
  IORTable/IOR_Table_Impl.cpp \
  IORTable/Table_Adapter.cpp

libTAO_IORTable_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_IORTable_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  IORTable/IORTable.h \
  IORTable/IORTableC.h \
  IORTable/IOR_Table_Impl.h \
  IORTable/Table_Adapter.h \
  IORTable/iortable_export.h \
  IORTable/IORTableC.i \
  IORTable/IORTable.pidl

pkgconfigdata_DATA += TAO_IORTable.pc

TAO_IORTable.pc: ${top_builddir}/config.status ${srcdir}/IORTable/TAO_IORTable.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/IORTable/TAO_IORTable.pc.in

EXTRA_DIST += \
  IORTable/TAO_IORTable.rc

## Makefile.RTCORBA.am

if BUILD_RT_CORBA

lib_LTLIBRARIES += libTAO_RTCORBA.la

libTAO_RTCORBA_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTCORBA_BUILD_DLL

libTAO_RTCORBA_la_SOURCES = \
  RTCORBA/Continuous_Priority_Mapping.cpp \
  RTCORBA/Direct_Priority_Mapping.cpp \
  RTCORBA/Linear_Network_Priority_Mapping.cpp \
  RTCORBA/Linear_Priority_Mapping.cpp \
  RTCORBA/Multi_Priority_Mapping.cpp \
  RTCORBA/Network_Priority_Mapping.cpp \
  RTCORBA/Network_Priority_Mapping_Manager.cpp \
  RTCORBA/Priority_Mapping.cpp \
  RTCORBA/Priority_Mapping_Manager.cpp \
  RTCORBA/RTCORBA.cpp \
  RTCORBA/RTCORBAC.cpp \
  RTCORBA/RT_Current.cpp \
  RTCORBA/RT_Endpoint_Selector_Factory.cpp \
  RTCORBA/RT_Endpoint_Utils.cpp \
  RTCORBA/RT_Invocation_Endpoint_Selectors.cpp \
  RTCORBA/RT_Mutex.cpp \
  RTCORBA/RT_ORB.cpp \
  RTCORBA/RT_ORBInitializer.cpp \
  RTCORBA/RT_ORB_Loader.cpp \
  RTCORBA/RT_PolicyFactory.cpp \
  RTCORBA/RT_Policy_i.cpp \
  RTCORBA/RT_Protocols_Hooks.cpp \
  RTCORBA/RT_Stub.cpp \
  RTCORBA/RT_Stub_Factory.cpp \
  RTCORBA/RT_Thread_Lane_Resources_Manager.cpp \
  RTCORBA/RT_Transport_Descriptor.cpp \
  RTCORBA/RT_Transport_Descriptor_Property.cpp \
  RTCORBA/Thread_Pool.cpp

libTAO_RTCORBA_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTCORBA_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  RTCORBA/Continuous_Priority_Mapping.h \
  RTCORBA/Direct_Priority_Mapping.h \
  RTCORBA/Linear_Network_Priority_Mapping.h \
  RTCORBA/Linear_Priority_Mapping.h \
  RTCORBA/Multi_Priority_Mapping.h \
  RTCORBA/Network_Priority_Mapping.h \
  RTCORBA/Network_Priority_Mapping_Manager.h \
  RTCORBA/Priority_Mapping.h \
  RTCORBA/Priority_Mapping_Manager.h \
  RTCORBA/RTCORBA.h \
  RTCORBA/RTCORBAC.h \
  RTCORBA/RTCORBA_includeC.h \
  RTCORBA/RTCORBA_includeS.h \
  RTCORBA/RT_Current.h \
  RTCORBA/RT_Endpoint_Selector_Factory.h \
  RTCORBA/RT_Endpoint_Utils.h \
  RTCORBA/RT_Invocation_Endpoint_Selectors.h \
  RTCORBA/RT_Mutex.h \
  RTCORBA/RT_ORB.h \
  RTCORBA/RT_ORBInitializer.h \
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
  RTCORBA/Network_Priority_Mapping_Manager.i \
  RTCORBA/Priority_Mapping_Manager.i \
  RTCORBA/RTCORBAC.inl \
  RTCORBA/RT_Transport_Descriptor.inl \
  RTCORBA/RT_Transport_Descriptor_Property.inl \
  RTCORBA/RTCORBA.pidl \
  RTCORBA/RTCORBA_include.pidl

pkgconfigdata_DATA += TAO_RTCORBA.pc

TAO_RTCORBA.pc: ${top_builddir}/config.status ${srcdir}/RTCORBA/TAO_RTCORBA.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/RTCORBA/TAO_RTCORBA.pc.in

endif BUILD_RT_CORBA

EXTRA_DIST += \
  RTCORBA/TAO_RTCORBA.rc

## Makefile.RTPortableServer.am

if BUILD_RT_CORBA

lib_LTLIBRARIES += libTAO_RTPortableServer.la

libTAO_RTPortableServer_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTPORTABLESERVER_BUILD_DLL

libTAO_RTPortableServer_la_SOURCES = \
  RTPortableServer/RTPortableServer.cpp \
  RTPortableServer/RTPortableServerA.cpp \
  RTPortableServer/RTPortableServerC.cpp \
  RTPortableServer/RT_Acceptor_Filters.cpp \
  RTPortableServer/RT_Collocation_Resolver.cpp \
  RTPortableServer/RT_Object_Adapter_Factory.cpp \
  RTPortableServer/RT_POA.cpp \
  RTPortableServer/RT_Policy_Validator.cpp \
  RTPortableServer/RT_Servant_Dispatcher.cpp

libTAO_RTPortableServer_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTPortableServer_la_LIBADD = \
  libTAO_PortableServer.la \
  libTAO_RTCORBA.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  RTPortableServer/RTPortableServer.h \
  RTPortableServer/RTPortableServerC.h \
  RTPortableServer/RT_Acceptor_Filters.h \
  RTPortableServer/RT_Collocation_Resolver.h \
  RTPortableServer/RT_Object_Adapter_Factory.h \
  RTPortableServer/RT_POA.h \
  RTPortableServer/RT_Policy_Validator.h \
  RTPortableServer/RT_Servant_Dispatcher.h \
  RTPortableServer/rtportableserver_export.h \
  RTPortableServer/RTPortableServerC.inl \
  RTPortableServer/RT_POA.i \
  RTPortableServer/RTPortableServer.pidl

pkgconfigdata_DATA += TAO_RTPortableServer.pc

TAO_RTPortableServer.pc: ${top_builddir}/config.status ${srcdir}/RTPortableServer/TAO_RTPortableServer.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/RTPortableServer/TAO_RTPortableServer.pc.in

endif BUILD_RT_CORBA

EXTRA_DIST += \
  RTPortableServer/TAO_RTPortableServer.rc

## Makefile.RTScheduler.am

if BUILD_RT_CORBA
if BUILD_CORBA_MESSAGING

lib_LTLIBRARIES += libTAO_RTScheduler.la

libTAO_RTScheduler_la_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -DTAO_RTSCHEDULER_BUILD_DLL

libTAO_RTScheduler_la_SOURCES = \
  RTScheduling/Current.cpp \
  RTScheduling/Distributable_Thread.cpp \
  RTScheduling/RTScheduler.cpp \
  RTScheduling/RTSchedulerC.cpp \
  RTScheduling/RTScheduler_Initializer.cpp \
  RTScheduling/RTScheduler_Loader.cpp \
  RTScheduling/RTScheduler_Manager.cpp \
  RTScheduling/Request_Interceptor.cpp

libTAO_RTScheduler_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_RTScheduler_la_LIBADD = \
  libTAO_PortableServer.la \
  libTAO_RTCORBA.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
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
  RTScheduling/RTScheduler_Manager.i \
  RTScheduling/RTScheduler.pidl \
  RTScheduling/RTScheduler_include.pidl

pkgconfigdata_DATA += TAO_RTScheduler.pc

TAO_RTScheduler.pc: ${top_builddir}/config.status ${srcdir}/RTScheduling/TAO_RTScheduler.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/RTScheduling/TAO_RTScheduler.pc.in

endif BUILD_CORBA_MESSAGING
endif BUILD_RT_CORBA

EXTRA_DIST += \
  RTScheduling/TAO_RTScheduler.rc

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

nobase_include_HEADERS += \
  SmartProxies/Smart_Proxies.h \
  SmartProxies/smartproxies_export.h \
  SmartProxies/Smart_Proxies.inl

pkgconfigdata_DATA += TAO_SmartProxies.pc

TAO_SmartProxies.pc: ${top_builddir}/config.status ${srcdir}/SmartProxies/TAO_SmartProxies.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/SmartProxies/TAO_SmartProxies.pc.in

EXTRA_DIST += \
  SmartProxies/SmartProxies.rc

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
  Strategies/DIOP_Connection_Handler.cpp \
  Strategies/DIOP_Connector.cpp \
  Strategies/DIOP_Endpoint.cpp \
  Strategies/DIOP_Factory.cpp \
  Strategies/DIOP_Profile.cpp \
  Strategies/DIOP_Transport.cpp \
  Strategies/FIFO_Connection_Purging_Strategy.cpp \
  Strategies/LFU_Connection_Purging_Strategy.cpp \
  Strategies/LF_Strategy_Null.cpp \
  Strategies/NULL_Connection_Purging_Strategy.cpp \
  Strategies/SCIOP_Acceptor.cpp \
  Strategies/SCIOP_Connection_Handler.cpp \
  Strategies/SCIOP_Connector.cpp \
  Strategies/SCIOP_Endpoint.cpp \
  Strategies/SCIOP_Factory.cpp \
  Strategies/SCIOP_Lite_Factory.cpp \
  Strategies/SCIOP_Profile.cpp \
  Strategies/SCIOP_Transport.cpp \
  Strategies/SHMIOP_Acceptor.cpp \
  Strategies/SHMIOP_Connection_Handler.cpp \
  Strategies/SHMIOP_Connector.cpp \
  Strategies/SHMIOP_Endpoint.cpp \
  Strategies/SHMIOP_Factory.cpp \
  Strategies/SHMIOP_Profile.cpp \
  Strategies/SHMIOP_Transport.cpp \
  Strategies/UIOP_Acceptor.cpp \
  Strategies/UIOP_Connection_Handler.cpp \
  Strategies/UIOP_Connector.cpp \
  Strategies/UIOP_Endpoint.cpp \
  Strategies/UIOP_Factory.cpp \
  Strategies/UIOP_Lite_Factory.cpp \
  Strategies/UIOP_Profile.cpp \
  Strategies/UIOP_Transport.cpp \
  Strategies/advanced_resource.cpp \
  Strategies/sciop_endpoints.cpp \
  Strategies/uiop_endpoints.cpp

libTAO_Strategies_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_Strategies_la_LIBADD = \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  Strategies/DIOP_Acceptor.h \
  Strategies/DIOP_Connection_Handler.h \
  Strategies/DIOP_Connector.h \
  Strategies/DIOP_Endpoint.h \
  Strategies/DIOP_Factory.h \
  Strategies/DIOP_Profile.h \
  Strategies/DIOP_Transport.h \
  Strategies/FIFO_Connection_Purging_Strategy.h \
  Strategies/LFU_Connection_Purging_Strategy.h \
  Strategies/LF_Strategy_Null.h \
  Strategies/NULL_Connection_Purging_Strategy.h \
  Strategies/SCIOP_Acceptor.h \
  Strategies/SCIOP_Connection_Handler.h \
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
  Strategies/DIOP_Endpoint.i \
  Strategies/LF_Strategy_Null.inl \
  Strategies/SCIOP_Acceptor.i \
  Strategies/SCIOP_Endpoint.i \
  Strategies/SHMIOP_Endpoint.i \
  Strategies/sciop_endpoints.i \
  Strategies/uiop_endpoints.i \
  Strategies/sciop_endpoints.pidl \
  Strategies/uiop_endpoints.pidl

pkgconfigdata_DATA += TAO_Strategies.pc

TAO_Strategies.pc: ${top_builddir}/config.status ${srcdir}/Strategies/TAO_Strategies.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/Strategies/TAO_Strategies.pc.in

EXTRA_DIST += \
  Strategies/TAO_Strategies.rc

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
  TypeCodeFactory/TypeCodeFactory_Loader.cpp \
  TypeCodeFactory/TypeCodeFactory_i.cpp

libTAO_TypeCodeFactory_la_LDFLAGS = \
 -version-number @TAO_MAJOR@:@TAO_MINOR@:@TAO_BETA@

libTAO_TypeCodeFactory_la_LIBADD = \
  libTAO_IFR_Client.la \
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
  TypeCodeFactory/TypeCodeFactoryC.h \
  TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h \
  TypeCodeFactory/TypeCodeFactory_Loader.h \
  TypeCodeFactory/TypeCodeFactory_i.h \
  TypeCodeFactory/typecodefactory_export.h \
  TypeCodeFactory/TypeCodeFactoryC.inl \
  TypeCodeFactory/TypeCodeFactory.pidl

pkgconfigdata_DATA += TAO_TypeCodeFactory.pc

TAO_TypeCodeFactory.pc: ${top_builddir}/config.status ${srcdir}/TypeCodeFactory/TAO_TypeCodeFactory.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/TypeCodeFactory/TAO_TypeCodeFactory.pc.in

EXTRA_DIST += \
  TypeCodeFactory/TypeCodeFactory.rc

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
  libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

nobase_include_HEADERS += \
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

pkgconfigdata_DATA += TAO_Utils.pc

TAO_Utils.pc: ${top_builddir}/config.status ${srcdir}/TAO_Utils.pc.in
	${top_builddir}/config.status --file $@:${srcdir}/TAO_Utils.pc.in

## Clean up template repositories, etc.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -f gcctemp.c gcctemp so_locations
	-rm -rf ptrepository SunWS_cache Templates.DB

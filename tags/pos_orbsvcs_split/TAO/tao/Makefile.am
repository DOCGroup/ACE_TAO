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
  POAC.cpp \
  POAS.cpp \
  POAManager.cpp \
  Active_Object_Map.cpp \
  Servant_Base.cpp \
  Forwarding_Servant.cpp

PLUGGABLE_PROTOCOLS_FILES = \
  Pluggable.cpp \
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
  IORS.cpp \
  IORManipulation.cpp

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
  CDR_Interpreter.cpp \
  Marshal.cpp \
  skip.cpp

IDL_COMPILER_FILES = \
  Managed_Types.cpp \
  Union.cpp \
  Operation_Table.cpp

ORB_CORE_FILES = \
  Any.cpp \
  CurrentC.cpp \
  CurrentS.cpp \
  Exception.cpp \
  WrongTransactionC.cpp \
  Environment.cpp \
  Object.cpp \
  ObjectIDList.cpp \
  ORB.cpp \
  corbafwd.cpp \
  PolicyC.cpp \
  PolicyS.cpp \
  DomainC.cpp \
  DomainS.cpp \
  TimeBaseC.cpp \
  TimeBaseS.cpp \
  Principal.cpp \
  Sequence.cpp \
  Server_Request.cpp \
  MProfile.cpp \
  Stub.cpp \
  Typecode.cpp \
  ValueBase.cpp \
  ValueFactory.cpp \
  ValueFactory_Map.cpp \
  CDR.cpp \
  Client_Strategy_Factory.cpp \
  debug.cpp \
  Object_KeyC.cpp \
  GIOP.cpp \
  GIOP_Server_Request.cpp \
  Invocation.cpp \
  Interceptor.cpp \
  InterceptorS.cpp \
  InterceptorC.cpp \
  Asynch_Invocation.cpp \
  ORB_Core.cpp \
  Leader_Follower.cpp \
  Reactor_Registry.cpp \
  Single_Reactor.cpp \
  Reactor_Per_Priority.cpp \
  params.cpp \
  Resource_Factory.cpp \
  Server_Strategy_Factory.cpp \
  Sync_Strategies.cpp \
  TAO_Internal.cpp \
  Typecode_Constants.cpp \
  IOR_LookupTable.cpp \
  TAO.cpp \
  Wait_Strategy.cpp \
  Transport_Mux_Strategy.cpp \
  Reply_Dispatcher.cpp \
  IOPC.cpp \
  IOPS.cpp \
  PollableC.cpp \
  PollableS.cpp \
  CONV_FRAMEC.cpp \
  CONV_FRAMES.cpp \
  Tagged_Components.cpp \
  Context.cpp \
  Request.cpp \
  Services.cpp \
  NVList.cpp \
  MessagingC.cpp \
  MessagingS.cpp \
  Policy_Manager.cpp \
  Messaging_Policy_i.cpp \
  TAOC.cpp \
  TAOS.cpp \
  Client_Priority_Policy.cpp \
  Buffering_Constraint_Policy.cpp \
  rtcorbafwd.cpp \
  Priority_Mapping.cpp \
  Linear_Priority_Mapping.cpp \
  Direct_Priority_Mapping.cpp \
  RT_Current.cpp \
  Pool_Per_Endpoint.cpp \
  GIOPC.cpp

DYNAMIC_ANY_FILES = \
  DynAnyC.cpp \
  DynAnyS.cpp \
  DynAny_i.cpp \
  DynArray_i.cpp \
  DynEnum_i.cpp \
  DynSequence_i.cpp \
  DynStruct_i.cpp \
  DynUnion_i.cpp \
  InconsistentTypeCodeC.cpp

# Build a libtool library, libTAO.la for installation in libdir.
lib_LTLIBRARIES = libTAO.la
libTAO_la_SOURCES = \
  $(POA_FILES) \
  $(PLUGGABLE_PROTOCOLS_FILES) \
  $(DEFAULT_RESOURCES_FILES) \
  $(INTERPRETIVE_MARSHALING_FILES) \
  $(IDL_COMPILER_FILES) \
  $(ORB_CORE_FILES) \
  $(DYNAMIC_ANY_FILES)

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
  varout.cpp
else
TEMPLATE_FILES = 
endif

IDL_HEADERS = \
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
  CDR_Interpreter.h \
  CONV_FRAMEC.h \
  CONV_FRAMES.h \
  CONV_FRAMES_T.h \
  Client_Priority_Policy.h \
  Client_Strategy_Factory.h \
  Connector_Registry.h \
  Context.h \
  CurrentC.h \
  Direct_Priority_Mapping.h \
  DomainC.h \
  DomainS_T.h \
  DynAnyC.h \
  DynAny_i.h \
  DynArray_i.h \
  DynEnum_i.h \
  DynSequence_i.h \
  DynStruct_i.h \
  DynUnion_i.h \
  DynUnion_i_T.h \
  Environment.h \
  Exception.h \
  Forwarding_Servant.h \
  GIOP.h \
  GIOPC.h \
  GIOP_Server_Request.h \
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
  InterfaceC.h \
  InterfaceS.h \
  Invocation.h \
  Key_Adapters.h \
  Leader_Follower.h \
  Linear_Priority_Mapping.h \
  MProfile.h \
  Managed_Types.h \
  Marshal.h \
  MessagingC.h \
  MessagingS.h \
  MessagingS_T.h \
  Messaging_Policy_i.h \
  NVList.h \
  ORB.h \
  ORB_Core.h \
  Object.h \
  ObjectIDList.h \
  Object_Adapter.h \
  Object_KeyC.h \
  Operation_Table.h \
  POA.h \
  POAC.h \
  POAManager.h \
  POAS.h \
  POA_CORBA.h \
  Pluggable.h \
  PolicyC.h \
  Policy_Manager.h \
  PollableC.h \
  PollableS.h \
  PollableS_T.h \
  Pool_Per_Endpoint.h \
  Principal.h \
  Priority_Mapping.h \
  Profile.h \
  Protocol_Factory.h \
  RT_Current.h \
  Reactor_Per_Priority.h \
  Reactor_Registry.h \
  Reply_Dispatcher.h \
  Request.h \
  Resource_Factory.h \
  Sequence.h \
  Sequence_T.h \
  Servant_Base.h \
  Server_Request.h \
  Server_Strategy_Factory.h \
  Services.h \
  Single_Reactor.h \
  Stub.h \
  Sync_Strategies.h \
  TAO.h \
  TAOC.h \
  TAOS.h \
  TAOS_T.h \
  TAO_Internal.h \
  Tagged_Components.h \
  TimeBaseC.h \
  TimeBaseS.h \
  TimeBaseS_T.h \
  Timeprobe.h \
  Transport_Mux_Strategy.h \
  Typecode.h \
  UIOP_Acceptor.h \
  UIOP_Connect.h \
  UIOP_Connector.h \
  UIOP_Factory.h \
  UIOP_Profile.h \
  UIOP_Transport.h \
  Union.h \
  ValueBase.h \
  ValueFactory.h \
  ValueFactory_Map.h \
  Version.h \
  Wait_Strategy.h \
  WrongTransactionC.h \
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
  rtcorbafwd.h \
  singletons.h \
  try_macros.h \
  varout.h \
  xt_resource.h

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
  Client_Priority_Policy.i \
  Connector_Registry.i \
  Context.i \
  CurrentC.i \
  Direct_Priority_Mapping.i \
  DomainC.i \
  DomainS_T.i \
  DynAnyC.i \
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
  InterfaceC.i \
  InterfaceS.i \
  Invocation.i \
  Key_Adapters.i \
  Leader_Follower.i \
  Linear_Priority_Mapping.i \
  MProfile.i \
  Managed_Types.i \
  Marshal.i \
  MessagingC.i \
  MessagingS.i \
  MessagingS_T.i \
  Messaging_Policy_i.i \
  NVList.i \
  ORB.i \
  ORB_Core.i \
  Object.i \
  ObjectIDList.i \
  Object_Adapter.i \
  Object_KeyC.i \
  POA.i \
  POAC.i \
  POAManager.i \
  POAS.i \
  Pluggable.i \
  PolicyC.i \
  Policy_Manager.i \
  PollableC.i \
  PollableS.i \
  PollableS_T.i \
  Pool_Per_Endpoint.i \
  Principal.i \
  Priority_Mapping.i \
  Profile.i \
  RT_Current.i \
  Reactor_Per_Priority.i \
  Reactor_Registry.i \
  Reply_Dispatcher.i \
  Request.i \
  Sequence.i \
  Sequence_T.i \
  Servant_Base.i \
  Server_Request.i \
  Services.i \
  Single_Reactor.i \
  Stub.i \
  Sync_Strategies.i \
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
  rtcorbafwd.i \
  varout.i \
  xt_resource.i


pkgincludedir = $(prefix)/include/tao
pkginclude_HEADERS = \
  $(HEADER_FILES) \
  $(INLINE_FILES) \
  $(TEMPLATE_FILES) \
  $(IDL_HEADERS)


## noinst_PROGRAMS = t-sizes

## t_sizes_SOURCES = t-sizes.cpp

EXTRA_DIST = \
  t-sizes.cpp

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations


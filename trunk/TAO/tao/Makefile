#----------------------------------------------------------------------------

#      $Id$
#
#       Makefile for TAO
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libTAO
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

DIRS = \
  PortableServer \
  DynamicAny \
  DynamicInterface \
  BiDir_GIOP \
  IORManipulation \
  IORTable \
  Strategies \
  SmartProxies

# These are headers for things which are exported and must be
# installed.  (Currently not used).
PUB_HDRS = \
  Pluggable \
  Transport \
  MProfile \
  IIOP_Factory \
  IIOP_Profile \
  IIOP_Transport \
  IIOP_Connector \
  IIOP_Acceptor \
  IIOP_Connection_Handler \
  IIOP_Endpoint \
  operation_details \
  Invocation \
  Asynch_Invocation \
  Params \
  Marshal \
  Debug \
  Default_Client \
  Default_Server \
  Server_Strategy_Factory \
  Client_Strategy_Factory \
  ORB_Core \
  ORB_Table \
  Leader_Follower \
  Reactor_Holder \
  Single_Reactor \
  Wait_Strategy \
  Wait_On_Read \
  Wait_On_Reactor \
  Wait_On_Leader_Follower \
  Transport_Mux_Strategy \
  Exclusive_TMS \
  Muxed_TMS \
  Reply_Dispatcher \
  Synch_Reply_Dispatcher \
  Asynch_Reply_Dispatcher \
  Asynch_Timeout_Handler \
  BoundC \
  GIOPC \
  TAO_Export \
  TAO_Singleton_Manager \
  TAO_Singleton \
  DLL_ORB \
  Adapter \
  Acceptor_Filter \
  Services_Activate \
  Service_Callbacks \
  Parser_Registry \
  IOR_Parser \
  DLL_Parser \
  FILE_Parser \
  CORBALOC_Parser \
  CORBANAME_Parser \
  CORBA_String \
  Connection_Purging_Strategy \
  LRU_Connection_Purging_Strategy

PLUGGABLE_PROTOCOLS_FILES = \
  Pluggable \
  Transport \
  Profile \
  Endpoint \
  Connector_Registry \
  Connection_Handler \
  Acceptor_Registry \
  Protocol_Factory \
  Acceptor_Filter \
  iiop_endpoints \
  IIOP_Factory \
  IIOP_Lite_Factory \
  IIOP_Profile \
  IIOP_Transport \
  IIOP_Connector \
  IIOP_Acceptor \
  IIOP_Connection_Handler \
  IIOP_Endpoint \
  IIOPC

PLUGGABLE_MESSAGING_FILES = \
  Pluggable_Messaging \
  Pluggable_Messaging_Utils \
  GIOP_Message_Base \
  GIOP_Message_Lite \
  GIOP_Message_Reactive_Handler \
  GIOP_Message_Generator_Parser \
  GIOP_Message_Generator_Parser_10 \
  GIOP_Message_Generator_Parser_11 \
  GIOP_Message_Generator_Parser_12 \
  GIOP_Message_Generator_Parser_Impl\
  GIOP_Utils \
  GIOP_Message_Locate_Header \
  target_specification \
  GIOP_Message_State \
  Tagged_Profile

DEFAULT_RESOURCES_FILES = \
  default_client \
  default_server \
  default_resource \
  xt_resource \
  qt_resource

INTERPRETIVE_MARSHALING_FILES = \
  append \
  Marshal \
  skip

IDL_COMPILER_FILES = \
  Managed_Types

ORB_CORE_FILES = \
  Any \
  CORBA_String \
  CurrentC \
  Exception \
  WrongTransactionC \
  Environment \
  Object \
  LocalObject \
  ObjectIDList \
  ORB \
  corbafwd \
  PolicyC \
  Encodable \
  Abstract_Servant_Base \
  Object_Proxy_Broker \
  Remote_Object_Proxy_Broker \
  Object_Proxy_Impl \
  Remote_Object_Proxy_Impl \
  DomainC \
  TimeBaseC \
  Principal \
  Sequence \
  MProfile \
  Stub \
  Typecode \
  ValueBase \
  ValueFactory \
  ValueFactory_Map \
  CDR \
  Client_Strategy_Factory \
  debug \
  Object_KeyC \
  Protocols_Hooks \
  Default_Protocols_Hooks \
  Invocation \
  Invocation_Endpoint_Selectors \
  RT_Invocation_Endpoint_Selectors \
  Asynch_Invocation \
  operation_details \
  PortableInterceptor \
  ClientRequestInfo \
  RequestInfo_Util \
  PortableInterceptorC \
  DynamicC \
  OctetSeqC \
  StringSeqC \
  ORB_Core \
  Stub_Factory \
  RT_Stub_Factory \
  RT_Stub \
  RT_Endpoint_Utils \
  ORB_Table \
  Leader_Follower \
  Reactor_Registry \
  params \
  Resource_Factory \
  Server_Strategy_Factory \
  Sync_Strategies \
  TAO_Internal \
  TAO_Server_Request \
  Typecode_Constants \
  TypeCodeFactory_Adapter \
  Single_Reactor \
  Wait_Strategy \
  Wait_On_Read \
  Wait_On_Reactor \
  Wait_On_Leader_Follower \
  Transport_Mux_Strategy \
  Exclusive_TMS \
  Muxed_TMS \
  Reply_Dispatcher \
  Synch_Reply_Dispatcher \
  Asynch_Reply_Dispatcher \
  Asynch_Timeout_Handler \
  IOPC \
  PollableC \
  CONV_FRAMEC \
  Tagged_Components \
  Service_Context \
  GIOPC \
  BoundsC \
  TAOC \
  Object_Loader \
  TAO_Singleton_Manager \
  DLL_ORB \
  Adapter \
  Services_Activate \
  Service_Callbacks \
  Parser_Registry \
  IOR_Parser \
  DLL_Parser \
  FILE_Parser \
  CORBALOC_Parser \
  CORBANAME_Parser \
  Bind_Dispatcher_Guard \
  Fault_Tolerance_Service \
  Interceptor_List \
  IFR_Client_Adapter \
  ORBInitInfo \
  ORBInitializer_Registry \
  PolicyFactory_Registry \
  Cache_Entries \
  Base_Transport_Property \
  Transport_Cache_Manager \
  Cleanup_Func_Registry \
  Transport_Descriptor_Interface \
  IORInfo \
  Object_Ref_Table \
  BiDir_Adapter \
  CodecFactory \
  CodecFactory_ORBInitializer \
  CDR_Encaps_Codec \
  Endpoint_Selector_Factory \
  RT_Endpoint_Selector_Factory \
  Flushing_Strategy \
  Block_Flushing_Strategy \
  Reactive_Flushing_Strategy \
  Queued_Message \
  Synch_Queued_Message \
  Asynch_Queued_Message \
  Transport_Timer \
  Connection_Purging_Strategy \
  LRU_Connection_Purging_Strategy

DYNAMIC_ANY_FILES =

TEMPLATE_FILES = \
  Acceptor_Impl \
  Connector_Impl \
  Connector_Impl \
  CONV_FRAMES_T \
  GIOPS_T \
  IOPS_T \
  PollableS_T \
  RTCORBAS_T \
  Sequence_T \
  TAOS_T \
  TAO_Singleton \
  TimeBaseS_T

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

DEFS    = $(addsuffix .h,$(PUB_HDRS))

ACE_SHLIBS = -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)

ORB_CORE_FILES += \
  Services \
  NVList \
  IFR_TypeCodes \
  Dynamic_Adapter

endif # minimum_corba

ifeq ($(corba_messaging),1)

ORB_CORE_FILES += \
  MessagingC \
  Policy_Manager \
  Messaging_Policy_i \
  Client_Priority_Policy \
  Buffering_Constraint_Policy \
  Messaging_PolicyFactory \
  Messaging_ORBInitializer

endif # corba_messaging

ifeq ($(rt_corba),1)

ORB_CORE_FILES += \
  Priority_Mapping \
  Priority_Mapping_Manager \
  RT_Current \
  RT_Mutex \
  RT_Policy_i \
  RT_ORB \
  Thread_Pool \
  RT_ORB_Loader \
  RT_Protocols_Hooks \
  Pool_Per_Endpoint \
  RTCORBAC \
  RT_PolicyFactory \
  RT_ORBInitializer \
  Private_Transport_Descriptor

#rtcorbafwd

endif # rt_corba

####
#### Build customization.
####
ifndef TAO_COMPONENTS
  TAO_COMPONENTS = \
    POA \
    Pluggable_Protocols \
    Pluggable_Messaging \
    Default_Resources \
    Interpretive_Marshaling \
    IDL_Compiler \
    ORB_Core \
    Dynamic_Any \
    Ft_Corba \

  ifeq (1,$(ACE_HAS_GNUG_PRE_2_8))
  else
    TAO_COMPONENTS += \
      Interface_repo
  endif # ! ACE_HAS_GNUG_PRE_2_8
endif # TAO_COMPONENTS

####
#### TAO_COMPONENTS support.
####
ifneq (,$(findstring Pluggable_Protocols,$(TAO_COMPONENTS)))
  FILES += $(PLUGGABLE_PROTOCOLS_FILES)
endif # Pluggable_Protocols

ifneq (,$(findstring Default_Resources,$(TAO_COMPONENTS)))
  FILES += $(DEFAULT_RESOURCES_FILES)
endif # Default_Resources

ifneq (,$(findstring Interpretive_Marshaling,$(TAO_COMPONENTS)))
  FILES += $(INTERPRETIVE_MARSHALING_FILES)
endif # Interpretive_Marshaling

ifneq (,$(findstring IDL_Compiler,$(TAO_COMPONENTS)))
  FILES += $(IDL_COMPILER_FILES)
endif # IDL_Compiler

ifneq (,$(findstring ORB_Core,$(TAO_COMPONENTS)))
  FILES += $(ORB_CORE_FILES)
endif # ORB_Core

ifneq (,$(findstring Dynamic_Any,$(TAO_COMPONENTS)))
  FILES += $(DYNAMIC_ANY_FILES)
endif # Dynamic_Any

ifneq (,$(findstring Interface_repo,$(TAO_COMPONENTS)))
  FILES += $(INTERFACE_REPO_FILES)
endif # INTERFACE_REPO

ifneq (,$(findstring Pluggable_Messaging,$(TAO_COMPONENTS)))
  FILES += $(PLUGGABLE_MESSAGING_FILES)
endif # PLUGGABLE_MESSAGING

#ifneq (,$(findstring Ft_Corba,$(TAO_COMPONENTS)))
#  FILES += $(FT_CORBA_FILES)
#endif # FT_CORBA

LSRC    = $(addsuffix .cpp,$(FILES))
BUILD   += TAO_COMPONENTS

all.nested: all.local

include $(ACE_ROOT)/include/makeinclude/macros.GNU

ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
  DIRS += IFR_Client Domain TypeCodeFactory
endif # ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

# @@ Can we generalize this to something like this:
# $(MANAGED_PIDL).target:
# 	......
#interceptor.target:
#	$(TAO_ROOT)/TAO_IDL/tao_idl -Ge 1 -Wb,export_macro=TAO_Export Interceptor.pidl; \
#	$(RM) -f InterceptorS.* InterceptorS_T.*; \
#	patch < diffs/InterceptorC.h.diff; \
#	patch < diffs/InterceptorC.cpp.diff
#.PHONY interceptor.target:
# Need to add how to generate for PortableInterceptor.pidl and Dynamic.pidl
ior.target:
	$(TAO_ROOT)/TAO_IDL/tao_idl -Ge 1 -Wb,export_macro=TAO_Export IOR.pidl; \
	$(RM) -f IORS.* IORS_T.*; \
	patch < diffs/IORC.h.diff
.PHONY ior.target:

ifeq ($(fakesvcconf),1)
  CPPFLAGS += -DTAO_PLATFORM_SVC_CONF_FILE_NOTSUP
endif # fakesvcconf

ifeq ($(LYNXTARGET),ppc)
#### POA.cpp raises internal compiler error with LynxOS 3.0.0 g++, but
#### only when optimization is enabled.  Disable it . . .
.obj/POA.o .obj/POA.so .shobj/POA.o .shobj/POA.so: POA.cpp
	@echo NOTE: compiling POA.cpp with -O instead of -O2, see Makefile.
	$(subst $(OCFLAGS) ,-O ,$(COMPILE.cc)) -o $@ $<
endif # LYNXTARGET == ppc

#RLDFLAGS := $(subst -L,-Wl\,-rpath ,$(LDFLAGS))
LDFLAGS += $(RLDFLAGS)

.PRECIOUS: Object_KeyC.h Object_KeyC.i Object_KeyC.cpp
.PRECIOUS: PortableServerC.h PortableServerC.i PortableServerC.cpp

.PHONY: TAO_COMPONENTS
TAO_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --tao --set ' $(TAO_COMPONENTS) '

.PHONY: SIZES
MODULES= \
	PLUGGABLE_PROTOCOLS_FILES \
	DEFAULT_RESOURCES_FILES \
	INTERPRETIVE_MARSHALING_FILES \
	IDL_COMPILER_FILES \
	ORB_CORE_FILES \
	DYNAMIC_ANY_FILES \
	PLUGGABLE_MESSAGING_FILES \
	INTERFACE_REPO_FILES \
	FT_CORBA_FILES

SIZES: $(addsuffix .size, $(MODULES))
	@for i in $^; do \
	  b=`cat $$i`; \
	  echo $$i : $$b; \
	done

%.size:
	@size $(addsuffix .o, $(addprefix .obj/,$($*))) | \
	  sort -n -k 4,4 >$@.details
	@size $(addsuffix .o, $(addprefix .obj/,$($*))) | \
	grep -v ' text ' | \
	awk '{s += $$4} END {print s}' > $@


realclean:
	@sh $(ACE_ROOT)/bin/ace_components --tao --remove

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

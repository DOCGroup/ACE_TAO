#----------------------------------------------------------------------------
#      $Id$
#       Makefile for TAO
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libTAO
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

# These are headers for things which are exported and must be
# installed.  (Currently not used).
PUB_HDRS = \
  Pluggable \
  MProfile \
  IIOP_Factory \
  IIOP_Profile \
  IIOP_Transport \
  IIOP_Connector \
  IIOP_Acceptor \
  IIOP_Connect \
  UIOP_Factory \
  UIOP_Profile \
  UIOP_Transport \
  UIOP_Connector \
  UIOP_Acceptor \
  UIOP_Connect \
  SHMIOP_Factory \
  SHMIOP_Profile \
  SHMIOP_Transport \
  SHMIOP_Connector \
  SHMIOP_Acceptor \
  SHMIOP_Connect \
  operation_details \
  Invocation \
  Asynch_Invocation \
  DII_Invocation \
  Params \
  Marshal \
  Debug \
  Default_Client \
  Default_Server \
  Server_Strategy_Factory \
  Client_Strategy_Factory \
  ORB_Core \
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
  DII_Reply_Dispatcher \
  IORC \
  IORManipulation \
  BoundC \
  GIOPC \
  TAO_Export \
  TAO_Singleton_Manager \
  TAO_Singleton \
  DLL_ORB

POA_FILES = \
  Object_Adapter \
  Key_Adapters \
  POA \
  PortableServerC \
  POAManager \
  Active_Object_Map \
  Servant_Base

PLUGGABLE_PROTOCOLS_FILES = \
  Pluggable \
  Profile \
  Connector_Registry \
  Acceptor_Registry \
  Protocol_Factory \
  IIOP_Factory \
  IIOP_Lite_Factory \
  IIOP_Profile \
  IIOP_Transport \
  IIOP_Connector \
  IIOP_Acceptor \
  IIOP_Connect \
  UIOP_Factory \
  UIOP_Lite_Factory \
  UIOP_Profile \
  UIOP_Transport \
  UIOP_Connector \
  UIOP_Acceptor \
  UIOP_Connect \
  SHMIOP_Factory \
  SHMIOP_Profile \
  SHMIOP_Transport \
  SHMIOP_Connector \
  SHMIOP_Acceptor \
  SHMIOP_Connect \
  IORC \
  IORManipulation

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
GIOP_Server_Request \
GIOP_Message_Lite \
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
  Managed_Types \
  Operation_Table

ORB_CORE_FILES = \
  Any \
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
  PolicyS \
  Encodable \
  Policy_Factory \
  DomainC \
  DomainS \
  TimeBaseC \
  TimeBaseS \
  Principal \
  Sequence \
  Server_Request \
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
  Invocation \
  Asynch_Invocation \
  DII_Invocation \
  operation_details \
  Interceptor \
  InterceptorC \
  Smart_Proxies \
  ORB_Core \
  Leader_Follower \
  Reactor_Registry \
  Single_Reactor \
  Reactor_Per_Priority \
  params \
  Resource_Factory \
  Server_Strategy_Factory \
  Sync_Strategies \
  TAO_Internal \
  Typecode_Constants \
  IOR_LookupTable \
  TAO \
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
  DII_Reply_Dispatcher \
  IOPC \
  IOPS \
  PollableC \
  PollableS \
  CONV_FRAMEC \
  CONV_FRAMES \
  Tagged_Components \
  GIOPC \
  BoundsC \
  TAOC \
  TAOS \
  Object_Loader \
  TAO_Singleton_Manager \
  DLL_ORB \
  FT_CORBAC \
  FT_CORBAS

DYNAMIC_ANY_FILES =

TEMPLATE_FILES = \
  TAO_Singleton

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

POA_FILES += \
  Forwarding_Servant \
  ImplRepoC \
  ImplRepoS

ORB_CORE_FILES += \
  Context \
  Request \
  Services \
  NVList

DYNAMIC_ANY_FILES += \
  DynAnyC \
  DynAnyS \
  DynAny_i \
  DynArray_i \
  DynEnum_i \
  DynSequence_i \
  DynStruct_i \
  DynUnion_i \
  InconsistentTypeCodeC

INTERFACE_REPO_FILES += \
  InterfaceC

endif # minimum_corba

ifeq ($(corba_messaging),1)

ORB_CORE_FILES += \
  MessagingC \
  MessagingS \
  Policy_Manager \
  Messaging_Policy_i \
  Client_Priority_Policy \
  Buffering_Constraint_Policy

endif # corba_messaging

ifeq ($(rt_corba),1)

ORB_CORE_FILES += \
  Priority_Mapping \
  Priority_Mapping_Manager \
  Linear_Priority_Mapping \
  Direct_Priority_Mapping \
  RT_Current \
  RT_Mutex \
  RT_Policy_i \
  RT_ORB \
  Pool_Per_Endpoint \
  RTCORBAC \
  RTCORBAS #\
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
    Dynamic_Any

  ifeq (1,$(CROSS-COMPILE))
  else
  ifeq (1,$(ACE_HAS_GNUG_PRE_2_8))
  else
    TAO_COMPONENTS += \
      Interface_repo
  endif # ! ACE_HAS_GNUG_PRE_2_8
  endif # ! CROSS-COMPILE
endif # TAO_COMPONENTS

####
#### TAO_COMPONENTS support.
####
ifneq (,$(findstring POA,$(TAO_COMPONENTS)))
  FILES += $(POA_FILES)
endif # POA

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
endif # INTERFACE_REPO

LSRC    = $(addsuffix .cpp,$(FILES))
BUILD   += TAO_COMPONENTS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

# @@ Can we generalize this to something like this:
# $(MANAGED_PIDL).target:
# 	......
interceptor.target:
	$(TAO_ROOT)/TAO_IDL/tao_idl -Ge 1 -Wb,export_macro=TAO_Export Interceptor.pidl; \
	$(RM) -f InterceptorS.* InterceptorS_T.*; \
	patch < diffs/InterceptorC.h.diff; \
	patch < diffs/InterceptorC.cpp.diff
.PHONY interceptor.target:

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
MODULES=POA_FILES \
	PLUGGABLE_PROTOCOLS_FILES \
	DEFAULT_RESOURCES_FILES \
	INTERPRETIVE_MARSHALING_FILES \
	IDL_COMPILER_FILES \
	ORB_CORE_FILES \
	DYNAMIC_ANY_FILES \
	PLUGGABLE_MESSAGING_FILES \
	INTERFACE_REPO_FILES

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


#----------------------------------------------------------------------------
#       $Id$
#
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
  IIOP_Profile \
  IIOP_Transport \
  IIOP_Connector \
  IIOP_Acceptor \
  IIOP_Connect \
  UIOP_Profile \
  UIOP_Transport \
  UIOP_Connector \
  UIOP_Acceptor \
  UIOP_Connect \
  GIOP \
  Invocation \
  Params \
  Marshal \
  Debug \
  Default_Client \
  Default_Server \
  Server_Strategy_Factory \
  Client_Strategy_Factory \
  ORB_Core \
  Wait_Strategy \
  Transport_Mux_Strategy \
  Reply_Dispatcher \
  IORC \
  IORS \
  IORManipulation

POA_FILES = \
  Object_Adapter \
  Key_Adapters \
  POA \
  POAC \
  POAS \
  POAManager \
  Active_Object_Map \
  Servant_Base

PLUGGABLE_PROTOCOLS_FILES = \
  Pluggable \
  Connector_Registry \
  Acceptor_Registry \
  Protocol_Factory \
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
  IORC \
  IORS \
  IORManipulation

DEFAULT_RESOURCES_FILES = \
  default_client \
  default_server \
  default_resource

INTERPRETIVE_MARSHALING_FILES = \
  append \
  decode \
  deep_free \
  encode \
  CDR_Interpreter \
  Marshal \
  skip

IDL_COMPILER_FILES = \
  Managed_Types \
  Union \
  Operation_Table

ORB_CORE_FILES = \
  Any \
  CurrentC \
  CurrentS \
  Exception \
  WrongTransactionC \
  Environment \
  Object \
  ORB \
  corbafwd \
  PolicyC \
  PolicyS \
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
  GIOP \
  GIOP_Server_Request \
  Invocation \
  ORB_Core \
  params \
  Resource_Factory \
  Server_Strategy_Factory \
  TAO_Internal \
  Typecode_Constants \
  IOR_LookupTable \
  TAO \
  Wait_Strategy \
  Transport_Mux_Strategy \
  Reply_Dispatcher \
  IOPC \
  IOPS \
  CONV_FRAMEC \
  CONV_FRAMES \
  Tagged_Components

DYNAMIC_ANY_FILES =

ifndef minimum_corba
  minimum_corba = 0
endif # minimum_corba

ifeq ($(minimum_corba),0)

POA_FILES += \
  Forwarding_Servant

ORB_CORE_FILES += \
  Context \
  Request \
  Services \
  NVList \
  ObjectIDList

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

endif # minimum_corba

ifndef corba_messaging
  corba_messaging = 1
endif # corba_messaging

ifeq ($(corba_messaging),1)
ORB_CORE_FILES += \
  MessagingC \
  MessagingS \
  Policy_Manager \
  Messaging_Policy_i
endif # corba_messaging

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

DEFS    = $(addsuffix .h,$(PUB_HDRS))

LDLIBS = -lACE
LIBS += $(ACELIB)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### Build customization.
####
ifndef TAO_COMPONENTS
  TAO_COMPONENTS = \
    POA \
    Pluggable_Protocols \
    Default_Resources \
    Interpretive_Marshaling \
    IDL_Compiler \
    ORB_Core \
    Dynamic_Any
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

LSRC    = $(addsuffix .cpp,$(FILES))
BUILD   += TAO_COMPONENTS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(fakesvcconf),1)
  CPPFLAGS += -DTAO_PLATFORM_SVC_CONF_FILE_NOTSUP
endif

#RLDFLAGS := $(subst -L,-Wl\,-rpath ,$(LDFLAGS))
LDFLAGS += $(RLDFLAGS)

.PRECIOUS: Object_KeyC.h Object_KeyC.i Object_KeyC.cpp
.PRECIOUS: POAS.h POAS.i POAS.cpp POAC.h POAC.i POAC.cpp

.PHONY: TAO_COMPONENTS
TAO_COMPONENTS:
	@$(ACE_ROOT)/bin/ace_components --tao --set " $(TAO_COMPONENTS) "

realclean:
	@$(ACE_ROOT)/bin/ace_components --tao --remove

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
DEPENDENCY_FILE = .depend.Makefile
LIBNAME  = libTAO_NotifyTests
LIB2      = $(LIBNAME).a
SHLIB2    = $(LIBNAME).$(SOEXT)

TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)

ifeq (Notify,$(findstring Notify,$(TAO_ORBSVCS)))
  LIB = $(LIB2)
  SHLIB = $(SHLIB2)
endif # Notify

IDL_FILES = Activation_Manager
IDL_SRC = $(foreach ext, C.cpp S.cpp, $(foreach file, $(IDL_FILES), $(file)$(ext)))

TAO_IDLFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -Ge 1 -Sc

ACE_SHLIBS = -lTAO_CosNotification -lTAO_ETCL -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_DynamicAny -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO $(ACELIB)

LDLIBS = -lTAO_CosNotification -lTAO_ETCL -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_DynamicAny -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO $(ACELIB)

SRCS = \
  Activation_ManagerC \
  Activation_ManagerS \
  Peer \
  StructuredPushConsumer \
  SequencePushConsumer \
  PushConsumer \
  StructuredPushSupplier \
  SequencePushSupplier \
  PushSupplier \
  Direct_Consumer \
  Direct_Supplier \
  Relay_Consumer \
  Activation_Manager \
  Application_Command \
  Command \
  Command_Builder \
  Command_Factory \
  Command_Factory_T \
  ConsumerAdmin_Command \
  Driver \
  EventChannel_Command \
  Factories_Define \
  Filter_Command \
  LookupManager \
  Name \
  Notify_Test_Client \
  Options_Parser \
  Periodic_Consumer_Command \
  Periodic_Supplier_Command \
  Periodic_Consumer \
  Periodic_Supplier \
  Priority_Mapping \
  StructuredEvent \
  SupplierAdmin_Command \
  Task_Stats

FILES = $(SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))


#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS  += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs

ADDITIONAL_IDL_TARGETS =

# IDL File Directory: .

idl_stubs: $(ADDITIONAL_IDL_TARGETS)

all: idl_stubs

.PRECIOUS: $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

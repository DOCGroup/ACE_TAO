#$Id$
#----------------------------------------------------------------------------
#       GNU Makefile
#----------------------------------------------------------------------------
MAKEFILE        = Makefile
DEPENDENCY_FILE = .depend.Makefile
LIB_UNCHECKED   = libTAO_NotifyTests.a
SHLIB_UNCHECKED = libTAO_NotifyTests.$(SOEXT)

LSRC   = $(addsuffix .cpp, $(FILES))
ACELIB = -lACE

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

IDL_FILES = Activation_Manager
IDL_SRC = $(foreach ext, C.cpp S.cpp, $(foreach file, $(IDL_FILES), $(file)$(ext)))



FILES = \
  Activation_Manager \
  Activation_ManagerC \
  Activation_ManagerS \
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
  Notify_PushConsumer \
  Notify_PushSupplier \
  Notify_SequencePushConsumer \
  Notify_SequencePushSupplier \
  Notify_StructuredPushConsumer \
  Notify_StructuredPushSupplier \
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


#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
CURRENT_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Notify, $(findstring Notify, $(CURRENT_COMPONENTS)))
ifeq (Naming, $(findstring Naming, $(CURRENT_COMPONENTS)))
LIB   = $(LIB_UNCHECKED)
SHLIB = $(SHLIB_UNCHECKED)
endif
endif
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

CPPFLAGS     += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao -I$(ACE_ROOT)
ifeq ($(shared_libs),1)
ifneq ($(SHLIB),)
CPPFLAGS     += -DTAO_NOTIFY_TEST_BUILD_DLL
endif
endif
ifeq ($(static_libs),1)
ifneq ($(LIB),)
CPPFLAGS     += -DTAO_AS_STATIC_LIBS -DACE_AS_STATIC_LIBS
endif
endif


LDFLAGS      += -L$(TAO_ROOT)/tao/PortableServer -L$(TAO_ROOT)/tao/DynamicAny -L$(TAO_ROOT)/tao/IORTable -L$(TAO_ROOT)/orbsvcs/orbsvcs/ETCL -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
TAO_IDLFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -Ge 1 -Sc

ACE_SHLIBS = -lTAO_CosNotification -lTAO_ETCL -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_DynamicAny -lTAO_PortableServer -lTAO $(ACELIB)

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

ADDITIONAL_IDL_TARGETS =

# IDL File Directory: .

idl_stubs: $(ADDITIONAL_IDL_TARGETS)

all: idl_stubs

.PRECIOUS: $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))

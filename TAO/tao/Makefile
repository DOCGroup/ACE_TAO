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
PUB_HDRS = Align CDR \
	   Pluggable MProfile IIOP_Profile IIOP_Transport IIOP_Connector IIOP_Acceptor \
	   GIOP Invocation IIOP_ORB Connect Params \
	   Marshal Debug \
	   Default_Client Default_Server \
	   Server_Strategy_Factory Client_Strategy_Factory ORB_Core

ORBCORE_SRCS = \
	Any \
	CurrentC \
	CurrentS \
	DynAnyC \
	DynAnyS \
	DynAny_i \
	DynArray_i \
	DynEnum_i \
	DynSequence_i \
	DynStruct_i \
	DynUnion_i \
	Exception \
	Environment \
        Forwarding_Servant \
	InconsistentTypeCodeC \
	Managed_Types	\
	NVList \
	Object \
	ORB \
	POA \
	POAC \
	POAS \
	PolicyC \
	PolicyS \
	Principal \
	Request \
	Sequence \
	Server_Request \
  Pluggable \
  MProfile \
  IIOP_Profile \
  IIOP_Transport \
  IIOP_Connector \
  IIOP_Acceptor \
	Stub \
	Typecode \
	Union \
	CDR \
	Client_Strategy_Factory \
	Connect \
	append \
	debug \
	decode \
	deep_free \
	default_client \
	default_server \
	Object_KeyC \
	encode \
	GIOP \
	Invocation \
	IIOP_ORB \
	IIOP_Interpreter \
	Marshal \
	Active_Object_Map \
	Operation_Table \
	ORB_Core \
	params \
	Server_Strategy_Factory \
	Servant_Base \
	skip \
	TAO_Internal \
	Typecode_Constants \
	TAO

FILES    = $(ORBCORE_SRCS)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

BIN = # t-sizes

DEFS    = $(addsuffix .h,$(PUB_HDRS))
LSRC    = $(addsuffix .cpp,$(FILES))
LSRC2   = $(addsuffix .cpp,$(TEMPLATE_FILES))

LDLIBS = -lACE
LIBS = -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(shared_libs),)
  override fakesvcconf = 1
endif # ($(shared_libs),)

ifeq ($(fakesvcconf),1)
  CPPFLAGS += -DTAO_PLATFORM_SVC_CONF_FILE_NOTSUP
endif

#RLDFLAGS := $(subst -L,-Wl\,-rpath ,$(LDFLAGS))
LDFLAGS += $(RLDFLAGS)

.PRECIOUS: Object_KeyC.h Object_KeyC.i Object_KeyC.cpp
.PRECIOUS: POAS.h POAS.i POAS.cpp POAC.h POAC.i POAC.cpp

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

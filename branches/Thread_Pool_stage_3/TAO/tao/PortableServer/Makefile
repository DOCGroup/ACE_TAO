#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
LIBNAME  = libTAO_PortableServer
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ACE_SHLIBS = -lTAO -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

CPP_SRCS += \
  PortableServer \
  PortableServerC \
  POA \
  Default_Acceptor_Filter \
  Object_Adapter \
  Operation_Table \
  Key_Adapters \
  POAManager \
  Active_Object_Map \
  Servant_Base \
  ThruPOA_Object_Proxy_Impl \
  Direct_Object_Proxy_Impl \
  Strategized_Object_Proxy_Broker \
  Collocated_Object \
  PolicyS \
  ORB_Manager \
  ImplRepo_i \
  ServerRequestInfo \
  ServerInterceptorAdapter \
  Default_Servant_Dispatcher \
  Servant_Dispatcher \
  POA_Cached_Policies \
  POA_Policies \
  POA_Policy_Set \
  Policy_Validator \
  Default_Policy_Validator

ifeq ($(minimum_corba),0)

CPP_SRCS += \
  ImplRepoC \
  ImplRepoS

endif # minimum_corba

ifeq ($(corba_messaging),1)

CPP_SRCS += \
  MessagingS

endif # corba_messaging

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))
FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

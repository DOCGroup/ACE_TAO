#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LIBNAME  = libTAO_RTCORBA
MAKEFILE = Makefile
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)
ACE_SHLIBS = -lTAO -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(corba_messaging),1)


CPP_SRCS += \
  Continuous_Priority_Mapping \
  Direct_Priority_Mapping \
  Linear_Priority_Mapping \
  RT_Thread_Lane_Resources_Manager \
  Priority_Mapping \
  Priority_Mapping_Manager \
  Private_Transport_Descriptor \
  RTCORBA \
  RTCORBAC \
  RT_Current \
  RT_Endpoint_Selector_Factory \
  RT_Endpoint_Utils \
  RT_Invocation_Endpoint_Selectors \
  RT_Mutex \
  RT_ORB \
  RT_ORBInitializer \
  RT_ORB_Loader \
  RT_PolicyFactory \
  RT_Policy_i \
  RT_Protocols_Hooks \
  RT_Stub \
  RT_Stub_Factory \
  Thread_Pool

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))
FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

endif # corba_messaging

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------
ifeq ($(corba_messaging),1)

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

else
all:
endif # corba_messaging

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

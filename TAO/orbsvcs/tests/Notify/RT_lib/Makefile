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
LIBNAME  = libTAO_RT_NotifyTests
LIB2      = $(LIBNAME).a
SHLIB2    = $(LIBNAME).$(SOEXT)

TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)

ifeq (RT_Notify,$(findstring RT_Notify,$(TAO_ORBSVCS)))
  LIB = $(LIB2)
  SHLIB = $(SHLIB2)
endif # RT_Notify

TAO_IDLFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -Ge 1 -Sc

ACE_SHLIBS =  -lTAO_RTCORBA -lTAO_CosNotification -lTAO_ETCL -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_DynamicAny -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO $(ACELIB)

LDLIBS =  -lTAO_RTCORBA -lTAO_CosNotification -lTAO_ETCL -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_DynamicAny -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO $(ACELIB)

SRCS = \
  RT_Application_Command \
  RT_POA_Command \
  RT_Priority_Mapping \
  RT_Factories_Define

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

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

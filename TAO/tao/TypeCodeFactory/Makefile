#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO_IFR_Client -lTAO -lACE
ACE_SHLIBS += -lTAO_IFR_Client -lTAO -lACE

LIBNAME = libTAO_TypeCodeFactory
LIB = $(LIBNAME).a
SHLIB = $(LIBNAME).$(SOEXT)



#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

CPP_SRCS += \
	TypeCodeFactoryC \
	TypeCodeFactory_i \
        TypeCodeFactory_Adapter_Impl \
	TypeCodeFactory_Loader

FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

include $(TAO_ROOT)/rules.tao.GNU
include $(TAO_ROOT)/taoconfig.mk
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU


#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -I$(TAO_ROOT)/tao

realclean: clean

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

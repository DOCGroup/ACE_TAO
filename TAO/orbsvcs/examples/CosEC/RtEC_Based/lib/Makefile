#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LIBNAME  = libTAO_RTEC_COSEC
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ACE_SHLIBS = -lTAO_RTEvent -lTAO_CosEvent \
             -lTAO_Svc_Utils -lTAO_PortableServer -lTAO -lACE

CPP_SRCS += \
        ConsumerAdmin_i \
        SupplierAdmin_i \
        EventChannel_i \
        ProxyPushConsumer_i \
        ProxyPushSupplier_i \
        CosEvent_Utilities

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))
FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES)) $(IDL_SRC)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/orbsvcs
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs \
        -I$(TAO_ROOT)/orbsvcs/orbsvcs/ESF

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#--------------------------------------------------------------------------
# $Id$
# Build the TAO Trading Service
#--------------------------------------------------------------------------

#---------------
#   STL (start)
#---------------

ifndef STL_ROOT
STL_ROOT=/project/doc/irfan/ObjectSpace/SC4.0
endif

STD=$(STL_ROOT)/ospace/std
STL=$(STL_ROOT)/stl

include $(STL_ROOT)/config/local.cfg

CC_INCLUDES += -I$(STD)
CC_INCLUDES += -pta

CPPFLAGS += \
	-I$(TOOLKIT) \
	$(CC_INCLUDES) \
	$(CC_FLAGS) \
	$(CC_DEFINES) \
	$(CC_EH) \
	$(CC_MT)

ifndef exceptions
CPPFLAGS += -DOS_NO_EXCEPTION_SPECIFIER
else
CPPFLAGS += -DOS_STL_ASSERT
endif

LDFLAGS += -L$(STL_ROOT)/lib

#---------------
#   STL (end)
#---------------

#--------------------------------------------------------------------------
#	Local macros
#--------------------------------------------------------------------------

BIN 	= trader

SVR_SRCS= trader.cpp

LDLIBS  = -lorbsvcs -lTAO -lospace

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD = $(VLIB) $(VSHLIB) $(SHLIBA) $(VBIN)

#--------------------------------------------------------------------------
#	Include macros and targets
#--------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

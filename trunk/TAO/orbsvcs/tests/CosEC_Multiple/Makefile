# $Id$
#
#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

LDLIBS = -lorbsvcs -lTAO

PROG_SRCS = \
        CosEC_Multiple.cpp \
        CosECConsumer.cpp \
        CosECSupplier.cpp

SRC = $(PROG_SRCS)

SIMPLE_CON_OBJS = \
        CosEC_Multiple.o \
        CosECConsumer.o

SIMPLE_SUP_OBJS = \
        CosEC_Multiple.o \
        CosECSupplier.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = supplier \
       consumer

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (CosEvent,$(findstring CosEvent,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # CosEvent

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS +=  -L$(TAO_ROOT)/tao
CPPFLAGS +=

supplier:$(addprefix $(VDIR),$(SIMPLE_SUP_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

consumer:$(addprefix $(VDIR),$(SIMPLE_CON_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

# $Id$

LDLIBS= -lorbsvcs -lTAO

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

IDL_HDR = dataC dataS
IDL_SRC = dataC dataS

ECM_Supplier_OBJS=$(addsuffix .o,$(IDL_SRC) ECM_Supplier ECM_Data)
ECM_Consumer_OBJS=$(addsuffix .o,$(IDL_SRC) ECM_Consumer ECM_Data)

SRC =  $(addsuffix .cpp, $(BIN)) \
        $(addsuffix .cpp, $(IDL_SRC)) \
        ECM_Data.cpp

BIN2 = ECM_Supplier ECM_Consumer

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Event

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

#### Local rules and variables...

.PRECIOUS: $(addsuffix .h, $(IDL_SRC)) $(addsuffix .i, $(IDL_SRC)) $(addsuffix .cpp, $(IDL_SRC))

ECM_Supplier: $(addprefix $(VDIR),$(ECM_Supplier_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ECM_Consumer: $(addprefix $(VDIR),$(ECM_Consumer_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean:
	$(RM) dataC.* dataS.* dataS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

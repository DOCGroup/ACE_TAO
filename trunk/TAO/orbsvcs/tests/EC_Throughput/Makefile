# $Id$

LDLIBS = -lorbsvcs -lTAO

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

ECT_Supplier_OBJS=$(addsuffix .o,ECT_Supplier ECT_Driver ECT_Supplier_Driver)
ECT_Consumer_OBJS=$(addsuffix .o,ECT_Consumer ECT_Driver ECT_Consumer_Driver)
ECT_Throughput_OBJS=$(addsuffix .o,ECT_Throughput ECT_Driver ECT_Consumer ECT_Supplier)

SRC = $(addsuffix .cpp, $(BIN)) \
      ECT_Supplier_Driver.cpp \
      ECT_Consumer_Driver.cpp

BIN2 = ECT_Supplier ECT_Consumer ECT_Throughput

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event2,$(findstring Event2,$(TAO_ORBSVCS)))
  ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
    ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
      BIN = $(BIN2)
    endif # Naming
  endif # Event
endif # Event2

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

ECT_Supplier: $(addprefix $(VDIR),$(ECT_Supplier_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ECT_Consumer: $(addprefix $(VDIR),$(ECT_Consumer_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ECT_Throughput: $(addprefix $(VDIR),$(ECT_Throughput_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

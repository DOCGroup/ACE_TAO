# $Id$

BIN = ECM_Supplier ECM_Consumer

LDLIBS= -lorbsvcs -lTAO

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

IDL_HDR	= dataC dataS
IDL_SRC	= dataC dataS

ECM_Supplier_OBJS=$(addsuffix .o,ECM_Supplier ECM_Data $(IDL_SRC))
ECM_Consumer_OBJS=$(addsuffix .o,ECM_Consumer ECM_Data $(IDL_SRC))

SRC =   $(addsuffix .cpp, $(BIN)) \
	$(addsuffix .cpp, $(IDL_SRC)) \
	ECM_Data.cpp

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
	$(RM) -f dataC.* dataS.* dataS_T.*
	$(RM) -f $(BIN)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

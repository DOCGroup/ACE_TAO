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

LDLIBS = -lTAO

IDL_FILES = Quoter
IDL_SRC = QuoterC.cpp QuoterS.cpp
BIN = client server

SRC = $(addsuffix .cpp, $(BIN) Handler_i Stock_i Stock_Factory_i Stock_Factory_Locator_i) \
	$(IDL_SRC)

CLIENT_OBJS = $(addsuffix .o, client Handler_i) \
	$(IDL_SRC:.cpp=.o)
SERVER_OBJS = $(addsuffix .o, server Stock_i Stock_Factory_i Stock_Factory_Locator_i) \
	$(IDL_SRC:.cpp=.o)

TAO_IDLFLAGS=-GC -in

#----------------------------------------------------------------------------
# Include macros and targets
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

.PRECIOUS: $(foreach ext, $(IDL_EXT), Quoter$(ext))

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Quoter$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY


#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

BIN_UNCHECKED = client server middle ping pong
TEST_IDLFILES = testC testS
PING_IDLFILES = pingC pingS
FILES= \
	$(BIN) \
	$(TEST_IDLFILES) \
	$(PING_IDLFILES) \
	test_i ping_i

SRC = $(addsuffix .cpp, $(FILES))

CLIENT_OBJS = client.o test_i.o $(addsuffix .o, $(TEST_IDLFILES))
SERVER_OBJS = server.o test_i.o $(addsuffix .o, $(TEST_IDLFILES))
MIDDLE_OBJS = middle.o test_i.o $(addsuffix .o, $(TEST_IDLFILES))
PING_OBJS = ping.o ping_i.o $(addsuffix .o, $(PING_IDLFILES))
PONG_OBJS = pong.o ping_i.o $(addsuffix .o, $(PING_IDLFILES))

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)

BIN=$(BIN_UNCHECKED)

endif # endif

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), test$(ext))

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

middle: $(addprefix $(VDIR),$(MIDDLE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ping: $(addprefix $(VDIR),$(PING_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

pong: $(addprefix $(VDIR),$(PONG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext) ping$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

IDLFILES = testC testS
BIN = client server

SRC = $(addsuffix .cpp, $(BIN) $(IDLFILES) test_i client_moc test_i_moc)

CLIENT_OBJS = client.o testC.o client_moc.o 
SERVER_OBJS = server.o test_i.o test_i_moc.o $(addsuffix .o, $(IDLFILES))

TAO_IDLFLAGS += -Ge 1


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

.PRECIOUS: testC.h testC.i testC.cpp
.PRECIOUS: testS.h testS.i testS.cpp
.PRECIOUS: testS_T.h testS_T.i testS_T.cpp

ifneq ($(qt_reactor),)
test_i_moc.cpp: test_i.h
		moc test_i.h > test_i_moc.cpp

client_moc.cpp: client.h
		moc client.h > client_moc.cpp

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

endif #qt_reactor

realclean: clean
	-$(RM) testC.* testS.* testS_T.* *moc.cpp

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

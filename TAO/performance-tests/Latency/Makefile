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
BIN_UNCHECKED = client st_client deferred_synch_client server st_server

SRC = client.cpp \
	st_client.cpp \
	dii_client.cpp \
	server.cpp \
	st_server.cpp \
	testC.cpp \
	testS.cpp \
	ami-latency-client.cpp \
	ami-throughput-client.cpp

CLIENT_OBJS = client.o testC.o
ST_CLIENT_OBJS = st_client.o testC.o
DII_CLIENT_OBJS = dii_client.o testC.o
SERVER_OBJS = server.o test_i.o $(addsuffix .o, $(IDLFILES))
ST_SERVER_OBJS = st_server.o test_i.o $(addsuffix .o, $(IDLFILES))
AMI_LATENCY_CLIENT_OBJS = ami-latency-client.o  $(addsuffix .o, $(IDLFILES))
AMI_THROUGHPUT_CLIENT_OBJS = ami-throughput-client.o $(addsuffix .o, $(IDLFILES))

BUILD = $(BIN)
TAO_IDLFLAGS += -Ge 1

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(ami),1)
TAO_IDLFLAGS += -GC
endif # ami

ifeq ($(minimum_corba),0)
  BIN=$(BIN_UNCHECKED)
  ifeq ($(ami),1)
    BIN += ami-latency-client ami-throughput-client
  endif # ami
endif # minimum_corba

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

st_client: $(addprefix $(VDIR),$(ST_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

deferred_synch_client: $(addprefix $(VDIR),$(DII_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

st_server: $(addprefix $(VDIR),$(ST_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ami-latency-client: $(addprefix $(VDIR),$(AMI_LATENCY_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ami-throughput-client: $(addprefix $(VDIR),$(AMI_THROUGHPUT_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Test.java: test.idl
	idl2java test.idl

java_version: java_client.class java_server.class

java_client.class: Test.java
	vbjc java_client.java

java_server.class: Test.java
	vbjc java_server.java

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

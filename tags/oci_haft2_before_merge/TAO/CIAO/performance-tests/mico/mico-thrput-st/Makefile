#
# $Id$
all: .depend client server

include ../MakeVars

MICO_ROOT=/build/arvindk/mico-build

INSTALL_DIR     = throughput
INSTALL_SRCS    = Makefile client.cc server.cc RoundTrip.idl RoundTrip_impl.h
INSTALL_SCRIPTS = RoundTrip

CXXFLAGS += -I$(ACE_ROOT)
LDFLAGS_MICO  += -L$(ACE_ROOT)/ace -L$(MICO_ROOT)/lib
LD_CLI_LIBS += -lACE -lpthread -ldl -lrt  -lmicocoss2.3.10
LD_SERV_LIBS += -lmicocoss2.3.10

client: RoundTrip.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGS_MICO) RoundTrip.o client.o $(LDLIBS) $(LD_CLI_LIBS) -o $@

server: RoundTrip.o RoundTrip_impl.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) RoundTrip.o RoundTrip_impl.o server.o $(LDLIBS) $(LD_SERV_LIBS) -o $@

RoundTrip.h RoundTrip.cc : RoundTrip.idl $(IDLGEN)
	$(IDL) --poa RoundTrip.idl

run:
	RoundTrip

clean:
	$(RM) -f RoundTrip.cc RoundTrip.h *.o core client *.ior server *~ .depend

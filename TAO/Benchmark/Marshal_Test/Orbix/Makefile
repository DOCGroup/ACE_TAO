#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Orbix Marshal Benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     	= -lACE -lbenchmark 

PROG_SRCS 	= marshalC.cpp marshalS.cpp orbix_marshal_client.cpp orbix_marshal_server.cpp \
		  orbix_marshal_impl.cpp client.cpp server.cpp

LSRC = $(PROG_SRCS)

MARSHAL_CLIENT_OBJS = marshalC.o orbix_marshal_client.o client.o

ifdef dsi
MARSHAL_SERVER_OBJS = orbix_marshal_impl.o orbix_marshal_server.o server.o
else
MARSHAL_SERVER_OBJS = marshalS.o orbix_marshal_impl.o orbix_marshal_server.o server.o
endif
BIN = client server

BUILD = $(BIN)

#CLIENT_LIBS	= -lITini -liiop -lorbix
#SERVER_LIBS	= -lITini -liiop -lDSI -lorbix -lIRclt
CLIENT_LIBS	= -liiop -lorbix
SERVER_LIBS	= -liiop -lDSI -lorbix -lIRclt

VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
# include Orbix specific file
include /project/mambo/gokhale/CONFIG/orbixsol2s4.mk

#----------------------------------------------------------------------------
# Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/Benchmark

server:	$(addprefix $(VDIR),$(MARSHAL_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(SERVER_LIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(MARSHAL_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(CLIENT_LIBS) $(POSTLINK)

marshal.h marshalC.cpp marshalS.cpp: marshal.idl
	$(IDL) $(IDLFLAGS) $< 

clean:
	rm -f core *.o *~ client server *C.cpp *S.cpp *S.h *C.h
	$(MAKE) realclean





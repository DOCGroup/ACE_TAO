#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the VisiBroker Marshal Benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     	= -lACE -lbenchmark -lorb

PROG_SRCS 	= marshalC.cpp marshalS.cpp visi_marshal_client.cpp visi_marshal_server.cpp \
		  visi_marshal_impl.cpp client.cpp server.cpp

LSRC = $(PROG_SRCS)

MARSHAL_CLIENT_OBJS = marshalC.o visi_marshal_client.o client.o

MARSHAL_SERVER_OBJS = marshalC.o marshalS.o visi_marshal_impl.o visi_marshal_server.o server.o

BIN = client server

BUILD = $(BIN)

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
# include VisiBroker specific file
include /project/mambo/gokhale/CONFIG/orbeline_defaults.mk

#----------------------------------------------------------------------------
# Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/Benchmark -D_PMC_INCLUDE_DSI

server:	$(addprefix $(VDIR),$(MARSHAL_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(MARSHAL_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

marshalC.h marshalS.h marshalC.cpp marshalS.cpp: marshal.idl
	$(IDL) $(IDLFLAGS) $< 

clean:
	rm -f core *.o *~ client server *C.cpp *S.cpp *S.h *C.h
	$(MAKE) realclean


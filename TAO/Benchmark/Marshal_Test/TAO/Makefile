#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Tao Marshal Benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     	= -lACE -lbenchmark -lTAO

PROG_SRCS 	= marshalC.cpp marshalS.cpp tao_marshal_client.cpp tao_marshal_server.cpp \
		  tao_marshal_impl.cpp client.cpp server.cpp

LSRC = $(PROG_SRCS)

MARSHAL_CLIENT_OBJS = marshalC.o tao_marshal_client.o client.o

MARSHAL_SERVER_OBJS = marshalC.o marshalS.o tao_marshal_impl.o tao_marshal_server.o server.o

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

#----------------------------------------------------------------------------
# Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/Benchmark -I$(TAO_ROOT)/tao/compat
server:	$(addprefix $(VDIR),$(MARSHAL_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(MARSHAL_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clean:
	rm -f core *.o *~ client server
	$(MAKE) realclean








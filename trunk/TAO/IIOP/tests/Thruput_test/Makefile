#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO_TTCP benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     	= -lcorba

PROG_SRCS 	= client.cpp ttcpC.cpp ttcpS.cpp ttcp_i.cpp server.cpp utils.cpp

LSRC = $(PROG_SRCS)

TTCP_CLIENT_OBJS = client.o ttcpC.o utils.o

TTCP_SERVER_OBJS = server.o ttcpC.o ttcpS.o ttcp_i.o utils.o

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

LDFLAGS    += -L../../proto/lib
CPPFLAGS   += -I../../proto/include -DUSE_ACE_EVENT_HANDLING -DDEBUG #-DONEWAY

server:	$(addprefix $(VDIR),$(TTCP_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(TTCP_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)




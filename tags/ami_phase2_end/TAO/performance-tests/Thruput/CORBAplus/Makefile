#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Thruput benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     	= -lcorba -lunixsvc -lpbroker -lboadc -lnaming -llifecycl -lfsstream

IDL_SRC		= ttcp.cpp ttcp_s.cpp
PROG_SRCS 	= $(IDL_SRC) client.cpp ttcp_i.cpp server.cpp utils.cpp

LSRC = $(PROG_SRCS)

TTCP_CLIENT_OBJS = ttcp.o client.o utils.o

TTCP_SERVER_OBJS = ttcp.o ttcp_s.o server.o ttcp_i.o utils.o

BIN = client server

BUILD = $(BIN)

VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

exceptions = 1

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

LDFLAGS += -L$(PBHOME)/lib
CPPFLAGS += -I$(PBHOME)/include

$(IDL_SRC): ttcp.idl
	$(PBHOME)/bin/idlc ttcp.idl
	-mv ttcp.C ttcp.cpp
	-mv ttcp_s.C ttcp_s.cpp

server:	$(addprefix $(VDIR),$(TTCP_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(PBHOME)/lib/iiop_s.o $(VLDLIBS) $(POSTLINK)
#patch on the previous line to allow reading from an environment variable
#the host (interface) I would like the server to bind to. 
#The patch is "$(PBHOME)/lib/iiop_s.o"

client:	$(addprefix $(VDIR),$(TTCP_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) ttcp.cpp ttcp.h ttcp_s.cpp ttcp_s.h

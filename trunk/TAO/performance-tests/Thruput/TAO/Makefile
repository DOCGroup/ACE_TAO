#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO_TTCP benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS     	= -lTAO

IDL_SRC		= ttcpC.cpp ttcpS.cpp
PROG_SRCS 	= $(IDL_SRC) client.cpp ttcp_i.cpp server.cpp utils.cpp tmplinst.cpp

LSRC = $(PROG_SRCS)

TTCP_CLIENT_OBJS = ttcpC.o client.o utils.o tmplinst.o ttcpS.o

TTCP_SERVER_OBJS = ttcpC.o ttcpS.o server.o ttcp_i.o utils.o tmplinst.o

BIN = client server

BUILD = $(BIN)

VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
# Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

server:	$(addprefix $(VDIR),$(TTCP_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(TTCP_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) ttcpC.* ttcpS.* ttcpS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

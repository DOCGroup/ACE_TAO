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
endif

IDL_FILES = INS
IDL_SRC = \
	INSC.cpp \
	INSS.cpp

LDLIBS = -lTAO -lTAO_PortableServer

PROG_SRCS = \
	INS_test_client.cpp \
	INS_i.cpp \
	Server_i.cpp \
	INS_test_server.cpp

SRC = $(PROG_SRCS) $(IDL_SRC)

SIMPLE_CLIENT_OBJS = \
		INSC.o \
		INS_test_client.o \

SIMPLE_SERVER_OBJS = \
		INSC.o \
		INSS.o \
		INS_i.o \
		Server_i.o \
		INS_test_server.o

BIN = \
	INS_test_server \
        INS_test_client

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

INS_test_server:$(addprefix $(VDIR),$(SIMPLE_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ -lTAO_IORTable $(TAO_SRVR_LIBS) $(POSTLINK)

INS_test_client:$(addprefix $(VDIR),$(SIMPLE_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_CLNT_LIBS) $(POSTLINK)

.PRECIOUS: $(foreach ext, $(IDL_EXT), INS$(ext))

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), INS$(ext))

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

IDL_SRCS = \
	INSC.cpp \
	INSS.cpp

LDLIBS = -lTAO_CosNaming -lTAO_Svc_Utils -lTAO

PROG_SRCS = \
	INS_test_client.cpp \
	INS_i.cpp \
	Server_i.cpp \
	INS_test_server.cpp

SRC = $(PROG_SRCS) $(IDL_SRCS)

SIMPLE_CLIENT_OBJS = \
		INSC.o \
		INSS.o \
		INS_test_client.o \

SIMPLE_SERVER_OBJS = \
		INSC.o \
		INSS.o \
		INS_i.o \
		Server_i.o \
		INS_test_server.o

BIN2 = \
	INS_test_server \
        INS_test_client

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Naming

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


LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

INS_test_server:$(addprefix $(VDIR),$(SIMPLE_SERVER_OBJS))
		$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

INS_test_client:$(addprefix $(VDIR),$(SIMPLE_CLIENT_OBJS))
		$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

.PRECIOUS: $(foreach ext, $(IDL_EXT), INS$(ext))

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), INS$(ext))

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

LDLIBS =  -lTAO_PSDL_DataStore -lTAO_PortableServer -lTAO

PSDL_FILES_EXT = psdl
PSDL_FILES = naming_data
IDL_FILES = Simple_Naming Server

PSDL_SRC = naming_dataC.cpp
IDL_SRC = Simple_NamingC.cpp Simple_NamingS.cpp ServerC.cpp ServerS.cpp
SIMPLE_NAMING_SRC = Simple_Naming.cpp Simple_Naming_i.cpp 
SERVER_SRC = Server_i.cpp ServerC.cpp ServerS.cpp server.cpp
CLIENT_SRC = client.cpp

LSRC = $(IDL_SRC) $(PSDL_SRC) $(SIMPLE_SERVER_SRC) $(SERVER_SRC) $(CLIENT_SRC)

PSDL_TAO = $(TAO_ROOT)/orbsvcs/PSS/psdl_tao

SIMPLE_NAMING_OBJS = \
	naming_dataC.o \
	Simple_NamingC.o \
	Simple_NamingS.o \
	Simple_Naming_i.o \
	Simple_Naming.o

SERVER_OBJS = \
	ServerC.o \
	ServerS.o \
	Server_i.o \
	Simple_NamingS.o \
	Simple_NamingC.o \
	Simple_Naming_i.o \
	server.o

CLIENT_OBJS = \
	Simple_NamingS.o \
	Simple_NamingC.o \
	Simple_Naming_i.o \
	ServerC.o \
	client.o

BIN = Simple_Naming server client

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

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

%C.cpp : %.psdl
	$(PSDL_TAO) $<

Simple_Naming : $(addprefix $(VDIR),$(SIMPLE_NAMING_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server : $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client : $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

.PRECIOUS: %C.cpp

realclean:
	-$(RM) Simple_NamingC.* Simple_NamingS* naming_dataC.* ServerC.* ServerS* Naming_Server server client

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

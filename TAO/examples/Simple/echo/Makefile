#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO -lorbsvcs

IDL_SRC	= EchoC.cpp EchoS.cpp

PROG_SRCS = \
	client.cpp \
	Client_i.cpp \
	server.cpp \
	Server_i.cpp \
	Echo_i.cpp \

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
		EchoC.o \
		EchoS.o \
		Client_i.o \
		client.o
SIMPLE_SVR_OBJS = \
		EchoC.o \
		EchoS.o \
		Echo_i.o \
		Server_i.o \
		server.o

BIN = server \
      client
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
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

LDFLAGS +=  -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: EchoC.cpp EchoC.i EchoC.h
.PRECIOUS: EchoS.cpp EchoS.i EchoS.h
.PRECIOUS: EchoS_T.cpp EchoS_T.i EchoS_T.h

server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf EchoC.* EchoS.* EchoS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/EchoC.o .obj/EchoC.so .shobj/EchoC.o .shobj/EchoC.so: EchoC.cpp EchoC.h EchoC.i EchoS.h EchoS_T.h EchoS_T.i \
 EchoS_T.cpp EchoS.i
.obj/EchoS.o .obj/EchoS.so .shobj/EchoS.o .shobj/EchoS.so: EchoS.cpp EchoS.h EchoC.h EchoC.i EchoS_T.h EchoS_T.i \
 EchoS_T.cpp EchoS.i
.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp Client_i.h EchoC.h EchoC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/Client_i.o .obj/Client_i.so .shobj/Client_i.o .shobj/Client_i.so: Client_i.cpp Client_i.h EchoC.h EchoC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i
.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp Server_i.h Echo_i.h EchoS.h EchoC.h EchoC.i \
 EchoS_T.h EchoS_T.i EchoS_T.cpp EchoS.i \
 $(TAO_ROOT)/tao/TAO.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/Server_i.o .obj/Server_i.so .shobj/Server_i.o .shobj/Server_i.so: Server_i.cpp Server_i.h Echo_i.h EchoS.h EchoC.h EchoC.i \
 EchoS_T.h EchoS_T.i EchoS_T.cpp EchoS.i \
 $(TAO_ROOT)/tao/TAO.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/Echo_i.o .obj/Echo_i.so .shobj/Echo_i.o .shobj/Echo_i.so: Echo_i.cpp Echo_i.h EchoS.h EchoC.h EchoC.i EchoS_T.h \
 EchoS_T.i EchoS_T.cpp EchoS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

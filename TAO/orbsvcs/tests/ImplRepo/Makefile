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
endif # ! TAO_ROOT

LDLIBS = -lTAO
CPPFLAGS += -I$(TAO_ROOT)

IDL_SRC = AirplaneS.cpp AirplaneC.cpp NesteaS.cpp NesteaC.cpp
PROG_SRCS = \
            airplane_client.cpp \
            airplane_server.cpp \
            airplane_client_i.cpp \
            airplane_server_i.cpp \
            airplane_i.cpp \
            nestea_client.cpp \
            nestea_server.cpp \
            nestea_client_i.cpp \
            nestea_server_i.cpp \
            nestea_i.cpp

SRC=$(IDL_SRC) $(PROG_SRCS)

AIRPLANE_CLIENT_OBJS =  AirplaneC.o \
                        AirplaneS.o \
                        airplane_client.o \
                        airplane_client_i.o

AIRPLANE_SERVER_OBJS =  AirplaneC.o \
                        AirplaneS.o \
                        airplane_server.o \
                        airplane_server_i.o \
                        airplane_i.o

NESTEA_CLIENT_OBJS =  NesteaC.o \
                      NesteaS.o \
                      nestea_client.o \
                      nestea_client_i.o

NESTEA_SERVER_OBJS =  NesteaC.o \
                      NesteaS.o \
                      nestea_server.o \
                      nestea_server_i.o \
                      nestea_i.o

BIN = airplane_server \
      airplane_client \
      nestea_server \
      nestea_client 

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

IDL_EXT=C.h C.i C.cpp S.h S.i S.cpp S_T.h S_T.i S_T.cpp

.PRECIOUS: $(foreach ext, $(IDL_EXT), Airplane$(ext))

airplane_server: $(addprefix $(VDIR),$(AIRPLANE_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

airplane_client: $(addprefix $(VDIR),$(AIRPLANE_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

nestea_server: $(addprefix $(VDIR),$(NESTEA_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

nestea_client: $(addprefix $(VDIR),$(NESTEA_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Airplane$(ext)) \
               $(foreach ext, $(IDL_EXT), Nestea$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

# $Id$
#
#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

IDL_FILES = simple

IDL_SRC = \
	$(addsuffix S.cpp, $(IDL_FILES)) \
	$(addsuffix C.cpp, $(IDL_FILES))

BIN = client server

CPP_SRCS = $(addsuffix .cpp, $(BIN)) $(IDL_SRC)

CLIENT_OBJS   = simpleC.o client.o
SERVER_OBJS   = simpleS.o server.o $(IDL_SRC:.cpp=.o)

TAO_IDLFLAGS += -Ge 1

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

simpleC.cpp:	 simple.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl -Ge 1 -Wb,export_macro=TAO_Export \
	simple.idl

simpleS.cpp:	 simple.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl -Ge 1 -Wb,export_macro=TAO_Export \
	simple.idl

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ -lTAO_Messaging $(TAO_SRVR_LIBS) \
	$(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ -lTAO_Messaging $(TAO_CLNT_LIBS) \
	$(POSTLINK)

clobber.local:
	$(RM) -f simple[CS]*.*
	$(RM) -f client server

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_FILES = test
IDL_SRC = testC.cpp testS.cpp
BIN_UNCHECKED = client server

SRC = $(addsuffix .cpp, $(BIN_UNCHECKED) test_i FOO_ClientRequestInterceptor FOO_Client_ORBInitializer FOO_IORInterceptor FOO_IORInterceptor_ORBInitializer) $(IDL_SRC)

CLIENT_OBJS = \
  client.o \
  testC.o \
  FOO_ClientRequestInterceptor.o \
  FOO_Client_ORBInitializer.o

SERVER_OBJS = \
  server.o \
  test_i.o \
  FOO_IORInterceptor.o
  FOO_IORInterceptor_ORBInitializer.o \
  $(IDL_SRC:.cpp=.o)

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(interceptors), 1)
BIN = $(BIN_UNCHECKED)
endif # interceptors

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), test$(ext))

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_SRVR_LIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_CLNT_LIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))


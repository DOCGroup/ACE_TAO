#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

IDL_FILES = ORT_test
TAO_IDLFLAGS += -I../../..
IDL_SRC = ORT_testC.cpp ORT_testS.cpp
BIN_UNCHECKED = client server 

SRC = $(addsuffix .cpp, $(BIN_UNCHECKED) ORT_test_i ORT_test_IORInterceptor_ORBInitializer ORT_test_IORInterceptor) $(IDL_SRC)

CLIENT_OBJS = \
  client.o \
  ORT_testC.o

SERVER_OBJS = \
  ORT_testC.o \
  ORT_testS.o \
  ORT_test_i.o \
  ORT_test_IORInterceptor_ORBInitializer.o \
  ORT_test_IORInterceptor.o \
  server.o

TAO_IDLFLAGS += -Ge 1

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#ifeq ($(interceptors), 1)
BIN = $(BIN_UNCHECKED)
#endif # interceptors

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), Gateway$(ext) sum_server$(ext))

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_SRVR_LIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_CLNT_LIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Gateway$(ext) sum_server$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

LDLIBS = -lTAO

IDL_FILES = test
IDL_SRC = testC.cpp testS.cpp
PROG_SRCS = $(IDL_SRC) server.cpp client.cpp 

LSRC = $(PROG_SRCS)

SVR_OBJS = testC.o testS.o server.o 
CLT_OBJS = testC.o client.o

BIN_UNCHECKED = server client

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)
  BIN = $(BIN_UNCHECKED)
endif # minimum_corba

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

.PRECIOUS: testC.h testC.i testC.cpp testS.h testS.i testS.cpp

server:	$(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_SRVR_LIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(TAO_SRVR_LIBS) $(POSTLINK)

realclean: clean
	-$(RM) testC.* testS.* testS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

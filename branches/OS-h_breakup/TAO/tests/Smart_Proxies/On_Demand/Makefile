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

LDLIBS  = -lTAO_PortableServer -lTAO_SmartProxies -lTAO
ACE_SHLIBS = -lTAO_PortableServer -lTAO_SmartProxies -lTAO -lACE

LIBNAME = libSmart_Test_Factory
SHLIB   = $(LIBNAME).$(SOEXT)
LIB     = $(LIBNAME).a

IDL_FILES = test
IDL_SRC = testC testS

SHLIB_SRC = $(IDL_SRC) Smart_Test_Factory Smart_Proxy_Impl

CLIENT_FILES = $(IDL_SRC) client
CLIENT_SRCS = $(addsuffix .cpp, $(CLIENT_FILES))
#CLIENT_OBJS = $(addprefix $(VDIR),$(addsuffix .o,$(CLIENT_FILES)))
CLIENT_OBJS = $(addsuffix .o,$(CLIENT_FILES))

SERVER_FILES = $(IDL_SRC) server
SERVER_SRCS = $(addsuffix .cpp, $(SERVER_FILES))
#SERVER_OBJS = $(addprefix $(VDIR),$(addsuffix .o,$(SERVER_FILES)))
SERVER_OBJS = $(addsuffix .o,$(SERVER_FILES))

BIN = client server
LSRC = $(addsuffix .cpp,$(SHLIB_SRC))
LOBJ = $(addsuffix .o,$(SHLIB_SRC))
PSRC = $(addsuffix .cpp,$(BIN))

TAO_IDLFLAGS += -Ge 1 -Gsp
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS  += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/tao

.PRECIOUS: testC.h testC.i testC.cpp
.PRECIOUS: testS.h testS.i testS.cpp

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

IDL_FILES = corbaloc
IDL_SRC = corbalocC corbalocS

LDLIBS = -lTAO_CosNaming -lTAO_PortableServer -lTAO 
#CPPFLAGS += -I$(TAO_ROOT)/orbsvcs
BIN = server client corbaname_client

#SRC = $(addsuffix .cpp, $(BIN) $(IDL_SRC) status_i)

CLIENT_OBJS = $(addsuffix .o, $(IDL_SRC) client ior_corbaloc_client_i)
SERVER_OBJS = $(addsuffix .o, $(IDL_SRC) server status_i)
CORBANAME_CLIENT_OBJS = $(addsuffix .o, $(IDL_SRC) corbaname_client status_i)

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
# Include macros and targets
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

.PRECIOUS: $(foreach ext, $(IDL_EXT), corbaloc$(ext))

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

corbaname_client: $(addprefix $(VDIR),$(CORBANAME_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), corbaloc$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

#----------------------------------------------------------------------------
#
#       $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

IDL_FILES += \
        test

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))

BIN_UNCHECKED = client server

SRC = $(addsuffix .cpp, $(BIN) $(IDLFILES) test_i) $(IDL_SRC)

CLIENT_OBJS = client.o testC.o testS.o
SERVER_OBJS = server.o test_i.o testC.o testS.o

TAO_IDLFLAGS += -Ge 1

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.

ifeq ($(ssl),1)
  BIN = $(BIN_UNCHECKED)
endif  # ssl == 1

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk


ifeq ($(static_libs_only),1)
CLNT_LIBS += -lTAO_SSLIOP
SRVR_LIBS += -lTAO_SSLIOP
endif  # static_libs_only = 1

CLNT_LIBS += -lTAO_PortableServer $(TAO_CLNT_LIBS)
SRVR_LIBS +=  $(TAO_SRVR_LIBS)

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

# Extra dependencies not caught by make depend.

.PRECIOUS: $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext))))

realclean: clean
	-$(RM) $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext)))

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(SRVR_LIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(CLNT_LIBS) $(POSTLINK)


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

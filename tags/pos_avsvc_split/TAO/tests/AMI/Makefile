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

LDLIBS = -lTAO

IDL_FILES	=	ami_test	

IDL_SRCS	= 	$(addsuffix C, $(IDL_FILES)) \
	  		$(addsuffix S, $(IDL_FILES))

BIN =  server simple_client client

CLIENT_OBJS = $(addsuffix .o, $(IDL_SRCS)) client.o
SIMPLE_CLIENT_OBJS = $(addsuffix .o, $(IDL_SRCS)) simple_client.o
SERVER_OBJS = $(addsuffix .o, $(IDL_SRCS)) server.o ami_test_i.o

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

override TAO_IDLFLAGS += -GC

.PRECIOUS: $(foreach ext, $(IDL_EXT), test$(ext))

all: server client simple_client

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

simple_client: $(addprefix $(VDIR),$(SIMPLE_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), ami_test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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
endif

LDLIBS = -lTAO

IDL_FILES = UDP
IDL_SRC = UDPC.cpp UDPS.cpp

PROG_SRCS = \
        client.cpp \
        server.cpp \
        UDP_i.cpp \
        UDP_Client_i.cpp

SRC = $(IDL_SRC) $(PROG_SRCS)

CLT_OBJS = \
           UDPC.o \
           UDPS.o \
           UDP_i.o \
           client.o \
           UDP_Client_i.o

SVR_OBJS = \
           UDPC.o \
           UDPS.o \
           UDP_i.o \
           server.o

BIN2 = server \
       client

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


LDFLAGS +=  -L$(TAO_ROOT)/tao 

.PRECIOUS: UDPC.cpp UDPC.i UDPC.h
.PRECIOUS: UDPS.cpp UDPS.i UDPS.h
.PRECIOUS: UDPS_T.cpp UDPS_T.i UDPS_T.h

server: $(addprefix $(VDIR),$(SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) -rf UDPC.* UDPS.* UDPS_T.*

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

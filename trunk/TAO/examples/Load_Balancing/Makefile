# $Id$
#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

override TAO_IDLFLAGS += -Ge 1

LDLIBS = -lTAO

IDL_SRC = \
        Load_BalancerC.cpp \
        Load_BalancerS.cpp \
        IdentityC.cpp \
        IdentityS.cpp

PROG_SRCS = \
        Load_Balancing_Service.cpp \
        Identity_Server.cpp \
        Identity_Client.cpp \
        Load_Balancer_i.cpp \
        Identity_i.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CLIENT_OBJS = Identity_Client.o \
              IdentityC.o \
              Load_BalancerC.o

SERVER_OBJS = Identity_Server.o \
              Identity_i.o \
              IdentityC.o \
              IdentityS.o \
              Load_BalancerC.o

LOAD_BALANCER_OBJS = Load_Balancer_i.o \
                     Load_BalancerC.o \
                     Load_BalancerS.o \
                     Load_Balancing_Service.o

BIN2 = load_balancer server client

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifneq (1,$(repo))
  #### ld on Linux (RedHat 6.0) with egcs 2.91.66 seg faults with repo
  #### when trying to link load_balancer.
  BIN = $(BIN2)
endif # ! repo

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------


LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

realclean: clean
	-$(RM) *C.* *S.* *S_T.*

.PRECIOUS: $(IDL_SRC)

load_balancer: $(addprefix $(VDIR),$(LOAD_BALANCER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

# $Id$
#
# needed for the Logging service stuff

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

override TAO_IDLFLAGS += -I$(TAO_ROOT)/orbsvcs/orbsvcs -Ge 1

# The following lines tell the souce code that the trading
# service is available.

LDLIBS = -lorbsvcs -lTAO

IDL_SRC = \
        LoggerC.cpp LoggerS.cpp

SERVICE_SRCS = \
        Logger_i.cpp Logging_Service.cpp Logging_Service_i.cpp

TEST_SRCS = \
	Logging_Test.cpp Logging_Test_i.cpp

LSRC= \
        $(IDL_SRC) $(SERVICE_SRCS) $(TEST_SRCS)

COMMON_OBJS = LoggerC.o LoggerS.o
SERVICE_OBJS = $(COMMON_OBJS) Logger_i.o Logging_Service.o Logging_Service_i.o
TEST_OBJS = $(COMMON_OBJS) Logging_Test.o Logging_Test_i.o

BIN2 = Logging_Service Logging_Test

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  ifeq (LifeCycle,$(findstring LifeCycle,$(TAO_ORBSVCS)))
    BIN = $(BIN2)
  endif # LifeCycle
endif # Naming

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### Local rules and variables...

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs \
        -I$(TAO_ROOT)
TAO_IDLFLAGS +=  -I$(TAO_ROOT)/orbsvcs/orbsvcs

all: Logging_Service Logging_Test

realclean: clean
	-$(RM) *C.* *S.* *S_T.*

.PRECIOUS: $(IDL_SRC)

Logging_Service: $(addprefix $(VDIR),$(SERVICE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Logging_Test: $(addprefix $(VDIR),$(TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

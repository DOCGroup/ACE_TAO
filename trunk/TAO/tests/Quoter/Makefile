#
# $Id$
#
# needed for the trading service stuff

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

TAO_IDLFLAGS=-I$(TAO_ROOT)/orbsvcs/orbsvcs

ifdef trader

ifndef STL_ROOT
  STL_ROOT=/project/doc/irfan/ObjectSpace/SC4.0
endif # STL_ROOT

STD=$(STL_ROOT)/ospace/std
STL=$(STL_ROOT)/stl

include $(STL_ROOT)/config/local.cfg

CC_INCLUDES += -I$(STD)
CC_INCLUDES += -pta

CPPFLAGS += \
        -I$(TOOLKIT) \
        $(CC_INCLUDES) \
        $(CC_FLAGS) \
        $(CC_DEFINES) \
        $(CC_EH) \
        $(CC_MT)


ifndef exceptions
CPPFLAGS += -DOS_NO_EXCEPTION_SPECIFIER
else
CPPFLAGS += -DOS_STL_ASSERT
endif

LDFLAGS += -L$(STL_ROOT)/lib

endif # trader
# end of the trading service stuff

BIN = client server Factory_Finder Generic_Factory

# The following lines tell the souce code that the trading
# service is available.

ifdef trader

CPPFLAGS += -DTRADER_AVAILABLE

endif # trader

BUILD = $(BIN)


IDL_SRC	= \
	QuoterC.cpp QuoterS.cpp

CLIENT_SRCS = \
	client.cpp

SERVER_SRCS = \
	server.cpp Quoter_Impl.cpp

FACTORY_FINDER_SRCS = \
	Factory_Finder.cpp Factory_Finder_Impl.cpp

GENERIC_FACTORY_SRCS = \
	Generic_Factory.cpp Generic_Factory_Impl.cpp


LSRC= \
	$(IDL_SRC) $(CLIENT_SRCS) $(SERVER_SRCS) \
	$(FACTORY_FINDER_SRCS) $(GENERIC_FACTORY_SRCS)

COMMON_OBJS = QuoterC.o QuoterS.o
CLIENT_OBJS = $(COMMON_OBJS) client.o
SERVER_OBJS = $(COMMON_OBJS) server.o Quoter_Impl.o
FACTORY_FINDER_OBJS = $(COMMON_OBJS) Factory_Finder.o Factory_Finder_Impl.o
GENERIC_FACTORY_OBJS = $(COMMON_OBJS) Generic_Factory.o Generic_Factory_Impl.o

ifdef trader

LDLIBS = -lorbsvcs -lospace -lTAO

else

LDLIBS = -lorbsvcs -lTAO

endif

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### Local rules and variables...

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/orbsvcs/orbsvcs \
	-I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

realclean: clean
	-/bin/rm -rf QuoterC.* QuoterS.* QuoterS_T.*

$(IDL_SRC): Quoter.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl Quoter.idl -I$(TAO_ROOT)/orbsvcs/orbsvcs

.PRECIOUS: $(IDL_SRC)

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Factory_Finder: $(addprefix $(VDIR),$(FACTORY_FINDER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Generic_Factory: $(addprefix $(VDIR),$(GENERIC_FACTORY_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

IDL_SRC	= cubitC.cpp cubitS.cpp
PROG_SRCS = Cubit_Server.cpp \
	    server.cpp \
            Cubit_Client.cpp \
            collocation_test.cpp \
	    client.cpp \
	    Cubit_Client.cpp \
	    RTI_IO.cpp \
	    tmplinst.cpp \
	    Cubit_i.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CUBIT_SVR_OBJS = cubitC.o \
		 cubitS.o \
		 Cubit_Server.o \
		 Cubit_Client.o \
		 Cubit_i.o \
		 server.o \
		 RTI_IO.o \
		 tmplinst.o
CUBIT_CLT_OBJS = cubitC.o \
		 cubitS.o \
		 Cubit_Client.o \
		 client.o \
		 RTI_IO.o \
		 tmplinst.o
COLLOCATION_OBJS = cubitC.o \
		   cubitS.o \
		   Cubit_Server.o \
		   Cubit_Client.o \
		   Cubit_i.o \
		   collocation_test.o \
		   RTI_IO.o \
		   tmplinst.o

BIN = server \
      client \
      collocation/collocation_test
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao

.PRECIOUS: cubitC.cpp cubitS.cpp cubitC.h cubitS.h

server:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(CUBIT_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

collocation/collocation_test: $(addprefix $(VDIR),$(COLLOCATION_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) cubitC.* cubitS.* cubitS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

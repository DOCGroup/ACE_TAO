#----------------------------------------------------------------------------
#
#	$Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS     =  -lTAO -lorbsvcs $(MATHLIB)

IDL_SRC	= cubitC.cpp cubitS.cpp
PROG_SRCS = server.cpp \
	cubit_i.cpp \
	Task_Client.cpp \
	client.cpp \
	Util_Thread.cpp \
	tmplinst.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CUBIT_SVR_OBJS = \
	cubitC.o \
	cubitS.o \
	server.o \
	cubit_i.o \
	tmplinst.o

CUBIT_CLIENT_OBJS = \
	cubitC.o \
	cubitS.o \
	Task_Client.o \
	client.o \
	Util_Thread.o \
	tmplinst.o 

BIN = server client
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
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# Local modifications to variables imported by includes above.
#CPPFLAGS += -DACE_NLOGGING

#CXX := purify $(CXX)

LDFLAGS += -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/tao/compat

# pccTimer will eventually get integrated into ACE.  It is specific for ClassiX on VME
ifdef CHORUS
	SRC += pccTimer.cpp
	CLT_OBJS += pccTimer.o
	LDLIBS += -lOrb
else
	LDLIBS += -lOrb-mt
endif

.PRECIOUS: cubitC.h cubitC.i cubitC.cpp cubitS.h cubitS.i cubitS.cpp

server:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(CUBIT_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)  $(POSTLINK)

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf cubitC.* cubitS.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

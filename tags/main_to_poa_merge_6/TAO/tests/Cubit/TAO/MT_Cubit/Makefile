#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the ACE-ified Sun Ref. implementation
#	of IIOP ORB
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lTAO

IDL_SRC	= cubitC.cpp cubitS.cpp
PROG_SRCS = svr.cpp cubit_i.cpp Task_Client.cpp client.cpp Util_Thread.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CUBIT_SVR_OBJS = cubitC.o cubitS.o svr.o cubit_i.o 
CUBIT_CLIENT_OBJS = cubitC.o Task_Client.o client.o Util_Thread.o

BIN = svr client
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# Local modifications to variables imported by includes above.
#CPPFLAGS += -DACE_NLOGGING
CPPFLAGS += -DPRIORITY_INV_TEST

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
#CXX := purify $(CXX)
##DCFLAGS = -g
OCFLAGS =
LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat

$(IDL_SRC): cubit.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl cubit.idl

svr:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(CUBIT_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf cubitC.* cubitS.*
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Param_Test test example
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lTAO
LDFLAGS += -L$(TAO_ROOT)/tao

IDL_HDR	= param_testC.h param_testS.h
IDL_SRC	= param_testC.cpp param_testS.cpp
PROG_SRCS =	$(IDL_SRC) server.cpp client.cpp param_test_i.cpp helper.cpp \
		driver.cpp options.cpp results.cpp tests.cpp tmplinst.cpp

LSRC = $(PROG_SRCS)

PARAM_TEST_SVR_OBJS = param_testC.o param_testS.o server.o param_test_i.o tmplinst.o
PARAM_TEST_CLT_OBJS = param_testC.o param_testS.o driver.o client.o options.o results.o \
			tmplinst.o tests.o helper.o

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

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
DCFLAGS = -g
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

$(IDL_HDR) $(IDL_SRC): param_test.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl param_test.idl

server:	$(addprefix $(VDIR),$(PARAM_TEST_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(PARAM_TEST_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf param_testC.* param_testS.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Logger Service of the TAO ORB
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lorbsvcs -lTAO

IDL_SRC	= loggerC.cpp loggerS.cpp
PROG_SRCS =	$(IDL_SRC) logger_i.cpp 

LSRC = $(PROG_SRCS)

logger_SVR_OBJS = loggerC.o loggerS.o svr.o logger_i.o 
logger_CLT_OBJS = loggerC.o clnt.o

BIN = svr clnt
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

$(IDL_SRC): logger.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl logger.idl

svr:	$(addprefix $(VDIR),$(logger_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clnt:	$(addprefix $(VDIR),$(logger_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

########
# Sanity check builds by running basic functionality tests.
#
# "sleep 5" in the server startup is usually enough to get the
# objref into the file so the client can read it.
#
check:	$(TESTS)
	@echo "testing with 'cube' calls, stub + DII, IOR strings"
	@./svr -i30 -o non-internet > obj.1 & sleep 5
	@./clnt -n250 -O `cat obj.1` -x
	@echo ''
	@echo "testing request forwarding with 'cube' calls, stub + DII"
	@./svr -f -i30 > obj.2 & sleep 5
	@./clnt -n250 -O `cat obj.2` -x
	@echo ''
	@echo "testing transmission of primitive data types"
	@./test1_svr -i30 > obj.3 & sleep 5
	@./test1_clnt -n50 -O `cat obj.3` -x
	@echo ''
#	@echo "testing echo of primitive data values"
#	@./echo_svr -i30 > obj.4 & sleep 5
#	@./echo_clnt -O `cat obj.4` -x
#	@echo ''
	@echo "testing with 'cube' calls, MT-ized (no forwarding)"
	@./svr -t -i30 -o non-internet > obj.5 & sleep 5
	@./clnt -n250 -O `cat obj.5` -x
	@echo ''

	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf loggerC.* loggerS.* 

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

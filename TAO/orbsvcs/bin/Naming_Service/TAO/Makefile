#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Naming Service of the TAO ORB
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lTAO

IDL_SRC	= CosNamingC.cpp CosNamingS.cpp loggerC.cpp

PROG_SRCS =	$(IDL_SRC) NS_CosNaming.cpp svr.cpp clnt.cpp

LSRC = $(PROG_SRCS)

COSNAMING_SVR_OBJS = CosNamingC.o CosNamingS.o svr.o CosNaming_i.o NS_CosNaming.o
COSNAMING_CLT_OBJS = CosNamingC.o clnt.o loggerC.o

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
LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

$(IDL_SRC): CosNaming.idl logger.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl CosNaming.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl logger.idl

svr:	$(addprefix $(VDIR),$(COSNAMING_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clnt:	$(addprefix $(VDIR),$(COSNAMING_CLT_OBJS))
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

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf CosNamingC.* CosNamingS.*

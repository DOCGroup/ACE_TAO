#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the ACE-ified Sun Ref. implementation
#	of IIOP ORB
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS = -lTAO

IDL_SRC	= FileC.cpp FileS.cpp
PROG_SRCS =	$(IDL_SRC) server.cpp client.cpp File_i.cpp

LSRC = $(PROG_SRCS)

FILE_SVR_OBJS = FileC.o FileS.o server.o File_i.o
FILE_CLT_OBJS = FileC.o FileS.o client.o

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
include $(TAO_ROOT)/taoconfig.mk

DCFLAGS = -g

#$(IDL_SRC): File.idl
#	$(TAO_ROOT)/TAO_IDL/tao_idl File.idl

server:	$(addprefix $(VDIR),$(FILE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(FILE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

########
# Sanity check builds by running basic functionality tests.
#
# "sleep 5" in the server startup is usually enough to get the
# objref into the file so the client can read it.
#
check:	$(TESTS)
	@echo "testing with 'cube' calls, stub + DII, IOR strings"
	@./server -i30 -o non-internet > obj.1 & sleep 5
	@./client -n250 -O `cat obj.1` -x
	@echo ''
	@echo "testing request forwarding with 'cube' calls, stub + DII"
	@./server -f -i30 > obj.2 & sleep 5
	@./client -n250 -O `cat obj.2` -x
	@echo ''
	@echo "testing transmission of primitive data types"
	@./test1_server -i30 > obj.3 & sleep 5
	@./test1_client -n50 -O `cat obj.3` -x
	@echo ''
#	@echo "testing echo of primitive data values"
#	@./echo_server -i30 > obj.4 & sleep 5
#	@./echo_client -O `cat obj.4` -x
#	@echo ''
	@echo "testing with 'cube' calls, MT-ized (no forwarding)"
	@./server -t -i30 -o non-internet > obj.5 & sleep 5
	@./client -n250 -O `cat obj.5` -x
	@echo ''

#clean:
#	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf FileC.* FileS.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

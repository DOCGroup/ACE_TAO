#----------------------------------------------------------------------------
#	$Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lcorba -lunixsvc -lpbroker -lboadc -lnaming -llifecycl -lfsstream

IDL_SRC	= cubit.cpp cubit_s.cpp
PROG_SRCS =	$(IDL_SRC) svr.cpp clnt.cpp

LSRC = $(PROG_SRCS)

CUBIT_SVR_OBJS = cubit.o cubit_s.o svr.o cubit_i.o
CUBIT_CLT_OBJS = cubit.o clnt.o

BIN = svr clnt
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

exceptions = 1

#include $(PBHOME)/make/make.cfg

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#$(VDIR)%.o:     %.C
#VOBJS   = $(LSRC:%.C=$(VDIR)%.o)

LDFLAGS += -L$(PBHOME)/lib
CPPFLAGS += -I$(PBHOME)/include 

$(IDL_SRC): cubit.idl
	$(PBHOME)/bin/idlc cubit.idl
	-/bin/mv cubit.C cubit.cpp
	-/bin/mv cubit_s.C cubit_s.cpp

svr:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

clnt:	$(addprefix $(VDIR),$(CUBIT_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

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
	-/bin/rm -rf *.o cubit.cpp cubit.h cubit_s.cpp cubit_s.h $(CUBIT_SVR_OBJS) $(CUBIT_CLT_OBJS) $(BIN) core
 
realclean: clean


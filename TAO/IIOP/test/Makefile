#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the ACE-ified Sun Ref. implementation
#	of IIOP ORB
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lcorba

PROG_SRCS =	svr.cpp clnt.cpp cubit.cpp \
		test1.cpp test1_clnt.cpp test1_svr.cpp \
		echo_clnt.cpp echo_svr.cpp

LSRC = $(PROG_SRCS)

CUBIT_SVR_OBJS = svr.o cubit_i.o cubitS.o
CUBIT_CLT_OBJS = clnt.o cubitC.o

BASIC_SVR_OBJS = test1.o test1_svr.o
BASIC_CLT_OBJS = test1.o test1_clnt.o

ECHO_SVR_OBJS = test1.o echo_svr.o
ECHO_CLT_OBJS = test1.o echo_clnt.o

BIN = svr clnt test1_svr test1_clnt
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

# Local modifications to variables imported by includes above.
LDFLAGS    += -L../proto/lib
CPPFLAGS   += -I../proto/include -DUSE_ACE_EVENT_HANDLING

svr:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clnt:	$(addprefix $(VDIR),$(CUBIT_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

test1_svr:	$(addprefix $(VDIR),$(BASIC_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

test1_clnt:	$(addprefix $(VDIR),$(BASIC_CLT_OBJS))
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

ifdef LOCALRULES
########
# CUBIT test
svr:		svr.o cubit.o
	$(LINK.cc) -o svr svr.o cubit.o $(LDLIBS)
clnt:		cubit.o clnt.o
	$(LINK.cc) -o clnt clnt.o cubit.o $(LDLIBS)

########
# BASIC DATATYPES test
test1_clnt:	test1.o test1_clnt.o
	$(LINK.cc) -o test1_clnt test1_clnt.o test1.o $(LDLIBS)
test1_svr:	test1.o test1_svr.o
	$(LINK.cc) -o test1_svr test1_svr.o test1.o $(LDLIBS)

########
# ECHO test ... "test1" where the operation semantics are violated;
# this aids some porting work, but is a less rigorous test
echo_clnt:	test1.o echo_clnt.o
	$(LINK.cc) -o echo_clnt echo_clnt.o test1.o $(LDLIBS)
echo_svr:	test1.o echo_svr.o
	$(LINK.cc) -o echo_svr echo_svr.o test1.o $(LDLIBS)
endif

clean:
	-rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

install:
	-@echo "Nothing to install, these are tests!"






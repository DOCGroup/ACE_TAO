#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the ACE-ified Sun Ref. implementation
#	of IIOP ORB
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

#LDLIBS     =  -lmtcorba -lmtunixsvc -lmtpbroker -lmtboadc -lmtnaming -lmtlifecycl -lmtfsstream
LDLIBS     =  -lcorba -lunixsvc -lpbroker -lboadc -lnaming -llifecycl -lfsstream

IDL_SRC	= cubit.cpp cubit_s.cpp
PROG_SRCS =	svr.cpp cubit_i.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CUBIT_SVR_OBJS = cubit.o cubit_s.o svr.o cubit_i.o

BIN = svr
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

#enable C++ exceptions
exceptions = 1

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# Local modifications to variables imported by includes above.
LDFLAGS += -L$(PBHOME)/lib
CPPFLAGS += -I$(PBHOME)/include

$(IDL_SRC): cubit.idl
	$(PBHOME)/bin/idlc cubit.idl
	-mv cubit.C cubit.cpp
	-mv cubit_s.C cubit_s.cpp

svr:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(PBHOME)/lib/iiop_s.o $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) cubit.cpp cubit.h cubit_s.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/cubit.o .shobj/cubit.: cubit.cpp
.obj/cubit_s.o .shobj/cubit_s.: cubit_s.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

# On non-Windows environment, we should at least define
# the export_include IDL flag.
TAO_IDLFLAGS = -Wb,export_macro=GENERIC_SERVANT_Export -Wb,export_include=generic_servant_export.h

LDLIBS = -lTAO

LIB = libserver.a

IDL_SRC	= FooC FooS

BINFILES = $(IDL_SRC) client

LIB_SRC = $(IDL_SRC) MyFooServant

LSRC = $(addsuffix .cpp,$(LIB_SRC))
LOBJ = $(addsuffix .o,$(LIB_SRC))

POA_CLT_SRCS = $(addsuffix .cpp,$(BINFILES))
POA_CLT_OBJS = $(addprefix $(VDIR),$(addsuffix .o,$(BINFILES)))

BIN = client

BUILD = $(VOBJS) $(VLIB) $(BIN)

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
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk


.PRECIOUS: FooC.h FooC.i FooC.cpp FooS.h FooS.i FooS.cpp 

#$(IDL_SRC): cubit.idl
#	$(TAO_ROOT)/TAO_IDL/tao_idl cubit.idl

client: $(POA_CLT_OBJS)
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#clean:
#	-/bin/rm -rf *.o $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf FooC.* FooS.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

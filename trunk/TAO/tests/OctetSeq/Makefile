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

LDLIBS     =  -lTAO
LDFLAGS += -L$(TAO_ROOT)/tao

IDL_HDR	= testC.h testS.h
IDL_SRC	= testC.cpp testS.cpp

PROG_SRCS = \
	OctetSeq.cpp \

LSRC = $(PROG_SRCS) $(IDL_SRC)

OCTETSEQ_OBJS = testC.o testS.o OctetSeq.o

BIN = OctetSeq
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

CPPFLAGS += -I$(TAO_ROOT) $(TSS_ORB_FLAG)

OctetSeq: $(addprefix $(VDIR),$(OCTETSEQ_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

.PRECIOUS: $(IDL_HDR) $(IDL_SRC)

realclean: clean
	-$(RM) testC.* testS.* testS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#----------------------------------------------------------------------------
#
#	$Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_SRC	= Multiple_InheritanceC.cpp Multiple_InheritanceS.cpp
PROG_SRCS = $(IDL_SRC) server.cpp client.cpp

LSRC = $(PROG_SRCS)

FILE_SVR_OBJS = Multiple_InheritanceC.o Multiple_InheritanceS.o server.o
FILE_CLT_OBJS = Multiple_InheritanceC.o Multiple_InheritanceS.o client.o

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


.PRECIOUS: Multiple_InheritanceC.h Multiple_InheritanceC.i Multiple_InheritanceC.cpp Multiple_InheritanceS.h Multiple_InheritanceS.i Multiple_InheritanceS.cpp

server:	$(addprefix $(VDIR),$(FILE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(FILE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) Multiple_InheritanceC.* Multiple_InheritanceS.* Multiple_InheritanceS_T.* client server

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

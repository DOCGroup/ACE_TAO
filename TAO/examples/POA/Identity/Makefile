#
# $Id$
#

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_SRC	= testC.cpp testS.cpp
PROG_SRCS = $(IDL_SRC) Identity.cpp

LSRC = $(PROG_SRCS)

PROG_OBJS = testS.o Identity.o

BIN = Identity
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


.PRECIOUS: testC.h testC.i testC.cpp testS.h testS.i testS.cpp

Identity: $(addprefix $(VDIR),$(PROG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf testC.* testS.* testS_T.*

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(TAO_ROOT)/tao/Timeprobe.h \
 $(ACE_ROOT)/ace/Timeprobe.h \
 ../Generic_Servant/MyFooServant.h ../Generic_Servant/FooS.h \
 ../Generic_Servant/FooC.h ../Generic_Servant/generic_servant_export.h \
 ../Generic_Servant/FooC.i ../Generic_Servant/FooS_T.h \
 ../Generic_Servant/FooS_T.i ../Generic_Servant/FooS_T.cpp \
 ../Generic_Servant/FooS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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

LDLIBS     = -lTAO $(MATHLIB)

IDL_SRC	= cubitC.cpp cubitS.cpp
PROG_SRCS = server.cpp \
	cubit_i.cpp \
	Task_Client.cpp \
	client.cpp \
	Util_Thread.cpp \
	tmplinst.cpp \
	Cubit_Task.cpp \
	Globals.cpp \
	Timer.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CUBIT_SVR_OBJS = \
	cubitC.o \
	cubitS.o \
	server.o \
	cubit_i.o \
	Globals.o \
	Cubit_Task.o \
	tmplinst.o \
	Timer.o

CUBIT_CLIENT_OBJS = \
	cubitC.o \
	cubitS.o \
	Task_Client.o \
	client.o \
	Util_Thread.o \
	tmplinst.o \
	Cubit_Task.o \
	cubit_i.o \
	Globals.o \
	Timer.o

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

# Local modifications to variables imported by includes above.
CPPFLAGS += -DDEBUG

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

# pccTimer will eventually get integrated into ACE.  It is specific for ClassiX on VME
ifdef CHORUS
	PCCTIMER_DIR = /project/doc/mvme
	LSRC += $(PCCTIMER_DIR)/pccTimer.cpp
	CPPFLAGS += -I$(PCCTIMER_DIR)
	CUBIT_CLIENT_OBJS += pccTimer.o
        CUBIT_SVR_OBJS += pccTimer.o
	LDLIBS += -lOrb
else
	LDLIBS += -lOrb-mt
endif

.PRECIOUS: cubitC.h cubitC.i cubitC.cpp cubitS.h cubitS.i cubitS.cpp

server:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(CUBIT_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)  $(POSTLINK)

.obj/pccTimer.o .obj/pccTimer.so .shobj/pccTimer.o .shobj/pccTimer.so: /project/doc/mvme/pccTimer.cpp
	@$(COMPILE.cc) -o $@ $<

realclean: clean
	-$(RM) cubitC.* cubitS.* cubitS_T.*

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Globals.o .obj/Globals.so .shobj/Globals.o .shobj/Globals.so: Globals.cpp Globals.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Sched_Params.h \
 $(ACE_ROOT)/ace/Sched_Params.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

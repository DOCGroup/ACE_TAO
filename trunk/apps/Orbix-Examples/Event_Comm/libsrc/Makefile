#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the Event Communications library
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LIB	= libEvent_Comm.a
SHLIB	= libEvent_Comm.so

FILES  	= Event_CommS \
	  Event_CommC \
	  Notifier_i \
	  Notification_Receiver_i

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VLIB)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Orbix related macros and target settings.
#----------------------------------------------------------------------------

ORBIX_BINDIR    = $(ORBIX_ROOT)/bin
ORBIX_LIBDIR    = $(ORBIX_ROOT)/lib
ORBIX_INCDIR    = $(ORBIX_ROOT)/include

CPPFLAGS += -DEXCEPTIONS -I$(ORBIX_INCDIR) -DWANT_ORBIX_FDS
LDFLAGS  += -L$(ORBIX_LIBDIR) -R $(ORBIX_LIBDIR)

IDLFLAGS = -A -s S.cpp -c C.cpp

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

Event_CommS.o: Event_CommS.cpp Event_Comm.hh Event_CommC.cpp
Event_CommC.o: Event_CommC.cpp Event_Comm.hh
Notifier_i.o: Notifier_i.cpp \
  ${WRAPPER_ROOT}ace/Log_Msg.h \
  ${WRAPPER_ROOT}ace/Log_Record.h \
  ${WRAPPER_ROOT}ace/sysincludes.h \
  ${WRAPPER_ROOT}ace/config.h \
  ${WRAPPER_ROOT}ace/Log_Priority.h \
  ${WRAPPER_ROOT}ace/Log_Record.i \
  ${WRAPPER_ROOT}ace/Log_Msg.i \
  ${WRAPPER_ROOT}ace/Thread_Specific.h \
  ${WRAPPER_ROOT}ace/Synch.h \
  ${WRAPPER_ROOT}ace/Time_Value.h \
  ${WRAPPER_ROOT}ace/Synch_T.h \
  ${WRAPPER_ROOT}ace/Thread_Specific.i \
  Notification_Receiver_i.h Notifier_i.h \
  ${WRAPPER_ROOT}ace/Map_Manager.h \
  ${WRAPPER_ROOT}ace/SString.h \
  ${WRAPPER_ROOT}ace/SString.i \
  Event_Comm.hh
Notification_Receiver_i.o: Notification_Receiver_i.cpp \
  ${WRAPPER_ROOT}ace/Log_Msg.h \
  ${WRAPPER_ROOT}ace/Log_Record.h \
  ${WRAPPER_ROOT}ace/sysincludes.h \
  ${WRAPPER_ROOT}ace/config.h \
  ${WRAPPER_ROOT}ace/Log_Priority.h \
  ${WRAPPER_ROOT}ace/Log_Record.i \
  ${WRAPPER_ROOT}ace/Log_Msg.i \
  ${WRAPPER_ROOT}ace/Thread_Specific.h \
  ${WRAPPER_ROOT}ace/Synch.h \
  ${WRAPPER_ROOT}ace/Time_Value.h \
  ${WRAPPER_ROOT}ace/Synch_T.h \
  ${WRAPPER_ROOT}ace/Thread_Specific.i \
  ${WRAPPER_ROOT}ace/Service_Config.h \
  ${WRAPPER_ROOT}ace/Reactor.h \
  ${WRAPPER_ROOT}ace/Handle_Set.h \
  ${WRAPPER_ROOT}ace/Handle_Set.i \
  ${WRAPPER_ROOT}ace/Timer_Queue.h \
  ${WRAPPER_ROOT}ace/Event_Handler.h \
  ${WRAPPER_ROOT}ace/Timer_Queue.i \
  ${WRAPPER_ROOT}ace/Signal.h \
  ${WRAPPER_ROOT}ace/Set.h \
  ${WRAPPER_ROOT}ace/Thread.h \
  ${WRAPPER_ROOT}ace/Token.h \
  ${WRAPPER_ROOT}ace/Reactor.i \
  ${WRAPPER_ROOT}ace/Service_Object.h \
  ${WRAPPER_ROOT}ace/Shared_Object.h \
  ${WRAPPER_ROOT}ace/Service_Record.h \
  ${WRAPPER_ROOT}ace/Thread_Manager.h \
  Notification_Receiver_i.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#----------------------------------------------------------------------------
#	$Id: Makefile	1.1	10/18/96
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

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

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
  ${ACE_ROOT}ace/Log_Msg.h \
  ${ACE_ROOT}ace/Log_Record.h \
  ${ACE_ROOT}ace/sysincludes.h \
  ${ACE_ROOT}ace/config.h \
  ${ACE_ROOT}ace/Log_Priority.h \
  ${ACE_ROOT}ace/Log_Record.i \
  ${ACE_ROOT}ace/Log_Msg.i \
  ${ACE_ROOT}ace/Thread_Specific.h \
  ${ACE_ROOT}ace/Synch.h \
  ${ACE_ROOT}ace/Time_Value.h \
  ${ACE_ROOT}ace/Synch_T.h \
  ${ACE_ROOT}ace/Thread_Specific.i \
  Notification_Receiver_i.h Notifier_i.h \
  ${ACE_ROOT}ace/Map_Manager.h \
  ${ACE_ROOT}ace/SString.h \
  ${ACE_ROOT}ace/SString.i \
  Event_Comm.hh
Notification_Receiver_i.o: Notification_Receiver_i.cpp \
  ${ACE_ROOT}ace/Log_Msg.h \
  ${ACE_ROOT}ace/Log_Record.h \
  ${ACE_ROOT}ace/sysincludes.h \
  ${ACE_ROOT}ace/config.h \
  ${ACE_ROOT}ace/Log_Priority.h \
  ${ACE_ROOT}ace/Log_Record.i \
  ${ACE_ROOT}ace/Log_Msg.i \
  ${ACE_ROOT}ace/Thread_Specific.h \
  ${ACE_ROOT}ace/Synch.h \
  ${ACE_ROOT}ace/Time_Value.h \
  ${ACE_ROOT}ace/Synch_T.h \
  ${ACE_ROOT}ace/Thread_Specific.i \
  ${ACE_ROOT}ace/Service_Config.h \
  ${ACE_ROOT}ace/Reactor.h \
  ${ACE_ROOT}ace/Handle_Set.h \
  ${ACE_ROOT}ace/Handle_Set.i \
  ${ACE_ROOT}ace/Timer_Queue.h \
  ${ACE_ROOT}ace/Event_Handler.h \
  ${ACE_ROOT}ace/Timer_Queue.i \
  ${ACE_ROOT}ace/Signal.h \
  ${ACE_ROOT}ace/Set.h \
  ${ACE_ROOT}ace/Thread.h \
  ${ACE_ROOT}ace/Token.h \
  ${ACE_ROOT}ace/Reactor.i \
  ${ACE_ROOT}ace/Service_Object.h \
  ${ACE_ROOT}ace/Shared_Object.h \
  ${ACE_ROOT}ace/Service_Types.h \
  ${ACE_ROOT}ace/Thread_Manager.h \
  Notification_Receiver_i.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

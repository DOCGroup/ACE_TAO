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
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/sysincludes.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Priority.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Time_Value.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch_T.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.i \
  Notification_Receiver_i.h Notifier_i.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Map_Manager.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.i \
  Event_Comm.hh
Notification_Receiver_i.o: Notification_Receiver_i.cpp \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/sysincludes.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Priority.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Time_Value.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch_T.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Reactor.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Handle_Set.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Handle_Set.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Timer_Queue.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Event_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Timer_Queue.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Signal.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Set.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Token.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Reactor.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Object.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Shared_Object.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Manager.h \
  Notification_Receiver_i.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

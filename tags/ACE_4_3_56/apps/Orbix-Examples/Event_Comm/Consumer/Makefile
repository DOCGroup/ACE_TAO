#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the Consumer.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= consumer

FILES  	= Notification_Receiver_Handler \
	  Input_Handler

LSRC	= $(addsuffix .cpp,$(FILES)) consumer.cpp
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	= $(addprefix $(VSHDIR),$(LOBJ)) ../libsrc/libEvent_Comm.a 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

CPPFLAGS += -I../include
VLDLIBS  += -lgen

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

Notification_Receiver_Handler.o: Notification_Receiver_Handler.cpp \
  Notification_Receiver_Handler.h \
  ${ACE_ROOT}ace/CORBA_Handler.h \
  ${ACE_ROOT}ace/Service_Config.h \
  ${ACE_ROOT}ace/Reactor.h \
  ${ACE_ROOT}ace/Handle_Set.h \
  ${ACE_ROOT}ace/sysincludes.h \
  ${ACE_ROOT}ace/config.h \
  ${ACE_ROOT}ace/Handle_Set.i \
  ${ACE_ROOT}ace/Timer_Queue.h \
  ${ACE_ROOT}ace/Event_Handler.h \
  ${ACE_ROOT}ace/Time_Value.h \
  ${ACE_ROOT}ace/Synch.h \
  ${ACE_ROOT}ace/Synch_T.h \
  ${ACE_ROOT}ace/Timer_Queue.i \
  ${ACE_ROOT}ace/Signal.h \
  ${ACE_ROOT}ace/Set.h \
  ${ACE_ROOT}ace/Thread.h \
  ${ACE_ROOT}ace/Token.h \
  ${ACE_ROOT}ace/Reactor.i \
  ${ACE_ROOT}ace/Log_Msg.h \
  ${ACE_ROOT}ace/Log_Record.h \
  ${ACE_ROOT}ace/Log_Priority.h \
  ${ACE_ROOT}ace/Log_Record.i \
  ${ACE_ROOT}ace/Log_Msg.i \
  ${ACE_ROOT}ace/Thread_Specific.h \
  ${ACE_ROOT}ace/Thread_Specific.i \
  ${ACE_ROOT}ace/Service_Object.h \
  ${ACE_ROOT}ace/Shared_Object.h \
  ${ACE_ROOT}ace/Service_Types.h \
  ${ACE_ROOT}ace/Thread_Manager.h \
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  ${ACE_ROOT}ace/Map_Manager.h \
  ${ACE_ROOT}ace/SString.h \
  ${ACE_ROOT}ace/SString.i \
  ../include/Event_Comm.hh
Input_Handler.o: Input_Handler.cpp Input_Handler.h \
  ${ACE_ROOT}ace/Service_Config.h \
  ${ACE_ROOT}ace/Reactor.h \
  ${ACE_ROOT}ace/Handle_Set.h \
  ${ACE_ROOT}ace/sysincludes.h \
  ${ACE_ROOT}ace/config.h \
  ${ACE_ROOT}ace/Handle_Set.i \
  ${ACE_ROOT}ace/Timer_Queue.h \
  ${ACE_ROOT}ace/Event_Handler.h \
  ${ACE_ROOT}ace/Time_Value.h \
  ${ACE_ROOT}ace/Synch.h \
  ${ACE_ROOT}ace/Synch_T.h \
  ${ACE_ROOT}ace/Timer_Queue.i \
  ${ACE_ROOT}ace/Signal.h \
  ${ACE_ROOT}ace/Set.h \
  ${ACE_ROOT}ace/Thread.h \
  ${ACE_ROOT}ace/Token.h \
  ${ACE_ROOT}ace/Reactor.i \
  ${ACE_ROOT}ace/Log_Msg.h \
  ${ACE_ROOT}ace/Log_Record.h \
  ${ACE_ROOT}ace/Log_Priority.h \
  ${ACE_ROOT}ace/Log_Record.i \
  ${ACE_ROOT}ace/Log_Msg.i \
  ${ACE_ROOT}ace/Thread_Specific.h \
  ${ACE_ROOT}ace/Thread_Specific.i \
  ${ACE_ROOT}ace/Service_Object.h \
  ${ACE_ROOT}ace/Shared_Object.h \
  ${ACE_ROOT}ace/Service_Types.h \
  ${ACE_ROOT}ace/Thread_Manager.h \
  Notification_Receiver_Handler.h \
  ${ACE_ROOT}ace/CORBA_Handler.h \
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  ${ACE_ROOT}ace/Map_Manager.h \
  ${ACE_ROOT}ace/SString.h \
  ${ACE_ROOT}ace/SString.i \
  ../include/Event_Comm.hh
consumer.o: consumer.cpp \
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
  Notification_Receiver_Handler.h \
  ${ACE_ROOT}ace/CORBA_Handler.h \
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
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  ${ACE_ROOT}ace/Map_Manager.h \
  ${ACE_ROOT}ace/SString.h \
  ${ACE_ROOT}ace/SString.i \
  ../include/Event_Comm.hh Input_Handler.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

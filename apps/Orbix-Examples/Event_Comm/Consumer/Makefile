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

SRX     = ../src/.obj

LDLIBS	= $(addprefix .shobj/,$(LOBJ)) ../src/libEvent_Comm.a 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include		$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

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
  ${WRAPPER_ROOT}ace/CORBA_Handler.h \
  ${WRAPPER_ROOT}ace/Service_Config.h \
  ${WRAPPER_ROOT}ace/Reactor.h \
  ${WRAPPER_ROOT}ace/Handle_Set.h \
  ${WRAPPER_ROOT}ace/sysincludes.h \
  ${WRAPPER_ROOT}ace/config.h \
  ${WRAPPER_ROOT}ace/Handle_Set.i \
  ${WRAPPER_ROOT}ace/Timer_Queue.h \
  ${WRAPPER_ROOT}ace/Event_Handler.h \
  ${WRAPPER_ROOT}ace/Time_Value.h \
  ${WRAPPER_ROOT}ace/Synch.h \
  ${WRAPPER_ROOT}ace/Synch_T.h \
  ${WRAPPER_ROOT}ace/Timer_Queue.i \
  ${WRAPPER_ROOT}ace/Signal.h \
  ${WRAPPER_ROOT}ace/Set.h \
  ${WRAPPER_ROOT}ace/Thread.h \
  ${WRAPPER_ROOT}ace/Token.h \
  ${WRAPPER_ROOT}ace/Reactor.i \
  ${WRAPPER_ROOT}ace/Log_Msg.h \
  ${WRAPPER_ROOT}ace/Log_Record.h \
  ${WRAPPER_ROOT}ace/Log_Priority.h \
  ${WRAPPER_ROOT}ace/Log_Record.i \
  ${WRAPPER_ROOT}ace/Log_Msg.i \
  ${WRAPPER_ROOT}ace/Thread_Specific.h \
  ${WRAPPER_ROOT}ace/Thread_Specific.i \
  ${WRAPPER_ROOT}ace/Service_Object.h \
  ${WRAPPER_ROOT}ace/Shared_Object.h \
  ${WRAPPER_ROOT}ace/Service_Record.h \
  ${WRAPPER_ROOT}ace/Thread_Manager.h \
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  ${WRAPPER_ROOT}ace/Map_Manager.h \
  ${WRAPPER_ROOT}ace/SString.h \
  ${WRAPPER_ROOT}ace/SString.i \
  ../include/Event_Comm.hh
Input_Handler.o: Input_Handler.cpp Input_Handler.h \
  ${WRAPPER_ROOT}ace/Service_Config.h \
  ${WRAPPER_ROOT}ace/Reactor.h \
  ${WRAPPER_ROOT}ace/Handle_Set.h \
  ${WRAPPER_ROOT}ace/sysincludes.h \
  ${WRAPPER_ROOT}ace/config.h \
  ${WRAPPER_ROOT}ace/Handle_Set.i \
  ${WRAPPER_ROOT}ace/Timer_Queue.h \
  ${WRAPPER_ROOT}ace/Event_Handler.h \
  ${WRAPPER_ROOT}ace/Time_Value.h \
  ${WRAPPER_ROOT}ace/Synch.h \
  ${WRAPPER_ROOT}ace/Synch_T.h \
  ${WRAPPER_ROOT}ace/Timer_Queue.i \
  ${WRAPPER_ROOT}ace/Signal.h \
  ${WRAPPER_ROOT}ace/Set.h \
  ${WRAPPER_ROOT}ace/Thread.h \
  ${WRAPPER_ROOT}ace/Token.h \
  ${WRAPPER_ROOT}ace/Reactor.i \
  ${WRAPPER_ROOT}ace/Log_Msg.h \
  ${WRAPPER_ROOT}ace/Log_Record.h \
  ${WRAPPER_ROOT}ace/Log_Priority.h \
  ${WRAPPER_ROOT}ace/Log_Record.i \
  ${WRAPPER_ROOT}ace/Log_Msg.i \
  ${WRAPPER_ROOT}ace/Thread_Specific.h \
  ${WRAPPER_ROOT}ace/Thread_Specific.i \
  ${WRAPPER_ROOT}ace/Service_Object.h \
  ${WRAPPER_ROOT}ace/Shared_Object.h \
  ${WRAPPER_ROOT}ace/Service_Record.h \
  ${WRAPPER_ROOT}ace/Thread_Manager.h \
  Notification_Receiver_Handler.h \
  ${WRAPPER_ROOT}ace/CORBA_Handler.h \
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  ${WRAPPER_ROOT}ace/Map_Manager.h \
  ${WRAPPER_ROOT}ace/SString.h \
  ${WRAPPER_ROOT}ace/SString.i \
  ../include/Event_Comm.hh
consumer.o: consumer.cpp \
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
  Notification_Receiver_Handler.h \
  ${WRAPPER_ROOT}ace/CORBA_Handler.h \
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
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  ${WRAPPER_ROOT}ace/Map_Manager.h \
  ${WRAPPER_ROOT}ace/SString.h \
  ${WRAPPER_ROOT}ace/SString.i \
  ../include/Event_Comm.hh Input_Handler.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

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
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/CORBA_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Reactor.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Handle_Set.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/sysincludes.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Handle_Set.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Timer_Queue.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Event_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Time_Value.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch_T.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Timer_Queue.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Signal.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Set.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Token.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Reactor.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Priority.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Object.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Shared_Object.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Manager.h \
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Map_Manager.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.i \
  ../include/Event_Comm.hh
Input_Handler.o: Input_Handler.cpp Input_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Reactor.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Handle_Set.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/sysincludes.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/config.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Handle_Set.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Timer_Queue.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Event_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Time_Value.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Synch_T.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Timer_Queue.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Signal.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Set.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Token.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Reactor.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Priority.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Record.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Log_Msg.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Specific.i \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Object.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Shared_Object.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Service_Record.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Thread_Manager.h \
  Notification_Receiver_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/CORBA_Handler.h \
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Map_Manager.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.i \
  ../include/Event_Comm.hh
consumer.o: consumer.cpp \
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
  Notification_Receiver_Handler.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/CORBA_Handler.h \
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
  ../include/Event_Comm_i.h ../include/Notification_Receiver_i.h \
  ../include/Notifier_i.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/Map_Manager.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.h \
  /project/adaptive/ACE_wrappers/build/SunOS5.4/include/ace/SString.i \
  ../include/Event_Comm.hh Input_Handler.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

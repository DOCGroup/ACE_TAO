#----------------------------------------------------------------------------
# $Id$
#
#	Makefile for the Reactor Server Logging Daemon
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= server_loggerd

FILES 	= Logging_Acceptor \
	  Logging_Handler

SRC	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Logging_Acceptor.o .obj/Logging_Acceptor.so .shobj/Logging_Acceptor.o .shobj/Logging_Acceptor.so: Logging_Acceptor.cpp \
  $(ACE_ROOT)/ace/WFMO_Reactor.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  Logging_Acceptor.h $(ACE_ROOT)/ace/SOCK_Acceptor.h \
  $(ACE_ROOT)/ace/SOCK_Stream.h \
  $(ACE_ROOT)/ace/SOCK_IO.h \
  $(ACE_ROOT)/ace/SOCK.h $(ACE_ROOT)/ace/Addr.h \
  $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/IPC_SAP.h \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/IPC_SAP.i \
  $(ACE_ROOT)/ace/SOCK.i $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i \
  $(ACE_ROOT)/ace/ACE.i $(ACE_ROOT)/ace/SOCK_IO.i \
  $(ACE_ROOT)/ace/INET_Addr.h \
  $(ACE_ROOT)/ace/INET_Addr.i \
  $(ACE_ROOT)/ace/SOCK_Stream.i \
  $(ACE_ROOT)/ace/SOCK_Acceptor.i \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i Logging_Handler.h \
  Reactor_Singleton.h $(ACE_ROOT)/ace/Singleton.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Singleton.i \
  $(ACE_ROOT)/ace/Singleton.cpp \
  $(ACE_ROOT)/ace/Object_Manager.h \
  $(ACE_ROOT)/ace/Object_Manager.i \
  $(ACE_ROOT)/ace/Managed_Object.h \
  $(ACE_ROOT)/ace/Managed_Object.i \
  $(ACE_ROOT)/ace/Managed_Object.cpp \
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
  $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Test_and_Set.h \
  $(ACE_ROOT)/ace/Test_and_Set.i \
  $(ACE_ROOT)/ace/Test_and_Set.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h

.obj/Logging_Handler.o .obj/Logging_Handler.so .shobj/Logging_Handler.o .shobj/Logging_Handler.so: Logging_Handler.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl Logging_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/INET_Addr.h \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i \
  $(ACE_ROOT)/ace/Addr.h $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/INET_Addr.i \
  $(ACE_ROOT)/ace/SOCK_Stream.h \
  $(ACE_ROOT)/ace/SOCK_IO.h \
  $(ACE_ROOT)/ace/SOCK.h \
  $(ACE_ROOT)/ace/IPC_SAP.h \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/IPC_SAP.i \
  $(ACE_ROOT)/ace/SOCK.i $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/ACE.i $(ACE_ROOT)/ace/SOCK_IO.i \
  $(ACE_ROOT)/ace/SOCK_Stream.i Reactor_Singleton.h \
  $(ACE_ROOT)/ace/Singleton.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Singleton.i \
  $(ACE_ROOT)/ace/Singleton.cpp \
  $(ACE_ROOT)/ace/Object_Manager.h \
  $(ACE_ROOT)/ace/Object_Manager.i \
  $(ACE_ROOT)/ace/Managed_Object.h \
  $(ACE_ROOT)/ace/Managed_Object.i \
  $(ACE_ROOT)/ace/Managed_Object.cpp \
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
  $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Test_and_Set.h \
  $(ACE_ROOT)/ace/Test_and_Set.i \
  $(ACE_ROOT)/ace/Test_and_Set.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(ACE_ROOT)/ace/Log_Record.h \
  $(ACE_ROOT)/ace/Log_Record.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Logging
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_Logging
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	Dump\
	Dump_T\
	Log_Msg\
	Log_Msg_Backend\
	Log_Msg_Callback\
	Log_Msg_IPC\
	Log_Msg_NT_Event_Log\
	Log_Msg_UNIX_Syslog\
	Log_Record\
	Logging_Strategy\
	Trace

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(LIB_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBNAME).a $(LIBNAME).so

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Dump.o .obj/Dump.o .obj/Dump.o .obj/Dump.o: Dump.cpp \
 $(ACE_ROOT)/ace/Threads/Synch_T.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Threads/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Threads/Synch.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.i \
 $(ACE_ROOT)/ace/Threads/Thread.h \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.cpp \
 Log_Msg.h \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i \
 Dump.h \
 Dump_T.h \
 Dump_T.cpp \
 $(ACE_ROOT)/ace/Utils/Object_Manager.h \
 $(ACE_ROOT)/ace/Utils/Object_Manager.i \
 $(ACE_ROOT)/ace/Utils/Templates/Managed_Object.h \
 $(ACE_ROOT)/ace/Utils/Templates/Managed_Object.i \
 $(ACE_ROOT)/ace/Utils/Templates/Managed_Object.cpp

.obj/Dump_T.o .obj/Dump_T.o .obj/Dump_T.o .obj/Dump_T.o: Dump_T.cpp \
 Dump_T.h \
 $(ACE_ROOT)/ace/pre.h \
 Dump.h \
 $(ACE_ROOT)/ace/Threads/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Threads/Synch.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.h \
 $(ACE_ROOT)/ace/Threads/Synch_T.i \
 $(ACE_ROOT)/ace/Threads/Thread.h \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.cpp \
 Log_Msg.h \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i \
 Dump_T.cpp

.obj/Log_Msg.o .obj/Log_Msg.o .obj/Log_Msg.o .obj/Log_Msg.o: Log_Msg.cpp \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 Trace.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Threads/Thread_Manager.h \
 $(ACE_ROOT)/ace/Threads/Thread.h \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread.i \
 $(ACE_ROOT)/ace/Threads/Synch.h \
 $(ACE_ROOT)/ace/Threads/Synch.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.h \
 $(ACE_ROOT)/ace/Threads/Synch_T.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.cpp \
 Log_Msg.h \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.h \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.i \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Singleton.h \
 $(ACE_ROOT)/ace/Utils/Templates/Singleton.i \
 $(ACE_ROOT)/ace/Utils/Templates/Singleton.cpp \
 $(ACE_ROOT)/ace/Utils/Object_Manager.h \
 $(ACE_ROOT)/ace/Utils/Object_Manager.i \
 $(ACE_ROOT)/ace/Utils/Templates/Managed_Object.h \
 $(ACE_ROOT)/ace/Utils/Templates/Managed_Object.i \
 $(ACE_ROOT)/ace/Utils/Templates/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Threads/Thread_Manager.i \
 Log_Msg_Callback.h \
 Log_Msg_IPC.h \
 Log_Msg_Backend.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Connector.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Stream.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_IO.h \
 $(ACE_ROOT)/ace/Sockets/SOCK.h \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.h \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.i \
 $(ACE_ROOT)/ace/Sockets/SOCK.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Connector.i \
 Log_Msg_NT_Event_Log.h \
 Log_Msg_UNIX_Syslog.h

.obj/Log_Msg_Backend.o .obj/Log_Msg_Backend.o .obj/Log_Msg_Backend.o .obj/Log_Msg_Backend.o: Log_Msg_Backend.cpp \
 Log_Msg_Backend.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl

.obj/Log_Msg_Callback.o .obj/Log_Msg_Callback.o .obj/Log_Msg_Callback.o .obj/Log_Msg_Callback.o: Log_Msg_Callback.cpp \
 Log_Msg_Callback.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl

.obj/Log_Msg_IPC.o .obj/Log_Msg_IPC.o .obj/Log_Msg_IPC.o .obj/Log_Msg_IPC.o: Log_Msg_IPC.cpp \
 Log_Msg_IPC.h \
 $(ACE_ROOT)/ace/pre.h \
 Log_Msg_Backend.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/Sockets/SOCK_Connector.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Stream.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_IO.h \
 $(ACE_ROOT)/ace/Sockets/SOCK.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.h \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.i \
 $(ACE_ROOT)/ace/Sockets/SOCK.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Connector.i \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i

.obj/Log_Msg_NT_Event_Log.o .obj/Log_Msg_NT_Event_Log.o .obj/Log_Msg_NT_Event_Log.o .obj/Log_Msg_NT_Event_Log.o: Log_Msg_NT_Event_Log.cpp \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl

.obj/Log_Msg_UNIX_Syslog.o .obj/Log_Msg_UNIX_Syslog.o .obj/Log_Msg_UNIX_Syslog.o .obj/Log_Msg_UNIX_Syslog.o: Log_Msg_UNIX_Syslog.cpp \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 Log_Msg.h \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 Log_Msg_UNIX_Syslog.h \
 Log_Msg_Backend.h

.obj/Log_Record.o .obj/Log_Record.o .obj/Log_Record.o .obj/Log_Record.o: Log_Record.cpp \
 Log_Record.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 Log_Priority.h \
 Log_Record.i \
 Log_Msg.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i

.obj/Logging_Strategy.o .obj/Logging_Strategy.o .obj/Logging_Strategy.o .obj/Logging_Strategy.o: Logging_Strategy.cpp \
 $(ACE_ROOT)/ace/Utils/Get_Opt.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Utils/SString.h \
 $(ACE_ROOT)/ace/Utils/Templates/String_Base.h \
 $(ACE_ROOT)/ace/Utils/String_Base_Const.h \
 $(ACE_ROOT)/ace/Utils/Templates/String_Base.i \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/String_Base.cpp \
 $(ACE_ROOT)/ace/Memory/Malloc.h \
 $(ACE_ROOT)/ace/Memory/Malloc.i \
 $(ACE_ROOT)/ace/Memory/Malloc_T.h \
 $(ACE_ROOT)/ace/Threads/Synch.h \
 $(ACE_ROOT)/ace/Threads/Synch.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.h \
 $(ACE_ROOT)/ace/Threads/Synch_T.i \
 $(ACE_ROOT)/ace/Threads/Thread.h \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.cpp \
 Log_Msg.h \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i \
 $(ACE_ROOT)/ace/Memory/Malloc_Allocator.h \
 $(ACE_ROOT)/ace/Memory/Malloc_Allocator.i \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.h \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.i \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.cpp \
 $(ACE_ROOT)/ace/Memory/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory/Memory_Pool.h \
 $(ACE_ROOT)/ace/Demux/Event_Handler.h \
 $(ACE_ROOT)/ace/Demux/Event_Handler.i \
 $(ACE_ROOT)/ace/IPC/Signal.h \
 $(ACE_ROOT)/ace/IPC/Signal.i \
 $(ACE_ROOT)/ace/Memory/Mem_Map.h \
 $(ACE_ROOT)/ace/Memory/Mem_Map.i \
 $(ACE_ROOT)/ace/IPC/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/IPC/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/IPC/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/IPC/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Memory/Memory_Pool.i \
 $(ACE_ROOT)/ace/Utils/Templates/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Utils/Templates/Auto_Ptr.i \
 $(ACE_ROOT)/ace/Utils/Templates/Auto_Ptr.cpp \
 $(ACE_ROOT)/ace/Streams/streams.h \
 $(ACE_ROOT)/ace/Utils/SString.i \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 $(ACE_ROOT)/ace/Utils/Get_Opt.i \
 $(ACE_ROOT)/ace/Demux/Reactor.h \
 $(ACE_ROOT)/ace/OS/Handle_Set.h \
 $(ACE_ROOT)/ace/OS/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Utils/Test_and_Set.h \
 $(ACE_ROOT)/ace/Utils/Test_and_Set.i \
 $(ACE_ROOT)/ace/Utils/Test_and_Set.cpp \
 $(ACE_ROOT)/ace/Timer/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Demux/Reactor.i \
 $(ACE_ROOT)/ace/Demux/Reactor_Impl.h \
 Logging_Strategy.h \
 $(ACE_ROOT)/ace/Svcconf/Service_Object.h \
 $(ACE_ROOT)/ace/Svcconf/Shared_Object.h \
 $(ACE_ROOT)/ace/Svcconf/Shared_Object.i \
 $(ACE_ROOT)/ace/Svcconf/Service_Object.i

.obj/Trace.o .obj/Trace.o .obj/Trace.o .obj/Trace.o: Trace.cpp \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 Trace.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 Log_Msg.h \
 Log_Record.h \
 Log_Priority.h \
 Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_QoS
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_QoS
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	QoS_Decorator\
	QoS_Manager\
	QoS_Session_Factory\
	QoS_Session_Impl\
	SOCK_Dgram_Mcast_QoS

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


.obj/QoS_Decorator.o .obj/QoS_Decorator.o .obj/QoS_Decorator.o .obj/QoS_Decorator.o: QoS_Decorator.cpp \
 QoS_Decorator.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Demux/Reactor.h \
 $(ACE_ROOT)/ace/OS/Handle_Set.h \
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
 $(ACE_ROOT)/ace/OS/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer/Timer_Queue.h \
 $(ACE_ROOT)/ace/Threads/Synch.h \
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
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/Timer/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.h \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.i \
 $(ACE_ROOT)/ace/Utils/Templates/Free_List.cpp \
 $(ACE_ROOT)/ace/Utils/Test_and_Set.h \
 $(ACE_ROOT)/ace/Event_Handler/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler/Event_Handler.i \
 $(ACE_ROOT)/ace/Utils/Test_and_Set.i \
 $(ACE_ROOT)/ace/Utils/Test_and_Set.cpp \
 $(ACE_ROOT)/ace/Timer/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/IPC/Signal.h \
 $(ACE_ROOT)/ace/IPC/Signal.i \
 $(ACE_ROOT)/ace/Demux/Reactor.i \
 $(ACE_ROOT)/ace/Demux/Reactor_Impl.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Dgram_Mcast_QoS.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/Sockets/SOCK.h \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.h \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.i \
 $(ACE_ROOT)/ace/Sockets/SOCK.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram_Mcast.i \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 ACE_QoS_Export.h \
 QoS_Session.h \
 $(ACE_ROOT)/ace/OS/OS_QoS.h \
 SOCK_Dgram_Mcast_QoS.i

.obj/QoS_Manager.o .obj/QoS_Manager.o .obj/QoS_Manager.o .obj/QoS_Manager.o: QoS_Manager.cpp \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/IPC/Addr.h \
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
 $(ACE_ROOT)/ace/IPC/Addr.i \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 ACE_QoS_Export.h \
 QoS_Session.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 $(ACE_ROOT)/ace/OS/OS_QoS.h

.obj/QoS_Session_Factory.o .obj/QoS_Session_Factory.o .obj/QoS_Session_Factory.o .obj/QoS_Session_Factory.o: QoS_Session_Factory.cpp \
 QoS_Session_Factory.h \
 $(ACE_ROOT)/ace/pre.h \
 QoS_Session.h \
 ACE_QoS_Export.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
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
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 $(ACE_ROOT)/ace/OS/OS_QoS.h \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 QoS_Session_Impl.h \
 QoS_Session_Impl.i

.obj/QoS_Session_Impl.o .obj/QoS_Session_Impl.o .obj/QoS_Session_Impl.o .obj/QoS_Session_Impl.o: QoS_Session_Impl.cpp \
 $(ACE_ROOT)/ace/Sockets/SOCK.h \
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
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.h \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.i \
 $(ACE_ROOT)/ace/Sockets/SOCK.i \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 ACE_QoS_Export.h \
 QoS_Session.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 $(ACE_ROOT)/ace/OS/OS_QoS.h \
 QoS_Session_Impl.h \
 QoS_Session_Impl.i

.obj/SOCK_Dgram_Mcast_QoS.o .obj/SOCK_Dgram_Mcast_QoS.o .obj/SOCK_Dgram_Mcast_QoS.o .obj/SOCK_Dgram_Mcast_QoS.o: SOCK_Dgram_Mcast_QoS.cpp \
 SOCK_Dgram_Mcast_QoS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/Sockets/SOCK.h \
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
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.h \
 $(ACE_ROOT)/ace/Sockets/IPC_SAP.i \
 $(ACE_ROOT)/ace/Sockets/SOCK.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/Sockets/SOCK_Dgram_Mcast.i \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.h \
 $(ACE_ROOT)/ace/Utils/Containers.h \
 $(ACE_ROOT)/ace/Memory/Malloc_Base.h \
 $(ACE_ROOT)/ace/Utils/Containers.i \
 $(ACE_ROOT)/ace/Utils/Templates/Node.h \
 $(ACE_ROOT)/ace/Utils/Templates/Node.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.h \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.inl \
 $(ACE_ROOT)/ace/Utils/Templates/Array_Base.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Utils/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.i \
 $(ACE_ROOT)/ace/Utils/Templates/Containers_T.cpp \
 ACE_QoS_Export.h \
 QoS_Session.h \
 $(ACE_ROOT)/ace/OS/OS_QoS.h \
 SOCK_Dgram_Mcast_QoS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

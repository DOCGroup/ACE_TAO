#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libACE_QoS.a
SHLIB    = libACE_QoS.$(SOEXT)

FILES=  QoS_Decorator \
        QoS_Manager \
        QoS_Session_Factory \
        QoS_Session_Impl \
        SOCK_Dgram_Mcast_QoS 

LIBS=$(ACELIB)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/QoS_Decorator.o .obj/QoS_Decorator.so .shobj/QoS_Decorator.o .shobj/QoS_Decorator.so: QoS_Decorator.cpp QoS_Decorator.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread_Adapter.h \
 $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Test_and_Set.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Test_and_Set.i \
 $(ACE_ROOT)/ace/Test_and_Set.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 SOCK_Dgram_Mcast_QoS.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 ACE_QoS_Export.h QoS_Session.h SOCK_Dgram_Mcast_QoS.i

.obj/QoS_Manager.o .obj/QoS_Manager.so .shobj/QoS_Manager.o .shobj/QoS_Manager.so: QoS_Manager.cpp QoS_Manager.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 ACE_QoS_Export.h QoS_Session.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i

.obj/QoS_Session_Factory.o .obj/QoS_Session_Factory.so .shobj/QoS_Session_Factory.o .shobj/QoS_Session_Factory.so: QoS_Session_Factory.cpp QoS_Session_Factory.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/QoS/QoS_Session.h \
 $(ACE_ROOT)/ace/QoS/ACE_QoS_Export.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 ACE_QoS_Export.h QoS_Session_Impl.h QoS_Session_Impl.i

.obj/QoS_Session_Impl.o .obj/QoS_Session_Impl.so .shobj/QoS_Session_Impl.o .shobj/QoS_Session_Impl.so: QoS_Session_Impl.cpp \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 ACE_QoS_Export.h QoS_Session.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 QoS_Session_Impl.h \
 $(ACE_ROOT)/ace/QoS/QoS_Session.h \
 QoS_Session_Impl.i

.obj/SOCK_Dgram_Mcast_QoS.o .obj/SOCK_Dgram_Mcast_QoS.so .shobj/SOCK_Dgram_Mcast_QoS.o .shobj/SOCK_Dgram_Mcast_QoS.so: SOCK_Dgram_Mcast_QoS.cpp \
 SOCK_Dgram_Mcast_QoS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 QoS_Manager.h \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Array_Base.h \
 $(ACE_ROOT)/ace/Array_Base.inl \
 $(ACE_ROOT)/ace/Array_Base.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.h \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 ACE_QoS_Export.h QoS_Session.h SOCK_Dgram_Mcast_QoS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

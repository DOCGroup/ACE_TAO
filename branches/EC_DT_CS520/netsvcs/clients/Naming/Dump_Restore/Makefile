#----------------------------------------------------------------------------
# $Id$
#
#       Makefile for a test of the Dump_Restore lib
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------
MAKEFILE=Makefile

# These examples don't require SSL support... those that do are below.
MKLIST = Makefile.Dump_Restore \
         Makefile.main


## Ensure that Makefiles in MKLIST are executed in sequence during a
## parallel build because we share some files between multiple libraries.
.NOTPARALLEL:

## Makefile.Timer.mkfile is a dummy target which will cause
## $(MAKE) -f Makefile.Timer to be invoked, then it cleans
## up tempinc when needed for AIX Visual Age C++.
%.mkfile: %
	@echo $(MAKE) -f $< $(MKFILE_TARGET)
	@$(MAKE) -f $< $(MKFILE_TARGET)
	-@$(RM) -rf tempinc

# This rule invokes make again with the list of .mkfile targets as a
# parameter.  For example, if the all target is being made, make is invoked
# as follows:
#
# make -f Makefile MKFILE_TARGET=all Makefile.Reactive_Logging_Server_Ex

all clean depend realclean:
ifneq ($(MKLIST),)
	@echo $(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
	@$(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
endif

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/main.o .obj/main.so .shobj/main.o .shobj/main.so: main.cpp \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/SStringfwd.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/OS_NS_string.inl \
  $(ACE_ROOT)/ace/OS_NS_wchar.h \
  $(ACE_ROOT)/ace/os_include/os_wchar.h \
  $(ACE_ROOT)/ace/os_include/os_string.h \
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/OS_NS_wchar.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_NS_stdlib.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.inl \
  $(ACE_ROOT)/ace/Object_Manager_Base.h \
  $(ACE_ROOT)/ace/Cleanup.h \
  $(ACE_ROOT)/ace/Cleanup.inl \
  $(ACE_ROOT)/ace/Object_Manager_Base.inl \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/OS_NS_fcntl.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
  $(ACE_ROOT)/ace/OS_NS_fcntl.inl \
  $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_in.h \
  $(ACE_ROOT)/ace/os_include/sys/os_socket.h \
  $(ACE_ROOT)/ace/os_include/sys/os_uio.h \
  $(ACE_ROOT)/ace/Sock_Connect.i \
  $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/OS_NS_unistd.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_time.h \
  $(ACE_ROOT)/ace/OS_NS_sys_time.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.inl \
  $(ACE_ROOT)/ace/OS_NS_errno.h \
  $(ACE_ROOT)/ace/OS_NS_errno.inl \
  $(ACE_ROOT)/ace/OS_NS_Thread.h \
  $(ACE_ROOT)/ace/os_include/os_pthread.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_sem.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/os_include/os_semaphore.h \
  $(ACE_ROOT)/ace/OS_NS_signal.h \
  $(ACE_ROOT)/ace/OS_NS_signal.inl \
  $(ACE_ROOT)/ace/OS_NS_macros.h \
  $(ACE_ROOT)/ace/OS_NS_Thread.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.h \
  $(ACE_ROOT)/ace/OS_NS_stropts.h \
  $(ACE_ROOT)/ace/os_include/os_stropts.h \
  $(ACE_ROOT)/ace/OS_NS_stropts.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_uio.h \
  $(ACE_ROOT)/ace/OS_NS_sys_uio.inl \
  $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i \
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i \
  $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/os_include/os_strings.h \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Synch_Traits.h \
  $(ACE_ROOT)/ace/Lock.h \
  $(ACE_ROOT)/ace/Lock.inl \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Guard_T.h \
  $(ACE_ROOT)/ace/Guard_T.inl \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.h \
  $(ACE_ROOT)/ace/RW_Mutex.h \
  $(ACE_ROOT)/ace/RW_Mutex.inl \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Guard_T.cpp \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Atomic_Op.h \
  $(ACE_ROOT)/ace/Thread_Mutex.h \
  $(ACE_ROOT)/ace/Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Reactor_Timer_Interface.h \
  $(ACE_ROOT)/ace/Null_Mutex.h \
  $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
  Dump_Restore.h \
  $(ACE_ROOT)/ace/Naming_Context.h \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/os_include/os_dlfcn.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Name_Proxy.h \
  $(ACE_ROOT)/ace/INET_Addr.h \
  $(ACE_ROOT)/ace/Addr.h \
  $(ACE_ROOT)/ace/Addr.i \
  $(ACE_ROOT)/ace/INET_Addr.i \
  $(ACE_ROOT)/ace/SOCK_Connector.h \
  $(ACE_ROOT)/ace/SOCK_Stream.h \
  $(ACE_ROOT)/ace/SOCK_IO.h \
  $(ACE_ROOT)/ace/SOCK.h \
  $(ACE_ROOT)/ace/IPC_SAP.h \
  $(ACE_ROOT)/ace/IPC_SAP.i \
  $(ACE_ROOT)/ace/SOCK.i \
  $(ACE_ROOT)/ace/SOCK_IO.i \
  $(ACE_ROOT)/ace/SOCK_Stream.i \
  $(ACE_ROOT)/ace/SOCK_Connector.i \
  $(ACE_ROOT)/ace/Synch_Options.h \
  $(ACE_ROOT)/ace/Synch_Options.i \
  $(ACE_ROOT)/ace/Name_Request_Reply.h \
  $(ACE_ROOT)/ace/Name_Space.h \
  $(ACE_ROOT)/ace/os_include/os_netdb.h \
  $(ACE_ROOT)/ace/svc_export.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

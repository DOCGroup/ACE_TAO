# $Id$

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC = Event_Service.cpp
ES_OBJS=$(LSRC:.cpp=.o)


BIN2 = Event_Service

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (RTOLDEvent,$(findstring RTOLDEvent,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # RTOLDEvent

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

LDLIBS = -lTAO_RTOLDEvent -lTAO_RTEvent -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_Messaging -lTAO_BiDirGIOP $(TAO_SRVR_LIBS)

Event_Service: $(addprefix $(VDIR),$(ES_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(LYNXTARGET),ppc)
#### Event_Service.cpp raises internal compiler error with LynxOS 3.0.0 g++,
#### but only when -O2 optimization is enabled.  Disable it . . .
.obj/Event_Service.o .obj/Event_Service.so .shobj/Event_Service.o .shobj/Event_Service.so: Event_Service.cpp
	@echo NOTE: compiling Event_Service.cpp with -O instead of -O2, see Makefile.
	$(subst $(OCFLAGS) ,-O ,$(COMPILE.cc)) -o $@ $<
endif # LYNXTARGET == ppc

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Event_Service.o .obj/Event_Service.so .shobj/Event_Service.o .shobj/Event_Service.so: Event_Service.cpp Event_Service.h \
  $(ACE_ROOT)/ace/pre.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseC.h \
  $(TAO_ROOT)/tao/CDR.h \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/SStringfwd.h \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/os_include/os_string.h \
  $(ACE_ROOT)/ace/os_include/os_strings.h \
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/CDR_Stream.i \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/Basic_Types.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(ACE_ROOT)/ace/Synch_Traits.h \
  $(ACE_ROOT)/ace/Lock.h \
  $(ACE_ROOT)/ace/Lock.inl \
  $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/VarOut_T.h \
  $(TAO_ROOT)/tao/VarOut_T.inl \
  $(TAO_ROOT)/tao/VarOut_T.cpp \
  $(TAO_ROOT)/tao/TimeBaseC.i \
  $(TAO_ROOT)/tao/TimeBaseS_T.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.i \
  $(TAO_ROOT)/tao/TimeBaseS_T.cpp \
  $(TAO_ROOT)/tao/TimeBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
  $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/Exception.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/IOP_IORC.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/default_environment.h \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.inl \
  $(TAO_ROOT)/tao/Objref_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/CORBA_methods.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.inl \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Array_VarOut_T.h \
  $(TAO_ROOT)/tao/Array_VarOut_T.inl \
  $(TAO_ROOT)/tao/Array_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Seq_Var_T.h \
  $(TAO_ROOT)/tao/Seq_Var_T.inl \
  $(TAO_ROOT)/tao/Seq_Var_T.cpp \
  $(TAO_ROOT)/tao/Seq_Out_T.h \
  $(TAO_ROOT)/tao/Seq_Out_T.inl \
  $(TAO_ROOT)/tao/Seq_Out_T.cpp \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/IOP_IORC.i \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/Object_Argument_T.h \
  $(TAO_ROOT)/tao/Argument.h \
  $(TAO_ROOT)/tao/Object_Argument_T.inl \
  $(TAO_ROOT)/tao/Object_Argument_T.cpp \
  $(TAO_ROOT)/tao/Dynamic_ParameterC.h \
  $(TAO_ROOT)/tao/ParameterMode.h \
  $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/Arg_Traits_T.h \
  $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/Dynamic_ParameterC.inl \
  $(TAO_ROOT)/tao/Object.i \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/PolicyC.i \
  $(TAO_ROOT)/tao/OBV_Constants.h \
  $(ACE_ROOT)/ace/Thread_Mutex.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/os_include/os_dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/os_include/os_dlfcn.h \
  $(ACE_ROOT)/ace/os_include/os_netdb.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_in.h \
  $(ACE_ROOT)/ace/os_include/sys/os_socket.h \
  $(ACE_ROOT)/ace/os_include/sys/os_uio.h \
  $(ACE_ROOT)/ace/os_include/net/os_if.h \
  $(ACE_ROOT)/ace/os_include/sys/os_sem.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/os_include/os_pthread.h \
  $(ACE_ROOT)/ace/os_include/os_assert.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
  $(ACE_ROOT)/ace/os_include/arpa/os_inet.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_tcp.h \
  $(ACE_ROOT)/ace/os_include/sys/os_shm.h \
  $(ACE_ROOT)/ace/os_include/os_pwd.h \
  $(ACE_ROOT)/ace/os_include/os_stropts.h \
  $(ACE_ROOT)/ace/os_include/os_termios.h \
  $(ACE_ROOT)/ace/os_include/os_aio.h \
  $(ACE_ROOT)/ace/os_include/sys/os_un.h \
  $(ACE_ROOT)/ace/os_include/os_poll.h \
  $(ACE_ROOT)/ace/os_include/sys/os_msg.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/os_include/os_syslog.h \
  $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Guard_T.h \
  $(ACE_ROOT)/ace/Guard_T.inl \
  $(ACE_ROOT)/ace/Guard_T.cpp \
  $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.i \
  $(TAO_ROOT)/tao/Collocation_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Collocation_Strategy.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/event_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecDefaultEventDataS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/sched_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.i \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/ACE.h \
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
  $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i \
  $(ACE_ROOT)/ace/Get_Opt.h \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
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
  $(ACE_ROOT)/ace/Get_Opt.i \
  $(ACE_ROOT)/ace/Argv_Type_Converter.h \
  $(ACE_ROOT)/ace/Argv_Type_Converter.inl \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
  $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/Auto_Event.h \
  $(ACE_ROOT)/ace/Event.h \
  $(ACE_ROOT)/ace/Event.inl \
  $(ACE_ROOT)/ace/Auto_Event.inl \
  $(ACE_ROOT)/ace/Barrier.h \
  $(ACE_ROOT)/ace/Condition_Thread_Mutex.h \
  $(ACE_ROOT)/ace/Condition_Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Barrier.inl \
  $(ACE_ROOT)/ace/Condition_Recursive_Thread_Mutex.h \
  $(ACE_ROOT)/ace/Recursive_Thread_Mutex.h \
  $(ACE_ROOT)/ace/Recursive_Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Condition_Recursive_Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Manual_Event.h \
  $(ACE_ROOT)/ace/Manual_Event.inl \
  $(ACE_ROOT)/ace/Mutex.h \
  $(ACE_ROOT)/ace/Mutex.inl \
  $(ACE_ROOT)/ace/Null_Barrier.h \
  $(ACE_ROOT)/ace/Null_Condition.h \
  $(ACE_ROOT)/ace/Null_Mutex.h \
  $(ACE_ROOT)/ace/Null_Semaphore.h \
  $(ACE_ROOT)/ace/RW_Mutex.h \
  $(ACE_ROOT)/ace/RW_Mutex.inl \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.h \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Semaphore.h \
  $(ACE_ROOT)/ace/Semaphore.inl \
  $(ACE_ROOT)/ace/Thread_Semaphore.h \
  $(ACE_ROOT)/ace/Thread_Semaphore.inl \
  $(ACE_ROOT)/ace/TSS_Adapter.h \
  $(ACE_ROOT)/ace/TSS_Adapter.inl \
  $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Lock_Adapter_T.h \
  $(ACE_ROOT)/ace/Lock_Adapter_T.inl \
  $(ACE_ROOT)/ace/Lock_Adapter_T.cpp \
  $(ACE_ROOT)/ace/Reverse_Lock_T.h \
  $(ACE_ROOT)/ace/Reverse_Lock_T.inl \
  $(ACE_ROOT)/ace/Reverse_Lock_T.cpp \
  $(ACE_ROOT)/ace/TSS_T.h \
  $(ACE_ROOT)/ace/TSS_T.inl \
  $(ACE_ROOT)/ace/TSS_T.cpp \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Condition_T.h \
  $(ACE_ROOT)/ace/Condition_T.inl \
  $(ACE_ROOT)/ace/Condition_T.cpp \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.h \
  $(ACE_ROOT)/ace/Map_Manager.h \
  $(ACE_ROOT)/ace/Map_Manager.i \
  $(ACE_ROOT)/ace/Map_Manager.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Module_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/old_event_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.h \
  $(ACE_ROOT)/ace/Unbounded_Set_Ex.h \
  $(ACE_ROOT)/ace/Unbounded_Set_Ex.inl \
  $(ACE_ROOT)/ace/Unbounded_Set_Ex.cpp \
  $(ACE_ROOT)/ace/Functor.h \
  $(ACE_ROOT)/ace/Functor.i \
  $(ACE_ROOT)/ace/Functor_T.h \
  $(ACE_ROOT)/ace/Functor_T.i \
  $(ACE_ROOT)/ace/Functor_T.cpp \
  $(TAO_ROOT)/tao/Timeprobe.h \
  $(ACE_ROOT)/ace/Timeprobe.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.h \
  $(ACE_ROOT)/ace/Task.h \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Singleton.h \
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
  $(ACE_ROOT)/ace/Thread_Manager.i \
  $(ACE_ROOT)/ace/Task.i \
  $(ACE_ROOT)/ace/Task_T.h \
  $(ACE_ROOT)/ace/Message_Queue.h \
  $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
  $(ACE_ROOT)/ace/Message_Queue_T.h \
  $(ACE_ROOT)/ace/Message_Queue_T.i \
  $(ACE_ROOT)/ace/Message_Queue_T.cpp \
  $(ACE_ROOT)/ace/Notification_Strategy.h \
  $(ACE_ROOT)/ace/Notification_Strategy.inl \
  $(ACE_ROOT)/ace/Message_Queue.i \
  $(ACE_ROOT)/ace/Task_T.i \
  $(ACE_ROOT)/ace/Task_T.cpp \
  $(ACE_ROOT)/ace/Module.h \
  $(ACE_ROOT)/ace/Module.i \
  $(ACE_ROOT)/ace/Module.cpp \
  $(ACE_ROOT)/ace/Stream_Modules.h \
  $(ACE_ROOT)/ace/Stream_Modules.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/ReactorTask.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Fast_Reactor.h \
  $(ACE_ROOT)/ace/Select_Reactor.h \
  $(ACE_ROOT)/ace/Select_Reactor_T.h \
  $(ACE_ROOT)/ace/Select_Reactor_Base.h \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Reactor_Timer_Interface.h \
  $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Token.h \
  $(ACE_ROOT)/ace/Token.i \
  $(ACE_ROOT)/ace/Pipe.h \
  $(ACE_ROOT)/ace/Pipe.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(ACE_ROOT)/ace/Select_Reactor_Base.i \
  $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Select_Reactor_T.cpp \
  $(ACE_ROOT)/ace/Timer_Heap.h \
  $(ACE_ROOT)/ace/Timer_Heap_T.h \
  $(ACE_ROOT)/ace/Timer_Heap_T.cpp \
  $(ACE_ROOT)/ace/Select_Reactor_T.i \
  $(ACE_ROOT)/ace/Timer_List.h \
  $(ACE_ROOT)/ace/Timer_List_T.h \
  $(ACE_ROOT)/ace/Timer_List_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Default_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Factory.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Factory.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Defaults.h \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Default_Factory.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Event_Channel.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Event_Channel_Base.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Event_Channel_Base.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Event_Channel.i \
  $(TAO_ROOT)/tao/BiDir_GIOP/BiDirGIOP.h \
  $(TAO_ROOT)/tao/BiDir_GIOP/bidirgiop_export.h \
  $(TAO_ROOT)/tao/BiDir_Adapter.h \
  $(TAO_ROOT)/tao/BiDir_GIOP/BiDirPolicyC.h \
  $(TAO_ROOT)/tao/BiDir_GIOP/BiDirPolicyC.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

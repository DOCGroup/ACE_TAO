# $Id$

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

SCHEDULE_SRCS = Scheduling_Service.cpp
LSRC =  $(SCHEDULE_SRCS)

SCHEDULE_OBJS = $(SCHEDULE_SRCS:.cpp=.o)


BIN2 = Scheduling_Service

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Sched,$(findstring Sched,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Sched

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs/orbsvcs/Sched -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)
LDLIBS = -lTAO_RTSched -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable $(TAO_SRVR_LIBS)

Scheduling_Service: $(addprefix $(VDIR),$(SCHEDULE_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Scheduling_Service.o .obj/Scheduling_Service.so .shobj/Scheduling_Service.o .shobj/Scheduling_Service.so: Scheduling_Service.cpp Scheduling_Service.h \
  $(TAO_ROOT)/tao/PortableServer/ORB_Manager.h \
  $(ACE_ROOT)/ace/pre.h \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_main.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.h \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(TAO_ROOT)/tao/Basic_Types.h \
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
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/CORBA_methods.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/Synch_Traits.h \
  $(ACE_ROOT)/ace/Lock.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.inl \
  $(TAO_ROOT)/tao/default_environment.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.inl \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/Exception.h \
  $(ACE_ROOT)/ace/SStringfwd.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/IOP_IORC.h \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Array_VarOut_T.h \
  $(TAO_ROOT)/tao/Array_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Array_VarOut_T.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Seq_Var_T.h \
  $(TAO_ROOT)/tao/Seq_Var_T.cpp \
  $(TAO_ROOT)/tao/Seq_Var_T.inl \
  $(TAO_ROOT)/tao/Seq_Out_T.h \
  $(TAO_ROOT)/tao/Seq_Out_T.cpp \
  $(TAO_ROOT)/tao/Seq_Out_T.inl \
  $(TAO_ROOT)/tao/CDR.h \
  $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(TAO_ROOT)/tao/VarOut_T.h \
  $(TAO_ROOT)/tao/VarOut_T.cpp \
  $(TAO_ROOT)/tao/VarOut_T.inl \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Object_Argument_T.h \
  $(TAO_ROOT)/tao/Argument.h \
  $(TAO_ROOT)/tao/Object_Argument_T.cpp \
  $(TAO_ROOT)/tao/Dynamic_ParameterC.h \
  $(TAO_ROOT)/tao/ParameterMode.h \
  $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/Arg_Traits_T.h \
  $(TAO_ROOT)/tao/Object_Argument_T.inl \
  $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/OBV_Constants.h \
  $(ACE_ROOT)/ace/Thread_Mutex.h \
  $(ACE_ROOT)/ace/OS_NS_Thread.h \
  $(ACE_ROOT)/ace/os_include/os_pthread.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/os_include/sys/os_sem.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/os_include/os_semaphore.h \
  $(ACE_ROOT)/ace/OS_NS_signal.h \
  $(ACE_ROOT)/ace/Guard_T.h \
  $(ACE_ROOT)/ace/Guard_T.cpp \
  $(ACE_ROOT)/ace/Guard_T.inl \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.h \
  $(ACE_ROOT)/ace/RW_Mutex.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Scheduler.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
  $(ACE_ROOT)/ace/Functor.h \
  $(ACE_ROOT)/ace/Functor_T.h \
  $(ACE_ROOT)/ace/Functor_T.cpp \
  $(ACE_ROOT)/ace/Functor_T.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
  $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
  $(ACE_ROOT)/ace/RB_Tree.h \
  $(ACE_ROOT)/ace/RB_Tree.cpp \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/OS_NS_fcntl.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
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
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.h \
  $(ACE_ROOT)/ace/os_include/net/os_if.h \
  $(ACE_ROOT)/ace/OS_NS_stropts.h \
  $(ACE_ROOT)/ace/os_include/os_stropts.h \
  $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/RB_Tree.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/naming_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/sched_export.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/svc_utils_export.h \
  $(TAO_ROOT)/tao/TimeBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.h \
  $(TAO_ROOT)/tao/TimeBaseS_T.cpp \
  $(TAO_ROOT)/tao/TimeBaseS_T.i \
  $(TAO_ROOT)/tao/Collocation_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Collocation_Strategy.h \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(ACE_ROOT)/ace/Atomic_Op.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/TimeBaseS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecBaseS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Sched_Utils.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Sched_Utils_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Sched_Utils_T.cpp \
  $(ACE_ROOT)/ace/Sched_Params.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Sched_Utils_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Scheduler_T.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Scheduler_T.cpp \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
  $(ACE_ROOT)/ace/OS_NS_time.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Reconfig_Scheduler_T.i \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.h \
  $(ACE_ROOT)/ace/OS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
  $(ACE_ROOT)/ace/Synch.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.h \
  $(ACE_ROOT)/ace/Map_Manager.h \
  $(ACE_ROOT)/ace/Map_Manager.cpp \
  $(ACE_ROOT)/ace/Map_Manager.i \
  $(ACE_ROOT)/ace/Recursive_Thread_Mutex.h \
  $(ACE_ROOT)/ace/Get_Opt.h \
  $(ACE_ROOT)/ace/Argv_Type_Converter.h \
  $(ACE_ROOT)/ace/OS_NS_stdio.h \
  $(ACE_ROOT)/ace/OS_NS_strings.h \
  $(ACE_ROOT)/ace/os_include/os_strings.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

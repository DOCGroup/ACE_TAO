#
# $Id$
#

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

BIN_UNCHECKED = POA_BiDir

LSRC = 	$(addsuffix .cpp,$(BIN))

CPPFLAGS += -I$(TAO_ROOT)

LDLIBS   = -lTAO_BiDirGIOP -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor  -lTAO 
LDFLAGS += -L$(TAO_ROOT)/tao

INSTALL =

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(minimum_corba),0)
  BIN = $(BIN_UNCHECKED)
endif # minimum_corba

include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/POA_BiDir.o .obj/POA_BiDir.so .shobj/POA_BiDir.o .shobj/POA_BiDir.so: POA_BiDir.cpp \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(ACE_ROOT)/ace/pre.h \
  $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
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
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(TAO_ROOT)/tao/varbase.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.inl \
  $(TAO_ROOT)/tao/Objref_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/CORBA_methods.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Synch_Traits.h \
  $(ACE_ROOT)/ace/Lock.h \
  $(ACE_ROOT)/ace/Lock.inl \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.inl \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.cpp \
  $(TAO_ROOT)/tao/default_environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
  $(TAO_ROOT)/tao/Exception.h \
  $(ACE_ROOT)/ace/SStringfwd.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(TAO_ROOT)/tao/Exception.i \
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
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Array_VarOut_T.h \
  $(TAO_ROOT)/tao/Array_VarOut_T.inl \
  $(TAO_ROOT)/tao/Array_VarOut_T.cpp \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(TAO_ROOT)/tao/Seq_Var_T.h \
  $(TAO_ROOT)/tao/Seq_Var_T.inl \
  $(TAO_ROOT)/tao/Seq_Var_T.cpp \
  $(TAO_ROOT)/tao/Seq_Out_T.h \
  $(TAO_ROOT)/tao/Seq_Out_T.inl \
  $(TAO_ROOT)/tao/Seq_Out_T.cpp \
  $(TAO_ROOT)/tao/CDR.h \
  $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/os_include/os_string.h \
  $(ACE_ROOT)/ace/os_include/os_strings.h \
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/CDR_Stream.i \
  $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/VarOut_T.h \
  $(TAO_ROOT)/tao/VarOut_T.inl \
  $(TAO_ROOT)/tao/VarOut_T.cpp \
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
  $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
  $(TAO_ROOT)/tao/BiDir_GIOP/BiDirGIOP.h \
  $(TAO_ROOT)/tao/BiDir_GIOP/bidirgiop_export.h \
  $(TAO_ROOT)/tao/BiDir_Adapter.h \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Atomic_Op.h \
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
  $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Guard_T.h \
  $(ACE_ROOT)/ace/Guard_T.inl \
  $(ACE_ROOT)/ace/Guard_T.cpp \
  $(ACE_ROOT)/ace/Atomic_Op_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Service_Config.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
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
  $(ACE_ROOT)/ace/XML_Svc_Conf.h \
  $(ACE_ROOT)/ace/Service_Config.i \
  $(ACE_ROOT)/ace/Reactor.h \
  $(ACE_ROOT)/ace/Handle_Set.h \
  $(ACE_ROOT)/ace/Handle_Set.i \
  $(ACE_ROOT)/ace/Timer_Queue.h \
  $(ACE_ROOT)/ace/Timer_Queue_T.h \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Timer_Queue_T.i \
  $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Reactor_Timer_Interface.h \
  $(ACE_ROOT)/ace/Null_Mutex.h \
  $(ACE_ROOT)/ace/Reactor.i \
  $(ACE_ROOT)/ace/Reactor_Impl.h \
  $(TAO_ROOT)/tao/BiDir_GIOP/BiDirPolicyC.h \
  $(TAO_ROOT)/tao/BiDir_GIOP/BiDirPolicyC.i \
  $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/OBV_Constants.h \
  $(TAO_ROOT)/tao/ORB.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

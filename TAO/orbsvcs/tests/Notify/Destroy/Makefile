# $Id$
#----------------------------------------------------------------------------
#       GNU Makefile
#----------------------------------------------------------------------------
MAKEFILE        = Makefile
BIN_UNCHECKED   = Notify_Tests_Destroy

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif



FILES = \
  main

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

CURRENT_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Notify, $(findstring Notify, $(CURRENT_COMPONENTS)))
ifeq (Naming, $(findstring Naming, $(CURRENT_COMPONENTS)))
BIN    = $(BIN_UNCHECKED)
endif
endif

OBJS   = $(addsuffix .o, $(notdir $(FILES)))
SRC    = $(addsuffix .cpp, $(FILES))
ACELIB = -lACE

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk
include $(TAO_ROOT)/orbsvcs/tests/Notify/notify_tests.GNU

CPPFLAGS     += -I$(TAO_ROOT)/orbsvcs/tests/Notify/lib -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao -I$(ACE_ROOT)
ifeq ($(static_libs),1)
  ifneq ($(LIB),)
    CPPFLAGS += -DTAO_AS_STATIC_LIBS -DACE_AS_STATIC_LIBS
  endif
endif


LDFLAGS      += -L$(TAO_ROOT)/orbsvcs/tests/Notify/lib -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

$(BIN): $(addprefix $(VDIR), $(OBJS))
	$(LINK.cc) $(LDFLAGS) $(CC_OUTPUT_FLAG) $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- depgen.pl uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/main.o .obj/main.so .shobj/main.o .shobj/main.so: \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/ARGV.h \
  $(ACE_ROOT)/ace/ARGV.i \
  $(ACE_ROOT)/ace/Argv_Type_Converter.h \
  $(ACE_ROOT)/ace/Argv_Type_Converter.inl \
  $(ACE_ROOT)/ace/Atomic_Op.h \
  $(ACE_ROOT)/ace/Atomic_Op.i \
  $(ACE_ROOT)/ace/Atomic_Op_T.h \
  $(ACE_ROOT)/ace/Atomic_Op_T.i \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/CDR_Base.h \
  $(ACE_ROOT)/ace/CDR_Base.inl \
  $(ACE_ROOT)/ace/CDR_Stream.h \
  $(ACE_ROOT)/ace/CDR_Stream.i \
  $(ACE_ROOT)/ace/CORBA_macros.h \
  $(ACE_ROOT)/ace/Cleanup.h \
  $(ACE_ROOT)/ace/Cleanup.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Exception_Macros.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Guard_T.cpp \
  $(ACE_ROOT)/ace/Guard_T.h \
  $(ACE_ROOT)/ace/Guard_T.inl \
  $(ACE_ROOT)/ace/Lock.h \
  $(ACE_ROOT)/ace/Lock.inl \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Msg_Callback.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Node.h \
  $(ACE_ROOT)/ace/Null_Mutex.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_NS_Thread.h \
  $(ACE_ROOT)/ace/OS_NS_Thread.inl \
  $(ACE_ROOT)/ace/OS_NS_errno.h \
  $(ACE_ROOT)/ace/OS_NS_errno.inl \
  $(ACE_ROOT)/ace/OS_NS_fcntl.h \
  $(ACE_ROOT)/ace/OS_NS_fcntl.inl \
  $(ACE_ROOT)/ace/OS_NS_macros.h \
  $(ACE_ROOT)/ace/OS_NS_pwd.h \
  $(ACE_ROOT)/ace/OS_NS_pwd.inl \
  $(ACE_ROOT)/ace/OS_NS_signal.h \
  $(ACE_ROOT)/ace/OS_NS_signal.inl \
  $(ACE_ROOT)/ace/OS_NS_stdio.h \
  $(ACE_ROOT)/ace/OS_NS_stdio.inl \
  $(ACE_ROOT)/ace/OS_NS_stdlib.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.inl \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/OS_NS_string.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.h \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_stat.h \
  $(ACE_ROOT)/ace/OS_NS_sys_stat.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_time.h \
  $(ACE_ROOT)/ace/OS_NS_sys_time.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.h \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.inl \
  $(ACE_ROOT)/ace/OS_NS_time.h \
  $(ACE_ROOT)/ace/OS_NS_time.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.h \
  $(ACE_ROOT)/ace/OS_NS_unistd.inl \
  $(ACE_ROOT)/ace/OS_NS_wchar.h \
  $(ACE_ROOT)/ace/OS_NS_wchar.inl \
  $(ACE_ROOT)/ace/OS_main.h \
  $(ACE_ROOT)/ace/Object_Manager_Base.h \
  $(ACE_ROOT)/ace/Object_Manager_Base.inl \
  $(ACE_ROOT)/ace/RW_Mutex.h \
  $(ACE_ROOT)/ace/RW_Mutex.inl \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.h \
  $(ACE_ROOT)/ace/RW_Thread_Mutex.inl \
  $(ACE_ROOT)/ace/SStringfwd.h \
  $(ACE_ROOT)/ace/Synch_Traits.h \
  $(ACE_ROOT)/ace/Thread_Mutex.h \
  $(ACE_ROOT)/ace/Thread_Mutex.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Trace.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/config-all.h \
  $(ACE_ROOT)/ace/config-borland-common.h \
  $(ACE_ROOT)/ace/config-cxx-common.h \
  $(ACE_ROOT)/ace/config-g++-common.h \
  $(ACE_ROOT)/ace/config-icc-common.h \
  $(ACE_ROOT)/ace/config-kcc-common.h \
  $(ACE_ROOT)/ace/config-linux-common.h \
  $(ACE_ROOT)/ace/config-linux.h \
  $(ACE_ROOT)/ace/config-lite.h \
  $(ACE_ROOT)/ace/config.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(ACE_ROOT)/ace/os_include/arpa/os_inet.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_in.h \
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_netdb.h \
  $(ACE_ROOT)/ace/os_include/os_pthread.h \
  $(ACE_ROOT)/ace/os_include/os_pwd.h \
  $(ACE_ROOT)/ace/os_include/os_search.h \
  $(ACE_ROOT)/ace/os_include/os_semaphore.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/os_string.h \
  $(ACE_ROOT)/ace/os_include/os_stropts.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/os_wchar.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/os_include/sys/os_sem.h \
  $(ACE_ROOT)/ace/os_include/sys/os_socket.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/sys/os_uio.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/streams.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyChannelAdminS.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/CosNotifyCommC.h \
  $(TAO_ROOT)/orbsvcs/orbsvcs/NotifyExtC.h \
  $(TAO_ROOT)/orbsvcs/tests/Notify/lib/notify_test_export.h \
  $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
  $(TAO_ROOT)/tao/Any.h \
  $(TAO_ROOT)/tao/Any.i \
  $(TAO_ROOT)/tao/Arg_Traits_T.h \
  $(TAO_ROOT)/tao/Argument.h \
  $(TAO_ROOT)/tao/Array_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Array_VarOut_T.h \
  $(TAO_ROOT)/tao/Array_VarOut_T.inl \
  $(TAO_ROOT)/tao/Basic_Types.h \
  $(TAO_ROOT)/tao/CDR.h \
  $(TAO_ROOT)/tao/CDR.i \
  $(TAO_ROOT)/tao/CORBA_String.h \
  $(TAO_ROOT)/tao/CORBA_String.inl \
  $(TAO_ROOT)/tao/CORBA_methods.h \
  $(TAO_ROOT)/tao/Collocation_Proxy_Broker.h \
  $(TAO_ROOT)/tao/Collocation_Strategy.h \
  $(TAO_ROOT)/tao/CurrentC.h \
  $(TAO_ROOT)/tao/CurrentC.i \
  $(TAO_ROOT)/tao/Dynamic_ParameterC.h \
  $(TAO_ROOT)/tao/Dynamic_ParameterC.i \
  $(TAO_ROOT)/tao/Environment.h \
  $(TAO_ROOT)/tao/Environment.i \
  $(TAO_ROOT)/tao/Exception.h \
  $(TAO_ROOT)/tao/Exception.i \
  $(TAO_ROOT)/tao/IOP_IORC.h \
  $(TAO_ROOT)/tao/IOP_IORC.i \
  $(TAO_ROOT)/tao/Managed_Types.h \
  $(TAO_ROOT)/tao/Managed_Types.i \
  $(TAO_ROOT)/tao/OBV_Constants.h \
  $(TAO_ROOT)/tao/ORB.h \
  $(TAO_ROOT)/tao/ORB.i \
  $(TAO_ROOT)/tao/Object.h \
  $(TAO_ROOT)/tao/Object.i \
  $(TAO_ROOT)/tao/Object_Argument_T.cpp \
  $(TAO_ROOT)/tao/Object_Argument_T.h \
  $(TAO_ROOT)/tao/Object_Argument_T.inl \
  $(TAO_ROOT)/tao/Objref_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Objref_VarOut_T.h \
  $(TAO_ROOT)/tao/Objref_VarOut_T.inl \
  $(TAO_ROOT)/tao/OctetSeqC.h \
  $(TAO_ROOT)/tao/OctetSeqC.i \
  $(TAO_ROOT)/tao/ParameterMode.h \
  $(TAO_ROOT)/tao/PolicyC.h \
  $(TAO_ROOT)/tao/PolicyC.i \
  $(TAO_ROOT)/tao/Policy_ForwardC.h \
  $(TAO_ROOT)/tao/Policy_ForwardC.i \
  $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
  $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.cpp \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.h \
  $(TAO_ROOT)/tao/Pseudo_VarOut_T.inl \
  $(TAO_ROOT)/tao/Seq_Out_T.cpp \
  $(TAO_ROOT)/tao/Seq_Out_T.h \
  $(TAO_ROOT)/tao/Seq_Out_T.inl \
  $(TAO_ROOT)/tao/Seq_Var_T.cpp \
  $(TAO_ROOT)/tao/Seq_Var_T.h \
  $(TAO_ROOT)/tao/Seq_Var_T.inl \
  $(TAO_ROOT)/tao/Sequence.h \
  $(TAO_ROOT)/tao/Sequence.i \
  $(TAO_ROOT)/tao/Sequence_T.cpp \
  $(TAO_ROOT)/tao/Sequence_T.h \
  $(TAO_ROOT)/tao/Sequence_T.i \
  $(TAO_ROOT)/tao/TAO_Export.h \
  $(TAO_ROOT)/tao/VarOut_T.cpp \
  $(TAO_ROOT)/tao/VarOut_T.h \
  $(TAO_ROOT)/tao/VarOut_T.inl \
  $(TAO_ROOT)/tao/default_environment.h \
  $(TAO_ROOT)/tao/objectid.h \
  $(TAO_ROOT)/tao/orbconf.h \
  $(TAO_ROOT)/tao/varbase.h \
  ./../lib/Notify_Test_Client.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

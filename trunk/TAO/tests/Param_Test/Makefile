#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the Param_Test test example
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS   = -lTAO
LDFLAGS += -L$(TAO_ROOT)/tao

IDL_HDR	= param_testCli.h param_testSer.h
IDL_SRC	= param_testCli.cpp param_testSer.cpp

SVR_FILES =	param_testCli	\
		param_testSer	\
		server		\
		param_test_i	\
		tmplinst

CLI_FILES =	param_testCli	\
		param_testSer	\
		driver		\
		client		\
		options		\
		results		\
		helper		\
		tmplinst	\
		any		\
		bd_long_seq	\
		bd_short_seq	\
		bd_str_seq	\
		bd_string	\
		bd_struct_seq	\
		fixed_array	\
		fixed_struct	\
		nested_struct	\
		objref_struct	\
		objref		\
		short		\
		ulonglong	\
		typecode	\
		ub_any_seq	\
		ub_long_seq	\
		ub_objref_seq	\
		ub_short_seq	\
		ub_str_seq	\
		ub_string	\
		ub_struct_seq	\
		var_array	\
		var_struct \
		except \
		big_union

PARAM_TEST_SVR_OBJS = $(addsuffix .o,$(SVR_FILES))
PARAM_TEST_CLT_OBJS = $(addsuffix .o,$(CLI_FILES))
SRC	= $(addsuffix .cpp,$(SVR_FILES)) \
	  $(addsuffix .cpp,$(CLI_FILES))

BIN = server client
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)

  # Since older g++ cannot handle nested classes properly, this test
  # will not build on g++ prior to 2.8.0.
  all clean realclean:
	@echo tests/Param_Test is not built with older versions of g++

else # ! ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

override TAO_IDLFLAGS += -hc Cli.h -hs Ser.h -hT Ser_T.h \
			 -cs Cli.cpp -ci Cli.i \
			 -ss Ser.cpp -sT Ser_T.cpp  -si Ser.i -st Ser_T.i

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
CPPFLAGS += -I$(TAO_ROOT) $(TSS_ORB_FLAG)

.PRECIOUS: $(IDL_HDR) $(IDL_SRC)
#$(IDL_HDR) $(IDL_SRC): param_test.idl
#	$(TAO_IDL/tao_idl param_test.idl

server:	$(addprefix $(VDIR),$(PARAM_TEST_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(PARAM_TEST_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

clean:
	-/bin/rm -rf $(VDIR)/*.o $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf param_testCli.* param_testSer.* param_testSer_T.*

endif # ! ACE_HAS_GNUG_PRE_2_8

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/param_testCli.o .obj/param_testCli.so .shobj/param_testCli.o .shobj/param_testCli.so: param_testCli.cpp param_testCli.h param_testCli.i \
 param_testSer.h param_testSer_T.h param_testSer_T.i \
 param_testSer_T.cpp param_testSer.i
.obj/param_testSer.o .obj/param_testSer.so .shobj/param_testSer.o .shobj/param_testSer.so: param_testSer.cpp param_testSer.h param_testCli.h \
 param_testCli.i param_testSer_T.h param_testSer_T.i \
 param_testSer_T.cpp param_testSer.i
.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.i \
 param_test_i.h param_testSer.h param_testCli.h param_testCli.i \
 param_testSer_T.h param_testSer_T.i param_testSer_T.cpp \
 param_testSer.i
.obj/param_test_i.o .obj/param_test_i.so .shobj/param_test_i.o .shobj/param_test_i.so: param_test_i.cpp \
 $(TAO_ROOT)/tao/corba.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/params.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/IIOP_Acceptor.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h \
 param_test_i.h param_testSer.h param_testCli.h param_testCli.i \
 param_testSer_T.h param_testSer_T.i param_testSer_T.cpp \
 param_testSer.i
.obj/tmplinst.o .obj/tmplinst.so .shobj/tmplinst.o .shobj/tmplinst.so: tmplinst.cpp param_testCli.h param_testCli.i
.obj/param_testCli.o .obj/param_testCli.so .shobj/param_testCli.o .shobj/param_testCli.so: param_testCli.cpp param_testCli.h param_testCli.i \
 param_testSer.h param_testSer_T.h param_testSer_T.i \
 param_testSer_T.cpp param_testSer.i
.obj/param_testSer.o .obj/param_testSer.so .shobj/param_testSer.o .shobj/param_testSer.so: param_testSer.cpp param_testSer.h param_testCli.h \
 param_testCli.i param_testSer_T.h param_testSer_T.i \
 param_testSer_T.cpp param_testSer.i
.obj/driver.o .obj/driver.so .shobj/driver.o .shobj/driver.so: driver.cpp driver.h options.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/params.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/IIOP_Acceptor.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h \
 param_testCli.h param_testCli.i results.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 client.h client.cpp tests.h any.h bd_long_seq.h bd_short_seq.h \
 bd_str_seq.h bd_string.h bd_struct_seq.h fixed_array.h fixed_struct.h \
 nested_struct.h objref_struct.h objref.h short.h ulonglong.h \
 typecode.h ub_any_seq.h ub_long_seq.h ub_objref_seq.h ub_short_seq.h \
 ub_str_seq.h ub_string.h ub_struct_seq.h var_array.h var_struct.h \
 except.h big_union.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp options.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/params.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/IIOP_Acceptor.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h results.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 client.h param_testCli.h param_testCli.i client.cpp
.obj/options.o .obj/options.so .shobj/options.o .shobj/options.so: options.cpp \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i options.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/params.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/IIOP_Acceptor.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h
.obj/results.o .obj/results.so .shobj/results.o .shobj/results.so: results.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i results.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/varout.i \
 $(TAO_ROOT)/tao/varout.cpp \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/Servant_Base.i \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/DynAnyC.h \
 $(TAO_ROOT)/tao/DynAnyC.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Active_Object_Map.i \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/params.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(TAO_ROOT)/tao/IIOP_Connector.h \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/IIOP_Acceptor.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/InconsistentTypeCodeC.h \
 $(TAO_ROOT)/tao/DynAny_i.h \
 $(TAO_ROOT)/tao/Union.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i
.obj/helper.o .obj/helper.so .shobj/helper.o .shobj/helper.so: helper.cpp helper.h param_testCli.h param_testCli.i
.obj/tmplinst.o .obj/tmplinst.so .shobj/tmplinst.o .shobj/tmplinst.so: tmplinst.cpp param_testCli.h param_testCli.i
.obj/any.o .obj/any.so .shobj/any.o .shobj/any.so: any.cpp helper.h param_testCli.h param_testCli.i any.h
.obj/bd_long_seq.o .obj/bd_long_seq.so .shobj/bd_long_seq.o .shobj/bd_long_seq.so: bd_long_seq.cpp helper.h param_testCli.h \
 param_testCli.i bd_long_seq.h
.obj/bd_short_seq.o .obj/bd_short_seq.so .shobj/bd_short_seq.o .shobj/bd_short_seq.so: bd_short_seq.cpp helper.h param_testCli.h \
 param_testCli.i bd_short_seq.h
.obj/bd_str_seq.o .obj/bd_str_seq.so .shobj/bd_str_seq.o .shobj/bd_str_seq.so: bd_str_seq.cpp helper.h param_testCli.h param_testCli.i \
 bd_str_seq.h
.obj/bd_string.o .obj/bd_string.so .shobj/bd_string.o .shobj/bd_string.so: bd_string.cpp helper.h param_testCli.h param_testCli.i \
 bd_string.h
.obj/bd_struct_seq.o .obj/bd_struct_seq.so .shobj/bd_struct_seq.o .shobj/bd_struct_seq.so: bd_struct_seq.cpp helper.h param_testCli.h \
 param_testCli.i bd_struct_seq.h
.obj/fixed_array.o .obj/fixed_array.so .shobj/fixed_array.o .shobj/fixed_array.so: fixed_array.cpp helper.h param_testCli.h \
 param_testCli.i fixed_array.h
.obj/fixed_struct.o .obj/fixed_struct.so .shobj/fixed_struct.o .shobj/fixed_struct.so: fixed_struct.cpp helper.h param_testCli.h \
 param_testCli.i fixed_struct.h
.obj/nested_struct.o .obj/nested_struct.so .shobj/nested_struct.o .shobj/nested_struct.so: nested_struct.cpp helper.h param_testCli.h \
 param_testCli.i nested_struct.h
.obj/objref_struct.o .obj/objref_struct.so .shobj/objref_struct.o .shobj/objref_struct.so: objref_struct.cpp helper.h param_testCli.h \
 param_testCli.i objref_struct.h
.obj/objref.o .obj/objref.so .shobj/objref.o .shobj/objref.so: objref.cpp helper.h param_testCli.h param_testCli.i objref.h
.obj/short.o .obj/short.so .shobj/short.o .shobj/short.so: short.cpp helper.h param_testCli.h param_testCli.i short.h
.obj/ulonglong.o .obj/ulonglong.so .shobj/ulonglong.o .shobj/ulonglong.so: ulonglong.cpp helper.h param_testCli.h param_testCli.i \
 ulonglong.h
.obj/typecode.o .obj/typecode.so .shobj/typecode.o .shobj/typecode.so: typecode.cpp helper.h param_testCli.h param_testCli.i \
 typecode.h
.obj/ub_any_seq.o .obj/ub_any_seq.so .shobj/ub_any_seq.o .shobj/ub_any_seq.so: ub_any_seq.cpp helper.h param_testCli.h param_testCli.i \
 ub_any_seq.h
.obj/ub_long_seq.o .obj/ub_long_seq.so .shobj/ub_long_seq.o .shobj/ub_long_seq.so: ub_long_seq.cpp helper.h param_testCli.h \
 param_testCli.i ub_long_seq.h
.obj/ub_objref_seq.o .obj/ub_objref_seq.so .shobj/ub_objref_seq.o .shobj/ub_objref_seq.so: ub_objref_seq.cpp helper.h param_testCli.h \
 param_testCli.i ub_objref_seq.h
.obj/ub_short_seq.o .obj/ub_short_seq.so .shobj/ub_short_seq.o .shobj/ub_short_seq.so: ub_short_seq.cpp helper.h param_testCli.h \
 param_testCli.i ub_short_seq.h
.obj/ub_str_seq.o .obj/ub_str_seq.so .shobj/ub_str_seq.o .shobj/ub_str_seq.so: ub_str_seq.cpp helper.h param_testCli.h param_testCli.i \
 ub_str_seq.h
.obj/ub_string.o .obj/ub_string.so .shobj/ub_string.o .shobj/ub_string.so: ub_string.cpp helper.h param_testCli.h param_testCli.i \
 ub_string.h
.obj/ub_struct_seq.o .obj/ub_struct_seq.so .shobj/ub_struct_seq.o .shobj/ub_struct_seq.so: ub_struct_seq.cpp helper.h param_testCli.h \
 param_testCli.i ub_struct_seq.h
.obj/var_array.o .obj/var_array.so .shobj/var_array.o .shobj/var_array.so: var_array.cpp helper.h param_testCli.h param_testCli.i \
 var_array.h
.obj/var_struct.o .obj/var_struct.so .shobj/var_struct.o .shobj/var_struct.so: var_struct.cpp helper.h param_testCli.h param_testCli.i \
 var_struct.h
.obj/except.o .obj/except.so .shobj/except.o .shobj/except.so: except.cpp helper.h param_testCli.h param_testCli.i except.h
.obj/big_union.o .obj/big_union.so .shobj/big_union.o .shobj/big_union.so: big_union.cpp helper.h param_testCli.h param_testCli.i \
 big_union.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

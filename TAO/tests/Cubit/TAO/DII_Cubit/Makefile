#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the ACE-ified Sun Ref. implementation
#	of IIOP ORB
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     =  -lTAO

PROG_SRCS =	svr.cpp clnt.cpp cubit.cpp \
		test1.cpp test1_clnt.cpp test1_svr.cpp

LSRC = $(PROG_SRCS)

CUBIT_SVR_OBJS = svr.o cubit_i.o cubitS.o
CUBIT_CLT_OBJS = clnt.o cubitC.o

BASIC_SVR_OBJS = test1.o test1_svr.o
BASIC_CLT_OBJS = test1.o test1_clnt.o

ECHO_SVR_OBJS = test1.o
ECHO_CLT_OBJS = test1.o

BIN = svr clnt
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

# Local modifications to variables imported by includes above.
#LDFLAGS    += -L../../../proto/lib
#CPPFLAGS   += -I../../../proto/include -DUSE_ACE_EVENT_HANDLING -DDEBUG
OCFLAGS = 

svr:	$(addprefix $(VDIR),$(CUBIT_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

clnt:	$(addprefix $(VDIR),$(CUBIT_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

test1_svr:	$(addprefix $(VDIR),$(BASIC_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

test1_clnt:	$(addprefix $(VDIR),$(BASIC_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

########
# Sanity check builds by running basic functionality tests.
#
# "sleep 5" in the server startup is usually enough to get the
# objref into the file so the client can read it.
#
check:	$(TESTS)
	@echo "testing with 'cube' calls, stub + DII, IOR strings"
	@./svr -i30 -o non-internet > obj.1 & sleep 5
	@./clnt -n250 -O `cat obj.1` -x
	@echo ''
	@echo "testing request forwarding with 'cube' calls, stub + DII"
	@./svr -f -i30 > obj.2 & sleep 5
	@./clnt -n250 -O `cat obj.2` -x
	@echo ''
	@echo "testing transmission of primitive data types"
	@./test1_svr -i30 > obj.3 & sleep 5
	@./test1_clnt -n50 -O `cat obj.3` -x
	@echo ''
#	@echo "testing echo of primitive data values"
#	@./echo_svr -i30 > obj.4 & sleep 5
#	@./echo_clnt -O `cat obj.4` -x
#	@echo ''
	@echo "testing with 'cube' calls, MT-ized (no forwarding)"
	@./svr -t -i30 -o non-internet > obj.5 & sleep 5
	@./clnt -n250 -O `cat obj.5` -x
	@echo ''

ifdef LOCALRULES
########
# CUBIT test
svr:		svr.o cubit.o
	$(LINK.cc) -o svr svr.o cubit.o $(LDLIBS) $(POSTLINK)
clnt:		cubit.o clnt.o
	$(LINK.cc) -o clnt clnt.o cubit.o $(LDLIBS) $(POSTLINK)

########
# BASIC DATATYPES test
test1_clnt:	test1.o test1_clnt.o
	$(LINK.cc) -o test1_clnt test1_clnt.o test1.o $(LDLIBS) $(POSTLINK)
test1_svr:	test1.o test1_svr.o
	$(LINK.cc) -o test1_svr test1_svr.o test1.o $(LDLIBS) $(POSTLINK)

########
# ECHO test ... "test1" where the operation semantics are violated;
# this aids some porting work, but is a less rigorous test
echo_clnt:	test1.o echo_clnt.o
	$(LINK.cc) -o echo_clnt echo_clnt.o test1.o $(LDLIBS) $(POSTLINK)
echo_svr:	test1.o echo_svr.o
	$(LINK.cc) -o echo_svr echo_svr.o test1.o $(LDLIBS) $(POSTLINK)
endif

	-rm -rf *.o $(BIN) obj.* core Templates.DB .make.state

	-@echo "Nothing to install, these are tests!"





# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/svr.o .obj/svr.so .shobj/svr.o .shobj/svr.so: svr.cpp \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(TAO_ROOT)/tao/compat/objbase.h \
 $(TAO_ROOT)/tao/compat/initguid.h \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/PolicyS.h \
 $(TAO_ROOT)/tao/PolicyS.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Object_Table.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h \
 cubit_i.h cubitS.h cubitC.h
.obj/clnt.o .obj/clnt.so .shobj/clnt.o .shobj/clnt.so: clnt.cpp \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(TAO_ROOT)/tao/compat/objbase.h \
 $(TAO_ROOT)/tao/compat/initguid.h \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/PolicyS.h \
 $(TAO_ROOT)/tao/PolicyS.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Object_Table.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h \
 cubitC.h clnt.h
.obj/cubit.o .obj/cubit.so .shobj/cubit.o .shobj/cubit.so: cubit.cpp cubit.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(TAO_ROOT)/tao/compat/objbase.h \
 $(TAO_ROOT)/tao/compat/initguid.h \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/PolicyS.h \
 $(TAO_ROOT)/tao/PolicyS.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Object_Table.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h
.obj/test1.o .obj/test1.so .shobj/test1.o .shobj/test1.so: test1.cpp test1.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(TAO_ROOT)/tao/compat/objbase.h \
 $(TAO_ROOT)/tao/compat/initguid.h \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/PolicyS.h \
 $(TAO_ROOT)/tao/PolicyS.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Object_Table.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h
.obj/test1_clnt.o .obj/test1_clnt.so .shobj/test1_clnt.o .shobj/test1_clnt.so: test1_clnt.cpp test1.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(TAO_ROOT)/tao/compat/objbase.h \
 $(TAO_ROOT)/tao/compat/initguid.h \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/PolicyS.h \
 $(TAO_ROOT)/tao/PolicyS.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Object_Table.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h
.obj/test1_svr.o .obj/test1_svr.so .shobj/test1_svr.o .shobj/test1_svr.so: test1_svr.cpp test1.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(TAO_ROOT)/tao/compat/objbase.h \
 $(TAO_ROOT)/tao/compat/initguid.h \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/PolicyS.h \
 $(TAO_ROOT)/tao/PolicyS.i \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Object_Table.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

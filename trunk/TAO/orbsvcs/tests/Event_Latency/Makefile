#
# $Id$
#

BIN = Event_Latency

BUILD = $(BIN)

EVENT_LATENCY_SRCS= \
	Event_Latency.cpp

LSRC= \
	$(EVENT_LATENCY_SRCS) \

EVENT_LATENCY_OBJS = $(EVENT_LATENCY_SRCS:.cpp=.o)

LDLIBS= -lorbsvcs -lTAO

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#### compiler-specific options
ifeq ($(CXX),g++)
  CCFLAGS += -pedantic
else
ifeq ($(CXX),CC)
endif
endif

ifdef quantify
  CCFLAGS += -Dquantify
  CPPFLAGS += -I/pkg/purify/quantify-2.1-solaris2
endif # quantify

#### Local rules and variables...

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

# Leave the scheduler output out if this is a config run.
ifeq ($(runtime),1)
EVENT_LATENCY_CONFIG_OBJS=Event_Latency_Scheduler_Runtime.o
endif # runtime

ifeq ($(probe),1)
    CCFLAGS += -DACE_ENABLE_TIMEPROBES
endif # probe

Event_Latency: $(addprefix $(VDIR),$(EVENT_LATENCY_OBJS) $(EVENT_LATENCY_CONFIG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Event_Latency.o .shobj/Event_Latency.: Event_Latency.cpp \
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
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Sched_Params.h \
 $(ACE_ROOT)/ace/Sched_Params.i \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(TAO_ROOT)/tao/Timeprobe.h \
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
 $(TAO_ROOT)/tao/Timeprobe.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
 $(TAO_ROOT)/tao/corba.h \
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
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
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
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
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
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
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
 $(TAO_ROOT)/tao/orbconf.h \
 $(TAO_ROOT)/tao/orb.h \
 $(TAO_ROOT)/tao/corbacom.h \
 $(TAO_ROOT)/tao/object.h \
 $(TAO_ROOT)/tao/align.h \
 $(TAO_ROOT)/tao/sequence.h \
 $(TAO_ROOT)/tao/varout.h \
 $(TAO_ROOT)/tao/any.h \
 $(TAO_ROOT)/tao/poa.h \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/client_factory.h \
 $(TAO_ROOT)/tao/server_factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/except.h \
 $(TAO_ROOT)/tao/orbobj.h \
 $(TAO_ROOT)/tao/nvlist.h \
 $(TAO_ROOT)/tao/principa.h \
 $(TAO_ROOT)/tao/request.h \
 $(TAO_ROOT)/tao/svrrqst.h \
 $(TAO_ROOT)/tao/typecode.h \
 $(TAO_ROOT)/tao/marshal.h \
 $(TAO_ROOT)/tao/cdr.h \
 $(TAO_ROOT)/tao/stub.h \
 $(TAO_ROOT)/tao/connect.h \
 $(TAO_ROOT)/tao/orb_core.h \
 $(TAO_ROOT)/tao/objtable.h \
 $(TAO_ROOT)/tao/optable.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/iiopobj.h \
 $(TAO_ROOT)/tao/iioporb.h \
 $(TAO_ROOT)/tao/giop.h \
 $(TAO_ROOT)/tao/orb_core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(TAO_ROOT)/tao/corbacom.i \
 $(TAO_ROOT)/tao/sequence.i \
 $(TAO_ROOT)/tao/typecode.i \
 $(TAO_ROOT)/tao/any.i \
 $(TAO_ROOT)/tao/stub.i \
 $(TAO_ROOT)/tao/object.i \
 $(TAO_ROOT)/tao/orbobj.i \
 $(TAO_ROOT)/tao/marshal.i \
 $(TAO_ROOT)/tao/cdr.i \
 $(TAO_ROOT)/tao/poa.i \
 $(TAO_ROOT)/tao/giop.i \
 $(TAO_ROOT)/tao/iioporb.i \
 $(TAO_ROOT)/tao/iiopobj.i \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/server_factory.i \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/connect.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.i \
 Event_Latency.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

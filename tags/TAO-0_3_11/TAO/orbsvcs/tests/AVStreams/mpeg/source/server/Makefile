#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
BIN =  augmented_server server vs as 

BUILD = $(BIN)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

AV_SERVER_OBJS = server.o 
AV_AUGSERVER_OBJS = Video_RepositoryC.o augmented_server.o \
		    Property_Exporter.o Machine_Properties.o \
		    Video_Repository.o 
AV_VS_OBJS = vs.o
AV_AS_OBJS = as.o

# On non-Windows environment, we should at least define
# the export_include IDL flag.
override TAO_IDLFLAGS += \
	-I$(TAO_ROOT)/orbsvcs/orbsvcs

IDL_FILES = Video_RepositoryC  Video_RepositoryS

FILES = augmented_server server vs as Property_Exporter Machine_Properties $(IDL_FILES)

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))


LIBS 	+= -lTAO -lACE -lorbsvcs -lmpeg_server -lmpeg_shared -lrpcsvc

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT) -I$(TAO_ROOT)/orbsvcs/orbsvcs

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

CPPFLAGS += -DSH_MEM -DNDEBUG 
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)

.PRECIOUS: Video_RepositoryC.cpp 

idl_stubs: $(addsuffix .h, $(IDL_FILES))


ifdef stat
CPPFLAGS += -DSTAT
endif

server:$(addprefix $(VDIR),$(AV_SERVER_OBJS))
	$(LINK.cc) $(LDFAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

augmented_server:$(addprefix $(VDIR),$(AV_AUGSERVER_OBJS))
	$(LINK.cc) $(LDFAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

vs:$(addprefix $(VDIR),$(AV_VS_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

as:$(addprefix $(VDIR),$(AV_AS_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES)) \
	$(addsuffix _T.h, $(IDL_FILES)) $(addsuffix _T.i, $(IDL_FILES)) $(addsuffix _T.cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp server.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
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
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
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
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Message_Queue.i \
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
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
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
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_CODgram.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.i \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Select_Reactor_T.h \
 $(ACE_ROOT)/ace/Select_Reactor_Base.h \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Select_Reactor_Base.i \
 $(ACE_ROOT)/ace/Select_Reactor_T.cpp \
 $(ACE_ROOT)/ace/Timer_Heap.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.cpp \
 $(ACE_ROOT)/ace/Select_Reactor_T.i \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/server_proto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Globals.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/fileio.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/com.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/sendpt.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Server.h \
 $(TAO_ROOT)/tao/TAO.h \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Control_State.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Control_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/AVStreams_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Property/CosPropertyService_i.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Audio_Server.h \
 vs.h
.obj/vs.o .obj/vs.so .shobj/vs.o .shobj/vs.so: vs.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Server.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
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
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(TAO_ROOT)/tao/TAO.h \
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
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/fileio.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/com.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/sendpt.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/server_proto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Globals.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Control_State.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Control_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/AVStreams_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Property/CosPropertyService_i.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i \
 vs.h $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Select_Reactor_T.h \
 $(ACE_ROOT)/ace/Select_Reactor_Base.h \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Select_Reactor_Base.i \
 $(ACE_ROOT)/ace/Select_Reactor_T.cpp \
 $(ACE_ROOT)/ace/Timer_Heap.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.cpp \
 $(ACE_ROOT)/ace/Select_Reactor_T.i \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/as.o .obj/as.so .shobj/as.o .shobj/as.so: as.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Audio_Server.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/fileio.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/com.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/server_proto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Globals.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_CODgram.i \
 $(ACE_ROOT)/ace/Singleton.h \
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
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/sendpt.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Server.h \
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
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(TAO_ROOT)/tao/TAO.h \
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
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlS.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Control_State.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Video_Control_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/AVStreams_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Property/CosPropertyService_i.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlS.i \
 as.h $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Select_Reactor_T.h \
 $(ACE_ROOT)/ace/Select_Reactor_Base.h \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Select_Reactor_Base.i \
 $(ACE_ROOT)/ace/Select_Reactor_T.cpp \
 $(ACE_ROOT)/ace/Timer_Heap.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.h \
 $(ACE_ROOT)/ace/Timer_Heap_T.cpp \
 $(ACE_ROOT)/ace/Select_Reactor_T.i \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_server/Audio_Control_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/Property_Exporter.o .obj/Property_Exporter.so .shobj/Property_Exporter.o .shobj/Property_Exporter.so: Property_Exporter.cpp Property_Exporter.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i
.obj/Machine_Properties.o .obj/Machine_Properties.so .shobj/Machine_Properties.o .shobj/Machine_Properties.so: Machine_Properties.cpp Machine_Properties.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Trader/Trader.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
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
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
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
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTradingS.i \
 Property_Exporter.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

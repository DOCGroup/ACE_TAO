# $Id$

BIN = ECM_Supplier ECM_Consumer

LDLIBS= -lorbsvcs -lTAO

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

IDL_HDR	= dataC dataS
IDL_SRC	= dataC dataS

ECM_Supplier_OBJS=$(addsuffix .o,$(IDL_SRC) ECM_Supplier ECM_Data)
ECM_Consumer_OBJS=$(addsuffix .o,$(IDL_SRC) ECM_Consumer ECM_Data)

SRC =   $(addsuffix .cpp, $(BIN)) \
	$(addsuffix .cpp, $(IDL_SRC)) \
	ECM_Data.cpp

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

#### Local rules and variables...

.PRECIOUS: $(addsuffix .h, $(IDL_SRC)) $(addsuffix .i, $(IDL_SRC)) $(addsuffix .cpp, $(IDL_SRC))

ECM_Supplier: $(addprefix $(VDIR),$(ECM_Supplier_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

ECM_Consumer: $(addprefix $(VDIR),$(ECM_Consumer_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean:
	$(RM) -f dataC.* dataS.* dataS_T.*
	$(RM) -f $(BIN)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ECM_Supplier.o .obj/ECM_Supplier.so .shobj/ECM_Supplier.o .shobj/ECM_Supplier.so: ECM_Supplier.cpp \
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
 $(ACE_ROOT)/ace/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Auto_Ptr.i \
 $(ACE_ROOT)/ace/Auto_Ptr.cpp \
 $(ACE_ROOT)/ace/Sched_Params.h \
 $(ACE_ROOT)/ace/Sched_Params.i \
 $(TAO_ROOT)/tao/Timeprobe.h \
 $(ACE_ROOT)/ace/Timeprobe.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/ReactorTask.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Fast_Reactor.h \
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
 $(ACE_ROOT)/ace/Timer_List.h \
 $(ACE_ROOT)/ace/Timer_List_T.h \
 $(ACE_ROOT)/ace/Timer_List_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.i \
 ECM_Supplier.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.cpp \
 ECM_Data.h dataC.h dataC.i
.obj/ECM_Consumer.o .obj/ECM_Consumer.so .shobj/ECM_Consumer.o .shobj/ECM_Consumer.so: ECM_Consumer.cpp \
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
 $(ACE_ROOT)/ace/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Auto_Ptr.i \
 $(ACE_ROOT)/ace/Auto_Ptr.cpp \
 $(ACE_ROOT)/ace/Sched_Params.h \
 $(ACE_ROOT)/ace/Sched_Params.i \
 $(TAO_ROOT)/tao/Timeprobe.h \
 $(ACE_ROOT)/ace/Timeprobe.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/RT_Task.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Timer_Module.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/ReactorTask.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Fast_Reactor.h \
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
 $(ACE_ROOT)/ace/Timer_List.h \
 $(ACE_ROOT)/ace/Timer_List_T.h \
 $(ACE_ROOT)/ace/Timer_List_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Manip.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.i \
 ECM_Consumer.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.cpp \
 ECM_Data.h dataC.h dataC.i
.obj/dataC.o .obj/dataC.so .shobj/dataC.o .shobj/dataC.so: dataC.cpp dataC.h dataC.i dataS.h dataS_T.h dataS_T.i \
 dataS_T.cpp dataS.i
.obj/dataS.o .obj/dataS.so .shobj/dataS.o .shobj/dataS.so: dataS.cpp dataS.h dataC.h dataC.i dataS_T.h dataS_T.i \
 dataS_T.cpp dataS.i
.obj/ECM_Data.o .obj/ECM_Data.so .shobj/ECM_Data.o .shobj/ECM_Data.so: ECM_Data.cpp ECM_Data.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
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
 dataC.h dataC.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

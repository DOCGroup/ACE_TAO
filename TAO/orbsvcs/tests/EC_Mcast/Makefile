# $Id$

BIN = EC_Mcast

SRC = $(BIN:%=%$(VAR).cpp)
LDLIBS= -lorbsvcs -lTAO

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

#### Local rules and variables...

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/EC_Mcast.o .obj/EC_Mcast.so .shobj/EC_Mcast.o .shobj/EC_Mcast.so: EC_Mcast.cpp \
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
 $(ACE_ROOT)/ace/Read_Buffer.h \
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
 $(TAO_ROOT)/orbsvcs/orbsvcs/Runtime_Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Runtime_Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Event_Channel.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Factory.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Event_Channel.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Basic_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Basic_Factory.i \
 EC_Mcast.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Channel_Clients_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Gateway_UDP.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.h \
 $(ACE_ROOT)/ace/SOCK_CODgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecUDPAdminS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_Gateway_UDP.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/EC_UDP_Admin.h \
 EC_Mcast.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

#
# $Id$
#

BIN = Event_Service


LSRC = 	\
	Event_Service.cpp \

ES_OBJS=$(LSRC:.cpp=.o)

LDLIBS = -lorbsvcs -lTAO

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

Event_Service: $(addprefix $(VDIR),$(ES_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Event_Service.o .obj/Event_Service.so .shobj/Event_Service.o .shobj/Event_Service.so: Event_Service.cpp Event_Service.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecSchedulerS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventChannelAdminS.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Auto_Ptr.i \
 $(ACE_ROOT)/ace/Auto_Ptr.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Service_Constants.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Time_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event_Utilities.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Scheduler_Generic.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Sched/Config_Scheduler.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Module_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.h \
 $(TAO_ROOT)/tao/Timeprobe.h \
 $(ACE_ROOT)/ace/Timeprobe.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Scheduler_Factory.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Local_ESTypes.h \
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
 $(TAO_ROOT)/orbsvcs/orbsvcs/Event/Event_Channel.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

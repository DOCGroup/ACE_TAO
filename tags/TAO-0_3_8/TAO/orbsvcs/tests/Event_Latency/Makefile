# $Id$

BIN = Event_Latency


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

#### Local rules and variables...

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif
TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

# Leave the scheduler output out if this is a config run.
ifeq ($(runtime),1)
EVENT_LATENCY_CONFIG_OBJS=Event_Latency_Scheduler_Runtime.o
endif # runtime

Event_Latency: $(addprefix $(VDIR),$(EVENT_LATENCY_OBJS) $(EVENT_LATENCY_CONFIG_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Event_Latency.o .obj/Event_Latency.so .shobj/Event_Latency.o .shobj/Event_Latency.so: Event_Latency.cpp \
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
 $(ACE_ROOT)/ace/Sched_Params.h \
 $(ACE_ROOT)/ace/Sched_Params.i \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
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
 Event_Latency.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosTimeBaseS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/RtecEventCommS.i \
 $(TAO_ROOT)/tao/Timeprobe.h \
 $(ACE_ROOT)/ace/Timeprobe.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

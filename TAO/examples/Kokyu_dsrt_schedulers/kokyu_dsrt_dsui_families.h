/* $Id$*/
// Automatically generated header file "kokyu_dsrt_dsui_families.h"  by dsui-parse.py 
#ifndef _kokyu_dsrt_DSUI_FAMILIES_H
#define _kokyu_dsrt_DSUI_FAMILIES_H

#define kokyu_dsrt_NUM_FAMILIES 1

/* DSUI Family: EDF_SCHED
 * "Events associated with the EDF Scheduler"
 */
#define EDF_SCHED_FAM 11
#define NUM_EVENTS_EDF_SCHED 29
#define NUM_COUNTERS_EDF_SCHED 0
#define NUM_HISTOGRAMS_EDF_SCHED 0

#define CONSTRUCTOR 0
#define DISPATCHER_SHUTDOWN 1
#define CREATE_SCHED_PARAM_START 2
#define CREATE_SCHED_PARAM_END 3
#define START_NEW_SCHED_SEGMENT 4
#define END_NEW_SCHED_SEGMENT 5
#define START_NESTED_SCHED_SEGMENT 6
#define END_NESTED_SCHED_SEGMENT 7
#define START_UPDATE_SCHED_SEGMENT 8
#define END_UPDATE_SCHED_SEGMENT 9
#define START_END_SCHED_SEGMENT 10
#define FINISH_END_SCHED_SEGMENT 11
#define START_CLIENT_SCHED_TIME 12
#define START_KOKYU_DISPATCH_UPDATE_SCHEDULE 13
#define END_KOKYU_DISPATCH_UPDATE_SCHEDULE 14
#define END_CLIENT_SCHED_TIME 15
#define ENTER_SERVER_SCHED_TIME 16
#define LEAVE_SERVER_SCHED_TIME 17
#define ENTER_SERVER_DISPATCH_SCHED 18
#define LEAVE_SERVER_DISPATCH_SCHED 19
#define INSIDE_SEND_POLL 20
#define ENTER_SEND_REPLY 21
#define EXIT_SEND_REPLY 22
#define SEND_EXCEPTION 23
#define SEND_OTHER 24
#define ENTER_RECEIVE_REPLY 25
#define EXIT_RECEIVE_REPLY 26
#define RECEIVE_EXCEPTION 27
#define RECEIVE_OTHER 28




#ifdef CONFIG_DSTREAM_EDF_SCHED
#define DSUI_EVENT_LOG_EDF_SCHED_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(EDF_SCHED_FAM, (event) ) ) \
            dsui_event_log( EDF_SCHED_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_EDF_SCHED_FAM( event, tag , len, data )
#endif
#endif /* _DSUI_FAMILIES_H */

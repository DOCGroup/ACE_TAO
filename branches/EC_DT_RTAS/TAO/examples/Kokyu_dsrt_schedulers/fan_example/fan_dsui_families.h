// $Id$
// Automatically generated header file "fan_dsui_families.h"  by dsui-parse.py 
#ifndef _fan_DSUI_FAMILIES_H
#define _fan_DSUI_FAMILIES_H

#define fan_NUM_FAMILIES 3

/* DSUI Family: TEST_ONE
 * "Description"
 */
#define TEST_ONE_FAM 1
#define NUM_EVENTS_TEST_ONE 3
#define NUM_COUNTERS_TEST_ONE 0
#define NUM_HISTOGRAMS_TEST_ONE 0

#define START_SERVICE 0
#define DEADLINE_MISSED 1
#define STOP_SERVICE 2



/* DSUI Family: WORKER_GROUP
 * "Description"
 */
#define WORKER_GROUP_FAM 3
#define NUM_EVENTS_WORKER_GROUP 9
#define NUM_COUNTERS_WORKER_GROUP 0
#define NUM_HISTOGRAMS_WORKER_GROUP 0

#define ONE_WAY_CALL_START 0
#define ONE_WAY_CALL_DONE 1
#define BEGIN_SCHED_SEGMENT 2
#define END_SCHED_SEGMENT 3
#define WORKER_STARTED 4
#define UPDATE_SCHED_SEGMENT_BEGIN 5
#define UPDATE_SCHED_SEGMENT_END 6
#define RUNNING_SUBTASK 7
#define FINISHING_SUBTASK 8



/* DSUI Family: MAIN_GROUP
 * "Description"
 */
#define MAIN_GROUP_FAM 2
#define NUM_EVENTS_MAIN_GROUP 9
#define NUM_COUNTERS_MAIN_GROUP 0
#define NUM_HISTOGRAMS_MAIN_GROUP 0

#define START 0
#define SCHEDULE_SETUP 1
#define SCHEDULER_STARTED 2
#define CALL_SERVER_SHUTDOWN 3
#define SCHEDULER_SHUTDOWN 4
#define AFTER_SERVER_SHUTDOWN 5
#define STOP 6
#define WORKER_ACTIVATED 7
#define WORKER_WAIT_DONE 8




#ifdef CONFIG_DSTREAM_TEST_ONE
#define DSTRM_EVENT_TEST_ONE_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(TEST_ONE_FAM, (event) ) ) \
            dsui_event_log( TEST_ONE_FAM, event, tag, len, data ); \
}
#else
#define DSTRM_EVENT_TEST_ONE_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_WORKER_GROUP
#define DSTRM_EVENT_WORKER_GROUP_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(WORKER_GROUP_FAM, (event) ) ) \
            dsui_event_log( WORKER_GROUP_FAM, event, tag, len, data ); \
}
#else
#define DSTRM_EVENT_WORKER_GROUP_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_MAIN_GROUP
#define DSTRM_EVENT_MAIN_GROUP_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(MAIN_GROUP_FAM, (event) ) ) \
            dsui_event_log( MAIN_GROUP_FAM, event, tag, len, data ); \
}
#else
#define DSTRM_EVENT_MAIN_GROUP_FAM( event, tag , len, data )
#endif
#endif /* _DSUI_FAMILIES_H */

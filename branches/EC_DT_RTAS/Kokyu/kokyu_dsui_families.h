/* $Id$ */
// Automatically generated header file "kokyu_dsui_families.h"  by dsui-parse.py
#ifndef _kokyu_DSUI_FAMILIES_H
#define _kokyu_DSUI_FAMILIES_H

#define NUM_FAMILIES 6

/* DSUI Family: DISP_TASK
 * "Dispatcher Task"
 */
#define DISP_TASK_FAM 8
#define NUM_EVENTS_DISP_TASK 4
#define NUM_COUNTERS_DISP_TASK 0
#define NUM_HISTOGRAMS_DISP_TASK 0

#define EVENT_DEQUEUED 0
#define EVENT_START_DISPATCHING 1
#define EVENT_FINISHED_DISPATCHING 2
#define EVENT_DEFERRED 3



/* DSUI Family: DSRT_CV_DISPATCH
 * "DSRT CV Dispatcher Events"
 */
#define DSRT_CV_DISPATCH_FAM 5
#define NUM_EVENTS_DSRT_CV_DISPATCH 14
#define NUM_COUNTERS_DSRT_CV_DISPATCH 0
#define NUM_HISTOGRAMS_DSRT_CV_DISPATCH 0

#define SCHEDULE_ENTER 0
#define SCHEDULE_EXIT 1
#define RG_EVENT_RELEASED 2
#define RG_DELAYED_EVENT_RELEASED 3
#define NONRG_EVENT_RELEASED 4
#define INIT_I 5
#define RELEASE_GUARD_START 6
#define RELEASE_GUARD_END 7
#define UPDATE_SCHEDULE_START 8
#define UPDATE_SCHEDULE_END 9
#define CANCEL_SCHEDULE_START 10
#define CANCEL_SCHEDULE_END 11
#define SHUTDOWN 12
#define INIT 13



/* DSUI Family: DSRT_DIRECT_DISPATCH
 * "DSRT Direct Dispatcher Events"
 */
#define DSRT_DIRECT_DISPATCH_FAM 6
#define NUM_EVENTS_DSRT_DIRECT_DISPATCH 2
#define NUM_COUNTERS_DSRT_DIRECT_DISPATCH 0
#define NUM_HISTOGRAMS_DSRT_DIRECT_DISPATCH 0

#define SCHEDULE_ENTER 0
#define SCHEDULE_EXIT 1



/* DSUI Family: DISP_DEFERRER
 * "Release Guard Events"
 */
#define DISP_DEFERRER_FAM 7
#define NUM_EVENTS_DISP_DEFERRER 2
#define NUM_COUNTERS_DISP_DEFERRER 0
#define NUM_HISTOGRAMS_DISP_DEFERRER 0

#define EVENT_DEFERRED_ENQUEUE 0
#define EVENT_DEFERRED_DEQUEUE 1



/* DSUI Family: DSRT_DISPATCH_IMPL
 * "DSRT Dispatcher_Impl Events"
 */
#define DSRT_DISPATCH_IMPL_FAM 9
#define NUM_EVENTS_DSRT_DISPATCH_IMPL 1
#define NUM_COUNTERS_DSRT_DISPATCH_IMPL 0
#define NUM_HISTOGRAMS_DSRT_DISPATCH_IMPL 0

#define INIT2 0



/* DSUI Family: DSRT_DISPATCH
 * "DSRT Dispatcher Events"
 */
#define DSRT_DISPATCH_FAM 4
#define NUM_EVENTS_DSRT_DISPATCH 3
#define NUM_COUNTERS_DSRT_DISPATCH 0
#define NUM_HISTOGRAMS_DSRT_DISPATCH 0

#define SCHEDULE 0
#define CREATE_DSRT_DISPATCH_SCHED_START 1
#define CREATE_DSRT_DISPATCH_SCHED_END 2




#ifdef CONFIG_DSTREAM_DISP_TASK
#define DSUI_EVENT_LOG_DISP_TASK_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(DISP_TASK_FAM, (event) ) ) \
            dsui_event_log( DISP_TASK_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_DISP_TASK_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_DSRT_CV_DISPATCH
#define DSUI_EVENT_LOG_DSRT_CV_DISPATCH_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(DSRT_CV_DISPATCH_FAM, (event) ) ) \
            dsui_event_log( DSRT_CV_DISPATCH_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_DSRT_CV_DISPATCH_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_DSRT_DIRECT_DISPATCH
#define DSUI_EVENT_LOG_DSRT_DIRECT_DISPATCH_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(DSRT_DIRECT_DISPATCH_FAM, (event) ) ) \
            dsui_event_log( DSRT_DIRECT_DISPATCH_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_DSRT_DIRECT_DISPATCH_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_DISP_DEFERRER
#define DSUI_EVENT_LOG_DISP_DEFERRER_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(DISP_DEFERRER_FAM, (event) ) ) \
            dsui_event_log( DISP_DEFERRER_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_DISP_DEFERRER_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_DSRT_DISPATCH_IMPL
#define DSUI_EVENT_LOG_DSRT_DISPATCH_IMPL_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(DSRT_DISPATCH_IMPL_FAM, (event) ) ) \
            dsui_event_log( DSRT_DISPATCH_IMPL_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_DSRT_DISPATCH_IMPL_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_DSRT_DISPATCH
#define DSUI_EVENT_LOG_DSRT_DISPATCH_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(DSRT_DISPATCH_FAM, (event) ) ) \
            dsui_event_log( DSRT_DISPATCH_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_DSRT_DISPATCH_FAM( event, tag , len, data )
#endif
#endif /* _DSUI_FAMILIES_H */

// $Id$
// Automatically generated header file "ace_dsui_families.h"  by dsui-parse.py 
#ifndef _ace_DSUI_FAMILIES_H
#define _ace_DSUI_FAMILIES_H

#define ace_NUM_FAMILIES 1

/* DSUI Family: MSG_QUEUE
 * "Message Queue events"
 */
#define MSG_QUEUE_FAM 10
#define NUM_EVENTS_MSG_QUEUE 13
#define NUM_COUNTERS_MSG_QUEUE 0
#define NUM_HISTOGRAMS_MSG_QUEUE 0

#define QUEUE_LEVEL 0
#define BEFORE_GETQ_CALL 1
#define AFTER_GETQ_CALL 2
#define BEFORE_PUTQ_CALL 3
#define AFTER_PUTQ_CALL 4
#define BEFORE_ENQUEUE_TAIL 5
#define AFTER_ENQUEUE_TAIL 6
#define BEFORE_DEQUEUE_HEAD 7
#define AFTER_DEQUEUE_HEAD 8
#define BEFORE_ENQUEUE_TAIL_LOCK_ACQUIRE 9
#define AFTER_ENQUEUE_TAIL_LOCK_ACQUIRE 10
#define BEFORE_DEQUEUE_HEAD_LOCK_ACQUIRE 11
#define AFTER_DEQUEUE_HEAD_LOCK_ACQUIRE 12




#ifdef CONFIG_DSTREAM_MSG_QUEUE
#define DSUI_EVENT_LOG_MSG_QUEUE_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(MSG_QUEUE_FAM, (event) ) ) \
            dsui_event_log( MSG_QUEUE_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_MSG_QUEUE_FAM( event, tag , len, data )
#endif
#endif /* _DSUI_FAMILIES_H */

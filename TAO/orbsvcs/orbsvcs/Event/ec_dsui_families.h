/*$Id$*/

// Automatically generated header file "ec_dsui_families.h"  by dsui-parse.py
#ifndef _ec_DSUI_FAMILIES_H
#define _ec_DSUI_FAMILIES_H

#define NUM_FAMILIES 2

/* DSUI Family: EC2_GROUP
 * "EC DSUI group"
 */
#define EC2_GROUP_FAM 11
#define NUM_EVENTS_EC2_GROUP 6
#define NUM_COUNTERS_EC2_GROUP 0
#define NUM_HISTOGRAMS_EC2_GROUP 0

#define ENTER_PROXY_PUSH_CONSUMER 0
#define ENTER_PROXY_PUSH_SUPPLIER 1
#define DISPATCH_THE_EVENT 2
#define DISPATCH_COMMAND_ENQUEUE_BEGIN 3
#define DISPATCH_COMMAND_ENQUEUE_END 4
#define ENTER_TIMEOUT_GENERATOR 5



/* DSUI Family: TIMEOUT
 * "Timeout handler group"
 */
#define TIMEOUT_FAM 12
#define NUM_EVENTS_TIMEOUT 1
#define NUM_COUNTERS_TIMEOUT 0
#define NUM_HISTOGRAMS_TIMEOUT 0

#define PROGRAM_STOP 0




#ifdef CONFIG_DSTREAM_EC2_GROUP
#define DSTRM_EVENT_EC2_GROUP_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(EC2_GROUP_FAM, (event) ) ) \
            dsui_event_log( EC2_GROUP_FAM, event, tag, len, data ); \
}
#else
#define DSTRM_EVENT_EC2_GROUP_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_TIMEOUT
#define DSTRM_EVENT_TIMEOUT_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(TIMEOUT_FAM, (event) ) ) \
            dsui_event_log( TIMEOUT_FAM, event, tag, len, data ); \
}
#else
#define DSTRM_EVENT_TIMEOUT_FAM( event, tag , len, data )
#endif
#endif /* _DSUI_FAMILIES_H */

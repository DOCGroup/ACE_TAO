// $Id$
// Automatically generated header file "ec_dsui_families.h"  by dsui-parse.py 
#ifndef _ec_DSUI_FAMILIES_H
#define _ec_DSUI_FAMILIES_H

#define ec_NUM_FAMILIES 2

/* DSUI Family: EC2_GROUP
 * "EC DSUI group"
 */
#define EC2_GROUP_FAM 1
#define NUM_EVENTS_EC2_GROUP 6
#define NUM_COUNTERS_EC2_GROUP 0
#define NUM_HISTOGRAMS_EC2_GROUP 1

#define ENTER_PROXY_PUSH_CONSUMER 0
#define ENTER_PROXY_PUSH_SUPPLIER 1
#define DISPATCH_THE_EVENT 2
#define DISPATCH_COMMAND_ENQUEUE_BEGIN 3
#define DISPATCH_COMMAND_ENQUEUE_END 4
#define ENTER_TIMEOUT_GENERATOR 5


#define HISTOGRAM_TIME_SPENT_IN_EC 0

/* DSUI Family: TIMEOUT
 * "Timeout handler group"
 */
#define TIMEOUT_FAM 2
#define NUM_EVENTS_TIMEOUT 2
#define NUM_COUNTERS_TIMEOUT 0
#define NUM_HISTOGRAMS_TIMEOUT 0

#define TIMEOUT_EVENT 1




#ifdef CONFIG_DSTREAM_EC2_GROUP
#define DSUI_EVENT_LOG_EC2_GROUP_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(EC2_GROUP_FAM, (event) ) ) \
            dsui_event_log( EC2_GROUP_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_EC2_GROUP_FAM( event, tag , len, data )
#endif

#ifdef CONFIG_DSTREAM_EC2_GROUP
#define DSUI_ADD_TO_HISTOGRAM_EC2_GROUP_FAM( histogram , amount ) \
        if(dsui_histogram_enabled(EC2_GROUP_FAM, (histogram))) \
                dsui_histogram_add( EC2_GROUP_FAM, (histogram), (amount), ec_family_table, ec_table_offset, dsui_family_table )
#define DSUI_LOG_HISTOGRAM_EC2_GROUP_FAM( histogram ) \
        if(dsui_histogram_enabled(EC2_GROUP_FAM, (histogram))) \
                dsui_histogram_log( EC2_GROUP_FAM, (histogram), ec_family_table, ec_table_offset )
#define DSUI_RESET_HISTOGRAM_EC2_GROUP_FAM( histogram ) \
        if(dsui_histogram_enabled(EC2_GROUP_FAM, (histogram))) \
               dsui_histogram_reset( EC2_GROUP_FAM, (histogram), ec_family_table, ec_table_offset )
#else
#define DSUI_ADD_TO_HISTOGRAM_EC2_GROUP_FAM( histogram , amount )
#define DSUI_LOG_HISTOGRAM_EC2_GROUP_FAM( histogram )
#define DSUI_RESET_HISTOGRAM_EC2_GROUP_FAM( histogram )
#define DSUI_INIT_HISTOGRAM_EC2_GROUP_FAM( histogram )
#endif


#ifdef CONFIG_DSTREAM_TIMEOUT
#define DSUI_EVENT_LOG_TIMEOUT_FAM( event,tag, len, data ) { \
    if( dsui_event_enabled(TIMEOUT_FAM, (event) ) ) \
            dsui_event_log( TIMEOUT_FAM, event, tag, len, data ); \
}
#else
#define DSUI_EVENT_LOG_TIMEOUT_FAM( event, tag , len, data )
#endif
#endif /* _DSUI_FAMILIES_H */

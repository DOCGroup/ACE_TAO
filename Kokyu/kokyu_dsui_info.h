//$Id$
#ifndef _kokyu_DSUI_INFO_H
#define _kokyu_DSUI_INFO_H


#include "kokyu_dsui_families.h"
#include "datastream_print_events.h"
static struct counter_info DSRT_DISPATCH_COUNTERS_INFO[NUM_COUNTERS_DSRT_DISPATCH] = {
};

static struct event_info DSRT_DISPATCH_EVENTS_INFO[NUM_EVENTS_DSRT_DISPATCH] = {
{ "SCHEDULE", "Schedule task", print_string },
{ "CREATE_DSRT_DISPATCH_SCHED_START", "Create appropriate Scheduler", print_string },
{ "CREATE_DSRT_DISPATCH_SCHED_END", "Done creating appropriate Scheduler", print_string }
};

static struct hist_info DSRT_DISPATCH_HISTOGRAMS_INFO[NUM_HISTOGRAMS_DSRT_DISPATCH] = {
};

static struct family_info DSRT_DISPATCH_INFO = {
	"DSRT_DISPATCH",
	NUM_EVENTS_DSRT_DISPATCH,
	NUM_COUNTERS_DSRT_DISPATCH,
	0,
	NUM_HISTOGRAMS_DSRT_DISPATCH,
	DSRT_DISPATCH_EVENTS_INFO,
	DSRT_DISPATCH_COUNTERS_INFO,
	NULL,
	DSRT_DISPATCH_HISTOGRAMS_INFO};

static struct counter_info DSRT_CV_DISPATCH_COUNTERS_INFO[NUM_COUNTERS_DSRT_CV_DISPATCH] = {
};

static struct event_info DSRT_CV_DISPATCH_EVENTS_INFO[NUM_EVENTS_DSRT_CV_DISPATCH] = {
{ "SCHEDULE_ENTER", "Enter Schedule_i", print_string },
{ "SCHEDULE_EXIT", "Exit Schedule_i", print_string },
{ "RG_EVENT_RELEASED", "Release guard Event release time on the server side", print_string },
{ "RG_EVENT_DELAYED_RELEASED", "Event delayed by release guard", print_string },
{ "NONRG_EVENT_RELEASED", "Event release time on the server side", print_string },
{ "INIT_I", "Initialization", print_string },
{ "RELEASE_GUARD_START", "Enter release_guard_i function", print_string },
{ "RELEASE_GUARD_END", "Exit release_guard_i function", print_string },
{ "UPDATE_SCHEDULE_START", "Enter update_schedule_i function", print_string },
{ "UPDATE_SCHEDULE_END", "Exit update_schedule_i function", print_string },
{ "CANCEL_SCHEDULE_START", "Enter cancel_schedule_i function", print_string },
{ "CANCEL_SCHEDULE_END", "Exit cancel_schedule_i function", print_string },
{ "SHUTDOWN", "Enter shutdown_i function", print_string },
{ "INIT", "Enter DSRT_Dispatcher_Impl init", print_string },
{ "RG_EVENT_DELAYED_RELEASED_FIRE", "Event delayed by release guard release time on the server side", print_string }
};

static struct hist_info DSRT_CV_DISPATCH_HISTOGRAMS_INFO[NUM_HISTOGRAMS_DSRT_CV_DISPATCH] = {
};

static struct family_info DSRT_CV_DISPATCH_INFO = {
	"DSRT_CV_DISPATCH",
	NUM_EVENTS_DSRT_CV_DISPATCH,
	NUM_COUNTERS_DSRT_CV_DISPATCH,
	0,
	NUM_HISTOGRAMS_DSRT_CV_DISPATCH,
	DSRT_CV_DISPATCH_EVENTS_INFO,
	DSRT_CV_DISPATCH_COUNTERS_INFO,
	NULL,
	DSRT_CV_DISPATCH_HISTOGRAMS_INFO};

static struct counter_info DSRT_DIRECT_DISPATCH_COUNTERS_INFO[NUM_COUNTERS_DSRT_DIRECT_DISPATCH] = {
};

static struct event_info DSRT_DIRECT_DISPATCH_EVENTS_INFO[NUM_EVENTS_DSRT_DIRECT_DISPATCH] = {
{ "SCHEDULE_ENTER", "Enter Schedule_i", print_string },
{ "SCHEDULE_EXIT", "Exit Schedule_i", print_string }
};

static struct hist_info DSRT_DIRECT_DISPATCH_HISTOGRAMS_INFO[NUM_HISTOGRAMS_DSRT_DIRECT_DISPATCH] = {
};

static struct family_info DSRT_DIRECT_DISPATCH_INFO = {
	"DSRT_DIRECT_DISPATCH",
	NUM_EVENTS_DSRT_DIRECT_DISPATCH,
	NUM_COUNTERS_DSRT_DIRECT_DISPATCH,
	0,
	NUM_HISTOGRAMS_DSRT_DIRECT_DISPATCH,
	DSRT_DIRECT_DISPATCH_EVENTS_INFO,
	DSRT_DIRECT_DISPATCH_COUNTERS_INFO,
	NULL,
	DSRT_DIRECT_DISPATCH_HISTOGRAMS_INFO};

static struct counter_info DISP_DEFERRER_COUNTERS_INFO[NUM_COUNTERS_DISP_DEFERRER] = {
};

static struct event_info DISP_DEFERRER_EVENTS_INFO[NUM_EVENTS_DISP_DEFERRER] = {
{ "EVENT_DEFERRED_ENQUEUE", "Buffer event until timer expires", print_string },
{ "EVENT_DEFERRED_DEQUEUE", "Enqueue event in dispatch queue", print_string }
};

static struct hist_info DISP_DEFERRER_HISTOGRAMS_INFO[NUM_HISTOGRAMS_DISP_DEFERRER] = {
};

static struct family_info DISP_DEFERRER_INFO = {
	"DISP_DEFERRER",
	NUM_EVENTS_DISP_DEFERRER,
	NUM_COUNTERS_DISP_DEFERRER,
	0,
	NUM_HISTOGRAMS_DISP_DEFERRER,
	DISP_DEFERRER_EVENTS_INFO,
	DISP_DEFERRER_COUNTERS_INFO,
	NULL,
	DISP_DEFERRER_HISTOGRAMS_INFO};

static struct counter_info DISP_TASK_COUNTERS_INFO[NUM_COUNTERS_DISP_TASK] = {
};

static struct event_info DISP_TASK_EVENTS_INFO[NUM_EVENTS_DISP_TASK] = {
{ "EVENT_DEQUEUED", "Time of event duqueue", print_string },
{ "EVENT_START_DISPATCHING", "Start time of actually dispatching event", print_string },
{ "EVENT_FINISHED_DISPATCHING", "End time of dispatching event", print_string },
{ "EVENT_DEFERRED", "Time event is deferred", print_string }
};

static struct hist_info DISP_TASK_HISTOGRAMS_INFO[NUM_HISTOGRAMS_DISP_TASK] = {
};

static struct family_info DISP_TASK_INFO = {
	"DISP_TASK",
	NUM_EVENTS_DISP_TASK,
	NUM_COUNTERS_DISP_TASK,
	0,
	NUM_HISTOGRAMS_DISP_TASK,
	DISP_TASK_EVENTS_INFO,
	DISP_TASK_COUNTERS_INFO,
	NULL,
	DISP_TASK_HISTOGRAMS_INFO};

static struct counter_info DSRT_DISPATCH_IMPL_COUNTERS_INFO[NUM_COUNTERS_DSRT_DISPATCH_IMPL] = {
};

static struct event_info DSRT_DISPATCH_IMPL_EVENTS_INFO[NUM_EVENTS_DSRT_DISPATCH_IMPL] = {
{ "INIT2", "Initialize Dispatcher object", print_string }
};

static struct hist_info DSRT_DISPATCH_IMPL_HISTOGRAMS_INFO[NUM_HISTOGRAMS_DSRT_DISPATCH_IMPL] = {
};

static struct family_info DSRT_DISPATCH_IMPL_INFO = {
	"DSRT_DISPATCH_IMPL",
	NUM_EVENTS_DSRT_DISPATCH_IMPL,
	NUM_COUNTERS_DSRT_DISPATCH_IMPL,
	0,
	NUM_HISTOGRAMS_DSRT_DISPATCH_IMPL,
	DSRT_DISPATCH_IMPL_EVENTS_INFO,
	DSRT_DISPATCH_IMPL_COUNTERS_INFO,
	NULL,
	DSRT_DISPATCH_IMPL_HISTOGRAMS_INFO};

struct family_info *dstream_family_table[NUM_FAMILIES] = {
	&DSRT_DISPATCH_INFO,
	&DSRT_CV_DISPATCH_INFO,
	&DSRT_DIRECT_DISPATCH_INFO,
	&DISP_DEFERRER_INFO,
	&DISP_TASK_INFO,
	&DSRT_DISPATCH_IMPL_INFO
};

#endif

//$Id$
#ifndef _dt_oneway_DSUI_INFO_H
#define _dt_oneway_DSUI_INFO_H


#include "dt_oneway_dsui_families.h"
#include "datastream_print_events.h"
static struct counter_info MAIN_GROUP_COUNTERS_INFO[NUM_COUNTERS_MAIN_GROUP] = {
};

static struct event_info MAIN_GROUP_EVENTS_INFO[NUM_EVENTS_MAIN_GROUP] = {
{ "START", "Start program", print_string },
{ "STOP", "Stop program", print_string },
{ "SCHEDULE_SETUP", "Scheduler setup", print_string },
{ "WORKER_ACTIVATED", "Worker activated", print_string },
{ "WORKER_WAIT_DONE", "Worker thread wait() call complete", print_string },
{ "CALL_SERVER_SHUTDOWN", "Called server to shutdown", print_string },
{ "AFTER_SERVER_SHUTDOWN", "Server shutdown complete", print_string },
{ "SCHEDULER_SHUTDOWN", "Scheduler stopped", print_string },
{ "SCHEDULER_STARTED", "Scheduler started", print_string }
};

static struct hist_info MAIN_GROUP_HISTOGRAMS_INFO[NUM_HISTOGRAMS_MAIN_GROUP] = {
};

static struct family_info MAIN_GROUP_INFO = {
	"MAIN_GROUP",
	NUM_EVENTS_MAIN_GROUP,
	NUM_COUNTERS_MAIN_GROUP,
	0,
	NUM_HISTOGRAMS_MAIN_GROUP,
	MAIN_GROUP_EVENTS_INFO,
	MAIN_GROUP_COUNTERS_INFO,
	NULL,
	MAIN_GROUP_HISTOGRAMS_INFO};

static struct counter_info WORKER_GROUP_COUNTERS_INFO[NUM_COUNTERS_WORKER_GROUP] = {
};

static struct event_info WORKER_GROUP_EVENTS_INFO[NUM_EVENTS_WORKER_GROUP] = {
{ "WORKER_STARTED", "Start worker thread", print_string },
{ "BEGIN_SCHED_SEGMENT", "Start scheduling segment", print_string },
{ "END_SCHED_SEGMENT", "End scheduling segment", print_string },
{ "ONE_WAY_CALL_START", "One way call started", print_string },
{ "ONE_WAY_CALL_DONE", "One way call done", print_string },
{ "UPDATE_SCHED_SEGMENT_BEGIN", "Update scheduling segment begin", print_string },
{ "UPDATE_SCHED_SEGMENT_END", "Update scheduling segment end", print_string },
{ "RUNNING_SUBTASK", "Begin to run local task on the host", print_string },
{ "FINISHING_SUBTASK", "Finish the local task", print_string } 
};

static struct hist_info WORKER_GROUP_HISTOGRAMS_INFO[NUM_HISTOGRAMS_WORKER_GROUP] = {
};

static struct family_info WORKER_GROUP_INFO = {
	"WORKER_GROUP",
	NUM_EVENTS_WORKER_GROUP,
	NUM_COUNTERS_WORKER_GROUP,
	0,
	NUM_HISTOGRAMS_WORKER_GROUP,
	WORKER_GROUP_EVENTS_INFO,
	WORKER_GROUP_COUNTERS_INFO,
	NULL,
	WORKER_GROUP_HISTOGRAMS_INFO};

static struct counter_info TEST_ONE_COUNTERS_INFO[NUM_COUNTERS_TEST_ONE] = {
};

static struct event_info TEST_ONE_EVENTS_INFO[NUM_EVENTS_TEST_ONE] = {
{ "START_SERVICE", "Start service time", print_string },
{ "STOP_SERVICE", "Stop service time", print_string }
};

static struct hist_info TEST_ONE_HISTOGRAMS_INFO[NUM_HISTOGRAMS_TEST_ONE] = {
};

static struct family_info TEST_ONE_INFO = {
	"TEST_ONE",
	NUM_EVENTS_TEST_ONE,
	NUM_COUNTERS_TEST_ONE,
	0,
	NUM_HISTOGRAMS_TEST_ONE,
	TEST_ONE_EVENTS_INFO,
	TEST_ONE_COUNTERS_INFO,
	NULL,
	TEST_ONE_HISTOGRAMS_INFO};

struct family_info *dstream_family_table[dt_oneway_NUM_FAMILIES] = {
	&MAIN_GROUP_INFO,
	&WORKER_GROUP_INFO,
	&TEST_ONE_INFO
};

#endif

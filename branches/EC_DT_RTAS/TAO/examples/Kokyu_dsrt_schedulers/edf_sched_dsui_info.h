//$Id$
#ifndef _edf_sched_DSUI_INFO_H
#define _edf_sched_DSUI_INFO_H


#include "edf_sched_dsui_families.h"
#include "datastream_print_events.h"
static struct counter_info EDF_SCHED_COUNTERS_INFO[NUM_COUNTERS_EDF_SCHED] = {
};

static struct event_info EDF_SCHED_EVENTS_INFO[NUM_EVENTS_EDF_SCHED] = {
{ "KOKYU_SCHED_START", "Start Kokyu Schedule call", print_string },
{ "BEGIN_NEW_SCHED_SEGMENT_END", "End new Schedule segment", print_string },
{ "BEGIN_NEW_SCHED_SEGMENT_START", "Start new Schedule segment", print_string },
{ "CREATE_SCHED_PARAM", "Create Scheduling parameter", print_string },
{ "DISPATCHER_SHUTDOWN", "Kokyu DSRT Dispatcher shutdown", print_string },
{ "ENTER_CLIENT_SCHED_TIME", "Point which records the entering scheduler time on the client side", print_string },
{ "CALL_KOKYU_DISPATCH_UPDATE_SCHEDULE", "Entering dispatcher time on the client side", print_string },
{ "LEAVE_KOKYU_DISPATCH_UPDATE_SCHEDULE", "Leaving dispatcher time on the client side", print_string },
{ "LEAVE_CLIENT_SCHED_TIME", "Record the leaving scheduler time on the client side", print_string },
{ "ENTER_SERVER_SCHED_TIME", "Record the entering scheduler time on the server side", print_string },
{ "LEAVE_SERVER_SCHED_TIME", "Record the exiting scheduler time on the server side", print_string },
{ "ENTER_SERVER_DISPATCH_SCHEDULE", "Record the entering dispatcher time on the server side", print_string },
{ "LEAVE_SERVER_DISPATCH_SCHEDULE", "Record the exiting dispatcher time on the server side", print_string },
{ "BEGIN_NESTED_SCHED_SEGMENT", "Start next Schedule segment", print_string },
{ "BEGIN_UPDATE_SCHED_SEGMENT", "Start update Schedule segment", print_string },
{ "END_UPDATE_SCHED_SEGMENT", "End update Schedule segment", print_string },
{ "END_SCHED_SEGMENT", "Enter end_scheduling_segment", print_string },
{ "END_NESTED_SCHED_SEGMENT", "End nested Schedule segment", print_string },
{ "SEND_POLL", "Enter send_poll", print_string },
{ "ENTER_SEND_REPLY", "Enter send_reply", print_string },
{ "EXIT_SEND_REPLY", "Exit send_reply", print_string },
{ "SEND_EXCEPTION", "Enter send_exception", print_string },
{ "SEND_OTHER", "Enter send_other", print_string },
{ "ENTER_RECEIVE_REPLY", "Enter receive_reply", print_string },
{ "EXIT_RECEIVE_REPLY", "Exit receive_reply", print_string },
{ "RECEIVE_EXCEPTION", "Enter receive_exception", print_string },
{ "RECEIVE_OTHER", "Enter receive_other", print_string },
{ "CONSTRUCTOR", "Dispatcher created in constructor", print_string },
{ "CREATE_SCHED_PARAM_END", "Create scheduling segment end", print_string },
{ "BEGIN_NESTED_SCHED_SEGMENT_END", "Begin nested scheduling segment end", print_string },
{ "END_SCHED_SEGMENT_END", "End scheduling segment end", print_string }
};

static struct hist_info EDF_SCHED_HISTOGRAMS_INFO[NUM_HISTOGRAMS_EDF_SCHED] = {
};

static struct family_info EDF_SCHED_INFO = {
	"EDF_SCHED",
	NUM_EVENTS_EDF_SCHED,
	NUM_COUNTERS_EDF_SCHED,
	0,
	NUM_HISTOGRAMS_EDF_SCHED,
	EDF_SCHED_EVENTS_INFO,
	EDF_SCHED_COUNTERS_INFO,
	NULL,
	EDF_SCHED_HISTOGRAMS_INFO};

struct family_info *dstream_family_table[edf_sched_NUM_FAMILIES] = {
	&EDF_SCHED_INFO
};

#endif

//$Id$
#ifndef _edf_sched_DSUI_TABLE_H
#define _edf_sched_DSUI_TABLE_H


#include "edf_sched_dsui_families.h"
counter_t EDF_SCHED_COUNTERS[NUM_COUNTERS_EDF_SCHED] = {
};

histogram_t EDF_SCHED_HISTOGRAMS[NUM_HISTOGRAMS_EDF_SCHED] = {
};

family_t EDF_SCHED = {
	"EDF_SCHED",
	NUM_COUNTERS_EDF_SCHED,
	NUM_EVENTS_EDF_SCHED,
	NUM_HISTOGRAMS_EDF_SCHED,
	EDF_SCHED_COUNTERS,
	EDF_SCHED_HISTOGRAMS,
	0,
	0,
	0,
	0,
};

int edf_sched_table_offset = 4;
family_t *edf_sched_family_table[edf_sched_NUM_FAMILIES] = {
	&EDF_SCHED
};

#define DSUI_INIT( identifier, enabled ) dsui_init( (identifier), (enabled) )

#endif

/* $Id$*/
#include <datastream_common.h>
#include <dsui_types.h>
#include "kokyu_dsrt_dsui_families.h"
counter_t EDF_SCHED_COUNTERS[NUM_COUNTERS_EDF_SCHED] = {
};
histogram_t EDF_SCHED_HISTOGRAMS[NUM_HISTOGRAMS_EDF_SCHED] = {
};
family_t EDF_SCHED = {
	"EDF_SCHED",
	NUM_EVENTS_EDF_SCHED,
	NUM_COUNTERS_EDF_SCHED,
	NUM_HISTOGRAMS_EDF_SCHED,
	EDF_SCHED_COUNTERS,
	EDF_SCHED_HISTOGRAMS,
	0, 
	0, 
	0, 
	0
};

int kokyu_dsrt_table_offset= 1;
family_t *kokyu_dsrt_family_table[kokyu_dsrt_NUM_FAMILIES] = {
	
	
	
	
	
	
	
	
	
	
	
	&EDF_SCHED,
};


// $Id$
#include <datastream_common.h>
#include <dsui_types.h>
#include "ace_dsui_families.h"
counter_t MSG_QUEUE_COUNTERS[NUM_COUNTERS_MSG_QUEUE] = {
};
histogram_t MSG_QUEUE_HISTOGRAMS[NUM_HISTOGRAMS_MSG_QUEUE] = {
};
family_t MSG_QUEUE = {
	"MSG_QUEUE",
	NUM_EVENTS_MSG_QUEUE,
	NUM_COUNTERS_MSG_QUEUE,
	NUM_HISTOGRAMS_MSG_QUEUE,
	MSG_QUEUE_COUNTERS,
	MSG_QUEUE_HISTOGRAMS,
	0,
	0,
	0,
	0
};

int ace_table_offset= 1;
family_t *ace_family_table[ace_NUM_FAMILIES] = {










	&MSG_QUEUE,
};


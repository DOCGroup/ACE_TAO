/*$Id$*/

#include <datastream_common.h>
#include <dsui_types.h>
#include "ec_dsui_families.h"
counter_t EC2_GROUP_COUNTERS[NUM_COUNTERS_EC2_GROUP] = {
};
histogram_t EC2_GROUP_HISTOGRAMS[NUM_HISTOGRAMS_EC2_GROUP] = {
};
family_t EC2_GROUP = {
        "EC2_GROUP",
        NUM_EVENTS_EC2_GROUP,
        NUM_COUNTERS_EC2_GROUP,
        NUM_HISTOGRAMS_EC2_GROUP,
        EC2_GROUP_COUNTERS,
        EC2_GROUP_HISTOGRAMS,
        0,
        0,
        0,
        0
};

counter_t TIMEOUT_COUNTERS[NUM_COUNTERS_TIMEOUT] = {
};
histogram_t TIMEOUT_HISTOGRAMS[NUM_HISTOGRAMS_TIMEOUT] = {
};
family_t TIMEOUT = {
        "TIMEOUT",
        NUM_EVENTS_TIMEOUT,
        NUM_COUNTERS_TIMEOUT,
        NUM_HISTOGRAMS_TIMEOUT,
        TIMEOUT_COUNTERS,
        TIMEOUT_HISTOGRAMS,
        0,
        0,
        0,
        0
};

int ec_table_offset= 2;
family_t *ec_family_table[NUM_FAMILIES] = {











        &EC2_GROUP,
        &TIMEOUT,
};


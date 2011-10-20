// $Id$

#include "rd_lifecycle_qos_test.h"

#include "duration.h"

ReaderDataLifecyclePolicyTest::ReaderDataLifecyclePolicyTest ()
{
}

bool
ReaderDataLifecyclePolicyTest::check (const ::DDS_ReaderDataLifecycleQosPolicy & dds_qos,
                                      const ::DDS::ReaderDataLifecycleQosPolicy & qos)
{
  return Duration::check (dds_qos.autopurge_nowriter_samples_delay,
                          qos.autopurge_nowriter_samples_delay)     &&
         Duration::check (dds_qos.autopurge_disposed_samples_delay,
                          qos.autopurge_disposed_samples_delay);
}

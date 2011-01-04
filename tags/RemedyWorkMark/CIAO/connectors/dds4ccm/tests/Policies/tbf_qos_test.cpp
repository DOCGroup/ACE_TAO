// $Id$

#include "tbf_qos_test.h"
#include "duration.h"

TimeBasedFilterPolicyTest::TimeBasedFilterPolicyTest ()
{
}

bool
TimeBasedFilterPolicyTest::check (const ::DDS_TimeBasedFilterQosPolicy & dds_qos,
                                  const ::DDS::TimeBasedFilterQosPolicy & qos)
{
  return Duration::check (dds_qos.minimum_separation, qos.minimum_separation);
}

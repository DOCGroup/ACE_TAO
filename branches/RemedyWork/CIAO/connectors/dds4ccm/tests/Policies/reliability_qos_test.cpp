// $Id$

#include "reliability_qos_test.h"

#include "duration.h"

ReliabilityPolicyTest::ReliabilityPolicyTest ()
{
}

bool
ReliabilityPolicyTest::check (const ::DDS_ReliabilityQosPolicy & dds_qos,
                              const ::DDS::ReliabilityQosPolicy & qos)
{
  return dds_qos.kind == static_cast <DDS_ReliabilityQosPolicyKind> (qos.kind) &&
         Duration::check (dds_qos.max_blocking_time, qos.max_blocking_time);
}

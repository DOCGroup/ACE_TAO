// $Id$

#include "lat_budget_qos_test.h"

#include "duration.h"

LatencyBudgetPolicyTest::LatencyBudgetPolicyTest ()
{
}

bool
LatencyBudgetPolicyTest::check (const ::DDS_LatencyBudgetQosPolicy & dds_qos,
                                const ::DDS::LatencyBudgetQosPolicy & qos)
{
  return Duration::check (dds_qos.duration, qos.duration);
}

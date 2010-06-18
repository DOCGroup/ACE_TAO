// $Id$

#include "lifespan_qos_test.h"

#include "duration.h"

LifespanPolicyTest::LifespanPolicyTest ()
{
}

bool
LifespanPolicyTest::check (const ::DDS_LifespanQosPolicy & dds_qos,
                           const ::DDS::LifespanQosPolicy & qos)
{
  return Duration::check (dds_qos.duration, qos.duration); 
}

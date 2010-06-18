// $Id$

#include "resource_limits_qos_test.h"

ResourceLimitsPolicyTest::ResourceLimitsPolicyTest ()
{
}


bool
ResourceLimitsPolicyTest::check (const ::DDS_ResourceLimitsQosPolicy & dds_qos,
                                 const ::DDS::ResourceLimitsQosPolicy & qos)
{
  return dds_qos.max_samples == qos.max_samples &&
         dds_qos.max_instances == qos.max_instances &&
         dds_qos.max_samples_per_instance == qos.max_samples_per_instance;
}

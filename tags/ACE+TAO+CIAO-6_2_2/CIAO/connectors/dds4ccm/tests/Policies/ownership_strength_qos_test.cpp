// $Id$

#include "ownership_strength_qos_test.h"

OwnershipStrengthPolicyTest::OwnershipStrengthPolicyTest ()
{
}

bool
OwnershipStrengthPolicyTest::check (const ::DDS_OwnershipStrengthQosPolicy & dds_qos,
                                    const ::DDS::OwnershipStrengthQosPolicy & qos)
{
  return dds_qos.value == qos.value;
}

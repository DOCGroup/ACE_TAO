// $Id$

#include "ownership_qos_test.h"

OwnershipPolicyTest::OwnershipPolicyTest ()
{
}

bool
OwnershipPolicyTest::check (const ::DDS_OwnershipQosPolicy & dds_qos,
                            const ::DDS::OwnershipQosPolicy & qos)
{
  return dds_qos.kind ==
         static_cast <DDS_OwnershipQosPolicyKind> (qos.kind);
}

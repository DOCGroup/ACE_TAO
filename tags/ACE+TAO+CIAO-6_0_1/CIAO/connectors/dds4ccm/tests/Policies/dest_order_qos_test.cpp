// $Id$

#include "dest_order_qos_test.h"

DestinationOrderPolicyTest::DestinationOrderPolicyTest ()
{
}

bool
DestinationOrderPolicyTest::check (const ::DDS_DestinationOrderQosPolicy & dds_qos,
                                   const ::DDS::DestinationOrderQosPolicy & qos)
{
  return dds_qos.kind == static_cast <DDS_DestinationOrderQosPolicyKind> (qos.kind);
}

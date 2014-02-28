// $Id$

#include "transport_prio_qos_test.h"

TransportPriorityPolicyTest::TransportPriorityPolicyTest ()
{
}

bool
TransportPriorityPolicyTest::check (const ::DDS_TransportPriorityQosPolicy & dds_qos,
                                    const ::DDS::TransportPriorityQosPolicy & qos)
{
  return dds_qos.value == qos.value;
}

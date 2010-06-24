//$Id$

#ifndef TRANSPORT_PRIORITY_QOS_TEST_H_
#define TRANSPORT_PRIORITY_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/TransportPriorityQosPolicy.h"

class TransportPriorityPolicyTest
{
public:
  TransportPriorityPolicyTest ();

  static bool check (const ::DDS_TransportPriorityQosPolicy & dds_qos,
                     const ::DDS::TransportPriorityQosPolicy & qos);
};

#endif /* TRANSPORT_PRIORITY_QOS_TEST_H_ */

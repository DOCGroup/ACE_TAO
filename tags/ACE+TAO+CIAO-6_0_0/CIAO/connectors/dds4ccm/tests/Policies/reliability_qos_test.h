//$Id$

#ifndef RELIABILITY_QOS_TEST_H_
#define RELIABILITY_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/ReliabilityQosPolicy.h"

class ReliabilityPolicyTest
{
public:
  ReliabilityPolicyTest ();

  static bool check (const ::DDS_ReliabilityQosPolicy & dds_qos,
                     const ::DDS::ReliabilityQosPolicy & qos);
};

#endif /* RELIABILITY_QOS_TEST_H_ */

//$Id$

#ifndef DURABILITY_QOS_TEST_H_
#define DURABILITY_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/DurabilityQosPolicy.h"

class DurabilityPolicyTest
{
public:
  DurabilityPolicyTest ();

  static bool check (const ::DDS_DurabilityQosPolicy & dds_qos,
                     const ::DDS::DurabilityQosPolicy & qos);
};

#endif /* DURABILITY_QOS_TEST_H_ */

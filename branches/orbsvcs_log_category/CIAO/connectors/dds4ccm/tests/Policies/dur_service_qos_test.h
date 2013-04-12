//$Id$

#ifndef DURABILITY_SERVICE_QOS_TEST_H_
#define DURABILITY_SERVICE_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/DurabilityServiceQosPolicy.h"

class DurabilityServicePolicyTest
{
public:
  DurabilityServicePolicyTest ();

  static bool check (const ::DDS_DurabilityServiceQosPolicy & dds_qos,
                     const ::DDS::DurabilityServiceQosPolicy & qos);
};

#endif /* DURABILITY_SERVICE_QOS_TEST_H_ */

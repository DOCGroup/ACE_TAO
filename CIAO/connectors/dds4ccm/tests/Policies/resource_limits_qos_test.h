//$Id$

#ifndef RESOURCE_LIMITS_QOS_TEST_H_
#define RESOURCE_LIMITS_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/ResourceLimitsQosPolicy.h"

class ResourceLimitsPolicyTest
{
public:
  ResourceLimitsPolicyTest ();

  static bool check (const ::DDS_ResourceLimitsQosPolicy & dds_qos,
                     const ::DDS::ResourceLimitsQosPolicy & qos);
};

#endif /* RESOURCE_LIMITS_QOS_TEST_H_ */

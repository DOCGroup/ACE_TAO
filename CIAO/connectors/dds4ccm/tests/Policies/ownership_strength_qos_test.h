//$Id$

#ifndef OWNERSHIP_STRENGTH_QOS_TEST_H_
#define OWNERSHIP_STRENGTH_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/OwnershipStrengthQosPolicy.h"

class OwnershipStrengthPolicyTest
{
public:
  OwnershipStrengthPolicyTest ();

  static bool check (const ::DDS_OwnershipStrengthQosPolicy & dds_qos,
                     const ::DDS::OwnershipStrengthQosPolicy & qos);
};

#endif /* OWNERSHIP_STRENGTH_QOS_TEST_H_ */

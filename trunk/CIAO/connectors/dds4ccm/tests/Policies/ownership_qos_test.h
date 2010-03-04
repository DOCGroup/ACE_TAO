//$Id$

#ifndef OWNERSHIP_QOS_TEST_H_
#define OWNERSHIP_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/OwnershipQosPolicy.h"

class OwnershipPolicyTest
{
public:
  OwnershipPolicyTest ();

  static bool check (const ::DDS_OwnershipQosPolicy & dds_qos,
                     const ::DDS::OwnershipQosPolicy & qos);
};

#endif /* OWNERSHIP_QOS_TEST_H_ */

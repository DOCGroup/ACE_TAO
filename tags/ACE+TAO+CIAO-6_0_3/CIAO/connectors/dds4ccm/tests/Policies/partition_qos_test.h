//$Id$

#ifndef PARTITION_QOS_TEST_H_
#define PARTITION_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/PartitionQosPolicy.h"

class PartitionPolicyTest
{
public:
  PartitionPolicyTest ();

  static bool check (const ::DDS_PartitionQosPolicy & dds_qos,
                     const ::DDS::PartitionQosPolicy & qos);
};

#endif /* PARTITION_QOS_TEST_H_ */

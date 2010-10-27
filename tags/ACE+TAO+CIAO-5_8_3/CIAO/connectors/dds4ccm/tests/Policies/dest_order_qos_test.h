//$Id$

#ifndef DEST_ORDER_QOS_TEST_H_
#define DEST_ORDER_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/DestinationOrderQosPolicy.h"

class DestinationOrderPolicyTest
{
public:
  DestinationOrderPolicyTest ();

  static bool check (const ::DDS_DestinationOrderQosPolicy & dds_qos,
                     const ::DDS::DestinationOrderQosPolicy & qos);
};

#endif /* DEST_ORDER_QOS_TEST_H_ */

//$Id$

#ifndef LIFESPAN_QOS_TEST_H_
#define LIFESPAN_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/LifespanQosPolicy.h"

class LifespanPolicyTest
{
public:
  LifespanPolicyTest ();

  static bool check (const ::DDS_LifespanQosPolicy & dds_qos,
                     const ::DDS::LifespanQosPolicy & qos);
};

#endif /* LIFESPAN_QOS_TEST_H_ */

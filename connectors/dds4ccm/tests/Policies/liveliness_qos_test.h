//$Id$

#ifndef LIVELINESS_QOS_TEST_H_
#define LIVELINESS_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/LivelinessQosPolicy.h"

class LivelinessPolicyTest
{
public:
  LivelinessPolicyTest ();

  static bool check (const ::DDS_LivelinessQosPolicy & dds_qos,
                     const ::DDS::LivelinessQosPolicy & qos);
};

#endif /* LIVELINESS_QOS_TEST_H_ */

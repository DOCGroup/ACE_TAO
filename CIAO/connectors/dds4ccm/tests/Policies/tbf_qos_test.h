//$Id$

#ifndef TIME_BASED_FILTER_QOS_TEST_H_
#define TIME_BASED_FILTER_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/TimeBasedFilterQosPolicy.h"

class TimeBasedFilterPolicyTest
{
public:
  TimeBasedFilterPolicyTest ();

  static bool check (const ::DDS_TimeBasedFilterQosPolicy & dds_qos,
                     const ::DDS::TimeBasedFilterQosPolicy & qos);
};

#endif /* TIME_BASED_FILTER_QOS_TEST_H_ */

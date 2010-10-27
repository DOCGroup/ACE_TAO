//$Id$

#ifndef TOPIC_DATA_QOS_TEST_H_
#define TOPIC_DATA_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/TopicDataQosPolicy.h"

class TopicDataPolicyTest
{
public:
  TopicDataPolicyTest ();

  static bool check (const ::DDS_TopicDataQosPolicy & dds_qos,
                     const ::DDS::TopicDataQosPolicy & qos);
};

#endif /* TOPIC_DATA_QOS_TEST_H_ */

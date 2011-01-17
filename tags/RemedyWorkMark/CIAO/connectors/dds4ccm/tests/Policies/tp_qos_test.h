// $Id$

#ifndef TOPIC_QOS_TEST_H_
#define TOPIC_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/TopicQos.h"

class TopicPolicyTest
{
public:
  TopicPolicyTest ();

  int run ();
};

#endif /* TOPIC_QOS_TEST_H_ */

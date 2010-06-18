// $Id$

#ifndef SUBSCRIBER_QOS_TEST_H_
#define SUBSCRIBER_QOS_TEST_H_


#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/SubscriberQos.h"

class SubscriberPolicyTest
{
public:
  SubscriberPolicyTest ();

  int run ();
};

#endif /* SUBSCRIBER_QOS_TEST_H_ */

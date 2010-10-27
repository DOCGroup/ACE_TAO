//$Id$

#ifndef DEADLINE_QOS_TEST_H_
#define DEADLINE_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/DeadlineQosPolicy.h"

class DeadlinePolicyTest
{
public:
  DeadlinePolicyTest ();

  static bool check (const ::DDS_DeadlineQosPolicy & dds_qos,
                     const ::DDS::DeadlineQosPolicy & qos);
};

#endif /* DEADLINE_QOS_TEST_H_ */

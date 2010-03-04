//$Id$

#ifndef LATENCY_BUDGET_QOS_TEST_H_
#define LATENCY_BUDGET_QOS_TEST_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/LatencyBudgetQosPolicy.h"

class LatencyBudgetPolicyTest
{
public:
  LatencyBudgetPolicyTest ();

  static bool check (const ::DDS_LatencyBudgetQosPolicy & dds_qos,
                     const ::DDS::LatencyBudgetQosPolicy & qos);
};

#endif /* LATENCY_BUDGET_QOS_TEST_H_ */

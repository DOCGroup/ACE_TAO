// $Id$

#include "tp_qos_test.h"

#include "tp_data_qos_test.h"
#include "dur_qos_test.h"
#include "dur_service_qos_test.h"
#include "deadline_qos_test.h"
#include "lat_budget_qos_test.h"
#include "liveliness_qos_test.h"
#include "reliability_qos_test.h"
#include "dest_order_qos_test.h"
#include "hist_qos_test.h"
#include "resource_limits_qos_test.h"
#include "transport_prio_qos_test.h"
#include "lifespan_qos_test.h"
#include "ownership_qos_test.h"

TopicPolicyTest::TopicPolicyTest ()
{
}

int
TopicPolicyTest::run ()
{
  int ret = 0;
  try
    {
      ::DDS::TopicQos policy;
      DDS_TopicQos dds_policy_org, dds_policy;

      dds_policy_org = DDS_TOPIC_QOS_DEFAULT;

      policy <<= dds_policy_org;
      dds_policy <<= policy;


      if (!TopicDataPolicyTest::check (dds_policy.topic_data,
                                       policy.topic_data))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in TopicDataQosPolicy.\n"));
          ++ret;
        }

      if (!DurabilityPolicyTest::check (dds_policy.durability,
                                        policy.durability))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in DurabilityQosPolicy.\n"));
          ++ret;
        }

      if (!DurabilityServicePolicyTest::check (dds_policy.durability_service,
                                               policy.durability_service))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in DurabilityServiceQosPolicy.\n"));
          ++ret;
        }

      if (!DeadlinePolicyTest::check (dds_policy.deadline,
                                      policy.deadline))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in DeadlineQosPolicy.\n"));
          ++ret;
        }

      if (!LatencyBudgetPolicyTest::check (dds_policy.latency_budget,
                                           policy.latency_budget))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in LatencyBudgetQosPolicy.\n"));
          ++ret;
        }

      if (!LivelinessPolicyTest::check (dds_policy.liveliness,
                                        policy.liveliness))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in LivelinessQosPolicy.\n"));
          ++ret;
        }

      if (!ReliabilityPolicyTest::check (dds_policy.reliability,
                                         policy.reliability))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in ReliabilityQosPolicy.\n"));
          ++ret;
        }

      if (!DestinationOrderPolicyTest::check (dds_policy.destination_order,
                                             policy.destination_order))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in DestinationOrderQosPolicy.\n"));
          ++ret;
        }

      if (!HistoryPolicyTest::check (dds_policy.history,
                                     policy.history))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in HistoryQosPolicy.\n"));
          ++ret;
        }

      if (!ResourceLimitsPolicyTest::check (dds_policy.resource_limits,
                                     policy.resource_limits))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in ResourceLimitsQosPolicy.\n"));
          ++ret;
        }

      if (!TransportPriorityPolicyTest::check (dds_policy.transport_priority,
                                              policy.transport_priority))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in TransportPriorityQosPolicy.\n"));
          ++ret;
        }

      if (!LifespanPolicyTest::check (dds_policy.lifespan,
                                      policy.lifespan))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in LifespanQosPolicy.\n"));
          ++ret;
        }

      if (!OwnershipPolicyTest::check (dds_policy.ownership,
                                       policy.ownership))
        {
          ACE_ERROR ((LM_ERROR, "ERROR TOPIC : "
                                "Error in OwnershipQosPolicy.\n"));
          ++ret;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR TOPIC: Unexpected exception caught."));
      return -1;
    }
  return ret;
}

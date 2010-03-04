// $Id$

#include "dw_qos_test.h"

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
#include "user_data_qos_test.h"
#include "ownership_qos_test.h"
#include "ownership_strength_qos_test.h"
#include "wr_lifecycle_qos_test.h"

DatawriterPolicyTest::DatawriterPolicyTest ()
{
}

int
DatawriterPolicyTest::run ()
{
  int ret = 0;
  try
    {
      ::DDS::DataWriterQos policy;
      DDS_DataWriterQos dds_policy_org, dds_policy;

      dds_policy_org = DDS_DATAWRITER_QOS_DEFAULT;

      policy <<= dds_policy_org;
      dds_policy <<= policy;

      if (!DurabilityPolicyTest::check (dds_policy.durability,
                                        policy.durability))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in DurabilityQosPolicy.\n"));
          ++ret;
        }

      if (!DurabilityServicePolicyTest::check (dds_policy.durability_service,
                                               policy.durability_service))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in DurabilityServiceQosPolicy.\n"));
          ++ret;
        }

      if (!DeadlinePolicyTest::check (dds_policy.deadline,
                                      policy.deadline))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in DeadlineQosPolicy.\n"));
          ++ret;
        }

      if (!LatencyBudgetPolicyTest::check (dds_policy.latency_budget,
                                           policy.latency_budget))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in LatencyBudgetQosPolicy.\n"));
          ++ret;
        }

      if (!LivelinessPolicyTest::check (dds_policy.liveliness,
                                        policy.liveliness))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in LivelinessQosPolicy.\n"));
          ++ret;
        }

      if (!ReliabilityPolicyTest::check (dds_policy.reliability,
                                         policy.reliability))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in ReliabilityQosPolicy.\n"));
          ++ret;
        }

      if (!DestinationOrderPolicyTest::check (dds_policy.destination_order,
                                             policy.destination_order))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in DestinationOrderQosPolicy.\n"));
          ++ret;
        }

      if (!HistoryPolicyTest::check (dds_policy.history,
                                     policy.history))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in HistoryQosPolicy.\n"));
          ++ret;
        }

      if (!ResourceLimitsPolicyTest::check (dds_policy.resource_limits,
                                     policy.resource_limits))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in ResourceLimitsQosPolicy.\n"));
          ++ret;
        }

      if (!TransportPriorityPolicyTest::check (dds_policy.transport_priority,
                                              policy.transport_priority))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in TransportPriorityQosPolicy.\n"));
          ++ret;
        }

      if (!LifespanPolicyTest::check (dds_policy.lifespan,
                                      policy.lifespan))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in LifespanQosPolicy.\n"));
          ++ret;
        }

      if (!UserDataPolicyTest::check (dds_policy.user_data, policy.user_data))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in UserDataQosPolicy.\n"));
          ++ret;
        }

      if (!OwnershipPolicyTest::check (dds_policy.ownership,
                                       policy.ownership))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in OwnershipQosPolicy.\n"));
          ++ret;
        }

      if (!OwnershipStrengthPolicyTest::check (dds_policy.ownership_strength,
                                       policy.ownership_strength))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in OwnershipStrengthQosPolicy.\n"));
          ++ret;
        }

      if (!WriterDataLifecyclePolicyTest::check (dds_policy.writer_data_lifecycle,
                                                 policy.writer_data_lifecycle))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER : "
                                "Error in WriterDataLifecycleQosPolicy.\n"));
          ++ret;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR DATAWRITER: Unexpected exception caught."));
      return -1;
    }
  return ret;
}

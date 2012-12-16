// $Id$

#include "dr_qos_test.h"

#include "dur_qos_test.h"
#include "deadline_qos_test.h"
#include "lat_budget_qos_test.h"
#include "liveliness_qos_test.h"
#include "reliability_qos_test.h"
#include "dest_order_qos_test.h"
#include "hist_qos_test.h"
#include "resource_limits_qos_test.h"
#include "user_data_qos_test.h"
#include "ownership_qos_test.h"
#include "tbf_qos_test.h"
#include "rd_lifecycle_qos_test.h"
#include "typeconsistencyenforcement_qos_test.h"

DatareaderPolicyTest::DatareaderPolicyTest ()
{
}

int
DatareaderPolicyTest::run ()
{
  int ret = 0;
  try
    {
      ::DDS::DataReaderQos policy;
      DDS_DataReaderQos dds_policy_org, dds_policy;

      dds_policy_org = DDS_DATAREADER_QOS_DEFAULT;

      policy <<= dds_policy_org;
      dds_policy <<= policy;

      if (!DurabilityPolicyTest::check (dds_policy.durability,
                                        policy.durability))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in DurabilityQosPolicy.\n"));
          ++ret;
        }

      if (!DeadlinePolicyTest::check (dds_policy.deadline,
                                      policy.deadline))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in DeadlineQosPolicy.\n"));
          ++ret;
        }

      if (!LatencyBudgetPolicyTest::check (dds_policy.latency_budget,
                                           policy.latency_budget))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in LatencyBudgetQosPolicy.\n"));
          ++ret;
        }

      if (!LivelinessPolicyTest::check (dds_policy.liveliness,
                                        policy.liveliness))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in LivelinessQosPolicy.\n"));
          ++ret;
        }

      if (!ReliabilityPolicyTest::check (dds_policy.reliability,
                                         policy.reliability))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in ReliabilityQosPolicy.\n"));
          ++ret;
        }

      if (!DestinationOrderPolicyTest::check (dds_policy.destination_order,
                                             policy.destination_order))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in DestinationOrderQosPolicy.\n"));
          ++ret;
        }

      if (!HistoryPolicyTest::check (dds_policy.history,
                                     policy.history))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in HistoryQosPolicy.\n"));
          ++ret;
        }

      if (!ResourceLimitsPolicyTest::check (dds_policy.resource_limits,
                                     policy.resource_limits))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in ResourceLimitsQosPolicy.\n"));
          ++ret;
        }

      if (!UserDataPolicyTest::check (dds_policy.user_data, policy.user_data))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in UserDataQosPolicy.\n"));
          ++ret;
        }

      if (!OwnershipPolicyTest::check (dds_policy.ownership,
                                       policy.ownership))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in OwnershipQosPolicy.\n"));
          ++ret;
        }

      if (!TimeBasedFilterPolicyTest::check (dds_policy.time_based_filter,
                                             policy.time_based_filter))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in TimeBasedFilterQosPolicy.\n"));
          ++ret;
        }

      if (!ReaderDataLifecyclePolicyTest::check (dds_policy.reader_data_lifecycle,
                                                 policy.reader_data_lifecycle))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in ReaderDataLifecycleQosPolicy.\n"));
          ++ret;
        }

#if defined (RTI_DDS_VERSION_MAJOR) && (RTI_DDS_VERSION_MAJOR >= 5)
      if (!TypeConsistencyEnforcementPolicyPolicyTest::check (dds_policy.type_consistency,
                                                 policy.type_consistency))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DATAREADER : "
                                "Error in TypeConsistencyEnforcementQosPolicy.\n"));
          ++ret;
        }
#endif
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR DATAREADER: Unexpected exception caught."));
      return -1;
    }
  return ret;
}

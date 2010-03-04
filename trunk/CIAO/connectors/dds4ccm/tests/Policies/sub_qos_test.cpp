// $Id$

#include "sub_qos_test.h"

#include "group_data_qos_test.h"
#include "entity_factory_qos_test.h"
#include "presentation_qos_test.h"
#include "partition_qos_test.h"

SubscriberPolicyTest::SubscriberPolicyTest ()
{
}

int
SubscriberPolicyTest::run ()
{
  int ret = 0;
  try
    {
      ::DDS::SubscriberQos policy;
      DDS_SubscriberQos dds_policy_org, dds_policy;

      dds_policy_org = DDS_SUBSCRIBER_QOS_DEFAULT;

      policy <<= dds_policy_org;
      dds_policy <<= policy;

      if (!PresentationPolicyTest::check (dds_policy.presentation, policy.presentation))
        {
          ACE_ERROR ((LM_ERROR, "ERROR SUBSCRIBER : "
                                "Error in PresentationQosPolicy.\n"));
          ++ret;
        }
      if (!PartitionPolicyTest::check (dds_policy.partition, policy.partition))
        {
          ACE_ERROR ((LM_ERROR, "ERROR SUBSCRIBER : "
                                "Error in PartitionQosPolicy.\n"));
          ++ret;
        }
      if (!GroupDataPolicyTest::check (dds_policy.group_data, policy.group_data))
        {
          ACE_ERROR ((LM_ERROR, "ERROR SUBSCRIBER : "
                                "Error in GroupDataQosPolicy.\n"));
          ++ret;
        }
      if (!EntityFactoryPolicyTest::check (dds_policy.entity_factory, policy.entity_factory))
        {
          ACE_ERROR ((LM_ERROR, "ERROR SUBSCRIBER : "
                                "Error in EntityFactoryQosPolicy.\n"));
          ++ret;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR SUBSCRIBER: Unexpected exception caught."));
      return -1;
    }
  return ret;
}
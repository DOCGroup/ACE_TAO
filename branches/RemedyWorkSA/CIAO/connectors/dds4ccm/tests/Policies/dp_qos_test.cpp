// $Id$

#include "dp_qos_test.h"

#include "entity_factory_qos_test.h"
#include "user_data_qos_test.h"

DomainParticipantPolicyTest::DomainParticipantPolicyTest ()
{
}

int
DomainParticipantPolicyTest::run ()
{
  int ret = 0;
  try
    {
      ::DDS::DomainParticipantQos policy;
      DDS_DomainParticipantQos dds_policy_org, dds_policy;

      dds_policy_org = DDS_PARTICIPANT_QOS_DEFAULT;

      policy <<= dds_policy_org;
      dds_policy <<= policy;

      if (!EntityFactoryPolicyTest::check (dds_policy.entity_factory, policy.entity_factory))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DOMAIN PARTICIPANT : "
                                "Error in EntityFactoryQosPolicy.\n"));
          ++ret;
        }

      if (!UserDataPolicyTest::check (dds_policy.user_data, policy.user_data))
        {
          ACE_ERROR ((LM_ERROR, "ERROR DOMAIN PARTICIPANT : "
                                "Error in UserDataQosPolicy.\n"));
          ++ret;
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR DOMAIN PARTICIPANT: Unexpected exception caught."));
      return -1;
    }
  return ret;
}

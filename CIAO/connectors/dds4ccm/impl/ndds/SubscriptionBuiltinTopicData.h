/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef SUBSCRIPTIONBUILTINTOPICDATA_H_
#define SUBSCRIPTIONBUILTINTOPICDATA_H_

#include "BuiltinTopicKey_t.h"
#include "DurabilityQosPolicy.h"
#include "DeadlineQosPolicy.h"
#include "LatencyBudgetQosPolicy.h"
#include "LivelinessQosPolicy.h"
#include "ReliabilityQosPolicy.h"
#include "OwnershipQosPolicy.h"
#include "OwnershipStrengthQosPolicy.h"
#include "DestinationOrderQosPolicy.h"
#include "UserDataQosPolicy.h"
#include "TimeBasedFilterQosPolicy.h"
#include "PresentationQosPolicy.h"
#include "PartitionQosPolicy.h"
#include "TopicDataQosPolicy.h"
#include "GroupDataQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::SubscriptionBuiltinTopicData &ddstopicdata, const ::DDS_SubscriptionBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.participant_key <<= topicdata.participant_key;
  ddstopicdata.topic_name = topicdata.topic_name;
  ddstopicdata.type_name = topicdata.type_name;

  ddstopicdata.durability <<= topicdata.durability;
  ddstopicdata.deadline <<= topicdata.deadline;
  ddstopicdata.latency_budget <<= topicdata.latency_budget;
  ddstopicdata.liveliness <<= topicdata.liveliness;
  ddstopicdata.reliability <<= topicdata.reliability;
  ddstopicdata.ownership <<= topicdata.ownership;
  ddstopicdata.destination_order <<= topicdata.destination_order;
  ddstopicdata.user_data <<= topicdata.user_data;
  ddstopicdata.time_based_filter <<= topicdata.time_based_filter;

  ddstopicdata.presentation <<= topicdata.presentation;
  ddstopicdata.partition <<= topicdata.partition;
  ddstopicdata.topic_data <<= topicdata.topic_data;
  ddstopicdata.group_data <<= topicdata.group_data;
}

inline void
operator<<= (::DDS_SubscriptionBuiltinTopicData & ddstopicdata, const ::DDS::SubscriptionBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.participant_key <<= topicdata.participant_key;
  ddstopicdata.topic_name = CORBA::string_dup (topicdata.topic_name);
  ddstopicdata.type_name = CORBA::string_dup (topicdata.type_name);

  ddstopicdata.durability <<= topicdata.durability;
  ddstopicdata.deadline <<= topicdata.deadline;
  ddstopicdata.latency_budget <<= topicdata.latency_budget;
  ddstopicdata.liveliness <<= topicdata.liveliness;
  ddstopicdata.reliability <<= topicdata.reliability;
  ddstopicdata.ownership <<= topicdata.ownership;
  ddstopicdata.destination_order <<= topicdata.destination_order;
  ddstopicdata.user_data <<= topicdata.user_data;
  ddstopicdata.time_based_filter <<= topicdata.time_based_filter;

  ddstopicdata.presentation <<= topicdata.presentation;
  ddstopicdata.partition <<= topicdata.partition;
  ddstopicdata.topic_data <<= topicdata.topic_data;
  ddstopicdata.group_data <<= topicdata.group_data;
}

#endif

#endif /* SUBSCRIPTIONBUILTINTOPICDATA_H_ */

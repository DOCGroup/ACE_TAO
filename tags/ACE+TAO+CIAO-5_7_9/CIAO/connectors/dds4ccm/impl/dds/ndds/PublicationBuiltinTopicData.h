/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef PUBLICATIONBUILTINTOPICDATA_H_
#define PUBLICATIONBUILTINTOPICDATA_H_

#include "BuiltinTopicKey_t.h"
#include "DurabilityQosPolicy.h"
#include "DurabilityServiceQosPolicy.h"
#include "DeadlineQosPolicy.h"
#include "LatencyBudgetQosPolicy.h"
#include "LivelinessQosPolicy.h"
#include "ReliabilityQosPolicy.h"
#include "LifespanQosPolicy.h"
#include "UserDataQosPolicy.h"
#include "OwnershipQosPolicy.h"
#include "OwnershipStrengthQosPolicy.h"
#include "DestinationOrderQosPolicy.h"
#include "PresentationQosPolicy.h"
#include "PartitionQosPolicy.h"
#include "TopicDataQosPolicy.h"
#include "GroupDataQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::PublicationBuiltinTopicData &ddstopicdata, const ::DDS_PublicationBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.participant_key <<= topicdata.participant_key;
  ddstopicdata.topic_name = topicdata.topic_name;
  ddstopicdata.type_name = topicdata.type_name;
  ddstopicdata.durability <<= topicdata.durability;
  ddstopicdata.durability_service <<= topicdata.durability_service;
  ddstopicdata.deadline <<= topicdata.deadline;
  ddstopicdata.latency_budget <<= topicdata.latency_budget;
  ddstopicdata.liveliness <<= topicdata.liveliness;
  ddstopicdata.reliability <<= topicdata.reliability;
  ddstopicdata.lifespan <<= topicdata.lifespan;
  ddstopicdata.user_data <<= topicdata.user_data;
  ddstopicdata.ownership <<= topicdata.ownership;
  ddstopicdata.ownership_strength <<= topicdata.ownership_strength;
  ddstopicdata.destination_order <<= topicdata.destination_order;
  ddstopicdata.presentation <<= topicdata.presentation;
  ddstopicdata.partition <<= topicdata.partition;
  ddstopicdata.topic_data <<= topicdata.topic_data;
  ddstopicdata.group_data <<= topicdata.group_data;
}

inline void
operator<<= (::DDS_PublicationBuiltinTopicData & ddstopicdata, const ::DDS::PublicationBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.participant_key <<= topicdata.participant_key;
  ddstopicdata.topic_name = CORBA::string_dup (topicdata.topic_name);
  ddstopicdata.type_name = CORBA::string_dup (topicdata.type_name);
  ddstopicdata.durability <<= topicdata.durability;
  ddstopicdata.durability_service <<= topicdata.durability_service;
  ddstopicdata.deadline <<= topicdata.deadline;
  ddstopicdata.latency_budget <<= topicdata.latency_budget;
  ddstopicdata.liveliness <<= topicdata.liveliness;
  ddstopicdata.reliability <<= topicdata.reliability;
  ddstopicdata.lifespan <<= topicdata.lifespan;
  ddstopicdata.user_data <<= topicdata.user_data;
  ddstopicdata.ownership <<= topicdata.ownership;
  ddstopicdata.ownership_strength <<= topicdata.ownership_strength;
  ddstopicdata.destination_order <<= topicdata.destination_order;
  ddstopicdata.presentation <<= topicdata.presentation;
  ddstopicdata.partition <<= topicdata.partition;
  ddstopicdata.topic_data <<= topicdata.topic_data;
  ddstopicdata.group_data <<= topicdata.group_data;
}

#endif

#endif

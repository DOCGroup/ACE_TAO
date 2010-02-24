/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef PUBLICATIONBUILTINTOPICDATA_H_
#define PUBLICATIONBUILTINTOPICDATA_H_

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::PublicationBuiltinTopicData &ddstopicdata, const ::DDS_PublicationBuiltinTopicData & topicdata)
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

inline void
operator<<= (::DDS_PublicationBuiltinTopicData & ddstopicdata, const ::DDS::PublicationBuiltinTopicData & topicdata)
{
  topicdata.key <<= ddstopicdata.key;
  topicdata.participant_key <<= ddstopicdata.participant_key;
  topicdata.topic_name = CORBA::string_dup (ddstopicdata.topic_name);
  topicdata.type_name = CORBA::string_dup (ddstopicdata.type_name);
  topicdata.durability <<= ddstopicdata.durability;
  topicdata.durability_service <<= ddstopicdata.durability_service;
  topicdata.deadline <<= ddstopicdata.deadline;
  topicdata.latency_budget <<= ddstopicdata.latency_budget;
  topicdata.liveliness <<= ddstopicdata.liveliness;
  topicdata.reliability <<= ddstopicdata.reliability;
  topicdata.lifespan <<= ddstopicdata.lifespan;
  topicdata.user_data <<= ddstopicdata.user_data;
  topicdata.ownership <<= ddstopicdata.ownership;
  topicdata.ownership_strength <<= ddstopicdata.ownership_strength;
  topicdata.destination_order <<= ddstopicdata.destination_order;
  topicdata.presentation <<= ddstopicdata.presentation;
  topicdata.partition <<= ddstopicdata.partition;
  topicdata.topic_data <<= ddstopicdata.topic_data;
  topicdata.group_data <<= ddstopicdata.group_data;
}

inline void
operator>>= (const ::DDS_PublicationBuiltinTopicData & topicdata, ::DDS::PublicationBuiltinTopicData & ddstopicdata)
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

inline void
operator>>= (const ::DDS::PublicationBuiltinTopicData & topicdata, ::DDS_PublicationBuiltinTopicData & ddstopicdata)
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

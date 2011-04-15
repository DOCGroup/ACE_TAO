/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef TOPICBUILTINTOPICDATA_H_
#define TOPICBUILTINTOPICDATA_H_

#include "BuiltinTopicKey_t.h"
#include "DurabilityQosPolicy.h"
#include "DurabilityServiceQosPolicy.h"
#include "DeadlineQosPolicy.h"
#include "LatencyBudgetQosPolicy.h"
#include "LivelinessQosPolicy.h"
#include "ReliabilityQosPolicy.h"
#include "TransportPriorityQosPolicy.h"
#include "LifespanQosPolicy.h"
#include "DestinationOrderQosPolicy.h"
#include "HistoryQosPolicy.h"
#include "ResourceLimitsQosPolicy.h"
#include "TopicDataQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::TopicBuiltinTopicData &ddstopicdata, const ::DDS_TopicBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.name = topicdata.name;
  ddstopicdata.type_name = topicdata.type_name;
  ddstopicdata.durability <<= topicdata.durability;
  ddstopicdata.durability_service <<= topicdata.durability_service;
  ddstopicdata.deadline <<= topicdata.deadline;
  ddstopicdata.latency_budget <<= topicdata.latency_budget;
  ddstopicdata.liveliness <<= topicdata.liveliness;
  ddstopicdata.reliability <<= topicdata.reliability;
  ddstopicdata.transport_priority <<= topicdata.transport_priority;
  ddstopicdata.lifespan <<= topicdata.lifespan;
  ddstopicdata.destination_order <<= topicdata.destination_order;
  ddstopicdata.history <<= topicdata.history;
  ddstopicdata.resource_limits <<= topicdata.resource_limits;
  ddstopicdata.ownership <<= topicdata.ownership;
  ddstopicdata.topic_data <<= topicdata.topic_data;
}

inline void
operator<<= (::DDS_TopicBuiltinTopicData & ddstopicdata, const ::DDS::TopicBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.name = CORBA::string_dup (topicdata.name);
  ddstopicdata.type_name = CORBA::string_dup (topicdata.type_name);
  ddstopicdata.durability <<= topicdata.durability;
  ddstopicdata.durability_service <<= topicdata.durability_service;
  ddstopicdata.deadline <<= topicdata.deadline;
  ddstopicdata.latency_budget <<= topicdata.latency_budget;
  ddstopicdata.liveliness <<= topicdata.liveliness;
  ddstopicdata.reliability <<= topicdata.reliability;
  ddstopicdata.transport_priority <<= topicdata.transport_priority;
  ddstopicdata.lifespan <<= topicdata.lifespan;
  ddstopicdata.destination_order <<= topicdata.destination_order;
  ddstopicdata.history <<= topicdata.history;
  ddstopicdata.resource_limits <<= topicdata.resource_limits;
  ddstopicdata.ownership <<= topicdata.ownership;
  ddstopicdata.topic_data <<= topicdata.topic_data;
}

#endif /* TOPICBUILTINTOPICDATA_H_ */

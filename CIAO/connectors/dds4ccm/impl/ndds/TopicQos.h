/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TOPICQOS_H
#define DDS4CCM_TOPICQOS_H

#include "TopicDataQosPolicy.h"
#include "DurabilityQosPolicy.h"
#include "DurabilityServiceQosPolicy.h"
#include "DeadlineQosPolicy.h"
#include "LatencyBudgetQosPolicy.h"
#include "LivelinessQosPolicy.h"
#include "ReliabilityQosPolicy.h"
#include "DestinationOrderQosPolicy.h"
#include "HistoryQosPolicy.h"
#include "ResourceLimitsQosPolicy.h"
#include "TransportPriorityQosPolicy.h"
#include "LifespanQosPolicy.h"
#include "OwnershipQosPolicy.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::TopicQos DDS_TopicQos;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::TopicQos &ddsqos, const ::DDS_TopicQos & qos)
{
  ddsqos.topic_data <<= qos.topic_data;
  ddsqos.durability <<= qos.durability;
  ddsqos.durability_service <<= qos.durability_service;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.transport_priority <<= qos.transport_priority;
  ddsqos.lifespan <<= qos.lifespan;
  ddsqos.ownership <<= qos.ownership;
}

inline void
operator<<= (::DDS_TopicQos &ddsqos, const ::DDS::TopicQos & qos)
{
  ddsqos.topic_data <<= qos.topic_data;
  ddsqos.durability <<= qos.durability;
  ddsqos.durability_service <<= qos.durability_service;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.transport_priority <<= qos.transport_priority;
  ddsqos.lifespan <<= qos.lifespan;
  ddsqos.ownership <<= qos.ownership;
}

inline void
operator>>= (const ::DDS_TopicQos &qos, ::DDS::TopicQos & ddsqos)
{
  ddsqos.topic_data <<= qos.topic_data;
  ddsqos.durability <<= qos.durability;
  ddsqos.durability_service <<= qos.durability_service;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.transport_priority <<= qos.transport_priority;
  ddsqos.lifespan <<= qos.lifespan;
  ddsqos.ownership <<= qos.ownership;
}

inline void
operator>>= (const ::DDS::TopicQos &qos, ::DDS_TopicQos & ddsqos)
{
  ddsqos.topic_data <<= qos.topic_data;
  ddsqos.durability <<= qos.durability;
  ddsqos.durability_service <<= qos.durability_service;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.transport_priority <<= qos.transport_priority;
  ddsqos.lifespan <<= qos.lifespan;
  ddsqos.ownership <<= qos.ownership;
}
#endif

#endif /* DDS4CCM_TOPICQOS_H */

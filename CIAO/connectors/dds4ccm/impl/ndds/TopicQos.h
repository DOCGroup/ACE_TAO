/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TOPICQOS_H
#define CIAO_RTI_TOPICQOS_H

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

#endif /* CIAO_RTI_TOPICQOS_H */

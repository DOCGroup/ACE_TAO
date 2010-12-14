/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DATAWRITERQOS_H
#define DDS4CCM_DATAWRITERQOS_H

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
#include "UserDataQosPolicy.h"
#include "OwnershipQosPolicy.h"
#include "OwnershipStrengthQosPolicy.h"
#include "WriterDataLifecycleQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::DataWriterQos &ddsqos, const ::DDS_DataWriterQos & qos)
{
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
  ddsqos.user_data <<= qos.user_data;
  ddsqos.ownership <<= qos.ownership;
  ddsqos.ownership_strength <<= qos.ownership_strength;
  ddsqos.writer_data_lifecycle <<= qos.writer_data_lifecycle;
}

inline void
operator<<= (::DDS_DataWriterQos &ddsqos, const ::DDS::DataWriterQos & qos)
{
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
  ddsqos.user_data <<= qos.user_data;
  ddsqos.ownership <<= qos.ownership;
  ddsqos.ownership_strength <<= qos.ownership_strength;
  ddsqos.writer_data_lifecycle <<= qos.writer_data_lifecycle;
}

#endif /* DDS4CCM_DATAWRITERQOS_H */

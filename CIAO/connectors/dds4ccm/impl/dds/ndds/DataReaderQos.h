/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DATAREADERQOS_H
#define DDS4CCM_DATAREADERQOS_H

#include "DurabilityQosPolicy.h"
#include "DeadlineQosPolicy.h"
#include "LatencyBudgetQosPolicy.h"
#include "LivelinessQosPolicy.h"
#include "ReliabilityQosPolicy.h"
#include "DestinationOrderQosPolicy.h"
#include "HistoryQosPolicy.h"
#include "ResourceLimitsQosPolicy.h"
#include "UserDataQosPolicy.h"
#include "OwnershipQosPolicy.h"
#include "TimeBasedFilterQosPolicy.h"
#include "ReaderDataLifecycleQosPolicy.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DataReaderQos DDS_DataReaderQos;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::DataReaderQos &ddsqos, const ::DDS_DataReaderQos & qos)
{
  ddsqos.durability <<= qos.durability;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.user_data <<= qos.user_data;
  ddsqos.ownership <<= qos.ownership;
  ddsqos.time_based_filter <<= qos.time_based_filter;
  ddsqos.reader_data_lifecycle <<= qos.reader_data_lifecycle;
}

inline void
operator<<= (::DDS_DataReaderQos &ddsqos, const ::DDS::DataReaderQos & qos)
{
  ddsqos.durability <<= qos.durability;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.user_data <<= qos.user_data;
  ddsqos.ownership <<= qos.ownership;
  ddsqos.time_based_filter <<= qos.time_based_filter;
  ddsqos.reader_data_lifecycle <<= qos.reader_data_lifecycle;
}

inline void
operator>>= (const ::DDS_DataReaderQos &qos, ::DDS::DataReaderQos & ddsqos)
{
  ddsqos.durability <<= qos.durability;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.user_data <<= qos.user_data;
  ddsqos.ownership <<= qos.ownership;
  ddsqos.time_based_filter <<= qos.time_based_filter;
  ddsqos.reader_data_lifecycle <<= qos.reader_data_lifecycle;
}

inline void
operator>>= (const ::DDS::DataReaderQos &qos, ::DDS_DataReaderQos & ddsqos)
{
  ddsqos.durability <<= qos.durability;
  ddsqos.deadline <<= qos.deadline;
  ddsqos.latency_budget <<= qos.latency_budget;
  ddsqos.liveliness <<= qos.liveliness;
  ddsqos.reliability <<= qos.reliability;
  ddsqos.destination_order <<= qos.destination_order;
  ddsqos.history <<= qos.history;
  ddsqos.resource_limits <<= qos.resource_limits;
  ddsqos.user_data <<= qos.user_data;
  ddsqos.ownership <<= qos.ownership;
  ddsqos.time_based_filter <<= qos.time_based_filter;
  ddsqos.reader_data_lifecycle <<= qos.reader_data_lifecycle;
}
#endif

#endif /* DDS4CCM_DATAREADERQOS_H */

/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_SUBSCRIBERQOS_H
#define CIAO_RTI_SUBSCRIBERQOS_H

#include "EntityFactoryQosPolicy.h"

inline void
operator<<= (::DDS::SubscriberQos &ddsqos, const ::DDS_SubscriberQos & qos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator<<= (::DDS_SubscriberQos &ddsqos, const ::DDS::SubscriberQos & qos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS_SubscriberQos &qos, ::DDS::SubscriberQos & ddsqos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS::SubscriberQos &qos, ::DDS_SubscriberQos & ddsqos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

#endif /* CIAO_RTI_SUBSCRIBERQOS_H */

/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_SUBSCRIBERQOS_H
#define DDS4CCM_SUBSCRIBERQOS_H

#include "EntityFactoryQosPolicy.h"
#include "GroupDataQosPolicy.h"
#include "PartitionQosPolicy.h"
#include "PresentationQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_SUBSCRIBERQOS_H */

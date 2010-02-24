/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_PUBLISHERQOS_H
#define CIAO_RTI_PUBLISHERQOS_H

#include "PresentationQosPolicy.h"
#include "PartitionQosPolicy.h"
#include "GroupDataQosPolicy.h"
#include "EntityFactoryQosPolicy.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::PublisherQos DDS_PublisherQos;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::PublisherQos &ddsqos, const ::DDS_PublisherQos & qos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator<<= (::DDS_PublisherQos &ddsqos, const ::DDS::PublisherQos & qos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS_PublisherQos &qos, ::DDS::PublisherQos & ddsqos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS::PublisherQos &qos, ::DDS_PublisherQos & ddsqos)
{
  ddsqos.presentation <<= qos.presentation;
  ddsqos.partition <<= qos.partition;
  ddsqos.group_data <<= qos.group_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}
#endif

#endif /* CIAO_RTI_PUBLISHERQOS_H */

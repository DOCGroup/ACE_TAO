/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DOMAINPARTICIPANTQOSPOLICY_H
#define CIAO_RTI_ENTITYFACTORYQOSPOLICY_H

#include "EntityFactoryQosPolicy.h"

inline void
operator<<= (::DDS::DomainParticipantFactoryQos &ddsqos, const ::DDS_DomainParticipantFactoryQos & qos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator<<= (::DDS_DomainParticipantFactoryQos &ddsqos, const ::DDS::DomainParticipantFactoryQos & qos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS_DomainParticipantFactoryQos &qos, ::DDS::DomainParticipantFactoryQos & ddsqos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS::DomainParticipantFactoryQos &qos, ::DDS_DomainParticipantFactoryQos & ddsqos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

#endif /* CIAO_RTI_ENTITYFACTORYQOSPOLICY_H */

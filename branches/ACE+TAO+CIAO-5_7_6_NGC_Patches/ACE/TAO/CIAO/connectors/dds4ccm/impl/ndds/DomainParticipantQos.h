/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DOMAINPARTICIPANTQOSPOLICY_H
#define CIAO_RTI_DOMAINPARTICIPANTQOSPOLICY_H

#include "UserDataQosPolicy.h"
#include "EntityFactoryQosPolicy.h"

inline void
operator<<= (::DDS::DomainParticipantQos &ddsqos, const ::DDS_DomainParticipantQos & qos)
{
  ddsqos.user_data <<= qos.user_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator<<= (::DDS_DomainParticipantQos &ddsqos, const ::DDS::DomainParticipantQos & qos)
{
  ddsqos.user_data <<= qos.user_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS_DomainParticipantQos &qos, ::DDS::DomainParticipantQos & ddsqos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator>>= (const ::DDS::DomainParticipantQos &qos, ::DDS_DomainParticipantQos & ddsqos)
{
  ddsqos.user_data <<= qos.user_data;
  ddsqos.entity_factory <<= qos.entity_factory;
}

#endif /* CIAO_RTI_DOMAINPARTICIPANTQOSPOLICY_H */

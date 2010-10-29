/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DOMAINPARTICIPANTQOSPOLICY_H
#define DDS4CCM_DOMAINPARTICIPANTQOSPOLICY_H

#include "UserDataQosPolicy.h"
#include "EntityFactoryQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DomainParticipantQos DDS_DomainParticipantQos;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
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

#endif

#endif /* DDS4CCM_DOMAINPARTICIPANTQOSPOLICY_H */

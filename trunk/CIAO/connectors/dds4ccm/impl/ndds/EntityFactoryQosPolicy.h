/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_ENTITYFACTORYQOSPOLICY_H
#define DDS4CCM_ENTITYFACTORYQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::EntityFactoryQosPolicy DDS_EntityFactoryQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::EntityFactoryQosPolicy &ddsqos, const ::DDS_EntityFactoryQosPolicy & qos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

inline void
operator<<= (::DDS_EntityFactoryQosPolicy &ddsqos, const ::DDS::EntityFactoryQosPolicy & qos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

inline void
operator>>= (const ::DDS_EntityFactoryQosPolicy &qos, ::DDS::EntityFactoryQosPolicy & ddsqos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

inline void
operator>>= (const ::DDS::EntityFactoryQosPolicy &qos, ::DDS_EntityFactoryQosPolicy & ddsqos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}
#endif

#endif /* DDS4CCM_ENTITYFACTORYQOSPOLICY_H */
